#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "wire_utils.h"

struct printer {
	char* canvas;
	unsigned int canvas_pos;
	unsigned int _canvas_alloc_size;
	unsigned int indent;
};
typedef struct printer printer_t;
typedef printer_t* pprinter_t;

#define CANVAS_ALLOC_SIZE 1024
#define INDENT "\t"
pprinter_t printer_new(){
	pprinter_t pprinter = (pprinter_t)malloc(sizeof(printer_t));
	if(pprinter == NULL){
		print_error("printer_new: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}
	
	pprinter->canvas = (char*)malloc(CANVAS_ALLOC_SIZE);
	if(pprinter->canvas == NULL){
		print_error("printer_new: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}
	pprinter->_canvas_alloc_size = CANVAS_ALLOC_SIZE;
	pprinter->canvas_pos = 0;
	pprinter->indent = 0;
	return pprinter;
}

void indent_inc(pprinter_t pprinter){
	pprinter->indent++;
}
void indent_dec(pprinter_t pprinter){
	pprinter->indent--;
}

void print(pprinter_t pprinter, char* string){
	if(pprinter->canvas_pos + pprinter->indent*strlen(INDENT) + strlen(string) 
		> pprinter->_canvas_alloc_size){
		pprinter->canvas = realloc(pprinter->canvas, pprinter->_canvas_alloc_size + CANVAS_ALLOC_SIZE);
		if(pprinter->canvas == NULL){
			print_error("print: Could not allocate space!\n");
			exit(EXIT_FAILURE);
		}
		pprinter->_canvas_alloc_size += CANVAS_ALLOC_SIZE;
	}
	
	for(int i=0; i<pprinter->indent; i++){
		sprintf(pprinter->canvas + pprinter->canvas_pos, INDENT);
		pprinter->canvas_pos += strlen(INDENT);
	}
	sprintf(pprinter->canvas + pprinter->canvas_pos, "%s", string);
	pprinter->canvas_pos += strlen(string);
}

void print_newline(pprinter_t pprinter){
	print(pprinter, "\n");
}

void print_line(pprinter_t pprinter, char* line){
	print(pprinter, line);
	print_newline(pprinter);
}

int main(){
	pprinter_t pprinter = printer_new();
	
	print_line(pprinter, "require(\"bitstring\")");
	print_line(pprinter, "require(\"serdes\")");
	print_newline(pprinter);
	print_line(pprinter, "Math {");
	indent_inc(pprinter);
	indent_dec(pprinter);
	print_line(pprinter, "}");
	
	printf("%s", pprinter->canvas);
	return 0;
}
