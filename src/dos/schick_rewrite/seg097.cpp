#include "../schick.h"

#include "seg096.h"

//000
//129
//15e
char GUI_lookup_char_height(char c, Bit8u *p) {
	short i;
	Bit8u *tab = MemBase + PhysMake(datseg, 0xab42);

	for (i = 0; i != 210; i+=3) {
		if (tab[3*i] != c)
			continue;

		host_writew(p, tab[3*i+2] & 0xff);
		return tab[3*i+1] & 0xff;
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		host_writew(p, 0);
		return 0;
	} else {
		host_writew(p, 8);
		return 0;
	}
}
