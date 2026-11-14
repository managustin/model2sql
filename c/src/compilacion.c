#include "compilacion.h"

#include <stdarg.h>
#include <stdio.h>

EstadoCompilacion *compilacion_actual;

void registrar_error(TipoError tipo, Posicion posicion, bool tiene_posicion,
                     const char *formato, ...) {
    if (compilacion_actual == NULL || compilacion_actual->error.tipo != ERROR_NINGUNO) {
        return;
    }

    ErrorCompilacion *error = &compilacion_actual->error;
    error->tipo = tipo;
    error->tiene_posicion = tiene_posicion;
    error->posicion = posicion;

    va_list argumentos;
    va_start(argumentos, formato);
    vsnprintf(error->mensaje, sizeof(error->mensaje), formato, argumentos);
    va_end(argumentos);
}

void imprimir_error(const ErrorCompilacion *error) {
    const char *tipo;
    switch (error->tipo) {
        case ERROR_LEXICO:
            tipo = "Error léxico";
            break;
        case ERROR_SINTACTICO:
            tipo = "Error sintáctico";
            break;
        case ERROR_SEMANTICO:
            tipo = "Error semántico";
            break;
        case ERROR_NINGUNO:
        default:
            tipo = "Error";
            break;
    }

    if (error->tiene_posicion) {
        fprintf(stderr, "%s en línea %d, columna %d:\n%s\n", tipo,
                error->posicion.linea, error->posicion.columna, error->mensaje);
    } else {
        fprintf(stderr, "%s:\n%s\n", tipo, error->mensaje);
    }
}
