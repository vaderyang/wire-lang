#ifndef WIRE_AST
#define WIRE_AST

/*Abstract syntax tree creation*/

/*Nodes*/
enum nodes{
	PROTOCOL,
	PROTOCOL_BODY,
	
	ATTRIBUTE_LIST,
	ATTRIBUTE,
	ATTRIBUTE_ARGUMENT_LIST,
	ATTRIBUTE_ARGUMENT,
	
	ENUM_DEFINITION,
	ENUM_BODY,
	ENUM_BODY_COMPONENT,
	
	UNION_DEFINITION,
	UNION_BODY,
	UNION_BODY_COMPONENT,
	
	STRUCT_DEFINITION,
	STRUCT_BODY,
	
	PDU_DEFINITION,
	PDU_BODY,
	
	LOCAL_DECLARATOR_LIST,
	PRIMITIVE_LOCAL_DECLARATOR,
	CONSTRUCTED_LOCAL_DECLARATOR,
	ANON_LOCAL_DECLARATOR,
	
	ARRAY_DECLARATOR_OPT,
	
	OPERATION_DECLARATOR,
	OPERATION_ARG_LIST,
	OPERATION_ARG,
	
	FLOAT_CONST_EXP,
	STRING_CONST_EXP,
	INTEGER_CONST_EXP,
	ARITHMETIC_EXP_ADD,
	ARITHMETIC_EXP_SUB,
	ARITHMETIC_EXP_MUL,
	ARITHMETIC_EXP_DIV,
	ARITHMETIC_EXP_MOD,
	RELATIONAL_EXP_G,
	RELATIONAL_EXP_L,
	RELATIONAL_EXP_EQU,
	RELATIONAL_EXP_NEQU,
	RELATIONAL_EXP_GE,
	RELATIONAL_EXP_LE,
	LOGICAL_EXP_NOT,
	LOGICAL_EXP_AND,
	LOGICAL_EXP_OR,
	BITWISE_EXP_NOT,
	BITWISE_EXP_AND,
	BITWISE_EXP_OR,
	BITWISE_EXP_XOR,
	BITWISE_EXP_SR,
	BITWISE_EXP_SL,
	IDENTIFIER,
};

#define MAX_CHILDREN 10

struct node {
	enum nodes type;
	unsigned int num_children;
	void* children[MAX_CHILDREN];
};
typedef struct node node_t;
typedef node_t* pnode_t;

int generate_code(pnode_t protocol);

pnode_t node_new(enum nodes type);
pnode_t node_add_child(pnode_t pnode_parent, void *pchild);

pnode_t new_wire(pnode_t protocol);
pnode_t new_protocol(pnode_t attribute_list_opt, char *tidentifier, pnode_t protocol_body);
pnode_t new_protocol_body(pnode_t protocol_body_component);
pnode_t new_attribute_list(pnode_t attribute);
pnode_t new_attribute(char *tidentifier, pnode_t attribute_argument_list);
pnode_t new_attribute_argument_list(pnode_t attribute_argument);
pnode_t new_attribute_argument(pnode_t const_exp);
pnode_t new_enum_definition(pnode_t attribute_list_opt, char *tidentifier, pnode_t enum_body);
pnode_t new_enum_body(pnode_t enum_body_component);
pnode_t new_enum_body_component(char *tidentifier, pnode_t const_exp);
pnode_t new_union_definition(pnode_t attribute_list_opt, char *tidentifier, pnode_t union_body);
pnode_t new_union_body(pnode_t union_body_component);
pnode_t new_union_body_component(pnode_t const_exp, pnode_t local_declarator_list);
pnode_t new_struct_definition(pnode_t attribute_list_opt, char *tidentifier, pnode_t struct_body);
pnode_t new_struct_body(pnode_t struct_body_component);
pnode_t new_pdu_definition(pnode_t attribute_list_opt, char *tidentifier, pnode_t pdu_body);
pnode_t new_pdu_body(pnode_t pdu_body_component);
pnode_t new_local_declarator_list(pnode_t local_declarator);
pnode_t new_primitive_local_declarator(pnode_t attribute_list_opt, char *tdatatype, char *tidentifier, pnode_t array_declarator_opt);
pnode_t new_constructed_local_declarator(pnode_t attribute_list_opt, char *tdatatype, char *tidentifier_type, char *tidentifier, pnode_t array_declarator_opt);
pnode_t new_anon_local_declarator(pnode_t attribute_list_opt, pnode_t const_exp, pnode_t union_body);
pnode_t new_array_declarator_opt(pnode_t const_exp);
pnode_t new_operation_declarator(pnode_t attribute_list_opt, char *tidentifier, pnode_t operation_arg_list);
pnode_t new_operation_arg_list(pnode_t operation_arg);
pnode_t new_operation_arg(pnode_t attribute_list, char *tidentifier_pdu, char *tidentifier);
pnode_t new_float_const_exp(char *tfloatconst);
pnode_t new_string_const_exp(char *tstringconst);
pnode_t new_integer_const_exp(char *tintconst);
pnode_t new_arithmetic_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri);
pnode_t new_relational_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri);
pnode_t new_logical_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri);
pnode_t new_bitwise_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri);
pnode_t new_identifier(char *tidentifier);
#endif //WIRE_AST
