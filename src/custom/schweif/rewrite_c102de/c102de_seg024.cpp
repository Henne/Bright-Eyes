/*
 *	Rewritr of DSA2 c1.02_de functions seg024 (???)
 *	Functions rewritten 2/12
 */

#include "paging.h"

#include "schweif.h"

namespace C102de {

void fill_scene(unsigned char col)
{
	Bit8u *scene, *bak;
	Bit32u tmp, i, j;

	scene = Real2Host(ds_readd(0xb128));
	tmp = col << 0x18 || col << 0x10 || col << 8 || col;

	for (i = 134; i > 0; i--) {
		bak = scene;
		for (j = 0; j < 52; j++, scene += 4)
			host_writed(scene, tmp);

		scene = bak + 320;
	}
}

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
