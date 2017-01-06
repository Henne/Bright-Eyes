/**
 *	Rewrite of DSA1 v3.02_de functions of seg115 (travel events 7 / 10)
 *	Functions rewritten: 13/13 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg115.cpp
 */
#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* the raft */
void tevent_016(void)
{
	signed short answer;
	Bit8u *hero;

	if (!ds_readb(TEVENT016_FLAG))
	{

		load_in_head(46);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx2(0x4c), 2,
						get_tx2(0x50),
						get_tx2(0x54));
		} while (answer == -1);

		if (answer == 1)
		{
			/* ignore */
			GUI_dialog_na(0, get_tx2(0x58));
		} else {
			/* wave */

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx2(0x5c), 2,
							get_tx2(0x60),
							get_tx2(0x64));
			} while (answer == -1);

			if (answer == 1)
			{
				/* run away */
				GUI_dialog_na(0, get_tx2(0x68));
			} else {
				/* go towards them */

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx2(0x6c), 2,
								get_tx2(0x70),
								get_tx2(0x74));
				} while (answer == -1);

				if (answer == 2)
				{
					/* walk away */
					GUI_dialog_na(0, get_tx2(0x78));
				} else {
					/* help them */

					GUI_dialog_na(0, get_tx2(0x7c));

					hero = get_hero(get_hero_KK_best());

					/* test KK+3 */
					if (test_attrib(hero, ATTRIB_KK, 3) > 0)
					{
						/* success */
						GUI_dialog_na(0, get_tx2(0x80));
						GUI_dialog_na(0, get_tx2(0x90));

						add_hero_ap_all(5);
					} else {
						/* fail */
						GUI_dialog_na(0, get_tx2(0x84));

						hero = get_hero(get_random_hero());

						/* GE+0 */
						if (test_attrib(hero, ATTRIB_GE, 0) > 0)
						{
							/* success */
							timewarp(MINUTES(15));

							sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_tx2(0x8c),
								(char*)hero + HERO_NAME2,
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

							GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

							GUI_dialog_na(0, get_tx2(0x90));
						} else {
							/* fail */
							sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_tx2(0x88),
								(char*)hero + HERO_NAME2,
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)),
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)),
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

							GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

							timewarp(HOURS(1));

							sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_tx2(0x94),
								(char*)hero + HERO_NAME2,
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 1)));

							GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

							add_hero_ap(hero, 5);

							loose_random_item(hero, 10, get_ttx(0x7e8));

							hero_disease_test(hero, 2, 20 - (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK))));
						}

					}
				}
			}
		}

		ds_writeb(TEVENT016_FLAG, 1);
	}
}

/* falling rocks */
void tevent_090(void)
{
	signed short i;
	Bit8u *hero;

	GUI_output(get_tx2(0x00));
	GUI_output(get_tx2(0x04));

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_skill(hero, TA_GEFAHRENSINN, 0) <= 0)
		{
			/* failed */
			sub_hero_le(hero, random_schick(10));

			loose_random_item(hero, 10, get_ttx(0x7e8));
			loose_random_item(hero, 10, get_ttx(0x7e8));
		}
	}

	GUI_output(get_tx2(0x08));
}

void tevent_091(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 5) > 0 && !ds_readb(TEVENT091_FLAG)) ||
		ds_readb(TEVENT091_FLAG) != 0)
	{
		ds_writeb(0x66d0, 122);
		TRV_found_herb_place(0);
		ds_writeb(0x66d0, -1);
		ds_writeb(TEVENT091_FLAG, 1);
	}
}

void tevent_093(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 4) > 0 && !ds_readb(TEVENT093_FLAG)) ||
		ds_readb(TEVENT093_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT093_FLAG, 1);
	}
}

/* entrance daspota-dungeon */
void tevent_094(void)
{
	if (ds_readb(TEVENT094_FLAG) != 0)
	{
		signed short answer;

		GUI_output(get_tx2(0x0c));

		load_in_head(53);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx2(0x10), 2,
						get_tx2(0x14),
						get_tx2(0x18));
		} while (answer == -1);

		if (answer == 1) {
			/* enter daspota dungeon */
			ds_writeb(TRAVEL_DETOUR, 6);
		}
	}
}

/* Arete */
void tevent_095(void)
{
	signed short counter_failed;
	signed short done;
	signed short i;
	signed short counter_heros;
	Bit8u *hero;
	signed short ret_spell_test;

	done = 0;

	do {

		GUI_output(get_tx2(0x1c));

		hero = get_hero(0);

		for (i = counter_failed = counter_heros = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero))
			{
				counter_heros++;

				if (test_attrib(hero, ATTRIB_HA, -1) > 0)
				{
					timewarp(MINUTES(30));

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(0x20),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));

					counter_failed++;
				}
			}
		}

		if (!counter_failed)
		{
			/* no hero failed HA-test */
			GUI_output(get_tx2(0x40));
			done = 1;

		} else if (counter_failed == counter_heros)
		{
			/* all heros failed HA-test */
			do {
				counter_failed = GUI_radio(get_tx2(0x24), 2,
								get_tx2(0x28),
								get_tx2(0x2c));
			} while (counter_failed == -1);

			if (counter_failed == 2)
			{
				/* make a rest */
				ds_writeb(LOCATION, LOCATION_WILDCAMP);
				do_location();
				ds_writeb(LOCATION, 0);

				TRV_load_textfile(-1);
			} else {
				ds_writew(TRV_RETURN, done = 1);
			}
		} else {
			/* at least one hero failed HA-test */
			counter_heros = 0;

			do {
				do {
					counter_failed = GUI_radio((counter_heros == 0 ? get_tx2(0x30) : get_tx2(0x15c)), 3,
									get_tx2(0x34),
									get_tx2(0x38),
									get_tx2(0x3c));
				} while (counter_failed == -1);

				if (counter_failed == 1)
				{
					/* "on hands and knees" */

					timewarp(HOURS(2));

					GUI_output(get_tx2(0x40));

					done = 1;

				} else if (counter_failed == 2)
				{
					/* try a spell */
					hero = get_hero(select_hero_ok_forced(get_ttx(0x4f4)));

					if (host_readbs(hero + HERO_TYPE) < 7)
					{
						/* this hero is no magic-user */
						GUI_output(get_ttx(0x528));
					} else {

						ret_spell_test = test_spell(hero, 7, 0);

						if (ret_spell_test > 0)
						{
							/* spell succeeded */

							/* TODO: magicians with 4th staff spell may pay less */
							sub_ae_splash(hero, get_spell_cost(7, 0));

							GUI_output(get_tx2(0x40));

							done = 1;

						} else if (ret_spell_test != -99) {

							/* spell failed */

							/* hero pays the half spell costs */
							sub_ae_splash(hero, get_spell_cost(7, 1));

							/* TODO: some output for the player */

							counter_heros = 1;
						} else {
							/* spell failed unluckily */

							/* TODO: this gets output, but no spell costst ??? */
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(0x97c),
								(char*)hero + HERO_NAME2);

							GUI_output(Real2Host(ds_readd(DTP2)));
						}

						timewarp(MINUTES(30));
					}
				} else
				{
					/* talk to the heros */

					/* wait for 4 hours */
					timewarp(HOURS(4));

					GUI_output(get_tx2(0x40));

					done = 1;
				}
			} while (!done);
		}
	} while (!done);
}

void tevent_096(void)
{
	signed short answer;

	do {
		answer = GUI_radio(get_tx2(0x44), 2,
					get_tx2(0x48),
					get_tx2(0x4c));
	} while (answer == -1);

	if (answer == 1)
	{
		/* try to keep on track */

		if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 2) > 0)
		{
			timewarp(HOURS(3));

			GUI_output(get_tx2(0x50));
		} else {
			timewarp(HOURS(4));

			GUI_output(get_tx2(0x58));

			answer = -1;
		}
	} else {
		/* try to go arround */

		if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 4) > 0)
		{
			timewarp(HOURS(4));

			GUI_output(get_tx2(0x54));
		} else {
			timewarp(HOURS(5));

			GUI_output(get_tx2(0x5c));

			answer = -1;
		}
	}

	if (answer == -1)
	{
		/* lost the way */

		if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 3) > 0)
		{
			/* find the way again */
			timewarp(HOURS(3));

			GUI_output(get_tx2(0x60));
		} else {
			/* lost the way completely */
			timewarp(HOURS(4));

			GUI_output(get_tx2(0x64));

			ds_writeb(LOCATION, LOCATION_WILDCAMP);
			do_location();
			ds_writeb(LOCATION, 0);

			TRV_load_textfile(-1);

			GUI_output(get_tx2(0x68));
		}
	}
}

/* entrance dungeon: temple of the nameless */
void tevent_126(void)
{
	signed short answer;

	do {
		answer = GUI_radio(get_tx2(0x00), 2,
					get_tx2(0x04),
					get_tx2(0x08));
	} while (answer == -1);

	if (answer == 1)
	{
		do {
			answer = GUI_radio(get_tx2(0x0c), 2,
						get_tx2(0x10),
						get_tx2(0x14));
		} while (answer == -1);

		if (answer == 1)
		{
			ds_writeb(TRAVEL_DETOUR, 9);
		}
	}
}

void tevent_127(void)
{
	signed short answer;

	load_in_head(14);

	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
					get_tx2(0x18), 3,
					get_tx2(0x1c),
					get_tx2(0x20),
					get_tx2(0x24));
	} while (answer == -1);

	if (answer == 1 || answer == 2)
	{
		GUI_dialog_na(0, (answer == 1 ? get_tx2(0x28) : get_tx2(0x2c)));

		timewarp(MINUTES(30));
	}
}

void tevent_128(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT128_FLAG)) ||
		ds_readb(TEVENT128_FLAG) != 0)
	{
		ds_writeb(TEVENT128_FLAG, 1);

		if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 4) > 0 && !ds_readb(TEVENT128_REPLEN_FLAG)) ||
			ds_readb(TEVENT128_REPLEN_FLAG) != 0)
		{
			ds_writeb(TEVENT128_REPLEN_FLAG, 1);
			TRV_found_replenish_place(1);
		} else {
			TRV_found_replenish_place(0);
		}
	}
}

/* entrance dungeon: dragon cave */
void tevent_129(void)
{
	signed short answer;

	if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 4) > 0 && !ds_readb(TEVENT129_FLAG))
	{
		ds_writeb(TEVENT129_FLAG, 1);

		GUI_output(get_tx2(0x30));

		load_in_head(53);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						get_tx2(0x34), 2,
						get_tx2(0x38),
						get_tx2(0x3c));
		} while (answer == -1);

		if (answer == 1) {
			ds_writeb(TRAVEL_DETOUR, 10);
		}

	} else if (ds_readb(TEVENT129_FLAG) != 0) {

		load_in_head(53);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						get_tx2(0x40), 2,
						get_tx2(0x44),
						get_tx2(0x48));
		} while (answer == -1);

		if (answer == 1) {
			ds_writeb(TRAVEL_DETOUR, 10);
		}
	}
}

/* a cutter */
void tevent_047(void)
{
	signed short answer;
	signed short l_di;

	l_di = 0;

	do {
		answer = GUI_radio(get_tx2(0x00), 2,
					get_tx2(0x04),
					get_tx2(0x08));
	} while (answer == -1);

	if (answer == 1)
	{
		/* ignore */
		GUI_output(get_tx2(0x0c));

	} else {
		/* wave */
		do {
			answer = GUI_radio(get_tx2(0x10), 2,
						get_tx2(0x14),
						get_tx2(0x18));
		} while (answer == -1);

		if (answer == 1)
		{
			/* run away */
			do {
				answer = GUI_radio(get_tx2(0x1c), 2,
							get_tx2(0x24),
							get_tx2(0x20));
			} while (answer == -1);

			if (answer == 1)
			{
				/* run */
				GUI_output(get_tx2(0x28));
			} else {
				/* wait */
				l_di = 1;
			}
		}

		if (answer == 2)
		{
			/* wait */
			load_in_head(42);

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
							(!l_di ? get_tx2(0x3c) : get_tx2(0x2c)), 2,
							get_tx2(0x30),
							get_tx2(0x34));
			} while (answer == -1);

			if (answer == 1)
			{
				/* deny */
				GUI_dialog_na(0, get_tx2(0x38));
			} else {
				/* accept */

				l_di = random_schick(2) - 1;

				if (!l_di)
				{
					do {
						answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
									get_tx2(0x40), 3,
									get_tx2(0x44),
									get_tx2(0x48),
									get_tx2(0x4c));
					} while (answer == -1);

					if (answer == 3)
					{
						/* no thanks */
						GUI_dialog_na(0, get_tx2(0x50));
					} else {
						/* 1 = LJASDAHL, 2 = OTTARJE */
						GUI_dialog_na(0, answer == 1 ? get_tx2(0x5c) : get_tx2(0x60));
						GUI_dialog_na(0, get_tx2(0x64));

						if (answer == 1)
						{
							ds_writeb(CURRENT_TOWN, 42);
							ds_writew(X_TARGET, 7);
							ds_writew(Y_TARGET, 3);
						} else {
							ds_writeb(CURRENT_TOWN, 37);
							ds_writew(X_TARGET, 9);
							ds_writew(Y_TARGET, 10);
						}

						ds_writeb(TRAVEL_DETOUR, 99);
					}
				} else {
					do {
						answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
									get_tx2(0x54), 2,
									get_tx2(0x58),
									get_tx2(0x4c));
					} while (answer == -1);

					if (answer == 2)
					{
						/* deny */
						GUI_dialog_na(0, get_tx2(0x50));
					} else {
						/* travel to VARNHEIM */
						GUI_dialog_na(0, get_tx2(0x5c));
						GUI_dialog_na(0, get_tx2(0x64));

						ds_writeb(CURRENT_TOWN, 43);
						ds_writew(X_TARGET, 4);
						ds_writew(Y_TARGET, 10);
						ds_writeb(TRAVEL_DETOUR, 99);
					}
				}
			}
		}
	}
}

/* entrance dungeon: a ruin */
void tevent_100(void)
{
	signed short answer;

	if (ds_readb(TEVENT100_FLAG) != 0)
	{
		do {
			answer = GUI_radio(get_tx2(0xd0), 3,
						get_tx2(0xd4),
						get_tx2(0xd8),
						get_tx2(0xdc));
		} while (answer == -1);

		if (answer == 1)
		{
			GUI_output(get_tx2(0xe0));

			ds_writeb(EVENT_ANI_BUSY, 1);

			load_ani(11);
			draw_main_screen();
			init_ani(0);

			GUI_output(get_tx2(0xe4));

			do {
				answer = GUI_radio(get_tx2(0xe8), 2,
							get_tx2(0xec),
							get_tx2(0xf0));
			} while (answer == -1);

			if (answer == 1)
			{
				ds_writeb(TRAVEL_DETOUR, 7);
			} else {

				GUI_output(get_tx2(0x10c));

				/* FF+4 */
				if (test_attrib(Real2Host(get_first_hero_available_in_group()), ATTRIB_GE, 4) > 0)
				{
					/* success */
					GUI_output(get_tx2(0x110));
				} else {
					/* fail */
					Bit8u *hero;
					hero = Real2Host(get_first_hero_available_in_group());

					answer = get_free_mod_slot();

					set_mod_slot(answer, DAYS(1), hero + (HERO_ATTRIB + 3 * ATTRIB_GE), -2, 0);

					timewarp(MINUTES(15));


					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(0x114),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

					GUI_output(Real2Host(ds_readd(DTP2)));
				}

				GUI_output(get_tx2(0x118));
			}

			ds_writew(REQUEST_REFRESH, 1);

		} else if (answer == 3)
		{
			ds_writew(TRV_RETURN, 1);
		}
	}

	set_var_to_zero();
	ds_writeb(EVENT_ANI_BUSY, 0);
}

#if !defined(__BORLANDC__)
}
#endif
