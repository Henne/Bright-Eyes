/*
	Rewrite of DSA1 Generator v1.05_de seg005 (Rasterlib)
	Functions rewritten: 2/2

	We only rewrite those which differ from the game.
*/

#include <stdlib.h>

#include "paging.h"
#include "callback.h"

#include "../../ints/int10.h"

#include "schick.h"

namespace G105de {

void set_video_mode(unsigned char mode) {
	INT10_SetVideoMode(mode);
}

void set_video_page(unsigned char mode) {
	INT10_SetActivePage(mode);
}

void save_display_stat(RealPt p)
{
	CPU_Push32(p);
	CALLBACK_RunRealFar(reloc_gen + 0xb6b, 0x34);
	CPU_Pop32();
}

void set_color(Bit8u *ptr, unsigned char color){
	INT10_SetSingleDacRegister(color, ptr[0], ptr[1], ptr[2]);
}

void set_palette(Bit8u *ptr, unsigned char first_color, unsigned short colors){
	unsigned short i;
	for (i = 0; i < colors; i++)
		INT10_SetSingleDacRegister(first_color + i,
			ptr[i*3], ptr[i*3+1], ptr[i*3+2]);
}

void draw_h_line(PhysPt ptr, unsigned short count, unsigned char color) {
	unsigned short i;

	for (i = 0; i < count; i++)
		mem_writeb_inline(ptr + i, color);
}

void draw_h_spaced_dots(PhysPt ptr, unsigned short count, unsigned char color, unsigned short space) {
	unsigned short i;

	for (i = 0; i < count; i++) {
		mem_writeb_inline(ptr, color);
		ptr += space;
	}
}

void draw_pic(PhysPt dst, Bit16u x, Bit16u y, Bit16u d1, Bit16u d2,
		Bit16u v1, Bit16u v2, Bit16u d3, Bit16u d4,
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

void fill_rect(PhysPt ptr, unsigned char color, unsigned short width, unsigned short height) {
	unsigned short x;

	for (; height; height--) {
		for (x = 0; x < width; x++)
			mem_writeb_inline(ptr++ , color);
	ptr += 320 - width;
	}
}

unsigned short swap_u16(unsigned short val) {
	return (val << 8) | (val >> 8);
}

void copy_to_screen(Bit8u *src, PhysPt dst, Bit16s w, Bit16s h, Bit16u mode)
{

	Bit16u v1, v2;
	Bit16s i;

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
				mem_writeb(dst++, *src++);
			dst += v1;
			src += v2;
		}
	}
}

}
