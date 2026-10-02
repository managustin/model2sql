"""Pruebas de punta a punta guiadas por archivos en tests/casos/.

- validos/<nombre>.model   -> debe producir exactamente validos/<nombre>.sql
- invalidos/<nombre>.model -> debe fallar con el texto de invalidos/<nombre>.err

Agregar un caso = agregar un par de archivos; no hace falta tocar este módulo.
"""

from pathlib import Path

import pytest

from model2sql import transpile
from model2sql.errors import Model2SqlError

CASES = Path(__file__).parent / "casos"

# Quitar este marcador cuando el pipeline esté completo; `strict` hace fallar
# la suite si los casos empiezan a pasar y nadie lo quitó.
PENDING = pytest.mark.xfail(raises=NotImplementedError, strict=True, reason="pipeline pendiente")


def _cases(folder: str) -> list[Path]:
    return sorted((CASES / folder).glob("*.model"))


def _read(path: Path) -> str:
    return path.read_text(encoding="utf-8")


@PENDING
@pytest.mark.parametrize("source", _cases("validos"), ids=lambda p: p.stem)
def test_valid_case_produces_expected_sql(source: Path) -> None:
    expected = _read(source.with_suffix(".sql")).strip()
    assert transpile(_read(source)) == expected


@PENDING
@pytest.mark.parametrize("source", _cases("invalidos"), ids=lambda p: p.stem)
def test_invalid_case_reports_expected_error(source: Path) -> None:
    expected = _read(source.with_suffix(".err")).strip()
    with pytest.raises(Model2SqlError) as error:
        transpile(_read(source))
    assert str(error.value) == expected
