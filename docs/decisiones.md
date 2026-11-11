# Decisiones

Una línea por decisión no obvia. La más nueva arriba. Formato: `AAAA-MM-DD · decisión · por qué`.

- 2026-11-10 · La entrega oficial es C con Flex y Bison; Python/Java quedan como prototipos incompletos · cumplir el requisito de generadores de la consigna sin duplicar la solución.
- 2026-11-10 · Las claves foráneas adelantadas se emiten con `ALTER TABLE` tras crear las tablas · preservar el orden fuente y producir DDL válido en PostgreSQL.
- 2026-11-10 · La columna referenciada por una FK debe ser `pk` o `unique` · PostgreSQL exige unicidad en el destino.
- 2026-11-10 · Un modelo vacío y `auto_increment` sobre tipos distintos de `int` son errores semánticos · impedir DDL no válido o sin contenido útil.
- 2026-10-02 · Dos versiones en paralelo (Python y Java 21) hasta llegar a `CREATE TABLE` · comparar ambas antes de elegir una.
- 2026-10-02 · Casos de prueba compartidos en `casos/` · las dos versiones deben producir la misma salida.
- 2026-10-02 · Nombres de código en español · consistencia con los mensajes y la documentación.
- 2026-10-02 · Sin dependencias de ejecución; solo pytest/ruff (Python) y JUnit 5 (Java) para pruebas · el lexer y el parser se escriben a mano.
- 2026-10-02 · El alcance termina en `CREATE TABLE` · acordado con la cátedra.
- 2026-10-02 · Parser descendente recursivo, un método por regla · la gramática es LL(1) y así se lee igual que el EBNF.
- 2026-10-02 · Salida en dialecto PostgreSQL (`SERIAL`) · es el del ejemplo del README.
- 2026-10-02 · Se corta en el primer error · simplifica el pipeline.
- 2026-10-02 · Comentarios con `//` · el README pide comentarios sin definir la sintaxis.
