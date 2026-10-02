package model2sql.errores;

import java.util.Optional;

/** Base de los errores que se reportan al usuario. */
public abstract class ErrorCompilacion extends RuntimeException {

    private static final long serialVersionUID = 1L;

    private final transient Posicion posicion;

    protected ErrorCompilacion(String mensaje, Posicion posicion) {
        super(mensaje);
        this.posicion = posicion;
    }

    protected abstract String tipo();

    public Optional<Posicion> posicion() {
        return Optional.ofNullable(posicion);
    }

    @Override
    public String toString() {
        if (posicion == null) {
            return tipo() + ":\n" + getMessage();
        }
        return "%s en línea %d, columna %d:\n%s"
                .formatted(tipo(), posicion.linea(), posicion.columna(), getMessage());
    }
}
