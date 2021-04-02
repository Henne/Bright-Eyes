/*
 *      Rewrite of DSA1 v3.02_de functions of seg105 (inventory)
 *      Functions rewritten: 14/14 (complete)
 *      Functions called rewritten 13/13
 *      Functions uncalled rewritten 1/1
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg105.cpp
 *
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg007.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg105.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   TODO
 *
 * \param   hero        the hero
 * \param   item        the item which gets unequipped
 * \param   pos         the position of the item
 */
void unequip(Bit8u *hero, unsigned short item, unsigned short pos)
{

	Bit8u *item_p;

	/* unequip of item 0 is not allowed */
	if (item == 0)
		return;

	item_p = get_itemsdat(item);

	/* if item is an armor ? */
	if (item_armor(item_p)) {

		sub_ptr_bs(hero + HERO_RS_BONUS1, ds_readbs(host_readbs(item_p + 4) * 2 + 0x877));

		add_ptr_bs(hero + HERO_RS_BONUS1, host_readbs(hero + HERO_ITEM_HEAD + 7 + pos * 14));

		sub_ptr_bs(hero + HERO_RS_BE, ds_readbs(host_readbs(item_p + 4) * 2 + 0x878));
	}
	/* if item is a weapon and in the right hand ? */
	if (item_weapon(item_p) && pos == 3) {
		host_writebs(hero + HERO_WP_CLASS, 0);
		host_writebs(hero + HERO_AT_MOD, host_writebs(hero + HERO_PA_MOD, 0));
	}
	/* unequip Kraftguertel KK - 5 */
	if (item == ITEM_GIRDLE_MIGHT)
		host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), host_readb(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) - 5);
	/* unequip Helm CH + 1 (cursed) */
	if (item == ITEM_HELMET)
		inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_CH));
	/* unequip Silberschmuck TA + 1 */
	if (item == ITEM_JEWELRY_SILVER)
		host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA), host_readb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA)) + 2);
	/* unequip Stirnreif or Ring MR - 2 */
	if (item == ITEM_CORONET_BLUE || item == ITEM_RING_RED)
		host_writeb(hero + HERO_MR, host_readb(hero + HERO_MR) - 2);
	/* unequip Totenkopfguertel TA + 4 */
	if (item == ITEM_BELT_SKULL)
		host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA), host_readb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA)) + 4);
	/* unequip Kristallkugel Gefahrensinn - 2 */
	if (item == ITEM_BALL_CRYSTAL)
		host_writeb(hero + HERO_TA_INTUITION, host_readb(hero + HERO_TA_INTUITION) - 2);
}


/**
 * \brief   account boni of special items when equipped
 *
 * \param   owner       the owner of the item
 * \param   equipper    the one who equips the item
 * \param   item        the item ID
 * \param   pos_i       the position in the inventory of the owner
 * \param   pos_b       the position in the inventory of the equipper
 */
void add_equip_boni(Bit8u *owner, Bit8u *equipper, signed short item, signed short pos_i, signed short pos_b)
{
	Bit8u *item_p;

	if (item) {
		/* calculate pointer to item description */
		item_p = Real2Host(ds_readd(ITEMSDAT)) + item * 12;

		/* armor and shield */
		if (item_armor(item_p)) {

			/* add RS boni */
			add_ptr_bs(equipper + HERO_RS_BONUS1, ds_readbs(ARMORS_TABLE + host_readbs(item_p + 4) * 2));

			/* subtract used item value */
			sub_ptr_bs(equipper + HERO_RS_BONUS1, host_readbs(owner + HERO_ITEM_HEAD + 7 + pos_i * 14));

			/* add RS-BE */
			add_ptr_bs(equipper + HERO_RS_BE, ds_readbs(ARMORS_TABLE + 1 + host_readbs(item_p + 4) * 2));

		}

		/* weapon right hand */
		if (item_weapon(item_p) && (pos_b == 3)) {

			/* set weapon type */
			host_writeb(equipper + HERO_WP_CLASS, host_readb(item_p + 3));

			/* set AT */
			host_writeb(equipper + HERO_AT_MOD,
				ds_readb(WEAPONS_TABLE + 5 + host_readbs(item_p + 4) * 7));

			/* set PA */
			host_writeb(equipper + HERO_PA_MOD,
				ds_readb(WEAPONS_TABLE + 6 + host_readbs(item_p + 4) * 7));

		}

		/* Girdle of might / Kraftguertel */
		if (item == ITEM_GIRDLE_MIGHT) {
			/* KK + 5 */
			host_writeb(equipper + (HERO_ATTRIB + 3 * ATTRIB_KK),
				host_readb(equipper + (HERO_ATTRIB + 3 * ATTRIB_KK)) + 5);
		}

		/* Helmet / Helm */
		if (item == ITEM_HELMET) {
			/* dec CH */
			dec_ptr_bs(equipper + (HERO_ATTRIB + 3 * ATTRIB_CH));
		}

		/* Silver Jewelry / Silberschmuckstueck (magisch) */
		if (item == ITEM_JEWELRY_SILVER) {
			/* TA - 2 */
			host_writeb(equipper + (HERO_ATTRIB + 3 * ATTRIB_TA),
				host_readbs(equipper + (HERO_ATTRIB + 3 * ATTRIB_TA)) - 2);
		}

		/* Coronet or Ring / Stirnreif oder Ring */
		if (item == ITEM_CORONET_BLUE || item == ITEM_RING_RED) {
			/* MR + 2 */
			host_writeb(equipper + HERO_MR,
				host_readb(equipper + HERO_MR) + 2);
		}

		/* Skull belt / Totenkopfguertel */
		if (item == ITEM_BELT_SKULL) {

			/* TA - 4 */
			host_writeb(equipper + (HERO_ATTRIB + 3 * ATTRIB_TA),
				host_readbs(equipper + (HERO_ATTRIB + 3 * ATTRIB_TA)) - 4);

			if (ds_readb(PP20_INDEX) == ARCHIVE_FILE_ZUSTA_UK) {
				equip_belt_ani();
			}
		}

		/* Crystal ball / Kristalkugel */
		if (item == ITEM_BALL_CRYSTAL) {

			/* Sinnesschaerfe + 2 */
			host_writeb(equipper + HERO_TA_INTUITION,
				host_readb(equipper + HERO_TA_INTUITION) + 2);
		}
	}
}

/**
 * \brief   checks if a hero can use an item
 *
 * \param   hero        the hero
 * \param   item        the item
 * \return              1 if the hero can use the item and 0 if not.
 */
unsigned short can_hero_use_item(Bit8u *hero, unsigned short item)
{

#if !defined(__BORLANDC__)
	/* some new error check */
	if (!host_readbs(hero + HERO_TYPE))
		D1_ERR("Warning: %s() typus == 0\n", __func__);
#endif

	/* calculate the address of the class forbidden items array */
	if (is_in_word_array(item, (signed short*)Real2Host(ds_readd((WEARABLE_ITEMS_INDEX - 4) + host_readbs(hero + HERO_TYPE) * 4))))
		return 0;
	else
		return 1;
}

/**
 * \brief   checks if an item is equipable at a body position
 *
 * \param   item        the item
 * \param   pos         ths position at the body
 * \return              1 if equipping is possible or 0 if not.
 */
unsigned short can_item_at_pos(unsigned short item, unsigned short pos)
{

	Bit8u *item_p;

	item_p = get_itemsdat(item);

	/* if item is an armor ? */
	if (item_armor(item_p)) {

		/* can be weared on the head */
		if ((pos == 0 && host_readb(item_p + 3) == 0) ||
			/* can be weared on the torso */
			(pos == 2 && host_readb(item_p + 3) == 2) ||
			/* can be weared at the feet */
			(pos == 6 && host_readb(item_p + 3) == 6) ||
			/* can be weared at the arms */
			(pos == 1 && host_readb(item_p + 3) == 1) ||
			/* can be weared at the legs */
			(pos == 5 && host_readb(item_p + 3) == 5) ||
			/* can be weared at the left hand */
			(pos == 4 && host_readb(item_p + 3) == 9)) {
				return 1;
		} else {
			return 0;
		}
	} else {

		/* Stirnreif (3 types) can be weared at the head */
		if ((item == ITEM_CORONET_BLUE || item == ITEM_CORONET_SILVER || item == ITEM_CORONET_GREEN)
			&& (pos == 0))
		{
			return 1;
		}

		/* you can take everything else in the hands */

		if ((pos != 3) && (pos != 4)) {
			return 0;
		}

	}

	return 1;
}

/**
 * \brief   returns the position of an equipped item
 *
 * \param   hero        the hero
 * \param   item        the item
 * \return              the position of item, if equipped, otherwise -1.
 * Is not used in the game.
 */
signed short has_hero_equipped(Bit8u *hero, unsigned short item)
{
	signed short i;

	for (i = 0; i < 7; i++)
		if (host_readw(hero + HERO_ITEM_HEAD + i * 14) == item)
			return i;

	return -1;
}

/**
 * \brief   returns the posotion of a non-full item stack
 *
 * \param   hero        the hero
 * \param   item        the item
 * \return              the postition of a non-full (<99) item stack or -1 if
 * the hero doesn't own this item or has only full stacks of them.
 */
//static
signed short has_hero_stacked(Bit8u *hero, unsigned short item)
{
	signed short i;

	for (i = 0; i < 23; i++) {
		/* has the hero the item */
		/* is the number of items < 99 */
		if ((host_readw(hero + HERO_ITEM_HEAD + i * 14) == item) &&
			 (host_readws(hero + HERO_ITEM_HEAD + 2 + i * 14) < 99))
			return i;
	}

	return -1;
}

/**
 * \brief   generates an item and gives it to hero
 *
 *          The mode parameter works that way: If the weight the hero carries
 *          is greater than KK*10 mode 0 and 1 will not give the hero that item.
 *          The difference is, that mode = 1 prints a warning, mode = 0 is quiet.
 *          mode = 2 ignores the carry weight completely.
 *
 * \param   hero        the hero who should get the item
 * \param   item        id of the item
 * \param   mode        0 = quiet / 1 = warn / 2 = ignore
 * \param   no          number of item the hero should get
 */
signed short give_hero_new_item(Bit8u *hero, signed short item, signed short mode, signed short no)
{
	signed short l1;
	signed short retval;
	signed short done;
	Bit8u *item_p;
	signed short si, di;

	si = no;

	retval = 0;

	/* check if hero can carry that item */
	if ((mode != 2) &&
		(host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 100 <= host_readws(hero + HERO_LOAD))) {

		if (mode != 0) {
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(779),
				(char*)(hero + HERO_NAME2));
			GUI_output(Real2Host(ds_readd(DTP2)));
		}
	} else {
		item_p = get_itemsdat(item);

		/* hero has a non-full stack of this item */
		if (item_stackable(item_p) &&
			(l1 = has_hero_stacked(hero, item)) != -1) {


			/* check for space on existing stack */
			if (host_readws(hero + HERO_ITEM_HEAD + 2 + l1 * 14) + si > 99) {
				si = 99 - host_readw(hero + HERO_ITEM_HEAD + 2 + l1 * 14);
			}

			/* add items to stack */
			add_ptr_ws(hero + HERO_ITEM_HEAD + 2 + l1 * 14, si);

			/* add weight */
			add_ptr_ws(hero + HERO_LOAD, (host_readws(item_p + 5) * si));

			retval = si;
		} else {

			/* Original-Bug: may lead to problems when the
				item counter is broken */
			if (host_readbs(hero + HERO_KS_TAKEN) < 23) {

				done = 0;

				do {
				/* Original-Bug: may lead to problems when the
					item counter is broken */
					if (host_readbs(hero + HERO_KS_TAKEN) < 23) {
						/* look for a free place : tricky */
						di = 6;
						while ((host_readw(hero + HERO_ITEM_HEAD + ++di * 14) != 0) && (di < 23));

						if (di < 23) {
							if (si > 99)
								si = 99;
							/* increment item counter */
							inc_ptr_bs(hero + HERO_KS_TAKEN);

							/* write item id */
							host_writew(hero + HERO_ITEM_HEAD + di * 14, item);


#if 1
							host_writew(hero + HERO_ITEM_HEAD + 2 + di * 14,
								(item_stackable(item_p)) ? si :
									(item_useable(item_p)) ?
										ds_readbs((SPECIALITEMS_TABLE + 1) + host_readbs(item_p + 4) * 3): 0);
#else

							/* write item counter */
							if (item_stackable(item_p))
								/* write stackable items */
								host_writew(hero + HERO_ITEM_HEAD + 2 + di * 14, si);
							else if (item_useable(item_p))
									/* unknown */
									host_writew(hero + HERO_ITEM_HEAD + 2 + di * 14,
										ds_readbs((SPECIALITEMS_TABLE + 1) + host_readbs(item_p + 4) * 3));
								 else
									host_writew(hero + HERO_ITEM_HEAD + 2 + di * 14, 0);
#endif

							/* set magical flag */
							if (host_readb(item_p + 0xb) != 0) {
								or_ptr_bs(hero + HERO_ITEM_HEAD + 4 + di * 14, 0x8);

#if !defined(__BORLANDC__)
								D1_INFO("%s hat soeben einen magischen Gegenstand erhalten: %s\n",
									(char*)hero + HERO_NAME2, get_itemname(item));
#endif
							}

							/* set breakfactor */
							if (item_weapon(item_p)) {
								host_writeb(hero + HERO_ITEM_HEAD + 6 + di * 14,
									ds_readb(WEAPONS_TABLE + 3 + host_readbs(item_p + 4) * 7));
							}

							/* adjust weight */
							if (item_stackable(item_p)) {
								/* add stackable items weight */
								add_ptr_ws(hero + HERO_LOAD,
									(host_readws(item_p + 5) * si));
								retval = si;
								si = 0;
							} else {
								/* add single item weight */
								add_ptr_ws(hero + HERO_LOAD,
									host_readws(item_p + 5));
								si--;
								retval++;
							}

							/* all items are distributed */
							if (si == 0)
								done = 1;

							/* special items */
							if (item == ITEM_SICKLE) {
								host_writeb(hero + (HERO_TA_NATURE+3), host_readb(hero + (HERO_TA_NATURE+3)) + 3);
							}
							if (item == ITEM_AMULET_BLUE) {
								host_writeb(hero + HERO_MR, host_readb(hero + HERO_MR) + 5);
							}

						} else {
							done = 1;
						}
					} else {
						done = 1;
					}
				} while (done == 0);
			}
		}
	}

	return retval;
}

/**
 * \brief   checks if Ingerimm accepts this item as sacrifice
 *
 * \param   item        the item
 */
//static
unsigned short item_pleasing_ingerimm(unsigned short item)
{

	Bit8u *p_item;

	p_item = get_itemsdat(item);

	if (item_weapon(p_item) && (host_readb(p_item + 3) == 4))
		return 1;

	if (item_armor(p_item) && (ds_readbs(host_readbs(p_item + 4) * 2 + ARMORS_TABLE) > 1))
		return 1;

	return 0;
}

/**
 * \brief   tries to drop an item
 *
 * \param   hero        pointer to the hero
 * \param   pos         position of the item to be dropped
 * \param   no          number of stacked items to be dropped / -1 to ask
 * \return              true if the item has been dropped or false if not
 *
 *	TODO: This function can be tuned a bit
 */
unsigned short drop_item(Bit8u *hero, signed short pos, signed short no)
{

	Bit8u *p_item;
	signed short answer;
	unsigned short retval = 0;
	signed short item;

	item = host_readws(hero + HERO_ITEM_HEAD + pos * 14);

	/* check if that item is valid */
	if (item != 0) {

		p_item = get_itemsdat(item);

		if (item_undropable(p_item)) {
			/* this item is not droppable */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(454),
				(char*)Real2Host(GUI_names_grammar((signed short)0x8002, item, 0)));

			GUI_output(Real2Host(ds_readd(DTP2)));
		} else {
			/* this item is droppable */

			if (item_stackable(p_item)) {
				if (no == -1) {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(219),
						(char*)Real2Host(GUI_names_grammar(6, item, 0)));

					do {
						answer = GUI_input(Real2Host(ds_readd(DTP2)), 2);
					} while (answer < 0);

					no = answer;
				}

				if (host_readws(hero + HERO_ITEM_HEAD + 2 + pos * 14) > no) {
					/* remove some stacked items */

					/* adjust stack counter */
					sub_ptr_ws(hero + HERO_ITEM_HEAD + 2 + pos * 14, no);
					/* adjust weight */
					sub_ptr_ws(hero + HERO_LOAD, host_readws(p_item + 5) * no);
				} else {
					/* remove all stacked items */

					/* adjust weight */
					sub_ptr_ws(hero + HERO_LOAD,
						host_readws(p_item + 5) * host_readws(hero + HERO_ITEM_HEAD + 2 + pos * 14));
					/* decrement item counter */
					dec_ptr_bs(hero + HERO_KS_TAKEN);

					/* clear the inventory pos */
					memset(hero + HERO_ITEM_HEAD + pos * 14, 0, 14);
				}

				retval = 1;
			} else {
				if (!(no != -1 || GUI_bool(get_ttx(220)))) {
				} else {

					/* check if item is equipped */
					if (pos < 7)
						unequip(hero, item, pos);

					/* decrement item counter */
					dec_ptr_bs(hero + HERO_KS_TAKEN);

					/* subtract item weight */
					sub_ptr_ws(hero + HERO_LOAD, host_readws(p_item + 5));

					/* check special items */
					/* item: SICHEL Pflanzenkunde -3 */
					if (item == ITEM_SICKLE) {
						host_writeb(hero + (HERO_TA_NATURE+3),
							host_readbs(hero + (HERO_TA_NATURE+3)) - 3);
					}

					/* item:  AMULETT MR -5 */
					if (item == ITEM_AMULET_BLUE) {
						host_writeb(hero + HERO_MR,
							host_readbs(hero + HERO_MR) - 5);
					}

					/* clear the inventory pos */
					memset(hero + HERO_ITEM_HEAD + pos * 14, 0, 14);
					retval = 1;
				}
			}
		}

		/* check for the piratecave to bring efferd a gift */
		if ((item == ITEM_TRIDENT || item == ITEM_NET) && ds_readb(DUNGEON_INDEX) == 11 &&
			ds_readw(X_TARGET) == 9 && ds_readw(Y_TARGET) == 9)
		{
			ds_writeb(DNG11_EFFERD_SACRIFICE, 1);
		}

		/* check for the mine in oberorken to bring ingerimm a gift */
		if (item_pleasing_ingerimm(item) && ds_readb(DUNGEON_INDEX) == 12 &&
			ds_readw(X_TARGET) == 2 && ds_readw(Y_TARGET) == 14 &&
			ds_readb(DUNGEON_LEVEL) == 1)
		{
			ds_writeb(DNG12_INGERIMM_SACRIFICE, 1);
		}
	}

	return retval;
}

/**
 * \brief   gives one item to the party
 *
 * \param   id          ID of the item
 * \param   unused      unused parameter
 * \param   no          number of items
 * \return              the number of given items.
 */
signed short get_item(signed short id, signed short unused, signed short no)
{
	signed short i;
	signed short retval = 0;
	signed short v6;
	signed short done = 0;
	signed short dropper;
	signed short vc;
	Bit8u * hero_i;
	signed short autofight_bak;

	/* Special stacked items */
	if (id == ITEM_200_ARROWS) { id = ITEM_ARROWS; no = 200;} else
	if (id == ITEM_50_BOLTS) { id = ITEM_BOLTS; no = 50;} else
	if (id == ITEM_20_CLIMBING_HOOKS) { id = ITEM_CLIMBING_HOOKS; no = 20;}

	do {
		hero_i = get_hero(0);
		for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
			if (host_readb(hero_i + HERO_TYPE) &&
				host_readb(hero_i + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP))
			{

				while ((no > 0) && (v6 = give_hero_new_item(hero_i, id, 0, no)) > 0) {
					no -= v6;
					retval += v6;
				}
			}
		}

		if (no > 0) {
			autofight_bak = ds_readws(AUTOFIGHT);
			ds_writew(AUTOFIGHT, 0);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(549),
				(char*)Real2Host(GUI_names_grammar(((no > 1) ? 4 : 0) + 2, id, 0)));

			if (GUI_bool(Real2Host(ds_readd(DTP2)))) {

				dropper = select_hero_ok(get_ttx(550));

				if (dropper != -1) {
					hero_i = get_hero(dropper);
					ds_writeb(PREVENT_DROP_EQUIPPED_ITEMS, 1);
					vc = select_item_to_drop(hero_i);
					ds_writeb(PREVENT_DROP_EQUIPPED_ITEMS, 0);

					if (vc != -1) {
						drop_item(hero_i, vc, -1);
					}
				}
			} else {
				done = 1;
			}
			ds_writew(AUTOFIGHT, autofight_bak);
		} else {
			done = 1;
		}
	} while (done == 0);

	return retval;
}

/**
 * \brief   returns how many items of one type the hero has
 *
 * \param   hero        the hero
 * \param   item        the item
 */
signed short hero_count_item(Bit8u *hero, unsigned short item) {

	signed short i;
	unsigned short ret = 0;

	for (i = 0; i < 23; i++)
		if (host_readw(hero + HERO_ITEM_HEAD + i * 14) == item)
			ret++;

	return ret;
}

/**
 * \brief   count the number of items of one type the current group has
 *
 * \param   item        the item-ID
 * \return              the number of items
 */
signed short group_count_item(signed short item)
{

	Bit8u *hero_i;
	signed short i;
	signed short ret = 0;

	hero_i = get_hero(0);
	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
		/* check class */
		if (host_readb(hero_i + HERO_TYPE) &&
			/* check group */
			(host_readb(hero_i + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP))) {

			ret += hero_count_item(hero_i, item);
		}
	}

	return ret;
}

/**
 * \brief   a hero will loose an item
 *
 * \param   hero        the hero
 * \param   percent     probability to loose
 * \param   text        the displayed text
 */
void loose_random_item(Bit8u *hero, signed short percent, Bit8u *text)
{
	Bit8u *p_item;
	unsigned short item, pos;

	if (random_schick(100) > percent)
		return;

	/* Original-Bug: infinite loop if the hero has no item */
	do {
		pos = random_schick(23) - 1;

		item = host_readw(hero + HERO_ITEM_HEAD + pos * 14);

		p_item = get_itemsdat(item);

		/* No item to drop */
		if (item != 0 && !item_undropable(p_item)) {

			/* drop 1 item */
			drop_item(hero, pos, 1);

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)text, hero + HERO_NAME2,
				Real2Host(GUI_names_grammar(0, item, 0)));

			GUI_output(Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			return;
		}
	} while (1);
}

signed short select_item_to_drop(Bit8u *hero)
{
	signed short i;
	signed short v4 = 0;
	signed short v6 = 0;
	signed short item;
	signed short va;
	signed short textbox_width_bak, bak2, bak3;
	RealPt ptr;
	signed short str[23];
	signed short di;

	/* check if we drop equipped items or not */
	i = (ds_readb(PREVENT_DROP_EQUIPPED_ITEMS) != 0) ? 7 : 0;
	for (; i < 23; i++) {
		if ((item = host_readws(hero + HERO_ITEM_HEAD + i * 14))) {
			str[v6] = i;
			ds_writed(RADIO_NAME_LIST + v6 * 4 , (Bit32u)((RealPt)ds_readd(DTP2) + v6 * 30));
			strcpy((char*)Real2Host(ds_readd(RADIO_NAME_LIST + v6 * 4)),
				(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(item))));
			v6++;
		}
	}

	if (v6 == 0) {
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(750),
			(char*)(hero + HERO_NAME2));
		GUI_output(Real2Host(ds_readd(DTP2)));
		return -1;
	}
	di = 0;
	while (v4 != -1) {
		va = -1;
		if (v6 > 12) {
			if (!di) {
				i = 13;
				va = i - 1;
				ptr = (RealPt)ds_readd(RADIO_NAME_LIST + 4 * va);
				ds_writed(RADIO_NAME_LIST + 4 * va,
					host_readd(Real2Host(ds_readd(TEXT_LTX_INDEX)) + 0xbbc));
			} else {
				i = v6 + 1;
				va = i - 1;
				ptr = (RealPt)ds_readd(RADIO_NAME_LIST + 4 * va);
				ds_writed(RADIO_NAME_LIST + 4 * va,
					host_readd(Real2Host(ds_readd(TEXT_LTX_INDEX)) + 0xbbc));
				i -= di;
			}
		} else {
			i = v6;
		}
		textbox_width_bak = ds_readw(TEXTBOX_WIDTH);
		bak2 = ds_readw(BASEPOS_X);
		bak3 = ds_readw(BASEPOS_Y);
		ds_writew(TEXTBOX_WIDTH, 6);
		ds_writew(BASEPOS_X, ds_writew(BASEPOS_Y, 0));
		v4 = GUI_radio((Bit8u*)get_ttx(752), (signed char)i,
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x00 + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x04 + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x08 + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x0c + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x10 + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x14 + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x18 + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x1c + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x20 + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x24 + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x28 + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x2c + di * 4)),
			Real2Host(ds_readd(RADIO_NAME_LIST + 0x30 + di * 4)));
		ds_writew(TEXTBOX_WIDTH, textbox_width_bak);
		ds_writew(BASEPOS_X, bak2);
		ds_writew(BASEPOS_Y, bak3);

		if (va != -1) {
			ds_writed(RADIO_NAME_LIST + 0x00 + va * 4, (Bit32u)ptr);
		}
		if ((v6 > 12) && (v4 == i)) {
			di += 12;
			if (di > v6) {
				di = 0;
			}
		} else {
			if (v4 != -1) {
				return str[di + v4 - 1];
			}
		}
	}

	return -1;
}

#if !defined(__BORLANDC__)
}
#endif
