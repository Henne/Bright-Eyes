/**
 *	Rewrite of DSA1 v3.02_de functions of seg055 (merchant: main)
 *	Functions rewritten: 1/6
 */

#include "v302de.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief		add an item to the shop
 * \param shop_ptr	pointer to the shop descrition
 * \param item_id	item_id
 * \param pos		position in the shop
 */
/* Borlandified and identical */
void add_item_to_shop(Bit8u *shop_ptr, signed short item_id, signed short pos)
{
	host_writews(Real2Host(ds_readd(0xc009)) + 7 * pos, item_id);

	host_writews(Real2Host(ds_readd(0xc009)) + 7 * pos + 2,
		host_readws(get_itemsdat(item_id) + 8) + host_readws(get_itemsdat(item_id) + 8) * host_readbs(shop_ptr) / 100);

	host_writews(Real2Host(ds_readd(0xc009)) + 7 * pos + 4,
			host_readbs(get_itemsdat(item_id) + 7));
}

void do_merchant(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else
#endif
}

void talk_merchant(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else
#endif
}

void TLK_ghandel(signed short state)
{
	DUMMY_WARNING();
}

void TLK_khandel(signed short state)
{
	DUMMY_WARNING();
}

void TLK_whandel(signed short state)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif
