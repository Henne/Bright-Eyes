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

	if (ds_readws(0xd2d1) == 1) {
		a2 = GUI_get_first_pos_centered(a1, a2, ds_readws(0xd2d5), 1);
	}

	l2 = a2;

	while ((c = host_readbs(a1 + l1++))) {

		if ((c == 0x0d) || (c == 0x40)) {
			a3 += 10;

			a2 = (ds_readws(0xd2d1) == 1) ?
				GUI_get_first_pos_centered(a1 + l1, ds_readws(0xd2d9), ds_readws(0xd2d5), 1) : l2;

		} else if (c == 0x7e) {
			if (a2 < ds_readws(0xd313)) {
				a2 = ds_readws(0xd313);
			} else if (a2 < ds_readws(0xd315)) {
				a2 = ds_readws(0xd315);
			} else if (a2 < ds_readws(0xd317)) {
				a2 = ds_readws(0xd317);
			} else if (a2 < ds_readws(0xd319)) {
				a2 = ds_readws(0xd319);
			} else if (a2 < ds_readws(0xd31b)) {
				a2 = ds_readws(0xd31b);
			} else if (a2 < ds_readws(0xd31d)) {
				a2 = ds_readws(0xd31d);
			} else if (a2 < ds_readws(0xd31f)) {
				a2 = ds_readws(0xd31f);
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
		if (c == ds_readbs(0xab42 + i)) {

			host_writew((Bit8u*)p, ((signed short)ds_readbs((0xab42 + 2) + i)) & 0xff);
			return ds_readbs((0xab42 + 1) + i) & 0xff;
		}
	}

	if ((c == (signed char)0x7e)
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
	ds_writew(0xc3d1, 0);

	c = 0;
	while ((c != 0xd) || (pos == 0)) {
		do {
			goto dummy;
dummy:

			/* This loop is evil */
			do {;} while ((CD_bioskey(1) == 0) && (ds_readws(0xc3d1) == 0));

			if (ds_readws(0xc3d1) != 0) {
				ds_writew(0xc3d7, 0x0d);
				ds_writew(0xc3d1, ds_writew(0xc3d5, 0x00));
			} else {
				ds_writew(ACTION, (signed short)(ds_writew(0xc3d7, bc_bioskey(0))) >> 8);
				and_ds_ws(0xc3d7, 0xff);
			}

		} while ((ds_readws(ACTION) == 0) && (ds_readws(0xc3d7) == 0));

		c = ds_readws(0xc3d7);

		if (c == 0x0d) {

		} else if (ds_readw(ACTION) == 1) {
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
			if (ds_readb(0xb4e9 + c) & 0xc)
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
	ds_writew(0xc011, ds_readw(TEXTBOX_POS_X));
	ds_writew(0xc013, ds_readw(TEXTBOX_POS_Y));
	/* set lower righti coordinates */
	ds_writew(0xc015, ds_readw(TEXTBOX_POS_X) + width - 1);
	ds_writew(0xc017, ds_readw(TEXTBOX_POS_Y) + height - 1);
	/* set pointer */
	ds_writed(0xc019, ds_readd(0xbff9));
	do_save_rect();

	/* play FX3.VOC */
	if (ds_readw(0xbd25) == 0)
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
	ds_writew(0xc011, ds_readw(TEXTBOX_POS_X));
	ds_writew(0xc013, ds_readw(TEXTBOX_POS_Y));
	ds_writew(0xc015, ds_readw(TEXTBOX_POS_X) + width - 1);
	ds_writew(0xc017, ds_readw(TEXTBOX_POS_Y) + height - 1);
	ds_writed(0xc019, ds_readd(0xbff9));
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

	l7 = ds_readw(0xc3cb);
	ds_writew(0xc3cb, 0);

	if (!NOT_NULL(str) || !host_readbs(str) || ds_readw(AUTOFIGHT) != 0)
		return -1;

	l6 = ds_readw(WALLCLOCK_UPDATE);
	ds_writew(WALLCLOCK_UPDATE, 0);
	ds_writeb(0x2c98, 1);
	ds_writew(0xd2d1, 1);

	l3 = ds_readw(0xd2d9);
	l4 = ds_readw(0xd2d7);
	l5 = ds_readw(0xd2d5);

	l_di = (ds_readw(TEXTBOX_WIDTH) * 32) + 32;
	ds_writew(TEXTBOX_POS_X, ((signed short)(320u - l_di) >> 1) + ds_readws(0x2ca2));

	ds_writew(0xd2d9, ds_readw(TEXTBOX_POS_X) + 5);
	ds_writew(0xd2d5, l_di - 8);

	l_si = GUI_count_lines(str);

	if (num != 0)
		l_si += 2;

	l2 = (l_si + 2) * 8;

	ds_writew(TEXTBOX_POS_Y, ((signed short)(200u - l2) >> 1) + ds_readw(0x2ca4));
	ds_writew(0xd2d7, ds_readw(TEXTBOX_POS_Y) + 7);

	get_textcolor(&fg_bak, &bg_bak);

	update_mouse_cursor();

	GUI_draw_radio_bg(l_si, 0, l_di, l2);

	GUI_print_header(str);

	ds_writew(0xc3d3, 0);

	refresh_screen_size();

	if (num != 0) {
		if (GUI_enter_text(Real2Host(ds_readd(TEXT_INPUT_BUF)), ds_readws(TEXTBOX_POS_X) + ((signed short)(l_di - num * 6) >> 1), ds_readws(TEXTBOX_POS_Y) + l_si * 8 -2, num, 0) != -1) {
			retval = (signed short)atol((char*)Real2Host(ds_readd(TEXT_INPUT_BUF)));
		} else {
			retval = -1;
		}
	} else {
		/* set action table */
		ds_writed(0x29e4, (Bit32u)RealMake(datseg, 0x29cc));

		if (ds_readw(0xc3c5) != 0) {
			wait_for_keypress();
		} else {
			delay_or_keypress(l_si * ds_readw(TEXTBOX_WIDTH) * 50);
		}

		/* delete action table */
		ds_writed(0x29e4, 0);
	}

	set_textcolor(fg_bak, bg_bak);

	update_mouse_cursor();

	GUI_copy_smth(l_di, l2);

	refresh_screen_size();

	ds_writew(0xd2d9, l3);
	ds_writew(0xd2d7, l4);
	ds_writew(0xd2d5, l5);

	ds_writew(ACTION, 0);
	ds_writeb(0x2c98, 0);

	ds_writew(WALLCLOCK_UPDATE, l6);
	ds_writew(0xd2d1, 0);
	ds_writew(0xc3cb, l7);

	return retval;
}

signed short GUI_bool(Bit8u *text)
{
	signed short ret_radio;

	ds_writew(0xac0b, 1);

	ret_radio = GUI_radio(text, 2, get_ltx(0x08), get_ltx(0x0c));
	ds_writew(0xac0b, 0);

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
			do_v_line((RealPt)ds_readd(0xd2ff), y + i, x, x + 3,
				(signed char)0xd8);
	}

	x = ds_readws(TEXTBOX_POS_Y) + (offset + new_pos) * 8 + 2;

	/* fill the new button */
	for (i = 0; i < 4; i++)
		do_v_line((RealPt)ds_readd(0xd2ff), y + i, x, x + 3,
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

	l13 = ds_readw(0x29ae);
	l12 = ds_readw(0xc3cb);
	ds_writew(0xc3cb, 0);

	set_var_to_zero();

	l11 = ds_readw(WALLCLOCK_UPDATE);
	ds_writew(WALLCLOCK_UPDATE, 0);
	ds_writeb(0x2c98, 1);
	l7 = ds_readw(0xd2d9);
	l8 = ds_readw(0xd2d7);
	l9 = ds_readw(0xd2d5);
	l6 = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 9);

	l_di = ds_readw(TEXTBOX_WIDTH) * 32 + 32;
	ds_writew(TEXTBOX_POS_X, ((signed short)(320 - l_di) >> 1) + ds_readw(0x2ca2));
	ds_writew(0xd2d9, ds_readw(TEXTBOX_POS_X) + 5);
	ds_writew(0xd2d5, l_di - 8);
	l10 = ds_readw(0xd313);
	ds_writew(0xd313, ds_readws(0xd2d9) + ds_readws(0xd2d5) - 24);
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
	ds_writew(0xd2d7, ds_readw(TEXTBOX_POS_Y) + 5);

	update_mouse_cursor();
	get_textcolor(&fg_bak, &bg_bak);

	GUI_draw_radio_bg(l_si, (signed char)options, l_di, l5);

	if (picture != 0) {
		/* draw a frame */
		do_border((RealPt)ds_readd(0xd2ff),
			ds_readw(TEXTBOX_POS_X) + 5, ds_readw(TEXTBOX_POS_Y) + 6,
			ds_readw(TEXTBOX_POS_X) + 38, ds_readw(TEXTBOX_POS_Y) + 39,
				(signed char)0xff);

		/* set the coordinates */
		ds_writew(0xc011, ds_readw(TEXTBOX_POS_X) + 6);
		ds_writew(0xc013, ds_readw(TEXTBOX_POS_Y) + 7);
		ds_writew(0xc015, ds_readw(TEXTBOX_POS_X) + 37);
		ds_writew(0xc017, ds_readw(TEXTBOX_POS_Y) + 38);
		ds_writed(0xc019, (Bit32u)picture);

		do_pic_copy(0);
	}

	if (NOT_NULL(name)) {
		/* set text color */
		ds_writew(TEXTCOLOR, 1);

		GUI_print_string(name, ds_readw(0xd2d9), ds_readw(0xd2d7));

		/* set text color */
		ds_writew(TEXTCOLOR, 0);

		add_ds_ws(0xd2d7, 14);
		sub_ds_ws(DIALOGBOX_INDENT_HEIGHT, 2);
	}

	if (l_si != 0) {
		GUI_print_header(text);
	}

	ds_writew(DIALOGBOX_INDENT_WIDTH, ds_writew(DIALOGBOX_INDENT_HEIGHT, 0));

	if ((signed char)options != 0) {
		l2 = ds_readw(0xd2d9) + 8;
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

	ds_writew(0xd2d9, l7);
	ds_writew(0xd2d7, l8);
	ds_writew(0xd2d5, l9);

	ds_writew(TEXTBOX_WIDTH, l6);

	ds_writew(0xd313, l10);

	ds_writew(WALLCLOCK_UPDATE, l11);

	ds_writew(ACTION, ds_writebs(0x2c98, 0));

	ds_writew(0xc3cb, l12);

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

	ds_writew(0xe5b0, ds_writew(0xe5ae, 1));
	add_ds_ws(0xe5b0, ds_readws(0xe5ac));

	if (positions != 0) {
		l6 = h_lines * 8;
		l3 = ds_readw(0x299c);
		l4 = ds_readw(0x299e);
		ds_writew(0x29a0, ds_writew(0x299c, ds_readw(TEXTBOX_POS_X) + 90));
		l1 = ds_readws(TEXTBOX_POS_Y) + l6;

		ds_writew(0x29a2, ds_writew(0x299e, (l2 = l1 + ds_readws(0xe5ac) * 8)));

		mouse_move_cursor(ds_readw(0x299c), ds_readw(0x299e));

		ds_writew(0x298e, ds_readws(TEXTBOX_POS_X) + width - 16);
		ds_writew(0x298a, ds_readws(TEXTBOX_POS_X));
		ds_writew(0x2988, ds_readws(TEXTBOX_POS_Y) + l6);

		ds_writew(0x298c, l6 + ds_readws(TEXTBOX_POS_Y) - 1 + positions * 8);
		refresh_screen_size();

		ds_writew(0xc3d5, ds_writew(0xc3d1, ds_writew(0xc3d3, 0)));

		while (!done) {
			ds_writed(0x29e4, (Bit32u)RealMake(datseg, 0x29cc));
			handle_input();
			ds_writed(0x29e4, 0);

			if (l5 != ds_readw(0xe5b0)) {
				GUI_fill_radio_button(l5, ds_readw(0xe5b0),
					h_lines - 1);
				l5 = ds_readw(0xe5b0);
			}

			if (ds_readw(0xc3d3) != 0 ||
				ds_readw(ACTION) == 1 ||
				ds_readw(ACTION) == 0x51) {

				retval = -1;
				done = 1;
				ds_writew(0xc3d3, 0);
			}

			if (ds_readw(ACTION) == 0x1c) {
				retval = ds_readw(0xe5b0);
				done = 1;
			}

			/* Key UP */
			if (ds_readw(ACTION) == 0x48) {
				if (dec_ds_ws_post(0xe5b0) == 1)
					ds_writew(0xe5b0, positions);
			}
			/* Key DOWN */
			if (ds_readw(ACTION) == 0x50) {
				if (inc_ds_ws_post(0xe5b0) == positions)
					ds_writew(0xe5b0, 1);
			}

			if (ds_readw(0x299e) != l2) {
				l2 = ds_readw(0x299e);
				ds_writew(0xe5b0, ((l2 - l1) >> 3) + 1);
			}

			if (ds_readw(0xac0b) != 0) {
				if (ds_readw(ACTION) == 0x2c) {
					retval = done = 1;
				} else if (ds_readw(ACTION) == 0x31) {
					retval = 2;
					done = 1;
				}
			}
		}

		update_mouse_cursor();

		ds_writew(0x29a0, ds_writew(0x299c, l3));
		ds_writew(0x29a2, ds_writew(0x299e, l4));
		ds_writew(0x298e, 319);
		ds_writew(0x298a, 0);
		ds_writew(0x2988, 0);
		ds_writew(0x298c, 199);

		mouse_move_cursor(ds_readw(0x299c), ds_readw(0x299e));

	} else {
		do {
			delay_or_keypress(10000);
		} while (ds_readw(ACTION) == 0);

		retval = -1;
	}

	ds_writew(0xe5ac, ds_writew(0xe5ae, 0));

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

	l12 = ds_readw(0xc3cb);
	ds_writew(0xc3cb, 0);

	if (!options) {
		GUI_output(text);
		return 0;
	}


	ds_writeb(0x2c98, 1);
	l7 = ds_readw(0xd2d9);
	l8 = ds_readw(0xd2d7);
	l9 = ds_readw(0xd2d5);

	l11 = ds_readw(TEXTBOX_WIDTH) * 32 + 32;
	ds_writew(TEXTBOX_POS_X, ((320 - l11) >> 1) + ds_readw(0x2ca2));
	ds_writew(0xd2d9, ds_readw(TEXTBOX_POS_X) + 5);
	ds_writew(0xd2d5, l11 - 8);

	l10 = ds_readw(0xd313);
	ds_writew(0xd313, ds_readws(0xd2d9) + ds_readws(0xd2d5) - 24);

	l_di = GUI_count_lines(text);
	l5 = l_di + options;
	l6 = (l5 + 2) * 8;
	ds_writew(TEXTBOX_POS_Y, ((200 - l6 + 2) >> 1) + ds_readw(0x2ca4));
	ds_writew(0xd2d7, ds_readw(TEXTBOX_POS_Y) + 7);

	update_mouse_cursor();
	get_textcolor(&fg_bak, &bg_bak);

	GUI_draw_radio_bg(l_di, options, l11, l6);

	if (l_di != 0)
		GUI_print_header(text);

	l3 = ds_readw(0xd2d9) + 8;
	l4 = ds_readws(TEXTBOX_POS_Y) + (l_di + 1) * 8;

	va_start(arguments, options);
	for (i = 0; i < options; l4 += 8, i++) {

		/* highlight special option */
		if (ds_readw(GAME_MODE) == 1 && ds_readw(SKILLED_HERO_POS) == i)
			set_textcolor(0xc9, 0xdf);

		GUI_print_string((Bit8u*)va_arg(arguments, char*), l3, l4);

		/* reset highlight special option */
		if (ds_readw(GAME_MODE) == 1 && ds_readw(SKILLED_HERO_POS) == i)
			set_textcolor(0xff, 0xdf);
	}

	retval = GUI_menu_input(options, l_di + 1, l11);

	GUI_copy_smth(l11, l6);
	refresh_screen_size();
	set_textcolor(fg_bak, bg_bak);

	ds_writew(0xd2d9, l7);
	ds_writew(0xd2d7, l8);
	ds_writew(0xd2d5, l9);
	ds_writew(0xd313, l10);
	ds_writew(ACTION, ds_writebs(0x2c98, 0));
	ds_writew(0xc3cb, l12);

	return retval;
}

/**
 * \brief	shows a fight intro message
 * \param	fight_id	ID of the fight
 */
void GUI_print_fight_intro_msg(signed short fight_id)
{
	signed short bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	if (ds_readbs(DUNGEON_INDEX) == 2) {
		DNG2_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == 5) {
		DNG5_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == 6) {
		DNG6_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == 9) {
		DNG9_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == 11) {
		DNG11_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == 12) {
		DNG12_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == 13) {
		DNG13_fight_intro(fight_id);
	}
	if (ds_readbs(DUNGEON_INDEX) == 14) {
		DNG14_fight_intro(fight_id);
	}

	ds_writew(TEXTBOX_WIDTH, bak);
}

/**
 *	GUI_dialog_na() - print a Dialog windows without answers
 *	@head_index:	the number of a head, if another should be loaded
 *	@text:		the text
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
