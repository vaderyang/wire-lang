#ifndef WIRE_LEX
#define WIRE_LEX

#include <stdio.h>
#include "wire_ast.h" //so wire.tab.h has a definition of pnode_t
#include "wire.tab.h"

/*LEXER data*/
typedef struct {
	const char *identifier;
	unsigned int token;
	} RESERVED_WORD;

static const RESERVED_WORD primitive_types[] = {
	{"bit",	tBIT},
	{"byte",	tBYTE},
	{"double",	tDOUBLE},
	{"string",	tSTRING},
	{"int",	tINT},
	{NULL, -1}
};

static const RESERVED_WORD constructed_types[] = {
	{"protocol",	tPROTOCOL},
	{"enum",	tENUM},
	{"struct",	tSTRUCT},
	{"union",	tUNION},
	{"packet",	tPACKET},
	{NULL,	-1}
};

static const RESERVED_WORD statements[] = {
	{"import",	tIMPORT},
	{"typedef",	tTYPEDEF},
	{"unsigned",	tUNSIGNED},
	{"signed",	tSIGNED},
	{"default",	tDEFAULT},
	{NULL,	-1}
};

unsigned int get_token_by_identifier(char* identifier);

#endif //WIRE_LEX
