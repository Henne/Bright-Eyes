/*
 *      Rewrite of DSA1 v3.02_de functions of seg105 (inventory)
 *      Functions rewritten 0/14
 *
 *      Functions called rewritten 1/13
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
