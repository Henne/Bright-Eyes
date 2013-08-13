/**
 *	Rewrite of DSA1 v3.02_de functions of seg072 (informers)
 *	Functions rewritten: 1/9
 */

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * count_map_parts - counts the parts of the treasure map
 */
short count_map_parts(void)
{
	int i;
	int parts;

	parts = 0;
	for (i = 0; i < 9; i++) {
		if (ds_readb(TREASURE_MAPS + i) != 0)
			parts++;
	}

	return parts;
}

#if !defined(__BORLANDC__)
}
#endif

