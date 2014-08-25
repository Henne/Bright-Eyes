/*
 *      Rewrite of DSA1 v3.02_de functions of seg102 (spells of monsters)
 *      Functions rewritten 18/22
 *
 *      Functions called rewritten 16/20
 *      Functions uncalled rewritten 2/2 (complete)
*/

#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg041.h"
#include "seg096.h"
#include "seg102.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if !defined(__BORLANDC__)
static void (*mspell[])(void) = {
	NULL,
	mspell_verwandlung,		/*  1 */
	mspell_bannbaladin,		/*  2 */
	mspell_boeser_blick,		/*  3 */
	mspell_horriphobus,		/*  4 */
	mspell_axxeleratus,		/*  5 */
	mspell_balsam,			/*  6 */
	mspell_blitz,			/*  7 */
	mspell_eisenrost,		/*  8 */
	mspell_fulminictus,		/*  9 */
	mspell_ignifaxius,		/* 10 */
};

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
signed short MON_get_spell_cost(signed short mspell_nr, signed short flag)
{
	signed char cost;

	cost = ds_readbs(0xf13 + 8 * mspell_nr);

	if (flag != 0) {

		cost = (cost == -1) ? random_interval(5, 10) : cost / 2;
	}

	return cost;
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

/* Borlandified and identical */
signed short MON_test_skill(Bit8u *monster, signed short mspell_nr, signed char bonus)
{
	Bit8u *desc;

	desc = p_datseg + 0xf13 + 8 * mspell_nr;

	/* depends on MR */
	if (host_readbs(desc + 6) != 0) {

		/* add MR */
		bonus += (host_readbs(monster + 0x2d) >= 10) ?
			ds_readbs(0xd0f8 + 62 * host_readbs(monster + 0x2d)) :
			host_readbs(get_hero(host_readbs(monster + 0x2d) - 1) + 0x66);
	}

	/* check if the monster spell has a valid ID */
	if ((mspell_nr >= 1) && (mspell_nr <= 14)) {
		return MON_test_attrib3(monster, host_readbs(desc + 3),
			host_readbs(desc + 4), host_readbs(desc + 5),
			bonus);
	}

	return 0;
}

/* Borlandified and identical */
void MON_sub_ae(Bit8u *monster, signed short ae)
{
	if (!enemy_dead(monster)) {
		sub_ptr_ws(monster + 0x17, ae);

		if (host_readws(monster + 0x17) < 0) {
			host_writew(monster + 0x17, 0);
		}
	}
}

/* Borlandified and identical */
signed short MON_cast_spell(RealPt monster, signed char bonus)
{
	signed short l_si;
	signed short l_di;
	signed short cost;
	void (*func)(void);
	volatile signed short bak;

	l_si = host_readbs(Real2Host(monster) + 0x2c);

	if (l_si > 0) {

		cost = MON_get_spell_cost(l_si, 0);

		/* check AE */
		if (host_readws(Real2Host(monster) + 0x17) < cost) {
			return -1;
		}

		ds_writew(0xe5b2, MON_test_skill(Real2Host(monster), l_si, bonus));

		if ((ds_readws(0xe5b2) <= 0) || (ds_readds(0x2dc4) > 0)) {

			/* spell failed */
			MON_sub_ae(Real2Host(monster), MON_get_spell_cost(l_si, 1));

			return 0;

		} else {

			ds_writed(SPELLUSER_E, (Bit32u)monster);

			ds_writew(0xaccc, -1);

			/* terminate output string */
			host_writeb(Real2Host(ds_readd(DTP2)), 0);

			bak = ds_readws(0x26bf);

			load_buffer_1(222);

#if !defined(__BORLANDC__)
			func = mspell[l_si];
#else
			func = (void (*)(void))ds_readd(0xfc2 + 4 * l_si);
#endif

			func();

			if ((bak != -1) && (bak != 222)) {
				load_buffer_1(bak);
			}

			l_di = 1;

			if (ds_readws(0xaccc) == 0) {
				l_di = -1;
			} else if (ds_readws(0xaccc) == -2) {
				MON_sub_ae(Real2Host(monster), MON_get_spell_cost(l_si, 1));
				l_di = 0;
			} else if (ds_readws(0xaccc) != -1) {
				MON_sub_ae(Real2Host(monster), ds_readws(0xaccc));
			} else {
				MON_sub_ae(Real2Host(monster), cost);
			}

			return l_di;
		}
	} else {
		return 0;
	}

}

/* Borlandified and identical */
void mspell_verwandlung(void)
{
	/* set pointer to monster target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)RealMake(datseg, host_readbs(get_spelluser_e() + 0x2d) * 0x3e + 0xd0df));

	if (enemy_stoned(get_spelltarget_e())) {

		/* set the spellcosts */
		ds_writew(0xaccc, 5 * random_schick(10));

		if (host_readws(get_spelluser_e() + 0x17) < ds_readws(0xaccc)) {
			ds_writew(0xaccc, host_readws(get_spelluser_e() + 0x17));
		} else {
			/* unset stoned flag */
			and_ptr_bs(get_spelltarget_e() + 0x31, 0xfb);

			/* prepare message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x1c8),
				Real2Host(GUI_names_grammar(0x8000, host_readbs(get_spelltarget_e()), 1)));
		}
	} else {
		if (enemy_uncon(get_spelltarget_e())) {

			/* set the spellcosts */
			ds_writew(0xaccc, 5 * random_schick(10));

			if (host_readws(get_spelluser_e() + 0x17) < ds_readws(0xaccc)) {
				ds_writew(0xaccc, host_readws(get_spelluser_e() + 0x17));
			} else {
				/* unset uncon flag */
				and_ptr_bs(get_spelltarget_e() + 0x31, 0xbf);

				ds_writew(0x618e, 1);
			}
		} else {
			ds_writew(0xaccc, 2);
		}
	}
}

/* Borlandified and identical */
void mspell_bannbaladin(void)
{
	/* set pointer to hero target */
	ds_writed(SPELLTARGET,
                        (Bit32u)((RealPt)ds_readd(HEROS) + (host_readbs(get_spelluser_e() + 0x2d) - 1) * 0x6da));

	/* set the flag */
	or_ptr_bs(get_spelltarget() + 0xab, 0x08);

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x1cc),
		get_spelltarget() + 0x10);
}

/* Borlandified and identical */
void mspell_boeser_blick(void)
{
	/* set pointer to hero target */
	ds_writed(SPELLTARGET,
                        (Bit32u)((RealPt)ds_readd(HEROS) + (host_readbs(get_spelluser_e() + 0x2d) - 1) * 0x6da));

	/* set the flag */
	or_ptr_bs(get_spelltarget() + 0xaa, 0x20);

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x1d0),
		get_spelltarget() + 0x10);
}

/* Borlandified and identical */
void mspell_horriphobus(void)
{
	/* set pointer to hero target */
	ds_writed(SPELLTARGET,
                        (Bit32u)((RealPt)ds_readd(HEROS) + (host_readbs(get_spelluser_e() + 0x2d) - 1) * 0x6da));

	/* set the flag */
	or_ptr_bs(get_spelltarget() + 0xab, 0x01);
	and_ptr_bs(get_spelltarget() + 0xaa, 0xdf);

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x1d4),
		get_spelltarget() + 0x10);
}

/* Borlandified and identical */
void mspell_axxeleratus(void)
{
	/* set pointer to monster target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)RealMake(datseg, host_readbs(get_spelluser_e() + 0x2d) * 0x3e + 0xd0df));

	/* #Attacks + 1 */
	inc_ptr_bs(get_spelltarget_e() + 0x1b);

	/* AT +1 */
	add_ptr_bs(get_spelltarget_e() + 0x1c, 1);

	/* PA +1 */
	add_ptr_bs(get_spelltarget_e() + 0x1d, 1);

	/* BP * 2 */
	host_writebs(get_spelltarget_e() + 0x23, 2 * host_readbs(get_spelltarget_e() + 0x23));

}

/* Borlandified and identical */
/* Original-Bug: how much LE are restored, when le is 7? */
void mspell_balsam(void)
{
	signed short le;

	/* set pointer to monster target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)RealMake(datseg, host_readbs(get_spelluser_e() + 0x2d) * 0x3e + 0xd0df));

	ds_writew(0xaccc, 0);

	le = (host_readws(get_spelltarget_e() + 0x11) - host_readws(get_spelltarget_e() + 0x13)) / 2;

	if (le) {

		if (le < 7) {
			/* AE costs are at least 7 */
			ds_writew(0xaccc, 7);
		}

		/* adjust costs with */
		if (host_readws(get_spelluser_e() + 0x17) < ds_readws(0xaccc)) {

			ds_writew(0xaccc, host_readws(get_spelluser_e() + 0x17));
		}

		add_ptr_ws(get_spelltarget_e() + 0x13, ds_readws(0xaccc));
	}
}


/* Borlandified and identical */
void mspell_blitz(void)
{
	if (host_readbs(get_spelluser_e() + 0x2d) < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROS) + 0x6da * (host_readbs(get_spelluser_e() + 0x2d) - 1)));

		/* set blitz timer to 3 rounds */
		host_writeb(get_spelltarget() + 0x96, 3);

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x158),
			get_spelltarget() + 0x10);
	} else {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser_e() + 0x2d) * 62));

		/* set blitz timer to 3 rounds */
		host_writeb(get_spelltarget_e() + 0x2f, 3);

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x154),
			Real2Host(GUI_names_grammar(0x8000, host_readbs(get_spelltarget_e()), 1)));
	}
}

/* Borlandified and identical */
void mspell_eisenrost(void)
{
	signed short id;

	if (host_readbs(get_spelluser_e() + 0x2d) < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROS) + 0x6da * (host_readbs(get_spelluser_e() + 0x2d) - 1)));

		id = host_readws(get_spelltarget() + 0x1c0);

		if (!id) {
			/* target hero has no weapon */
			ds_writew(0xaccc, 2);
		} else if (!ks_broken(get_spelltarget() + 0x1c0)) {

			if (host_readbs(get_spelltarget() + 0x1c6) > 0) {

				/* set the broken flag */
				or_ptr_bs(get_spelltarget() + 0x1c4, 1);

				/* prepare message */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x170),
					Real2Host(GUI_names_grammar(0x8000, id, 0)),
					get_spelltarget() + 0x10);
			} else {
				ds_writew(0xaccc, -2);
			}
		}

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser_e() + 0x2d) * 62));

		/* if weapon is not broken */
		if (!host_readbs(get_spelltarget_e() + 0x30)) {

			/* set weapon broken */
			host_writebs(get_spelltarget_e() + 0x30, 1);

			/* prepare message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x16c),
				Real2Host(GUI_names_grammar(0x8000, host_readbs(get_spelltarget_e()), 1)));
		}
	}
}

/* Borlandified and identical */
void mspell_fulminictus(void)
{
	signed short damage;

	/* roll 3W6 */
	damage = dice_roll(3, 6, 0);

	/* add the level of the spelluser */
	damage += host_readbs(get_spelluser_e() + 0x29);

	/* adjust damage */
	if (host_readws(get_spelluser_e() + 0x17) < damage) {
		damage =  host_readws(get_spelluser_e() + 0x17);
	}

	/* do the damage */
	MON_do_damage(damage);

	/* set the cost */
	ds_writew(0xaccc, damage);
}

/* Borlandified and identical */
void mspell_ignifaxius(void)
{
	signed short damage;
	signed short level;
	signed short rs_malus;
	signed short hero_pos;
	signed short slot;
	Bit8u *p_armour;

	/* get the level of the spelluser */
	level = host_readbs(get_spelluser_e() + 0x29);

	/* roll the damage: damage = level * W6 */
	damage = dice_roll(level, 6, 0);

	/* damage must not exceed AE of the spelluser */
	if (host_readws(get_spelluser_e() + 0x17) < damage) {
		damage = host_readws(get_spelluser_e() + 0x17);
	}

	/* do the damage */
	MON_do_damage(damage);

	/* calc RS malus */
	rs_malus = damage / 10;

	if (host_readbs(get_spelluser_e() + 0x2d) < 10) {
		/* target is a hero */

		/* get the position of the target hero */
		hero_pos = host_readbs(get_spelluser_e() + 0x2d) - 1;

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROS) + 0x6da * hero_pos));

		/* pointer to the armour of the target hero */
		p_armour = get_spelltarget() + 0x1b2;

		if ((host_readws(p_armour) != 0) && (rs_malus != 0)) {

			/* adjust rs_malus */
			if ((host_readbs(p_armour + 7) + rs_malus) > ds_readbs(0x877 + 2 * host_readbs(4 + get_itemsdat(host_readws(p_armour)))))
			{
				rs_malus = ds_readbs(0x877 + 2 * host_readbs(4 + get_itemsdat(host_readws(p_armour))))
						- host_readbs(p_armour + 7);
			}

			host_writeb(p_armour + 7, host_readbs(p_armour + 7) + rs_malus);
			host_writeb(get_spelltarget()  + 0x30, host_readbs(get_spelltarget() + 0x30) - rs_malus);
		}

		/* AT - level / 2 */
		slot = get_free_mod_slot();
		set_mod_slot(slot, 0x1518, get_spelltarget() + 0x68 + host_readbs(get_spelltarget() + 0x78), -level / 2, hero_pos);

		/* PA - level / 2 */
		slot = get_free_mod_slot();
		set_mod_slot(slot, 0x1518, get_spelltarget() + 0x6f + host_readbs(get_spelltarget() + 0x78), -level / 2, hero_pos);

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser_e() + 0x2d) * 62));

		/* subtract RS malus */
		sub_ptr_bs(get_spelltarget_e() + 0x02, rs_malus);

		/* AT - level / 2 */
		host_writeb(get_spelltarget_e() + 0x1c,
			host_readbs(get_spelltarget_e() + 0x1c) - level / 2);

		/* PA - level / 2 */
		host_writeb(get_spelltarget_e() + 0x1d,
			host_readbs(get_spelltarget_e() + 0x1d) - level / 2);
	}

	/* terminate output string */
	host_writebs(Real2Host(ds_readd(DTP2)), 0);
	ds_writew(0xaccc, damage);
}

#if !defined(__BORLANDC__)
}
#endif
