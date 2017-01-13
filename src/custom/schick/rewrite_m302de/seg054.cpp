/**
 *	Rewrite of DSA1 v3.02_de functions of seg054 (inn)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg054.cpp
 */

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg025.h"
#include "seg027.h"
#include "seg029.h"
#include "seg031.h"
#include "seg047.h"
#include "seg049.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg104.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void talk_inn(void)
{
	do_random_talk(1, 0);
}

/**
 * \brief   returns a pointer to the first busy hero, who is not in this group, but in this location
 */
RealPt get_first_busy_hero(void)
{
	RealPt hero;
	signed short i;

	hero = (RealPt)ds_readd(HEROS);
	for (i = 0; i < 6; i++, hero += SIZEOF_HERO) {
		if (host_readbs(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(Real2Host(hero) + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP) &&
			hero_busy(Real2Host(hero)) &&
			host_readbs(Real2Host(hero) + HERO_HOSTEL_ID) == ds_readws(TYPEINDEX))
		{
			return hero;
		}
	}

	return (RealPt)NULL;
}

/**
 * \brief   location handler for inns
 */
void do_inn(void)
{
	signed short i;
	signed short answer;
	signed short done;
	signed short refresh;
	signed short portion_size;
	signed char magic_act[6];
	RealPt hero;
	Bit8u *hero2;
	Bit8u *inn_ptr;
	Bit32s party_money;
	Bit32s price;
	signed char stay;
	signed short tw_bak;
	signed short nr_heros;
	Bit32s l8;
	Bit32s l9;
	Bit32s l10;
	signed short booked_days;
	signed short rested_days;
	Bit8u *tavern_ptr;

	done = 0;
	stay = 0;
	ds_writebs(SLEEP_QUALITY, -1);

	if (ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) == 1) {

		hero = get_first_hero_available_in_group();

		if (hero_busy(Real2Host(hero))) {

			draw_status_line();

			if (host_readbs(Real2Host(hero) + HERO_RECIPE_TIMER) != 0) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(0xb74),
					(char*)Real2Host(hero) + HERO_NAME2,
					host_readbs(Real2Host(hero) + HERO_RECIPE_TIMER),
					(char*)(host_readbs(Real2Host(hero) + HERO_RECIPE_TIMER) < 2 ? get_ttx(0xb7c) : get_ttx(0xb80)));

				answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2, get_ttx(0xb78), get_ttx(0x864));

				if (answer == 1) {
					do_alchemy(Real2Host(hero), host_readbs(Real2Host(hero) + HERO_RECIPE_ID), 1);
				} else {
					done = 1;
					ds_writew(COMBO_MODE, 0);
					stay = 1;
				}
			} else {
				do_alchemy(Real2Host(hero), host_readbs(Real2Host(hero) + HERO_RECIPE_ID), 0);
			}
		}

	} else if ((hero = get_first_busy_hero())) {

		draw_status_line();

		if (host_readbs(Real2Host(hero) + HERO_RECIPE_TIMER) != 0) {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(0xb74),
				(char*)Real2Host(hero) + HERO_NAME2,
				host_readbs(Real2Host(hero) + HERO_RECIPE_TIMER),
				(char*)(host_readbs(Real2Host(hero) + HERO_RECIPE_TIMER) < 2 ? get_ttx(0xb7c) : get_ttx(0xb80)));

			tw_bak = ds_readws(TEXTBOX_WIDTH);
			ds_writews(TEXTBOX_WIDTH, 4);

			answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2, get_ttx(0xb78), get_ttx(0x8c8));

			ds_writews(TEXTBOX_WIDTH, tw_bak);

			if (answer == 1) {
				do_alchemy(Real2Host(hero), host_readbs(Real2Host(hero) + HERO_RECIPE_ID), 1);
				GRP_merge();
			}
		} else {
			do_alchemy(Real2Host(hero), host_readbs(Real2Host(hero) + HERO_RECIPE_ID), 0);
			GRP_merge();
		}
	}

	if (done == 0) {

		refresh = ds_writews(REQUEST_REFRESH, 1);

		draw_loc_icons(ds_readws(COMBO_MODE) == 0 ? 7 : 8, 21, 13, 19, 25, 11, 17, 8, 50);

		for (i = 0; i < 6; i++) {
			magic_act[i] = 0;
		}
	}

	while (done == 0) {

		if (ds_readws(REQUEST_REFRESH) != 0) {

			draw_main_screen();

			set_var_to_zero();

			load_ani(4);

			init_ani(0);

			GUI_print_loc_line(get_tx(4 * ds_readws(CITYINDEX)));

			ds_writews(REQUEST_REFRESH, refresh = 0);
		}

		if (refresh != 0) {

			GUI_print_loc_line(get_tx(4 * ds_readws(CITYINDEX)));

			refresh = 0;
		}

		inn_ptr = p_datseg + INN_DESCR_TABLE + 4 * ds_readws(TYPEINDEX);

		handle_gui_input();

		if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == 73) {

			answer = GUI_radio(get_ttx(0x564), ds_readws(COMBO_MODE) == 0 ? 7 : 8,
						get_ttx(0x55c), get_ttx(0x758),
						get_ttx(0x560), get_ttx(0x350),
						get_ttx(0x4d8), get_ttx(0x4f0),
						get_ttx(0x56c), get_ttx(0xcdc)) - 1;

			if (answer != -2) {
				ds_writews(ACTION, answer + 129);
			}
		}

		if (ds_readws(ACTION) == 129) {
			talk_inn();
			ds_writews(REQUEST_REFRESH, 1);
		} else if (ds_readws(ACTION) == 130) {

			price = count_heroes_in_group() * (6L - host_readws(inn_ptr) / 4L);
			price += (price * host_readws(inn_ptr + 2)) / 100L;

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(0x764),
				(signed short)price);

			if (GUI_bool(Real2Host(ds_readd(DTP2)))) {

				party_money = get_party_money();
				price *= 10;

				if (price < 0) {
					price = 1;
				}

				if (party_money < price) {
					GUI_output(get_ttx(0x644));
				} else {

					GUI_output(host_readws(inn_ptr) < 5 ? get_ttx(0x76c) :
							(host_readws(inn_ptr) < 15 ? get_ttx(0x770) : get_ttx(0x774)));

					for (i = 0, hero2 = get_hero(0); i <= 6; i++, hero2 += SIZEOF_HERO) {

						if (host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero2 + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
							!hero_dead(hero2))
						{
							portion_size = (21 - host_readws(inn_ptr)) * 20;
							if (portion_size > 100) {
								portion_size = 100;
							}

							sub_ptr_bs(hero2 + HERO_HUNGER, (unsigned char)portion_size);

							if (host_readbs(hero2 + HERO_HUNGER) < 0) {
								host_writebs(hero2 + HERO_HUNGER, 0);
							}

							portion_size = (21 - host_readws(inn_ptr)) * 30;
							if (portion_size > 100) {
								portion_size = 100;
							}

							sub_ptr_bs(hero2 + HERO_THIRST, (unsigned char)portion_size);

							if (host_readbs(hero2 + HERO_THIRST) < 0) {
								host_writebs(hero2 + HERO_THIRST, 0);
							}
						}
					}

					party_money -= price;
					set_party_money(party_money);
				}
			}

		} else if (ds_readws(ACTION) == 131 && ds_readbs(SLEEP_QUALITY) == -1) {

			l8 = 5;
			l9 = 30;
			l10 = 100;
			l8 += l8 * host_readws(inn_ptr + 2) / 100;
			l9 += l8 * host_readws(inn_ptr + 2) / 100;
			l10 += l8 * host_readws(inn_ptr + 2) / 100;

			tw_bak = ds_readws(TEXTBOX_WIDTH);
			ds_writews(TEXTBOX_WIDTH, 5);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(0x634),
				(signed short)l8);

			sprintf((char*)Real2Host(ds_readd(DTP2)) + 50,
				(char*)get_ttx(0x638),
				(signed short)l9);

			sprintf((char*)Real2Host(ds_readd(DTP2)) + 100,
				(char*)get_ttx(0x63c),
				(signed short)l10);

			ds_writebs(SLEEP_QUALITY, (signed char)GUI_radio(get_ttx(0x630), 3,
							Real2Host(ds_readd(DTP2)),
							Real2Host(ds_readd(DTP2)) + 50,
							Real2Host(ds_readd(DTP2)) + 100));

			if (ds_readbs(SLEEP_QUALITY) != -1) {
				ds_writebs(BOOKED_INN_DAYS, (signed char)GUI_input(get_ttx(0xce8), 2));
			}

			if (ds_readbs(BOOKED_INN_DAYS) <= 0) {
				ds_writebs(SLEEP_QUALITY, -1);
			}

			ds_writews(TEXTBOX_WIDTH, tw_bak);

			if (ds_readbs(SLEEP_QUALITY) != -1) {

				nr_heros = count_heroes_in_group();

				l8 *= nr_heros;
				l9 *= nr_heros;
				l10 *= nr_heros;
				party_money = get_party_money();

				price = ds_readbs(SLEEP_QUALITY) == 1 ? l8 : (ds_readbs(SLEEP_QUALITY) == 2 ? l9 : l10);

				price *= ds_readbs(BOOKED_INN_DAYS);

				if (price < 0) {
					price = 1;
				}

				if (party_money < price) {
					GUI_output(get_ttx(0x644));
					ds_writebs(SLEEP_QUALITY, -1);
				} else {
					party_money -= price;
					set_party_money(party_money);
					GUI_output(get_ttx(0x648));
				}
			}

		} else if (ds_readws(ACTION) == 132) {

			if (ds_readbs(SLEEP_QUALITY) != -1) {
				GUI_use_skill2(0, get_ttx(0x62c));
				refresh = 1;
			} else {
				GUI_output(get_ttx(0x568));
			}

		} else if (ds_readws(ACTION) == 133) {

			if (ds_readbs(SLEEP_QUALITY) != -1) {

				answer = select_hero_ok(get_ttx(0x4f4));

				if (answer != -1) {

					hero = (RealPt)ds_readd(HEROS) + SIZEOF_HERO * answer;

					if (host_readbs(Real2Host(hero) + HERO_TYPE) >= 7) {

						if (magic_act[answer] != 0) {
							GUI_output(get_ttx(0x538));
						} else {
							magic_act[answer] = (signed char)use_magic(hero);
						}
					} else {
						GUI_output(get_ttx(0x528));
					}
				}
			} else {
				GUI_output(get_ttx(0x568));
			}

		} else if (ds_readws(ACTION) == 134) {

			if (ds_readbs(SLEEP_QUALITY) != -1 && ds_readbs(BOOKED_INN_DAYS) > 0) {

				if (GUI_bool(get_ttx(0x4f8))) {
					booked_days = ds_readbs(BOOKED_INN_DAYS);

					if (host_readws(inn_ptr) < 8) {
						inc_ds_bs_post(SLEEP_QUALITY);
					} else if (host_readws(inn_ptr) > 15) {
						dec_ds_bs_post(SLEEP_QUALITY);
					}

					done = 1;
					ds_writews(COMBO_MODE, 0);
					ds_writebs(FOOD_MOD, 1);

					do {
						timewarp_until(HOURS(8));
					} while (dec_ds_bs(BOOKED_INN_DAYS));

					ds_writeb(FOOD_MOD, 0);

					hero = (RealPt)ds_readd(HEROS);
					for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

						if (host_readbs(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(Real2Host(hero) + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
						{
							if (booked_days > 1) {
								magic_act[i] = 0;
							}

							if (magic_act[i] != 1) {

								for (rested_days = 0; rested_days < booked_days; rested_days++)
								{
									GRP_hero_sleep(Real2Host(hero), ds_readbs(SLEEP_QUALITY));
								}
							}
						}
					}
				}
			} else {
				GUI_output(get_ttx(0x568));
			}
		} else if (ds_readws(ACTION) == 135) {

			if (ds_readbs(SLEEP_QUALITY) != -1) {

				if (GUI_bool(get_ttx(0x640))) {
					done = 1;
					ds_writews(COMBO_MODE, 0);
				}
			} else {
				done = 1;
				ds_writews(COMBO_MODE, 0);
			}

		} else if (ds_readws(ACTION) == 136 && ds_readws(COMBO_MODE) != 0) {

			tavern_ptr = p_datseg + TAVERN_DESCR_TABLE + 4 * ds_readws(TYPEINDEX);

			if (host_readws(tavern_ptr) >= 6 && host_readws(tavern_ptr) <= 13 &&
				ds_readds(DAY_TIMER) < HOURS(11) && ds_readds(DAY_TIMER) > HOURS(3)) {
				GUI_output(get_ttx(0xc84));
			} else if ((host_readws(tavern_ptr) < 6 || host_readws(tavern_ptr) > 13) &&
				ds_readds(DAY_TIMER) < HOURS(16) && ds_readds(DAY_TIMER) > HOURS(3)) {
				GUI_output(get_ttx(0x784));
			} else if (ds_readbs(SLEEP_QUALITY) != -1) {

				if (GUI_bool(get_ttx(0x640))) {
					done = 1;
					ds_writews(COMBO_MODE, 2);
				}
			} else {
				done = 1;
				ds_writews(COMBO_MODE, 2);
			}
		}
	}

	if (!stay) {
		copy_palette();
		turnaround();
	} else {
		GRP_switch_to_next(1);
		draw_status_line();
	}
}

void TLK_herberg(signed short state)
{
	Bit8u *hero = Real2Host(get_first_hero_available_in_group());

	if (!state) {
		ds_writews(DIALOG_NEXT_STATE, ds_readb(HERBERG_KICKED_FLAGS + ds_readws(TYPEINDEX)) != 0 ? 1 : 2);
	} else if (state == 1 || state == 14) {
		ds_writeb(HERBERG_KICKED_FLAGS + ds_readws(TYPEINDEX), 1);
	} else if (state == 11) {
		tumult();
		ds_writeb(TOWN_OUTLAWED_FLAGS + ds_readbs(CURRENT_TOWN), 1);
		ds_writeb(HERBERG_KICKED_FLAGS + ds_readws(TYPEINDEX), 1);
	} else if (state == 12) {
		/* CH + 5 */
		ds_writews(DIALOG_NEXT_STATE, test_attrib(hero, ATTRIB_CH, 5) > 0 ? 14 : 11);
	} else if (state == 13) {
		/* CH + 0 */
		ds_writews(DIALOG_NEXT_STATE, test_attrib(hero, ATTRIB_CH, 0) > 0 ? 14 : 7);
	} else if (state == 15) {
		/* CH - 3 */
		ds_writews(DIALOG_NEXT_STATE, test_attrib(hero, ATTRIB_CH, -3) > 0 ? 16 : 17);
	} else if (state == 17) {
		ds_writew(ACTION, 130);
	}
}

#if !defined(__BORLANDC__)
}
#endif
