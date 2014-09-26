/*
        Rewrite of DSA1 v3.02_de functions of seg063 (harbour)
        Functions rewritten: 2/5
*/

#include "v302de.h"

#include "seg007.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void passages_init(void)
{
	signed short si;
	signed short i;
	Bit8u *p = p_datseg + 0x6f00;


	for (i = 0; i < 45; p += 8, i++) {

		host_writeb(p + 4, (unsigned char)random_interval(0, host_readbs(p + 3)));
		host_writeb(p + 7, (unsigned char)random_interval(70, 130));

		si = random_schick(100);

		if (!host_readbs(p + 5)) {

			/* Hochseerouten */
			host_writeb(p + 6, si <= 50 ? 0 : (si <= 80 ? 1 : (si <= 95 ? 2 : 3)));

		} else {

			/* Seerouten */
			host_writeb(p + 6, si <= 10 ? 4 : (si <= 40 ? 5 : (si <= 80 ? 6 : 7)));
		}

#if !defined(__BORLANDC__)
	D1_LOG("%16s - %16s: %d %d %d %d %d %d\n",
		(char*)get_ltx((host_readb(p + 0) + 0xeb) * 4),
		(char*)get_ltx((host_readb(p + 1) + 0xeb) * 4),
		host_readb(p + 2),
		host_readb(p + 3),
		host_readb(p + 4),
		host_readb(p + 5),
		host_readb(p + 6),
		host_readb(p + 7));
#endif
	}
}

unsigned short get_srout_len(Bit8u *ptr)
{
	unsigned short i = 0;

	while (host_readw(ptr) != 0xffff) {
		ptr += 4;
		i++;
	}
	return i;
}

#if !defined(__BORLANDC__)
}
#endif
