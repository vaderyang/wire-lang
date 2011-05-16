#include "bitstring.h"
#include "utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

/*BYTE ORDER*/
char native_byte_order = ORD_LE;
char get_native_byte_order(){
	return native_byte_order;
}

char check_native_byte_order(){
	unsigned short int sh = 0xabcd;
	unsigned char *ptr = (unsigned char*)&sh;
	if(*ptr == 0xcd && *(ptr+1) == 0xab)
		return ORD_LE;
	else
		return ORD_BE;
}

/*BIT ORDER*/
/*@TODO: make check_native_bit_order()*/
char native_bit_order = ORD_BE;
char get_native_bit_order(){
	return native_bit_order;
}

/*LONG SIZE*/
unsigned int get_native_long_int_sz(){
	return SZ_BYTE*sizeof(long int);
}

/*FLOATING POINT*/
/*@TODO: make check_native_fp_rep()*/
char native_fp_rep = FP_IEEE754;
char get_native_fp_rep(){
	return native_fp_rep;
}

/*INIT*/
/*
@desc Initializes the bitstring library. Should be called before usage.
@note The assumption is that bitstrings are running on a
	big endian bit machines.
*/
void bitstring_lib_init(){
	print_debug("[BITSTRING_LIB_INIT]\n");

	native_byte_order = check_native_byte_order();
	if(native_byte_order == ORD_LE)
		print_debug("\t[NATIVE_BYTE_ORDER]\tLITTLE_ENDIAN/LSB_FIRST\n");
	else
		print_debug("\t[NATIVE_BYTE_ORDER]\tBIG_ENDIAN/MSB_FIRST\n");

	print_debug("\t[NATIVE LONG INT SIZE]\t%dB\n", get_native_long_int_sz());

	print_debug("\t[NATIVE FP REP]\tIEEE754\n");
}

/*
#############################################################################
#############################################################################
#############################################################################
*/

/*
@desc Makes an empty bitstring.
*/
pbitstring_t bitstring_new(){
	pbitstring_t pbs = (pbitstring_t)malloc(sizeof(bitstring_t));
	if(pbs == NULL){
		print_error("bitstring_new: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}

	bitstring_set(pbs, NULL, 0);

	return pbs;
}

pbitstring_t bitstring_copy(pbitstring_t pbs_src){
    pbitstring_t pbs = bitstring_new();
    bitstring_set(pbs, pbs_src->data, pbs_src->size);
    return pbs;
}
/*
@desc Deletes a bitstring.
*/
void bitstring_del(pbitstring_t pbs){
	free(pbs->data);
	free(pbs);
}

/*
@desc Sets bitstrings data and size. Handles allocation and error handling.
@note Bits from >=size are cleared.
*/
pbitstring_t bitstring_set(pbitstring_t pbs, void* data, unsigned int size){
	unsigned int size_bytes = size/SZ_BYTE;
	size_bytes = size%SZ_BYTE?size_bytes+1:size_bytes;

	pbs->data = calloc(size_bytes + BSTR_DATA_SZ, 1);
	if(pbs->data == NULL){
		print_error("bitstring_set: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}
	pbs->alloc_size_bytes = size_bytes + BSTR_DATA_SZ;

	memcpy(pbs->data, data, size_bytes);
	//clear the last bits from the last byte
	unsigned char mask = 0xff<<((SZ_BYTE - size%SZ_BYTE)%SZ_BYTE);
	pbs->data[size_bytes-1] &= mask;

	pbs->size = size;

	return pbs;
}

/*
@desc Get a bit value from a specified position.
@note Positions are indexed starting with 0. Assume native big endian bit order.
*/
char bitstring_get_bit(pbitstring_t pbs, int pos){
	char get_byte = pbs->data[pos/SZ_BYTE];
	char get_bit = ((get_byte>>(SZ_BYTE - pos%SZ_BYTE - 1)) & 0x01);
	return get_bit;
}

/*
@desc Appends a single bit onto a bitstring.
@note Assume native big endian bit order.
*/
void bitstring_append_bit(pbitstring_t pbs, char append_bit){
	unsigned int size_bytes = (pbs->size+1)/SZ_BYTE;
	if(size_bytes > pbs->alloc_size_bytes){
		pbs->data = realloc(pbs->data, size_bytes + BSTR_DATA_SZ);
		if(pbs->data == NULL){
			print_error("bitstring_append_bit: Could not allocate space!\n");
			exit(EXIT_FAILURE);
		}
		pbs->alloc_size_bytes = size_bytes + BSTR_DATA_SZ;
	}

	char append_byte = pbs->data[(pbs->size)/SZ_BYTE];
	append_byte = append_byte | (append_bit<<(SZ_BYTE-pbs->size%SZ_BYTE - 1));
	pbs->data[(pbs->size)/SZ_BYTE] = append_byte;
	pbs->size++;
}

/*
@desc Append a source bitstring onto a destination bitstring.
*/
pbitstring_t bitstring_append(pbitstring_t dest, pbitstring_t src){
	int appended_bits = 0;
	/*If byte aligned then great...we can use memcpy*/
	if(dest->size%SZ_BYTE == 0){
		if((int)(dest->size/SZ_BYTE)+(int)(src->size/SZ_BYTE) > dest->alloc_size_bytes){
			dest->data =
				realloc(dest->data, (int)(dest->size/SZ_BYTE) + (int)(src->size/SZ_BYTE) + BSTR_DATA_SZ);
			if(dest->data == NULL){
				print_error("bitstring_append: Could not allocate space!\n");
				exit(EXIT_FAILURE);
			}
		}
		memcpy(dest->data + (int)(dest->size/SZ_BYTE), src->data, (int)(src->size/SZ_BYTE));
		appended_bits += (src->size/SZ_BYTE)*SZ_BYTE;
		dest->size += appended_bits;
	}
	/*Take care of the bits that weren't byte aligned*/
	for(int i=appended_bits; i<src->size; i++){
		bitstring_append_bit(dest, bitstring_get_bit(src, i));
	}
	return dest;
}

/*
@desc Concatenate two bitstrings into a new one.
*/
pbitstring_t bitstring_concat(pbitstring_t src1, pbitstring_t src2){
	pbitstring_t dest = bitstring_new();
	bitstring_append(dest, src1);
	bitstring_append(dest, src2);

	return dest;
}

/*
@desc Prints a bitstring into a file.
*/
void bitstring_print(FILE* fp, pbitstring_t pbs){
	fprintf(fp, "0b");
	for(int i=0; i<pbs->size; i++){
		fprintf(fp, "%d", bitstring_get_bit(pbs, i));
	}
	fprintf(fp, "\n");
}

/*
@note for practical examples there isn't much need for numbers
that are bigger than 64 bits. So we'll keep it down and assume
that they are max native long sized. In the future if there is
a need for such a feature we'll introduce a dependencie lib such
as http://gmplib.org/ which deals with bignums.
Exceptions: crypto
*/

/*
@desc Converts byte order of memory pointed to by "pval".
@note Method is independent of data type you wish to convert byte order to
	(integers, floats, arrays).
@faq Why is here "size_bytes" arg when everywhere else is "size" in bits?
	- Because there's no byte order concept for non byte oriented data.
*/
void conv_byte_order(void* pval, unsigned int size_bytes, char byte_order){
	if(byte_order == ORD_NAT_BYTE){
		return; //no need for conversion
	}

	char *pold_val = (char*)malloc(size_bytes);
	if(pold_val == NULL){
		print_error("conv_byte_order: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}
	memcpy(pold_val, pval, size_bytes);
	char* pold = (char*)pold_val;
	char* pnew = (char*)pval + size_bytes - 1;

	for(int i=0; i<size_bytes; i++){
		*pnew = *pold;
		pnew--;
		pold++;
	}

	free(pold_val);
}

/*
@desc Converts bit order in a byte.
*/
void conv_bit_order(char* pval, char bit_order){
	if(bit_order == get_native_bit_order()){
		return; //no need for conversion
	}

	char old_val = *pval;
	*pval = 0;
	char mask = 1;
	for(int i=0; i<SZ_BYTE; i++){
		if(old_val&mask){
			*pval = *pval | (1<<(SZ_BYTE-1-i));
		}
		mask = mask<<1;
	}
}

void conv_byte_bit_order(
	char* pval, unsigned int size_bytes,
	char byte_order, char bit_order){

	char *pold_val = (char*)malloc(size_bytes);
	if(pold_val == NULL){
		print_error("conv_byte_bit_order: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}
	memcpy(pold_val, pval, size_bytes);
	char* pold = (char*)pold_val;
	char* pnew = (char*)pval + size_bytes - 1;

	if(byte_order == ORD_NAT_BYTE){
		for(int i=0; i<size_bytes; i++){
			conv_bit_order(pnew, bit_order);
			pnew--;
		}
	}else{ //reverse the order
		for(int i=0; i<size_bytes; i++){
			*pnew = *pold;
			conv_bit_order(pnew, bit_order);
			pnew--;
			pold++;
		}
	}

	free(pold_val);
}

/*
@desc Shifts bits to the left in a byte array (little endian machines).
@note This is a workaround for le machine integer shifts.
@example: val_mem = 0x45 | 0x23 | 0x01 ->(shift 3)-> val_mem = 0x29 | 0x18 | 0x08
*/
void shift_left_le(char* pval, unsigned int size_bytes, unsigned int shift_count){
	if(shift_count == 0){
		return;
	}

	//val = 0x12345 val_mem = 0x45 | 0x23 | 0x01 - 0x00
	//shift_count = 3 size_bytes = 3
	unsigned short int two_bytes = 0;
	for(int i=0; i<size_bytes; i++){
		//i=1
		two_bytes = (unsigned char)pval[i]; //this was an issue!!! (0xff -> 0xffff)
		//two_bytes = 0x0023 two_bytes_mem = 0x23 | 0x00
		two_bytes = two_bytes << shift_count;
		//two_bytes = 0x00118 two_bytes_mem = 0x18 | 0x01
		pval[i] = *((char*)&two_bytes);
		//pval[1] = 0x18
		if(i>0){
			pval[i-1] |= *((char*)&two_bytes+1);
			//pval[0] = 0x28 | 0x01 = 0x29
		}
	}
}

/*
@desc Shifts bits to the left in a byte array (big endian machines).
@TODO: check
*/
void shift_left_be(char* pval, unsigned int size_bytes, unsigned int shift_count){
	//val = 0x123456789a val_mem = 0x12 | 0x34 | 0x56 | 0x78 | 0x9a
	//shift_count = 3 size_bytes = 5
	unsigned short int two_bytes = 0;
	for(int i=0; i<size_bytes/sizeof(long); i+=sizeof(long)){
		*((unsigned long int*)pval) <<= shift_count;
		//val_mem = 0x91 | 0xA2 | 0xB3 | 0xC0 | 0x9a
		two_bytes = pval[i+sizeof(long)];
		//two_bytes = 0x009a two_bytes_mem = 0x00 | 0x9a
		two_bytes <<= shift_count;
		//two_bytes = 0x04d0 two_bytes_mem = 0x04 | 0xd0
		pval[i+sizeof(long)-1] |= *((char*)&two_bytes);
		//0xc0 | 0x04 = 0xc4
		//val_mem = 0x91 | 0xA2 | 0xB3 | 0xC4 | 0x9a
	}
	for(int i=(size_bytes/sizeof(long))*sizeof(long); i<size_bytes; i++){
		//i=4
		two_bytes = pval[i];
		//two_bytes = 0x009a two_bytes_mem = 0x00 | 0x9a
		two_bytes = two_bytes << shift_count;
		//two_bytes = 0x04d0 two_bytes_mem = 0x04 | 0xd0
		pval[i] = *((char*)&two_bytes+1);
		//pval[4] = 0xd0
		if(i>(size_bytes/sizeof(long))*sizeof(long)){
			pval[i-1] |= *((char*)&two_bytes);
		}
	}
}

void shift_left(char* pval, unsigned int size_bytes, unsigned int shift_count){
	if(ORD_NAT_BYTE == ORD_LE){
		shift_left_le(pval, size_bytes, shift_count);
	}else{ //ORD_NAT_BYTE == ORD_BE
		shift_left_be(pval, size_bytes, shift_count);
	}
}

/*
@desc Shifts bits to the right in a byte array (little endian machines).
@note This is a workaround for le machine integer shifts.
*/
void shift_right_le(char* pval, unsigned int size_bytes, unsigned int shift_count){
	if(shift_count == 0){
		return;
	}
	//shift_count = 3 size_bytes = 3
	unsigned short int two_bytes = 0;
	char shifted = 0;
	for(int i=0; i<size_bytes; i++){
		//i=1
		*((char*)&two_bytes) = 0;
		*((char*)&two_bytes+1) = (unsigned char)pval[i]; //this was an issue!!! (0xff -> 0xffff)
		//two_bytes = 0x2300 two_bytes_mem = 0x00 | 0x23
		two_bytes = two_bytes >> shift_count;
		//two_bytes = 0x0460 two_bytes_mem = 0x60 | 0x04
		pval[i] = *((char*)&two_bytes+1) | shifted;
		//pval[1] = 0x04
		shifted = *((char*)&two_bytes);
		//shifted = 0x60
	}
}

/*
@desc Makes a new bitstring which represents a serialized unsigned integer value.
@arg "val" Unsigned integer value which is to be serialized into a bitstring.
@arg "size" Size of serialized object.
@arg "byte_order" Byte ordering for serialized object.
	ORD_BE - big endian
	ORD_LE - little endian
	ORD_NONE - no byte ordering, the integer is to be serialized into a bit
		oriented instead of byte oriented object
@arg "bit_order" Bit ordering for serialized object. If integer is to be
	serialized into a byte oriented object rather than a bit oriented, bit order
	is consider for each byte of serialized object (rather than a whole)
	ORD_BE - big endian
	ORD_LE - little endian
*/
pbitstring_t bitstring_new_uint(
		uint_t val, unsigned int size,
		char byte_order, char bit_order){

	unsigned int size_bytes = size/SZ_BYTE;
	size_bytes = size%SZ_BYTE?size_bytes+1:size_bytes;

	pbitstring_t pbs = bitstring_new();

	/*align the value in uint_t sized (for later casting) memory container*/
	char *psized_val = (char*)malloc(sizeof(uint_t));
	if(psized_val == NULL){
		print_error("bitstring_new_uint: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}
	*((uint_t*)psized_val) = 0;

	if(ORD_NAT_BYTE == ORD_LE){
		memcpy(psized_val, &val, size_bytes);
	}else //ORD_NAT_BYTE == ORD_BE
	{
		memcpy(psized_val, ((char*)&val) + sizeof(uint_t) - size_bytes, size_bytes);
	}


	if(byte_order != ORD_NONE){ //byte oriented (byte unit)
		conv_byte_bit_order(psized_val, size_bytes, byte_order, bit_order);
	}else{ //bit oriented (bit unit)
		/*
			The algorithm for serialization to bit oriented object
			val = 0x12345 - serialize to bit oriented of size=21
				and bit_order:
					ORD_BE --> 0b 0000 1001 | 0001 1010 | 0010 1 - 000
					ORD_LE --> 0b 0000 1001 | 0001 1010 | 0010 1 - 000
			Step 1: determine bit/byte memory layout of our value
			Step 2: determine bit memory layout of serialized object
			Step 3: transfom one into another
		*/
		if(ORD_NAT_BIT == ORD_BE){
			////printf("psized_val: 0x%.2x 0x%.2x 0x%.2x 0x%.2x\n", psized_val[0],psized_val[1],psized_val[2],psized_val[3]);
			if(ORD_NAT_BYTE == ORD_BE){
				//psized_val == 0x01 | 0x23 | 0x45 - 0x00 - byte/bit memory layout
				if(bit_order == ORD_BE){
					char shift_count = size_bytes*SZ_BYTE - size; //printf("shift_count: %d\n", shift_count);
					//shift_count == 3
					*((uint_t*)psized_val) = *((uint_t*)psized_val) << shift_count; //printf("psized_val: 0x%.2x 0x%.2x 0x%.2x 0x%.2x\n", psized_val[0],psized_val[1],psized_val[2],psized_val[3]);
					//psized_val == 0b 0000 1001 | 0001 1010 | 0010 1 - 000
				}
				else{ //bit_order == ORD_LE
					for(int i=0; i<size_bytes; i++){
						conv_bit_order(psized_val+i, ORD_LE);
					}
					//psized_val == 0b 1000 0000 | 0100 1100 | 1010 0010 - 0x00
					conv_byte_order(psized_val, size_bytes, ORD_LE);
					//psized_val == 0b 1010 0010 | 0100 1100 | 1000 0000 - 0x00
				}
			}else{ //ORD_NAT_BYTE == ORD_LE
				//psized_val == 0x45 | 0x23 | 0x01 - 0x00 -byte/bit memory layout
				if(bit_order == ORD_BE){
					conv_byte_order(psized_val, size_bytes, ORD_BE); //reverse
					//psized_val == 0x01 | 0x23 | 0x45 - 0x00
					char shift_count = size_bytes*SZ_BYTE - size; //printf("shift_count: %d\n", shift_count);
					//shift_count == 3
					shift_left_le(psized_val, size_bytes, shift_count); //printf("psized_val: 0x%.2x 0x%.2x 0x%.2x 0x%.2x\n", psized_val[0],psized_val[1],psized_val[2],psized_val[3]);
					//psized_val == 0b 0000 1001 | 0001 1010 | 0010 1 - 000
				}else{//bit_order == ORD_LE
					for(int i=0; i<size_bytes; i++){
						conv_bit_order(psized_val+i, ORD_LE);
					}
					//psized_val == 0b 1010 0010 | 1100 0100 | 1000 0000 - 0x00
				}
			}
		}else{ //ORD_NAT_BIT == ORD_LE
			//TODO: implement serialization for LE bit order machines (if exists one)
		}
	}

	bitstring_set(pbs, psized_val, size);
	free(psized_val);
	return pbs;
}

/*
@desc Makes a new bitstring which represents a serialized signed integer value.
*/
pbitstring_t bitstring_new_sint(
		sint_t val, unsigned int size,
		char byte_order, char bit_order){

	return bitstring_new_uint((uint_t)val, size, byte_order, bit_order);
}

/*
@desc Makes a new bitstring which represents a serialized floating point value.
@arg "val" Floating point value which is to be serialized into a bitstring.
@arg "size" Bitstring size (32, 64).
@arg "fp_rep" Floating point representation (see header for list of possible
	values).
@note Currently IEEE754 format is supported with 32 and 64bit sizes.
*/
pbitstring_t bitstring_new_fp(
		fp_t val, unsigned int size, char fp_rep,
		char byte_order, char bit_order){

	if(fp_rep != FP_IEEE754){
		print_warning("Only IEEE754 fp representation is supported!\n");
	}

	pbitstring_t pbs = bitstring_new();

	if(size == sizeof(float)*SZ_BYTE){
		float new_val = (float)val;
		unsigned int size_bytes = sizeof(float);
		conv_byte_bit_order((char*)&new_val, size_bytes, byte_order, bit_order);
		bitstring_set(pbs, &new_val, size);
	}else if(size == sizeof(double)*SZ_BYTE){
		double new_val = (double)val;
		unsigned int size_bytes = sizeof(double);
		conv_byte_bit_order((char*)&new_val, size_bytes, byte_order, bit_order);
		bitstring_set(pbs, &new_val, size);
	}else{
		print_warning("Only 32b/64b fps are supported\n");
	}

	return pbs;
}

