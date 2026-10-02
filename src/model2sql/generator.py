"""Generación de SQL DDL (dialecto PostgreSQL) a partir de un AST validado."""

from model2sql.nodes import PrimitiveType, Schema

SQL_TYPES: dict[PrimitiveType, str] = {
    PrimitiveType.INT: "INTEGER",
    PrimitiveType.STRING: "VARCHAR(255)",
    PrimitiveType.FLOAT: "REAL",
    PrimitiveType.BOOLEAN: "BOOLEAN",
    PrimitiveType.DATE: "DATE",
}


def generate(schema: Schema) -> str:
    """Devuelve un CREATE TABLE por modelo, separados por una línea en blanco."""
    raise NotImplementedError("Generador pendiente")
