#include <serdes.h>
#include <bitstring.h>

int main(){
	pserdes_t psd = serdes_new();
	serdes_push_uint(psd, 1,13, ORD_LE, ORD_BE);
	serdes_push_sint(psd, -1,13, ORD_LE, ORD_BE);
	serdes_push_fp(psd, 3.14,SZ_LONG, ORD_LE, ORD_BE);
	return 0;
}
