/*
 *	Rewrite of DSA1 v3.02_de functions of seg108 (consume)
 *	Functions rewritten: 1/1
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg108.cpp
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * consume() - owner gives consumer an item from his inventory
 * @owner:	the owner
 * @consumer:	the consumer
 * @pos:	the position in the inventory of the owner
 */
void consume(Bit8u *owner, Bit8u *consumer, signed short pos)
{
	Bit8u *item_p;
	signed short item;

	signed short id_bad_elex;
	signed short le_diff;

	signed short disease;
	signed short poison;
	signed short bak;
	signed short consumer_idx;

	signed short l_di, l_si;

	/* this hero cannot consume */
	if (check_hero(consumer) == 0) {
		return;
	}

	consumer_idx = get_hero_index(consumer);

	/* get item id */
	item = host_readw(owner + 0x196 + pos * 14);

	/* get pointer to ITEMS.DAT */
	item_p = get_itemsdat(item);

	/* is food */
	if (item_food(item_p)) {

		if (host_readb(item_p + 3) == 1) {
			/* eating */

#if !defined(__BORLANDC__)
				int diff = host_readbs(consumer + 0x7f) - host_readbs(item_p + 4);
				D1_INFO("%s isst %s mit Naehrwert %d. Der Hunger sinkt von %d auf %d\n",
					(consumer + 0x10),
					(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(item))),
					host_readbs(item_p + 4),
					host_readbs(consumer + 0x7f),
					(diff >= 0) ? diff : 0);
#endif

			/* subtract from hunger value */
			sub_ptr_bs(consumer + 0x7f, host_readbs(item_p + 4));

			/* adjust hunger value */
			if (host_readbs(consumer + 0x7f) < 0) {
				host_writeb(consumer + 0x7f, 0);
			}

			/* consume quietly */
			if (!ds_readbs(CONSUME_QUIET)) {
				GUI_output(get_ltx(0x33c));
			}

			/* drop one unit of that item */
			drop_item(owner, pos, 1);
		} else {
			/* drinking */

			/* check if item is not empty */
			if (!ks_empty(owner + 0x196 + pos * 14)) {

#if !defined(__BORLANDC__)
				int diff = host_readbs(consumer + 0x80) - host_readbs(item_p + 4);
				D1_INFO("%s trinkt aus %s mit Naehrwert %d. Der Durst sinkt von %d auf %d\n",
					(consumer + 0x10),
					(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(item))),
					host_readbs(item_p + 4),
					host_readbs(consumer + 0x80),
					(diff >= 0) ? diff : 0);
#endif

				/* subtract from thirst value */
				sub_ptr_bs(consumer + 0x80, host_readbs(item_p + 4));

				/* adjust thirst value */
				if (host_readbs(consumer + 0x80) < 0) {
					host_writeb(consumer + 0x80, 0);
				}

				/* consume quietly */
				if (!ds_readbs(CONSUME_QUIET)) {
					GUI_output(get_ltx(0x340));
				}


				if (item == 0x1e) {
					/* water */

					if (ks_half_empty(owner + 0x196 + pos * 14)) {
						/* empty */
						or_ptr_bs(owner + 0x196 + 4 + pos * 14, 4);
					} else {
						/* half empty */
						or_ptr_bs(owner + 0x196 + 4 + pos * 14, 2);
					}

				} else if (item == 0x5c || item == 0x5b) {
					/* wine or snaps */
					hero_get_drunken(consumer);
					drop_item(owner, pos, 1);
				} else {
					/* everything else: Beer */
					drop_item(owner, pos, 1);

					/* That does not happen */
					if (item != 0x17) {
						/* get an empty glass bottle */
						give_hero_new_item(owner, 0x1f, 2, 1);
					}
				}
			} else {
				/* print empty message */
				GUI_output(get_ltx(0x6b4));
			}
		}

		ds_writew(0x2846, 1);

	} else if (item_herb_potion(item_p)) {

		if (host_readb(item_p + 3) == 0) {

			if (is_in_word_array(item, (signed short*)(p_datseg + 0x8f1))) {
				GUI_output(get_ltx(0x7cc));
			} else if (is_in_word_array(item, (signed short*)(p_datseg + 0x8e7)) ||
					is_in_word_array(item, (signed short*)(p_datseg + 0x8d3))) {
				/* herbs and poisons */
				GUI_output(get_ltx(0x7d0));
			} else {
				/* consume with effects */

				/* drop one entity */
				drop_item(owner, pos, 1);

				/* terminate output string */
				host_writeb(Real2Host(ds_readd(DTP2)), 0);

				switch (item) {
				case 0x3f: {
					/* Gulmond Blatt */

					/* KK+2 for 12h */
					l_di = get_free_mod_slot();
					set_mod_slot(l_di, 0xfd20, consumer + 0x47, 2, (signed char)consumer_idx);

					/* LE + 2 */
					add_hero_le(consumer, 2);

					/* prepare output */
					strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x7d4));
					break;
				}
				case 0x3c: {
					/* Vierblaettrige Einbeere */

					l_di = random_schick(6);
					le_diff = host_readw(consumer + 0x5e) - host_readw(consumer + 0x60);
					if (l_di > le_diff)
						l_di = le_diff;

					add_hero_le(consumer, l_di);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x7d8), l_di);

					break;
				}
				case 0x7b: {
					/* Belmart */
					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison <= 5) {
						/* cure the first poison */
						host_writeb(consumer + poison * 5 + 0xd7, 0);
						host_writeb(consumer + poison * 5 + 0xd6, 1);
					}

					/* TODO: unknown for 24h */
					l_di = get_free_mod_slot();
					set_mod_slot(l_di, 0x1fa40, consumer + 0x7d, 1, (signed char)consumer_idx);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x7dc),
						(char*)consumer + 0x10);
					break;
				}
				case 0x7d: {
					/* Menchalkaktus */
					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison <= 10) {
						/* cure the first poison */
						host_writeb(consumer + poison * 5 + 0xd7, 0);
						host_writeb(consumer + poison * 5 + 0xd6, 1);
					}

					/* TODO: unknown for 24h */
					l_di = get_free_mod_slot();
					set_mod_slot(l_di, 0x1fa40, consumer + 0x7d, 2, (signed char)consumer_idx);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x7dc),
						(char*)consumer + 0x10);
					break;
				}
				case 0x7f: {
					/* Atmon */
					for (l_si = 9; l_si < 19; l_si++) {
						/* All body skills + 2 for 5h */
						l_di = get_free_mod_slot();
						set_mod_slot(l_di, 0x6978, consumer + 0x108 + l_si, 2, (signed char)consumer_idx);
					}

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x7e0),
						(char*)consumer + 0x10);
					break;
				}
				case 0x3d: {
					/* Wirselkraut */
					l_di = 10;
					le_diff = host_readw(consumer + 0x5e) - host_readw(consumer + 0x60);
					if (le_diff < l_di)
						l_di = le_diff;

					add_hero_le(consumer, l_di);
					/* prepare output */

					sprintf((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x7e4), l_di);
					break;
				}
				case 0x40: {
					/* Tarnelle */
					host_writeb(consumer + 0x95, 1);
					break;
				}
				}

				/* print the output */
				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		} else {

			/* check if item is an elexire */
			l_si = is_in_word_array(item, (signed short*)(p_datseg + 0x8ff));
			id_bad_elex = is_in_word_array(item, (signed short*)(p_datseg + 0x90f));


			if (l_si != 0) {
				/* handle good elexires */

				/* drop elexire */
				drop_item(owner, pos, 1);

				/* get glassbottle */
				give_hero_new_item(owner, 0x1f, 2, 1);

				/* Attribute +5 for 1h */
				l_di = get_free_mod_slot();
				set_mod_slot(l_di, 0x1518,
					consumer + 0x35 + (l_si - 1) * 3,
					5, (signed char)consumer_idx);

				/* prepare output */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ltx(0x7f4), (char*)consumer + 0x10,
					(char*)get_ltx((0x19b + l_si) * 4), 5);

				/* print output */
				GUI_output(Real2Host(ds_readd(DTP2)));

			} else if (id_bad_elex != 0) {
				/* handle bad elexires */

				/* drop elexire */
				drop_item(owner, pos, 1);

				/* get glassbottle */
				give_hero_new_item(owner, 0x1f, 2, 1);

				/* Attribute -7 for 1h */
				l_di = get_free_mod_slot();
				set_mod_slot(l_di, 0x1518,
					consumer + 0x35 + (id_bad_elex - 1) * 3,
					-7, (signed char)consumer_idx);

				/* prepare output */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ltx(0xa40), (char*)consumer + 0x10,
					(char*)get_ltx((0x19b + id_bad_elex) * 4), 7);

				/* print output */
				GUI_output(Real2Host(ds_readd(DTP2)));

			} else {
				/* everything else */

				/* drop the item */
				drop_item(owner, pos, 1);

				switch (item) {
				case 0x91 : {
					/* Heiltrank */

					l_si = host_readw(consumer + 0x5e) - host_readw(consumer + 0x60);
					if (l_si > 10)
						l_si = 10;

					/* singular POINT/ PUNKT */
					strcpy((char*)Real2Host(ds_readd(0xd2eb)),
						(char*)get_ltx(0x620));

					if (l_si > 1) {
						/* make plural POINTS/PUNKTE */
						strcat((char*)Real2Host(ds_readd(0xd2eb)),
						(char*)get_ltx(0x624));
					}

					/* add LE */
					add_hero_le(consumer, l_si);

					/* give owner a glassbottle */
					give_hero_new_item(owner, 0x1f, 2, 1);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x7f8),
						(char*)consumer + 0x10,
						l_si,
						(char*)Real2Host(ds_readd(0xd2eb)));
					break;
				}
				case 0x92 : {
					/* Starker Heiltrank */

					/* 1W20+10 */
					l_si = dice_roll(1, 20, 10);
					if (host_readws(consumer + 0x5e) - host_readws(consumer + 0x60) < l_si)
						l_si = host_readw(consumer + 0x5e) - host_readw(consumer + 0x60);

					/* add LE */
					add_hero_le(consumer, l_si);

					/* give owner a copperbottle */
					give_hero_new_item(owner, 0x2a, 2, 1);

					/* singular POINT/ PUNKT */
					strcpy((char*)Real2Host(ds_readd(0xd2eb)),
						(char*)get_ltx(0x620));

					if (l_si > 1) {
						/* make plural POINTS/PUNKTE */
						strcat((char*)Real2Host(ds_readd(0xd2eb)),
						(char*)get_ltx(0x624));
					}

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x7f8),
						(char*)consumer + 0x10,
						l_si,
						(char*)Real2Host(ds_readd(0xd2eb)));
					break;
				}
				case 0xec: {
					/* Wunderkur */

					/* undo starvation damage */
					add_ptr_ws(consumer + 0x5e, host_readbs(consumer + 0x7a));
					host_writeb(consumer + 0x7a, 0);


					/* fill up LE */
					if (host_readws(consumer + 0x60) < host_readws(consumer + 0x5e)) {
						add_hero_le(consumer, host_readw(consumer + 0x5e));
					}

					/* diseases, not all */
					disease = hero_is_diseased(consumer);

					if (disease == 2 || disease == 3) {
						host_writeb(consumer + disease * 5 + 0xae, 1);
						host_writeb(consumer + disease * 5 + 0xaf, 0);
					}

					/* poison */
					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison < 7) {
						host_writeb(consumer + poison * 5 + 0xd7, 0);
						host_writeb(consumer + poison * 5 + 0xd6, 1);
					}

					/* give owner a glasbottle */
					give_hero_new_item(owner, 0x1f, 2, 1);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x7fc),
						(char*)consumer + 0x10,
						(char*)Real2Host(GUI_get_ptr(host_readbs(consumer + 0x22), 0)));

					break;
				}
				case 0xed: {
					/* Schlaftrunk */

					/* 3 Rounds of sleep */
					host_writeb(consumer + 0x95, 3);

					/* give owner a glasbottle */
					give_hero_new_item(owner, 0x1f, 2, 1);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0xb88),
						(char*)consumer + 0x10);
					break;
				}
				case 0x9a: {
					/* Zaubertrank */

					if (host_readbs(consumer + 0x21) >= 7) {
						/* Magicuser */

						l_si = host_readw(consumer + 0x62) - host_readw(consumer + 0x64);

						if (l_si > 10)
							l_si = 10;

						/* add AE */
						add_hero_ae(consumer, l_si);

						/* give hero a glassbottle */
						give_hero_new_item(owner, 0x1f, 2, 1);

						/* prepare output */
						strcpy((char*)Real2Host(ds_readd(0xd2eb)),
							(char*)get_ltx(0x620));

						if (l_si > 1) {
							strcat((char*)Real2Host(ds_readd(0xd2eb)),
								(char*)get_ltx(0x624));
						}

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x800),
							l_si,
							(char*)Real2Host(ds_readd(0xd2eb)),
							(char*)consumer + 0x10);
					} else {
						/* Not a magicuser */

						/* prepare output */
						strcpy((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0xc90));

						/* give owner a glassbottle */
						give_hero_new_item(owner, 0x1f, 2, 1);
					}

					break;
				}
				case 0x9b: {
					/* Zaubertrank (stark) */

					if (host_readbs(consumer + 0x21) >= 7) {
						/* Magicuser */

						l_si = host_readw(consumer + 0x62) - host_readw(consumer + 0x64);

						if (l_si > 30)
							l_si = 30;

						/* add AE */
						add_hero_ae(consumer, l_si);

						/* give owner a glassbottle */
						give_hero_new_item(owner, 0x1f, 2, 1);


						/* prepare output */
						if (host_readws(consumer + 0x64) >= host_readws(consumer + 0x62)) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ltx(0x804),
								(char*)consumer + 0x10);
						} else {
							strcpy((char*)Real2Host(ds_readd(0xd2eb)),
								(char*)get_ltx(0x620));

							if (l_si > 1) {
								strcat((char*)Real2Host(ds_readd(0xd2eb)),
									(char*)get_ltx(0x624));
							}

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ltx(0x800),
								l_si,
								(char*)Real2Host(ds_readd(0xd2eb)),
								(char*)consumer + 0x10);
						}
					} else {
						/* Not a magicuser */

						/* prepare output */
						strcpy((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0xc90));

						/* give owner a glassbottle */
						give_hero_new_item(owner, 0x1f, 2, 1);
					}
					break;
				}
				case 0xb4: {
					/* Gegengift */

					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison < 5) {
						host_writeb(consumer + poison * 5 + 0xd7, 0);
						host_writeb(consumer + poison * 5 + 0xd6, 1);
					}

					/* give owner a glassbottle */
					give_hero_new_item(owner, 0x1f, 2, 1);


					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x74c),
						(char*)consumer + 0x10);
					break;
				}
				case 0xdf: {
					/* Antikrankheitselexier */

					disease = hero_is_diseased(consumer);

					host_writeb(consumer + disease * 5 + 0xae, 1);
					host_writeb(consumer + disease * 5 + 0xaf, 0);

					/* give owner a glassbottle */
					give_hero_new_item(owner, 0x1f, 2, 1);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x74c),
						(char*)consumer + 0x10);
					break;

				}
				}

				/* print output: maybe garbage - string not set to 0 */
				bak = ds_readw(0xbffd);
				ds_writew(0xbffd, 5);
				GUI_output(Real2Host(ds_readd(DTP2)));
				ds_writew(0xbffd, bak);
			}
		}

		ds_writew(0x2846, 1);
	} else {
		/* this item cannot be consumed */
		GUI_output(get_ltx(0x338));
	}
}

#if !defined(__BORLANDC__)
}
#endif
