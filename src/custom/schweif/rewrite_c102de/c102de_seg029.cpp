/*
 *	Rewritr of DSA2 c1.02_de functions seg029 (decomp_pp20)
 *	Functions rewritten 1/1 (complete)
 */

#include "paging.h"

#include "schweif.h"

#include "c102de_seg033.h"

namespace C102de {

static inline unsigned int val(const unsigned char *p) {
	return (p[0]<<16 | p[1] << 8 | p[2]);
}

static unsigned long depackedlen(const unsigned char *p, unsigned long plen) {
	if (p[0] == 'P' && p[1] == 'P' && p[2] == '2' && p[3] == '0')
		return val(p+plen-4);

	if (host_readd((Bit8u*)p) == plen)
		return val(p+plen-4);

	if (host_readd((Bit8u*)p) + 8 == plen)
		return val(p+plen-4);
	return 0; /* not a powerpacker file */
}

unsigned int decomp_pp20(Bit8u *dst, Bit8u *src, unsigned int plen) {
	size_t unplen;

	if (plen < 4)
		D2_ERR("PP20: Length argument is below 4\n");

	unplen = depackedlen(src, plen);

	if (unplen == 0) {
		D2_ERR("PP20: No PP20 file\n");
	}
	ppDecrunch(&src[8],  dst, &src[4], plen - 12, unplen, src[plen -1]);

	return unplen;
}

}
