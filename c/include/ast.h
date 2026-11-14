#ifndef MODEL2SQL_AST_H
#define MODEL2SQL_AST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int linea;
    int columna;
} Posicion;

typedef enum {
    TIPO_INT,
    TIPO_STRING,
    TIPO_FLOAT,
    TIPO_BOOLEAN,
    TIPO_DATE
} TipoPrimitivo;

typedef struct {
    char *modelo;
    char *atributo;
    Posicion posicion;
} ClaveForanea;

typedef struct {
    bool clave_primaria;
    bool unico;
    bool no_nulo;
    bool autoincremental;
    bool clave_foranea_duplicada;
    ClaveForanea *clave_foranea;
} Modificadores;

typedef struct Atributo {
    char *nombre;
    TipoPrimitivo tipo;
    Posicion posicion;
    Modificadores modificadores;
    struct Atributo *siguiente;
} Atributo;

typedef struct {
    Atributo *primero;
    Atributo *ultimo;
} ListaAtributos;

typedef struct Modelo {
    char *nombre;
    Posicion posicion;
    ListaAtributos atributos;
    struct Modelo *siguiente;
} Modelo;

typedef struct {
    Modelo *primero;
    Modelo *ultimo;
} Esquema;

Modificadores modificadores_vacios(void);
Modificadores combinar_modificadores(Modificadores izquierdo, Modificadores derecho);
ClaveForanea *crear_clave_foranea(char *modelo, char *atributo, Posicion posicion);
Atributo *crear_atributo(char *nombre, TipoPrimitivo tipo, Posicion posicion,
                         Modificadores modificadores);
void agregar_atributo(ListaAtributos *lista, Atributo *atributo);
Modelo *crear_modelo(char *nombre, Posicion posicion, ListaAtributos atributos);
void agregar_modelo(Esquema *esquema, Modelo *modelo);
void liberar_modificadores(Modificadores *modificadores);
void liberar_atributos(ListaAtributos *atributos);
void liberar_modelo(Modelo *modelo);
void liberar_esquema(Esquema *esquema);
const char *nombre_tipo(TipoPrimitivo tipo);

#endif
