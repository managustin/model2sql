"""Análisis sintáctico descendente recursivo: tokens -> AST.

Cada regla de docs/gramatica.md es un método `_analizar_<regla>`.
"""

from model2sql.nodos import Esquema
from model2sql.tokens import Token


def analizar_sintaxis(tokens: list[Token]) -> Esquema:
    """Construye el AST. Lanza ErrorSintactico ante el primer token inesperado."""
    raise NotImplementedError("Análisis sintáctico pendiente")
