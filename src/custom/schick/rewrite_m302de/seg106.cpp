/*
	Rewrite of DSA1 v3.02_de functions of seg106 (inventory misc)
	Functions rewritten: 8/8 (complete)
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
 * \brief   check for a two hand collision
 *
 *          A hero can not equip a two-handed weapon if one hand is in use,
 *          nor can he equip something in his free hand if he has a two-handed weapon
 *          in the other.
 *          If such a collision is detected this function returns 1 else 0.
 *
 * \param   hero        the hero
 * \param   item        the item which should be equipped
 * \param   pos         the position the item should be placed
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
		in_hand = host_readw(hero + other_pos * 0x0e + HERO_ITEM_HEAD);
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
			item1 = host_readws(hero + HERO_ITEM_HEAD + pos1  * SIZEOF_KS_ITEM);
			item2 = host_readws(hero + HERO_ITEM_HEAD + pos2  * SIZEOF_KS_ITEM);
		} else {
			item1 = host_readws(hero + HERO_ITEM_HEAD + pos1  * SIZEOF_KS_ITEM);
			item2 = host_readws(hero + HERO_ITEM_HEAD + pos2  * SIZEOF_KS_ITEM);

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
				GUI_output(get_ttx(209));
			} else {
				if (item2 != 0) {
					/* item have the same ids and are stackable */
					if ((item2 == item1) && item_stackable(get_itemsdat(item1))) {
						/* merge them */

						/* add counter from item at pos2 to item at pos1 */
						add_ks_counter(pos1, pos2, hero);

						/* delete item at pos2 */
						memset(hero + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM,
							0, SIZEOF_KS_ITEM);
#ifdef M302de_ORIGINAL_BUGFIX
						/* Decrement the item counter */
						dec_ptr_bs(hero + HERO_KS_TAKEN);
#endif
					} else {
						if (!can_hero_use_item(hero, item2)) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(221),
								(char*)hero + HERO_NAME2,
								(char*)get_ttx((host_readbs(hero + HERO_SEX) != 0 ? 593 : 9) + host_readbs(hero + HERO_TYPE)),
								(char*)Real2Host(GUI_names_grammar(2, item2, 0)));


							GUI_output(Real2Host(ds_readd(DTP2)));
						} else {
							if (!can_item_at_pos(item2, pos1)) {
								if (is_in_word_array(item2, (signed short*)(p_datseg + ITEMS_PLURALWORDS)))
									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ttx(222),
										(char*)Real2Host(GUI_names_grammar(0x4000, item2, 0)),
										(char*)get_ttx(557));
								else
									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ttx(222),
										(char*)Real2Host(GUI_names_grammar(0, item2, 0)),
										(char*)get_ttx(556));
								GUI_output(Real2Host(ds_readd(DTP2)));
							} else {
								if (two_hand_collision(hero, item2, pos1)) {

									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ttx(829),
										(char*)hero + HERO_NAME2);

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
				memset(hero + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM,
							0, SIZEOF_KS_ITEM);
#ifdef M302de_ORIGINAL_BUGFIX
				/* Decrement the item counter */
				dec_ptr_bs(hero + HERO_KS_TAKEN);
#endif
			} else {

#if !defined(__BORLANDC__)
				struct_copy((Bit8u*)&tmp, hero + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM, SIZEOF_KS_ITEM);
				struct_copy(hero + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM,
					hero + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM, SIZEOF_KS_ITEM);
				struct_copy(hero + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM, (Bit8u*)&tmp, SIZEOF_KS_ITEM);
#else
				/* exchange the items */
				tmp = *(struct knapsack_item*)(hero + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM);

				*(struct knapsack_item*)(hero + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM) =
					*(struct knapsack_item*)(hero + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM);
				*(struct knapsack_item*)(hero + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM) = tmp;
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
	item_p = hero + HERO_ITEM_HEAD + pos * 14;

	if (host_readw(item_p) != 0) {
		/* normal item */

		if ((((signed short)host_readw(item_p + 2) > 1) &&
			item_stackable(get_itemsdat(host_readw(item_p)))) ||
			is_in_word_array(host_readw(item_p), (signed short*)(p_datseg + ITEMS_PLURALWORDS))) {
			/* more than one item or special */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx2(72),
				get_ttx(305),
				Real2Host(GUI_names_grammar(0x4004, host_readw(item_p), 0)));
		} else {
			/* one item */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx2(11),
				get_ttx(304),
				Real2Host(GUI_names_grammar(0, host_readw(item_p), 0)));
		}
	} else {
		/* no item */
		strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(209));
	}


	/* broken */
	if (ks_broken(item_p)) {
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(478));
	}

	/* magic */
	if (ks_magic_hidden(item_p) &&	/* is magic */
		ks_magic_known(item_p)) { /* and you know it */
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(479));
	}

	/* used */
	if (host_readb(item_p + 7) != 0) {
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(480));
	}

	/* poisoned */
	if (host_readw(item_p) == 0xc1 || host_readw(item_p) == 0xc8 ||
		ks_poison1(item_p) || ks_poison2(item_p) ||
		host_readb(hero + HERO_ITEM_HEAD + 9 + pos * 14) != 0) {
		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(548));
	}

	/* staff */
	if (host_readw(item_p) == 0x85) {
		sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
			(char*)get_tx2(53),
			host_readbs(hero + HERO_STAFFSPELL_LVL));
		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
	}

	GUI_output(Real2Host(ds_readd(DTP2)));
}

/* Borlandified and nearly identical */
void pass_item(Bit8u *hero1, signed short old_pos1, Bit8u *hero2, signed short pos2)
{
	signed short l_di;
	register signed short pos1 = old_pos1;

	signed short item1;
	signed short item2;
	Bit8u *item1_desc;
	Bit8u *item2_desc;
	signed short flag;
	signed short desc1_5;
	signed short desc2_5;
	struct knapsack_item tmp;


	item1 = host_readws(hero1 + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM);

	/* check if item1 is an item */
	if (item1 == 0) {
		GUI_output(get_ttx(209));
		return;
	}

	item2 = host_readws(hero2 + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM);

	item1_desc = get_itemsdat(item1);
	item2_desc = get_itemsdat(item2);

	if (item_undropable(item1_desc)) {

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(454),
			(char*)Real2Host(GUI_names_grammar((signed short)0x8002, item1, 0)));

		GUI_output(Real2Host(ds_readd(DTP2)));
		return;
	}

	if (item_undropable(item2_desc)) {

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(454),
			(char*)Real2Host(GUI_names_grammar((signed short)0x8002, item2, 0)));

		GUI_output(Real2Host(ds_readd(DTP2)));
		return;

	}

	/* identical until here */
	if (pos2 < 7) {
		if (!can_hero_use_item(hero2, item1)) {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(221),
				(char*)(hero2 + HERO_NAME2),
				(char*)get_ttx((host_readbs(hero2 + HERO_SEX) ? 593 : 9) + host_readbs(hero2 + HERO_TYPE)),
				(char*)Real2Host(GUI_names_grammar(2, item1, 0)));


			GUI_output(Real2Host(ds_readd(DTP2)));
			return;

		} else if (!can_item_at_pos(item1, pos2)) {

			if (is_in_word_array(item1, (signed short*)(p_datseg + ITEMS_PLURALWORDS))) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(222),
					(char*)Real2Host(GUI_names_grammar(0x4000, item1, 0)),
					(char*)get_ttx(557));
			} else {
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(222),
					(char*)Real2Host(GUI_names_grammar(0, item1, 0)),
					(char*)get_ttx(556));
			}

			GUI_output(Real2Host(ds_readd(DTP2)));
			return;

		} else if (two_hand_collision(hero2, item1, pos2)) {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx2(67),
				(char*)(hero2 + HERO_NAME2));

			GUI_output(Real2Host(ds_readd(DTP2)));
			return;
		}
	}
#if defined(__BORLANDC__)
	/* this assembler code here is only for comparization the disassemblies */
		asm { db 0x9a, 0xff, 0xff, 0x00, 0x00 }
		asm { db 0x9a, 0xff, 0xff, 0x00, 0x00 }
#endif

/* 0x8ff */

	if ((item2 != 0) && (pos1 < 7)) {

		if (!can_hero_use_item(hero1, item2)) {
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(221),
				(char*)(hero1 + HERO_NAME2),
				(char*)get_ttx((host_readbs(hero1 + HERO_SEX) ? 593 : 9) + host_readbs(hero1 + HERO_TYPE)),
				(char*)Real2Host(GUI_names_grammar(2, item2, 0)));

#if defined(__BORLANDC__)
			desc1_5 = desc1_5;
#endif

			GUI_output(Real2Host(ds_readd(DTP2)));
			return;

		} else if (!can_item_at_pos(item2, pos1)) {

			if (is_in_word_array(item2, (signed short*)(p_datseg + ITEMS_PLURALWORDS))) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(222),
					(char*)Real2Host(GUI_names_grammar(0x4000, item2, 0)),
					(char*)get_ttx(557));
			} else {
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(222),
					(char*)Real2Host(GUI_names_grammar(0, item2, 0)),
					(char*)get_ttx(556));
			}

			GUI_output(Real2Host(ds_readd(DTP2)));
			return;
		}
	}

/* 0xa14 */

	/* identical from here */
	if (item2 != 0) {

		flag = 1;
		if ((item2 == item1) && item_stackable(item2_desc)) {

			flag = 0;
			l_di = 1;

			if (host_readws(hero1 + (HERO_ITEM_HEAD+2) + pos1 * SIZEOF_KS_ITEM) > 1) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(210),
					host_readws(hero1 + (HERO_ITEM_HEAD+2) + pos1 * SIZEOF_KS_ITEM),
					(char*)Real2Host(GUI_names_grammar(6, item1, 0)),
					(char*)hero2 + HERO_NAME2);


				l_di = GUI_input(Real2Host(ds_readd(DTP2)), 2);
			}

			if (host_readws(hero1 + (HERO_ITEM_HEAD+2) + pos1 * SIZEOF_KS_ITEM) < l_di) {
				l_di = host_readws(hero1 + (HERO_ITEM_HEAD+2) + pos1 * SIZEOF_KS_ITEM);
			}

			if ((l_di > 0) && (host_readws(hero2 + (HERO_ITEM_HEAD+2) + pos2 * SIZEOF_KS_ITEM) < 99)) {

				if (host_readws(hero2 + (HERO_ITEM_HEAD+2) + pos2 * SIZEOF_KS_ITEM) + l_di > 99) {
					l_di = 99 - host_readws(hero2 + (HERO_ITEM_HEAD+2) + pos2 * SIZEOF_KS_ITEM);
				}

				while ((host_readbs(hero2 + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 100 <= host_readws(hero2 + HERO_LOAD) + host_readws(item1_desc + 5) * l_di) && l_di > 0) {
					l_di--;
				}

				if (l_di > 0) {
					add_ptr_ws(hero2 + HERO_LOAD, host_readws(item1_desc + 5) * l_di);
					add_ptr_ws(hero2 + (HERO_ITEM_HEAD+2) + pos2 * SIZEOF_KS_ITEM, l_di);
					drop_item(hero1, pos1, l_di);
				} else {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(779),
						(char*)hero2 + HERO_NAME2);
					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			}
		}

		if (flag != 0) {

			desc1_5 = (item_stackable(item1_desc)) ?
				host_readws(hero1 + (HERO_ITEM_HEAD+2) + pos1 * SIZEOF_KS_ITEM) * host_readw(item1_desc + 5) :
				host_readws(item1_desc + 5);

			desc2_5 = (item_stackable(item2_desc)) ?
				host_readws(hero2 + (HERO_ITEM_HEAD+2) + pos2 * SIZEOF_KS_ITEM) * host_readw(item2_desc + 5) :
				host_readws(item2_desc + 5);

			if (host_readbs(hero2 + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 100 <= host_readws(hero2 + HERO_LOAD) + desc1_5 - desc2_5) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(779),
					(char*)hero2 + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			} else {
				if (pos1 < 7) {
					unequip(hero1, item1, pos1);
					add_equip_boni(hero2, hero1, item2, pos2, pos1);
				}

				if (pos2 < 7) {
					unequip(hero2, item2, pos2);
					add_equip_boni(hero1, hero2, item1, pos1, pos2);
				}

				/* exchange two items */
#if !defined(__BORLANDC__)
				struct_copy((Bit8u*)&tmp, hero2 + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM, SIZEOF_KS_ITEM);
#else
				tmp = *(struct knapsack_item*)(hero2 + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM);
#endif

				sub_ptr_ws(hero2 + HERO_LOAD, desc2_5);

#if !defined(__BORLANDC__)
				struct_copy(hero2 + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM,
						hero1 + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM,
						SIZEOF_KS_ITEM);
#else
				*(struct knapsack_item*)(hero2 + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM) =
					*(struct knapsack_item*)(hero1 + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM);
#endif

				add_ptr_ws(hero2 + HERO_LOAD, desc1_5);
				sub_ptr_ws(hero1 + HERO_LOAD, desc1_5);

#if !defined(__BORLANDC__)
				struct_copy(hero1 + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM,
						(Bit8u*)&tmp,
						SIZEOF_KS_ITEM);
#else
				*(struct knapsack_item*)(hero1 + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM) = tmp;
#endif

				add_ptr_ws(hero1 + HERO_LOAD, desc2_5);

				/* item counter */
				dec_ptr_bs(hero1 + HERO_KS_TAKEN);
				inc_ptr_bs(hero2 + HERO_KS_TAKEN);

				/* special items */
				if (item2 == 0xa1) {
					host_writeb(hero1 + (HERO_TA_NATURE+3), host_readbs(hero1 + (HERO_TA_NATURE+3)) + 3);
					host_writeb(hero2 + (HERO_TA_NATURE+3), host_readbs(hero2 + (HERO_TA_NATURE+3)) + -3);
				}
				if (item2 == 0xa3) {
					host_writeb(hero1 + HERO_MR, host_readbs(hero1 + HERO_MR) + 5);
					host_writeb(hero2 + HERO_MR, host_readbs(hero2 + HERO_MR) + -5);
				}
				if (item1 == 0xa1) {
					host_writeb(hero1 + (HERO_TA_NATURE+3), host_readbs(hero1 + (HERO_TA_NATURE+3)) + -3);
					host_writeb(hero2 + (HERO_TA_NATURE+3), host_readbs(hero2 + (HERO_TA_NATURE+3)) + 3);
				}
				if (item1 == 0xa3) {
					host_writeb(hero1 + HERO_MR, host_readbs(hero1 + HERO_MR) + -5);
					host_writeb(hero2 + HERO_MR, host_readbs(hero2 + HERO_MR) + 5);
				}
			}
		}
	} else if (item_stackable(item1_desc)) {

		l_di = 1;

		if (host_readws(hero1 + (HERO_ITEM_HEAD+2) + pos1 * SIZEOF_KS_ITEM) > 1) {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(210),
				host_readws(hero1+ (HERO_ITEM_HEAD + 2) + pos1 * SIZEOF_KS_ITEM),
				(char*)Real2Host(GUI_names_grammar(6, item1, 0)),
				(char*)hero2 + HERO_NAME2);


			l_di = GUI_input(Real2Host(ds_readd(DTP2)), 2);
		}

		if (host_readws(hero1 + (HERO_ITEM_HEAD+2) + pos1 * SIZEOF_KS_ITEM) < l_di) {
			l_di = host_readws(hero1 + (HERO_ITEM_HEAD+2) + pos1 * SIZEOF_KS_ITEM);
		}

		while ((host_readbs(hero2 + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 100 <= host_readws(hero2 + HERO_LOAD) + host_readws(item1_desc + 5) * l_di) && l_di > 0) {
			l_di--;
		}

		if (l_di > 0) {
#if !defined(__BORLANDC__)
			struct_copy(hero2 + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM,
				hero1 + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM, SIZEOF_KS_ITEM);
#else
			*(struct knapsack_item*)(hero2 + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM) =
					*(struct knapsack_item*)(hero1 + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM);
#endif
			add_ptr_ws(hero2 + HERO_LOAD, host_readws(item1_desc + 5) * l_di);
			host_writews(hero2 + (HERO_ITEM_HEAD+2) + pos2 * SIZEOF_KS_ITEM, l_di);
			drop_item(hero1, pos1, l_di);

		} else {
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(779),
				(char*)hero2 + HERO_NAME2);
			GUI_output(Real2Host(ds_readd(DTP2)));
		}
	} else if (host_readbs(hero2 + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 100 <= host_readws(hero2 + HERO_LOAD) + host_readws(item1_desc + 5)) {
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(779),
			(char*)hero2 + HERO_NAME2);
		GUI_output(Real2Host(ds_readd(DTP2)));
	} else {

		/* do the change */

		if (pos1 < 7) {
			unequip(hero1, item1, pos1);
		}

		if (pos2 < 7) {
			add_equip_boni(hero1, hero2, item1, pos1, pos2);
		}

#if !defined(__BORLANDC__)
		struct_copy(hero2 + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM,
			hero1 + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM, SIZEOF_KS_ITEM);
#else
		*(struct knapsack_item*)(hero2 + HERO_ITEM_HEAD + pos2 * SIZEOF_KS_ITEM) =
			*(struct knapsack_item*)(hero1 + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM);
#endif
		/* adjust weight */
		add_ptr_ws(hero2 + HERO_LOAD, host_readws(item1_desc + 5));
		sub_ptr_ws(hero1 + HERO_LOAD, host_readws(item1_desc + 5));

		/* adjust item counter */
		dec_ptr_bs(hero1 + HERO_KS_TAKEN);
		inc_ptr_bs(hero2 + HERO_KS_TAKEN);

		/* clear slot */
		memset(hero1 + HERO_ITEM_HEAD + pos1 * SIZEOF_KS_ITEM, 0, SIZEOF_KS_ITEM);

		/* special items */
		if (item1 == 0xa1) {
			host_writeb(hero1 + (HERO_TA_NATURE+3), host_readbs(hero1 + (HERO_TA_NATURE+3)) + -3);
			host_writeb(hero2 + (HERO_TA_NATURE+3), host_readbs(hero2 + (HERO_TA_NATURE+3)) + 3);
		}
		if (item1 == 0xa3) {
			host_writeb(hero1 + HERO_MR, host_readbs(hero1 + HERO_MR) + -5);
			host_writeb(hero2 + HERO_MR, host_readbs(hero2 + HERO_MR) + 5);
		}
	}
}

struct items_all {
	signed short a[4];
};

/* Borlandified and identical */
void startup_equipment(Bit8u *hero)
{
	signed short i;
	struct items_all all;
#if !defined(__BORLANDC__)
	all.a[0] = 30;
	all.a[1] = 45;
	all.a[2] = 45;
	all.a[3] = 49;
#else
	*(struct items_all*)&all = *(struct items_all*)(p_datseg + HERO_STARTUP_ITEMS_ALL);
#endif

	for (i = 0; i < 4; i++) {
		give_hero_new_item(hero, all.a[i], 1, 1);
	}

	move_item(5, 9, hero);

	if (host_readbs(hero + HERO_SEX) != 0 && host_readbs(hero + HERO_TYPE) != HERO_TYPE_WARRIOR && host_readbs(hero + HERO_TYPE) != HERO_TYPE_MAGE) {
		give_hero_new_item(hero, ITEM_SHIRT, 1, 1);
		move_item(2, 9, hero);
	}

	i = 0;
	while (ds_readws((HERO_STARTUP_ITEMS-8) + 8 * host_readbs(hero + HERO_TYPE) + 2 * i) != -1 && (i < 4)) {

		give_hero_new_item(hero, ds_readws((HERO_STARTUP_ITEMS-8) + 8 * host_readbs(hero + HERO_TYPE) + 2 * i++), 1, 1);

		if (i == 1) {
			move_item(3, 9, hero);
		}
	}

	if (host_readbs(hero + HERO_TYPE) == HERO_TYPE_WARRIOR) {
		move_item(2, get_item_pos(hero, ITEM_ARMOR_LEATHER), hero);
	}

	if (host_readbs(hero + HERO_TYPE) == HERO_TYPE_MAGE) {
		move_item(2, get_item_pos(hero, ITEM_ROBE_GREEN), hero);
	}

	if (host_readbs(hero + HERO_TYPE) == HERO_TYPE_HUNTER ||
		host_readbs(hero + HERO_TYPE) == HERO_TYPE_GREEN_ELF ||
		host_readbs(hero + HERO_TYPE) == HERO_TYPE_SYLVAN_ELF)
	{
		give_hero_new_item(hero, ITEM_ARROWS, 1, 20);
		move_item(4, get_item_pos(hero, ITEM_ARROWS), hero);
	}
}

/**
 * \brief   get the maximum time of a burning lightsource
 */
/* Borlandified and identical */
signed short get_max_light_time(void)
{
	Bit8u *hero;
	signed short i;
	signed short j;
	signed short retval;

	retval = -1;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

#ifdef M302de_ORIGINAL_BUGFIX
		if (!host_readb(hero + HERO_TYPE))
			continue;
#endif

		for (j = 0; j < 23; j++) {

			/* search for a burning torch */
			if (host_readw(hero + HERO_ITEM_HEAD + j * 14) == ITEM_TORCH_ON) {

				if (host_readbs(hero + j * 14 + HERO_ITEM_HEAD + 8) > retval) {
					retval = host_readbs(hero + j * 14 + HERO_ITEM_HEAD + 8);
				}
			} else if (host_readw(hero + HERO_ITEM_HEAD + j * 14) == ITEM_LANTERN_ON) {
				/* search for a burning lantern */

				if (host_readbs(hero + j * 14 + HERO_ITEM_HEAD + 8) / 10 > retval) {
					retval = host_readbs(hero + j * 14 + HERO_ITEM_HEAD + 8) / 10;
				}
			}
		}
	}

	return retval;
}

/**
 * \brief   shows an animation of a man equipping a belt
 *
 *          This is only executed when equipping a skullbelt.
 */
/* Borlandified and identical */
void equip_belt_ani(void)
{
	signed short i;
	signed short handle;

	signed short width;
	signed short height;
	Bit32s nvf_length;
	Bit8u *p_pal;
	struct nvf_desc nvf;

	/* open GUERTEL.NVF */
	handle = load_archive_file(ARCHIVE_FILE_GUERTEL_NVF);

	/* read NVF part 1 */
	nvf_length = read_archive_file(handle,
			Real2Host(ds_readd(BUFFER9_PTR)), 64000);
	/* read NVF part 2 */
	nvf_length += read_archive_file(handle, Real2Host(F_PADD(ds_readd(BUFFER9_PTR), 64000)), 64000);

	bc_close(handle);

	/* calculate palette pointer */
	p_pal = Real2Host(F_PADD(F_PADD(ds_readd(BUFFER9_PTR), nvf_length), -0x60));

	wait_for_vsync();

	set_palette(p_pal, 0x80, 0x20);

	do_border((RealPt)ds_readd(FRAMEBUF_PTR), 209, 79, 215, 89, 9);

	do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 209, 79, 215, 89, 0);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 189, 69, 235, 99, 0);

	do_border((RealPt)ds_readd(FRAMEBUF_PTR), 189, 69, 235, 99, 9);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 169, 59, 255, 109, 0);

	do_border((RealPt)ds_readd(FRAMEBUF_PTR), 169, 59, 255, 109, 9);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 164, 54, 260, 114, 0);

	do_border((RealPt)ds_readd(FRAMEBUF_PTR), 164, 54, 260, 114, 9);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 159, 49, 263, 117, 0);

	do_border((RealPt)ds_readd(FRAMEBUF_PTR), 159, 49, 263, 117, 9);

	for (i = 0; i < 12; i++) {

		nvf.dst = Real2Host(ds_readd(RENDERBUF_PTR));
		nvf.src = Real2Host(ds_readd(BUFFER9_PTR));
		nvf.no = i;
		nvf.type = 3;
		nvf.width = (Bit8u*)&width;
		nvf.height = (Bit8u*)&height;

		process_nvf(&nvf);
#if !defined(__BORLANDC__)
		/* BE-fix */
		width = host_readws((Bit8u*)&width);
		height = host_readws((Bit8u*)&height);
#endif

		ds_writew(PIC_COPY_X1, 160);
		ds_writew(PIC_COPY_Y1, 50);
		ds_writew(PIC_COPY_X2, width + 159);
		ds_writew(PIC_COPY_Y2, height + 49);
		ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));

		wait_for_vsync();
		wait_for_vsync();
		wait_for_vsync();
		update_mouse_cursor();
		wait_for_vsync();

		do_pic_copy(0);

		refresh_screen_size();
	}

	ds_writew(CURRENT_ANI, ds_writebs(DNG_AREA_LOADED, ds_writebs(CITY_AREA_LOADED, -1)));
}

/**
 * \brief   search for a full waterskin
 *
 * \param   hero        pointer to the hero
 * \return              position of a non-empty waterskin
 */
/* Borlandified and identical */
signed short get_full_waterskin_pos(Bit8u *hero)
{
	signed short pos = -1;
	signed short i;

	/* Original-BUG: should start from inventory pos 0 */
	for (i = 7; i < 23; i++) {

		/* look for a non-empty waterskin */
		if ((host_readw(hero + HERO_ITEM_HEAD + i * 14) == 30) &&
			!ks_empty(hero + HERO_ITEM_HEAD + i * 14))
		{
			pos = i;
			break;
		}
	}

	return pos;
}

#if !defined(__BORLANDC__)
}
#endif
