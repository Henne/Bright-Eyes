/*
 *	Rewrite of DSA1 v3.02_de functions of seg111 (travel events 3 / 10)
 *	Functions rewritten: 8/15
*/

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg109.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void tevent_053(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 1) > 0 && !ds_readb(0x3dc6)) ||
		ds_readbs(0x3dc6) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(0x3dc6, 1);
	}

}

/* Borlandified and identical */
void tevent_054(void)
{
	TRV_found_inn(47, 68);
}

/* Path ??? */
/* Borlandified and identical */
void tevent_055(void)
{
	signed short season;

	season = get_current_season();

	/* only in autumn or spring */
	if (season == 1 || season == 3) {

		load_in_head(52);

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_city(0), 0);

		timewarp(HOURS(3));
	}
}

/* Borlandified and identical */
void tevent_056(void)
{
	TRV_found_inn(48, 69);
}

/* Borlandified and identical */
void tevent_057(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_city(0x04), 2, get_city(0x08), get_city(0x0c));

	} while (answer == -1);

	if (answer == 2) {

		if (test_skill(Real2Host(get_first_hero_available_in_group()), 28, 2) > 0) {

			timewarp(HOURS(2));

			GUI_output(get_city(0x10));

		} else {

			timewarp(HOURS(6));

			GUI_output(get_city(0x14));

			hero = get_hero(0);

			for (i = 0; i <= 6; i++, hero += 0x6da) {

				if (host_readbs(hero + 0x21) != 0 &&
					host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP))
				{
					sub_hero_le(hero, random_schick(3));
				}
			}
		}
	} else {

		hero = get_hero(0);

		for (i = 0; i <= 6; i++, hero += 0x6da) {

			if (host_readbs(hero + 0x21) != 0 &&
				host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, 10, 3) <= 0)
			{

				/* Original-Bug: that condition does not make sense */
				if (get_first_hero_with_item(121) == -1 || get_first_hero_with_item(32) != -1)
				{

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x20),
						(char*)hero + 0x10,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_hero_le(hero, random_schick(9) + 3);

				} else {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x1c),
						(char*)hero + 0x10);

					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_hero_le(hero, random_schick(8));
				}
			}
		}

		GUI_output(get_city(0x18));
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
	signed short i;
	signed short max;

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
	set_mod_slot(i, 0x1fa40, hero + 0x35, -2, (signed char)vomiter);

	/* outro message */
	GUI_output(get_city(0xc0));
}

/* The rider Orvil <-> Ala */
void tevent_065(void)
{
	signed short answer;

	/* load dialog head */
	load_in_head(0x38);

	/* show dialogbox */
	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
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
			if (ds_readb(INFORMER_UMBRIK) == 0)
				ds_writeb(INFORMER_UMBRIK, 1);
#else
			ds_writeb(INFORMER_UMBRIK, 1);
#endif
		}

	}
}

#if !defined(__BORLANDC__)
}
#endif
