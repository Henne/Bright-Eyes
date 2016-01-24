/**
 *	Rewrite of DSA1 v3.02_de functions of seg112 (travel events 4 / 10)
 *	Functions rewritten: 2/13
 */

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg092.h"
#include "seg097.h"
#include "seg103.h"
#include "seg109.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void tevent_067(void)
{
	signed short i;
	signed short answer;
	signed short count;
	Bit8u *hero;

	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 51, 6) > 0 && !ds_readb(0x3dcc)) ||
		ds_readb(0x3dcc) != 0)
	{
		GUI_output(get_city(0x168));

		do {
			answer = GUI_radio(get_city(0x16c), 3,
						get_city(0x170),
						get_city(0x174),
						get_city(0x178));
		} while (answer == -1);

		if (answer == 2) {

			ds_writeb(LOCATION, 6);
			do_location();
			ds_writeb(LOCATION, 0);

		} else if (answer == 3) {

			hero = get_hero(0);
			for (i = count = 0; i <= 6; i++, hero += 0x6da) {

				if (host_readbs(hero + 0x21) != 0 &&
					host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero) &&
					test_attrib(hero, 4, 0) > 0)
				{
					count++;
				}
			}

			if (!ds_readb(0x3dcc) && count >= 3) {

				loot_multi_chest(p_datseg + 0xb16a, get_city(0x180));

				GUI_output(get_city(0x184));

				hero = Real2Host(get_first_hero_available_in_group());

				add_party_money(7L);

				ds_writeb(LOCATION, 6);
				do_location();
				ds_writeb(LOCATION, 0);
			} else {

				GUI_output(get_city(0x17c));

				hero = get_hero(0);
				for (i = 0; i <= 6; i++, hero += 0x6da) {

					if (host_readbs(hero + 0x21) != 0 &&
						host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP))
					{
						sub_hero_le(hero, random_schick(8));
					}
				}

				ds_writeb(LOCATION, 6);
				do_location();
				ds_writeb(LOCATION, 0);

				TRV_load_textfile(-1);
			}

			ds_writeb(0x3dcc, 1);
		}
	} else {

		GUI_output(get_city(0x168));

		do {
			answer = GUI_radio(get_city(0x148), 2,
						get_city(0x170),
						get_city(0x174));
		} while (answer == -1);

		if (answer == 2) {
			ds_writeb(LOCATION, 6);
			do_location();
			ds_writeb(LOCATION, 0);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
