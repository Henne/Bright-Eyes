/*
 *	Rewrite of DSA1 v3.02_de functions of seg064 (harbour_helper)
 *	Functions rewritten: 6/6 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg064.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg007.h"
#include "seg028.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   returns a pointer to the name of a ship
 *
 * \param   passage_type           eg 0 = longship, 5 = coastal ship, 6 = cutter,7 = fishing boat
 * \param   nr_ships_created	number of ships which have already been created before (for avoiding duplicate ship names)
 */

RealPt get_ship_name(signed char passage_type, signed short nr_ships_created)
	/* Function is called in a single place only, which is contained in the code of Original-Bug 23 (see below).
	 * Therefore, the function could be removed if M302de_ORIGINAL_BUGFIX is activated.
	 */
{
	signed char done, i;
	signed short name;

	done = 0;

	do {
		name = passage_type * 10 + random_schick(10) + 0x29;
		done = 1;

		/* check if there is already a ship with the same name */
		for (i = 0; i < nr_ships_created; i++) {
			if (ds_readd(HARBOR_OPTIONS + i * SIZEOF_HARBOR_OPTION + HARBOR_OPTION_SHIP_NAME_PTR)
				== host_readd(Real2Host(ds_readd(TX_INDEX)) + name * 4)) {
				done = 0;
				break;
			}
		}

	} while (!done);

	return (RealPt)host_readd(Real2Host(ds_readd(TX_INDEX)) + name * 4);
}

/**
 * \brief   */
unsigned short prepare_passages(void)
{
	signed short prepared;
	signed short i;
	RealPt ent;

#if !defined(__BORLANDC__)
	ent = RealMake(datseg, SEA_ROUTES);
#else
	ent = p_datseg + SEA_ROUTES;
#endif

	for (i = prepared = 0; i < NR_SEA_ROUTES; ent += SIZEOF_SEA_ROUTE, i++) {
		if (!host_readbs(Real2Host(ent) + SEA_ROUTE_PASSAGE_TIMER) && /* passage is available today */
			(host_readb(Real2Host(ent) + SEA_ROUTE_TOWN_1) == ds_readb(CURRENT_TOWN) || 
			(host_readb(Real2Host(ent) + SEA_ROUTE_TOWN_2) == ds_readb(CURRENT_TOWN)))) {

			/* prepare an entry of 12 byte for a passage today */
			ds_writeb((HARBOR_OPTIONS + HARBOR_OPTION_ROUTE_ID) + prepared * SIZEOF_HARBOR_OPTION, (unsigned char)i);
			ds_writed((HARBOR_OPTIONS + HARBOR_OPTION_ROUTE_PTR) + prepared * SIZEOF_HARBOR_OPTION, (Bit32u)ent);
			ds_writeb((HARBOR_OPTIONS + HARBOR_OPTION_SHIP_TIMER) + prepared * SIZEOF_HARBOR_OPTION, 0);
			ds_writeb((HARBOR_OPTIONS + HARBOR_OPTION_SHIP_TYPE) + prepared * SIZEOF_HARBOR_OPTION, host_readb(Real2Host(ent) + SEA_ROUTE_PASSAGE_TYPE));

#ifndef M302de_ORIGINAL_BUGFIX
			/* Original-Bug 23:
			 * In the function get_ship_name(..), the ship names are created randomly every time the party checks the available ships at a harbour.
			 * In this way, the names of the ships can (and usually do) change when repeatedly checking the available ships. */
			ds_writed(HARBOR_OPTIONS + prepared * SIZEOF_HARBOR_OPTION + HARBOR_OPTION_SHIP_NAME_PTR,
				(Bit32u)get_ship_name(host_readb(Real2Host(ent) + SEA_ROUTE_PASSAGE_TYPE), prepared));
#else
			/* As a fix, we derive the name from the PASSAGE_PRICE_MOD entry of the SEA_ROUTE, which is created
			 * randomly once the new passage of the route is set up, and is kept fixed over the lifetime of the passage.
			 *
			 * In this way, it may however happen that two ships of the same name are located in a harbour
			 * (which has been avoided in the original random assignment code). But this is a rare event and not be a big issue anyway.
			 */
			ds_writed(HARBOR_OPTIONS + prepared * SIZEOF_HARBOR_OPTION + HARBOR_OPTION_SHIP_NAME_PTR,
				(RealPt)host_readd(Real2Host(ds_readd(TX_INDEX)) + 4 * (host_readb(Real2Host(ent) + SEA_ROUTE_PASSAGE_TYPE) * 10 + (host_readb(Real2Host(ent) + SEA_ROUTE_PASSAGE_PRICE_MOD)) % 10 + 0x2a))
			);
#endif

			ds_writeb((HARBOR_OPTIONS + HARBOR_OPTION_DESTINATION) + prepared * SIZEOF_HARBOR_OPTION,
				host_readb(Real2Host(ent) + SEA_ROUTE_TOWN_1) == ds_readb(CURRENT_TOWN) ?
					host_readb(Real2Host(ent) + SEA_ROUTE_TOWN_2) :
					host_readb(Real2Host(ent) + SEA_ROUTE_TOWN_1)
			); /* store the other town of the connection */
			prepared++;
		} else {
			/* not before 14.00 o'clock */
			if (((signed long)ds_readd(DAY_TIMER) > (0x1518 * 14L))
				/* only for ships tomorrow */
				&& (host_readb(Real2Host(ent) + SEA_ROUTE_PASSAGE_TIMER) == 1)
				/* only in this city */
				&& ((host_readb(Real2Host(ent) + SEA_ROUTE_TOWN_1) == ds_readb(CURRENT_TOWN))
				|| (host_readb(Real2Host(ent) + SEA_ROUTE_TOWN_2) == ds_readb(CURRENT_TOWN))))
			{
				/* prepare an entry of 12 byte for a passage tomorrow */
				ds_writeb((HARBOR_OPTIONS + HARBOR_OPTION_ROUTE_ID) + prepared * SIZEOF_HARBOR_OPTION, (unsigned char)i);
				ds_writed((HARBOR_OPTIONS + HARBOR_OPTION_ROUTE_PTR) + prepared * SIZEOF_HARBOR_OPTION, (Bit32u)ent);
				ds_writeb((HARBOR_OPTIONS + HARBOR_OPTION_SHIP_TIMER) + prepared * SIZEOF_HARBOR_OPTION, 1);
				ds_writeb((HARBOR_OPTIONS + HARBOR_OPTION_SHIP_TYPE) + prepared * SIZEOF_HARBOR_OPTION, host_readb(Real2Host(ent) + SEA_ROUTE_PASSAGE_TYPE));
				ds_writed(HARBOR_OPTIONS + prepared * SIZEOF_HARBOR_OPTION + HARBOR_OPTION_SHIP_NAME_PTR,
					(Bit32u)get_ship_name(host_readb(Real2Host(ent) + SEA_ROUTE_PASSAGE_TYPE), prepared));
				ds_writeb((HARBOR_OPTIONS + HARBOR_OPTION_DESTINATION) + prepared * SIZEOF_HARBOR_OPTION ,
					host_readb(Real2Host(ent) + SEA_ROUTE_TOWN_1) == ds_readb(CURRENT_TOWN) ?
						host_readb(Real2Host(ent) + SEA_ROUTE_TOWN_2) :
						host_readb(Real2Host(ent) + SEA_ROUTE_TOWN_1)
				);
				prepared++;
			}
		}
	}
	return prepared;
}

/**
 * \brief   calculates the price and prints it to the text buffer
 *
 * \param   price             base price per unit of distance
 * \param   route_pointer     pointer to the sea route
 * \return                    a pointer to the text buffer.
 */
RealPt print_passage_price(signed short price, Bit8u *route_ptr)
{
	unsigned short di;

	if (price != 0) {
		/* calc price per distance */
		di = ((unsigned char)host_readb(route_ptr + SEA_ROUTE_PASSAGE_PRICE_MOD) * price + 4) / 10;
		/* multiply with distance */
		di = di * (unsigned char)host_readb(route_ptr + SEA_ROUTE_DISTANCE);
		/* round up and divide by 100 */
		price = (di + 49) / 100;

		/* generate a price string "%d^HELLER" */
		sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
			(char*)p_datseg + SEA_TRAVEL_STR_HELLER, price);
	} else {
		/* "NICHTS" */
		strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
			(char*)p_datseg + SEA_TRAVEL_STR_NOTHING);
	}
	ds_writew(SEA_TRAVEL_PASSAGE_PRICE, price);

	return (RealPt)ds_readd(TEXT_OUTPUT_BUF);

}

/**
 * \brief   calculates the travelling time of a ship passage in hours
 *
 * \param   distance     the distance
 * \param   base_speed   base speed
 * \return               the travelling time in hours
 */
unsigned short get_passage_travel_hours(signed short distance, signed short base_speed)
{
	Bit32u tmp;

	base_speed = (base_speed * 10 + 11) / 24;

	/*	WEATHER1 = random(6)
	 *	WEATHER2 = random(7) */
	ds_writew(SEA_TRAVEL_PASSAGE_SPEED2,
		(base_speed * (ds_readw(WEATHER2) + 6) * (ds_readw(WEATHER1) * 15 + 100) + 499L) / 1000L);

	tmp = (ds_readws(SEA_TRAVEL_PASSAGE_SPEED2) + 4) / 10; /* the speed of the ship */

	if (tmp == 0)
		tmp = 1;

	tmp = distance / tmp; /* now 'tmp' is the number of travelling hours */

	return (unsigned short)tmp;
}

/**
 * \brief   get destination harbours
 *
 * \param   type        1 = passages next days / 2 = all passages
 */
unsigned short get_next_passages(unsigned short type)
{
	Bit8u *entry;
	signed short destinations;
	signed short i;

	entry = p_datseg + SEA_ROUTES;

	for (i = destinations = 0; i < NR_SEA_ROUTES; entry += SIZEOF_SEA_ROUTE, i++) {

		if (type == 1) {
			/* check passages in the next two days */
			if (host_readb(entry + SEA_ROUTE_PASSAGE_TIMER) == 1 || host_readb(entry + SEA_ROUTE_PASSAGE_TIMER) == 2) {
				/* compare town */
				if (host_readb(entry + SEA_ROUTE_TOWN_1) == ds_readb(CURRENT_TOWN) ||
					host_readb(entry + SEA_ROUTE_TOWN_2) == ds_readb(CURRENT_TOWN))
				{
#if !defined(__BORLANDC__)
					ds_writeb(HARBOR_OPTIONS + 10 + destinations * SIZEOF_HARBOR_OPTION,
						host_readb(entry + SEA_ROUTE_TOWN_1) == ds_readb(CURRENT_TOWN) ?
							host_readb(entry + SEA_ROUTE_TOWN_2):
							host_readb(entry + SEA_ROUTE_TOWN_1));
#else
					((struct passages*)(p_datseg + HARBOR_OPTIONS))[destinations].town =
						host_readb(entry + SEA_ROUTE_TOWN_1) == ds_readb(CURRENT_TOWN) ?
							host_readb(entry + SEA_ROUTE_TOWN_2):
							host_readb(entry + SEA_ROUTE_TOWN_1);
#endif
					destinations++;
				}
			}
		} else {
			/* compare town */
			if (host_readb(entry + SEA_ROUTE_TOWN_1) == ds_readb(CURRENT_TOWN) ||
				host_readb(entry + SEA_ROUTE_TOWN_2) == ds_readb(CURRENT_TOWN))
			{
#if !defined(__BORLANDC__)
				ds_writeb(HARBOR_OPTIONS + 10 + destinations * SIZEOF_HARBOR_OPTION,
					host_readb(entry) == ds_readb(CURRENT_TOWN) ?
						host_readb(entry + SEA_ROUTE_TOWN_2):
						host_readb(entry + SEA_ROUTE_TOWN_1));
#else
					((struct passages*)(p_datseg + HARBOR_OPTIONS))[destinations].town =
						host_readb(entry + SEA_ROUTE_TOWN_1) == ds_readb(CURRENT_TOWN) ?
							host_readb(entry + SEA_ROUTE_TOWN_2):
							host_readb(entry + SEA_ROUTE_TOWN_1);
#endif
				destinations++;
			}
		}
	}

	return destinations;
}

unsigned short passage_arrival(void)
{
	signed short tmp;
	Bit8u *harbor_ptr;
	Bit8u *buildings;
	Bit8u *p_sched;
	signed short si;
	signed short harbor_id;

	harbor_id = 0;
	harbor_ptr = p_datseg + HARBORS;

	p_sched = p_datseg + SEA_ROUTES + ds_readb(SEA_TRAVEL_PASSAGE_ID) * SIZEOF_SEA_ROUTE;

	/* write the destination to a global variable (assignement in condition)*/
	if ((ds_writew(TRV_DEST_REACHED, host_readb(p_sched + SEA_ROUTE_TOWN_1))) == ds_readbs(CURRENT_TOWN))
		ds_writew(TRV_DEST_REACHED, host_readb(p_sched + SEA_ROUTE_TOWN_2));

	do {
		if (host_readb(harbor_ptr) == ds_readw(TRV_DEST_REACHED)) {
			si = 0;
			do {
				tmp = host_readb(Real2Host(host_readd(harbor_ptr + 2)) + si) - 1;
				if (host_readb(p_datseg + SEA_ROUTES + tmp * SIZEOF_SEA_ROUTE + SEA_ROUTE_TOWN_1) == ds_readb(CURRENT_TOWN) ||
					host_readb(p_datseg + SEA_ROUTES + tmp * SIZEOF_SEA_ROUTE + SEA_ROUTE_TOWN_2) == ds_readb(CURRENT_TOWN)) {
					harbor_id = (unsigned char)host_readb(harbor_ptr + 1);
					break;
				}

				si++;
			} while (host_readb(Real2Host(host_readd(harbor_ptr + 2)) + si) != 0xff);
		}
		/* set pointer to the next structure */
		harbor_ptr += 6;
	} while (harbor_id == 0 && host_readb(harbor_ptr) != 0xff);

	if (harbor_id != 0) {

		/* save the old town in tmp */
		tmp = (signed char)ds_readb(CURRENT_TOWN);
		/* set the new current_town */
		ds_writeb(CURRENT_TOWN, ds_readb(TRV_DEST_REACHED));

		/* load the area  of the new town */
		call_load_area(1);


		/* search for the harbour in the map */
		buildings = p_datseg + LOCATIONS_TAB;
		while ((host_readb(buildings + 2) != 0x0b) ||
				(host_readb(buildings + 3) != harbor_id)) {
			buildings += 6;
		}

		/* set the position of the party */
		si = host_readw(buildings + 4);
		ds_writew(ARRIVAL_X_TARGET, (si >> 8) & 0xff);
		ds_writew(ARRIVAL_Y_TARGET, si & 0x0f);
		ds_writew(ARRIVAL_DIRECTION, (si >> 4) & 0x0f);

		/* restore the old town area / TODO: a bit bogus */
		ds_writeb(CURRENT_TOWN, (unsigned char)tmp);
		call_load_area(1);
	}

	return 0;

}

#if !defined(__BORLANDC__)
}
#endif
