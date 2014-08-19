/*
 *	Rewrite of DSA1 v3.02_de functions of seg003 (movement)
 *	Functions rewritten: 4/5
 *
*/

#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg008.h"
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
void door_frame(signed short nr, signed short x, signed short y, signed short frame)
{
	signed short width;
	signed short height;
	signed short l1;
	Bit8u *p1;
	Bit8u *p2;
	struct nvf_desc nvf;

	nvf.dst = Real2Host(F_PADD(ds_readd(0xd019), 0x2e248));
	nvf.src = Real2Host(ds_readd(0xd019));
	nvf.nr = nr;
	nvf.type = 3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	height -= frame;
	l1 = width;

	p1 = Real2Host(F_PADD(F_PADD(ds_readd(0xd019), frame * width), 0x2e248));

	if ((x < 0) && ((x + width) > 0)) {
		width += x;
		p1 += __abs__(x);
		x = 0;
	}

	if ((y < 0) && ((y + height) > 0)) {
		height -= y;
		p1 += __abs__(y) * l1;
		y = 0;
	}

	if ( (x < 208) && (y < 135) && (x >= 0) && (y >= 0)) {

		if (x + width > 208) {
			width = 208 - x;
		}

		if (y + height > 135) {
			height = 135 - y;
		}

		p2 = Real2Host(ds_readd(0xd303)) + 208 * y + x;

		copy_solid(p2, p1, width, height, 208, l1, 0);
	}
}

void loot_corpse(Bit8u *p1, Bit8u *p2, Bit8u *p3)
{
	signed short answer;

	sprintf((char*)Real2Host(ds_readd(0xd2eb)),
		(!host_readbs(p3)) ? (char*)get_ltx(0x82c) : (char*)get_ltx(0x83c),
		p2);

	if (!host_readbs(p3)) {

		ds_writew(0xbffd, 7);

		answer = GUI_radio(Real2Host(ds_readd(0xd2eb)), 3,
					get_ltx(0x830),
					get_ltx(0x834),
					get_ltx(0x838)) - 1;

		ds_writew(0xbffd, 3);

		if (answer == 0) {
#if !defined(__BORLANDC__)
			/* TODO */
#else
			((void(*)(Bit8u*))(Real2Host(host_readd(p1 + 0xb))))(p1);
#endif


			if (!host_readbs(p3)) {
				/* mark this corpse as: left alone */
				host_writeb(p3, 1);
				/* Boron - 20 */
				sub_ds_ds(0x3162, 20);
			}
		} else if (answer == 1) {
			if (!host_readbs(p3)) {
				/* mark this corpse as: left alone */
				host_writeb(p3, 1);
				/* Boron + 20 */
				add_ds_ds(0x3162, 20);
			}
		}
	} else {
		GUI_output(Real2Host(ds_readd(0xd2eb)));
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
