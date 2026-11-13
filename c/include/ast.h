#ifndef MODEL2SQL_AST_H
#define MODEL2SQL_AST_H

#include <stdbool.h>
#include <stddef.h>

/* Ubicación de una construcción en el archivo .model, usada en los diagnósticos. */
typedef struct {
    int linea;
    int columna;
} Posicion;

/* Tipos del DSL; el generador los traduce a tipos equivalentes de SQL. */
typedef enum {
    TIPO_INT,
    TIPO_STRING,
    TIPO_FLOAT,
    TIPO_BOOLEAN,
    TIPO_DATE
} TipoPrimitivo;

/* Destino de una referencia fk(Modelo.atributo), más su ubicación de origen. */
typedef struct {
    char *modelo;
    char *atributo;
    Posicion posicion;
} ClaveForanea;

/* Propiedades opcionales que pueden aparecer después del tipo de un atributo. */
typedef struct {
    bool clave_primaria;
    bool unico;
    bool no_nulo;
    bool autoincremental;
    bool clave_foranea_duplicada;
    ClaveForanea *clave_foranea;
} Modificadores;

/* Nodo del AST para una columna; los atributos de un modelo forman una lista enlazada. */
typedef struct Atributo {
    char *nombre;
    TipoPrimitivo tipo;
    Posicion posicion;
    Modificadores modificadores;
    struct Atributo *siguiente;
} Atributo;

/* Extremos de la lista para agregar atributos en tiempo constante y mantener el orden. */
typedef struct {
    Atributo *primero;
    Atributo *ultimo;
} ListaAtributos;

/* Nodo del AST para una tabla del modelo de entrada. */
typedef struct Modelo {
    char *nombre;
    Posicion posicion;
    ListaAtributos atributos;
    struct Modelo *siguiente;
} Modelo;

/* Raíz del AST: contiene los modelos en el mismo orden en que fueron declarados. */
typedef struct {
    Modelo *primero;
    Modelo *ultimo;
} Esquema;

/* Constructores y operaciones de listas que mantienen las invariantes del AST. */
Modificadores modificadores_vacios(void);
Modificadores combinar_modificadores(Modificadores izquierdo, Modificadores derecho);
ClaveForanea *crear_clave_foranea(char *modelo, char *atributo, Posicion posicion);
Atributo *crear_atributo(char *nombre, TipoPrimitivo tipo, Posicion posicion,
                         Modificadores modificadores);
void agregar_atributo(ListaAtributos *lista, Atributo *atributo);
Modelo *crear_modelo(char *nombre, Posicion posicion, ListaAtributos atributos);
void agregar_modelo(Esquema *esquema, Modelo *modelo);
/* Liberan recursivamente la memoria propiedad del AST y reinician las estructuras. */
void liberar_modificadores(Modificadores *modificadores);
void liberar_atributos(ListaAtributos *atributos);
void liberar_modelo(Modelo *modelo);
void liberar_esquema(Esquema *esquema);
const char *nombre_tipo(TipoPrimitivo tipo);

#endif
