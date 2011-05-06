/*1.2 SCOPES*/
enum scopes {
	PROTOCOL,
	STRUCTURE,
	UNION,
	OPERATION
};

struct scope {
	int type;
	char* name;
	psymlist syms;
};

typedef struct scope scope_t;
typedef scope_t* pscope_t;

pscope_t new_scope(int type, char* name);

pscope_t new_scope(int type, char* name){
	pscope_t pscope = (pscope_t)malloc(sizeof(scope_t));
	
	print_debug("new_scope: %s\n", name);
	
	if(pscope == NULL){
		print_error("new_scope: Could not allocate space!\n");
		exit(1);
	}
	pscope->type = type;
	pscope->name = name;
	pscope->syms = NULL;
	return pscope;
};

void del_scope(pscope_t pscope){
	del_symlist(pscope->symlist);
	free(pscope); 
}

struct scope_stack {
	pscope_t pscope;
	struct scope_stack* prev;
};

typedef struct scope_stack scope_stack_t;
typedef scope_stack_t* pscope_stack_t;

pscope_stack_t glob_pscope_stack = NULL;

void push_scope(pscope_t pscope){
	pscope_stack_t pscope_stack_prev = glob_pscope_stack;

	print_debug("push_scope: %s\n", pscope->name);
	
	glob_pscope_stack = (pscope_stack_t)malloc(sizeof(scope_stack_t));
	if(glob_pscope_stack == NULL){
		print_error("new_scope_stack: Could not allocate space!\n");
		exit(1);
	}
	glob_pscope_stack->prev = pscope_stack_prev;
	glob_pscope_stack->pscope = pscope;
}

pscope_t pull_scope(){
	pscope_stack_t pscope_stack_prev = glob_pscope_stack->prev;
	
	print_debug("pull_scope: %s\n", glob_pscope_stack->pscope->name);	
	
	free(glob_pscope_stack);
	glob_pscope_stack = pscope_stack_prev;
}



/*1.3 SYMBOLS*/

struct symbol {
	pnode_t declarator;
	pscope_t scope;
};

typedef struct symbol symbol_t;
typedef symbol_t* psymbol_t;

struct symlist {
	psymbol_t psymbol;
	struct symlist* next;
};

typedef symlist symlist_t;
typedef symlist* psymlist_t;

psymlist_t add_sym(psymlist_t psymlist, psym_t psym){
	
}

