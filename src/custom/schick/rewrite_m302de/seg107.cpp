/**
 *	Rewrite of DSA1 v3.02_de functions of seg107 (using items)
 *	Functions rewritten: 2/14
 */

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg101.h"
#include "seg107.h"
#include "seg108.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if !defined(__BORLANDC__)

static void (*handler[])(void) = {
	NULL,
	item_ring,
	dummy2,
	dummy3,
	dummy4,
	dummy5,
	dummy6,
	dummy7,
	dummy8,
	dummy9,
	dummy10,
	dummy11,
	dummy12,
	dummy13
};

#endif

/* Borlandified and identical */
void use_item(signed short item_pos, signed short hero_pos)
{
	void (*func)(void);

	/* set global variables for item usage */
	ds_writew(USED_ITEM_POS, item_pos);

	ds_writed(ITEMUSER, (Bit32u)((RealPt)ds_readd(HEROS) + hero_pos * 0x6da));

	ds_writew(USED_ITEM_ID, host_readws(get_itemuser() + ds_readws(USED_ITEM_POS) * 14 + 0x196));

	ds_writed(USED_ITEM_DESC, (Bit32u)((RealPt)ds_readd(ITEMSDAT) + ds_readws(USED_ITEM_ID) * 12));

	if (check_hero(get_itemuser())) {

			if (!item_useable(Real2Host(ds_readd(USED_ITEM_DESC)))) {
				/* item is not usable */

				if (is_in_word_array(ds_readws(USED_ITEM_ID), (signed short*)(p_datseg + 0x29e)))
				{
					/* german grammar, singular and plural are the same */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0xc60),
						(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(ds_readws(USED_ITEM_ID)))));
				} else {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x8ec),
						(char*)Real2Host(GUI_names_grammar(0, ds_readws(USED_ITEM_ID), 0)));
				}

				GUI_output(Real2Host(ds_readd(DTP2)));

			} else if ((item_herb_potion(Real2Host(ds_readd(USED_ITEM_DESC)))) &&
					!is_in_word_array(ds_readws(USED_ITEM_ID), (signed short*)(p_datseg + POISON_POTIONS)))
			{
				/* don't consume poison */
				consume(get_itemuser(), get_itemuser(), item_pos);

			} else if ((host_readws(get_itemuser() + 14 * ds_readws(USED_ITEM_POS) + 0x198)) <= 0) {
				/* magic item is used up */
				GUI_output(get_ltx(0x9f8));
			} else {
				/* special item */
#if !defined(__BORLANDC__)
				func = handler[ds_readbs(0x8ab + 3 * host_readbs(Real2Host(ds_readd(USED_ITEM_DESC)) + 4))];
#else
				func = (void (*)(void))ds_readd(0xaeb0 + 4 * ds_readbs(0x8ab + 3 * host_readbs(Real2Host(ds_readd(USED_ITEM_DESC)) + 4)));
#endif
				func();
			}
	}
}

/* Borlandified and identical */
void item_ring(void)
{
	signed short b1_index;

	/* save index off buffer1 */
	b1_index = ds_readws(0x26bf);

	/* load */
	load_buffer_1(0xde);

	ds_writed(SPELLUSER, ds_readd(ITEMUSER));

	/* ask who should be affected */
	host_writeb(get_spelluser() + 0x86,
		select_hero_from_group(get_ltx(0x9f4)) + 1);

	if (host_readbs(get_spelluser() + 0x86) > 0) {
		/* use it */
		spell_arcano();
		/* decrement usage counter */
		dec_ptr_ws(get_itemuser() + 0x198 + ds_readws(USED_ITEM_POS) * 14);
	}

	if ((b1_index != -1) && (b1_index != 0xde)) {
		load_buffer_1(b1_index);
	}

#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy2(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy3(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy4(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy5(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy6(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy7(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy8(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy9(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy10(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy11(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy12(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy13(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

#if !defined(__BORLANDC__)
}
#endif
