/**
 *	Rewrite of DSA1 v3.02_de functions of seg025 (locations)
 *	Functions rewritten: 5/15
 */

#include <string.h>
#include <stdio.h>

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x483 */
/**
 * enter_map() -
 */
void enter_map(void)
{
	ds_writew(0x4334, ds_readw(TYPEINDEX));

	ds_writew(TYPEINDEX, (signed char)ds_readb(CURRENT_TOWN));

	ds_writeb(CURRENT_TOWN, 0);
	ds_writeb(LOCATION, 0);
	ds_writeb(TRAVELING, 1);
}

/* 0xea9 */
/**
 * turnaround() - turn around in a pseudo 3d-view
 */
void turnaround(void)
{
	set_var_to_zero();

	/* reset location */
	ds_writeb(LOCATION, ds_readb(0x2d9f));

	/* set target  coordinates*/
	ds_writew(0x2d44, ds_readw(0x2d83));
	ds_writew(0x2d46, ds_readw(0x2d85));

	/* recalc direction */
	ds_writeb(0x2d3d, (ds_readb(0x2d3d) + 2) % 4);

	set_to_ff();

	ds_writeb(0x45b8, 1);
	ds_writew(0x2846, 1);

}

/* 0x100a */
/**
 * tumult() - party gets 1W6 LE damage
 */
void tumult(void)
{
	signed short bak;

	bak = ds_readw(0xbffd);
	ds_writew(0xbffd, 7);

	/* print message */
	GUI_output(get_ltx(0xbf0));

	/* each hero in the group looses 1W6 LE */
	sub_group_le(random_schick(6));

	if (ds_readb(CURRENT_TOWN) == 0x27 || ds_readb(CURRENT_TOWN) == 0x12 ||
		ds_readb(CURRENT_TOWN) == 0x1 || ds_readb(CURRENT_TOWN) == 0x11) {

			/* the guards */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0xbf4),
				(char*)get_ltx(0xbf8));
	} else {
			/* a mob */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0xbf4),
				(char*)get_ltx(0xbfc));
	}

	GUI_output(Real2Host(ds_readd(DTP2)));

	ds_writew(0xbffd, bak);
}

/* 0x10b3 */

/**
 * fade_into() - fade when leaving a location
 */
void fade_into(void)
{
	Bit8u *ptr;
	signed short i;

	ptr = Real2Host(ds_readd(0xd303)) + 0xfa00;

	memset(Real2Host(ds_readd(0xd303)), 0, 0xc0);

	wait_for_vsync();

	set_palette(Real2Host(ds_readd(0xd303)), 0x80, 0x40);

	for (i = 0; i < 0x20; i++) {

		pal_fade(ptr, Real2Host(ds_readd(0xd303)));

		pal_fade(ptr, Real2Host(ds_readd(0xd303)));

		wait_for_vsync();

		set_palette(ptr, 0, 0x20);
	}


}

/* 0x114a */
void copy_palette(void)
{
	memcpy(Real2Host(ds_readd(0xd303)) + 0xfa00, Real2Host(ds_readd(0xce3b)), 0x60);
	ds_writeb(0x4475, 2);
}

#if !defined(__BORLANDC__)
}
#endif
