/*
	Rewrite of DSA2 c1.02_de functions of seg136 (tests)
	Functions rewritten: 3/20
*/

#include "schweif.h"

#include "c102de_seg066.h"
#include "c102de_seg037.h"

namespace C102de {

/**
 *	test_attrib() - make an attribute test
 *
 *	A test is positive if the return value is greater than zero.
*/
signed short test_attrib(Bit8u* hero, unsigned short attrib, signed short bonus)
{

	signed short si;
	signed short tmp;

	si = random_schweif(20);

	D2_INFO("Eigenschaftsprobe %s auf %s %+d: W20 = %d",
		(char*)(hero + 0x22), schweif_common::names_attrib[attrib],
			bonus, si);

	if (si == 20) {
		D2_INFO("Ungluecklich\n");
		return -99;
	}

	si += bonus;

	tmp = host_readb(hero + 0x47 + attrib * 3);
	tmp += host_readb(hero + 0x48 + attrib * 3);
	tmp -= si + 1;

	D2_INFO(" -> %s mit %d\n",
		tmp > 0 ? "bestanden" : "nicht bestanden", tmp);

	return tmp;
}

signed short test_attrib3(Bit8u *hero, signed short a1, signed short a2, signed short a3, signed char taw)
{
	signed short throw_sum = 0;
	signed short rolls;
	signed short twens = 0;

	signed short attrib_sum;
	signed short this_throw;

	for (rolls = 0; rolls < 3; throw_sum += this_throw, rolls++) {

		this_throw = random_schweif(20);

		if (this_throw == 20) {

			twens++;

			if (twens == 2) {
				return -99;
			}
		}
	}

	throw_sum += taw;

	attrib_sum = host_readbs(hero + 0x47 + a1 * 3) +
			host_readbs(hero + 0x48 + a1 * 3) +
			host_readbs(hero + 0x47 + a2 * 3) +
			host_readbs(hero + 0x48 + a2 * 3) +
			host_readbs(hero + 0x47 + a3 * 3) +
			host_readbs(hero + 0x48 + a3 * 3);

	return attrib_sum - throw_sum + 1;
}

signed short test_skill(Bit8u *hero, signed short skill, signed char modifier)
{
	signed short this_throw;
	signed short l_di;

	if ((skill >= 7) && (skill <= 51)) {

		if ((skill == 7) || (skill == 8)) {
			/* Range Attack test */

			l_di = (host_readbs(hero + 0x4a) +
				host_readbs(hero + 0x4b) +
				host_readbs(hero + 0x53) +
				host_readbs(hero + 0x54) +
				host_readbs(hero + 0x59) +
				host_readbs(hero + 0x5a)) / 4;

			l_di += get_skill_value(hero, skill);
			l_di -= modifier;

			this_throw = random_schweif(20);

			if (this_throw == 20) {
				return -1;
			}

			if (this_throw == 1) {
				return 99;
			}

			if (this_throw <= l_di) {
				return l_di - this_throw + 1;
			}
		} else {
			/* Skill test */

			modifier -= get_skill_value(hero, skill);
			return test_attrib3(hero,
				(signed char)ds_readb(0x1af2 + 0 + skill * 3),
				(signed char)ds_readb(0x1af2 + 1 + skill * 3),
				(signed char)ds_readb(0x1af2 + 2 + skill * 3),
				modifier);
		}
	}

	return 0;
}

}
