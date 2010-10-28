#include "../schick.h"

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
