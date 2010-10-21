/*
        Rewrite of DSA1 v3.02_de functions of segment 008 (Rasterlib)
        Functions rewritten: 3/14
*/

#include "mem.h"

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
