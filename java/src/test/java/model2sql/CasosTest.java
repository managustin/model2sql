package model2sql;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.stream.Stream;
import model2sql.errores.ErrorCompilacion;
import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;

/**
 * Pruebas de punta a punta con los casos compartidos de /casos.
 * validos/x.model debe producir validos/x.sql; invalidos/x.model debe fallar con invalidos/x.err.
 */
@Disabled("Quitar al completar el pipeline")
class CasosTest {

    private static final Path CASOS = Path.of("..", "casos");

    static Stream<Path> validos() throws IOException {
        return casos("validos");
    }

    static Stream<Path> invalidos() throws IOException {
        return casos("invalidos");
    }

    @ParameterizedTest
    @MethodSource("validos")
    void casoValidoProduceElSqlEsperado(Path fuente) throws IOException {
        assertEquals(leer(conExtension(fuente, ".sql")), Transpilador.transpilar(leer(fuente)));
    }

    @ParameterizedTest
    @MethodSource("invalidos")
    void casoInvalidoInformaElErrorEsperado(Path fuente) throws IOException {
        String fuenteModel = leer(fuente);
        var error = assertThrows(ErrorCompilacion.class, () -> Transpilador.transpilar(fuenteModel));
        assertEquals(leer(conExtension(fuente, ".err")), error.toString());
    }

    private static Stream<Path> casos(String carpeta) throws IOException {
        try (Stream<Path> archivos = Files.list(CASOS.resolve(carpeta))) {
            return archivos.filter(ruta -> ruta.toString().endsWith(".model")).sorted().toList().stream();
        }
    }

    private static Path conExtension(Path modelo, String extension) {
        return Path.of(modelo.toString().replaceFirst("\\.model$", extension));
    }

    private static String leer(Path ruta) throws IOException {
        return Files.readString(ruta, StandardCharsets.UTF_8).strip();
    }
}
