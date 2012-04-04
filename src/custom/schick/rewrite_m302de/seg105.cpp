/*
 *      Rewrite of DSA1 v3.02_de functions of seg105 (inventory)
 *      Functions rewritten 10/14
 *
 *      Functions called rewritten 9/13
 *      Functions uncalled rewritten 1/1
*/

#include <string.h>
#include "schick.h"

#include "v302de.h"

#include "seg007.h"
#include "seg096.h"
#include "seg097.h"

namespace M302de {

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

		rs_mod = host_readb(hero + 0x19d + pos * 14);
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
 * can_hero_use_item -	checks if a hero can use an item
 * @hero:	the hero
 * @item:	the item
 *
 * Returns 1 if the hero can use the item and 0 if not.
 */
unsigned short can_hero_use_item(Bit8u *hero, unsigned short item) {

	Bit8u * array;
	unsigned char typus;

	/* get the class of the hero */
	typus =	host_readb(hero + 0x21);

	/* some new error check */
	if (!typus)
		D1_ERR("Warning: %s() typus == 0\n", __func__);

	/* calculate the address of the class forbidden items array */
	array = MemBase + Real2Phys(ds_readd(0x634 + typus * 4));

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
		if (host_readw(hero + 0x198 + i * 14) >= 99)
			continue;

		return i;
	}

	return -1;
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
		ds_readw(0x2d44) == 9 && ds_readw(0x2d46) == 9)
		ds_writeb(0x415f, 1);

	/* check for the mine in oberorken to bring ingerimm a gift */
	if (item_pleasing_ingerimm(item) && ds_readb(0x2d6e) == 0x0c &&
		ds_readw(0x2d44) == 2 && ds_readw(0x2d46) == 14 &&
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
		if (host_readb(hero_i + 0x87) != ds_readb(0x2d35))
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

}
