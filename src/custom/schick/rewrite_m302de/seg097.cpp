/*
 *      Rewrite of DSA1 v3.02_de functions of seg097 (GUI)
 *      Functions rewritten 4/16
 *
 *      Functions called rewritten 4/13
 *      Functions uncalled rewritten 1/3
*/

#include <string.h>
#include <ctype.h>

#include "schick.h"

#include "seg000.h"
#include "seg001.h"
#include "seg002.h"
#include "seg004.h"
#include "seg096.h"

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
