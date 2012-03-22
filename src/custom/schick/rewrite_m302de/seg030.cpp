/*
	Rewrite of DSA1 v3.02_de functions of seg030 (???)
	Functions rewritten: 2/6
*/

#include <string.h>
#include "schick.h"

#include "schick_common.h"
#include "v302de.h"

#include "seg002.h"
#include "seg030.h"
#include "seg097.h"

namespace M302de {

/* 0x11e */
/* unused in the game */
void print_date(void)
{
	unsigned short bak;

	prepare_date_str();
	bak = ds_readw(0xbffd);
	ds_writew(0xbffd, 3);

	GUI_input(Real2Host(ds_readd(DTP2)), 0);
	ds_writew(0xbffd, bak);
}

/* 0x14f */
void prepare_date_str(void)
{
	unsigned short unused;
	signed short hour;

	unused = 0;

	hour = ds_readd(0x2dbb) / 0x1518;

	if ((signed char)ds_readb(DAY_OF_MONTH) < 0) {
		/* Days of the nameless */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0x61c),
			get_ltx((0x15d + ds_readb(DAY_OF_WEEK)) * 4),
			ds_readb(YEAR), hour);
	} else {
		/* Normal day */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0x590),
			get_ltx((0x15d + ds_readb(0x2dbf)) * 4),
			ds_readb(DAY_OF_MONTH),
			get_ltx((0x15 + ds_readb(MONTH)) * 4),
			get_ltx((0x227 + get_current_season()) * 4),
			ds_readb(YEAR), hour);
	}

	if (ds_readb(SPECIAL_DAY) != 0) {
		sprintf((char*)Real2Host(ds_readd(0xd2eb)),
			(char*)get_ltx(0x594),
			get_ltx((0x165 + ds_readb(SPECIAL_DAY)) * 4));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)Real2Host(ds_readd(0xd2eb)));
	}
}

}
