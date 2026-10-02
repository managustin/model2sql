package model2sql.semantica;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Optional;
import model2sql.sintaxis.ast.Atributo;
import model2sql.sintaxis.ast.Modelo;

/** Tabla de símbolos global: modelos y atributos indexados por nombre. */
public final class TablaSimbolos {

    public record SimboloModelo(Modelo modelo, Map<String, Atributo> atributos) {}

    private final Map<String, SimboloModelo> modelos = new LinkedHashMap<>();

    public void registrar(SimboloModelo simbolo) {
        modelos.put(simbolo.modelo().nombre(), simbolo);
    }

    public Optional<SimboloModelo> buscarModelo(String nombre) {
        return Optional.ofNullable(modelos.get(nombre));
    }

    public Optional<Atributo> buscarAtributo(String modelo, String atributo) {
        return buscarModelo(modelo).map(simbolo -> simbolo.atributos().get(atributo));
    }
}
