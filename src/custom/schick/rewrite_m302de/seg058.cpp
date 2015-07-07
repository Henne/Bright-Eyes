/**
 *	Rewrite of DSA1 v3.02_de functions of seg058 (smith)
 *	Functions rewritten: 1/5
 */

#include "v302de.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief	add all items of a hero to the repair list
 * \param smith_ptr	pointer to the smith description
 * \param hero		pointer to the hero
 * \param item_pos	the position of the item in the inventory
 * \param smith_pos	the position of the item in the repair list
 */
/* Borlandified and identical */
void add_item_to_smith(Bit8u *smith_ptr, Bit8u *hero, signed short item_pos, signed short smith_pos)
{
	signed short item_id;

	item_id = host_readws(hero + 0x196 + 14 * item_pos);

	host_writews(Real2Host(ds_readd(0xc005)) + 7 * smith_pos, item_id);

	if (item_armor(get_itemsdat(item_id)) || item_weapon(get_itemsdat(item_id))) {

		if (ks_broken(hero + 0x196 + 14 * item_pos)) {

			host_writews(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 2,
				(host_readws(get_itemsdat(item_id) + 8) +
					(host_readws(get_itemsdat(item_id) + 8) * host_readbs(smith_ptr) / 100)) / 2);

			if (host_readws(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 2) == 0) {
				host_writews(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 2, 1);
			}

			host_writews(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 4,
				host_readbs(get_itemsdat(item_id) + 7));

		} else {

			if (host_readbs(hero + 0x196 + 7 + 14 * item_pos) != 0) {

				host_writews(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 2,
					(host_readws(get_itemsdat(item_id) + 8) +
						(host_readws(get_itemsdat(item_id) + 8) * host_readbs(smith_ptr) / 100)) / 4);

				if (host_readws(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 2) == 0) {
					host_writews(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 2, 1);
				}

				host_writews(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 4,
					host_readbs(get_itemsdat(item_id) + 7));
			} else {
				host_writews(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 2, 0);
				host_writews(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 4, 1);
			}
		}
	} else {
		host_writews(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 2, 0);
		host_writews(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 4, 1);
	}

	host_writebs(Real2Host(ds_readd(0xc005)) + 7 * smith_pos + 6, item_pos);
}

void do_smith(void)
{
	DUMMY_WARNING();
}

void TLK_schmied(signed short state)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif
