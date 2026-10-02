"""Tipos de token y palabras reservadas del DSL."""

from dataclasses import dataclass
from enum import Enum, auto

from model2sql.errores import Posicion


class TipoToken(Enum):
    MODEL = auto()

    INT = auto()
    STRING = auto()
    FLOAT = auto()
    BOOLEAN = auto()
    DATE = auto()

    PK = auto()
    UNIQUE = auto()
    NOT_NULL = auto()
    AUTO_INCREMENT = auto()
    FK = auto()

    LLAVE_ABRE = auto()
    LLAVE_CIERRA = auto()
    PARENTESIS_ABRE = auto()
    PARENTESIS_CIERRA = auto()
    PUNTO = auto()

    IDENTIFICADOR = auto()
    FIN = auto()


PALABRAS_RESERVADAS: dict[str, TipoToken] = {
    "model": TipoToken.MODEL,
    "int": TipoToken.INT,
    "string": TipoToken.STRING,
    "float": TipoToken.FLOAT,
    "boolean": TipoToken.BOOLEAN,
    "date": TipoToken.DATE,
    "pk": TipoToken.PK,
    "unique": TipoToken.UNIQUE,
    "not_null": TipoToken.NOT_NULL,
    "auto_increment": TipoToken.AUTO_INCREMENT,
    "fk": TipoToken.FK,
}

SIMBOLOS: dict[str, TipoToken] = {
    "{": TipoToken.LLAVE_ABRE,
    "}": TipoToken.LLAVE_CIERRA,
    "(": TipoToken.PARENTESIS_ABRE,
    ")": TipoToken.PARENTESIS_CIERRA,
    ".": TipoToken.PUNTO,
}

TIPOS_PRIMITIVOS = frozenset(
    {TipoToken.INT, TipoToken.STRING, TipoToken.FLOAT, TipoToken.BOOLEAN, TipoToken.DATE}
)


@dataclass(frozen=True)
class Token:
    tipo: TipoToken
    lexema: str
    posicion: Posicion
