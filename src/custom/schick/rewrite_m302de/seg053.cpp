/*
	Rewrite of DSA1 v3.02_de functions of seg053 (healer)
	Functions rewritten: 1/2
*/

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "common.h"
#include "v302de.h"

#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * is_hero_healable() - check if hero can healed by a healer
 * @hero:	pointer to hero
 *
 * Returns 1 if hero can be healed, else 0.
 */
/* Borlandified and identical */
unsigned short is_hero_healable(Bit8u *hero)
{

	if (hero_dead(hero) || hero_stoned(hero)) {
		/* this hero can not be helped */
		GUI_output(get_ltx(0xc28));
		return 0;
	}

	return 1;

}

#if !defined(__BORLANDC__)
}
#endif
