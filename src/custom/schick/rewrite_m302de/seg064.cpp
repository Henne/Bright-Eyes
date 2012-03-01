/*
	Rewrite of DSA1 v3.02_de functions of seg064 (harbour_helper)
	Functions rewritten: 6/6 (complete)
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
				!= host_readd(Real2Host(ds_readd(DIALOG_TEXT)) + name * 4))
				continue;
			done = 0;
			break;
		}

	} while (done == 0);

	return host_readd(Real2Host(ds_readd(DIALOG_TEXT)) + name * 4);
}

/**
 *	prepare_passages()	-
 */
unsigned short prepare_passages(void)
{
	Bit8u *entry;
	RealPt ent;
	unsigned short prepared, i;

	entry = p_datseg + 0x6f00;
	ent = RealMake(datseg, 0x6f00);
	prepared = 0;

	for (i = 0; i < 45; entry += 8, ent += 8, i++) {
		if (host_readb(entry + 4) == 0 &&
			(host_readb(entry) == ds_readb(0x2d67) ||
			(host_readb(entry + 1) == ds_readb(0x2d67)))) {

			/* prepare an entry of 12 byte for a passage today */
			ds_writeb(0x42bd + prepared * 12, (unsigned char)i);
			ds_writed(0x42b6 + prepared * 12, ent);
			ds_writeb(0x42ba + prepared * 12, 0);
			ds_writeb(0x42bb + prepared * 12, host_readb(entry + 6));
			ds_writed(0x42b2 + prepared * 12,
				get_ship_name(host_readb(entry + 6), prepared));
			ds_writeb(0x42bc + prepared * 12,
				host_readb(entry) == ds_readb(0x2d67) ?
					host_readb(entry + 1) :
					host_readb(entry));
			prepared++;
		} else {
			/* not before 14.00 o'clock */
			if (ds_readd(0x2dbb) < 0x1518 * 14)
				continue;
			/* only for ships tomorrow */
			if (host_readb(entry + 4) != 1)
				continue;
			/* only in this city */
			if (host_readb(entry) != ds_readb(0x2d67) &&
				host_readb(entry + 1) != ds_readb(0x2d67))
				continue;

			/* prepare an entry of 12 byte for a passage tomorrow */
			ds_writeb(0x42bd + prepared * 12, (unsigned char)i);
			ds_writed(0x42b6 + prepared * 12, ent);
			ds_writeb(0x42ba + prepared * 12, 1);
			ds_writeb(0x42bb + prepared * 12, host_readb(entry + 6));
			ds_writed(0x42b2 + prepared * 12,
				get_ship_name(host_readb(entry + 6), prepared));
			ds_writeb(0x42bc + prepared * 12,
				host_readb(entry) == ds_readb(0x2d67) ?
					host_readb(entry + 1) :
					host_readb(entry));
			prepared++;
		}
	}
	return prepared;
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
	unsigned short di;

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

/**
 *	get_next_passages() -	get destination harbours
 *	@type:	1 = passages next days / 2 = all passages
 */
unsigned short get_next_passages(unsigned short type)
{
	Bit8u *entry;
	unsigned short i, destinations;

	entry = p_datseg + 0x6f00;
	destinations = 0;
	i = 0;

	for (i = 0; i < 45; entry += 8, i++) {

		if (type == 1) {
			/* check passages in the next two days */
			if (host_readb(entry + 4) == 1 || host_readb(entry + 4) == 2) {
				/* compare town */
				if (host_readb(entry) == ds_readb(0x2d67) ||
					host_readb(entry + 1) == ds_readb(0x2d67))
				{
					ds_writeb(0x42bc + destinations * 12,
						host_readb(entry) == ds_readb(0x2d67) ?
							host_readb(entry + 1):
							host_readb(entry));
					destinations++;
				}
			}
		} else {
			/* compare town */
			if (host_readb(entry) == ds_readb(0x2d67) ||
				host_readb(entry + 1) == ds_readb(0x2d67))
			{
				ds_writeb(0x42bc + destinations * 12,
					host_readb(entry) == ds_readb(0x2d67) ?
						host_readb(entry + 1):
						host_readb(entry));
				destinations++;
			}
		}
	}
	return destinations;

}

unsigned short passage_arrival(void)
{
	Bit8u *p1, *buildings, *p_sched;
	signed short tmp, si, di;

	di = 0;
	p1 = p_datseg + 0xa3a3;

	p_sched = p_datseg + 0x6f00 + ds_readb(0x42b1) * 8;

	/* write the destination to a global variable */
	ds_writew(0x4338, host_readb(p_sched));
	if (ds_readw(0x4338) == (signed char)ds_readb(0x2d67))
		ds_writew(0x4338, host_readb(p_sched + 1));

	do {
		if (host_readb(p1) == ds_readw(0x4338)) {
			si = 0;
			do {
				tmp = host_readb(Real2Host(host_readd(p1 + 2)) + si) - 1;
				if (host_readb(p_datseg + 0x6f00 + tmp * 8) == ds_readb(0x2d67) ||
					host_readb(p_datseg + 0x6f00 + tmp * 8 + 1) == ds_readb(0x2d67)) {
					di = (unsigned char)host_readb(p1 + 1);
					break;
				}

				si++;
			} while (host_readb(Real2Host(host_readd(p1 + 2)) + si) != 0xff);
		}
		/* set pointer to the next structure */
		p1 += 6;
	} while (di == 0 && host_readb(p1) != 0xff);

	/* TODO: check until here */
	if (di == 0)
		return 0;

	/* save the old town in tmp */
	tmp = (signed char)ds_readb(0x2d67);
	/* set the new current_town */
	ds_writeb(0x2d67, ds_readb(0x4338));

	/* load the area  of the new town */
	call_load_area(1);


	/* search for the harbour in the map */
	buildings = p_datseg + 0xc025;
	while ((host_readb(buildings + 2) != 0x0b) ||
			(host_readb(buildings + 3) != di)) {
		buildings += 6;
	}

	/* set the position of the party */
	si = host_readw(buildings + 4);
	ds_writew(0x433a, (si >> 8) & 0xff);
	ds_writew(0x433c, si & 0x0f);
	ds_writew(0x433e, (si >> 4) & 0x0f);

	/* restore the old town area / TODO: a bit bogus */
	ds_writeb(0x2d67, (unsigned char)tmp);
	call_load_area(1);

	return 0;

}

}
