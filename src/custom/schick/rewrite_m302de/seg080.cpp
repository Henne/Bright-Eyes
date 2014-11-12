/**
 *	Rewrite of DSA1 v3.02_de functions of seg080 (dungeon: wolvcave, cave)
 *	Functions rewritten: 1/9
 */

#include "v302de.h"

#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief print introductions of dungeon #5 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG5_fight_intro(signed short fight_id)
{
	if (fight_id == 76) {
		GUI_output(get_dtp(0x28));
	}
}

#if !defined(__BORLANDC__)
}
#endif
