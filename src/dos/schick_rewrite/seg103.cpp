#include "schick.h"

#include "seg002.h"
#include "seg007.h"

/**
	get_proper_hero - returns hero which seems best for a skill
	@skill:		skill
*/
RealPt get_proper_hero(unsigned short skill) {
	RealPt hero_i, retval = 0;
	signed short best;
	unsigned short i;
	signed short tmp, dx;

	best = -1;
	hero_i = ds_readd(0xbd34);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* Check class */
		if (mem_readb(Real2Phys(hero_i) + 0x21) == 0)
			continue;
		/* Check if in current group */
		if (mem_readb(Real2Phys(hero_i) + 0x87) != ds_readb(0x2d35))
			continue;
		/* Check hero is dead */
		if (mem_readb(Real2Phys(hero_i) + 0xaa) & 1)
			continue;
		/* add current and maximum attibute values */
		tmp = ds_readb(0xffe + skill * 4) * 3;
		dx = mem_readb(Real2Phys(hero_i) + 0x35 + tmp);
		dx += mem_readb(Real2Phys(hero_i) + 0x36 + tmp);
		tmp = ds_readb(0xfff + skill * 4) * 3;
		dx += mem_readb(Real2Phys(hero_i) + 0x35 + tmp);
		dx += mem_readb(Real2Phys(hero_i) + 0x36 + tmp);
		tmp = ds_readb(0x1000 + skill * 4) * 3;
		dx += mem_readb(Real2Phys(hero_i) + 0x35 + tmp);
		dx += mem_readb(Real2Phys(hero_i) + 0x36 + tmp);

		/* add skillvalue */
		dx += mem_readb(Real2Phys(hero_i + 0x108 + skill));

		if (dx <= best)
			continue;

		/* if the sum is greater than the current best */
		best = dx;
		retval = hero_i;
	}

	/* sanity check for Original Bug hunting */
	if (retval == 0)
		D1_ERR("Original-Bug: %s undefinierter Rückgabewert\n",
			__func__);

	return retval;
}

/**
	test_skill - performs a skill test
	@hero:	hero which should be tested
	@skill:	the skill to test
	@bonus: the modification
*/
signed short test_skill(Bit8u *hero, unsigned short skill, signed char bonus) {
	unsigned short tmp;
	signed short ax;

	/* dont test for weapon skills */
	if (skill < 7)
		return 0;
	/* check if skill number is valid */
	if (skill > 51)
		return 0;

	D1_INFO("Talentprobe %s %+d: ", names_skill[skill], bonus);

	/* special test if skill is a range weapon skill */
	if ((skill == 7) || (skill == 8)) {
		/* add current and max attributes together */
		ax = host_readb(hero + 0x38);
		ax += host_readb(hero + 0x39);
		ax += host_readb(hero + 0x41);
		ax += host_readb(hero + 0x42);
		ax += host_readb(hero + 0x47);
		ax += host_readb(hero + 0x48);

		ax = ax / 4;

		/* add skill value */
		ax += host_readb(hero + 0x108 + skill);

		ax -= bonus;

		tmp = random_schick(20);

		/* Unlucky */
		if (tmp == 20) {
			D1_INFO("Unglücklich\n");
			return -1;
		}
		/* Lucky */
		if (tmp == 1) {
			D1_INFO("Glücklich\n");
			return 99;
		}
		/* test unsuccessful */
		if (ax > tmp) {
			D1_INFO(" (%d) -> nicht bestanden\n", tmp);
			return 0;
		}
		/* test successful */
		D1_INFO(" (%d) -> bestanden\n", tmp);
		return ax - tmp + 1;
	}

	/* automatically get hero with best senses in beginner mode */
	if (skill == 51 && ds_readb(0xc003) == 1)
		hero = MemBase + Real2Phys(get_proper_hero(51));

	/* do the test */
	return test_attrib3(hero, ds_readb(0xffe + skill * 4), ds_readb(0xfff + skill * 4), ds_readb(0x1000 + skill * 4), bonus - host_readb(hero + 0x108 + skill));

}

