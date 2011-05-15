#include <serdes.h>
#include <bitstring.h>

int main(){
	pserdes_t psd_push = serdes_new();
	
	for(int i=20; i<33; i++){
		serdes_push_uint(psd_push, 0x12345, i, ORD_NONE, ORD_BE);
	}
	for(int i=20; i<33; i++){
		serdes_push_uint(psd_push, 0x12345, i, ORD_NONE, ORD_LE);
	}
	serdes_push_uint(psd_push, 0x12345678, SZ_LONG, ORD_LE, ORD_BE);
	serdes_push_uint(psd_push, 0x12345678, SZ_LONG, ORD_BE, ORD_BE);
	serdes_push_uint(psd_push, 0x12345678, SZ_LONG, ORD_LE, ORD_LE);
	serdes_push_uint(psd_push, 0x12345678, SZ_LONG, ORD_BE, ORD_LE);

	
	pserdes_t psd_pull = serdes_new();
	serdes_set(psd_pull, psd_push->pbs, 0);

	for(int i=20; i<33; i++){
		fprintf(stdout, "%d -> 0x%.8x\n", i, serdes_pull_uint(psd_pull, i, ORD_NONE, ORD_BE));
	}
	
	for(int i=20; i<33; i++){
		fprintf(stdout, "%d -> 0x%.8x\n", i, serdes_pull_uint(psd_pull, i, ORD_NONE, ORD_LE));
	}
	
	fprintf(stdout, "1: 0x%.8x\n", serdes_pull_uint(psd_pull, SZ_LONG, ORD_LE, ORD_BE));
	fprintf(stdout, "2: 0x%.8x\n", serdes_pull_uint(psd_pull, SZ_LONG, ORD_BE, ORD_BE));
	fprintf(stdout, "3: 0x%.8x\n", serdes_pull_uint(psd_pull, SZ_LONG, ORD_LE, ORD_LE));
	fprintf(stdout, "4: 0x%.8x\n", serdes_pull_uint(psd_pull, SZ_LONG, ORD_BE, ORD_LE));

	return 0;
}
