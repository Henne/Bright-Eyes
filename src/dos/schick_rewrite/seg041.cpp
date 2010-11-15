#include <string.h>

#include "../schick.h"

void seg041_1fd() {
	memset(MemBase + PhysMake(datseg, 0xd333), 0 , 20);
}

void seg041_218(unsigned short v1, unsigned short v2) {
	ds_writew(0xd333 + 4 * ds_readw(0x26ad), v1);
	ds_writew(0xd333 + 4 * ds_readw(0x26ad) + 2 , v2);
	if (ds_readw(0x26ad) < 4)
		ds_writew(0x26ad, ds_readw(0x26ad) + 1);
}
