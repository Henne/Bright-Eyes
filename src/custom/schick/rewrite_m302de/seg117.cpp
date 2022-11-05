/**
 *	Rewrite of DSA1 v3.02_de functions of seg117 (travel events 9 / 10, hunt and helpers)
 *	Functions rewritten: 16/16 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg117.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg030.h"
#include "seg032.h"
#include "seg047.h"
#include "seg048.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#ifdef M302de_ORIGINAL_BUGFIX
/* Original-Bug 22: see below */
#include "seg113.h"
#endif

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
static
#endif
void pause_traveling(signed short ani_no)
{

	ds_writeb(EVENT_ANI_BUSY, 1);

	load_ani(ani_no);

	draw_main_screen();

	init_ani(0);

	load_tx2(ARCHIVE_FILE_WILD_LTX);

	ds_writew(BASEPOS_X_BAK, ds_readw(BASEPOS_X));
	ds_writew(BASEPOS_Y_BAK, ds_readw(BASEPOS_Y));
	ds_writew(TEXTBOX_WIDTH_BAK, ds_readw(TEXTBOX_WIDTH));
	ds_writew(WALLCLOCK_UPDATE_BAK, ds_readw(WALLCLOCK_UPDATE));

	ds_writeb(TRAVEL_EVENT_ACTIVE, 1);

	/* c = b = a = 0 */
	ds_writeb(SHOW_TRAVEL_MAP, (unsigned char)ds_writew(BASEPOS_X, ds_writew(WALLCLOCK_UPDATE, 0)));

	ds_writew(BASEPOS_Y, ani_no == 21 ? 60: 70);
	ds_writew(TEXTBOX_WIDTH, 9);
}

#if defined(__BORLANDC__)
static
#endif
void resume_traveling(void)
{
	ds_writew(BASEPOS_X, ds_readw(BASEPOS_X_BAK));
	ds_writew(BASEPOS_Y, ds_readw(BASEPOS_Y_BAK));
	ds_writew(TEXTBOX_WIDTH, ds_readw(TEXTBOX_WIDTH_BAK));
	ds_writew(WALLCLOCK_UPDATE, ds_readw(WALLCLOCK_UPDATE_BAK));

	set_var_to_zero();

	ds_writew(REQUEST_REFRESH, ds_writeb(SHOW_TRAVEL_MAP, 1));

	ds_writeb(EVENT_ANI_BUSY, 0);
	ds_writeb(TRAVEL_EVENT_ACTIVE, 0);
}

void hunt_karen(void)
{
	signed short answer;
	Bit8u *hero;
	signed short passed;
	signed short i;

	pause_traveling(21);

	do {
		answer = GUI_radio(get_tx2(0), 2, get_tx2(1), get_tx2(2));
	} while (answer == -1);

	if (answer == 1) {
		/* check for a hunting weapon, BOWS, CROSSBOWS or SPEAR */ /* TODO 2021-04-18: Original-Bug: What about magic spear? sling? (maybe not) */
		if ((get_first_hero_with_item(ITEM_SHORTBOW) != -1) ||
			(get_first_hero_with_item(ITEM_LONGBOW) != -1) ||
			(get_first_hero_with_item(ITEM_CROSSBOW) != -1) ||
			(get_first_hero_with_item(ITEM_SPEAR) != -1))
		{

			hero = get_hero(0);
			/* make a STEALTH+2 test and count the heroes who passed it */
			for (i = passed = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
					(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
					!hero_dead(hero) &&
					(test_skill(hero, TA_SCHLEICHEN, 2) > 0))
				{
					passed++;
				}
			}

			if (count_heroes_in_group() <= passed) {
				/* all heroes passed STEALTH */

				GUI_output(get_tx2(5));

				/* make a MISSLE WEAPON+0 test and count the heroes who passed it */ /* TODO 2021-04-18: Original-Bug: Why TA_SCHUSSWAFFEN for spears? */
				hero = get_hero(0);
				for (i = passed = 0; i <= 6; i++, hero += SIZEOF_HERO) {

					if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
						(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
						!hero_dead(hero) &&
						(test_skill(hero, TA_SCHUSSWAFFEN, 0) > 0))
					{
						passed++;
					}
				}

				if ((count_heroes_in_group() / 2) <= passed) {
					/* over the half of the group passed MISSLE WEAPON+0 */

					GUI_output(get_tx2(8));
					/* get 80 FOOD PACKAGES */
					get_item(ITEM_FOOD_PACKAGE, 1, 80);

				} else if (passed) {
					/* at least one of the group passed MISSLE WEAPON+0 */

					GUI_output(get_tx2(7));
					/* get 40 FOOD PACKAGES */
					get_item(ITEM_FOOD_PACKAGE, 1, 40);
				} else {
					/* everybody failed MISSLE WEAPON+0 */
					GUI_output(get_tx2(6));
				}
			} else {
				/* at least one hero failed STEALTH+2 */
				GUI_output(get_tx2(4));
			}
		} else {
			/* no hunting weapon in the group */
			GUI_output(get_tx2(9));
		}
	}

	resume_traveling();
}

void hunt_wildboar(void)
{
	signed short answer;
	Bit8u *hero;
	signed short i;
	signed short passed;

	pause_traveling(25);

	do {
		answer = GUI_radio(get_tx2(10), 2, get_tx2(11), get_tx2(12));
	} while (answer == -1);

	if (answer == 1) {
		/* check for a hunting weapon, BOWS, CROSSBOWS or SPEAR */ /* TODO 2021-04-18: Original-Bug: What about magic spear? sling? (maybe not) */
		if ((get_first_hero_with_item(ITEM_SHORTBOW) != -1) ||
			(get_first_hero_with_item(ITEM_LONGBOW) != -1) ||
			(get_first_hero_with_item(ITEM_CROSSBOW) != -1) ||
			(get_first_hero_with_item(ITEM_SPEAR) != -1))
		{

			hero = get_hero(0);
			/* make a STEALTH+0 test and count the heroes who passed it */
			for (i = passed = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
					(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
					!hero_dead(hero) &&
					(test_skill(hero, TA_SCHLEICHEN, 0) > 0))
				{
					passed++;
				}
			}

			if (count_heroes_in_group() <= passed) {
				/* all heroes passed STEALTH */

				GUI_output(get_tx2(15));

				/* make a MISSLE WEAPON+0 test and count the heroes who passed it */ /* TODO 2021-04-18: Original-Bug: Why TA_SCHUSSWAFFEN for spears? */
				hero = get_hero(0);
				for (i = passed = 0; i <= 6; i++, hero += SIZEOF_HERO) {

					if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
						(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
						!hero_dead(hero) &&
						(test_skill(hero, TA_SCHUSSWAFFEN, 0) > 0))
					{
						passed++;
					}
				}

				if ((count_heroes_in_group() / 2) <= passed) {
					/* over the half of the group passed MISSLE WEAPON+0 */

					GUI_output(get_tx2(17));
					/* get 30 FOOD PACKAGES */
					get_item(ITEM_FOOD_PACKAGE, 1, 30);

				} else {
					/* everybody failed MISSLE WEAPON+0 */
					GUI_output(get_tx2(16));
				}
			} else {
				/* at least one hero failed STEALTH+0 */
				GUI_output(get_tx2(14));
			}
		} else {
			/* no hunting weapon in the group */
			GUI_output(get_tx2(18));
		}
	}

	resume_traveling();
}

void hunt_cavebear(void)
{
	Bit8u *hero;
	signed short answer;
	signed short i;

	pause_traveling(32);

	do {
		answer = GUI_radio(get_tx2(19), 2, get_tx2(20), get_tx2(21));
	} while (answer == -1);

	if (answer == 1) {

		/* print message */
		GUI_output(get_tx2(24));

		/* time to flee = 1.5 hour */
		timewarp(MINUTES(90));

	} else {
		GUI_output(get_tx2(22));

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
				(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
				!hero_dead(hero))
			{

				/* AT of the current weapon - (RS-BE / 2) <= 1W20 */
				if ((host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WEAPON_TYPE)) - (host_readbs(hero + HERO_RS_BE) / 2)) <= random_schick(20))
				{
#if !defined(__BORLANDC__)
					D1_INFO("%-16s erhaelt 5 AP fuer eine gelungene Attacke.\n",
						(char*)(hero + HERO_NAME2));
#endif
					add_hero_ap(hero, 5);
				}

				/* PA of the current weapon - (RS-BE / 2) <= 1W20 */
				if ((host_readbs(hero + HERO_PA + host_readbs(hero + HERO_WEAPON_TYPE)) - (host_readbs(hero + HERO_RS_BE) / 2)) > random_schick(20))
				{
#if !defined(__BORLANDC__)
					D1_INFO("%-16s erhaelt 3 AP fuer eine misslungene Parade.\n",
						(char*)(hero + HERO_NAME2));
#endif

					add_hero_ap(hero, 3);
					sub_hero_le(hero, dice_roll(2, 6, 0));
				}
			}
		}

		GUI_output(get_tx2(23));
	}

	resume_traveling();
}

/* should be static */
void hunt_viper(void)
{
	signed short choosen_hero;
	Bit8u *hero_i;
	signed short l_di;
	signed short i;

	/* load snake picture */
	pause_traveling(30);

	GUI_output(get_tx2(25));

	hero_i = get_hero(0);

	for (i = l_di = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

		/* hero is valid */
		/* hero is in current group */
		/* hero is not dead */
		/* check GE+0 */
		/* Original-Bug: something was forgotten */
		if ((host_readb(hero_i + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readb(hero_i + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP)) &&
			(!hero_dead(hero_i)) &&
			(test_attrib(hero_i, ATTRIB_GE, 0) < l_di))
		{
			/* remember the hero */
			choosen_hero = i;
		}
	}

	if (l_di) {

		/* select the chosen hero */
		hero_i = get_hero(choosen_hero);

		/* print a message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx2(26),
			hero_i + HERO_NAME2);

		GUI_output(Real2Host(ds_readd(DTP2)));

		/* hero gets 2 AP */
		add_hero_ap(hero_i, 2);

		if (random_schick(100) <= 50) {
			/* hero gets bitten */

			/* hero gets 3 AP */
			add_hero_ap(hero_i, 3);

			/* and 2 * 3W6 damage */
			sub_hero_le(hero_i, dice_roll(3, 6, 0) * 2);
		}
	} else {
		GUI_output(get_tx2(27));
	}

	resume_traveling();
}

void octopus_attack(void)
{
	signed short i;
	signed short hits;
	signed short tmp;
	char overboard[7];
	Bit8u *hero;
#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 22: see below */
	char any_hero_active;
#endif

	hits = 0;
	pause_traveling(31);
	memset(overboard, 0, 7);

	GUI_output(get_tx2(28));
	GUI_output(get_tx2(29));

	do {
#ifdef M302de_ORIGINAL_BUGFIX
		/* Original-Bug 22: see below */
		any_hero_active = 0;
#endif
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				!overboard[i])
			{
#ifdef M302de_ORIGINAL_BUGFIX
				/* Original-Bug 22: see below */
				any_hero_active = 1;
#endif
#ifndef M302de_ORIGINAL_BUGFIX
				/* Original-Bug 21:
				 * Apparently, the outcome of the following GE-attrib-test is misinterpreted.
				 * For GE <= 17, its always 5% strangling attack, 5% grabbing attack, 90% hero attacks octopus.
				 * For GE = 18 its 5% strangling attack, 95% hero attacks octopus.
				 * For GE <= 19 it is 100% hero attacks octopus.
				 * The intended behavior probably was: critical failure: grabbing attack; normal failure: strangling attack; success: hero attacks octopus. */
				if (!(tmp = test_attrib(hero, ATTRIB_GE, 0)))
#else
				tmp = test_attrib(hero, ATTRIB_GE, 0);
				if (tmp <= 0 && tmp != -99)
#endif
				{
					/* strangling attack */
					/* <hero> GERAET IN DEN WUERGEGRIFF DES KRAKENMOLCHS UND KANN SICH NUR UNTER MUEHEN WIEDER BEFREIEN. */

					add_hero_ap(hero, 5);
					sub_hero_le(hero, random_schick(6));
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(30),
						(char*)hero + HERO_NAME2);
					GUI_output(Real2Host(ds_readd(DTP2)));
				}

#ifndef M302de_ORIGINAL_BUGFIX
				/* Original-Bug 21: see above */
				else if (tmp == -1)
#else
				else if (tmp == -99)
#endif
				{
					/* grabbing attack */
					/* <hero> WIRD VON EINEM TENTAKEL GEPACKT UND UEBER BORD GERISSEN! */

					add_hero_ap(hero, 20);
					sub_hero_le(hero, random_schick(6));
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(31),
						(char*)hero + HERO_NAME2);
					GUI_output(Real2Host(ds_readd(DTP2)));

					if (test_skill(hero, TA_SCHWIMMEN, 0) <= 0) {
						sub_hero_le(hero, random_schick(6));
						overboard[i] = 1;
					}

				} else {
					/* chance to hit the beast */
					/* Original-Bug: Petrified, sleeping etc. heroes may hit the octopus (found by siebenstreich 2021-08-12 https://www.crystals-dsa-foren.de/showthread.php?tid=4589&pid=167425#pid167425) */

					/* Original-Bug: The following is the wrong way round, should be ">= random_schick(20)" (found by siebenstreich 2021-08-12 https://www.crystals-dsa-foren.de/showthread.php?tid=4589&pid=167430#pid167430) */
					/* Further dubiosity: Only the bare HERO_AT value of weapon type of the equipped weapon is taken into account. HERO_AT_MOD (modifier of the current weapon) and HERO_RS_BE are ignored. */
					if (host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WEAPON_TYPE)) <= random_schick(20)) {
						hits++;
					}
				}
			}
		}
	}
#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 22: infinite loop if all heroes are overboard/dead */
	while (hits <= 5);
#else
	while ((hits <= 5) && any_hero_active);
	if (!any_hero_active) {
		/* octopus has won. all heroes disappear in the open sea. */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE && host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) {
				hero_disappear(hero, i, -2);
			}
		}
	}
#endif

	sub_group_le(3);
	add_hero_ap_all(5);
	GUI_output(get_tx2(32));

	ds_writew(BASEPOS_X, ds_writew(BASEPOS_Y, 0));
	status_menu(get_hero_index(Real2Host(get_first_hero_available_in_group())));
	resume_traveling();
}

void hunt_bison(void)
{
	signed short answer;
	Bit8u *hero;

	pause_traveling(29);

	do {
		answer = GUI_radio(get_tx2(33), 2, get_tx2(34), get_tx2(35));

	} while (answer == -1);

	if (answer == 1) {
		timewarp(MINUTES(30));
		GUI_output(get_tx2(38));
	} else {
		GUI_output(get_tx2(36));

		hero = get_hero(get_random_hero());
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx2(37),
			(char*)hero + HERO_NAME2);
		GUI_output(Real2Host(ds_readd(DTP2)));
		sub_hero_le(hero, random_schick(6));
		add_hero_ap(hero, 2);
	}

	resume_traveling();
}

void hunt_rhino(void)
{
	signed short answer;
	Bit8u *hero;

	pause_traveling(33);

	do {
		answer = GUI_radio(get_tx2(39), 2, get_tx2(40), get_tx2(41));

	} while (answer == -1);

	if (answer == 1) {
		timewarp(MINUTES(30));
		GUI_output(get_tx2(44));
	} else {
		GUI_output(get_tx2(42));

		hero = get_hero(get_random_hero());
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx2(43),
			(char*)hero + HERO_NAME2);
		GUI_output(Real2Host(ds_readd(DTP2)));
		sub_hero_le(hero, dice_roll(2, 6, 0));
		add_hero_ap(hero, 6);
	}

	resume_traveling();
}

void pirates_attack(void)
{
	pause_traveling(9);

	GUI_output(get_tx2(45));
	GUI_output(get_tx2(46));
	GUI_output(get_tx2(47));

	ds_writew(MAX_ENEMIES, random_interval(3, 8));
	ds_writew(FIG_DISCARD, 1);

	do_fight(FIGHTS_S001);

	ds_writew(BASEPOS_X, ds_writew(BASEPOS_Y, 0));

	status_menu(get_hero_index(Real2Host(get_first_hero_available_in_group())));

	resume_traveling();
}

void do_wild8_fight(void)
{
	signed short bak1;
	signed short bak2;

	bak1 = ds_readws(BASEPOS_X);
	bak2 = ds_readws(BASEPOS_Y);
	ds_writew(WALLCLOCK_UPDATE_BAK, ds_readws(WALLCLOCK_UPDATE));
	ds_writew(BASEPOS_X, 0);
	ds_writew(BASEPOS_Y, 0);
	ds_writeb(SHOW_TRAVEL_MAP, 0);

	ds_writew(MAX_ENEMIES, random_interval(5, 10));
	ds_writew(FIG_DISCARD, 1);

	do_fight(FIGHTS_WILD8);

	ds_writew(ROUTE_FIGHT_FLAG, 0);
	ds_writeb(SHOW_TRAVEL_MAP, 1);
	ds_writew(BASEPOS_X, bak1);
	ds_writew(BASEPOS_Y, bak2);
}

void random_encounter(signed short arg)
{
	signed short l_si;
	signed short i;
	signed short randval;
	signed short bak1;
	signed short bak2;
	signed short wallclock_update_bak;

	l_si = 0;

	if (random_schick(100) > 33) {
		l_si = 1;
	}

	bak1 = ds_readws(BASEPOS_X);
	bak2 = ds_readws(BASEPOS_Y);
	wallclock_update_bak = ds_readws(WALLCLOCK_UPDATE);
	ds_writew(BASEPOS_X, 0);
	ds_writew(BASEPOS_Y, 0);

	arg = ds_readb((RANDOM_ENCOUNTER_INDEX-1) + arg);

	randval = random_schick(100);

	for (i = 0; i < 14; i++) {

		if ((ds_readb(RANDOM_ENCOUNTER_DESCR + 7 * i + arg) <= randval) && (ds_readb(RANDOM_ENCOUNTER_DESCR + 7 * i + arg) != 0)) {

			ds_writeb(SHOW_TRAVEL_MAP, (signed char)ds_writew(WALLCLOCK_UPDATE, 0));
			ds_writeb(TRAVEL_EVENT_ACTIVE, 1);
			ds_writew(FIG_DISCARD, 1);

			switch (i) {
				case 0: {
					if (!l_si) {
						ds_writew(MAX_ENEMIES, random_schick(6) + 1);
						do_fight(FIGHTS_WILD1);
					}
					break;
				}
				case 1: {
					if (!l_si) {
						ds_writew(MAX_ENEMIES, random_schick(3));
						do_fight(FIGHTS_WILD2);
					}
					break;
				}
				case 2: {
					hunt_karen();
					break;
				}
				case 3: {
					if (!l_si) {
						ds_writew(MAX_ENEMIES, random_schick(2));
						do_fight(FIGHTS_WILD3);
					}
					break;
				}
				case 4: {
					hunt_viper();
					break;
				}
				case 5: {
					if (!l_si) {
						ds_writew(MAX_ENEMIES, random_schick(6) + 1);
						do_fight(FIGHTS_WILD4);
					}
					break;
				}
				case 6: {
					if (!l_si) {
						ds_writew(MAX_ENEMIES, random_schick(3) + 3);
						do_fight(FIGHTS_WILD4);
					}
					break;
				}
				case 7: {
					if (!l_si) {
						ds_writew(MAX_ENEMIES, random_schick(2) + 1);
						do_fight(FIGHTS_WILD5);
					}
					break;
				}
				case 8: {
					hunt_rhino();
					break;
				}
				case 9: {
					hunt_wildboar();
					break;
				}
				case 10: {
					if (!l_si) {
						ds_writew(MAX_ENEMIES, random_schick(3));
						do_fight(FIGHTS_WILD6);
					}
					break;
				}
				case 11: {
					hunt_bison();
					break;
				}
				case 12: {
					if (!l_si) {
						ds_writew(MAX_ENEMIES, random_schick(3));
						do_fight(FIGHTS_WILD7);
					}
					break;
				}
				case 13: {
					hunt_cavebear();
					break;
				}
			}

			ds_writew(FIG_DISCARD, 0);
			ds_writeb(SHOW_TRAVEL_MAP, 1);
			ds_writeb(TRAVEL_EVENT_ACTIVE, 0);
			break;
		}
	}

	ds_writew(BASEPOS_X, bak1);
	ds_writew(BASEPOS_Y, bak2);
	ds_writew(WALLCLOCK_UPDATE, wallclock_update_bak);
	load_tx(ARCHIVE_FILE_MAPTEXT_LTX);
}

void search_ruin1(void)
{
	do_talk(17, 0);
	set_var_to_zero();
}

void tevent_115(void)
{
	if (ds_readb(FIND_HYGGELIK) != 0) {
		ds_writeb(TEVENT115_FLAG, 1);
		do_talk(17, 0);
		set_var_to_zero();
	}
}

void TLK_way_to_ruin(signed short state)
{
	signed short i;
	RealPt hero;
	Bit8u *hero2;

	hero2 = Real2Host(get_first_hero_available_in_group());

	if (!state) {
		ds_writew(DIALOG_NEXT_STATE, ds_readb(TEVENT115_FLAG) != 0 ? 45 : 66);
		ds_writew(TLK_RUIN_HERO_COUNTER, 0);
	} else if (state == 66 || state == 45) {
		show_treasure_map();
	} else if (state == 4 || state == 7) {
		timewarp(HOURS(1));
	} else if (state == 6) {
		hero = (RealPt)ds_readd(HEROES) + SIZEOF_HERO * get_random_hero();
		ds_writew(DIALOG_NEXT_STATE, test_skill(Real2Host(hero), TA_WILDNISLEBEN, 6) > 0 ? 8 : 7);
	} else if (state == 8) {
		timewarp(HOURS(1));
		TRV_ford_test(0, 30);
	} else if (state == 9) {

		do {
			hero = (RealPt)ds_readds(HEROES) + SIZEOF_HERO * ds_readws(TLK_RUIN_HERO_COUNTER);
			inc_ds_ws(TLK_RUIN_HERO_COUNTER);

			if (host_readbs(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(Real2Host(hero) + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(Real2Host(hero))) {

				ds_writed(RUIN_HERO, (Bit32u)hero);
				break;
			}

		} while (ds_readws(TLK_RUIN_HERO_COUNTER) != 7);

		ds_writew(DIALOG_NEXT_STATE, ds_readws(TLK_RUIN_HERO_COUNTER) == 7 ? 13 : 10);
	} else if (state == 10) {
		ds_writew(DIALOG_NEXT_STATE, test_skill(Real2Host(ds_readd(RUIN_HERO)), TA_SCHWIMMEN, 5) > 0 ? 11 : 12);
	} else if (state == 12) {
		sub_hero_le(Real2Host(ds_readd(RUIN_HERO)), random_schick(4) + 1);

		/* Original-Bug: hero != RUIN_HERO */
		hero_disease_test(Real2Host(ds_readd(RUIN_HERO)), 2,
			25 - (host_readbs(Real2Host(hero) + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(Real2Host(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK)))));

		loose_random_item(Real2Host(ds_readd(RUIN_HERO)), 10, get_ttx(506));

	} else if (state == 13 || state == 25 || state == 34 || state == 59 || state == 62) {
		timewarp(MINUTES(30));
	} else if (state == 14 || state == 64) {
		timewarp(MINUTES(10));
	} else if (state == 15 || state == 16) {
		timewarp(MINUTES(20));
	} else if (state == 17) {
		ds_writew(DIALOG_NEXT_STATE, test_skill(hero2, TA_ORIENTIERUNG, 5) > 0 ? 18 : 19);
	} else if (state == 19) {
		timewarp(MINUTES(20));
		ds_writed(RUIN_HERO, (Bit32u)((RealPt)ds_readd(HEROES) + SIZEOF_HERO * get_random_hero()));
		ds_writew(DIALOG_NEXT_STATE, test_skill(Real2Host(ds_readd(RUIN_HERO)), TA_AEXTE, 2) > 0 ? 20 : 21);
	} else if (state == 20) {
		loose_random_item(get_hero(get_random_hero()), 5, get_ttx(506));
	} else if (state == 21) {
		timewarp(MINUTES(10));
		loose_random_item(hero2, 10, get_ttx(506));
		sub_hero_le(hero2, random_schick(4) + 2);
	} else if (state == 22 || state == 30 || state == 52 || state == 53) {
		timewarp(HOURS(3));
	} else if (state == 26 || state == 36 || state == 37 || state == 38 || state == 39) {
		timewarp(HOURS(1));
	} else if (state == 27 || state == 29 || state == 65) {
		timewarp(HOURS(2));
	} else if (state == 54) {
		timewarp(HOURS(5));
	} else if (state == 28) {

		hero = (RealPt)ds_readds(HEROES);

		for (i = ds_writews(TLK_RUIN_HERO_COUNTER, 0); i <= 6; i++, hero += SIZEOF_HERO) {

			if (host_readbs(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(Real2Host(hero) + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(Real2Host(hero)) &&
				test_skill(Real2Host(hero), TA_ORIENTIERUNG, 0) > 0) {

					inc_ds_ws(TLK_RUIN_HERO_COUNTER);
			}
		}

		ds_writew(DIALOG_NEXT_STATE, (count_heroes_in_group() >> 1) < ds_readws(TLK_RUIN_HERO_COUNTER) ? 29 : 30);

	} else if (state == 41) {
		ds_writeb(EVENT_ANI_BUSY, 1);
		load_ani(11);
		draw_main_screen();
		init_ani(0);
		timewarp(MINUTES(90));
		ds_writew(REQUEST_REFRESH, 1);
	} else if (state == 42 || state == 60) {
		timewarp(MINUTES(150));
	} else if (state == 67 || state == 44) {
		ds_writeb(TRAVEL_DETOUR, DUNGEONS_HYGGELIKS_RUINE);

	} else if (state == 48) {

		hero = (RealPt)ds_readds(HEROES);

		for (i = ds_writews(TLK_RUIN_HERO_COUNTER, 0); i <= 6; i++, hero += SIZEOF_HERO) {

			if (host_readbs(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(Real2Host(hero) + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(Real2Host(hero)) &&
				test_skill(Real2Host(hero), TA_ORIENTIERUNG, 0) > 0) {

					inc_ds_ws(TLK_RUIN_HERO_COUNTER);
			}
		}

		ds_writew(DIALOG_NEXT_STATE, (count_heroes_in_group() >> 1) < ds_readws(TLK_RUIN_HERO_COUNTER) ? 49 : 50);
	}

	ds_writeb(EVENT_ANI_BUSY, 0);
}

void tevent_087(void)
{
	if (!ds_readb(MET_UNICORN_FLAG) && ds_readws(GOT_MAIN_QUEST) != 0) {
		do_talk(11, 2);
		ds_writeb(MET_UNICORN_FLAG, 1);
	}
}

#if !defined(__BORLANDC__)
}
#endif
