/*
	Rewrite of DSA1 v3.02_de functions of seg030 (???)
	Functions rewritten: 4/6
*/

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg030.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
RealPt seg030_0000(signed short arg0)
{
	signed short i;
	signed short v2;

	for (i = v2 = 0; i < 15; i++) {
		if ((ds_readb(0x3364 + i) != 0)
			&& (arg0 - 1 != i)
			&& (i != 9)
			&& (i != 11)
			&& (i != 12)) {

			v2++;
		}
	}

	if (v2 == 0) {
		return seg030_008d(arg0);
	} else {

		do {
			i = random_schick(15);
		} while (i == 10 || i == 12 || i == 13 || !ds_readb(0x3363 + i)|| i == arg0);

		return (RealPt)host_readd(Real2Host((RealPt)ds_readd(TEXT_LTX) + (0x291 + i) * 4));
	}
}

/* Borlandified and identical */
RealPt seg030_008d(signed short arg0)
{
	signed short i;
	signed short v2;

	for (i = v2 = 0; i < 15; i++) {
		if (!(ds_readb(0x3364 + i))
			&& (arg0 - 1 != i)
			&& (i != 9)
			&& (i != 11)
			&& (i != 12)) {

			v2++;
		}
	}

	if (v2 == 0) {
		return seg030_0000(arg0);
	} else {

		do {
			i = random_schick(15);
		} while (i == 10 || i == 12 || i == 13 || ds_readb(0x3363 + i)|| i == arg0);
		ds_writeb(0x3363 + i, 1);

		return (RealPt)host_readd(Real2Host((RealPt)ds_readd(TEXT_LTX) + (0x291 + i) * 4));
	}
}

/* 0x11e */
/* unused in the game */
/* Borlandified and identical */
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
/* Borlandified and identical */
void prepare_date_str(void)
{
	unsigned short unused;
	signed short hour;

	unused = 0;

	hour = (signed short)(ds_readd(DAY_TIMER) / 0x1518);

	if (ds_readbs(DAY_OF_MONTH) < 0) {
		/* Days of the nameless */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0x61c),
			get_ltx((0x15d + ds_readbs(DAY_OF_WEEK)) * 4),
			ds_readbs(YEAR), hour);
	} else {
		/* Normal day */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0x590),
			get_ltx((0x15d + ds_readbs(0x2dbf)) * 4),
			ds_readbs(DAY_OF_MONTH),
			get_ltx((0x15 + ds_readbs(MONTH)) * 4),
			get_ltx((0x227 + get_current_season()) * 4),
			ds_readbs(YEAR), hour);
	}

	if (ds_readbs(SPECIAL_DAY) != 0) {
		sprintf((char*)Real2Host(ds_readd(0xd2eb)),
			(char*)get_ltx(0x594),
			get_ltx((0x165 + ds_readbs(SPECIAL_DAY)) * 4));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)Real2Host(ds_readd(0xd2eb)));
	}
}

#if !defined(__BORLANDC__)
}
#endif
