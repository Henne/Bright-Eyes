#include "mem.h"

#include "schick.h"

/**
 * get_hero_CH_best - get the index of the hero with the best CH value
 *
 * Returns the index of the hero with the highest unmodified CH value.
 * The hero must be alive and in the current group.
 */
unsigned short get_hero_CH_best() {
	Bit8u *hero_i;
	unsigned short i, retval;
	signed short ch_val = -1;

	hero_i = MemBase + Real2Phys(ds_readd(0xbd34));

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* check class */
		if (*(hero_i + 0x21) == 0)
			continue;
		/* check if in group */
		if (*(hero_i + 0x87) != ds_readb(0x2d35))
			continue;
		/* check if dead */
		if (*(hero_i + 0xaa) & 1)
			continue;
		/* check if CH is the highest */
		if (*(signed char*)(hero_i + 0x3b) <= ch_val)
			continue;

		ch_val = *(signed char*)(hero_i + 0x3b);
		retval = i;
	}

	return retval;
}

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


