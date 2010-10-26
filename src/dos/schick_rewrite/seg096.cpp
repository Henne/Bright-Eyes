#include "../schick.h"

void GUI_set_smth(unsigned short v1, unsigned short v2) {
	real_writew(datseg, 0xd2c9, v1);
	real_writew(datseg, 0xd2c7, v2);
}

void GUI_get_smth(Bit8u *p1, Bit8u *p2) {
	host_writew(p1, real_readw(datseg, 0xd2c9));
	host_writew(p2, real_readw(datseg, 0xd2c7));
}

unsigned short GUI_count_lines(Bit8u *str) {
	unsigned short lines = 0;

	while (str) {
		if (*str++ == 0x0d)
			lines++;
	}

	return lines;
}
