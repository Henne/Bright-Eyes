/*
 *	Rewrite of DSA1 v3.02_de functions of seg109 (travel events 1 / 10)
 *	Functions rewritten: 30/30 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg109.cpp
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
#include "seg030.h"
#include "seg032.h"
#include "seg047.h"
#include "seg051.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void TRV_load_textfile(signed short travel_event)
{
	load_buffer_1(ARCHIVE_FILE_FEATURE_LTX);

	if (travel_event == -1) {
		travel_event = ds_readws(0xb133);
	}

	load_city_ltx( (travel_event == 37 || travel_event == 47 || travel_event == 100) ? ARCHIVE_FILE_FEATURE9_LTX :
			( travel_event == 16 ? ARCHIVE_FILE_FEATURE8_LTX :
			( travel_event < 30 ? ARCHIVE_FILE_FEATURE1_LTX :
			( travel_event < 54 ? ARCHIVE_FILE_FEATURE2_LTX :
			( travel_event < 70 ? ARCHIVE_FILE_FEATURE3_LTX :
			( travel_event < 90 ? ARCHIVE_FILE_FEATURE4_LTX :
			( travel_event < 111 ? ARCHIVE_FILE_FEATURE5_LTX :
			( travel_event < 126 ? ARCHIVE_FILE_FEATURE6_LTX :
			( travel_event < 143 ? ARCHIVE_FILE_FEATURE7_LTX : ARCHIVE_FILE_FEATURE8_LTX)))))))));

	ds_writews(0xb133, travel_event);
}

#if defined(__BORLANDC__)
void TRV_event(signed short travel_event)
{
	signed short tw_bak;
	signed short bak1;
	signed short bak2;
	signed short traveling_bak;
	void (*event_handler)(void);

	TRV_load_textfile(travel_event);

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	bak1 = ds_readws(0x2ca2);
	bak2 = ds_readws(0x2ca4);
	traveling_bak = ds_readb(TRAVELING);
	ds_writews(0x2ca2, 0);
	ds_writews(0x2ca4, 0);
	ds_writeb(TRAVELING, 0);
	ds_writews(TEXTBOX_WIDTH, 9);
	ds_writeb(0x2c98, 1);
	ds_writeb(TRAVEL_EVENT_ACTIVE, 1);

	event_handler = (void (*)(void))ds_readd(TRAVEL_EVENT_HANDLERS + 4 * travel_event);
	if (event_handler) event_handler();

	ds_writeb(TRAVEL_EVENT_ACTIVE, 0);
	ds_writeb(TRAVELING, traveling_bak);
	ds_writews(0x2ca2, bak1);
	ds_writews(0x2ca4, bak2);
	ds_writews(TEXTBOX_WIDTH, tw_bak);
	ds_writeb(0x2c98, 0);
	load_buffer_1(ARCHIVE_FILE_MAPTEXT_LTX);
	ds_writew(0xe113, 1);
}
#endif

/**
 * \brief	executes a fight and load a textfile
 * \param fight_nr	ID of the fight
 * \param travel_event	ID of the travel event
 * \return return value of the fight
 */
signed short TRV_fight_event(signed short fight_nr, signed short travel_event)
{
	signed short retval;

	retval = do_fight(fight_nr);
	TRV_load_textfile(travel_event);

	return retval;
}

void TRV_found_herb_place(signed short a0)
{
	signed short answer;
	signed short randval;
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());

	randval = random_schick(5) + 2;

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x0000),
		(char*)get_dtp(4 * randval),
		(char*)hero + HERO_NAME2,
		(char*)(a0 != 0 ? get_dtp(0x00a8) : p_datseg + EMPTY_STRING10));

	do {
		answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
				get_dtp(0x0004),
				get_dtp(0x0008));

	} while (answer == -1);

	if (answer == 1) {
		TRV_inside_herb_place();
	}
}

void TRV_inside_herb_place(void)
{
	signed short hero_pos;
	signed short hours;
	signed short bak;

	load_ani(1);
	draw_main_screen();
	init_ani(0);
	hero_pos = select_hero_ok(get_ltx(0x518));

	if (hero_pos != -1) {

		hours = GUI_input(get_ltx(0x51c), 1);

		if (hours > 0) {
			bak = ds_readbs(0x45b8);
			ds_writebs(0x45b8, 1);

			gather_herbs(get_hero(hero_pos), hours - 1, -3);

			ds_writebs(0x45b8, (signed char)bak);
		}
	}

	set_var_to_zero();
	ds_writew(0x2846, 1);
}

signed short TRV_found_camp_place(signed short a0)
{
	signed short answer;
	signed short randval;

	randval = random_schick(5) + 10;
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x020),
		(char*)get_dtp(4 * randval),
		(char*)(a0 == 1 ? get_dtp(0xa8) : (a0 == 2 ? get_dtp(0xb4) : p_datseg + EMPTY_STRING11)));
	do {
		answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
					get_dtp(0x24),
					get_dtp(0x28));
	} while (answer == -1);

	if (answer == 1) {

		ds_writew(HERO_SLEEP_MOD, ds_writews(REPLENISH_STOCKS_MOD, ds_writews(GATHER_HERBS_MOD, 0)));

		if (a0 == 1) {
			ds_writews(REPLENISH_STOCKS_MOD, -3);
		} else if (a0 == 2) {
			ds_writews(GATHER_HERBS_MOD, -3);
		}

		ds_writeb(0xe4c8, 1);
		ds_writeb(LOCATION, 6);

		do_location();

		ds_writeb(LOCATION, ds_writeb(0xe4c8, 0));

		TRV_load_textfile(-1);

		ds_writew(HERO_SLEEP_MOD, ds_writews(REPLENISH_STOCKS_MOD, ds_writews(GATHER_HERBS_MOD, 0)));
		ds_writew(0x2846, 2);

		return 1;
	}

	return 0;
}

void TRV_found_replenish_place(signed short a0)
{
	signed short hero_pos;
	signed short answer;

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x040),
		(char*)get_dtp(4 * (random_schick(5) + 11)),
		(char*)(a0 != 0 ? get_dtp(0xb4) : p_datseg + EMPTY_STRING12));
	do {
		answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
					get_dtp(0x54),
					get_dtp(0x58));
	} while (answer == -1);

	if (answer == 1) {

		load_ani(1);
		draw_main_screen();
		init_ani(0);

		/* Original-Bug: This code assumes, that the leader of the group will replenish the stocks,
		 *		 which may not be the case, since replenish_stocks() asks who may do it.
		 *		 To be prepared clear these arrays for all heros.
		*/
#ifdef M302de_ORIGINAL_BUGFIX
		for (hero_pos = 0; hero_pos <= 7; hero_pos++) {
			ds_writeb(WILDCAMP_REPLSTATUS + hero_pos,
				ds_writeb(WILDCAMP_HERBSTATUS + hero_pos,
				ds_writeb(WILDCAMP_MAGICSTATUS + hero_pos,
				ds_writeb(WILDCAMP_GUARDSTATUS + hero_pos, 0))));
		}
#else
		hero_pos = get_hero_index(Real2Host(get_first_hero_available_in_group()));

		ds_writeb(WILDCAMP_REPLSTATUS + hero_pos, ds_writeb(WILDCAMP_HERBSTATUS + hero_pos, ds_writeb(WILDCAMP_MAGICSTATUS + hero_pos, ds_writeb(WILDCAMP_GUARDSTATUS + hero_pos, 0))));
#endif

		/* Original-Bug: the second argument is is the counter of replenish tries, not the position of the leader.
		 * Trigger:	Leave the first two hero slots empty an call this function. */
#ifdef M302de_ORIGINAL_BUGFIX
		replenish_stocks(-3, 0);
#else
		replenish_stocks(-3, hero_pos);
#endif

		timewarp(HOURS(1));

		set_var_to_zero();

		ds_writew(0x2846, 1);
	}
}

void TRV_found_inn(signed short city, signed short type)
{
	load_ani(12);
	draw_main_screen();
	init_ani(0);

	if (GUI_bool(get_dtp(0x5c))) {
		ds_writew(CITYINDEX, city);
		ds_writew(TYPEINDEX, type);
		ds_writeb(LOCATION, 7);

		do_location();

		ds_writeb(LOCATION, 0);

		TRV_load_textfile(-1);
	}

	set_var_to_zero();
	ds_writew(0x2846, 1);
}

/**
 * \brief	question if you want to rest in a hut
 * \return the answer 0 = no, 1 = yes
 */
signed short TRV_enter_hut_question(void)
{
	signed short answer;

	load_ani(8);
	draw_main_screen();
	init_ani(0);

	answer = GUI_bool(get_dtp(0x60));

	set_var_to_zero();
	ds_writew(0x2846, 1);

	return answer;
}

signed short TRV_follow_trail_question(void)
{
	signed short answer;
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x64),
		(char*)hero + HERO_NAME2);

	do {
		answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
				get_dtp(0x68),
				get_dtp(0x6c));

	} while (answer == -1);

	return answer - 1;
}

signed short TRV_cross_a_ford(Bit8u *msg, signed short time, signed short mod)
{
	signed short answer;
	signed short done;

	done = 0;
	ds_writeb(0xe5d2, 1);

	load_ani(7);
	draw_main_screen();
	init_ani(0);

	do {
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x88),
			(char*)msg);

		do {
			answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
						get_dtp(0x8c),
						get_dtp(0x90));
		} while (answer == -1);

		if (answer == 1) {
			done = 1;
			TRV_ford_test(mod, time);
		} else {
			answer = GUI_bool(get_dtp(0x9c));

			if (answer == 1) {
				done = ds_writew(TRV_RETURN, 1);
			}
		}

	} while (!done);

	set_var_to_zero();
	ds_writeb(0xe5d2, 0);
	ds_writew(0x2846, 1);
	return 1;
}

void TRV_ford_test(signed short mod, signed short time)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readbs(hero + HERO_TYPE) != 0 &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero))
		{
			/* Original-Bugfix: tests fail if their result is lower or equal than zero */
#ifdef M302de_ORIGINAL_BUGFIX
			if (test_attrib(hero, 4, mod) < 0)
#else
			if (test_attrib(hero, 4, mod) == 0)
#endif
			{
				/* test failed */
				timewarp(MINUTES(time));

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x94),
					(char*)hero + HERO_NAME2);

				hero_disease_test(hero, 2, 20 - (host_readbs(hero + HERO_KK) + host_readbs(hero + HERO_KK_MOD)));

				loose_random_item(hero, 1, get_ltx(0x7e8));
			} else {
				/* test succeded */
				timewarp(MINUTES(10));

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x98),
					(char*)hero + HERO_NAME2);
			}

			GUI_output(Real2Host(ds_readd(DTP2)));
		}
	}
}

signed short TRV_ferry(Bit8u *msg, signed short price)
{
	signed short done;
	signed short answer;
	signed short nr_heros;
	Bit32u p_money;

	done = 0;
	nr_heros = count_heroes_in_group();

	do {

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x70),
			(char*)msg,
			price, 5 * price);

		do {
			answer = GUI_radio(Real2Host(ds_readd(DTP2)), 3,
						get_dtp(0x74),
						get_dtp(0x78),
						get_dtp(0x7c));
		} while (answer == -1);

		if (answer == 1) {

			price *= 5 * nr_heros;
			p_money = get_party_money();

			if ((Bit32u)price > p_money) {
				GUI_output(get_ltx(0x644));
			} else {
				done = 1;
				p_money -= price;
				set_party_money(p_money);
				timewarp(MINUTES(30));
			}

		} else if (answer == 2) {

			price *= nr_heros;
			p_money = get_party_money();

			if ((Bit32u)price > p_money) {
				GUI_output(get_ltx(0x644));
			} else {
				p_money -= price;
				set_party_money(p_money);

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x80),
					done = random_schick(6));

				GUI_output(Real2Host(ds_readd(DTP2)));

				timewarp(HOURS(done));

				done = 1;
			}
		} else {

			if (GUI_bool(get_dtp(0x84))) {
				ds_writew(TRV_RETURN, done = 1);
			}
		}

	} while (!done);

	return 1;
}

void tevent_001(void)
{
	TRV_found_inn(40, 65);
}

/* The hunter Varnheim <-> Daspota */
void tevent_002(void)
{
	signed short answer;

	/* load dialog head */
	load_in_head(10);

	/* show dialogbox */
	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
				NULL,
				get_city(0x00),
				3,
				get_city(0x04),
				get_city(0x08),
				get_city(0x0c));

	} while (answer == -1);

	/* talk to the hunter for 15 min */
	if (answer == 1 || answer == 2) {

		GUI_dialog_na(0, answer == 1 ? get_city(0x10) : get_city(0x14));

		timewarp(MINUTES(15));
	}
}

void tevent_003(void)
{
	TRV_ferry(get_dtp(0x114), 5);
}

void tevent_004(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 26, 2) > 0 && !ds_readb(0x3da2)) ||
		ds_readb(0x3da2) != 0)
	{
		ds_writeb(0x3da2, 1);
		if (!TRV_follow_trail_question()) {
			TRV_hunt_generic(29, 6, -2, 4, 10, 8, 7, 5, 7, 80, 0);
		}
	}
}

void TRV_hunt_generic(signed short ani_id, signed short city_index, signed short mod1, signed short mod2,
			signed short mod3, signed short ap_all1, signed short ap_hero, signed short ap_all2,
			signed short ap_all3, signed short foods1, signed short foods2)
{
	signed short l_di;
	signed short i;
	signed short l4;
	signed short answer;
	Bit8u *hero;

	ds_writeb(0xe5d2, 1);

	load_ani(ani_id);
	draw_main_screen();
	init_ani(0);

	GUI_output(get_city(4 * city_index));

	hero = get_hero(0);
	for (i = l_di = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if ((host_readbs(hero + HERO_TYPE) != 0) &&
			(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
			!hero_dead(hero) &&
			test_skill(hero, 13, (signed char)mod1) <= 0)
		{
			l_di++;
		}
	}

	if (l_di) {

		do {
			answer = GUI_radio(get_city(4 * (city_index + 1)), 2,
						get_city(4 * (city_index + 7)),
						get_city(4 * (city_index + 8)));
		} while (answer == -1);

	} else {

		i = select_hero_ok_forced(get_city(4 * (city_index + 2)));

		hero = get_hero(i);

		if (test_skill(hero, 13, (signed char)mod2) <= 0) {

			do {
				answer = GUI_radio(get_city(4 * (city_index + 1)), 2,
							get_city(4 * (city_index + 7)),
							get_city(4 * (city_index + 8)));
			} while (answer == -1);
		} else {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(4 * (city_index + 3)),
				(char*)hero + HERO_NAME2);

			GUI_input(Real2Host(ds_readd(DTP2)), l_di = 0);

			if ((i = test_skill(hero, 7, (signed char)mod3)) > 0) {
				l_di++;
			}

			if ((l4 = test_skill(hero, 7, (signed char)mod3)) > 0) {
				l_di++;
			}

			if (l_di == 2 || i == 99 || l4 == 99) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(4 * (city_index + 4)),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				add_hero_ap_all(ap_all1);
				add_hero_ap(hero, ap_hero);

				timewarp(HOURS(1));

				get_item(45, 1, foods1);

				answer = 0;
			} else {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(4 * (l_di == 1 ? city_index + 5 : city_index + 6)),
					(char*)hero + HERO_NAME2);

				do {
					answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
								get_city(4 * (city_index + 7)),
								get_city(4 * (city_index + 8)));
				} while (answer == -1);
			}
		}
	}

	if (answer == 2) {

		add_hero_ap_all(ap_all2);

		if (foods2 != 0) {
			get_item(45, 1, foods2);
		}

	} else if (answer == 1) {

		GUI_output(get_city(4 * (city_index + 9)));

		timewarp(HOURS(2));

		add_hero_ap_all(ap_all3);
	}

	set_var_to_zero();
	ds_writeb(0xe5d2, 0);
	ds_writew(0x2846, 1);
}

void tevent_005(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 0) > 0 && !ds_readb(0x3da3)) ||
		ds_readb(0x3da3) != 0)
	{
		TRV_found_camp_place(1);
		ds_writeb(0x3da3, 1);
	}
}

void tevent_006(void)
{
	TRV_ferry(get_dtp(0x114), 4);
}

void tevent_007(void)
{
	signed short season = get_current_season();

	if (season == 3 || season == 1) {
		TRV_barrier(16);
	}
}

void TRV_barrier(signed short text_start)
{
	signed short i;
	signed short l_di;
	signed short answer;
	signed short done;
	Bit8u *hero;

	done = 0;

	load_in_head(54);

	do {

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						get_city(4 * (text_start + 0)), 2,
						get_city(4 * (text_start + 1)),
						get_city(4 * (text_start + 2)));
		} while (answer == -1);

		if (answer == 1) {

			timewarp(HOURS(2));

			GUI_dialog_na(0, get_city(4 * (text_start + 3)));

			done = 1;
		} else {

			hero = get_hero(0);
			for (i = l_di = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if (host_readbs(hero + HERO_TYPE) != 0 &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					l_di+=hero_count_item(hero, 121);
					l_di+=hero_count_item(hero, 32);
				}
			}

			if (l_di >= 2) {
				/* enough */

				i = 0;
				l_di = 1;

				do {
					if (get_first_hero_with_item(ds_readb(0xb135 + i)) != -1)
					{
						l_di = 0;
					}

					i++;

				} while (l_di && ds_readbs(0xb135 + i) != -1);

				if (l_di || get_first_hero_with_item(27) == -1) {

					GUI_dialog_na(0, get_city(4 * (text_start + 4)));

				} else {
					hero = get_hero(0);

					for (i = l_di = 0; i <= 6; i++, hero += SIZEOF_HERO)
					{
						if (test_skill(hero, 31, 0) > 0) l_di++;
					}

					add_hero_ap_all(10);

					i = get_item_pos(hero = get_hero(get_first_hero_with_item(121)), 121);
					if (i == -1) {
						i = get_item_pos(hero = get_hero(get_first_hero_with_item(32)), 32);
					}
					drop_item(hero, i, 1);

					i = get_item_pos(hero = get_hero(get_first_hero_with_item(121)), 121);
					if (i == -1) {
						i = get_item_pos(hero = get_hero(get_first_hero_with_item(32)), 32);
					}
					drop_item(hero, i, 1);

					if (l_di >= 3) {

						GUI_dialog_na(0, get_city(4 * (text_start + 6)));
					} else {

						hero = get_hero(select_hero_ok_forced(get_city(4 * (text_start + 5))));

						sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_city(4 * (text_start + 7)),
								(char*)hero + HERO_NAME2,
								Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)),
								Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));
						GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

						hero_disease_test(hero, 2, 30);

						loose_random_item(hero, 30, get_ltx(0x7e8));
					}
					done = 1;
				}
			} else {
				/* not enough */
				GUI_dialog_na(0, get_city(4 * (text_start + 4)));
			}
		}

	} while (done == 0);
}

void tevent_008(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 2) > 0 && !ds_readb(0x3da4)) ||
		ds_readb(0x3da4) != 0)
	{
		TRV_found_replenish_place(0);
		ds_writeb(0x3da4, 1);
	}
}

void tevent_009(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 29, 4) > 0 && !ds_readb(0x3da5)) ||
		ds_readb(0x3da5) != 0)
	{
		ds_writeb(0x66d0, 60);
		TRV_found_herb_place(0);
		ds_writeb(0x66d0, 255);
		ds_writeb(0x3da5, 1);
	}
}

void tevent_010(void)
{
	TRV_found_inn(43, 66);
}

void tevent_012(void)
{
	TRV_cross_a_ford(get_dtp(0xd4), 30, 0);
}

void tevent_018(void)
{
	TRV_cross_a_ford(get_dtp(0xd4), 20, 0);
}

void tevent_019(void)
{
	TRV_cross_a_ford(get_dtp(0x118), 20, 0);
}

void tevent_023(void)
{
	TRV_ferry(get_dtp(0x114), 3);
}

void tevent_092(void)
{
	do_talk(0, 0);
}

#if !defined(__BORLANDC__)
}
#endif
