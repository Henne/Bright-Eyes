/**
 *	Rewrite of DSA1 v3.02_de functions of seg078 (dungeon: inn)
 *	Functions rewritten: 1/10
 */

#include "v302de.h"

#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief print introductions of dungeon #2 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG2_fight_intro(signed short fight_id)
{
	if (fight_id == 62) {
		GUI_output(get_dtp(0x80));
	} else if (fight_id == 66) {
		GUI_output(get_dtp(0x84));
	} else if (fight_id == 68) {
		GUI_output(get_dtp(0x88));
	} else if (fight_id == 72) {
		GUI_output(get_dtp(0x90));
	}
}

#if !defined(__BORLANDC__)
}
#endif
