/*
	Rewrite of DSA1 v3.02_de functions of seg106 (inventory misc)
	Functions rewritten: 1/8
*/
#include <stdlib.h>
#include <string.h>

#include "schick.h"

#include "schick_common.h"
#include "v302de.h"

namespace M302de {

/**
 * get_full_waterskin_pos() - search for a full waterskin
 * @hero:	pointer to the hero
 */
signed short get_full_waterskin_pos(Bit8u *hero)
{
	signed short i, pos;

	pos = -1;

	/* Original-BUG: should start from inventory pos 0 */
	for (i = 7; i < 23; i++) {

		/* look for a waterskin...*/
		if (host_readw(hero + 0x196 + i * 14) != 0x1e)
			continue;

		/* ...which is not empty */
		if ((host_readb(hero + 0x196 + 4 + i * 14) >> 2) & 1)
			continue;

		pos = i;
		break;
	}

	return pos;
}

}
