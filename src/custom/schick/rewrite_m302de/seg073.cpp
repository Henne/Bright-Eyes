/*
 *	Rewrite of DSA1 v3.02_de functions of seg073 (tavern: gossip, drinkmates, quests & informers)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg073.cpp
*/

#include <stdio.h>

#include "v302de.h"

#include "seg007.h"
#include "seg030.h"
#include "seg031.h"
#include "seg071.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief	get gossip in taverns
 *
 * \return	index of the gossip message
 *
 * This sets also some informers and camp grounds as known.
*/
unsigned short get_tavern_gossip(void)
{
	/* Orig-Bug: Informants can be reenabled if they had been set to 2 */
	unsigned short r_si;
	unsigned short r_di;

	r_di = ds_readb(MAX_GOSSIP + ds_readbs(CURRENT_TOWN));

	r_si = r_di = random_schick(r_di) - 1;

	switch (ds_readbs(CURRENT_TOWN) - 1) {
	/* Thorwal */
	case 0x00: {
		if (r_si == 6)
			ds_writeb_z(INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 7)
			ds_writeb_z(INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Serske */
	case 0x01: {
		if (r_si == 1)
			ds_writeb(0x3da3, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(0x3da2, 1);
		else if (r_si == 9)
			ds_writeb_z(INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 10)
			ds_writeb_z(INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

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
			ds_writeb(0x3df5, 1);
		else if (r_si == 4) {
			ds_writeb(0x3df9, ds_writeb(0x3dfa, 1));
		}

		break;
	}
	/* Merske (has no tavern) */
	case 0x08: {
		if (r_si == 1)
			ds_writeb(0x3dfd, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(0x3dfe, 1);
		else if (r_si == 3)
			ds_writeb(0x3dff, 1);
		else if (r_si == 11)
			ds_writeb_z(INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 12)
			ds_writeb_z(INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Tjoila */
	case 0x0a: {
		if (r_si == 2)
			ds_writeb(0x3da4, 1);	/* Restplaces */
		else if (r_si == 7)
			ds_writeb_z(INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 8)
			ds_writeb_z(INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Rukian and Angbodirtal */
	case 0x0b:
	case 0x0c:
	 {
		if (r_si == 1)
			ds_writeb(0x3dad, 1);	/* Restplaces */
		break;
	}
	/* Auplog */
	case 0x0d: {
		if (r_si == 1) {
			/* Restplaces */
			ds_writeb(0x3daf, ds_writeb(0x3db0, 1));
		}
		else if (r_si == 2)
			ds_writeb(0x3dbc, 1);
		else if (r_si == 3) {
			ds_writeb(0x3dd5, ds_writeb(0x3dbc, 1));
		}

		break;
	}
	/* Vilnheim */
	case 0x0e: {
		if (r_si == 1) {
			/* Restplaces */
			ds_writeb(0x3daf, ds_writeb(0x3db0, 1));
		}
		else if (r_si == 2)
			ds_writeb(0x3db1, 1);
		else if (r_si == 3) {
			ds_writeb(0x3db2, 1);
		}
		else if (r_si == 4) {
			ds_writeb(0x3db4, ds_writeb(0x3db5, 1));
		}
		else if (r_si == 5) {
			ds_writeb(0x3db6, 1);
		}

		break;
	}
	/* Bodon */
	case 0x0f: {
		if (r_si == 3) {
			/* Restplaces */
			ds_writeb(0x3daf, ds_writeb(0x3db0, 1));
		}
		else if (r_si == 4)
			ds_writeb(0x3db1, 1);
		else if (r_si == 5) {
			ds_writeb(0x3db2, 1);
		}
		else if (r_si == 6) {
			ds_writeb(0x3db4, ds_writeb(0x3db5, 1));
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
			ds_writeb(0x3db4, ds_writeb(0x3db5, 1));
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
			/* Restplaces */
			ds_writeb(0x3dcd, ds_writeb(0x3dce, ds_writeb(0x3dcf, 1)));
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
			ds_writeb(0x3dcd, ds_writeb(0x3dce, ds_writeb(0x3dcf, 1)));
		}

		break;
	}
	/* Overthorn */
	case 0x1b: {
		if (r_si == 1) {
			/* Restplaces */
			ds_writeb(0x3dc7, ds_writeb(0x3dd6, 1));
		}
		else if (r_si == 2)
			ds_writeb(0x3dcb, 1);
		else if (r_si == 3) {
			/* Restplaces */
			ds_writeb(0x3e11, ds_writeb(0x3e12, 1));
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
			/* Restplaces */
			ds_writeb(0x3e02, ds_writeb(0x3e0b, 1));
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
			ds_writeb(0x3dc2, ds_writeb(0x3dc6, 1));
		}
		else if (r_si == 4)
			ds_writeb(0x3dc3, 1);
		else if (r_si == 5) {
			/* Yes this was done twice */
			ds_writeb(0x3dc7, ds_writeb(0x3dc7, 1));
		}

		break;
	}
	/* Skjal */
	case 0x25: {
		if (r_si == 1) {
			/* Restplaces */
			ds_writeb(0x3dc2, ds_writeb(0x3dc6, 1));
		}
		else if (r_si == 2)
			ds_writeb(0x3dc3, 1);
		else if (r_si == 3)
			ds_writeb(0x3dd9, 1);
		else if (r_si == 4) {
			ds_writeb(0x3e11, ds_writeb(0x3e12, 1));
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
		if (r_si == 0)
			ds_writeb(0x3df1, 1);	/* Restplaces */

		break;
	}
	/* Varnheim */
	case 0x2a: {
		if (r_si == 2)
			ds_writeb(0x3dbc, 1);	/* Restplaces */
		else if (r_si == 3) {
			/* hunt and Restplace */
			ds_writeb(0x3dd5, ds_writeb(0x3dbc, 1));
		}
		else if (r_si == 9)
			ds_writeb_z(INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 10)
			ds_writeb_z(INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Vaermhag */
	case 0x2b: {
		if (r_si == 7)
			ds_writeb_z(INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 8)
			ds_writeb_z(INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

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

	return r_di + ds_readb(0x7cd1 + ds_readbs(CURRENT_TOWN));

}

/**
 * \brief generates the name of a drinkmate and copies it to a buffer
 */
RealPt get_drinkmate(void)
{
	signed short name;
	signed short surname;

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

	return (RealPt)ds_readd(0xd2eb);
}

/**
 * \brief	get quest information in taverns / meet informants
 *
 * \return	TODO {0, 1}
 */
signed short tavern_quest_infos(void)
{
	signed short l_si;

	if (ds_readws(HEARD_ANNOUNCE) == 0) {

		/* print the announcement from the hetman */
		GUI_output(get_dtp(0x348));

		/* remember you heard the announcement */
		ds_writews(HEARD_ANNOUNCE, 1);

		/* calculate the day in a week */
		/* not used in the game */
		/* TODO: Does this work with the nameless days ? */
		ds_writeb(ANNOUNCE_DAY, ds_readbs(DAY_OF_MONTH) + 7);

		if (ds_readbs(ANNOUNCE_DAY) > 30) {

			sub_ds_bs(ANNOUNCE_DAY, 30);
		}
		/* Original-Bug: which return value here? */

	} else if ((ds_readbs(CURRENT_TOWN) == 1) &&
			!(ds_readb(QUEST_DEADSHIP)) &&
			(ds_readws(TYPEINDEX) == 6))
	{
		/* print the message about a ghost ship */
		GUI_output(get_dtp(0x344));

		/* remember that */
		ds_writeb(QUEST_DEADSHIP, l_si = 1);

	} else if ((ds_readbs(CURRENT_TOWN) == 38) &&
			(ds_readws(TYPEINDEX) == 69) &&
			(ds_readb(INFORMER_JURGE) != 2) &&
			(ds_readb(INFORMER_JURGE) != 0) &&
			!(ds_readb(0x344c)))
	{
		/* meet Informer Jurge */

		if (random_schick(100) <= 30) {
			ds_writeb(CURRENT_INFORMER, l_si = 1);
		}

	} else if ((ds_readbs(CURRENT_TOWN) == 46) &&
			(ds_readws(TYPEINDEX) == 84) &&
			(ds_readb(INFORMER_RAGNA) != 2) &&
			(ds_readb(INFORMER_RAGNA) != 0))
	{
		/* meet Informer Ragna */

		if (random_schick(100) <= 30) {
			ds_writeb(CURRENT_INFORMER, l_si = 6);
		}

	} else if ((ds_readbs(CURRENT_TOWN) == 13) &&
			((ds_readws(TYPEINDEX) == 27) || (ds_readws(TYPEINDEX) == 28)) &&
			(ds_readb(INFORMER_BEORN) != 2) &&
			(ds_readb(INFORMER_BEORN) != 0))
	{
		/* meet Informer Beorn */

		if (((ds_readws(TYPEINDEX) == 27) && (random_schick(100) <= 50)) ||
			((ds_readws(TYPEINDEX) == 28) && (random_schick(100) <= 20)))
		{
			ds_writeb(CURRENT_INFORMER, l_si = 7);
		}


	} else if ((ds_readbs(CURRENT_TOWN) == 3) &&
			((ds_readws(TYPEINDEX) == 14) || (ds_readws(TYPEINDEX) == 15)) &&
			(ds_readb(INFORMER_ASGRIMM) != 2) &&
			(ds_readb(INFORMER_ASGRIMM) != 0) &&
			(ds_readws(GOT_MAIN_QUEST) != 0))
	{
		/* meet Informer Asgrimm */

		if (random_schick(100) <= 50) {
			ds_writeb(CURRENT_INFORMER, l_si = 8);
		}

	} else if ((ds_readbs(CURRENT_TOWN) == 31) &&
			((ds_readws(TYPEINDEX) == 61) || (ds_readws(TYPEINDEX) == 62)) &&
			(ds_readb(INFORMER_ALGRID) != 2) &&
			(ds_readb(INFORMER_ALGRID) != 0))
	{
		/* meet Informer Algrid */

		if (((ds_readws(TYPEINDEX) == 61) && (random_schick(100) <= 50)) ||
			((ds_readws(TYPEINDEX) == 62) && (random_schick(100) <= 20)))
		{
			ds_writeb(CURRENT_INFORMER, l_si = 14);
		}

	} else if ((ds_readbs(CURRENT_TOWN) == 18) && (ds_readb(ALRIK_DERONDAN))) {

		/* meet Alrik Derondan */
		PHX_alrik_derondan();
	}

	return l_si ? 1 : 0;
}

/**
 * \brief	follow a informer from the tavern to his/her home
 */
void tavern_follow_informer(void)
{

	switch (get_town_lookup_entry() + 1) {
	case 1: {
		do_talk(6, 0);
		ds_writew(0x2d83, 8);
		ds_writew(0x2d85, 6);
		ds_writeb(DIRECTION, 3);
		break;
	}
	case 6: {
		do_talk(8, 0);
		ds_writew(0x2d83, 5);
		ds_writew(0x2d85, 4);
		ds_writeb(DIRECTION, 3);
		break;
	}
	case 7: {
		do_talk(8, 1);
		ds_writew(0x2d83, 12);
		ds_writew(0x2d85, 10);
		ds_writeb(DIRECTION, 1);
		break;
	}
	case 8: {
		do_talk(10, 1);
		ds_writew(0x2d83, ds_writew(0x2d85, 7));
		ds_writeb(DIRECTION, 1);
		break;
	}
	case 14: {
		do_talk(8, 2);
		ds_writew(0x2d83, 5);
		ds_writew(0x2d85, 8);
		ds_writeb(DIRECTION, 0);
		break;
	}
	}
}

#if !defined(__BORLANDC__)
}
#endif
