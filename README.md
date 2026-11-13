# Model2SQL

Transpilador de un DSL declarativo para la definición de esquemas relacionales y la generación automática de SQL DDL.

El proyecto permite describir modelos de datos mediante archivos `.model`, validar su estructura y relaciones, y generar las sentencias `CREATE TABLE` correspondientes.

## Ejemplo

Entrada:

```text
model Rol {
    id int pk auto_increment
    nombre string unique not_null
}

model Usuario {
    id int pk auto_increment
    nombre string not_null
    email string unique not_null
    edad int
    rol_id int not_null fk(Rol.id)
}
```

Salida:

```sql
CREATE TABLE Rol (
    id SERIAL PRIMARY KEY,
    nombre VARCHAR(255) UNIQUE NOT NULL
);

CREATE TABLE Usuario (
    id SERIAL PRIMARY KEY,
    nombre VARCHAR(255) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    edad INTEGER,
    rol_id INTEGER NOT NULL,
    FOREIGN KEY (rol_id) REFERENCES Rol(id)
);
```

## Arquitectura

El transpilador sigue una arquitectura clásica de compilación:

```text
Código fuente .model
        │
        ▼
      Lexer
        │
        ▼
      Parser
        │
        ▼
       AST
        │
        ▼
Analizador semántico
        │
        ▼
Generador de código
        │
        ▼
   Archivo .sql
```

El análisis semántico utiliza dos pasadas:

1. Registro de modelos, atributos, tipos y modificadores en la tabla de símbolos.
2. Validación de restricciones, claves y relaciones entre modelos.

## Lenguaje

Tipos primitivos soportados:

- `int`
- `string`
- `float`
- `boolean`
- `date`

Modificadores disponibles:

- `pk`
- `unique`
- `not_null`
- `auto_increment`
- `fk(Modelo.atributo)`

## Etapas del transpilador

### Análisis léxico

El lexer reconoce:

- palabras reservadas;
- identificadores;
- tipos primitivos;
- modificadores;
- llaves, paréntesis y puntos;
- espacios en blanco y comentarios.

### Análisis sintáctico

El parser valida la estructura del DSL y genera un Árbol de Sintaxis Abstracta (AST) compuesto por los modelos, atributos y restricciones declaradas.

### Análisis semántico

El analizador semántico comprueba:

- modelos duplicados;
- atributos duplicados;
- múltiples claves primarias en un mismo modelo;
- existencia de modelos y atributos referenciados;
- que el atributo referenciado por una FK sea `pk` o `unique`;
- compatibilidad de tipos en claves foráneas;
- que `auto_increment` se aplique a un atributo `int`;
- que los modelos tengan al menos un atributo;
- referencias entre modelos.

Para permitir referencias adelantadas, el análisis se realiza en dos pasadas.
Las claves foráneas que apuntan a un modelo declarado después se generan como
`ALTER TABLE`, una vez creadas todas las tablas.

### Generación de código

Una vez validado el AST, el generador produce las sentencias SQL DDL correspondientes.

Entre las construcciones generadas se encuentran:

- `CREATE TABLE`
- `PRIMARY KEY`
- `FOREIGN KEY`
- `NOT NULL`
- `UNIQUE`

Los tipos del DSL se traducen a tipos SQL equivalentes.

## Manejo de errores

El transpilador contempla errores en las distintas etapas del proceso.

Ejemplos:

```text
Error léxico en línea L, columna C:
Carácter inesperado
```

```text
Error sintáctico en línea L, columna C:
Token inesperado '}'
```

```text
Error semántico en línea L, columna C:
El atributo 'email' ya fue declarado en 'Usuario'
```

```text
Error semántico en línea L, columna C:
La tabla 'Cliente' referenciada en la FK no existe
```

```text
Error semántico en línea L, columna C:
Tipo incompatible en FK: 'rol_id' (string) no coincide con 'Rol.id' (int)
```

## Objetivos

- Definir formalmente la gramática del DSL mediante BNF/EBNF.
- Implementar el análisis léxico y sintáctico.
- Generar un AST.
- Construir una tabla de símbolos global.
- Implementar análisis semántico en dos pasadas.
- Validar claves primarias, claves foráneas y compatibilidad de tipos.
- Generar SQL DDL a partir del modelo validado.
- Incorporar pruebas automatizadas con casos válidos e inválidos.

## Resultado esperado

El proyecto proporciona una herramienta de consola capaz de recibir un archivo `.model` y producir un archivo `.sql` equivalente o informar errores léxicos, sintácticos o semánticos con información de línea y columna.

## Desarrollo y uso

La implementación está en C y utiliza Flex para generar el scanner y Bison
para generar el parser.

### Requisitos

- GCC o Clang, GNU Make, Flex y Bison.
- En Windows, usar WSL 2 con Ubuntu (o un entorno equivalente que provea esas herramientas).

En Ubuntu/WSL se pueden instalar con:

```sh
sudo apt update
sudo apt install build-essential flex bison
```

Desde la raíz del repositorio:

```sh
make                 # genera parser, scanner y build/model2sql
make test            # ejecuta todos los casos compartidos
./build/model2sql ejemplos/usuarios.model -o salida.sql
make clean           # elimina los artefactos generados
```

Para imprimir el SQL en la terminal, se puede omitir `-o salida.sql`.
La CLI devuelve código 0 si termina correctamente, 1 ante errores léxicos,
sintácticos o semánticos, y 2 ante errores de uso o de archivos.

`make test` compara el SQL y los diagnósticos completos con los archivos de
referencia de `casos/`: actualmente hay cinco escenarios válidos y once inválidos.
La salida conserva el orden de declaración de los modelos; las FK adelantadas
se agregan al final con `ALTER TABLE`.

El informe no forma parte de este repositorio; su fuente y materiales de entrega se
mantienen fuera de GitHub.

### Estructura

```text
casos/        casos válidos (.model + .sql) e inválidos (.model + .err), compartidos
ejemplos/     archivos .model de ejemplo
docs/         gramática del lenguaje
c/include/    interfaces del compilador, AST y etapas
c/src/        implementación C, gramática Bison y reglas Flex
scripts/      pruebas de integración invocadas por Makefile
funcionamiento.md explicación del funcionamiento, las pruebas y el uso
```

La gramática está documentada en [docs/gramatica.md](docs/gramatica.md).
