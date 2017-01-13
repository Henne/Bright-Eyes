/**
 *	Rewrite of DSA1 v3.02_de functions of seg057 (merchant: sell)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg057.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg029.h"
#include "seg047.h"
#include "seg056.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg104.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

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
 * \brief   shows the sell-screen an provides interaction
 */
void sell_screen(Bit8u *shop_ptr)
{
	signed short l_di;
	signed short j;
	signed short items_x;
	signed short fg_bak;
	signed short bg_bak;
	signed short answer;
	signed short nice;
	signed short percent;
	signed short percent_old = 100;
	signed short item_id;
	signed short l5;
	signed short l6;
	signed short done = 0;
	signed short item = 0;
	signed short l8;
	signed short l20;
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
	struct dummy_c5 fmt_h = *(struct dummy_c5*)(p_datseg + SELL_SCREEN_STR_MONEY_H);
	struct dummy_c5 fmt_s = *(struct dummy_c5*)(p_datseg + SELL_SCREEN_STR_MONEY_S);
	struct dummy_c5 fmt_d = *(struct dummy_c5*)(p_datseg + SELL_SCREEN_STR_MONEY_D);
	struct dummy3 array3 = *(struct dummy3*)(p_datseg + SELL_SCREEN_ITEMS_POSX);
	struct dummy5 array5 = *(struct dummy5*)(p_datseg + SELL_SCREEN_ITEMS_POSY);
#endif
	Bit32s p_money;
	Bit32s price = 0;
	signed short l12 = 0;
	Bit8u *hero1;
	Bit8u *hero2;
	Bit8u *hero3;
	signed short width;
	signed short height;
	signed short l15;
	struct nvf_desc nvf;
	signed short tmp[7][23];


	ds_writew(HEROSWAP_ALLOWED, 0);
	l8 = ds_writews(REQUEST_REFRESH, 1);
	ds_writed(SELLITEMS, (Bit32u)((RealPt)ds_readd(FIG_FIGURE1_BUF) + 2100));

	while (done == 0) {

		if (ds_readws(REQUEST_REFRESH) != 0) {

			set_var_to_zero();
			ds_writeb(PP20_INDEX, 0xff);
			memset(Real2Host(ds_readd(SELLITEMS)), 0, 2100);

			for (items_x = 0; items_x <= 6; items_x++) {
				for (l_di = 0; l_di < 23; l_di++) {
					tmp[items_x][l_di] = 0;
				}
			}

			price = 0;
			draw_loc_icons(5, 23, 26, 27, 28, 8);
			draw_main_screen();

			/* ICONS */
			l_di = load_archive_file(ARCHIVE_FILE_ICONS);
			seek_archive_file(l_di, 24 * 576L);
			read_archive_file(l_di, Real2Host(ds_readd(ICON)), 576L);
			bc_close(l_di);

			ds_writew(PIC_COPY_X1, 108);
			ds_writew(PIC_COPY_Y1, 5);
			ds_writew(PIC_COPY_X2, 131);
			ds_writew(PIC_COPY_Y2, 28);
			ds_writed(PIC_COPY_SRC, ds_readd(ICON));
			do_pic_copy(0);

			get_textcolor(&fg_bak, &bg_bak);
			set_textcolor(255, 0);

			l5 = -1;
			l6 = 0;
			l10 = 1;
			ds_writew(REQUEST_REFRESH, 0);
		}

		if (l8 != 0 || l10 != 0 || l11 != 0) {

			/* refresh goods */

			if (l10 != 0) {

				hero_pos = select_hero_ok(get_ttx(0x700));

				if (hero_pos == -1 || host_readbs(get_hero(hero_pos) + HERO_KS_TAKEN) == 0) {
					break;
				}

				hero1 = get_hero(hero_pos);

				deselect_hero_icon(hero_pos_old);
				select_hero_icon(hero_pos);
				hero_pos_old = hero_pos;
				l10 = 0;
				l11 = 1;

			}

			if (l11 != 0) {

				l20 = 0;
				for (l_di = 0; l_di < 23; l_di++) {
					if (host_readws(hero1 + HERO_ITEM_HEAD + 14 * l_di) != 0) {
						insert_sell_items(shop_ptr, hero1, l_di, l20++);
					}
				}

				for (l_di = l20; l_di < 23; l_di++) {
					host_writew(Real2Host(ds_readd(SELLITEMS)) + 7 * l_di, 0);
				}

				l11 = 0;
				l5 = -1;
				l6 = item = l12 = 0;
				percent_old = 100;
				p_money = get_party_money();

				do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 26, 26, 105, 33, 0);

				make_valuta_str((char*)Real2Host(ds_readd(DTP2)), p_money);
				GUI_print_string(Real2Host(ds_readd(DTP2)),
					104 - GUI_get_space_for_string(Real2Host(ds_readd(DTP2)), 0), 26);
			}

			update_mouse_cursor();

			do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 29, 34, 214, 133, 0);

			do_v_line((RealPt)ds_readd(FRAMEBUF_PTR), 87, 35, 131, -1);
			do_v_line((RealPt)ds_readd(FRAMEBUF_PTR), 152, 35, 131, -1);

			nvf.dst = Real2Host(ds_readd(RENDERBUF_PTR));
			nvf.src = Real2Host(ds_readd(BUFFER10_PTR));
			nvf.type = 0;
			nvf.width =  (Bit8u*)&width;
			nvf.height = (Bit8u*)&height;

			for (items_x = 0; items_x < 3; items_x++) {

				for (l_di = 0; l_di < 5; l_di++) {

					answer = 5 * items_x + l_di + item;

					if ((j = host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * answer))) {

						ds_writew(PIC_COPY_X1, array3.a[items_x]);
						ds_writew(PIC_COPY_Y1, array5.a[l_di]);
						ds_writew(PIC_COPY_X2, array3.a[items_x] + 15);
						ds_writew(PIC_COPY_Y2, array5.a[l_di] + 15);
						ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));

						nvf.nr = host_readws(get_itemsdat(j));

						process_nvf(&nvf);

						do_pic_copy(0);

						if (item_stackable(get_itemsdat(j))) {

							if ((nice = host_readws(hero1 + (HERO_ITEM_HEAD+2) + 14 * host_readbs(Real2Host(ds_readd(SELLITEMS)) + 7 * answer + 6))) > 1)
							{
								my_itoa(nice, (char*)Real2Host(ds_readd(DTP2)), 10);

								GUI_print_string(Real2Host(ds_readd(DTP2)),
									array3.a[items_x] + 16 - GUI_get_space_for_string(Real2Host(ds_readd(DTP2)), 0),
									array5.a[l_di] + 9);

								if (tmp[hero_pos][host_readbs(Real2Host(ds_readd(SELLITEMS)) + 7 * answer + 6)] != 0)
								{
									set_textcolor(201, 0);
									my_itoa(tmp[hero_pos][host_readbs(Real2Host(ds_readd(SELLITEMS)) + 7 * answer + 6)], (char*)Real2Host(ds_readd(DTP2)), 10);

									GUI_print_string(Real2Host(ds_readd(DTP2)),
										array3.a[items_x] + 16 - GUI_get_space_for_string(Real2Host(ds_readd(DTP2)), 0),
										array5.a[l_di] + 1);

									set_textcolor(255, 0);
								}
							}
						}

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							host_readws(Real2Host(ds_readd(SELLITEMS)) + 4 + 7 * answer) == 1 ? fmt_h.a :
								(host_readws(Real2Host(ds_readd(SELLITEMS)) + 4 + 7 * answer) == 10 ? fmt_s.a : fmt_d.a),
							host_readws(Real2Host(ds_readd(SELLITEMS)) + 2 + 7 * answer));

						if (tmp[hero_pos][host_readbs(Real2Host(ds_readd(SELLITEMS)) + 7 * answer + 6)]) {
							set_textcolor(201, 0);
						}

						GUI_print_string(Real2Host(ds_readd(DTP2)), array3.a[items_x] + 20, array5.a[l_di] + 5);
						set_textcolor(255, 0);
					}
				}
			}

			do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 135, 26, 214, 33, 0);

			make_valuta_str((char*)Real2Host(ds_readd(DTP2)), price);
			GUI_print_string(Real2Host(ds_readd(DTP2)), 135, 26);

			l5 = -1;

			refresh_screen_size();
			l8 = 0;
		}

		if (ds_readws(HAVE_MOUSE) == 2) {
			select_with_mouse((Bit8u*)&l6, Real2Host(ds_readd(SELLITEMS)) + 7 * item);
		} else {
			select_with_keyboard((Bit8u*)&l6, Real2Host(ds_readd(SELLITEMS)) + 7 * item);
		}

#if !defined(__BORLANDC__)
		/* BE-fix */
		l6 = host_readws((Bit8u*)&l6);
#endif

		ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)RealMake(datseg, ACTION_TABLE_MERCHANT));
		handle_gui_input();
		ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)0L);

		if (l5 != l6) {

			if (l5 != -1) {
				do_border((RealPt)ds_readd(FRAMEBUF_PTR),
					array3.a[l5 / 5] - 1,
					array5.a[l5 % 5] - 1,
					array3.a[l5 / 5] + 16,
					array5.a[l5 % 5] + 16,
					0);
			 }

			do_border((RealPt)ds_readd(FRAMEBUF_PTR),
				array3.a[l6 / 5] - 1,
				array5.a[l6 % 5] - 1,
				array3.a[l6 / 5] + 16,
				array5.a[l6 % 5] + 16,
				-1);

			l5 = l6;

			clear_loc_line();


			GUI_print_loc_line(Real2Host(GUI_name_singular((Bit8u*)get_itemname(host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l6 + item))))));
		}

		if (ds_readws(MOUSE2_EVENT) != 0  || ds_readws(ACTION) == 73) {

			answer = GUI_radio(NULL, 5,
					get_ttx(0x6c4),
					get_ttx(0x6cc),
					get_ttx(0x6d0),
					get_ttx(0x6f8),
					get_ttx(0x6d4)) - 1;

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

		if (ds_readws(ACTION) == 144 || ds_readws(ACTION) == 28) {


			item_id = host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l6 + item));

			if (host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l6 + item) + 2) == 0) {
				GUI_output(get_ttx(0x710));
			} else {

				if (item_undropable(get_itemsdat(item_id))) {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(0x718),
						(char*)Real2Host(GUI_names_grammar((signed short)0x8002, item_id, 0)));
					GUI_output(Real2Host(ds_readd(DTP2)));
				} else {

					nice = 1;
					l15 = host_readbs(Real2Host(ds_readd(SELLITEMS)) + 7 * (l6 + item) + 6);

					if (tmp[hero_pos][l15] != 0) {

						if (item_stackable(get_itemsdat(item_id)) && host_readws(hero1 + (HERO_ITEM_HEAD+2) + 14 * l15) > 1) {

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(0x6fc),
								(char*)Real2Host(GUI_names_grammar(4, item_id, 0)));

							nice = GUI_input(Real2Host(ds_readd(DTP2)), 2);

							if (nice < 0) {
								nice = 0;
							}

							if (host_readws(hero1 + (HERO_ITEM_HEAD+2) + 14 * l15) < nice) {
								nice = host_readws(hero1 + (HERO_ITEM_HEAD+2) + 14 * l15);
							}

							price -= ((Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l6 + item) + 4) *
									tmp[hero_pos][l15] * ds_readws(PRICE_MODIFICATOR)) / 4L;

							tmp[hero_pos][l15] = nice;

							price += ((Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l6 + item) + 4) *
									tmp[hero_pos][l15] * ds_readws(PRICE_MODIFICATOR)) / 4L;
						} else {
							tmp[hero_pos][l15] = 0;

							price -= ((Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l6 + item) + 4) *
									nice * ds_readws(PRICE_MODIFICATOR)) / 4L;
						}
					} else {
						if (item_stackable(get_itemsdat(item_id)) && host_readws(hero1 + (HERO_ITEM_HEAD+2) + 14 * l15) > 1) {

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(0x6fc),
								(char*)Real2Host(GUI_names_grammar(4, item_id, 0)));

							nice = GUI_input(Real2Host(ds_readd(DTP2)), 2);

							if (nice < 0) {
								nice = 0;
							}

							if (host_readws(hero1 + (HERO_ITEM_HEAD+2) + 14 * l15) < nice) {
								nice = host_readws(hero1 + (HERO_ITEM_HEAD+2) + 14 * l15);
							}

							price -= ((Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l6 + item) + 4) *
									tmp[hero_pos][l15] * ds_readws(PRICE_MODIFICATOR)) / 4L;

							tmp[hero_pos][l15] = nice;

							price += ((Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l6 + item) + 4) *
									tmp[hero_pos][l15] * ds_readws(PRICE_MODIFICATOR)) / 4L;
						} else {
							tmp[hero_pos][l15] = 1;
							price += ((Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws(Real2Host(ds_readd(SELLITEMS)) + 7 * (l6 + item) + 4) *
									nice * ds_readws(PRICE_MODIFICATOR)) / 4L;
						}
					}
				}
			}
			l8 = 1;
		}


		if (ds_readws(ACTION) == 129 && price) {

			j = nice = 0;
			for (items_x = 0; items_x <= 6; items_x++) {
				for (l_di = 0; l_di < 23; l_di++) {
					nice += tmp[items_x][l_di];
				}
			}

			/* 0xe83 - 11ba */
			while (l12 == 0 && j < 3) {

				make_valuta_str((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), price);
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(0x704),
					(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));


				do {
					percent = GUI_input(Real2Host(ds_readd(DTP2)), 2);

				} while (percent > 50);

				if (percent == 0) {
					GUI_output(get_ttx(0x6e0));
					l12 = 2;
				} else if (percent >= percent_old) {
					j = 2;
				} else if (percent < 0) {
					break;
				} else {
					ds_writew(SKILLED_HERO_POS, get_skilled_hero_pos(TA_FEILSCHEN));

					answer = select_hero_ok_forced(get_ttx(0x6e8));

					hero2 = get_hero(answer);

					l12 = bargain(hero2, nice, price, percent, 0) > 0 ? 1 : 0;
				}

				if (l12 > 0) {

					hero3 = get_hero(0);
					for (items_x = 0; items_x <= 6; items_x++, hero3 += SIZEOF_HERO) {
						for (l_di = 0; l_di < 23; l_di++) {

							if (tmp[items_x][l_di] != 0) {
								item_id = host_readws(hero3 + HERO_ITEM_HEAD + 14 * l_di);
								drop_item(hero3, l_di, tmp[items_x][l_di]);
								ds_writeb(MARKET_ITEMSALDO_TABLE + item_id, ds_readbs(MARKET_ITEMSALDO_TABLE + item_id) - tmp[items_x][l_di]);

								if (ds_readbs(MARKET_ITEMSALDO_TABLE + item_id) <= -10) {
									ds_writeb(MARKET_ITEMSALDO_TABLE + item_id, 0);
									sub_ptr_ws(get_itemsdat(item_id) + 8, host_readws(get_itemsdat(item_id) + 8) * 10 / 100);
								}
							}
						}
					}

					price = price + percent * price / 100L;

					if (price > 0) {
						add_party_money(price);
					}

					if (l12 != 2) {
						GUI_output(get_ttx(0x70c));

						make_valuta_str((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), price);

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(0xccc),
							(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

						GUI_output(Real2Host(ds_readd(DTP2)));
					}

					l11 = 1;
					price = 0;

					for (items_x = 0; items_x <= 6; items_x++) {
						for (l_di = 0; l_di < 23; l_di++) {
							tmp[items_x][l_di] = 0;
						}
					}
					continue;
				}

				if (j == 2) {
					GUI_output(get_ttx(0x714));
					ds_writeb(MERCHANT_OFFENDED_FLAGS + ds_readws(TYPEINDEX), 1);
					done = 1;
				} else {
					GUI_output(get_ttx(0x708));
					percent_old = percent;
				}

				j++;
			}
		}

		if (ds_readws(ACTION) >= 241 && ds_readws(ACTION) <= 247) {

			hero_pos = ds_readws(ACTION) - 241;
			hero3 = get_hero(hero_pos);

			if ((host_readbs(hero3 + HERO_TYPE) != HERO_TYPE_NONE) &&
				host_readbs(hero3 + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
			{
				hero1 = get_hero(hero_pos);
				deselect_hero_icon(hero_pos_old);
				select_hero_icon(hero_pos);
				hero_pos_old = hero_pos;
				l11 = 1;
			}
		}

		if (ds_readws(ACTION) == 132) {
			l10 = 1;
		}

		if (ds_readws(ACTION) == 133) {
			done = 1;
		}

		if (host_readbs(hero1 + HERO_KS_TAKEN) == 0) {
			done = 1;
		}
	}

	set_textcolor(fg_bak, bg_bak);
	ds_writew(REQUEST_REFRESH, 1);
	ds_writeb(PP20_INDEX, 0xff);
	ds_writew(HEROSWAP_ALLOWED, 1);
}

#if !defined(__BORLANDC__)
}
#endif
