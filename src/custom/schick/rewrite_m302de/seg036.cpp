/**
 *	Rewrite of DSA1 v3.02_de functions of seg036 (Fight Hero KI)
 *	Functions rewritten: 1/10
 */

#include "schick.h"

#include "v302de.h"

#include "seg002.h"

namespace M302de {

/* 0x863 */

/**
 * KI_get_spell() - ???
 * @spell:	spell index
 * @mode:	mode 0/1
 */
signed short KI_get_spell(signed short spell, signed short mode)
{
	Bit8u * p;
	signed short retval = -1;

	/* make a pointer to the spell description */
	p = p_datseg + spell * 10 + 0x99d;

	if (mode == 0) {
		if (host_readb(p + 7) == 2)
			retval = 1;
		else if (host_readb(p + 7) == 1 || host_readb(p + 7) == 3)
			retval = 0;
		else
			retval = 2;
	} else {
		if (host_readb(p + 7) == 3)
			retval = 1;
		else if (host_readb(p + 7) == 0)
			retval = 2;
	}

	return retval;
}

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
