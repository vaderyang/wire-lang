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
/*Initializes the bitstring library. Should be called for every instance.*/
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

pbitstring_t bitstring_new(){
	pbitstring_t pbs = (pbitstring_t)malloc(sizeof(bitstring_t));
	if(pbs == NULL){
		print_error("bitstring_new: Could not allocate space!\n");
		exit(1);
	}
	
	pbs->data = (char*)malloc(BSTR_DATA_SZ);
	if(pbs->data == NULL){
		print_error("bitstring_new: Could not allocate space!\n");
		exit(1);
	}
	pbs->size = 0;
	pbs->alloc_size_bytes = BSTR_DATA_SZ;

	return pbs;
}

void bitstring_del(pbitstring_t pbs){
	free(pbs->data);
	free(pbs);
}
/*pos begins with 0*/
char bitstring_get_bit(pbitstring_t pbs, int pos){
	char get_byte = pbs->data[pos/SZ_BYTE];
	char get_bit = ((get_byte>>(SZ_BYTE - pos%SZ_BYTE - 1)) & 0x01);
	return get_bit;
}

void bitstring_append_bit(pbitstring_t pbs, char append_bit){
	unsigned int size_bytes = (pbs->size+1)/SZ_BYTE;
	if(size_bytes > pbs->alloc_size_bytes){
		pbs->data = realloc(pbs->data, size_bytes + BSTR_DATA_SZ);
		if(pbs->data == NULL){
			print_error("bitstring_append_bit: Could not allocate space!\n");
			exit(1);
		}
		pbs->alloc_size_bytes = size_bytes + BSTR_DATA_SZ;
	}
	
	char append_byte = pbs->data[(pbs->size)/SZ_BYTE];
	append_byte = append_byte | (append_bit<<(SZ_BYTE-pbs->size%SZ_BYTE - 1));
	pbs->data[(pbs->size)/SZ_BYTE] = append_byte;
	pbs->size++;
}

pbitstring_t bitstring_append(pbitstring_t dest, pbitstring_t src){
	int appended_bits = 0;
	/*If byte aligned then great...we can use memcpy*/
	if(dest->size%SZ_BYTE == 0){
		if((int)(dest->size/SZ_BYTE)+(int)(src->size/SZ_BYTE) > dest->alloc_size_bytes){
			dest->data = 
				realloc(dest->data, (int)(dest->size/SZ_BYTE) + (int)(src->size/SZ_BYTE) + BSTR_DATA_SZ);
			if(dest->data == NULL){
				print_error("bitstring_append: Could not allocate space!\n");
				exit(1);
			}
		}
		memcpy(dest->data + (int)(dest->size/SZ_BYTE), src->data, (int)(src->size/SZ_BYTE));
		appended_bits += (src->size/SZ_BYTE)*SZ_BYTE;
		dest->size += appended_bits;
	}
	/*Take care of the bits that weren't bit aligned*/
	for(int i=appended_bits; i<src->size; i++){
		bitstring_append_bit(dest, bitstring_get_bit(src, i));
	}
	return dest;
}

void bitstring_print(FILE* fp, pbitstring_t pbs){
	fprintf(fp, "[BITSTRING]\n");
	fprintf(fp, "\tsize: %d", pbs->size);

	for(int i=0; i<pbs->size; i++){
		if(i%SZ_BYTE == 0){
			fprintf(fp, "\n\t0b");
		}
		fprintf(fp, "%d", bitstring_get_bit(pbs, i));
	}
	fprintf(fp, "\n\t0x");
	for(int i=0; i<pbs->size; i+=SZ_BYTE){
		fprintf(fp, "%.2x", pbs->data[i/SZ_BYTE]);
	}
	fprintf(fp, "\n");
}

/*
@NOTE: for practical examples there isn't much need for numbers
that are bigger than 64 bits. So we'll keep it down and assume
that they are max native long sized. In the future if there is
a need for such a feature we'll introduce a dependencie lib such
as http://gmplib.org/ which deals with bignums.
Exceptions: crypto
*/

/*
Method is independent of data type you wish to convert byte order to.
For example it can be short long ,float or double...
*/
void conv_byte_order(void* pval, unsigned int size, char byte_order){
	if(byte_order == get_native_byte_order()){
		return; //no need for conversion
	}

	if(size%SZ_BYTE != 0){
		return; //there's no byte order concept for non byte sized data
	}
	
	char *pold_val = (char*)malloc(size/SZ_BYTE);
	if(pold_val == NULL){
		print_error("conv_byte_order: Could not allocate space!\n");
		exit(1);
	}
	memcpy(pold_val, pval, size/SZ_BYTE);
	char* pold = (char*)pold_val;
	char* pnew = (char*)pval + size/SZ_BYTE - 1;
	
	for(int i=0; i<size/SZ_BYTE; i++){
		*pnew = *pold;
		pnew--;
		pold++;
	}
}

/*Converts bit order in a byte - if needed*/
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

char calc_leading_zeroes(uint_t val, unsigned int size){
	char count = 0;
	for(int i=size-1; i>=0; i--){
		if(!((1<<i) & val)){
			count++;
		}else{
			break;
		}
	}
	printf("val: %.8x size: %d count: %d\n", val, size, count);
	return count;
}

/*ORD_NONE - for bit oriented values rather than byte*/
pbitstring_t bitstring_new_uint(
		uint_t val, unsigned int size, 
		char byte_order, char bit_order){
	
	unsigned int size_bytes = size/SZ_BYTE;
	size_bytes = size%SZ_BYTE?size_bytes+1:size_bytes;
	
	pbitstring_t pbs = bitstring_new();
	pbs->size = size;
	
	/*align the value in uint_t sized (for later casting) memory container*/
	char *psized_val = (char*)malloc(sizeof(uint_t));
	if(psized_val == NULL){
		print_error("bitstring_new_uint: Could not allocate space!\n");
		exit(1);
	}
	if(ORD_NAT_BYTE == ORD_LE){
		memcpy(psized_val, &val, size_bytes);
	}else //ORD_NAT_BYTE == ORD_BE
	{
		memcpy(psized_val, ((char*)&val) + sizeof(uint_t) - size_bytes, size_bytes);
	}
	
	
	if(byte_order != ORD_NONE){ //byte oriented (byte unit)
		conv_byte_order(psized_val, size, byte_order); //TODO: conv_byte_and_bit_order()
		for(int i=0; i<size_bytes; i++){
			conv_bit_order(psized_val+i, bit_order);
		}
	}else{ //bit oriented (bit unit)
		/* 
			The algorithm for serialization to bit oriented object
			val = 0x12345 - serialize to bit oriented of size=21
				and bit_order:
					ORD_BE --> 0b0000 1001 | 0001 1010 | 0010 1 - 000
					ORD_LE --> 0x54 0x32 0b1000 0b0
			Step 1: determine bit/byte memory layout of our value
			Step 2: determine bit memory layout of serialized object
					- count leading zeroes that must be stripped (not all)
			Step 3: transfom one into another
		*/
		if(ORD_NAT_BIT == ORD_BE){ 
			if(ORD_NAT_BYTE == ORD_BE){
				printf("psized_val: 0x%.2x 0x%.2x 0x%.2x 0x%.2x\n", psized_val[0],psized_val[1],psized_val[2],psized_val[3]);
				//psized_val == 0x01 | 0x23 | 0x45 - 0x00 - byte/bit memory layout
				if(bit_order == ORD_BE){
					char leading_zeroes = 
						calc_leading_zeroes(psized_val[0], 8) - calc_leading_zeroes(val, size);	printf("leading_zeroes: %d\n", leading_zeroes);
					//leading_zeroes == 3
					*((uint_t*)psized_val) = *((uint_t*)psized_val) << leading_zeroes; printf("psized_val: 0x%.2x 0x%.2x 0x%.2x 0x%.2x\n", psized_val[0],psized_val[1],psized_val[2],psized_val[3]);
					//0b0000 1001 | 0001 1010 | 0010 1 - 000
				}
				else{ //bit_order == ORD_LE
					for(int i=0; i<size_bytes; i++){
						conv_bit_order(psized_val+i, ORD_LE);
					}
					//psized_val == 0x21 0x43 0x50 | 0x00
					conv_byte_order(psized_val, size_bytes*SZ_BYTE, ORD_LE);
					//psized_val == 0x50 0x43 0x21 | 0x00
					char first_byte = psized_val[0];
					//first_byte == 0x50
					uint_t temp = *((uint_t*)psized_val);
					//temp == 0x50432100
					temp = 
						first_byte << (sizeof(uint_t)*SZ_BYTE-SZ_BYTE) //0x50000000
						| 
						temp << (SZ_BYTE - size%SZ_BYTE); //0x04321000
					//temp = 0x50000000 | 0x04321000 == 0x54321000
					memcpy(psized_val, &temp, size_bytes);
					//psized_val == 0x54 0x32 0x10 | 0x00
				}
			}else{ //ORD_NAT_BYTE == ORD_LE
			
			}	
		}else{ //ORD_NAT_BIT == ORD_LE
			//TODO: pain in the arss
		}
	}
	
	bitstring_set(pbs, psized_val, size);
		
	return pbs;
}

pbitstring_t bitstring_new_sint(
		sint_t val, unsigned int size, 
		char byte_order, char bit_order){
	
	return bitstring_new_uint((uint_t)val, size, byte_order, bit_order);
}

/*@NOTE: Consider byte/bit ordering with fps?*/
pbitstring_t bitstring_new_fp(
		fp_t val, unsigned int size, char fp_rep){
	
	if(fp_rep != FP_IEEE754){
		print_warning("Only IEEE754 fp representation is supported!\n");
	}

	pbitstring_t pbs = bitstring_new();
	pbs->size = size;
	
	if(size == 32){
		float new_val = (float)val;
		bitstring_set(pbs, &new_val, sizeof(float)*SZ_BYTE);
	}else if(size == 64){
		double new_val = (double)val;
		bitstring_set(pbs, &new_val, sizeof(double)*SZ_BYTE);
	}else{
		print_warning("Only 32b/64b fps are supported\n");
	}
	
	return pbs;
}

pbitstring_t bitstring_set(pbitstring_t pbs, void* data, unsigned int size){
	unsigned int size_bytes = size/SZ_BYTE;
	size_bytes = size%SZ_BYTE?size_bytes+1:size_bytes;

	if(size_bytes > pbs->alloc_size_bytes){
		pbs->data = realloc(pbs->data, size_bytes + BSTR_DATA_SZ);
		if(pbs->data == NULL){
			print_error("bitstring_set: Could not allocate space!\n");
			exit(1);
		}
		pbs->alloc_size_bytes = size_bytes + BSTR_DATA_SZ;
	}
	memcpy(pbs->data, data, size_bytes);
	
	return pbs;
}

