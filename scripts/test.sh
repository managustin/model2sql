#!/bin/sh
set -eu

# Resuelve las rutas desde la ubicación del propio script, no desde el directorio actual.
raiz=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
ejecutable=${1:-"$raiz/build/model2sql"}
case "$ejecutable" in
    /*) ;;
    *) ejecutable="$raiz/$ejecutable" ;;
esac
temporal=$(mktemp -d)
trap 'rm -rf "$temporal"' EXIT HUP INT TERM

# Cada modelo válido debe terminar bien, no emitir errores y coincidir con su SQL esperado.
for fuente in "$raiz"/casos/validos/*.model; do
    [ -e "$fuente" ] || continue
    nombre=${fuente%.model}
    if ! "$ejecutable" "$fuente" >"$temporal/salida" 2>"$temporal/error"; then
        printf 'Falló el caso válido: %s\n' "$fuente" >&2
        cat "$temporal/error" >&2
        exit 1
    fi
    tr -d '\r' <"$nombre.sql" >"$temporal/esperado"
    if [ -s "$temporal/error" ] || ! diff -u "$temporal/esperado" "$temporal/salida"; then
        printf 'La salida no coincide para: %s\n' "$fuente" >&2
        exit 1
    fi
done

# Cada modelo inválido debe devolver el código de compilación y el diagnóstico exactos.
for fuente in "$raiz"/casos/invalidos/*.model; do
    [ -e "$fuente" ] || continue
    nombre=${fuente%.model}
    estado=0
    "$ejecutable" "$fuente" >"$temporal/salida" 2>"$temporal/error" || estado=$?
    tr -d '\r' <"$nombre.err" >"$temporal/esperado"
    if [ "$estado" -ne 1 ] || ! diff -u "$temporal/esperado" "$temporal/error"; then
        printf 'El caso inválido no produjo el error esperado: %s\n' "$fuente" >&2
        exit 1
    fi
done

# Comprueba también el modo de salida a archivo y que no cambie el contenido generado.
"$ejecutable" "$raiz/casos/validos/readme.model" -o "$temporal/archivo.sql"
tr -d '\r' <"$raiz/casos/validos/readme.sql" >"$temporal/esperado"
cmp "$temporal/esperado" "$temporal/archivo.sql"

estado=0
# Sin argumentos, la CLI debe explicar el uso y distinguirlo de un error del modelo.
"$ejecutable" >"$temporal/salida" 2>"$temporal/error" || estado=$?
if [ "$estado" -ne 2 ] || ! grep -q '^Uso:' "$temporal/error"; then
    printf 'La CLI no informó el uso incorrecto\n' >&2
    exit 1
fi

estado=0
# Una ruta de entrada inexistente debe producir un error de archivo, no SQL parcial.
"$ejecutable" "$temporal/no_existe.model" >"$temporal/salida" 2>"$temporal/error" || estado=$?
if [ "$estado" -ne 2 ] || ! grep -q '^No se pudo leer' "$temporal/error"; then
    printf 'La CLI no informó el archivo de entrada inexistente\n' >&2
    exit 1
fi

estado=0
# Un directorio de salida inválido valida que también se informen fallos de escritura.
"$ejecutable" "$raiz/casos/validos/readme.model" -o "$temporal/no_existe/salida.sql" \
    >"$temporal/salida" 2>"$temporal/error" || estado=$?
if [ "$estado" -ne 2 ] || ! grep -q '^No se pudo escribir' "$temporal/error"; then
    printf 'La CLI no informó el error al escribir la salida\n' >&2
    exit 1
fi

printf 'Pruebas de integración: %s válidos y %s inválidos, todas correctas.\n' \
    "$(find "$raiz/casos/validos" -maxdepth 1 -name '*.model' | wc -l | tr -d ' ')" \
    "$(find "$raiz/casos/invalidos" -maxdepth 1 -name '*.model' | wc -l | tr -d ' ')"
