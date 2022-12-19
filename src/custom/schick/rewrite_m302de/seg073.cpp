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
 * \brief   get gossip in taverns
 *
 *          This sets also some informers and camp grounds as known.
 *
 * \return              index of the gossip message
 */
unsigned short get_tavern_gossip(void)
{
	/* Orig-Bug: Informants can be reenabled if they had been set to 2 */
	unsigned short r_si;
	unsigned short r_di;

	r_di = ds_readb(TOWNS_GOSSIP_MAX + ds_readbs(CURRENT_TOWN));

	r_si = r_di = random_schick(r_di) - 1;

	switch (ds_readbs(CURRENT_TOWN) - 1) {
	/* Thorwal */
	case (TOWNS_THORWAL - 1): {
		if (r_si == 6)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 7)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Serske */
	case (TOWNS_SERSKE - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT005_FLAG, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(TEVENT004_FLAG, 1);
		else if (r_si == 9)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 10)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Breida */
	case (TOWNS_BREIDA - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT005_FLAG, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(TEVENT004_FLAG, 1);
		else if (r_si == 4)
			ds_writeb(TEVENT008_FLAG, 1);
		else if (r_si == 5)
			ds_writeb(TEVENT009_FLAG, 1);

		break;
	}
	/* Peilinen */
	case (TOWNS_PEILINEN - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT009_FLAG, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(TEVENT011_FLAG, 1);

		break;
	}
	/* Rovamund */
	case (TOWNS_ROVAMUND - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT013_FLAG, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(TEVENT011_FLAG, 1);
		else if (r_si == 4)
			ds_writeb(TEVENT119_FLAG, 1);
		else if (r_si == 5)
			ds_writeb(TEVENT120_FLAG, 1);

		break;
	}
	/* Nordvest */
	case (TOWNS_NORDVEST - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT013_FLAG, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(TEVENT017_FLAG, 1);

		break;
	}
	/* Kravik */
	case (TOWNS_KRAVIK - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT017_FLAG, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(TEVENT020_FLAG, 1);
		else if (r_si == 3)
			ds_writeb(TEVENT021_FLAG, 1);

		break;
	}
	/* Skelellen */
	case (TOWNS_SKELELLEN - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT021_FLAG, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(TEVENTU07_FLAG, 1);
		else if (r_si == 3)
			ds_writeb(TEVENT106_FLAG, 1);
		else if (r_si == 4) {
			ds_writeb(TEVENT112_FLAG, ds_writeb(TEVENT112_HERB_FLAG, 1));
		}

		break;
	}
	/* Merske (has no tavern) */
	case (TOWNS_MERSKE - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT118_FLAG, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(TEVENT119_FLAG, 1);
		else if (r_si == 3)
			ds_writeb(TEVENT120_FLAG, 1);
		else if (r_si == 11)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 12)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Tjoila */
	case (TOWNS_TJOILA - 1): {
		if (r_si == 2)
			ds_writeb(TEVENT008_FLAG, 1);	/* Restplaces */
		else if (r_si == 7)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 8)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Rukian and Angbodirtal */
	case (TOWNS_RUKIAN - 1):
	case (TOWNS_ANGBODIRTAL - 1):
	 {
		if (r_si == 1)
			ds_writeb(TEVENT022_FLAG, 1);	/* Restplaces */
		break;
	}
	/* Auplog */
	case (TOWNS_AUPLOG - 1): {
		if (r_si == 1) {
			/* Restplaces */
			ds_writeb(TEVENT024_FLAG, ds_writeb(TEVENT025_FLAG, 1));
		}
		else if (r_si == 2)
			ds_writeb(TEVENT040_FLAG, 1);
		else if (r_si == 3) {
			ds_writeb(TEVENTU02_FLAG, ds_writeb(TEVENT040_FLAG, 1));
		}

		break;
	}
	/* Vilnheim */
	case (TOWNS_VILNHEIM - 1): {
		if (r_si == 1) {
			/* Restplaces */
			ds_writeb(TEVENT024_FLAG, ds_writeb(TEVENT025_FLAG, 1));
		}
		else if (r_si == 2)
			ds_writeb(TEVENT028_FLAG, 1);
		else if (r_si == 3) {
			ds_writeb(TEVENT030_FLAG, 1);
		}
		else if (r_si == 4) {
			ds_writeb(TEVENT032_FLAG, ds_writeb(TEVENT032_HERB_FLAG, 1));
		}
		else if (r_si == 5) {
			ds_writeb(TEVENT034_FLAG, 1);
		}

		break;
	}
	/* Bodon */
	case (TOWNS_BODON - 1): {
		if (r_si == 3) {
			/* Restplaces */
			ds_writeb(TEVENT024_FLAG, ds_writeb(TEVENT025_FLAG, 1));
		}
		else if (r_si == 4)
			ds_writeb(TEVENT028_FLAG, 1);
		else if (r_si == 5) {
			ds_writeb(TEVENT030_FLAG, 1);
		}
		else if (r_si == 6) {
			ds_writeb(TEVENT032_FLAG, ds_writeb(TEVENT032_HERB_FLAG, 1));
		}
		else if (r_si == 7) {
			ds_writeb(TEVENT034_FLAG, 1);
		}

		break;
	}
	/* Oberorken */
	case (TOWNS_OBERORKEN - 1): {
		if (r_si == 2)
			ds_writeb(TEVENT072_FLAG, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(TEVENT083_FLAG, 1);

		break;
	}
	/* Phexcaer */
	case (TOWNS_PHEXCAER - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT034_FLAG, 1);	/* Restplaces */
		else if (r_si == 2) {
			ds_writeb(TEVENT032_FLAG, ds_writeb(TEVENT032_HERB_FLAG, 1));
		}
		else if (r_si == 3)
			ds_writeb(TEVENT030_FLAG, 1);
		else if (r_si == 4)
			ds_writeb(TEVENT028_FLAG, 1);
		else if (r_si == 5)
			ds_writeb(TEVENT036_FLAG, 1);
		else if (r_si == 6)
			ds_writeb(TEVENTU07_FLAG, 1);
		else if (r_si == 7)
			ds_writeb(TEVENT116_FLAG, 1);

		break;
	}
	/* Felsteyn */
	case (TOWNS_FELSTEYN - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT072_FLAG, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(TEVENT075_FLAG, 1);
		else if (r_si == 3)
			ds_writeb(TEVENT091_FLAG, 1);
		else if (r_si == 4)
			ds_writeb(TEVENT093_FLAG, 1);

		break;
	}
	/* Orkanger */
	case (TOWNS_ORKANGER - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT075_FLAG, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(TEVENT079_FLAG, 1);

		break;
	}
	/* Clanegh */
	case (TOWNS_CLANEGH - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT079_FLAG, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(TEVENT081_FLAG, 1);

		break;
	}
	/* Thoss */
	case (TOWNS_THOSS - 1): {
		if (r_si == 1) {
			/* Restplaces */
			ds_writeb(TEVENT069_FLAG, ds_writeb(TEVENT070_FLAG, ds_writeb(TEVENT070_HERB_FLAG, 1)));
		}
		else if (r_si == 2)
			ds_writeb(TEVENTU08_FLAG, 1);

		break;
	}
	/* Ala */
	case (TOWNS_ALA - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT066_TRACK_FLAG, 1);	/* Restplaces */
		else if (r_si == 2) {
			ds_writeb(TEVENT069_FLAG, ds_writeb(TEVENT070_FLAG, ds_writeb(TEVENT070_HERB_FLAG, 1)));
		}

		break;
	}
	/* Orvil */
	case (TOWNS_ORVIL - 1): {
		if (r_si == 1) {
			/* Restplaces */
			ds_writeb(TEVENT058_FLAG, ds_writeb(TEVENTU03_FLAG, 1));
		}
		else if (r_si == 2)
			ds_writeb(TEVENT066_TRACK_FLAG, 1);
		else if (r_si == 3) {
			/* Restplaces */
			ds_writeb(TEVENTU10_FLAG, ds_writeb(TEVENTU11_FLAG, 1));
		}
		else if (r_si == 4)
			ds_writeb(TEVENTU12_FLAG, 1);

		break;
	}
	/* Rovik */
	case (TOWNS_ROVIK - 1): {
		if (r_si == 1)
			ds_writeb(TEVENTU12_FLAG, 1);	/* Restplaces */

		break;
	}
	/* Kord */
	case (TOWNS_KORD - 1): {
		if (r_si == 1)
			ds_writeb(TEVENTU05_FLAG, 1);	/* Restplaces */

		break;
	}
	/* Runinshaven */
	case (TOWNS_RUNINSHAVEN - 1): {
		if (r_si == 1) {
			/* Restplaces */
			ds_writeb(TEVENT128_FLAG, ds_writeb(TEVENTU09_FLAG, 1));
		}

		break;
	}

	/* Ottarje */
	case (TOWNS_OTTARJE - 1): {
		if (r_si == 1)
			ds_writeb(TEVENTU04_FLAG, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(TEVENT048_FLAG, 1);
		else if (r_si == 3) {
			ds_writeb(TEVENT049_FLAG, ds_writeb(TEVENT053_FLAG, 1));
		}
		else if (r_si == 4)
			ds_writeb(TEVENT050_FLAG, 1);
		else if (r_si == 5) {
			/* Yes this was done twice */
			ds_writeb(TEVENT058_FLAG, ds_writeb(TEVENT058_FLAG, 1));
		}

		break;
	}
	/* Skjal */
	case (TOWNS_SKJAL - 1): {
		if (r_si == 1) {
			/* Restplaces */
			ds_writeb(TEVENT049_FLAG, ds_writeb(TEVENT053_FLAG, 1));
		}
		else if (r_si == 2)
			ds_writeb(TEVENT050_FLAG, 1);
		else if (r_si == 3)
			ds_writeb(TEVENTU06_FLAG, 1);
		else if (r_si == 4) {
			ds_writeb(TEVENTU10_FLAG, ds_writeb(TEVENTU11_FLAG, 1));
		}

		break;
	}
	/* Prem */
	case (TOWNS_PREM - 1): {
		if (r_si == 2)
			ds_writeb(TEVENTU06_FLAG, 1);	/* Restplaces */
		else if (r_si == 3)
			ds_writeb(TEVENTU05_FLAG, 1);

		break;
	}
	/* Rybon */
	case (TOWNS_RYBON - 1): {
		if (r_si == 0)
			ds_writeb(TEVENTU08_FLAG, 1);	/* Restplaces */

		break;
	}
	/* Varnheim */
	case (TOWNS_VARNHEIM - 1): {
		if (r_si == 2)
			ds_writeb(TEVENT040_FLAG, 1);	/* Restplaces */
		else if (r_si == 3) {
			/* hunt and Restplace */
			ds_writeb(TEVENTU02_FLAG, ds_writeb(TEVENT040_FLAG, 1));
		}
		else if (r_si == 9)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 10)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Vaermhag */
	case (TOWNS_VAERMHAG - 1): {
		if (r_si == 7)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ELIANE, 1);	/* Eliane Windenbeck */
		else if (r_si == 8)
			ds_writeb_z(INFORMER_FLAGS + INFORMER_ASGRIMM, 1);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Tyldon */
	case (TOWNS_TYLDON - 1): {
		if (r_si == 1)
			ds_writeb(TEVENT081_FLAG, 1);	/* Restplaces */

		break;
	}
	/* Brendhil */
	case (TOWNS_BRENDHIL - 1): {
		if (r_si == 1)
			ds_writeb(TEVENTU13_FLAG, 1);	/* Restplaces */

		break;
	}
	/* Manrin */
	case (TOWNS_MANRIN - 1): {
		if (r_si == 1)
			ds_writeb(TEVENTU13_FLAG, 1);	/* Restplaces */
		else if (r_si == 2)
			ds_writeb(TEVENT132_HERB_FLAG, 1);

		break;
	}
	}

	return r_di + ds_readb(TOWNS_GOSSIP_OFF + ds_readbs(CURRENT_TOWN));

}

/**
 * \brief   generates the name of a drinkmate and copies it to a buffer
 */
RealPt get_drinkmate(void)
{
	signed short name;
	signed short surname;

	name = ((ds_readb(TLK_TAV_INFORMERSEX) - 1) == 0 ? 0xa7 : 0xbb);
	name += random_schick(20) - 1;

	surname = random_schick(2) - 1;

	surname = (surname == 0 ? 0xa7 : 0xbb);
	surname += random_schick(20) - 1;

	sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
		(char*)p_datseg + GOSSIP_STR_TRIPLE_WILDCARD, /* "%s %s%s" */
		get_tx(name),
		get_tx(surname),
		get_tx((ds_readb(TLK_TAV_INFORMERSEX) - 1) == 0 ? 207 : 208));

	return (RealPt)ds_readd(TEXT_OUTPUT_BUF);
}

/**
 * \brief   get quest information in taverns / meet informants
 *
 * \return              TODO {0, 1}
 */
signed short tavern_quest_infos(void)
{
	signed short l_si;

	if (ds_readws(HEARD_ANNOUNCE) == 0) {

		/* print the announcement from the hetman */
		GUI_output(get_tx(210));

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

	} else if ((ds_readbs(CURRENT_TOWN) == TOWNS_THORWAL) &&
			!(ds_readb(QUEST_DEADSHIP)) &&
			(ds_readws(TYPEINDEX) == 6))
	{
		/* print the message about a ghost ship */
		GUI_output(get_tx(209));

		/* remember that */
		ds_writeb(QUEST_DEADSHIP, (unsigned char)(l_si = 1));

	} else if ((ds_readbs(CURRENT_TOWN) == TOWNS_SKJAL) &&
			(ds_readws(TYPEINDEX) == 69) &&
			(ds_readb(INFORMER_FLAGS + INFORMER_JURGE) != 2) &&
			(ds_readb(INFORMER_FLAGS + INFORMER_JURGE) != 0) &&
			!(ds_readb(JURGE_AWAITS_LETTER)))
	{
		/* meet Informer Jurge */

		if (random_schick(100) <= 30) {
			ds_writeb(CURRENT_INFORMER, (unsigned char)(l_si = 1));
		}

	} else if ((ds_readbs(CURRENT_TOWN) == TOWNS_VIDSAND) &&
			(ds_readws(TYPEINDEX) == 84) &&
			(ds_readb(INFORMER_FLAGS + INFORMER_RAGNA) != 2) &&
			(ds_readb(INFORMER_FLAGS + INFORMER_RAGNA) != 0))
	{
		/* meet Informer Ragna */

		if (random_schick(100) <= 30) {
			ds_writeb(CURRENT_INFORMER, (unsigned char)(l_si = 6));
		}

	} else if ((ds_readbs(CURRENT_TOWN) == TOWNS_ANGBODIRTAL) &&
			((ds_readws(TYPEINDEX) == 27) || (ds_readws(TYPEINDEX) == 28)) &&
			(ds_readb(INFORMER_FLAGS + INFORMER_BEORN) != 2) &&
			(ds_readb(INFORMER_FLAGS + INFORMER_BEORN) != 0))
	{
		/* meet Informer Beorn */

		if (((ds_readws(TYPEINDEX) == 27) && (random_schick(100) <= 50)) ||
			((ds_readws(TYPEINDEX) == 28) && (random_schick(100) <= 20)))
		{
			ds_writeb(CURRENT_INFORMER, (unsigned char)(l_si = 7));
		}


	} else if ((ds_readbs(CURRENT_TOWN) == TOWNS_BREIDA) &&
			((ds_readws(TYPEINDEX) == 14) || (ds_readws(TYPEINDEX) == 15)) &&
			(ds_readb(INFORMER_FLAGS + INFORMER_ASGRIMM) != 2) &&
			(ds_readb(INFORMER_FLAGS + INFORMER_ASGRIMM) != 0) &&
			(ds_readws(GOT_MAIN_QUEST) != 0))
	{
		/* meet Informer Asgrimm */

		if (random_schick(100) <= 50) {
			ds_writeb(CURRENT_INFORMER, (unsigned char)(l_si = 8));
		}

	} else if ((ds_readbs(CURRENT_TOWN) == TOWNS_HJALSINGOR) &&
			((ds_readws(TYPEINDEX) == 61) || (ds_readws(TYPEINDEX) == 62)) &&
			(ds_readb(INFORMER_FLAGS + INFORMER_ALGRID) != 2) &&
			(ds_readb(INFORMER_FLAGS + INFORMER_ALGRID) != 0))
	{
		/* meet Informer Algrid */

		if (((ds_readws(TYPEINDEX) == 61) && (random_schick(100) <= 50)) ||
			((ds_readws(TYPEINDEX) == 62) && (random_schick(100) <= 20)))
		{
			ds_writeb(CURRENT_INFORMER, (unsigned char)(l_si = 14));
		}

	} else if ((ds_readbs(CURRENT_TOWN) == TOWNS_PHEXCAER) && (ds_readb(ALRIK_DERONDAN))) {

		/* meet Alrik Derondan */
		PHX_alrik_derondan();
	}

	return l_si ? 1 : 0;
}

/**
 * \brief   follow a informer from the tavern to his/her home
 */
void tavern_follow_informer(void)
{

	switch (get_town_lookup_entry() + 1) {
	case 1: {
		do_talk(6, 0);
		ds_writew(X_TARGET_BAK, 8);
		ds_writew(Y_TARGET_BAK, 6);
		ds_writeb(DIRECTION, 3);
		break;
	}
	case 6: {
		do_talk(8, 0);
		ds_writew(X_TARGET_BAK, 5);
		ds_writew(Y_TARGET_BAK, 4);
		ds_writeb(DIRECTION, 3);
		break;
	}
	case 7: {
		do_talk(8, 1);
		ds_writew(X_TARGET_BAK, 12);
		ds_writew(Y_TARGET_BAK, 10);
		ds_writeb(DIRECTION, 1);
		break;
	}
	case 8: {
		do_talk(10, 1);
		ds_writew(X_TARGET_BAK, ds_writew(Y_TARGET_BAK, 7));
		ds_writeb(DIRECTION, 1);
		break;
	}
	case 14: {
		do_talk(8, 2);
		ds_writew(X_TARGET_BAK, 5);
		ds_writew(Y_TARGET_BAK, 8);
		ds_writeb(DIRECTION, 0);
		break;
	}
	}
}

#if !defined(__BORLANDC__)
}
#endif
