#ifndef MODEL2SQL_COMPILACION_H
#define MODEL2SQL_COMPILACION_H

#include "ast.h"

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    ERROR_NINGUNO,
    ERROR_LEXICO,
    ERROR_SINTACTICO,
    ERROR_SEMANTICO
} TipoError;

typedef struct {
    TipoError tipo;
    bool tiene_posicion;
    Posicion posicion;
    char mensaje[1024];
} ErrorCompilacion;

typedef struct {
    Esquema esquema;
    ErrorCompilacion error;
} EstadoCompilacion;

extern EstadoCompilacion *compilacion_actual;

void registrar_error(TipoError tipo, Posicion posicion, bool tiene_posicion,
                     const char *formato, ...);
void imprimir_error(const ErrorCompilacion *error);

#endif
