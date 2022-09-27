/*
 *      Rewrite of DSA1 v3.02_de functions of seg097 (GUI)
 *      Functions rewritten: 16/16 (complete)
 *
 *      Functions called rewritten 13/13
 *      Functions uncalled rewritten 3/3
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg097.cpp
 */

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "v302de.h"

#include "seg000.h"
#include "seg001.h"
#include "seg002.h"
#include "seg004.h"
#include "seg026.h"

#include "seg078.h"
#include "seg080.h"
#include "seg081.h"
#include "seg084.h"
#include "seg086.h"
#include "seg088.h"
#include "seg090.h"
#include "seg091.h"

#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void GUI_unused1(Bit8u *a1, signed short a2, signed short a3)
{
	signed short l1;
	signed short l2;
	signed char c;

	l1 = 0;

	update_mouse_cursor();

	if (ds_readws(GUI_TEXT_CENTERED) == 1) {
		a2 = GUI_get_first_pos_centered(a1, a2, ds_readws(TEXTLINE_MAXLEN), 1);
	}

	l2 = a2;

	while ((c = host_readbs(a1 + l1++))) {

		if ((c == 0x0d) || (c == 0x40)) {
			a3 += 10;

			a2 = (ds_readws(GUI_TEXT_CENTERED) == 1) ?
				GUI_get_first_pos_centered(a1 + l1, ds_readws(TEXTLINE_POSX), ds_readws(TEXTLINE_MAXLEN), 1) : l2;

		} else if (c == '~') {
			if (a2 < ds_readws(TXT_TABPOS1)) {
				a2 = ds_readws(TXT_TABPOS1);
			} else if (a2 < ds_readws(TXT_TABPOS2)) {
				a2 = ds_readws(TXT_TABPOS2);
			} else if (a2 < ds_readws(TXT_TABPOS3)) {
				a2 = ds_readws(TXT_TABPOS3);
			} else if (a2 < ds_readws(TXT_TABPOS4)) {
				a2 = ds_readws(TXT_TABPOS4);
			} else if (a2 < ds_readws(TXT_TABPOS5)) {
				a2 = ds_readws(TXT_TABPOS5);
			} else if (a2 < ds_readws(TXT_TABPOS6)) {
				a2 = ds_readws(TXT_TABPOS6);
			} else if (a2 < ds_readws(TXT_TABPOS7)) {
				a2 = ds_readws(TXT_TABPOS7);
			}
		} else if ((c == (signed char)0xf0) || (c == (signed char)0xf1) || (c == (signed char)0xf2) || (c == (signed char)0xf3)) {
			ds_writew(TEXTCOLOR, (unsigned char)c + 0xff10);
		} else {
			a2 += GUI_unused2(c, (RealPt)RealMake(a3, a2));
		}
	}

	refresh_screen_size();
}

signed short GUI_unused2(signed short c, RealPt p)
{
	signed short v1;
	signed short v2;

	v2 = GUI_lookup_char_height((signed char)c, &v1);
#if !defined(__BORLANDC__)
	/* BE-fix */
	v1 = host_readws((Bit8u*)&v1);
#endif
	GUI_1c2(v2, v1, p);

	return v1;
}

signed short GUI_lookup_char_height(signed char c, signed short *p)
{
	signed short i;

	for (i = 0; i != 201; i += 3) {
		if (c == ds_readbs(GUI_CHAR_HEIGHT + i)) {

			host_writew((Bit8u*)p, ((signed short)ds_readbs((GUI_CHAR_HEIGHT + 2) + i)) & 0xff);
			return ds_readbs((GUI_CHAR_HEIGHT + 1) + i) & 0xff;
		}
	}

	if ((c == (signed char)'~')
			|| (c == (signed char)0xf0)
			|| (c == (signed char)0xf1)
			|| (c == (signed char)0xf2)
			|| (c == (signed char)0xf3))
	{
		return host_writews((Bit8u*)p, 0);
	}

	host_writew((Bit8u*)p, 8);

	return 0;
}

void GUI_1c2(signed short v1, signed short v2, RealPt v3)
{

	GUI_blank_char();
	GUI_font_to_buf(Real2Host(ds_readd(SPLASH_BUFFER)) + v1 * 8);
	GUI_write_char_to_screen(v3, 8, v2);
}

//static
signed short GUI_enter_text(Bit8u* dst, signed short x, signed short y, signed short num, signed short zero)
{
	signed short di;
	register signed short si;
	signed short pos;
	signed short c;
	Bit8u *dst_start;
	signed short length;

	dst_start = dst;
	update_mouse_cursor();
	di = x;
	pos = 0;
	length = strlen((char*)dst);

	if (zero == 0) {
		for (si = 0; si < num; si++) {
			if ((ds_readws(GUI_ENTERING_SAVEGAME) != 0) && (length >= si)) {
				GUI_print_char(0x20, di, y);
				GUI_print_char(*dst, di, y);
				pos++;
				dst++;
				di += 6;
				x = di;
			} else {
				GUI_print_char(0x20, di, y);
				GUI_print_char(0x5f, di, y);
				di += 6;
			}
		}
		di = x;
	} else {
		GUI_print_char(0x20, di, y);
		GUI_print_char(0x5f, di, y);
	}

	wait_for_keyboard2();
	ds_writew(MOUSE1_EVENT1, 0);

	c = 0;
	while ((c != 0xd) || (pos == 0)) {
		do {
			goto dummy;
dummy:

			/* This loop is evil */
			do {;} while ((CD_bioskey(1) == 0) && (ds_readws(MOUSE1_EVENT1) == 0));

			if (ds_readws(MOUSE1_EVENT1) != 0) {
				ds_writew(BIOSKEY_EVENT, 0x0d);
				ds_writew(MOUSE1_EVENT1, ds_writew(MOUSE1_EVENT2, 0x00));
			} else {
				ds_writew(ACTION, (signed short)(ds_writew(BIOSKEY_EVENT, bc_bioskey(0))) >> 8);
				and_ds_ws(BIOSKEY_EVENT, 0xff);
			}

		} while ((ds_readws(ACTION) == 0) && (ds_readws(BIOSKEY_EVENT) == 0));

		c = ds_readws(BIOSKEY_EVENT);

		if (c == 0x0d) {

		} else if (ds_readw(ACTION) == ACTION_ID_ESC) {
			host_writeb(dst_start, 0);
			refresh_screen_size();
			ds_writew(ACTION, 0);
			return -1;
		} else if (c == 8) {
			if (pos > 0) {

				if ((zero == 1) && (pos != num)) {
					GUI_print_char(0x20, di, y);
				}

				di -= 6;
				pos--;
				dst--;
				GUI_print_char(0x20, di, y);
				GUI_print_char(0x5f, di, y);
			}
		} else

		/* check if character is valid */
		if (((c >= 0x20) && (c <= 0x7a)) ||
			(c == 0x81) || (c == 0x84) || (c == 0x94))
		{
			/* is_alpha(c) */
			if (ds_readb(CHAR_TYPE_TABLE + c) & 0xc)
				c = toupper(c);

			/* ae */
			if (c == 0x84)
				c = (signed char)0x8e;
			/* oe */
			if (c == 0x94)
				c = (signed char)0x99;
			/* ue */
			if (c == 0x81)
				c = (signed char)0x9a;

			/* are we at the end of the input field ? */
			if (pos == num) {
				dst--;
				di -= 6;
				pos--;
			}

			host_writeb(dst++, (unsigned char)c);
			GUI_print_char(0x20, di, y);
			GUI_print_char((unsigned char)c, di, y);
			di += 6;
			pos++;

			if ((zero == 1) && (pos != num)) {
				GUI_print_char(0x20, di, y);
				GUI_print_char(0x5f, di, y);
			}
		}
	}

	if (zero == 0) {
		while (pos < num) {
			GUI_print_char(0x20, di, y);
			di += 6;
			pos++;
		}
	}

	host_writeb(dst, 0);
	refresh_screen_size();

	return 0;
}

//static
void GUI_draw_radio_bg(signed short header, signed short options, signed short width,
								signed short height)
{
	signed short i;

	/* set upper left coordinates */
	ds_writew(PIC_COPY_X1, ds_readw(TEXTBOX_POS_X));
	ds_writew(PIC_COPY_Y1, ds_readw(TEXTBOX_POS_Y));
	/* set lower righti coordinates */
	ds_writew(PIC_COPY_X2, ds_readw(TEXTBOX_POS_X) + width - 1);
	ds_writew(PIC_COPY_Y2, ds_readw(TEXTBOX_POS_Y) + height - 1);
	/* set pointer */
	ds_writed(PIC_COPY_SRC, ds_readd(GUI_BUFFER_UNKN));
	do_save_rect();

	/* play FX3.VOC */
	if (ds_readw(PREGAME_STATE) == 0)
		play_voc(ARCHIVE_FILE_FX3_VOC);

	GUI_draw_popup_line(0, 0);

	for (i = 0; i < header; i++)
		GUI_draw_popup_line(i + 1, 1);

	if (options != 0)
		for (i = 0; i < options; i++)
			GUI_draw_popup_line(header + i + 1, 2);

	GUI_draw_popup_line(header + options + 1, 3);

	set_textcolor(0xff, 0xdf);
	wait_for_keyboard1();
}

void GUI_copy_smth(unsigned short width, unsigned short height)
{
	ds_writew(PIC_COPY_X1, ds_readw(TEXTBOX_POS_X));
	ds_writew(PIC_COPY_Y1, ds_readw(TEXTBOX_POS_Y));
	ds_writew(PIC_COPY_X2, ds_readw(TEXTBOX_POS_X) + width - 1);
	ds_writew(PIC_COPY_Y2, ds_readw(TEXTBOX_POS_Y) + height - 1);
	ds_writed(PIC_COPY_SRC, ds_readd(GUI_BUFFER_UNKN));
	do_pic_copy(0);
}

void GUI_output(Bit8u *str)
{
	GUI_input(str, 0);
}

signed short GUI_input(Bit8u *str, unsigned short num)
{
	signed short retval;
	signed short l2;
	signed short fg_bak;
	signed short bg_bak;
	signed short l3;
	signed short l4;
	signed short l5;
	signed short l6;
	signed short l7;
	signed short l_si;
	signed short l_di;

	retval = 0;

	l7 = ds_readw(UPDATE_STATUSLINE);
	ds_writew(UPDATE_STATUSLINE, 0);

	if (!NOT_NULL(str) || !host_readbs(str) || ds_readw(AUTOFIGHT) != 0)
		return -1;

	l6 = ds_readw(WALLCLOCK_UPDATE);
	ds_writew(WALLCLOCK_UPDATE, 0);
	ds_writeb(DIALOGBOX_LOCK, 1);
	ds_writew(GUI_TEXT_CENTERED, 1);

	l3 = ds_readw(TEXTLINE_POSX);
	l4 = ds_readw(TEXTLINE_POSY);
	l5 = ds_readw(TEXTLINE_MAXLEN);

	l_di = (ds_readw(TEXTBOX_WIDTH) * 32) + 32;
	ds_writew(TEXTBOX_POS_X, ((signed short)(320u - l_di) >> 1) + ds_readws(BASEPOS_X));

	ds_writew(TEXTLINE_POSX, ds_readw(TEXTBOX_POS_X) + 5);
	ds_writew(TEXTLINE_MAXLEN, l_di - 8);

	l_si = GUI_count_lines(str);

	if (num != 0)
		l_si += 2;

	l2 = (l_si + 2) * 8;

	ds_writew(TEXTBOX_POS_Y, ((signed short)(200u - l2) >> 1) + ds_readw(BASEPOS_Y));
	ds_writew(TEXTLINE_POSY, ds_readw(TEXTBOX_POS_Y) + 7);

	get_textcolor(&fg_bak, &bg_bak);

	update_mouse_cursor();

	GUI_draw_radio_bg(l_si, 0, l_di, l2);

	GUI_print_header(str);

	ds_writew(MOUSE2_EVENT, 0);

	refresh_screen_size();

	if (num != 0) {
		if (GUI_enter_text(Real2Host(ds_readd(TEXT_INPUT_BUF)), ds_readws(TEXTBOX_POS_X) + ((signed short)(l_di - num * 6) >> 1), ds_readws(TEXTBOX_POS_Y) + l_si * 8 -2, num, 0) != -1) {
			retval = (signed short)atol((char*)Real2Host(ds_readd(TEXT_INPUT_BUF)));
		} else {
			retval = -1;
		}
	} else {
		/* set action table */
		ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)RealMake(datseg, ACTION_TABLE_MENU));

		if (ds_readw(BIOSKEY_EVENT10) != 0) {
			wait_for_keypress();
		} else {
			delay_or_keypress(l_si * ds_readw(TEXTBOX_WIDTH) * 50);
		}

		/* delete action table */
		ds_writed(ACTION_TABLE_SECONDARY, 0);
	}

	set_textcolor(fg_bak, bg_bak);

	update_mouse_cursor();

	GUI_copy_smth(l_di, l2);

	refresh_screen_size();

	ds_writew(TEXTLINE_POSX, l3);
	ds_writew(TEXTLINE_POSY, l4);
	ds_writew(TEXTLINE_MAXLEN, l5);

	ds_writew(ACTION, 0);
	ds_writeb(DIALOGBOX_LOCK, 0);

	ds_writew(WALLCLOCK_UPDATE, l6);
	ds_writew(GUI_TEXT_CENTERED, 0);
	ds_writew(UPDATE_STATUSLINE, l7);

	return retval;
}

/**
 * \brief   shows a text box with two radio buttons "Ja" (yes) and "Nein" (no).
 *
 * \param   text	the displayed text
 */
signed short GUI_bool(Bit8u *text)
{
	signed short ret_radio;

	ds_writew(GUI_BOOL_FLAG, 1);

	ret_radio = GUI_radio(text, 2, get_ttx(2), get_ttx(3));
	ds_writew(GUI_BOOL_FLAG, 0);

	return (ret_radio == 1) ? 1 : 0;
}


//static
void GUI_fill_radio_button(signed short old_pos, unsigned short new_pos,
	unsigned short offset)
{
	signed short i;
	signed short x;
	signed short y;

	update_mouse_cursor();

	y = ds_readw(TEXTBOX_POS_X) + 6;

	if (old_pos != -1) {

		x = ds_readws(TEXTBOX_POS_Y) + (offset + old_pos) * 8 + 2;

		/* clear the old button */
		for (i = 0; i < 4; i++)
			do_v_line((RealPt)ds_readd(FRAMEBUF_PTR), y + i, x, x + 3,
				(signed char)0xd8);
	}

	x = ds_readws(TEXTBOX_POS_Y) + (offset + new_pos) * 8 + 2;

	/* fill the new button */
	for (i = 0; i < 4; i++)
		do_v_line((RealPt)ds_readd(FRAMEBUF_PTR), y + i, x, x + 3,
			(signed char)0xd9);

	refresh_screen_size();
}

signed short GUI_dialogbox(RealPt picture, Bit8u *name, Bit8u *text,
		signed short options, ...)
{
	va_list arguments;
	signed short i;
	signed short l2, l3, l4, l5, l6;
	signed short fg_bak, bg_bak;
	signed short l7, l8, l9, l10;
	signed short retval;
	signed short l11, l12, l13;
	signed short l_si, l_di;

	l13 = ds_readw(ANI_ENABLED);
	l12 = ds_readw(UPDATE_STATUSLINE);
	ds_writew(UPDATE_STATUSLINE, 0);

	set_var_to_zero();

	l11 = ds_readw(WALLCLOCK_UPDATE);
	ds_writew(WALLCLOCK_UPDATE, 0);
	ds_writeb(DIALOGBOX_LOCK, 1);
	l7 = ds_readw(TEXTLINE_POSX);
	l8 = ds_readw(TEXTLINE_POSY);
	l9 = ds_readw(TEXTLINE_MAXLEN);
	l6 = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 9);

	l_di = ds_readw(TEXTBOX_WIDTH) * 32 + 32;
	ds_writew(TEXTBOX_POS_X, ((signed short)(320 - l_di) >> 1) + ds_readw(BASEPOS_X));
	ds_writew(TEXTLINE_POSX, ds_readw(TEXTBOX_POS_X) + 5);
	ds_writew(TEXTLINE_MAXLEN, l_di - 8);
	l10 = ds_readw(TXT_TABPOS1);
	ds_writew(TXT_TABPOS1, ds_readws(TEXTLINE_POSX) + ds_readws(TEXTLINE_MAXLEN) - 24);
	ds_writew(DIALOGBOX_INDENT_WIDTH, 40);
	ds_writew(DIALOGBOX_INDENT_HEIGHT, 5);

	l_si = GUI_count_lines(text) - 1;

	if (NOT_NULL(name))
		l_si += 2;

	if (l_si < ds_readws(DIALOGBOX_INDENT_HEIGHT))
		l_si = ds_readw(DIALOGBOX_INDENT_HEIGHT) - 1;

	l4 = l_si + (signed char)options;
	l5 = (l4 + 2) * 8;
	ds_writew(TEXTBOX_POS_Y, (200 - (l5 + 2)) >> 1);
	ds_writew(TEXTLINE_POSY, ds_readw(TEXTBOX_POS_Y) + 5);

	update_mouse_cursor();
	get_textcolor(&fg_bak, &bg_bak);

	GUI_draw_radio_bg(l_si, (signed char)options, l_di, l5);

	if (picture != 0) {
		/* draw a frame */
		do_border((RealPt)ds_readd(FRAMEBUF_PTR),
			ds_readw(TEXTBOX_POS_X) + 5, ds_readw(TEXTBOX_POS_Y) + 6,
			ds_readw(TEXTBOX_POS_X) + 38, ds_readw(TEXTBOX_POS_Y) + 39,
				(signed char)0xff);

		/* set the coordinates */
		ds_writew(PIC_COPY_X1, ds_readw(TEXTBOX_POS_X) + 6);
		ds_writew(PIC_COPY_Y1, ds_readw(TEXTBOX_POS_Y) + 7);
		ds_writew(PIC_COPY_X2, ds_readw(TEXTBOX_POS_X) + 37);
		ds_writew(PIC_COPY_Y2, ds_readw(TEXTBOX_POS_Y) + 38);
		ds_writed(PIC_COPY_SRC, (Bit32u)picture);

		do_pic_copy(0);
	}

	if (NOT_NULL(name)) {
		/* set text color */
		ds_writew(TEXTCOLOR, 1);

		GUI_print_string(name, ds_readw(TEXTLINE_POSX), ds_readw(TEXTLINE_POSY));

		/* set text color */
		ds_writew(TEXTCOLOR, 0);

		add_ds_ws(TEXTLINE_POSY, 14);
		sub_ds_ws(DIALOGBOX_INDENT_HEIGHT, 2);
	}

	if (l_si != 0) {
		GUI_print_header(text);
	}

	ds_writew(DIALOGBOX_INDENT_WIDTH, ds_writew(DIALOGBOX_INDENT_HEIGHT, 0));

	if ((signed char)options != 0) {
		l2 = ds_readw(TEXTLINE_POSX) + 8;
		l3 = ds_readws(TEXTBOX_POS_Y) + (l_si + 1) * 8;

		va_start(arguments, options);
		for (i = 0; i < (signed char)options; l3 += 8, i++) {
			GUI_print_string((Bit8u*)va_arg(arguments, char*), l2, l3);
		}
	}

	va_end(arguments);

	retval = GUI_menu_input((signed char)options, l_si + 1, l_di);

	GUI_copy_smth(l_di, l5);

	refresh_screen_size();
	set_textcolor(fg_bak, bg_bak);

	ds_writew(TEXTLINE_POSX, l7);
	ds_writew(TEXTLINE_POSY, l8);
	ds_writew(TEXTLINE_MAXLEN, l9);

	ds_writew(TEXTBOX_WIDTH, l6);

	ds_writew(TXT_TABPOS1, l10);

	ds_writew(WALLCLOCK_UPDATE, l11);

	ds_writew(ACTION, ds_writebs(DIALOGBOX_LOCK, 0));

	ds_writew(UPDATE_STATUSLINE, l12);

	if (l13 != 0)
		init_ani(2);

	return retval;
}

//static
signed short GUI_menu_input(signed short positions, signed short h_lines,
			signed short width)
{
	volatile signed short l1, l2, l3, l4, l5, l6;
	signed short done;
	signed short retval;

	l5 = -1;
	done = 0;

	ds_writew(MENU_SELECTED, ds_writew(MENU_INPUT_BUSY, 1));
	add_ds_ws(MENU_SELECTED, ds_readws(MENU_DEFAULT_SELECT));

	if (positions != 0) {
		l6 = h_lines * 8;
		l3 = ds_readw(MOUSE_POSX);
		l4 = ds_readw(MOUSE_POSY);
		ds_writew(MOUSE_POSX_BAK, ds_writew(MOUSE_POSX, ds_readw(TEXTBOX_POS_X) + 90));
		l1 = ds_readws(TEXTBOX_POS_Y) + l6;

		ds_writew(MOUSE_POSY_BAK, ds_writew(MOUSE_POSY, (l2 = l1 + ds_readws(MENU_DEFAULT_SELECT) * 8)));

		mouse_move_cursor(ds_readw(MOUSE_POSX), ds_readw(MOUSE_POSY));

		ds_writew(MOUSE_POSX_MAX, ds_readws(TEXTBOX_POS_X) + width - 16);
		ds_writew(MOUSE_POSX_MIN, ds_readws(TEXTBOX_POS_X));
		ds_writew(MOUSE_POSY_MIN, ds_readws(TEXTBOX_POS_Y) + l6);

		ds_writew(MOUSE_POSY_MAX, l6 + ds_readws(TEXTBOX_POS_Y) - 1 + positions * 8);
		refresh_screen_size();

		ds_writew(MOUSE1_EVENT2, ds_writew(MOUSE1_EVENT1, ds_writew(MOUSE2_EVENT, 0)));

		while (!done) {
			ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)RealMake(datseg, ACTION_TABLE_MENU));
			handle_input();
			ds_writed(ACTION_TABLE_SECONDARY, 0);

			if (l5 != ds_readw(MENU_SELECTED)) {
				GUI_fill_radio_button(l5, ds_readw(MENU_SELECTED),
					h_lines - 1);
				l5 = ds_readw(MENU_SELECTED);
			}

			if (ds_readw(MOUSE2_EVENT) != 0 ||
				ds_readw(ACTION) == ACTION_ID_ESC ||
				ds_readw(ACTION) == ACTION_ID_PAGE_DOWN) {
				/* close menu */

				retval = -1;
				done = 1;
				ds_writew(MOUSE2_EVENT, 0);
			}

			if (ds_readw(ACTION) == ACTION_ID_RETURN) {
				retval = ds_readw(MENU_SELECTED);
				done = 1;
			}

			if (ds_readw(ACTION) == ACTION_ID_UP) {
				if (dec_ds_ws_post(MENU_SELECTED) == 1)
					ds_writew(MENU_SELECTED, positions);
			}
			if (ds_readw(ACTION) == ACTION_ID_DOWN) {
				if (inc_ds_ws_post(MENU_SELECTED) == positions)
					ds_writew(MENU_SELECTED, 1);
			}

			if (ds_readw(MOUSE_POSY) != l2) {
				l2 = ds_readw(MOUSE_POSY);
				ds_writew(MENU_SELECTED, ((l2 - l1) >> 3) + 1);
			}

			if (ds_readw(GUI_BOOL_FLAG) != 0) {
				/* in yes-no-mode, answer "Ja" (yes) can be selected with the 'J' key, and answer "Nein" (no) can be selected with the 'N' key. */
				if (ds_readw(ACTION) == ACTION_ID_J) {
					retval = done = 1;
				} else if (ds_readw(ACTION) == ACTION_ID_N) {
					retval = 2;
					done = 1;
				}
			}
		}

		update_mouse_cursor();

		ds_writew(MOUSE_POSX_BAK, ds_writew(MOUSE_POSX, l3));
		ds_writew(MOUSE_POSY_BAK, ds_writew(MOUSE_POSY, l4));
		ds_writew(MOUSE_POSX_MAX, 319);
		ds_writew(MOUSE_POSX_MIN, 0);
		ds_writew(MOUSE_POSY_MIN, 0);
		ds_writew(MOUSE_POSY_MAX, 199);

		mouse_move_cursor(ds_readw(MOUSE_POSX), ds_readw(MOUSE_POSY));

	} else {
		do {
			delay_or_keypress(10000);
		} while (ds_readw(ACTION) == 0);

		retval = -1;
	}

	ds_writew(MENU_DEFAULT_SELECT, ds_writew(MENU_INPUT_BUSY, 0));

	return retval;
}

signed short GUI_radio(Bit8u *text, signed char options, ...)
{
	signed short i;
	signed short l_di;

	va_list arguments;
	signed short l3, l4, l5, l6;
	signed short fg_bak, bg_bak;
	signed short l7, l8, l9, l10, l11;
	signed short retval;
	signed short l12;

	l12 = ds_readw(UPDATE_STATUSLINE);
	ds_writew(UPDATE_STATUSLINE, 0);

	if (!options) {
		GUI_output(text);
		return 0;
	}


	ds_writeb(DIALOGBOX_LOCK, 1);
	l7 = ds_readw(TEXTLINE_POSX);
	l8 = ds_readw(TEXTLINE_POSY);
	l9 = ds_readw(TEXTLINE_MAXLEN);

	l11 = ds_readw(TEXTBOX_WIDTH) * 32 + 32;
	ds_writew(TEXTBOX_POS_X, ((320 - l11) >> 1) + ds_readw(BASEPOS_X));
	ds_writew(TEXTLINE_POSX, ds_readw(TEXTBOX_POS_X) + 5);
	ds_writew(TEXTLINE_MAXLEN, l11 - 8);

	l10 = ds_readw(TXT_TABPOS1);
	ds_writew(TXT_TABPOS1, ds_readws(TEXTLINE_POSX) + ds_readws(TEXTLINE_MAXLEN) - 24);

	l_di = GUI_count_lines(text);
	l5 = l_di + options;
	l6 = (l5 + 2) * 8;
	ds_writew(TEXTBOX_POS_Y, ((200 - l6 + 2) >> 1) + ds_readw(BASEPOS_Y));
	ds_writew(TEXTLINE_POSY, ds_readw(TEXTBOX_POS_Y) + 7);

	update_mouse_cursor();
	get_textcolor(&fg_bak, &bg_bak);

	GUI_draw_radio_bg(l_di, options, l11, l6);

	if (l_di != 0)
		GUI_print_header(text);

	l3 = ds_readw(TEXTLINE_POSX) + 8;
	l4 = ds_readws(TEXTBOX_POS_Y) + (l_di + 1) * 8;

	va_start(arguments, options);
	for (i = 0; i < options; l4 += 8, i++) {

		/* highlight special option */
		if (ds_readw(GAME_MODE) == GAME_MODE_BEGINNER && ds_readw(SKILLED_HERO_POS) == i)
			set_textcolor(0xc9, 0xdf);

		GUI_print_string((Bit8u*)va_arg(arguments, char*), l3, l4);

		/* reset highlight special option */
		if (ds_readw(GAME_MODE) == GAME_MODE_BEGINNER && ds_readw(SKILLED_HERO_POS) == i)
			set_textcolor(0xff, 0xdf);
	}

	retval = GUI_menu_input(options, l_di + 1, l11);

	GUI_copy_smth(l11, l6);
	refresh_screen_size();
	set_textcolor(fg_bak, bg_bak);

	ds_writew(TEXTLINE_POSX, l7);
	ds_writew(TEXTLINE_POSY, l8);
	ds_writew(TEXTLINE_MAXLEN, l9);
	ds_writew(TXT_TABPOS1, l10);
	ds_writew(ACTION, ds_writebs(DIALOGBOX_LOCK, 0));
	ds_writew(UPDATE_STATUSLINE, l12);

	return retval;
}

/**
 * \brief   shows a fight intro message
 *
 * \param   fight_id    ID of the fight
 */
void GUI_print_fight_intro_msg(signed short fight_id)
{
	signed short textbox_width_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	if (ds_readbs(DUNGEON_INDEX) == DUNGEONS_VERFALLENE_HERBERGE) {
		DNG02_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == DUNGEONS_GOBLINHOEHLE) {
		DNG5_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == DUNGEONS_DASPOTASCHATZ) {
		DNG06_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == DUNGEONS_KULTSTAETTE_DES_NAMENLOSEN) {
		DNG09_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == DUNGEONS_PIRATENHOEHLE) {
		DNG11_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == DUNGEONS_ZWERGENFESTE) {
		DNG12_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == DUNGEONS_VERLASSENE_MINE) {
		DNG13_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == DUNGEONS_ZWINGFESTE) {
		DNG14_fight_intro(fight_id);
	}

	ds_writew(TEXTBOX_WIDTH, textbox_width_bak);
}

/**
 * \brief   print a Dialog windows without answers
 *
 * \param   head_index  the number of a head, if another should be loaded
 * \param   text        the text
 */
void GUI_dialog_na(unsigned short head_index, Bit8u *text)
{

	if (head_index != 0)
		load_in_head(head_index);

	GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, text, 0);

}

#if !defined(__BORLANDC__)
}
#endif
