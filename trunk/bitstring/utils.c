#include <stdarg.h>
#include <stdio.h>
#include "utils.h"

void print_debug(char *fmt, ...){
#if defined DBG_VERB_1
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "[DEBUG] ");
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#endif
}

void print_error(char* fmt, ...){
#if defined DBG_VERB_1
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "[ERROR] ");
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#endif
}

void print_warning(char* fmt, ...){
#if defined DBG_VERB_1
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "[WARNING] ");
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#endif
}

