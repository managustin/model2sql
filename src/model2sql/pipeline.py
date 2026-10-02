"""Orquesta las etapas del transpilador. Único punto de entrada de la lógica."""

from model2sql.generator import generate
from model2sql.lexer import tokenize
from model2sql.parser import parse
from model2sql.semantic import analyze


def transpile(source: str) -> str:
    """Convierte código .model en SQL. Lanza Model2SqlError si algo falla."""
    tokens = tokenize(source)
    schema = parse(tokens)
    analyze(schema)
    return generate(schema)
