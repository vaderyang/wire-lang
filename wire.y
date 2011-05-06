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
%token <text> tREALCONST
%token <text> tSTRINGCONST

/*PRIMITIVE TYPES*/
%token <text> tBIT
%token <text> tBYTE
%token <text> tDOUBLE
%token <text> tSTRING
%token <text> tINT

/*CONSTRUCTED TYPES*/
%token <text> tPROTOCOL
%token <text> tENUM
%token <text> tSTRUCT
%token <text> tUNION
%token <text> tPACKET

/*STATEMENTS*/
%token <text> tIMPORT
%token <text> tTYPEDEF
%token <text> tUNSIGNED
%token <text> tSIGNED
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
%type <pnode> attributes_opt
%type <pnode> attributes
%type <pnode> attribute
%type <pnode> attribute_arguments
%type <pnode> attribute_argument
%type <pnode> protocol_body
%type <pnode> protocol_components
%type <pnode> protocol_component
%type <pnode> global_declarator
%type <pnode> local_declarator
%type <pnode> global_constructed_type_declarator
%type <pnode> primitive_type_declarator
%type <pnode> local_constructed_type_declarator
%type <pnode> local_union_declarator
%type <pnode> union_body
%type <pnode> union_body_component
%type <pnode> local_declarator_list
%type <pnode> local_enum_declarator
%type <pnode> local_enum_declarator_anon
%type <pnode> local_enum_declarator_named
%type <pnode> global_enum_declarator
%type <pnode> enum_body
%type <pnode> enum_body_component
%type <pnode> array_declarator_opt
%type <pnode> primitive_type
%type <pnode> typedef_declarator
%type <pnode> local_struct_declarator
%type <pnode> local_struct_declarator_anon
%type <pnode> local_struct_declarator_named
%type <pnode> global_struct_declarator
%type <pnode> struct_body
%type <pnode> struct_body_component
%type <pnode> packet_declarator
%type <pnode> packet_body
%type <pnode> packet_body_component
%type <pnode> const_exp
%type <pnode> double_const_exp
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
    Protocol
*/

	protocol:
		'[' attributes ']' tPROTOCOL tIDENTIFIER '{' protocol_body '}' {
			$$ = new_protocol($2, $5, $7);
		}
		|
		tPROTOCOL tIDENTIFIER '{' protocol_body '}' {
			$$ = new_protocol(NULL, $2, $4);
		}
		;

	protocol_body:
		protocol_components {
			$$ = $1;
		}
		;
		
	protocol_components:
		protocol_component {
			$$ = new_protocol_components($1);
		}
		|
		protocol_components protocol_component {
			$$ = add_child($1, $2);
		}
		;
		
	protocol_component:
		global_declarator ';' {
			$$ = $1;
		}
		;

/*
    Attributes
    
    Attributes can be assigned to every data type in wire. They provide various
    information about type's semantics, sizes and other.
*/	

	attributes_opt:
		'[' attributes ']' {
			$$ = $2;
		}
		|
		{
			$$ = NULL;
		}
		;
	
	attributes:
		attribute {
			$$ = new_attributes($1);
		}
		|
		attributes ',' attribute {
			$$ = add_child($1, $3);
		}
		;
	
	attribute:
		tIDENTIFIER {
			$$ = new_attribute($1, NULL);
		}
		|
		tIDENTIFIER '(' attribute_arguments ')'{
			$$ = new_attribute($1, $3);
		}
		;
	
	attribute_arguments:
		attribute_argument {
			$$ = new_attribute_arguments($1);
		}
		|
		attribute_arguments ',' attribute_argument {
			$$ = add_child($1, $3);
		}
		;
	
	attribute_argument:
		const_exp {
			$$ = new_attribute_argument($1);
		}
		;

/*
    Declarators
    There are two types of declarators:
        a) Global declarator - a declarator statement made in global protocol
        scope
        b) Local declarator - a declarator statement made in scope of other types
        such as typedefs, structure declarations and operation declarations.
*/

	global_declarator:
		typedef_declarator {
			$$ = $1;
		}
		|
		global_constructed_type_declarator {
			$$ = $1;
		}
		|
		packet_declarator {
			$$ = $1;
		}
		//|
		//operation_declarator ';'
		;
	
	local_declarator:
		primitive_type_declarator {
			$$ = $1;
		}
		|
		local_constructed_type_declarator {
			$$ = $1;
		}
		;
	
	global_constructed_type_declarator:
		global_enum_declarator {
			$$ = $1;
		}
		|
		global_struct_declarator {
			$$ = $1;
		}
		//|
		//global_union_declarator
		;
		
	primitive_type_declarator:
		attributes_opt primitive_type tIDENTIFIER array_declarator_opt {
			$$ = new_primitive_type_declarator($1, $2, $3, $4);
		}
		;
	
	local_constructed_type_declarator:
		local_enum_declarator {
			$$ = $1;
		}
		|
		local_struct_declarator {
			$$ = $1;
		}
		|
		local_union_declarator {
			$$ = $1;
		}
		;
	
	local_union_declarator:
		attributes_opt tUNION '<' const_exp '>' '{' union_body '}' tIDENTIFIER array_declarator_opt {
			$$ = new_local_union_declarator($1, $4, $7, $9, $10);
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

	local_enum_declarator:
		local_enum_declarator_anon {
		    $$ = $1;
		}
		|
		local_enum_declarator_named {
		    $$ = $1;
		}
		;

    local_enum_declarator_anon:
   		attributes_opt tENUM '{' enum_body '}' tIDENTIFIER array_declarator_opt {
			$$ = new_local_enum_declarator_anon($1, $4, $6, $7);
		}
        ;
        
    local_enum_declarator_named:
		attributes_opt tENUM tIDENTIFIER tIDENTIFIER array_declarator_opt {
			$$ = new_local_enum_declarator_named($1, $3, $4, $5);
		}
        ;
            	
	global_enum_declarator:
		attributes_opt tENUM tIDENTIFIER '{' enum_body '}' {
			$$ = new_global_enum_declarator($1, $3, $5);
		}
		;

	typedef_declarator:
		tTYPEDEF local_declarator {
			$$ = new_typedef_declarator($2);
		}
		;
	
	local_struct_declarator:
    	local_struct_declarator_anon {
    	    $$ = $1;
    	}
    	|
    	local_struct_declarator_named {
    	    $$ = $1;
    	}
    	;

   	local_struct_declarator_anon:
		attributes_opt tSTRUCT '{' struct_body '}' tIDENTIFIER array_declarator_opt {
			$$ = new_local_struct_declarator_anon($1, $4, $6, $7);
		}
		;

   	local_struct_declarator_named:
		attributes_opt tSTRUCT tIDENTIFIER tIDENTIFIER array_declarator_opt {
			$$ = new_local_struct_declarator_named($1, $3, $4, $5);
		}
		;
	
	global_struct_declarator:
		attributes_opt tSTRUCT tIDENTIFIER '{' struct_body '}' {
			$$ = new_global_struct_declarator($1, $3, $5);
		}
		;

	packet_declarator:
		attributes_opt tPACKET tIDENTIFIER '{' packet_body '}' {
			$$ = new_packet_declarator($1, $3, $5);
		}
		;

/*
    Union
*/

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
		
/*
    Enums
    
		Global enum declaration:
			enum counts {One, Two, Three};
		Local enum declaration:
			struct foo {
				int num;
				enum {One, Two, Three} count;
				enum {One = 1, Two = 2, Three = 4} count2[3];
				enum counts count3;
			};
*/

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

/*
    Arrays
*/
		
	array_declarator_opt:
		'[' const_exp ']' {
			$$ = $2;
		}
		| {
			$$ = NULL;
		}
		;

/*
    Types
*/	

	primitive_type:
		tBIT {
		    $$ = new_primitive_type(tBIT);
		}
		|
		tBYTE {
		    $$ = new_primitive_type(tBYTE);
		}
		|
		tSTRING {
		    $$ = new_primitive_type(tSTRING);
		}
		|
		tDOUBLE {
		    $$ = new_primitive_type(tDOUBLE);
		}
		|
		tUNSIGNED tINT {
			$$ = new_primitive_type(tUNSIGNED);
		}
		|
		tSIGNED tINT {
			$$ = new_primitive_type(tSIGNED);
		}
		|
		tINT {
			$$ = new_primitive_type(tSIGNED);
		}		
		;

/*
    Structures
*/				

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

/*
    Packets
*/

	packet_body:
		packet_body_component {
			$$ = new_packet_body($1);
		}
		|
		packet_body packet_body_component {
			$$ = add_child($1, $2);
		}
		;
	
	packet_body_component:
		local_declarator ';' {
			$$ = $1;
		}
		;

/*
    Expressions
*/	

	const_exp:
		integer_const_exp {
			$$ = $1;
		}
		|
		double_const_exp {
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
	
	double_const_exp:
		tREALCONST {
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
}
