/*
	Rewrite of DSA1 v3.02_de functions of seg111 (travel events 3 / 10)
	Functions rewritten: 4/15
*/

#include "schick.h"

#include "v302de.h"

#include "seg002.h"
#include "seg026.h"
#include "seg097.h"

namespace M302de {

/* Path ??? */
void tevent_055(void)
{
	register signed short season;

	season = get_current_season();

	/* only in autumn or spring */
	if (season == 1 || season == 3) {

		load_in_head(0x34);

		GUI_dialogbox(ds_readd(DTP2), NULL, get_city(0), 0);

		/* you got hold for 3 hours */
		timewarp(0x3f48);
	}
}

/* dummy Orvil<->Skjal */
void tevent_062(void)
{

}

/* dead animal Orvil<->Skjal */
void tevent_063(void)
{
	Bit8u *hero;
	signed short vomiter;
	signed short proof;
	register signed short i;
	register signed short max;

	max = 9999;

	/* intro message */
	GUI_output(get_city(0xb8));

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero += 0x6da) {

		if (host_readb(hero + 0x21) == 0)
			continue;

		if (host_readb(hero + 0x87) != ds_readb(CURRENT_GROUP))
			continue;

		if (hero_dead(hero))
			continue;

		/* MU+0 */
		proof = test_attrib(hero, 0, 0);

		if (proof < max) {
			max = proof;
			vomiter = i;
		}
	}

	hero = get_hero(vomiter);

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_city(0xbc),
		(char*)hero + 0x10);

	/* print who vomits */
	GUI_output(Real2Host(ds_readd(DTP2)));

	/* LE - 2 */
	sub_hero_le(hero, 2);

	i = get_free_mod_slot();

	/* MU -2 for 24 hours */
	set_mod_slot(i, 0x1fa40, hero + 0x35, -2, vomiter);

	/* outro message */
	GUI_output(get_city(0xc0));
}

/* The rider Orvil <-> Ala */
void tevent_065(void)
{
	register signed short answer;

	/* load dialog head */
	load_in_head(0x38);

	/* show dialogbox */
	do {
		answer = GUI_dialogbox(ds_readd(DTP2),
				NULL,
				get_city(0x12c),
				3,
				get_city(0x130),
				get_city(0x134),
				get_city(0x138));

	} while (answer == -1);

	/* talk to the rider for 15 min */
	if (answer == 1 || answer == 2) {

		timewarp(0x546);

		if (answer == 1)
			GUI_dialog_na(0, get_city(0x13c));
		else
			GUI_dialog_na(0, get_city(0x140));

		if (answer == 2) {
			/* Make Umbrik Siebenstein known */

			/* Original-Bug: if Umbrik is done the value 2
					is overwritten with 1 */
#ifdef M302de_ORIGINAL_BUGFIX
			if (ds_readb(0x3367) == 0)
				ds_writeb(0x3367, 1);
#else
			ds_writeb(0x3367, 1);
#endif
		}

	}
}

}
