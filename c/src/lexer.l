%option noyywrap nounput noinput never-interactive

%{
#include "compilacion.h"
#include "parser.tab.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static int linea_actual = 1;
static int columna_actual = 1;

/* Registra la ubicación inicial y final del token para errores posteriores del parser. */
static void marcar_posicion(const char *texto, int longitud, YYLTYPE *ubicacion) {
    ubicacion->first_line = linea_actual;
    ubicacion->first_column = columna_actual;
    for (int indice = 0; indice < longitud; indice++) {
        ubicacion->last_line = linea_actual;
        ubicacion->last_column = columna_actual;
        if (texto[indice] == '\n') {
            linea_actual++;
            columna_actual = 1;
        } else {
            columna_actual++;
        }
    }
}

#define YY_USER_ACTION marcar_posicion(yytext, yyleng, &yylloc);
%}

IDENTIFICADOR [A-Za-z_][A-Za-z0-9_]*

%%

    /* Las palabras reservadas se evalúan antes que la regla general de identificadores. */
"model"             return TOK_MODEL;
"int"               return TOK_INT;
"string"            return TOK_STRING;
"float"             return TOK_FLOAT;
"boolean"           return TOK_BOOLEAN;
"date"              return TOK_DATE;
"pk"                return TOK_PK;
"unique"            return TOK_UNIQUE;
"not_null"          return TOK_NOT_NULL;
"auto_increment"    return TOK_AUTO_INCREMENT;
"fk"                return TOK_FK;

{IDENTIFICADOR} {
    /* Bison recibe una copia propia del lexema para incorporarlo al AST. */
    yylval.texto = malloc((size_t)yyleng + 1);
    if (yylval.texto == NULL) {
        fputs("Error interno:\nNo hay memoria suficiente\n", stderr);
        exit(2);
    }
    snprintf(yylval.texto, (size_t)yyleng + 1, "%s", yytext);
    return IDENTIFICADOR;
}

"{"             return '{';
"}"             return '}';
"("             return '(';
")"             return ')';
"."             return '.';

"//"[^\r\n]*    ;
[ \t\f\r\n]+    ;

    /* Cualquier carácter no reconocido detiene el análisis con un error léxico ubicado. */
. {
    Posicion posicion = {yylloc.first_line, yylloc.first_column};
    unsigned char caracter = (unsigned char)yytext[0];
    if (isprint(caracter)) {
        registrar_error(ERROR_LEXICO, posicion, true,
                        "Carácter inesperado '%c'", caracter);
    } else {
        registrar_error(ERROR_LEXICO, posicion, true,
                        "Carácter inesperado '\\x%02X'", caracter);
    }
    return TOK_INVALID;
}

%%
