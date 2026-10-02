package model2sql.errores;

public final class ErrorSemantico extends ErrorCompilacion {

    private static final long serialVersionUID = 1L;

    public ErrorSemantico(String mensaje, Posicion posicion) {
        super(mensaje, posicion);
    }

    public ErrorSemantico(String mensaje) {
        this(mensaje, null);
    }

    @Override
    protected String tipo() {
        return "Error semántico";
    }
}
