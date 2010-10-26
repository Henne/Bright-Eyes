#include "paging.h"

#include "../schick.h"

#include "seg096.h"

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

//8c5
void GUI_set_something() {
	PhysPt ptr = Real2Phys(real_readd(datseg, 0xce87));
	D1_INFO("0x%05x\n", ptr);
	unsigned char i,j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++)
			mem_writeb_inline(ptr + j, real_readb(datseg, 0xd2c7));
		ptr += 8;
	}
}

//8f8
//958
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
