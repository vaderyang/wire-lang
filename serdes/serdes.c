#include "serdes.h"
#include "utils.h"
#include <bitstring.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

pserdes_t serdes_new(){
	pserdes_t psd = (pserdes_t)malloc(sizeof(pserdes_t));
	if(psd == NULL){
		print_error("serdes_new: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}
	psd->pbs = NULL;
	psd->pos = 0;
	
	return psd;
}

void serdes_del(pserdes_t psd){
	free(psd);
}

void serdes_set(pserdes_t psd, pbitstring_t pbs, unsigned int pos){
	psd->pbs = pbs;
	psd->pos = pos;
}

void serdes_push_bitstring(pserdes_t psd, pbitstring_t pbs){
	if(psd->pbs == NULL){
		psd->pbs = bitstring_new();
	}
	
	bitstring_append(psd->pbs, pbs);
	psd->pos += pbs->size;
}

void serdes_push_uint(pserdes_t psd, uint_t val, unsigned int size, char byte_order, char bit_order){
	pbitstring_t pbs = bitstring_new_uint(val, size, byte_order, bit_order);
	serdes_push_bitstring(psd, pbs);
}

void serdes_push_sint(pserdes_t psd, sint_t val, unsigned int size, char byte_order, char bit_order){
	pbitstring_t pbs = bitstring_new_sint(val, size, byte_order, bit_order);
	serdes_push_bitstring(psd, pbs);
}

void serdes_push_fp(pserdes_t psd, fp_t val, unsigned int size, char fp_rep, char byte_order, char bit_order){
	pbitstring_t pbs = bitstring_new_fp(val, size, fp_rep, byte_order, bit_order);
	serdes_push_bitstring(psd, pbs);
}

pbitstring_t serdes_pull_bitstring(pserdes_t psd, unsigned int size){
	//pbs = 0x12 | 0x34 | 0x56 
	//pos = 7
	//size = 13
	unsigned int pos_end_byte = (psd->pos + size - 1)/SZ_BYTE; //end = 2
	unsigned int pos_start_byte = psd->pos/SZ_BYTE; //start = 0
	unsigned int size_bytes = pos_end_byte - pos_start_byte + 1; //size_bytes = 3

	pbitstring_t pbs = bitstring_new();
	
	char *pval_ser = (char*)malloc(sizeof(size_bytes));
	if(pval_ser == NULL){
		print_error("serdes_pull_bitstring: Could not allocate space!\n");
		exit(EXIT_FAILURE);
	}
	
	printf("pos: %d size: %d end: %d start: %d\n", psd->pos, size_bytes, pos_end_byte, pos_start_byte);
	memcpy(pval_ser, psd->pbs->data+pos_start_byte, size_bytes); 
	//pval_ser = 0x12 | 0x34 | 0x56
	shift_left(pval_ser, size_bytes, psd->pos%SZ_BYTE);
	//pval_ser = 0x1A | 0x2B | 0x00
	bitstring_set(pbs, pval_ser, size);

	free(pval_ser);
	
	psd->pos += size;
	
	print_debug("[PULL]");
	bitstring_print(stdout, pbs);
	return pbs;
}

uint_t serdes_pull_uint(pserdes_t psd, unsigned int size, char byte_order, char bit_order){
	unsigned int size_bytes = size/SZ_BYTE;
	size_bytes = size%SZ_BYTE?size_bytes+1:size_bytes;

	pbitstring_t pbs = serdes_pull_bitstring(psd, size);

	uint_t val = 0;

	if(byte_order != ORD_NONE){ //byte oriented (byte unit)
		conv_byte_bit_order(pbs->data, size_bytes, byte_order, bit_order);
	}else{ //bit oriented (bit unit)
		if(ORD_NAT_BIT == ORD_BE){ 
			if(ORD_NAT_BYTE == ORD_BE){
				if(bit_order == ORD_BE){
				}
				else{ //bit_order == ORD_LE
				}
			}else{ //ORD_NAT_BYTE == ORD_LE
				if(bit_order == ORD_BE){
					char shift_count = size_bytes*SZ_BYTE - size;
					shift_right_le(pbs->data, size_bytes, shift_count);
					conv_byte_order(pbs->data, size_bytes, ORD_BE); //reverse
				}else{//bit_order == ORD_LE
					//val_ser = 0b1010 0010 | 1100 0100 | 1000 0 - 000 val_ser_mem = 
					for(int i=0; i<size_bytes; i++){
						conv_bit_order(pbs->data+i, ORD_LE); //reverse
					}
				}
			}	
		}else{ //ORD_NAT_BIT == ORD_LE
			//TODO: implement deserialization for LE bit order machines (if exists one)
		}
	}
	
	val = *((uint_t*)pbs->data);
	bitstring_print(stdout, pbs);
	bitstring_del(pbs);		
	return val;
}

sint_t serdes_pull_sint(pserdes_t psd, unsigned int size, char byte_order, char bit_order){
	return 0;
}

fp_t serdes_pull_fp(pserdes_t psd, unsigned int size, char fp_rep, char byte_order, char bit_order){
	return 0;
}
