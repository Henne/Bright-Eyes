/*
 *	Rewrite of DSA1 v3.02_de functions of seg039 (fight)
 *	Functions rewritten 2/7
*/

#include <stdlib.h>

#include "schick.h"

#include "v302de.h"

/* is used at selecting a target */
signed short seg039_0000(unsigned short v1, unsigned short v2, unsigned short v3, unsigned short v4) {
	return abs(v1 - v3) + abs(v2 - v4);
}

signed short seg039_0023(Bit8u *hero) {
	Bit8u *ptr;
	signed short retval = -1;
	unsigned short weapon;

	/* get equipped weapon of the hero */
	weapon = host_readw(hero + 0x1c0);
	/* make a pointer to the entry of ITEMS.DAT */
	ptr = get_itemsdat(weapon);

	D1_LOG("weapon 0x%x +2 0x%x +3 0x%x\n", weapon,
		host_readb(ptr + 2), host_readb(ptr + 3));

	if (((host_readb(ptr + 2) >> 1) & 1) == 0)
		return retval;

	/* weapons are not MagicStaffs or Fightstaffs */
	if (host_readb(ptr + 3) == 5 && weapon != 0x85 && weapon != 0x45)
		return 5;

	if (host_readb(ptr + 3) == 7)
		return 3;

	if (host_readb(ptr + 3) == 8)
		return 4;

	return retval;
}
