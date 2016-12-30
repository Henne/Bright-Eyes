/*
 *	Rewrite of DSA1 v3.02_de functions of seg111 (travel events 3 / 10)
 *	Functions rewritten: 15/15 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg111.cpp
*/

#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg032.h"
#include "seg047.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void tevent_053(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 1) > 0 && !ds_readb(0x3dc6)) ||
		ds_readbs(0x3dc6) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(0x3dc6, 1);
	}

}

void tevent_054(void)
{
	TRV_found_inn(47, 68);
}

/* Path ??? */
void tevent_055(void)
{
	signed short season;

	season = get_current_season();

	/* only in autumn or spring */
	if (season == 1 || season == 3) {

		load_in_head(52);

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_city(0), 0);

		timewarp(HOURS(3));
	}
}

void tevent_056(void)
{
	TRV_found_inn(48, 69);
}

void tevent_057(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_city(0x04), 2, get_city(0x08), get_city(0x0c));

	} while (answer == -1);

	if (answer == 2) {

		if (test_skill(Real2Host(get_first_hero_available_in_group()), 28, 2) > 0) {

			timewarp(HOURS(2));

			GUI_output(get_city(0x10));

		} else {

			timewarp(HOURS(6));

			GUI_output(get_city(0x14));

			hero = get_hero(0);

			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					sub_hero_le(hero, random_schick(3));
				}
			}
		}
	} else {

		hero = get_hero(0);

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, 10, 3) <= 0)
			{

				/* Original-Bug: that condition does not make sense */
				if (get_first_hero_with_item(121) == -1 || get_first_hero_with_item(32) != -1)
				{

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x20),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_hero_le(hero, random_schick(9) + 3);

				} else {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x1c),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_hero_le(hero, random_schick(8));
				}
			}
		}

		GUI_output(get_city(0x18));
	}
}

void tevent_058(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 2) > 0 && !ds_readb(0x3dc7)) ||
		ds_readbs(0x3dc7) != 0)
	{
		TRV_found_replenish_place(0);
		ds_writeb(0x3dc7, 1);
	}

}

void tevent_059(void)
{
	signed short answer;

	if (TRV_enter_hut_question())
	{
		ds_writews(CAMP_INCIDENT, 0);
		ds_writeb(LOCATION, 6);
		do_location();
		ds_writeb(LOCATION, 0);
		ds_writews(CAMP_INCIDENT, -1);

		TRV_load_textfile(-1);

		load_in_head(10);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (Bit8u*)NULL,
						get_city(0x24), 2,
						get_city(0x28),
						get_city(0x2c));
		} while (answer == -1);

		if (answer == 1) {

			if (get_first_hero_with_item(92) != -1 || get_first_hero_with_item(91) != -1)
			{
				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (Bit8u*)NULL,
								get_city(0x30), 3,
								get_city(0x34),
								get_city(0x38),
								get_city(0x3c));
				} while (answer == -1);

				if (answer == 1) {
					GUI_dialog_na(0, get_city(0x40));
				} else if (answer == 2) {
					GUI_dialog_na(0, get_city(0x44));
				} else if (test_attrib(Real2Host(get_first_hero_available_in_group()), 2, 0) > 0) {
					GUI_dialog_na(0, get_city(0x44));
				} else {
					GUI_dialog_na(0, get_city(0x40));
				}

			} else {
				GUI_dialogbox((RealPt)ds_readd(DTP2), (Bit8u*)NULL,
						get_city(0x30), 1,
						get_city(0x48));

				GUI_dialog_na(0, get_city(0x44));
			}
		}

		ds_writeb(LOCATION, 6);
		do_location();
		ds_writeb(LOCATION, 0);
	}
}

void tevent_060(void)
{
	signed short answer;
	signed short i;
	signed short done;
	signed short nr_items;
	signed short has_magic_rope;
	Bit8u *hero;

	done = 0;
	load_in_head(57);

	do {
		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (Bit8u*)NULL,
						get_city(0x4c), 2,
						get_city(0x50),
						get_city(0x54));
		} while (answer == -1);

		if (answer == 1) {

			if (test_skill(Real2Host(get_first_hero_available_in_group()), 28, 4) > 0) {

				sub_group_le(1);

				timewarp(HOURS(3));

				GUI_dialogbox((RealPt)ds_readd(DTP2), (Bit8u*)NULL, get_city(0x58), 0);

				done = 1;
			} else {

				sub_group_le(1);

				timewarp(HOURS(4));

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (Bit8u*)NULL,
								get_city(0x60), 2,
								get_city(0x68),
								get_city(0x6c));
				} while (answer == -1);

				if (answer == 1) {

					timewarp(HOURS(1));

					hero = get_hero(0);

					for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
							!hero_dead(hero) &&
							test_skill(hero, 10, 0) <= 0)
						{
							sub_hero_le(hero, random_schick(10));
						}
					}

					GUI_dialogbox((RealPt)ds_readd(DTP2), (Bit8u*)NULL, get_city(0x74), 0);

					done = 1;
				} else {
					timewarp(HOURS(2));
				}
			}
		} else {


			if (test_skill(Real2Host(get_first_hero_available_in_group()), 28, 2) > 0) {

				timewarp(HOURS(3));

				GUI_dialogbox((RealPt)ds_readd(DTP2), (Bit8u*)NULL, get_city(0x5c), 0);

				done = 1;
			} else {

				sub_group_le(1);

				timewarp(HOURS(4));

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (Bit8u*)NULL,
								get_city(0x64), 2,
								get_city(0x6c),
								get_city(0x70));
				} while (answer == -1);

				if (answer == 2) {

					hero = get_hero(0);

					for (i = has_magic_rope = nr_items = 0; i <= 6 ; i++, hero += SIZEOF_HERO){

						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
							!hero_dead(hero))
						{
							nr_items += hero_count_item(hero, 121);
							nr_items += hero_count_item(hero, 32);

							if (host_readbs(hero + HERO_STAFFSPELL_LVL) >= 3)
							{
								has_magic_rope = 1;
							}
						}
					}

					if (nr_items >= 3) {

						sub_group_le(3);

						for (i = 0; i < 3; i++) {

							answer = get_item_pos(hero = get_hero(get_first_hero_with_item(121)), 121);
							if (answer == -1) {
								answer = get_item_pos(hero = get_hero(get_first_hero_with_item(32)), 32);
							}

							drop_item(hero, answer, 1);
						}

						timewarp(HOURS(1));

						GUI_dialog_na(0, get_city(0x80));

						done = 1;

					} else if (nr_items == 2 && has_magic_rope != 0) {

						sub_group_le(3);

						timewarp(HOURS(1));

						GUI_dialog_na(0, get_city(0x7c));

						done = 1;
					} else {

						GUI_dialog_na(0, get_city(0x78));

						timewarp(HOURS(2));

						GUI_dialog_na(0, get_city(0x84));
					}
				} else {
					timewarp(HOURS(2));
				}
			}
		}

	} while (done == 0);
}

/* entrance dungeon wolfcave */
void tevent_061(void)
{
	signed short answer;

	/* TODO: not needed here */
	answer = 0;

	if (test_skill(Real2Host(get_first_hero_available_in_group()), 51, 6) > 0 &&
		test_skill(Real2Host(get_first_hero_available_in_group()), 26, 3) > 0 &&
		!ds_readb(0x3dc8))
	{
		ds_writeb(0x3dc8, 1);

		/* TODO: is this correct ? */
		if (!TRV_follow_trail_question()) {

			GUI_output(get_city(0x88));
			GUI_output(get_city(0x8c));

			load_in_head(53);

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (Bit8u*)NULL,
							get_city(0x90), 2,
							get_city(0x94),
							get_city(0x98));
			} while (answer == -1);

			if (answer == 1) {
				ds_writeb(0x4333, 4);
			}
		}

	} else if (ds_readb(0x3dc8) != 0) {

		load_in_head(53);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (Bit8u*)NULL,
						get_city(0x9c), 2,
						get_city(0xa0),
						get_city(0xa4));
		} while (answer == -1);

		if (answer == 1) {
			ds_writeb(0x4333, 4);
		}
	}
}

/* dummy Orvil<->Skjal */
void tevent_062(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Reiseevent %d ist nicht implementiert\n", 62);
#endif
}

/* dead animal Orvil<->Skjal */
void tevent_063(void)
{
	signed short i;
	signed short max;
	signed short proof;
	signed short vomiter;
	Bit8u *hero;

	max = 9999;

	/* intro message */
	GUI_output(get_city(0xb8));

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
			!hero_dead(hero))
		{

			/* MU+0 */
			if ((proof = test_attrib(hero, 0, 0)) < max) {
				max = proof;
				vomiter = i;
			}
		}
	}

	hero = get_hero(vomiter);

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_city(0xbc),
		(char*)hero + HERO_NAME2);

	/* print who vomits */
	GUI_output(Real2Host(ds_readd(DTP2)));

	/* LE - 2 */
	sub_hero_le(hero, 2);

	i = get_free_mod_slot();

	/* MU -2 for 24 hours */
	set_mod_slot(i, DAYS(1), hero + HERO_MU, -2, (signed char)vomiter);

	/* outro message */
	GUI_output(get_city(0xc0));
}

/* Gorahs place */
void tevent_064(void)
{
	signed short l_si;
	signed short l_di;
	signed short i;
	signed short l3;
	signed short l4;
	Bit8u *hero;

	l3 = 0;
	l4 = 0;

	if (!ds_readb(0x3dc9) && ds_readb(QUEST_GORAH) != 0) {

		do {

			do {
				l_si = GUI_radio(l4 != 0 ? get_city(0x198) : get_city(0xc4), 3,
							get_city(0xc8),
							get_city(0xcc),
							get_city(0xd0));
			} while (l_si == -1);

			if (l_si == 1) {

				hero = get_hero(select_hero_ok_forced(get_ltx(0x4f4)));

				if (host_readbs(hero + HERO_TYPE) < 7) {
					GUI_output(get_ltx(0x528));
				} else {
					l3 = 1;
				}

				if (l3 == 0) {
					l3 = 1;
				} else {
					if (test_spell(hero, 84, 0) > 0) {

						sub_ae_splash(hero, 2);

						ds_writeb(0x3dd3, 1);

						GUI_output(get_city(0xd4));
					} else {
						sub_ae_splash(hero, 1);

						GUI_output(get_ltx(0x978));
					}
				}

			} else if (l_si == 2) {

				l3 = 1;

				GUI_output(get_city(0xd8));

				hero = get_hero(0);

				for (i = l_di = 0; i <= 6; i++, hero += SIZEOF_HERO) {

					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero) &&
						test_skill(hero, 13, 0) <= 0)
					{
						l_di++;
					}
				}

				if (l_di > 1) {
					ds_writeb(0x3dd3, 1);
				}

			} else {

				l3 = 1;

				do {
					l_si = GUI_radio(get_city(0xdc), 2,
							get_city(0xe0),
							get_city(0xe4));
				} while (l_si == -1);

				if (l_si == 2)  return;
			}

		} while (l3 == 0);

		do {
			l_si = GUI_radio(get_city(0xe8), 2,
					get_city(0xec),
					get_city(0xf0));
		} while (l_si == -1);

		if (l_si == 1) {

			if (!ds_readb(0x3dd3)) {
				GUI_output(get_city(0xf4));
			}

			GUI_output(get_city(0xf8));

			l_di = TRV_fight_event(FIGHTS_F064, 64);

			ds_writeb(0x3dd3, 1);

		} else {
			GUI_output(ds_readb(0x3dd3) != 0 ? get_city(0xf8) : get_city(0xfc));
			l_di = TRV_fight_event(FIGHTS_F064, 64);
		}

		if (!l_di) {
			ds_writeb(0x3dc9, 1);

			add_hero_ap_all(100);

			do {
				do {
					l_si = GUI_radio(get_city(0x100), 2,
							get_city(0x104),
							get_city(0x108));
				} while (l_si == -1);

				if (l_si == 1) {

					loot_multi_chest(p_datseg + 0x3e2b,
							get_city(0x11c));

					do {
						l_si = GUI_radio(get_city(0x10c), 2,
							get_city(0x110),
							get_city(0x114));
					} while (l_si == -1);

					if (l_si == 1) {
						use_lockpicks_on_chest((RealPt)RealMake(datseg, 0xb154));
					}
				}

			} while (l_si != 2);

			GUI_output(get_city(0x118));

			ds_writews(TRV_RETURN, ds_readws(TRV_RETURN) == 0 ? 1 : -1);
		}

	} else if (ds_readb(0x3dc9) != 0) {

		do {
			do {
				l_si = GUI_radio(get_city(0x100), 2,
					get_city(0x104),
					get_ltx(0xcb8));

			} while (l_si == -1);

			if (l_si == 1) {

				loot_multi_chest(p_datseg + 0x3e2b, get_city(0x11c));

				if (ds_readbs(0x3e3e) != -1) {

					do {
						l_si = GUI_radio(get_city(0x10c), 2,
							get_city(0x110),
							get_ltx(0xcb8));

					} while (l_si == -1);

					if (l_si == 1) {
						use_lockpicks_on_chest((RealPt)RealMake(datseg, 0xb154));
					}
				}
			}

		} while (l_si != 2);

	}
}

void tevent_064_chest(void)
{
	loot_multi_chest(p_datseg + 0x3e3e, get_city(0x128));
}

/* The rider Orvil <-> Ala */
void tevent_065(void)
{
	signed short answer;

	/* load dialog head */
	load_in_head(56);

	/* show dialogbox */
	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
				NULL,
				get_city(0x12c),
				3,
				get_city(0x130),
				get_city(0x134),
				get_city(0x138));

	} while (answer == -1);

	/* talk to the rider for 15 min */
	if (answer == 1 || answer == 2) {

		timewarp(MINUTES(15));

		GUI_dialog_na(0, answer == 1 ? get_city(0x13c): get_city(0x140));

		if (answer == 2) {
			/* Make Umbrik Siebenstein known */

			/* Original-Bug: if Umbrik is done the value 2
					is overwritten with 1 */
			ds_writeb_z(INFORMER_UMBRIK, 1);
		}
	}
}

void tevent_066(void)
{
	signed short l_si;
	signed short l_di;
	signed short count;
	Bit8u *hero;

	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 2) > 0 && !ds_readb(0x3dca)) ||
		ds_readbs(0x3dca) != 0)
	{
		TRV_found_replenish_place(0);
		ds_writeb(0x3dca, 1);

		if ((test_skill(Real2Host(get_first_hero_available_in_group()), 26, 4) > 0 && !ds_readb(0x3dcb)))
		{

			do {
				l_si = GUI_radio(get_city(0x144), 2,
							get_city(0x148),
							get_city(0x14c));
			} while (l_si == -1);

			if (l_si == 1) {

				GUI_output(get_city(0x150));

				hero = get_hero(0);
				for (l_di = count = 0; l_di <= 6; l_di++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero) &&
						test_skill(hero, 13, -2) <= 0)
					{
						count++;
					}
				}

				if (count != 0) {

					GUI_output(get_city(0x154));

				} else {
					do {
						l_si = GUI_radio(get_city(0x158), 2,
									get_city(0x15c),
									get_city(0x160));
					} while (l_si == -1);

					if (l_si == 1) {

						GUI_output(get_city(0x164));
						return;
					}
				}

				if (!do_fight(FIGHTS_F066)) {
					ds_writeb(0x3dcb, 1);
					add_hero_ap_all(50);
				}
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
