#include "wire_semantics.h"
#include "wire_ast.h"
#include "wire_utils.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*SYMBOLS*/
psymbol_t symbol_new(pnode_t declarator){
	psymbol_t psym = (psymbol_t)malloc(sizeof(symbol_t));
	if(psym == NULL){
		print_error("symbol_new: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}

	psym->declarator = declarator;
	return psym;
}

void symbol_del(psymbol_t psym){
	free(psym);
}

/*SYMBOL LIST*/

/*
@note Allocs a symlist_t
*/
psymlist_t symlist_new(){
	psymlist_t psl = (psymlist_t)malloc(sizeof(symlist_t));
	if(psl == NULL){
		print_error("symlist_new: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}

	psl->psym = NULL;
	psl->psl_elem_next = NULL;
	return psl;
}

/*
@note Frees all the symbols as well.
*/
void symlist_del(psymlist_t psl){
	psymlist_t psl_elem = psl;
	psymlist_t psl_elem_prev = NULL;
	while(psl_elem != NULL){
		free(psl_elem->psym);
		psl_elem_prev = psl_elem;
		psl_elem = psl_elem->psl_elem_next;
		free(psl_elem_prev);
	}
}

/*
@note psym needs to be allocated with symbol_new first.
*/
void symlist_add_sym(psymlist_t psl, psymbol_t psym){
	if(psl->psym == NULL){//first symbol
		psl->psym = psym;
		return;
	}

	psymlist_t psl_elem = psl;
	while(psl_elem->psl_elem_next != NULL){
		psl_elem = psl_elem->psl_elem_next;
	}

	psl_elem->psl_elem_next = (psymlist_t)malloc(sizeof(symlist_t));
	if(psl_elem->psl_elem_next == NULL){
		print_error("symlist_add_sym: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}
	psl_elem->psl_elem_next->psym = psym;

}

/*SCOPES*/

/*
@note Makes it's own copy of "name"
*/
pscope_t scope_new(int type, char* name){
	pscope_t pscope = (pscope_t)malloc(sizeof(scope_t));

	print_debug("scope_new: %s\n", name);

	if(pscope == NULL){
		print_error("scope_new: Could not allocate space!\n");
		exit(1);
	}
	pscope->type = type;
	strncpy(pscope->name, name, SCOPE_NAME_LEN);
	pscope->psyms = symlist_new();
	return pscope;
};

/*
@note Calls symlist_del.
*/
void scope_del(pscope_t pscope){
	symlist_del(pscope->psyms);
	free(pscope);
}

void scope_add_sym(pscope_t pscope, psymbol_t psym){
	symlist_add_sym(pscope->psyms, psym);
}

/*SCOPE STACK*/
pscope_stack_t glob_pscope_stack = NULL;

void scope_stack_push(pscope_t pscope){
	pscope_stack_t pss_up = glob_pscope_stack;

	print_debug("scope_stack_push: %s\n", pscope->name);

	glob_pscope_stack = (pscope_stack_t)malloc(sizeof(scope_stack_t));
	if(glob_pscope_stack == NULL){
		print_error("scope_stack_push: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}
	glob_pscope_stack->pss_up = pss_up;
	glob_pscope_stack->pscope = pscope;
}

pscope_t scope_stack_pull(){
	pscope_t pscope_top = glob_pscope_stack->pscope;
	pscope_stack_t pss_up = glob_pscope_stack->pss_up;

	print_debug("scope_stack_pull: %s\n", pscope_top->name);

	free(glob_pscope_stack);
	glob_pscope_stack = pss_up;

	return pscope_top;
}
int check_semantics(pnode_t pnd_protocol){
    return check_protocol(pnd_protocol);
}

int check_protocol(pnode_t pnd_protocol){
	int check = CHECK_SUCCESS;
	/*Retrieve node elements*/
	pnode_t pnd_attributes = (pnode_t)pnd_protocol->children[0];
	char* identifier = (char*)pnd_protocol->children[1];
	pnode_t pnd_protocol_body = (pnode_t)pnd_protocol->children[2];

	/*Create scope*/
	pscope_t pscope = scope_new(SCOPE_PROTOCOL, identifier);

	/*Push to scope stack*/
	scope_stack_push(pscope);

	/*Check attributes*/
	check &= check_attributes(pnd_attributes);
	/*Check protocol body*/
	check &= check_protocol_body(pnd_protocol_body);

	return check;
}

/*ATTRIBUTES*/
#define ARGS_TYPE_NUM 512
struct attribute{
	char* identifier;
	unsigned int args_num;
	char* args_type[ARGS_TYPE_NUM];
};
struct attribute attributes[] = {
	{"byte_order", 1, {"string"}},
	{NULL, 0},
};

int check_attributes(pnode_t pnd_attributes){
	int check = CHECK_SUCCESS;
	for(int i=0; i<pnd_attributes->num_children; i++){
		pnode_t pnd_attribute = pnd_attributes->children[i];
		check &= check_attribute(pnd_attribute);
	}
	return check;
}

int check_attribute(pnode_t pnd_attribute){
	int check = CHECK_SUCCESS;
	return check;
}


int check_protocol_body(pnode_t pnd_protocol_body){
	int check = CHECK_SUCCESS;
	return check;
}
