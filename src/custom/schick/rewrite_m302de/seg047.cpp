/**
 *	Rewrite of DSA1 v3.02_de functions of seg047 (heros, group)
 *	Functions rewritten: 18/18 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg047.cpp
 */

#include <stdio.h>
#include <stdlib.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct helper {
	signed short v[7];
};

/**
 * get_hero_CH_best - get the index of the hero with the best CH value
 *
 * Returns the index of the hero with the highest unmodified CH value.
 * The hero must be alive and in the current group.
 */
unsigned short get_hero_CH_best()
{

	unsigned short retval;
	Bit8u *hero_i;
	signed short i;
	signed short ch_val = -1;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

		if ((host_readb(hero_i + HERO_TYPE) != 0) &&
				/* check class */
			(host_readb(hero_i + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP)) &&
				/* check if in group */
			(!hero_dead(hero_i)) &&
				/* check if not dead */
			(host_readbs(hero_i + HERO_CH) > ch_val)) {
				/* check if CH is the highest */

				ch_val = host_readbs(hero_i + HERO_CH);
				retval = i;
		}
	}

	return retval;
}

/**
 * get_hero_KK_best - get the index of the hero with the best KK value
 *
 * Returns the index of the hero with the highest unmodified KK value.
 * The hero must be alive and in the current group.
 */
unsigned short get_hero_KK_best() {
	signed short retval;
	Bit8u *hero_i;
	signed short i;
	signed short kk_val = -1;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
		if ((host_readb(hero_i + HERO_TYPE) != 0) &&
				/* check class */
			(host_readb(hero_i + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP)) &&
				/* check if in group */
			(!hero_dead(hero_i)) &&
				/* check if not dead */
			(host_readbs(hero_i + HERO_KK) > kk_val)) {
				/* check if KK is the highest */

				kk_val = host_readbs(hero_i + HERO_KK);
				retval = i;
			}
	}

	return retval;
}

/**
 * hero_is_diseased - returns the disease number the hero has
 * @hero:	the hero which should be checked
 *
 * Only the first disease is returned here.
 */
unsigned short hero_is_diseased(Bit8u *hero)
{
	signed short i;

	for (i = 1; i <= 7; i++)
		if (host_readbs(hero + (HERO_ILLNESS-5) + i * 5) == -1)
			return i;

	return 0;
}

/**
 * hero_is_poisoned - returns the poison number the hero has
 * @hero:	the hero which should be checked
 *
 * Only the first poison is returned here.
 */
unsigned short hero_is_poisoned(Bit8u *hero)
{

	signed short i;

	for (i = 1; i <= 9; i++)
		if (host_readbs(hero + (HERO_POISON-5) + i * 5) == -1)
			return i;

	return 0;
}

/**
 * hero_gets_poisoned - poison a hero
 * @hero:	the hero which gets poisoned
 * @poison:	the kind of poison
 */
void hero_gets_poisoned(Bit8u *hero, unsigned short poison) {

	if (!hero_dead(hero)) {

		host_writeb(hero + poison * 5 + 0xd6, 0xff);
		host_writeb(hero + poison * 5 + 0xd7, 0x00);
		host_writeb(hero + poison * 5 + 0xd8, 0x00);
		host_writeb(hero + poison * 5 + 0xd9, 0x00);
		host_writeb(hero + poison * 5 + 0xda, 0x00);
	}
}

/**
 * hero_gets_diseased - diseases a hero
 * @hero:	the hero which gets diseased
 * @disease:	the kind of disease
 */
void hero_gets_diseased(Bit8u *hero, unsigned short disease)
{
#ifdef M302de_ORIGINAL_BUGFIX
	/* not a real BUG, but very useless */
	if (host_readb(hero + HERO_TYPE) == 0)
		return;
#endif

	if (!hero_dead(hero)) {
#if !defined(__BORLANDC__)
		D1_INFO("%s erkrankt an %s\n",
			(char*)hero + HERO_NAME2,
			(char*)get_ltx((disease + 0x193) * 4));
#endif

		host_writeb(hero + disease * 5 + 0xae, 0xff);
		host_writeb(hero + disease * 5 + 0xaf, 0x00);
		host_writeb(hero + disease * 5 + 0xb0, 0x00);
		host_writeb(hero + disease * 5 + 0xb1, 0x00);
		host_writeb(hero + disease * 5 + 0xb2, 0x00);
	}
}

/**
 * hero_disease_test - the hero may get a disease
 * @hero:	the hero which may gets diseased
 * @disease:	the kind of disease
 * @probability: the probability to get diseased in percent
 */
void hero_disease_test(Bit8u *hero, unsigned short disease, signed short probability) {

#ifdef M302de_ORIGINAL_BUGFIX
	/* not a real BUG, but very useless */
	if (host_readb(hero + HERO_TYPE) == 0)
		return;
#endif

	/* check the probability and if hero is diseased*/
	if (random_schick(100) <= probability &&
		host_readb(hero + disease * 5 + 0xae) != 0xff) {

		hero_gets_diseased(hero, disease);
	}
}

/**
 *	check_hero_KK_unused
 *
 *	This function is not used in the game!
 */
short check_hero_KK_unused(short val)
{

	return (host_readbs(get_hero(0) + HERO_KK) + host_readbs(get_hero(0) + HERO_KK_MOD) >= val) ? 1 : 0;
}

/**
 *	check_heros_KK
 *	@val:	value to compare KK with
 *
 *	This function, like hero_check_KK_unused, is buggy!
 *	It does not check if the first slot is a valid hero.
 */
short check_heros_KK(short val) {

	Bit8u *hero;
	signed short sum;

	hero = get_hero(0);

	/* Orig-BUG: not checked if hero is valid */
	sum = host_readbs(hero + HERO_KK) + host_readbs(hero + HERO_KK_MOD);

	hero = get_hero(1);

	/* check class, group and dead status of hero in slot 2*/
	if (host_readb(hero + HERO_TYPE) && host_readb(hero + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) && (!hero_dead(hero))) {
		sum += host_readbs(hero + HERO_KK) + host_readbs(hero + HERO_KK_MOD);
	}

#if !defined(__BORLANDC__)
	D1_INFO("Pruefe KK der ersten beiden Helden (%d) >= %d: ", sum, val);
	D1_INFO("%s\n", sum >= val ? "gelungen" : "mislungen");
#endif

	return (sum >= val) ? 1 : 0;
}

/**
 *	make_valuta_str	-	makes a valuta string
 *	@dst:	the destination
 *	@money:	the money in Heller
 *
 *	This funcion is buggy.
 */
void make_valuta_str(char *dst, Bit32s money) {
	/* Orig-BUG: d can overflow  on D > 65536*/
	unsigned short d = 0;
	unsigned short s = 0;

	/*	These loops are not very performant.
		They take longer the more money you have.
		Here is a much better solution.
	*/

	/*
	d = money / 100;
	money -= d * 100;

	s = money / 10;
	money -= s * 10;
	*/
	while (money / 100) {
		d++;
		money -= 100;
	}

	while (money / 10) {
		s++;
		money -= 10;
	}

	sprintf(dst, (char*)get_ltx(0xbb0), d, s, (short)money);
}

/**
 *	update_atpa() -	recalculates the AT PA values
 *
 */
void update_atpa(Bit8u *hero)
{
	div_t erg;
	signed short diff;
	signed short i;

	/* ATPA base = (IN + KK + GE) / 5 rounded */
	erg = div(host_readbs(hero + HERO_IN_ORIG) + host_readbs(hero + HERO_KK_ORIG) + host_readbs(hero + HERO_GE_ORIG), 5);

	/* round up */
	if (erg.rem >= 3)
		erg.quot++;

	/* calculate difference */
	diff = erg.quot - host_readbs(hero + HERO_ATPA_BASIS);

	if (diff != 0) {

		/* update atpa base value */
		host_writeb(hero + HERO_ATPA_BASIS, erg.quot);

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0x20), host_readbs(hero + HERO_ATPA_BASIS));

		/* print message */
		GUI_output(Real2Host(ds_readd(DTP2)));

		for (i = 0; i < 7; i++) {
			/* add diff to AT value */
			host_writeb(hero + HERO_AT + i,
				host_readbs(hero + HERO_AT + i) + diff);
			/* add diff to PA value */
			host_writeb(hero + HERO_PA + i,
				host_readbs(hero + HERO_PA + i) + diff);
		}
	}
}

/**
 * menu_enter_delete() - selects a hero to enter / get deleted
 * @ptr:	pointer
 * @entries:	how many heros
 * @mode:	1 = enter / -1 = delete
 *
 * Returns: the number of the selected hero.
 * Used only in temples.
 */
signed short menu_enter_delete(RealPt ptr, signed short entries, signed short mode)
{
	signed short i;
	signed short answer;
	signed short a;
	signed short l_di;

	answer = 0;
	l_di = 0;

	while (answer != -1) {

		a = (l_di + 10 < entries) ? 10 : entries - l_di;

		/* fill a pointer array with the pointer to the names */
		for (i = 0; i < a; i++) {
			ds_writed(RADIO_NAME_LIST + 4 * i,
				(Bit32u)((i + l_di) * 32 + ptr + 0x10));
		}

		i = a;
		if (entries > 10) {
			ds_writed(RADIO_NAME_LIST + 4 * i,
				host_readd(Real2Host((RealPt)ds_readd(TEXT_LTX) + 0x48c)));
			i++;
		}

		answer = GUI_radio( (mode == -1) ? get_ltx(0x8dc) : get_ltx(0x490),
				(signed char)i,
				Real2Host(ds_readd(RADIO_NAME_LIST)),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 2 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 3 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 5 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 6 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 7 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 8 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 9 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 10 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 11 * 4))));

		if ((entries > 10) && (answer == i)) {
			l_di += 10;

			if (l_di > entries) {
				l_di = 0;
			}

		} else {

			if (answer != -1) {
				return l_di + answer - 1;
			}
		}
	}

	return -1;
}

/**
 * select_hero_from_group() - menu for selecting a hero
 * @title:	titlestring for the menu
 *
 * Returns: index of the hero or -1 (ESC).
 * Remark: The available heros must be in the group only.
 */
signed short select_hero_from_group(Bit8u *title)
{
	signed short i;
	signed short answer;
	/* Hack for
	signed short dst[7] = {0, 0, 0, 0, 0, 0, 0};
	*/
#if !defined (__BORLANDC__)
	struct helper dst = {{0, 0, 0, 0, 0, 0, 0}};
#else
	struct helper dst = *(((struct helper*)(p_datseg + SEG047_INIT1)));
#endif
	signed short cnt;
	signed short bak_1;
	signed short bak_2;
	signed short bak_3;
	RealPt hero;

	bak_1 = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);
	cnt = 0;

	for (i = 0; i <= 6; i++) {

		hero = (RealPt)ds_readd(HEROS) + i * SIZEOF_HERO;

		if (host_readb(Real2Host(hero) + HERO_TYPE) != 0 &&
			host_readb(Real2Host(hero) + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
				/* TODO: find out what that means */
				ds_readbs(0x64a2) != i) {

			/* save pointer to the name of the hero */
			ds_writed(RADIO_NAME_LIST + cnt * 4, (Bit32u)(hero + HERO_NAME2));
			dst.v[cnt] = i;
			cnt++;
		}
	}


	ds_writeb(0x64a2, -1);

	if (cnt != 0) {
		bak_2 = ds_readw(0x2ca2);
		bak_3 = ds_readw(0x2ca4);

		ds_writew(0x2ca2, ds_writew(0x2ca4, 0));

		answer = GUI_radio(title, (signed char)cnt,
				Real2Host(ds_readd(RADIO_NAME_LIST)),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 2 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 3 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 5 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 6 * 4)))) - 1;

		ds_writew(0x2ca2, bak_2);
		ds_writew(0x2ca4, bak_3);
		ds_writew(TEXTBOX_WIDTH, bak_1);

		if (answer != -2)
			return dst.v[answer];
		else
			return -1;
	}

	ds_writew(TEXTBOX_WIDTH, bak_1);
	return -1;
}

/**
 * select_hero_ok() - menu for selecting a hero
 * @title:	titlestring for the menu
 *
 * Returns: index of the hero or -1 (ESC).
 * Remark: The available heros must be in the group and pass check_hero().
 */
signed short select_hero_ok(Bit8u *title)
{
	signed short i;
	signed short answer;
	/* Hack for
	signed short dst[7] = {0, 0, 0, 0, 0, 0, 0};
	*/
#if !defined (__BORLANDC__)
	struct helper dst = {{0, 0, 0, 0, 0, 0, 0}};
#else
	struct helper dst = *(((struct helper*)(p_datseg + SEG047_INIT2)));
#endif
	signed short cnt;
	signed short bak_1;
	signed short bak_2;
	signed short bak_3;
	RealPt hero;

	bak_1 = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);
	cnt = 0;

	for (hero = (RealPt)ds_readd(HEROS), i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readb(Real2Host(hero) + HERO_TYPE) != 0 &&
			host_readb(Real2Host(hero) + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
			check_hero(Real2Host(hero)) &&
				/* TODO: find out what that means */
				ds_readbs(0x64a2) != i) {

			/* save pointer to the name of the hero */
			ds_writed(RADIO_NAME_LIST + cnt * 4, (Bit32u)(hero + HERO_NAME2));
			dst.v[cnt] = i;
			cnt++;
		}
	}

	ds_writeb(0x64a2, -1);

	if (cnt != 0) {
		bak_2 = ds_readw(0x2ca2);
		bak_3 = ds_readw(0x2ca4);

		ds_writew(0x2ca2, ds_writew(0x2ca4, 0));

		answer = GUI_radio(title, (signed char)cnt,
				Real2Host(ds_readd(RADIO_NAME_LIST)),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 2 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 3 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 5 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 6 * 4)))) - 1;

		ds_writew(0x2ca2, bak_2);
		ds_writew(0x2ca4, bak_3);
		ds_writew(TEXTBOX_WIDTH, bak_1);
		ds_writew(SKILLED_HERO_POS, -1);

		if (answer != -2)
			return dst.v[answer];
		else
			return -1;
	}

	ds_writew(TEXTBOX_WIDTH, bak_1);
	ds_writew(SKILLED_HERO_POS, -1);
	return -1;
}

/**
 * select_hero_ok_forced() - menu for selecting a hero
 * @title:	titlestring for the menu
 *
 * Returns: index of the hero or -1 (ESC).
 * Remark: The available heros must be in the group, pass check_hero() and
 *		you are forced to select a hero.
 */
signed short select_hero_ok_forced(Bit8u *title)
{
	signed short i;
	signed short answer;
	/* Hack for
	signed short dst[7] = {0, 0, 0, 0, 0, 0, 0};
	*/
#if !defined (__BORLANDC__)
	struct helper dst = {{0, 0, 0, 0, 0, 0, 0}};
#else
	struct helper dst = *(((struct helper*)(p_datseg + SEG047_INIT3)));
#endif
	signed short cnt;
	signed short bak_1;
	signed short bak_2;
	signed short bak_3;
	RealPt hero;

	bak_1 = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);
	cnt = 0;

	for (hero = (RealPt)ds_readd(HEROS), i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readb(Real2Host(hero) + HERO_TYPE) != 0 &&
			host_readb(Real2Host(hero) + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
			check_hero(Real2Host(hero)) &&
				/* TODO: find out what that means */
				ds_readbs(0x64a2) != i) {

			/* save pointer to the name of the hero */
			ds_writed(RADIO_NAME_LIST + cnt * 4, (Bit32u)(hero + HERO_NAME2));
			dst.v[cnt] = i;
			cnt++;
		}
	}

	ds_writeb(0x64a2, -1);

	if (cnt != 0) {
		do {
			bak_2 = ds_readw(0x2ca2);
			bak_3 = ds_readw(0x2ca4);

			ds_writew(0x2ca2, ds_writew(0x2ca4, 0));

			answer = GUI_radio(title, (signed char)cnt,
				Real2Host(ds_readd(RADIO_NAME_LIST)),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 2 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 3 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 5 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 6 * 4)))) - 1;

			ds_writew(0x2ca2, bak_2);
			ds_writew(0x2ca4, bak_3);

		} while (answer == -2);

		ds_writew(SKILLED_HERO_POS, -1);
		ds_writew(TEXTBOX_WIDTH, bak_1);

		return dst.v[answer];
	} else {

		ds_writew(SKILLED_HERO_POS, -1);
		return 0;
	}
}

/**
 * count_heroes_in_group() - counts the heroes in the current group
 *
 * Returns how many alive heros are in the group.
 */
signed short count_heroes_in_group(void)
{
	Bit8u *hero_i;
	signed short i;
	signed short retval;

	retval = 0;

	for (hero_i = get_hero(0), i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
		/* Check class, group and dead */
		if ((host_readb(hero_i + HERO_TYPE) != 0) &&
			(host_readb(hero_i + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP)) &&
			(!hero_dead(hero_i))) {

			retval++;
		}
	}

	return retval;
}

/**
 * hero_get_drunken() - let the hero feel the result of too much alcohol
 * @hero:	the hero
 */
void hero_get_drunken(Bit8u *hero)
{

	if (!host_readbs(hero + HERO_DRUNK)) {

		/* set the hero drunken */
		host_writeb(hero + HERO_DRUNK, 1);

		/* change good attributes */
		add_ptr_bs(hero + HERO_MU, 1);
		sub_ptr_bs(hero + HERO_KL, 1);
		sub_ptr_bs(hero + HERO_CH, 1);
		sub_ptr_bs(hero + HERO_FF, 1);
		sub_ptr_bs(hero + HERO_GE, 1);
		add_ptr_bs(hero + HERO_IN, 1);
		add_ptr_bs(hero + HERO_KK, 1);

		/* Reset bad attributes */
		add_ptr_bs(hero + HERO_AG, 1);
		sub_ptr_bs(hero + HERO_HA, 1);
		sub_ptr_bs(hero + HERO_RA, 1);
		add_ptr_bs(hero + HERO_GG, 1);
		sub_ptr_bs(hero + HERO_TA, 1);
		add_ptr_bs(hero + HERO_NG, 1);
		add_ptr_bs(hero + HERO_JZ, 1);

		/* do a burp FX2.VOC */
		if (ds_readb(PP20_INDEX) == ARCHIVE_FILE_ZUSTA_UK) {
			ds_writew(REQUEST_REFRESH, 1);
		}

		play_voc_delay(ARCHIVE_FILE_FX2_VOC);
	}
}

/**
 *	hero_get_sober	-	makes a drunken hero sober
 *	@hero:	pointer to the hero
 *
 */
void hero_get_sober(Bit8u *hero) {
	/* This is checked twice */
	/* Is hero drunken ? */
	if (host_readb(hero + HERO_DRUNK) == 0)
		return;

#if !defined(__BORLANDC__)
	D1_INFO("%s ist wieder nuechtern\n", (char*)hero + HERO_NAME2);
#endif

	/* set hero sober */
	host_writeb(hero + HERO_DRUNK, 0);

	/* Reset good attributes */
	sub_ptr_bs(hero + HERO_MU, 1);
	add_ptr_bs(hero + HERO_KL, 1);
	add_ptr_bs(hero + HERO_CH, 1);
	add_ptr_bs(hero + HERO_FF, 1);
	add_ptr_bs(hero + HERO_GE, 1);
	sub_ptr_bs(hero + HERO_IN, 1);
	sub_ptr_bs(hero + HERO_KK, 1);

	/* Reset bad attributes */
	sub_ptr_bs(hero + HERO_AG, 1);
	add_ptr_bs(hero + HERO_HA, 1);
	add_ptr_bs(hero + HERO_RA, 1);
	sub_ptr_bs(hero + HERO_GG, 1);
	add_ptr_bs(hero + HERO_TA, 1);
	sub_ptr_bs(hero + HERO_NG, 1);
	sub_ptr_bs(hero + HERO_JZ, 1);

	if (ds_readb(PP20_INDEX) == ARCHIVE_FILE_ZUSTA_UK)
		ds_writew(REQUEST_REFRESH, 1);
}

#if !defined(__BORLANDC__)
}
#endif
