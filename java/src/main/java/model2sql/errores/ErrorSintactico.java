package model2sql.errores;

public final class ErrorSintactico extends ErrorCompilacion {

    private static final long serialVersionUID = 1L;

    public ErrorSintactico(String mensaje, Posicion posicion) {
        super(mensaje, posicion);
    }

    public ErrorSintactico(String mensaje) {
        this(mensaje, null);
    }

    @Override
    protected String tipo() {
        return "Error sintáctico";
    }
}
