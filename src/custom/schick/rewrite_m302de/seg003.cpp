#include "mem.h"

#include "schick.h"

#include "seg097.h"

namespace M302de {

/* DS:0x4a68 - DS:0x4a7f */
static const char no_way_msg[][41] = {
	"IN DIESE RICHTUNG GEHT ES NICHT WEITER.",
	"YOU CAN GO NO FURTHER IN THIS DIRECTION."
};

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

void no_way()
{
	GUI_output(Real2Host(RealMake(datseg, 0x4a68)));
}

}
