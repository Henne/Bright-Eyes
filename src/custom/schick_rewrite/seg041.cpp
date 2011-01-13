#include <string.h>

#include "schick.h"

#include "v302de.h"

#include "seg007.h"

void seg041_1fd() {
	memset(MemBase + PhysMake(datseg, 0xd333), 0 , 20);
	ds_writew(0x26ad, 0);
}

void seg041_218(unsigned short v1, unsigned short v2) {
	ds_writew(0xd333 + 4 * ds_readw(0x26ad), v1);
	ds_writew(0xd333 + 4 * ds_readw(0x26ad) + 2 , v2);
	if (ds_readw(0x26ad) < 4)
		ds_writew(0x26ad, ds_readw(0x26ad) + 1);
}

void seg041_8c8() {
	unsigned short i;

	for (i = 0; i < 8; i++)
		memset(MemBase + PhysMake(datseg, 0xd8ce) + i * 0xf3, 0xffff, 0xf3);

//		That would be better
//		memset(MemBase + PhysMake(datseg, 0xd8ce), 0xffff, 0xf3 * 8);
}
