from pathlib import Path

from model2sql.cli import SALIDA_ERROR_ARCHIVO, principal


def test_archivo_inexistente_devuelve_error_de_archivo(tmp_path: Path, capsys) -> None:
    codigo = principal([str(tmp_path / "no_existe.model")])

    assert codigo == SALIDA_ERROR_ARCHIVO
    assert "No se pudo leer" in capsys.readouterr().err
