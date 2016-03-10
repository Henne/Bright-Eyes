/*
 *	Rewrite of DSA1 v3.02_de functions of seg024 (diary)
 *	Functions rewritten: 3/3 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg024.cpp
*/

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg024.h"
#include "seg027.h"
#include "seg096.h"

#if !defined (__BORLANDC__)
namespace M302de {
#endif

#if 0
/* That comes in a later phase of development */

/* DIARY_STRING1-7 */
static const char diary_fmt[][30] = {
	/* V3.02de */
	"%2d.~%-8s~%s.",
	"%2d.~%-8s~IN %s.",
	"%2d.~%-8s~BESUCHEN VON %s.",
	"~~%s.",
	"~~IN %s.",
	"~~ABENTEUER IN %s.",
	"~~RUMH\x8eNGEN IN %s."
};
#endif

void diary_show(void)
{
	signed short fg_bak;
	signed short bg_bak;
	Bit16u bak1, bak2, bak3, bak4, bak5;
	signed short i;

	bak5 = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);

	ds_writeb(0x45b8, 1);
	ds_writew(WALLCLOCK_UPDATE, 0);
	ds_writew(0x2ccb, 0xffff);
	ds_writed(0xcecb, (Bit32u)RealMake(datseg, DEFAULT_MOUSE_CURSOR));

	load_pp20(ARCHIVE_FILE_BUCH_DAT);
	ds_writeb(0x2845, ARCHIVE_FILE_BUCH_DAT);

	get_textcolor(&fg_bak, &bg_bak);

	ds_writed(0xd2fb, ds_readd(BUFFER9_PTR));
	bak1 = ds_readw(0xd2d5);
	bak2 = ds_readw(0xd2d9);
	bak3 = ds_readw(TXT_TABPOS1);
	bak4 = ds_readw(TXT_TABPOS2);
	ds_writew(0xd2d5, 200);
	ds_writew(0xd2d9, 65);
	ds_writew(TXT_TABPOS1, 83);
	ds_writew(TXT_TABPOS2, 130);

	set_textcolor(4, 0);

	/* print all diary entries */
	i = 0;
	do {
		i = diary_print_entry(i);
	} while (i < ds_readws(DIARY_ENTRY_COUNTER));

	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 199);
	ds_writed(0xc019, ds_readd(BUFFER1_PTR));
	ds_writed(0xc00d, ds_readd(0xd2ff));

	update_mouse_cursor();

	set_palette(Real2Host(ds_readd(BUFFER1_PTR)) + 0xfa02, 0, 0x20);

	do_pic_copy(0);

	refresh_screen_size();

	set_textcolor(fg_bak, bg_bak);

	ds_writed(0xc00d, ds_writed(0xd2fb, ds_readd(0xd2ff)));

	ds_writew(0xd2d9, bak2);
	ds_writew(0xd2d5, bak1);
	ds_writew(TXT_TABPOS1, bak3);
	ds_writew(TXT_TABPOS2, bak4);
	ds_writew(TEXTBOX_WIDTH, bak5);

	delay_or_keypress(5000);
}

void diary_new_entry(void)
{
	Bit8u *ptr;

	/* move all entries if the list is full */
	if (ds_readw(DIARY_ENTRY_COUNTER) == 23) {
		memcpy(p_datseg + (DIARY_ENTRIES+8), p_datseg + (DIARY_ENTRIES+16), (22*8));
		dec_ds_ws(DIARY_ENTRY_COUNTER);
	}

	/* make a pointer to the last entry */
	ptr = p_datseg + DIARY_ENTRIES + ds_readw(DIARY_ENTRY_COUNTER) * 8;

	/* avoid double entries for the same town */
	if (ds_readbs(CURRENT_TOWN) != host_readw(ptr + 6)) {

		/* make a pointer to the current entry */
		ptr = p_datseg + (DIARY_ENTRIES+8) + ds_readw(DIARY_ENTRY_COUNTER) * 8;

		/* deccrement entry counter */
		inc_ds_ws(DIARY_ENTRY_COUNTER);

		/* Write day of month */
		host_writew(ptr, ds_readbs(DAY_OF_MONTH));
		/* Write month */
		host_writew(ptr + 2, ds_readbs(MONTH));
		/* Write year */
		host_writew(ptr + 4, ds_readbs(YEAR));
		/* Write city */
		host_writew(ptr + 6, ds_readbs(CURRENT_TOWN));
	}
}

/**
 *	diary_print_entry() - print a diary entry
 *	@line:	the line to start the entry
 */
Bit16u diary_print_entry(Bit16u line)
{
	signed short day;
	signed short month;
	signed short startline;
	Bit8u *ptr;
	char *city_name;
	signed short di = 0;

	memset(Real2Host(ds_readd(BUFFER9_PTR)), 0, 64000);

	ptr = p_datseg + (DIARY_ENTRIES+8) + line * 8;

	startline = line;

	do {
		day = host_readw(ptr);
		month = host_readw(ptr + 2);
		city_name = (char*)get_ltx((host_readw(ptr + 6) + 0xeb) * 4);

		if (di == 0) {
			if ((signed short)strlen(city_name) > 24) {
				sprintf(getString(ds_readd(DTP2)),
					(char*)(p_datseg + DIARY_STRING1),
					host_readw(ptr),
					(char*)get_ltx((host_readw(ptr + 2) + 0x15) * 4),
					city_name);
			} else if ((signed short)strlen(city_name) > 15 ) {
				sprintf(getString(ds_readd(DTP2)),
					(char*)(p_datseg + DIARY_STRING2),
					host_readw(ptr),
					(char*)get_ltx((host_readw(ptr + 2) + 0x15) * 4),
					city_name);
			} else {
				sprintf(getString(ds_readd(DTP2)),
					(char*)(p_datseg + DIARY_STRING3),
					host_readw(ptr),
					(char*)get_ltx((host_readw(ptr + 2) + 0x15) * 4),
					city_name);
			}
		} else {
			if ((signed short)strlen(city_name) > 24) {
				sprintf(getString(ds_readd(DTP2)),
					(char*)(p_datseg + DIARY_STRING4),
					city_name);
			} else if ((signed short)strlen(city_name) > 15) {
				sprintf(getString(ds_readd(DTP2)),
					(char*)(p_datseg + DIARY_STRING5),
					city_name);
			} else if ((signed short)strlen(city_name) > 6) {
				sprintf(getString(ds_readd(DTP2)),
					(char*)(p_datseg + DIARY_STRING6),
					city_name);
			} else {
				sprintf(getString(ds_readd(DTP2)),
					(char*)(p_datseg + DIARY_STRING7),
					city_name);
			}
		}
		GUI_print_string(Real2Host(ds_readd(DTP2)),
			65, (line - startline) * 7);
		ptr += 8;
		line++;
		di++;

	} while ((host_readws(ptr) == day) && (host_readws(ptr + 2) == month));

	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, line * 7);
	ds_writed(0xc019, ds_readd(BUFFER9_PTR));
#if !defined(__BORLANDC__)
	ds_writed(0xc00d,
		(Bit32u)(((RealPt)ds_readd(BUFFER1_PTR) + startline * 2240) + 9600));
#else
	/* TODO: ugly hack */
	/*	this calculation of the address of
		a twodimiensional array is done
		here with inline assembly */
	calc_twodim_array_ptr(BUFFER1_PTR, 0x8c0, startline, 9600, 0xc00d);
#endif
	do_pic_copy(2);

	return line;

}

#if !defined (__BORLANDC__)
}
#endif
