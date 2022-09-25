/*
 *      Rewrite of DSA1 v3.02_de functions of seg102 (spells of monsters)
 *      Functions rewritten: 22/22 (complete)
 *      Functions called rewritten 20/20 (complete)
 *      Functions uncalled rewritten 2/2 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg102.cpp
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
	mspell_plumbumbarum,		/* 11 */
	mspell_saft_kraft,		/* 12 */
	mspell_armatrutz,		/* 13 */
	mspell_paralue,			/* 14 */
};

#endif

void MON_do_spell_damage(signed short damage)
{
	if (damage > 0) {

		if (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) < 10) {
			/* target is a hero */

			/* set the pointer to the target */
			ds_writed(SPELLTARGET,
				(Bit32u)((RealPt)ds_readd(HEROES) + SIZEOF_HERO * (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) - 1)));

			/* do the damage */
			sub_hero_le(get_spelltarget(), damage);

			/* add a fight message */
			FIG_add_msg(8, damage);

			/* check if someone died */
			if (hero_dead(get_spelltarget())) {
				ds_writew(DEFENDER_DEAD, 1);
			}

		} else {
			/* target is a monster */

			/* set the pointer to the target */
			ds_writed(SPELLTARGET_E,
				(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

			/* do the damage */
			FIG_damage_enemy(get_spelltarget_e(), damage, 1);

			/* add a fight message */
			FIG_add_msg(11, damage);

			/* check if someone died */
			if (enemy_dead(get_spelltarget_e())) {
				ds_writew(DEFENDER_DEAD, 1);
			}
		}
	}
}

/* unused */
signed short MON_get_target_PA(void)
{
	if (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROES) + SIZEOF_HERO * (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) - 1)));

		/* calc and return PA-value */
		return host_readbs(get_spelltarget() + HERO_PA + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE))
			- host_readbs(get_spelltarget() + HERO_ATTACK_TYPE);

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* calc and return PA-value */
		return host_readbs(get_spelltarget_e() + ENEMY_SHEET_PA);
	}
}

/* unused */
signed short MON_get_target_RS(void)
{
	if (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROES) + SIZEOF_HERO * (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) - 1)));

		/* return RS-value */
		return host_readbs(get_spelltarget() + HERO_RS_BONUS1);

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* return PA-value */
		return host_readbs(get_spelltarget_e() + ENEMY_SHEET_RS);
	}
}

signed short MON_get_spell_cost(signed short mspell_no, signed short flag)
{
	signed char cost;

	cost = ds_readbs((MON_SPELL_DESCRIPTIONS + MON_SPELL_DESCRIPTIONS_AE_COST) + SIZEOF_MON_SPELL_DESCRIPTIONS * mspell_no);

	if (flag != 0) {

		cost = (cost == -1) ? random_interval(5, 10) : cost / 2;
	}

	return cost;
}


/**
 * \brief   skill test for monsters
 *
 * \param   monster     pointer to monster
 * \param   attrib1     no of 1st attribute
 * \param   attrib2     no of 2nd attribute
 * \param   attrib3     no of 3rd attribute
 * \param   handicap    may be positive or negative. The higher the value, the harder the test.
 */
signed short MON_test_attrib3(Bit8u *monster, signed short attrib1, signed short attrib2, signed short attrib3, signed char handicap)
/* called only from a single position, in MON_test_skill(..) */
{
#ifndef M302de_FEATURE_MOD
	/* Feature mod 6: The implementation of the skill test logic differs from the original DSA2/3 rules.
	 * It is sometimes called the 'pool' variant, where '3W20 + handicap' is compared to the sum of the attributes.
	 * It is significantly easier than the original rule, where each individuall roll must be at most the corresponding attribute,
	 * where positive handicap must be used up during the process, and negative handicap may be used for compensation. */

	signed short randval;
	signed short attr_sum;

	randval = dice_roll(3, 20, handicap);

	attr_sum = host_readbs(monster + ENEMY_SHEET_ATTRIB + 2 * attrib1)
		+ host_readbs(monster + ENEMY_SHEET_ATTRIB + 2 * attrib2)
		+ host_readbs(monster + ENEMY_SHEET_ATTRIB + 2 * attrib3);

	return attr_sum - randval + 1;
#else
	/* Here, the original DSA2/3 skill test logic is implemented.
	 * WARNING: This makes skill tests and spell casting (on both sides), and thus the game, significantly harder!
	 * Note that we are not implementing the DSA4 rules, where tests with a positive handicap are yet harder. */
	signed short i;
	signed short tmp;
	signed short nr_rolls_1 = 0;
	signed short nr_rolls_20 = 0;
	signed short fail = 0;
	signed char attrib [3];

	attrib[0] = host_readbs(monster + ENEMY_SHEET_ATTRIB + 2 * attrib1);
	attrib[1] = host_readbs(monster + ENEMY_SHEET_ATTRIB + 2 * attrib2);
	attrib[2] = host_readbs(monster + ENEMY_SHEET_ATTRIB + 2 * attrib3);

#if !defined(__BORLANDC__)
	D1_INFO(" (%s %d/%s %d/%s %d) ->",
		names_attrib[attrib1],
		attrib[0],
		names_attrib[attrib2],
		attrib[1],
		names_attrib[attrib3],
		attrib[2]
	);
#endif

	for (i = 0; i < 3; i++) {

		tmp = random_schick(20);

#if !defined(__BORLANDC__)
		D1_INFO(" W20 = %d;", tmp);
#endif

		if (tmp == 20) {
			if (++nr_rolls_20 == 2) {
#if !defined(__BORLANDC__)
				D1_INFO(" -> UNGLUECKLICH nicht bestanden\n");
#endif
				return -99;
			}
		}

		if (tmp == 1) {
			if (++nr_rolls_1 == 2) {
#if !defined(__BORLANDC__)
				D1_INFO(" -> GLUECKLICH bestanden\n");
#endif
				return 99;
			}
		}

		if (!fail) {
			tmp -= attrib[i];
			if (handicap <= 0) {
				if (tmp > 0) {
					if (tmp > -handicap) {
						fail = 1;
#if !defined(__BORLANDC__)
						D1_INFO(" zu hoch!");
#endif
					} else  {
						handicap += tmp;
					}
				}
			}
			if (handicap > 0) {
				if (tmp > 0) {
					fail = 1;
#if !defined(__BORLANDC__)
					D1_INFO(" zu hoch!");
#endif
				} else {
					handicap += tmp;
					if (handicap < 0) {
						handicap = 0;
					}
				}
			}
		}
	}
	if (fail || (handicap > 0)) {
#if !defined(__BORLANDC__)
		D1_INFO(" -> nicht bestanden.\n");
#endif
		return 0;
	} else {
#if !defined(__BORLANDC__)
		D1_INFO(" -> bestanden mit %d.\n",-handicap);
#endif
		return 1 - handicap;
	}
#endif
}

signed short MON_test_skill(Bit8u *monster, signed short mspell_no, signed char handicap)
/* called only from a single position, in MON_cast_spell(..) */
{
	Bit8u *desc;

	desc = p_datseg + MON_SPELL_DESCRIPTIONS + SIZEOF_MON_SPELL_DESCRIPTIONS * mspell_no;

	/* depends on MR */
	if (host_readbs(desc + MON_SPELL_DESCRIPTIONS_VS_MR) != 0) {

		/* add MR */
		handicap += (host_readbs(monster + ENEMY_SHEET_ENEMY_ID) >= 10) ?
			ds_readbs(((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_MR) + SIZEOF_ENEMY_SHEET * host_readbs(monster + ENEMY_SHEET_ENEMY_ID)) :
			host_readbs(get_hero(host_readbs(monster + ENEMY_SHEET_ENEMY_ID) - 1) + HERO_MR);
	}

	/* check if the monster spell has a valid ID */
	if ((mspell_no >= 1) && (mspell_no <= 14)) {
#if !defined(__BORLANDC__)
		D1_INFO("Gegnerischer Zauber %s Probe %+d",names_mspell[mspell_no], handicap);
#endif
		/* TODO: balancing problem: enemy spells are always cast with skill value 0 */
		return MON_test_attrib3(monster, host_readbs(desc + MON_SPELL_DESCRIPTIONS_ATTRIB1),
			host_readbs(desc + MON_SPELL_DESCRIPTIONS_ATTRIB2), host_readbs(desc + MON_SPELL_DESCRIPTIONS_ATTRIB3),
			handicap);
	}

	return 0;
}

void MON_sub_ae(Bit8u *monster, signed short ae)
{
	if (!enemy_dead(monster)) {
		sub_ptr_ws(monster + ENEMY_SHEET_AE, ae);

		if (host_readws(monster + ENEMY_SHEET_AE) < 0) {
			host_writew(monster + ENEMY_SHEET_AE, 0);
		}
	}
}

signed short MON_cast_spell(RealPt monster, signed char handicap)
{
	signed short l_si;
	signed short l_di;
	signed short cost;
	void (*func)(void);
	volatile signed short tx_file_bak;

	l_si = host_readbs(Real2Host(monster) + ENEMY_SHEET_CUR_SPELL);

	if (l_si > 0) {

		cost = MON_get_spell_cost(l_si, 0);

		/* check AE */
		if (host_readws(Real2Host(monster) + ENEMY_SHEET_AE) < cost) {
			return -1;
		}

		ds_writew(SPELLTEST_RESULT, MON_test_skill(Real2Host(monster), l_si, handicap));

		if ((ds_readws(SPELLTEST_RESULT) <= 0) || (ds_readds(INGAME_TIMERS) > 0)) {

			/* spell failed */
			MON_sub_ae(Real2Host(monster), MON_get_spell_cost(l_si, 1));

			return 0;

		} else {

			ds_writed(SPELLUSER_E, (Bit32u)monster);

			ds_writew(MONSTER_SPELL_AE_COST, -1);

			/* terminate output string */
			host_writeb(Real2Host(ds_readd(DTP2)), 0);

			tx_file_bak = ds_readws(TX_FILE_INDEX);

			load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

#if !defined(__BORLANDC__)
			func = mspell[l_si];
#else
			func = (void (*)(void))ds_readd(MON_SPELL_HANDLERS + 4 * l_si);
#endif

			func();

			if ((tx_file_bak != -1) && (tx_file_bak != ARCHIVE_FILE_SPELLTXT_LTX)) {
				load_tx(tx_file_bak);
			}

			l_di = 1;

			if (ds_readws(MONSTER_SPELL_AE_COST) == 0) {
				l_di = -1;
			} else if (ds_readws(MONSTER_SPELL_AE_COST) == -2) {
				MON_sub_ae(Real2Host(monster), MON_get_spell_cost(l_si, 1));
				l_di = 0;
			} else if (ds_readws(MONSTER_SPELL_AE_COST) != -1) {
				MON_sub_ae(Real2Host(monster), ds_readws(MONSTER_SPELL_AE_COST));
			} else {
				MON_sub_ae(Real2Host(monster), cost);
			}

			return l_di;
		}
	} else {
		return 0;
	}

}

void mspell_verwandlung(void)
{
	/* set pointer to monster target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)RealMake(datseg, host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

	if (enemy_petrified(get_spelltarget_e())) {

		/* set the spellcosts */
		ds_writew(MONSTER_SPELL_AE_COST, 5 * random_schick(10));

		if (host_readws(get_spelluser_e() + ENEMY_SHEET_AE) < ds_readws(MONSTER_SPELL_AE_COST)) {
			/* if not enough AE, all AE will be consumed, without further effect */
			ds_writew(MONSTER_SPELL_AE_COST, host_readws(get_spelluser_e() + ENEMY_SHEET_AE));
		} else {
			and_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS1, 0xfb); /* unset 'petrified' flag */

			/* prepare message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(114),
				Real2Host(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1)));
		}
	} else if (enemy_mushroom(get_spelltarget_e())) {

		/* set the spellcosts */
		ds_writew(MONSTER_SPELL_AE_COST, 5 * random_schick(10));

		if (host_readws(get_spelluser_e() + ENEMY_SHEET_AE) < ds_readws(MONSTER_SPELL_AE_COST)) {
			/* if not enough AE, all AE will be consumed, without further effect */
			ds_writew(MONSTER_SPELL_AE_COST, host_readws(get_spelluser_e() + ENEMY_SHEET_AE));
		} else {
			and_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS1, 0xbf); /* unset 'mushroom' flag */

			ds_writew(MSPELL_AWAKE_FLAG, 1);
		}
	} else {
		ds_writew(MONSTER_SPELL_AE_COST, 2);
	}
}

void mspell_bannbaladin(void)
{
	/* set pointer to hero target */
	ds_writed(SPELLTARGET,
                        (Bit32u)((RealPt)ds_readd(HEROES) + (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) - 1) * SIZEOF_HERO));

	/* set the flag */
	or_ptr_bs(get_spelltarget() + HERO_FLAGS2, 0x08); /* set 'tame' flag */

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx(115),
		get_spelltarget() + HERO_NAME2);
}

void mspell_boeser_blick(void)
{
	/* set pointer to hero target */
	ds_writed(SPELLTARGET,
                        (Bit32u)((RealPt)ds_readd(HEROES) + (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) - 1) * SIZEOF_HERO));

	/* set the flag */
	or_ptr_bs(get_spelltarget() + HERO_FLAGS1, 0x20); /* set 'renegade' flag */

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx(116),
		get_spelltarget() + HERO_NAME2);
}

void mspell_horriphobus(void)
{
	/* set pointer to hero target */
	ds_writed(SPELLTARGET,
                        (Bit32u)((RealPt)ds_readd(HEROES) + (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) - 1) * SIZEOF_HERO));

	/* set the flag */
	or_ptr_bs(get_spelltarget() + HERO_FLAGS2, 0x01); /* set 'scared' flag */
	and_ptr_bs(get_spelltarget() + HERO_FLAGS1, 0xdf); /* unset 'renegade' flag */

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx(117),
		get_spelltarget() + HERO_NAME2);
}

void mspell_axxeleratus(void)
{
	/* set pointer to monster target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)RealMake(datseg, host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

	/* #Attacks + 1 */
	inc_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_ATTACKS);

	/* AT + 1 */
	add_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_AT, 1);

	/* PA + 1 */
	add_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_PA, 1);

	/* BP * 2 */
	host_writebs(get_spelltarget_e() + ENEMY_SHEET_BP, 2 * host_readbs(get_spelltarget_e() + ENEMY_SHEET_BP));

}

void mspell_balsam(void)
{
	signed short le;

	/* set pointer to monster target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)RealMake(datseg, host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug:
	 * very strange behaviour.
	 * If LE loss is less then 14, 7 LE will be healed, price AE = LE.
	 * If not enouh AE available, number of healed LE is lowered accordingly.
	 *
	 * Problem 1: For LE loss < 7, LE may be healed above max.
	 * Problem 2: For LE loss >=14, no effect. No healing.
	 *
	 * Hard to guess what the intended behaviour was. */

	ds_writew(MONSTER_SPELL_AE_COST, 0);

	le = (host_readws(get_spelltarget_e() + ENEMY_SHEET_LE_ORIG) - host_readws(get_spelltarget_e() + ENEMY_SHEET_LE)) / 2; /* half of the missing LE */

	if (le) {
		if (le < 7) {
			/* AE costs are at least 7 */
			ds_writew(MONSTER_SPELL_AE_COST, 7);
		}
		if (host_readws(get_spelluser_e() + ENEMY_SHEET_AE) < ds_readws(MONSTER_SPELL_AE_COST)) {
			/* not enough AE: heal only that many LE as the spellcaster has AE available */
			ds_writew(MONSTER_SPELL_AE_COST, host_readws(get_spelluser_e() + ENEMY_SHEET_AE));
		}
		add_ptr_ws(get_spelltarget_e() + ENEMY_SHEET_LE, ds_readws(MONSTER_SPELL_AE_COST));
	}
#else
	/* Fix:
	 * Let's do it this way:
	 * Heal (missing LE)/2 LE, but at least 7, as long as it won't be healed above max.
	 * If not enouh AE available, number of healed LE is lowered accordingly. */
	le = host_readws(get_spelltarget_e() + ENEMY_SHEET_LE_ORIG) - host_readws(get_spelltarget_e() + ENEMY_SHEET_LE); /* missing LE */
	if (le >= 16) {
		le /= 2;
	} else if (le > 7) {
		le = 7;
	}
	if (host_readws(get_spelluser_e() + ENEMY_SHEET_AE) < le) {
		le = host_readws(get_spelluser_e() + ENEMY_SHEET_AE);
	}
	ds_writew(MONSTER_SPELL_AE_COST, le);
	add_ptr_ws(get_spelltarget_e() + ENEMY_SHEET_LE, le);
#endif
}

void mspell_blitz(void)
{
	if (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROES) + SIZEOF_HERO * (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) - 1)));

		/* set blitz timer to 3 rounds */
		host_writeb(get_spelltarget() + HERO_BLIND, 3);

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(86),
			get_spelltarget() + HERO_NAME2);
	} else {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* set blitz timer to 3 rounds */
		host_writeb(get_spelltarget_e() + ENEMY_SHEET_BLIND, 3);

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(85),
			Real2Host(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1)));
	}
}

void mspell_eisenrost(void)
{
	signed short id;

	if (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROES) + SIZEOF_HERO * (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) - 1)));

		id = host_readws(get_spelltarget() + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID);

		if (!id) {
			/* target hero has no weapon */
			ds_writew(MONSTER_SPELL_AE_COST, 2);
		} else if (!inventory_broken(get_spelltarget() + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY)) {

			if (host_readbs(get_spelltarget() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_BF)) > 0) {

				or_ptr_bs(get_spelltarget() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_FLAGS), 1); /* set 'broken' flag */

				/* prepare message */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(92),
					Real2Host(GUI_names_grammar((signed short)0x8000, id, 0)),
					get_spelltarget() + HERO_NAME2);
			} else {
				ds_writew(MONSTER_SPELL_AE_COST, -2);
			}
		}

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* if weapon is not broken */
		if (!host_readbs(get_spelltarget_e() + ENEMY_SHEET_BROKEN)) {

			/* set weapon broken */
			host_writebs(get_spelltarget_e() + ENEMY_SHEET_BROKEN, 1);

			/* prepare message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(91),
				Real2Host(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1)));
		}
	}
}

void mspell_fulminictus(void)
{
	signed short damage;

	/* roll 3W6 */
	damage = dice_roll(3, 6, 0);

	/* add the level of the spelluser */
	damage += host_readbs(get_spelluser_e() + ENEMY_SHEET_LEVEL);

	/* adjust damage */
	if (host_readws(get_spelluser_e() + ENEMY_SHEET_AE) < damage) {
		damage =  host_readws(get_spelluser_e() + ENEMY_SHEET_AE);
	}

	/* do the damage */
	MON_do_spell_damage(damage);

	/* set the cost */
	ds_writew(MONSTER_SPELL_AE_COST, damage);
}

void mspell_ignifaxius(void)
{
	signed short damage;
	signed short level;
	signed short rs_malus;
	signed short hero_pos;
	signed short slot;
	Bit8u *p_armour;

	/* get the level of the spelluser */
	level = host_readbs(get_spelluser_e() + ENEMY_SHEET_LEVEL);

	/* roll the damage: damage = level * W6 */
	damage = dice_roll(level, 6, 0);

	/* damage must not exceed AE of the spelluser */
	if (host_readws(get_spelluser_e() + ENEMY_SHEET_AE) < damage) {
		damage = host_readws(get_spelluser_e() + ENEMY_SHEET_AE);
	}

	/* do the damage */
	MON_do_spell_damage(damage);

	/* calc RS malus */
	rs_malus = damage / 10;

	if (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) < 10) {
		/* target is a hero */

		/* get the position of the target hero */
		hero_pos = host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) - 1;

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROES) + SIZEOF_HERO * hero_pos));

		/* pointer to the armour of the target hero */
		p_armour = get_spelltarget() + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY;

		if ((host_readws(p_armour + INVENTORY_ITEM_ID) != ITEM_NONE) && (rs_malus != 0)) {

			/* adjust rs_malus such that the RS of the worn body armour won't be negative */
			if ((host_readbs(p_armour + INVENTORY_RS_LOST) + rs_malus) > ds_readbs(ARMORS_TABLE + 2 * host_readbs(4 + get_itemsdat(host_readws(p_armour + INVENTORY_ITEM_ID)))))
			{
				rs_malus = ds_readbs(ARMORS_TABLE + 2 * host_readbs(4 + get_itemsdat(host_readws(p_armour + INVENTORY_ITEM_ID))))
						- host_readbs(p_armour + INVENTORY_RS_LOST);
			}

			host_writeb(p_armour + INVENTORY_RS_LOST, host_readbs(p_armour + INVENTORY_RS_LOST) + rs_malus);
			host_writeb(get_spelltarget()  + HERO_RS_BONUS1, host_readbs(get_spelltarget() + HERO_RS_BONUS1) - rs_malus);
		}

		/* AT - level / 2 */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1), get_spelltarget() + HERO_AT + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE), -level / 2, (signed char)hero_pos);

		/* PA - level / 2 */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1), get_spelltarget() + HERO_PA + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE), -level / 2, (signed char)hero_pos);

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* subtract RS malus */
		host_writeb(get_spelltarget_e() + ENEMY_SHEET_RS,
			host_readbs(get_spelltarget_e() + ENEMY_SHEET_RS) - rs_malus);

		/* AT - level / 2 */
		host_writeb(get_spelltarget_e() + ENEMY_SHEET_AT,
			host_readbs(get_spelltarget_e() + ENEMY_SHEET_AT) - level / 2);

		/* PA - level / 2 */
		host_writeb(get_spelltarget_e() + ENEMY_SHEET_PA,
			host_readbs(get_spelltarget_e() + ENEMY_SHEET_PA) - level / 2);
	}

	/* terminate output string */
	host_writebs(Real2Host(ds_readd(DTP2)), 0);
	ds_writew(MONSTER_SPELL_AE_COST, damage);
}

void mspell_plumbumbarum(void)
{
	signed short slot;
	signed short hero_pos;

	if (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) < 10) {
		/* target is a hero */

		/* get the position of the target hero */
		hero_pos = host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) - 1;

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROES) + SIZEOF_HERO * hero_pos));

		/* AT - 3 */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1), get_spelltarget() + HERO_AT + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE), -3, (signed char)hero_pos);

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(94),
			get_spelltarget() + HERO_NAME2);
	} else {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* AT - 3 */
		host_writeb(get_spelltarget_e() + ENEMY_SHEET_AT,
			host_readbs(get_spelltarget_e() + ENEMY_SHEET_AT) - 3);

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(95),
			Real2Host(GUI_names_grammar((signed short)0x8001, host_readbs(get_spelltarget_e()), 1)));
	}
}

void mspell_saft_kraft(void)
{

	/* set the pointer to the target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

	/* AT + 5 */
	host_writeb(get_spelltarget_e() + ENEMY_SHEET_AT,
		host_readbs(get_spelltarget_e() + ENEMY_SHEET_AT) + 5);

	/* PA - 5 */
	host_writeb(get_spelltarget_e() + ENEMY_SHEET_PA,
		host_readbs(get_spelltarget_e() + ENEMY_SHEET_PA) - 5);

	/* damage + 5 */
	host_writeb(get_spelltarget_e() + ENEMY_SHEET_SAFTKRAFT,
		host_readbs(get_spelltarget_e() + ENEMY_SHEET_SAFTKRAFT) + 5);

	/* set spellcost */
	ds_writew(MONSTER_SPELL_AE_COST, random_schick(20));
}

void mspell_armatrutz(void)
{
	signed short i;
	signed short rs_bonus;

	i = 0;
	while ((i * i) < host_readws(get_spelluser_e() + ENEMY_SHEET_AE)) {
		i++;
	}

	rs_bonus = random_interval(1, i);

	/* set spellcost */
	ds_writew(MONSTER_SPELL_AE_COST, rs_bonus * rs_bonus);

	/* RS + rs_bonus */
	host_writebs(get_spelluser_e() + ENEMY_SHEET_RS,
		host_readbs(get_spelluser_e() + ENEMY_SHEET_RS) + (signed char)rs_bonus);
}

void mspell_paralue(void)
{

	if (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) >= 10) {
		/* target is a monster */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* set the flag */
		or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS1, 0x04); /* set 'petrified' flag */

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(103),
			Real2Host(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1)));
	} else {
		/* target is a hero */

		/* set the pointer to the target */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROES) + SIZEOF_HERO * (host_readbs(get_spelluser_e() + ENEMY_SHEET_ENEMY_ID) - 1)));

		/* set the flag */
		or_ptr_bs(get_spelltarget() + HERO_FLAGS1, 0x04); /* set 'petrified' flag */

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(103),
			get_spelltarget() + HERO_NAME2);
	}
}

#if !defined(__BORLANDC__)
}
#endif
