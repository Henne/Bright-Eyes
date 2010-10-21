/*
        Rewrite of DSA1 v3.02_de functions of segment 008 (Rasterlib)
        Functions rewritten: 7/14
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

