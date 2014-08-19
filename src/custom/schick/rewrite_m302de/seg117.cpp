/**
 *	Rewrite of DSA1 v3.02_de functions of seg117 (2xTravelevent, hunt and helpers)
 *	Functions rewritten: 4/16
 */

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg047.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* static */
/* Borlandified and identical */
void pause_traveling(signed short ani_nr)
{

	ds_writeb(0xe5d2, 1);

	load_ani(ani_nr);

	draw_main_screen();

	init_ani(0);

	load_city_ltx(0xf0);

	ds_writew(0xe5d9, ds_readw(0x2ca2));
	ds_writew(0xe5d7, ds_readw(0x2ca4));
	ds_writew(0xe5d5, ds_readw(0xbffd));
	ds_writew(0xe5d3, ds_readw(0xe113));

	ds_writeb(0xb132, 1);

	/* c = b = a = 0 */
	ds_writeb(TRAVELING, ds_writew(0x2ca2, ds_writew(0xe113, 0)));

	ds_writew(0x2ca4, ani_nr == 21 ? 60: 70);
	ds_writew(0xbffd, 9);
}

/* static */
/* Borlandified and identical */
void resume_traveling(void)
{
	ds_writew(0x2ca2, ds_readw(0xe5d9));
	ds_writew(0x2ca4, ds_readw(0xe5d7));
	ds_writew(0xbffd, ds_readw(0xe5d5));
	ds_writew(0xe113, ds_readw(0xe5d3));

	set_var_to_zero();

	ds_writew(0x2846, ds_writeb(TRAVELING, 1));

	ds_writeb(0xe5d2, 0);
	ds_writeb(0xb132, 0);
}

/* Borlandified and identical */
void hunt_karen(void)
{
	signed short answer;
	Bit8u *hero;
	signed short passed;
	signed short i;

	pause_traveling(21);

	do {
		answer = GUI_radio(get_city(0x00), 2, get_city(0x04), get_city(0x08));
	} while (answer == -1);

	if (answer == 1) {
		/* check for a hunting weapon, BOWS, CROSSBOWS or SPEAR */
		if ((get_first_hero_with_item(9) != -1) ||
			(get_first_hero_with_item(19) != -1) ||
			(get_first_hero_with_item(12) != -1) ||
			(get_first_hero_with_item(5) != -1))
		{

			hero = get_hero(0);
			/* make a STEALTH+2 test and count the heroes who passed it */
			for (i = passed = 0; i <= 6; i++, hero += 0x6da) {

				if ((host_readbs(hero + 0x21) != 0) &&
					(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)) &&
					!hero_dead(hero) &&
					(test_skill(hero, 13, 2) > 0))
				{
					passed++;
				}
			}

			if (count_heroes_in_group() <= passed) {
				/* all heros passed STEALTH */

				GUI_output(get_city(0x14));

				/* make a MISSLE WEAPON+0 test and count the heroes who passed it */
				hero = get_hero(0);
				for (i = passed = 0; i <= 6; i++, hero += 0x6da) {

					if ((host_readbs(hero + 0x21) != 0) &&
						(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)) &&
						!hero_dead(hero) &&
						(test_skill(hero, 7, 0) > 0))
					{
						passed++;
					}
				}

				if ((count_heroes_in_group() / 2) <= passed) {
					/* over the half of the group passed MISSLE WEAPON+0 */

					GUI_output(get_city(0x20));
					/* get 80 FOOD PACKAGES */
					get_item(45, 1, 80);

				} else if (passed) {
					/* at least one of the group passed MISSLE WEAPON+0 */

					GUI_output(get_city(0x1c));
					/* get 40 FOOD PACKAGES */
					get_item(45, 1, 40);
				} else {
					/* everybody failed MISSLE WEAPON+0 */
					GUI_output(get_city(0x18));
				}
			} else {
				/* at least one hero failed STEALTH+2 */
				GUI_output(get_city(0x10));
			}
		} else {
			/* no hunting weapon in the group */
			GUI_output(get_city(0x24));
		}
	}

	resume_traveling();
}

/* should be static */
void do_snake_attack(void)
{
	signed short choosen_hero;
	Bit8u *hero_i;
	signed short l_di;
	signed short i;

	/* load snake picture */
	pause_traveling(30);

	GUI_output(get_city(0x64));

	hero_i = get_hero(0);

	for (i = l_di = 0; i <= 6; i++, hero_i += 0x6da) {

		/* hero is invalid */
		if (host_readb(hero_i + 0x21) == 0)
			continue;

		/* hero is not in current group */
		if (host_readb(hero_i + 0x87) != ds_readb(CURRENT_GROUP))
			continue;

		/* hero is dead */
		if (hero_dead(hero_i))
			continue;

		/* Original-Bug: something was forgotten */
		/* check GE+0 */
		if (test_attrib(hero_i, 4, 0) < l_di) {
			/* remember the hero */
			choosen_hero = i;
		}
	}

	if (l_di) {

		/* select the chosen hero */
		hero_i = get_hero(choosen_hero);

		/* print a message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0x68),
			hero_i + 0x10);

		GUI_output(Real2Host(ds_readd(DTP2)));

		/* hero gets 2 AP */
		add_hero_ap(hero_i, 2);

		if (random_schick(100) <= 50) {
			/* hero gets bitten */

			/* hero gets 3 AP */
			add_hero_ap(hero_i, 3);

			/* and 2 * 3W6 damage */
			sub_hero_le(hero_i, dice_roll(3, 6, 0) * 2);
		}
	} else {
		GUI_output(get_city(0x6c));
	}

	resume_traveling();
}

#if !defined(__BORLANDC__)
}
#endif

