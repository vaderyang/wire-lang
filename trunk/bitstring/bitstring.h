#ifndef BSTR_H
#define BSTR_H

#include <stdio.h>

#define BSTR_DATA_SZ 128

#define SZ_BYTE 8
#define SZ_LONG SZ_BYTE*sizeof(long)
#define SZ_SHORT SZ_BYTE*sizeof(short)
#define SZ_CHAR SZ_BYTE*sizeof(char)

#define ORD_BE 1
#define ORD_LE 0
#define ORD_NONE 2

#define ORD_NAT_BYTE get_native_byte_order()
#define ORD_NAT_BIT get_native_bit_order()

#define FP_IEEE754 0
#define FP_IBM 1
#define FP_CRAY 2
#define FP_VAX 3

typedef unsigned long int uint_t;
typedef signed long int sint_t;
typedef double fp_t;

struct bitstring{
	char *data;
	unsigned int size;
	unsigned int alloc_size_bytes;
	};
typedef struct bitstring bitstring_t;
typedef bitstring_t* pbitstring_t;

void bitstring_lib_init(void);
pbitstring_t bitstring_new(void);
void bitstring_del(pbitstring_t pbs);
char bitstring_get_bit(pbitstring_t pbs, int pos);
void bitstring_append_bit(pbitstring_t pbs, char append_bit);
pbitstring_t bitstring_append(pbitstring_t dest, pbitstring_t src);
void bitstring_print(FILE *fp, pbitstring_t pbs);
pbitstring_t bitstring_set(pbitstring_t pbs, void* data, unsigned int size);
pbitstring_t bitstring_new_uint(uint_t val, unsigned int size, char byte_order, char bit_order);
pbitstring_t bitstring_new_sint(sint_t val, unsigned int size, char byte_order, char bit_order);
pbitstring_t bitstring_new_fp(fp_t val, unsigned int size, char fp_rep);

#endif //BSTR_H
