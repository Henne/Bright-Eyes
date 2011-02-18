/*
	Rewrite of DSA1 Generator v1.05_de seg005 (Rasterlib)
	Functions rewritten: 1/2

	We only rewrite those which differ from the game.
*/

#include <stdlib.h>

#include "schick.h"

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
