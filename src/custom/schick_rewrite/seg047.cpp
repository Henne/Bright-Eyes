#include "mem.h"

#include "schick.h"

/**
 * count_heroes_in_group - counts the heroes in the current group
 */
unsigned short count_heroes_in_group() {
	PhysPt hero_i;
	unsigned short i, retval = 0;

	hero_i = Real2Phys(ds_readd(0xbd34));

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* Check class */
		if (mem_readb(hero_i + 0x21) == 0)
			continue;
		/* Check if in current group */
		if (mem_readb(hero_i + 0x87) != ds_readb(0x2d35))
			continue;
                /* Check if hero is dead */
		if (mem_readb(hero_i + 0xaa) & 1)
			continue;

		retval++;
	}

	return retval;
}


