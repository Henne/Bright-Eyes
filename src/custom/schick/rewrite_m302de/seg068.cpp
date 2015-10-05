/*
 *	Rewrite of DSA1 v3.02_de functions of seg068 (Thorwal)
 *	Special City: Thorwal
 *	Functions rewritten: 10/13
 *
*/

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg055.h"
#include "seg058.h"
#include "seg075.h"
#include "seg097.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void THO_eisenhof(void)
{
	signed short answer;
	Bit32s money;

	do {
		answer = GUI_radio(get_city(0xbc), 3,
					get_city(0xc0),
					get_city(0xc4),
					get_city(0xc8));
	} while (answer == -1);

	if (answer == 1) {

		ds_writews(TYPEINDEX, 41);
		do_smith();

	} else if (answer == 2) {

		GUI_input(get_city(0xcc), 0);

		money = get_party_money();
		money -= 10;
		set_party_money(money);

		/* test for CH+0 */
		if (test_attrib(Real2Host(get_first_hero_available_in_group()), 2, 0) > 0) {

			GUI_input(get_city(0xd0), 0);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(4 * (random_schick(26) + 55)));
			GUI_input(Real2Host(ds_readd(DTP2)), 0);
		} else {
			GUI_input(get_city(0xd4), 0);
		}
	}
}

/* Borlandified and identical */
void THO_imman(void)
{

	unsigned short tmp;

	tmp = get_current_season();

	if ((tmp == 1 || tmp == 3) && (ds_readb(DAY_OF_WEEK) == 5)) {
		/* ask to visit the game */
		if (GUI_bool(get_city(0xdc)) != 0) {

		tmp = random_schick(4) + 0x38;
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0xe0),
			/* winner */
			(char*)get_city(tmp * 4),
			/* looser */
			(char*)get_city((random_schick(7) + 0x3c) * 4),
			/* winner */
			(char*)get_city(tmp * 4),
			/* winners points */
			random_interval(15, 30),
			/* loosers points */
			random_schick(14));

		GUI_input(Real2Host(ds_readd(DTP2)), 0);
		}
	} else {
		/* no imman game at the moment */
		GUI_input(get_city(0xd8), 0);
	}

}

/* Borlandified and identical */
void THO_botschaft(void)
{
	int closed = 0;

	/* At the 6th month in year 17 Hal another message is shown */
	if (ds_readbs(YEAR) > 17 ||
		(ds_readbs(YEAR) == 17 && ds_readbs(MONTH) > 5)) {

		closed = 1;
	}

	GUI_input( (!closed) ? get_city(0x110): get_city(0x114), 0);
}

/* Borlandified and identical */
void THO_bank(void)
{
	signed short answer;
	signed short done;
	signed short l3;
	Bit32s p_money;

	done = 0;

	if (ds_readws(BANK_DEPOSIT) <= -1000) {

		GUI_input(get_city(0x134), 0);

		if (ds_readws(DEBT_DAYS) == 0) {
			ds_writews(DEBT_DAYS, 7);
		}
	}

	load_in_head(16);

	do {

		sprintf((char*)Real2Host(ds_readd(0xd2eb)),
			(char*)get_city(0x120),
			ds_readws(BANK_DEPOSIT));

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x144),
						Real2Host(ds_readd(0xd2eb)), 3,
						get_city(0x124), get_city(0x128), get_city(0x140));
		} while (answer == -1);


		if (answer == 2) {

			answer = GUI_input(get_city(0x12c), 3);

			if (answer <= 0) {
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x144),
						get_city(0x13c), 0);
			} else {

				if (ds_readws(DAYS_TO_CENS) != 0 ||
					(ds_readws(BANK_DEPOSIT) > 0 && ds_readws(BANK_DEPOSIT) + 200 < answer) ||
					(ds_readws(BANK_DEPOSIT) <= 0 && answer > 200))
				{
					GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x144),
							get_city(0x130), 0);
				} else {

					if (ds_readws(BANK_DEPOSIT) < answer) {

						if (ds_readws(BANK_DEPOSIT) > 0) {
							answer -= ds_readws(BANK_DEPOSIT);
							p_money = get_party_money();
							p_money += 10 * ds_readws(BANK_DEPOSIT);
							set_party_money(p_money);
							ds_writews(BANK_DEPOSIT, 0);
						}

						add_ds_ws(MONTHLY_CREDIT, answer);

						if (ds_readws(MONTHLY_CREDIT) > 200) {

							GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x144),
									get_city(0x130), 0);

							l3 = ds_readws(MONTHLY_CREDIT) - 200;
							answer -= l3;
							ds_writews(MONTHLY_CREDIT, 200);

							if (answer < 0) {
								answer = 0;
							}
						}

						if (ds_readws(MONTHLY_CREDIT) >= 200) {
							ds_writews(DAYS_TO_CENS, 30);
						}
					}

					sub_ds_ws(BANK_DEPOSIT, answer);
					p_money = get_party_money();
					p_money += 10 * answer;
					set_party_money(p_money);
				}
			}

		} else if (answer == 1) {

			if (ds_readws(BANK_DEPOSIT) >= 30000) {

				/* prevent overflow  at 32767 */
				GUI_output(p_datseg + 0x7c44);

			} else {

				answer = GUI_input(get_city(0x12c), 3);
				p_money = get_party_money();

				if (answer * 10 > p_money) {
					GUI_output(get_ltx(0x644));
				} else {

					if (answer <= 0) {

						GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x144),
								get_city(0x13c), 0);

					} else {

						p_money -= 10 * answer;
						set_party_money(p_money);
						ds_writews(DAYS_TO_CENS, 0);
						add_ds_ws(BANK_DEPOSIT, answer);

						if (ds_readws(MONTHLY_CREDIT) != 0) {

							sub_ds_ws(MONTHLY_CREDIT, answer);

							if (ds_readws(MONTHLY_CREDIT) < 0) {
								ds_writews(MONTHLY_CREDIT, 0);
							}
						}
						if (ds_readws(BANK_DEPOSIT) > -1000) {
							ds_writews(DEBT_DAYS, 0);
						}
					}
				}
			}
		} else {
			done = 1;
		}

	} while (!done);
}

/* Borlandified and identical */
void THO_arsenal(void)
{
	/* ARSENAL / ZEUGHAUS */
	signed short answer;
	signed short options;
	signed short tw_bak;
	Bit32s p_money;

	if (ds_readds(DAY_TIMER) < HOURS(8) || ds_readds(DAY_TIMER) > HOURS(19)) {

		GUI_output(get_ltx(0x788));

	} else if (ds_readb(0x34d6 + ds_readws(TYPEINDEX)) != 0) {

			talk_merchant();

	} else if (ds_readws(ARSENAL_MONEY) != 0) {

		load_in_head(13);

		/* only show two options when the group has "LETTER FROM JADRA" or "LETTER OF INTRODUCTION" */
		options = get_first_hero_with_item(187) != -1 || get_first_hero_with_item(235) != -1 ? 2 : 1;

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x000), options,
					get_city(0x008), get_city(0x004));

		} while (answer == -1);

		if (answer == 2) {

			if (ds_readws(ARSENAL_MONEY) == -1) {

				/* calculate the maximal shopping price [10-60] D */
				if (ds_writews(ARSENAL_MONEY, 15 * ds_readws(SUBVENTION)) < 10) {
					/* at least 10D */
					ds_writew(ARSENAL_MONEY, 10);
				}
			}

			sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
				(char*)get_city(0x00c),
				ds_readws(ARSENAL_MONEY));

			mul_ds_ws(ARSENAL_MONEY, 100);
			GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);
			p_money = get_party_money();
			set_party_money(ds_readws(ARSENAL_MONEY));

			ds_writew(TYPEINDEX, 92);
			tw_bak = ds_readws(TEXTBOX_WIDTH);
			ds_writew(TEXTBOX_WIDTH, 3);

			do_merchant();

			ds_writews(ARSENAL_MONEY, get_party_money());
			div_ds_ws(ARSENAL_MONEY, 100);
			ds_writew(TEXTBOX_WIDTH, tw_bak);
			set_party_money(p_money);


		} else {
			GUI_dialog_na(0, get_city(0x010));
			ds_writeb(NEED_LETTER, 1);
		}

	} else {

		GUI_output(p_datseg + 0x7c7d);
	}
}

/* Borlandified and identical */
void THO_magistracy(void)
{
	signed short answer;

	if (ds_readw(GOT_MAIN_QUEST) == 0) {

		GUI_output(get_city(0x014));
		GUI_output(get_city(0x018));
		GUI_output(get_city(0x01c));

	} else {

		do {
			answer = GUI_radio(get_city(0x014), 3,
						get_city(0x020),
						get_city(0x024),
						get_city(0x028));
		} while (answer == -1);

		if (answer == 1) {

			if (ds_readws(GOT_LETTER_JAD) == 0) {

				ds_writews(GOT_LETTER_JAD, 1);
				GUI_output(get_city(0x038));

				/* get "LETTER FROM JADRA" */
				get_item(187, 1, 1);

			} else {
				GUI_output(get_city(0x03c));
			}
		} else if (answer == 2) {

			GUI_output(get_city(0x030));
		} else {
			GUI_output(get_city(0x034));
		}
	}
}

/* Borlandified and identical */
void THO_mueller(void)
{
	if (GUI_bool(get_city(0x40))) {

		GUI_output((ds_readw(VISITED_MILLER) == 0) ? /* first visit ? */
			get_city(0x44) : get_city(0x48));

		/* mark the miller as visited */
		ds_writew(VISITED_MILLER, 1);
	}
}

/* Borlandified and identical */
void THO_black_finger(void)
{
	if (GUI_radio(get_city(0x4c), 2, get_city(0x50), get_city(0x54)) == 1) {

		GUI_output(get_city(0x58));
	}
}

/* Borlandified and identical */
/* static */
void dramosch_says(Bit8u *msg)
{
	GUI_dialogbox((RealPt)ds_readd(DTP2),
			Real2Host(host_readd(Real2Host(ds_readd(CITY_LTX)) + 0xc0)), msg, 0);
}

/* Borlandified and identical */
void THO_ugdalf(void)
{
	signed short answer;
	signed short randval;

	load_in_head(ds_readw(QUEST_UGDALF) == 0 ? 0 : 14);

	if (ds_readw(QUEST_UGDALF) == 0) {

		/* talk to the guards */
		randval = random_schick(10) - 1;

		answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x05c), 3,
					get_city(4 * (randval + 38)),
					get_city(0x060),
					get_city(0x064));

		if (answer == 1) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x6c), 0);

		} else if (answer == 2) {

			/* talk to DRAMOSCH */
			GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x70), 0);

			load_in_head(14);

			dramosch_says(get_city(0x074));

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							Real2Host(host_readd(Real2Host(ds_readd(CITY_LTX)) + 0xc0)),

							get_city(0x078), 2,
							get_city(0x07c),
							get_city(0x080));
			} while (answer == -1);

			if (answer == 1) {
				/* take the quest */
				dramosch_says(get_city(0x084));
				ds_writew(QUEST_UGDALF, 1);

			} else {

				dramosch_says(get_city(0x088));
			}
		}
	} else if (ds_readw(QUEST_UGDALF) == 1 || !ds_readb(0x35ff)) {

		dramosch_says(get_city(0x8c));

		/* enter the dungeon */
		DNG_enter_dungeon(14);

		ds_writews(0x2d83, ds_readw(X_TARGET));
		ds_writews(0x2d85, ds_readw(Y_TARGET));

		if (ds_readw(QUEST_UGDALF) == 1) {
			add_party_money(2000L);

		/* Original-Bug:	Everytime the heros enter the dungeon the get 20D.
					Why this fix works is not seen that easy.
					As long as ds_readb(0x35ff) is 0 this block is executed.
		 */
#ifdef M302de_ORIGINAL_BUGFIX
			ds_writew(QUEST_UGDALF, 2);
#endif
		}

	} else if (ds_readw(QUEST_UGDALF) == 3) {

		/* talk with DRAMOSCH for 8 h */
		dramosch_says(get_city(0x90));
		timewarp(HOURS(8));

		/* mark this quest as done */
		ds_writew(QUEST_UGDALF, 4);

		/* get the reward */
		add_hero_ap_all(25);
		add_party_money(5000);
	} else {

		dramosch_says(get_city(0x94));

		sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
			(char*)get_dtp(4 * (random_schick(26) + 55)));

		dramosch_says(Real2Host(ds_readd(DTP2)) + 0x400);

		/* enter the dungeon */
		DNG_enter_dungeon(14);

		ds_writews(0x2d83, ds_readw(X_TARGET));
		ds_writews(0x2d85, ds_readw(Y_TARGET));
	}
}

#if !defined(__BORLANDC__)
}
#endif
