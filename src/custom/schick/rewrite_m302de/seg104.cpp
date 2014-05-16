/**
 *	Rewrite of DSA1 v3.02_de functions of seg104 (heros)
 *	Functions rewritten: 2/9
 */

#include "v302de.h"

#include "seg002.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
signed short hero_has_ingrendients(Bit8u *hero, signed short recipe_index)
{
	signed short i = 0;
	signed short retval = 1;
	Bit8u* r_ptr = p_datseg + 0xacda + recipe_index * 28;
	signed short item_pos;

	/* loop over ingrendients */
	while ((host_readws(r_ptr + i * 2 + 2) != -1) && (retval)) {

		item_pos = get_item_pos(hero, host_readws(r_ptr + i * 2 + 2));

		if (item_pos == -1) {
			/* needed item missing */
			retval = 0;
			ds_writew(0xe5c4, host_readws(r_ptr + i * 2 + 2));
		} else {
			/* drop all needed items */
			drop_item(hero, item_pos, 1);
		}

		i++;
	}

	i--;

	if (!retval) {
		i--;
	}

	while (i >= 0) {
		/* give all needed items back */
		give_hero_new_item(hero, host_readws(r_ptr + i * 2 + 2), 1, 1);
		i--;
	}

	return retval;
}

/* Borlandified and identical */
void hero_use_ingrendients(Bit8u *hero, signed short recipe_index)
{
	signed short i = 0;
	Bit8u* r_ptr = p_datseg + 0xacda + recipe_index * 28;
	signed short item_pos;

	/* loop over ingrendients */
	while (host_readws(r_ptr + i * 2 + 2) != -1) {

		item_pos = get_item_pos(hero, host_readws(r_ptr + i * 2 + 2));

		/* drop all needed items */
		drop_item(hero, item_pos, 1);

		/* exchange wine- or brandybottles into glass flask */
		if ((host_readws(r_ptr + i * 2 + 2) == 0x5b) ||
			(host_readws(r_ptr + i * 2 + 2) == 0x5c))
		{
			give_hero_new_item(hero, 0x1f, 1, 1);
		}

		/* exchange oil into bronze flask */
		if (host_readws(r_ptr + i * 2 + 2) == 0x29)
		{
			give_hero_new_item(hero, 0x2a, 1, 1);
		}

		i++;
	}
}

#if !defined(__BORLANDC__)
}
#endif
