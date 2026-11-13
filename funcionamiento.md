# Funcionamiento de Model2SQL

## Alcance e implementación

Model2SQL transpila archivos `.model` a SQL DDL para PostgreSQL. La implementación
está escrita en C: Flex genera el scanner y Bison genera el parser.

El pipeline es:

```text
archivo .model
    → scanner Flex (tokens y ubicaciones)
    → parser Bison (AST)
    → análisis semántico (tabla de símbolos)
    → generador SQL
    → salida estándar o archivo .sql
```

`Makefile` genera `build/parser.tab.c` y `build/parser.tab.h` con Bison, genera
`build/lexer.yy.c` con Flex y compila esos archivos junto con las etapas escritas
en C para producir `build/model2sql`. Los archivos generados viven en `build/`;
no se editan manualmente.

## Lenguaje fuente

Cada declaración `model Nombre { ... }` define una tabla. Los atributos tienen
nombre, tipo y modificadores. Los tipos son `int`, `string`, `float`, `boolean`
y `date`. Los modificadores son:

- `pk`: clave primaria.
- `unique`: valor único.
- `not_null`: atributo obligatorio.
- `auto_increment`: contador generado; solo se admite con `int`.
- `fk(Modelo.atributo)`: referencia a otra columna.

Los espacios y saltos de línea separan tokens, no delimitan atributos. Se
descartan los comentarios que comienzan con `//`. La gramática completa está
documentada en [docs/gramatica.md](./docs/gramatica.md).

## Etapas del compilador

1. **Análisis léxico:** reconoce palabras reservadas, identificadores, signos,
   espacios y comentarios. Los tokens conservan línea y columna; un carácter
   desconocido produce un error léxico.
2. **Análisis sintáctico:** Bison valida la gramática y construye el AST de
   esquemas, modelos, atributos y restricciones. Los errores sintácticos
   indican ubicación y token inesperado.
3. **Análisis semántico:** la primera pasada registra modelos y atributos y
   detecta nombres duplicados, modelos vacíos, más de una clave primaria y el
   uso de `auto_increment` sobre tipos distintos de `int`. La segunda resuelve
   las FK y comprueba existencia de tabla/columna, unicidad (`pk` o `unique`)
   y compatibilidad de tipos.
4. **Generación SQL:** traduce los tipos a `INTEGER`, `VARCHAR(255)`, `REAL`,
   `BOOLEAN` y `DATE`. `int auto_increment` se representa como `SERIAL`.
   Conserva el orden de los modelos; las FK a modelos anteriores se incluyen
   en `CREATE TABLE`, y las referencias adelantadas se emiten al final como
   `ALTER TABLE`.

El compilador se detiene ante el primer error. Los errores léxicos, sintácticos
y semánticos muestran etapa, línea y columna.

## Requisitos

Se necesita GCC o Clang, GNU Make, Flex y Bison.

En Windows, se recomienda usar WSL 2 con Ubuntu. En Ubuntu o WSL, las
herramientas se pueden instalar con:

```sh
sudo apt update
sudo apt install build-essential flex bison
```

## Compilar y usar

Ejecutar los comandos desde la raíz del repositorio:

```sh
make
```

Para imprimir el SQL:

```sh
./build/model2sql ejemplos/usuarios.model
```

Para escribirlo en un archivo:

```sh
./build/model2sql ejemplos/usuarios.model -o salida.sql
```

La opción larga `--salida` equivale a `-o`. Los códigos de salida son:

- `0`: compilación correcta.
- `1`: error léxico, sintáctico o semántico.
- `2`: uso incorrecto o error de lectura/escritura.

## Probar

```sh
make test
```

El script `scripts/test.sh` ejecuta los archivos de `casos/`. Actualmente hay
cinco entradas válidas, que comparan el SQL producido con su `.sql`, y once
inválidas, que comparan el diagnóstico completo con su `.err`. Los escenarios
cubren traducción de tipos, comentarios, claves primarias y foráneas,
referencias adelantadas y circulares, errores léxicos/sintácticos/semánticos,
la escritura con `-o`, argumentos incompletos y archivos inexistentes.

Estas pruebas no necesitan una instancia PostgreSQL: verifican el resultado
generado y los mensajes, pero no ejecutan el DDL en una base. Para eliminar el
parser, scanner y ejecutable generados:

```sh
make clean
```

