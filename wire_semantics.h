#include "wire_ast.h"
#include <stdlib.h>


/*SYMBOLS*/
struct symbol {
 pnode_t declarator;
};
typedef struct symbol symbol_t;
typedef symbol_t* psymbol_t;

psymbol_t symbol_new(pnode_t declarator);
void symbol_del(psymbol_t psym);

/*SYMBOL LIST*/
struct symlist {
 psymbol_t psym;
 struct symlist* psl_elem_next;
};
typedef struct symlist symlist_t;
typedef symlist_t* psymlist_t;

psymlist_t symlist_new(void);
void symlist_del(psymlist_t psl);
void symlist_add_sym(psymlist_t psl, psymbol_t psym);

/*SCOPE*/
#define SCOPE_WIRE -1
#define SCOPE_PROTOCOL 0
#define SCOPE_STRUCTURE 1
#define SCOPE_UNION 2
#define SCOPE_OPERATION 3

#define SCOPE_NAME_LEN 512

struct scope {
 int type;
 char name[512];
 psymlist_t psyms;
};
typedef struct scope scope_t;
typedef scope_t* pscope_t;

pscope_t scope_new(int type, char *name);
void scope_del(pscope_t pscope);
void scope_add_sym(pscope_t pscope, psymbol_t psym);

/*SCOPE STACK*/
struct scope_stack {
 pscope_t pscope;
 struct scope_stack* pss_up;
};
typedef struct scope_stack scope_stack_t;
typedef scope_stack_t* pscope_stack_t;

void scope_stack_push(pscope_t pscope);
pscope_t scope_stack_pull(void);

#define CHECK_SUCCESS 1
#define CHECK_FAIL 0
int check_semantics(pnode_t pnd_protocol);
int check_protocol(pnode_t pnd_rotocol);
int check_attributes(pnode_t pnd_attributes);
int check_protocol_body(pnode_t pnd_protocol_body);
