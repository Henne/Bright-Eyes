/*
	Rewrite of DSA1 v3.02_de functions of seg024 (diary)
	Functions rewritten: 1/3
*/

#include "schick.h"
#include "string.h"

namespace M302de {

void diary_new_entry()
{
	Bit8u *ptr;

	/* move all entries if the list is full */
	if (ds_readw(0x43ba) == 23) {
		memcpy(p_datseg + 0x43bc, p_datseg + 0x43c4, 176);
		ds_writew(0x43ba, ds_readw(0x43ba) - 1);
	}

	/* make a pointer to the last entry */
	ptr = MemBase + PhysMake(datseg, 0x43b4 + ds_readw(0x43ba) * 8);

	/* avoid double entries for the same town */
	if (ds_readb(0x2d67) == host_readw(ptr + 6))
		return;

	/* make a pointer to the current entry */
	ptr = MemBase + PhysMake(datseg, 0x43bc + ds_readw(0x43ba) * 8);

	/* increment entry counter */
	ds_writew(0x43ba, ds_readw(0x43ba) + 1);

	/* Write day of month */
	host_writew(ptr, ds_readb(0x2dc0));
	/* Write month */
	host_writew(ptr + 2, ds_readb(0x2dc1));
	/* Write year */
	host_writew(ptr + 4, ds_readb(0x2dc2));
	/* Write city */
	host_writew(ptr + 6, ds_readb(0x2d67));
}

}
