# Pautas de código

La implementación oficial es C con Flex/Bison y sus pautas se describen primero. La sección
«Prototipos Python y Java» se conserva solo como referencia para esos prototipos incompletos,
que no forman parte de la entrega ejecutable.

## Implementación oficial en C

- `c/src/lexer.l` y `c/src/parser.y` son las fuentes del scanner y el parser; los archivos
  generados y el ejecutable viven en `build/` y no se editan manualmente.
- El AST y sus reglas de propiedad/liberación están en `c/include/ast.h` y `c/src/ast.c`.
- Cada etapa recibe los tipos definidos por la anterior; solo `main.c` coordina el pipeline
  y la entrada/salida de la CLI.
- Los errores se registran con etapa y posición; las etapas no imprimen diagnósticos.
- Los nombres y mensajes propios del proyecto se escriben en español.
- La generación del parser, scanner y ejecutable se realiza mediante el Makefile raíz.
- Antes de entregar, ejecutar `make clean && make test`.

## Prototipos Python y Java

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

- Los casos de punta a punta están en `casos/` y los ejecuta la implementación oficial con `make test`:
  `validos/x.model` + `x.sql`, o `invalidos/x.model` + `x.err` (mensaje exacto).
- Cada error semántico documentado tiene al menos un caso inválido.

## Antes de subir

```
make clean && make test
```

Commits chicos, en imperativo y en español: `Agrega validación de PK duplicada`.
