/*
	Rewrite of DSA1 v3.02_de functions of seg045 (fight helper)
	Functions rewritten: 3/8
*/

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"

#include "seg006.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

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

	for (i = 0; host_readbs(src) != term; i = i + 3) {

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

/* 0x37c */
void FIG_remove_smth2(void)
{
	FIG_remove_from_list(ds_readb(0xe38c), 0);
	ds_writeb(0xe38c, 0xff);
}

#if !defined(__BORLANDC__)
}
#endif
