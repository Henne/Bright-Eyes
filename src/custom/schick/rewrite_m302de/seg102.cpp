/*
 *      Rewrite of DSA1 v3.02_de functions of seg102 (spells of monsters)
 *      Functions rewritten 5/22
 *
 *      Functions called rewritten 3/20
 *      Functions uncalled rewritten 2/2 (complete)
*/

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg041.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void MON_do_damage(signed short damage)
{
	if (damage > 0) {

		if (host_readbs(get_spelluser_e() + 0x2d) < 10) {
			/* target is a hero */

			/* set the pointer to the target */
			ds_writed(SPELLTARGET,
				(Bit32u)((RealPt)ds_readd(HEROS) + 0x6da * (host_readbs(get_spelluser_e() + 0x2d) - 1)));

			/* do the damage */
			sub_hero_le(get_spelltarget(), damage);

			/* add a fight message */
			FIG_add_msg(8, damage);

			/* check if someone died */
			if (hero_dead(get_spelltarget())) {
				ds_writew(0xe3a6, 1);
			}

		} else {
			/* target is a hero */

			/* set the pointer to the target */
			ds_writed(SPELLTARGET_E,
				(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser_e() + 0x2d) * 62));

			/* do the damage */
			FIG_damage_enemy(get_spelltarget_e(), damage, 1);

			/* add a fight message */
			FIG_add_msg(11, damage);

			/* check if someone died */
			if (enemy_dead(get_spelltarget_e())) {
				ds_writew(0xe3a6, 1);
			}
		}
	}
}

/* Borlandified and identical */
/* unused */
signed short MON_get_target_PA(void)
{
	if (host_readbs(get_spelluser_e() + 0x2d) < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROS) + 0x6da * (host_readbs(get_spelluser_e() + 0x2d) - 1)));

		/* calc and return PA-value */
		return host_readbs(get_spelltarget() + 0x6f + host_readbs(get_spelltarget() + 0x78))
			- host_readbs(get_spelltarget() + 0x79);

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser_e() + 0x2d) * 62));

		/* calc and return PA-value */
		return host_readbs(get_spelltarget_e() + 0x1d);
	}
}

/* Borlandified and identical */
/* unused */
signed short MON_get_target_RS(void)
{
	if (host_readbs(get_spelluser_e() + 0x2d) < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROS) + 0x6da * (host_readbs(get_spelluser_e() + 0x2d) - 1)));

		/* return RS-value */
		return host_readbs(get_spelltarget() + 0x30);

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser_e() + 0x2d) * 62));

		/* return PA-value */
		return host_readbs(get_spelltarget_e() + 0x02);
	}
}

/* Borlandified and identical */
signed short MON_get_val(signed short mspell_nr, signed short flag)
{
	signed char l1;

	l1 = ds_readbs(0xf13 + 8 * mspell_nr);

	if (flag != 0) {

		l1 = (l1 == -1) ? random_interval(5, 10) : l1 / 2;
	}


	return l1;
}


/**
 * MON_test_skill() -	talent test for monsters
 * @monster:		pointer to monster
 * @t1:			nr of 1st attribute
 * @t2:			nr of 2nd attribute
 * @t3:			nr of 3rd attribute
 * @bonus:		modificator
*/

/* Borlandified and identical */
signed short MON_test_attrib3(Bit8u *monster, signed short t1, signed short t2, signed short t3, signed char bonus)
{
	signed short randval;
	signed short attr_sum;

	randval = dice_roll(3, 20, bonus);

	attr_sum = host_readbs(monster + 0x04 + 2 * t1)
		+ host_readbs(monster + 0x04 + 2 * t2)
		+ host_readbs(monster + 0x04 + 2 * t3);

	return attr_sum - randval + 1;
}

#if !defined(__BORLANDC__)
}
#endif
