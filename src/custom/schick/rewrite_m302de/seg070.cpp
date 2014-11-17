/**
 *	Rewrite of DSA1 v3.02_de functions of seg070 (phexcaer: buildings 1/2)
 *	Functions rewritten: 2/7
 */

#include "v302de.h"

#include "seg002.h"
#include "seg026.h"
#include "seg097.h"

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

		money = get_party_money();

		/* reuse answer as number of options */
		answer = money >= 1000 ? 3 : 1;

		/* Original-Bug: pressing ESC calls GUI_radio() again with -1 options */
		do {
			answer = GUI_radio(get_city(0x4c), answer,
						get_city(0x54),
						get_city(0x50),
						get_city(0x58));

		} while (answer == -1);

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
#if !defined(__BORLANDC__)
}
#endif
