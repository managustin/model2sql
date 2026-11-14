#include "compilacion.h"
#include "generador.h"
#include "semantica.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
void yyrestart(FILE *entrada);
int yyparse(void);
int yylex_destroy(void);

enum {
    SALIDA_OK = 0,
    SALIDA_ERROR_COMPILACION = 1,
    SALIDA_ERROR_ARCHIVO = 2,
    SALIDA_USO_INCORRECTO = 2
};

static void mostrar_uso(FILE *salida) {
    fputs("Uso: model2sql archivo.model [-o archivo.sql]\n", salida);
}

static bool escribir_archivo(const char *ruta, const char *contenido) {
    FILE *archivo = fopen(ruta, "w");
    if (archivo == NULL) {
        fprintf(stderr, "No se pudo escribir '%s': %s\n", ruta, strerror(errno));
        return false;
    }

    size_t longitud = strlen(contenido);
    bool correcto = fwrite(contenido, 1, longitud, archivo) == longitud &&
                    fputc('\n', archivo) != EOF;
    if (fclose(archivo) != 0) {
        correcto = false;
    }
    if (!correcto) {
        fprintf(stderr, "No se pudo completar la escritura de '%s'\n", ruta);
    }
    return correcto;
}

int main(int cantidad_argumentos, char **argumentos) {
    const char *ruta_entrada = NULL;
    const char *ruta_salida = NULL;
    for (int indice = 1; indice < cantidad_argumentos; indice++) {
        if (strcmp(argumentos[indice], "--help") == 0 ||
            strcmp(argumentos[indice], "-h") == 0) {
            mostrar_uso(stdout);
            return SALIDA_OK;
        }
        if (strcmp(argumentos[indice], "-o") == 0 ||
            strcmp(argumentos[indice], "--salida") == 0) {
            if (ruta_salida != NULL || indice + 1 >= cantidad_argumentos) {
                mostrar_uso(stderr);
                return SALIDA_USO_INCORRECTO;
            }
            ruta_salida = argumentos[++indice];
            continue;
        }
        if (argumentos[indice][0] == '-' || ruta_entrada != NULL) {
            mostrar_uso(stderr);
            return SALIDA_USO_INCORRECTO;
        }
        ruta_entrada = argumentos[indice];
    }
    if (ruta_entrada == NULL) {
        mostrar_uso(stderr);
        return SALIDA_USO_INCORRECTO;
    }

    FILE *entrada = fopen(ruta_entrada, "r");
    if (entrada == NULL) {
        fprintf(stderr, "No se pudo leer '%s': %s\n", ruta_entrada, strerror(errno));
        return SALIDA_ERROR_ARCHIVO;
    }

    EstadoCompilacion estado = {0};
    compilacion_actual = &estado;
    yyin = entrada;
    yyrestart(entrada);
    int resultado_parser = yyparse();
    int error_lectura = ferror(entrada);
    if (fclose(entrada) != 0) {
        error_lectura = 1;
    }
    yylex_destroy();

    if (error_lectura) {
        fprintf(stderr, "No se pudo leer '%s'\n", ruta_entrada);
        liberar_esquema(&estado.esquema);
        return SALIDA_ERROR_ARCHIVO;
    }
    if (resultado_parser != 0 || estado.error.tipo != ERROR_NINGUNO) {
        if (estado.error.tipo == ERROR_NINGUNO) {
            Posicion posicion = {1, 1};
            registrar_error(ERROR_SINTACTICO, posicion, true,
                            "No se pudo analizar la entrada");
        }
        imprimir_error(&estado.error);
        liberar_esquema(&estado.esquema);
        return SALIDA_ERROR_COMPILACION;
    }
    if (!analizar_semantica(&estado.esquema)) {
        imprimir_error(&estado.error);
        liberar_esquema(&estado.esquema);
        return SALIDA_ERROR_COMPILACION;
    }

    char *sql = generar_sql(&estado.esquema);
    bool correcto = ruta_salida == NULL
                        ? (fputs(sql, stdout) != EOF && fputc('\n', stdout) != EOF)
                        : escribir_archivo(ruta_salida, sql);
    free(sql);
    liberar_esquema(&estado.esquema);
    if (!correcto) {
        return SALIDA_ERROR_ARCHIVO;
    }
    return SALIDA_OK;
}
