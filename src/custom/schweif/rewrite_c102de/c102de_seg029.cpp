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
/*	DSA1/ROA1 doesn't use the first bytes as a signature "PP20".
 *	It's used instead for the lenght of the packed data. */

	if (p[0] == 'P' && p[1] == 'P' && p[2] == '2' && p[3] == '0')
		return val(p+plen-4);

	if (host_readd((Bit8u*)p) == plen)
		return val(p+plen-4);

	if (host_readd((Bit8u*)p) + 8 == plen)
		return val(p+plen-4);

	return 0; /* not a powerpacker file */
}

/*	DSA2 writes with this function direct to the graphics mem,
	so we unpack it to buf and then with mem_writeb to gfx mem */
static Bit8u buf[64000];

void decomp_pp20(RealPt dst, Bit8u *src, unsigned int plen)
{
	signed int unplen;
	signed int i;
	PhysPt p_dst = Real2Phys(dst);

	if (plen < 4)
		D2_ERR("PP20: Length argument is below 4\n");

	unplen = depackedlen(src, plen);

	if (unplen == 0) {
		D2_ERR("PP20: No PP20 file\n");
	}
	if (!ppDecrunch(&src[8],  buf, &src[4], plen - 12, unplen, src[plen - 1])) {
		D2_ERR("error on ppDecrunch();\n");
	}

	for (i = 0; i < unplen; i++)
		mem_writeb_inline(p_dst + i, buf[i]);

	return;
}

}
