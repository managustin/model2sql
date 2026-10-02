package model2sql.lexico;

import java.util.Map;
import java.util.Set;

/** Tipos de token y palabras reservadas del DSL. */
public enum TipoToken {
    MODEL,

    INT, STRING, FLOAT, BOOLEAN, DATE,

    PK, UNIQUE, NOT_NULL, AUTO_INCREMENT, FK,

    LLAVE_ABRE, LLAVE_CIERRA, PARENTESIS_ABRE, PARENTESIS_CIERRA, PUNTO,

    IDENTIFICADOR, FIN;

    public static final Map<String, TipoToken> PALABRAS_RESERVADAS = Map.ofEntries(
            Map.entry("model", MODEL),
            Map.entry("int", INT),
            Map.entry("string", STRING),
            Map.entry("float", FLOAT),
            Map.entry("boolean", BOOLEAN),
            Map.entry("date", DATE),
            Map.entry("pk", PK),
            Map.entry("unique", UNIQUE),
            Map.entry("not_null", NOT_NULL),
            Map.entry("auto_increment", AUTO_INCREMENT),
            Map.entry("fk", FK));

    public static final Map<Character, TipoToken> SIMBOLOS = Map.of(
            '{', LLAVE_ABRE,
            '}', LLAVE_CIERRA,
            '(', PARENTESIS_ABRE,
            ')', PARENTESIS_CIERRA,
            '.', PUNTO);

    public static final Set<TipoToken> TIPOS_PRIMITIVOS = Set.of(INT, STRING, FLOAT, BOOLEAN, DATE);
}
