#ifndef MODEL2SQL_GENERADOR_H
#define MODEL2SQL_GENERADOR_H

#include "ast.h"

/* Convierte un AST ya validado en SQL asignado dinámicamente; quien llama lo libera. */
char *generar_sql(const Esquema *esquema);

#endif
