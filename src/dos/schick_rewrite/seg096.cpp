#include "paging.h"

#include "../schick.h"

#include "seg096.h"
//330
/**
*/
RealPt GUI_get_ptr(unsigned short v1, unsigned short v2) {

	if (v1 == 0) {
		switch (v2) {
			case 0:
				return RealMake(datseg, 0xa9f3);
			case 1:
				return RealMake(datseg, 0xa9fd);
			case 3:
				return RealMake(datseg, 0xaa0a);
			default:
				return RealMake(datseg, 0xaa06);
		}
	} else {
		switch (v2) {
			case 0:
				return RealMake(datseg, 0xa9f6);
			case 1:
				return RealMake(datseg, 0xaa02);
			case 3:
				return RealMake(datseg, 0xaa02);
			default:
				return RealMake(datseg, 0xa9f6);
		}
	}
}

//394
/**
*/
RealPt GUI_get_ptr2(unsigned short v1, unsigned short v2) {

	if (v1 == 0) {
		switch (v2) {
			case 0:
				return RealMake(datseg, 0xa8d4);
			case 1:
				return RealMake(datseg, 0xa8e0);
			case 3:
				return RealMake(datseg, 0xa8e8);
			default:
				return RealMake(datseg, 0xa8e4);
		}
	} else {
		switch (v2) {
			case 0:
				return RealMake(datseg, 0xa8d8);
			case 1:
				return RealMake(datseg, 0xa8d4);
			case 3:
				return RealMake(datseg, 0xa8d4);
			default:
				return RealMake(datseg, 0xa8d4);
		}
	}
}
//3f8
void GUI_write_char_to_screen(PhysPt dst, unsigned short char_width, unsigned short char_height) {
	Bit8u *ptr;
	unsigned short y,x;

	ptr = MemBase + PhysMake(datseg, 0xce87);

	for (y = 0; y < char_width; ptr += 8 - char_height, dst += 320, y++) {
		for (x = 0; x < char_height; x++)
			mem_writeb_inline(dst+x, *ptr++);
	}
}
//442
/**
	GUI_count_lines - return the number of lines a string needs on a screen
*/
unsigned short GUI_count_lines(Bit8u *str) {
	Bit8u *str_loc;
	unsigned short v6, v8;
	short si, di;
	unsigned short max_line_width, width_char, width_line;
	unsigned short ax;

	v8 = 0;

	if (str == NULL)
		return 0;

	/* replace all CR and CL with Whitespaces */
	for (str_loc = str; *str_loc; str_loc++)
		if (*str_loc == 0x0a || *str_loc == 0x0d)
			*str_loc = 0x20;

	str_loc = str;
	si = di = v6 = 0;
	max_line_width = real_readw(datseg, 0xd2d5);

	ax = real_readw(datseg, 0xe4db);
	if (ax != 0)
		real_writew(datseg, 0xd2d5, real_readw(datseg, 0xd2d5) - ax);
	width_line = 0;

	while (str_loc[si]) {
		GUI_lookup_char(str_loc[si], &width_char);
		width_line += width_char;

		if (width_line >=  real_readw(datseg, 0xd2d5)) {
			if ( di != v6) {
				str_loc[di] = 0x0d;
				str_loc += di;
			} else {
				str_loc[si] = 0x0d;
				str_loc += si + 1;
			}
			if (++v8 == real_readw(datseg, 0xe4d9))
				real_writew(datseg, 0xd2d5, real_readw(datseg, 0xd2d5) + real_readw(datseg, 0xe4db));

		v6 = si = di = width_line = 0;
		}

		if (str_loc[si] == 0x20)
			di = si;

		if (str_loc[si] == 0x40) {
			str_loc = str_loc + si + 1;
			si = -1;
			v6 = di = width_line = 0;
			if (++v8 == real_readw(datseg, 0xe4d9))
				real_writew(datseg, 0xd2d5, real_readw(datseg, 0xd2d5) + real_readw(datseg, 0xe4db));
		}

		si++;
	}

	if (width_line >= real_readw(datseg, 0xd2d5)) {

		if (v6 == di)
			str_loc[si - 1] = 0;
		else {
			str_loc[di] = 0x0d;
			if (++v8 == real_readw(datseg, 0xe4d9))
				real_writew(datseg, 0xd2d5, real_readw(datseg, 0xd2d5) + real_readw(datseg, 0xe4db));
		}
	}

	real_writew(datseg, 0xd2d5, max_line_width);
	return ++v8;
}
//7f0
unsigned short GUI_print_char(char c, unsigned short x, unsigned short y) {
	unsigned short char_width, font_index;

	ds_writeb(0xe4d8, c);
	font_index = GUI_lookup_char(c, &char_width);
	D1_INFO("GUI_lookup_char(%c); w=%d, fi=%d\n", c, char_width, font_index);
	GUI_write_fonti_to_screen(font_index, char_width, x, y);

	return char_width;
}
//82b
unsigned short GUI_lookup_char(char c, unsigned short *p){
	unsigned short i;

	for (i = 0; i != 75*3; i += 3) {
		if (c != real_readb(datseg, 0xaa51 + i))
			continue;

		host_writew((Bit8u*)p, real_readb(datseg, 0xaa51 + i + 2) & 0xff);
		return real_readb(datseg, 0xaa51 + i + 1) & 0xff;
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		host_writew((Bit8u*)p, 0);
		return 0;
	} else {
		host_writew((Bit8u*)p, 5);
		return 0;
	}
}
//88f
void GUI_write_fonti_to_screen(unsigned short font_index, unsigned short char_width, unsigned short x, unsigned short y) {
	PhysPt p_font6 = ds_readd(0xd2c1);

	D1_INFO("GUI_write_fonti_to_screen(fi=%d, cw=%d,x=%d, y=%d)\n", font_index, char_width, x, y);
	GUI_blank_char();
	GUI_font_to_buf(MemBase + Real2Phys(p_font6) + font_index * 8);
	GUI_write_char_to_screen_xy(x, y, 7, char_width);
}
//8c5
/**
	GUI_blank_char() - sets the area of a char to a color
*/
void GUI_blank_char() {
	PhysPt ptr = PhysMake(datseg, 0xce87);
	unsigned char color = ds_readb(0xd2c7);
	unsigned char i,j;

	D1_INFO("ptr = 0x%x color = 0x%x\n", ptr, color);

	for (i = 0; i < 8; ptr += 8, i++) {
		for (j = 0; j < 8; j++)
			mem_writeb_inline(ptr + j, color);
	}
}

//8f8
void GUI_font_to_buf(Bit8u *fc) {
	Bit8u * p;
	char c;
	short i, j;

	/* current text position */
	p = MemBase + PhysMake(datseg, 0xce87);

	if (ds_readb(0xe4d8) == 0x3a)
		fc++;

	for (i = 0; i < 8; p += 8, i++) {
		c = *fc++;
		for (j = 0; j < 8; j++)
			if ((0x80 >> j) & c)
				 p[j] = ds_readb(0xd2c9 + ds_readw(0xd2c5) * 2);
	}
}

//956
void GUI_write_char_to_screen_xy(unsigned short x, unsigned short y, unsigned short char_height, unsigned short char_width) {
	/* screen_start */
	PhysPt dst = Real2Phys(ds_readd(0xd2fb));

	dst += y * 320 + x;
	GUI_write_char_to_screen(dst, char_height, char_width);
}

void GUI_set_smth(unsigned short v1, unsigned short v2) {
	real_writew(datseg, 0xd2c9, v1);
	real_writew(datseg, 0xd2c7, v2);
}

void GUI_get_smth(Bit8u *p1, Bit8u *p2) {
	host_writew(p1, real_readw(datseg, 0xd2c9));
	host_writew(p2, real_readw(datseg, 0xd2c7));
}

unsigned short GUI_unused(Bit8u *str) {
	unsigned short lines = 0;

	while (str) {
		if (*str++ == 0x0d)
			lines++;
	}

	return lines;
}

