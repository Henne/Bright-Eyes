/*
        Rewrite of DSA1 v3.02_de functions of seg073 (tavern helper)
        Functions rewritten: 1/4
*/

#include "schick.h"
#include "v302de.h"

#include "seg007.h"

namespace M302de {

RealPt get_drinkmate(void)
{
	unsigned short name, surname;

	name = ((ds_readb(0x360d) - 1) == 0 ? 0xa7 : 0xbb);
	name += random_schick(20) - 1;

	surname = random_schick(2) - 1;

	surname = (surname == 0 ? 0xa7 : 0xbb);
	surname += random_schick(20) - 1;

	sprintf((char*)Real2Host(ds_readd(0xd2eb)),
		(char*)p_datseg + 0x7d06, /* "%s %s%s" */
		get_dtp(name * 4),
		get_dtp(surname * 4),
		get_dtp(((ds_readb(0x360d) - 1) == 0 ? 0xcf : 0xd0) * 4));

	return ds_readd(0xd2eb);
}

}
