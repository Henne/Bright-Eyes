/*
	Rewrite of DSA2 c1.02_de functions of seg136 (tests)
	Functions rewritten: 1/20 (complete)
*/

#include "schweif.h"

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

}
