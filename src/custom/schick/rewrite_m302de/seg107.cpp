/**
 *	Rewrite of DSA1 v3.02_de functions of seg107 (using items)
 *	Functions rewritten: 14/14 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg107.cpp
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
	item_myastmatic,
	item_hylailic,
	item_magic_book,
	item_brenne,
	item_bag
};

#endif

/* Borlandified and identical */
void use_item(signed short item_pos, signed short hero_pos)
{
	void (*func)(void);

	/* set global variables for item usage */
	ds_writew(USED_ITEM_POS, item_pos);

	ds_writed(ITEMUSER, (Bit32u)((RealPt)ds_readd(HEROES) + hero_pos * SIZEOF_HERO));

	ds_writew(USED_ITEM_ID, host_readws(get_itemuser() + ds_readws(USED_ITEM_POS) * SIZEOF_INVENTORY + HERO_INVENTORY + INVENTORY_ITEM_ID));

	ds_writed(USED_ITEM_DESC, (Bit32u)((RealPt)ds_readd(ITEMSDAT) + ds_readws(USED_ITEM_ID) * 12));

	if (check_hero(get_itemuser())) {

			if (!item_useable(Real2Host(ds_readd(USED_ITEM_DESC)))) {
				/* item is not usable */

				if (is_in_word_array(ds_readws(USED_ITEM_ID), (signed short*)(p_datseg + ITEMS_PLURALWORDS)))
				{
					/* german grammar, singular and plural are the same */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(792),
						(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(ds_readws(USED_ITEM_ID)))));
				} else {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(571),
						(char*)Real2Host(GUI_names_grammar(0, ds_readws(USED_ITEM_ID), 0)));
				}

				GUI_output(Real2Host(ds_readd(DTP2)));

			} else if ((item_herb_potion(Real2Host(ds_readd(USED_ITEM_DESC)))) &&
					!is_in_word_array(ds_readws(USED_ITEM_ID), (signed short*)(p_datseg + POISON_POTIONS)))
			{
				/* don't consume poison */
				consume(get_itemuser(), get_itemuser(), item_pos);

			} else if ((host_readws(get_itemuser() + SIZEOF_INVENTORY * ds_readws(USED_ITEM_POS) + (HERO_INVENTORY + INVENTORY_QUANTITY))) <= 0) {
				/* magic item is used up */
				GUI_output(get_ttx(638));
			} else {
				/* special item */
#if !defined(__BORLANDC__)
				func = handler[ds_readbs((SPECIALITEMS_TABLE + 2) + 3 * host_readbs(Real2Host(ds_readd(USED_ITEM_DESC)) + 4))];
#else
				func = (void (*)(void))ds_readd(USE_SPECIAL_ITEM_HANDLERS + 4 * ds_readbs((SPECIALITEMS_TABLE + 2) + 3 * host_readbs(Real2Host(ds_readd(USED_ITEM_DESC)) + 4)));
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
	b1_index = ds_readws(TX_FILE_INDEX);

	/* load SPELLTXT*/
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	ds_writed(SPELLUSER, ds_readd(ITEMUSER));

	/* ask who should be affected */
	host_writeb(get_spelluser() + HERO_ENEMY_ID,
		select_hero_from_group(get_ttx(637)) + 1);

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) > 0) {
		/* use it */
		spell_arcano();
		/* decrement usage counter */
		dec_ptr_ws(get_itemuser() + (HERO_INVENTORY + INVENTORY_QUANTITY) + ds_readws(USED_ITEM_POS) * SIZEOF_INVENTORY);
	}

	if ((b1_index != -1) && (b1_index != 0xde)) {
		/* need to reload buffer1 */
		load_tx(b1_index);
	}
}

/* Borlandified and identical */
void item_read_recipe(void)
{
	Bit8u *str;

	switch (ds_readws(USED_ITEM_ID)) {
	case 0xa7: str = get_ttx(639); break;
	case 0xa9: str = get_ttx(640); break;
	case 0xca: str = get_ttx(649); break;
	case 0xcb: str = get_ttx(650); break;
	case 0xcc: str = get_ttx(651); break;
	case 0xcd: str = get_ttx(652); break;
	case 0xce: str = get_ttx(653); break;
	case 0xba: str = get_ttx(681); break;
	case 0xf0: str = get_ttx(682); break;
	case 0xf2: str = get_ttx(684); break;
	case 0xf3: str = get_ttx(685); break;
	case 0xf4: str = get_ttx(686); break;
	case 0xf1: str = get_ttx(683); break;
	}

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_ttx(636),
		(char*)str);

	GUI_output(Real2Host(ds_readd(DTP2)));
}

/* Borlandified and identical */
void item_read_document(void)
{
	Bit8u *str;
	signed short textbox_width_bak;

	switch (ds_readws(USED_ITEM_ID)) {
	case 0xaa: str = get_ttx(641); break;
	case 0xbb: str = get_ttx(645); break;
	case 0xbd: str = get_ttx(646); break;
	case 0xbe: str = get_ttx(647); break;
	case 0xc0: str = get_ttx(648); break;
	case 0xde: str = get_ttx(654); break;
	case 0xe0: str = get_ttx(655); break;
	case 0xeb: str = get_ttx(657); break;
	case 0xf7: str = get_ttx(759); break;
	}

	textbox_width_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);
	GUI_output(str);
	ds_writew(TEXTBOX_WIDTH, textbox_width_bak);
}

/* Borlandified and identical */
void item_armatrutz(void)
{
	/* ID 171 = ITEM_CORONET_SILVER, 245 = ITEM_CORONET_GREEN */
	signed short b1_index;

	/* save index off buffer1 */
	b1_index = ds_readws(TX_FILE_INDEX);

	/* load SPELLTXT */
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	ds_writed(SPELLUSER, ds_readd(ITEMUSER));

	/* ask who should be affected */
	host_writeb(get_spelluser() + HERO_ENEMY_ID,
		select_hero_from_group(get_ttx(637)) + 1);

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) > 0) {
		/* use it */
		spell_armatrutz();
		/* decrement usage counter */
		dec_ptr_ws(get_itemuser() + (HERO_INVENTORY + INVENTORY_QUANTITY) + ds_readws(USED_ITEM_POS) * SIZEOF_INVENTORY);

		GUI_output(Real2Host(ds_readd(DTP2)));
	}

	if ((b1_index != -1) && (b1_index != 0xde)) {
		/* need to reload buffer1 */
		load_tx(b1_index);
	}
}

/* Borlandified and identical */
void item_flimflam(void)
{
	/* ID 174 = ITEM_AMULET_GREEN */
	signed short b1_index;

	/* save index off buffer1 */
	b1_index = ds_readws(TX_FILE_INDEX);

	/* load SPELLTXT*/
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	ds_writed(SPELLUSER, ds_readd(ITEMUSER));

	spell_flimflam();

	/* decrement usage counter */
	dec_ptr_ws(get_itemuser() + (HERO_INVENTORY + INVENTORY_QUANTITY) + ds_readws(USED_ITEM_POS) * SIZEOF_INVENTORY);

	if ((b1_index != -1) && (b1_index != 0xde)) {
		/* need to reload buffer1 */
		load_tx(b1_index);
	}

	GUI_output(Real2Host(ds_readd(DTP2)));

}

/* Borlandified and identical */
void item_debtbook(void)
{
	/* DEBTBOOK, ID 176 */
	if (ds_readbs(DEBTBOOK_READ_FLAG) != 0) {

		/* mark this event */
		ds_writeb(DEBTBOOK_READ_FLAG, 0);

		/* 15 AP */
		add_hero_ap_all(15);

		/* mark informer Hjore as known */
		ds_writeb_z(INFORMER_FLAGS + INFORMER_HJORE, 1);
	}

	GUI_output(get_ttx(642));
}

/* Borlandified and identical */
void item_orcdocument(void)
{
	/* ORCDOCUMENT, ID 179 */

	/* Languages + 4, or already read successful */
	if ((test_skill(get_itemuser(), TA_SPRACHEN, 4) > 0) || (ds_readb(ORCDOCUMENT_READ_FLAG) != 0)) {

		if (!ds_readbs(ORCDOCUMENT_READ_FLAG)) {
			add_group_ap(20);
			ds_writeb(ORCDOCUMENT_READ_FLAG, 1);
		}

		GUI_output(get_ttx(644));
	} else {
		GUI_output(get_ttx(643));
	}
}

/* Borlandified and identical */
void item_weapon_poison(void)
{
	/*	WEAPON POISON, EXPURGICUM, VOMICUM
		ID 55-59, 141-144, 166, 168 */

	signed short bottle;

	if ((host_readws(get_itemuser() + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY) != ITEM_NONE) &&
		(host_readws(get_itemuser() + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY) != ITEM_SHORTBOW) &&
		(host_readws(get_itemuser() + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY) != ITEM_LONGBOW) &&
		(host_readws(get_itemuser() + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY) != ITEM_CROSSBOW))
		/* TODO: potential Original-Bug: What about sling? */
	{

		switch (ds_readws(USED_ITEM_ID)) {
		case ITEM_VOMICUM : {
			/* VOMICUM */
			or_ptr_bs(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_FLAGS), 0x40); /* set 'poison_vomicum' flag */

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_VOMICUM), 1);

			bottle = ITEM_FLASK_BRONZE;
			break;
		}
		case ITEM_EXPURGICUM : {
			/* EXPURGICUM */
			or_ptr_bs(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_FLAGS), 0x20); /* set 'poison_expurgicum' flag */

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_EXPURGICUM), 1);

			bottle = ITEM_FLASK_BRONZE;
			break;
		}
		case ITEM_SHURIN_POISON: {
			/* SHURIN-BULB POISON / SHURINKNOLLENGIFT */
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE), POISON_TYPE_SHURINKNOLLENGIFT);
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_NR_POISON_CHARGES), 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_SHURIN_POISON), 1);
			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_ARAX_POISON: {
			/* ARAX POISON / ARAXGIFT */
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE), POISON_TYPE_ARAX);
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_NR_POISON_CHARGES), 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_ARAX_POISON), 1);
			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_ANGST_POISON: {
			/* FEAR POISON / ANGSTGIFT */
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE), POISON_TYPE_ANGSTGIFT);
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_NR_POISON_CHARGES), 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_ANGST_POISON), 1);
			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_SLEEP_POISON: {
			/* SLEPPING POISON / SCHALFGIFT */
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE), POISON_TYPE_SCHLAFGIFT);
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_NR_POISON_CHARGES), 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_SLEEP_POISON), 1);
			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_GOLDLEIM: {
			/* GOLDEN GLUE / GOLDLEIM */
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE), POISON_TYPE_GOLDLEIM);
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_NR_POISON_CHARGES), 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_GOLDLEIM), 1);
			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_LOTUS_POISON: {
			/* LOTUS POISON / LOTUSGIFT */
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE), POISON_TYPE_LOTUSGIFT);
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_NR_POISON_CHARGES), 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_LOTUS_POISON), 1);
			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_KUKRIS: {
			/* KUKRIS */
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE), POISON_TYPE_KUKRIS);
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_NR_POISON_CHARGES), 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_KUKRIS), 1);
			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_BANNSTAUB: {
			/* BANE DUST / BANNSTAUB */
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE), POISON_TYPE_BANNSTAUB);
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_NR_POISON_CHARGES), 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_BANNSTAUB), 1);
			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_KROETEN_POISON: {
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE), POISON_TYPE_KROETENSCHEMEL);
			host_writeb(get_itemuser() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_NR_POISON_CHARGES), 5);
			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_KROETEN_POISON), 1);
			bottle = ITEM_FLASK_GLASS;
			break;
		}
		}

		give_hero_new_item(get_itemuser(), bottle, 1, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(739),
			(char*)Real2Host(GUI_names_grammar((signed short)0x8000, host_readws(get_itemuser() + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID), 0)));
	} else {
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(805),
			(char*)get_itemuser() + HERO_NAME2);
	}

	GUI_output(Real2Host(ds_readd(DTP2)));
}

void item_myastmatic(void)
{
	/* MYASTMATIC, ID 238 */
	if (ds_readws(IN_FIGHT) == 0) {
		GUI_output(get_ttx(687));
		return;
	}
}

void item_hylailic(void)
{
	/* HYLAILIC FIRE, ID 239 */
	if (ds_readws(IN_FIGHT) == 0) {
		GUI_output(get_ttx(687));
		return;
	}
}

void item_magic_book(void)
{
	/* BOOK, ID 246 */

	/* print message */
	GUI_output(get_ttx(749));

	/* Heptagon +2 */
	add_ptr_bs(get_itemuser() + (HERO_SP_DEMON + 3), 2);

	/* drop the book */
	drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_BOOK_HEPTAGON), 1);
}

/* Borlandified and identical */
void item_brenne(void)
{
	/*	LANTERN, TORCH, TINDERBOX, LANTERN
		ID 25, 65, 85, 249 */
	signed short b1_index;
	signed short pos;
	signed short refill_pos;

	/* save index off buffer1 */
	b1_index = ds_readws(TX_FILE_INDEX);

	/* load SPELLTXT*/
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	if (ds_readws(USED_ITEM_ID) == ITEM_LANTERN_ON) {
		/* refill burning lantern */

#ifdef M302de_ORIGINAL_BUGFIX
		if (get_spelluser() != get_itemuser()) {
			ds_writed(SPELLUSER, ds_readd(ITEMUSER));
		}
#endif

		/* look for oil at the spelluser() */
		pos = get_item_pos(get_spelluser(), ITEM_OIL);

		if (pos != -1) {
			/* look for the burning lantern at the spelluser() ??? */
			refill_pos = get_item_pos(get_spelluser(), ITEM_LANTERN_ON);

			/* reset the burning time of the lantern */
			host_writeb(get_itemuser() + (HERO_INVENTORY + INVENTORY_LIGHTING_TIMER) + refill_pos * SIZEOF_INVENTORY, 100);

			/* drop the oil */
			drop_item(get_itemuser(), pos, 1);

			/* give a bronze flask */
			give_hero_new_item(get_itemuser(), ITEM_FLASK_BRONZE, 0, 1);

			/* prepare message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(119),
				(char*)get_itemuser() + HERO_NAME2);
		} else {
			/* prepare message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(120),
				(char*)get_itemuser() + HERO_NAME2);
		}
	} else {

		if (get_item_pos(get_itemuser(), ITEM_TINDERBOX) == -1) {
			/* No tinderbox */
			/* prepare message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(122),
				(char*)get_itemuser() + HERO_NAME2);
		} else {

			if (ds_readws(USED_ITEM_ID) == ITEM_TORCH_OFF) {
				/* TORCH */
				ds_writew(LIGHT_TYPE, 1);
			} else if (ds_readws(USED_ITEM_ID) == ITEM_LANTERN_OFF) {
				/* LANTERN */
				ds_writew(LIGHT_TYPE, 2);
			} else {
				ds_writew(LIGHT_TYPE, 0);
			}

			ds_writed(SPELLUSER, ds_readd(ITEMUSER));

			spell_brenne();
		}
	}

	if ((b1_index != -1) && (b1_index != 0xde)) {
		/* need to reload buffer1 */
		load_tx(b1_index);
	}

	GUI_output(Real2Host(ds_readd(DTP2)));
}

/* Borlandified and identical */
void item_bag(void)
{
	/*	MAGIC BREADBAG, BAG
		ID 184, 221 */

	Bit8u *ptr;

	if ((ds_readbs(DUNGEON_INDEX) == 7) && (ds_readbs(DUNGEON_LEVEL) == 0)) {
		/* set ptr to the map */
		ptr = p_datseg + DNG_MAP;

		/* remove the wall there */
		host_writeb(ptr + 0x3a, 1);
	}
#if !defined(__BORLANDC__)
	else {
		D1_INFO("WARNUNG:\tDer BEUTEL wurde nicht im ersten Level der Magierruine geoeffnet!\n");
		D1_INFO("\t\tEventuell kann das Spiel nicht mehr erfolgreich beendet werden.\n");
	}
#endif

	/* print message */
	GUI_output(get_ttx(775));

	/* drop the BAG */
	drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_BAG), 1);
}

#if !defined(__BORLANDC__)
}
#endif
