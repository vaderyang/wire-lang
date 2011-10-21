//#include "wire_attr.h"
#include <stdlib.h>

enum wire_types{
	WIRE_TYPES_UINT,
	WIRE_TYPES_SINT,
	WIRE_TYPES_FLOAT,
	WIRE_TYPES_STRING,
	WIRE_TYPES_PDU,
	WIRE_TYPES_STRUCT,
	WIRE_TYPES_UNION,
	WIRE_TYPES_ENUM,
	WIRE_TYPES_ARRAY,
	WIRE_TYPES_ANY
};
typedef enum wire_types wire_types_t;

#define ATTR_MAX_NAME_LEN 512
#define ATTR_VAR_ARGCNT -1
#define ATTR_MAX_ARGCNT 512
struct attribute{
	char name[ATTR_MAX_NAME_LEN];
	unsigned int argcnt;
	wire_types_t wrtype[ATTR_MAX_ARGCNT];
};
typedef struct attribute attribute_t;
typedef attribute_t* pattribute_t;

/*
there can be >=0 attribute arguments
there can be variable attribute arguments count
*/
attribute_t serdes_attributes[] = {
	{"size", 1, {WIRE_TYPES_UINT}},
	{"size_bits", 1, {WIRE_TYPES_UINT}},
	{"align", 1, {WIRE_TYPES_UINT}},
	{"byte_order", 1, {WIRE_TYPES_STRING}},
	{"bit_order", 1, {WIRE_TYPES_STRING}},
	{"fp_rep", 1, {WIRE_TYPES_STRING}},
	{"char_enc", 1, {WIRE_TYPES_STRING}},
	{"delim", 1, {WIRE_TYPES_ANY}},
	{"", 0, {}}
};

attribute_t comm_attributes[] = {
	{"endpoint", 1, {WIRE_TYPES_STRING}},
	{"timeout", 1, {WIRE_TYPES_UINT}},
	{"", 0, {}}
};

attribute_t sanity_attributes[] = {
	{"exception", ATTR_VAR_ARGCNT, {WIRE_TYPES_ANY}},
	{"list", ATTR_VAR_ARGCNT, {WIRE_TYPES_ANY}},
	{"range", 2, {WIRE_TYPES_ANY, WIRE_TYPES_ANY}},
	{"", 0, {}}
};

int main(){

  return 0;
}
