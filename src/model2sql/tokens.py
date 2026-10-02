"""Tipos de token y palabras reservadas del DSL."""

from dataclasses import dataclass
from enum import Enum, auto

from model2sql.errors import Position


class TokenType(Enum):
    # Palabra reservada de bloque
    MODEL = auto()

    # Tipos primitivos
    INT = auto()
    STRING = auto()
    FLOAT = auto()
    BOOLEAN = auto()
    DATE = auto()

    # Modificadores
    PK = auto()
    UNIQUE = auto()
    NOT_NULL = auto()
    AUTO_INCREMENT = auto()
    FK = auto()

    # Símbolos
    LBRACE = auto()
    RBRACE = auto()
    LPAREN = auto()
    RPAREN = auto()
    DOT = auto()

    IDENTIFIER = auto()
    EOF = auto()


KEYWORDS: dict[str, TokenType] = {
    "model": TokenType.MODEL,
    "int": TokenType.INT,
    "string": TokenType.STRING,
    "float": TokenType.FLOAT,
    "boolean": TokenType.BOOLEAN,
    "date": TokenType.DATE,
    "pk": TokenType.PK,
    "unique": TokenType.UNIQUE,
    "not_null": TokenType.NOT_NULL,
    "auto_increment": TokenType.AUTO_INCREMENT,
    "fk": TokenType.FK,
}

SYMBOLS: dict[str, TokenType] = {
    "{": TokenType.LBRACE,
    "}": TokenType.RBRACE,
    "(": TokenType.LPAREN,
    ")": TokenType.RPAREN,
    ".": TokenType.DOT,
}

PRIMITIVE_TYPES = frozenset(
    {TokenType.INT, TokenType.STRING, TokenType.FLOAT, TokenType.BOOLEAN, TokenType.DATE}
)


@dataclass(frozen=True)
class Token:
    type: TokenType
    lexeme: str
    position: Position
