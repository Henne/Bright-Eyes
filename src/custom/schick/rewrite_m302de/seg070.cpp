/**
 *	Rewrite of DSA1 v3.02_de functions of seg070 (phexcaer: buildings 1/2)
 *	Functions rewritten: 4/7
 */
#include <stdio.h>


#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg032.h"
#include "seg047.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief		carriage station
 */
/* Borlandified and identical */
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
/* Borlandified and identical */
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
/* Borlandified and identical */
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

		for (pos = counter = answer = 0; pos <= 6; pos++, hero += 0x6da) {

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
/* Borlandified and identical */
void PHX_villa_gremob(void)
{
	GUI_output(!ds_readb(GREMOB_INVITED)? get_city(0xa0) : get_city(0x9c));

	if (ds_readb(GREMOB_INVITED) != 0) {
		do_fight(239);
	}
}

#if !defined(__BORLANDC__)
}
#endif
