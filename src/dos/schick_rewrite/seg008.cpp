/*
        Rewrite of DSA1 v3.02_de functions of segment 008 (Rasterlib)
        Functions rewritten: 10/14
*/

#include "mem.h"
#include "paging.h"

#include "../../ints/int10.h"

unsigned short swap_u16(unsigned short val) {
	return (val << 8) | (val >> 8);
}

void set_video_mode(unsigned char mode) {
	INT10_SetVideoMode(mode);
}

void set_video_page(unsigned char mode) {
	INT10_SetActivePage(mode);
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

void save_rect(PhysPt src, PhysPt dest, unsigned short width, unsigned short height) {
	for (; height; height--) {
		mem_memcpy(dest, src, width);
		src += 320;
		dest += width;
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

void decomp_rle(unsigned short width, unsigned short height, Bit8u *dst,
	Bit8u *src, Bit8u *tmp_buffer, unsigned short mode) {

	Bit8u *my_dst;
	unsigned short i,x;
	unsigned char tmp;
	unsigned char cnt;

	/* select destination buffer */
	if (mode == 5 || mode == 4)
		my_dst = tmp_buffer;
	else
		my_dst = dst;

	do {
		/* decode one line */
		x = width;
		do {
			tmp = *src++;
			if (tmp == 0x7f) {
				cnt = *src++;
				tmp = *src++;
				for (i = 0; i < cnt; i++)
					*my_dst++ = tmp;
				x -= cnt;
			} else {
				*my_dst++ = tmp;
				x--;
			}
		} while (x);

		/* reverse line */
		if (mode == 5 || mode == 4) {
			my_dst--;
			for (i = width; i; i--)
				*dst++ = *my_dst--;
		}

		/* set destination to next line */
		switch (mode) {
		case	5:	break;
		case	3:	dst += width;
				break;
		case	2:	dst += 320;
				break;
		case	4:	dst += 320 - width;
				break;
		}

	} while (--height);
}
