Desarrollar un compilador o intérprete para un problema definido por los estudiantes. La solución debe incluir las siguientes características:

Lenguaje fuente:
Definir la sintaxis y gramática del lenguaje que van a procesar.
Puede ser un mini-lenguaje para expresiones aritméticas, comandos simples o cualquier problema concreto que quieran resolver.


Frontend (análisis):
Implementar el análisis léxico usando un generador de scanners tipo Flex.
Implementar el análisis sintáctico y semántico usando un generador de parsers tipo Bison.


Backend (resultado):
Evaluar la entrada o generar código en un lenguaje destino simple.
Mostrar mensajes claros de errores sintácticos o semánticos.


Escenarios de prueba:
Implementar y ejecutar varios casos representativos que demuestren el funcionamiento del compilador/interprete.
Todos los escenarios deben ser ejecutables fácilmente en el ambiente de desarrollo elegido.


Durante la defensa el 14/11 deberán presentar:
El problema identificado y su relevancia.
El lenguaje diseñado (fuente y destino).
La arquitectura del compilador/interprete (frontend y backend).
Los escenarios probados, demostrando su funcionamiento.
Máximo tiempo estipulado: 15 minutos por grupo.


Qué debe tenerse en cuenta en la entrega:
Informe con formato LNCS (excluyente). No se aceptará formatos que no se adecue a dicho formato. Debe contener, Titulo, autores, resumen, palabras clave, Introducción, antecedentes, desarollo, ... y debe toda referencia bibliográfica ser citada en el texto y figurar en la sección final Referencias bibliográficas, así como toda tabla, código o gráficos, figurar con sus respectivos epigrafes y citados en el texto, previo a su aparición. Esto es nada más que un recordatorio, ya se ha mencionado previamente.
Compilador con framework, debe incluir un buen "ambiente" para ejecutar los distintos casos, con secuencias de ejecución (scripts). Toda la generación de scaner, parser y el respectivo ejecutable, debe realizarse a través de makefile