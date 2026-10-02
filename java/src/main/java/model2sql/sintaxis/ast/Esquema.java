package model2sql.sintaxis.ast;

import java.util.List;

/** Raíz del AST: los modelos del archivo en orden de aparición. Inmutable. */
public record Esquema(List<Modelo> modelos) {

    public Esquema {
        modelos = List.copyOf(modelos);
    }
}
