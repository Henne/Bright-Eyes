/**
 *	Rewrite of DSA1 v3.02_de functions of seg107 (using items)
 *	Functions rewritten: 9/14
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
#include "seg103.h"
#include "seg105.h"
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
	item_flimflam,
	item_debtbook,
	item_orcdocument,
	item_weapon_poison,
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
	/* RING ID 165 */
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
	/*	SILVER CORONET
		ID 171, 245 */
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

/* Borlandified and identical */
void item_flimflam(void)
{
	/* AMULET, ID 174 */
	signed short b1_index;

	/* save index off buffer1 */
	b1_index = ds_readws(0x26bf);

	/* load SPELLTXT*/
	load_buffer_1(0xde);

	ds_writed(SPELLUSER, ds_readd(ITEMUSER));

	spell_flimflam();

	/* decrement usage counter */
	dec_ptr_ws(get_itemuser() + 0x198 + ds_readws(USED_ITEM_POS) * 14);

	if ((b1_index != -1) && (b1_index != 0xde)) {
		/* need to reload buffer1 */
		load_buffer_1(b1_index);
	}

	GUI_output(Real2Host(ds_readd(DTP2)));

}

/* Borlandified and identical */
void item_debtbook(void)
{
	/* DEBTBOOK, ID 176 */
	if (ds_readbs(0x333c) != 0) {

		/* mark this event */
		ds_writeb(0x333c, 0);

		/* 15 AP */
		add_hero_ap_all(15);

		/* mark informer Hjore as known */
		ds_writeb(0x3365, 1);
	}

	GUI_output(get_ltx(0xa08));
}

/* Borlandified and identical */
void item_orcdocument(void)
{
	/* ORCDOCUMENT, ID 179 */

	/* Languages + 4, or already read successful */
	if ((test_skill(get_itemuser(), 0x28, 4) > 0) || (ds_readb(0x333d) != 0)) {

		if (!ds_readbs(0x333d)) {
			add_group_ap(20);
			ds_writeb(0x333d, 1);
		}

		GUI_output(get_ltx(0xa10));
	} else {
		GUI_output(get_ltx(0xa0c));
	}
}

/* Borlandified and identical */
void item_weapon_poison(void)
{
	/*	WEAPON POISON, EXPURGICUM, VOMICUM
		ID 55-59, 141-144, 166, 168 */

	signed short bottle;

	if ((host_readws(get_itemuser() + 0x1c0) != 0) &&
		(host_readws(get_itemuser() + 0x1c0) != 9) &&
		(host_readws(get_itemuser() + 0x1c0) != 19) &&
		(host_readws(get_itemuser() + 0x1c0) != 12))
	{

		switch (ds_readws(USED_ITEM_ID)) {
		case 168 : {
			/* VOMICUM */
			or_ptr_bs(get_itemuser() + 0x1c4, 0x40);

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), 168), 1);

			bottle = 42;
			break;
		}
		case 166 : {
			/* EXPURGICUM */
			or_ptr_bs(get_itemuser() + 0x1c4, 0x20);

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), 166), 1);

			bottle = 42;
			break;
		}
		case 55: {
			/* SHURIN-BULB POISON / KROETENSCHEMELGIFT */
			host_writeb(get_itemuser() + 0x1c9, 1);
			host_writeb(get_itemuser() + 0x1ca, 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), 55), 1);
			bottle = 31;
			break;
		}
		case 56: {
			/* ARAX POISON / ARAXGIFT */
			host_writeb(get_itemuser() + 0x1c9, 2);
			host_writeb(get_itemuser() + 0x1ca, 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), 56), 1);
			bottle = 31;
			break;
		}
		case 57: {
			/* FEAR POISON / ANGSTGIFT */
			host_writeb(get_itemuser() + 0x1c9, 3);
			host_writeb(get_itemuser() + 0x1ca, 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), 57), 1);
			bottle = 31;
			break;
		}
		case 58: {
			/* SLEPPING POISON / SCHALFGIFT */
			host_writeb(get_itemuser() + 0x1c9, 4);
			host_writeb(get_itemuser() + 0x1ca, 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), 58), 1);
			bottle = 31;
			break;
		}
		case 59: {
			/* GOLDEN GLUE / GOLDLEIM */
			host_writeb(get_itemuser() + 0x1c9, 5);
			host_writeb(get_itemuser() + 0x1ca, 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), 59), 1);
			bottle = 31;
			break;
		}
		case 141: {
			/* LOTUS POISON / LOTUSGIFT */
			host_writeb(get_itemuser() + 0x1c9, 7);
			host_writeb(get_itemuser() + 0x1ca, 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), 141), 1);
			bottle = 31;
			break;
		}
		case 142: {
			/* KUKRIS */
			host_writeb(get_itemuser() + 0x1c9, 8);
			host_writeb(get_itemuser() + 0x1ca, 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), 142), 1);
			bottle = 31;
			break;
		}
		case 143: {
			/* BANE DUST / BANNSTAUB */
			host_writeb(get_itemuser() + 0x1c9, 9);
			host_writeb(get_itemuser() + 0x1ca, 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), 143), 1);
			bottle = 31;
			break;
		}
		case 144: {
			host_writeb(get_itemuser() + 0x1c9, 6);
			host_writeb(get_itemuser() + 0x1ca, 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), 144), 1);
			bottle = 31;
			break;
		}
		}

		give_hero_new_item(get_itemuser(), bottle, 1, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0xb8c),
			(char*)Real2Host(GUI_names_grammar(0x8000, host_readws(get_itemuser() + 0x1c0), 0)));
	} else {
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0xc94),
			(char*)get_itemuser() + 0x10);
	}

	GUI_output(Real2Host(ds_readd(DTP2)));
}

void dummy9(void)
{
	/* MYASTMATIC, ID 238 */
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy10(void)
{
	/* HYLAILIC FIRE, ID 239 */
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy11(void)
{
	/* BOOK, ID 246 */
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy12(void)
{
	/*	LANTERN, TORCH, TINDERBOX, LANTERN
		ID 25, 65, 85, 249 */
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

void dummy13(void)
{
	/*	MAGIC BREADBAG, BAG
		ID 184, 221 */
#if !defined(__BORLANDC__)
	D1_INFO("Item %s\n", __func__);
#endif
}

#if !defined(__BORLANDC__)
}
#endif
