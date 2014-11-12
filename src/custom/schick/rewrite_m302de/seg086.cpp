/**
 *	Rewrite of DSA1 v3.02_de functions of seg086 (dungeon: piratecave)
 *	Functions rewritten: 1/16
 */

#include "v302de.h"

#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief print introductions of dungeon #11 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG11_fight_intro(signed short fight_id)
{
	if (fight_id == 127) {
		GUI_output(get_dtp(0x38));
	}
}

#if !defined(__BORLANDC__)
}
#endif
