package model2sql.generacion;

import java.util.Map;
import model2sql.sintaxis.ast.Esquema;
import model2sql.sintaxis.ast.TipoPrimitivo;

/** Generación de SQL DDL (PostgreSQL) a partir de un AST validado. */
public final class GeneradorSql {

    static final Map<TipoPrimitivo, String> TIPOS_SQL = Map.of(
            TipoPrimitivo.INT, "INTEGER",
            TipoPrimitivo.STRING, "VARCHAR(255)",
            TipoPrimitivo.FLOAT, "REAL",
            TipoPrimitivo.BOOLEAN, "BOOLEAN",
            TipoPrimitivo.DATE, "DATE");

    private GeneradorSql() {}

    /** Un CREATE TABLE por modelo, separados por una línea en blanco. */
    public static String generar(Esquema esquema) {
        throw new UnsupportedOperationException("Generación pendiente");
    }
}
