# Gramática

Fuente de verdad del lenguaje. Si cambia la gramática, se cambia primero acá y después el parser.

```ebnf
schema     = { model } EOF ;
model      = "model" IDENT "{" { attribute } "}" ;
attribute  = IDENT type { modifier } ;
type       = "int" | "string" | "float" | "boolean" | "date" ;
modifier   = "pk" | "unique" | "not_null" | "auto_increment" | foreign_key ;
foreign_key = "fk" "(" IDENT "." IDENT ")" ;
```

## Léxico

```ebnf
IDENT      = letter { letter | digit | "_" } ;   (* que no sea palabra reservada *)
letter     = "a".."z" | "A".."Z" | "_" ;
digit      = "0".."9" ;
comment    = "//" { any char except newline } ;
```

- Palabras reservadas: `model`, los tipos y los modificadores.
- Espacios, tabs, saltos de línea y comentarios se descartan.
- Los saltos de línea no son significativos: un atributo termina cuando aparece un `IDENT`
  (empieza el siguiente) o `}`.

## Traducción de tipos

| DSL       | SQL            |
|-----------|----------------|
| `int`     | `INTEGER`      |
| `string`  | `VARCHAR(255)` |
| `float`   | `REAL`         |
| `boolean` | `BOOLEAN`      |
| `date`    | `DATE`         |

`int pk auto_increment` se genera como `SERIAL PRIMARY KEY`.
