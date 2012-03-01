#include "paging.h"

#include "v302de.h"

#include "schick.h"

#include "seg002.h"
#include "seg004.h"
#include "seg029.h"
#include "seg096.h"
#include "seg097.h"

namespace M302de {

//000
/**
	GUI_names_grammar - makes a grammatical wordgroup
	@flag:
	@index: index of the word of which a worgroup should be made
	@type: if index is true the index is an enemy, if not it is an item
*/
RealPt GUI_names_grammar(unsigned short flag, unsigned short index, unsigned short type) {
	Bit8u *lp1;
	unsigned short l2;
	RealPt p_name;
	short l4;
	short *lp5;

	l2 = 0;
	lp5 = (short*)(MemBase + PhysMake(datseg, 0xa9ed));

	if (type == 0) {
		/* string_array_itemnames */
		p_name = ds_readd(0xe22f) + index * 4;
		p_name = mem_readd(Real2Phys(p_name));

		flag += lp5[ds_readb(0x02ac + index)];

		lp1 = MemBase + PhysMake(datseg, 0x270);
		do {
			l4 = host_readw(lp1);
			lp1 += 2;
		} while (l4 != -1 && l4 != index);

		if (l4 == index) {
			flag += 4;
			flag &= 0x7fff;
			flag |= 0x4000;
			if (flag & 1)
				l2 = 1;
		}
	} else {
		p_name = ds_readd(0xe129) + index * 4;
		p_name = mem_readd(Real2Phys(p_name));
		flag += lp5[ds_readb(0x0925 + index)];
	}

	if (flag & 0x8000)
		lp1 = MemBase + PhysMake(datseg, 0xa953 + (flag & 0xf) * 6);
	else if (flag & 0x4000)
			lp1 = MemBase + PhysMake(datseg, 0xa9b3);
		else
			lp1 = MemBase + PhysMake(datseg, 0xa983 + (flag & 0xf) * 6);



	sprintf((char*)MemBase + PhysMake(datseg, 0xe50b + ds_readw(0xa9eb) *40),
		(l2 == 0) ? (char*)Real2Host(ds_readd(0xa9e3)) : (char*)Real2Host(ds_readd(0xa9e7)),
		(char*)Real2Host(ds_readd(0xa917 + (host_readw(lp1 + ((((flag & 0x3000) - 1) >> 12) << 1)) << 2))),
		(char*)Real2Host(GUI_name_plural(flag, Real2Host(p_name))));

	p_name = RealMake(datseg, ds_readw(0xa9eb) * 40 + 0xe50b);

	if (mem_readb(Real2Phys(p_name)) == 0x20){
		do {
			p_name++;
			l4 = mem_readb(Real2Phys(p_name));
			mem_writeb(Real2Phys(p_name) - 1, (signed char)l4);
		} while (l4 != 0);
	};

	l4 = ds_readw(0xa9eb);
	ds_writew(0xa9eb, ds_readw(0xa9eb) + 1);

	if (ds_readw(0xa9eb) == 4)
		ds_writew(0xa9eb, 0);

	D1_LOG("%s\n", (char*)MemBase + PhysMake(datseg, 0xe50b + l4 * 40));
	return RealMake(datseg, 0xe50b + l4 * 40);
}

//1a7
RealPt GUI_name_plural(unsigned short v1, Bit8u *s) {
	Bit8u *p = p_datseg + GRAMMAR_STRING;
	char tmp;

	while ((tmp = *s++) && (tmp != 0x2e))
		host_writeb(p++, tmp);

	if (v1 & 4)
		while ((tmp = *s++) && (tmp != 0x2e));

	while ((tmp = *s++) && (tmp != 0x2e))
		host_writeb(p++, tmp);

	if ((v1 & 0x0f) == 1 && (v1 & 0x3000) != 0x2000) {
		if (host_readb(p-1) == 'B' || host_readb(p-1) == 'D')
			host_writeb(p++, 'E');
		host_writeb(p++, 'S');
	} else {
		if (((v1 & 0x0f) == 7) && (host_readb(p-1) != 'N' || host_readb(p-1) != 'S'))
				host_writeb(p++, 'N');
	}

	host_writeb(p, 0);
	return RealMake(datseg, GRAMMAR_STRING);
}


//290
RealPt GUI_name_singular(Bit8u *s) {
	Bit8u *p = p_datseg + GRAMMAR_STRING;
	char tmp;

	while ((tmp = *s++) && (tmp != 0x2e))
		host_writeb(p++, tmp);

	while ((*s) && (*s != 0x2e))
		host_writeb(p++, *s++);

	host_writeb(p, 0);
	return RealMake(datseg, GRAMMAR_STRING);
}
//2f2
RealPt GUI_2f2(unsigned short v1, unsigned short v2, unsigned short v3) {
	short l, tmp;

	l = (v3 == 0) ? ds_readb(0x2ac + v2) : ds_readb(0x925);
	tmp = ds_readb(0xaa30 + v1 * 3 + l);

	return ds_readd(0xaa14 + tmp * 4);
}

//330
/**
 * GUI_get_ptr() - return a pointer to the personalpronomen
 * @genus:	gender of the hero
 * @causus:	the grammatical causus
*/
RealPt GUI_get_ptr(Bit16u genus, Bit16u causus) {

	if (genus == 0) {
		switch (causus) {
			case 0:
				return RealMake(datseg, 0xa9f3);
			case 1:
				return RealMake(datseg, 0xa9fd);
			case 3:
				return RealMake(datseg, 0xaa0a);
			default:
				return RealMake(datseg, 0xaa06);
		}
	} else {
		switch (causus) {
			case 0:
				return RealMake(datseg, 0xa9f6);
			case 1:
				return RealMake(datseg, 0xaa02);
			case 3:
				return RealMake(datseg, 0xaa02);
			default:
				return RealMake(datseg, 0xa9f6);
		}
	}
}

//394
/**
*/
RealPt GUI_get_ptr2(unsigned short v1, unsigned short v2) {

	if (v1 == 0) {
		switch (v2) {
			case 0:
				return RealMake(datseg, 0xa8d4);
			case 1:
				return RealMake(datseg, 0xa8e0);
			case 3:
				return RealMake(datseg, 0xa8e8);
			default:
				return RealMake(datseg, 0xa8e4);
		}
	} else {
		switch (v2) {
			case 0:
				return RealMake(datseg, 0xa8d8);
			case 1:
				return RealMake(datseg, 0xa8d4);
			case 3:
				return RealMake(datseg, 0xa8d4);
			default:
				return RealMake(datseg, 0xa8d4);
		}
	}
}
//3f8
void GUI_write_char_to_screen(PhysPt dst, unsigned short char_width, unsigned short char_height) {
	Bit8u *ptr;
	unsigned short y,x;

	ptr = MemBase + PhysMake(datseg, 0xce87);

	for (y = 0; y < char_width; ptr += 8 - char_height, dst += 320, y++) {
		for (x = 0; x < char_height; x++)
			mem_writeb_inline(dst+x, *ptr++);
	}
}
//442
/**
	GUI_count_lines - return the number of lines a string needs on a screen
*/
unsigned short GUI_count_lines(Bit8u *str) {
	Bit8u *str_loc;
	unsigned short v6, lines;
	short si, di;
	unsigned short max_line_width, width_char, width_line;

	lines = 0;

	if (str == NULL || str == MemBase)
		return 0;

	/* replace all CR and CL with Whitespaces */
	for (str_loc = str; *str_loc; str_loc++)
		if (*str_loc == 0x0a || *str_loc == 0x0d)
			*str_loc = 0x20;

	str_loc = str;
	si = di = v6 = 0;
	max_line_width = ds_readw(0xd2d5);

	if (ds_readw(0xe4db) != 0)
		ds_writew(0xd2d5, ds_readw(0xd2d5) - ds_readw(0xe4db));

	width_line = 0;

	for ( ; str_loc[si]; si++) {
		GUI_lookup_char_width(str_loc[si], &width_char);
		width_line += width_char;

		if (width_line >=  ds_readw(0xd2d5)) {
			if ( di != v6) {
				/* TODO: this caused a crash on
					no_way() in the city */
				str_loc[di] = 0x0d;
				str_loc += di;
			} else {
				str_loc[si] = 0x0d;
				str_loc += si + 1;
			}
			if (++lines == ds_readw(0xe4d9))
				ds_writew(0xd2d5, ds_readw(0xd2d5) + ds_readw(0xe4db));

			v6 = si = di = width_line = 0;
		}

		if (str_loc[si] == 0x20)
			di = si;

		if (str_loc[si] == 0x40) {
			str_loc += si + 1;
			si = -1;
			v6 = di = width_line = 0;
			if (++lines == ds_readw(0xe4d9))
				ds_writew(0xd2d5, ds_readw(0xd2d5) + ds_readw(0xe4db));
		}
	}

	if (width_line >= ds_readw(0xd2d5)) {

		if (v6 == di)
			str_loc[si - 1] = 0;
		else {
			str_loc[di] = 0x0d;
			if (++lines == ds_readw(0xe4d9))
				ds_writew(0xd2d5, ds_readw(0xd2d5) + ds_readw(0xe4db));
		}
	}

	ds_writew(0xd2d5, max_line_width);
	return ++lines;
}

//5d7
unsigned short GUI_print_header(Bit8u *str) {
	unsigned short retval = 1;

	update_mouse_cursor();
	retval = GUI_count_lines(str);
	GUI_print_string(str, ds_readw(0xd2d9), ds_readw(0xd2d7));
	refresh_screen_size();

	return retval;
}

//614
void GUI_print_loc_line(Bit8u * str) {
	unsigned short tmp1, tmp2;
	unsigned short l1, l2, l3;

	get_textcolor(&tmp1, &tmp2);
	set_textcolor(0xff, 0);

	l1 = ds_readw(0xd2d9);
	l2 = ds_readw(0xd2d7);
	l3 = ds_readw(0xd2d5);

	ds_writew(0xd2d9, 6);
	ds_writew(0xd2d7, 143);
	ds_writew(0xd2d5, 307);

	clear_loc_line();
	GUI_print_header(str);

	ds_writew(0xd2d9, l1);
	ds_writew(0xd2d7, l2);
	ds_writew(0xd2d5, l3);

	set_textcolor(tmp1, tmp2);
}

//691
void GUI_print_string(Bit8u *str, unsigned short x, unsigned short y) {
	unsigned short l1, l2, l3;
	unsigned char l4;
	unsigned short si, di;

	si = x;
	di = y;

	l1 = 0;
	l2 = 0;

	update_mouse_cursor();

	if (ds_readw(0xd2d1) == 1) {
		si = GUI_get_first_pos_centered(str, x, ds_readw(0xd2d5), 0);
	} else
		if (ds_readw(0xe4db))
			si += ds_readw(0xe4db);
	l3 = si;

	while (l4 = str[l2++]) {
		/* handle line breaks */
		if (l4 == 0x0d || l4 == 0x40) {
			if (++l1 == ds_readw(0xe4d9)) {
				ds_writew(0xd2d5, ds_readw(0xd2d5) + ds_readw(0xe4db));
				l3 -= ds_readw(0xe4db);
			}
			di += 7;
			if (ds_readw(0xd2d1) == 1)
				si = GUI_get_first_pos_centered(str + l2, ds_readw(0xd2d9), ds_readw(0xd2d5), 0);
			else
				si = l3;

			continue;
		}

		if (l4 == 0x7e) {
			if (si < ds_readw(0xd313))
				si = ds_readw(0xd313);
			else if (si < ds_readw(0xd315))
				si = ds_readw(0xd315);
			else if (si < ds_readw(0xd317))
				si = ds_readw(0xd317);
			else if (si < ds_readw(0xd319))
				si = ds_readw(0xd319);
			else if (si < ds_readw(0xd31b))
				si = ds_readw(0xd31b);
			else if (si < ds_readw(0xd31d))
				si = ds_readw(0xd31d);
			else if (si < ds_readw(0xd31f))
				si = ds_readw(0xd31f);
			continue;
		}

		if (l4 == 0xf0 || l4 == 0xf1 || l4 == 0xf2 || l4 == 0xf3) {
			ds_writew(0xd2c5, l4 - 0xf0);
			continue;
		}

		if (l4 == 0x3c)
			l4 = 0x3e;

		si += GUI_print_char(l4, si, di);
	}
	refresh_screen_size();
}

//7f0
unsigned short GUI_print_char(unsigned char c, unsigned short x, unsigned short y) {
	unsigned short char_width, font_index;

	ds_writeb(0xe4d8, c);
	font_index = GUI_lookup_char_width(c, &char_width);
	D1_LOG("GUI_lookup_char_width(%c); w=%d, fi=%d\n", c, char_width, font_index);
	GUI_write_fonti_to_screen(font_index, char_width, x, y);

	return char_width;
}
//82b
unsigned short GUI_lookup_char_width(unsigned char c, unsigned short *p){
	unsigned short i;

	for (i = 0; i != 75*3; i += 3) {
		if (c != ds_readb(0xaa51 + i))
			continue;

		host_writew((Bit8u*)p, ds_readb(0xaa51 + i + 2) & 0xff);
		return ds_readb(0xaa51 + i + 1) & 0xff;
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		host_writew((Bit8u*)p, 0);
		return 0;
	} else {
		host_writew((Bit8u*)p, 5);
		return 0;
	}
}
//88f
void GUI_write_fonti_to_screen(unsigned short font_index, unsigned short char_width, unsigned short x, unsigned short y) {
	PhysPt p_font6 = ds_readd(0xd2c1);

	D1_LOG("GUI_write_fonti_to_screen(fi=%d, cw=%d,x=%d, y=%d)\n", font_index, char_width, x, y);
	GUI_blank_char();
	GUI_font_to_buf(MemBase + Real2Phys(p_font6) + font_index * 8);
	GUI_write_char_to_screen_xy(x, y, 7, char_width);
}
//8c5
/**
	GUI_blank_char() - sets the area of a char to a color
*/
void GUI_blank_char() {
	PhysPt ptr = PhysMake(datseg, 0xce87);
	unsigned char color = ds_readb(0xd2c7);
	unsigned char i,j;

	D1_LOG("ptr = 0x%x color = 0x%x\n", ptr, color);

	for (i = 0; i < 8; ptr += 8, i++) {
		for (j = 0; j < 8; j++)
			mem_writeb_inline(ptr + j, color);
	}
}

//8f8
void GUI_font_to_buf(Bit8u *fc) {
	Bit8u * p;
	char c;
	short i, j;

	/* current text position */
	p = MemBase + PhysMake(datseg, 0xce87);

	if (ds_readb(0xe4d8) == 0x3a)
		fc++;

	for (i = 0; i < 8; p += 8, i++) {
		c = *fc++;
		for (j = 0; j < 8; j++)
			if ((0x80 >> j) & c)
				 p[j] = ds_readb(0xd2c9 + ds_readw(0xd2c5) * 2);
	}
}

//956
void GUI_write_char_to_screen_xy(unsigned short x, unsigned short y, unsigned short char_height, unsigned short char_width) {
	/* screen_start */
	PhysPt dst = Real2Phys(ds_readd(0xd2fb));

	dst += y * 320 + x;
	GUI_write_char_to_screen(dst, char_height, char_width);
}

/**
 * set_textcolor() - sets the textcolor
 * @fg:	foreground color index
 * @bg: background color index
 */
void set_textcolor(unsigned short fg, unsigned short bg) {
	ds_writew(TEXTCOLOR_FG, fg);
	ds_writew(TEXTCOLOR_BG, bg);

}

/**
 * get_textcolor() - gets the textcolor
 * @fg:	foreground color index
 * @bg: background color index
 *
 */
void get_textcolor(unsigned short *fg, unsigned short *bg) {
	host_writew((Bit8u*)fg, ds_readw(TEXTCOLOR_FG));
	host_writew((Bit8u*)bg, ds_readw(TEXTCOLOR_BG));
}

unsigned short GUI_unused(Bit8u *str) {
	unsigned short lines = 0;

	while (str) {
		if (*str++ == 0x0d)
			lines++;
	}

	return lines;
}
//9D6
unsigned short GUI_get_space_for_string(Bit8u *p, unsigned short dir) {
	unsigned short sum, tmp;

	for (sum = 0; *p; sum += tmp)
		if (dir) {
			GUI_lookup_char_height(*p++, &tmp);
			D1_INFO("%d\n", tmp); }
		else
			GUI_lookup_char_width(*p++, &tmp);

	return sum;
}

//A26
unsigned short GUI_get_first_pos_centered(Bit8u *p, unsigned short x, unsigned short v2, unsigned short dir) {
	unsigned short tmp, i;

	for (i = 0; *p && *p != 0x40 && *p != 0x0d; i += tmp) {
		if (dir)
			GUI_lookup_char_height(*p++, &tmp);
		else
			GUI_lookup_char_width(*p++, &tmp);
	}

	return (v2 - i) / 2 + x;
}

//A93
/**
	GUI_draw_popup_line - draws a line of a popup window
	@line: number of the current line
	@type: type of line 0 = top / 1 = middle normal / 2 = middle radio / 3 =bottom
*/
void GUI_draw_popup_line(unsigned short line, unsigned short type) {
	short i, popup_middle, popup_right, y;
	short x, popup_left;

	/* set the offsets in the popup.dat buffer */
	switch (type) {
		case 0:
			popup_left = 0;
			popup_middle = 0x380;
			popup_right = 0x80;
			break;
		case 1:
			popup_left = 0x100;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		case 2:
			popup_left = 0x200;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		case 3:
			popup_left = 0x280;
			popup_middle = 0x580;
			popup_right = 0x300;
			break;
	}

	x = ds_readw(0xbfff);
	y = (line * 8) + ds_readw(0xc001);
	ds_writew(0xc011, x);
	ds_writew(0xc013, y);
	ds_writew(0xc015, x + 15);
	ds_writew(0xc017, y + 7);
	ds_writed(0xc019, ds_readd(0xd2ad) + popup_left);

	do_pic_copy(0);

	ds_writed(0xc019, ds_readd(0xd2ad) + popup_middle);

	x += 16;

	for (i = 0; i < ds_readw(0xbffd); i++) {
		ds_writew(0xc011, x);
		ds_writew(0xc015, x + 31);
		do_pic_copy(0);
		x += 32;
	}

	ds_writed(0xc019, ds_readd(0xd2ad) + popup_right);
	ds_writew(0xc011, x);
	ds_writew(0xc015, x + 15);

	do_pic_copy(0);
}

}
