/*
 *      Rewrite of DSA1 v3.02_de functions of seg048 (status menu)
 *      Functions rewritten: 3/3 (complete)
 *
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg046.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"
#include "seg106.h"
#include "seg107.h"
#include "seg108.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void reset_item_selector(void)
{
	if (ds_readws(STATUS_PAGE_MODE) < 3) {

		/* remove the previous border */
		do_border((RealPt)ds_readd(FRAMEBUF_PTR),
				ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
				ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
				ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
				ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
				30);

		/* mark the actual selected item */
		ds_writeb(STATUSPAGE_SELITEM1_NO, ds_writeb(STATUSPAGE_SELITEM3_NO, 7));

		/* set the new red border */
		do_border((RealPt)ds_readd(FRAMEBUF_PTR),
				ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
				ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
				ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
				ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
				9);

		ds_writebs(STATUSPAGE_SELITEM4_NO, -1);
		ds_writebs(STATUSPAGE_SELITEM2_NO, 23);
	}

	ds_writed(CURRENT_CURSOR, ds_writed(CURRENT_CURSOR_BAK, (Bit32u)RealMake(datseg, DEFAULT_MOUSE_CURSOR)));
}

/* nearly identical, same length */
void status_menu(signed short hero_pos)
{
	signed short l_di;
	signed short flag1;
	signed short flag2;
	signed short width;
	signed short height;
	signed short file_bak;
	Bit8u *hero2;
	Bit8u *hero1;
	signed short flag3;
	signed short l1;
	signed short tw_bak;
	signed short flag4;
	struct nvf_desc nvf;

	flag1 = 0;
	flag2 = 1;
	flag3 = 0;
	flag4 = 0;
	ds_writew(MOUSE1_DOUBLECLICK, 0);

	hero1 = hero2 = get_hero(hero_pos);

	if (!host_readbs(hero2 + HERO_TYPE) || ds_readw(STATUSMENU_ALLOWED) == 0) {
		return;
	}

	/* disable timers */
	inc_ds_ws(TIMERS_DISABLED);

	/* set textbox width */
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writews(TEXTBOX_WIDTH, 3);

	set_audio_track(ARCHIVE_FILE_SUMMARY_XMI);

	file_bak = ds_readws(TEXT_FILE_INDEX);

	load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);

	load_ggsts_nvf();

	ds_writew(REQUEST_REFRESH, 1);
	ds_writew(ACTION, 0);
	ds_writew(STATUS_PAGE_MODE, 1);

	while (flag1 == 0) {

		if (ds_readw(REQUEST_REFRESH) != 0 || flag2 != 0) {

			ds_writew(STATUS_PAGE_HERO, hero_pos);

			update_mouse_cursor();

			status_show(hero_pos);

			ds_writebs(STATUS_PAGE_HUNGER, ds_writebs(STATUS_PAGE_THIRST, -1));

			update_status_bars();

			if (ds_readws(STATUS_PAGE_MODE) < 3) {

				if (hero1 == hero2) {
					/* set the new red border */
					do_border((RealPt)ds_readd(FRAMEBUF_PTR),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
						9);
				}

				if (host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO)) != 0) {

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						/* "%s %s " */
						(char*)(p_datseg + EXTRASPACE_SEPARATED_STRINGS),
						Real2Host(GUI_name_singular((Bit8u*)get_itemname(host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO))))),
						!is_in_word_array(
						    host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						    (signed short*)Real2Host(ds_readd((WEARABLE_ITEMS_INDEX - 4) + 4 * host_readbs(hero2 + HERO_TYPE)))
                        ) ? p_datseg + EMPTY_STRING8 : get_tx2(66));

					if (item_weapon(get_itemsdat(host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO))))) {
						strcat((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(48 + host_readbs(get_itemsdat(host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO))) + 3)));
					}

					GUI_print_string(Real2Host(ds_readd(DTP2)), 16, 192);
				}

				if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {
					/* set the new ??? border */
					do_border((RealPt)ds_readd(FRAMEBUF_PTR),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
						8);
				}
			}

			refresh_screen_size();

			ds_writew(REQUEST_REFRESH, flag2 = 0);
		}

		handle_input();

		/* RIGHT_KEY */
		if (ds_readws(ACTION) == 77 && ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) > 1)
		{

			/* set hero_pos to the next possible hero */
			do {
				hero_pos++;

				if (hero_pos > 6) hero_pos = 0;
			} while (!host_readbs(get_hero(hero_pos) + HERO_TYPE) ||
					host_readbs(get_hero(hero_pos) + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP) ||
					(host_readbs(get_hero(hero_pos) + HERO_TYPE) < 7 && ds_readws(STATUS_PAGE_MODE) > 3));


			if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

				if (flag4 == 0) {
					hero1 = hero2;
				}

				hero2 = get_hero(hero_pos);
				flag4 = 1;
			} else {
				flag4 = 0;

				hero1 = hero2 = get_hero(hero_pos);

				reset_item_selector();
			}

			ds_writew(REQUEST_REFRESH, 1);
		}

		/* LEFT_KEY */
		if (ds_readws(ACTION) == 75 && ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) > 1)
		{

			/* set hero_pos to the next possible hero */
			do {
				hero_pos--;

				if (hero_pos < 0) hero_pos = 6;
			} while (!host_readbs(get_hero(hero_pos) + HERO_TYPE) ||
					host_readbs(get_hero(hero_pos) + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP) ||
					(host_readbs(get_hero(hero_pos) + HERO_TYPE) < 7 && ds_readws(STATUS_PAGE_MODE) > 3));


			if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

				if (flag4 == 0) {
					hero1 = hero2;
				}

				hero2 = get_hero(hero_pos);
				flag4 = 1;
			} else {
				flag4 = 0;

				hero1 = hero2 = get_hero(hero_pos);

				reset_item_selector();
			}

			ds_writew(REQUEST_REFRESH, 1);
		}

		if (ds_readws(STATUS_PAGE_MODE) < 3) {

			/* UP_KEY */
			if (ds_readws(ACTION) == 72) {

				if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

					if (!ds_readbs(STATUSPAGE_SELITEM4_NO)) {
						ds_writeb(STATUSPAGE_SELITEM4_NO, 24);
					} else {
						dec_ds_bs_post(STATUSPAGE_SELITEM4_NO);
					}
				} else {
					if (ds_readbs(STATUSPAGE_SELITEM3_NO) == 0) {
						ds_writeb(STATUSPAGE_SELITEM3_NO, 22);
					} else {
						dec_ds_bs_post(STATUSPAGE_SELITEM3_NO);
					}
				}
			}

			/* DOWN_KEY */
			if (ds_readws(ACTION) == 80) {

				if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

					if (hero1 != hero2) {

						if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 24) {
							ds_writeb(STATUSPAGE_SELITEM4_NO, 0);
						} else {
							inc_ds_bs_post(STATUSPAGE_SELITEM4_NO);
						}
					} else {
						if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 24) {
							ds_writeb(STATUSPAGE_SELITEM4_NO, 0);
						} else {
							inc_ds_bs_post(STATUSPAGE_SELITEM4_NO);
						}
					}
				} else {
					if (ds_readbs(STATUSPAGE_SELITEM3_NO) == 22) {
						ds_writeb(STATUSPAGE_SELITEM3_NO, 0);
					} else {
						inc_ds_bs_post(STATUSPAGE_SELITEM3_NO);
					}
				}
			}

			if (ds_readws(ACTION) >= 128 && ds_readws(ACTION) <= 152)
			{
				if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {
					ds_writeb(STATUSPAGE_SELITEM4_NO, ds_readws(ACTION) + 128);
					ds_writew(ACTION, 28);
				} else if (ds_readws(ACTION) <= 150) {
					ds_writeb(STATUSPAGE_SELITEM3_NO, ds_readws(ACTION) + 128);
					ds_writew(ACTION, 28);
				}
			}

			if (ds_readbs(STATUSPAGE_SELITEM1_NO) != ds_readbs(STATUSPAGE_SELITEM3_NO) && hero1 == hero2) {

				/* set the new ??? border */
				do_border((RealPt)ds_readd(FRAMEBUF_PTR),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM1_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM1_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM1_NO)) + 17,
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM1_NO)) + 17,
					30);
				/* set the new ??? border */
				do_border((RealPt)ds_readd(FRAMEBUF_PTR),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
					9);

				ds_writeb(STATUSPAGE_SELITEM1_NO, ds_readbs(STATUSPAGE_SELITEM3_NO));

				memset(Real2Host(ds_readd(DTP2)), ' ', 60);
				host_writeb(Real2Host(ds_readd(DTP2)) + 60, 0);
				GUI_print_string(Real2Host(ds_readd(DTP2)), 16, 192);

				if (host_readws(hero2 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO))) {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						/* "%s %s " */
						(char*)(p_datseg + EXTRASPACE_SEPARATED_STRINGS2),
						Real2Host(GUI_name_singular((Bit8u*)get_itemname(host_readws(hero2 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO))))),
						!is_in_word_array(
						    host_readws(hero2 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						    (signed short*)Real2Host(ds_readd((WEARABLE_ITEMS_INDEX - 4) + 4 * host_readbs(hero2 + HERO_TYPE)))
                        ) ? p_datseg + EMPTY_STRING9 : get_tx2(66));

					if (item_weapon(get_itemsdat(host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO))))) {
						strcat((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(48 + host_readbs(get_itemsdat(host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO))) + 3)));
					}

					GUI_print_string(Real2Host(ds_readd(DTP2)), 16, 192);
				}
			}

			if (ds_readbs(STATUSPAGE_SELITEM2_NO) != ds_readbs(STATUSPAGE_SELITEM4_NO) && ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

				/* set the new ??? border */
				do_border((RealPt)ds_readd(FRAMEBUF_PTR),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM2_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM2_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM2_NO)) + 17,
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM2_NO)) + 17,
					30);

				if (hero1 == hero2) {

					/* set the new ??? border */
					do_border((RealPt)ds_readd(FRAMEBUF_PTR),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
						9);
				}


				/* set the new ??? border */
				do_border((RealPt)ds_readd(FRAMEBUF_PTR),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
					8);

				ds_writeb(STATUSPAGE_SELITEM2_NO, ds_readbs(STATUSPAGE_SELITEM4_NO));
			}

			if (ds_readws(ACTION) == 28) {
				if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

					if (flag4 != 0) {

						if (ds_readbs(STATUSPAGE_SELITEM4_NO) < 23) {
							pass_item(hero1, ds_readbs(STATUSPAGE_SELITEM3_NO), hero2, ds_readbs(STATUSPAGE_SELITEM4_NO));
							ds_writew(REQUEST_REFRESH, 1);
						} else if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 23) {
							print_item_description(hero1, ds_readbs(STATUSPAGE_SELITEM3_NO));
						} else if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 24) {
							consume(hero1, hero2, ds_readbs(STATUSPAGE_SELITEM3_NO));
						}

						/* set the new ??? border */
						do_border((RealPt)ds_readd(FRAMEBUF_PTR),
							ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
							ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
							ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
							ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
							30);

						flag4 = 0;
						hero1 = hero2;

					} else {
						if (ds_readbs(STATUSPAGE_SELITEM4_NO) < 23) {
							move_item(ds_readbs(STATUSPAGE_SELITEM3_NO), ds_readbs(STATUSPAGE_SELITEM4_NO), hero2);
							ds_writew(REQUEST_REFRESH, 1);
						} else if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 23) {
							print_item_description(hero2, ds_readbs(STATUSPAGE_SELITEM3_NO));
						} else if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 24) {
							consume(hero2, hero2, ds_readbs(STATUSPAGE_SELITEM3_NO));
						}

						/* set the new ??? border */
						do_border((RealPt)ds_readd(FRAMEBUF_PTR),
							ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
							ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
							ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
							ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
							30);
					}

					reset_item_selector();
					flag2 = 1;
				} else {

					ds_writeb(STATUSPAGE_SELITEM4_NO, 23);

					/* set the new ??? border */
					do_border((RealPt)ds_readd(FRAMEBUF_PTR),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
						8);

					if (host_readws(hero2 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO))) {

						nvf.dst = Real2Host(ds_readd(ICON));
						nvf.src = Real2Host(ds_readd(BUFFER10_PTR));
						nvf.type = 0;
						nvf.width = (Bit8u*)&width;
						nvf.height = (Bit8u*)&height;
						nvf.nr = host_readws(get_itemsdat(host_readws(hero2 + HERO_ITEM_HEAD + 14 * ds_readbs(STATUSPAGE_SELITEM3_NO))));
						process_nvf(&nvf);

						make_ggst_cursor(Real2Host(ds_readd(ICON)));

						ds_writed(CURRENT_CURSOR, (Bit32u)RealMake(datseg, GGST_CURSOR));
					}
				}
			}
		}

		/* check if the hero is diseased and print a message */
		if (ds_readws(STATUS_PAGE_MODE) == 1 &&
			ds_readws(ACTION) == 240 &&
			hero_is_diseased(hero2))
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx2(25 + hero_is_diseased(hero2)),
				(char*)(hero2 + HERO_NAME2),
				Real2Host(GUI_get_ptr(host_readbs(hero2 + HERO_SEX), 1)));

			GUI_output(Real2Host(ds_readd(DTP2)));

		}

		if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == 73) {

			ds_writed(CURRENT_CURSOR_BAK, ds_readd(CURRENT_CURSOR));
			ds_writed(CURRENT_CURSOR, (Bit32u)RealMake(datseg, DEFAULT_MOUSE_CURSOR));

			switch (ds_readws(STATUS_PAGE_MODE)) {
			case 1: {
				/* from start-page */

				if (ds_readws(GAME_MODE) == 2) {
					ds_writed(RADIO_NAME_LIST + 0x0, host_readd(Real2Host(ds_readd(TX2_INDEX)) + 0x4c));
					ds_writed((RADIO_NAME_LIST + 0x4), host_readd(Real2Host(ds_readd(TX2_INDEX)) + 0x50));
					ds_writed((RADIO_NAME_LIST + 0x8), host_readd(Real2Host(ds_readd(TX2_INDEX)) + 0x54));

					flag3 = hero_is_diseased(hero2);

					if (flag3 != 0) {
						ds_writed((RADIO_NAME_LIST + 0x0c), host_readd(Real2Host(ds_readd(TX2_INDEX)) + 0x64));
						ds_writed((RADIO_NAME_LIST + 0x10), host_readd(Real2Host(ds_readd(TX2_INDEX)) + 0x3c));
						l1 = 10;
					} else {
						ds_writed((RADIO_NAME_LIST + 0x0c), host_readd(Real2Host(ds_readd(TX2_INDEX)) + 0x3c));
						l1 = 9;
					}
				} else {

					if ((flag3 = hero_is_diseased(hero2))) {
						ds_writed((RADIO_NAME_LIST + 0), host_readd(Real2Host(ds_readd(TX2_INDEX)) + 0x64));
						ds_writed((RADIO_NAME_LIST + 4), host_readd(Real2Host(ds_readd(TX2_INDEX)) + 0x3c));
						l1 = 7;
					} else {
						ds_writed((RADIO_NAME_LIST + 0), host_readd(Real2Host(ds_readd(TX2_INDEX)) + 0x3c));
						l1 = 6;
					}
				}

				l_di = GUI_radio((Bit8u*)NULL, (signed char)l1,
						get_tx2(69),
						get_tx2(16),
						get_tx2(17),
						get_ttx(212),
						get_ttx(213),
						Real2Host(ds_readd((RADIO_NAME_LIST + 0x00))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 0x04))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 0x08))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 0x0c))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 0x10))));

				if (l_di != -1) {
					switch(l_di) {
					case 1: {
						/* change name */
						if (host_readbs(hero2 + HERO_NPC_ID) != 0) {
							GUI_output(get_tx2(71));
						} else {
							GUI_input(get_tx2(70), 15);
							strcpy((char*)hero2 + HERO_NAME2, (char*)Real2Host(ds_readd(TEXT_INPUT_BUF)));
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 2: {
						/* use item */
						if (hero1 != hero2) {
							GUI_output(get_tx2(68));
						} else {
							use_item(ds_readbs(STATUSPAGE_SELITEM3_NO), hero_pos);
							reset_item_selector();
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 3: {
						/* drop item */
						if (hero1 != hero2) {
							GUI_output(get_tx2(68));
						} else {
							drop_item(hero2, ds_readbs(STATUSPAGE_SELITEM3_NO), -1);
							reset_item_selector();
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 4: {
						/* use skill */
						ds_writew(CHEATSKILL_USABLE, 1);
						GUI_use_skill(hero_pos, 0);
						ds_writew(CHEATSKILL_USABLE, 0);

						if (ds_readws(TX_FILE_INDEX) == 19) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
						}
						break;
					}
					case 5: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
							reset_item_selector();
						}
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 6: {
						/* show ATPA values */
						if (l1 == 6) {
							flag1 = 1;
							reset_item_selector();
						} else if (l1 == 7) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_tx2(25 + flag3),
								(char*)(hero2 + HERO_NAME2),
								Real2Host(GUI_get_ptr(host_readbs(hero2 + HERO_SEX), 1)));

								GUI_output(Real2Host(ds_readd(DTP2)));
						} else {
							ds_writew(STATUS_PAGE_MODE, 2);
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 7: {
						/* show skills */
						if (l1 == 7) {
							flag1 = 1;
							reset_item_selector();
						} else {
							reset_item_selector();
							ds_writew(STATUS_PAGE_MODE, 3);
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 8: {
						/* show spells */
						if (host_readbs(hero2 + HERO_TYPE) < 7) {
							GUI_output(get_ttx(215));
						} else {
							reset_item_selector();
							ds_writew(STATUS_PAGE_MODE, 4);
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 9: {
						if (flag3 != 0) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_tx2(25 + flag3),
								(char*)(hero2 + HERO_NAME2),
								Real2Host(GUI_get_ptr(host_readbs(hero2 + HERO_SEX), 1)));

								GUI_output(Real2Host(ds_readd(DTP2)));
								break;
						}
					}
					case 10: {
						flag1 = 1;
						reset_item_selector();
						break;
					}
					}
				}

				break;
			}
			case 2: {
				/* from ATPA-page */

				l_di = GUI_radio((Bit8u*)0, 8,
						get_tx2(16),
						get_tx2(17),
						get_ttx(212),
						get_ttx(213),
						get_tx2(24),
						get_tx2(20),
						get_tx2(21),
						get_tx2(15));

				if (l_di != -1) {
					switch(l_di) {
					case 1: {
						/* use item */
						if (hero1 != hero2) {
							GUI_output(get_tx2(68));
						} else {
							use_item(ds_readbs(STATUSPAGE_SELITEM3_NO), hero_pos);
							reset_item_selector();
						}
						break;
					}
					case 2: {
						/* drop item */
						if (hero1 != hero2) {
							GUI_output(get_tx2(68));
						} else {
							drop_item(hero2, ds_readbs(STATUSPAGE_SELITEM3_NO), -1);
							reset_item_selector();
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 3: {
						/* use skill */
						ds_writew(CHEATSKILL_USABLE, 1);
						GUI_use_skill(hero_pos, 0);
						ds_writew(CHEATSKILL_USABLE, 0);

						if (ds_readws(TX_FILE_INDEX) == 19) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
						}
						break;
					}
					case 4: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
							reset_item_selector();
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 5: {
						/* TODO: different code is generated here */
						ds_writew(STATUS_PAGE_MODE, 1);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 6: {
						reset_item_selector();
						ds_writew(STATUS_PAGE_MODE, 3);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 7: {
						if (host_readbs(hero2 + HERO_TYPE) < 7) {
							GUI_output(get_ttx(215));
						} else {
							reset_item_selector();
							ds_writew(STATUS_PAGE_MODE, 4);
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 8: {
						flag1 = 1;
						reset_item_selector();
						break;
					}
					}
				}
				break;
			}
			case 3: {
				/* from skills-page */
				l_di = GUI_radio((Bit8u*)0, 6,
						get_ttx(212),
						get_tx2(24),
						get_tx2(19),
						get_tx2(21),
						get_ttx(213),
						get_tx2(15));

				if (l_di != -1) {
					switch (l_di) {
					case 1: {
						/* use skill */
						ds_writew(CHEATSKILL_USABLE, 1);
						GUI_use_skill(hero_pos, 0);
						ds_writew(CHEATSKILL_USABLE, 0);

						if (ds_readws(TX_FILE_INDEX) == 19) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
						}
						break;
					}
					case 2: {
						ds_writew(STATUS_PAGE_MODE, 1);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 3: {
						ds_writew(STATUS_PAGE_MODE, 2);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 4: {
						if (host_readbs(hero2 + HERO_TYPE) < 7) {
							GUI_output(get_ttx(215));
						} else {
							ds_writew(STATUS_PAGE_MODE, 4);
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 5: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 6: {
						flag1 = 1;
						break;
					}
					}
				}
				break;
			}
			case 4:
			case 5:{
				/* from spells-page */
				l_di = GUI_radio((Bit8u*)0, 6,
						get_ttx(213),
						get_tx2(24),
						get_tx2(19),
						get_tx2(20),
						get_ttx(214),
						get_tx2(15));

				if (l_di != -1) {
					switch (l_di) {
					case 1: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 2: {
						ds_writew(STATUS_PAGE_MODE, 1);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 3: {
						ds_writew(STATUS_PAGE_MODE, 2);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 4: {
						ds_writew(STATUS_PAGE_MODE, 3);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 5: {
						if (ds_readws(STATUS_PAGE_MODE) == 4)
							ds_writew(STATUS_PAGE_MODE, 5);
						else
							ds_writew(STATUS_PAGE_MODE, 4);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 6: {
						flag1 = 1;
						break;
					}
					}
				}
				break;
			}
			}

			ds_writed(CURRENT_CURSOR, ds_readd(CURRENT_CURSOR_BAK));
		}
	}

	/* restore text file except for CHARTEXT.LTX, TAVERN.TLK and except for dialogs */
	if (file_bak != -1 &&
		file_bak != ARCHIVE_FILE_CHARTEXT_LTX &&
		file_bak != ARCHIVE_FILE_TAVERN_TLK &&
		(file_bak < 156 || file_bak > 176))
	{
		load_tx2(file_bak);
	}

	ds_writew(REQUEST_REFRESH, 1);
	ds_writew(TEXTBOX_WIDTH, tw_bak);
	dec_ds_ws(TIMERS_DISABLED);

	if (ds_readbs(CURRENT_TOWN) != 0) {
		ds_writeb(FADING_STATE, 3);
	}
}

/* Borlandified and identical */
void status_select_hero(void)
{

	if (ds_readws(STATUSMENU_ALLOWED) == 0 || !ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
		/* Yes, it was written that way! */
	} else {
		signed short hero_pos = select_hero_from_group(get_ttx(301));

		if (hero_pos != -1) {
			status_menu(hero_pos);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
