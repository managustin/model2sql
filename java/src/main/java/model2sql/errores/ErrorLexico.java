package model2sql.errores;

public final class ErrorLexico extends ErrorCompilacion {

    private static final long serialVersionUID = 1L;

    public ErrorLexico(String mensaje, Posicion posicion) {
        super(mensaje, posicion);
    }

    public ErrorLexico(String mensaje) {
        this(mensaje, null);
    }

    @Override
    protected String tipo() {
        return "Error léxico";
    }
}
