/*
 *      Rewrite of DSA1 v3.02_de functions of seg105 (inventory)
 *      Functions rewritten 6/14
 *
 *      Functions called rewritten 5/13
 *      Functions uncalled rewritten 1/1
*/

#include "schick.h"

#include "v302de.h"

#include "seg007.h"

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
