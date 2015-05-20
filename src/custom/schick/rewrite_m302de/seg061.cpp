/**
 *	Rewrite of DSA1 v3.02_de functions of seg061 (temple)
 *	Functions rewritten: 3/8
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg047.h"
#include "seg061.h"
#include "seg062.h"
#include "seg095.h"
#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void do_temple(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else
	signed short l_si;
	signed short l_di;
	signed short input;
	signed char done = 0;
	Bit32s money;
	Bit32s donation;
	signed short game_state;

	ds_writew(0x29b6, ds_writew(0x29b8, 0));
	ds_writew(0x2846, 1);

	draw_loc_icons(9,	0, 1, 2,
				3, 4, 5,
				6, 7, 8);

	while (!done) {

		if (ds_readws(0x2846) != 0) {

			/* search which god owns this temple */
			ds_writew(0xe3f8, 1);
			for (l_si = 1; l_si < 15; l_si++) {
				if (is_in_byte_array(ds_readws(TYPEINDEX), Real2Host(ds_readd(0x6e36 + 4 * l_si))))
				{
					ds_writew(0xe3f8, l_si);
					break;
				}
			}

			draw_main_screen();
			load_ani(3);
			load_tempicon(ds_readws(0xe3f8) - 1);
			init_ani(0);
			set_audio_track(144);

			/* draw temple icon */
			ds_writew(0xc011, ds_writew(0xc013, 0));
			ds_writew(0xc015, 40);
			ds_writew(0xc017, 22);
			ds_writed(0xc00d, (Bit32u)((RealPt)ds_readd(0xd2ff) + 28259));
			ds_writed(0xc019, (Bit32u)((RealPt)ds_readd(0xc3a9) + 7000));

			update_mouse_cursor();
			do_pic_copy(0);
			refresh_screen_size();
			ds_writed(0xc00d, ds_readd(0xd2ff));

			/* location string */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0x3ac),
				(char*)get_ltx(4 * (ds_readws(0xe3f8) + 21)),	/* name of the god */
				(char*)(ds_readws(TYPEINDEX) != 58 ? get_ltx(4 * (ds_readbs(CURRENT_TOWN) + 235)): get_ltx(0x9b8)));

			GUI_print_loc_line(Real2Host(ds_readd(DTP2)));

			ds_writew(0x2846, 0);
		}

		handle_gui_input();

		/* input window */
		if (ds_readws(0xc3d3) != 0 || ds_readws(ACTION) == 73) {

			l_di = GUI_radio(get_ltx(0x384), 9,
						get_ltx(0x388),
						get_ltx(0x38c),
						get_ltx(0x494),
						get_ltx(0x390),
						get_ltx(0x394),
						get_ltx(0x398),
						get_ltx(0x9b0),
						get_ltx(0x4a0),
						get_ltx(0x39c)) - 1;

			if (l_di != -2) {
				ds_writew(ACTION, l_di + 129);
			}
		}

		if (ds_readws(ACTION) == 137) {
			/* leave temple */
			if (!ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP))) {
				GUI_output(get_ltx(0x3a0));
			} else {
				done = 1;
			}
		} else if (ds_readws(ACTION) == 129) {
			/* add character */
			char_add(ds_readws(TYPEINDEX));
			draw_status_line();
		} else if (ds_readws(ACTION) == 130) {
			/* let go character */
			char_letgo(ds_readws(TYPEINDEX));
			draw_status_line();
		} else if (ds_readws(ACTION) == 131) {
			/* erase character */
			char_erase();
		} else if (ds_readws(ACTION) == 132) {
			/* load game */
			if (ds_readws(TYPEINDEX) != 58) {

				do {
					game_state = load_game_state();
				} while (game_state == -1);

				/* location string */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ltx(0x3ac),
					(char*)get_ltx(4 * (ds_readws(0xe3f8) + 21)),	/* name of the god */
					(char*)get_ltx(4 * (ds_readbs(CURRENT_TOWN) + 235)));
				GUI_print_loc_line(Real2Host(ds_readd(DTP2)));

				draw_status_line();

				if (ds_readbs(LOCATION) != 2) {
					done = 1;
				}
			} else {
				GUI_output(get_ltx(0xcc4));
			}
		} else if (ds_readws(ACTION) == 133) {
			/* save game */
			if (ds_readws(TYPEINDEX) != 58) {
				if (!ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP))) {
					GUI_output(get_ltx(0x3a0));
				} else {
					save_game_state();
				}
			} else {
				GUI_output(p_datseg + 0x6e7a);
			}
		}

		if (ds_readws(ACTION) == 134) {
			/* quit game */
			if (GUI_bool(get_ltx(0x4ac))) {
				done = 1;
				ds_writews(0xc3c1, 3);
			}
		}

		if (ds_readws(ACTION) == 135) {
			/* ask for a miracle */
			if (!ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP))) {
				GUI_output(get_ltx(0x3a0));
			} else {
				ask_miracle();
			}
		}

		if (ds_readws(ACTION) == 136) {
			/* make a donation */
			if (!ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP))) {
				GUI_output(get_ltx(0x3a0));
			} else {

				money = get_party_money();

				if (!money) {
					GUI_output(get_ltx(0x644));
				} else {

					make_valuta_str((char*)Real2Host(ds_readd(0xd2eb)), money);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x4a4),
						(char*)Real2Host(ds_readd(0xd2eb)));

					input = GUI_input(Real2Host(ds_readd(DTP2)), 3);

					donation = input;

					if (donation > 0) {

						if (10 * donation >= money) {
							/* donate all money */
							add_ds_ds(0x314e + 4 * ds_readws(0xe3f8), money / 10);
							money = 0;
						} else {
							add_ds_ds(0x314e + 4 * ds_readws(0xe3f8), donation);
							money -= 10 * donation;
						}

						set_party_money(money);
						GUI_output(get_ltx(0x4a8));
					}
				}
			}
		}
	}

	copy_palette();
	turnaround();

	ds_writew(0x29b6, ds_writew(0x29b8, 1));
#endif
}

/* Borlandified and identical */
void char_add(signed short temple_id)
{
	signed short l_si;
	signed short l_di;
	signed short i;
	RealPt ptr;
	Bit8u *hero;

	ptr = (RealPt)ds_readd(0xd303) + 50000;
	l_di = copy_chr_names(Real2Host(ptr), temple_id);

	if (ds_readbs(0x2d3c) == 7 ||
		(ds_readbs(0x2d3c) == 6 && !host_readbs(get_hero(6) + 0x21)))
	{
		GUI_output(get_ltx(0x480));
	} else {

		do {

			if (!l_di) {
				GUI_output(get_ltx(0x488));
				l_si = -1;
			} else {

				l_si = menu_enter_delete(ptr, l_di, 1);

				if (l_si != -1) {

					hero = get_hero(0);

					for (i = 0; i < 6; i++, hero += 0x6da) {

						if (!host_readbs(hero + 0x21)) {

							prepare_chr_name((char*)Real2Host(ds_readd(DTP2)),
										(char*)(Real2Host(ptr) + 32 * l_si));

							if (read_chr_temp((RealPt)ds_readd(DTP2), i, ds_readbs(CURRENT_GROUP))) {
								inc_ds_bs_post(0x2d3c);
								inc_ds_bs_post(0x2d36 + ds_readbs(CURRENT_GROUP));
								host_writebs(hero + 0x8a, i + 1);
								write_chr_temp(i);
							}
							break;
						}
					}

					draw_main_screen();
					init_ani(2);

					/* location string */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x3ac),
						(char*)get_ltx(4 * (ds_readws(0xe3f8) + 21)),	/* name of the god */
						(char*)get_ltx(4 * (ds_readbs(CURRENT_TOWN) + 235)));
					GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
				}

				l_di = copy_chr_names(Real2Host(ptr), temple_id);
			}
		} while (l_si != -1 && ds_readbs(0x2d3c) < (host_readbs(get_hero(6) + 0x21) ? 7 : 6));
	}
}

/* Borlandified and identical */
void char_letgo(signed short temple_id)
{
	signed short hero_pos;
	Bit8u *hero;

	if (!ds_readbs(0x2d3c) || !ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP))) {
		GUI_output(get_ltx(0x3a0));
	} else {

		do {

			hero_pos = select_hero_from_group(get_ltx(0x9a8));

			if (hero_pos != -1) {

				if (hero_pos == 6) {
					/* let go an NPC */
					ds_writew(NPC_MONTHS, 99);
					npc_farewell();
				} else {
					/* let go a hero */
					hero = get_hero(hero_pos);
					dec_ds_bs_post(0x2d3c);
					dec_ds_bs_post(0x2d36 + ds_readbs(CURRENT_GROUP));

					host_writeb(hero + 0x88, temple_id);
					host_writeb(hero + 0x8a, 0);

					write_chr_temp(hero_pos);

					memset(hero, 0, 0x6da);

					draw_main_screen();
					init_ani(2);

					/* location string */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x3ac),
						(char*)get_ltx(4 * (ds_readws(0xe3f8) + 21)),	/* name of the god */
						(char*)get_ltx(4 * (ds_readbs(CURRENT_TOWN) + 235)));
					GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
				}
			}

		} while (hero_pos != -1 && ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP)) > (host_readbs(get_hero(6) + 0x21) ? 1 : 0));
	}
}

signed short char_erase(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else

#endif
	return 1;
}

#if !defined(__BORLANDC__)
}
#endif
