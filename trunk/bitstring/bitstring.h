#ifndef BSTR_H
#define BSTR_H

#include <stdio.h>

/*
@desc Used internally for forward allocation of bitstring.data
*/
#define BSTR_DATA_SZ 128

/*
@desc Common sizes.
@note Size in a bitstring context is in bits rather than in bytes. When size in
	bytes is used it's explicitly stated as so.
*/
#define SZ_BYTE 8
#define SZ_LONG SZ_BYTE*sizeof(long)
#define SZ_SHORT SZ_BYTE*sizeof(short)
#define SZ_CHAR SZ_BYTE*sizeof(char)

/*
@desc Byte order enum.
@note ORD_NONE is for bit oriented serialization (rather than byte oriented).
*/
#define ORD_BE 1
#define ORD_LE 0
#define ORD_NONE 2

/*
@desc Native byte/bit ordering macros.
*/
#define ORD_NAT_BYTE get_native_byte_order()
#define ORD_NAT_BIT get_native_bit_order()

/*
@desc Floating point representation enum.
*/
#define FP_IEEE754 0
#define FP_IBM 1
#define FP_CRAY 2
#define FP_VAX 3

/*
@desc Type definitions for bitstring input values.
*/
typedef unsigned long int uint_t;
typedef signed long int sint_t;
typedef double fp_t;

/*
@desc Bitstring data type.
*/
struct bitstring{
	char *data;
	unsigned int size;
	unsigned int alloc_size_bytes;
	};
typedef struct bitstring bitstring_t;
typedef bitstring_t* pbitstring_t;

/*
@desc Bitstring API.
*/
void bitstring_lib_init(void);
pbitstring_t bitstring_new(void);
void bitstring_del(pbitstring_t pbs);
char bitstring_get_bit(pbitstring_t pbs, int pos);
void bitstring_append_bit(pbitstring_t pbs, char append_bit);
pbitstring_t bitstring_append(pbitstring_t dest, pbitstring_t src);
pbitstring_t bitstring_concat(pbitstring_t src1, pbitstring_t src2);
void bitstring_print(FILE *fp, pbitstring_t pbs);
pbitstring_t bitstring_set(pbitstring_t pbs, void* data, unsigned int size);
pbitstring_t bitstring_copy(pbitstring_t pbs_src);
pbitstring_t bitstring_new_uint(uint_t val, unsigned int size, char byte_order, char bit_order);
pbitstring_t bitstring_new_sint(sint_t val, unsigned int size, char byte_order, char bit_order);
pbitstring_t bitstring_new_fp(fp_t val, unsigned int size, char fp_rep, char byte_order, char bit_order);

/*
@desc Basic operations.
*/
void conv_byte_order(void* pval, unsigned int size_bytes, char byte_order);
void conv_bit_order(char* pval, char bit_order);
void conv_byte_bit_order(char* pval, unsigned int size_bytes, char byte_order, char bit_order);
void shift_right_le(char* pval, unsigned int size_bytes, unsigned int shift_count);
void shift_left_le(char* pval, unsigned int size_bytes, unsigned int shift_count);
void shift_left_be(char* pval, unsigned int size_bytes, unsigned int shift_count);
void shift_left(char* pval, unsigned int size_bytes, unsigned int shift_count);
char get_native_byte_order();
char get_native_bit_order();
unsigned int get_native_long_int_sz();
char get_native_fp_rep();

#endif //BSTR_H
