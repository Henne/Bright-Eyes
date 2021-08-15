/*
 *	Rewrite of DSA1 v3.02_de functions of seg108 (consume)
 *	Functions rewritten: 1/1
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg108.cpp
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
 * \brief   owner gives consumer an item from his inventory
 *
 * \param   owner       the owner
 * \param   consumer    the consumer
 * \param   pos         the position in the inventory of the owner
 */
void consume(Bit8u *owner, Bit8u *consumer, signed short pos)
{
	Bit8u *item_p;
	signed short item;

	signed short id_bad_elex;
	signed short le_diff;

	signed short disease;
	signed short poison;
	signed short textbox_width_bak;
	signed short consumer_idx;

	signed short l_di, l_si;

	/* this hero cannot consume */
	if (check_hero(consumer) == 0) {
		return;
	}

	consumer_idx = get_hero_index(consumer);

	/* get item id */
	item = host_readw(owner + HERO_INVENTORY + INVENTORY_ITEM_ID + pos * SIZEOF_INVENTORY);

	/* get pointer to ITEMS.DAT */
	item_p = get_itemsdat(item);

	/* is food */
	if (item_food(item_p)) {

		if (host_readb(item_p + 3) == 1) {
			/* eating */

#if !defined(__BORLANDC__)
				int diff = host_readbs(consumer + HERO_HUNGER) - host_readbs(item_p + 4);
				D1_INFO("%s isst %s mit Naehrwert %d. Der Hunger sinkt von %d auf %d\n",
					(consumer + HERO_NAME2),
					(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(item))),
					host_readbs(item_p + 4),
					host_readbs(consumer + HERO_HUNGER),
					(diff >= 0) ? diff : 0);
#endif

			/* subtract from hunger value */
			sub_ptr_bs(consumer + HERO_HUNGER, host_readbs(item_p + 4));

			/* adjust hunger value */
			if (host_readbs(consumer + HERO_HUNGER) < 0) {
				host_writeb(consumer + HERO_HUNGER, 0);
			}

			/* consume quietly */
			if (!ds_readbs(CONSUME_QUIET)) {
				GUI_output(get_ttx(207));
			}

			/* drop one unit of that item */
			drop_item(owner, pos, 1);
		} else {
			/* drinking */

			/* check if item is not empty */
			if (!ks_empty(owner + HERO_INVENTORY + pos * SIZEOF_INVENTORY)) {

#if !defined(__BORLANDC__)
				int diff = host_readbs(consumer + HERO_THIRST) - host_readbs(item_p + 4);
				D1_INFO("%s trinkt aus %s mit Naehrwert %d. Der Durst sinkt von %d auf %d\n",
					(consumer + HERO_NAME2),
					(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(item))),
					host_readbs(item_p + 4),
					host_readbs(consumer + HERO_THIRST),
					(diff >= 0) ? diff : 0);
#endif

				/* subtract from thirst value */
				sub_ptr_bs(consumer + HERO_THIRST, host_readbs(item_p + 4));

				/* adjust thirst value */
				if (host_readbs(consumer + HERO_THIRST) < 0) {
					host_writeb(consumer + HERO_THIRST, 0);
				}

				/* consume quietly */
				if (!ds_readbs(CONSUME_QUIET)) {
					GUI_output(get_ttx(208));
				}


				if (item == ITEM_WATERSKIN) {
					/* water */

					if (ks_half_empty(owner + HERO_INVENTORY + pos * SIZEOF_INVENTORY)) {
						/* empty */
						or_ptr_bs(owner + HERO_INVENTORY + INVENTORY_FLAGS + pos * SIZEOF_INVENTORY, 4); /* set 'empty' flag */
					} else {
						/* half empty */
						or_ptr_bs(owner + HERO_INVENTORY + INVENTORY_FLAGS + pos * SIZEOF_INVENTORY, 2); /* set 'half empty' flag */
					}

				} else if (item == ITEM_BRANDY || item == ITEM_WINE) {
					/* wine or snaps */
					hero_get_drunken(consumer);
					drop_item(owner, pos, 1);
				} else {
					/* everything else: Beer */
					drop_item(owner, pos, 1);

					/* That does not happen */
					if (item != ITEM_BEER) {
						/* get an empty glass bottle */
						give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);
					}
				}
			} else {
				/* print empty message */
				GUI_output(get_ttx(429));
			}
		}

		ds_writew(REQUEST_REFRESH, 1);

	} else if (item_herb_potion(item_p)) {

		if (host_readb(item_p + 3) == 0) {

			if (is_in_word_array(item, (signed short*)(p_datseg + HERBS_UNEATABLE))) {
				GUI_output(get_ttx(499));
			} else if (is_in_word_array(item, (signed short*)(p_datseg + HERBS_TOXIC)) ||
					is_in_word_array(item, (signed short*)(p_datseg + POISON_POTIONS))) {
				/* herbs and poisons */
				GUI_output(get_ttx(500));
			} else {
				/* consume with effects */

				/* drop one entity */
				drop_item(owner, pos, 1);

				/* terminate output string */
				host_writeb(Real2Host(ds_readd(DTP2)), 0);

				switch (item) {
				case ITEM_GULMOND_LEAF: {
					/* Gulmond Blatt */

					/* KK+2 for 12h */
					l_di = get_free_mod_slot();
					set_mod_slot(l_di, HOURS(12), consumer + (HERO_ATTRIB + 3 * ATTRIB_KK), 2, (signed char)consumer_idx);

					/* LE + 2 */
					add_hero_le(consumer, 2);

					/* prepare output */
					strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(501));
					break;
				}
				case ITEM_EINBEERE: {
					/* Vierblaettrige Einbeere */

					l_di = random_schick(6);
					le_diff = host_readw(consumer + HERO_LE_ORIG) - host_readw(consumer + HERO_LE);
					if (l_di > le_diff)
						l_di = le_diff;

					add_hero_le(consumer, l_di);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(502), l_di);

					break;
				}
				case ITEM_BELMART: {
					/* Belmart */
					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison <= 5) {
						/* cure the first poison */
						host_writeb(consumer + poison * 5 + 0xd7, 0);
						host_writeb(consumer + poison * 5 + 0xd6, 1);
					}

					/* TODO: unknown for 24h */
					l_di = get_free_mod_slot();
					set_mod_slot(l_di, DAYS(1), consumer + HERO_HERBS, 1, (signed char)consumer_idx);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(503),
						(char*)consumer + HERO_NAME2);
					break;
				}
				case ITEM_MENCHAL: {
					/* Menchalkaktus */
					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison <= 10) {
						/* cure the first poison */
						host_writeb(consumer + poison * 5 + 0xd7, 0);
						host_writeb(consumer + poison * 5 + 0xd6, 1);
					}

					l_di = get_free_mod_slot();
					set_mod_slot(l_di, DAYS(1), consumer + HERO_HERBS, 2, (signed char)consumer_idx);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(503),
						(char*)consumer + HERO_NAME2);
					break;
				}
				case ITEM_ATMONBLUETE: {
					/* Atmon */
					for (l_si = 9; l_si < 19; l_si++) {
						/* All body skills + 2 for 5h */
						l_di = get_free_mod_slot();
						set_mod_slot(l_di, HOURS(5), consumer + HERO_TALENTS + l_si, 2, (signed char)consumer_idx);
					}

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(504),
						(char*)consumer + HERO_NAME2);
					break;
				}
				case ITEM_WHIRLWEED: {
					/* Wirselkraut */
					l_di = 10;
					le_diff = host_readw(consumer + HERO_LE_ORIG) - host_readw(consumer + HERO_LE);
					if (le_diff < l_di)
						l_di = le_diff;

					add_hero_le(consumer, l_di);
					/* prepare output */

					sprintf((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(505), l_di);
					break;
				}
				case ITEM_TARNELE: {
					/* Tarnelle */
					host_writeb(consumer + HERO_RUHE_KOERPER, 1);
					break;
				}
				}

				/* print the output */
				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		} else {

			/* check if item is an elexire */
			l_si = is_in_word_array(item, (signed short*)(p_datseg + ELIXIR_POTIONS));
			id_bad_elex = is_in_word_array(item, (signed short*)(p_datseg + BAD_ELIXIRS));


			if (l_si != 0) {
				/* handle good elexires */

				/* drop elexire */
				drop_item(owner, pos, 1);

				/* get glassbottle */
				give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

				/* Attribute +5 for 1h */
				l_di = get_free_mod_slot();
				set_mod_slot(l_di, HOURS(1),
					consumer + HERO_ATTRIB + (l_si - 1) * 3,
					5, (signed char)consumer_idx);

				/* prepare output */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(509), (char*)consumer + HERO_NAME2,
					(char*)get_ttx(411 + l_si), 5);

				/* print output */
				GUI_output(Real2Host(ds_readd(DTP2)));

			} else if (id_bad_elex != 0) {
				/* handle bad elexires */

				/* drop elexire */
				drop_item(owner, pos, 1);

				/* get glassbottle */
				give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

				/* Attribute -7 for 1h */
				l_di = get_free_mod_slot();
				set_mod_slot(l_di, HOURS(1),
					consumer + HERO_ATTRIB + (id_bad_elex - 1) * 3,
					-7, (signed char)consumer_idx);

				/* prepare output */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(656), (char*)consumer + HERO_NAME2,
					(char*)get_ttx(411 + id_bad_elex), 7);

				/* print output */
				GUI_output(Real2Host(ds_readd(DTP2)));

			} else {
				/* everything else */

				/* drop the item */
				drop_item(owner, pos, 1);

				switch (item) {
				case 0x91 : {
					/* Heiltrank */

					l_si = host_readw(consumer + HERO_LE_ORIG) - host_readw(consumer + HERO_LE);
					if (l_si > 10)
						l_si = 10;

					/* singular POINT/ PUNKT */
					strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_ttx(392));

					if (l_si > 1) {
						/* make plural POINTS/PUNKTE */
						strcat((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_ttx(393));
					}

					/* add LE */
					add_hero_le(consumer, l_si);

					/* give owner a glassbottle */
					give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(510),
						(char*)consumer + HERO_NAME2,
						l_si,
						(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
					break;
				}
				case 0x92 : {
					/* Starker Heiltrank */

					/* 1W20+10 */
					l_si = dice_roll(1, 20, 10);
					if (host_readws(consumer + HERO_LE_ORIG) - host_readws(consumer + HERO_LE) < l_si)
						l_si = host_readw(consumer + HERO_LE_ORIG) - host_readw(consumer + HERO_LE);

					/* add LE */
					add_hero_le(consumer, l_si);

					/* give owner a copperbottle */
					give_hero_new_item(owner, ITEM_FLASK_BRONZE, 2, 1);

					/* singular POINT/ PUNKT */
					strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_ttx(392));

					if (l_si > 1) {
						/* make plural POINTS/PUNKTE */
						strcat((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_ttx(393));
					}

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(510),
						(char*)consumer + HERO_NAME2,
						l_si,
						(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
					break;
				}
				case 0xec: {
					/* Wunderkur */

					/* undo starvation damage */
					add_ptr_ws(consumer + HERO_LE_ORIG, host_readbs(consumer + HERO_LE_MOD));
					host_writeb(consumer + HERO_LE_MOD, 0);


					/* fill up LE */
					if (host_readws(consumer + HERO_LE) < host_readws(consumer + HERO_LE_ORIG)) {
						add_hero_le(consumer, host_readw(consumer + HERO_LE_ORIG));
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
					give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(511),
						(char*)consumer + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(consumer + HERO_SEX), 0)));

					break;
				}
				case 0xed: {
					/* Schlaftrunk */

					/* 3 Rounds of sleep */
					host_writeb(consumer + HERO_RUHE_KOERPER, 3);

					/* give owner a glasbottle */
					give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

					/* prepare output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(738),
						(char*)consumer + HERO_NAME2);
					break;
				}
				case 0x9a: {
					/* Zaubertrank */

					if (host_readbs(consumer + HERO_TYPE) >= 7) {
						/* Magicuser */

						l_si = host_readw(consumer + HERO_AE_ORIG) - host_readw(consumer + HERO_AE);

						if (l_si > 10)
							l_si = 10;

						/* add AE */
						add_hero_ae(consumer, l_si);

						/* give hero a glassbottle */
						give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

						/* prepare output */
						strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
							(char*)get_ttx(392));

						if (l_si > 1) {
							strcat((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
								(char*)get_ttx(393));
						}

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(512),
							l_si,
							(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
							(char*)consumer + HERO_NAME2);
					} else {
						/* Not a magicuser */

						/* prepare output */
						strcpy((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(804));

						/* give owner a glassbottle */
						give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);
					}

					break;
				}
				case 0x9b: {
					/* Zaubertrank (stark) */

					if (host_readbs(consumer + HERO_TYPE) >= 7) {
						/* Magicuser */

						l_si = host_readw(consumer + HERO_AE_ORIG) - host_readw(consumer + HERO_AE);

						if (l_si > 30)
							l_si = 30;

						/* add AE */
						add_hero_ae(consumer, l_si);

						/* give owner a glassbottle */
						give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);


						/* prepare output */
						if (host_readws(consumer + HERO_AE) >= host_readws(consumer + HERO_AE_ORIG)) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(513),
								(char*)consumer + HERO_NAME2);
						} else {
							strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
								(char*)get_ttx(392));

							if (l_si > 1) {
								strcat((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
									(char*)get_ttx(393));
							}

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(512),
								l_si,
								(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
								(char*)consumer + HERO_NAME2);
						}
					} else {
						/* Not a magicuser */

						/* prepare output */
						strcpy((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(804));

						/* give owner a glassbottle */
						give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);
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
					give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);


					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(467),
						(char*)consumer + HERO_NAME2);
					break;
				}
				case 0xdf: {
					/* Antikrankheitselexier */

					disease = hero_is_diseased(consumer);

					host_writeb(consumer + disease * 5 + 0xae, 1);
					host_writeb(consumer + disease * 5 + 0xaf, 0);

					/* give owner a glassbottle */
					give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(467),
						(char*)consumer + HERO_NAME2);
					break;

				}
				}

				/* print output: maybe garbage - string not set to 0 */
				textbox_width_bak = ds_readw(TEXTBOX_WIDTH);
				ds_writew(TEXTBOX_WIDTH, 5);
				GUI_output(Real2Host(ds_readd(DTP2)));
				ds_writew(TEXTBOX_WIDTH, textbox_width_bak);
			}
		}

		ds_writew(REQUEST_REFRESH, 1);
	} else {
		/* this item cannot be consumed */
		GUI_output(get_ttx(206));
	}
}

#if !defined(__BORLANDC__)
}
#endif
