package model2sql.sintaxis;

import java.util.List;
import model2sql.lexico.Token;
import model2sql.sintaxis.ast.Esquema;

/**
 * Análisis sintáctico descendente recursivo: tokens -> AST.
 * Cada regla de docs/gramatica.md es un método {@code analizar<Regla>}.
 */
public final class AnalizadorSintactico {

    private AnalizadorSintactico() {}

    /** Construye el AST. Lanza ErrorSintactico ante el primer token inesperado. */
    public static Esquema analizar(List<Token> tokens) {
        throw new UnsupportedOperationException("Análisis sintáctico pendiente");
    }
}
