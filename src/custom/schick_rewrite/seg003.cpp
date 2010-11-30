#include "mem.h"

#include "schick.h"

short update_direction(unsigned char mod) {
	unsigned char dir = real_readb(datseg, 0x2d3d);

	real_writeb(datseg, 0x2d7c, dir);
	real_writeb(datseg, 0x2d3d, (dir+mod) & 0x3);
	real_writeb(datseg, 0xbd4f, 0x1);
	return -1;
}

