"""Pruebas de punta a punta con los casos compartidos de /casos.

- validos/<x>.model   -> debe producir exactamente validos/<x>.sql
- invalidos/<x>.model -> debe fallar con el texto de invalidos/<x>.err
"""

from pathlib import Path

import pytest

from model2sql import transpilar
from model2sql.errores import ErrorCompilacion

CASOS = Path(__file__).resolve().parents[2] / "casos"

# Quitar al completar el pipeline; `strict` hace fallar la suite si ya pasan.
PENDIENTE = pytest.mark.xfail(raises=NotImplementedError, strict=True, reason="pendiente")


def _casos(carpeta: str) -> list[Path]:
    return sorted((CASOS / carpeta).glob("*.model"))


def _leer(ruta: Path) -> str:
    return ruta.read_text(encoding="utf-8")


@PENDIENTE
@pytest.mark.parametrize("fuente", _casos("validos"), ids=lambda ruta: ruta.stem)
def test_caso_valido_produce_el_sql_esperado(fuente: Path) -> None:
    esperado = _leer(fuente.with_suffix(".sql")).strip()
    assert transpilar(_leer(fuente)) == esperado


@PENDIENTE
@pytest.mark.parametrize("fuente", _casos("invalidos"), ids=lambda ruta: ruta.stem)
def test_caso_invalido_informa_el_error_esperado(fuente: Path) -> None:
    esperado = _leer(fuente.with_suffix(".err")).strip()
    with pytest.raises(ErrorCompilacion) as error:
        transpilar(_leer(fuente))
    assert str(error.value) == esperado
