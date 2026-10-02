"""Tabla de símbolos global: modelos y sus atributos, indexados por nombre."""

from dataclasses import dataclass, field

from model2sql.nodes import Attribute, Model


@dataclass
class ModelSymbol:
    model: Model
    attributes: dict[str, Attribute] = field(default_factory=dict)


@dataclass
class SymbolTable:
    models: dict[str, ModelSymbol] = field(default_factory=dict)

    def lookup_model(self, name: str) -> ModelSymbol | None:
        return self.models.get(name)

    def lookup_attribute(self, model: str, attribute: str) -> Attribute | None:
        symbol = self.lookup_model(model)
        return symbol.attributes.get(attribute) if symbol else None
