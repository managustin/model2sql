"""Análisis léxico: texto fuente -> lista de tokens."""

from model2sql.tokens import Token


def tokenize(source: str) -> list[Token]:
    """Devuelve los tokens de `source`, terminando siempre en un token EOF.

    Ignora espacios y comentarios `//` hasta fin de línea.
    Lanza LexicalError ante un carácter inesperado.
    """
    raise NotImplementedError("Lexer pendiente")
