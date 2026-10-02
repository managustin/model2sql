package model2sql.semantica;

import model2sql.sintaxis.ast.Esquema;

/**
 * Análisis semántico en dos pasadas.
 * Pasada 1: registra modelos y atributos. Pasada 2: valida claves y referencias.
 */
public final class AnalizadorSemantico {

    private AnalizadorSemantico() {}

    /** Valida el esquema y devuelve la tabla de símbolos. Lanza ErrorSemantico. */
    public static TablaSimbolos analizar(Esquema esquema) {
        throw new UnsupportedOperationException("Análisis semántico pendiente");
    }
}
