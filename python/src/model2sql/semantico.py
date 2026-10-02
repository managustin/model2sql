"""Análisis semántico en dos pasadas.

Pasada 1: registra modelos y atributos en la tabla de símbolos.
Pasada 2: valida claves y referencias (admite referencias adelantadas).
"""

from model2sql.nodos import Esquema
from model2sql.simbolos import TablaSimbolos


def analizar_semantica(esquema: Esquema) -> TablaSimbolos:
    """Valida el esquema y devuelve la tabla de símbolos. Lanza ErrorSemantico."""
    raise NotImplementedError("Análisis semántico pendiente")
