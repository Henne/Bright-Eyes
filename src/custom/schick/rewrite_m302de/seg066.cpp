/**
 *	Rewrite of DSA1 v3.02_de functions of seg025 (city)
 *	Functions rewritten: 1/18
 */

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short get_border_index(unsigned char val)
{
	signed short i;

	i = 0;
	while (ds_readb(0x7246 + i) < val) {
		i++;
	}

	ds_writew(0xe412, (((val & 3) + 4) - ds_readbs(DIRECTION)) & 3);

	if (i == 0)
		i = 1;

	i--;

	return i;

}

#if !defined(__BORLANDC__)
}
#endif

