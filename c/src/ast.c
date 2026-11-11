#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

static void *reservar(size_t tamano) {
    void *memoria = malloc(tamano);
    if (memoria == NULL) {
        fputs("Error interno:\nNo hay memoria suficiente\n", stderr);
        exit(2);
    }
    return memoria;
}

Modificadores modificadores_vacios(void) {
    return (Modificadores){0};
}

Modificadores combinar_modificadores(Modificadores izquierdo, Modificadores derecho) {
    izquierdo.clave_primaria |= derecho.clave_primaria;
    izquierdo.unico |= derecho.unico;
    izquierdo.no_nulo |= derecho.no_nulo;
    izquierdo.autoincremental |= derecho.autoincremental;
    izquierdo.clave_foranea_duplicada |= derecho.clave_foranea_duplicada;

    if (derecho.clave_foranea != NULL) {
        if (izquierdo.clave_foranea == NULL) {
            izquierdo.clave_foranea = derecho.clave_foranea;
        } else {
            izquierdo.clave_foranea_duplicada = true;
            free(derecho.clave_foranea->modelo);
            free(derecho.clave_foranea->atributo);
            free(derecho.clave_foranea);
        }
    }
    return izquierdo;
}

ClaveForanea *crear_clave_foranea(char *modelo, char *atributo, Posicion posicion) {
    ClaveForanea *clave = reservar(sizeof(*clave));
    clave->modelo = modelo;
    clave->atributo = atributo;
    clave->posicion = posicion;
    return clave;
}

Atributo *crear_atributo(char *nombre, TipoPrimitivo tipo, Posicion posicion,
                         Modificadores modificadores) {
    Atributo *atributo = reservar(sizeof(*atributo));
    atributo->nombre = nombre;
    atributo->tipo = tipo;
    atributo->posicion = posicion;
    atributo->modificadores = modificadores;
    atributo->siguiente = NULL;
    return atributo;
}

void agregar_atributo(ListaAtributos *lista, Atributo *atributo) {
    if (lista->ultimo == NULL) {
        lista->primero = atributo;
    } else {
        lista->ultimo->siguiente = atributo;
    }
    lista->ultimo = atributo;
}

Modelo *crear_modelo(char *nombre, Posicion posicion, ListaAtributos atributos) {
    Modelo *modelo = reservar(sizeof(*modelo));
    modelo->nombre = nombre;
    modelo->posicion = posicion;
    modelo->atributos = atributos;
    modelo->siguiente = NULL;
    return modelo;
}

void agregar_modelo(Esquema *esquema, Modelo *modelo) {
    if (esquema->ultimo == NULL) {
        esquema->primero = modelo;
    } else {
        esquema->ultimo->siguiente = modelo;
    }
    esquema->ultimo = modelo;
}

void liberar_modificadores(Modificadores *modificadores) {
    if (modificadores->clave_foranea != NULL) {
        free(modificadores->clave_foranea->modelo);
        free(modificadores->clave_foranea->atributo);
        free(modificadores->clave_foranea);
    }
    *modificadores = modificadores_vacios();
}

void liberar_atributos(ListaAtributos *atributos) {
    Atributo *actual = atributos->primero;
    while (actual != NULL) {
        Atributo *siguiente = actual->siguiente;
        free(actual->nombre);
        liberar_modificadores(&actual->modificadores);
        free(actual);
        actual = siguiente;
    }
    *atributos = (ListaAtributos){0};
}

void liberar_modelo(Modelo *modelo) {
    while (modelo != NULL) {
        Modelo *siguiente = modelo->siguiente;
        free(modelo->nombre);
        liberar_atributos(&modelo->atributos);
        free(modelo);
        modelo = siguiente;
    }
}

void liberar_esquema(Esquema *esquema) {
    liberar_modelo(esquema->primero);
    *esquema = (Esquema){0};
}

const char *nombre_tipo(TipoPrimitivo tipo) {
    switch (tipo) {
        case TIPO_INT:
            return "int";
        case TIPO_STRING:
            return "string";
        case TIPO_FLOAT:
            return "float";
        case TIPO_BOOLEAN:
            return "boolean";
        case TIPO_DATE:
            return "date";
    }
    return "desconocido";
}
