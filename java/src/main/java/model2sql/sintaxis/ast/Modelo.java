package model2sql.sintaxis.ast;

import java.util.List;
import model2sql.errores.Posicion;

public record Modelo(String nombre, List<Atributo> atributos, Posicion posicion) {

    public Modelo {
        atributos = List.copyOf(atributos);
    }
}
