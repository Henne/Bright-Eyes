/**
 *	Rewrite of DSA1 v3.02_de functions of seg118 (travel events 10 / 10)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg118.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg029.h"
#include "seg030.h"
#include "seg075.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg109.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* The griffon */
void tevent_037(void)
{
	signed short answer;
	signed short done;
	signed short hours;
	Bit8u *hero;

	done = 0;

	if (!ds_readb(0x3dba))
	{
		ds_writeb(0xe5d2, 1);

		load_ani(28);
		draw_main_screen();
		init_ani(0);

		do {
			answer = GUI_radio(get_city(0x68), 2,
						get_city(0x6c),
						get_city(0x70));
		} while (answer == -1);

		if (answer == 1) {
			/* wait */

			timewarp(MINUTES(15));

			do {
				answer = GUI_radio(get_city(0x74), 2,
							get_city(0x78),
							get_city(0x70));
			} while (answer == -1);

			if (answer == 1)
			{

				timewarp(MINUTES(15));

				/* loop until you decicide to go on */
				do {
					answer = GUI_radio(get_city(0x7c), 2,
								get_city(0x78),
								get_city(0x70));

				} while (answer == -1 || answer == 1);
			}
		}

		if (answer == 2) {
			/* go on */

			do {
				answer = GUI_radio(get_city(0x80), 2,
							get_city(0x84),
							get_city(0x88));

			} while (answer == -1);
		}

		do {

			if (answer == 1)
			{
				/* turn around */
				hours = GUI_input(get_city(0x8c), 2);

				if (hours >= 4)
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x90),
						hours);

					GUI_output(Real2Host(ds_readd(DTP2)));

					done = 1;

				} else {

					/* entered time was to short */
					do {
						answer = GUI_radio(get_city(0x94), 2,
									get_city(0x84),
									get_city(0x88));

					} while (answer == -1);

					if (answer == 1)
					{
						timewarp(HOURS(4));

						GUI_output(get_city(0x98));

						done = 1;
					}
				}
			} else {
				do {
					answer = GUI_radio(get_city(0x9c), 3,
								get_city(0xa0),
								get_city(0xa4),
								get_city(0xa8));

				} while (answer == -1);

				if (answer == 1)
				{
					/* offer a deposit */
					ds_writeb(0x3dba, 1);

					/* select a hero randomly */
					hero = get_hero(answer = get_random_hero());

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0xb4),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					add_hero_ap(hero, 20);

					add_hero_ap_all(20);

					GUI_output(get_city(0xb8));

					GUI_input(get_city(0xbc), 10);

					/* check if the answer is "RAD" 1st */
					if (strcmp((char*)Real2Host(ds_readd(TEXT_INPUT_BUF)), (char*)p_datseg + 0xb21e))
					{
						GUI_input(get_city(0xc0), 10);

						/* check if the answer is "RAD" 2nd */
						if (strcmp((char*)Real2Host(ds_readd(TEXT_INPUT_BUF)), (char*)p_datseg + 0xb222))
						{
							GUI_input(get_city(0xc4), 10);

							/* check if the answer is "RAD" 3rd */
							if (strcmp((char*)Real2Host(ds_readd(TEXT_INPUT_BUF)), (char*)p_datseg + 0xb226))
							{
								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_city(0xc8),
									(char*)hero + HERO_NAME2,
									(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
									(char*)hero + HERO_NAME2,
									(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

								GUI_output(Real2Host(ds_readd(DTP2)));

								/* the hero is now lost completely */
								hero_disappear(hero, answer, -1);

								done = 1;
							} else {

								GUI_output(get_city(0xcc));

								add_hero_ap_all(10);

								done = 1;
							}
						} else {

							GUI_output(get_city(0xcc));

							add_hero_ap_all(10);

							done = 1;
						}
					} else {

						GUI_output(get_city(0xcc));

						add_hero_ap_all(10);

						done = 1;
					}

				} else {
					/* talk of hetmann or hyggelik */

					do {
						answer = GUI_radio(get_city(0xac), 2,
									get_city(0x84),
									get_city(0x88));

					} while (answer == -1);

					if (answer == 2)
					{
						/* select a hero randomly */
						hero = get_hero(get_random_hero());

						timewarp(HOURS(1));

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_city(0xb0),
							(char*)hero + HERO_NAME2);

						GUI_output(Real2Host(ds_readd(DTP2)));
					}
				}
			}

		} while (!done);

		set_var_to_zero();

		ds_writeb(0xe5d2, 0);
		ds_writew(REQUEST_REFRESH, 1);
	}
}

/* unicorn 1st time */
void tevent_038(void)
{
	if (!ds_readb(0x3ddb) && ds_readws(GOT_MAIN_QUEST) != 0)
	{
		do_talk(11, 2);
		ds_writeb(0x3ddb, 1);
	}
}

void tevent_078(void)
{
	signed short hours;
	signed short answer;
	signed short found_path;
	signed short days;
	Bit8u *hero;

	/* This event happens only in winter */
	if (!get_current_season())
	{

		do {
			answer = GUI_radio(get_city(0x138), 2,
						get_city(0x13c),
						get_city(0x140));
		} while (answer == -1);

		load_in_head(57);

		if (answer == 1)
		{
			/* try to clean the path */

			/* with SHOVEL/SCHAUFEL 5 Hours, without 8 Hours */
			if (get_first_hero_with_item(73) != -1) {
				hours = 5;
			} else {
				hours = 8;
			}

			timewarp(HOURS(hours));

			GUI_dialog_na(0, get_city(0x144));

			/* each hero in the group looses hours / 2 LE */
			sub_group_le(hours >> 1);

			ds_writews(WILDCAMP_SLEEP_QUALITY, -6);

			ds_writeb(LOCATION, 6);
			do_location();
			ds_writeb(LOCATION, 0);

			TRV_load_textfile(-1);

#ifdef M302de_ORIGINAL_BUGFIX
			/* if a campfight occurs, the picture needs to be reloaded */
			load_in_head(57);
#endif
			GUI_dialog_na(0, get_city(0x150));

		} else {
			/* try to find another path */
			found_path = days = 0;

			do {
				/* ORIENTATION + 8 */
				if (test_skill(Real2Host(get_first_hero_available_in_group()), 28, 8) > 0)
				{
					/* success, you found a way */
					timewarp(HOURS(5));

					sub_group_le(2);

					GUI_dialog_na(0, !answer ? get_city(0x148) : get_city(0x154));

					found_path = 1;
				} else {
					/* fail */
					timewarp(HOURS(8));

					GUI_dialog_na(0, get_city(0x14c));

					sub_group_le(4);

					ds_writews(WILDCAMP_SLEEP_QUALITY, -6);
					/* make a camp */
					ds_writeb(LOCATION, 6);
					do_location();
					ds_writeb(LOCATION, 0);

					TRV_load_textfile(-1);

					answer = 1;
				}
#ifdef M302de_ORIGINAL_BUGFIX
				/* if a campfight occurs, the picture needs to be reloaded */
				load_in_head(57);
#endif

			} while (found_path == 0 && ++days < 6);

			if (found_path == 0)
			{
				GUI_dialog_na(0, get_city(0x158));

				timewarp(HOURS(8));

				/* Original-Bug: all heros die, even if they are not in the current group */
				hero = get_hero(0);
				for (hours = 0; hours <= 6; hours++, hero += SIZEOF_HERO)
				{
					hero_disappear(hero, hours, -1);
				}
			}
		}

		ds_writews(WILDCAMP_SLEEP_QUALITY, 0);
	}
}

void tevent_079(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 4) > 0 && !ds_readb(0x3de3)) ||
		ds_readb(0x3de3) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(0x3de3, 1);
	}
}

void tevent_051(void)
{
	signed short answer;

	if (test_skill(Real2Host(get_first_hero_available_in_group()), 26, 4) > 0 && !ds_readb(0x3dc5))
	{
		ds_writeb(0x3dc5, 1);

		if (!TRV_follow_trail_question())
		{

			GUI_output(get_city(0x104));
			GUI_output(get_city(0x108));

			load_in_head(53);

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_city(0x10c), 2,
							get_city(0x110),
							get_city(0x114));
			} while (answer == -1);

			if (answer == 1)
			{
				ds_writeb(0x4333, 3);
			}
		}

	} else if (ds_readb(0x3dc5) != 0) {

		load_in_head(53);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_city(0x118), 2,
						get_city(0x11c),
						get_city(0x120));
		} while (answer == -1);

		if (answer == 1)
		{
			ds_writeb(0x4333, 3);
		}
	}
}

void tevent_052(void)
{
	signed short done;
	signed short i;
	signed short answer;

	done = 0;

	load_in_head(52);

	do {

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_city(0x124), 2,
						get_city(0x128),
						get_city(0x12c));
		} while (answer == -1);

		if (answer == 1)
		{
			timewarp(HOURS(1));

			GUI_dialog_na(0, get_city(0x130));

			done = 1;
		} else {

			i = 0;

			do {
				/* check for a chopping tool */
				if (get_first_hero_with_item(ds_readb(0xb135 + i)) != -1)
				{
					done = 1;
				}

			} while(!done && ds_readb(0xb135 + ++i) != 255);

			if (done)
			{
				timewarp(HOURS(3));

				GUI_dialog_na(0, get_city(0x134));
			} else {
				GUI_dialog_na(0, get_city(0x138));
			}
		}

	} while (!done);
}

void tevent_120(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 3) > 0 && !ds_readb(0x3dff)) ||
		ds_readb(0x3dff) != 0)
	{
		ds_writeb(0x3dff, 1);
		TRV_found_camp_place(1);
	}
}

void tevent_121(void)
{
	TRV_cross_a_ford(get_dtp(0xd4), 30, 0);
}

void tevent_124(void)
{
	signed short i;
	signed short counter;
	signed short answer;
	signed short have_climb_tools;
	signed short skill_ret;
	Bit8u *hero;

	have_climb_tools = 0;

	GUI_output(get_city(0xc4));

	if (DNG_check_climb_tools() != -1)
	{
		have_climb_tools = 1;

		GUI_output(get_city(0xc8));
	}

	do {
		answer = GUI_radio(get_city(0xcc), 2,
					get_city(0xd0),
					get_city(0xd4));
	} while (answer == -1);

	if (answer == 2)
	{
		ds_writew(TRV_RETURN, 1);

	} else {

		hero = get_hero(0);
		for (i = counter = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				(skill_ret = test_skill(hero, 10, -2)) <= 0)
			{
				/* the climb test failed */
				counter++;

				/* Original-Bug: a skilltest with a fatal result return -99, not -1 */
#ifdef M302de_ORIGINAL_BUGFIX
				if (skill_ret == -99 && have_climb_tools == 0)
#else
				if (skill_ret == -1 && have_climb_tools == 0)
#endif
				{
					/* fatal */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0xe0),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					hero_disappear(hero, i, -1);

				} else {

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0xdc),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_hero_le(hero, random_schick(5));
				}
			}
		}

		if (!counter)
		{
			GUI_output(get_city(0xd8));
		}
	}

}

void tevent_125(void)
{
	signed short answer;

	load_in_head(17);

	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_city(0xe4), 3,
					get_city(0xe8),
					get_city(0xec),
					get_city(0xf0));
	} while (answer == -1);

	if (answer == 1 || answer == 2)
	{
		GUI_dialog_na(0, answer == 1 ? get_city(0xf4) : get_city(0xf8));

		timewarp(MINUTES(30));
	}
}

void tevent_145(void)
{
	signed short answer;

	load_area_description(0);

	if (!(cast_u16(ds_readb(0xe4a2)) & 1))
	{
		do {
			answer = GUI_radio(get_city(0xa8), 2,
						get_city(0xac),
						get_city(0xb0));
		} while (answer == -1);

		if (answer == 1)
		{
			ds_writew(0x434a, 7);
			ds_writeb(CURRENT_TOWN, 8);

		} else {
			ds_writew(0x434a, 8);
			ds_writeb(CURRENT_TOWN, 7);
		}

	} else {
		do {
			answer = GUI_radio(get_city(0x9c), 2,
						get_city(0xa0),
						get_city(0xa4));
		} while (answer == -1);

		if (answer == 1)
		{
			ds_writew(0x434a, 4);
			ds_writeb(CURRENT_TOWN, 5);

		} else {
			ds_writew(0x434a, 5);
			ds_writeb(CURRENT_TOWN, 4);
		}
	}

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_city(0xb4),
		(char*)get_ltx(4 * (ds_readws(0x434a) + 0xeb)));

	GUI_output(Real2Host(ds_readd(DTP2)));
}

#if !defined(__BORLANDC__)
}
#endif
