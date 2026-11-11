FLEX ?= flex
BISON ?= bison
CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
CPPFLAGS ?=
INCLUDES := -Ic/include -Ibuild

BUILD_DIR := build
EJECUTABLE := $(BUILD_DIR)/model2sql
FUENTES_C := c/src/main.c c/src/ast.c c/src/compilacion.c c/src/semantica.c c/src/generador.c
CABECERAS_C := c/include/ast.h c/include/compilacion.h c/include/semantica.h \
	c/include/generador.h

.PHONY: all test clean

all: $(EJECUTABLE)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/parser.tab.c: c/src/parser.y c/include/ast.h c/include/compilacion.h | $(BUILD_DIR)
	$(BISON) -Wall -Werror=conflicts-sr -Werror=conflicts-rr \
		--defines=$(BUILD_DIR)/parser.tab.h --output=$@ $<

$(BUILD_DIR)/parser.tab.h: $(BUILD_DIR)/parser.tab.c
	@test -f $@ || $(BISON) -Wall --defines=$@ --output=$(BUILD_DIR)/parser.tab.c c/src/parser.y

$(BUILD_DIR)/lexer.yy.c: c/src/lexer.l $(BUILD_DIR)/parser.tab.h $(CABECERAS_C) | $(BUILD_DIR)
	$(FLEX) --outfile=$@ $<

$(EJECUTABLE): $(FUENTES_C) $(CABECERAS_C) $(BUILD_DIR)/parser.tab.c $(BUILD_DIR)/parser.tab.h \
		$(BUILD_DIR)/lexer.yy.c
	$(CC) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) $(FUENTES_C) \
		$(BUILD_DIR)/parser.tab.c $(BUILD_DIR)/lexer.yy.c -o $@

test: all
	sh scripts/test.sh ./$(EJECUTABLE)

clean:
	rm -rf $(BUILD_DIR)
