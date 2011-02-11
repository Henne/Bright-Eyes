#include "regs.h"

#include "../schick.h"

/* static */
RealPt get_gfx_ptr(Bit16u x, Bit16u y) {

	RealPt start;

	start = ds_readd(0x47c7) + 320 * y + x;

	return start;
}

/* static */
Bit16u ret_zero() {
	return 0;
}


/* static */
void set_vals(Bit16u v1, Bit16u v2) {
	ds_writew(0x4781, v1);
	ds_writew(0x477f, v2);
}

/* static */
void get_vals(Bit8u *p1, Bit8u *p2) {
	host_writew(p1, ds_readw(0x4781));
	host_writew(p2, ds_readw(0x477f));
}

/* static */
void clear_hero() {

	Bit16u i;

	ds_writew(0x2780, 0);
	ds_writew(0x2782, 0);

	ds_writeb(0x40b6, 0);
	ds_writeb(0x40b4, 0);
	ds_writeb(0x40b5, 0);
	ds_writeb(0x40b7, 0);

	for (i = 0; i < 14; i++)
		ds_writeb(0x4076 + i, 0);

	for (i = 0; i < 86; i++) {
		ds_writeb(0x3f63 + i * 2, 0);
		ds_writeb(0x3f62 + i * 2, 0);
	}
	for (i = 0; i < 52; i++) {
		ds_writeb(0x400f + i * 2, 0);
		ds_writeb(0x400e + i * 2, 0);
	}

	ds_writeb(0x1353, 1);
};
