"""Nodos del AST. Son inmutables: cada etapa lee el árbol, no lo modifica."""

from dataclasses import dataclass
from enum import Enum

from model2sql.errors import Position


class PrimitiveType(Enum):
    INT = "int"
    STRING = "string"
    FLOAT = "float"
    BOOLEAN = "boolean"
    DATE = "date"


@dataclass(frozen=True)
class ForeignKey:
    model: str
    attribute: str
    position: Position


@dataclass(frozen=True)
class Attribute:
    name: str
    type: PrimitiveType
    position: Position
    primary_key: bool = False
    unique: bool = False
    not_null: bool = False
    auto_increment: bool = False
    foreign_key: ForeignKey | None = None


@dataclass(frozen=True)
class Model:
    name: str
    attributes: tuple[Attribute, ...]
    position: Position


@dataclass(frozen=True)
class Schema:
    """Raíz del AST: todos los modelos de un archivo, en orden de aparición."""

    models: tuple[Model, ...]
