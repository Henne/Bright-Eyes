/*
 *	Rewrite of DSA1 v3.02_de functions of seg003 (movement)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg003.cpp
 *
*/

#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg008.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
#include "t_map.h"
#endif


#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if 0
/* DS:NOWAY_STRING */
static const char no_way_msg[][41] = {
	"IN DIESE RICHTUNG GEHT ES NICHT WEITER.",
	"YOU CAN GO NO FURTHER IN THIS DIRECTION."
};
#endif

signed short update_direction(unsigned char mod)
{
	/* save old direction */
	ds_writeb(DIRECTION_BAK, ds_readb(DIRECTION));
	/* set new direction */
	ds_writeb(DIRECTION, (ds_readb(DIRECTION) + mod) & 0x3);
	/* set bogus variable to 1 */
	ds_writeb(DIRECTION_UNKN, 0x1);

	return -1;
}

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


	p1 = p2 = p_datseg + DNG_MAP;

	/* direction */

	p3 = Real2Host(RealMake(datseg, ((ds_readb(DIRECTION) == 0) ? 0x4970 :
				((ds_readb(DIRECTION) == 1) ? 0x49ae :
				((ds_readb(DIRECTION) == 2) ? 0x49ec : 0x4a2a)))));

	for (i = 0; i < 29; i++, p3 += 2) {
		v2 = 0;
		x = ds_readws(X_TARGET) + host_readbs(p3);
		y = ds_readw(Y_TARGET) + host_readbs(p3 + 1);

		if (x < 0) {
			x = 0;
			v2 = 1;
		} else {
			if (ds_readb(DNG_MAP_SIZE) - 1 < x) {
				x = ds_readb(DNG_MAP_SIZE) - 1;
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

		v3 = (ds_readb(DNG_MAP_SIZE) == 0x10) ?
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

	if (ds_readb(DNG_MAP_SIZE) == 0x10) {
		/* dungeon mode */
		ds_writeb(STEPTARGET_FRONT, host_readb(p1 +
			((ds_readw(Y_TARGET) + host_readbs(p3 + 1)) << 4) +
			ds_readw(X_TARGET) + host_readbs(p3)));

		ds_writeb(STEPTARGET_BACK, host_readb(p1 +
			((ds_readw(Y_TARGET) + host_readbs(p3 + 3)) << 4) +
			ds_readw(X_TARGET) + host_readbs(p3 + 2)));
	} else {
		/* city mode */
		ds_writeb(STEPTARGET_FRONT, host_readb(p2 +
			((ds_readw(Y_TARGET) + host_readbs(p3 + 1)) << 5) +
			 ds_readw(X_TARGET) + host_readbs(p3)));

		ds_writeb(STEPTARGET_BACK, host_readb(p2 +
			((ds_readw(Y_TARGET) + host_readbs(p3 + 3)) << 5) +
			ds_readw(X_TARGET) + host_readbs(p3 + 2)));
	}
}

void door_frame(signed short nr, signed short x, signed short y, signed short frame)
{
	signed short width;
	signed short height;
	signed short l1;
	Bit8u *p1;
	Bit8u *p2;
	struct nvf_desc nvf;

	nvf.dst = Real2Host(F_PADD(ds_readd(BUFFER9_PTR3), 0x2e248));
	nvf.src = Real2Host(ds_readd(BUFFER9_PTR3));
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

	p1 = Real2Host(F_PADD(F_PADD(ds_readd(BUFFER9_PTR3), frame * width), 0x2e248));

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

		p2 = Real2Host(ds_readd(RENDERBUF_PTR)) + 208 * y + x;

		copy_solid(p2, p1, width, height, 208, l1, 0);
	}
}

/**
 * \brief	looting of a corpse
 * \param	chest_ptr	a pointer to the chest description
 * \param	text		output text
 * \param	flag		to mark this chest as done
 */
void loot_corpse(RealPt chest_ptr, Bit8u *text, Bit8u *flag)
{
	signed short answer;

	sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
		(char*)(!host_readbs(flag) ? get_ttx(0x82c) : get_ttx(0x83c)),
		text);

	if (!host_readbs(flag))
	{
		ds_writew(TEXTBOX_WIDTH, 7);

		answer = GUI_radio(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), 3,
					get_ttx(0x830),
					get_ttx(0x834),
					get_ttx(0x838)) - 1;

		ds_writew(TEXTBOX_WIDTH, 3);

		if (answer == 0)
		{
			/* examine the corpse */
#if !defined(__BORLANDC__)
			(t_map(chest_ptr, 11))(chest_ptr);
#else
			((void(*)(Bit8u*))(Real2Host(host_readd(Real2Host(chest_ptr) + 0xb))))(chest_ptr);
#endif

			if (!host_readbs(flag))
			{
				/* mark this corpse as done */
				host_writeb(flag, 1);
				/* Boron - 20 */
				sub_ds_ds(GODS_ESTIMATION + 4 * 5, 20);
			}

		} else if (answer == 1)
		{
			/* Borons Blessing */
			if (!host_readbs(flag))
			{
				/* mark this corpse as done */
				host_writeb(flag, 1);
				/* Boron + 20 */
				add_ds_ds(GODS_ESTIMATION + 4 * 5, 20);
			}
		}
	} else {
		GUI_output(Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
	}
}

void no_way(void)
{
	GUI_output(p_datseg + NOWAY_STRING);
}

#if !defined(__BORLANDC__)
}
#endif
