# Pautas de código

Aplican a las dos versiones (`python/` y `java/`). Lo que no esté acá lo decide el linter o el compilador.

## Estructura

- Una etapa = un módulo o paquete: léxico → sintáctico → semántico → generación.
  Solo el transpilador las encadena; la consola solo lee y escribe archivos.
- Las etapas se pasan tipos definidos (`Token`, nodos del AST, `TablaSimbolos`), nunca mapas sueltos.
- El AST es inmutable (`dataclass(frozen=True)` / `record`). Ninguna etapa lo modifica.
- Una etapa no conoce a la siguiente. El generador no valida: recibe un AST ya validado.
- Los errores para el usuario heredan de `ErrorCompilacion` y llevan `Posicion` cuando la hay.
  Ninguna etapa imprime.
- Las dos versiones mantienen los mismos nombres de etapas, tipos y mensajes de error.

## Estilo

- Nombres en español (variables, funciones, clases). Se mantienen en inglés solo las
  palabras del DSL (`model`, `pk`, ...) y lo que impone el lenguaje (`main`, `__init__`).
- Nombres que digan qué es: `buscar_modelo`, no `obtener_item`.
- Funciones cortas, un nivel de abstracción, retorno temprano en vez de `if` anidados.
- Sin valores mágicos repetidos: van a una constante o tabla (`PALABRAS_RESERVADAS`, `TIPOS_SQL`).
- Python: type hints en toda firma pública, privados con `_`, complejidad máxima 8 (ruff).
- Java: clases `final`, `record` para datos, `Optional` en vez de `null` en lo público.

## Documentación

- Una línea de docstring/Javadoc por módulo, clase y función pública: qué hace y qué lanza.
- Comentarios solo para el porqué.
- `docs/gramatica.md` cambia en el mismo commit que el lenguaje.
- `docs/decisiones.md`: una línea por decisión no obvia.
- No se agregan otros documentos salvo que el trabajo práctico lo pida.

## Pruebas

- Los casos de punta a punta están en `casos/` y los usan ambas versiones:
  `validos/x.model` + `x.sql`, o `invalidos/x.model` + `x.err` (mensaje exacto).
- Pruebas unitarias por etapa, estilo preparar / ejecutar / verificar.
- Cada error semántico del README tiene al menos un caso inválido.

## Antes de subir

```
cd python && ruff check . && ruff format . && pytest
cd java && ./mvnw test        # Windows: mvnw.cmd test
```

Commits chicos, en imperativo y en español: `Agrega validación de PK duplicada`.
