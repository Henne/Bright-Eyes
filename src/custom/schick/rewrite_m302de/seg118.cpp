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

	if (!ds_readb(TEVENT037_FLAG))
	{
		ds_writeb(EVENT_ANI_BUSY, 1);

		load_ani(28);
		draw_main_screen();
		init_ani(0);

		do {
			answer = GUI_radio(get_tx2(26), 2,
						get_tx2(27),
						get_tx2(28));
		} while (answer == -1);

		if (answer == 1) {
			/* wait */

			timewarp(MINUTES(15));

			do {
				answer = GUI_radio(get_tx2(29), 2,
							get_tx2(30),
							get_tx2(28));
			} while (answer == -1);

			if (answer == 1)
			{

				timewarp(MINUTES(15));

				/* loop until you decide to go on */
				do {
					answer = GUI_radio(get_tx2(31), 2,
								get_tx2(30),
								get_tx2(28));

				} while (answer == -1 || answer == 1);
			}
		}

		if (answer == 2) {
			/* go on */

			do {
				answer = GUI_radio(get_tx2(32), 2,
							get_tx2(33),
							get_tx2(34));

			} while (answer == -1);
		}

		do {

			if (answer == 1)
			{
				/* turn around */
				hours = GUI_input(get_tx2(35), 2);

				if (hours >= 4)
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(36),
						hours);

					GUI_output(Real2Host(ds_readd(DTP2)));

					done = 1;

				} else {

					/* entered time was too short */
					do {
						answer = GUI_radio(get_tx2(37), 2,
									get_tx2(33),
									get_tx2(34));

					} while (answer == -1);

					if (answer == 1)
					{
						timewarp(HOURS(4));

						GUI_output(get_tx2(38));

						done = 1;
					}
				}
			} else {
				do {
					answer = GUI_radio(get_tx2(39), 3,
								get_tx2(40),
								get_tx2(41),
								get_tx2(42));

				} while (answer == -1);

				if (answer == 1)
				{
					/* offer a deposit */
					ds_writeb(TEVENT037_FLAG, 1);

					/* select a hero randomly */
					hero = get_hero(answer = get_random_hero());

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(45),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					add_hero_ap(hero, 20);

					add_hero_ap_all(20);

					GUI_output(get_tx2(46));

					GUI_input(get_tx2(47), 10);

					/* check if the answer is "RAD" 1st */
					if (strcmp((char*)Real2Host(ds_readd(TEXT_INPUT_BUF)), (char*)p_datseg + STR_RAD1))
					{
						GUI_input(get_tx2(48), 10);

						/* check if the answer is "RAD" 2nd */
						if (strcmp((char*)Real2Host(ds_readd(TEXT_INPUT_BUF)), (char*)p_datseg + STR_RAD2))
						{
							GUI_input(get_tx2(49), 10);

							/* check if the answer is "RAD" 3rd */
							if (strcmp((char*)Real2Host(ds_readd(TEXT_INPUT_BUF)), (char*)p_datseg + STR_RAD3))
							{
								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_tx2(50),
									(char*)hero + HERO_NAME2,
									(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
									(char*)hero + HERO_NAME2,
									(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

								GUI_output(Real2Host(ds_readd(DTP2)));

								/* the hero is now lost completely */
								hero_disappear(hero, answer, -1);

								done = 1;
							} else {

								GUI_output(get_tx2(51));

								add_hero_ap_all(10);

								done = 1;
							}
						} else {

							GUI_output(get_tx2(51));

							add_hero_ap_all(10);

							done = 1;
						}
					} else {

						GUI_output(get_tx2(51));

						add_hero_ap_all(10);

						done = 1;
					}

				} else {
					/* talk of hetmann or hyggelik */

					do {
						answer = GUI_radio(get_tx2(43), 2,
									get_tx2(33),
									get_tx2(34));

					} while (answer == -1);

					if (answer == 2)
					{
						/* select a hero randomly */
						hero = get_hero(get_random_hero());

						timewarp(HOURS(1));

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_tx2(44),
							(char*)hero + HERO_NAME2);

						GUI_output(Real2Host(ds_readd(DTP2)));
					}
				}
			}

		} while (!done);

		set_var_to_zero();

		ds_writeb(EVENT_ANI_BUSY, 0);
		ds_writew(REQUEST_REFRESH, 1);
	}
}

/* unicorn 1st time */
void tevent_038(void)
{
	if (!ds_readb(MET_UNICORN_FLAG) && ds_readws(GOT_MAIN_QUEST) != 0)
	{
		do_talk(11, 2);
		ds_writeb(MET_UNICORN_FLAG, 1);
	}
}

void tevent_078(void)
{
	signed short tmp;
	signed short answer;
	signed short found_path;
	signed short days;
	Bit8u *hero;

	/* This event happens only in winter */
	if (!get_current_season())
	{

		do {
			answer = GUI_radio(get_tx2(78), 2,
						get_tx2(79),
						get_tx2(80));
		} while (answer == -1);

		load_in_head(57);

		if (answer == 1)
		{
			/* try to clean the path */

			/* with SHOVEL/SCHAUFEL 5 Hours, without 8 Hours */
			if (get_first_hero_with_item(73) != -1) {
				tmp = 5;
			} else {
				tmp = 8;
			}

			timewarp(HOURS(tmp));

			GUI_dialog_na(0, get_tx2(81));

			/* each hero in the group looses tmp / 2 LE */
			sub_group_le(tmp >> 1);

			ds_writews(WILDCAMP_SLEEP_QUALITY, -6);

			ds_writeb(LOCATION, LOCATION_WILDCAMP);
			do_location();
			ds_writeb(LOCATION, 0);

			TRV_load_textfile(-1);

#ifdef M302de_ORIGINAL_BUGFIX
			/* if a campfight occurs, the picture needs to be reloaded */
			load_in_head(57);
#endif
			GUI_dialog_na(0, get_tx2(84));

		} else {
			/* try to find another path */
			found_path = days = 0;

			do {
				/* ORIENTATION + 8 */
				if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 8) > 0)
				{
					/* success, you found a way */
					timewarp(HOURS(5));

					sub_group_le(2);

					GUI_dialog_na(0, !answer ? get_tx2(82) : get_tx2(85));

					found_path = 1;
				} else {
					/* fail */
					timewarp(HOURS(8));

					GUI_dialog_na(0, get_tx2(83));

					sub_group_le(4);

					ds_writews(WILDCAMP_SLEEP_QUALITY, -6);
					/* make a camp */
					ds_writeb(LOCATION, LOCATION_WILDCAMP);
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
				GUI_dialog_na(0, get_tx2(86));

				timewarp(HOURS(8));

				/* TODO: Original-Bug: all heroes die, even if they are not in the current group */
				hero = get_hero(0);
				for (tmp = 0; tmp <= 6; tmp++, hero += SIZEOF_HERO)
				{
#ifndef M302de_ORIGINAL_BUGFIX
					/* Original-Bug 16: all heroes die, even if they are not in the current group */
					hero_disappear(hero, tmp, -1);
#else
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP)
					) {
						hero_disappear(hero, tmp, -1);
					}
#endif
				}
			}
		}

		ds_writews(WILDCAMP_SLEEP_QUALITY, 0);
	}
}

void tevent_079(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 4) > 0 && !ds_readb(TEVENT079_FLAG)) ||
		ds_readb(TEVENT079_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT079_FLAG, 1);
	}
}

void tevent_051(void)
{
	signed short answer;

	if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_FAEHRTENSUCHEN, 4) > 0 && !ds_readb(TEVENT051_FLAG))
	{
		ds_writeb(TEVENT051_FLAG, 1);

		if (!TRV_follow_trail_question())
		{

			GUI_output(get_tx2(65));
			GUI_output(get_tx2(66));

			load_in_head(53);

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx2(67), 2,
							get_tx2(68),
							get_tx2(69));
			} while (answer == -1);

			if (answer == 1)
			{
				ds_writeb(TRAVEL_DETOUR, 3);
			}
		}

	} else if (ds_readb(TEVENT051_FLAG) != 0) {

		load_in_head(53);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx2(70), 2,
						get_tx2(71),
						get_tx2(72));
		} while (answer == -1);

		if (answer == 1)
		{
			ds_writeb(TRAVEL_DETOUR, 3);
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
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx2(73), 2,
						get_tx2(74),
						get_tx2(75));
		} while (answer == -1);

		if (answer == 1)
		{
			timewarp(HOURS(1));

			GUI_dialog_na(0, get_tx2(76));

			done = 1;
		} else {

			i = 0;

			do {
				/* check for a chopping tool */
				if (get_first_hero_with_item(ds_readb(TRAVEL_EVENT_AXES + i)) != -1)
				{
					done = 1;
				}

			} while(!done && ds_readb(TRAVEL_EVENT_AXES + ++i) != 255);

			if (done)
			{
				timewarp(HOURS(3));

				GUI_dialog_na(0, get_tx2(77));
			} else {
				GUI_dialog_na(0, get_tx2(78));
			}
		}

	} while (!done);
}

void tevent_120(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 3) > 0 && !ds_readb(TEVENT120_FLAG)) ||
		ds_readb(TEVENT120_FLAG) != 0)
	{
		ds_writeb(TEVENT120_FLAG, 1);
		TRV_found_camp_place(1);
	}
}

void tevent_121(void)
{
	TRV_cross_a_ford(get_tx(53), 30, 0);
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

	GUI_output(get_tx2(49));

	if (DNG_check_climb_tools() != -1)
	{
		have_climb_tools = 1;

		GUI_output(get_tx2(50));
	}

	do {
		answer = GUI_radio(get_tx2(51), 2,
					get_tx2(52),
					get_tx2(53));
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
				(skill_ret = test_skill(hero, TA_KLETTERN, -2)) <= 0)
			{
				/* the climb test failed */
				counter++;

				/* Original-Bug: a skill test with a fatal result returns -99, not -1 */
#ifdef M302de_ORIGINAL_BUGFIX
				if (skill_ret == -99 && have_climb_tools == 0)
#else
				if (skill_ret == -1 && have_climb_tools == 0)
#endif
				{
					/* fatal */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(56),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					hero_disappear(hero, i, -1);

				} else {

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(55),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_hero_le(hero, random_schick(5));
				}
			}
		}

		if (!counter)
		{
			GUI_output(get_tx2(54));
		}
	}

}

void tevent_125(void)
{
	signed short answer;

	load_in_head(17);

	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx2(57), 3,
					get_tx2(58),
					get_tx2(59),
					get_tx2(60));
	} while (answer == -1);

	if (answer == 1 || answer == 2)
	{
		GUI_dialog_na(0, answer == 1 ? get_tx2(61) : get_tx2(62));

		timewarp(MINUTES(30));
	}
}

/* Reach street (either Kravik-Skelellen or Peilinen-Rovamund). */
void tevent_145(void)
{
	signed short answer;

	load_area_description(0);

	if (!(cast_u16(ds_readb(ROUTE59_FLAG)) & 1))
	{
		do {
			answer = GUI_radio(get_tx2(42), 2,
						get_tx2(43),
						get_tx2(44));
		} while (answer == -1);

		if (answer == 1)
		{
			ds_writew(TRV_DESTINATION, TOWNS_KRAVIK);
			ds_writeb(CURRENT_TOWN, TOWNS_SKELELLE);

		} else {
			ds_writew(TRV_DESTINATION, TOWNS_SKELELLE);
			ds_writeb(CURRENT_TOWN, TOWNS_KRAVIK);
		}

	} else {
		do {
			answer = GUI_radio(get_tx2(39), 2,
						get_tx2(40),
						get_tx2(41));
		} while (answer == -1);

		if (answer == 1)
		{
			ds_writew(TRV_DESTINATION, TOWNS_PEILINEN);
			ds_writeb(CURRENT_TOWN, TOWNS_ROVAMUND);

		} else {
			ds_writew(TRV_DESTINATION, TOWNS_ROVAMUND);
			ds_writeb(CURRENT_TOWN, TOWNS_PEILINEN);
		}
	}

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx2(45),
		(char*)get_ttx(ds_readws(TRV_DESTINATION) + 0xeb));

	GUI_output(Real2Host(ds_readd(DTP2)));
}

#if !defined(__BORLANDC__)
}
#endif
