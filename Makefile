all: wire

wire: wire.tab.c wire.yy.c wire_utils.c wire_lex.c wire_var.c wire_semantics.c
	gcc -Wall wire_utils.c wire_ast.c wire_lex.c wire_var.c wire_semantics.c wire.yy.c wire.tab.c -o wire

wire.tab.c: wire.y
	bison --defines --report all --report-file wire.bison.report --debug wire.y

wire.yy.c: wire.l
	flex -o wire.yy.c wire.l

clean:
	rm -f wire wire.yy.c wire.tab.c *~ wire.bison.report wire.tab.h *.o

