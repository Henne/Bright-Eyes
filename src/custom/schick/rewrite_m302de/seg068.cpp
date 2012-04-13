/*
 *	Rewrite of DSA1 v3.02_de functions of seg068 (Thorwal)
 *	Special City: Thorwal
 *	Functions rewritten 3/13
 *
*/

#include "schick.h"

#include "string.h"

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg097.h"

namespace M302de {

void thorwal_imman()
{

	unsigned short tmp;

	tmp = get_current_season();

	if ((tmp == 1 || tmp == 3) && (ds_readb(0x2dbf == 5))) {
		/* ask to visit the game */
		if (GUI_bool(get_city(0xdc)) == 0)
			return;

		tmp = random_schick(4) + 0x38;
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_city(0xe0),
			/* winner */
			(char*)get_city(tmp * 4),
			/* looser */
			(char*)get_city((random_schick(7) + 0x3c) * 4),
			/* winner */
			(char*)get_city(tmp * 4),
			/* winners points */
			random_interval(15, 30),
			/* loosers points */
			random_schick(14));

		GUI_input(Real2Host(ds_readd(0xd2f3)), 0);
	} else {
		/* no imman game at the moment */
		GUI_input(get_city(0xd8), 0);
	}

}

void thorwal_botschaft()
{
	bool closed = false;

	/* At the 6th month in year 17 Hal another message is shown */
	if (ds_readb(YEAR) > 17 ||
		(ds_readb(YEAR) == 17 && ds_readb(MONTH) > 5)) {

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
