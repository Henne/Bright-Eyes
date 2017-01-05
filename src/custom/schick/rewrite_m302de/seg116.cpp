/**
 *	Rewrite of DSA1 v3.02_de functions of seg116 (travel events 8 / 10)
 *	Functions rewritten: 17/17 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg116.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg030.h"
#include "seg047.h"
#include "seg048.h"
#include "seg065.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
void sub_light_timers(signed short);
#endif

void tevent_130(void)
{
	signed short answer;

	if (TRV_enter_hut_question()) {

		ds_writews(CAMP_INCIDENT, 0);

		ds_writeb(LOCATION, LOCATION_WILDCAMP);
		do_location();
		ds_writeb(LOCATION, 0);

		TRV_load_textfile(-1);
		ds_writews(CAMP_INCIDENT, -1);

		load_in_head(10);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						get_city(0x4c), 2,
						get_city(0x50),
						get_city(0x54));
		} while (answer == -1);

		if (answer == 1) {

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
							get_city(0x58), 3,
							get_city(0x5c),
							get_city(0x60),
							get_city(0x64));
			} while (answer == -1);

			if (answer == 1 || answer == 2) {
				GUI_dialog_na(0, answer == 1 ? get_city(0x68) : get_city(0x6c));
			} else {
					GUI_dialog_na(0, test_attrib(Real2Host(get_first_hero_available_in_group()), ATTRIB_CH, 0) > 0 ? get_city(0x6c) : get_city(0x68));
			}
		}

		ds_writeb(LOCATION, LOCATION_WILDCAMP);
		do_location();
		ds_writeb(LOCATION, 0);
	}
}

void tevent_131(void)
{
	/* entrance to the pirate cave */
	signed short answer;

	if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 8) > 0 && !ds_readb(TEVENT131_FLAG)) {

		ds_writeb(TEVENT131_FLAG, 1);

		GUI_output(get_city(0x70));

		do {
			answer = GUI_radio(get_city(0x74), 2,
						get_city(0x78),
						get_city(0x7c));
		} while (answer == -1);

		if (answer == 1) {
			ds_writeb(0x4333, 11);
		}

	} else {

		if (ds_readb(TEVENT131_FLAG) != 0) {

			do {
				answer = GUI_radio(get_city(0x80), 2,
							get_city(0x84),
							get_city(0x88));
			} while (answer == -1);

			if (answer == 1) {
				ds_writeb(0x4333, 11);
			}
		}
	}
}

void tevent_132(void)
{

	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 1) > 0 && !ds_readb(TEVENT132_FLAG)) || ds_readb(TEVENT132_FLAG) != 0)
	{

		/* set this camp place as known */
		ds_writeb(TEVENT132_FLAG, 1);

		if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 1) > 0 && !ds_readb(TEVENT132_HERB_FLAG)) || ds_readb(TEVENT132_HERB_FLAG) != 0) {
			ds_writeb(TEVENT132_HERB_FLAG, 1);
			ds_writebs(0x66d0, 61);
			TRV_found_camp_place(2);
			ds_writebs(0x66d0, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

void tevent_133(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_city(0x8c), 2,
					get_city(0x90),
					get_city(0x94));
	} while (answer == -1);

	if (answer == 1) {
		ds_writews(TRV_RETURN, 1);
	} else {

		GUI_output(get_city(0x98));

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero))
			{
				timewarp(HOURS(1));

				if (test_attrib(hero, ATTRIB_GE, 4) <= 0) {
					/* test failed */

					timewarp(HOURS(1));

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x9c),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)),
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_hero_le(hero, random_schick(8));

					loose_random_item(hero, 50, get_ltx(0x7e8));
					loose_random_item(hero, 50, get_ltx(0x7e8));
					loose_random_item(hero, 50, get_ltx(0x7e8));
				}
			}
		}

		GUI_output(get_city(0xa0));
	}
}

void tevent_134(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 6) > 0 && !ds_readb(TEVENT134_FLAG)) ||
		ds_readb(TEVENT134_FLAG) != 0)
	{
		ds_writeb(0x66d0, 157);
		TRV_found_herb_place(0);
		ds_writeb(0x66d0, -1);
		ds_writeb(TEVENT134_FLAG, 1);
	}
}

void tevent_135(void)
{
	signed short l_si;
	signed short l_di;
	signed short answer;
	signed short done;
	signed short count;
	Bit8u *hero;

	load_ani(1);
	draw_main_screen();
	init_ani(0);

	GUI_output(get_city(0xa4));

	do {
		answer = GUI_radio(get_city(0xa8), 2,
					get_city(0xac),
					get_city(0xb0));
	} while (answer == -1);

	if (answer == 1) {

		do {
			done = 0;

			hero = get_hero(select_hero_ok_forced(get_city(0xb4)));

			l_si = 1;
			if (test_skill(hero, TA_KLETTERN, -1) > 0) {
				l_si = 2;
				GUI_output(get_city(0xb8));

				if (test_skill(hero, TA_KLETTERN, 1) > 0) {
					l_si = 3;
					GUI_output(get_city(0xbc));

					if (test_skill(hero, TA_KLETTERN, 0) > 0) {
						l_si = 4;
						GUI_output(get_city(0xc0));

						if (test_skill(hero, TA_KLETTERN, 2) > 0) {
							l_si = 5;
							GUI_output(get_city(0xc4));

							if (test_skill(hero, TA_KLETTERN, 1) > 0) {

								GUI_output(get_city(0xc8));

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_city(0xd8),
									(char*)hero + HERO_NAME2,
									(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));
								GUI_output(Real2Host(ds_readd(DTP2)));

								load_in_head(45);

								sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
									(char*)get_city(0xdc),
									(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
									(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

								GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

								for (l_di = count = 0; l_di < 9; l_di++) {
									if (ds_readb(TREASURE_MAPS + l_di) != 0) {
										count++;
									}
								}

								if (count < 5) {
									sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
										(char*)get_city(0xe0),
										(char*)hero + HERO_NAME2,
										(char*)get_city(0xe4));
								} else {
									sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
										(char*)get_city(0xe0),
										(char*)hero + HERO_NAME2,
										(char*)get_dtp(4 * random_interval(54, 67)));
								}

								GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

								if (!(host_readbs(hero + HERO_START_GEAR) & 0x2)) {
									or_ptr_bs(hero + HERO_START_GEAR, 2);
									inc_ptr_bs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_IN));
									inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_IN));
								}

								sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
									(char*)get_city(0xe8),
									(char*)hero + HERO_NAME2,
									(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));
								GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

								l_si = 0;
								done = 1;
							}
						}
					}
				}
			}

			if (l_si) {
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0xcc),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				l_di = random_interval(ds_readb(0xb172 + 2 * l_si), host_readb((p_datseg + 0xb172 + 1) + (2 * l_si)));
				l_si = host_readws(hero + HERO_LE);
				l_si -= l_di;

				sub_hero_le(hero, l_di);

				if (l_si <= 0) {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)(!l_si ? get_city(0xd4) : get_city(0xd0)),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));
				}

				if (count_heroes_available_in_group()) {

					if (!GUI_bool(get_city(0xec))) {
						done = 1;
					}
				} else {
					done = 1;
				}
			}

		} while (done == 0);
	}

	set_var_to_zero();
	ds_writew(REQUEST_REFRESH, 1);
}

void tevent_137(void)
{
	signed short i;
	signed short answer;
	signed short item_pos;
	Bit8u *hero;

	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 5) > 0 && !ds_readb(TEVENT137_FLAG)) ||
		ds_readb(TEVENT137_FLAG) != 0)
	{
		ds_writeb(TEVENT137_FLAG, 1);

		do {
			answer = GUI_radio(get_city(0xf0), 2,
						get_city(0xf4),
						get_city(0xf8));
		} while (answer == -1);

		if (answer == 1) {

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
					(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
					!hero_dead(hero))
				{
					/* each hero gets five FOODPACKAGES */
					give_hero_new_item(hero, 45, 1, 5);

					/* search for the WATERSKIN */
					if ((item_pos = get_item_pos(hero, 30)) != -1)
					{
						/* reset empty and half_empty bits of the knapsack item status */
#if !defined(__BORLANDC__)
						and_ptr_bs(hero + HERO_ITEM_HEAD + 4 + 14 * item_pos, 0xfb);
						and_ptr_bs(hero + HERO_ITEM_HEAD + 4 + 14 * item_pos, 0xfd);
#else
						(*(struct knapsack_status*)(hero + HERO_ITEM_HEAD + 4 + 14 * item_pos)).half_empty =
							(*(struct knapsack_status*)(hero + HERO_ITEM_HEAD + 4 + 14 * item_pos)).empty = 0;
#endif
					}

					host_writebs(hero + HERO_HUNGER, host_writebs(hero + HERO_THIRST, 0));

					add_hero_le(hero, 2);
				}
			}
		}
	}
}

void tevent_138(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT138_FLAG)) ||
		ds_readb(TEVENT138_FLAG) != 0)
	{
		ds_writeb(TEVENT138_FLAG, 1);
		TRV_found_camp_place(1);
	}
}

void tevent_139(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_city(0xfc), 2,
					get_city(0x100),
					get_city(0x104));
	} while (answer == -1);

	if (answer == 1) {

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
				(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
				!hero_dead(hero))
			{
				sub_hero_le(hero, random_schick(2));
			}
		}

		timewarp(HOURS(6));

		GUI_output((i = test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 3)) > 0 ? get_city(0x108) : get_city(0x110));
	} else {
		timewarp(HOURS(2));

		GUI_output((i = test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 5)) > 0 ? get_city(0x10c) : get_city(0x114));
	}

	if (i <= 0) {
		if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 3) > 0) {

			timewarp(HOURS(2));

			GUI_output(get_city(0x118));

		} else {

			timewarp(HOURS(3));

			GUI_output(get_city(0x11c));

			ds_writeb(LOCATION, LOCATION_WILDCAMP);
			do_location();
			ds_writeb(LOCATION, 0);

		}
	}
}

void tevent_140(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT140_FLAG)) || ds_readb(TEVENT140_FLAG) != 0) {

		/* set this camp place as known */
		ds_writeb(TEVENT140_FLAG, 1);

		if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 4) > 0 && !ds_readb(TEVENT140_HERB_FLAG)) || ds_readb(TEVENT140_HERB_FLAG) != 0) {
			ds_writeb(TEVENT140_HERB_FLAG, 1);
			ds_writebs(0x66d0, -126);
			TRV_found_camp_place(2);
			ds_writebs(0x66d0, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

void tevent_141(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_FAEHRTENSUCHEN, 0) > 0 && !ds_readb(TEVENT141_FLAG)) ||
		ds_readb(TEVENT141_FLAG) != 0)
	{
		/* set this camp place as known */
		ds_writeb(TEVENT141_FLAG, 1);

		if (!TRV_follow_trail_question()) {
			TRV_hunt_generic(21, 73, -1, 5, 10, 15, 5, 5, 10, 50, 0);
		}
	}
}

void tevent_142(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT142_FLAG)) ||
		ds_readb(TEVENT142_FLAG) != 0)
	{

		/* set this camp place as known */
		ds_writeb(TEVENT142_FLAG, 1);
		TRV_found_camp_place(1);
	}
}

void tevent_143(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_city(0x00), 2,
					get_city(0x04),
					get_city(0x08));
	} while (answer == -1);

	if (answer == 1) {

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
				(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)))
			{
				sub_hero_le(hero, random_schick(2) + 1);
			}
		}

		timewarp(HOURS(4));

		GUI_output((i = test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 2)) > 0 ? get_city(0x0c) : get_city(0x14));
	} else {
		timewarp(HOURS(1));

		GUI_output((i = test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 4)) > 0 ? get_city(0x10) : get_city(0x18));
	}

	if (i <= 0) {

		if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 3) > 0) {

			timewarp(HOURS(1));

			GUI_output(get_city(0x1c));

		} else {

			timewarp(HOURS(3));

			GUI_output(get_city(0x20));

			ds_writeb(LOCATION, LOCATION_WILDCAMP);
			do_location();
			ds_writeb(LOCATION, 0);
		}
	}
}

/* Orcs' military camp */
void tevent_144(void)
{
	signed short grimring_hero_pos;
	signed short right_time_flag;
	Bit8u *hero;

	right_time_flag = 0;

	if ((grimring_hero_pos = get_first_hero_with_item(181)) != -1) {

		if (ds_readbs(YEAR) == 17 && ds_readbs(MONTH) == 10 && ds_readbs(DAY_OF_MONTH) >= 10) {
			right_time_flag = 1;
		} else {
			if (ds_readb(ORCDOCUMENT_READ_FLAG) != 0 && GUI_bool(get_city(0xb8))) {
			/* the time is not right, forward time  */

				GUI_output(get_city(0xbc));

				/* set date */
				ds_writebs(YEAR, 17);
				ds_writebs(MONTH, 10);
				ds_writebs(DAY_OF_MONTH, 10);

				sub_ingame_timers(MONTHS(1));
				sub_mod_timers(MONTHS(1));
				seg002_2f7a(0x21c0);
				sub_light_timers(100);
				right_time_flag = 1;
			}
		}

		if (right_time_flag) {

			load_in_head(44);
			memmove(Real2Host(ds_readd(BUFFER10_PTR)), Real2Host(ds_readd(DTP2)), 0x400);

			hero = get_hero(grimring_hero_pos);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x28),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x2c),
				(char*)hero + HERO_NAME2);

			GUI_dialogbox((RealPt)ds_readd(BUFFER10_PTR), (RealPt)0, Real2Host(ds_readd(DTP2)), 0);
			GUI_dialogbox((RealPt)ds_readd(BUFFER10_PTR), (RealPt)0, get_city(0x30), 0);
			GUI_dialogbox((RealPt)ds_readd(BUFFER10_PTR), (RealPt)0, get_city(0x34), 0);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x38),
				(char*)hero + HERO_NAME2);

			GUI_dialogbox((RealPt)ds_readd(BUFFER10_PTR), (RealPt)0, Real2Host(ds_readd(DTP2)), 0);
			GUI_dialogbox((RealPt)ds_readd(BUFFER10_PTR), (RealPt)0, get_city(0x3c), 0);

			do {
				status_menu(grimring_hero_pos);

				grimring_hero_pos = get_first_hero_with_item(181);

				if (grimring_hero_pos == -1) {
					grimring_hero_pos = 0;
				}

				if (grimring_hero_pos == 6) {

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x98),
						(char*)get_hero(6) + HERO_NAME2);

					GUI_dialogbox((RealPt)ds_readd(HEROS) + SIZEOF_HERO * 6 + HERO_PORTRAIT,
							Real2Host(ds_readd(HEROS)) + SIZEOF_HERO * 6  + HERO_NAME2,
							Real2Host(ds_readd(DTP2)), 0);
				}

			} while (grimring_hero_pos == 6);

			ds_writed(MAIN_ACTING_HERO, (Bit32u)((RealPt)ds_readd(HEROS) + SIZEOF_HERO * grimring_hero_pos));

			final_intro();
			if (!TRV_fight_event(FIGHTS_F144, 144)) {
				/* you won the final fight */
				GUI_output(get_city(0x44));

				ds_writews(GAME_STATE, GAME_STATE_VICTORY);

				add_hero_ap_all(500);

				ds_writebs(LOCATION, 0);
				ds_writews(NPC_MONTHS, 200);
			} else {
				/* you lost the final fight */
				GUI_output(get_city(0x48));
				ds_writews(GAME_STATE, GAME_STATE_DEAD);
			}
		}
	}
}

/* TODO: this function is unused */
void tevent_144_unused(void)
{
	GUI_output(get_city(0x40));
}

void TLK_old_woman(signed short state)
{
	signed short l_di;
	signed short counter;
	Bit8u *hero;

	if (state == 3) {

		hero = get_hero(0);
		for (l_di = counter = 0; l_di <= 6; l_di++, hero += SIZEOF_HERO) {
			/* Original-Bug: check if this is realy a hero in the current group and alive before test_skill() */
			if (test_skill(hero, TA_VERSTECKEN, -5) > 0) {
				counter++;
			}
		}

		ds_writew(DIALOG_NEXT_STATE, count_heroes_in_group() == counter ? 4 : 5);

	} else if (state == 4 || state == 14 || state == 21) {

		timewarp(MINUTES(15));

	} else if (state == 6 || state == 30) {

		timewarp(MINUTES(30));

	} else if (state == 23) {

		hero = get_hero(0);
		for (l_di = counter = 0; l_di <= 6; l_di++, hero += SIZEOF_HERO) {
			/* Original-Bug: check if this is realy a hero in the current group and alive before test_skill() */
			if (test_skill(hero, TA_VERSTECKEN, -5) > 0) {
				counter++;
			}
		}

		ds_writed(RANDOM_TLK_HERO, (Bit32u)((RealPt)ds_readd(HEROS) + SIZEOF_HERO * get_random_hero()));

		ds_writew(DIALOG_NEXT_STATE, count_heroes_in_group() == counter ? 24 : 25);

	} else if (state == 33) {

		/* Infinity-Loop */
		do {
		} while (1);

	} else if (state == 34) {
		ds_writew(DIALOG_NEXT_STATE, ds_readb(CURRENT_TOWN) == 20 ? 35 : 39);
	} else if (state == 37) {
		ds_writeb(TEVENT093_FLAG, ds_writeb(TEVENT088_FLAG, 1));
	} else if (state == 38) {
		timewarp(HOURS(1));
	} else if (state == 41) {
		ds_writeb(TEVENT093_FLAG, ds_writeb(TEVENT091_FLAG, ds_writeb(INFORMER_FLAGS + INFORMER_ISLEIF, ds_writeb(ISLEIF_JANDAS_REGARDS, 1))));
	}
}

void tevent_136(void)
{
	if (ds_readb(MET_UNICORN_FLAG) != 0 &&
		ds_readw(GOT_MAIN_QUEST) != 0 &&
		ds_readb(UNICORN_GET_MAP) != 0 &&
		!ds_readb(UNICORN_TIMER))
	{
		do_talk(12, 1);
		ds_writeb(MET_UNICORN_FLAG, 1);
		ds_writeb(UNICORN_GET_MAP, 0);
	}
}

#if !defined(__BORLANDC__)
}
#endif
