"""Generación de SQL DDL (PostgreSQL) a partir de un AST validado."""

from model2sql.nodos import Esquema, TipoPrimitivo

TIPOS_SQL: dict[TipoPrimitivo, str] = {
    TipoPrimitivo.INT: "INTEGER",
    TipoPrimitivo.STRING: "VARCHAR(255)",
    TipoPrimitivo.FLOAT: "REAL",
    TipoPrimitivo.BOOLEAN: "BOOLEAN",
    TipoPrimitivo.DATE: "DATE",
}


def generar_sql(esquema: Esquema) -> str:
    """Un CREATE TABLE por modelo, separados por una línea en blanco."""
    raise NotImplementedError("Generación pendiente")
