/*
        Rewrite of DSA1 v3.02_de functions of seg098 (Magic)
        Functions rewritten: 4/11
*/
#include <stdlib.h>

#include "mem.h"

#include "schick.h"

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"

namespace M302de {

short seg098_3e() {
	Bit8u *ptr = Real2Host(ds_readd(0xe5bc));
	signed char tmp = host_readb(ptr + 0x86);

	if (tmp < 10) {
		RealPt hero = ds_readd(HEROS);
		Bit8u *hptr;
		short ax, dx;

		ds_writew(0xe5b4+2, RealSeg(hero));
		ds_writew(0xe5b4, RealOff(hero) + (tmp - 1) * 0x6da);
		hero = ds_readd(0xe5b4);
		hptr = Real2Host(hero);

		/* PA Wert ausrechnen */
		ax = (char)host_readb(hptr + (char)host_readb(hptr + 0x78) + 0x6f);
		dx = (char)host_readb(hptr + 0x79);
		ax -= dx;
		/* Rüstungsschutz Behinderung */
		dx = (char)host_readb(hptr + 0x32);
		dx = dx >= 0 ? dx / 2 : (dx + 1) / 2;
		ax -= dx;
		return ax;
	} else {
		ds_writew(0xe5b4+2, datseg);
		ds_writew(0xe5b4, tmp * 62 + 0xd0df);
		return (char)host_readb(ptr + 0x1d);
	}
}

/**
 *	get_spell_cost() - get the amount of AE-Points needed for a spell
 *	@spell:		numberof the spell
 *	@half cost:	the caster needs only half AE
*/
short get_spell_cost(unsigned short spell, unsigned short half_cost)
{

	signed char ret;

	ret = ds_readb(0x99d + 4 + spell * 10);

	if (half_cost != 0) {
		if (ret == -1)
			ret = (signed char)random_interval(5, 10);
		else
			ret = abs(ret) / 2;

		if (ret == 0)
			ret = 1;
	}
	return ret;
}

/**
	test_spell - makes a spell test
*/
short test_spell(Bit8u *hero, unsigned short spell, signed char bonus) {

	Bit8u *spell_desc;
	signed short retval;

	/* check if class is magic user */
	if (host_readb(hero+0x21) < 7)
		return 0;
	/* check hero */
	if (check_hero(hero) == 0)
		return 0;
	/* check if spell skill >= -5 */
	if (host_readb(hero + spell + 0x13d) < -5)
		return 0;
	/* check if hero has enough AE */
	if (get_spell_cost(spell, 0) > host_readw(hero+0x64))
		return -99;

	spell_desc = p_datseg + spell * 10 + 0x99d;

	if (host_readb(spell_desc+0x9) != 0) {

		unsigned short addr;

		if ((char)host_readb(hero+0x86) >= 10) {

			addr = (char)host_readb(hero+0x86) * 62 + 0xd0f8;
			bonus += ds_readb(addr);

			addr = (char)host_readb(hero+0x86) * 62 + 0xd110;
			if ((ds_readb(addr) >> 6) & 1)
				return 0;
		} else {
			addr = ((char)host_readb(hero+0x86) - 1) * 0x6da;
			bonus += host_readb(p_datseg + HEROS + addr + 0x66);

		}
	}

	if (spell < 1 || spell > 85)
		return 0;

	D1_INFO("Zauberprobe : %s %+d ", names_spell[spell], bonus);

	bonus -= (signed char)host_readb(hero + spell + 0x13d);

	retval = test_attrib3(hero, host_readb(spell_desc+1),
		host_readb(spell_desc+2), host_readb(spell_desc+3), bonus);

	if (retval == -99)
		return -1;

	return retval;
}

/**
	test_spell_group - makes a spell test for all magic users in the current group
*/
unsigned short test_spell_group(unsigned short spell, signed char bonus) {

	Bit8u *hero_i = get_hero(0);
	short i;

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* Check class is magicuser */
		if (host_readb(hero_i + 0x21) < 7)
			continue;
		/* Check class  BOGUS */
		if (host_readb(hero_i + 0x21) == 0)
			continue;
		/* Check in group */
		if (host_readb(hero_i + 0x87) != ds_readb(CURRENT_GROUP))
			continue;
		/* Check if dead */
		if (host_readb(hero_i + 0xaa) & 1)
			continue;

		if (test_spell(hero_i, spell, bonus) > 0)
			return 1;
	}
	return 0;
}

}
