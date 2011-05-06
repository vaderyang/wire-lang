#include "wire_lex.h"
#include <string.h>
#include <stdarg.h>

unsigned int get_token_by_identifier(char* identifier){
	int i = 0;
	while(1){
		if(primitive_types[i].identifier == NULL)
			{
				break;
			}
			
		if(!strcmp(primitive_types[i].identifier, identifier)){
			return primitive_types[i].token;
		}
		i++;
	}
	
	i = 0;
	while(1){
		if(constructed_types[i].identifier == NULL)
			{
				break;
			}
			
		if(!strcmp(constructed_types[i].identifier, identifier)){
			return constructed_types[i].token;
		}
		i++;
	}
	
	i = 0;
	while(1){
		if(statements[i].identifier == NULL)
			{
				break;
			}
			
		if(!strcmp(statements[i].identifier, identifier)){
			return statements[i].token;
		}
		i++;
	}
	
	/*If not a reserved word of any kind then it's a new identifier*/
	return tIDENTIFIER;
}

