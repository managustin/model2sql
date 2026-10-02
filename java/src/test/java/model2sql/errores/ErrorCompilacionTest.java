package model2sql.errores;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Test;

class ErrorCompilacionTest {

    @Test
    void errorConPosicionMuestraLineaYColumna() {
        var error = new ErrorLexico("Carácter inesperado '$'", new Posicion(2, 12));

        assertEquals("Error léxico en línea 2, columna 12:\nCarácter inesperado '$'", error.toString());
    }

    @Test
    void errorSinPosicionMuestraSoloElTipo() {
        var error = new ErrorSemantico("La tabla 'Cliente' referenciada en la FK no existe");

        assertEquals(
                "Error semántico:\nLa tabla 'Cliente' referenciada en la FK no existe",
                error.toString());
    }
}
