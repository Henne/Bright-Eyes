/**
 *	Rewrite of DSA1 v3.02_de functions of seg091 (dungeon: prem mine)
 *	Functions rewritten: 1/9
 */

#include "v302de.h"

#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief print introductions of dungeon #13 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG13_fight_intro(signed short fight_id)
{
	if (fight_id == 140) {
		GUI_output(get_dtp(0x34));
	}
}

#if !defined(__BORLANDC__)
}
#endif
