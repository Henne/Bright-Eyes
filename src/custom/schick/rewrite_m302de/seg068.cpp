/*
 *	Rewrite of DSA1 v3.02_de functions of seg068 (Thorwal)
 *	Special City: Thorwal
 *	Functions rewritten: 4/13
 *
*/

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg058.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void THO_eisenhof(void)
{
	signed short answer;
	Bit32s money;

	do {
		answer = GUI_radio(get_city(0xbc), 3,
					get_city(0xc0),
					get_city(0xc4),
					get_city(0xc8));
	} while (answer == -1);

	if (answer == 1) {

		ds_writews(TYPEINDEX, 41);
		do_smith();

	} else if (answer == 2) {

		GUI_input(get_city(0xcc), 0);

		money = get_party_money();
		money -= 10;
		set_party_money(money);

		/* test for CH+0 */
		if (test_attrib(Real2Host(get_first_hero_available_in_group()), 2, 0) > 0) {

			GUI_input(get_city(0xd0), 0);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(4 * (random_schick(26) + 55)));
			GUI_input(Real2Host(ds_readd(DTP2)), 0);
		} else {
			GUI_input(get_city(0xd4), 0);
		}
	}
}

/* Borlandified and identical */
void thorwal_imman(void)
{

	unsigned short tmp;

	tmp = get_current_season();

	if ((tmp == 1 || tmp == 3) && (ds_readb(DAY_OF_WEEK) == 5)) {
		/* ask to visit the game */
		if (GUI_bool(get_city(0xdc)) != 0) {

		tmp = random_schick(4) + 0x38;
		sprintf((char*)Real2Host(ds_readd(DTP2)),
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

		GUI_input(Real2Host(ds_readd(DTP2)), 0);
		}
	} else {
		/* no imman game at the moment */
		GUI_input(get_city(0xd8), 0);
	}

}

/* Borlandified and identical */
void thorwal_botschaft(void)
{
	int closed = 0;

	/* At the 6th month in year 17 Hal another message is shown */
	if (ds_readbs(YEAR) > 17 ||
		(ds_readbs(YEAR) == 17 && ds_readbs(MONTH) > 5)) {

		closed = 1;
	}

	GUI_input( (!closed) ? get_city(0x110): get_city(0x114), 0);
}

/* Borlandified and identical */
void thorwal_mueller(void)
{
	if (GUI_bool(get_city(0x40))) {

		GUI_output((ds_readw(0x3354) == 0) ? /* first visit ? */
			get_city(0x44) : get_city(0x48));

		/* mark the miller as visited */
		ds_writew(0x3354, 1);
	}
}

#if !defined(__BORLANDC__)
}
#endif
