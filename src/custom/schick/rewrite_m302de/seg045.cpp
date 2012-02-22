/*
	Rewrite of DSA1 v3.02_de functions of seg045 (fight helper)
	Functions rewritten: 1/8
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

}

