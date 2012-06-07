/**
 *	Rewrite of DSA1 v3.02_de functions of seg025 (locations)
 *	Functions rewritten: 1/15
 */

#include <string.h>

#include "schick.h"

#include "v302de.h"

namespace M302de {

/* 0x114a */
void copy_palette(void)
{
	memcpy(Real2Host(ds_readd(0xd303)) + 0xfa00, Real2Host(ds_readd(0xce3b)), 0x60);
	ds_writeb(0x4475, 2);
}

}
