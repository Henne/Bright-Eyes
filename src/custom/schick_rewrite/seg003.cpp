#include "mem.h"

#include "schick.h"

short update_direction(unsigned char mod) {
	unsigned char dir = ds_readb(0x2d3d);

	ds_writeb(0x2d7c, dir);
	ds_writeb(0x2d3d, (dir+mod) & 0x3);
	ds_writeb(0xbd4f, 0x1);
	return -1;
}

