# Decisiones

Una línea por decisión que no se deduce leyendo el código. La más nueva arriba.
Formato: `AAAA-MM-DD · decisión · por qué`.

- 2026-10-02 · El alcance termina en `CREATE TABLE` (sin ALTER, índices ni datos) · acordado con el grupo y el profesor.
- 2026-10-02 · Python 3.11+ sin dependencias de runtime · corre igual en Windows/Linux/macOS y el lexer/parser se escriben a mano, que es lo que evalúa la materia.
- 2026-10-02 · Parser descendente recursivo, un método por regla de la gramática · se lee igual que el EBNF y da buenos mensajes de error.
- 2026-10-02 · Dialecto de salida: PostgreSQL (`SERIAL`) · es el que usa el ejemplo del README.
- 2026-10-02 · Se corta en el primer error · simplifica el pipeline; reportar varios errores queda como mejora opcional.
- 2026-10-02 · Comentarios con `//` · el README pide comentarios pero no define sintaxis.
