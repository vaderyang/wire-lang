%error-verbose
%debug

%{
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wire_ast.h"
#include "wire_utils.h"
#include "wire.tab.h"

int yylex(void); //supress warning
int yyerror(char const * msg);

#if defined DBG_VERB_2
int yydebug = 1;
#endif

%}

%union {
	char* text;
	pnode_t pnode;
}

/*LITERALS/CONSTANTS*/
%token <text> tINTCONST
%token <text> tFLOATCONST
%token <text> tSTRINGCONST

/*PRIMITIVE TYPES*/
%token <text> tBYTE
%token <text> tFLOAT
%token <text> tSTRING
%token <text> tUINT
%token <text> tSINT

/*CONSTRUCTED TYPES*/
%token <text> tENUM
%token <text> tSTRUCT
%token <text> tUNION
%token <text> tPDU

/*PROTOCOL*/
%token <text> tPROTOCOL

/*OPERATION*/
%token <text> tOPERATION

/*STATEMENTS*/
%token <text> tIMPORT
%token <text> tTYPEDEF
%token <text> tDEFAULT

/*IDENTIFIER TOKEN*/
%token <text> tIDENTIFIER

/*MULTICHARACTER OPERATORS*/
%token <text> tRELEQU //==
%token <text> tRELNEQU //!=
%token <text> tRELGE //>=
%token <text> tRELLE //<=
%token <text> tLOGAND //&&
%token <text> tLOGOR //||
%token <text> tBITSR //>>
%token <text> tBITSL //<<

/*PRECEDENCE AND ASSOCIATIVITY*/
%left tLOGOR //11
%left tLOGAND //10
%left '|' //9
%left '^' //8
%left '&' //7
%left tRELEQU tRELNEQU //6
%left  '<' '>' tRELGE tRELLE //5
%left tBITSR tBITSL //4
%left '+' '-' //3
%left '*' '/' '%' //2
%right '!' '~' //1

%type <pnode> wire

%type <pnode> protocol
%type <pnode> protocol_body
%type <pnode> protocol_body_component

%type <pnode> attribute_list_opt
%type <pnode> attribute_list
%type <pnode> attribute
%type <pnode> attribute_argument_list
%type <pnode> attribute_argument

%type <pnode> type_definition
%type <pnode> enum_definition
%type <pnode> enum_body
%type <pnode> enum_body_component
%type <pnode> union_definition
%type <pnode> union_body
%type <pnode> union_body_component
%type <pnode> struct_definition
%type <pnode> struct_body
%type <pnode> struct_body_component
%type <pnode> pdu_definition
%type <pnode> pdu_body
%type <pnode> pdu_body_component

%type <pnode> local_declarator
%type <pnode> local_declarator_list
%type <pnode> primitive_local_declarator
%type <pnode> constructed_local_declarator
%type <pnode> anon_local_declarator

%type <pnode> array_declarator_opt

%type <pnode> operation_declarator
%type <pnode> operation_arg_list
%type <pnode> operation_arg

%type <pnode> const_exp
%type <pnode> float_const_exp
%type <pnode> string_const_exp
%type <pnode> integer_const_exp
%type <pnode> struct_const_exp
%type <pnode> struct_const_exp_body
%type <pnode> struct_const_exp_body_component
%type <pnode> arithmetic_exp
%type <pnode> relational_exp
%type <pnode> logical_exp
%type <pnode> bitwise_exp
%type <pnode> identifier


%start wire
%%
	wire:
		protocol {
			print_debug("NICE\n");
			$$ = new_wire($1);
		}
		|
		{
		    $$ = NULL;
		}
		;

/*
* PROTOCOL
*/

	protocol:
		attribute_list_opt tPROTOCOL tIDENTIFIER '{' protocol_body '}' {
			$$ = new_protocol($1, $3, $5);
		}
		;

	protocol_body:
		protocol_body_component ';'{
			$$ = new_protocol_body($1);
		}
		|
		protocol_body protocol_body_component {
			$$ = add_child($1, $2);
		}
		;

	protocol_body_component:
		type_definition {
			$$ = $1;
		}
		|
		operation_declarator {
			$$ = $1
		}
		;

		
/*
* ATTRIBUTES
*/

	attribute_list_opt:
		'[' attribute_list ']' {
			$$ = $2;
		}
		|
		{
			$$ = NULL;
		}
		;

	attribute_list:
		attribute {
			$$ = new_attribute_list($1);
		}
		|
		attribute_list ',' attribute {
			$$ = add_child($1, $3);
		}
		;

	attribute:
		tIDENTIFIER {
			$$ = new_attribute($1, NULL);
		}
		|
		tIDENTIFIER '(' attribute_argument_list ')'{
			$$ = new_attribute($1, $3);
		}
		;

	attribute_argument_list:
		attribute_argument {
			$$ = new_attribute_argument_list($1);
		}
		|
		attribute_argument_list ',' attribute_argument {
			$$ = add_child($1, $3);
		}
		;

	attribute_argument:
		const_exp {
			$$ = new_attribute_argument($1);
		}
		;

/*
* TYPE DEFINITIONS
*/

	type_definition:
		enum_definition {
		}
		|
		union_definition {
		}
		|
		struct_definition {
		}
		|
		pdu_definition {
		}
		//|
		//typedef {
		//}
		;
	
	/*ENUM*/
	enum_definition:
		attribute_list_opt tENUM tIDENTIFIER '{' enum_body '}' {
		}
		;
	
	enum_body:
		enum_body_component {
			$$ = new_enum_body($1);
		}
		|
		enum_body ',' enum_body_component {
			$$ = add_child($1, $3);
		}
		;

	enum_body_component:
		tIDENTIFIER '=' const_exp {
			$$ = new_enum_body_component($1, $3);
		}
		|
		tIDENTIFIER {
			$$ = new_enum_body_component($1, NULL);
		}
		;
	
	/*UNION*/
	union_definition:
		attribute_list_opt tUNION '{' union_body '}' tIDENTIFIER {
			$$ = new_union_definition($1, $4, $6);
		}
	;
	
	union_body:
		union_body_component {
			$$ = new_union_body($1);
		}
		|
		union_body union_body_component {
			$$ = add_child($1, $2);
		}
		;

	union_body_component:
		const_exp ':' local_declarator_list {
			$$ = new_union_body_component($1, $3);
		}
		|
		tDEFAULT ':' local_declarator_list {
			$$ = new_union_body_component(NULL, $3);
		}
		;
	
	/*STRUCT*/
	struct_definition:
		attribute_list_opt tSTRUCT tIDENTIFIER '{' struct_body '}' {
			$$ = new_struct_definition($1, $3, $5);
		}
	;

	struct_body:
		struct_body_component {
			$$ = new_struct_body($1);
		}
		|
		struct_body struct_body_component {
			$$ = add_child($1, $2);
		}
		;

	struct_body_component:
		local_declarator ';' {
			$$ = $1;
		}
		;

	/*PDU*/
	pdu_definition:
		attribute_list_opt tPDU tIDENTIFIER '{' pdu_body '}' {
			$$ = new_pdu_definition($1, $3, $5);
		}
		;
	
	pdu_body:
		pdu_body_component {
			$$ = new_pdu_body($1);
		}
		|
		pdu_body pdu_body_component {
			$$ = add_child($1, $2);
		}
		;

	pdu_body_component:
		local_declarator ';' {
			$$ = $1;
		}
		;
	
	/*LOCAL DECLARATOR*/
	local_declarator:
		primitive_local_declarator {
			$$ = $1;
		}
		|
		constructed_local_declarator {
			$$ = $1;
		}
		|
		anon_local_declarator {
			$$ = $1;
		}
		;
	
	local_declarator_list:
		local_declarator ';' {
			$$ = new_local_declarator_list($1);
		}
		|
		local_declarator_list local_declarator ';' {
			$$ = add_child($1, $2);
		}
		;
		
	primitive_local_declarator:
		attribute_list_opt tBYTE tIDENTIFIER array_declarator_opt {
			$$ = new_primitive_local_declarator($1, $2, $3, $4);
		}
		|
		attribute_list_opt tFLOAT tIDENTIFIER array_declarator_opt {
			$$ = new_primitive_local_declarator($1, $2, $3, $4);
		}
		|
		attribute_list_opt tSTRING tIDENTIFIER array_declarator_opt {
			$$ = new_primitive_local_declarator($1, $2, $3, $4);
		}
		|
		attribute_list_opt tUINT tIDENTIFIER array_declarator_opt {
			$$ = new_primitive_local_declarator($1, $2, $3, $4);
		}
		|
		attribute_list_opt tSINT tIDENTIFIER array_declarator_opt {
			$$ = new_primitive_local_declarator($1, $2, $3, $4);
		}
		;
	
	constructed_local_declarator:
		attribute_list_opt tENUM tIDENTIFIER tIDENTIFIER array_declarator_opt {
			$$ = new_constructed_local_declarator($1, $2, $3, $4, $5);		
		}
		|
		attribute_list_opt tSTRUCT tIDENTIFIER tIDENTIFIER array_declarator_opt {
			$$ = new_constructed_local_declarator($1, $2, $3, $4, $5);		
		}
		|
		attribute_list_opt tUNION tIDENTIFIER tIDENTIFIER array_declarator_opt {
			$$ = new_constructed_local_declarator($1, $2, $3, $4, $5);		
		}
		|
		attribute_list_opt tPDU tIDENTIFIER tIDENTIFIER array_declarator_opt {
			$$ = new_constructed_local_declarator($1, $2, $3, $4, $5);		
		}
		;

	anon_local_declarator:
		attribute_list_opt tUNION '<' const_exp '>' '{' union_body '}' array_declarator_opt {
			$$ = new_anon_local_declarator($1, $2, $4, $7, $9);
		}
		//TODO: struct, enum anon declarators
		;
		
	/*ARRAY DECLARATOR*/
	array_declarator_opt:
		'[' const_exp ']' {
			$$ = $2;
		}
		| {
			$$ = NULL;
		}
		;

/*
* OPERATIONS
*/
	operation_declarator:
		attribute_list_opt tOPERATION tIDENTIFIER '(' operation_arg_list ')' {
			$$ = new_operation_declarator($1, $3, $5);
		}
		;
	
	operation_arg_list:
		operation_arg ',' {
			$$ = new_operation_arg_list($1);
		}
		|
		operation_arg_list operation_arg{
			$$ = add_child($1, $2);
		}
		;
	
	operation_arg:
		attribute_list tPDU tIDENTIFIER tIDENTIFIER {
			$$ = new_operation_arg($1, $3, $4);
		}
		;
/*
* EXPRESSIONS
*/

	const_exp:
		integer_const_exp {
			$$ = $1;
		}
		|
		float_const_exp {
			$$ = $1;
		}
		|
		string_const_exp {
			$$ = $1;
		}
		|
		struct_const_exp {
			$$ = $1;
		}
		|
		identifier {
			$$ = $1;
		}
		|
		arithmetic_exp {
			$$ = $1;
		}
		|
		relational_exp {
			$$ = $1;
		}
		|
		logical_exp {
			$$ = $1;
		}
		|
		bitwise_exp {
			$$ = $1;
		}
		;

	float_const_exp:
		tFLOATCONST {
			$$ = new_double_const_exp($1);
		}
		;

	string_const_exp:
		tSTRINGCONST {
            $$ = new_string_const_exp($1);
		}
		;

	integer_const_exp:
		tINTCONST {
            $$ = new_integer_const_exp($1);
		}
		;

	struct_const_exp:
		'{' struct_const_exp_body '}' {
			$$ = new_struct_const_exp($2);
		}
		;

	struct_const_exp_body:
		struct_const_exp_body_component {
			$$ = new_struct_const_exp_body($1);
		}
		|
		struct_const_exp_body ',' struct_const_exp_body_component {
			$$ = add_child($1, $3);
		}
		;

	struct_const_exp_body_component:
		tIDENTIFIER '=' const_exp {
			$$ = new_struct_const_exp_body_component($1, $3);
		}
        ;

	arithmetic_exp:
		const_exp '+' const_exp {
			$$ = new_arithmetic_exp($1, '+', $3);
		}
		|
		const_exp '-' const_exp {
			$$ = new_arithmetic_exp($1, '-', $3);
		}
		|
		const_exp '*' const_exp {
			$$ = new_arithmetic_exp($1, '*', $3);
		}
		|
		const_exp '/' const_exp {
			$$ = new_arithmetic_exp($1, '/', $3);
		}
		|
		const_exp '%' const_exp {
			$$ = new_arithmetic_exp($1, '%', $3);
		}
		;

	relational_exp:
		const_exp '>' const_exp {
			$$ = new_relational_exp($1, '>', $3);
		}
		|
		const_exp '<' const_exp {
			$$ = new_relational_exp($1, '<', $3);
		}
		|
		const_exp tRELEQU const_exp {
			$$ = new_relational_exp($1, tRELEQU, $3);
		}
		|
		const_exp tRELNEQU const_exp {
			$$ = new_relational_exp($1, tRELNEQU, $3);
		}
		|
		const_exp tRELGE const_exp {
			$$ = new_relational_exp($1, tRELGE, $3);
		}
		|
		const_exp tRELLE const_exp {
			$$ = new_relational_exp($1, tRELLE, $3);
		}
		;

	logical_exp:
		'!' const_exp {
			$$ = new_logical_exp(NULL, '!', $2);
		}
		|
		const_exp tLOGAND const_exp {
			$$ = new_logical_exp($1, tLOGAND, $3);
		}
		|
		const_exp tLOGOR const_exp {
			$$ = new_logical_exp($1, tLOGOR, $3);
		}
		;

	bitwise_exp:
		'~' const_exp {
			$$ = new_bitwise_exp(NULL, '~', $2);
		}
		|
		const_exp '&' const_exp {
			$$ = new_bitwise_exp($1, '&', $3);
		}
		|
		const_exp '|' const_exp {
			$$ = new_bitwise_exp($1, '|', $3);
		}
		|
		const_exp '^' const_exp {
			$$ = new_bitwise_exp($1, '^', $3);
		}
		|
		const_exp tBITSR const_exp {
			$$ = new_bitwise_exp($1, tBITSR, $3);
		}
		|
		const_exp tBITSL const_exp {
			$$ = new_bitwise_exp($1, tBITSL, $3);
		}
		;

	identifier:
		tIDENTIFIER {
			$$ = new_identifier($1);
		}
		|
		identifier '.' tIDENTIFIER {
			$$ = add_child($1, $3);
		}
		;


%%

int yyerror (char const * msg){
	fprintf(stderr, "%s\n", msg);
	return 1;
}
