/*
 *	Rewritr of DSA2 c1.02_de functions seg034 (gfx_copy)
 *	Functions rewritten 0/8
 */

#include "paging.h"

#include "schweif.h"

namespace C102de {

void fill_line(PhysPt dst, unsigned short count, signed char color)
{
	while (count--) {
		mem_writeb_inline(dst++, color);
	}

}

}
