package model2sql.sintaxis.ast;

import java.util.Optional;
import model2sql.errores.Posicion;

public record Atributo(
        String nombre,
        TipoPrimitivo tipo,
        Posicion posicion,
        boolean clavePrimaria,
        boolean unico,
        boolean noNulo,
        boolean autoincremental,
        Optional<ClaveForanea> claveForanea) {}
