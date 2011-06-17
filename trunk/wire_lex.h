#ifndef WIRE_LEX
#define WIRE_LEX

#include <stdio.h>
#include "wire_ast.h" //so wire.tab.h has a definition of pnode_t
#include "wire.tab.h"

/*LEXER data*/
struct {
	const char *identifier;
	unsigned int token;
} reserved_word;
typedef reserved_word reserved_word_t;
typedef reserved_word_t* preserved_word_t;

static const reserved_word_t primitive_types[] = {
//	{"bit",	tBIT},
	{"byte",	tBYTE},
	{"float",	tFLOAT},
	{"string",	tSTRING},
	{"uint",	tUINT},
    {"sint",	tSINT},
	{NULL, -1}
};

static const reserved_word_t constructed_types[] = {
//	{"protocol",	tPROTOCOL},
	{"enum",	tENUM},
	{"struct",	tSTRUCT},
	{"union",	tUNION},
	{"pdu",	tPDU},
	{NULL,	-1}
};

static const reserved_word_t protocol = {"protocol", tPROTOCOL};

static const reserved_word_t operation = {"operation", tOPERATION};

static const reserved_word_t statements[] = {
	{"import",	tIMPORT},
	{"typedef",	tTYPEDEF},
	{"default",	tDEFAULT},
	{NULL,	-1}
};

unsigned int get_token_by_identifier(char* identifier);

#endif //WIRE_LEX
