"""Interfaz de consola: `model2sql entrada.model [-o salida.sql]`."""

import argparse
import sys
from pathlib import Path

from model2sql.errors import Model2SqlError
from model2sql.pipeline import transpile

EXIT_OK = 0
EXIT_COMPILE_ERROR = 1
EXIT_IO_ERROR = 2


def main(argv: list[str] | None = None) -> int:
    args = _parse_args(argv)
    try:
        source = args.input.read_text(encoding="utf-8")
    except OSError as error:
        print(f"No se pudo leer '{args.input}': {error.strerror}", file=sys.stderr)
        return EXIT_IO_ERROR

    try:
        sql = transpile(source)
    except Model2SqlError as error:
        print(error, file=sys.stderr)
        return EXIT_COMPILE_ERROR

    if args.output is None:
        print(sql)
    else:
        args.output.write_text(sql + "\n", encoding="utf-8")
    return EXIT_OK


def _parse_args(argv: list[str] | None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        prog="model2sql", description="Transpila un archivo .model a SQL DDL."
    )
    parser.add_argument("input", type=Path, help="archivo .model de entrada")
    parser.add_argument("-o", "--output", type=Path, help="archivo .sql de salida")
    return parser.parse_args(argv)
