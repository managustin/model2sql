package model2sql;

import model2sql.generacion.GeneradorSql;
import model2sql.lexico.AnalizadorLexico;
import model2sql.semantica.AnalizadorSemantico;
import model2sql.sintaxis.AnalizadorSintactico;
import model2sql.sintaxis.ast.Esquema;

/** Encadena las etapas. Único punto de entrada de la lógica. */
public final class Transpilador {

    private Transpilador() {}

    /** Convierte código .model en SQL. Lanza ErrorCompilacion. */
    public static String transpilar(String fuente) {
        Esquema esquema = AnalizadorSintactico.analizar(AnalizadorLexico.tokenizar(fuente));
        AnalizadorSemantico.analizar(esquema);
        return GeneradorSql.generar(esquema);
    }
}
