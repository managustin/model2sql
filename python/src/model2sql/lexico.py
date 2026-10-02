"""Análisis léxico: texto fuente -> tokens."""

from model2sql.tokens import Token


def tokenizar(fuente: str) -> list[Token]:
    """Devuelve los tokens de `fuente`, terminando en FIN. Lanza ErrorLexico.

    Descarta espacios y comentarios `//` hasta fin de línea.
    """
    raise NotImplementedError("Análisis léxico pendiente")
