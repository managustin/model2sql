#include "generador.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    /* Buffer dinámico que crece conforme se agregan fragmentos formateados de SQL. */
    char *contenido;
    size_t longitud;
    size_t capacidad;
} ConstructorTexto;

/* El error de memoria se considera fatal porque no puede devolverse un SQL parcial. */
static void *reservar(size_t tamano) {
    void *memoria = malloc(tamano);
    if (memoria == NULL) {
        fputs("Error interno:\nNo hay memoria suficiente\n", stderr);
        exit(2);
    }
    return memoria;
}

static void agregar_formato(ConstructorTexto *texto, const char *formato, ...) {
    /* Primero calcula el tamaño exacto; luego amplía el buffer si hace falta. */
    va_list argumentos;
    va_start(argumentos, formato);
    va_list copia;
    va_copy(copia, argumentos);
    int cantidad = vsnprintf(NULL, 0, formato, copia);
    va_end(copia);
    if (cantidad < 0) {
        fputs("Error interno:\nNo se pudo generar el SQL\n", stderr);
        exit(2);
    }

    /* El byte adicional reserva espacio para el terminador nulo de C. */
    size_t requerido = texto->longitud + (size_t)cantidad + 1;
    if (requerido > texto->capacidad) {
        size_t nueva_capacidad = texto->capacidad == 0 ? 256 : texto->capacidad;
        while (nueva_capacidad < requerido) {
            nueva_capacidad *= 2;
        }
        char *nuevo_contenido = realloc(texto->contenido, nueva_capacidad);
        if (nuevo_contenido == NULL) {
            va_end(argumentos);
            fputs("Error interno:\nNo hay memoria suficiente\n", stderr);
            exit(2);
        }
        texto->contenido = nuevo_contenido;
        texto->capacidad = nueva_capacidad;
    }

    vsnprintf(texto->contenido + texto->longitud,
              texto->capacidad - texto->longitud, formato, argumentos);
    texto->longitud += (size_t)cantidad;
    va_end(argumentos);
}

static const char *tipo_sql(TipoPrimitivo tipo) {
    /* Traducción de los cinco tipos del DSL a los tipos compatibles con PostgreSQL. */
    switch (tipo) {
        case TIPO_INT:
            return "INTEGER";
        case TIPO_STRING:
            return "VARCHAR(255)";
        case TIPO_FLOAT:
            return "REAL";
        case TIPO_BOOLEAN:
            return "BOOLEAN";
        case TIPO_DATE:
            return "DATE";
    }
    return "INTEGER";
}

static size_t indice_modelo(const Esquema *esquema, const Modelo *buscado) {
    /* El orden del AST permite decidir si la tabla destino ya fue declarada. */
    size_t indice = 0;
    for (Modelo *modelo = esquema->primero; modelo != NULL; modelo = modelo->siguiente) {
        if (modelo == buscado) {
            return indice;
        }
        indice++;
    }
    return indice;
}

static size_t indice_modelo_por_nombre(const Esquema *esquema, const char *nombre) {
    /* Busca la posición de una tabla por nombre para compararla con la de origen. */
    size_t indice = 0;
    for (Modelo *modelo = esquema->primero; modelo != NULL; modelo = modelo->siguiente) {
        if (strcmp(modelo->nombre, nombre) == 0) {
            return indice;
        }
        indice++;
    }
    return indice;
}

static bool es_referencia_adelantada(const Esquema *esquema, const Modelo *origen,
                                     const ClaveForanea *clave) {
    /* Una tabla posterior aún no existe al ejecutar el CREATE TABLE de origen. */
    return indice_modelo_por_nombre(esquema, clave->modelo) >
           indice_modelo(esquema, origen);
}

static size_t contar_claves_en_linea(const Esquema *esquema, const Modelo *modelo) {
    /* Cuenta solo las FK que pueden incluirse ahora y sirven para separar columnas. */
    size_t cantidad = 0;
    for (Atributo *atributo = modelo->atributos.primero; atributo != NULL;
         atributo = atributo->siguiente) {
        ClaveForanea *clave = atributo->modificadores.clave_foranea;
        cantidad += clave != NULL && !es_referencia_adelantada(esquema, modelo, clave);
    }
    return cantidad;
}

static size_t contar_referencias_adelantadas(const Esquema *esquema) {
    /* Determina si hace falta una sección posterior de ALTER TABLE. */
    size_t cantidad = 0;
    for (Modelo *modelo = esquema->primero; modelo != NULL; modelo = modelo->siguiente) {
        for (Atributo *atributo = modelo->atributos.primero; atributo != NULL;
             atributo = atributo->siguiente) {
            ClaveForanea *clave = atributo->modificadores.clave_foranea;
            cantidad += clave != NULL && es_referencia_adelantada(esquema, modelo, clave);
        }
    }
    return cantidad;
}

static void emitir_modelo(ConstructorTexto *texto, const Esquema *esquema,
                          const Modelo *modelo) {
    /* Emite columnas y restricciones inline cuando sus tablas destino ya existen. */
    agregar_formato(texto, "CREATE TABLE %s (\n", modelo->nombre);
    size_t claves_foraneas = contar_claves_en_linea(esquema, modelo);
    for (Atributo *atributo = modelo->atributos.primero; atributo != NULL;
         atributo = atributo->siguiente) {
        bool hay_siguiente = atributo->siguiente != NULL || claves_foraneas > 0;
        /* En PostgreSQL, SERIAL representa el tipo int con incremento automático. */
        const char *tipo = atributo->modificadores.autoincremental
                               ? "SERIAL"
                               : tipo_sql(atributo->tipo);
        agregar_formato(texto, "    %s %s", atributo->nombre, tipo);
        if (atributo->modificadores.clave_primaria) {
            agregar_formato(texto, " PRIMARY KEY");
        }
        if (atributo->modificadores.unico) {
            agregar_formato(texto, " UNIQUE");
        }
        if (atributo->modificadores.no_nulo) {
            agregar_formato(texto, " NOT NULL");
        }
        agregar_formato(texto, "%s\n", hay_siguiente ? "," : "");
    }

    /* Las claves en línea van al final de las columnas, con comas según corresponda. */
    size_t indice_clave = 0;
    for (Atributo *atributo = modelo->atributos.primero; atributo != NULL;
         atributo = atributo->siguiente) {
        ClaveForanea *clave = atributo->modificadores.clave_foranea;
        if (clave == NULL || es_referencia_adelantada(esquema, modelo, clave)) {
            continue;
        }
        indice_clave++;
        agregar_formato(texto, "    FOREIGN KEY (%s) REFERENCES %s(%s)%s\n",
                        atributo->nombre, clave->modelo, clave->atributo,
                        indice_clave < claves_foraneas ? "," : "");
    }
    agregar_formato(texto, ");");
}

char *generar_sql(const Esquema *esquema) {
    /* Primera etapa: crear tablas en el orden de entrada. */
    ConstructorTexto texto = {0};
    for (Modelo *modelo = esquema->primero; modelo != NULL; modelo = modelo->siguiente) {
        if (texto.longitud > 0) {
            agregar_formato(&texto, "\n\n");
        }
        emitir_modelo(&texto, esquema, modelo);
    }

    /* Segunda etapa: agregar FK adelantadas cuando ya se crearon todas las tablas. */
    size_t cantidad_adelantadas = contar_referencias_adelantadas(esquema);
    if (cantidad_adelantadas > 0) {
        agregar_formato(&texto, "\n\n");
        for (Modelo *modelo = esquema->primero; modelo != NULL; modelo = modelo->siguiente) {
            for (Atributo *atributo = modelo->atributos.primero; atributo != NULL;
                 atributo = atributo->siguiente) {
                ClaveForanea *clave = atributo->modificadores.clave_foranea;
                if (clave == NULL || !es_referencia_adelantada(esquema, modelo, clave)) {
                    continue;
                }
                agregar_formato(&texto, "ALTER TABLE %s ADD FOREIGN KEY (%s) "
                                       "REFERENCES %s(%s);\n",
                                modelo->nombre, atributo->nombre, clave->modelo,
                                clave->atributo);
            }
        }
    }
    /* Devuelve siempre una cadena válida, incluso para un esquema vacío. */
    if (texto.contenido == NULL) {
        texto.contenido = reservar(1);
        texto.contenido[0] = '\0';
    } else if (texto.longitud > 0 && texto.contenido[texto.longitud - 1] == '\n') {
        texto.contenido[--texto.longitud] = '\0';
    }
    return texto.contenido;
}
