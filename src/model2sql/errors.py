"""Errores del transpilador, uno por etapa del pipeline."""

from dataclasses import dataclass


@dataclass(frozen=True)
class Position:
    """Ubicación en el código fuente (1-based)."""

    line: int
    column: int


class Model2SqlError(Exception):
    """Base de todos los errores que se le reportan al usuario."""

    kind = "Error"

    def __init__(self, message: str, position: Position | None = None) -> None:
        super().__init__(message)
        self.message = message
        self.position = position

    def __str__(self) -> str:
        if self.position is None:
            return f"{self.kind}:\n{self.message}"
        where = f"línea {self.position.line}, columna {self.position.column}"
        return f"{self.kind} en {where}:\n{self.message}"


class LexicalError(Model2SqlError):
    kind = "Error léxico"


class ParseError(Model2SqlError):
    kind = "Error sintáctico"


class SemanticError(Model2SqlError):
    kind = "Error semántico"
