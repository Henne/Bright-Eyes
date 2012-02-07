/*
        Rewrite of DSA1 v3.02_de functions of seg073 (tavern helper)
        Functions rewritten: 2/4
*/

#include "schick.h"
#include "v302de.h"

#include "seg007.h"

namespace M302de {

/**
 *	get_tavern_gossip() - get gossip in taverns
 *
 *	Returns index of the message.
 *	This sets also some Informers and camp grounds
*/
unsigned short get_tavern_gossip(void)
{
	/* Orig-Bug: Informants can be reenabled if they had been set to 2 */
	unsigned short r_si, r_di;

	r_di = ds_readb(0x7c9d + ds_readb(0x2d67));

	r_si = r_di = random_schick(r_di) - 1;

	switch (ds_readb(0x2d67) - 1) {
	/* Thorwal */
	case 0x00: {
		if (r_si == 6)
			ds_writeb(0x336c, 1);	/* Eliane Windenbeck */
		else if (r_si == 7)
			ds_writeb(0x336b, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Serske */
	case 0x01: {
		if (r_si == 1)
			ds_writeb(0x3da3, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(0x3da2, 1);
		else if (r_si == 9)
			ds_writeb(0x336c, 1);	/* Eliane Windenbeck */
		else if (r_si == 10)
			ds_writeb(0x336b, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Breida */
	case 0x02: {
		if (r_si == 1)
			ds_writeb(0x3da3, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(0x3da2, 1);
		else if (r_si == 4)
			ds_writeb(0x3da4, 1);
		else if (r_si == 5)
			ds_writeb(0x3da5, 1);

		break;
	}
	/* Peilinen */
	case 0x03: {
		if (r_si == 1)
			ds_writeb(0x3da5, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(0x3da6, 1);

		break;
	}
	/* Rovamund */
	case 0x04: {
		if (r_si == 1)
			ds_writeb(0x3da7, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(0x3da6, 1);
		else if (r_si == 4)
			ds_writeb(0x3dfe, 1);
		else if (r_si == 5)
			ds_writeb(0x3dff, 1);

		break;
	}
	/* Nordvest */
	case 0x05: {
		if (r_si == 1)
			ds_writeb(0x3da7, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(0x3daa, 1);

		break;
	}
	/* Kravik */
	case 0x06: {
		if (r_si == 1)
			ds_writeb(0x3daa, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(0x3dab, 1);
		else if (r_si == 3)
			ds_writeb(0x3dac, 1);

		break;
	}
	/* Skelellen */
	case 0x07: {
		if (r_si == 1)
			ds_writeb(0x3dac, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(0x3df0, 1);
		else if (r_si == 3)
			ds_writeb(0x3df0, 1);
		else if (r_si == 4) {
			ds_writeb(0x3dfa, 1);
			ds_writeb(0x3df9, 1);
		}

		break;
	}
	/* Merske */
	case 0x08: {
		if (r_si == 1)
			ds_writeb(0x3dfd, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(0x3dfe, 1);
		else if (r_si == 3)
			ds_writeb(0x3dff, 1);
		else if (r_si == 11)
			ds_writeb(0x336c, 1);	/* Eliane Windenbeck ? */
		else if (r_si == 12)
			ds_writeb(0x336b, 1);	/* Asgrimm Thurboldsson ? */

		break;
	}
	/* Tjoila */
	case 0x0a: {
		if (r_si == 2)
			ds_writeb(0x3da4, 1);	/* Restplaces */
		else if (r_si == 7)
			ds_writeb(0x336c, 1);	/* Eliane Windenbeck ? */
		else if (r_si == 8)
			ds_writeb(0x336b, 1);	/* Asgrimm Thurboldsson ? */

		break;
	}
	/* Rukian */
	case 0x0b: {
		if (r_si == 1)
			ds_writeb(0x3dad, 1);	/* Restplaces */
		break;
	}
	/* Auplog */
	case 0x0d: {
		if (r_si == 1) {
			ds_writeb(0x3db0, 1);	/* Restplaces */
			ds_writeb(0x3daf, 1);
		}
		else if (r_si == 2)
			ds_writeb(0x3dbc, 1);
		else if (r_si == 3) {
			ds_writeb(0x3dbc, 1);
			ds_writeb(0x3dd5, 1);
		}

		break;
	}
	/* Vilnheim */
	case 0x0e: {
		if (r_si == 1) {
			ds_writeb(0x3db0, 1);	/* Restplaces */
			ds_writeb(0x3daf, 1);
		}
		else if (r_si == 2)
			ds_writeb(0x3db1, 1);
		else if (r_si == 3) {
			ds_writeb(0x3db2, 1);
		}
		else if (r_si == 4) {
			ds_writeb(0x3db5, 1);
			ds_writeb(0x3db4, 1);
		}
		else if (r_si == 5) {
			ds_writeb(0x3db6, 1);
		}

		break;
	}
	/* Bodon */
	case 0x0f: {
		if (r_si == 3) {
			ds_writeb(0x3db0, 1);	/* Restplaces */
			ds_writeb(0x3daf, 1);
		}
		else if (r_si == 4)
			ds_writeb(0x3db1, 1);
		else if (r_si == 5) {
			ds_writeb(0x3db2, 1);
		}
		else if (r_si == 6) {
			ds_writeb(0x3db5, 1);
			ds_writeb(0x3db4, 1);
		}
		else if (r_si == 7) {
			ds_writeb(0x3db6, 1);
		}

		break;
	}
	/* Oberorken */
	case 0x10: {
		if (r_si == 2)
			ds_writeb(0x3ddd, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(0x3de7, 1);

		break;
	}
	/* Phexcaer */
	case 0x11: {
		if (r_si == 1)
			ds_writeb(0x3db6, 1);	/* Restplaces */
		else if (r_si == 2) {
			ds_writeb(0x3db5, 1);
			ds_writeb(0x3db4, 1);
		}
		else if (r_si == 3)
			ds_writeb(0x3db2, 1);
		else if (r_si == 4)
			ds_writeb(0x3db1, 1);
		else if (r_si == 5)
			ds_writeb(0x3db8, 1);
		else if (r_si == 6)
			ds_writeb(0x3df0, 1);
		else if (r_si == 7)
			ds_writeb(0x3dfc, 1);

		break;
	}
	/* Felsteyn */
	case 0x13: {
		if (r_si == 1)
			ds_writeb(0x3ddd, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(0x3de0, 1);
		else if (r_si == 3)
			ds_writeb(0x3ded, 1);
		else if (r_si == 4)
			ds_writeb(0x3dee, 1);

		break;
	}
	/* Orkanger */
	case 0x15: {
		if (r_si == 1)
			ds_writeb(0x3de0, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(0x3de3, 1);

		break;
	}
	/* Clanegh */
	case 0x16: {
		if (r_si == 1)
			ds_writeb(0x3de3, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(0x3de6, 1);

		break;
	}
	/* Thoss */
	case 0x18: {
		if (r_si == 1) {
			ds_writeb(0x3dcf, 1);	/* Restplaces */
			ds_writeb(0x3dce, 1);
			ds_writeb(0x3dcd, 1);
		}
		else if (r_si == 2)
			ds_writeb(0x3df1, 1);

		break;
	}
	/* Orvil */
	case 0x1a: {
		if (r_si == 1)
			ds_writeb(0x3dcb, 1);	/* Restplaces */
		else if (r_si == 2) {
			ds_writeb(0x3dcf, 1);
			ds_writeb(0x3dce, 1);
			ds_writeb(0x3dcd, 1);
		}

		break;
	}
	/* Overthorn */
	case 0x1b: {
		if (r_si == 1) {
			ds_writeb(0x3dd6, 1);	/* Restplaces */
			ds_writeb(0x3dc7, 1);
		}
		else if (r_si == 2)
			ds_writeb(0x3dcb, 1);
		else if (r_si == 3) {
			ds_writeb(0x3de1, 1);	/* Restplaces */
			ds_writeb(0x3de2, 1);
		}
		else if (r_si == 4)
			ds_writeb(0x3e13, 1);

		break;
	}
	/* Hjalsingor */
	case 0x1d: {
		if (r_si == 1)
			ds_writeb(0x3e13, 1);	/* Restplaces */

		break;
	}
	/* Kord */
	case 0x20: {
		if (r_si == 1)
			ds_writeb(0x3dd8, 1);	/* Restplaces */

		break;
	}
	/* Runinshaven */
	case 0x23: {
		if (r_si == 1) {
			ds_writeb(0x3e0b, 1);	/* Restplaces */
			ds_writeb(0x3e02, 1);
		}

		break;
	}

	/* Ottarje */
	case 0x24: {
		if (r_si == 1)
			ds_writeb(0x3dd7, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(0x3dc0, 1);
		else if (r_si == 3) {
			ds_writeb(0x3dc6, 1);
			ds_writeb(0x3dc2, 1);
		}
		else if (r_si == 4)
			ds_writeb(0x3dc3, 1);
		else if (r_si == 5) {
			ds_writeb(0x3dc7, 1);
			ds_writeb(0x3dc7, 1);	/* Yes this was done twice */
		}

		break;
	}
	/* Skjal */
	case 0x25: {
		if (r_si == 1) {
			ds_writeb(0x3dc6, 1);	/* Restplaces */
			ds_writeb(0x3dc2, 1);
		}
		else if (r_si == 2)
			ds_writeb(0x3dc3, 1);
		else if (r_si == 3)
			ds_writeb(0x3dd9, 1);
		else if (r_si == 4) {
			ds_writeb(0x3e12, 1);
			ds_writeb(0x3e11, 1);
		}

		break;
	}
	/* Prem */
	case 0x26: {
		if (r_si == 2)
			ds_writeb(0x3dd9, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(0x3dd8, 1);

		break;
	}
	/* Rybon */
	case 0x28: {
		if (r_si != 0)
			ds_writeb(0x3df1, 1);	/* Restplaces */

		break;
	}
	/* Varnheim */
	case 0x2a: {
		if (r_si == 2)
			ds_writeb(0x3dbc, 1);	/* Restplaces */
		else if (r_si == 3) {
			ds_writeb(0x3dbc, 1);	/* Restplace */
			ds_writeb(0x3dd5, 1);	/* Huntplace */
		}
		else if (r_si == 9)
			ds_writeb(0x336c, 1);	/* Eliane Windenbeck wt */
		else if (r_si == 10)
			ds_writeb(0x336b, 1);	/* Asgrimm Thurboldsson wt */

		break;
	}
	/* Vaermhag */
	case 0x2b: {
		if (r_si == 7)
			ds_writeb(0x336c, 1);	/* Eliane Windenbeck wt */
		else if (r_si == 8)
			ds_writeb(0x336b, 1);	/* Asgrimm Thurboldsson wt */

		break;
	}
	/* Tyldon */
	case 0x2c: {
		if (r_si == 1)
			ds_writeb(0x3de6, 1);	/* Restplaces */

		break;
	}
	/* Brendhil */
	case 0x2e: {
		if (r_si == 1)
			ds_writeb(0x3e14, 1);	/* Restplaces */

		break;
	}
	/* Manrin */
	case 0x2f: {
		if (r_si == 1)
			ds_writeb(0x3e14, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(0x3e07, 1);

		break;
	}
	}

	return r_di + ds_readb(0x7cd1 + ds_readb(0x2d67));

}

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
