"""Encadena las etapas. Único punto de entrada de la lógica."""

from model2sql.generador import generar_sql
from model2sql.lexico import tokenizar
from model2sql.semantico import analizar_semantica
from model2sql.sintactico import analizar_sintaxis


def transpilar(fuente: str) -> str:
    """Convierte código .model en SQL. Lanza ErrorCompilacion."""
    esquema = analizar_sintaxis(tokenizar(fuente))
    analizar_semantica(esquema)
    return generar_sql(esquema)
