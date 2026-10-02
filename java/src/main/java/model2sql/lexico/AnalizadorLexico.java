package model2sql.lexico;

import java.util.List;

/** Análisis léxico: texto fuente -> tokens. */
public final class AnalizadorLexico {

    private AnalizadorLexico() {}

    /**
     * Devuelve los tokens de {@code fuente}, terminando en FIN. Lanza ErrorLexico.
     * Descarta espacios y comentarios {@code //} hasta fin de línea.
     */
    public static List<Token> tokenizar(String fuente) {
        throw new UnsupportedOperationException("Análisis léxico pendiente");
    }
}
