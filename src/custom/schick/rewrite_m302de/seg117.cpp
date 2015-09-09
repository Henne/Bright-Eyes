/**
 *	Rewrite of DSA1 v3.02_de functions of seg117 (travel events 9 / 10, hunt and helpers)
 *	Functions rewritten: 13/16
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg030.h"
#include "seg032.h"
#include "seg047.h"
#include "seg048.h"
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
	ds_writew(0xe5d5, ds_readw(TEXTBOX_WIDTH));
	ds_writew(0xe5d3, ds_readw(0xe113));

	ds_writeb(0xb132, 1);

	/* c = b = a = 0 */
	ds_writeb(TRAVELING, (unsigned char)ds_writew(0x2ca2, ds_writew(0xe113, 0)));

	ds_writew(0x2ca4, ani_nr == 21 ? 60: 70);
	ds_writew(TEXTBOX_WIDTH, 9);
}

/* static */
/* Borlandified and identical */
void resume_traveling(void)
{
	ds_writew(0x2ca2, ds_readw(0xe5d9));
	ds_writew(0x2ca4, ds_readw(0xe5d7));
	ds_writew(TEXTBOX_WIDTH, ds_readw(0xe5d5));
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

/* Borlandified and identical */
void hunt_wildboar(void)
{
	signed short answer;
	Bit8u *hero;
	signed short i;
	signed short passed;

	pause_traveling(25);

	do {
		answer = GUI_radio(get_city(0x28), 2, get_city(0x2c), get_city(0x30));
	} while (answer == -1);

	if (answer == 1) {
		/* check for a hunting weapon, BOWS, CROSSBOWS or SPEAR */
		if ((get_first_hero_with_item(9) != -1) ||
			(get_first_hero_with_item(19) != -1) ||
			(get_first_hero_with_item(12) != -1) ||
			(get_first_hero_with_item(5) != -1))
		{

			hero = get_hero(0);
			/* make a STEALTH+0 test and count the heroes who passed it */
			for (i = passed = 0; i <= 6; i++, hero += 0x6da) {

				if ((host_readbs(hero + 0x21) != 0) &&
					(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)) &&
					!hero_dead(hero) &&
					(test_skill(hero, 13, 0) > 0))
				{
					passed++;
				}
			}

			if (count_heroes_in_group() <= passed) {
				/* all heros passed STEALTH */

				GUI_output(get_city(0x3c));

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

					GUI_output(get_city(0x44));
					/* get 30 FOOD PACKAGES */
					get_item(45, 1, 30);

				} else {
					/* everybody failed MISSLE WEAPON+0 */
					GUI_output(get_city(0x40));
				}
			} else {
				/* at least one hero failed STEALTH+0 */
				GUI_output(get_city(0x38));
			}
		} else {
			/* no hunting weapon in the group */
			GUI_output(get_city(0x48));
		}
	}

	resume_traveling();
}

/* Borlandified and identical */
void hunt_cavebear(void)
{
	Bit8u *hero;
	signed short answer;
	signed short i;

	pause_traveling(32);

	do {
		answer = GUI_radio(get_city(0x4c), 2, get_city(0x50), get_city(0x54));
	} while (answer == -1);

	if (answer == 1) {

		/* print message */
		GUI_output(get_city(0x60));

		/* time to flee = 1.5 hour */
		timewarp(0x1fa4);

	} else {
		GUI_output(get_city(0x58));

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += 0x6da) {

			if ((host_readbs(hero + 0x21) != 0) &&
				(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)) &&
				!hero_dead(hero))
			{

				/* AT of the current weapon - (RS-BE / 2) <= 1W20 */
				if ((host_readbs(hero + 0x68 + host_readbs(hero + 0x78)) - (host_readbs(hero + 0x32) / 2)) <= random_schick(20))
				{
#if !defined(__BORLANDC__)
					D1_INFO("%-16s erhaelt 5 AP fuer eine gelungene Attacke.\n",
						(char*)(hero + 0x10));
#endif
					add_hero_ap(hero, 5);
				}

				/* PA of the current weapon - (RS-BE / 2) <= 1W20 */
				if ((host_readbs(hero + 0x6f + host_readbs(hero + 0x78)) - (host_readbs(hero + 0x32) / 2)) > random_schick(20))
				{
#if !defined(__BORLANDC__)
					D1_INFO("%-16s erhaelt 3 AP fuer eine misslungene Parade.\n",
						(char*)(hero + 0x10));
#endif

					add_hero_ap(hero, 3);
					sub_hero_le(hero, dice_roll(2, 6, 0));
				}
			}
		}

		GUI_output(get_city(0x5c));
	}

	resume_traveling();
}

/* Borlandified and identical */
/* should be static */
void hunt_viper(void)
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

		/* hero is valid */
		/* hero is in current group */
		/* hero is not dead */
		/* check GE+0 */
		/* Original-Bug: something was forgotten */
		if ((host_readb(hero_i + 0x21) != 0) &&
			(host_readb(hero_i + 0x87) == ds_readb(CURRENT_GROUP)) &&
			(!hero_dead(hero_i)) &&
			(test_attrib(hero_i, 4, 0) < l_di))
		{
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

/* Borlandified and identical */
void octopus_attack(void)
{
	signed short i;
	signed short hits;
	signed short tmp;
	char overboard[7];
	Bit8u *hero;

	hits = 0;
	pause_traveling(31);
	memset(overboard, 0, 7);

	GUI_output(get_city(0x70));
	GUI_output(get_city(0x74));

	do {
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += 0x6da) {

			if (host_readbs(hero + 0x21) != 0 &&
				host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				!overboard[i])
			{
				/* ORIGINAL-BUG: bad things happen only when the result of the test is 0 or -1.
				 * all other results are success.
				*/
				/* GE+0 */
				if (!(tmp = test_attrib(hero, 4, 0))) {
					/* strangling attack */

					add_hero_ap(hero, 5);
					sub_hero_le(hero, random_schick(6));
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x78),
						(char*)hero + 0x10);
					GUI_output(Real2Host(ds_readd(DTP2)));

				} else if (tmp == -1) {
					/* over board */

					add_hero_ap(hero, 20);
					sub_hero_le(hero, random_schick(6));
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x7c),
						(char*)hero + 0x10);
					GUI_output(Real2Host(ds_readd(DTP2)));

					if (test_skill(hero, 14, 0) <= 0) {
						sub_hero_le(hero, random_schick(6));
						overboard[i] = 1;
					}

				} else {
					/* chance to hit the beast */

					if (host_readbs(hero + 0x68 + host_readbs(hero + 0x78)) <= random_schick(20)) {
						hits++;
					}
				}
			}
		}
		/* ORIGINAL-BUG: infinite-loop if all heroes are overboard */
	} while (hits <= 5);

	sub_group_le(3);
	add_hero_ap_all(5);
	GUI_output(get_city(0x80));

	ds_writew(0x2ca2, ds_writew(0x2ca4, 0));
	status_menu(get_hero_index(Real2Host(get_first_hero_available_in_group())));
	resume_traveling();
}

/* Borlandified and identical */
void hunt_bison(void)
{
	signed short answer;
	Bit8u *hero;

	pause_traveling(29);

	do {
		answer = GUI_radio(get_city(0x84), 2, get_city(0x88), get_city(0x8c));

	} while (answer == -1);

	if (answer == 1) {
		timewarp(MINUTES(30));
		GUI_output(get_city(0x98));
	} else {
		GUI_output(get_city(0x90));

		hero = get_hero(get_random_hero());
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0x94),
			(char*)hero + 0x10);
		GUI_output(Real2Host(ds_readd(DTP2)));
		sub_hero_le(hero, random_schick(6));
		add_hero_ap(hero, 2);
	}

	resume_traveling();
}

/* Borlandified and identical */
void hunt_rhino(void)
{
	signed short answer;
	Bit8u *hero;

	pause_traveling(33);

	do {
		answer = GUI_radio(get_city(0x9c), 2, get_city(0xa0), get_city(0xa4));

	} while (answer == -1);

	if (answer == 1) {
		timewarp(MINUTES(30));
		GUI_output(get_city(0xb0));
	} else {
		GUI_output(get_city(0xa8));

		hero = get_hero(get_random_hero());
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0xac),
			(char*)hero + 0x10);
		GUI_output(Real2Host(ds_readd(DTP2)));
		sub_hero_le(hero, dice_roll(2, 6, 0));
		add_hero_ap(hero, 6);
	}

	resume_traveling();
}

/* Borlandified and identical */
void pirates_attack(void)
{
	pause_traveling(9);

	GUI_output(get_city(0xb4));
	GUI_output(get_city(0xb8));
	GUI_output(get_city(0xbc));

	ds_writew(0x5f16, random_interval(3, 8));
	ds_writew(0x26c1, 1);

	/* Fight: S001 */
	do_fight(251);

	ds_writew(0x2ca2, ds_writew(0x2ca4, 0));

	status_menu(get_hero_index(Real2Host(get_first_hero_available_in_group())));

	resume_traveling();
}

/* Borlandified and identical */
void do_wild8_fight(void)
{
	signed short bak1;
	signed short bak2;

	bak1 = ds_readws(0x2ca2);
	bak2 = ds_readws(0x2ca4);
	ds_writew(0xe5d3, ds_readws(0xe113));
	ds_writew(0x2ca2, 0);
	ds_writew(0x2ca4, 0);
	ds_writeb(TRAVELING, 0);

	ds_writew(0x5f16, random_interval(5, 10));
	ds_writew(0x26c1, 1);

	/* Fight: WILD8 */
	do_fight(250);

	ds_writew(0x4248, 0);
	ds_writeb(TRAVELING, 1);
	ds_writew(0x2ca2, bak1);
	ds_writew(0x2ca4, bak2);
}

/* Borlandified and identical */
void random_encounter(signed short arg)
{
	signed short l_si;
	signed short i;
	signed short randval;
	signed short bak1;
	signed short bak2;
	signed short bak3;

	l_si = 0;

	if (random_schick(100) > 33) {
		l_si = 1;
	}

	bak1 = ds_readws(0x2ca2);
	bak2 = ds_readws(0x2ca4);
	bak3 = ds_readws(0xe113);
	ds_writew(0x2ca2, 0);
	ds_writew(0x2ca4, 0);

	arg = ds_readb(0xb17d + arg);

	randval = random_schick(100);

	for (i = 0; i < 14; i++) {

		if ((ds_readb(0xb1b9 + 7 * i + arg) <= randval) && (ds_readb(0xb1b9 + 7 * i + arg) != 0)) {

			ds_writeb(TRAVELING, ds_writew(0xe113, 0));
			ds_writeb(0xb132, 1);
			ds_writew(0x26c1, 1);

			switch (i) {
				case 0: {
					if (!l_si) {
						ds_writew(0x5f16, random_schick(6) + 1);
						/* Fight: WILD1 */
						do_fight(243);
					}
					break;
				}
				case 1: {
					if (!l_si) {
						ds_writew(0x5f16, random_schick(3));
						/* Fight: WILD2 */
						do_fight(244);
					}
					break;
				}
				case 2: {
					hunt_karen();
					break;
				}
				case 3: {
					if (!l_si) {
						ds_writew(0x5f16, random_schick(2));
						/* Fight: WILD3 */
						do_fight(245);
					}
					break;
				}
				case 4: {
					hunt_viper();
					break;
				}
				case 5: {
					if (!l_si) {
						ds_writew(0x5f16, random_schick(6) + 1);
						/* Fight: WILD4 */
						do_fight(246);
					}
					break;
				}
				case 6: {
					if (!l_si) {
						ds_writew(0x5f16, random_schick(3) + 3);
						/* Fight: WILD4 */
						do_fight(246);
					}
					break;
				}
				case 7: {
					if (!l_si) {
						ds_writew(0x5f16, random_schick(2) + 1);
						/* Fight: WILD5 */
						do_fight(247);
					}
					break;
				}
				case 8: {
					hunt_rhino();
					break;
				}
				case 9: {
					hunt_wildboar();
					break;
				}
				case 10: {
					if (!l_si) {
						ds_writew(0x5f16, random_schick(3));
						/* Fight: WILD6 */
						do_fight(248);
					}
					break;
				}
				case 11: {
					hunt_bison();
					break;
				}
				case 12: {
					if (!l_si) {
						ds_writew(0x5f16, random_schick(3));
						/* Fight: WILD7 */
						do_fight(249);
					}
					break;
				}
				case 13: {
					hunt_cavebear();
					break;
				}
			}

			ds_writew(0x26c1, 0);
			ds_writeb(TRAVELING, 1);
			ds_writeb(0xb132, 0);
			break;
		}
	}

	ds_writew(0x2ca2, bak1);
	ds_writew(0x2ca4, bak2);
	ds_writew(0xe113, bak3);
	load_buffer_1(19);
}

/* Borlandified and identical */
void search_ruin1(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else
	do_talk(17, 0);
	set_var_to_zero();
#endif

}

void TLK_way_to_ruin(signed short state)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif

