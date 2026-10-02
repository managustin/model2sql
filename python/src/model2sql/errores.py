"""Errores del transpilador, uno por etapa."""

from dataclasses import dataclass


@dataclass(frozen=True)
class Posicion:
    """Ubicación en el código fuente (desde 1)."""

    linea: int
    columna: int


class ErrorCompilacion(Exception):
    """Base de los errores que se reportan al usuario."""

    tipo = "Error"

    def __init__(self, mensaje: str, posicion: Posicion | None = None) -> None:
        super().__init__(mensaje)
        self.mensaje = mensaje
        self.posicion = posicion

    def __str__(self) -> str:
        if self.posicion is None:
            return f"{self.tipo}:\n{self.mensaje}"
        ubicacion = f"línea {self.posicion.linea}, columna {self.posicion.columna}"
        return f"{self.tipo} en {ubicacion}:\n{self.mensaje}"


class ErrorLexico(ErrorCompilacion):
    tipo = "Error léxico"


class ErrorSintactico(ErrorCompilacion):
    tipo = "Error sintáctico"


class ErrorSemantico(ErrorCompilacion):
    tipo = "Error semántico"
