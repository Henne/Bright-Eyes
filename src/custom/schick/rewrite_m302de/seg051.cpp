/*
 *	Rewrite of DSA1 v3.02_de functions of seg051 (wilderness camp)
 *	Functions rewritten: 3/3 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg051.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg027.h"
#include "seg029.h"
#include "seg032.h"
#include "seg047.h"
#include "seg049.h"
#include "seg051.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg104.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void do_wildcamp(void)
{
	signed short l_si;
	signed short l_di;
	signed short i;
	signed short done;
	signed short answer;
	signed char stock_tries;
	signed char herb_tries;
	signed char herb_hours;
	RealPt hero;
	signed char l3;
	signed short l4;
	signed short l5;
	signed short l6;
	signed char have_guards;
	signed char l8;

	done = 0;
	stock_tries = 0;
	herb_tries = 0;
	l_di = ds_writews(REQUEST_REFRESH, 1);

	for (i = 0; i <= 6; i++) {
		ds_writebs(WILDCAMP_HERBSTATUS + i, ds_writebs(WILDCAMP_REPLSTATUS + i, ds_writebs(WILDCAMP_MAGICSTATUS + i , ds_writeb(WILDCAMP_GUARDSTATUS + i, 0))));
	}

	for (i = 0; i < 3; i++) {
		ds_writebs(WILDCAMP_GUARDS + i, -1);
	}

	i = !ds_readb(GOOD_CAMP_PLACE) ? 6 : 7;
	draw_loc_icons(i, MENU_ICON_GUARDS, MENU_ICON_REPLENISH_SUPPLIES, MENU_ICON_APPLY_SKILL, MENU_ICON_MAGIC, MENU_ICON_COLLECT_HERBS, MENU_ICON_SLEEP, MENU_ICON_QUIT_CAMP);

	while (done == 0) {

		if (ds_readws(REQUEST_REFRESH) != 0) {
			draw_main_screen();
			set_var_to_zero();
			load_ani(2);
			init_ani(0);
			GUI_print_loc_line(get_ttx(306));
			set_audio_track(ARCHIVE_FILE_CAMP_XMI);
			ds_writew(REQUEST_REFRESH, l_di = 0);
		}

		if (l_di) {
			GUI_print_loc_line(get_ttx(306));
			l_di = 0;
		}

		handle_gui_input();

		if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

			i = !ds_readb(GOOD_CAMP_PLACE) ? 6 : 7;

			answer = GUI_radio(get_ttx(307), (signed char)i,
						get_ttx(308), get_ttx(309),
						get_ttx(212), get_ttx(310),
						get_ttx(315), get_ttx(316),
						get_ttx(814)) -1;

			if (answer != -2) {
				ds_writews(ACTION, answer + ACTION_ID_ICON_1);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_1) {

			answer = -1;

			for (i = 0; i <= 6; i++) {

				if (!ds_readbs(WILDCAMP_MAGICSTATUS + i) && !ds_readbs(WILDCAMP_HERBSTATUS + i) &&
					!ds_readbs(WILDCAMP_REPLSTATUS + i) && is_hero_available_in_group(get_hero(i)))
				{
					ds_writeb(WILDCAMP_GUARDSTATUS + i, 0);
					answer = 0;
				}
			}

			if (answer == -1) {
				GUI_output(get_ttx(332));
			} else {

				for (i = 0; i < 3; i++) {

					sprintf((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(321), i + 1);

					do {
						answer = select_hero_ok(Real2Host(ds_readd(DTP2)));

						/* Original-Bug: not checked answer for following options */
						if ((answer != -1 && ds_readbs(WILDCAMP_MAGICSTATUS + answer) != 0) ||
							ds_readbs(WILDCAMP_HERBSTATUS + answer) != 0 ||
							ds_readbs(WILDCAMP_REPLSTATUS + answer) != 0)
						{
							GUI_output(get_ttx(331));
							answer = -1;
						}

						if (answer != -1) {
							if (hero_brewing(get_hero(answer))) {
								GUI_output(get_ttx(730));
								answer = -1;
							}
						}

					} while (answer == -1);

					inc_ds_bs_post(WILDCAMP_GUARDSTATUS + answer);
					ds_writebs(WILDCAMP_GUARDS + i, (signed char)answer);
				}
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_2) {

			if (ds_readb(GOOD_CAMP_PLACE) == 99) {
				l_di = replenish_stocks(ds_readws(REPLENISH_STOCKS_MOD) + 99, stock_tries);
			} else {
				l_di = replenish_stocks(ds_readws(REPLENISH_STOCKS_MOD), stock_tries);
			}

			if (l_di) {
				stock_tries++;
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_3) {

			GUI_use_skill2(0, get_ttx(395));

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_4) {

			answer = select_hero_ok(get_ttx(317));

			if (answer != -1) {
				if (hero_brewing(get_hero(answer))) {
					GUI_output(get_ttx(730));
					answer = -1;
				}
			}

			if (answer != -1) {

				hero = (RealPt)ds_readd(HEROES) + SIZEOF_HERO * answer;


				if (host_readbs(Real2Host(hero) + HERO_TYPE) >= 7) {

					if (ds_readbs(WILDCAMP_GUARDSTATUS + answer) != 0 ||
						ds_readbs(WILDCAMP_HERBSTATUS + answer) != 0 ||
						ds_readbs(WILDCAMP_REPLSTATUS + answer) != 0)
					{
						GUI_output(get_ttx(331));

					} else {

						if (ds_readbs(WILDCAMP_MAGICSTATUS + answer) != 0) {
							GUI_output(get_ttx(334));
						} else {
							ds_writebs(WILDCAMP_MAGICSTATUS + answer, (signed char)use_magic(hero));
						}
					}
				} else {
					GUI_output(get_ttx(330));
				}
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_5) {
			/* COLLECT HERBS */

			ds_writews(SKILLED_HERO_POS, get_skilled_hero_pos(TA_PFLANZENKUNDE));

			answer = select_hero_ok(get_ttx(326));

			if (answer != -1 && hero_brewing(get_hero(answer))) {
				GUI_output(get_ttx(730));
				answer = -1;
			}

			if (answer != -1) {

				if (ds_readbs(WILDCAMP_HERBSTATUS + answer) != 0)
				{

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(803),
						(char*)get_hero(answer) + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));

				} else if (ds_readbs(WILDCAMP_GUARDSTATUS + answer) != 0 ||
						ds_readbs(WILDCAMP_REPLSTATUS + answer) != 0 ||
						ds_readbs(WILDCAMP_MAGICSTATUS + answer) != 0)
				{
					GUI_output(get_ttx(331));

				} else {

					if (herb_tries < 1)
					{
						hero = (RealPt)ds_readd(HEROES) + SIZEOF_HERO * answer;

						herb_hours = (signed char)GUI_input(get_ttx(327), 1);

						if (herb_hours > 0)
						{
							ds_writebs(WILDCAMP_HERBSTATUS + answer, herb_tries = (signed char)(l_di = 1));

							if (ds_readbs(GOOD_CAMP_PLACE) == 99) {
								gather_herbs(Real2Host(hero), herb_hours - 1, ds_readws(GATHER_HERBS_MOD) + 99);
							} else {
								gather_herbs(Real2Host(hero), herb_hours - 1, ds_readws(GATHER_HERBS_MOD));
							}
						}
					} else {
						GUI_output(get_ttx(336));
					}
				}
			}
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_6) {

			if (GUI_bool(get_ttx(318))) {

				l3 = (signed char)(ds_readds(DAY_TIMER) / HOURS(1));

				l3 = ds_readds(DAY_TIMER) < HOURS(8) ? 8 - l3 : 24 - l3 + 8;

				l4 = l3 / 3;
				l5 = l4;
				l_si = 0;
				have_guards = 0;

				if (ds_readws(CAMP_INCIDENT) == -1) {

					if ((ds_readbs(WILDCAMP_GUARDS) == -1 ? 60 : 10) > random_schick(100) && !ds_readds(INGAME_TIMERS + 0x10))
					{
						ds_writews(CAMP_INCIDENT, random_schick(3) - 1);
					}
				} else {
					have_guards = 1;
				}

				l8 = 0;
				l6 = l3;

				if (ds_readbs(WILDCAMP_GUARDS + l_si) != -1) {

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(774),
						(char*)get_hero(ds_readbs(WILDCAMP_GUARDS + l_si)) + HERO_NAME2);

					GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
				}

				do {

					ds_writeb(FOOD_MOD, 1);
					timewarp(HOURS(1));
					ds_writeb(FOOD_MOD, 0);
					l5--;
					l8++;
					l6--;

					if (l_si == ds_readws(CAMP_INCIDENT) && l4 / 2 >= l5) {
						done = 1;
					}

					if (l5 == 0 && l_si < 2) {

						l5 = l4;
						l_si++;

						if (ds_readbs(WILDCAMP_GUARDS + l_si) != -1) {

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(774),
								(char*)get_hero(ds_readbs(WILDCAMP_GUARDS + l_si)) + HERO_NAME2);

							GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
						}
					}

				} while (l6 > 0 && done == 0);

				if (done == 0) {

					hero = (RealPt)ds_readd(HEROES);

					for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {
						if (host_readbs(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(Real2Host(hero) + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
							ds_readbs(WILDCAMP_GUARDSTATUS + i) < 2 &&
							ds_readbs(WILDCAMP_MAGICSTATUS + i) != 1)
						{
							GRP_hero_sleep(Real2Host(hero), ds_readws(WILDCAMP_SLEEP_QUALITY));
						}
					}

				} else if (!have_guards) {

					ds_writews(CAMP_INCIDENT, -1);
					ds_writeb(FIG_INITIATIVE, 1);
					ds_writew(FIG_DISCARD, 1);

					do_fight(ds_readws((CAMPFIGHTS-2) + 2 * random_schick(4)));

					if (ds_readb(TRAVEL_DETOUR) != 99 && ds_readw(GAME_STATE) == GAME_STATE_MAIN) {

						draw_main_screen();
						set_var_to_zero();
						load_ani(2);
						init_ani(0);
						GUI_print_loc_line(get_ttx(306));
						set_audio_track(ARCHIVE_FILE_CAMP_XMI);

						ds_writew(REQUEST_REFRESH, l_di = 0);

						if (l6 > 0) {
							ds_writeb(FOOD_MOD, 1);
							timewarp_until(HOURS(8));
							ds_writeb(FOOD_MOD, 0);
						}
					}
				} else {
					ds_writews(CAMP_INCIDENT, ds_readbs(WILDCAMP_GUARDS + ds_readws(CAMP_INCIDENT)));
				}

				done = 1;
			}
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_7) {
			done = 1;
		}
	}

	for (i = 0; i <= 6; i++) {
		ds_writeb(WILDCAMP_HERBSTATUS + i, ds_writeb(WILDCAMP_REPLSTATUS + i, 0));
	}

	leave_location();
}

signed short gather_herbs(Bit8u *hero, signed short hours, signed short mod)
{
	signed short i;
	signed short herbs;
	Bit8u *ptr;
	signed char herb_count[12];

	memset(herb_count, 0 , 12);

	timewarp(HOURS((hours + 1)));

	ptr = p_datseg + GATHER_HERBS_TABLE;

	for (herbs = i = 0; i < 12; i++, ptr += 4) {

		if (host_readb(ptr) == ds_readb(GATHER_HERBS_SPECIAL)) {
			add_ptr_bs(ptr + 1, 10);
			inc_ptr_bs(ptr + 2);
		}

		if (random_schick(100) <= host_readb(ptr + 1) &&
			test_skill(hero, TA_PFLANZENKUNDE, host_readb(ptr + 3) - hours + mod) > 0) {

			herb_count[i] = (signed char)give_hero_new_item(hero, host_readb(ptr), 0, random_schick(host_readb(ptr + 2)));

			if (herb_count[i] != 0) {
				herbs++;
			}
		}

		if (host_readb(ptr) == ds_readb(GATHER_HERBS_SPECIAL)) {
			sub_ptr_bs(ptr + 1, 10);
			dec_ptr_bs(ptr + 2);
		}
	}

	if (herbs) {

		/* print a sentence with all the herb names */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(328),
			(char*)hero + HERO_NAME2);

		for (i = 0; i < 12; i++) {

			if (herb_count[i] != 0) {

				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					(char*)p_datseg + GATHER_HERBS_STR_FOUND, /* "%d %s" */
					herb_count[i],
					Real2Host(GUI_names_grammar((herb_count[i] > 1 ? 4 : 0) + 0x4002, ds_readb(GATHER_HERBS_TABLE + 4 * i), 0)));

				strcat((char*)Real2Host(ds_readd(DTP2)), (char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

				if (--herbs > 1) {
					/* add a comma ", " */
					strcat((char*)Real2Host(ds_readd(DTP2)), (char*)p_datseg + GATHER_HERBS_STR_COMMA);
				} else if (herbs == 1) {
					/* add an and " UND " */
					strcat((char*)Real2Host(ds_readd(DTP2)), (char*)p_datseg + GATHER_HERBS_STR_AND);
				}
			}
		}

		/* add a dot "." */
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)p_datseg + GATHER_HERBS_STR_DOT);

	} else {

		/* no herbs found */

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(342),
			(char*)hero + HERO_NAME2);
	}

	GUI_output(Real2Host(ds_readd(DTP2)));

	return 0;
}

/**
 * \brief   replenish the stocks (water and food)
 *
 * \param   mod         modificator for the skill test
 * \param   tries       how often was tried to replenish stocks
 * \return              0 if replenish was not possible or 1 if replenish was possible
 */
signed short replenish_stocks(signed short mod, signed short tries)
{
	signed short hero_pos;
	signed short l_di;
	signed short retval;
	signed short j;
	RealPt hero;
	Bit8u *hero2;

	retval = 0;
	mod += 5;

	ds_writews(SKILLED_HERO_POS, get_skilled_hero_pos(TA_WILDNISLEBEN));
	hero_pos = select_hero_ok(get_ttx(322));

	if (hero_pos != -1 && hero_brewing(get_hero(hero_pos))) {
		GUI_output(get_ttx(730));
		hero_pos = -1;
	}

	if (hero_pos != -1) {

		if (ds_readb(WILDCAMP_REPLSTATUS + hero_pos) != 0) {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(802),
				(char*)get_hero(hero_pos) + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

		} else {

			if (ds_readb(WILDCAMP_HERBSTATUS + hero_pos) != 0 ||
				ds_readb(WILDCAMP_MAGICSTATUS + hero_pos) != 0 ||
				ds_readb(WILDCAMP_GUARDSTATUS + hero_pos) != 0)
			{
				GUI_output(get_ttx(331));

			} else {

				if (tries < 2) {

					timewarp(HOURS(1));
					ds_writed(MAIN_ACTING_HERO, (Bit32u)(hero = (RealPt)ds_readd(HEROES) + SIZEOF_HERO * hero_pos));
					ds_writeb(WILDCAMP_REPLSTATUS + hero_pos, 1);
					retval = 1;

					/* search for water */
					if (test_skill(Real2Host(hero), TA_WILDNISLEBEN, (signed char)mod) > 0 || ds_readd(INGAME_TIMERS + 4)) {

						/* found water */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(324),
							(char*)Real2Host(hero) + HERO_NAME2);

						/* fill up all waterskins and remove thirst of all living heroes in the current group */
						hero2 = get_hero(0);
						for (l_di = 0; l_di <= 6; l_di++, hero2 += SIZEOF_HERO) {
							if (host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE &&
								host_readbs(hero2 + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_dead(hero2))
							{
								host_writebs(hero2 + HERO_THIRST, 0);

								for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {
									if (host_readws(hero2 + SIZEOF_INVENTORY * j + HERO_INVENTORY + INVENTORY_ITEM_ID) == ITEM_WATERSKIN) {
										and_ptr_bs(hero2 + SIZEOF_INVENTORY * j + HERO_INVENTORY + INVENTORY_FLAGS, 0xfb); /* unset 'empty' flag */
										and_ptr_bs(hero2 + SIZEOF_INVENTORY * j + HERO_INVENTORY + INVENTORY_FLAGS, 0xfd); /* unset 'half_empty' flag */
									}
								}
							}
						}
					} else {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(340),
							(char*)Real2Host(hero) + HERO_NAME2);
					}

					GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
					delay_or_keypress(200);

					/* search for food */
					if (test_skill(Real2Host(hero), TA_FAEHRTENSUCHEN, (signed char)mod) > 0 || ds_readd(INGAME_TIMERS + 0xc)) {

						/* remove hunger of all living heroes in the current group */
						hero2 = get_hero(0);
						for (l_di = 0; l_di <= 6; l_di++, hero2 += SIZEOF_HERO) {
							if (host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE &&
								host_readbs(hero2 + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_dead(hero2))
							{
								host_writebs(hero2 + HERO_HUNGER, 0);
							}
						}

						/* the group may get three food packages */
						if (!get_item(ITEM_FOOD_PACKAGE, 1, 3)) {
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(306));
							ds_writew(REQUEST_REFRESH, 1);
						} else {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(325),
								(char*)Real2Host(hero) + HERO_NAME2);
						}

					} else {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(341),
							(char*)Real2Host(hero) + HERO_NAME2);
					}

					GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
					delay_or_keypress(200);

				} else {
					GUI_output(get_ttx(323));
				}
			}
		}
	}

	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
