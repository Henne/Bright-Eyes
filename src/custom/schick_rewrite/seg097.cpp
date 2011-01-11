/*
 *      Rewrite of DSA1 v3.02_de functions of seg097 (GUI)
 *      Functions called rewritten 3/16
 *
 *      Functions called rewritten 2/13
 *      Functions uncalled rewritten 1/3
*/

#include "schick.h"

#include "seg004.h"
#include "seg096.h"

//000
//129
//15e
char GUI_lookup_char_height(char c, unsigned short *p) {
	short i;
	Bit8u *tab = MemBase + PhysMake(datseg, 0xab42);

	for (i = 0; i != 210; i+=3) {
		if (c != ds_readb(0xab42 + i))
			continue;

		host_writew((Bit8u*)p, ds_readb(0xab42 + i + 2) & 0xff);
		return ds_readb(0xab42 + i + 1) & 0xff;
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		host_writew((Bit8u*)p, 0);
		return 0;
	} else {
		host_writew((Bit8u*)p, 8);
		return 0;
	}
}

//1c2
void GUI_1c2(unsigned short v1, unsigned short v2, RealPt v3) {

	GUI_blank_char();
	GUI_font_to_buf(MemBase + Real2Phys(ds_readd(0xd2bd + v1 * 8)));
	GUI_write_char_to_screen(v3, 8, v2);
}

//564
void GUI_copy_smth(unsigned short width, unsigned short height) {
	ds_writew(0xc011, ds_readw(0xbfff));
	ds_writew(0xc013, ds_readw(0xc001));
	ds_writew(0xc015, ds_readw(0xbfff) + width - 1);
	ds_writew(0xc017, ds_readw(0xc001) + height - 1);
	ds_writed(0xc019, ds_readd(0xbff9));
	do_pic_copy(0);
}
