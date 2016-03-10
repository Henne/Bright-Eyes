/**
 *	Rewrite of DSA1 v3.02_de functions of seg115 (travel events 7 / 10)
 *	Functions rewritten: 9/13
 */
#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
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
/* Borlandified and identical */
void tevent_016(void)
{
	signed short answer;
	Bit8u *hero;

	if (!ds_readb(0x3da9))
	{

		load_in_head(46);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_city(0x4c), 2,
						get_city(0x50),
						get_city(0x54));
		} while (answer == -1);

		if (answer == 1)
		{
			/* ignore */
			GUI_dialog_na(0, get_city(0x58));
		} else {
			/* wave */

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_city(0x5c), 2,
							get_city(0x60),
							get_city(0x64));
			} while (answer == -1);

			if (answer == 1)
			{
				/* run away */
				GUI_dialog_na(0, get_city(0x68));
			} else {
				/* go towards them */

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_city(0x6c), 2,
								get_city(0x70),
								get_city(0x74));
				} while (answer == -1);

				if (answer == 2)
				{
					/* walk away */
					GUI_dialog_na(0, get_city(0x78));
				} else {
					/* help them */

					GUI_dialog_na(0, get_city(0x7c));

					hero = get_hero(get_hero_KK_best());

					/* test KK+3 */
					if (test_attrib(hero, 6, 3) > 0)
					{
						/* success */
						GUI_dialog_na(0, get_city(0x80));
						GUI_dialog_na(0, get_city(0x90));

						add_hero_ap_all(5);
					} else {
						/* fail */
						GUI_dialog_na(0, get_city(0x84));

						hero = get_hero(get_random_hero());

						/* GE+0 */
						if (test_attrib(hero, 4, 0) > 0)
						{
							/* success */
							timewarp(MINUTES(15));

							sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_city(0x8c),
								(char*)hero + HERO_NAME2,
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

							GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

							GUI_dialog_na(0, get_city(0x90));
						} else {
							/* fail */
							sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_city(0x88),
								(char*)hero + HERO_NAME2,
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)),
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)),
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

							GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

							timewarp(HOURS(1));

							sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_city(0x94),
								(char*)hero + HERO_NAME2,
								(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 1)));

							GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

							add_hero_ap(hero, 5);

							loose_random_item(hero, 10, get_ltx(0x7e8));

							hero_disease_test(hero, 2, 20 - (host_readbs(hero + HERO_KK) + host_readbs(hero + HERO_KK_MOD)));
						}

					}
				}
			}
		}

		ds_writeb(0x3da9, 1);
	}
}

/* falling rocks */
/* Borlandified and identical */
void tevent_090(void)
{
	signed short i;
	Bit8u *hero;

	GUI_output(get_city(0x00));
	GUI_output(get_city(0x04));

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_skill(hero, 50, 0) <= 0)
		{
			/* failed */
			sub_hero_le(hero, random_schick(10));

			loose_random_item(hero, 10, get_ltx(0x7e8));
			loose_random_item(hero, 10, get_ltx(0x7e8));
		}
	}

	GUI_output(get_city(0x08));
}

/* Borlandified and identical */
void tevent_091(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 29, 5) > 0 && !ds_readb(0x3ded)) ||
		ds_readb(0x3ded) != 0)
	{
		ds_writeb(0x66d0, 122);
		TRV_found_herb_place(0);
		ds_writeb(0x66d0, -1);
		ds_writeb(0x3ded, 1);
	}
}

/* Borlandified and identical */
void tevent_093(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 4) > 0 && !ds_readb(0x3dee)) ||
		ds_readb(0x3dee) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(0x3dee, 1);
	}
}

/* entrance daspota-dungeon */
/* Borlandified and identical */
void tevent_094(void)
{
	if (ds_readb(0x3def) != 0)
	{
		signed short answer;

		GUI_output(get_city(0x0c));

		load_in_head(53);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_city(0x10), 2,
						get_city(0x14),
						get_city(0x18));
		} while (answer == -1);

		if (answer == 1) {
			/* enter daspota dungeon */
			ds_writeb(0x4333, 6);
		}
	}
}

/* Arete */
/* Borlandified and identical */
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

		GUI_output(get_city(0x1c));

		hero = get_hero(0);

		for (i = counter_failed = counter_heros = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero))
			{
				counter_heros++;

				if (test_attrib(hero, 8, -1) > 0)
				{
					timewarp(MINUTES(30));

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x20),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));

					counter_failed++;
				}
			}
		}

		if (!counter_failed)
		{
			/* no hero failed HA-test */
			GUI_output(get_city(0x40));
			done = 1;

		} else if (counter_failed == counter_heros)
		{
			/* all heros failed HA-test */
			do {
				counter_failed = GUI_radio(get_city(0x24), 2,
								get_city(0x28),
								get_city(0x2c));
			} while (counter_failed == -1);

			if (counter_failed == 2)
			{
				/* make a rest */
				ds_writeb(LOCATION, 6);
				do_location();
				ds_writeb(LOCATION, 0);

				TRV_load_textfile(-1);
			} else {
				ds_writew(0x4336, done = 1);
			}
		} else {
			/* at least one hero failed HA-test */
			counter_heros = 0;

			do {
				do {
					counter_failed = GUI_radio((counter_heros == 0 ? get_city(0x30) : get_city(0x15c)), 3,
									get_city(0x34),
									get_city(0x38),
									get_city(0x3c));
				} while (counter_failed == -1);

				if (counter_failed == 1)
				{
					/* "on hands and knees" */

					timewarp(HOURS(2));

					GUI_output(get_city(0x40));

					done = 1;

				} else if (counter_failed == 2)
				{
					/* try a spell */
					hero = get_hero(select_hero_ok_forced(get_ltx(0x4f4)));

					if (host_readbs(hero + HERO_TYPE) < 7)
					{
						/* this hero is no magic-user */
						GUI_output(get_ltx(0x528));
					} else {

						ret_spell_test = test_spell(hero, 7, 0);

						if (ret_spell_test > 0)
						{
							/* spell succeeded */

							/* TODO: magicians with 4th staff spell may pay less */
							sub_ae_splash(hero, get_spell_cost(7, 0));

							GUI_output(get_city(0x40));

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
								(char*)get_ltx(0x97c),
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

					GUI_output(get_city(0x40));

					done = 1;
				}
			} while (!done);
		}
	} while (!done);
}

/* Borlandified and identical */
void tevent_096(void)
{
	signed short answer;

	do {
		answer = GUI_radio(get_city(0x44), 2,
					get_city(0x48),
					get_city(0x4c));
	} while (answer == -1);

	if (answer == 1)
	{
		/* try to keep on track */

		if (test_skill(Real2Host(get_first_hero_available_in_group()), 28, 2) > 0)
		{
			timewarp(HOURS(3));

			GUI_output(get_city(0x50));
		} else {
			timewarp(HOURS(4));

			GUI_output(get_city(0x58));

			answer = -1;
		}
	} else {
		/* try to go arround */

		if (test_skill(Real2Host(get_first_hero_available_in_group()), 28, 4) > 0)
		{
			timewarp(HOURS(4));

			GUI_output(get_city(0x54));
		} else {
			timewarp(HOURS(5));

			GUI_output(get_city(0x5c));

			answer = -1;
		}
	}

	if (answer == -1)
	{
		/* lost the way */

		if (test_skill(Real2Host(get_first_hero_available_in_group()), 28, 3) > 0)
		{
			/* find the way again */
			timewarp(HOURS(3));

			GUI_output(get_city(0x60));
		} else {
			/* lost the way completely */
			timewarp(HOURS(4));

			GUI_output(get_city(0x64));

			ds_writeb(LOCATION, 6);
			do_location();
			ds_writeb(LOCATION, 0);

			TRV_load_textfile(-1);

			GUI_output(get_city(0x68));
		}
	}
}

/* entrance dungeon: temple of the nameless */
/* Borlandified and identical */
void tevent_126(void)
{
	signed short answer;

	do {
		answer = GUI_radio(get_city(0x00), 2,
					get_city(0x04),
					get_city(0x08));
	} while (answer == -1);

	if (answer == 1)
	{
		do {
			answer = GUI_radio(get_city(0x0c), 2,
						get_city(0x10),
						get_city(0x14));
		} while (answer == -1);

		if (answer == 1)
		{
			ds_writeb(0x4333, 9);
		}
	}
}

/* Borlandified and identical */
void tevent_127(void)
{
	signed short answer;

	load_in_head(14);

	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
					get_city(0x18), 3,
					get_city(0x1c),
					get_city(0x20),
					get_city(0x24));
	} while (answer == -1);

	if (answer == 1 || answer == 2)
	{
		GUI_dialog_na(0, (answer == 1 ? get_city(0x28) : get_city(0x2c)));

		timewarp(MINUTES(30));
	}
}

#if !defined(__BORLANDC__)
}
#endif
