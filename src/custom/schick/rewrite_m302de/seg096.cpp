/*
 *	Rewrite of DSA1 v3.02_de functions of seg096 (text output)
 *	Functions rewritten: 23/23 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg096.cpp
 *
*/
#include <stdio.h>
#include <string.h>

#if !defined(__BORLANDC__)
#include "paging.h"
#endif

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg029.h"
#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif


struct dummy {
	signed short a[3];
};

struct dummy2 {
	char a[40];
};

//000
/**
	GUI_names_grammar - makes a grammatical wordgroup
	@flag:
	@index: index of the word of which a worgroup should be made
	@type: if index is true the index is an enemy, if not it is an item
*/
RealPt GUI_names_grammar(signed short flag, signed short index, signed short type)
{
	signed short *lp1;
	signed short l2 = 0;
	Bit8u *p_name;
	signed short l4;
#if !defined(__BORLANDC__)
	struct dummy lp5 = { {0x1000, 0x2000, 0x3000} };
#else
	struct dummy lp5 = *(struct dummy*)(p_datseg + 0xa9ed);
#endif

	if (type == 0) {
		/* string_array_itemnames */
		p_name = (Bit8u*)get_itemname(index);

		flag += lp5.a[ds_readbs(0x02ac + index)];

		lp1 = (signed short*)(p_datseg + 0x270);

		while (((l4 = host_readws((Bit8u*)(lp1++))) != -1) && (l4 != index));

		if (l4 == index) {
			flag += 4;
			flag &= 0x7fff;
			flag |= 0x4000;
			if (flag & 1)
				l2 = 1;
		}
	} else {
		p_name = get_monname(index);
		flag += lp5.a[ds_readbs(0x0925 + index)];
	}

	lp1 = (flag & 0x8000) ? (signed short*)(p_datseg + 0xa953 + (flag & 0xf) * 6) :
			((flag & 0x4000) ? (signed short*)(p_datseg + 0xa9b3) :
				(signed short*)(p_datseg + 0xa983 + (flag & 0xf) * 6));


	sprintf((char*)p_datseg + 0xe50b + ds_readw(0xa9eb) * 40,
		(l2 == 0) ? (char*)Real2Host(ds_readd(0xa9e3)) : (char*)Real2Host(ds_readd(0xa9e7)),
		(char*)Real2Host(ds_readd(0xa917 + 4 * host_readws((Bit8u*)lp1 + 2 * (((flag & 0x3000) - 1) >> 12)))),
		(char*)Real2Host(GUI_name_plural(flag, p_name)));

	p_name = p_datseg + ds_readw(0xa9eb) * 40 + 0xe50b;

	if (host_readb(p_name) == 0x20) {
		do {
			l4 = host_readbs(++p_name);
			host_writeb(p_name - 1, (signed char)l4);
		} while (l4 != 0);
	}

	l4 = ds_readw(0xa9eb);

	if (inc_ds_ws(0xa9eb) == 4)
		ds_writew(0xa9eb, 0);

#if !defined(__BORLANDC__)
	return (RealPt)RealMake(datseg, 0xe50b + (l4 * 40));
#else
	/* TODO: Sorry dear ! */
	return (RealPt) (&((struct dummy2*)(p_datseg + 0xe50b))[l4]);
#endif

}

//1a7
RealPt GUI_name_plural(signed short v1, Bit8u *s)
{
	Bit8u *p = p_datseg + GRAMMAR_STRING;
	char tmp;

	while ((tmp = *s++) && (tmp != 0x2e))
		host_writeb(p++, tmp);

	if (v1 & 4)
		while ((tmp = *s++) && (tmp != 0x2e));

	while ((tmp = *s) && (tmp != 0x2e))
		host_writeb(p++, *s++);

	if ((v1 & 0x0f) == 1 && (v1 & 0x3000) != 0x2000) {
		if (host_readb(p-1) == 'B' || host_readb(p-1) == 'D')
			host_writeb(p++, 'E');
		host_writeb(p++, 'S');
	} else {
		if (((v1 & 0x0f) == 7) && (host_readb(p-1) != 'N') && (host_readb(p-1) != 'S'))
				host_writeb(p++, 'N');
	}

	host_writeb(p, 0);
	return (RealPt)RealMake(datseg, GRAMMAR_STRING);
}

//290
RealPt GUI_name_singular(Bit8u *s)
{
	Bit8u *p = p_datseg + GRAMMAR_STRING;
	char tmp;

	while ((tmp = *s++) && (tmp != 0x2e))
		host_writeb(p++, tmp);

	while ((tmp = *s) && (tmp != 0x2e))
		host_writeb(p++, *s++);

	host_writeb(p, 0);
	return (RealPt)RealMake(datseg, GRAMMAR_STRING);
}

//2f2
RealPt GUI_2f2(signed short v1, signed short v2, signed short v3)
{
	signed short l;

	l = (v3 == 0) ? ds_readbs(0x2ac + v2) : ds_readbs(v2 + 0x925);

	return (RealPt)ds_readd(0xaa14 + 4 * ds_readbs(0xaa30 + v1 * 3 + l));
}

//330
/**
 * GUI_get_ptr() - return a pointer to the personalpronomen
 * @genus:	gender of the hero
 * @causus:	the grammatical causus
*/
RealPt GUI_get_ptr(signed short genus, signed short causus)
{
	if (genus == 0) {
		return (causus == 0) ? (RealPt)RealMake(datseg, 0xa9f3) :
				((causus == 1) ? (RealPt)RealMake(datseg, 0xa9fd) :
				((causus == 3) ? (RealPt)RealMake(datseg, 0xaa0a) :
					(RealPt)RealMake(datseg, 0xaa06)));
	} else {
		return (causus == 0) ? (RealPt)RealMake(datseg, 0xa9f6) :
				((causus == 1) ? (RealPt)RealMake(datseg, 0xaa02) :
				((causus == 3) ? (RealPt)RealMake(datseg, 0xaa02) :
					(RealPt)RealMake(datseg, 0xa9f6)));
	}
}

//394
/**
*/
RealPt GUI_get_ptr2(signed short genus, signed short causus)
{
	if (genus == 0) {
		return (causus == 0) ? (RealPt)RealMake(datseg, 0xa8d4) :
				((causus == 1) ? (RealPt)RealMake(datseg, 0xa8e0) :
				((causus == 3) ? (RealPt)RealMake(datseg, 0xa8e8) :
					(RealPt)RealMake(datseg, 0xa8e4)));
	} else {
		return (causus == 0) ? (RealPt)RealMake(datseg, 0xa8d8) :
				((causus == 1) ? (RealPt)RealMake(datseg, 0xa8d4) :
				((causus == 3) ? (RealPt)RealMake(datseg, 0xa8d4) :
					(RealPt)RealMake(datseg, 0xa8d8)));
	}
}

//3f8
void GUI_write_char_to_screen(RealPt dst, signed short char_width, signed short char_height)
{
	Bit8u *ptr;
	signed short y;
	signed short x;

	ptr = p_datseg + GUI_TEXT_BUFFER;

	for (y = 0; y < char_width; ptr += 8 - char_height, dst += 320, y++) {
		for (x = 0; x < char_height; x++)
			mem_writeb(Real2Phys(dst) + x, *ptr++);
	}
}

//442
/**
	GUI_count_lines - return the number of lines a string needs on a screen
*/
unsigned short GUI_count_lines(Bit8u *str)
{
	signed short si;
	signed short di;

	Bit8u *str_loc;
	signed short v6;
	signed short lines;
	signed short max_line_width;
	signed short width_char;
	signed short width_line;

	lines = 0;

	if (!NOT_NULL(str))
		return 0;

	/* replace all CR and CL with Whitespaces */
	for (str_loc = str; *str_loc; str_loc++)
		if (*str_loc == 0x0d || *str_loc == 0x0a)
			*str_loc = 0x20;

	str_loc = str;
	si = di = v6 = 0;
	max_line_width = ds_readw(0xd2d5);

	if (ds_readw(DIALOGBOX_INDENT_WIDTH) != 0)
		sub_ds_ws(0xd2d5, ds_readws(DIALOGBOX_INDENT_WIDTH));

	width_line = 0;

	for ( ; str_loc[si]; si++) {

		GUI_lookup_char_width(str_loc[si], &width_char);
#if !defined(__BORLANDC__)
		/* BE-fix */
		width_char = host_readws((Bit8u*)&width_char);
#endif
		width_line += width_char;

		if (width_line >=  ds_readws(0xd2d5)) {
			if (di != v6) {
				/* TODO: this caused a crash on
					no_way() in the city */
				str_loc[di] = 0x0d;
				str_loc = str_loc + di;
			} else {
				str_loc[si] = 0x0d;
				str_loc = &str_loc[si + 1];
			}

			if (++lines == ds_readw(DIALOGBOX_INDENT_HEIGHT))
				add_ds_ws(0xd2d5, ds_readws(DIALOGBOX_INDENT_WIDTH));

			v6 = si = di = width_line = 0;
		}

		if (str_loc[si] == 0x20)
			di = si;

		if (str_loc[si] == 0x40) {
			str_loc = &str_loc[si + 1];
			si = -1;
			v6 = di = width_line = 0;
			if (++lines == ds_readw(DIALOGBOX_INDENT_HEIGHT))
				add_ds_ws(0xd2d5, ds_readws(DIALOGBOX_INDENT_WIDTH));
		}
	}

	if (width_line >= ds_readws(0xd2d5)) {

		if (v6 == di)
			str_loc[si - 1] = 0;
		else {
			str_loc[di] = 0x0d;
			if (++lines == ds_readw(DIALOGBOX_INDENT_HEIGHT))
				add_ds_ws(0xd2d5, ds_readws(DIALOGBOX_INDENT_WIDTH));
		}
	}

	ds_writew(0xd2d5, max_line_width);
	return ++lines;
}

//5d7
signed short GUI_print_header(Bit8u *str)
{
	signed short retval = 1;

	update_mouse_cursor();
	retval = GUI_count_lines(str);
	GUI_print_string(str, ds_readws(0xd2d9), ds_readws(0xd2d7));
	refresh_screen_size();

	return retval;
}

//614
void GUI_print_loc_line(Bit8u * str)
{
	signed short tmp1;
	signed short tmp2;
	signed short l1;
	signed short l2;
	signed short l3;

	get_textcolor(&tmp1, &tmp2);
	set_textcolor(0xff, 0);

	l1 = ds_readws(0xd2d9);
	l2 = ds_readws(0xd2d7);
	l3 = ds_readws(0xd2d5);

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
void GUI_print_string(Bit8u *str, signed short x, signed short y)
{
	signed short l1;
	signed short l2;
	signed short l3;
	unsigned char l4;

	l1 = 0;
	l2 = 0;

	update_mouse_cursor();

	if (ds_readws(0xd2d1) == 1) {
		x = GUI_get_first_pos_centered(str, x, ds_readws(0xd2d5), 0);
	} else
		if (ds_readws(DIALOGBOX_INDENT_WIDTH))
			x += ds_readws(DIALOGBOX_INDENT_WIDTH);
	l3 = x;

	while ((l4 = str[l2++])) {

		/* handle line breaks */
		if ((l4 == 0x0d) || (l4 == 0x40)) {

			if (++l1 == ds_readws(DIALOGBOX_INDENT_HEIGHT)) {
				add_ds_ws(0xd2d5, ds_readws(DIALOGBOX_INDENT_WIDTH));
				l3 -= ds_readws(DIALOGBOX_INDENT_WIDTH);
			}

			y += 7;
			x = (ds_readw(0xd2d1) == 1) ?
				GUI_get_first_pos_centered(str + l2, ds_readws(0xd2d9), ds_readws(0xd2d5), 0) : l3;

		} else	if (l4 == '~') {

			if (x < ds_readws(TXT_TABPOS1))
				x = ds_readws(TXT_TABPOS1);
			else if (x < ds_readws(TXT_TABPOS2))
				x = ds_readws(TXT_TABPOS2);
			else if (x < ds_readws(TXT_TABPOS3))
				x = ds_readws(TXT_TABPOS3);
			else if (x < ds_readws(TXT_TABPOS4))
				x = ds_readws(TXT_TABPOS4);
			else if (x < ds_readws(TXT_TABPOS5))
				x = ds_readws(TXT_TABPOS5);
			else if (x < ds_readws(TXT_TABPOS6))
				x = ds_readws(TXT_TABPOS6);
			else if (x < ds_readws(TXT_TABPOS7))
				x = ds_readws(TXT_TABPOS7);

		} else if (l4 == (unsigned char)0xf0 ||
				l4 == (unsigned char)0xf1 ||
				l4 == (unsigned char)0xf2 ||
				l4 == (unsigned char)0xf3)
		{
			/* changes of the text color are only control bytes */
			ds_writew(TEXTCOLOR, l4 - 0xf0);
		} else	{

			if (l4 == 0x3c) {
				l4 = 0x3e;
			} else { }

			x += GUI_print_char(l4, x, y);
		}
	}

	refresh_screen_size();
}

//7f0
signed short GUI_print_char(unsigned char c, unsigned short x, unsigned short y)
{
	signed short char_width;
	signed short font_index;

	ds_writeb(0xe4d8, c);
	font_index = GUI_lookup_char_width(c, &char_width);

#if !defined(__BORLANDC__)
	/* BE-fix */
	char_width = host_readws((Bit8u*)&char_width);
	D1_LOG("GUI_lookup_char_width(%c); w=%d, fi=%d\n",
		c, char_width, font_index);
#endif

	GUI_write_fonti_to_screen(font_index, char_width, x, y);

	return char_width;
}

//82b
signed short GUI_lookup_char_width(signed char c, signed short *p)
{
	signed short i;

	for (i = 0; i != 75 * 3; i += 3) {

		if (c == ds_readbs(0xaa51 + i)) {

			host_writew((Bit8u*)p,
				ds_readbs(0xaa51 + i + 2) & 0xff);

			return ds_readbs(0xaa51 + i + 1) & 0xff;
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

	host_writew((Bit8u*)p, 5);
	return 0;
}

//88f
void GUI_write_fonti_to_screen(unsigned short font_index, unsigned short char_width, unsigned short x, unsigned short y)
{
	GUI_blank_char();
	GUI_font_to_buf(Real2Host(ds_readd(BUF_FONT6)) + font_index * 8);
	GUI_write_char_to_screen_xy(x, y, 7, char_width);
}
//8c5
/**
	GUI_blank_char() - sets the area of a char to a color
*/
void GUI_blank_char(void)
{
	PhysPt ptr = (PhysPt)PhysMake(datseg, GUI_TEXT_BUFFER);
	signed short i;
	signed short j;

	for (i = 0; i < 8; ptr += 8, i++) {
		for (j = 0; j < 8; j++)
			mem_writeb(ptr + j, ds_readbs(TEXTCOLOR_BG));
	}
}

//8f8
void GUI_font_to_buf(Bit8u *fc)
{
	Bit8u * p;
	char c;
	short i;
	short j;

	/* current text position */
	p = p_datseg + GUI_TEXT_BUFFER;

	if (ds_readb(0xe4d8) == 0x3a)
		fc++;

	for (i = 0; i < 8; p += 8, i++) {
		c = *fc++;
		for (j = 0; j < 8; j++)
			if ((0x80 >> j) & c)
				 p[j] = ds_readb(TEXTCOLOR_FG + ds_readw(TEXTCOLOR) * 2);
	}
}

//956
void GUI_write_char_to_screen_xy(unsigned short x, unsigned short y, unsigned short char_height, unsigned short char_width)
{
	/* screen_start */
	RealPt dst = ((RealPt)ds_readd(0xd2fb)) + y * 320 + x;

	GUI_write_char_to_screen(dst, char_height, char_width);
}

/**
 * set_textcolor() - sets the textcolor
 * @fg:	foreground color index
 * @bg: background color index
 */
void set_textcolor(signed short fg, signed short bg)
{
	ds_writew(TEXTCOLOR_FG, fg);
	ds_writew(TEXTCOLOR_BG, bg);
}

/**
 * get_textcolor() - gets the textcolor
 * @fg:	foreground color index
 * @bg: background color index
 *
 */
void get_textcolor(signed short *fg, signed short *bg)
{
	host_writew((Bit8u*)fg, ds_readw(TEXTCOLOR_FG));
	host_writew((Bit8u*)bg, ds_readw(TEXTCOLOR_BG));
}

unsigned short GUI_unused(Bit8u *str)
{
	unsigned short lines = 0;

	while (*str) {
		if (*str++ == 0x0d)
			lines++;
	}

	return lines;
}

//9D6
signed short GUI_get_space_for_string(Bit8u *p, signed short dir)
{
	signed short sum;
	signed short tmp;

	for (sum = 0; *p; sum += tmp) {
		if (dir) {
			GUI_lookup_char_height(*p++, &tmp);
		} else {
			GUI_lookup_char_width(*p++, &tmp);
		}
#if !defined(__BORLANDC__)
		/* BE-fix */
		tmp = host_readws((Bit8u*)&tmp);
#endif
	}

	return sum;
}

//A26
signed short GUI_get_first_pos_centered(Bit8u *p, signed short x, signed short v2, unsigned short dir)
{
	register signed short i;
	register signed short c;
	signed short tmp;

	for (i = 0;  (c = host_readbs(p)) && (c != 0x40) && (c != 0x0d); i += tmp) {
		if (dir)
			GUI_lookup_char_height(*p++, &tmp);
		else
			GUI_lookup_char_width(*p++, &tmp);

#if !defined(__BORLANDC__)
		/* BE-fix */
		tmp = host_readws((Bit8u*)&tmp);
#endif
	}

	v2 -= i;
	v2 >>= 1;
	v2 += x;

	return v2;
}

//A93
/**
	GUI_draw_popup_line - draws a line of a popup window
	@line: number of the current line
	@type: type of line 0 = top / 1 = middle normal / 2 = middle radio / 3 =bottom
*/
void GUI_draw_popup_line(signed short line, signed short type)
{
	short popup_left;
	short i;
	short popup_middle;
	short popup_right;
	short y;
	short x;

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

	x = ds_readws(TEXTBOX_POS_X);
	y = ds_readws(TEXTBOX_POS_Y) + (line * 8);
	ds_writew(0xc011, x);
	ds_writew(0xc013, y);
	ds_writew(0xc015, x + 15);
	ds_writew(0xc017, y + 7);
	ds_writed(0xc019, (Bit32u)((RealPt)ds_readd(POPUP) + popup_left));

	do_pic_copy(0);

	ds_writed(0xc019, (Bit32u)((RealPt)ds_readd(POPUP) + popup_middle));

	x += 16;

	for (i = 0; i < ds_readws(TEXTBOX_WIDTH); i++) {
		ds_writew(0xc011, x);
		ds_writew(0xc015, x + 31);
		do_pic_copy(0);
		x += 32;
	}

	ds_writed(0xc019, (Bit32u)((RealPt)ds_readd(POPUP) + popup_right));
	ds_writew(0xc011, x);
	ds_writew(0xc015, x + 15);

	do_pic_copy(0);
}

#if !defined(__BORLANDC__)
}
#endif
