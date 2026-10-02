"""Nodos del AST. Son inmutables: ninguna etapa los modifica."""

from dataclasses import dataclass
from enum import Enum

from model2sql.errores import Posicion


class TipoPrimitivo(Enum):
    INT = "int"
    STRING = "string"
    FLOAT = "float"
    BOOLEAN = "boolean"
    DATE = "date"


@dataclass(frozen=True)
class ClaveForanea:
    modelo: str
    atributo: str
    posicion: Posicion


@dataclass(frozen=True)
class Atributo:
    nombre: str
    tipo: TipoPrimitivo
    posicion: Posicion
    clave_primaria: bool = False
    unico: bool = False
    no_nulo: bool = False
    autoincremental: bool = False
    clave_foranea: ClaveForanea | None = None


@dataclass(frozen=True)
class Modelo:
    nombre: str
    atributos: tuple[Atributo, ...]
    posicion: Posicion


@dataclass(frozen=True)
class Esquema:
    """Raíz del AST: los modelos del archivo en orden de aparición."""

    modelos: tuple[Modelo, ...]
