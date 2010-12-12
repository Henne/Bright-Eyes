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
 * get_hero_KK_best - get the index of the hero with the best KK value
 *
 * Returns the index of the hero with the highest unmodified KK value.
 * The hero must be alive and in the current group.
 */
unsigned short get_hero_KK_best() {
	Bit8u *hero_i;
	unsigned short i, retval;
	signed short kk_val = -1;

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
		if (*(signed char*)(hero_i + 0x47) <= kk_val)
			continue;

		kk_val = *(signed char*)(hero_i + 0x47);
		retval = i;
	}

	return retval;
}

/**
 * hero_is_diseased - returns the disease number the hero has
 * @hero:	the hero which should be checked
 *
 * Only the first disease is returned here.
 */
unsigned short hero_is_diseased(Bit8u *hero) {
	unsigned short i;

	for (i = 0; i <= 7; i++)
		if (*(hero + 0xae + i * 5) == 0xff)
			return i;

	return 0;
}

/**
 * hero_is_poisoned - returns the poison number the hero has
 * @hero:	the hero which should be checked
 *
 * Only the first poison is returned here.
 */
unsigned short hero_is_poisoned(Bit8u *hero) {
	unsigned short i;

	for (i = 0; i <= 9; i++)
		if (*(hero + 0xd6 + i * 5) == 0xff)
			return i;

	return 0;
}

/**
 * hero_gets_poisoned - poison a hero
 * @hero:	the hero which gets poisoned
 * @poison:	the kind of poison
 */
void hero_gets_poisoned(Bit8u *hero, unsigned short poison) {

	if (*(hero + 0xaa) & 1)
		return;

	host_writeb(hero + poison * 5 + 0xd6, 0xff);
	host_writeb(hero + poison * 5 + 0xd7, 0x00);
	host_writeb(hero + poison * 5 + 0xd8, 0x00);
	host_writeb(hero + poison * 5 + 0xd9, 0x00);
	host_writeb(hero + poison * 5 + 0xda, 0x00);
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


