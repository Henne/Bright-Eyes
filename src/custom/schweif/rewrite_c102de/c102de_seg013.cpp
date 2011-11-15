/*
 *	Rewritr of DSA2 c1.02_de functions seg013 (3d)
 *	Functions rewritten 1/8
 */

#include "paging.h"

#include "schweif.h"

namespace C102de {

void seg013_91d(signed short a, signed short b)
{
	signed short i;

	for (i = 0; i < 4; i++) {
		if (ds_readw(0x98fe + i * 4) != a)
			continue;
		if (ds_readw(0x98fe + 2 + i * 4) != b)
			continue;

		ds_writew(0x98fe + 2 + i * 4, 0xffff);
		ds_writew(0x98fe + i * 4, 0xffff);
	}
}

}
