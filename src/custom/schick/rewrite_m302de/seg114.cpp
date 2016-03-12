/**
 *	Rewrite of DSA1 v3.02_de functions of seg114 (travel events 6 / 10)
 *	Functions rewritten: 1/11
 */
#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* wall of rock */
/* Borlandified and identical */
void tevent_110(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_city(0x13c), 2,
					get_city(0x140),
					get_city(0x144));
	} while (answer == -1);

	if (answer == 1)
	{
		/* try to climb */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, 10, 0) <= 0)
			{
				/* skill test failed */

				if (get_first_hero_with_item(121) != -1)
				{
					/* one hero in the group has a rope */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x154),
						(char*)hero + HERO_NAME2);

					sub_hero_le(hero, random_schick(10));

				} else {
					/* all heros in the group have no ropes */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x158),
						(char*)hero + HERO_NAME2);

					sub_hero_le(hero, random_schick(16) + 4);
				}

				GUI_output(Real2Host(ds_readd(DTP2)));
			}

		}

		GUI_output(get_city(0x150));

	} else {

		/* try to walk arround */

		if (test_skill(Real2Host(get_first_hero_available_in_group()), 28, 0) > 0)
		{
			/* success */
			timewarp(HOURS(4));
			GUI_output(get_city(0x148));
		} else {
			/* fail */
			timewarp(DAYS(1) + HOURS(6));
			GUI_output(get_city(0x14c));
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
