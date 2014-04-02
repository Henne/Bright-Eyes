/*
 *	Rewrite of DSA1 v3.02_de functions of seg003 (movement)
 *	Functions rewritten: 3/5
 *
*/

#include "v302de.h"

#include "seg000.h"
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

/* Borlandified and identical */
signed short update_direction(unsigned char mod)
{
	/* save old direction */
	ds_writeb(0x2d7c, ds_readb(DIRECTION));
	/* set new direction */
	ds_writeb(DIRECTION, (ds_readb(DIRECTION) + mod) & 0x3);
	/* set bogus variable to 1 */
	ds_writeb(0xbd4f, 0x1);

	return -1;
}

/* Borlandified and identical */
void move(void)
{
	volatile signed short v2;
	unsigned char v3;
	volatile signed short i;
	Bit8u *p1;
	Bit8u *p2;
	Bit8u *p3;

	signed short x;
	signed short y;


	p1 = p2 = p_datseg + 0xbd95;

	/* direction */

	p3 = Real2Host(RealMake(datseg, ((ds_readb(0x2d3d) == 0) ? 0x4970 :
				((ds_readb(0x2d3d) == 1) ? 0x49ae :
				((ds_readb(0x2d3d) == 2) ? 0x49ec : 0x4a2a)))));

	for (i = 0; i < 29; i++, p3 += 2) {
		v2 = 0;
		x = ds_readws(0x2d44) + host_readbs(p3);
		y = ds_readw(0x2d46) + host_readbs(p3 + 1);

		if (x < 0) {
			x = 0;
			v2 = 1;
		} else {
			if (ds_readb(0xbd94) - 1 < x) {
				x = ds_readb(0xbd94) - 1;
				v2 = 1;
			}
		}

		if (y < 0) {
			y = 0;
			v2 = 1;
		} else {
			if (y > 15) {
				y = 15;
				v2 = 1;
			}
		}

		v3 = (ds_readb(0xbd94) == 0x10) ?
			/* dungeon */
			host_readb(p1 + (y << 4) + x) :
			/* city */
			host_readb(p2 + (y << 5) + x);

		if (v2 != 0) {
			ds_writeb(0xbd6e + i, ((v3 == 0xa0) || (v3 == 0xb0)) ? v3 : 0xb0);
		} else {
			ds_writeb(0xbd6e + i, v3);
		}
	}

	if (ds_readb(0xbd94) == 0x10) {
		/* dungeon mode */
		ds_writeb(0xbd4d, host_readb(p1 +
			((ds_readw(0x2d46) + host_readbs(p3 + 1)) << 4) +
			ds_readw(0x2d44) + host_readbs(p3)));

		ds_writeb(0xbd4e, host_readb(p1 +
			((ds_readw(0x2d46) + host_readbs(p3 + 3)) << 4) +
			ds_readw(0x2d44) + host_readbs(p3 + 2)));
	} else {
		/* city mode */
		ds_writeb(0xbd4d, host_readb(p2 +
			((ds_readw(0x2d46) + host_readbs(p3 + 1)) << 5) +
			 ds_readw(0x2d44) + host_readbs(p3)));

		ds_writeb(0xbd4e, host_readb(p2 +
			((ds_readw(0x2d46) + host_readbs(p3 + 3)) << 5) +
			ds_readw(0x2d44) + host_readbs(p3 + 2)));
	}
}

/* Borlandified and identical */
void no_way()
{
	GUI_output(p_datseg + 0x4a68);
}

#if !defined(__BORLANDC__)
}
#endif
