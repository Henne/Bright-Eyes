/**
 *	Rewrite of DSA1 v3.02_de functions of seg058 (smith)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg058.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg031.h"
#include "seg047.h"
#include "seg056.h"
#include "seg058.h"
#include "seg075.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg104.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief	add all items of a hero to the repair list
 * \param smith_ptr	pointer to the smith description
 * \param hero		pointer to the hero
 * \param item_pos	the position of the item in the inventory
 * \param smith_pos	the position of the item in the repair list
 */
void add_item_to_smith(Bit8u *smith_ptr, Bit8u *hero, signed short item_pos, signed short smith_pos)
{
	signed short item_id;

	item_id = host_readws(hero + HERO_ITEM_HEAD + 14 * item_pos);

	host_writews(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos, item_id);

	if (item_armor(get_itemsdat(item_id)) || item_weapon(get_itemsdat(item_id))) {

		if (ks_broken(hero + HERO_ITEM_HEAD + 14 * item_pos)) {

			host_writews(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 2,
				(host_readws(get_itemsdat(item_id) + 8) +
					(host_readws(get_itemsdat(item_id) + 8) * host_readbs(smith_ptr) / 100)) / 2);

			if (host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 2) == 0) {
				host_writews(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 2, 1);
			}

			host_writews(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 4,
				host_readbs(get_itemsdat(item_id) + 7));

		} else {

			if (host_readbs(hero + HERO_ITEM_HEAD + 7 + 14 * item_pos) != 0) {

				host_writews(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 2,
					(host_readws(get_itemsdat(item_id) + 8) +
						(host_readws(get_itemsdat(item_id) + 8) * host_readbs(smith_ptr) / 100)) / 4);

				if (host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 2) == 0) {
					host_writews(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 2, 1);
				}

				host_writews(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 4,
					host_readbs(get_itemsdat(item_id) + 7));
			} else {
				host_writews(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 2, 0);
				host_writews(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 4, 1);
			}
		}
	} else {
		host_writews(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 2, 0);
		host_writews(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 4, 1);
	}

	host_writebs(Real2Host(ds_readd(SELLITEMS)) + 7 * smith_pos + 6, (signed char)item_pos);
}

struct dummy3 {
	signed short a[3];
};

struct dummy5 {
	signed short a[5];
};

struct dummy_c5 {
	char a[5];
};

/**
 * \brief		shows the repair-screen an provides interaction
 * \param smith_ptr	pointer to the smith descriptor
 * \param a1		???
 */
void repair_screen(Bit8u *smith_ptr, signed short a1)
{
	signed short l_si;
	signed short j;
	signed short items_x;
	signed short fg_bak;
	signed short bg_bak;
	signed short answer;
	signed short val;
	signed short percent;
	signed short percent_old = 100;
	signed short item_id;
	signed short l6;
	signed short l7;
	signed short done = 0;
	signed short item = 0;
	signed short l8;
	signed short smith_pos;
	signed short l10 = 1;
	signed short l11 = 1;
	signed short hero_pos_old = 1;
	signed short hero_pos = -1;
#if !defined(__BORLANDC__)
	struct dummy_c5 fmt_h = { { "H %d" } };
	struct dummy_c5 fmt_s = { { "S %d" } };
	struct dummy_c5 fmt_d = { { "D %d" } };
	struct dummy3 array3 = { { 30, 95, 160 } };
	struct dummy5 array5 = { { 35, 55, 75, 95, 115 } };
#else
	struct dummy_c5 fmt_h = *(struct dummy_c5*)(p_datseg + SMITH_STR_MONEY_H);
	struct dummy_c5 fmt_s = *(struct dummy_c5*)(p_datseg + SMITH_STR_MONEY_S);
	struct dummy_c5 fmt_d = *(struct dummy_c5*)(p_datseg + SMITH_STR_MONEY_D);
	struct dummy3 array3 = *(struct dummy3*)(p_datseg + 0x6c73);
	struct dummy5 array5 = *(struct dummy5*)(p_datseg + 0x6c79);
#endif

	Bit32s price;
	Bit32s p_money;
	signed short l12 = 0;
	Bit8u *hero2;
	Bit8u *hero1;
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	/* check if this smith has an item in repair */
	if (ds_readws(SMITH_REPAIRITEMS + 6 * a1) != 0) {

		if (ds_readds(SMITH_REPAIRITEMS + 2 + 6 * a1) > ds_readds(DAY_TIMER)) {
			/* not ready yet */
			GUI_output(get_ltx(0x794));

		} else if (get_item(ds_readws(SMITH_REPAIRITEMS + 6 * a1), 1, 1)) {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0x798),
				(char*)Real2Host(GUI_names_grammar((signed short)0x8002, ds_readws(SMITH_REPAIRITEMS + 6 * a1), 0)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			ds_writed(SMITH_REPAIRITEMS + 2 + 6 * a1, 0);
			ds_writew(SMITH_REPAIRITEMS + 6 * a1, 0);
		}
	} else {

		set_var_to_zero();
		ds_writeb(0x2845, -1);

		draw_loc_icons(5, 23, 26, 27, 28, 8);
		draw_main_screen();

		/* ICONS */
		l_si = load_archive_file(ARCHIVE_FILE_ICONS);
		seek_archive_file(l_si, 18 * 576L);
		read_archive_file(l_si, Real2Host(ds_readd(ICON)), 576L);
		bc_close(l_si);

		ds_writew(0xc011, 108);
		ds_writew(0xc013, 5);
		ds_writew(0xc015, 131);
		ds_writew(0xc017, 28);
		ds_writed(0xc019, ds_readd(ICON));
		do_pic_copy(0);

		ds_writed(SELLITEMS, ds_readd(FIG_FIGURE1_BUF));
		memset(Real2Host(ds_readd(SELLITEMS)), 0, 350);

		get_textcolor(&fg_bak, &bg_bak);
		set_textcolor(255, 0);

		l8 = 1;

		while (done == 0) {

			if (l8 != 0 || l10 != 0 || l11 != 0) {

				/* refresh goods */

				if (l10 != 0) {

					hero_pos = select_hero_ok(get_ltx(0x7bc));

					hero2 = get_hero(hero_pos);

					deselect_hero_icon(hero_pos_old);
					select_hero_icon(hero_pos);
					hero_pos_old = hero_pos;
					l10 = 0;
					l11 = 1;
				}

				if (l11 != 0) {

					smith_pos = 0;
					for (l_si = 0; l_si < 23; l_si++) {
						if (host_readws(hero2 + HERO_ITEM_HEAD + 14 * l_si) != 0) {
							add_item_to_smith(smith_ptr, hero2, l_si, smith_pos++);
						}
					}

					for (l_si = smith_pos; l_si < 23; l_si++) {
						host_writew(Real2Host(ds_readd(SELLITEMS)) + 7 * l_si, 0);
					}

					l11 = 0;
					l6 = 1;
					l7 = item = l12 = 0;
					percent_old = 100;

					do_fill_rect((RealPt)ds_readd(0xd2ff), 26, 26, 105, 33, 0);

					make_valuta_str((char*)Real2Host(ds_readd(DTP2)), host_readds(hero2 + HERO_MONEY));
					GUI_print_string(Real2Host(ds_readd(DTP2)),
						104 - GUI_get_space_for_string(Real2Host(ds_readd(DTP2)), 0), 26);
				}

				update_mouse_cursor();

				do_fill_rect((RealPt)ds_readd(0xd2ff), 29, 34, 214, 133, 0);

				do_v_line((RealPt)ds_readd(0xd2ff), 87, 35, 131, -1);
				do_v_line((RealPt)ds_readd(0xd2ff), 152, 35, 131, -1);

				nvf.dst = Real2Host(ds_readd(BUFFER1_PTR));
				nvf.src = Real2Host(ds_readd(BUFFER10_PTR));
				nvf.type = 0;
				nvf.width =  (Bit8u*)&width;
				nvf.height = (Bit8u*)&height;

				for (items_x = 0; items_x < 3; items_x++) {

					for (l_si = 0; l_si < 5; l_si++) {

						answer = 5 * items_x + l_si + item;

						if ((j = host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * answer))) {

							ds_writew(0xc011, array3.a[items_x]);
							ds_writew(0xc013, array5.a[l_si]);
							ds_writew(0xc015, array3.a[items_x] + 15);
							ds_writew(0xc017, array5.a[l_si] + 15);
							ds_writed(0xc019, ds_readd(BUFFER1_PTR));

							nvf.nr = host_readws(get_itemsdat(j));

							process_nvf(&nvf);

							do_pic_copy(0);

							if (item_stackable(get_itemsdat(j))) {

								if ((val = host_readws(hero2 + (HERO_ITEM_HEAD+2) + 14 * host_readbs(Real2Host(ds_readd(SELLITEMS)) + 7 * answer + 6))) > 1)
								{
#if !defined(__BORLANDC__)
									sprintf((char*)Real2Host(ds_readd(DTP2)), "%d", val);
#else
									itoa(val, Real2Host(ds_readd(DTP2)), 10);
#endif

									GUI_print_string(Real2Host(ds_readd(DTP2)),
										array3.a[items_x] + 16 - GUI_get_space_for_string(Real2Host(ds_readd(DTP2)), 0),
										array5.a[l_si] + 9);

								}
							}

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								host_readws(Real2Host(ds_readd(SELLITEMS)) + 4 + 7 * answer) == 1 ? fmt_h.a :
									(host_readws(Real2Host(ds_readd(SELLITEMS)) + 4 + 7 * answer) == 10 ? fmt_s.a : fmt_d.a),
								host_readws(Real2Host(ds_readd(SELLITEMS)) + 2 + 7 * answer));


							GUI_print_string(Real2Host(ds_readd(DTP2)), array3.a[items_x] + 20, array5.a[l_si] + 5);
						}
					}
				}

				refresh_screen_size();
				l6 = 1;
				l7 = 0;
				l8 = 0;

			}

			ds_writed(0x29e4, (Bit32u)RealMake(datseg, 0x2c04));
			handle_input();
			ds_writed(0x29e4, (Bit32u)0L);

			if (ds_readws(0xc3c7) == 2) {
				select_with_mouse((Bit8u*)&l7, Real2Host(ds_readd(SELLITEMS)) + 7 * item);
			} else {
				select_with_keyboard((Bit8u*)&l7, Real2Host(ds_readd(SELLITEMS)) + 7 * item);
			}

			if (l6 != l7) {

				do_border((RealPt)ds_readd(0xd2ff),
					array3.a[l6 / 5] - 1,
					array5.a[l6 % 5] - 1,
					array3.a[l6 / 5] + 16,
					array5.a[l6 % 5] + 16,
					0);

				do_border((RealPt)ds_readd(0xd2ff),
					array3.a[l7 / 5] - 1,
					array5.a[l7 % 5] - 1,
					array3.a[l7 / 5] + 16,
					array5.a[l7 % 5] + 16,
					-1);

				l6 = l7;

				clear_loc_line();

				GUI_print_loc_line(Real2Host(GUI_name_singular((Bit8u*)get_itemname(host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l7 + item))))));
			}

			if (ds_readws(0xc3d3) != 0  || ds_readws(ACTION) == 73) {

				answer = GUI_radio(NULL, 5,
						get_ltx(0x6c4),
						get_ltx(0x6cc),
						get_ltx(0x6d0),
						get_ltx(0x6f8),
						get_ltx(0x6d4)) - 1;

				if (answer != -2) {
					ds_writew(ACTION, answer + 129);
				}
			}

			if (ds_readws(ACTION) == 131 && item != 0) {
				l8 = 1;
				item -= 15;
			} else if (ds_readws(ACTION) == 130 && host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (item + 15))) {
				l8 = 1;
				item += 15;
			}

			if (ds_readws(ACTION) == 144 || ds_readws(ACTION) == 129 || ds_readws(ACTION) == 28) {

				item_id = host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l7 + item));

				p_money = get_party_money();

				if (host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l7 + item) + 2) == 0) {
					GUI_output(get_ltx(0x79c));
				} else {


					j = 0;

					while (l12 == 0 && j < 3) {


						price = (host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l7 + item) + 2)
							* host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l7 + item) + 4)) * ds_readws(PRICE_MODIFICATOR) / 4;

						make_valuta_str((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), price);

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x7a0),
							(char*)Real2Host(GUI_names_grammar((signed short)0x8002, item_id, 0)),
							(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));


						do {
							percent = GUI_input(Real2Host(ds_readd(DTP2)), 2);

						} while (percent > 50);

						price -= (percent * price) / 100L;

						if (percent == 0 && p_money > price) {
							GUI_output(get_ltx(0x7a4));
							l12 = 2;
						} else if (percent >= percent_old) {
							j = 2;
						} else if (percent < 0) {
							break;
						} else {

							answer = select_hero_ok_forced(get_ltx(0x6e8));

							hero1 = get_hero(answer);

							l12 = bargain(hero1, 1, price, percent, 2) > 0 ? 1 : 0;
						}

						if (l12 > 0) {

							if (p_money < price) {
								GUI_output(get_ltx(0x644));
							} else {
								if (l12 != 2) {
									GUI_output(get_ltx(0x7b0));
								}

								ds_writew(SMITH_REPAIRITEMS + 6 * a1, item_id);

								if (ds_readds(DAY_TIMER) > HOURS(14)) {
									ds_writed(SMITH_REPAIRITEMS + 2 + 6 * a1, HOURS(23));
									GUI_output(get_ltx(0x7a8));
								} else {
									ds_writed(SMITH_REPAIRITEMS + 2 + 6 * a1, ds_readd(DAY_TIMER) + HOURS(6));
									GUI_output(get_ltx(0x7ac));
								}

								drop_item(hero2, host_readbs(Real2Host(ds_readd(SELLITEMS)) + 6 + 7 * (l7 + item)), 1);
								p_money -= price;
								set_party_money(p_money);

							}
							done = 1;
							continue;

						}
						 if (j == 2) {
							GUI_output(get_ltx(0x7b4));
							done = 1;
						} else {
							GUI_output(get_ltx(0x7b8));
							percent_old = percent;
						}

						j++;
					}
				}
			}

			if (ds_readws(ACTION) >= 241 && ds_readws(ACTION) <= 247) {

				hero_pos = ds_readws(ACTION) - 241;
				hero2 = get_hero(hero_pos);
				deselect_hero_icon(hero_pos_old);
				select_hero_icon(hero_pos);
				hero_pos_old = hero_pos;
				l11 = 1;
			}

			if (ds_readws(ACTION) == 132) {
				l10 = 1;
			}

			if (ds_readws(ACTION) == 133) {
				done = 1;
			}

		}

		set_textcolor(fg_bak, bg_bak);
		ds_writew(0x2846, 1);
		ds_writeb(0x2845, -1);
	}
}

/**
 * \brief	handler for entering smith location
 */
void do_smith(void)
{
	signed short done = 0;
	signed short answer;
	Bit8u *smith_ptr;

	if (ds_readds(DAY_TIMER) < HOURS(6) || ds_readds(DAY_TIMER) > HOURS(20)) {

		GUI_output(get_ltx(0x78c));
		turnaround();
		return;
	}

	if (ds_readbs(0x3472 + ds_readws(TYPEINDEX)) != 0 ||
		ds_readbs(0x34a4 + ds_readws(TYPEINDEX)) != 0 ||
		(ds_readws(TYPEINDEX) == 1 && ds_readb(0x3fc6))) {

		talk_smith();
		turnaround();
		return;
	}

	load_ggsts_nvf();
	ds_writew(0x2846, 1);
	smith_ptr = p_datseg + 0x6c10 + 2 * ds_readws(TYPEINDEX);
	ds_writew(PRICE_MODIFICATOR, 4);

	while (!done) {

		if (ds_readws(0x2846) != 0) {

			draw_loc_icons(3, 21, 18, 8);
			draw_main_screen();
			set_var_to_zero();
			load_ani(5);
			init_ani(0);
			GUI_print_loc_line(get_dtp(4 * ds_readws(CITYINDEX)));
			set_audio_track(ARCHIVE_FILE_SMITH_XMI);
			ds_writew(0x2846, 0);
		}

		handle_gui_input();

		if (ds_readws(0xc3d3) != 0 || ds_readws(ACTION) == 73) {

			ds_writew(TEXTBOX_WIDTH, 4);

			answer = GUI_radio(get_ltx(0x7c0), 3,
						get_ltx(0x55c),
						get_ltx(0x7c4),
						get_ltx(0x7c8)) - 1;

			/* TODO: why should it be 3??? Better make a backup */
			ds_writew(TEXTBOX_WIDTH, 3);

			if (answer != -2) {
				ds_writew(ACTION, answer + 129);
			}
		}

		if (ds_readws(ACTION) == 131) {
			done = 1;
		} else if (ds_readws(ACTION) == 129) {

			talk_smith();
			ds_writew(0x2846, 1);

			if (ds_readbs(0x3472 + ds_readws(TYPEINDEX)) != 0 ||
				ds_readbs(0x34a4 + ds_readws(TYPEINDEX)) != 0 ||
				ds_readbs(DUNGEON_INDEX) != 0)
			{
				done = 1;
			}
		} else if (ds_readws(ACTION) == 130) {
			repair_screen(smith_ptr, ds_readws(TYPEINDEX));
		}
	}

	turnaround();
	copy_palette();
}

void talk_smith(void)
{
	do_random_talk(13, 0);
}

void TLK_schmied(signed short state)
{
	if (!state) {
		ds_writew(0xe30e, ds_readb(0x3472 + ds_readws(TYPEINDEX)) != 0 ? 1 :
					(ds_readws(TYPEINDEX) == 17 ? 27 :
					(ds_readws(TYPEINDEX) == 1 && ds_readb(0x3fc6) != 0 ? 28 : 4)));
	} else if (state == 1) {
		ds_writew(0xe30e, ds_readb(0x34a4 + ds_readws(TYPEINDEX)) != 0 ? 2 : 3);
	} else if (state == 3) {
		ds_writeb(0x34a4 + ds_readws(TYPEINDEX), 1);
	} else if (state == 6 || state == 26) {
		tumult();
		ds_writeb(0x3472 + ds_readws(TYPEINDEX), ds_writeb(0x34a4 + ds_readws(TYPEINDEX), 1));
	} else if (state == 11 || state == 14 || state == 16 || state == 23) {
		ds_writeb(0x3472 + ds_readws(TYPEINDEX), 1);
	} else if (state == 19 || state == 31) {
		ds_writew(PRICE_MODIFICATOR, 3);
	} else if (state == 30) {

		DNG_enter_dungeon(14);
		ds_writeb(DUNGEON_LEVEL, 3);
		ds_writews(0x2d83, ds_writews(X_TARGET, 11));
		ds_writews(0x2d85, ds_writews(Y_TARGET, 2));
		ds_writeb(DIRECTION, 2);
	}
}

#if !defined(__BORLANDC__)
}
#endif
