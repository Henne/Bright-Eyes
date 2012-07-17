/*
	Rewrite of DSA1 v3.02_de functions of seg045 (fight helper)
	Functions rewritten: 2/8
*/

#include "schick.h"
#include "v302de.h"

#include "seg006.h"

namespace M302de {

/* 0x137 */
void FIG_remove_smth(void)
{
	FIG_remove_from_list(ds_readb(0xe38d), 0);
	ds_writeb(0xe38d, 0xff);
}

/* 0x14f */
/**
 * FIG_copy_it() - copies chunks of 3 bytes
 * @dst:	pointer to destination
 * @src:	pointer to sources
 * @term:	termination sign
 */
signed short FIG_copy_it(Bit8u *dst, Bit8u *src, signed char term)
{
	signed short i;

	for (i = 0; (signed char)host_readb(src) != term; i = i + 3) {

		host_writeb(dst, host_readb(src));
		src++;
		dst++;

		host_writeb(dst, host_readb(src));
		src++;
		dst++;

		host_writeb(dst, host_readb(src));
		src++;
		dst++;
	}

	return i;
}

}

