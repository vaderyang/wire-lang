%option nodefault noyywrap yylineno

%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wire_utils.h" //debug,error
#include "wire_lex.h" //lex utils
#include "wire_ast.h" //so wire.tab.h has a definition of pnode_t
#include "wire.tab.h" //tokens

int yyparse();

#define YYDEBUG 1
%}

reCOMMENT	("/*"([^"*"]|[\r\n]|("*"+([^"*/"]|[\r\n])))*\*+\/)|("//".*)|(#.*)

reNEWLINE ([\r?\n])

reWHITESPACE ([\t\f ]+|{reNEWLINE})

reIDENTIFIER ([A-Za-z_][A-Za-z0-9_]*)

reINTCONST ({reHEXCONST}|{reBINCONST}|{reOCTCONST}|{reDECCONST})

reHEXCONST (0(x|X)[0-9a-fA-F]+)

reBINCONST (0(b|B)[01]+)

reOCTCONST (0[0-7]+)

reDECCONST ([0-9][0-9]*)

reFLOATCONST ([0-9]*\.[0-9]+([eE][-+]?[0-9]+)?)

reSTRINGCONST (\"([\40-\41\43-\176])*\")

%%
{reINTCONST} {
		yylval.text = strdup(yytext);
		print_debug("INT_CONST: %s\n",yylval.text);
		return tINTCONST;
	}

{reFLOATCONST} {
		yylval.text = strdup(yytext);
		print_debug("REAL_CONST: %s\n",yylval.text);
		return tFLOATCONST;
	}

{reSTRINGCONST} {
		yylval.text = strdup(yytext);
		print_debug("STRING_CONST: %s\n",yylval.text);
		return tSTRINGCONST;
	}

{reIDENTIFIER} {
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

{reWHITESPACE}	;

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
