#include "semantica.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    Modelo *modelo;
    Atributo **atributos;
    size_t cantidad_atributos;
} SimboloModelo;

typedef struct {
    SimboloModelo *modelos;
    size_t cantidad_modelos;
} TablaSimbolos;

static void *reservar(size_t tamano) {
    void *memoria = malloc(tamano);
    if (memoria == NULL) {
        fputs("Error interno:\nNo hay memoria suficiente\n", stderr);
        exit(2);
    }
    return memoria;
}

static SimboloModelo *buscar_modelo(TablaSimbolos *tabla, const char *nombre) {
    for (size_t indice = 0; indice < tabla->cantidad_modelos; indice++) {
        if (strcmp(tabla->modelos[indice].modelo->nombre, nombre) == 0) {
            return &tabla->modelos[indice];
        }
    }
    return NULL;
}

static Atributo *buscar_atributo(SimboloModelo *modelo, const char *nombre) {
    for (size_t indice = 0; indice < modelo->cantidad_atributos; indice++) {
        if (strcmp(modelo->atributos[indice]->nombre, nombre) == 0) {
            return modelo->atributos[indice];
        }
    }
    return NULL;
}

static void liberar_tabla(TablaSimbolos *tabla) {
    for (size_t indice = 0; indice < tabla->cantidad_modelos; indice++) {
        free(tabla->modelos[indice].atributos);
    }
    free(tabla->modelos);
    *tabla = (TablaSimbolos){0};
}

static size_t contar_atributos(const ListaAtributos *atributos) {
    size_t cantidad = 0;
    for (Atributo *atributo = atributos->primero; atributo != NULL;
         atributo = atributo->siguiente) {
        cantidad++;
    }
    return cantidad;
}

static bool registrar_modelos(const Esquema *esquema, TablaSimbolos *tabla) {
    size_t cantidad = 0;
    for (Modelo *modelo = esquema->primero; modelo != NULL; modelo = modelo->siguiente) {
        cantidad++;
    }
    tabla->modelos = cantidad == 0 ? NULL : reservar(cantidad * sizeof(*tabla->modelos));

    for (Modelo *modelo = esquema->primero; modelo != NULL; modelo = modelo->siguiente) {
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
            if (!referenciado->modificadores.clave_primaria &&
                !referenciado->modificadores.unico) {
                registrar_error(ERROR_SEMANTICO, clave->posicion, true,
                                "El atributo '%s' de '%s' debe ser pk o unique para usarse en una FK",
                                clave->atributo, clave->modelo);
                return false;
            }
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
    TablaSimbolos tabla = {0};
    bool valido = registrar_modelos(esquema, &tabla);
    if (valido) {
        valido = validar_claves_foraneas(&tabla);
    }
    liberar_tabla(&tabla);
    return valido;
}
