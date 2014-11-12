/**
 *	Rewrite of DSA1 v3.02_de functions of seg081 (dungeon: cave2)
 *	Functions rewritten: 1/5
 */

#include <stdio.h>

#include "v302de.h"

#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief print introductions of dungeon #6 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG6_fight_intro(signed short fight_id)
{
	if (fight_id == 86) {

		if (!ds_readb(0x3617)) {
			/* this is true all the time */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x78),
				(char*)get_dtp(0x80));
		} else {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x78),
				(char*)get_dtp(0x7c));
		}

		GUI_output(Real2Host(ds_readd(DTP2)));
	}
}

#if !defined(__BORLANDC__)
}
#endif
