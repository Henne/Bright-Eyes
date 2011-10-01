/*
 *      Rewrite of DSA1 v3.02_de functions of seg097 (GUI)
 *      Functions rewritten 9/16
 *
 *      Functions called rewritten 8/13
 *      Functions uncalled rewritten 1/3
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "schick.h"

#include "seg000.h"
#include "seg001.h"
#include "seg002.h"
#include "seg004.h"
#include "seg096.h"
#include "seg097.h"

//000
//129
//15e
char GUI_lookup_char_height(char c, unsigned short *p) {
	short i;
	Bit8u *tab = MemBase + PhysMake(datseg, 0xab42);

	for (i = 0; i != 210; i+=3) {
		if (c != ds_readb(0xab42 + i))
			continue;

		host_writew((Bit8u*)p, ds_readb(0xab42 + i + 2) & 0xff);
		return ds_readb(0xab42 + i + 1) & 0xff;
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		host_writew((Bit8u*)p, 0);
		return 0;
	} else {
		host_writew((Bit8u*)p, 8);
		return 0;
	}
}

//1c2
void GUI_1c2(unsigned short v1, unsigned short v2, RealPt v3) {

	GUI_blank_char();
	GUI_font_to_buf(MemBase + Real2Phys(ds_readd(0xd2bd + v1 * 8)));
	GUI_write_char_to_screen(v3, 8, v2);
}

//1f8
//static
signed short GUI_enter_text(Bit8u* dst, unsigned short x, unsigned short y, unsigned short num, unsigned short zero) {
	Bit8u *dst_start;
	unsigned short si, di;
	signed short pos, c;
	unsigned short length;

	dst_start = dst;
	update_mouse_cursor();
	di = x;
	pos = 0;
	length = strlen((char*)dst);

	if (zero == 0) {
		for (si = 0; si < num; si++) {
			if (ds_readw(0x26b7) != 0 && length >= si) {
				GUI_print_char(0x20, di, y);
				GUI_print_char(*dst, di, y);
				dst++;
				pos++;
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
	while (c != 0xd || pos == 0) {
		/* This loop is evil */
		do {} while (CD_bioskey(1) == 0 && ds_readw(0xc3d1) == 0);

		if (ds_readw(0xc3d1) != 0) {
			ds_writew(0xc3d7, 0x0d);
			ds_writew(0xc3d5, 0x00);
			ds_writew(0xc3d1, 0x00);
		} else {
			ds_writew(0xc3d7, bioskey(0));
			ds_writew(0xc3d9, (ds_readw(0xc3d7) >> 8));
		}

		ds_writew(0xc3d7, ds_readw(0xc3d7) & 0xff);

		if (ds_readw(0xc3d9) == 0 && ds_readw(0xc3d7) == 0)
			continue;

		c = ds_readw(0xc3d7);

		if (c == 0x0d)
			continue;

		if (ds_readw(0xc3d9) == 1) {
			host_writeb(dst_start, 0);
			refresh_screen_size();
			ds_writew(0xc3d9, 0);
			return -1;
		}

		if (c == 8) {
			if (pos <= 0)
				continue;

			if (zero == 1 && pos != num)
				GUI_print_char(0x20, di, y);

			di -= 6;
			pos--;
			dst--;
			GUI_print_char(0x20, di, y);
			GUI_print_char(0x5f, di, y);
			continue;
		}

		/* check if character is valid */
		if (c < 0x20 && c > 0x7a &&
			c != 0x81 && c != 0x84 && c != 0x94)
				continue;
		/* is_alpha(c) */
		if (ds_readb(0xb4e9 + c) & 0xc)
			c = toupper(c);

		/* ae */
		if (c == 0x84)
			c = 0xff8e;
		/* oe */
		if (c == 0x94)
			c = 0xff99;
		/* ue */
		if (c == 0x81)
			c = 0xff9a;

		/* are we at the end of the input field ? */
		if (pos == num) {
			dst--;
			di -= 6;
			pos--;
		}

		host_writeb(dst++, c);
		GUI_print_char(0x20, di, y);
		GUI_print_char(c, di, y);
		di += 6;
		pos++;

		if (zero != 1)
			continue;
		if (pos == num)
			continue;
		GUI_print_char(0x20, di, y);
		GUI_print_char(0x5f, di, y);
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

//4ae
//static
void GUI_draw_radio_bg(Bit16u header, Bit16u options, Bit16u width,
								Bit16u height)
{
	Bit16u i;

	/* set upper left coordinates */
	ds_writew(0xc011, ds_readw(0xbfff));
	ds_writew(0xc013, ds_readw(0xc001));
	/* set lower righti coordinates */
	ds_writew(0xc015, ds_readw(0xbfff) + width - 1);
	ds_writew(0xc017, ds_readw(0xc001) + height - 1);
	/* set pointer */
	ds_writed(0xc019, ds_readd(0xbff9));
	do_save_rect();

	/* play FX3.VOC */
	if (ds_readw(0xbd25) == 0)
		play_voc(291);

	GUI_draw_popup_line(0, 0);

	for (i = 0; i < header; i++)
		GUI_draw_popup_line(i + 1, 1);

	if (options != 0)
		for (i = 0; i < options; i++)
			GUI_draw_popup_line(header + i + 1, 2);

	GUI_draw_popup_line(header + options + 1, 3);

	GUI_set_smth(0xff, 0xdf);
	wait_for_keyboard1();
}

//564
void GUI_copy_smth(unsigned short width, unsigned short height) {
	ds_writew(0xc011, ds_readw(0xbfff));
	ds_writew(0xc013, ds_readw(0xc001));
	ds_writew(0xc015, ds_readw(0xbfff) + width - 1);
	ds_writew(0xc017, ds_readw(0xc001) + height - 1);
	ds_writed(0xc019, ds_readd(0xbff9));
	do_pic_copy(0);
}

void GUI_output(Bit8u *str)
{
	GUI_input(str, 0);
}

signed short GUI_input(Bit8u *str, unsigned short num)
{
	Bit16s fg_bak, bg_bak;
	Bit16s retval, l2, l3, l4, l5, l6, l7;
	Bit16s l_si, l_di;

	retval = 0;

	l7 = ds_readw(0xc3cb);
	ds_writew(0xc3cb, 0);

	if (str == NULL || str == MemBase || *str == '\0' || ds_readw(0xe318) != 0)
		return -1;

	l6 = ds_readw(0xe113);
	ds_writew(0xe113, 0);
	ds_writeb(0x2c98, 1);
	ds_writew(0xd2d1, 1);

	l3 = ds_readw(0xd2d9);
	l4 = ds_readw(0xd2d7);
	l5 = ds_readw(0xd2d5);

	l_di = (ds_readw(0xbffd) * 32) + 32;
	ds_writew(0xbfff, (320 - l_di) / 2 + ds_readw(0x2ca2));

	ds_writew(0xd2d9, ds_readw(0xbfff) + 5);
	ds_writew(0xd2d5, l_di - 8);

	l_si = GUI_count_lines(str);

	if (num != 0)
		l_si += 2;

	l2 = (l_si + 2) * 8;

	ds_writew(0xc001, (200 - l2) / 2 + ds_readw(0x2ca4));
	ds_writew(0xd2d7, ds_readw(0xc001) + 7);

	GUI_get_smth(&fg_bak, &bg_bak);

	update_mouse_cursor();

	GUI_draw_radio_bg(l_si, 0, l_di, l2);

	GUI_print_header(str);

	ds_writew(0xc3d3, 0);

	refresh_screen_size();

	if (num != 0) {
		if (GUI_enter_text(Real2Host(ds_readd(0xd2ef)), (l_di - num * 6) / 2 + ds_readw(0xbfff), l_si * 8 + ds_readw(0xc001) - 2, num, 0) == -1) {
			retval = -1;
		} else {
			retval = atol((char*)Real2Host(ds_readd(0xd2ef)));
		}
	} else {
		/* set action table */
		ds_writed(0x29e4, RealMake(datseg, 0x29cc));

		if (ds_readw(0xc3c5) != 0) {
			wait_for_keypress();
		} else {
			delay_or_keypress(l_si * ds_readw(0xbffd) * 50);
		}

		/* delete action table */
		ds_writed(0x29e4, 0);
	}

	GUI_set_smth(fg_bak, bg_bak);

	update_mouse_cursor();

	GUI_copy_smth(l_di, l2);

	refresh_screen_size();

	ds_writew(0xd2d9, l3);
	ds_writew(0xd2d7, l4);
	ds_writew(0xd2d5, l5);

	ds_writew(0xc3d9, 0);
	ds_writeb(0x2c98, 0);

	ds_writew(0xe113, l6);
	ds_writew(0xd2d1, 0);
	ds_writew(0xc3cb, l7);

	return retval;
}

//0x7f4
///static
void GUI_fill_radio_button(signed short old_pos, unsigned short new_pos,
	unsigned short offset)
{
	unsigned short i, x, y;

	update_mouse_cursor();

	y = ds_readw(0xbfff) + 6;

	if (old_pos != -1) {

		x = (offset + old_pos) * 8 + ds_readw(0xc001) + 2;

		/* clear the old button */
		for (i = 0; i < 4; i++)
			do_v_line(Real2Phys(ds_readd(0xd2ff)), y + i, x, x + 3, 0xd8);
	}

	x = (offset + new_pos) * 8 + ds_readw(0xc001) + 2;

	/* fill the new button */
	for (i = 0; i < 4; i++)
		do_v_line(Real2Phys(ds_readd(0xd2ff)), y + i, x, x + 3, 0xd9);

	refresh_screen_size();
}

//0xb43
//static
signed short GUI_menu_input(unsigned short positions, unsigned short h_lines,
			unsigned short width)
{
	Bit16s l1, l2, l3, l4, l5, l6;
	Bit16s retval;
	bool done;

	l5 = -1;
	done = false;

	ds_writew(0xe5ae, 1);
	ds_writew(0xe5b0, 1);
	ds_writew(0xe5b0, ds_readw(0xe5b0) + ds_readw(0xe5ac));

	if (positions != 0) {
		l6 = h_lines * 8;
		l3 = ds_readw(0x299c);
		l4 = ds_readw(0x299e);
		ds_writew(0x299c, ds_readw(0xbfff) + 90);
		ds_writew(0x29a0, ds_readw(0xbfff) + 90);
		l1 = ds_readw(0xc001) + l6;
		l2 = ds_readw(0xe5ac) * 8 + l1;
		ds_writew(0x299e, l2);
		ds_writew(0x29a2, l2);

		mouse_move_cursor(ds_readw(0x299c), ds_readw(0x299e));

		ds_writew(0x298e, ds_readw(0xbfff) + width - 16);
		ds_writew(0x298a, ds_readw(0xbfff));
		ds_writew(0x2988, ds_readw(0xc001) + l6);
		ds_writew(0x298c, positions * 8 + l6 + ds_readw(0xc001) - 1);
		refresh_screen_size();

		ds_writew(0xc3d3, 0);
		ds_writew(0xc3d1, 0);
		ds_writew(0xc3d5, 0);

		while (!done) {
			ds_writed(0x29e4, RealMake(datseg, 0x29cc));
			handle_input();
			ds_writed(0x29e4, 0);

			if (l5 != ds_readw(0xe5b0)) {
				GUI_fill_radio_button(l5, ds_readw(0xe5b0),
					h_lines - 1);
				l5 = ds_readw(0xe5b0);
			}

			if (ds_readw(0xc3d3) != 0 ||
				ds_readw(0xc3d9) == 1 ||
				ds_readw(0xc3d9) == 0x51) {

				retval = -1;
				done = true;
				ds_writew(0xc3d3, 0);
			}

			if (ds_readw(0xc3d9) == 0x1c) {
				retval = ds_readw(0xe5b0);
				done = true;
			}

			/* Key UP */
			if (ds_readw(0xc3d9) == 0x48) {
				ds_writew(0xe5b0, ds_readw(0xe5b0) - 1);
				if (ds_readw(0xe5b0) + 1 == 1)
					ds_writew(0xe5b0, positions);
			}
			/* Key DOWN */
			if (ds_readw(0xc3d9) == 0x50) {
				ds_writew(0xe5b0, ds_readw(0xe5b0) + 1);
				if (ds_readw(0xe5b0) - 1 == positions)
					ds_writew(0xe5b0, 1);
			}

			if (ds_readw(0x299e) != l2) {
				l2 = ds_readw(0x299e);
				ds_writew(0xe5b0, (l2 - l1) / 8 + 1);
			}

			if (ds_readw(0xac0b) != 0) {
				if (ds_readw(0xc3d9) == 0x2c) {
					retval = 1;
					done = true;
				} else if (ds_readw(0xc3d9) == 0x31) {
					retval = 2;
					done = true;
				}
			}
		}

		update_mouse_cursor();

		ds_writew(0x299c, l3);
		ds_writew(0x29a0, l3);
		ds_writew(0x299e, l4);
		ds_writew(0x29a2, l4);
		ds_writew(0x298e, 319);
		ds_writew(0x298a, 0);
		ds_writew(0x2988, 0);
		ds_writew(0x298c, 199);

		mouse_move_cursor(ds_readw(0x299c), ds_readw(0x299e));

	} else {
		delay_or_keypress(10000);
		if (ds_readw(0xc3d9) != 0)
			retval = -1;
	}

	ds_writew(0xe5ae, 0);
	ds_writew(0xe5ac, 0);

	return retval;
}
