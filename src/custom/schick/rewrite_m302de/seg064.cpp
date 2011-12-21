/*
	Rewrite of DSA1 v3.02_de functions of seg064 (harbour_helper)
	Functions rewritten: 3/6
*/
#include <string.h>

#include "schick.h"

#include "schick_common.h"
#include "v302de.h"

#include "seg007.h"
#include "seg028.h"

namespace M302de {

/**
 *	get_ship_name() - returns a pointer to the name of a ship
 *	@ship_type:	eg 0 = longship, 5= coastal ship,
 *				6= cutter,7= fishing boat
 *	@arg2:		???
 */
RealPt get_ship_name(signed char ship_type, signed short arg2)
{
	signed char done, i;
	signed short name;

	done = 0;

	do {
		name = ship_type * 10 + random_schick(10) + 0x29;
		done = 1;
		for (i = 0; i < arg2; i++) {
			if (host_readd((Real2Host(ds_readd(0x42b2)) + i * 12))
				!= host_readd(Real2Host(ds_readd(0xc3b1)) + name * 4))
				continue;
			done = 0;
			break;
		}

	} while (done == 0);

	return host_readd(Real2Host(ds_readd(0xc3b1)) + name * 4);
}

/**
 *	print_passage_price()	- calculates the price and prints to buffer
 *	@price:	a price factor
 *	@entry:	pointer to the schedule
 *
 *	Returns a pointer to the buffer.
 */
RealPt print_passage_price(signed short price, Bit8u *entry)
{
	signed short di;

	if (price != 0) {
		/* calc price per distance */
		di = ((unsigned char)host_readb(entry + 7) * price + 4) / 10;
		/* multiply with distance */
		di = di * (unsigned char)host_readb(entry + 2);
		/* round up and divide by 100 */
		price = (di + 49) / 100;

		/* generate a price string "%d^HELLER" */
		sprintf((char*)Real2Host(ds_readd(0xd2eb)),
			(char*)p_datseg + 0x7096, price);
	} else {
		/* "NICHTS" */
		strcpy((char*)Real2Host(ds_readd(0xd2eb)),
			(char*)p_datseg + 0x70a0);
	}
	ds_writew(0x432a, price);
	return ds_readd(0xd2eb);

}

unsigned short get_passage_travel_hours(signed short arg1, signed short arg2)
{
	int hours;

	arg2 = (arg2 * 10 + 11) / 24;

	/*	ds:0x331b = random(6)
	 *	ds:0x331d = random(7) */
	ds_writew(0x432c, (unsigned short)
		((ds_readw(0x331d) + 6) * arg2 * (ds_readw(0x331b) * 15 + 100) + 499)/ 1000);

	hours = (ds_readw(0x432c) + 4) / 10;

	if (hours == 0)
		hours = 1;

	hours = arg1 / hours;

	return (unsigned short)hours;
}

}
