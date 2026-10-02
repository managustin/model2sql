"""Tabla de símbolos global: modelos y atributos indexados por nombre."""

from dataclasses import dataclass, field

from model2sql.nodos import Atributo, Modelo


@dataclass
class SimboloModelo:
    modelo: Modelo
    atributos: dict[str, Atributo] = field(default_factory=dict)


@dataclass
class TablaSimbolos:
    modelos: dict[str, SimboloModelo] = field(default_factory=dict)

    def buscar_modelo(self, nombre: str) -> SimboloModelo | None:
        return self.modelos.get(nombre)

    def buscar_atributo(self, modelo: str, atributo: str) -> Atributo | None:
        simbolo = self.buscar_modelo(modelo)
        return simbolo.atributos.get(atributo) if simbolo else None
