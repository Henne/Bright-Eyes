/**
 *	Rewrite of DSA1 v3.02_de functions of seg067 (cityevents, waffinfo)
 *	Functions rewritten: 13/13 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg067.cpp
 */

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg031.h"
#include "seg055.h"
#include "seg067.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   select a random city event type
 */
void city_event_switch(void)
{
	/* load STRASSE.LTX */
	load_tx(ARCHIVE_FILE_STRASSE_LTX);

	/* set city flag */
	ds_writeb(C_EVENT_ACTIVE, 1);

	switch (random_schick(9)) {
		case 1: city_event_1(); break;
		case 2: city_event_2(); break;
		case 3: city_event_3(); break;
		case 4: city_event_4(); break;
		case 5: city_event_5(); break;
		case 6: city_event_6(); break;
		case 7: city_event_7(); break;
		case 8: city_event_8(); break;
		case 9: city_event_9(); break;
	}

	/* reset city flag */
	ds_writeb(C_EVENT_ACTIVE, 0);

	/* load the LTX-file of the current town */
	load_tx(ds_readbs(CURRENT_TOWN) + 77);

	/* update the current position / make the step */
	ds_writews(X_TARGET, ds_readws(X_TARGET_BAK));
	ds_writews(Y_TARGET, ds_readws(Y_TARGET_BAK));
}

/**
 * \brief   random city event 1: a hero may loose W20 S to a pickpocket
 */
void city_event_1(void)
{
	signed short randval;
	signed short answer;
	RealPt hero;

	hero = (RealPt)ds_readd(HEROES) + SIZEOF_HERO * get_random_hero();

	randval = random_schick(20);

	if (test_skill(Real2Host(hero), TA_SINNESSCHAERFE, 2) <= 0) {

		/* TODO:	check the format strings in the english version
		 *		they may be broken.
		 *		Or, at least some changes on the code are neccessary.
		 */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(random_schick(4) - 1),
			(char*)Real2Host(hero) + HERO_NAME2,
			randval);

		GUI_dialogbox(hero + HERO_PORTRAIT, Real2Host(hero) + HERO_NAME2, Real2Host(ds_readd(DTP2)), 0);

		randval *= 10;

		sub_ptr_ds(Real2Host(hero) + HERO_MONEY, randval);

		if (host_readds(Real2Host(hero) + HERO_MONEY) < 0) {
			host_writeds(Real2Host(hero) + HERO_MONEY, 0);
		}
	} else {

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(random_schick(4) + 3),
			(char*)Real2Host(hero) + HERO_NAME2,
			(char*)Real2Host(GUI_get_ptr(host_readbs(Real2Host(hero) + HERO_SEX), 1)));

		answer = GUI_dialogbox(hero + HERO_PORTRAIT, Real2Host(hero) + HERO_NAME2, Real2Host(ds_readd(DTP2)), 3,
				get_tx(random_schick(4) + 7),
				get_tx(random_schick(4) + 11),
				get_tx(random_schick(4) + 15));

		if (answer == 1) {
			GUI_output(get_tx(random_schick(4) + 19));
		} else {

			randval = random_schick(5) - 1;

			if (randval == 2) {
				GUI_output(get_tx(26));
				GUI_output(get_tx(27));
				GUI_output(get_tx(28));
			} else {
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(randval + 24),
					(char*)Real2Host(hero) + HERO_NAME2);
				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}
	}
}

/**
 * \brief   random city event 2: a hero may loose all money to a pickpocket
 */
void city_event_2(void)
{
	signed short answer;
	RealPt hero;

	hero = (RealPt)ds_readd(HEROES) + SIZEOF_HERO * get_random_hero();

	if (test_skill(Real2Host(hero), TA_SINNESSCHAERFE, 2) <= 0) {

		/* hero looses all money */
		host_writeds(Real2Host(hero) + HERO_MONEY, 0);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(random_schick(4) + 30),
			(char*)Real2Host(hero) + HERO_NAME2);

		GUI_dialogbox(hero + HERO_PORTRAIT, Real2Host(hero) + HERO_NAME2, Real2Host(ds_readd(DTP2)), 0);

	} else {

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(random_schick(4) + 34),
			(char*)Real2Host(hero) + HERO_NAME2);

		answer = GUI_dialogbox(hero + HERO_PORTRAIT, Real2Host(hero) + HERO_NAME2, Real2Host(ds_readd(DTP2)), 3,
				get_tx(random_schick(4) + 38),
				get_tx(random_schick(4) + 42),
				get_tx(random_schick(4) + 46));

		if (answer == 1) {
			GUI_output(get_tx(random_schick(4) + 50));
		} else {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(random_schick(4) + 54),
				(char*)Real2Host(hero) + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));
		}
	}
}

/**
 * \brief   random city event 3: a beggar asks for 1D
 */
void city_event_3(void)
{
	signed short answer;
	Bit32s money;

	load_in_head(43);

	money = get_party_money();

	answer = money >= 100 ? 3 : 2;

	answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx(random_schick(4) + 58), answer,
			get_tx(random_schick(4) + 62),
			get_tx(random_schick(4) + 66),
			get_tx(random_schick(4) + 70));

	if (answer == 3) {
		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx(random_schick(4) + 74), 0);
		money -= 100;
		set_party_money(money);
	}
}

/**
 * \brief   random city event 4: a beggar asks for 1D and tells some gossip
 */
void city_event_4(void)
{
	signed short answer;
	Bit32s money;

	load_in_head(43);

	money = get_party_money();

	answer = money >= 100 ? 3 : 2;

	answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx(random_schick(4) + 58), answer,
			get_tx(random_schick(4) + 78),
			get_tx(random_schick(4) + 82),
			get_tx(random_schick(4) + 86));

	if (answer == 3) {
		money -= 100;
		set_party_money(money);

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx(random_schick(4) + 90), 0);


		sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
			(char*)get_tx(random_schick(4) + 94),
			(char*)Real2Host(load_current_town_gossip()));

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, Real2Host(ds_readd(DTP2)) + 0x400, 0);
	}
}

/**
 * \brief   random city event 5: ??
 */
void city_event_5(void)
{
	signed short randval;
	signed short tw_bak;
	RealPt hero;

	load_in_head(48);

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writews(TEXTBOX_WIDTH, 5);

	randval = random_schick(4) - 1;

	if (randval == 1) {

		hero = get_first_hero_available_in_group();

		sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
			(char*)get_tx(randval + 99),
			(char*)Real2Host(hero) + HERO_NAME2);

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, Real2Host(ds_readd(DTP2)) + 0x400, 0);
	} else {
		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx(randval + 99), 0);
	}

	ds_writews(TEXTBOX_WIDTH, tw_bak);
}

/**
 * \brief   random city event 6: meet a merchant
 */
void city_event_6(void)
{
	signed short answer;
	signed short location_bak;
#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 24 */
	signed short type_bak;
#endif

	if (ds_readds(DAY_TIMER) >= HOURS(8) && ds_readds(DAY_TIMER) <= HOURS(20)) {

		load_in_head(4);

		sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
			(char*)get_tx(random_schick(4) + 102),
			(char*)get_hero(get_random_hero()) + HERO_NAME2);

		answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, Real2Host(ds_readd(DTP2)) + 0x400, 3,
			get_tx(random_schick(4) + 106),
			get_tx(random_schick(4) + 110),
			get_tx(random_schick(4) + 114));

		if (answer == 1 || answer == 2) {
			GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx(random_schick(4) + 118), 0);
		} else if (answer == 3) {
			location_bak = ds_readbs(LOCATION);
			ds_writeb(LOCATION, LOCATION_MERCHANT);
#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 24:
	 * When entering a building in Thorwal, Prem, Phexcaer or Oberorken between 8:00 and 20:00 o'clock, the street merchant (random city event) shows up with a chance 1:900. Selecting the third answer in the text box, the buy screen appears. After leaving the street merchant, the entered building is corrupted. For example, an entered temple will be a Praios temple (which otherwise does not exist in the game), or an entered tavern may offer negative food prices.
	 */
			type_bak = ds_readw(TYPEINDEX);
#endif
			ds_writew(TYPEINDEX, 93);
			do_merchant();
			ds_writeb(LOCATION, (unsigned char)location_bak);
#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 24 */
			ds_writew(TYPEINDEX, type_bak);
#endif
		}
	}
}

/**
 * \brief   random city event 7: some harmless events
 */
void city_event_7(void)
{
	signed short randval;
	RealPt hero;

	randval = random_schick(4) - 1;
	hero = (RealPt)ds_readd(HEROES) + SIZEOF_HERO * get_random_hero();

	if (!randval) {

		sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
			(char*)get_tx(123),
			(char*)Real2Host(hero) + HERO_NAME2);

		GUI_dialogbox(hero + HERO_PORTRAIT, Real2Host(hero) + HERO_NAME2, Real2Host(ds_readd(DTP2)) + 0x400, 0);

	} else if (randval == 1) {

		load_in_head(12);

		sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
			(char*)get_tx(124),
			(char*)Real2Host(hero) + HERO_NAME2);

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, Real2Host(ds_readd(DTP2)) + 0x400, 0);

		sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
			(char*)get_tx(125),
			(char*)Real2Host(GUI_get_ptr(host_readbs(Real2Host(hero) + HERO_SEX), 3)));

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, Real2Host(ds_readd(DTP2)) + 0x400, 0);

	} else if (randval == 2) {

		load_in_head(47);

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx(126), 0);

		sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
			(char*)get_tx(127),
			(char*)Real2Host(hero) + HERO_NAME2);

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, Real2Host(ds_readd(DTP2)) + 0x400, 0);

		sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
			(char*)get_tx(128),
			(char*)Real2Host(hero) + HERO_NAME2);

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, Real2Host(ds_readd(DTP2)) + 0x400, 0);
	} else {
		GUI_output(get_tx(129));
	}
}

/**
 * \brief   random city event 8: some harmless events
 */
void city_event_8(void)
{
	signed short randval;
	RealPt hero;

	randval = random_schick(4) - 1;
	hero = (RealPt)ds_readd(HEROES) + SIZEOF_HERO * get_random_hero();

	if (!randval) {

		GUI_output(get_tx(130));

	} else if (randval == 1) {

		load_in_head(12);

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx(131), 0);

		GUI_dialogbox((RealPt)ds_readd(DTP2), NULL, get_tx(132), 0);

	} else if (randval == 3) {
		GUI_output(get_tx(133));
	} else {
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(134),
			(char*)Real2Host(hero) + HERO_NAME2);

		GUI_output(Real2Host(ds_readd(DTP2)));
	}
}

/**
 * \brief   random city event 9: some harmless events
 */
void city_event_9(void)
{
	signed short randval = random_schick(4) - 1;

	if (!randval) {
		GUI_output(get_tx(135));
	} else if (randval == 1) {
		GUI_output(get_tx(136));
	} else if (randval == 2) {
		GUI_output(get_tx(137));
	} else {
		GUI_output(get_tx(138));
		GUI_output(get_tx(139));
	}
}

/**
 * \brief   return a pointer to a weapon related information
 */
RealPt waffinfo_weapons(void)
{
	signed short randval;
	Bit8u *ptr;

	load_ltx(ARCHIVE_FILE_WAFFINFO_LTX);

	ptr = Real2Host(ds_readd(BUFFER9_PTR3));

	randval = random_schick(19) - 1;

	return (RealPt)host_readd(ptr + 4 * randval);
}

/**
 * \brief   return a pointer to a herb related information
 */
RealPt waffinfo_herbs(void)
{
	signed short randval;
	Bit8u *ptr;

	load_ltx(ARCHIVE_FILE_WAFFINFO_LTX);

	ptr = Real2Host(ds_readd(BUFFER9_PTR3)) + 0x4c;

	randval = random_schick(40) - 1;

	return (RealPt)host_readd(ptr + 4 * randval);
}

/**
 * \brief   return a pointer to a general information
 */
RealPt waffinfo_general(void)
{
	signed short randval;
	Bit8u *ptr;

	load_ltx(ARCHIVE_FILE_WAFFINFO_LTX);

	ptr = Real2Host(ds_readd(BUFFER9_PTR3)) + 0xec;

	randval = random_schick(67) - 1;

	return (RealPt)host_readd(ptr + 4 * randval);
}


#if !defined(__BORLANDC__)
}
#endif
