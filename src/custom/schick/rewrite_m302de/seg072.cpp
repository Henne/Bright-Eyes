/**
 *	Rewrite of DSA1 v3.02_de functions of seg072 (informers)
 *	Functions rewritten: 2/9
 */

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"

#include "seg002.h"

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

	for (i = parts = 0; i < 9; i++) {
		if (ds_readb(TREASURE_MAPS + i) != 0)
			parts++;
	}

	return parts;
}

/**
 * has_intro_letter - check if a hero in the group has the introduction letter
 *
 */
short has_intro_letter(void)
{
	/* check for the introduction letter / Empfehlungsschreiben */
	return (get_first_hero_with_item(0xeb) != -1) ? 1 : 0;
}

#if !defined(__BORLANDC__)
}
#endif

