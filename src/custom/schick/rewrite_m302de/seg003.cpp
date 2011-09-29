#include "mem.h"

#include "schick.h"

namespace M302de {

signed short update_direction(unsigned char mod) {
	unsigned char dir = ds_readb(0x2d3d);

	/* save old direction */
	ds_writeb(0x2d7c, dir);
	/* set new direction */
	ds_writeb(0x2d3d, (dir+mod) & 0x3);
	/* set bogus variable to 1 */
	ds_writeb(0xbd4f, 0x1);

	return -1;
}

}
