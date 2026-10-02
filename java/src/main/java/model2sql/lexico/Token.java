package model2sql.lexico;

import model2sql.errores.Posicion;

public record Token(TipoToken tipo, String lexema, Posicion posicion) {}
