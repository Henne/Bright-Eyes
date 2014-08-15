/**
 *	Rewrite of DSA1 v3.02_de functions of seg107 (using items)
 *	Functions rewritten: 5/14
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
	item_arcano,
	item_read_recipe,
	item_read_document,
	item_armatrutz,
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
void item_arcano(void)
{
	signed short b1_index;

	/* save index off buffer1 */
	b1_index = ds_readws(0x26bf);

	/* load SPELLTXT*/
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
		/* need to reload buffer1 */
		load_buffer_1(b1_index);
	}
}

/* Borlandified and identical */
void item_read_recipe(void)
{
	Bit8u *str;

	switch (ds_readws(USED_ITEM_ID)) {
	case 0xa7: str = get_ltx(0x9fc); break;
	case 0xa9: str = get_ltx(0xa00); break;
	case 0xca: str = get_ltx(0xa24); break;
	case 0xcb: str = get_ltx(0xa28); break;
	case 0xcc: str = get_ltx(0xa2c); break;
	case 0xcd: str = get_ltx(0xa30); break;
	case 0xce: str = get_ltx(0xa34); break;
	case 0xba: str = get_ltx(0xaa4); break;
	case 0xf0: str = get_ltx(0xaa8); break;
	case 0xf2: str = get_ltx(0xab0); break;
	case 0xf3: str = get_ltx(0xab4); break;
	case 0xf4: str = get_ltx(0xab8); break;
	case 0xf1: str = get_ltx(0xaac); break;
	}

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_ltx(0x9f0),
		(char*)str);

	GUI_output(Real2Host(ds_readd(DTP2)));
}

/* Borlandified and identical */
void item_read_document(void)
{
	Bit8u *str;
	signed short bak;

	switch (ds_readws(USED_ITEM_ID)) {
	case 0xaa: str = get_ltx(0xa04); break;
	case 0xbb: str = get_ltx(0xa14); break;
	case 0xbd: str = get_ltx(0xa18); break;
	case 0xbe: str = get_ltx(0xa1c); break;
	case 0xc0: str = get_ltx(0xa20); break;
	case 0xde: str = get_ltx(0xa38); break;
	case 0xe0: str = get_ltx(0xa3c); break;
	case 0xeb: str = get_ltx(0xa44); break;
	case 0xf7: str = get_ltx(0xbdc); break;
	}

	bak = ds_readws(0xbffd);
	ds_writew(0xbffd, 7);
	GUI_output(str);
	ds_writew(0xbffd, bak);
}

/* Borlandified and identical */
void item_armatrutz(void)
{
	signed short b1_index;

	/* save index off buffer1 */
	b1_index = ds_readws(0x26bf);

	/* load SPELLTXT*/
	load_buffer_1(0xde);

	ds_writed(SPELLUSER, ds_readd(ITEMUSER));

	/* ask who should be affected */
	host_writeb(get_spelluser() + 0x86,
		select_hero_from_group(get_ltx(0x9f4)) + 1);

	if (host_readbs(get_spelluser() + 0x86) > 0) {
		/* use it */
		spell_armatrutz();
		/* decrement usage counter */
		dec_ptr_ws(get_itemuser() + 0x198 + ds_readws(USED_ITEM_POS) * 14);

		GUI_output(Real2Host(ds_readd(DTP2)));
	}

	if ((b1_index != -1) && (b1_index != 0xde)) {
		/* need to reload buffer1 */
		load_buffer_1(b1_index);
	}
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
