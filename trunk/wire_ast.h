#ifndef WIRE_AST
#define WIRE_AST

/*Abstract syntax tree creation*/

/*Nodes*/

enum nodes{
    PROTOCOL ,
    ATTRIBUTES ,
    ATTRIBUTE ,
    ATTRIBUTE_ARGUMENTS ,
    ATTRIBUTE_ARGUMENT ,
    DOUBLE_CONST_EXP ,
    STRING_CONST_EXP ,
    INTEGER_CONST_EXP ,
    STRUCT_CONST_EXP ,
    STRUCT_CONST_EXP_BODY ,
    STRUCT_CONST_EXP_BODY_COMPONENT ,
    ARITHMETIC_EXP_ADD ,
    ARITHMETIC_EXP_SUB ,
    ARITHMETIC_EXP_MUL ,
    ARITHMETIC_EXP_DIV ,
    ARITHMETIC_EXP_MOD ,
    RELATIONAL_EXP_G ,
    RELATIONAL_EXP_L ,
    RELATIONAL_EXP_EQU ,
    RELATIONAL_EXP_NEQU ,
    RELATIONAL_EXP_GE ,
    RELATIONAL_EXP_LE ,
    LOGICAL_EXP_NOT ,
    LOGICAL_EXP_AND ,
    LOGICAL_EXP_OR ,
    BITWISE_EXP_NOT ,
    BITWISE_EXP_AND ,
    BITWISE_EXP_OR ,
    BITWISE_EXP_XOR ,
    BITWISE_EXP_SR ,
    BITWISE_EXP_SL ,
    IDENTIFIER,
    PROTOCOL_COMPONENTS,
    PRIMITIVE_TYPE_DECLARATOR,
    LOCAL_UNION_DECLARATOR,
    LOCAL_DECLARATOR_LIST,
    LOCAL_ENUM_DECLARATOR_ANON,
    LOCAL_ENUM_DECLARATOR_NAMED,
    GLOBAL_ENUM_DECLARATOR,
    TYPEDEF_DECLARATOR,
    LOCAL_STRUCT_DECLARATOR_ANON,
    LOCAL_STRUCT_DECLARATOR_NAMED,
    GLOBAL_STRUCT_DECLARATOR,
    PACKET_DECLARATOR,
    UNION_BODY,
    UNION_BODY_COMPONENT,
    ENUM_BODY,
    ENUM_BODY_COMPONENT,
    ARRAY_DECLARATOR_OPT,
    PRIMITIVE_TYPE_BIT,
    PRIMITIVE_TYPE_BYTE,
    PRIMITIVE_TYPE_STRING,
    PRIMITIVE_TYPE_DOUBLE,
    PRIMITIVE_TYPE_UNSIGNED,
    PRIMITIVE_TYPE_SIGNED,
    STRUCT_BODY,
    STRUCT_BODY_COMPONENT,
    PACKET_BODY,
    PACKET_BODY_COMPONENT
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

pnode_t new_node(enum nodes type);
pnode_t add_child(pnode_t pnode_parent, void *pchild);

pnode_t new_wire(pnode_t protocol);
pnode_t new_protocol(pnode_t attributes, char *tidentifier, pnode_t protocol_body);
pnode_t new_protocol_components(pnode_t protocol_component);
pnode_t new_attributes(pnode_t attribute);
pnode_t new_attribute(char *tidentifier, pnode_t attribute_arguments);
pnode_t new_attribute_arguments(pnode_t attribute_argument);
pnode_t new_attribute_argument(pnode_t const_exp);
pnode_t new_double_const_exp(char *trealconst);
pnode_t new_string_const_exp(char *tstringconst);
pnode_t new_integer_const_exp(char *tintconst);
pnode_t new_struct_const_exp(pnode_t struct_const_exp_body);
pnode_t new_struct_const_exp_body(pnode_t struct_const_exp_body_component);
pnode_t new_struct_const_exp_body_component(char *tidentifier, pnode_t const_exp);
pnode_t new_arithmetic_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri);
pnode_t new_relational_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri);
pnode_t new_logical_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri);
pnode_t new_bitwise_exp(pnode_t const_exp_le, unsigned int op, pnode_t const_exp_ri);
pnode_t new_identifier(char *tidentifier);
pnode_t new_primitive_type_declarator(pnode_t attributes_opt, pnode_t primitive_type, char *tidentifier, pnode_t array_declarator_opt);
pnode_t new_local_union_declarator(pnode_t attributes_opt, pnode_t const_exp, pnode_t union_body, char *tidentifier, pnode_t array_declarator_opt);
pnode_t new_local_declarator_list(pnode_t local_declarator);
pnode_t new_local_enum_declarator_anon(pnode_t attributes_opt, pnode_t enum_body, char *tidentifier, pnode_t array_declarator_opt);
pnode_t new_local_enum_declarator_named(pnode_t attributes_opt, char *tidentifier_name, char *tidentifier, pnode_t array_declarator_opt);
pnode_t new_global_enum_declarator(pnode_t attributes_opt, char *tidentifier, pnode_t enum_body);
pnode_t new_typedef_declarator(pnode_t local_declarator);
pnode_t new_local_struct_declarator_anon(pnode_t attributes_opt, pnode_t struct_body, char *tidentifier, pnode_t array_declarator_opt);
pnode_t new_local_struct_declarator_named(pnode_t attributes_opt, char *tidentifier_name, char *tidentifier, pnode_t array_declarator_opt);
pnode_t new_global_struct_declarator(pnode_t attributes_opt, char *tidentifier, pnode_t struct_body);
pnode_t new_packet_declarator(pnode_t attributes_opt, char *tidentifier, pnode_t packet_body);
pnode_t new_union_body(pnode_t union_body_component);
pnode_t new_union_body_component(pnode_t const_exp, pnode_t local_declarator_list);
pnode_t new_enum_body(pnode_t enum_body_component);
pnode_t new_enum_body_component(char *tidentifier, pnode_t const_exp);
pnode_t new_array_declarator_opt(pnode_t const_exp);
pnode_t new_primitive_type(unsigned int type);
pnode_t new_struct_body(pnode_t struct_body_component);
pnode_t new_struct_body_component(pnode_t local_declarator);
pnode_t new_packet_body(pnode_t packet_body_component);
pnode_t new_packet_body_component(pnode_t local_declarator);

#endif //WIRE_AST
