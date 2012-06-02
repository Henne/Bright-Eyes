/*
	Rewrite of DSA1 v3.02_de functions of seg111 (travel events 3 / 10)
	Functions rewritten: 1/15
*/

#include "schick.h"

#include "v302de.h"

#include "seg002.h"
#include "seg026.h"
#include "seg097.h"

namespace M302de {

/* The rider Orvil <-> Ala */
void tevent_065(void)
{
	register signed short answer;

	/* load dialog head */
	load_in_head(0x38);

	/* show dialogbox */
	do {
		answer = GUI_dialogbox(ds_readd(DTP2),
				NULL,
				get_city(0x12c),
				3,
				get_city(0x130),
				get_city(0x134),
				get_city(0x138));

	} while (answer == -1);

	/* talk to the rider for 15 min */
	if (answer == 1 || answer == 2) {

		timewarp(0x546);

		if (answer == 1)
			GUI_dialog_na(0, get_city(0x13c));
		else
			GUI_dialog_na(0, get_city(0x140));

		if (answer == 2) {
			/* Make Umbrik Siebenstein known */

			/* Original-Bug: if Umbrik is done the value 2
					is overwritten with 1 */
#ifdef M302de_ORIGINAL_BUGFIX
			if (ds_readb(0x3367) == 0)
				ds_writeb(0x3367, 1);
#else
			ds_writeb(0x3367, 1);
#endif
		}

	}
}

}
