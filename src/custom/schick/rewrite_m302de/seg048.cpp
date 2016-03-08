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
	if (ds_readws(0x2c9b) < 3) {

		/* remove the previous border */
		do_border((RealPt)ds_readd(0xd2ff),
				ds_readws(0x64ce + 4 * ds_readbs(0x6370)),
				ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)),
				ds_readws(0x64ce + 4 * ds_readbs(0x6370)) + 17,
				ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)) + 17,
				30);

		/* mark the actual selected item */
		ds_writeb(0x636e, ds_writeb(0x6370, 7));

		/* set the new red border */
		do_border((RealPt)ds_readd(0xd2ff),
				ds_readws(0x64ce + 4 * ds_readbs(0x6370)),
				ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)),
				ds_readws(0x64ce + 4 * ds_readbs(0x6370)) + 17,
				ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)) + 17,
				9);

		ds_writebs(0x6371, -1);
		ds_writebs(0x636f, 23);
	}

	ds_writed(0xcecb, ds_writed(0xe3ae, (Bit32u)RealMake(datseg, 0x2848)));
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
	ds_writew(0xc3cf, 0);

	hero1 = hero2 = get_hero(hero_pos);

	if (!host_readbs(hero2 + HERO_TYPE) || ds_readw(0x29b2) == 0) {
		return;
	}

	/* disable timers */
	inc_ds_ws(TIMERS_DISABLED);

	/* set textbox width */
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writews(TEXTBOX_WIDTH, 3);

	set_audio_track(ARCHIVE_FILE_SUMMARY_XMI);

	file_bak = ds_readws(0x26bd);

	load_city_ltx(ARCHIVE_FILE_CHARTEXT_LTX);

	load_ggsts_nvf();

	ds_writew(0x2846, 1);
	ds_writew(ACTION, 0);
	ds_writew(0x2c9b, 1);

	while (flag1 == 0) {

		if (ds_readw(0x2846) != 0 || flag2 != 0) {

			ds_writew(0x2c9d, hero_pos);

			update_mouse_cursor();

			status_show(hero_pos);

			ds_writebs(0x2c9f, ds_writebs(0x2ca0, -1));

			update_status_bars();

			if (ds_readws(0x2c9b) < 3) {

				if (hero1 == hero2) {
					/* set the new red border */
					do_border((RealPt)ds_readd(0xd2ff),
						ds_readws(0x64ce + 4 * ds_readbs(0x6370)),
						ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)),
						ds_readws(0x64ce + 4 * ds_readbs(0x6370)) + 17,
						ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)) + 17,
						9);
				}

				if (host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370)) != 0) {

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						/* "%s %s " */
						(char*)(p_datseg + 0x6534),
						Real2Host(GUI_name_singular((Bit8u*)get_itemname(host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370))))),
						!is_in_word_array(host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370)), (signed short*)Real2Host(ds_readd(0x634 + 4 * host_readbs(hero2 + HERO_TYPE)))) ? p_datseg + 0x653b : get_city(0x108));

					if (item_weapon(get_itemsdat(host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370))))) {
						strcat((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(4 * (48 + host_readbs(get_itemsdat(host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370))) + 3))));
					}

					GUI_print_string(Real2Host(ds_readd(DTP2)), 16, 192);
				}

				if (ds_readbs(0x6371) != -1) {
					/* set the new ??? border */
					do_border((RealPt)ds_readd(0xd2ff),
						ds_readws(0x64ce + 4 * ds_readbs(0x6371)),
						ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6371)),
						ds_readws(0x64ce + 4 * ds_readbs(0x6371)) + 17,
						ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6371)) + 17,
						8);
				}
			}

			refresh_screen_size();

			ds_writew(0x2846, flag2 = 0);
		}

		handle_input();

		/* RIGHT_KEY */
		if (ds_readws(ACTION) == 77 && ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP)) > 1)
		{

			/* set hero_pos to the next possible hero */
			do {
				hero_pos++;

				if (hero_pos > 6) hero_pos = 0;
			} while (!host_readbs(get_hero(hero_pos) + HERO_TYPE) ||
					host_readbs(get_hero(hero_pos) + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP) ||
					(host_readbs(get_hero(hero_pos) + HERO_TYPE) < 7 && ds_readws(0x2c9b) > 3));


			if (ds_readbs(0x6371) != -1) {

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

			ds_writew(0x2846, 1);
		}

		/* LEFT_KEY */
		if (ds_readws(ACTION) == 75 && ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP)) > 1)
		{

			/* set hero_pos to the next possible hero */
			do {
				hero_pos--;

				if (hero_pos < 0) hero_pos = 6;
			} while (!host_readbs(get_hero(hero_pos) + HERO_TYPE) ||
					host_readbs(get_hero(hero_pos) + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP) ||
					(host_readbs(get_hero(hero_pos) + HERO_TYPE) < 7 && ds_readws(0x2c9b) > 3));


			if (ds_readbs(0x6371) != -1) {

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

			ds_writew(0x2846, 1);
		}

		if (ds_readws(0x2c9b) < 3) {

			/* UP_KEY */
			if (ds_readws(ACTION) == 72) {

				if (ds_readbs(0x6371) != -1) {

					if (!ds_readbs(0x6371)) {
						ds_writeb(0x6371, 24);
					} else {
						dec_ds_bs_post(0x6371);
					}
				} else {
					if (ds_readbs(0x6370) == 0) {
						ds_writeb(0x6370, 22);
					} else {
						dec_ds_bs_post(0x6370);
					}
				}
			}

			/* DOWN_KEY */
			if (ds_readws(ACTION) == 80) {

				if (ds_readbs(0x6371) != -1) {

					if (hero1 != hero2) {

						if (ds_readbs(0x6371) == 24) {
							ds_writeb(0x6371, 0);
						} else {
							inc_ds_bs_post(0x6371);
						}
					} else {
						if (ds_readbs(0x6371) == 24) {
							ds_writeb(0x6371, 0);
						} else {
							inc_ds_bs_post(0x6371);
						}
					}
				} else {
					if (ds_readbs(0x6370) == 22) {
						ds_writeb(0x6370, 0);
					} else {
						inc_ds_bs_post(0x6370);
					}
				}
			}

			if (ds_readws(ACTION) >= 128 && ds_readws(ACTION) <= 152)
			{
				if (ds_readbs(0x6371) != -1) {
					ds_writeb(0x6371, ds_readws(ACTION) + 128);
					ds_writew(ACTION, 28);
				} else if (ds_readws(ACTION) <= 150) {
					ds_writeb(0x6370, ds_readws(ACTION) + 128);
					ds_writew(ACTION, 28);
				}
			}

			if (ds_readbs(0x636e) != ds_readbs(0x6370) && hero1 == hero2) {

				/* set the new ??? border */
				do_border((RealPt)ds_readd(0xd2ff),
					ds_readws(0x64ce + 4 * ds_readbs(0x636e)),
					ds_readws(0x64ce + 2 + 4 * ds_readbs(0x636e)),
					ds_readws(0x64ce + 4 * ds_readbs(0x636e)) + 17,
					ds_readws(0x64ce + 2 + 4 * ds_readbs(0x636e)) + 17,
					30);
				/* set the new ??? border */
				do_border((RealPt)ds_readd(0xd2ff),
					ds_readws(0x64ce + 4 * ds_readbs(0x6370)),
					ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)),
					ds_readws(0x64ce + 4 * ds_readbs(0x6370)) + 17,
					ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)) + 17,
					9);

				ds_writeb(0x636e, ds_readbs(0x6370));

				memset(Real2Host(ds_readd(DTP2)), ' ', 60);
				host_writeb(Real2Host(ds_readd(DTP2)) + 60, 0);
				GUI_print_string(Real2Host(ds_readd(DTP2)), 16, 192);

				if (host_readws(hero2 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370))) {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						/* "%s %s " */
						(char*)(p_datseg + 0x653c),
						Real2Host(GUI_name_singular((Bit8u*)get_itemname(host_readws(hero2 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370))))),
						!is_in_word_array(host_readws(hero2 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370)), (signed short*)Real2Host(ds_readd(0x634 + 4 * host_readbs(hero2 + HERO_TYPE)))) ? p_datseg + 0x6543 : get_city(0x108));

					if (item_weapon(get_itemsdat(host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370))))) {
						strcat((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(4 * (48 + host_readbs(get_itemsdat(host_readws(hero1 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370))) + 3))));
					}

					GUI_print_string(Real2Host(ds_readd(DTP2)), 16, 192);
				}
			}

			if (ds_readbs(0x636f) != ds_readbs(0x6371) && ds_readbs(0x6371) != -1) {

				/* set the new ??? border */
				do_border((RealPt)ds_readd(0xd2ff),
					ds_readws(0x64ce + 4 * ds_readbs(0x636f)),
					ds_readws(0x64ce + 2 + 4 * ds_readbs(0x636f)),
					ds_readws(0x64ce + 4 * ds_readbs(0x636f)) + 17,
					ds_readws(0x64ce + 2 + 4 * ds_readbs(0x636f)) + 17,
					30);

				if (hero1 == hero2) {

					/* set the new ??? border */
					do_border((RealPt)ds_readd(0xd2ff),
						ds_readws(0x64ce + 4 * ds_readbs(0x6370)),
						ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)),
						ds_readws(0x64ce + 4 * ds_readbs(0x6370)) + 17,
						ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)) + 17,
						9);
				}


				/* set the new ??? border */
				do_border((RealPt)ds_readd(0xd2ff),
					ds_readws(0x64ce + 4 * ds_readbs(0x6371)),
					ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6371)),
					ds_readws(0x64ce + 4 * ds_readbs(0x6371)) + 17,
					ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6371)) + 17,
					8);

				ds_writeb(0x636f, ds_readbs(0x6371));
			}

			if (ds_readws(ACTION) == 28) {
				if (ds_readbs(0x6371) != -1) {

					if (flag4 != 0) {

						if (ds_readbs(0x6371) < 23) {
							pass_item(hero1, ds_readbs(0x6370), hero2, ds_readbs(0x6371));
							ds_writew(0x2846, 1);
						} else if (ds_readbs(0x6371) == 23) {
							print_item_description(hero1, ds_readbs(0x6370));
						} else if (ds_readbs(0x6371) == 24) {
							consume(hero1, hero2, ds_readbs(0x6370));
						}

						/* set the new ??? border */
						do_border((RealPt)ds_readd(0xd2ff),
							ds_readws(0x64ce + 4 * ds_readbs(0x6371)),
							ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6371)),
							ds_readws(0x64ce + 4 * ds_readbs(0x6371)) + 17,
							ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6371)) + 17,
							30);

						flag4 = 0;
						hero1 = hero2;

					} else {
						if (ds_readbs(0x6371) < 23) {
							move_item(ds_readbs(0x6370), ds_readbs(0x6371), hero2);
							ds_writew(0x2846, 1);
						} else if (ds_readbs(0x6371) == 23) {
							print_item_description(hero2, ds_readbs(0x6370));
						} else if (ds_readbs(0x6371) == 24) {
							consume(hero2, hero2, ds_readbs(0x6370));
						}

						/* set the new ??? border */
						do_border((RealPt)ds_readd(0xd2ff),
							ds_readws(0x64ce + 4 * ds_readbs(0x6371)),
							ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6371)),
							ds_readws(0x64ce + 4 * ds_readbs(0x6371)) + 17,
							ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6371)) + 17,
							30);
					}

					reset_item_selector();
					flag2 = 1;
				} else {

					ds_writeb(0x6371, 23);

					/* set the new ??? border */
					do_border((RealPt)ds_readd(0xd2ff),
						ds_readws(0x64ce + 4 * ds_readbs(0x6371)),
						ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6371)),
						ds_readws(0x64ce + 4 * ds_readbs(0x6371)) + 17,
						ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6371)) + 17,
						8);

					if (host_readws(hero2 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370))) {

						nvf.dst = Real2Host(ds_readd(ICON));
						nvf.src = Real2Host(ds_readd(0xd2a9));
						nvf.type = 0;
						nvf.width = (Bit8u*)&width;
						nvf.height = (Bit8u*)&height;
						nvf.nr = host_readws(get_itemsdat(host_readws(hero2 + HERO_ITEM_HEAD + 14 * ds_readbs(0x6370))));
						process_nvf(&nvf);

						make_ggst_cursor(Real2Host(ds_readd(ICON)));

						ds_writed(0xcecb, (Bit32u)RealMake(datseg, 0xcecf));
					}
				}
			}
		}

		/* check if the hero is diseased and print a message */
		if (ds_readws(0x2c9b) == 1 &&
			ds_readws(ACTION) == 240 &&
			hero_is_diseased(hero2))
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(4 * (25 + hero_is_diseased(hero2))),
				(char*)(hero2 + HERO_NAME2),
				Real2Host(GUI_get_ptr(host_readbs(hero2 + HERO_SEX), 1)));

			GUI_output(Real2Host(ds_readd(DTP2)));

		}

		if (ds_readws(0xc3d3) != 0 || ds_readws(ACTION) == 73) {

			ds_writed(0xe3ae, ds_readd(0xcecb));
			ds_writed(0xcecb, (Bit32u)RealMake(datseg, 0x2848));

			switch (ds_readws(0x2c9b)) {
			case 1: {
				/* from start-page */

				if (ds_readws(GAME_MODE) == 2) {
					ds_writed(0xbf95 + 0x0, host_readd(Real2Host(ds_readd(CITY_LTX)) + 0x4c));
					ds_writed((0xbf95 + 0x4), host_readd(Real2Host(ds_readd(CITY_LTX)) + 0x50));
					ds_writed((0xbf95 + 0x8), host_readd(Real2Host(ds_readd(CITY_LTX)) + 0x54));

					flag3 = hero_is_diseased(hero2);

					if (flag3 != 0) {
						ds_writed((0xbf95 + 0x0c), host_readd(Real2Host(ds_readd(CITY_LTX)) + 0x64));
						ds_writed((0xbf95 + 0x10), host_readd(Real2Host(ds_readd(CITY_LTX)) + 0x3c));
						l1 = 10;
					} else {
						ds_writed((0xbf95 + 0x0c), host_readd(Real2Host(ds_readd(CITY_LTX)) + 0x3c));
						l1 = 9;
					}
				} else {

					if ((flag3 = hero_is_diseased(hero2))) {
						ds_writed((0xbf95 + 0), host_readd(Real2Host(ds_readd(CITY_LTX)) + 0x64));
						ds_writed((0xbf95 + 4), host_readd(Real2Host(ds_readd(CITY_LTX)) + 0x3c));
						l1 = 7;
					} else {
						ds_writed((0xbf95 + 0), host_readd(Real2Host(ds_readd(CITY_LTX)) + 0x3c));
						l1 = 6;
					}
				}

				l_di = GUI_radio((Bit8u*)0, l1,
						get_city(0x114),
						get_city(0x40),
						get_city(0x44),
						get_ltx(0x350),
						get_ltx(0x354),
						Real2Host(ds_readd((0xbf95 + 0x00))),
						Real2Host(ds_readd((0xbf95 + 0x04))),
						Real2Host(ds_readd((0xbf95 + 0x08))),
						Real2Host(ds_readd((0xbf95 + 0x0c))),
						Real2Host(ds_readd((0xbf95 + 0x10))));

				if (l_di != -1) {
					switch(l_di) {
					case 1: {
						/* change name */
						if (host_readbs(hero2 + HERO_NPC_ID) != 0) {
							GUI_output(get_city(0x11c));
						} else {
							GUI_input(get_city(0x118), 15);
							strcpy((char*)hero2 + HERO_NAME2, (char*)Real2Host(ds_readd(0xd2ef)));
							ds_writew(0x2846, 1);
						}
						break;
					}
					case 2: {
						/* use item */
						if (hero1 != hero2) {
							GUI_output(get_city(0x110));
						} else {
							use_item(ds_readbs(0x6370), hero_pos);
							reset_item_selector();
							ds_writew(0x2846, 1);
						}
						break;
					}
					case 3: {
						/* drop item */
						if (hero1 != hero2) {
							GUI_output(get_city(0x110));
						} else {
							drop_item(hero2, ds_readbs(0x6370), -1);
							reset_item_selector();
							ds_writew(0x2846, 1);
						}
						break;
					}
					case 4: {
						/* use talent */
						ds_writew(0x6532, 1);
						GUI_use_talent(hero_pos, 0);
						ds_writew(0x6532, 0);

						if (ds_readws(0x26bf) == 19) {
							load_city_ltx(ARCHIVE_FILE_CHARTEXT_LTX);
						}
						break;
					}
					case 5: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_city_ltx(ARCHIVE_FILE_CHARTEXT_LTX);
							reset_item_selector();
						}
						ds_writew(0x2846, 1);
						break;
					}
					case 6: {
						/* show ATPA values */
						if (l1 == 6) {
							flag1 = 1;
							reset_item_selector();
						} else if (l1 == 7) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_city(4 * (25 + flag3)),
								(char*)(hero2 + HERO_NAME2),
								Real2Host(GUI_get_ptr(host_readbs(hero2 + HERO_SEX), 1)));

								GUI_output(Real2Host(ds_readd(DTP2)));
						} else {
							ds_writew(0x2c9b, 2);
							ds_writew(0x2846, 1);
						}
						break;
					}
					case 7: {
						/* show talents */
						if (l1 == 7) {
							flag1 = 1;
							reset_item_selector();
						} else {
							reset_item_selector();
							ds_writew(0x2c9b, 3);
							ds_writew(0x2846, 1);
						}
						break;
					}
					case 8: {
						/* show spells */
						if (host_readbs(hero2 + HERO_TYPE) < 7) {
							GUI_output(get_ltx(0x35c));
						} else {
							reset_item_selector();
							ds_writew(0x2c9b, 4);
							ds_writew(0x2846, 1);
						}
						break;
					}
					case 9: {
						if (flag3 != 0) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_city(4 * (25 + flag3)),
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
						get_city(0x40),
						get_city(0x44),
						get_ltx(0x350),
						get_ltx(0x354),
						get_city(0x60),
						get_city(0x50),
						get_city(0x54),
						get_city(0x3c));

				if (l_di != -1) {
					switch(l_di) {
					case 1: {
						/* use item */
						if (hero1 != hero2) {
							GUI_output(get_city(0x110));
						} else {
							use_item(ds_readbs(0x6370), hero_pos);
							reset_item_selector();
						}
						break;
					}
					case 2: {
						/* drop item */
						if (hero1 != hero2) {
							GUI_output(get_city(0x110));
						} else {
							drop_item(hero2, ds_readbs(0x6370), -1);
							reset_item_selector();
							ds_writew(0x2846, 1);
						}
						break;
					}
					case 3: {
						/* use talent */
						ds_writew(0x6532, 1);
						GUI_use_talent(hero_pos, 0);
						ds_writew(0x6532, 0);

						if (ds_readws(0x26bf) == 19) {
							load_city_ltx(ARCHIVE_FILE_CHARTEXT_LTX);
						}
						break;
					}
					case 4: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_city_ltx(ARCHIVE_FILE_CHARTEXT_LTX);
							reset_item_selector();
							ds_writew(0x2846, 1);
						}
						break;
					}
					case 5: {
						/* TODO: different code is generated here */
						ds_writew(0x2c9b, 1);
						ds_writew(0x2846, 1);
						break;
					}
					case 6: {
						reset_item_selector();
						ds_writew(0x2c9b, 3);
						ds_writew(0x2846, 1);
						break;
					}
					case 7: {
						if (host_readbs(hero2 + HERO_TYPE) < 7) {
							GUI_output(get_ltx(0x35c));
						} else {
							reset_item_selector();
							ds_writew(0x2c9b, 4);
							ds_writew(0x2846, 1);
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
				/* from talents-page */
				l_di = GUI_radio((Bit8u*)0, 6,
						get_ltx(0x350),
						get_city(0x60),
						get_city(0x4c),
						get_city(0x54),
						get_ltx(0x354),
						get_city(0x3c));

				if (l_di != -1) {
					switch (l_di) {
					case 1: {
						/* use talent */
						ds_writew(0x6532, 1);
						GUI_use_talent(hero_pos, 0);
						ds_writew(0x6532, 0);

						if (ds_readws(0x26bf) == 19) {
							load_city_ltx(ARCHIVE_FILE_CHARTEXT_LTX);
						}
						break;
					}
					case 2: {
						ds_writew(0x2c9b, 1);
						ds_writew(0x2846, 1);
						break;
					}
					case 3: {
						ds_writew(0x2c9b, 2);
						ds_writew(0x2846, 1);
						break;
					}
					case 4: {
						if (host_readbs(hero2 + HERO_TYPE) < 7) {
							GUI_output(get_ltx(0x35c));
						} else {
							ds_writew(0x2c9b, 4);
							ds_writew(0x2846, 1);
						}
						break;
					}
					case 5: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_city_ltx(ARCHIVE_FILE_CHARTEXT_LTX);
							ds_writew(0x2846, 1);
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
						get_ltx(0x354),
						get_city(0x60),
						get_city(0x4c),
						get_city(0x50),
						get_ltx(0x358),
						get_city(0x3c));

				if (l_di != -1) {
					switch (l_di) {
					case 1: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_city_ltx(ARCHIVE_FILE_CHARTEXT_LTX);
							ds_writew(0x2846, 1);
						}
						break;
					}
					case 2: {
						ds_writew(0x2c9b, 1);
						ds_writew(0x2846, 1);
						break;
					}
					case 3: {
						ds_writew(0x2c9b, 2);
						ds_writew(0x2846, 1);
						break;
					}
					case 4: {
						ds_writew(0x2c9b, 3);
						ds_writew(0x2846, 1);
						break;
					}
					case 5: {
						if (ds_readws(0x2c9b) == 4)
							ds_writew(0x2c9b, 5);
						else
							ds_writew(0x2c9b, 4);
						ds_writew(0x2846, 1);
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

			ds_writed(0xcecb, ds_readd(0xe3ae));
		}
	}

	/* restore text file except for CHARTEXT.LTX, TAVERN.TLK and except for dialogs */
	if (file_bak != -1 &&
		file_bak != ARCHIVE_FILE_CHARTEXT_LTX &&
		file_bak != ARCHIVE_FILE_TAVERN_TLK &&
		(file_bak < 156 || file_bak > 176))
	{
		load_city_ltx(file_bak);
	}

	ds_writew(0x2846, 1);
	ds_writew(TEXTBOX_WIDTH, tw_bak);
	dec_ds_ws(TIMERS_DISABLED);

	if (ds_readbs(CURRENT_TOWN) != 0) {
		ds_writeb(0x4475, 3);
	}
}

/* Borlandified and identical */
void status_select_hero(void)
{

	if (ds_readws(0x29b2) == 0 || !ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP))) {
		/* Yes, it was written that way! */
	} else {
		signed short hero_pos = select_hero_from_group(get_ltx(0x4b4));

		if (hero_pos != -1) {
			status_menu(hero_pos);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
