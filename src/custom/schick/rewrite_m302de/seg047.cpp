/**
 *	Rewrite of DSA1 v3.02_de functions of seg047 (heroes, group)
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
 * \brief   get the index of the hero with the best CH value
 *
 * \return              the index of the hero with the highest unmodified CH value.
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

		if ((host_readb(hero_i + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readb(hero_i + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP)) &&
				/* check if in group */
			(!hero_dead(hero_i)) &&
				/* check if not dead */
			(host_readbs(hero_i + (HERO_ATTRIB + 3 * ATTRIB_CH)) > ch_val)) {
				/* check if CH is the highest */

				ch_val = host_readbs(hero_i + (HERO_ATTRIB + 3 * ATTRIB_CH));
				retval = i;
		}
	}

	return retval;
}

/**
 * \brief   get the index of the hero with the best KK value
 *
 * \return              the index of the hero with the highest unmodified KK value.
 * The hero must be alive and in the current group.
 */
unsigned short get_hero_KK_best() {
	signed short retval;
	Bit8u *hero_i;
	signed short i;
	signed short kk_val = -1;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
		if ((host_readb(hero_i + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readb(hero_i + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP)) &&
				/* check if in group */
			(!hero_dead(hero_i)) &&
				/* check if not dead */
			(host_readbs(hero_i + (HERO_ATTRIB + 3 * ATTRIB_KK)) > kk_val)) {
				/* check if KK is the highest */

				kk_val = host_readbs(hero_i + (HERO_ATTRIB + 3 * ATTRIB_KK));
				retval = i;
			}
	}

	return retval;
}

/**
 * \brief   returns the disease number the hero has
 *
 *          Only the first disease is returned here.
 *
 * \param   hero        the hero which should be checked
 * \return              number of the first disease the hero has
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
 * \brief   returns the poison number the hero has
 *
 *          Only the first poison is returned here.
 *
 * \param   hero        the hero which should be checked
 * \return              number of the first poisoning the hero has
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
 * \brief   poison a hero
 *
 * \param   hero        the hero which gets poisoned
 * \param   poison      the kind of poison
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
 * \brief   diseases a hero
 *
 * \param   hero        the hero which gets diseased
 * \param   disease     the kind of disease
 */
void hero_gets_diseased(Bit8u *hero, unsigned short disease)
{
#ifdef M302de_ORIGINAL_BUGFIX
	/* not a real BUG, but very useless */
	if (host_readb(hero + HERO_TYPE) == HERO_TYPE_NONE)
		return;
#endif

	if (!hero_dead(hero)) {
#if !defined(__BORLANDC__)
		D1_INFO("%s erkrankt an %s\n",
			(char*)hero + HERO_NAME2,
			(char*)get_ttx(disease + 0x193));
#endif

		host_writeb(hero + disease * 5 + 0xae, 0xff);
		host_writeb(hero + disease * 5 + 0xaf, 0x00);
		host_writeb(hero + disease * 5 + 0xb0, 0x00);
		host_writeb(hero + disease * 5 + 0xb1, 0x00);
		host_writeb(hero + disease * 5 + 0xb2, 0x00);
	}
}

/**
 * \brief   the hero may get a disease
 *
 * \param   hero        the hero which may gets diseased
 * \param   disease     the kind of disease
 * \param   probability the probability to get diseased in percent
 */
void hero_disease_test(Bit8u *hero, unsigned short disease, signed short probability) {

#ifdef M302de_ORIGINAL_BUGFIX
	/* not a real BUG, but very useless */
	if (host_readb(hero + HERO_TYPE) == HERO_TYPE_NONE) {
		D1_ERR("WARNING: called %s with an invalid hero\n", __func__);
		return;
	}
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

	return (host_readbs(get_hero(0) + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(get_hero(0) + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK)) >= val) ? 1 : 0;
}

/**
 *	check_heroes_KK
 * \param   val         value to compare KK with
 *
 *	This function, like hero_check_KK_unused, is buggy!
 *	It does not check if the first slot is a valid hero.
 */
short check_heroes_KK(short val) {

	Bit8u *hero;
	signed short sum;

	hero = get_hero(0);

	/* Orig-BUG: not checked if hero is valid */
	sum = host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK));

	hero = get_hero(1);

	/* check class, group and dead status of hero in slot 2*/
	if (host_readb(hero + HERO_TYPE) && host_readb(hero + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) && (!hero_dead(hero))) {
		sum += host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK));
	}

#if !defined(__BORLANDC__)
	D1_INFO("Pruefe KK der ersten beiden Helden (%d) >= %d: ", sum, val);
	D1_INFO("%s\n", sum >= val ? "gelungen" : "mislungen");
#endif

	return (sum >= val) ? 1 : 0;
}

/**
 * \brief   makes a valuta string
 *
 * \param   dst         the destination
 * \param   money       the money in Heller
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

	sprintf(dst, (char*)get_ttx(748), d, s, (short)money);
}

/**
 * \brief   recalculates the AT PA values
 */
void update_atpa(Bit8u *hero)
{
	div_t erg;
	signed short diff;
	signed short i;

	/* ATPA base = (IN + KK + GE) / 5 rounded */
	erg = div(host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_IN)) + host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_GE)), 5);
	/* Original-Bug:
	 * According to DSA 3 rules, AT basis value is (MU + KK + GE) / 5
	 * (PA basis (IN + KK + GE)/5 is correct */

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
			(char*)get_ttx(8), host_readbs(hero + HERO_ATPA_BASIS));

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
 * \brief   selects a hero to enter / get deleted
 *
 * \param   ptr         pointer
 * \param   entries     how many heroes
 * \param   mode        1 = enter / -1 = delete
 * \return              the number of the selected hero.
 * Used only in temples.
 */
signed short menu_enter_delete(RealPt ptr, signed short entries, signed short mode)
{
	signed short i;
	signed short answer;
	signed short i_max;
	signed short i_min;

	answer = 0;
	i_min = 0;

	while (answer != -1) {

		i_max = (i_min + 10 < entries) ? 10 : entries - i_min;

		/* fill a pointer array with the pointer to the names */
		for (i = 0; i < i_max; i++) {
			ds_writed(RADIO_NAME_LIST + 4 * i,
				(Bit32u)((i + i_min) * 32 + ptr + 0x10));
		}

		i = i_max;
		if (entries > 10) {
			ds_writed(RADIO_NAME_LIST + 4 * i,
				host_readd(Real2Host((RealPt)ds_readd(TEXT_LTX_INDEX) + 0x48c)));
			i++;
		}

		answer = GUI_radio( (mode == -1) ? get_ttx(567) : get_ttx(292),
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
			i_min += 10;

			if (i_min > entries) {
				i_min = 0;
			}

		} else {

			if (answer != -1) {
				return i_min + answer - 1;
			}
		}
	}

	return -1;
}

/**
 * \brief   menu for selecting a hero
 *
 * \param   title       titlestring for the menu
 * \return              index of the hero or -1 (ESC).
 * Remark: The available heroes must be in the group only.
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
	signed short textbox_width_bak;
	signed short bak_2;
	signed short bak_3;
	RealPt hero;

	textbox_width_bak = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);
	cnt = 0;

	for (i = 0; i <= 6; i++) {

		hero = (RealPt)ds_readd(HEROES) + i * SIZEOF_HERO;

		if (host_readb(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readb(Real2Host(hero) + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
				/* TODO: find out what that means */
				ds_readbs(HERO_SEL_EXCLUDE) != i) {

			/* save pointer to the name of the hero */
			ds_writed(RADIO_NAME_LIST + cnt * 4, (Bit32u)(hero + HERO_NAME2));
			dst.v[cnt] = i;
			cnt++;
		}
	}


	ds_writeb(HERO_SEL_EXCLUDE, -1);

	if (cnt != 0) {
		bak_2 = ds_readw(BASEPOS_X);
		bak_3 = ds_readw(BASEPOS_Y);

		ds_writew(BASEPOS_X, ds_writew(BASEPOS_Y, 0));

		answer = GUI_radio(title, (signed char)cnt,
				Real2Host(ds_readd(RADIO_NAME_LIST)),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 2 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 3 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 5 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 6 * 4)))) - 1;

		ds_writew(BASEPOS_X, bak_2);
		ds_writew(BASEPOS_Y, bak_3);
		ds_writew(TEXTBOX_WIDTH, textbox_width_bak);

		if (answer != -2)
			return dst.v[answer];
		else
			return -1;
	}

	ds_writew(TEXTBOX_WIDTH, textbox_width_bak);
	return -1;
}

/**
 * \brief   menu for selecting a hero
 *
 * \param   title       titlestring for the menu
 * \return              index of the hero or -1 (ESC).
 * Remark: The available heroes must be in the group and pass check_hero().
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
	signed short textbox_width_bak;
	signed short bak_2;
	signed short bak_3;
	RealPt hero;

	textbox_width_bak = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);
	cnt = 0;

	for (hero = (RealPt)ds_readd(HEROES), i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readb(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readb(Real2Host(hero) + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
			check_hero(Real2Host(hero)) &&
				/* TODO: find out what that means */
				ds_readbs(HERO_SEL_EXCLUDE) != i) {

			/* save pointer to the name of the hero */
			ds_writed(RADIO_NAME_LIST + cnt * 4, (Bit32u)(hero + HERO_NAME2));
			dst.v[cnt] = i;
			cnt++;
		}
	}

	ds_writeb(HERO_SEL_EXCLUDE, -1);

	if (cnt != 0) {
		bak_2 = ds_readw(BASEPOS_X);
		bak_3 = ds_readw(BASEPOS_Y);

		ds_writew(BASEPOS_X, ds_writew(BASEPOS_Y, 0));

		answer = GUI_radio(title, (signed char)cnt,
				Real2Host(ds_readd(RADIO_NAME_LIST)),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 2 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 3 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 5 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 6 * 4)))) - 1;

		ds_writew(BASEPOS_X, bak_2);
		ds_writew(BASEPOS_Y, bak_3);
		ds_writew(TEXTBOX_WIDTH, textbox_width_bak);
		ds_writew(SKILLED_HERO_POS, -1);

		if (answer != -2)
			return dst.v[answer];
		else
			return -1;
	}

	ds_writew(TEXTBOX_WIDTH, textbox_width_bak);
	ds_writew(SKILLED_HERO_POS, -1);
	return -1;
}

/**
 * \brief   menu for selecting a hero
 *
 * \param   title       titlestring for the menu
 * \return              index of the hero or -1 (ESC).
 * Remark: The available heroes must be in the group, pass check_hero() and
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
	signed short textbox_width_bak;
	signed short bak_2;
	signed short bak_3;
	RealPt hero;

	textbox_width_bak = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);
	cnt = 0;

	for (hero = (RealPt)ds_readd(HEROES), i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readb(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readb(Real2Host(hero) + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
			check_hero(Real2Host(hero)) &&
				/* TODO: find out what that means */
				ds_readbs(HERO_SEL_EXCLUDE) != i) {

			/* save pointer to the name of the hero */
			ds_writed(RADIO_NAME_LIST + cnt * 4, (Bit32u)(hero + HERO_NAME2));
			dst.v[cnt] = i;
			cnt++;
		}
	}

	ds_writeb(HERO_SEL_EXCLUDE, -1);

	if (cnt != 0) {
		do {
			bak_2 = ds_readw(BASEPOS_X);
			bak_3 = ds_readw(BASEPOS_Y);

			ds_writew(BASEPOS_X, ds_writew(BASEPOS_Y, 0));

			answer = GUI_radio(title, (signed char)cnt,
				Real2Host(ds_readd(RADIO_NAME_LIST)),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 2 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 3 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 4 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 5 * 4))),
				Real2Host(ds_readd((RADIO_NAME_LIST + 6 * 4)))) - 1;

			ds_writew(BASEPOS_X, bak_2);
			ds_writew(BASEPOS_Y, bak_3);

		} while (answer == -2);

		ds_writew(SKILLED_HERO_POS, -1);
		ds_writew(TEXTBOX_WIDTH, textbox_width_bak);

		return dst.v[answer];
	} else {

		ds_writew(SKILLED_HERO_POS, -1);
		return 0;
	}
}

/**
 * \brief   counts the heroes in the current group
 *
 * \return              how many alive heroes are in the group.
 */
signed short count_heroes_in_group(void)
{
	Bit8u *hero_i;
	signed short i;
	signed short retval;

	retval = 0;

	for (hero_i = get_hero(0), i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
		/* Check class, group and dead */
		if ((host_readb(hero_i + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readb(hero_i + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP)) &&
			(!hero_dead(hero_i))) {

			retval++;
		}
	}

	return retval;
}

/**
 * \brief   let the hero feel the result of too much alcohol
 *
 * \param   hero        the hero
 */
void hero_get_drunken(Bit8u *hero)
{

	if (!host_readbs(hero + HERO_DRUNK)) {

		/* set the hero drunken */
		host_writeb(hero + HERO_DRUNK, 1);

		/* change good attributes */
		add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_MU), 1);
		sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KL), 1);
		sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_CH), 1);
		sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_FF), 1);
		sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), 1);
		add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_IN), 1);
		add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), 1);

		/* Reset bad attributes */
		add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_AG), 1);
		sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_HA), 1);
		sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_RA), 1);
		add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GG), 1);
		sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_TA), 1);
		add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_NG), 1);
		add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_JZ), 1);

		/* do a burp FX2.VOC */
		if (ds_readb(PP20_INDEX) == ARCHIVE_FILE_ZUSTA_UK) {
			ds_writew(REQUEST_REFRESH, 1);
		}

		play_voc_delay(ARCHIVE_FILE_FX2_VOC);
	}
}

/**
 * \brief   makes a drunken hero sober
 *
 * \param   hero        pointer to the hero
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
	sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_MU), 1);
	add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KL), 1);
	add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_CH), 1);
	add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_FF), 1);
	add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), 1);
	sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_IN), 1);
	sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), 1);

	/* Reset bad attributes */
	sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_AG), 1);
	add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_HA), 1);
	add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_RA), 1);
	sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GG), 1);
	add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_TA), 1);
	sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_NG), 1);
	sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_JZ), 1);

	if (ds_readb(PP20_INDEX) == ARCHIVE_FILE_ZUSTA_UK)
		ds_writew(REQUEST_REFRESH, 1);
}

#if !defined(__BORLANDC__)
}
#endif
