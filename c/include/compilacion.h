#ifndef MODEL2SQL_COMPILACION_H
#define MODEL2SQL_COMPILACION_H

#include "ast.h"

#include <stdbool.h>
#include <stddef.h>

/* Etapa que detectó el primer problema del proceso de compilación. */
typedef enum {
    ERROR_NINGUNO,
    ERROR_LEXICO,
    ERROR_SINTACTICO,
    ERROR_SEMANTICO
} TipoError;

/* Diagnóstico único por compilación, con posición opcional y mensaje formateado. */
typedef struct {
    TipoError tipo;
    bool tiene_posicion;
    Posicion posicion;
    char mensaje[1024];
} ErrorCompilacion;

/* Estado compartido por Flex, Bison y las etapas posteriores durante una entrada. */
typedef struct {
    Esquema esquema;
    ErrorCompilacion error;
} EstadoCompilacion;

extern EstadoCompilacion *compilacion_actual;

/* Registra solo el primer error, para conservar el diagnóstico más útil. */
void registrar_error(TipoError tipo, Posicion posicion, bool tiene_posicion,
                     const char *formato, ...);
/* Presenta el diagnóstico con el nombre de la etapa y su ubicación, si existe. */
void imprimir_error(const ErrorCompilacion *error);

#endif
