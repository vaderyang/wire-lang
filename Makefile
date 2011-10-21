CC=gcc
C_OPTS=-std=c99
WARN_OPTS=-Wall
DBG_OPTS=-ggdb
OPTMZ_OPTS=
DIR_OPTS=-I./ -L./
COMPILE=$(CC) $(DIR_OPTS) $(LNK_OPTS) $(C_OPTS) $(WARN_OPTS) $(DBG_OPTS) $(OPTMZ_OPTS)

all: wire

wire: wire_utils.c wire_utils.h wire.yy.c wire_lex.c wire_lex.h wire.tab.c wire_ast.c wire_ast.h wire_semantics.c wire_semantics.h
	$(COMPILE) wire_utils.c wire.yy.c wire_lex.c wire.tab.c wire_ast.c wire_semantics.c -o wire
	
wire.tab.c: wire.y
	bison --defines --report all --report-file wire.bison.report --debug wire.y

wire.yy.c: wire.l
	flex -o wire.yy.c wire.l

clean:
	rm -f wire wire.yy.c wire.tab.c *~ wire.bison.report wire.tab.h *.o
	
wire_codegen: wire_codegen.c
	$(COMPILE) wire_utils.c wire_codegen.c -o wire_codegen

