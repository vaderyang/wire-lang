#include "wire_ast.h"
#include "wire.tab.h" //we need tokens
#include "wire_utils.h"
#include "wire_semantics.h"
#include <stdlib.h>

pnode_t node_new(enum nodes type){
	pnode_t pnode = (pnode_t)malloc(sizeof(node_t));
	if(pnode == NULL){
		print_error("node_new: Could not allocate space!\n");
		exit(1);
	}
	pnode->type = type;
	pnode->num_children = 0;
	return pnode;
}

pnode_t node_add_child(pnode_t pnode_parent, void* pchild){
	pnode_parent->children[pnode_parent->num_children++] = pchild;
	return pnode_parent;
}


int generate_code(pnode_t protocol){
	return 0;
}

pnode_t new_wire(pnode_t protocol){
    check_semantics(protocol);
	generate_code(protocol);
	return NULL;
}

/*
* PROTOCOL
*/
pnode_t new_protocol(pnode_t attribute_list_opt, char* tidentifier, pnode_t protocol_body){
	/*Alloc a node*/
	pnode_t pnode_protocol = node_new(PROTOCOL);

	/*Add children*/
	node_add_child(pnode_protocol, attribute_list_opt);
	node_add_child(pnode_protocol, tidentifier);
	node_add_child(pnode_protocol, protocol_body);

    print_debug("new_protocol\n");

	return pnode_protocol;
}

pnode_t new_protocol_body(pnode_t protocol_body_component){
	/*Alloc a node*/
	pnode_t pnode_protocol_body = node_new(PROTOCOL_BODY);

	/*Add children*/
	node_add_child(pnode_protocol_body, protocol_body_component);

    print_debug("new_protocol_body\n");

	return pnode_protocol_body;
}

/*
* attribute_list
*/
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

/*
* TYPE DEFINITIONS
*/

/*ENUM*/
pnode_t new_enum_definition(
	pnode_t attribute_list_opt, char* tidentifier, pnode_t enum_body)
	{
	/*Alloc a node*/
	pnode_t pnode_enum_definition = node_new(ENUM_DEFINITION);

	/*Add children*/
	node_add_child(pnode_enum_definition, attribute_list_opt);
	node_add_child(pnode_enum_definition, tidentifier);
	node_add_child(pnode_enum_definition, enum_body);

    print_debug("new_enum_definition\n");

	return pnode_enum_definition;	
}

pnode_t new_enum_body(pnode_t enum_body_component){
	/*Alloc a node*/
	pnode_t pnode_enum_body = node_new(ENUM_BODY);

	/*Add children*/
	node_add_child(pnode_enum_body, enum_body_component);

    print_debug("new_enum_body\n");

	return pnode_enum_body;	
}

pnode_t new_enum_body_component(char* tidentifier, pnode_t const_exp){
	/*Alloc a node*/
	pnode_t pnode_enum_body_component = node_new(ENUM_BODY_COMPONENT);

	/*Add children*/
	node_add_child(pnode_enum_body_component, tidentifier);
	node_add_child(pnode_enum_body_component, const_exp);

    print_debug("new_enum_body_component\n");

	return pnode_enum_body_component;	
}

/*UNION*/
pnode_t new_union_definition(
	pnode_t attribute_list_opt, char* tidentifier, pnode_t union_body)
	{
	/*Alloc a node*/
	pnode_t pnode_union_definition = node_new(UNION_DEFINITION);

	/*Add children*/
	node_add_child(pnode_union_definition, attribute_list_opt);
	node_add_child(pnode_union_definition, tidentifier);
	node_add_child(pnode_union_definition, union_body);
	
    print_debug("new_union_definition\n");

	return pnode_union_definition;	
}

pnode_t new_union_body(pnode_t union_body_component){
	/*Alloc a node*/
	pnode_t pnode_union_body = node_new(UNION_BODY);

	/*Add children*/
	node_add_child(pnode_union_body, union_body_component);

    print_debug("new_union_body\n");

	return pnode_union_body;	
}

pnode_t new_union_body_component(
	pnode_t const_exp, pnode_t local_declarator_list)
	{
	/*Alloc a node*/
	pnode_t pnode_union_body_component = node_new(UNION_BODY_COMPONENT);

	/*Add children*/
	node_add_child(pnode_union_body_component, const_exp);
	node_add_child(pnode_union_body_component, local_declarator_list);
	
    print_debug("new_union_body_component\n");

	return pnode_union_body_component;	
}

/*STRUCT*/
pnode_t new_struct_definition(
	pnode_t attribute_list_opt, char* tidentifier, pnode_t struct_body)
	{
	/*Alloc a node*/
	pnode_t pnode_struct_definition = node_new(STRUCT_DEFINITION);

	/*Add children*/
	node_add_child(pnode_struct_definition, attribute_list_opt);
	node_add_child(pnode_struct_definition, tidentifier);
	node_add_child(pnode_struct_definition, struct_body);
	
    print_debug("new_struct_definition\n");

	return pnode_struct_definition;	
}

pnode_t new_struct_body(pnode_t struct_body_component){
	/*Alloc a node*/
	pnode_t pnode_struct_body = node_new(STRUCT_BODY);

	/*Add children*/
	node_add_child(pnode_struct_body, struct_body_component);

    print_debug("new_struct_body\n");

	return pnode_struct_body;	
}

/*PDU*/
pnode_t new_pdu_definition(
	pnode_t attribute_list_opt, char* tidentifier, pnode_t pdu_body)
	{
	/*Alloc a node*/
	pnode_t pnode_pdu_definition = node_new(PDU_DEFINITION);

	/*Add children*/
	node_add_child(pnode_pdu_definition, attribute_list_opt);
	node_add_child(pnode_pdu_definition, tidentifier);
	node_add_child(pnode_pdu_definition, pdu_body);
	
    print_debug("new_pdu_definition\n");

	return pnode_pdu_definition;	
}

pnode_t new_pdu_body(pnode_t pdu_body_component){
	/*Alloc a node*/
	pnode_t pnode_pdu_body = node_new(PDU_BODY);

	/*Add children*/
	node_add_child(pnode_pdu_body, pdu_body_component);

    print_debug("new_pdu_body\n");

	return pnode_pdu_body;	
}

/*
* LOCAL DECLARATOR
*/
pnode_t new_local_declarator_list(pnode_t local_declarator){
	/*Alloc a node*/
	pnode_t pnode_local_declarator_list = node_new(LOCAL_DECLARATOR_LIST);

	/*Add children*/
	node_add_child(pnode_local_declarator_list, local_declarator);

    print_debug("new_local_declarator_list\n");

	return pnode_local_declarator_list;	
}

pnode_t new_primitive_local_declarator(
	pnode_t attribute_list_opt, char* tdatatype, 
	char* tidentifier, pnode_t array_declarator_opt)
	{
	/*Alloc a node*/
	pnode_t pnode_primitive_local_declarator = 
		node_new(PRIMITIVE_LOCAL_DECLARATOR);

	/*Add children*/
	node_add_child(pnode_primitive_local_declarator, attribute_list_opt);
	node_add_child(pnode_primitive_local_declarator, tdatatype);//FIXME
	node_add_child(pnode_primitive_local_declarator, tidentifier);
	node_add_child(pnode_primitive_local_declarator, array_declarator_opt);

    print_debug("new_primitive_local_declarator\n");

	return pnode_primitive_local_declarator;	
}

pnode_t new_constructed_local_declarator(
	pnode_t attribute_list_opt, char* tdatatype, char* tidentifier_type,
	char* tidentifier, pnode_t array_declarator_opt)
	{
	/*Alloc a node*/
	pnode_t pnode_constructed_local_declarator = 
		node_new(CONSTRUCTED_LOCAL_DECLARATOR);

	/*Add children*/
	node_add_child(pnode_constructed_local_declarator, attribute_list_opt);
	node_add_child(pnode_constructed_local_declarator, tdatatype);//FIXME
	node_add_child(pnode_constructed_local_declarator, tidentifier_type);
	node_add_child(pnode_constructed_local_declarator, tidentifier);
	node_add_child(pnode_constructed_local_declarator, array_declarator_opt);

    print_debug("new_constructed_local_declarator\n");

	return pnode_constructed_local_declarator;	
}

pnode_t new_anon_local_declarator(
	pnode_t attribute_list_opt,	pnode_t const_exp, pnode_t union_body){
	/*Alloc a node*/
	pnode_t pnode_anon_local_declarator = node_new(ANON_LOCAL_DECLARATOR);

	/*Add children*/
	node_add_child(pnode_anon_local_declarator, attribute_list_opt);
	node_add_child(pnode_anon_local_declarator, const_exp);
	node_add_child(pnode_anon_local_declarator, union_body);

    print_debug("new_anon_local_declarator\n");

	return pnode_anon_local_declarator;	
}

/*ARRAY DECLARATOR*/
pnode_t new_array_declarator_opt(pnode_t const_exp){
	/*Alloc a node*/
	pnode_t pnode_array_declarator_opt = node_new(ARRAY_DECLARATOR_OPT);

	/*Add children*/
	node_add_child(pnode_array_declarator_opt, const_exp);

    print_debug("new_array_declarator_opt\n");

	return pnode_array_declarator_opt;
}

/*
* OPERATIONS
*/
pnode_t new_operation_declarator(
	pnode_t attribute_list_opt, char* tidentifier, pnode_t operation_arg_list){
	/*Alloc a node*/
	pnode_t pnode_operation_declarator = node_new(OPERATION_DECLARATOR);

	/*Add children*/
	node_add_child(pnode_operation_declarator, attribute_list_opt);
	node_add_child(pnode_operation_declarator, tidentifier);
	node_add_child(pnode_operation_declarator, operation_arg_list);

    print_debug("new_operation_declarator\n");

	return pnode_operation_declarator;
}

pnode_t new_operation_arg_list(pnode_t operation_arg){
	/*Alloc a node*/
	pnode_t pnode_operation_arg_list = node_new(OPERATION_ARG_LIST);

	/*Add children*/
	node_add_child(pnode_operation_arg_list, operation_arg);

    print_debug("new_operation_arg_list\n");

	return pnode_operation_arg_list;
}

pnode_t new_operation_arg(
	pnode_t attribute_list, char* tidentifier_pdu, char* tidentifier){
	/*Alloc a node*/
	pnode_t pnode_operation_arg = node_new(OPERATION_ARG);

	/*Add children*/
	node_add_child(pnode_operation_arg, attribute_list);
	node_add_child(pnode_operation_arg, tidentifier_pdu);
	node_add_child(pnode_operation_arg, tidentifier);

    print_debug("new_operation_arg\n");

	return pnode_operation_arg;
}

/*
* EXPRESSIONS
*/
pnode_t new_float_const_exp(char* tfloatconst){
	/*Alloc a node*/
	pnode_t pnode_float_const_exp = node_new(FLOAT_CONST_EXP);

	/*Add children*/
	node_add_child(pnode_float_const_exp, tfloatconst);

    print_debug("new_float_const_exp\n");

	return pnode_float_const_exp;
}

pnode_t new_string_const_exp(char* tstringconst){
	/*Alloc a node*/
	pnode_t pnode_string_const_exp = node_new(STRING_CONST_EXP);

	/*Add children*/
	node_add_child(pnode_string_const_exp, tstringconst);

    print_debug("new_string_const_exp\n");

	return pnode_string_const_exp;
}

pnode_t new_integer_const_exp(char* tintconst){
	/*Alloc a node*/
	pnode_t pnode_integer_const_exp = node_new(INTEGER_CONST_EXP);

	/*Add children*/
	node_add_child(pnode_integer_const_exp, tintconst);

    print_debug("new_integer_const_exp\n");

	return pnode_integer_const_exp;
}

pnode_t new_arithmetic_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri){
	/*Alloc a node*/
	pnode_t pnode_arithmetic_exp = NULL;
	switch(op){
	    case '+':
	    	pnode_arithmetic_exp = node_new(ARITHMETIC_EXP_ADD);
	    case '-':
   	    	pnode_arithmetic_exp = node_new(ARITHMETIC_EXP_SUB);
  	    case '*':
	    	pnode_arithmetic_exp = node_new(ARITHMETIC_EXP_MUL);
        case '/':
	    	pnode_arithmetic_exp = node_new(ARITHMETIC_EXP_DIV);
	    case '%':
	    	pnode_arithmetic_exp = node_new(ARITHMETIC_EXP_MOD);
	}

	/*Add children*/
	node_add_child(pnode_arithmetic_exp, const_exp_le);
	node_add_child(pnode_arithmetic_exp, const_exp_ri);

    print_debug("new_arithmetic_exp\n");

	return pnode_arithmetic_exp;
}

pnode_t new_relational_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri){
	/*Alloc a node*/
	pnode_t pnode_relational_exp = NULL;
	switch(op){
	    case '>':
	    	pnode_relational_exp = node_new(RELATIONAL_EXP_G);
	    case '<':
   	    	pnode_relational_exp = node_new(RELATIONAL_EXP_L);
  	    case tRELEQU:
	    	pnode_relational_exp = node_new(RELATIONAL_EXP_EQU);
        case tRELNEQU:
	    	pnode_relational_exp = node_new(RELATIONAL_EXP_NEQU);
	    case tRELGE:
	    	pnode_relational_exp = node_new(RELATIONAL_EXP_GE);
	    case tRELLE:
	    	pnode_relational_exp = node_new(RELATIONAL_EXP_LE);
	}

	/*Add children*/
	node_add_child(pnode_relational_exp, const_exp_le);
	node_add_child(pnode_relational_exp, const_exp_ri);

    print_debug("new_relational_exp\n");

	return pnode_relational_exp;
}

pnode_t new_logical_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri){
	/*Alloc a node*/
	pnode_t pnode_logical_exp = NULL;
	switch(op){
	    case '!':
	    	pnode_logical_exp = node_new(LOGICAL_EXP_NOT);
	    case tLOGAND:
   	    	pnode_logical_exp = node_new(LOGICAL_EXP_AND);
  	    case tLOGOR:
	    	pnode_logical_exp = node_new(LOGICAL_EXP_OR);
 	}

	/*Add children*/
	node_add_child(pnode_logical_exp, const_exp_le);
	node_add_child(pnode_logical_exp, const_exp_ri);

    print_debug("new_logical_exp\n");

	return pnode_logical_exp;
}

pnode_t new_bitwise_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri){
	/*Alloc a node*/
	pnode_t pnode_bitwise_exp = NULL;
	switch(op){
	    case '~':
	    	pnode_bitwise_exp = node_new(BITWISE_EXP_NOT);
	    case '&':
   	    	pnode_bitwise_exp = node_new(BITWISE_EXP_AND);
  	    case '|':
	    	pnode_bitwise_exp = node_new(BITWISE_EXP_OR);
        case '^':
	    	pnode_bitwise_exp = node_new(BITWISE_EXP_XOR);
	    case tBITSR:
	    	pnode_bitwise_exp = node_new(BITWISE_EXP_SR);
	    case tBITSL:
	    	pnode_bitwise_exp = node_new(BITWISE_EXP_SL);
	}

	/*Add children*/
	node_add_child(pnode_bitwise_exp, const_exp_le);
	node_add_child(pnode_bitwise_exp, const_exp_ri);

    print_debug("new_bitwise_exp\n");

	return pnode_bitwise_exp;
}

pnode_t new_identifier(char* tidentifier){
	/*Alloc a node*/
	pnode_t pnode_identifier = node_new(IDENTIFIER);

	/*Add children*/
	node_add_child(pnode_identifier, tidentifier);

    print_debug("new_identifier\n");

	return pnode_identifier;
}
