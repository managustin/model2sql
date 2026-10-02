from pathlib import Path

from model2sql.cli import EXIT_IO_ERROR, main


def test_missing_input_file_returns_io_error(tmp_path: Path, capsys) -> None:
    exit_code = main([str(tmp_path / "no_existe.model")])

    assert exit_code == EXIT_IO_ERROR
    assert "No se pudo leer" in capsys.readouterr().err
