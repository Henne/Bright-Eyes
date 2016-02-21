/**
 *	Rewrite of DSA1 v3.02_de functions of seg070 (phexcaer: buildings 1/2)
 *	Functions rewritten: 7/7 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg070.cpp
 */
#include <stdio.h>


#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg027.h"
#include "seg032.h"
#include "seg047.h"
#include "seg053.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief		carriage station
 */
void PHX_fuhrhaus(void)
{
	signed short answer;

	load_in_head(42);

	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
				get_city(0x00), 3,
				get_city(0x04), get_city(0x08), get_city(0x0c));

	} while (answer == -1);

	if (answer == 1) {

		GUI_dialog_na(0, get_city(0x10));

	} else if (answer == 2) {

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
					get_city(0x14), 3,
					get_city(0x18), get_city(0x1c), get_city(0x20));

		} while (answer == -1);

		GUI_dialog_na(0, answer == 1 ? get_city(0x28): (answer == 2 ? get_city(0x2c) : get_city(0x30)));

		if (answer == 3) {
			/* You are now able to talk to Alrik */
			ds_writeb(ALRIK_DERONDAN, 1);
		}

		GUI_dialog_na(0, get_city(0x34));

	} else {

		GUI_dialog_na(0, get_city(0x24));
	}
}

/**
 * \brief	stadthaus of Phexcaer
 */
void PHX_stadthaus(void)
{
	signed short answer;
	Bit32s money;

	do {
		answer = GUI_radio(get_city(0x38), 3,
					get_city(0x3c),
					get_city(0x40),
					get_city(0x44));

	} while (answer == -1);

	if (answer == 1) {

		GUI_output(get_city(0x48));

	} else {

#ifdef M302de_ORIGINAL_BUGFIX
		signed short answer2;
#endif
		money = get_party_money();

		/* reuse answer as number of options */
		answer = money >= 1000 ? 3 : 1;

#ifdef M302de_ORIGINAL_BUGFIX

		do {
			answer2 = GUI_radio(get_city(0x4c), (signed char)answer,
						get_city(0x54),
						get_city(0x50),
						get_city(0x58));
		} while (answer2 == -1);

		answer = answer2;

#else
		/* Original-Bug: pressing ESC calls GUI_radio() again with -1 options */
		do {
			answer = GUI_radio(get_city(0x4c), answer,
						get_city(0x54),
						get_city(0x50),
						get_city(0x58));

		} while (answer == -1);
#endif

		if (answer == 1) {

			GUI_output(get_city(0x5c));

		} else {

			/* the Party looses 10D */
			money -= 1000;

			set_party_money(money);

			GUI_output(get_city(0x60));
		}
	}
}

/**
 * \brief	spielhaus of Phexcaer
 */
void PHX_spielhaus(void)
{
	signed short answer;
	signed short counter;
	signed short pos;
	Bit32s money;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_city(0x64), 3,
					get_city(0x68),
					get_city(0x6c),
					get_city(0x70));

	} while (answer == -1);

	if (answer == 1) {

		GUI_output(get_city(0x74));

		hero = get_hero(0);

		for (pos = counter = answer = 0; pos <= 6; pos++, hero += SIZEOF_HERO) {

			if ((host_readbs(hero + 0x21) != 0) &&
				(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)) &&
				!hero_dead(hero) &&
				(test_skill(hero, 43, 3) > 0))
			{
				/* Gambling +3 */
				counter++;
			}
		}

		pos = random_schick(20);

		money = get_party_money();

#ifdef M302de_ORIGINAL_BUGFIX
		if ((count_heroes_in_group() >> 1) >= counter) {
#else
		if ((count_heroes_in_group() >> 1) < counter) {
#endif

			/* you loose: at least the half of your group have not passed the test */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x78), pos);

			money -= pos * 100;

			if (money < 0) {
				money = 0;
			}
		} else {

			/* you win */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x7c), pos);

			money += pos * 100;
		}

		set_party_money(money);

		GUI_output(Real2Host(ds_readd(DTP2)));

	} else if (answer == 2) {

		GUI_output(get_city(0x80));

		/* You are now able to talk to Alrik */
		ds_writeb(ALRIK_DERONDAN, 1);

	} else {

		money = get_party_money();

		pos = money >= 500 ? 3 : 2;

		do {
			answer = GUI_radio(get_city(0x84), (signed char)pos,
						get_city(0x88),
						get_city(0x90),
						get_city(0x8c));

		} while (answer == -1);

		GUI_output(answer == 3 ? get_city(0x98): get_city(0x94));

		if (answer == 3) {

			money -= 500;

			if (money < 0) {
				money = 0;
			}
		}

		set_party_money(money);
	}
}

/**
 * \brief	the villa of Ektor Gremob
 */
void PHX_villa_gremob(void)
{
	GUI_output(!ds_readb(GREMOB_INVITED)? get_city(0xa0) : get_city(0x9c));

	if (ds_readb(GREMOB_INVITED) != 0) {
		do_fight(239);
	}
}

/**
 * \brief	the brothel
 */
void PHX_bordell(void)
{
	signed short answer;

	load_ani(24);

	init_ani(0);

	do {
		answer = GUI_radio(get_city(0xa4), 3,
					get_city(0xa8),
					get_city(0xac),
					get_city(0xb0));

	} while (answer == -1);

	if (answer == 1) {

		do {
			answer = GUI_radio(get_city(0xb4), 3,
						get_city(0xb8),
						get_city(0xbc),
						get_city(0xc0));

		} while (answer == -1);

		GUI_input(answer == 3 ? get_city(0xcc) : get_city(0xd0), 0);

		if ((answer == 1) || (answer == 2)) {
			ds_writeb(HARLOT_DATE, 1);
		}

	} else if (answer == 2) {

		add_ds_ds(0x317e, 30L);

		timewarp_until(0x93a8);

		GUI_input(get_city(0xc4), 0);

	} else if (answer == 3) {	/* this check is bogus */

		GUI_input(get_city(0xc8), 0);
	}

	set_var_to_zero();
}

/**
 * \brief	the pharmacy
 */
void PHX_apotheke(void)
{
	signed short answer;
	signed short options;
	Bit32s money;

	load_ani(23);

	init_ani(0);

	do {
		answer = GUI_radio(get_city(0xd4), 3,
					get_city(0xd8),
					get_city(0xdc),
					get_city(0xe0));

	} while (answer == -1);

	if (answer == 1) {

		money = get_party_money();

		options = money >= 1000 ? 3 : 2;


		do {
			answer = GUI_radio(get_city(0xe4), (signed char)options,
						get_city(0xf8),
						get_city(0xf4),
						get_city(0xf0));

		} while (answer == -1);

		if (answer == 3) {

			money -= 1000;

			set_party_money(money);

			get_item(61, 1, 1);
		} else {
			GUI_input(get_city(0x114), 0);
		}

	} else if (answer == 2) {

		money = get_party_money();

		answer = money >= 500 ? 3 : 2;

		do {
			answer = GUI_radio(get_city(0xe8), (signed char)answer,
						get_city(0xfc),
						get_city(0x104),
						get_city(0x100));

		} while (answer == -1);


		if (answer == 3) {

			money -= 500;

			set_party_money(money);

			GUI_input(get_city(0x11c), 0);
		} else {

			GUI_input(answer == 1 ? get_city(0x118) : get_city(0x114), 0);

		}
	} else {

		do {
			answer = GUI_radio(get_city(0xec), 3,
						get_city(0x108),
						get_city(0x10c),
						get_city(0x110));

		} while (answer == -1);

		if (answer == 1) {

			GUI_input(get_city(0x114), 0);

		} else if (answer == 2) {

			do {
				answer = GUI_radio(get_city(0x120), 3,
							get_city(0xdc),
							get_city(0x124),
							get_city(0x128));

			} while (answer == -1);

			if (answer == 1) {
				money = get_party_money();

				answer = money >= 500 ? 3 : 2;

				do {
					answer = GUI_radio(get_city(0xe8), (signed char)answer,
								get_city(0xfc),
								get_city(0x104),
								get_city(0x100));

				} while (answer == -1);

				if (answer == 3) {

					money -= 500;

					set_party_money(money);

					GUI_input(get_city(0x11c), 0);
				} else {
					GUI_input(answer == 1 ? get_city(0x118) : get_city(0x114), 0);
				}
			} else {
				GUI_input(get_city(0x12c), 0);
			}

		} else {

			money = get_party_money();

			answer = money >= 1000 ? 3 : 2;

			do {
				answer = GUI_radio(get_city(0xe4), (signed char)answer,
							get_city(0xf8),
							get_city(0xf4),
							get_city(0xf0));

			} while (answer == -1);

			if (answer == 3) {

				money -= 1000;

				set_party_money(money);

				get_item(61, 1, 1);
			} else {
				GUI_input(get_city(0x114), 0);
			}
		}
	}

	set_var_to_zero();
}

/**
 * \brief	the healer
 */
void PHX_healer(void)
{
	signed short answer;
	Bit32s money;

	answer = random_schick(100);

	if (answer <= 70) {

		GUI_output(get_city(0x130));

	} else if (answer <= 85) {

		GUI_output(get_city(0x134));

	} else {

		load_ani(23);

		init_ani(0);

		do {
			answer = GUI_radio(get_city(0x138), 3,
						get_city(0x13c),
						get_city(0x140),
						get_city(0x144));

		} while (answer == -1);

		if (answer == 1) {

			GUI_output(get_city(0x148));

			/* enter the healer */
			ds_writew(TYPEINDEX, 13);
			ds_writew(CITYINDEX, 15);

			do_healer();

		} else if (answer == 3) {

			GUI_output(get_city(0x150));

		} else {
			/* answer == 2 */
			do {
				answer = GUI_radio(get_city(0x14c), 3,
							get_city(0x154),
							get_city(0x158),
							get_city(0x15c));

			} while (answer == -1);

			if (answer == 1) {

				do {
					answer = GUI_radio(get_city(0x160), 3,
								get_city(0x164),
								get_city(0x168),
								get_city(0x16c));

				} while (answer == -1);

			} else if (answer == 2) {

				money = get_party_money();

				if (money >= 200) {

					GUI_output(get_city(0x170));

					money -= 200;

					set_party_money(money);
				} else {
					GUI_output(get_city(0x18c));
				}
			} else {

				money = get_party_money();

				answer = money >= 500 ? 3 : (money >= 10 ? 2 : 1);

				do {
					answer = GUI_radio(get_city(0x174), (signed char)answer,
								get_city(0x180),
								get_city(0x178),
								get_city(0x17c));

				} while (answer == -1);

				if (answer == 2) {

					money -= 10;

					set_party_money(money);

					GUI_output(get_city(0x184));

				} else if (answer == 3) {

					money -= 500;

					set_party_money(money);

					GUI_output(get_city(0x188));
				}
			}
		}
	}

	set_var_to_zero();
}

#if !defined(__BORLANDC__)
}
#endif
