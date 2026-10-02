# Pautas de código

Cortas a propósito. Si algo no está acá, manda `ruff`.

## Estructura

- Una etapa del pipeline = un módulo: `lexer` → `parser` → `semantic` → `generator`.
  `pipeline.transpile()` es lo único que las encadena; `cli` solo hace E/S.
- Las etapas se comunican con tipos definidos (`Token`, nodos de `nodes.py`, `SymbolTable`),
  nunca con dicts sueltos ni strings.
- El AST es inmutable (`@dataclass(frozen=True)`, tuplas en vez de listas). Ninguna etapa lo modifica.
- Una etapa no importa a la siguiente. El generador no valida: asume un AST ya validado.
- Los errores para el usuario heredan de `Model2SqlError` y llevan `Position` cuando la hay.
  Nunca `print` dentro de una etapa.

## Estilo

- Código en inglés (nombres, variables); mensajes al usuario y docs en español.
- Nombres que digan qué es, no cómo está hecho: `lookup_model`, no `get_dict_item`.
- Funciones cortas, un solo nivel de abstracción. Complejidad máxima 8 (lo controla ruff).
- Retorno temprano en vez de `if` anidados.
- Type hints en toda firma pública.
- Nada de números ni strings mágicos repetidos: van a una constante o tabla
  (ej. `KEYWORDS`, `SQL_TYPES`).
- Funciones/métodos privados con `_` al principio.

## Documentación (sin verbosidad)

- **Docstring de una línea** en cada módulo y función pública: qué hace y qué lanza.
  Nada de repetir los parámetros si los type hints ya lo dicen.
- **Comentarios solo para el porqué**, nunca para el qué.
- **`docs/gramatica.md`** se actualiza en el mismo commit que cambia el lenguaje.
- **`docs/decisiones.md`**: una línea por decisión no obvia (formato en el archivo).
- No se escriben otros documentos salvo que el TP lo pida.

## Pruebas

- Un caso nuevo = un par de archivos en `tests/casos/`:
  `validos/x.model` + `x.sql`, o `invalidos/x.model` + `x.err` (mensaje exacto esperado).
- Pruebas unitarias por etapa en `tests/test_<etapa>.py`, estilo preparar / ejecutar / verificar.
- Cada error semántico del README tiene al menos un caso inválido.

## Antes de subir

```
ruff check . && ruff format . && pytest
```

Commits chicos en imperativo, en español: `Agrega validación de PK duplicada`.
Una rama por etapa o funcionalidad, mergeada por PR.
