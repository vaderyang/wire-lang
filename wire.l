%option nodefault noyywrap yylineno

%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wire_utils.h" //debug,error
#include "wire_lex.h" //lex utils
#include "wire_ast.h" //so wire.tab.h has a definition of pnode_t
#include "wire.tab.h" //tokens
#define YYDEBUG 1
%}


COMMENT	("/*"([^"*"]|[\r\n]|("*"+([^"*/"]|[\r\n])))*\*+\/)|("//".*)|(#.*)

NEWLINE ([\r?\n])

WHITESPACE ([\t\f ]+|{NEWLINE})

IDENTIFIER ([A-Za-z_][A-Za-z0-9_]*)

INT_CONST ({HEX_CONST}|{BIN_CONST}|{OCT_CONST}|{DEC_CONST})

HEX_CONST (0(x|X)[0-9a-fA-F]+)

BIN_CONST (0(b|B)[01]+)

OCT_CONST (0[0-7]+)

DEC_CONST ([0-9][0-9]*)

REAL_CONST ([0-9]*\.[0-9]+([eE][-+]?[0-9]+)?)

STRING_CONST (\"[\40-\176]*\")

%%

{INT_CONST} {
		yylval.text = strdup(yytext);
		print_debug("INT_CONST: %s\n",yylval.text);
		return tINTCONST;
	}

{REAL_CONST} {
		yylval.text = strdup(yytext);
		print_debug("REAL_CONST: %s\n",yylval.text);		
		return tREALCONST;
	}

{STRING_CONST} {
		yylval.text = strdup(yytext);
		print_debug("STRING_CONST: %s\n",yylval.text);
		return tSTRINGCONST;
	}

{IDENTIFIER} {
		yylval.text = strdup(yytext);
		print_debug("IDENTIFIER: %s\n",yylval.text);
		return get_token_by_identifier(yytext);
	}

"==" {
	print_debug("RELATIONAL OP: %s\n",yylval.text);
	return tRELEQU;
	}

"!=" {
	print_debug("RELATIONAL OP: %s\n",yylval.text);
	return tRELNEQU;
	}

">=" {
	print_debug("RELATIONAL OP: %s\n",yylval.text);
	return tRELGE;
	}

"<=" {
	print_debug("RELATIONAL OP: %s\n",yylval.text);
	return tRELLE;
	}

"&&" {
	print_debug("LOGICAL OP: %s\n",yylval.text);
	return tLOGAND;
	}

"||" {
	print_debug("LOGICAL OP: %s\n",yylval.text);
	return tLOGOR;
	}

"<<" {
	print_debug("BITWISE OP: %s\n",yylval.text);
	return tBITSL;
	}

">>" {
	print_debug("BITWISE OP: %s\n",yylval.text);
	return tBITSR;
	}

["\[\](){}%/*+\-;,=&|\^<>:!."] {
		print_debug("OP: %c\n", *yytext);
		return *yytext;
	}

{WHITESPACE}	;

.	{
		print_error("%s <%s> -- line %d","invalid character", yytext, yylineno);
		exit(1);
	}


%%
int main(int argc, char* argv[])
{
	yyin = fopen(argv[argc-1], "r" );
    if(yyin == NULL)
    {
		perror("fopen");
		return 2;
    }

	yyparse();
	return 0;
}
