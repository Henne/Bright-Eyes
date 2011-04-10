/*
	Rewrite of DSA1 Generator v1.05_de seg005 (Rasterlib)
	Functions rewritten: 1/2

	We only rewrite those which differ from the game.
*/

#include <stdlib.h>

#include "paging.h"

#include "schick.h"

namespace G105de {

void draw_pic(PhysPt dst, Bit16u x, Bit16u y, Bit32u dummy1,
		Bit16u v1, Bit16u v2, Bit32u dummy2,
		Bit16u w, Bit16u h, PhysPt src, Bit16u mode)
{
	PhysPt d, s;

	d = dst + y * 320 + x;
	s = src;

	switch (mode) {
		/* this is not used in GEN */
		case 1: {
			Bit16u diff, i;

			diff = 320 - w;
			do {
				for (i = w; i; i--) {
					if (mem_readb_inline(d) < 0xc8)
						mem_writeb_inline(d, mem_readb_inline(s));
					s++;
					d++;
				}
				d += diff;
			} while (--h > 0);
			break;
		}
		case 2: {
			Bit16u diff, i;
			Bit8u al;

			diff = 320 - w;

			do {
				for (i = w; i != 0; i--) {
					if (al = mem_readb_inline(s++))
						mem_writeb_inline(d, al);
					d++;
				}
				d += diff;
			} while (--h > 0);

			break;
		}
		case 3: {
			Bit16u diff, i;

			s += v2 * 320 + v1;
			diff = 320 - w;

			do {
				for (i = w; i > 0; i--)
					mem_writeb_inline(d++,
						mem_readb_inline(s++));
				d += diff;
				s += diff;
			} while (--h > 0);
			break;
		}
		default: {
			Bit16u diff, i;

			diff = 320 - w;

			do {
				for (i = w; i; i--)
					mem_writeb_inline(d++,
						mem_readb_inline(s++));
				d += diff;
			} while (--h > 0);

			break;
		}
	}
}

}

void copy_to_screen(PhysPt src, PhysPt dst, Bit16s w, Bit16s h, Bit16u mode)
{

	Bit16u v1, v2;
	Bit16s i, j;

	v1 = v2 = 320 - w;

	switch (mode & 0x7f) {
		case 0:	v2 = 0;
			break;
		case 2: v1 = 0;
			break;
	}

	if (mode & 0x80) {
		/* This does not happen */
		D1_ERR("%s mode bit set\n", __func__);
		exit(0);
	} else {
		for (; h; h--) {
			for (i = 0; i < w; i++)
				mem_writeb(dst++, mem_readb(src++));
			dst += v1;
			src += v2;
		}
	}
}
