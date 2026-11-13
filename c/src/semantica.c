#include "semantica.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Cada entrada apunta al modelo del AST y guarda sus atributos para búsquedas rápidas. */
typedef struct {
    Modelo *modelo;
    Atributo **atributos;
    size_t cantidad_atributos;
} SimboloModelo;

/* Tabla temporal que permite resolver referencias aunque el destino aparezca después. */
typedef struct {
    SimboloModelo *modelos;
    size_t cantidad_modelos;
} TablaSimbolos;

/* Reporta errores de recursos antes de continuar con una tabla incompleta. */
static void *reservar(size_t tamano) {
    void *memoria = malloc(tamano);
    if (memoria == NULL) {
        fputs("Error interno:\nNo hay memoria suficiente\n", stderr);
        exit(2);
    }
    return memoria;
}

static SimboloModelo *buscar_modelo(TablaSimbolos *tabla, const char *nombre) {
    /* Búsqueda lineal adecuada para el tamaño acotado de los modelos de entrada. */
    for (size_t indice = 0; indice < tabla->cantidad_modelos; indice++) {
        if (strcmp(tabla->modelos[indice].modelo->nombre, nombre) == 0) {
            return &tabla->modelos[indice];
        }
    }
    return NULL;
}

static Atributo *buscar_atributo(SimboloModelo *modelo, const char *nombre) {
    /* Recorre los atributos registrados de un modelo por nombre. */
    for (size_t indice = 0; indice < modelo->cantidad_atributos; indice++) {
        if (strcmp(modelo->atributos[indice]->nombre, nombre) == 0) {
            return modelo->atributos[indice];
        }
    }
    return NULL;
}

static void liberar_tabla(TablaSimbolos *tabla) {
    /* La tabla posee los arreglos auxiliares, no los nodos originales del AST. */
    for (size_t indice = 0; indice < tabla->cantidad_modelos; indice++) {
        free(tabla->modelos[indice].atributos);
    }
    free(tabla->modelos);
    *tabla = (TablaSimbolos){0};
}

static size_t contar_atributos(const ListaAtributos *atributos) {
    /* Cuenta nodos para reservar exactamente el arreglo de símbolos. */
    size_t cantidad = 0;
    for (Atributo *atributo = atributos->primero; atributo != NULL;
         atributo = atributo->siguiente) {
        cantidad++;
    }
    return cantidad;
}

static bool registrar_modelos(const Esquema *esquema, TablaSimbolos *tabla) {
    /* Primera pasada: registrar destinos posibles y validar restricciones locales. */
    size_t cantidad = 0;
    for (Modelo *modelo = esquema->primero; modelo != NULL; modelo = modelo->siguiente) {
        cantidad++;
    }
    tabla->modelos = cantidad == 0 ? NULL : reservar(cantidad * sizeof(*tabla->modelos));

    for (Modelo *modelo = esquema->primero; modelo != NULL; modelo = modelo->siguiente) {
        /* Al completar cada entrada se puede detectar duplicación de tablas. */
        if (buscar_modelo(tabla, modelo->nombre) != NULL) {
            Posicion posicion = modelo->posicion;
            registrar_error(ERROR_SEMANTICO, posicion, true,
                            "El modelo '%s' ya fue declarado", modelo->nombre);
            return false;
        }

        SimboloModelo *simbolo = &tabla->modelos[tabla->cantidad_modelos++];
        simbolo->modelo = modelo;
        simbolo->cantidad_atributos = 0;
        size_t total_atributos = contar_atributos(&modelo->atributos);
        simbolo->atributos = total_atributos == 0
                                 ? NULL
                                 : reservar(total_atributos * sizeof(*simbolo->atributos));

        /* La tabla de atributos permite detectar duplicados y luego resolver las FK. */
        size_t cantidad_claves_primarias = 0;
        for (Atributo *atributo = modelo->atributos.primero; atributo != NULL;
             atributo = atributo->siguiente) {
            if (buscar_atributo(simbolo, atributo->nombre) != NULL) {
                registrar_error(ERROR_SEMANTICO, atributo->posicion, true,
                                "El atributo '%s' ya fue declarado en '%s'",
                                atributo->nombre, modelo->nombre);
                return false;
            }
            simbolo->atributos[simbolo->cantidad_atributos++] = atributo;

            if (atributo->modificadores.clave_primaria &&
                ++cantidad_claves_primarias > 1) {
                registrar_error(ERROR_SEMANTICO, atributo->posicion, true,
                                "El modelo '%s' declara más de una clave primaria",
                                modelo->nombre);
                return false;
            }
            if (atributo->modificadores.clave_foranea_duplicada) {
                registrar_error(ERROR_SEMANTICO, atributo->posicion, true,
                                "El atributo '%s' declara más de una clave foránea",
                                atributo->nombre);
                return false;
            }
            if (atributo->modificadores.autoincremental && atributo->tipo != TIPO_INT) {
                registrar_error(ERROR_SEMANTICO, atributo->posicion, true,
                                "El atributo '%s' con auto_increment debe ser de tipo int",
                                atributo->nombre);
                return false;
            }
        }
        if (simbolo->cantidad_atributos == 0) {
            registrar_error(ERROR_SEMANTICO, modelo->posicion, true,
                            "El modelo '%s' no tiene atributos", modelo->nombre);
            return false;
        }
    }
    return true;
}

static bool validar_claves_foraneas(TablaSimbolos *tabla) {
    /* Segunda pasada: todas las tablas ya están registradas, incluso las adelantadas. */
    for (size_t indice_modelo = 0; indice_modelo < tabla->cantidad_modelos; indice_modelo++) {
        SimboloModelo *origen = &tabla->modelos[indice_modelo];
        for (size_t indice_atributo = 0;
             indice_atributo < origen->cantidad_atributos;
             indice_atributo++) {
            Atributo *atributo = origen->atributos[indice_atributo];
            ClaveForanea *clave = atributo->modificadores.clave_foranea;
            if (clave == NULL) {
                continue;
            }

            /* Una FK solo es válida si existen la tabla y la columna de destino. */
            SimboloModelo *destino = buscar_modelo(tabla, clave->modelo);
            if (destino == NULL) {
                registrar_error(ERROR_SEMANTICO, clave->posicion, true,
                                "La tabla '%s' referenciada en la FK no existe",
                                clave->modelo);
                return false;
            }

            Atributo *referenciado = buscar_atributo(destino, clave->atributo);
            if (referenciado == NULL) {
                registrar_error(ERROR_SEMANTICO, clave->posicion, true,
                                "El atributo '%s' referenciado en la FK no existe en '%s'",
                                clave->atributo, clave->modelo);
                return false;
            }
            /* SQL requiere que la columna referenciada identifique filas de forma única. */
            if (!referenciado->modificadores.clave_primaria &&
                !referenciado->modificadores.unico) {
                registrar_error(ERROR_SEMANTICO, clave->posicion, true,
                                "El atributo '%s' de '%s' debe ser pk o unique para usarse en una FK",
                                clave->atributo, clave->modelo);
                return false;
            }
            /* Ambos extremos deben tener el mismo tipo primitivo en este DSL. */
            if (atributo->tipo != referenciado->tipo) {
                registrar_error(ERROR_SEMANTICO, clave->posicion, true,
                                "Tipo incompatible en FK: '%s' (%s) no coincide con '%s.%s' (%s)",
                                atributo->nombre, nombre_tipo(atributo->tipo), clave->modelo,
                                clave->atributo, nombre_tipo(referenciado->tipo));
                return false;
            }
        }
    }
    return true;
}

bool analizar_semantica(const Esquema *esquema) {
    /* La tabla vive solo durante el análisis; el AST se conserva para generar SQL. */
    TablaSimbolos tabla = {0};
    bool valido = registrar_modelos(esquema, &tabla);
    if (valido) {
        valido = validar_claves_foraneas(&tabla);
    }
    liberar_tabla(&tabla);
    return valido;
}
