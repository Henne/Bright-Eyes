/**
 *	Rewrite of DSA1 v3.02_de functions of seg084 (dungeon: dungeon)
 *	Functions rewritten: 1/10
 */

#include "v302de.h"

#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief print introductions of dungeon #9 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG9_fight_intro(signed short fight_id)
{
	if (fight_id == 94) {
		GUI_output(get_dtp(0x34));
	}
}

#if !defined(__BORLANDC__)
}
#endif
