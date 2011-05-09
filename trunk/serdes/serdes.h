#include "bitstring.h"

struct serdes{
	pbitstring_t pbs;
	};
typedef struct serdes serdes_t;
typedef serdes_t* pserdes_t;

pserdes_t serdes_new(pbitstring_t pbs);
pserdes_t serdes_del(pbitstring_t pbs);

pbitstring_t serdes_push_uint(pserdes_t psd, uint_t val, unsigned int size, char byte_order, char bit_order);
pbitstring_t serdes_push_sint(pserdes_t psd, uint_t val, unsigned int size, char byte_order, char bit_order);
pbitstring_t serdes_push_fp(pserdes_t psd, fp_t val, unsigned int size, char fp_rep, char byte_order, char bit_order);

uint_t serdes_pull_uint(pserdes_t psd, unsigned int size, char byte_order, char bit_order);
sint_t serdes_pull_sint(pserdes_t psd, unsigned int size, char byte_order, char bit_order);
fp_t serdes_pull_fp(pserdes_t psd, unsigned int size, char fp_rep, char byte_order, char bit_order);
