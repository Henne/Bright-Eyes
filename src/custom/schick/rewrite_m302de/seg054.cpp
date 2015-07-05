/**
 *	Rewrite of DSA1 v3.02_de functions of seg054 (inn)
 *	Functions rewritten: 1/4
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

void TLK_herberg(signed short state)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif
