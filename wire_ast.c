#include "wire_ast.h"
#include "wire.tab.h" //we need tokens
#include "wire_utils.h"
#include "wire_semantics.h"
#include <stdlib.h>

pnode_t new_node(enum nodes type){
	pnode_t pnode = (pnode_t)malloc(sizeof(node_t));
	if(pnode == NULL){
		print_error("new_node: Could not allocate space!\n");
		exit(1);
	}
	pnode->type = type;
	pnode->num_children = 0;
	return pnode;
}

pnode_t add_child(pnode_t pnode_parent, void* pchild){
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
    Protocol
*/

pnode_t new_protocol(pnode_t attributes, char* tidentifier, pnode_t protocol_body){
	/*Alloc a node*/
	pnode_t pnode_protocol = new_node(PROTOCOL);

	/*Add children*/
	add_child(pnode_protocol, attributes);
	add_child(pnode_protocol, tidentifier);
	add_child(pnode_protocol, protocol_body);

    print_debug("new_protocol\n");

	return pnode_protocol;
}

pnode_t new_protocol_components(pnode_t protocol_component){
	/*Alloc a node*/
	pnode_t pnode_protocol_components = new_node(PROTOCOL_COMPONENTS);

	/*Add children*/
	add_child(pnode_protocol_components, protocol_component);

    print_debug("new_protocol_components\n");

	return pnode_protocol_components;
}

/*
    Attributes
*/

pnode_t new_attributes(pnode_t attribute){
	/*Alloc a node*/
	pnode_t pnode_attributes = new_node(ATTRIBUTES);

	/*Add children*/
	add_child(pnode_attributes, attribute);

    print_debug("new_attributes\n");

	return pnode_attributes;
}

pnode_t new_attribute(char* tidentifier, pnode_t attribute_arguments){
	/*Alloc a node*/
	pnode_t pnode_attribute = new_node(ATTRIBUTE);

	/*Add children*/
	add_child(pnode_attribute, tidentifier);
	add_child(pnode_attribute, attribute_arguments);

    print_debug("new_attribute\n");

	return pnode_attribute;
}

pnode_t new_attribute_arguments(pnode_t attribute_argument){
	/*Alloc a node*/
	pnode_t pnode_attribute_arguments = new_node(ATTRIBUTE_ARGUMENTS);

	/*Add children*/
	add_child(pnode_attribute_arguments, attribute_argument);

    print_debug("new_attribute_arguments\n");

	return pnode_attribute_arguments;
}

pnode_t new_attribute_argument(pnode_t const_exp){
	/*Alloc a node*/
	pnode_t pnode_attribute_argument = new_node(ATTRIBUTE_ARGUMENT);

	/*Add children*/
	add_child(pnode_attribute_argument, const_exp);

    print_debug("new_attribute_argument\n");

	return pnode_attribute_argument;
}

/*
    Expressions
*/

pnode_t new_double_const_exp(char* trealconst){
	/*Alloc a node*/
	pnode_t pnode_double_const_exp = new_node(DOUBLE_CONST_EXP);

	/*Add children*/
	add_child(pnode_double_const_exp, trealconst);

    print_debug("new_double_const_exp\n");

	return pnode_double_const_exp;
}

pnode_t new_string_const_exp(char* tstringconst){
	/*Alloc a node*/
	pnode_t pnode_string_const_exp = new_node(STRING_CONST_EXP);

	/*Add children*/
	add_child(pnode_string_const_exp, tstringconst);

    print_debug("new_string_const_exp\n");

	return pnode_string_const_exp;
}

pnode_t new_integer_const_exp(char* tintconst){
	/*Alloc a node*/
	pnode_t pnode_integer_const_exp = new_node(INTEGER_CONST_EXP);

	/*Add children*/
	add_child(pnode_integer_const_exp, tintconst);

    print_debug("new_integer_const_exp\n");

	return pnode_integer_const_exp;
}

pnode_t new_struct_const_exp(pnode_t struct_const_exp_body){
	/*Alloc a node*/
	pnode_t pnode_struct_const_exp = new_node(STRUCT_CONST_EXP);

	/*Add children*/
	add_child(pnode_struct_const_exp, struct_const_exp_body);

    print_debug("new_struct_const_exp\n");

	return pnode_struct_const_exp;
}

pnode_t new_struct_const_exp_body(pnode_t struct_const_exp_body_component){
	/*Alloc a node*/
	pnode_t pnode_struct_const_exp_body = new_node(STRUCT_CONST_EXP_BODY);

	/*Add children*/
	add_child(pnode_struct_const_exp_body, struct_const_exp_body_component);

    print_debug("new_struct_const_exp_body\n");

	return pnode_struct_const_exp_body;
}

pnode_t new_struct_const_exp_body_component(char* tidentifier, pnode_t const_exp){
	/*Alloc a node*/
	pnode_t pnode_struct_const_exp_body_component =
	    new_node(STRUCT_CONST_EXP_BODY_COMPONENT);

	/*Add children*/
	add_child(pnode_struct_const_exp_body_component, tidentifier);
	add_child(pnode_struct_const_exp_body_component, const_exp);

    print_debug("new_struct_const_exp_body_component\n");

	return pnode_struct_const_exp_body_component;
}

pnode_t new_arithmetic_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri){
	/*Alloc a node*/
	pnode_t pnode_arithmetic_exp = NULL;
	switch(op){
	    case '+':
	    	pnode_arithmetic_exp = new_node(ARITHMETIC_EXP_ADD);
	    case '-':
   	    	pnode_arithmetic_exp = new_node(ARITHMETIC_EXP_SUB);
  	    case '*':
	    	pnode_arithmetic_exp = new_node(ARITHMETIC_EXP_MUL);
        case '/':
	    	pnode_arithmetic_exp = new_node(ARITHMETIC_EXP_DIV);
	    case '%':
	    	pnode_arithmetic_exp = new_node(ARITHMETIC_EXP_MOD);
	}

	/*Add children*/
	add_child(pnode_arithmetic_exp, const_exp_le);
	add_child(pnode_arithmetic_exp, const_exp_ri);

    print_debug("new_arithmetic_exp\n");

	return pnode_arithmetic_exp;
}

pnode_t new_relational_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri){
	/*Alloc a node*/
	pnode_t pnode_relational_exp = NULL;
	switch(op){
	    case '>':
	    	pnode_relational_exp = new_node(RELATIONAL_EXP_G);
	    case '<':
   	    	pnode_relational_exp = new_node(RELATIONAL_EXP_L);
  	    case tRELEQU:
	    	pnode_relational_exp = new_node(RELATIONAL_EXP_EQU);
        case tRELNEQU:
	    	pnode_relational_exp = new_node(RELATIONAL_EXP_NEQU);
	    case tRELGE:
	    	pnode_relational_exp = new_node(RELATIONAL_EXP_GE);
	    case tRELLE:
	    	pnode_relational_exp = new_node(RELATIONAL_EXP_LE);
	}

	/*Add children*/
	add_child(pnode_relational_exp, const_exp_le);
	add_child(pnode_relational_exp, const_exp_ri);

    print_debug("new_relational_exp\n");

	return pnode_relational_exp;
}

pnode_t new_logical_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri){
	/*Alloc a node*/
	pnode_t pnode_logical_exp = NULL;
	switch(op){
	    case '!':
	    	pnode_logical_exp = new_node(LOGICAL_EXP_NOT);
	    case tLOGAND:
   	    	pnode_logical_exp = new_node(LOGICAL_EXP_AND);
  	    case tLOGOR:
	    	pnode_logical_exp = new_node(LOGICAL_EXP_OR);
 	}

	/*Add children*/
	add_child(pnode_logical_exp, const_exp_le);
	add_child(pnode_logical_exp, const_exp_ri);

    print_debug("new_logical_exp\n");

	return pnode_logical_exp;
}

pnode_t new_bitwise_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri){
	/*Alloc a node*/
	pnode_t pnode_bitwise_exp = NULL;
	switch(op){
	    case '~':
	    	pnode_bitwise_exp = new_node(BITWISE_EXP_NOT);
	    case '&':
   	    	pnode_bitwise_exp = new_node(BITWISE_EXP_AND);
  	    case '|':
	    	pnode_bitwise_exp = new_node(BITWISE_EXP_OR);
        case '^':
	    	pnode_bitwise_exp = new_node(BITWISE_EXP_XOR);
	    case tBITSR:
	    	pnode_bitwise_exp = new_node(BITWISE_EXP_SR);
	    case tBITSL:
	    	pnode_bitwise_exp = new_node(BITWISE_EXP_SL);
	}

	/*Add children*/
	add_child(pnode_bitwise_exp, const_exp_le);
	add_child(pnode_bitwise_exp, const_exp_ri);

    print_debug("new_bitwise_exp\n");

	return pnode_bitwise_exp;
}

pnode_t new_identifier(char* tidentifier){
	/*Alloc a node*/
	pnode_t pnode_identifier = new_node(IDENTIFIER);

	/*Add children*/
	add_child(pnode_identifier, tidentifier);

    print_debug("new_identifier\n");

	return pnode_identifier;
}

/*
    Declarators
*/

pnode_t new_primitive_type_declarator(
    pnode_t attributes_opt,
    pnode_t primitive_type,
    char* tidentifier,
    pnode_t array_declarator_opt)
    {
	/*Alloc a node*/
	pnode_t pnode_primitive_type_declarator = new_node(PRIMITIVE_TYPE_DECLARATOR);

	/*Add children*/
	add_child(pnode_primitive_type_declarator, attributes_opt);
	add_child(pnode_primitive_type_declarator, primitive_type);
	add_child(pnode_primitive_type_declarator, tidentifier);
	add_child(pnode_primitive_type_declarator, array_declarator_opt);

    print_debug("new_primitive_type_declarator\n");

	return pnode_primitive_type_declarator;
}

pnode_t new_local_union_declarator(
    pnode_t attributes_opt,
    pnode_t const_exp,
    pnode_t union_body,
    char* tidentifier,
    pnode_t array_declarator_opt)
    {
	/*Alloc a node*/
	pnode_t pnode_local_union_declarator = new_node(LOCAL_UNION_DECLARATOR);

	/*Add children*/
	add_child(pnode_local_union_declarator, attributes_opt);
	add_child(pnode_local_union_declarator, const_exp);
	add_child(pnode_local_union_declarator, union_body);
	add_child(pnode_local_union_declarator, tidentifier);
	add_child(pnode_local_union_declarator, array_declarator_opt);

    print_debug("new_local_union_declarator\n");

	return pnode_local_union_declarator;
}

pnode_t new_local_declarator_list(pnode_t local_declarator){
	/*Alloc a node*/
	pnode_t pnode_local_declarator_list = new_node(LOCAL_DECLARATOR_LIST);

	/*Add children*/
	add_child(pnode_local_declarator_list, local_declarator);

    print_debug("new_local_declarator_list\n");

	return pnode_local_declarator_list;
}

pnode_t new_local_enum_declarator_anon(
    pnode_t attributes_opt,
    pnode_t enum_body,
    char* tidentifier,
    pnode_t array_declarator_opt)
    {
	/*Alloc a node*/
	pnode_t pnode_local_enum_declarator_anon = new_node(LOCAL_ENUM_DECLARATOR_ANON);

	/*Add children*/
	add_child(pnode_local_enum_declarator_anon, attributes_opt);
	add_child(pnode_local_enum_declarator_anon, enum_body);
	add_child(pnode_local_enum_declarator_anon, tidentifier);
	add_child(pnode_local_enum_declarator_anon, array_declarator_opt);

    print_debug("new_local_enum_declarator_anon\n");

	return pnode_local_enum_declarator_anon;
}

pnode_t new_local_enum_declarator_named(
    pnode_t attributes_opt,
    char* tidentifier_name,
    char* tidentifier,
    pnode_t array_declarator_opt)
    {
	/*Alloc a node*/
	pnode_t pnode_local_enum_declarator_named = new_node(LOCAL_ENUM_DECLARATOR_NAMED);

	/*Add children*/
	add_child(pnode_local_enum_declarator_named, attributes_opt);
	add_child(pnode_local_enum_declarator_named, tidentifier_name);
	add_child(pnode_local_enum_declarator_named, tidentifier);
	add_child(pnode_local_enum_declarator_named, array_declarator_opt);

    print_debug("new_local_enum_declarator_named\n");

	return pnode_local_enum_declarator_named;
}

pnode_t new_global_enum_declarator(
    pnode_t attributes_opt,
    char* tidentifier,
    pnode_t enum_body)
    {
	/*Alloc a node*/
	pnode_t pnode_global_enum_declarator = new_node(GLOBAL_ENUM_DECLARATOR);

	/*Add children*/
	add_child(pnode_global_enum_declarator, attributes_opt);
	add_child(pnode_global_enum_declarator, tidentifier);
	add_child(pnode_global_enum_declarator, enum_body);

    print_debug("new_global_enum_declarator\n");

	return pnode_global_enum_declarator;
}

pnode_t new_typedef_declarator(pnode_t local_declarator){
	/*Alloc a node*/
	pnode_t pnode_typedef_declarator = new_node(TYPEDEF_DECLARATOR);

	/*Add children*/
	add_child(pnode_typedef_declarator, local_declarator);

    print_debug("new_typedef_declarator\n");

	return pnode_typedef_declarator;
}

pnode_t new_local_struct_declarator_anon(
    pnode_t attributes_opt,
    pnode_t struct_body,
    char* tidentifier,
    pnode_t array_declarator_opt)
    {
	/*Alloc a node*/
	pnode_t pnode_local_struct_declarator_anon =
	    new_node(LOCAL_STRUCT_DECLARATOR_ANON);

	/*Add children*/
	add_child(pnode_local_struct_declarator_anon, attributes_opt);
	add_child(pnode_local_struct_declarator_anon, struct_body);
	add_child(pnode_local_struct_declarator_anon, tidentifier);
	add_child(pnode_local_struct_declarator_anon, array_declarator_opt);

    print_debug("new_local_struct_declarator_anon\n");

	return pnode_local_struct_declarator_anon;
}

pnode_t new_local_struct_declarator_named(
    pnode_t attributes_opt,
    char* tidentifier_name,
    char* tidentifier,
    pnode_t array_declarator_opt)
    {
	/*Alloc a node*/
	pnode_t pnode_local_struct_declarator_named =
	    new_node(LOCAL_STRUCT_DECLARATOR_NAMED);

	/*Add children*/
	add_child(pnode_local_struct_declarator_named, attributes_opt);
	add_child(pnode_local_struct_declarator_named, tidentifier_name);
	add_child(pnode_local_struct_declarator_named, tidentifier);
	add_child(pnode_local_struct_declarator_named, array_declarator_opt);

    print_debug("new_local_struct_declarator_named\n");

	return pnode_local_struct_declarator_named;
}

pnode_t new_global_struct_declarator(
    pnode_t attributes_opt,
    char* tidentifier,
    pnode_t struct_body)
    {
	/*Alloc a node*/
	pnode_t pnode_global_struct_declarator = new_node(GLOBAL_STRUCT_DECLARATOR);

	/*Add children*/
	add_child(pnode_global_struct_declarator, attributes_opt);
	add_child(pnode_global_struct_declarator, tidentifier);
	add_child(pnode_global_struct_declarator, struct_body);

    print_debug("new_global_struct_declarator\n");

	return pnode_global_struct_declarator;
}

pnode_t new_packet_declarator(
    pnode_t attributes_opt,
    char* tidentifier,
    pnode_t packet_body)
    {
	/*Alloc a node*/
	pnode_t pnode_packet_declarator = new_node(PACKET_DECLARATOR);

	/*Add children*/
	add_child(pnode_packet_declarator, attributes_opt);
	add_child(pnode_packet_declarator, tidentifier);
	add_child(pnode_packet_declarator, packet_body);

    print_debug("new_packet_declarator\n");

	return pnode_packet_declarator;
}

/*
    Union
*/

pnode_t new_union_body(pnode_t union_body_component){
	/*Alloc a node*/
	pnode_t pnode_union_body = new_node(UNION_BODY);

	/*Add children*/
	add_child(pnode_union_body, union_body_component);

    print_debug("new_union_body\n");

	return pnode_union_body;
}

pnode_t new_union_body_component(pnode_t const_exp, pnode_t local_declarator_list){
	/*Alloc a node*/
	pnode_t pnode_union_body_component = new_node(UNION_BODY_COMPONENT);

	/*Add children*/
	add_child(pnode_union_body_component, const_exp);
	add_child(pnode_union_body_component, local_declarator_list);

    print_debug("new_union_body_component\n");

	return pnode_union_body_component;
}

/*
    Enums
*/

pnode_t new_enum_body(pnode_t enum_body_component){
	/*Alloc a node*/
	pnode_t pnode_enum_body = new_node(ENUM_BODY);

	/*Add children*/
	add_child(pnode_enum_body, enum_body_component);

    print_debug("new_enum_body\n");

	return pnode_enum_body;
}

pnode_t new_enum_body_component(char* tidentifier, pnode_t const_exp){
	/*Alloc a node*/
	pnode_t pnode_enum_body_component = new_node(ENUM_BODY_COMPONENT);

	/*Add children*/
	add_child(pnode_enum_body_component, tidentifier);
	add_child(pnode_enum_body_component, const_exp);

    print_debug("new_enum_body_component\n");

	return pnode_enum_body_component;
}

/*
    Arrays
*/

pnode_t new_array_declarator_opt(pnode_t const_exp){
	/*Alloc a node*/
	pnode_t pnode_array_declarator_opt = new_node(ARRAY_DECLARATOR_OPT);

	/*Add children*/
	add_child(pnode_array_declarator_opt, const_exp);

    print_debug("new_array_declarator_opt\n");

	return pnode_array_declarator_opt;
}

/*
    Types
*/

pnode_t new_primitive_type(unsigned int type){
	/*Alloc a node*/
	pnode_t pnode_primitive_type = NULL;
	switch(type){
	    case tBIT:
        	pnode_primitive_type = new_node(PRIMITIVE_TYPE_BIT);
	    case tBYTE:
        	pnode_primitive_type = new_node(PRIMITIVE_TYPE_BYTE);
	    case tSTRING:
        	pnode_primitive_type = new_node(PRIMITIVE_TYPE_STRING);
	    case tDOUBLE:
        	pnode_primitive_type = new_node(PRIMITIVE_TYPE_DOUBLE);
	    case tUNSIGNED:
        	pnode_primitive_type = new_node(PRIMITIVE_TYPE_UNSIGNED);
	    case tSIGNED:
        	pnode_primitive_type = new_node(PRIMITIVE_TYPE_SIGNED);
    }

    print_debug("new_primitive_type\n");

	return pnode_primitive_type;
}

/*
    Structures
*/

pnode_t new_struct_body(pnode_t struct_body_component){
	/*Alloc a node*/
	pnode_t pnode_struct_body = new_node(STRUCT_BODY);

	/*Add children*/
	add_child(pnode_struct_body, struct_body_component);

    print_debug("new_struct_body\n");

	return pnode_struct_body;
}

pnode_t new_struct_body_component(pnode_t local_declarator){
	/*Alloc a node*/
	pnode_t pnode_struct_body_component = new_node(STRUCT_BODY_COMPONENT);

	/*Add children*/
	add_child(pnode_struct_body_component, local_declarator);

    print_debug("new_struct_body_component\n");

	return pnode_struct_body_component;
}

/*
    Packets
*/

pnode_t new_packet_body(pnode_t packet_body_component){
	/*Alloc a node*/
	pnode_t pnode_packet_body = new_node(PACKET_BODY);

	/*Add children*/
	add_child(pnode_packet_body, packet_body_component);

    print_debug("new_packet_body\n");

	return pnode_packet_body;
}

pnode_t new_packet_body_component(pnode_t local_declarator){
	/*Alloc a node*/
	pnode_t pnode_packet_body_component = new_node(PACKET_BODY_COMPONENT);

	/*Add children*/
	add_child(pnode_packet_body_component, local_declarator);

    print_debug("new_packet_body_component\n");

	return pnode_packet_body_component;
}

