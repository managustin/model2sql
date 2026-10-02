from model2sql.errores import ErrorLexico, ErrorSemantico, Posicion


def test_error_con_posicion_muestra_linea_y_columna() -> None:
    error = ErrorLexico("Carácter inesperado '$'", Posicion(linea=2, columna=12))

    assert str(error) == "Error léxico en línea 2, columna 12:\nCarácter inesperado '$'"


def test_error_sin_posicion_muestra_solo_el_tipo() -> None:
    error = ErrorSemantico("La tabla 'Cliente' referenciada en la FK no existe")

    assert str(error) == "Error semántico:\nLa tabla 'Cliente' referenciada en la FK no existe"
