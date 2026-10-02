"""Análisis semántico en dos pasadas sobre el AST.

Pasada 1: registra modelos y atributos en la tabla de símbolos.
Pasada 2: valida claves y referencias (permite referencias adelantadas).
"""

from model2sql.nodes import Schema
from model2sql.symbols import SymbolTable


def analyze(schema: Schema) -> SymbolTable:
    """Valida `schema` y devuelve la tabla de símbolos. Lanza SemanticError."""
    raise NotImplementedError("Análisis semántico pendiente")
