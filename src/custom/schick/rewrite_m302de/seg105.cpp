/*
 *      Rewrite of DSA1 v3.02_de functions of seg105 (inventory)
 *      Functions rewritten 12/14
 *
 *      Functions called rewritten 11/13
 *      Functions uncalled rewritten 1/1
*/

#include <string.h>
#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"

#include "seg007.h"
#include "seg096.h"
#include "seg097.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 *	@hero:	the hero
 *	@item:	the item which gets unequipped
 *	@pos:	the position of the item
 */
void unequip(Bit8u *hero, unsigned short item, unsigned short pos) {

	Bit8u *item_p;

	/* unequip of item 0 is not allowed */
	if (item == 0)
		return;

	item_p = get_itemsdat(item);

	/* if item is an armor ? */
	if (host_readb(item_p + 2) & 1) {
		signed char rs_mod;

		rs_mod = ds_readb(host_readb(item_p + 4) * 2 + 0x877);
		host_writeb(hero + 0x30, host_readb(hero + 0x30) - rs_mod);

		rs_mod = host_readb(hero + 0x196 + 7 + pos * 14);
		host_writeb(hero + 0x30, host_readb(hero + 0x30) + rs_mod);

		rs_mod = ds_readb(host_readb(item_p + 4) * 2 + 0x878);
		host_writeb(hero + 0x32, host_readb(hero + 0x32) - rs_mod);
	}
	/* if item is a weapon and in the right hand ? */
	if (((host_readb(item_p + 2) >> 1) & 1) && pos == 3) {
		host_writeb(hero + 0x78, 0);
		host_writeb(hero + 0x77, 0);
		host_writeb(hero + 0x76, 0);
	}
	/* unequip Kraftguertel KK - 5 */
	if (item == 183)
		host_writeb(hero + 0x47, host_readb(hero + 0x47) - 5);
	/* unequip Helm CH + 1 (cursed) */
	if (item == 196)
		host_writeb(hero + 0x3b, host_readb(hero + 0x3b) + 1);
	/* unequip Silberschmuck TA + 1 */
	if (item == 215)
		host_writeb(hero + 0x56, host_readb(hero + 0x56) + 2);
	/* unequip Stirnreif or Ring MR - 2 */
	if (item == 217 || item == 165)
		host_writeb(hero + 0x66, host_readb(hero + 0x66) - 2);
	/* unequip Totenkopfguertel TA + 4 */
	if (item == 182)
		host_writeb(hero + 0x56, host_readb(hero + 0x56) + 4);
	/* unequip Kristallkugel Gefahrensinn - 2 */
	if (item == 70)
		host_writeb(hero + 0x13a, host_readb(hero + 0x13a) - 2);
}


/**
 * add_equip_boni() - account boni of special items when equipped
 * @owner:	the owner of the item
 * @equipper:	the one who equips the item
 * @item:	the item ID
 * @pos_i:	the position in the inventory of the owner
 * @pos_b:	the position in the inventory of the equipper
 *
 */
void add_equip_boni(Bit8u *owner, Bit8u *equipper, signed short item, signed short pos_i, signed short pos_b)
{
	Bit8u *item_p;

	if (item) {
		/* calculate pointer to item description */
		item_p = Real2Host(ds_readd(ITEMSDAT)) + item * 12;

		/* armor and shield */
		if (host_readb(item_p + 2) & 1) {

			/* add RS boni */
			host_writeb(equipper + 0x30,
				host_readb(equipper + 0x30) + ds_readb(0x877 + host_readb(item_p + 4) * 2));

			/* subtract used item value */
			host_writeb(equipper + 0x30,
				host_readb(equipper + 0x30) - host_readb(owner + 0x196 + 7 + pos_i * 14));

			/* add RS-BE */
			host_writeb(equipper + 0x32,
				host_readb(equipper + 0x32) + ds_readb(0x877  + 1 + host_readb(item_p + 4) * 2));

		}

		/* weapon right hand */
		if (((host_readb(item_p + 2) >> 1) & 1) && (pos_b == 3)) {

			/* set weapon type */
			host_writeb(equipper + 0x78, host_readb(item_p + 3));

			/* set AT */
			host_writeb(equipper + 0x76,
				ds_readb(0x6b0 + 5 + host_readbs(item_p + 4) * 7));

			/* set PA */
			host_writeb(equipper + 0x77,
				ds_readb(0x6b0 + 6 + host_readbs(item_p + 4) * 7));

		}

		/* Girdle of might / Kraftguertel */
		if (item == 0xb7) {
			/* KK + 5 */
			host_writeb(equipper + 0x47,
				host_readb(equipper + 0x47) + 5);
		}

		/* Helmet / Helm */
		if (item == 0xc4) {
			/* dec CH */
			host_writeb(equipper + 0x3b,
				host_readb(equipper + 0x3b) - 1);
		}

		/* Silver Jewelry / Silberschmuckstueck (magisch) */
		if (item == 0xd7) {
			/* TA - 2 */
			host_writeb(equipper + 0x56,
				host_readb(equipper + 0x56) - 2);
		}

		/* Coronet or Ring / Stirnreif oder Ring */
		if (item == 0xd9 || item == 0xa5) {
			/* MR + 2 */
			host_writeb(equipper + 0x66,
				host_readb(equipper + 0x66) + 2);
		}

		/* Death-Head belt / Totenkopfguertel */
		if (item == 0xb6) {

			/* TA - 5 */
			host_writeb(equipper + 0x56,
				host_readb(equipper + 0x56) - 5);

			if (ds_readb(0x2845) == 20) {
				equip_belt_ani();
			}
		}

		/* Crystal ball / Kristalkugel */
		if (item == 0x46) {

			/* Sinnesschaerfe + 2 */
			host_writeb(equipper + 0x13a,
				host_readb(equipper + 0x13a) + 2);
		}
	}
}

/**
 * can_hero_use_item -	checks if a hero can use an item
 * @hero:	the hero
 * @item:	the item
 *
 * Returns 1 if the hero can use the item and 0 if not.
 */
unsigned short can_hero_use_item(Bit8u *hero, unsigned short item) {

	signed short *array;
	unsigned char typus;

	/* get the class of the hero */
	typus =	host_readb(hero + 0x21);

	/* some new error check */
	if (!typus)
		D1_ERR("Warning: %s() typus == 0\n", __func__);

	/* calculate the address of the class forbidden items array */
	array = (signed short*)Real2Host(ds_readd(0x634 + typus * 4));

	if (!is_in_word_array(item, array))
		return 1;

	return 0;
}

/**
 * can_item_at_pos - checks if an item is equipable at a body position
 * @item:	the item
 * @pos:	ths position at the body
 *
 * Returns 1 if equipping is possible or 0 if not.
 */
unsigned short can_item_at_pos(unsigned short item, unsigned short pos) {

	Bit8u *item_p;

	item_p = get_itemsdat(item);

	/* if item is an armor ? */
	if (host_readb(item_p + 2) & 1)	{
		unsigned char v = host_readb(item_p + 3);

		/* can be weared on the head */
		if (pos == 0 && v == 0)
			return 1;
		/* can be weared on the torso */
		if (pos == 2 && v == 2)
			return 1;
		/* can be weared at the feet */
		if (pos == 6 && v == 6)
			return 1;
		/* can be weared at the arms */
		if (pos == 1 && v == 1)
			return 1;
		/* can be weared at the legs */
		if (pos == 5 && v == 5)
			return 1;
		/* can be weared at the left hand */
		if (pos == 4 && v == 9)
			return 1;

		return 0;
	}

	/* Stirnreif (3 types) can be weared at the head */
	if (item == 217 || item == 171 || item == 245)
		if (pos == 0)
			return 1;

	/* you can take everything else in the hands */
	if (pos == 3 || pos == 4)
		return 1;

	return 0;
}

/**
 * has_hero_equipped - returns the position of an equipped item
 * @hero:	the hero
 * @item:	the item
 *
 * Returns the position of item, if equipped, otherwise -1.
 * Is not used in the game.
 */
signed short has_hero_equipped(Bit8u *hero, unsigned short item) {

	unsigned short i;

	for (i = 0; i < 7; i++)
		if (host_readw(hero + 0x196 + item * 14) == item)
			return i;

	return -1;
}

/**
 * has_hero_stacked - returns the posotion of a non-full item stack
 * @hero:	the hero
 * @item:	the item
 *
 * Returns the postition of a non-full (<99) item stack or -1 if
 * the hero doesn't own this item or has only full stacks of them.
 */
//static
signed short has_hero_stacked(Bit8u *hero, unsigned short item) {

	unsigned short i;

	for (i = 0; i < 23; i++) {
		/* has the hero the item */
		if (host_readw(hero + 0x196 + i * 14) != item)
			continue;
		/* is the number of items < 99 */
		if (host_readw(hero + 0x196 + 2 + i * 14) >= 99)
			continue;

		return i;
	}

	return -1;
}

/**
 * give_hero_new_item() - generates an item and gives it to hero
 * @hero:	the hero who should get the item
 * @item:	id of the item
 * @mode:	0 = quiet / 1 = warn / 2 = ignore
 * @nr:		number of item the hero should get
 *
 * The mode parameter works that way: If the weight the hero carries
 * is greater than KK*10 mode 0 and 1 will not give the hero that item.
 * The difference is, that mode = 1 prints a warning, mode = 0 is quiet.
 * mode = 2 ignores the carry weight completely.
*/
signed short give_hero_new_item(Bit8u *hero, signed short item, signed short mode, signed short nr)
{
	Bit8u *item_p;
	signed short done;
	signed short retval;
	signed short l1;
	signed short si, di;

	si = nr;

	retval = 0;

	/* check if hero can carry that item */
	if ((mode != 2) &&
		(host_readb(hero + 0x47) * 100 <= host_readw(hero + 0x2d8))) {

		if (mode != 0) {
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0xc2c),
				(char*)(hero + 0x10));
			GUI_output(Real2Host(ds_readd(DTP2)));
		}
	} else {
		item_p = get_itemsdat(item);

		/* hero has a non-full stack of this item */
		if (((host_readb(item_p + 2) >> 4) & 1) &&
			((l1 = has_hero_stacked(hero, item)) != -1)) {


			/* check for space on existing stack */
			if (host_readw(hero + 0x196 + 2 + l1 * 14) + si > 99) {
				si = 99 - host_readw(hero + 0x196 + 2 + l1 * 14);
			}

			/* add items to stack */
			host_writew(hero + 0x196 + 2 + l1 * 14,
				host_readw(hero + 0x196 + 2 + l1 * 14) + si);

			/* add weight */
			host_writew(hero + 0x2d8,
				host_readw(hero + 0x2d8) + host_readw(item_p + 5) * si);

			retval = si;
		} else {

			/* Original-Bug: may lead to problems when the
				item counter is broken */
			if (host_readb(hero + 0x20) < 23) {

				done = 0;

				do {
				/* Original-Bug: may lead to problems when the
					item counter is broken */
					if (host_readb(hero + 0x20) < 23) {
						/* look for a free place */
						di = 6;
						do {
							di++;
							if (host_readw(hero + 0x196 + di * 14) == 0)
								break;
						} while (di < 23);

						if (di < 23) {
							if (si > 99)
								si = 99;
							/* increment item counter */
							host_writeb(hero + 0x20, host_readb(hero + 0x20) + 1);

							/* write item id */
							host_writew(hero + 0x196 + di * 14, item);

							/* write item counter */
							if ((host_readb(item_p + 2) >> 4) & 1) {
								/* write stackable items */
								host_writew(hero + 0x196 + 2 + di * 14, si);
							} else if ((host_readb(item_p + 2) >> 2) & 1) {
								/* unknown */
								host_writew(hero + 0x196 + 2 + di * 14,
									ds_readb(0x8aa + host_readb(item_p + 4) * 3));
							} else {
								host_writew(hero + 0x196 + 2 + di * 14, 0);
							}

							/* set magical flag */
							if (host_readb(item_p + 0xb) != 0) {
								host_writeb(hero + 0x196 + 4 + di * 14,
									host_readb(hero + 0x196 + 4 + di * 14) | 8);

#if !defined(__BORLANDC__)
								D1_INFO("%s hat soeben einen magischen Gegenstand erhalten: %s\n",
									(char*)hero + 0x10, get_itemname(item));
#endif
							}

							/* set breakfactor */
							if ((host_readb(item_p + 2) >> 1) & 1) {
								host_writeb(hero + 0x196 + 6 + di * 14,
									ds_readb(0x6b0 + 3 + host_readb(item_p + 4) * 7));
							}

							/* adjust weight */
							if ((host_readb(item_p + 2) >> 4) & 1) {
								/* add stackable items weight*/
								host_writew(hero + 0x2d8,
									host_readw(hero + 0x2d8) + host_readw(item_p + 5) * si);
								retval = si;
								si = 0;
							} else {
								/* add single item weight */
								host_writew(hero + 0x2d8,
									host_readw(hero + 0x2d8) + host_readw(item_p + 5));
								si--;
								retval++;
							}

							/* all items are distributed */
							if (si == 0)
								done = 1;

							/* special items */
							if (item == 0xa1) {
								host_writeb(hero + 0x125, host_readb(hero + 0x125) + 3);
							}
							if (item == 0xa3) {
								host_writeb(hero + 0x66, host_readb(hero + 0x66) + 5);
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
 * item_pleasing_ingerimm - checks if Ingerimm accepts this item as sacrifice
 * @item:	the item
 *
 */
//static
unsigned short item_pleasing_ingerimm(unsigned short item) {

	Bit8u *p_item;

	p_item = get_itemsdat(item);

	if (((host_readb(p_item + 2) >> 1) & 1) && host_readb(p_item + 3) == 4)
		return 1;

	if ((host_readb(p_item + 2) & 1) == 0)
		return 0;

	if ((signed char)ds_readb((signed char)host_readb(p_item + 4) * 2 + 0x877) <= 1)
		return 0;

	return 1;
}

/**
 *	drop_item()	-	tries to drop an item
 *	@hero:		pointer to the hero
 *	@pos:		position of the item to be dropped
 *	@nr:		number of stacked items to be dropped / -1 to ask
 *
 *	returns true if the item has been dropped or false if not
 *
 *	TODO: This function can be tuned a bit
 */
unsigned short drop_item(Bit8u *hero, unsigned short pos, signed short nr)
{

	Bit8u *p_item;
	signed short answer;
	unsigned short retval;
	unsigned short item;

	retval = 0;

	item = host_readw(hero + 0x196 + pos * 14);

	/* check if that item is valid */
	if (item == 0)
		return retval;

	p_item = get_itemsdat(item);

	if ((host_readb(p_item + 2) >> 6) & 1) {
		/* this item is not droppable */

		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_ltx(0x718),
			(char*)Real2Host(GUI_names_grammar(0x8002, item, 0)));

		GUI_output(Real2Host(ds_readd(0xd2f3)));
	} else {
		/* this item is droppable */

		if ((host_readb(p_item + 2) >> 4) & 1) {
			if (nr == -1) {
				sprintf((char*)Real2Host(ds_readd(0xd2f3)),
					(char*)get_ltx(0x36c),
					(char*)Real2Host(GUI_names_grammar(6, item, 0)));

				do {
					answer = GUI_input(Real2Host(ds_readd(0xd2f3)), 2);
				} while (answer < 0);

				nr = answer;
			}

			if (host_readw(hero + 0x196 + 2 + pos * 14) > nr) {
				/* remove some stacked items */

				/* adjust stack counter */
				host_writew(hero + 0x196 + 2 + pos * 14,
					host_readw(hero + 0x196 + 2 + pos * 14) - nr);
				/* adjust weight */
				host_writew(hero + 0x2d8,
					host_readw(hero + 0x2d8) - host_readw(p_item + 5) * nr);
			} else {
				/* remove all stacked items */

				/* adjust weight */
				host_writew(hero + 0x2d8,
					host_readw(hero + 0x2d8) -
					host_readw(p_item + 5) * host_readw(hero + 0x196 + 2 + pos * 14));

				/* decrement item counter */
				host_writeb(hero + 0x20,
					host_readw(hero + 0x20) - 1);

				/* clear the inventory pos */
				memset(hero + 0x196 + pos * 14, 0, 14);
			}

			retval = 1;
		} else {
			if (nr != -1 || GUI_bool(get_ltx(0x370)) == 1) {

				/* check if item is equipped */
				if (pos < 7)
					unequip(hero, item, pos);

				/* decrement item counter */
				host_writeb(hero + 0x20,
					host_readb(hero + 0x20) - 1);
				/* subtract item weight */
				host_writew(hero + 0x2d8,
					host_readw(hero + 0x2d8) - host_readw(p_item + 5));
				/* check special items */
				/* item: SICHEL Pflanzenkunde -3 */
				if (item == 0xa1)
					host_writeb(hero + 0x125,
						host_readb(hero + 0x125) - 3);
				/* item:  AMULETT MR -5 */
				if (item == 0xa3)
					host_writeb(hero + 0x66,
						host_readb(hero + 0x66) - 5);

				/* clear the inventory pos */
				memset(hero + 0x196 + pos * 14, 0, 14);
				retval = 1;
			}
		}
	}

	/* check for the piratecave to bring efferd a gift */
	if ((item == 0x66 || item == 0x61) && ds_readb(0x2d6e) == 0x0b &&
		ds_readw(X_TARGET) == 9 && ds_readw(Y_TARGET) == 9)
		ds_writeb(0x415f, 1);

	/* check for the mine in oberorken to bring ingerimm a gift */
	if (item_pleasing_ingerimm(item) && ds_readb(0x2d6e) == 0x0c &&
		ds_readw(X_TARGET) == 2 && ds_readw(Y_TARGET) == 14 &&
		ds_readb(0x2d75) == 1)
		ds_writeb(0x3f9f, 1);

	return retval;
}

/**
 * hero_count_item -	returns how many items of one type the hero has
 * @hero:	the hero
 * @item:	the item
 */
unsigned short hero_count_item(Bit8u *hero, unsigned short item) {

	unsigned short i;
	unsigned short ret = 0;

	for (i = 0; i < 23; i++)
		if (host_readw(hero + 0x196 + i * 14) == item)
			ret++;

	return ret;
}
/**
 * group_count_item -	returns how many items of one type the current group has
 * @item:	the item
 */
unsigned short group_count_item(unsigned short item) {

	Bit8u *hero_i;
	unsigned short i;
	unsigned short ret = 0;

	hero_i = get_hero(0);
	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* check class */
		if (host_readb(hero_i + 0x21) == 0)
			continue;
		/* check group */
		if (host_readb(hero_i + 0x87) != ds_readb(CURRENT_GROUP))
			continue;
		ret += hero_count_item(hero_i, item);
	}

	return ret;
}

/**
 *	loose_random_item() - a hero will loose an item
 *	@hero:		the hero
 *	@percent:	probability to loose
 *	@text:		the displayed text
 *
 */
void loose_random_item(Bit8u *hero, unsigned short percent, Bit8u *text)
{
	Bit8u *p_item;
	unsigned short item, pos;

	if (random_schick(100) > percent)
		return;

	/* Original-Bug: infinite loop if the hero has no item */
	do {
		pos = random_schick(23) - 1;

		item = host_readw(hero + 0x196 + pos * 14);

		p_item = get_itemsdat(item);

		/* No item to drop */
		if (item == 0)
			continue;

		if (((host_readb(p_item + 2) >> 6) & 1) == 1)
			continue;

		/* drop 1 item */
		drop_item(hero, pos, 1);

		sprintf((char*)Real2Host(ds_readd(0xd2eb)),
			(char*)text, hero + 0x10,
			Real2Host(GUI_names_grammar(0, item, 0)));

		GUI_output(Real2Host(ds_readd(0xd2eb)));

		return;
	} while (1);
}

#if !defined(__BORLANDC__)
}
#endif
