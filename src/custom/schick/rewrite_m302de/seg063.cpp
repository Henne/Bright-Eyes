/*
        Rewrite of DSA1 v3.02_de functions of seg063 (harbour)
        Functions rewritten: 2/5
*/

#include "schick.h"

#include "v302de.h"

#include "seg007.h"

namespace M302de {

void passages_init()
{
	Bit8u *p;
	Bit16u si, i;

	p = MemBase + PhysMake(datseg, 0x6f00);
	for (i = 0; i < 45; p += 8, i++) {
		host_writeb(p + 4,
			(unsigned char)random_interval(0, host_readb(p + 3)));
		host_writeb(p + 7, (unsigned char)random_interval(70, 130));

		si = random_schick(100);
		if (host_readb(p + 5) == 0) {
			/* Hochseerouten */
			if (si <= 50) {
				host_writeb(p + 6, 0);
			} else if (si <= 80) {
				host_writeb(p + 6, 1);
			} else if (si <= 95) {
				host_writeb(p + 6, 2);
			} else {
				host_writeb(p + 6, 3);
			}
		} else {
			if (si <= 10) {
				host_writeb(p + 6, 4);
			} else if (si <= 40) {
				host_writeb(p + 6, 5);
			} else if (si <= 80) {
				host_writeb(p + 6, 6);
			} else {
				host_writeb(p + 6, 7);
			}
		}

	D1_LOG("%s - %s: %d %d %d %d %d %d\n",
		(char*)get_ltx((host_readb(p + 0) + 0xeb) * 4),
		(char*)get_ltx((host_readb(p + 1) + 0xeb) * 4),
		host_readb(p + 2),
		host_readb(p + 3),
		host_readb(p + 4),
		host_readb(p + 5),
		host_readb(p + 6),
		host_readb(p + 7));
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

}

