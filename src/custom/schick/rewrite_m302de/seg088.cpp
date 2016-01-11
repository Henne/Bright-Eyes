/**
 *	Rewrite of DSA1 v3.02_de functions of seg088 (dungeon: thorwal 2 / 2)
 *	Functions rewritten: 2/12
 */
#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif


/* Borlandified and identical */
void DNG14_dive(signed short diver_pos, signed char mod, signed short dest_x)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += 0x6da)
	{
		if (i != diver_pos &&
			host_readbs(hero + 0x21) != 0 &&
			host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero))
		{

			if (test_skill(hero, 14, mod) <= 0) {
				/* swimming failed */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0xa8),
					(char*)hero + 0x10,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)),
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)));

				GUI_output(Real2Host(ds_readd(DTP2)));

				/* loose 2W6 LE */
				sub_hero_le(hero, dice_roll(2, 6, 0));
			} else {
				/* swimming succeeded */

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0xac),
					(char*)hero + 0x10);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}
	}

	ds_writew(X_TARGET, dest_x);
	ds_writews(0xe482, -1);

}

/**
 * \brief print introductions of dungeon #14 fights
 *
 * \param fight_id	ID of the fight
 */
/* Borlandified and identical */
void DNG14_fight_intro(signed short fight_id)
{
	if ((fight_id == 172) || (fight_id == 174)) {
		GUI_output(get_dtp(0xe0));
	}
}

#if !defined(__BORLANDC__)
}
#endif
