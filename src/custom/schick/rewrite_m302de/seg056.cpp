/**
 *	Rewrite of DSA1 v3.02_de functions of seg056 (merchant: buy)
 *	Functions rewritten: 1/3
 */

#include "v302de.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief	compare function for qsort()
 * \param p1_in first pointer
 * \param p2_in second pointer
 * \return -1 -> p1 < p2; 0 -> p1 == p2; 1 -> p1 > p1
 */
/* Borlandified and identical */
int shop_compar(const void *p1_in, const void *p2_in)
{
	Bit32s v1;
	Bit32s v2;
	Bit8u *p1;
	Bit8u *p2;

	p1 = (Bit8u*)p1_in;
	p2 = (Bit8u*)p2_in;

	v1 = host_readws(p1 + 2) * host_readws(p1 + 4);
	v2 = host_readws(p2 + 2) * host_readws(p2 + 4);

	return v1 < v2 ? -1 : (v1 == v2 ? 0 : 1);
}

#if !defined(__BORLANDC__)
}
#endif
