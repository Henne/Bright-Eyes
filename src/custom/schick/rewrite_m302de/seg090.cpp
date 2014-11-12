/**
 *	Rewrite of DSA1 v3.02_de functions of seg090 (dungeon: oberorken mine)
 *	Functions rewritten: 1/4
 */

#include "v302de.h"

#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief print introductions of dungeon #12 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG12_fight_intro(signed short fight_id)
{
	if (fight_id == 141) {
		GUI_output(get_dtp(0x30));
	}
}

#if !defined(__BORLANDC__)
}
#endif
