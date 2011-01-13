/*
 *      Rewrite of DSA1 v3.02_de functions of seg105 (inventory)
 *      Functions rewritten 8/14
 *
 *      Functions called rewritten 7/13
 *      Functions uncalled rewritten 1/1
*/

#include "schick.h"

#include "v302de.h"

#include "seg007.h"

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

		rs_mod = ds_readb(host_readb(item_p + 4) + 0x877);
		host_writeb(hero + 0x30, host_readb(hero + 0x30) - rs_mod);

		rs_mod = host_readb(hero + 0x19d + pos * 14);
		host_writeb(hero + 0x30, host_readb(hero + 0x30) + rs_mod);

		rs_mod = ds_readb(host_readb(item_p + 4) + 0x878);
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
		D1_INFO("Ruestung %d\n", v);

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

	/* Strinreif (3 types) can be weared at the head */
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
