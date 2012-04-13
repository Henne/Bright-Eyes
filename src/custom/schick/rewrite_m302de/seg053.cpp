/*
	Rewrite of DSA1 v3.02_de functions of seg053 (healer)
	Functions rewritten: 1/2
*/

#include "schick.h"

#include "schick_common.h"
#include "v302de.h"

#include "seg097.h"

namespace M302de {

/**
 * is_hero_healable() - check if hero can healed by a healer
 * @hero:	pointer to hero
 *
 * Returns 1 if hero can be healed, else 0.
 */
unsigned short is_hero_healable(Bit8u *hero)
{

	if (hero_dead(hero) || hero_stoned(hero)) {
		/* this hero can not be helped */
		GUI_output(get_ltx(0xc28));
		return 0;
	}

	return 1;

}

}
