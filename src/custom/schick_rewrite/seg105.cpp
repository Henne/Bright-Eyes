/*
 *      Rewrite of DSA1 v3.02_de functions of seg105 (inventory)
 *      Functions rewritten 4/14
 *
 *      Functions called rewritten 4/13
 *      Functions uncalled rewritten 0/1
*/

#include "schick.h"

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
 * item_pleasing_ingerimm - checks if Ingerimm accepts this item as sacrifice
 * @item:	the item
 *
 */
//static
unsigned short item_pleasing_ingerimm(unsigned short item) {

	Bit8u *p_item;

	p_item = MemBase + Real2Phys(0xe22b) + item * 12;

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
