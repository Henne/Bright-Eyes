/*
	Rewrite of DSA1 v3.02_de functions of seg106 (inventory misc)
	Functions rewritten: 6/8
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg096.h"
#include "seg097.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

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
/* Borlandified and identical */
signed short two_hand_collision(Bit8u* hero, signed short item, signed short pos)
{
	signed short retval = 0;
	signed short other_pos;
	signed short in_hand;

	if (pos == 3 || pos == 4) {

		other_pos = 3;

		if (pos == 3) {
			other_pos = 4;
		}

		/* get the item in the other hand */
		in_hand = host_readw(hero + other_pos * 0x0e + 0x196);
		if (in_hand) {

			/* check if one hand has a two-handed weapon */
			if ((item_weapon(get_itemsdat(item)) && host_readbs(get_itemsdat(item) + 3) == 6) ||
			(item_weapon(get_itemsdat(in_hand)) && host_readbs(get_itemsdat(in_hand) + 3) == 6)) {
					retval = 1;
			}
		}
	}

	return retval;
}

/* Borlandified and identical */
void move_item(signed short pos1, signed short pos2, Bit8u *hero)
{
	signed short item1;
	signed short item2;
	signed short v3 = 0;
	signed short temp;
	struct knapsack_item tmp;

	if (!check_hero(hero) || (pos1 == pos2)) { }
	else {

		if ((pos2 > 6) && (pos1 > 6)) {
			/* Both items are in knapsacks */
			v3 = 1;
			item1 = host_readws(hero + 0x196 + pos1  * SIZEOF_KS_ITEM);
			item2 = host_readws(hero + 0x196 + pos2  * SIZEOF_KS_ITEM);
		} else {
			item1 = host_readws(hero + 0x196 + pos1  * SIZEOF_KS_ITEM);
			item2 = host_readws(hero + 0x196 + pos2  * SIZEOF_KS_ITEM);

			if ((pos2 < pos1) || ((pos1 < 7) && (pos2 < 7))) {
				if (pos1 < 7) {
					if (item1 != 0)
						v3 = 1;
				} else {
					v3 = 1;
				}

				if (v3 != 0) {
					/* exchange positions */
					temp = pos1;
					pos1 = pos2;
					pos2 = temp;

					/* exchange ids */
					temp = item1;
					item1 = item2;
					item2 = temp;
				}
			}

			v3 = 0;

			if ((item1 == 0) && (item2 == 0)) {
				GUI_output(get_ltx(0x344));
			} else {
				if (item2 != 0) {
					/* item have the same ids and are stackable */
					if ((item2 == item1) && item_stackable(get_itemsdat(item1))) {
						/* merge them */

						/* add counter from item at pos2 to item at pos1 */
						add_ks_counter(pos1, pos2, hero);

						/* delete item at pos2 */
						memset(hero + 0x196 + pos2 * SIZEOF_KS_ITEM,
							0, SIZEOF_KS_ITEM);
#ifdef M302de_ORIGINAL_BUGFIX
						/* Decrement the item counter */
						dec_ptr_bs(hero + 0x20);
#endif
					} else {
						if (!can_hero_use_item(hero, item2)) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ltx(0x374),
								(char*)hero + 0x10,
								(char*)get_ltx( ((host_readbs(hero + 0x22) != 0 ? 0x251 : 0x9) + host_readbs(hero + 0x21)) * 4),
								(char*)Real2Host(GUI_names_grammar(2, item2, 0)));


							GUI_output(Real2Host(ds_readd(DTP2)));
						} else {
							if (!can_item_at_pos(item2, pos1)) {
								if (is_in_word_array(item2, (signed short*)(p_datseg + 0x29e)))
									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ltx(0x378),
										(char*)Real2Host(GUI_names_grammar(0x4000, item2, 0)),
										(char*)get_ltx(0x8b4));
								else
									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ltx(0x378),
										(char*)Real2Host(GUI_names_grammar(0, item2, 0)),
										(char*)get_ltx(0x8b0));
								GUI_output(Real2Host(ds_readd(DTP2)));
							} else {
								if (two_hand_collision(hero, item2, pos1)) {

									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ltx(0xcf4),
										(char*)hero + 0x10);

									GUI_output(Real2Host(ds_readd(DTP2)));

								} else {
									if (item1 != 0)
										unequip(hero, item1, pos1);

									add_equip_boni(hero, hero, item2, pos2, pos1);
									v3 = 1;
								}
							}
						}
					}
				} else {
					unequip(hero, item1, pos1);
					v3 = 1;
				}
			}
		}

		if (v3 != 0) {

			/* item have the same ids and are stackable */
			if ((item2 == item1) && item_stackable(get_itemsdat(item1))) {
				/* merge them */

				/* add counter from item at pos2 to item at pos1 */
				add_ks_counter(pos1, pos2, hero);

				/* delete item at pos2 */
				memset(hero + 0x196 + pos2 * SIZEOF_KS_ITEM,
							0, SIZEOF_KS_ITEM);
#ifdef M302de_ORIGINAL_BUGFIX
				/* Decrement the item counter */
				dec_ptr_bs(hero + 0x20);
#endif
			} else {

#if !defined(__BORLANDC__)
				struct_copy((Bit8u*)&tmp, hero + 0x196 + pos1 * SIZEOF_KS_ITEM, SIZEOF_KS_ITEM);
				struct_copy(hero + 0x196 + pos1 * SIZEOF_KS_ITEM,
					hero + 0x196 + pos2 * SIZEOF_KS_ITEM, SIZEOF_KS_ITEM);
				struct_copy(hero + 0x196 + pos2 * SIZEOF_KS_ITEM, (Bit8u*)&tmp, SIZEOF_KS_ITEM);
#else
				/* exchange the items */
				tmp = *(struct knapsack_item*)(hero + 0x196 + pos1 * SIZEOF_KS_ITEM);

				*(struct knapsack_item*)(hero + 0x196 + pos1 * SIZEOF_KS_ITEM) =
					*(struct knapsack_item*)(hero + 0x196 + pos2 * SIZEOF_KS_ITEM);
				*(struct knapsack_item*)(hero + 0x196 + pos2 * SIZEOF_KS_ITEM) = tmp;
#endif
			}
		}
	}


}

/* Borlandified and identical */
void print_item_description(Bit8u *hero, signed short pos)
{
	Bit8u *item_p;

	/* create pointer to the item in the inventory */
	item_p = hero + 0x196 + pos * 14;

	if (host_readw(item_p) != 0) {
		/* normal item */

		if ((((signed short)host_readw(item_p + 2) > 1) &&
			item_stackable(get_itemsdat(host_readw(item_p)))) ||
			is_in_word_array(host_readw(item_p), (signed short*)(p_datseg + 0x29e))) {
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
	if (ks_broken(item_p)) {
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x778));
	}

	/* magic */
	if (ks_magic_hidden(item_p) &&	/* is magic */
		ks_magic_known(item_p)) { /* and you know it */
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x77c));
	}

	/* used */
	if (host_readb(item_p + 7) != 0) {
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x780));
	}

	/* poisoned */
	if (host_readw(item_p) == 0xc1 || host_readw(item_p) == 0xc8 ||
		ks_poison1(item_p) || ks_poison2(item_p) ||
		host_readb(hero + 0x196 + 9 + pos * 14) != 0) {
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x890));
	}

	/* staff */
	if (host_readw(item_p) == 0x85) {
		sprintf((char*)Real2Host(ds_readd(0xd2eb)),
			(char*)get_city(0xd4),
			host_readbs(hero + 0x195));
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

#ifdef M302de_ORIGINAL_BUGFIX
		if (!host_readb(hero + 0x21))
			continue;
#endif

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
 * equip_belt_ani() - shows an animation of a man equipping a belt
 *
 * This is only executed when equipping a skullbelt.
 */
void equip_belt_ani(void)
{
	struct nvf_desc nvf;
	Bit8u *p_pal;
	unsigned int nvf_length;
	signed short height;
	signed short width;

	signed short i;
	signed short handle;

	/* open GUERTEL.NVF */
	handle = load_archive_file(0x84);

	/* read NVF part 1 */
	nvf_length = read_archive_file(handle,
			Real2Host(ds_readd(0xc3db)), 64000);
	/* read NVF part 2 */
	nvf_length += read_archive_file(handle,
			Real2Host(ds_readd(0xc3db)) + 64000, 64000);

	bc_close(handle);

	/* calculate palette pointer */
	p_pal = Real2Host(ds_readd(0xc3db)) + nvf_length - 0x60;

	set_palette(p_pal, 0x80, 0x20);

	do_border((RealPt)ds_readd(0xd2ff), 209, 79, 215, 89, 9);

	do_fill_rect((RealPt)ds_readd(0xd2ff), 209, 79, 215, 89, 0);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect((RealPt)ds_readd(0xd2ff), 189, 69, 235, 99, 0);

	do_border((RealPt)ds_readd(0xd2ff), 189, 69, 235, 99, 9);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect((RealPt)ds_readd(0xd2ff), 169, 59, 255, 109, 0);

	do_border((RealPt)ds_readd(0xd2ff), 169, 59, 255, 109, 9);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect((RealPt)ds_readd(0xd2ff), 164, 54, 260, 114, 0);

	do_border((RealPt)ds_readd(0xd2ff), 164, 54, 260, 114, 9);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect((RealPt)ds_readd(0xd2ff), 159, 49, 263, 117, 0);

	do_border((RealPt)ds_readd(0xd2ff), 159, 49, 263, 117, 9);

	for (i = 0; i < 12; i++) {

		nvf.dst = Real2Host(ds_readd(0xd303));
		nvf.src = Real2Host(ds_readd(0xc3db));
		nvf.nr = i;
		nvf.type = 3;
		nvf.width = (Bit8u*)&width;
		nvf.height = (Bit8u*)&height;

		process_nvf(&nvf);
#if !defined(__BORLANDC__)
		/* BE-fix */
		width = host_readws((Bit8u*)&width);
		height = host_readws((Bit8u*)&height);
#endif

		ds_writew(0xc011, 160);
		ds_writew(0xc013, 50);
		ds_writew(0xc015, width + 159);
		ds_writew(0xc017, height + 49);
		ds_writed(0xc019, ds_readd(0xd303));

		wait_for_vsync();
		wait_for_vsync();
		wait_for_vsync();
		update_mouse_cursor();
		wait_for_vsync();

		do_pic_copy(0);

		refresh_screen_size();
	}

	/* a = b = c = -1 */
	ds_writeb(0x2ca7, -1);
	ds_writeb(0x2ca6, -1);
	ds_writew(CURRENT_ANI, -1);
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

#if !defined(__BORLANDC__)
}
#endif
