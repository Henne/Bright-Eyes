/*
	Rewrite of DSA1 v3.02_de functions of seg024 (diary)
	Functions rewritten: 2/3
*/

#include "schick.h"
#include "string.h"

#include "v302de.h"

#include "seg004.h"
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
