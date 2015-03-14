/**
 *	Rewrite of DSA1 v3.02_de functions of seg090 (dungeon: oberorken mine)
 *	Functions rewritten: 2/4
 */

#include "v302de.h"

#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void DNG_clear_corridor(Bit8u *ptr)
{
	/* ask if the corridor should be cleared */
	if (GUI_bool(get_dtp(0x4))) {

		/* clear the corridor */

		GUI_output(get_dtp(0x8));

		timewarp(0x1fa40L);

		host_writebs(ptr, -1);
	} else {
		ds_writew(X_TARGET, ds_readws(0x2d83));
		ds_writew(Y_TARGET, ds_readws(0x2d85));
	}
}

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
