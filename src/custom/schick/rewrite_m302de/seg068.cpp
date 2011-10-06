/*
 *	Rewrite of DSA1 v3.02_de functions of seg068 (Thorwal)
 *	Special City: Thorwal
 *	Functions rewritten 2/13
 *
*/

#include "schick.h"

#include "string.h"

#include "v302de.h"

#include "seg097.h"

namespace M302de {

void thorwal_botschaft()
{
	bool closed = false;

	/* At the 6th month in year 17 Hal another message is shown */
	if (ds_readw(0x2dc2) > 17 ||
		(ds_readw(0x2dc2) == 17 && ds_readw(0x2dc1) > 5)) {

		closed = true;
	}

	if (!closed)
		GUI_input(get_city(0x110), 0);
	else
		GUI_input(get_city(0x114), 0);
}

void thorwal_mueller()
{
	if (GUI_bool(get_city(0x40)) == 1) {

		if(ds_readw(0x3354) == 0) {
			/* first_visit */
			GUI_output(get_city(0x44));
		} else {
			/* after the first_visit */
			GUI_output(get_city(0x48));
		}

		/* mark the miller as visited */
		ds_writew(0x3354, 1);
	}
}

}
