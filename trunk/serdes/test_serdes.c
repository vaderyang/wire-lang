#include <serdes.h>
#include <bitstring.h>

int main(){
	pserdes_t psd_push = serdes_new();
	
	serdes_push_uint(psd_push, 0x12345678, SZ_LONG, ORD_LE, ORD_BE);
	serdes_push_uint(psd_push, 0x12345678, SZ_LONG, ORD_BE, ORD_BE);
	serdes_push_uint(psd_push, 0x12345678, SZ_LONG, ORD_LE, ORD_LE);
	serdes_push_uint(psd_push, 0x12345678, SZ_LONG, ORD_BE, ORD_LE);

	serdes_push_uint(psd_push, 0x12345, 21, ORD_NONE, ORD_LE);
	serdes_push_uint(psd_push, 0x12345, 21, ORD_NONE, ORD_BE);
	
	pserdes_t psd_pull = serdes_new();
	serdes_set(psd_pull, psd_push->pbs, 0);

	fprintf(stdout, "1: 0x%.8x\n", serdes_pull_uint(psd_pull, SZ_LONG, ORD_LE, ORD_BE));
	fprintf(stdout, "2: 0x%.8x\n", serdes_pull_uint(psd_pull, SZ_LONG, ORD_BE, ORD_BE));
	fprintf(stdout, "3: 0x%.8x\n", serdes_pull_uint(psd_pull, SZ_LONG, ORD_LE, ORD_LE));
	fprintf(stdout, "4: 0x%.8x\n", serdes_pull_uint(psd_pull, SZ_LONG, ORD_BE, ORD_LE));
	
	fprintf(stdout, "5: 0x%.8x\n", serdes_pull_uint(psd_pull, 21, ORD_NONE, ORD_LE));
	fprintf(stdout, "6: 0x%.8x\n", serdes_pull_uint(psd_pull, 21, ORD_NONE, ORD_BE));

	return 0;
}
