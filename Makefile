FLEX ?= flex
BISON ?= bison
CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
CPPFLAGS ?=
INCLUDES := -Ic/include -Ibuild

# Los artefactos generados y el ejecutable se mantienen juntos en build/.
BUILD_DIR := build
EJECUTABLE := $(BUILD_DIR)/model2sql
# Solo los fuentes mantenidos a mano; parser y scanner se generan con Bison y Flex.
FUENTES_C := c/src/main.c c/src/ast.c c/src/compilacion.c c/src/semantica.c c/src/generador.c
CABECERAS_C := c/include/ast.h c/include/compilacion.h c/include/semantica.h \
	c/include/generador.h

.PHONY: all test clean

all: $(EJECUTABLE)

# Crea la carpeta de salida si todavía no existe.
$(BUILD_DIR):
	mkdir -p $@

# Bison convierte la gramática declarativa en el parser C y su cabecera de tokens.
$(BUILD_DIR)/parser.tab.c: c/src/parser.y c/include/ast.h c/include/compilacion.h | $(BUILD_DIR)
	$(BISON) -Wall -Werror=conflicts-sr -Werror=conflicts-rr \
		--defines=$(BUILD_DIR)/parser.tab.h --output=$@ $<

# Garantiza la cabecera incluso en entornos donde solo se solicita como dependencia.
$(BUILD_DIR)/parser.tab.h: $(BUILD_DIR)/parser.tab.c
	@test -f $@ || $(BISON) -Wall --defines=$@ --output=$(BUILD_DIR)/parser.tab.c c/src/parser.y

# Flex convierte las reglas léxicas en scanner y necesita los tokens declarados por Bison.
$(BUILD_DIR)/lexer.yy.c: c/src/lexer.l $(BUILD_DIR)/parser.tab.h $(CABECERAS_C) | $(BUILD_DIR)
	$(FLEX) --outfile=$@ $<

# Compila las etapas escritas a mano junto con los archivos generados.
$(EJECUTABLE): $(FUENTES_C) $(CABECERAS_C) $(BUILD_DIR)/parser.tab.c $(BUILD_DIR)/parser.tab.h \
		$(BUILD_DIR)/lexer.yy.c
	$(CC) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) $(FUENTES_C) \
		$(BUILD_DIR)/parser.tab.c $(BUILD_DIR)/lexer.yy.c -o $@

# La prueba de integración compara SQL y diagnósticos con los resultados esperados.
test: all
	sh scripts/test.sh ./$(EJECUTABLE)

# Elimina parser, scanner y ejecutable para permitir una compilación limpia.
clean:
	rm -rf $(BUILD_DIR)
