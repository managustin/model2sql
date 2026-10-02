"""Consola: `model2sql entrada.model [-o salida.sql]`."""

import argparse
import sys
from pathlib import Path

from model2sql.errores import ErrorCompilacion
from model2sql.transpilador import transpilar

SALIDA_OK = 0
SALIDA_ERROR_COMPILACION = 1
SALIDA_ERROR_ARCHIVO = 2


def principal(argumentos: list[str] | None = None) -> int:
    opciones = _leer_opciones(argumentos)
    try:
        fuente = opciones.entrada.read_text(encoding="utf-8")
    except OSError as error:
        print(f"No se pudo leer '{opciones.entrada}': {error.strerror}", file=sys.stderr)
        return SALIDA_ERROR_ARCHIVO

    try:
        sql = transpilar(fuente)
    except ErrorCompilacion as error:
        print(error, file=sys.stderr)
        return SALIDA_ERROR_COMPILACION

    if opciones.salida is None:
        print(sql)
    else:
        opciones.salida.write_text(sql + "\n", encoding="utf-8")
    return SALIDA_OK


def _leer_opciones(argumentos: list[str] | None) -> argparse.Namespace:
    lector = argparse.ArgumentParser(
        prog="model2sql", description="Transpila un archivo .model a SQL DDL."
    )
    lector.add_argument("entrada", type=Path, help="archivo .model")
    lector.add_argument("-o", "--salida", type=Path, help="archivo .sql de salida")
    return lector.parse_args(argumentos)
