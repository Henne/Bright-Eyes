/*
 *      Rewrite of DSA1 v3.02_de functions of seg103 (talents)
 *      Functions rewritten 3/8
 *
*/

#include "schick.h"

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"

namespace M302de {

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
	hero_i = ds_readd(HEROS);

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
		/* add boni and current attributes together */
		ax = host_readb(hero + 0x38) + host_readb(hero + 0x39);
		ax += host_readb(hero + 0x41) + host_readb(hero + 0x42);
		ax += host_readb(hero + 0x47) + host_readb(hero + 0x48);
		ax = ax / 4;

		/* add skill value */
		ax += host_readb(hero + 0x108 + skill);

		ax -= bonus;

		tmp = random_schick(20);

		/* Unlucky */
		if (tmp == 20) {
			D1_INFO("Ungluecklich\n");
			return -1;
		}
		/* Lucky */
		if (tmp == 1) {
			D1_INFO("Gluecklich\n");
			return 99;
		}
		/* test unsuccessful */
		if (tmp > ax) {
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

/**
 *	bargain -	does a bargain throw
 *	@hero:		the hero who bargain
 *	@items:		the number of different goods
 *	@price:		the total price
 *	@percent:	how many percent the player wants to get
 *	@mod_init:	initial value for the modificator
 *
 *	Returns the result of the throw. A value greater than zero
 *	means success, below or zero means failed.
 */
signed short bargain(Bit8u *hero, unsigned short items, signed int price,
	signed short percent, signed char mod_init) {

	signed char mod;

	mod = mod_init;

	/* maybe a special NPC ? */
	if (host_readb(get_hero(7) + 0x89) == 2)
		mod -= 2;

	/* the more different items you buy, the easier the bargain */
	if (items == 1)
		mod += 2;
	else if (items == 2)
		mod += 1;
	else if (items < 5)
		mod += 0;
	else if (items < 9)
		mod -= 1;
	else mod -= 2;

	/* the higher the price, the easier the bargain */
	if (price < 100)
		mod += 2;
	if (price < 500)
		mod += 1;
	if (price < 1000)
		mod += 0;
	if (price < 2000)
		mod -= 1;
	else
		mod -= 2;

	/* the lower the percent, the easier the bargain */
	mod += percent / 5 + 1;

	return test_skill(hero, 0x15, mod);
}

}
