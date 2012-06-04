/*
	Rewrite of DSA1 v3.02_de functions of seg106 (inventory misc)
	Functions rewritten: 4/8
*/
#include <stdlib.h>
#include <string.h>

#include "schick.h"

#include "schick_common.h"
#include "v302de.h"

#include "seg007.h"
#include "seg096.h"
#include "seg097.h"

namespace M302de {

/**
 * two_hand_collision() - check for a two hand collision
 * @hero:	the hero
 * @item:	the item which should be equipped
 * @pos:	the position the item should be placed
 *
 * A hero can not equip a two-handed weapon if one hand is in use,
 * nor can he equip something in his free hand if he has a two-handed weapon
 * in the other.
 * If such a collision is detected this function returns 1 else 0.
 */
signed short two_hand_collision(Bit8u* hero, signed short item, signed short pos)
{
	signed short other_pos;
	register signed short retval;	/* di */
	register signed short in_hand;	/* cx */

	retval = 0;

	if (pos == 3 || pos == 4) {

		other_pos = 3;


		if (pos == 3) {
			other_pos = 4;
		}

		/* get the item in the other hand */
		in_hand = host_readw(hero + other_pos * 0x0e + 0x196);
		if (in_hand) {

			/* check if one hand has a two-handed weapon */
			if ( ((host_readb(get_itemsdat(item) + 2) >> 1) & 1) &&
				(host_readb(get_itemsdat(item) + 3) == 6) ||
				(((host_readb(get_itemsdat(in_hand) + 2) >> 1) & 1) &&
				(host_readb(get_itemsdat(in_hand) + 3) == 6))) {
					retval = 1;
			}
		}
	}

	return retval;
}

void print_item_description(Bit8u *hero, signed short pos)
{
	Bit8u *item_p;

	/* create pointer to the item in the inventory */
	item_p = hero + 0x196 + pos * 14;

	if (host_readw(item_p) != 0) {
		/* normal item */

		if ((((signed short)host_readw(item_p + 2) > 1) &&
			(host_readb(get_itemsdat(host_readw(item_p)) + 2) >> 4) & 1) ||
			is_in_word_array(host_readw(item_p), p_datseg + 0x29e)) {
			/* more than one item or special */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x120),
				get_ltx(0x4c4),
				Real2Host(GUI_names_grammar(0x4004, host_readw(item_p), 0)));
		} else {
			/* one item */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x2c),
				get_ltx(0x4c0),
				Real2Host(GUI_names_grammar(0, host_readw(item_p), 0)));
		}
	} else {
		/* no item */
		strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x344));
	}


	/* broken */
	if ((host_readb(item_p + 4) & 1) != 0) {
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x778));
	}

	/* magic */
	if (((host_readb(item_p + 4) >> 3) & 1) != 0 &&	/* is magic */
		((host_readb(item_p + 4) >> 7) & 1) != 0) { /* and you know it */
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x77c));
	}

	/* used */
	if (host_readb(item_p + 7) != 0) {
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x780));
	}

	/* poisoned */
	if (host_readw(item_p) == 0xc1 || host_readw(item_p) == 0xc3 ||
		((host_readb(item_p + 4) >> 5) & 1) != 0 ||
		((host_readb(item_p + 4) >> 6) & 1) != 0 ||
		host_readb(hero + 0x196 + 9 + pos * 14) != 0) {
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x890));
	}

	/* staff */
	if (host_readw(item_p) == 0x85) {
		sprintf((char*)Real2Host(ds_readd(0xd2eb)),
			(char*)get_city(0xd4),
			host_readb(hero + 0x195));
		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)Real2Host(ds_readd(0xd2eb)));
	}

	GUI_output(Real2Host(ds_readd(DTP2)));
}

/**
 * get_max_light_time() - get the maximum time of a burning lightsource
 *
 */
signed short get_max_light_time(void)
{
	Bit8u *hero;
	register signed short i;	/* di */
	register signed short j;	/* cx */
	register signed short retval;	/* si */

	retval = -1;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += 0x6da) {

		for (j = 0; j < 23; j++) {

			/* search for a burning torch */
			if (host_readw(hero + 0x196 + j * 14) == 0x16) {

				if (host_readb(hero + j * 14 + 0x196 + 8) > retval) {
					retval = host_readb(hero + j * 14 + 0x196 + 8);
				}
			} else if (host_readw(hero + 0x196 + j * 14) == 0xf9) {
				/* search for a burning lantern */

				if (host_readb(hero + j * 14 + 0x196 + 8) / 10 > retval) {
					retval = host_readb(hero + j * 14 + 0x196 + 8) / 10;
				}
			}
		}
	}

	return retval;
}

/**
 * get_full_waterskin_pos() - search for a full waterskin
 * @hero:	pointer to the hero
 */
signed short get_full_waterskin_pos(Bit8u *hero)
{
	signed short i, pos;

	pos = -1;

	/* Original-BUG: should start from inventory pos 0 */
	for (i = 7; i < 23; i++) {

		/* look for a waterskin...*/
		if (host_readw(hero + 0x196 + i * 14) != 0x1e)
			continue;

		/* ...which is not empty */
		if ((host_readb(hero + 0x196 + 4 + i * 14) >> 2) & 1)
			continue;

		pos = i;
		break;
	}

	return pos;
}

}
