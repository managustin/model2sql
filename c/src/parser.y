%{
#include "compilacion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *mensaje);

static void traducir_error_sintactico(const char *mensaje, char *salida, size_t capacidad) {
    /* Bison produce el diagnóstico en inglés; se extraen token inesperado y esperados. */
    const char *inesperado = strstr(mensaje, "unexpected ");
    if (inesperado == NULL) {
        snprintf(salida, capacidad, "La entrada no respeta la gramática del DSL");
        return;
    }

    inesperado += strlen("unexpected ");
    const char *esperado = strstr(inesperado, ", expecting ");
    size_t longitud_token = esperado == NULL
                                ? strlen(inesperado)
                                : (size_t)(esperado - inesperado);
    if (esperado == NULL) {
        snprintf(salida, capacidad, "Token inesperado %.*s",
                 (int)longitud_token, inesperado);
        return;
    }

    esperado += strlen(", expecting ");
    char lista_esperada[512];
    size_t posicion_salida = 0;
    /* Convierte el separador inglés "or" para mostrar una lista legible en español. */
    while (*esperado != '\0' && posicion_salida + 1 < sizeof(lista_esperada)) {
        if (strncmp(esperado, " or ", 4) == 0) {
            if (posicion_salida + 3 >= sizeof(lista_esperada)) {
                break;
            }
            lista_esperada[posicion_salida++] = ' ';
            lista_esperada[posicion_salida++] = 'o';
            lista_esperada[posicion_salida++] = ' ';
            esperado += 4;
        } else {
            lista_esperada[posicion_salida++] = *esperado++;
        }
    }
    lista_esperada[posicion_salida] = '\0';
    snprintf(salida, capacidad, "Token inesperado %.*s; se esperaba %s",
             (int)longitud_token, inesperado, lista_esperada);
}
%}

%code requires {
#include "ast.h"
}

%define parse.error detailed
%locations

%union {
    /* Valores semánticos que las producciones del parser transportan entre reglas. */
    char *texto;
    TipoPrimitivo tipo;
    Modificadores modificadores;
    Atributo *atributo;
    ListaAtributos atributos;
    Modelo *modelo;
}

%token TOK_MODEL "model"
%token TOK_INT "int"
%token TOK_STRING "string"
%token TOK_FLOAT "float"
%token TOK_BOOLEAN "boolean"
%token TOK_DATE "date"
%token TOK_PK "pk"
%token TOK_UNIQUE "unique"
%token TOK_NOT_NULL "not_null"
%token TOK_AUTO_INCREMENT "auto_increment"
%token TOK_FK "fk"
%token TOK_INVALID "token inválido"
%token <texto> IDENTIFICADOR

%type <tipo> tipo
%type <modificadores> modificadores modificador
%type <atributo> atributo
%type <atributos> atributos
%type <modelo> modelo

%destructor { free($$); } <texto>
%destructor { liberar_modificadores(&$$); } <modificadores>
%destructor { free($$->nombre); liberar_modificadores(&$$->modificadores); free($$); } <atributo>
%destructor { liberar_atributos(&$$); } <atributos>
%destructor { free($$->nombre); liberar_atributos(&$$->atributos); free($$); } <modelo>

%%

/* La entrada admite cero o más modelos y los agrega a la raíz del AST. */
entrada:
    %empty
  | entrada modelo
    {
        agregar_modelo(&compilacion_actual->esquema, $2);
    }
;

modelo:
    /* Un modelo agrupa su nombre y la lista de atributos entre llaves. */
    TOK_MODEL IDENTIFICADOR '{' atributos '}'
    {
        Posicion posicion = {@1.first_line, @1.first_column};
        $$ = crear_modelo($2, posicion, $4);
    }
;

atributos:
    /* La lista puede estar vacía; el análisis semántico la rechazará después. */
    %empty
    {
        $$ = (ListaAtributos){0};
    }
  | atributos atributo
    {
        $$ = $1;
        agregar_atributo(&$$, $2);
    }
;

atributo:
    /* Cada atributo comienza con nombre y tipo, seguido de cero o más modificadores. */
    IDENTIFICADOR tipo modificadores
    {
        Posicion posicion = {@1.first_line, @1.first_column};
        $$ = crear_atributo($1, $2, posicion, $3);
    }
;

tipo:
    /* Correspondencia directa entre tokens reservados y tipos internos del AST. */
    TOK_INT       { $$ = TIPO_INT; }
  | TOK_STRING    { $$ = TIPO_STRING; }
  | TOK_FLOAT     { $$ = TIPO_FLOAT; }
  | TOK_BOOLEAN   { $$ = TIPO_BOOLEAN; }
  | TOK_DATE      { $$ = TIPO_DATE; }
;

modificadores:
    /* Se acumulan modificadores sin depender de un orden específico en el archivo. */
    %empty
    {
        $$ = modificadores_vacios();
    }
  | modificadores modificador
    {
        $$ = combinar_modificadores($1, $2);
    }
;

modificador:
    /* Las propiedades simples activan una bandera en la estructura de modificadores. */
    TOK_PK
    {
        $$ = modificadores_vacios();
        $$.clave_primaria = true;
    }
  | TOK_UNIQUE
    {
        $$ = modificadores_vacios();
        $$.unico = true;
    }
  | TOK_NOT_NULL
    {
        $$ = modificadores_vacios();
        $$.no_nulo = true;
    }
  | TOK_AUTO_INCREMENT
    {
        $$ = modificadores_vacios();
        $$.autoincremental = true;
    }
  | TOK_FK '(' IDENTIFICADOR '.' IDENTIFICADOR ')'
    {
        /* La FK conserva el nombre de tabla, columna y ubicación para validarla luego. */
        Posicion posicion = {@1.first_line, @1.first_column};
        $$ = modificadores_vacios();
        $$.clave_foranea = crear_clave_foranea($3, $5, posicion);
    }
;

%%

void yyerror(const char *mensaje) {
    /* Bison informa la ubicación actual; el mensaje se normaliza al formato del proyecto. */
    Posicion posicion = {yylloc.first_line, yylloc.first_column};
    char mensaje_espanol[768];
    traducir_error_sintactico(mensaje, mensaje_espanol, sizeof(mensaje_espanol));
    registrar_error(ERROR_SINTACTICO, posicion, true, "%s", mensaje_espanol);
}
