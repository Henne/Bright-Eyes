/**
 *	Rewrite of DSA1 v3.02_de functions of seg036 (Fight Hero KI)
 *	Functions rewritten: 1/10
 */

#include "schick.h"

#include "v302de.h"

#include "seg002.h"

namespace M302de {

/* 0xc39 */

/**
 * KI_count_available_heros() - count heros in group
 * @index:	index of the calling hero
 *
 * Retval: Number of heros in the group - 1
 */
signed short KI_count_heros(signed short index)
{
	signed short cnt = 0;
	signed short i;

	/* for each hero in this group */
	for (i = 0; ds_readb(0x2d36 + ds_readb(CURRENT_GROUP)) > i; i++) {

		if (index == i)
			continue;

		if (!check_hero(get_hero(i)))
			continue;

		cnt++;
	}

	return cnt;
}

}
