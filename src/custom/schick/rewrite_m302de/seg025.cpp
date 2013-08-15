/**
 *	Rewrite of DSA1 v3.02_de functions of seg025 (locations)
 *	Functions rewritten: 6/15
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
#include "seg026.h"
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

	ds_writew(TYPEINDEX, ds_readbs(CURRENT_TOWN));

	ds_writeb(CURRENT_TOWN, 0);
	ds_writeb(LOCATION, 0);
	ds_writeb(TRAVELING, 1);
}


/* 0xd54 */
/**
 * show_storytext() - show storytexts
 *
 * Returns: 1 if dialog was shown / 0 if had already been shown
 * These were introduced in V3.00 (de and en) to find better into the story.
 */
/* static */
signed short show_storytext(void)
{
	Bit8u *ptr;
	signed short person, icon;

	load_buffer_1(0x120);

	person = random_schick(17) - 1;

	ptr = get_dtp(person * 4);

	switch (person) {

		case 0:	icon = 0x3f; break;
		case 1: icon = 0x44; break;
		case 2: icon = 0x44; break;
		case 3: icon = 0x45; break;
		case 4: icon = 0x43; break;
		case 5: icon = 0x46; break;
		case 6: icon = 0x41; break;
		case 7: icon = 0x42; break;
		case 8: icon = 0x40; break;
		case 9: icon = 0x46; break;
		case 10: icon = 0x40; break;
		case 11: icon = 0x45; break;
		case 12: icon = 0x43; break;
		case 13: icon = 0x40; break;
		case 14: icon = 0x43; break;
		case 15: icon = 0x41; break;
		case 16: icon = 0x44; break;
		default: break;
	}

	if (!ds_readbs(0x43a6 + person)) {
		GUI_dialog_na(icon, ptr);
		ds_writeb(0x43a6 + person, 1);
		return 1;
	}

	return 0;
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
	ds_writew(X_TARGET, ds_readw(0x2d83));
	ds_writew(Y_TARGET, ds_readw(0x2d85));

	/* recalc direction */
	ds_writeb(DIRECTION, (ds_readb(DIRECTION) + 2) % 4);

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
