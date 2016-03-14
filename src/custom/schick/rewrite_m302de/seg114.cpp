/**
 *	Rewrite of DSA1 v3.02_de functions of seg114 (travel events 6 / 10)
 *	Functions rewritten: 5/11
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
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* wall of rock */
/* Borlandified and identical */
void tevent_110(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_city(0x13c), 2,
					get_city(0x140),
					get_city(0x144));
	} while (answer == -1);

	if (answer == 1)
	{
		/* try to climb */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, 10, 0) <= 0)
			{
				/* skill test failed */

				if (get_first_hero_with_item(121) != -1)
				{
					/* one hero in the group has a rope */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x154),
						(char*)hero + HERO_NAME2);

					sub_hero_le(hero, random_schick(10));

				} else {
					/* all heros in the group have no ropes */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x158),
						(char*)hero + HERO_NAME2);

					sub_hero_le(hero, random_schick(16) + 4);
				}

				GUI_output(Real2Host(ds_readd(DTP2)));
			}

		}

		GUI_output(get_city(0x150));

	} else {

		/* try to walk arround */

		if (test_skill(Real2Host(get_first_hero_available_in_group()), 28, 0) > 0)
		{
			/* success */
			timewarp(HOURS(4));
			GUI_output(get_city(0x148));
		} else {
			/* fail */
			timewarp(DAYS(1) + HOURS(6));
			GUI_output(get_city(0x14c));
		}
	}
}

/* hunt a rhino */
/* Borlandified and identical */
void tevent_111(void)
{
	signed short counter;
	signed short answer;
	signed short i;
	signed short ret_skill_test2;
	signed short ret_skill_test3;
	signed short unlucky_tests;
	Bit8u *hero;

	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 26, 1) > 0 && !ds_readb(0x3df8)) ||
		ds_readb(0x3df8) == 1)
	{
		ds_writeb(0x3df8, 1);
		ds_writeb(0xe5d2, 1);

		load_ani(33);
		draw_main_screen();
		init_ani(0);

		GUI_output(get_city(0x00));

		hero = get_hero(0);
		for (i = counter = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, 13, -5) <= 0)
			{
				counter++;
			}
		}

		if (counter)
		{
			do {
				answer = GUI_radio(get_city(0x04), 2,
							get_city(0x1c),
							get_city(0x20));
			} while (answer == -1);

			if (answer == 1)
			{
				GUI_output(get_city(0x24));

				add_hero_ap_all(7);

				timewarp(HOURS(2));
			} else {
				add_hero_ap_all(5);
			}

		} else {

			hero = get_hero(select_hero_ok_forced(get_city(0x08)));

			if (test_skill(hero, 13, 0) <= 0)
			{
				/* skill test failed */
				do {
					answer = GUI_radio(get_city(0x04), 2,
								get_city(0x1c),
								get_city(0x20));
				} while (answer == -1);

				if (answer == 1)
				{
					GUI_output(get_city(0x24));

					add_hero_ap_all(7);

					timewarp(HOURS(2));
				} else {
					add_hero_ap_all(5);
				}

			} else {
				/* skill test succeeded */

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x0c),
					(char*)hero + HERO_NAME2);

				GUI_input(Real2Host(ds_readd(DTP2)), counter = unlucky_tests = 0);

				if ((i = test_skill(hero, 7, 12)) > 0) counter++;
				if ((ret_skill_test2 = test_skill(hero, 7, 12)) > 0) counter++;
				if ((ret_skill_test3 = test_skill(hero, 7, 12)) > 0) counter++;

				if (i == 99) unlucky_tests++;
				if (ret_skill_test2 == 99) unlucky_tests++;
				if (ret_skill_test3 == 99) unlucky_tests++;

				if (counter == 3 || unlucky_tests >= 2)
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x10),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));

					add_hero_ap_all(5);

					add_hero_ap(hero, 20);

					timewarp(HOURS(3));

					get_item(45, 1, 200);

					ds_writeb(0x3df8, 2);

				} else if (counter == 2 || unlucky_tests == 1 || counter == 1)
				{

					do {
						answer = GUI_radio(get_city(0x14), 2,
									get_city(0x28),
									get_city(0x2c));
					} while (answer == -1);

					if (answer == 1)
					{
						GUI_output(get_city(0x30));

						add_hero_ap_all(7);

						add_hero_ap(hero, 5);
					} else {

						GUI_output(get_city(0x34));

						add_hero_ap_all(15);

						add_hero_ap(hero, 5);

						timewarp(HOURS(4));

						hero = get_hero(0);
						for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
						{
							if (host_readbs(hero + HERO_TYPE) != 0 &&
								host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_dead(hero) &&
								test_attrib(hero, 4, 2) <= 0)
							{
								/* attribute test failed */
								sub_hero_le(hero, random_schick(10));
							}
						}

						get_item(45, 1, 180);
						ds_writeb(0x3df8, 2);
					}

				} else {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x18),
						(char*)hero + HERO_NAME2);

					do {
						answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
									get_city(0x1c),
									get_city(0x20));
					} while (answer == -1);

					if (answer == 1)
					{
						GUI_output(get_city(0x24));

						add_hero_ap_all(7);

						timewarp(HOURS(2));
					} else {
						add_hero_ap_all(5);
					}
				}
			}
		}

		set_var_to_zero();
		ds_writew(REQUEST_REFRESH, 1);
	}

	ds_writeb(0xe5d2, 0);
}

/* a camp place */
/* Borlandified and identical */
void tevent_112(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 2) > 0 && !ds_readb(0x3df9)) ||
		ds_readb(0x3df9) != 0)
	{
		ds_writeb(0x3df9, 1);

		if ((test_skill(Real2Host(get_first_hero_available_in_group()), 29, 2) > 0 && !ds_readb(0x3dfa)) ||
			ds_readb(0x3dfa) != 0)
		{
			ds_writeb(0x3dfa, 1);

			ds_writeb(0x66d0, 131);
			TRV_found_camp_place(2);
			ds_writeb(0x66d0, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

/* FIRUN-temple in the wilderness */
/* Borlandified and identical */
void tevent_113(void)
{
	signed short answer;
	Bit32s est_old;
	Bit32s est_diff;

	do {
		answer = GUI_radio(get_city(0x38), 2,
					get_city(0x3c),
					get_city(0x40));
	} while (answer == -1);

	if (answer == 1)
	{
		GUI_output(get_city(0x44));

		/* enter the temple */
		ds_writew(TYPEINDEX, 58);
		est_old = ds_readds(GODS_ESTIMATION + 4 * 7);
		ds_writeb(LOCATION, 2);
		do_location();
		TRV_load_textfile(-1);

		GUI_output(get_city(0x48));

		/* calculate the difference of the estimation */
		est_diff = ds_readds(GODS_ESTIMATION + 4 * 7) - est_old;

		ds_writew(TEXTBOX_WIDTH, 3);

		/* the more money you spend, the better the group will sleep */
		ds_writew(WILDCAMP_SLEEP_QUALITY, (est_diff <= 10 ? 1 :
					(est_diff <= 50 ? 2 :
					(est_diff <= 100 ? 3 : 4))));

		ds_writeb(LOCATION, 6);
		do_location();
		ds_writeb(LOCATION, 0);

		ds_writew(TEXTBOX_WIDTH, 9);
		ds_writew(REQUEST_REFRESH, 2);
	}
}

/* a swamp */
/* Borlandified and identical */
void tevent_114(void)
{
	signed short i;
	signed short answer;
	signed short done;
	signed short j;
	Bit8u *hero;

	done = 0;

	do {

		do {
			answer = GUI_radio(get_city(0x4c), 2,
						get_city(0x50),
						get_city(0x54));
		} while (answer == -1);

		if (answer == 1)
		{
			do {
				answer = GUI_radio(get_city(0x70), 2,
							get_city(0x74),
							get_city(0x78));
			} while (answer == -1);

			if (answer == 1)
			{
				/* turn around */
				ds_writew(TRV_RETURN, done = 1);
			}

			if (answer == 2)
			{
				/* go on careful */
				GUI_output(get_city(0x58));
			}

		} else {
			GUI_output(get_city(0x58));
		}

		if (done == 0)
		{
			/* walk through the swamp */
			for (j = 0; j < 2; j++)
			{
				for (i = 0, hero = get_hero(0); i <= 6; i++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != 0 &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero) &&
						test_attrib(hero, 4, 4) <= 0)
					{
						/* attrib test failed */
						timewarp(MINUTES(30));

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_city(0x5c),
							(char*)hero + HERO_NAME2,
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

						GUI_output(Real2Host(ds_readd(DTP2)));

						sub_hero_le(hero, random_schick(8));

						loose_random_item(hero, 50, get_ltx(0x7e8));
						loose_random_item(hero, 50, get_ltx(0x7e8));
						loose_random_item(hero, 50, get_ltx(0x7e8));
					}
				}
			}

			if (!ds_readb(0x3e15))
			{
				/* meet OLIMONE */
				GUI_output(get_city(0x60));

				load_in_head(12);

				GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0xd0), get_city(0x64), 0);
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0xd0), get_city(0x68), 0);

				/* get RECIPE FOR POTENT HEALING POTION */
				get_item(240, 1, 1);

				/* ORIGINAL-BUG: this item could have been not taken => get_hero(-1) => SEGFAULT */
				hero = get_hero(get_first_hero_with_item(240));

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x6c),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				timewarp(HOURS(8));

				for (i = 0, hero = get_hero(0); i <= 6; i++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != 0 &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero))
					{
						add_hero_le(hero, 7);
					}
				}

				ds_writeb(0x3e15, (signed char)(done = 1));

			} else {
				GUI_output(get_city(0x100));

				timewarp(HOURS(8));

				for (i = 0, hero = get_hero(0); i <= 6; i++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != 0 &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero))
					{
						add_hero_le(hero, 4);
					}
				}

				done = 1;
			}
		}

	} while (done == 0);
}

#if !defined(__BORLANDC__)
}
#endif
