#include "v302de.h"

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if 0
/* DS:0x4a68 - DS:0x4a7f */
static const char no_way_msg[][41] = {
	"IN DIESE RICHTUNG GEHT ES NICHT WEITER.",
	"YOU CAN GO NO FURTHER IN THIS DIRECTION."
};
#endif

signed short update_direction(unsigned char mod)
{
	unsigned char dir = ds_readb(DIRECTION);

	/* save old direction */
	ds_writeb(0x2d7c, dir);
	/* set new direction */
	ds_writeb(DIRECTION, (dir+mod) & 0x3);
	/* set bogus variable to 1 */
	ds_writeb(0xbd4f, 0x1);

	return -1;
}

void no_way()
{
	GUI_output(p_datseg + 0x4a68);
}

#if !defined(__BORLANDC__)
}
#endif
