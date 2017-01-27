/**
 *	Rewrite of DSA1 v3.02_de functions of seg112 (travel events 4 / 10)
 *	Functions rewritten: 13/13 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg112.cpp
 */
#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg047.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#include "seg112.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void tevent_067(void)
{
	signed short i;
	signed short answer;
	signed short count;
	Bit8u *hero;

	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 6) > 0 && !ds_readb(TEVENT067_FLAG)) ||
		ds_readb(TEVENT067_FLAG) != 0)
	{
		GUI_output(get_tx2(90));

		do {
			answer = GUI_radio(get_tx2(91), 3,
						get_tx2(92),
						get_tx2(93),
						get_tx2(94));
		} while (answer == -1);

		if (answer == 2) {

			ds_writeb(LOCATION, LOCATION_WILDCAMP);
			do_location();
			ds_writeb(LOCATION, 0);

		} else if (answer == 3) {

			hero = get_hero(0);
			for (i = count = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero) &&
					test_attrib(hero, ATTRIB_GE, 0) > 0)
				{
					count++;
				}
			}

			if (!ds_readb(TEVENT067_FLAG) && count >= 3) {

				loot_multi_chest(p_datseg + TEVENT067_CHEST, get_tx2(96));

				GUI_output(get_tx2(97));

				hero = Real2Host(get_first_hero_available_in_group());

				add_party_money(7L);

				ds_writeb(LOCATION, LOCATION_WILDCAMP);
				do_location();
				ds_writeb(LOCATION, 0);
			} else {

				GUI_output(get_tx2(95));

				hero = get_hero(0);
				for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
					{
						sub_hero_le(hero, random_schick(8));
					}
				}

				ds_writeb(LOCATION, LOCATION_WILDCAMP);
				do_location();
				ds_writeb(LOCATION, 0);

				TRV_load_textfile(-1);
			}

			ds_writeb(TEVENT067_FLAG, 1);
		}
	} else {

		GUI_output(get_tx2(90));

		do {
			answer = GUI_radio(get_tx2(82), 2,
						get_tx2(92),
						get_tx2(93));
		} while (answer == -1);

		if (answer == 2) {
			ds_writeb(LOCATION, LOCATION_WILDCAMP);
			do_location();
			ds_writeb(LOCATION, 0);
		}
	}
}

void tevent_068(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Reiseevent %d ist nicht implementiert\n", 68);
#endif
}

void tevent_069(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 5) > 0 && !ds_readb(TEVENT069_FLAG)) ||
		ds_readb(TEVENT069_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT069_FLAG, 1);
	}
}

void tevent_070(void)
{
	signed short l_si;

	if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 3) > 0 ||
		ds_readb(TEVENT070_FLAG) != 0)
	{
		ds_writeb(TEVENT070_FLAG, 1);

		if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 4) > 0 ||
			ds_readb(TEVENT070_HERB_FLAG) != 0)
		{
			ds_writeb(TEVENT070_HERB_FLAG, 1);
			ds_writeb(GATHER_HERBS_SPECIAL, 130);
			l_si = TRV_found_camp_place(2);
			ds_writeb(GATHER_HERBS_SPECIAL, -1);
		} else {
			l_si = TRV_found_camp_place(0);
		}

		if ((l_si && !ds_readb(TEVENT070_TRAIL_FLAG) && test_skill(Real2Host(get_first_hero_available_in_group()), TA_FAEHRTENSUCHEN, 0) > 0) ||
			ds_readb(TEVENT070_TRAIL_FLAG) != 0) {

			ds_writeb(TEVENT070_TRAIL_FLAG, 1);

			if (!TRV_follow_trail_question()) {

				do {
					l_si = GUI_radio(get_tx2(0), 2, get_tx2(1), get_tx2(2));

				} while (l_si == -1);

				if (l_si == 1) {
					TRV_hunt_generic(25, 3, -1, 3, 8, 3, 7, 2, 4, 35, 0);
				}
			}
		}
	}
}

/* Orc-Monolith */
void tevent_071(void)
{
	signed short l_si;
	signed short have_raft;
	signed short i;
	Bit8u *hero;

	have_raft = 0;

	/* Perception + 8, Sinnesschaerfe + 8 */
	if (test_skill(hero = Real2Host(get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 8) > 0 &&
		!ds_readb(TEVENT071_FLAG))
	{
		ds_writeb(TEVENT071_FLAG, 1);

		/* Track + 4, Faehrtensuche + 4 */
		if (test_skill(hero, TA_FAEHRTENSUCHEN, 4) > 0) {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx2(13),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));


			do {
				l_si = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
						get_tx2(14),
						get_tx2(15));
			} while (l_si == -1);

			if (l_si == 2) {
				/* investigate */

				do {
					l_si = GUI_radio(get_tx2(16), 3,
							get_tx2(17),
							get_tx2(18),
							get_tx2(19));
				} while (l_si == -1);

				if (l_si == 1) {

					/* just swim */
					TRV_swimm(2, 5);

				} else if (l_si == 2) {

					/* build a raft before swimming */
					have_raft = 1;
					timewarp(HOURS(1));
					TRV_swimm(-1, 0);
				}

				if (l_si == 1 || l_si == 2) {

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(22),
						(char*)hero + HERO_NAME2);
					do {
						l_si = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
								get_tx2(23),
								get_tx2(24));
					} while (l_si == -1);

					if (l_si == 1) {
						/* destroy monolith */

						GUI_output(get_tx2(25));

						/* REWARD: get gods estimation + 500 for each god */
						for (i = 1; i <= 14; i++) {
							add_ds_ds(GODS_ESTIMATION + 4 * i, 500L);
						}

						/* mark the statuette as destroyed => has effects in fights */
						ds_writeb(TEVENT071_ORCSTATUE, 1);

						add_hero_ap_all(10);

					} else {
						/* leave monolith */

						/* PUNISHMENT: set gods estimation to 0 for each god */
						for (i = 1; i <= 14; i++) {
							ds_writed(GODS_ESTIMATION + 4 * i, 0L);
						}
					}

					i = (!have_raft ? 1 : 2);

					do {
						l_si = GUI_radio(get_tx2(26), (signed char)i,
								get_tx2(27),
								get_tx2(28));
					} while (l_si == -1);

					if (l_si == 1) {
						TRV_swimm(2, 5);
					} else {
						timewarp(HOURS(1));
						TRV_swimm(-1, 0);
					}
				}
			}
		}
	}
}

/**
 * \brief   all members of the current group try to swim
 *
 * \param   mod         modificator for the swim test
 * \param   percent     probability to loose an item
 */
void TRV_swimm(signed short mod, signed short percent)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero))
		{

			if (test_skill(hero, TA_SCHWIMMEN, (signed char)mod) <= 0) {
				/* test failed */

				hero_disease_test(hero, 2, 20 - (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK))));

				loose_random_item(hero, percent, get_ttx(506));

				sub_hero_le(hero, random_schick(5));


				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx2(21),
					(char*)hero + HERO_NAME2);
			} else {
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx2(20),
					(char*)hero + HERO_NAME2);
			}

			GUI_output(Real2Host(ds_readd(DTP2)));
		}
	}
}

/* a herb place you need to swim to */
void tevent_unused01(void)
{
	signed short answer;
	signed short has_raft;
	signed short options;
	Bit8u *hero;

	has_raft = 0;

	hero = Real2Host(get_first_hero_available_in_group());

	if ((test_skill(hero, TA_SINNESSCHAERFE, 8) > 0 && !ds_readb(TEVENTU01_FLAG)) ||
		ds_readb(TEVENTU01_FLAG) != 0)
	{

		ds_writeb(TEVENTU01_FLAG, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx2(29),
			(char*)hero + HERO_NAME2,
			(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
			(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));


		do {
			answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
					get_tx2(30),
					get_tx2(31));
		} while (answer == -1);

		if (answer == 2) {

			do {
				answer = GUI_radio(get_tx2(32), 3,
						get_tx2(33),
						get_tx2(34),
						get_tx2(35));
			} while (answer == -1);

			if (answer == 1) {

				TRV_swimm(2, 5);

			} else if (answer == 2) {

				has_raft = 1;
				timewarp(HOURS(1));
				TRV_swimm(-1, 0);
			}

			if (answer == 1 || answer == 2) {

				GUI_output(get_tx2(38));

				ds_writeb(GATHER_HERBS_SPECIAL, 63);
				TRV_inside_herb_place();
				ds_writeb(GATHER_HERBS_SPECIAL, -1);

				options = (!has_raft ? 1 : 2);

				do {
					answer = GUI_radio(get_tx2(39), (signed char)options,
								get_tx2(40),
								get_tx2(41));
				} while (answer == -1);

				if (answer == 1) {

					TRV_swimm(2, 5);

				} else {

					timewarp(HOURS(1));
					TRV_swimm(-1, 0);
				}
			}
		}
	}
}

void tevent_072(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 3) > 0 && !ds_readb(TEVENT072_FLAG)) ||
		ds_readb(TEVENT072_FLAG) != 0)
	{
		TRV_found_camp_place(1);
		ds_writeb(TEVENT072_FLAG, 1);
	}
}

/* a corpse with the unicorn letter */
void tevent_073(void)
{
	signed short answer;

	if (!ds_readb(TEVENT073_FLAG)) {

		load_in_head(55);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
							get_tx2(42), 2,
							get_tx2(43), get_tx2(44));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialog_na(0, get_tx2(45));

			loot_multi_chest(p_datseg + TEVENT073_CORPSE, get_tx2(105));

			ds_writeb(TEVENT073_FLAG, 1);
		}
	}
}

/* brigants */
void tevent_074(void)
{
	signed short answer;
	signed short i;
	Bit32s p_money;
	Bit8u *hero;

	if (!ds_readb(TEVENT074_FLAG)) {

		ds_writeb(TEVENT074_FLAG, 1);

		load_in_head(49);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(49), get_tx2(46), 3,
						get_tx2(47),
						get_tx2(48),
						get_tx2(49));
		} while (answer == -1);

		if (answer == 1) {

			/* fight */
			ds_writeb(TEVENT074_FIGHT_FLAG, (signed char)TRV_fight_event(FIGHTS_F074, 74));

		} else if (answer == 2) {

			/* try to flee */

			if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_KRIEGSKUNST, 2) <= 0)
			{
				/* failed, so fight */
				ds_writeb(FIG_INITIATIVE, 1);
				ds_writeb(TEVENT074_FIGHT_FLAG, (signed char)TRV_fight_event(FIGHTS_F074, 74));

			} else {

				/* remember the position of the last hero in the group */
				hero = get_hero(0);
				for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero))
					{
						answer = i;
					}
				}

				hero = get_hero(answer);

				sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
					(char*)get_tx2(54),
					(char*)hero + HERO_NAME2);

				GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(49),
						Real2Host(ds_readd(DTP2)) + 0x400, 0);

				/* this hero gets a damage of 2W6+4 */
				sub_hero_le(hero, dice_roll(2, 6, 4));
			}
		} else {

			/* try to make a deal */
			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(49), get_tx2(50), 3,
							get_tx2(51),
							get_tx2(52),
							get_tx2(53));
			} while (answer == -1);

			if (answer == 1) {

				p_money = get_party_money();

				if (p_money < 1500) {
					/* not enough money, so fight */
					ds_writeb(TEVENT074_FIGHT_FLAG, (signed char)TRV_fight_event(FIGHTS_F074, 74));
				} else {
					/* pay 1500 */
					p_money -= 1500;
					set_party_money(p_money);
				}

			} else if (answer == 2) {

				/* fight */
				ds_writeb(TEVENT074_FIGHT_FLAG, (signed char)TRV_fight_event(FIGHTS_F074, 74));

			} else {

				/* try to be Charismatic */
				answer = count_heroes_in_group();

				if (test_attrib(Real2Host(get_first_hero_available_in_group()), ATTRIB_CH, 14 - answer) <= 0)
				{
					/* fight */
					ds_writeb(FIG_INITIATIVE, 1);
					ds_writeb(TEVENT074_FIGHT_FLAG, (signed char)TRV_fight_event(FIGHTS_F074, 74));
				}
			}
		}
	}
}

void tevent_075(void)
{
	signed short i;
	signed short answer;
	signed short ret;
	Bit8u *hero;

	ret = -1;

	if (!ds_readb(TEVENT075_FLAG)) {

		do {
			answer = GUI_radio(get_tx2(55), 2,
						get_tx2(56),
						get_tx2(57));
		} while (answer == -1);

		if (answer == 1) {

			do {
				answer = GUI_radio(get_tx2(58), 3,
							get_tx2(59),
							get_tx2(60),
							get_tx2(61));
			} while (answer == -1);

			if (answer == 1) {

				ds_writeb(FIG_INITIATIVE, 2);

				ret = TRV_fight_event(FIGHTS_F075_A, 75);

			} else if (answer == 2) {

				i = FIGHTS_F075_B;

				if (!ds_readb(TEVENT074_FIGHT_FLAG) && !ds_readb(TEVENT077_FIGHT_FLAG)) {

					i = FIGHTS_F075_A;

				} else if (ds_readb(TEVENT074_FIGHT_FLAG) != 0 && ds_readb(TEVENT077_FIGHT_FLAG) != 0) {

					i = FIGHTS_F075_C;
				}

				ret = TRV_fight_event(i, 75);

			} else {

				hero = get_hero(0);
				for (i = answer = 0; i <= 6; i++, hero += SIZEOF_HERO) {

					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero) &&
						test_skill(hero, TA_SCHLEICHEN, 0) <= 0)
					{
						answer++;
					}
				}

				if (answer > 1) {

					GUI_output(get_tx2(62));

					ds_writeb(FIG_INITIATIVE, 1);

					i = FIGHTS_F075_B;

					if (!ds_readb(TEVENT074_FIGHT_FLAG) && !ds_readb(TEVENT077_FIGHT_FLAG)) {

						i = FIGHTS_F075_A;

					} else if (ds_readb(TEVENT074_FIGHT_FLAG) != 0 && ds_readb(TEVENT077_FIGHT_FLAG) != 0) {

						i = FIGHTS_F075_C;
					}

					ret = TRV_fight_event(i, 75);

				} else {
					return;
				}
			}
		} else {
			return;
		}

		if (ret == 0) {
			ds_writeb(TEVENT075_FLAG, 1);
		}
	}
}

/* dungeon entrance: a cave */
void tevent_076(void)
{
	signed short answer;

	if (!ds_readb(TEVENT076_FLAG)) {

		if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 5) > 0)
		{
			ds_writeb(TEVENT076_FLAG, 1);

			GUI_dialog_na(53, get_tx2(63));

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
								get_tx2(64), 2,
								get_tx2(65),
								get_tx2(66));
			} while (answer == -1);

			if (answer == 1) {
				ds_writeb(TRAVEL_DETOUR, 5);
			}
		}

	} else {

		load_in_head(53);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
							get_tx2(67), 2,
							get_tx2(68),
							get_tx2(69));
		} while (answer == -1);

		if (answer == 1) {
			ds_writeb(TRAVEL_DETOUR, 5);
		}
	}
}

/* brigants */
void tevent_077(void)
{
	signed short answer;
	signed short i;
	Bit32s p_money;
	Bit8u *hero;

	if (!ds_readb(TEVENT077_FLAG)) {

		ds_writeb(TEVENT077_FLAG, 1);

		load_in_head(4);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(50),
						get_tx2(70), 3,
						get_tx2(71),
						get_tx2(72),
						get_tx2(73));
		} while (answer == -1);

		if (answer == 1) {

			/* fight */
			ds_writeb(TEVENT077_FIGHT_FLAG, (signed char)TRV_fight_event(186, 77));

		} else if (answer == 2) {

			if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_KRIEGSKUNST, 6) <= 0) {

				/* test failed, so fight */
				ds_writeb(FIG_INITIATIVE, 1);
				ds_writeb(TEVENT077_FIGHT_FLAG, (signed char)TRV_fight_event(FIGHTS_F077, 77));

			} else {

				/* remember the last hero */
				hero = get_hero(0);
				for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero))
					{
						answer = i;
					}
				}

				hero = get_hero(answer);

				sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
					(char*)get_tx2(54),
					(char*)hero + HERO_NAME2);

				GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(50),
						Real2Host(ds_readd(DTP2)) + 0x400, 0);

				/* the last hero looses between 6 and 16 LE */
				sub_hero_le(hero, random_schick(11) + 5);
			}

		} else {

			/* try to make a deal */
			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(50), get_tx2(74), 3,
							get_tx2(75),
							get_tx2(76),
							get_tx2(77));
			} while (answer == -1);

			if (answer == 1) {

				p_money = get_party_money();

				if (p_money < 1500) {
					/* not enough money, so fight */
					ds_writeb(TEVENT077_FIGHT_FLAG, (signed char)TRV_fight_event(FIGHTS_F077, 77));
				} else {
					/* pay 1500 */
					p_money -= 1500;
					set_party_money(p_money);
				}

			} else if (answer == 2) {

				/* fight */
				ds_writeb(TEVENT077_FIGHT_FLAG, (signed char)TRV_fight_event(FIGHTS_F077, 77));

			} else {

				/* try to be Charismatic */
				answer = count_heroes_in_group();

				if (test_attrib(Real2Host(get_first_hero_available_in_group()), ATTRIB_CH, 14 - answer) <= 0)
				{
					/* fight */
					ds_writeb(FIG_INITIATIVE, 1);
					ds_writeb(TEVENT077_FIGHT_FLAG, (signed char)TRV_fight_event(FIGHTS_F077, 77));
				}
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
