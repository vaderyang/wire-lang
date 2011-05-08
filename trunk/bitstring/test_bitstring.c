#include "bitstring.h"
#include <string.h>
#include <stdio.h>

int main(){
	
	bitstring_lib_init();

	/*INT TEST, BYTE/BIT ORDERING*/
	fprintf(stdout, "INT TEST, BYTE/BIT ORDERING\n");

	pbitstring_t pbs1;
	pbitstring_t pbs2;
	
	for(int i=20; i<25; i++){
		pbs1 = bitstring_new_uint(0x12345, i, ORD_NONE, ORD_BE);
		bitstring_print(stdout, pbs1);
		bitstring_del(pbs1);
	}


	for(int i=20; i<25; i++){
		pbs1 = bitstring_new_uint(0x12345, i, ORD_NONE, ORD_LE);
		bitstring_print(stdout, pbs1);
		bitstring_del(pbs1);
	}
	
	
	pbs1 = bitstring_new_uint(-2, 13, ORD_NONE, ORD_BE);
	bitstring_print(stdout, pbs1);
	bitstring_del(pbs1);
	
	pbs1 = bitstring_new_uint(-2, 13, ORD_NONE, ORD_LE);
	bitstring_print(stdout, pbs1);
	bitstring_del(pbs1);
	
	/*
	pbs1 = bitstring_new_uint(0x1234, SZ_SHORT, ORD_LE, ORD_BE);
	bitstring_print(stdout, pbs1);
	pbs1 = bitstring_new_uint(0x1234, SZ_SHORT, ORD_BE, ORD_BE);
	bitstring_print(stdout, pbs1);
	pbs1 = bitstring_new_uint(0x1234, SZ_SHORT, ORD_LE, ORD_LE);
	bitstring_print(stdout, pbs1);
	pbs1 = bitstring_new_uint(0x1234, SZ_SHORT, ORD_BE, ORD_LE);
	bitstring_print(stdout, pbs1);
	*/
	// /*FLOATING POINT TEST*/
	// fprintf(stdout, "FP TEST:\n");
	// pbs1 = bitstring_new_fp(3.14, 32, FP_IEEE754);
	// bitstring_print(stdout, pbs1);
	// fprintf(stdout, "%f\n", *((float*)pbs1->data));
	// pbs1 = bitstring_new_fp(3.14, 64, FP_IEEE754);
	// bitstring_print(stdout, pbs1);
	// fprintf(stdout, "%f\n", *((double*)pbs1->data));
	
	// /*APPEND AND ALLOC TEST*/
	// fprintf(stdout, "APPEND AND ALLOC TEST\n");
	// pbs2 = bitstring_new_uint(1, 13, ORD_LE, ORD_BE);
	// for(int i=0; i<100; i++){
		// bitstring_append(pbs1, pbs2);
		//bitstring_print(stdout, pbs1);
	// }
	
	// pbs1 = bitstring_new_uint(255, 13, ORD_LE, ORD_BE);
	// bitstring_print(stdout, pbs1);
	// pbs1 = bitstring_new_uint(255, 13, ORD_LE, ORD_LE);
	// pbs1->size = 32;
	// bitstring_print(stdout, pbs1);
	
	// /*APPEND BIT*/
	// pbs1 = bitstring_new_uint(255, 8, ORD_LE, ORD_LE);
	// bitstring_print(stdout, pbs1);
	// bitstring_append_bit(pbs1, 1);
	// bitstring_print(stdout, pbs1);
	// bitstring_append_bit(pbs1, 0);
	// bitstring_print(stdout, pbs1);
	// bitstring_append_bit(pbs1, 1);
	// bitstring_print(stdout, pbs1);
	// bitstring_append_bit(pbs1, 0);
	// bitstring_print(stdout, pbs1);

	
	//bitstring_del(pbs1);
	//bitstring_del(pbs2);
	return 0;
}
