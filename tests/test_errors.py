from model2sql.errors import LexicalError, Position, SemanticError


def test_error_with_position_shows_line_and_column() -> None:
    error = LexicalError("Carácter inesperado '$'", Position(line=2, column=12))

    assert str(error) == "Error léxico en línea 2, columna 12:\nCarácter inesperado '$'"


def test_error_without_position_shows_only_kind() -> None:
    error = SemanticError("La tabla 'Cliente' referenciada en la FK no existe")

    assert str(error) == "Error semántico:\nLa tabla 'Cliente' referenciada en la FK no existe"
