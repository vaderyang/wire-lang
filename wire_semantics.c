#include "wire_semantics.h"
#include "wire_ast.h"
#include "wire_utils.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
*	CHECKS
*/
int check_semantics(pnode_t pnd_protocol){
    return check_protocol(pnd_protocol);
}

int check_protocol(pnode_t pnd_protocol){
	int check = CHECK_SUCCESS;
	/*Retrieve node elements*/
	pnode_t pnd_attributes = (pnode_t)pnd_protocol->children[0];
	char* identifier = (char*)pnd_protocol->children[1];
	pnode_t pnd_protocol_body = (pnode_t)pnd_protocol->children[2];

	/*Check attributes*/
	check &= check_attributes(pnd_attributes);
	/*Check protocol body*/
	check &= check_protocol_body(pnd_protocol_body);

	return check;
}


int check_protocol_body(pnode_t pnd_protocol_body){
	int check = CHECK_SUCCESS;
	return check;
}


/*ATTRIBUTES*/
pnode_t new_attribute_list(pnode_t attribute){
	/*Alloc a node*/
	pnode_t pnode_attribute_list = node_new(ATTRIBUTE_LIST);

	/*Add children*/
	node_add_child(pnode_attribute_list, attribute);

    print_debug("new_attribute_list\n");

	return pnode_attribute_list;
}

pnode_t new_attribute(char* tidentifier, pnode_t attribute_argument_list){
	/*Alloc a node*/
	pnode_t pnode_attribute = node_new(ATTRIBUTE);

	/*Add children*/
	node_add_child(pnode_attribute, tidentifier);
	node_add_child(pnode_attribute, attribute_argument_list);

    print_debug("new_attribute\n");

	return pnode_attribute;
}

pnode_t new_attribute_argument_list(pnode_t attribute_argument){
	/*Alloc a node*/
	pnode_t pnode_attribute_argument_list = node_new(ATTRIBUTE_ARGUMENT_LIST);

	/*Add children*/
	node_add_child(pnode_attribute_argument_list, attribute_argument);

    print_debug("new_attribute_argument_list\n");

	return pnode_attribute_argument_list;
}

pnode_t new_attribute_argument(pnode_t const_exp){
	/*Alloc a node*/
	pnode_t pnode_attribute_argument = node_new(ATTRIBUTE_ARGUMENT);

	/*Add children*/
	node_add_child(pnode_attribute_argument, const_exp);

    print_debug("new_attribute_argument\n");

	return pnode_attribute_argument;
}

