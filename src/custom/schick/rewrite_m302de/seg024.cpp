/*
	Rewrite of DSA1 v3.02_de functions of seg024 (diary)
	Functions rewritten: 3/3
*/

#include "schick.h"
#include "string.h"

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg024.h"
#include "seg027.h"
#include "seg096.h"

namespace M302de {

/* DS:0x4900 - DS:0x496f */
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

void diary_show()
{
	Bit16u bak1, bak2, bak3, bak4, bak5;
	unsigned short fg_bak, bg_bak;
	Bit16u i;

	bak5 = ds_readw(0xbffd);
	ds_writew(0xbffd, 3);

	ds_writeb(0x45b8, 1);
	ds_writew(0xe113, 0);
	ds_writew(0x2ccb, 0xffff);
	ds_writed(0xcecb, RealMake(datseg, 0x2848));

	/* load BUCH.DAT */
	load_pp20(0xb1);
	ds_writeb(0x2845, 0xb1);

	GUI_get_smth(&fg_bak, &bg_bak);

	ds_writed(0xd2fb, ds_readd(0xc3db));
	bak1 = ds_readw(0xd2d5);
	bak2 = ds_readw(0xd2d9);
	bak3 = ds_readw(0xd313);
	bak4 = ds_readw(0xd315);
	ds_writew(0xd2d5, 200);
	ds_writew(0xd2d9, 65);
	ds_writew(0xd313, 83);
	ds_writew(0xd315, 130);

	GUI_set_smth(4, 0);

	/* print all diary entries */
	i = 0;
	do {
		i = diary_print_entry(i);
	} while (i < ds_readw(0x43ba));

	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 199);
	ds_writed(0xc019, ds_readd(0xd303));
	ds_writed(0xc00d, ds_readd(0xd2ff));

	update_mouse_cursor();

	set_palette(Real2Host(ds_readd(0xd303)) + 0xfa02, 0, 0x20);

	do_pic_copy(0);

	refresh_screen_size();

	GUI_set_smth(fg_bak, bg_bak);

	ds_writed(0xd2fb, ds_readd(0xd2ff));
	ds_writed(0xc00d, ds_readd(0xd2ff));

	ds_writew(0xd2d9, bak2);
	ds_writew(0xd2d5, bak1);
	ds_writew(0xd313, bak3);
	ds_writew(0xd315, bak4);
	ds_writew(0xbffd, bak5);

	delay_or_keypress(5000);

}

void diary_new_entry()
{
	Bit8u *ptr;

	/* move all entries if the list is full */
	if (ds_readw(0x43ba) == 23) {
		memcpy(p_datseg + 0x43bc, p_datseg + 0x43c4, 176);
		ds_writew(0x43ba, ds_readw(0x43ba) - 1);
	}

	/* make a pointer to the last entry */
	ptr = MemBase + PhysMake(datseg, 0x43b4 + ds_readw(0x43ba) * 8);

	/* avoid double entries for the same town */
	if (ds_readb(0x2d67) == host_readw(ptr + 6))
		return;

	/* make a pointer to the current entry */
	ptr = MemBase + PhysMake(datseg, 0x43bc + ds_readw(0x43ba) * 8);

	/* increment entry counter */
	ds_writew(0x43ba, ds_readw(0x43ba) + 1);

	/* Write day of month */
	host_writew(ptr, ds_readb(0x2dc0));
	/* Write month */
	host_writew(ptr + 2, ds_readb(0x2dc1));
	/* Write year */
	host_writew(ptr + 4, ds_readb(0x2dc2));
	/* Write city */
	host_writew(ptr + 6, ds_readb(0x2d67));
}

/**
 *	diary_print_entry() - print a diary entry
 *	@line:	the line to start the entry
 */
Bit16u diary_print_entry(Bit16u line)
{
	Bit8u *ptr;
	char *city_name;
	Bit16s di = 0;
	Bit16u startline;
	Bit16u day, month;

	memset(Real2Host(ds_readd(0xc3db)), 0, 64000);

	ptr = MemBase + PhysMake(datseg, 0x43bc) + line * 8;

	startline = line;

	do {
		day = host_readw(ptr);
		month = host_readw(ptr + 2);
		city_name = (char*)get_ltx((host_readw(ptr + 6) + 0xeb) * 4);

		if (di == 0) {
			if (strlen(city_name) > 24) {
				sprintf(getString(ds_readd(0xd2f3)),
					diary_fmt[0], host_readw(ptr),
					(char*)get_ltx((host_readw(ptr + 2) + 0x15) * 4),
					city_name);
			} else if (strlen(city_name) > 15 ) {
				sprintf(getString(ds_readd(0xd2f3)),
					diary_fmt[1], host_readw(ptr),
					(char*)get_ltx((host_readw(ptr + 2) + 0x15) * 4),
					city_name);
			} else {
				sprintf(getString(ds_readd(0xd2f3)),
					diary_fmt[2], host_readw(ptr),
					(char*)get_ltx((host_readw(ptr + 2) + 0x15) * 4),
					city_name);
			}
		} else {
			if (strlen(city_name) > 24) {
				sprintf(getString(ds_readd(0xd2f3)),
					diary_fmt[3], city_name);
			} else if (strlen(city_name) > 15) {
				sprintf(getString(ds_readd(0xd2f3)),
					diary_fmt[4], city_name);
			} else if (strlen(city_name) > 6) {
				sprintf(getString(ds_readd(0xd2f3)),
					diary_fmt[5], city_name);
			} else {
				sprintf(getString(ds_readd(0xd2f3)),
					diary_fmt[6], city_name);
			}
		}
		GUI_print_string(Real2Host(ds_readd(0xd2f3)),
			65, (line - startline) * 7);
		ptr += 8;
		line++;
		di++;

	} while ((host_readw(ptr) == day) && (host_readw(ptr + 2) == month));

	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, line * 7);
	ds_writed(0xc019, ds_readd(0xc3db));
	ds_writed(0xc00d, startline * 2240 + ds_readd(0xd303) + 9600);
	do_pic_copy(2);

	return line;

}

}
