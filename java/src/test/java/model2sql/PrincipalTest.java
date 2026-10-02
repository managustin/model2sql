package model2sql;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

class PrincipalTest {

    private final ByteArrayOutputStream errores = new ByteArrayOutputStream();

    private int ejecutar(String... argumentos) {
        var salida = new PrintStream(new ByteArrayOutputStream(), true, StandardCharsets.UTF_8);
        var error = new PrintStream(errores, true, StandardCharsets.UTF_8);
        return Principal.ejecutar(argumentos, salida, error);
    }

    @Test
    void archivoInexistenteDevuelveErrorDeArchivo(@TempDir Path carpeta) {
        int codigo = ejecutar(carpeta.resolve("no_existe.model").toString());

        assertEquals(Principal.SALIDA_ERROR_ARCHIVO, codigo);
        assertTrue(errores.toString(StandardCharsets.UTF_8).contains("No se pudo leer"));
    }

    @Test
    void sinArgumentosMuestraElUso() {
        int codigo = ejecutar();

        assertEquals(Principal.SALIDA_USO_INCORRECTO, codigo);
        assertTrue(errores.toString(StandardCharsets.UTF_8).startsWith("Uso:"));
    }
}
