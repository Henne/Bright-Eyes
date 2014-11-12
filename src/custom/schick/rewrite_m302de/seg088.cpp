/**
 *	Rewrite of DSA1 v3.02_de functions of seg088 ()
 *	Functions rewritten: 1/12
 */

#include "v302de.h"

#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief print introductions of dungeon #14 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG14_fight_intro(signed short fight_id)
{
	if ((fight_id == 172) || (fight_id == 174)) {
		GUI_output(get_dtp(0xe0));
	}
}

#if !defined(__BORLANDC__)
}
#endif
