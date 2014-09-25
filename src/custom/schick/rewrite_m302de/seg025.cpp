/**
 *	Rewrite of DSA1 v3.02_de functions of seg025 (locations)
 *	Functions rewritten: 9/15
 */

#include <string.h>
#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg049.h"
#include "seg075.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void show_entrance(void)
{
	draw_main_screen();
	set_var_to_zero();
	load_ani(34);
	init_ani(1);

	if (GUI_bool(get_ltx(0xbe0))) {

		init_ani_busy_loop(2);
		DNG_enter_dungeon(ds_readws(TYPEINDEX));
	} else {

		turnaround();
	}
}

/**
 * show_citizen() - the screen when entering a house in the city
 */
/* Borlandified and identical */
void show_citizen(void)
{

	ds_writew(0x2846, 1);

	do {
		handle_gui_input();
		if (ds_readw(0x2846) != 0) {

			draw_main_screen();
			set_var_to_zero();
			load_ani(20);
			init_ani(ds_writew(0x2846, 0));

			strcpy((char*)Real2Host((RealPt)ds_readd(0xd2eb)),
				(char*)get_dtp(ds_readw(CITYINDEX) * 4));

			if (ds_readbs(YEAR) == 15 && ds_readbs(MONTH) == 1 && random_schick(100) <= 20) {

				if (!show_storytext()) {
					GUI_print_loc_line(Real2Host((RealPt)ds_readd(0xd2eb)));
				} else {
					ds_writew(ACTION, 1);
				}
			} else {
				GUI_print_loc_line(Real2Host((RealPt)ds_readd(0xd2eb)));
#ifdef M302de_SPEEDFIX
				delay_or_keypress(200);
#endif
			}
		}

	} while ((ds_readw(ACTION) == 0) && (ds_readw(0xc3d5) == 0));

	ds_writew(0xc3d5, 0);
	set_var_to_zero();
	copy_palette();
	turnaround();
}

/**
 * \brief	break into a house
 */
/* Borlandified and identical */
void do_house(void)
{
	signed short i;
	signed short l_di;
	Bit8u *hero;

	/* prepare the question */
	strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(4 * ds_readws(CITYINDEX)));

	strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x9bc));

	ds_writew(0xe5ac, 1);

	if (GUI_bool(Real2Host(ds_readd(DTP2)))) {

		/* break into the house */

		set_var_to_zero();
		load_ani(10);
		init_ani(0);

		/* print a randomized text */
		GUI_output(get_ltx(4 * (random_schick(8) + 623)));

		hero = get_hero(0);

		for (i = 0; i < 6; i++, hero += 0x6da) {

			if ((host_readbs(hero + 0x21) != 0) &&
				(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)) &&
				!hero_dead(hero) &&
				(test_skill(hero, 17, -2) <= 0))
			{
				/* every hero must pass a sneak -2 test */

				i = ds_readbs(CURRENT_TOWN);

				if ((i == 1) || (i == 39) || (i == 18) || (i == 17)) {

					/* sneak test failed in a town with guards */

					GUI_output(get_ltx(0x9e0));

					l_di = 0;

					for (i = 0; i < 6; i++) {

						hero = get_hero(i);

						if (check_hero(hero) && !host_readbs(hero + 0x9f)) {
							l_di = 1;
						}
					}

					if ((ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP)) < ds_readbs(0x2d3c)) && l_di)
					{
						i = 0;

						while (host_readbs(get_hero(i) + 0x87) == ds_readbs(CURRENT_GROUP))
						{
							/* imprison hero */
							host_writeb(get_hero(i) + 0x9f, 1);
							i++;
						}

						GRP_switch_to_next(1);

					} else {

						if (ds_readds(DAY_TIMER) < HOURS(6)) {
							/* before 6:00 turn clock to 0:00 */
							timewarp_until(0);
						}

						timewarp_until(HOURS(6));

						GUI_output(get_ltx(0x9e4));
					}
				}
				break;
			}
		}

		set_var_to_zero();

		turnaround();

	} else {
		ds_writeb(LOCATION, ds_readb(0x2d9f));
		ds_writew(X_TARGET, ds_readw(0x2d83));
		ds_writew(Y_TARGET, ds_readw(0x2d85));
	}

}

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

void do_location(void)
{
	DUMMY_WARNING();
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
