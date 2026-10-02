"""Análisis sintáctico descendente recursivo: tokens -> AST.

Cada regla de docs/gramatica.md se implementa como un método `_parse_<regla>`.
"""

from model2sql.nodes import Schema
from model2sql.tokens import Token


def parse(tokens: list[Token]) -> Schema:
    """Construye el AST. Lanza ParseError en el primer token inesperado."""
    raise NotImplementedError("Parser pendiente")
