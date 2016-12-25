/**
 *	Rewrite of DSA1 v3.02_de functions of seg061 (temple, chr-management, miracles)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg061.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
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

void do_temple(void)
{
	signed short l_si;
	signed short l_di;
	signed short input;
	signed char done = 0;
	Bit32s money;
	Bit32s donation;
	signed short game_state;

	ds_writew(0x29b6, ds_writew(0x29b8, 0));
	ds_writew(REQUEST_REFRESH, 1);

	draw_loc_icons(9,	0, 1, 2,
				3, 4, 5,
				6, 7, 8);

	while (!done) {

		if (ds_readws(REQUEST_REFRESH) != 0) {

			/* search which god owns this temple */
			ds_writew(TEMPLE_GOD, 1);
			for (l_si = 1; l_si < 15; l_si++) {
				if (is_in_byte_array((signed char)ds_readws(TYPEINDEX), Real2Host(ds_readd(0x6e36 + 4 * l_si))))
				{
					ds_writew(TEMPLE_GOD, l_si);
					break;
				}
			}

			draw_main_screen();
			load_ani(3);
			load_tempicon(ds_readws(TEMPLE_GOD) - 1);
			init_ani(0);
			set_audio_track(ARCHIVE_FILE_TEMPLE_XMI);

			/* draw temple icon */
			ds_writew(PIC_COPY_X1, ds_writew(PIC_COPY_Y1, 0));
			ds_writew(PIC_COPY_X2, 40);
			ds_writew(PIC_COPY_Y2, 22);
			ds_writed(PIC_COPY_DST, (Bit32u)((RealPt)ds_readd(0xd2ff) + 28259));
			ds_writed(PIC_COPY_SRC, (Bit32u)((RealPt)ds_readd(BUFFER8_PTR) + 7000));

			update_mouse_cursor();
			do_pic_copy(0);
			refresh_screen_size();
			ds_writed(PIC_COPY_DST, ds_readd(0xd2ff));

			/* location string */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0x3ac),
				(char*)get_ltx(4 * (ds_readws(TEMPLE_GOD) + 21)),	/* name of the god */
				(char*)(ds_readws(TYPEINDEX) != 58 ? get_ltx(4 * (ds_readbs(CURRENT_TOWN) + 235)): get_ltx(0x9b8)));

			GUI_print_loc_line(Real2Host(ds_readd(DTP2)));

			ds_writew(REQUEST_REFRESH, 0);
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
			if (!ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
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
					(char*)get_ltx(4 * (ds_readws(TEMPLE_GOD) + 21)),	/* name of the god */
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
				if (!ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
					GUI_output(get_ltx(0x3a0));
				} else {
					save_game_state();
				}
			} else {
				GUI_output(p_datseg + STR_NO_SAVE_IN_TEMPLE);
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
			if (!ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
				GUI_output(get_ltx(0x3a0));
			} else {
				ask_miracle();
			}
		}

		if (ds_readws(ACTION) == 136) {
			/* make a donation */
			if (!ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
				GUI_output(get_ltx(0x3a0));
			} else {

				money = get_party_money();

				if (!money) {
					GUI_output(get_ltx(0x644));
				} else {

					make_valuta_str((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), money);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x4a4),
						(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

					input = GUI_input(Real2Host(ds_readd(DTP2)), 3);

					donation = input;

					if (donation > 0) {

						if (10 * donation >= money) {
							/* donate all money */
							add_ds_ds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD), money / 10);
							money = 0;
						} else {
							add_ds_ds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD), donation);
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
}

void char_add(signed short temple_id)
{
	signed short l_si;
	signed short l_di;
	signed short i;
	RealPt ptr;
	Bit8u *hero;

	ptr = (RealPt)ds_readd(BUFFER1_PTR) + 50000;
	l_di = copy_chr_names(Real2Host(ptr), temple_id);

	if (ds_readbs(TOTAL_HERO_COUNTER) == 7 ||
		(ds_readbs(TOTAL_HERO_COUNTER) == 6 && !host_readbs(get_hero(6) + HERO_TYPE)))
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

					for (i = 0; i < 6; i++, hero += SIZEOF_HERO) {

						if (!host_readbs(hero + HERO_TYPE)) {

							prepare_chr_name((char*)Real2Host(ds_readd(DTP2)),
										(char*)(Real2Host(ptr) + 32 * l_si));

							if (read_chr_temp((RealPt)ds_readd(DTP2), i, ds_readbs(CURRENT_GROUP))) {
								inc_ds_bs_post(TOTAL_HERO_COUNTER);
								inc_ds_bs_post(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));
								host_writebs(hero + HERO_GROUP_POS, i + 1);
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
						(char*)get_ltx(4 * (ds_readws(TEMPLE_GOD) + 21)),	/* name of the god */
						(char*)get_ltx(4 * (ds_readbs(CURRENT_TOWN) + 235)));
					GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
				}

				l_di = copy_chr_names(Real2Host(ptr), temple_id);
			}
		} while (l_si != -1 && ds_readbs(TOTAL_HERO_COUNTER) < (host_readbs(get_hero(6) + HERO_TYPE) ? 7 : 6));
	}
}

void char_letgo(signed short temple_id)
{
	signed short hero_pos;
	Bit8u *hero;

	if (!ds_readbs(TOTAL_HERO_COUNTER) || !ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
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
					dec_ds_bs_post(TOTAL_HERO_COUNTER);
					dec_ds_bs_post(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));

					host_writeb(hero + HERO_TEMPLE_ID, (signed char)temple_id);
					host_writeb(hero + HERO_GROUP_POS, 0);

					write_chr_temp(hero_pos);

					memset(hero, 0, SIZEOF_HERO);

					draw_main_screen();
					init_ani(2);

					/* location string */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x3ac),
						(char*)get_ltx(4 * (ds_readws(TEMPLE_GOD) + 21)),	/* name of the god */
						(char*)get_ltx(4 * (ds_readbs(CURRENT_TOWN) + 235)));
					GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
				}
			}

		} while (hero_pos != -1 && ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) > (host_readbs(get_hero(6) + HERO_TYPE) ? 1 : 0));
	}
}

signed short char_erase(void)
{
	signed short l_si;
	signed short l_di;
	signed short unlink_ret;
	RealPt ptr;

	if (ds_readbs(0x4c3a) != 0) {
		ptr = F_PADD((HugePt)ds_readd(BUFFER9_PTR), 30000);
	} else {
		ptr = (RealPt)ds_readd(BUFFER1_PTR) + 50000;
	}

	l_di = copy_chr_names(Real2Host(ptr), -1);

	do {
		if (!l_di) {
			l_si = -1;
		} else {
			l_si = menu_enter_delete(ptr, l_di, -1);

			if (l_si != -1) {
				strcpy((char*)Real2Host(ds_readd(DTP2)),
					(char*)Real2Host(ptr) + 32 * l_si);

				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					(char*)get_ltx(0x49c),
					(char*)Real2Host(ds_readd(DTP2)));

				if (GUI_bool(Real2Host(ds_readd(TEXT_OUTPUT_BUF)))) {

					prepare_chr_name((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
								(char*)Real2Host(ds_readd(DTP2)));

					unlink_ret = bc_unlink((RealPt)ds_readd(TEXT_OUTPUT_BUF));

					if (unlink_ret != 0) {
						GUI_output(get_ltx(0x498));
						return 0;
					}

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)p_datseg + STR_TEMP_FILE_WILDCARD,
						(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
					bc_unlink((RealPt)ds_readd(DTP2));
				}

				l_di = copy_chr_names(Real2Host(ptr), -1);

			} else {
				return 0;
			}
		}

	} while (l_si != -1);

	return 1;
}

/**
 * \brief		MIRACLE heal one hero
 * \param le_in		healable LE maximum
 * \param str		a format-string for the output
 */
void miracle_heal_hero(signed short le_in, Bit8u *str)
{
	signed short i;
	signed short le;
	signed short hero_pos;
	signed short le_diff;
	Bit8u *hero;

	le = 0;
	hero_pos = -1;

	/* search for the hero with the largest LE-difference */
	for (i = 0; i <= 6; i++) {
		hero = get_hero(i);

		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			!hero_dummy4(hero) &&
			!hero_dead(hero) &&
			((le_diff = host_readws(hero + HERO_LE_ORIG) - host_readws(hero + HERO_LE)) > le))
		{
			le = le_diff;
			hero_pos = i;
		}
	}

	if (hero_pos != -1) {

		/* adjust le_in if the maximum is not reached */
		if (le_in > le) {
			le_in = le;
		}

		add_hero_le(get_hero(hero_pos), le_in);

		/* prepare a message */
		strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_ltx(0x620));

		if (le_in > 1) {
			strcat((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_ltx(0x624));
		}

		sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)str,
				(char*)get_hero(hero_pos) + HERO_NAME2,
				le_in,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
	}
}

void miracle_resurrect(Bit8u *str)
{
	signed short i;
	Bit8u *hero;

	for (i = 0; i <= 6; i++) {
		hero = get_hero(i);

		if (hero_dead(hero) &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dummy4(hero))
		{

			/* resurrect from the dead */
			and_ptr_bs(hero + HERO_STATUS1, 0xfe);

			/* add 7 LE */
			add_hero_le(hero, 7);

			/* update_ the status line */
			draw_status_line();

			/* prepare a message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)str,
				(char*)hero + HERO_NAME2);

			break;
		}
	}
}

/**
 * \brief		MIRACLE modify all living group members
 * \param offset	offset in the datasheet
 * \param timer_value	how long should the modification last
 * \param mod		modification value
 */
void miracle_modify(unsigned short offset, Bit32s timer_value, signed short mod)
{
	signed short i;
	signed short slot;
	HugePt ptr;
	RealPt hero = (RealPt)ds_readd(HEROS);

	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readbs(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(Real2Host(hero) + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(Real2Host(hero)) &&
			!hero_dummy4(Real2Host(hero)))
		{
			slot = get_free_mod_slot();
			ptr = hero;
			ptr += offset;

			set_mod_slot(slot, timer_value, Real2Host(ptr), (signed char)mod, (signed char)i);
		}
	}
}

/**
 * \brief		MIRACLE repair or magicise weapon
 * \param str		format string for output
 * \param mode		0 = magic, != 0 repair
 */
void miracle_weapon(Bit8u *str, signed short mode)
{
	signed short i;
	signed short j;
	signed short done;
	signed short item_id;
	Bit8u *hero;

	j = done = 0;

	while (j <= 6 && done == 0) {

		hero = get_hero(j);

		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			!hero_dummy4(hero))
		{
			for (i = 0; i < 23; i++)
			{

				if ((item_id = host_readws(hero + HERO_ITEM_HEAD + 14 * i)) &&
					item_weapon(get_itemsdat(item_id)))
				{

					if (mode == 0) {
						/* make a non-broken, non-magic weapon magic */

						if (!ks_broken(hero + HERO_ITEM_HEAD + 14 * i) &&
							!ks_magic_hidden(hero + HERO_ITEM_HEAD + 14 * i))
						{
							/* weapon is magic and known */
							or_ptr_bs(hero + HERO_ITEM_HEAD + 4 + 14 * i, 0x08);
							or_ptr_bs(hero + HERO_ITEM_HEAD + 4 + 14 * i, 0x80);

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)str,
								(char*)Real2Host(GUI_names_grammar((signed short)0x8000, item_id, 0)),
								(char*)hero + HERO_NAME2);

							done = 1;
							break;
						}
					} else {
						/* repair a broken weapon */
						if (ks_broken(hero + HERO_ITEM_HEAD + 14 * i))
						{
							and_ptr_bs(hero + HERO_ITEM_HEAD + 4 + 14 * i, 0xfe);

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)str,
								(char*)Real2Host(GUI_names_grammar((signed short)0x8000, item_id, 0)),
								(char*)hero + HERO_NAME2);

							done = 1;
							break;
						}
					}
				}
			}
		}

		j++;
	}
}

#if !defined(__BORLANDC__)
}
#endif
