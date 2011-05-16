#include <bitstring.h>

struct serdes{
	pbitstring_t pbs;
	unsigned int pos; //start index 0
	};
typedef struct serdes serdes_t;
typedef serdes_t* pserdes_t;

pserdes_t serdes_new();
/*
@desc Sets the bitstring (copy) and the initial position.
*/
void serdes_set(pserdes_t psd, pbitstring_t pbs, unsigned int pos);
void serdes_del(pserdes_t psd);

void serdes_push_bitstring(pserdes_t psd, pbitstring_t pbs);
void serdes_push_uint(pserdes_t psd, uint_t val, unsigned int size, char byte_order, char bit_order);
void serdes_push_sint(pserdes_t psd, sint_t val, unsigned int size, char byte_order, char bit_order);
void serdes_push_fp(pserdes_t psd, fp_t val, unsigned int size, char fp_rep, char byte_order, char bit_order);

pbitstring_t serdes_pull_bitstring(pserdes_t psd, unsigned int size);
uint_t serdes_pull_uint(pserdes_t psd, unsigned int size, char byte_order, char bit_order);
sint_t serdes_pull_sint(pserdes_t psd, unsigned int size, char byte_order, char bit_order);
fp_t serdes_pull_fp(pserdes_t psd, unsigned int size, char fp_rep, char byte_order, char bit_order);
