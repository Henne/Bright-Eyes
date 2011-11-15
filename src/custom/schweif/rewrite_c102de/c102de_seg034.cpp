/*
 *	Rewritr of DSA2 c1.02_de functions seg034 (gfx_copy)
 *	Functions rewritten 2/8
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

void fill_v_line(PhysPt dst, unsigned short count, signed char color,
			unsigned short disp)
{
	while (count--) {
		mem_writeb_inline(dst, color);
		dst += disp;
	}
}

}
