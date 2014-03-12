/*
 *      Rewrite of DSA1 v3.02_de functions of seg103 (talents)
 *      Functions rewritten 3/8
 *
*/

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
	get_proper_hero - returns hero which seems best for a skill
	@skill:		skill
*/
RealPt get_proper_hero(unsigned short skill) {
	RealPt hero_i;
	RealPt retval = 0;
	signed short best;
	unsigned short i;
	signed short tmp, dx;

	best = -1;
	hero_i = (RealPt)ds_readd(HEROS);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* Check class */
		if (host_readb(Real2Host(hero_i) + 0x21) == 0)
			continue;
		/* Check if in current group */
		if (host_readb(Real2Host(hero_i) + 0x87) != ds_readb(CURRENT_GROUP))
			continue;
		/* Check hero is dead */
		if (hero_dead(Real2Host(hero_i)))
			continue;
		/* add current and maximum attibute values */
		tmp = ds_readb(0xffe + skill * 4) * 3;
		dx = host_readb(Real2Host(hero_i) + 0x35 + tmp);
		dx += host_readb(Real2Host(hero_i) + 0x36 + tmp);
		tmp = ds_readb(0xfff + skill * 4) * 3;
		dx += host_readb(Real2Host(hero_i) + 0x35 + tmp);
		dx += host_readb(Real2Host(hero_i) + 0x36 + tmp);
		tmp = ds_readb(0x1000 + skill * 4) * 3;
		dx += host_readb(Real2Host(hero_i) + 0x35 + tmp);
		dx += host_readb(Real2Host(hero_i) + 0x36 + tmp);

		/* add skillvalue */
		dx += host_readb(Real2Host(hero_i) + 0x108 + skill);

		if (dx <= best)
			continue;

		/* if the sum is greater than the current best */
		best = dx;
		retval = hero_i;
	}

#if !defined(__BORLANDC__)
	/* sanity check for Original Bug hunting */
	if (retval == 0)
		D1_ERR("Original-Bug: %s undefinierter Rückgabewert\n",
			__func__);
#endif

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

#if !defined(__BORLANDC__)
	D1_INFO("Talentprobe %s %+d: ", names_skill[skill], bonus);
#endif

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
#if !defined(__BORLANDC__)
			D1_INFO("Ungluecklich\n");
#endif
			return -1;
		}
		/* Lucky */
		if (tmp == 1) {
#if !defined(__BORLANDC__)
			D1_INFO("Gluecklich\n");
#endif
			return 99;
		}
		/* test unsuccessful */
		if (tmp > ax) {
#if !defined(__BORLANDC__)
			D1_INFO(" (%d) -> nicht bestanden\n", tmp);
#endif
			return 0;
		}
		/* test successful */
#if !defined(__BORLANDC__)
		D1_INFO(" (%d) -> bestanden\n", tmp);
#endif
		return ax - tmp + 1;
	}

	/* automatically get hero with best senses in beginner mode */
	if (skill == 51 && ds_readb(0xc003) == 1)
		hero = Real2Host(get_proper_hero(51));

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
	if (host_readb(get_hero(6) + 0x89) == 2)
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

#if !defined(__BORLANDC__)
}
#endif
