package model2sql.sintaxis.ast;

import model2sql.errores.Posicion;

public record ClaveForanea(String modelo, String atributo, Posicion posicion) {}
