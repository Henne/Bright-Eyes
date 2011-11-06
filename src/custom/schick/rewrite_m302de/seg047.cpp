#include <stdio.h>

#include "schick.h"
#include "v302de.h"

#include "seg007.h"
#include "seg097.h"

namespace M302de {

/**
 * get_hero_CH_best - get the index of the hero with the best CH value
 *
 * Returns the index of the hero with the highest unmodified CH value.
 * The hero must be alive and in the current group.
 */
unsigned short get_hero_CH_best() {
	Bit8u *hero_i;
	unsigned short i, retval;
	signed short ch_val = -1;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* check class */
		if (*(hero_i + 0x21) == 0)
			continue;
		/* check if in group */
		if (*(hero_i + 0x87) != ds_readb(0x2d35))
			continue;
		/* check if dead */
		if (*(hero_i + 0xaa) & 1)
			continue;
		/* check if CH is the highest */
		if (*(signed char*)(hero_i + 0x3b) <= ch_val)
			continue;

		ch_val = *(signed char*)(hero_i + 0x3b);
		retval = i;
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
	Bit8u *hero_i;
	unsigned short i, retval;
	signed short kk_val = -1;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* check class */
		if (*(hero_i + 0x21) == 0)
			continue;
		/* check if in group */
		if (*(hero_i + 0x87) != ds_readb(0x2d35))
			continue;
		/* check if dead */
		if (*(hero_i + 0xaa) & 1)
			continue;
		/* check if CH is the highest */
		if (*(signed char*)(hero_i + 0x47) <= kk_val)
			continue;

		kk_val = *(signed char*)(hero_i + 0x47);
		retval = i;
	}

	return retval;
}

/**
 * hero_is_diseased - returns the disease number the hero has
 * @hero:	the hero which should be checked
 *
 * Only the first disease is returned here.
 */
unsigned short hero_is_diseased(Bit8u *hero) {
	unsigned short i;

	for (i = 0; i <= 7; i++)
		if (*(hero + 0xae + i * 5) == 0xff)
			return i;

	return 0;
}

/**
 * hero_is_poisoned - returns the poison number the hero has
 * @hero:	the hero which should be checked
 *
 * Only the first poison is returned here.
 */
unsigned short hero_is_poisoned(Bit8u *hero) {
	unsigned short i;

	for (i = 0; i <= 9; i++)
		if (*(hero + 0xd6 + i * 5) == 0xff)
			return i;

	return 0;
}

/**
 * hero_gets_poisoned - poison a hero
 * @hero:	the hero which gets poisoned
 * @poison:	the kind of poison
 */
void hero_gets_poisoned(Bit8u *hero, unsigned short poison) {

	if (*(hero + 0xaa) & 1)
		return;

	host_writeb(hero + poison * 5 + 0xd6, 0xff);
	host_writeb(hero + poison * 5 + 0xd7, 0x00);
	host_writeb(hero + poison * 5 + 0xd8, 0x00);
	host_writeb(hero + poison * 5 + 0xd9, 0x00);
	host_writeb(hero + poison * 5 + 0xda, 0x00);
}

/**
 * hero_gets_diseased - diseases a hero
 * @hero:	the hero which gets diseased
 * @disease:	the kind of disease
 */
void hero_gets_diseased(Bit8u *hero, unsigned short disease) {

	if (*(hero + 0xaa) & 1)
		return;

	host_writeb(hero + disease * 5 + 0xae, 0xff);
	host_writeb(hero + disease * 5 + 0xaf, 0x00);
	host_writeb(hero + disease * 5 + 0xb0, 0x00);
	host_writeb(hero + disease * 5 + 0xb1, 0x00);
	host_writeb(hero + disease * 5 + 0xb2, 0x00);
}

/**
 * hero_disease_test - the hero may get a disease
 * @hero:	the hero which may gets diseased
 * @disease:	the kind of disease
 * @probability: the probability to get diseased in percent
 */
void hero_disease_test(Bit8u *hero, unsigned short disease, unsigned short probability) {

	/* check the probability */
	if (random_schick(100) > probability)
		return;
	/* check if the hero already has the disease */
	if (host_readb(hero + disease * 5 + 0xae) == 0xff)
		return;

	hero_gets_diseased(hero, disease);
}

/**
 *	check_hero_KK_unused
 *
 *	This function is not used in the game!
 */
short check_hero_KK_unused(short val) {

	PhysPt hero;

	hero = Real2Phys(ds_readd(HEROS));

	if ((short)mem_readb(hero + 0x47) + (short)mem_readb(hero + 0x48) < val)
		return 0;
	else
		return 1;

}

/**
 *	check_heros_KK
 *	@val:	value to compare KK with
 *
 *	This function, like hero_check_KK_unused, is buggy!
 *	It does not check if the first slot is a valid hero.
 */
short check_heros_KK(short val) {

	PhysPt hero;
	signed short sum;

	hero = Real2Phys(ds_readd(HEROS));

	/* Orig-BUG: not checked if hero is valid */
	sum = (short)mem_readb(hero + 0x47) + (short)mem_readb(hero + 0x48);

	hero += 0x6da;

	/* check class, group and dead status of hero in slot 2*/
	if (mem_readb(hero + 0x21) && mem_readb(hero + 0x87) == ds_readb(0x2d35) && !(mem_readb(hero + 0xaa) & 1)) {
		sum += (short)mem_readb(hero + 0x47);
		sum += (short)mem_readb(hero + 0x48);
	}

	D1_INFO("Pruefe KK der ersten beiden Helden (%d) >= %d: ", sum, val);

	if (sum >= val) {
		D1_INFO("gelungen\n");
		return 1;
	}

	D1_INFO("mislungen\n");
	return 0;
}
/**
 *	make_valuta_str	-	makes a valuta string
 *	@dst:	the destination
 *	@money:	the money in Heller
 *
 *	This funcion is buggy.
 */
void make_valuta_str(char *dst, unsigned int money) {
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

	sprintf(dst, (char*)get_ltx(0xbb0), d, s, money);
}

/**
 *	update_atpa() -	recalculates the AT PA values
 *
 */
void update_atpa(Bit8u *hero)
{
	unsigned short modulo, atpa;
	signed short diff;
	unsigned short i;

	/* ATPA base = (IN + KK + GE) / 5 rounded */
	modulo = (hero[0x43] + hero[0x46] + hero[0x40]) % 5;
	atpa = (hero[0x43] + hero[0x46] + hero[0x40]) / 5;

	/* round up */
	if (modulo >= 3)
		atpa++;

	/* calculate difference */
	diff = atpa - host_readb(hero + 0x67);

	if (diff == 0)
		return;

	/* update atpa base value */
	host_writeb(hero + 0x67, (signed char)atpa);

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_ltx(0x20), host_readb(hero + 0x67));

	/* print message */
	GUI_output(Real2Host(ds_readd(0xd2f3)));

	for (i = 0; i < 7; i++) {
		/* add diff to AT value */
		host_writeb(hero + 0x68 + i,
			host_readb(hero + 0x68 + i) + diff);
		/* add diff to PA value */
		host_writeb(hero + 0x6f + i,
			host_readb(hero + 0x6f + i) + diff);
	}
}

/**
 * count_heroes_in_group - counts the heroes in the current group
 */
unsigned short count_heroes_in_group() {
	PhysPt hero_i;
	unsigned short i, retval = 0;

	hero_i = Real2Phys(ds_readd(HEROS));

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* Check class */
		if (mem_readb(hero_i + 0x21) == 0)
			continue;
		/* Check if in current group */
		if (mem_readb(hero_i + 0x87) != ds_readb(0x2d35))
			continue;
                /* Check if hero is dead */
		if (mem_readb(hero_i + 0xaa) & 1)
			continue;

		retval++;
	}

	return retval;
}

/**
 *	hero_get_sober	-	makes a drunken hero sober
 *	@hero:	pointer to the hero
 *
 */
void hero_get_sober(Bit8u *hero) {
	/* This is checked twice */
	/* Is hero drunken ? */
	if (host_readb(hero + 0xa1) == 0)
		return;

	D1_INFO("%s ist wieder nuechtern\n", (char*)hero + 0x10);

	/* set hero sober */
	host_writeb(hero + 0xa1, 0);

	/* Reset good attributes */
	host_writeb(hero + 0x35, host_readb(hero + 0x35) - 1);
	host_writeb(hero + 0x38, host_readb(hero + 0x38) + 1);
	host_writeb(hero + 0x3b, host_readb(hero + 0x3b) + 1);
	host_writeb(hero + 0x3e, host_readb(hero + 0x3e) + 1);
	host_writeb(hero + 0x41, host_readb(hero + 0x41) + 1);
	host_writeb(hero + 0x44, host_readb(hero + 0x44) - 1);
	host_writeb(hero + 0x47, host_readb(hero + 0x47) - 1);

	/* Reset bad attributes */
	host_writeb(hero + 0x4a, host_readb(hero + 0x4a) - 1);
	host_writeb(hero + 0x4d, host_readb(hero + 0x4d) + 1);
	host_writeb(hero + 0x50, host_readb(hero + 0x50) + 1);
	host_writeb(hero + 0x53, host_readb(hero + 0x53) - 1);
	host_writeb(hero + 0x56, host_readb(hero + 0x56) + 1);
	host_writeb(hero + 0x59, host_readb(hero + 0x59) - 1);
	host_writeb(hero + 0x5c, host_readb(hero + 0x5c) - 1);

	if (ds_readb(0x2845) == 20)
		ds_writew(0x2846, 1);
}

}
