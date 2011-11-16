/*
 *	Rewritr of DSA2 c1.02_de functions seg024 (???)
 *	Functions rewritten 0/12
 */

#include "paging.h"

#include "schweif.h"

namespace C102de {

void copy_3d_to_screen(void)
{
	Bit8u *src, *t_src;
	PhysPt dst, t_dst;
	signed short i, j;

	src = Real2Host(ds_readd(0xb128));
	dst = PhysMake(0xa000, 0x28f);

	i = 134;
	do {
		t_src = src;
		t_dst = dst;
		for (j = 52; j > 0; j--) {
			mem_writed_inline(t_dst, host_readd(t_src));
			t_dst+=4;
			t_src+=4;
		}
		src += 320;
		dst += 320;
		i--;
	} while (i != 0);
}


}
