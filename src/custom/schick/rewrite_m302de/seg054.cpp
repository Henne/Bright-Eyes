/**
 *	Rewrite of DSA1 v3.02_de functions of seg054 (inn)
 *	Functions rewritten: 2/4
 */

#include "v302de.h"

#include "seg031.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void talk_inn(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else
	do_random_talk(1, 0);
#endif
}

/**
 * \brief	returns a pointer to the first busy hero, who is not in this group, but in this location
 */
/* Borlandified and identical */
RealPt get_first_busy_hero(void)
{
	RealPt hero;
	signed short i;

	hero = (RealPt)ds_readd(HEROS);
	for (i = 0; i < 6; i++, hero += 0x6da) {
		if (host_readbs(Real2Host(hero) + 0x21) != 0 &&
			host_readbs(Real2Host(hero) + 0x87) != ds_readbs(CURRENT_GROUP) &&
			hero_busy(Real2Host(hero)) &&
			host_readbs(Real2Host(hero) + 0x9c) == ds_readws(TYPEINDEX))
		{
			return hero;
		}
	}

	return (RealPt)NULL;
}

void TLK_herberg(signed short state)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif
