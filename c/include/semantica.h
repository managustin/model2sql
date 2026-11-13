#ifndef MODEL2SQL_SEMANTICA_H
#define MODEL2SQL_SEMANTICA_H

#include "compilacion.h"

/* Comprueba nombres, modificadores y referencias antes de permitir generar SQL. */
bool analizar_semantica(const Esquema *esquema);

#endif
