/*
        Rewrite of DSA1 v3.02_de functions of segment 008 (Rasterlib)
        Functions rewritten: 13/14
*/

#include "paging.h"

#include "../../ints/int10.h"

#include "../schick.h"

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

void pic_copy(PhysPt dst, short x1, short y1, short x2, short y2,
	unsigned short val1, unsigned short val2,
	unsigned short val3, unsigned short val4,
	unsigned short src_width, unsigned short src_height,
	Bit8u *src, unsigned short mode) {

	signed short ds_3, ds_1, ds_2, ds_4;
	short cur_height, cur_width;
	signed short lv1, lv2, lv3, lv4, lv5;

	cur_width = src_width;
	cur_height = src_height;

	lv1 = 0;
	lv2 = 0;
	lv3 = 0;
	lv4 = 0;

	ds_1 = ds_readw(0x2990);
	ds_2 = ds_readw(0x2992);
	ds_3 = ds_readw(0x2994);
	ds_4 = ds_readw(0x2996);

	if (y1 < ds_1) {
		lv3 = ds_1 - y1;
		cur_height -= lv3;
		src += lv3 * src_width;
	}

	if (y2 > ds_3) {
		lv4 = y2 - ds_3;
		cur_height -= lv4;
	}

	if (x1 < ds_2) {
		lv1 = ds_2 - x1;
		cur_width -= lv1;
		src += lv1;
	}


	if (x2 > ds_4) {
		lv2 = x2 - ds_4;
		cur_width -= lv2;
	}

	if (cur_height < 0)
		return;

	if (cur_width < 0)
		return;

	dst += ds_1 * 320 + ds_2;

	if (y1 > ds_1)
		dst += (y1 - ds_1) * 320;

	if (x1 > ds_2)
		dst += x1 - ds_2;

	lv5 = 0;
	lv5 = src_width - cur_width;

	switch (mode) {
	case 1: {
		short bx, cols, lines;

		lines = cur_height;
		bx = 320 - cur_width;
		do {
			cols = cur_width;
			if (real_readw(datseg, 0x4a92)) {
				while (cols--) {
					/* TODO 357 - 37d only check dst in
					special positions */
					if (mem_readb_inline(dst) < 0xc8)
						mem_writeb_inline(dst, *src);

				src++;
				dst++;
				}
			} else {
				while (cols--) {
					if (mem_readb_inline(dst) < 0xc8)
						mem_writeb_inline(dst, *src);
					src++;
					dst++;
				}
			}
			dst += bx;
			src += lv5;
		} while (--lines > 0);

		break;
	}
	case 2: {
		short bx, lines, cols;

		lines = cur_height;
		bx = 320 - cur_width;

		do {
			cols = cur_width;
			do {
				if (*src != 0)
					mem_writeb_inline(dst, *src);
				src++;
				dst++;
			} while (--cols);
			dst += bx;
			src += lv5;
		} while (--lines > 0);

		break;
	}
	case 3: {
		short bx, lines, cols;

		src += val2 * 320 + val1;

		lines = cur_height;
		bx = 320 - cur_width;
		do {
			for (cols = cur_width; cols; cols--)
				mem_writeb_inline(dst++, *src++);
			dst += bx;
			src += bx;
		} while (--lines > 0);
		break;
	}
	default: {
		short bx, lines, cols;
		lines = cur_height;
		bx = 320 - cur_width;
		do {
			for (cols = cur_width; cols; cols--)
				mem_writeb_inline(dst++, *src++);
			dst += bx;
			src += lv5;
		} while (--lines > 0);
	}
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

void copy_solid_permuted(Bit8u *dst, Bit8u *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid) {

	Bit8u *s, *d;
	unsigned short y,x;

	s = src;
	d = dst;
	for (y = 0; y < height; y++, s = src += src_width, d = dst += dst_width)
		for (x = width_to_copy; x ; x--) {
			if (*s != solid)
				*d = *s;
			d++;
			s--;
		}
}

void copy_solid(Bit8u *dst, Bit8u *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid) {

	Bit8u *s, *d;
	unsigned short y,x;

	s = src;
	d = dst;
	for (y = 0; y < height; y++, s = src += src_width, d = dst += dst_width)
		for (x = width_to_copy; x ; x--) {
			if (*s != solid)
				*d = *s;
			s++;
			d++;
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
