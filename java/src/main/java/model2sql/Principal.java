package model2sql;

import java.io.IOException;
import java.io.PrintStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import model2sql.errores.ErrorCompilacion;

/** Consola: {@code java -jar model2sql.jar entrada.model [-o salida.sql]}. */
public final class Principal {

    static final int SALIDA_OK = 0;
    static final int SALIDA_ERROR_COMPILACION = 1;
    static final int SALIDA_ERROR_ARCHIVO = 2;
    static final int SALIDA_USO_INCORRECTO = 3;

    private static final String USO = "Uso: model2sql entrada.model [-o salida.sql]";

    private Principal() {}

    public static void main(String[] argumentos) {
        System.exit(ejecutar(argumentos, System.out, System.err));
    }

    static int ejecutar(String[] argumentos, PrintStream salida, PrintStream errores) {
        if (!usoValido(argumentos)) {
            errores.println(USO);
            return SALIDA_USO_INCORRECTO;
        }
        Path entrada = Path.of(argumentos[0]);
        try {
            String sql = Transpilador.transpilar(Files.readString(entrada, StandardCharsets.UTF_8));
            escribir(sql, argumentos, salida);
            return SALIDA_OK;
        } catch (ErrorCompilacion error) {
            errores.println(error);
            return SALIDA_ERROR_COMPILACION;
        } catch (IOException error) {
            errores.println("No se pudo leer o escribir '%s'".formatted(entrada));
            return SALIDA_ERROR_ARCHIVO;
        }
    }

    private static boolean usoValido(String[] argumentos) {
        return argumentos.length == 1 || (argumentos.length == 3 && argumentos[1].equals("-o"));
    }

    private static void escribir(String sql, String[] argumentos, PrintStream salida)
            throws IOException {
        if (argumentos.length == 1) {
            salida.println(sql);
            return;
        }
        Files.writeString(Path.of(argumentos[2]), sql + "\n", StandardCharsets.UTF_8);
    }
}
