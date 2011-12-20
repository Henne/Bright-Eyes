/*
	Rewrite of DSA1 v3.02_de functions of seg064 (harbour_helper)
	Functions rewritten: 1/6
*/
#include "schick.h"

#include "schick_common.h"
#include "v302de.h"

#include "seg007.h"
#include "seg028.h"

namespace M302de {

/**
 *	get_ship_name() - returns a pointer to the name of a ship
 *	@ship_type:	eg 0 = longship, 5= coastal ship,
 *				6= cutter,7= fishing boat
 *	@arg2:		???
 */
RealPt get_ship_name(signed char ship_type, signed short arg2)
{
	signed char done, i;
	signed short name;

	done = 0;

	do {
		name = ship_type * 10 + random_schick(10) + 0x29;
		done = 1;
		for (i = 0; i < arg2; i++) {
			if (host_readd((Real2Host(ds_readd(0x42b2)) + i * 12))
				!= host_readd(Real2Host(ds_readd(0xc3b1)) + name * 4))
				continue;
			done = 0;
			break;
		}

	} while (done == 0);

	return host_readd(Real2Host(ds_readd(0xc3b1)) + name * 4);
}

}
