#ifndef BSTR_H
#define BSTR_H

#include <stdio.h> //FILE
#include <stddef.h> //size_t
#include <inttypes.h> //uintmax_t intmax_t

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
enum order{
	ORD_BE,
	ORD_LE,
	ORD_NONE
};
typedef enum order order_t;

/*
@desc Native byte/bit ordering macros.
*/
#define ORD_NAT_BYTE get_native_byte_order()
#define ORD_NAT_BIT get_native_bit_order()

/*
@desc Floating point representation enum.
*/
enum fprep{
	FP_IEEE754,
	FP_IBM, //not supported
	FP_CRAY, //not supported
	FP_VAX //not supported
};
typedef enum fprep fprep_t;

/*
@desc Type definitions for bitstring input values.
*/
typedef uintmax_t uint_t;
typedef intmax_t sint_t;
typedef double fp_t;

/*
@desc Bitstring data type.
*/
struct bitstring{
	char *data;
	size_t size;
	size_t alloc_size_bytes;
	};
typedef struct bitstring bitstring_t;
typedef bitstring_t* pbitstring_t;

/*
@desc Bitstring API.
*/
void bitstring_lib_init(void);
pbitstring_t bitstring_new(void);
void bitstring_del(pbitstring_t pbs);
char bitstring_get_bit(pbitstring_t pbs, unsigned int pos);
void bitstring_append_bit(pbitstring_t pbs, char append_bit);
pbitstring_t bitstring_append(pbitstring_t dest, pbitstring_t src);
pbitstring_t bitstring_concat(pbitstring_t src1, pbitstring_t src2);
void bitstring_print(FILE *fp, pbitstring_t pbs);
pbitstring_t bitstring_set(pbitstring_t pbs, void* data, size_t size);
pbitstring_t bitstring_copy(pbitstring_t pbs_src);
pbitstring_t bitstring_new_uint(uint_t val, size_t size, order_t byte_order, order_t bit_order);
pbitstring_t bitstring_new_sint(sint_t val, size_t size, order_t byte_order, order_t bit_order);
pbitstring_t bitstring_new_fp(fp_t val, size_t size, fprep_t fp_rep, order_t byte_order, order_t bit_order);

/*
@desc Basic operations used internally by the bitstring library. They can be usefull
	so I decided to export them.
*/
void conv_byte_order(void* pval, size_t size_bytes, order_t byte_order);
void conv_bit_order(char* pval, order_t bit_order);
void conv_byte_bit_order(char* pval, size_t size_bytes, order_t byte_order, order_t bit_order);
void shift_right_le(char* pval, size_t size_bytes, unsigned int shift_count);
void shift_left_le(char* pval, size_t size_bytes, unsigned int shift_count);
void shift_left_be(char* pval, size_t size_bytes, unsigned int shift_count);
void shift_left(char* pval, size_t size_bytes, unsigned int shift_count);
order_t get_native_byte_order();
order_t get_native_bit_order();
size_t get_native_long_int_sz();
fprep_t get_native_fp_rep();

#endif //BSTR_H
