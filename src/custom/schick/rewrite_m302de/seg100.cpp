/*
 *	Rewrite of DSA1 v3.02_de functions of seg100 (spells 2/3)
 *	Spells: Clairvoyance / Illusion / Combat / Communication
 *	Functions rewritten: 20/20 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg100.cpp
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg006.h"
#include "seg007.h"
#include "seg039.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg099.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Clairvoyance / Hellsicht */

void spell_eigenschaften(void)
{
	signed short min;
	signed short max;

	ds_writed(SPELLTARGET_E,
		(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

	damage_range_template(host_readws(get_spelltarget_e() + ENEMY_SHEET_DAM1),
		(Bit8u*)&min, (Bit8u*)&max);

	min = min * 8 / 10;
	max = max * 8 / 10;

	sprintf((char*)Real2Host(ds_readd(DTP2)),
          (char*)get_tx(25),
          Real2Host(GUI_name_singular(get_monname(host_readbs(get_spelltarget_e())))),
          host_readbs(get_spelltarget_e() + ENEMY_SHEET_LEVEL),	/* Level */
          host_readbs(get_spelltarget_e() + ENEMY_SHEET_AT),	  /* AT */
          host_readbs(get_spelltarget_e() + ENEMY_SHEET_PA),	  /* PA */
          host_readbs(get_spelltarget_e() + ENEMY_SHEET_RS),		/* RS */
          host_readbs(get_spelltarget_e() + ENEMY_SHEET_ATTACKS),	/* Attacks */
          (host_readbs(get_spelltarget_e() + ENEMY_SHEET_ATTACKS) > 1) ?
          get_tx(26) : get_tx(27),
          min,							/* TPmin */
          max,							/* TPmax */
          host_readws(get_spelltarget_e() + ENEMY_SHEET_LE),	      /* LE */
          host_readws(get_spelltarget_e() + ENEMY_SHEET_LE_ORIG),	  /* LEmax */
          host_readws(get_spelltarget_e() + ENEMY_SHEET_AE),	      /* AE */
          host_readws(get_spelltarget_e() + ENEMY_SHEET_AE_ORIG));	/* AEmax */
}

void spell_exposami(void)
{
	signed short j;
	signed short id;
	signed short changed;
	int arr[20][2];

	signed short i;
	signed short count;

	count = 0;

	for (i = 0; i < ds_readws(NR_OF_ENEMIES); i++) {

		if (host_readbs(Real2Host(ds_readd(CURRENT_FIGHT)) + SIZEOF_FIGHT_MONSTER * i + FIGHT_MONSTERS_ROUND_APPEAR) != 0) {

			id = host_readbs(Real2Host(ds_readd(CURRENT_FIGHT)) + SIZEOF_FIGHT_MONSTER * i + FIGHT_MONSTERS_ID);

			changed = 0;

			for (j = 0; j <= count; j++) {
				if (arr[j][0] == id) {
					/* found an entry with this id */
					arr[j][1]++;
					changed = 1;
					break;
				}
			}

			/* create a new entry */
			if (changed == 0) {
				arr[count][0] = id;
				arr[count][1] = 1;
				count++;
			}
		}
	}


	if (count) {
		/* Intro text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(31));

		for (i = 0; count - 1 > i; i++) {
			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)get_tx(33),		/* "%d %s" */
				arr[i][1],
				(char*)Real2Host(GUI_names_grammar(((arr[i][1] > 1)? 4 : 0) + 0x4000,
									arr[i][0], 1)));
			strcat((char*)Real2Host(ds_readd(DTP2)),
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			if (count - 2 > i) {
				strcat((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(28));		/* "," */
			}
		}

		if (count > 1) {
			strcat((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(29));		/* "AND" */
		}

		sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
			(char*)get_tx(33),
			arr[count - 1][1],	/* TODO: this field access produces other code */
			(char*)Real2Host(GUI_names_grammar((arr[count - 1][1] > 1 ? 4 : 0) + 0x4000,
								arr[count - 1][0], 1)));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(30));			/* "." */
	} else {
		/* no more hidden enemies */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(32));
	}
}

void spell_odem_arcanum(void)
{
	signed short pos;
	signed short id;

	pos = select_item_to_drop(get_spelluser());

	id = host_readws(get_spelluser() + pos * 14 + HERO_ITEM_HEAD);

	if (id) {

		if (ks_magic_hidden(get_spelluser() + pos * 14 + HERO_ITEM_HEAD)) {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(81),
				(char*)Real2Host(GUI_names_grammar((signed short)0x8000, id, 0)));

			/* set known flag */
			or_ptr_bs(get_spelluser() + pos * 14 + (HERO_ITEM_HEAD + 4), 0x80);

		} else {
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(82),
				(char*)Real2Host(GUI_names_grammar((signed short)0x8000, id, 0)));
		}
	}
}

void spell_penetrizzel(void)
{
	signed short y;
	signed short x;

	for (y = -2;  y <= 2; y++) {
		for (x = -2;  x <= 2; x++) {
			if ((ds_readws(Y_TARGET) + y >= 0) && (ds_readws(X_TARGET) + x >= 0)) {
				if ((ds_readb(DNG_MAP_SIZE) - 1 >= ds_readws(X_TARGET) + x) && (ds_readws(Y_TARGET) + y <= 15)) {
					set_automap_tile(ds_readws(X_TARGET) + x, ds_readws(Y_TARGET) + y);
				}
			}
		}
	}
}

void spell_sensibar(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Sensibar\" ist nicht implementiert\n");
#endif
}

/* Illusion */
void spell_chamaelioni(void)
{
	/* set the flag for this spell */
	or_ptr_bs(get_spelluser() + HERO_STATUS1, 0x10);

	/* prepare the message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx(83),
		(char*)get_spelluser() + HERO_NAME2,
		(char*)Real2Host(GUI_get_ptr(host_readbs(get_spelluser() + HERO_SEX), 0)));

}

void spell_duplicatus(void)
{
	/* set the flag for this spell */
	or_ptr_bs(get_spelluser() + HERO_STATUS2, 0x04);

	/* prepare the message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx(84),
		(char*)get_spelluser() + HERO_NAME2,
		(char*)Real2Host(GUI_get_ptr(host_readbs(get_spelluser() + HERO_SEX), 0)));

}

void spell_harmlos(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Harmlos\" ist nicht implementiert\n");
#endif
}

void spell_hexenknoten(void)
{
	Bit8u *ptr;
	RealPt rp;
	signed short x;
	signed short y;
	signed short no;
	signed short height;
	signed short width;
	struct nvf_desc nvf;

	if (ds_readds(FIGHTOBJ_BUF_FREESPACE) < 0x240) {
		ds_writew(SPELL_SPECIAL_AECOST, -2);
		return;
	}

	ptr = Real2Host(FIG_get_ptr(host_readbs(get_spelluser() + HERO_FIGHTER_ID)));
	x = host_readbs(ptr + 3);
	y = host_readbs(ptr + 4);

	if (!host_readbs(get_spelluser() + HERO_VIEWDIR)) {
		x++;
	}
	if (host_readbs(get_spelluser() + HERO_VIEWDIR) == 1) {
		y--;
	}
	if (host_readbs(get_spelluser() + HERO_VIEWDIR) == 2) {
		x--;
	}
	if (host_readbs(get_spelluser() + HERO_VIEWDIR) == 3) {
		y++;
	}

	if (get_cb_val(x, y) != 0) {
		ds_writew(SPELL_SPECIAL_AECOST, -2);
		return;
	}

	no = 24;

	if (NOT_NULL(Real2Host(ds_readfp(HEXENKNOTEN_GFX_BUF)))) {
		rp = ds_readfp(HEXENKNOTEN_GFX_BUF);
		/* TODO: graphic bug if cast more than once */
	} else {
		rp = ds_readfp(FIGHTOBJ_BUF_SEEK_PTR);
		nvf.dst = Real2Host(rp);
		nvf.src = Real2Host(ds_readfp(SPELLOBJ_NVF_BUF));
		nvf.no = no;
		nvf.type = 0;
		nvf.width = (Bit8u*)&width;
		nvf.height = (Bit8u*)&height;
		process_nvf(&nvf);

#if !defined(__BORLANDC__)
		/* BE-fix */
		width = host_readws((Bit8u*)&width);
		height = host_readws((Bit8u*)&height);
#endif

		ds_writefp(HEXENKNOTEN_GFX_BUF, ds_readfp(FIGHTOBJ_BUF_SEEK_PTR));

		/* move pointer further */
		add_ds_fp(FIGHTOBJ_BUF_SEEK_PTR, width * height + 8);

		sub_ds_ds(FIGHTOBJ_BUF_FREESPACE, width * height + 8L);
	}

	ds_writew(FIG_LIST_ELEM, 0);
	ds_writeb((FIG_LIST_ELEM+2), 127);
	ds_writeb((FIG_LIST_ELEM+3), (signed char)x);
	ds_writeb((FIG_LIST_ELEM+4), (signed char)y);
	ds_writeb((FIG_LIST_ELEM+5), 0);
	ds_writeb((FIG_LIST_ELEM+6), 0);
	ds_writeb((FIG_LIST_ELEM+7), (signed char)height);
	ds_writeb((FIG_LIST_ELEM+8), (signed char)width);
	ds_writeb((FIG_LIST_ELEM+9), 0);
	ds_writeb((FIG_LIST_ELEM+10), 0);
	ds_writebs((FIG_LIST_ELEM+11), (signed char)(width) - 1);
	ds_writebs((FIG_LIST_ELEM+12), (signed char)(height) - 1);
	ds_writeb((FIG_LIST_ELEM+13), 0);
	ds_writeb((FIG_LIST_ELEM+15), -1);
	ds_writeb((FIG_LIST_ELEM+14), -1);
	ds_writed((FIG_LIST_ELEM+23), (Bit32u)rp);
	ds_writeb((FIG_LIST_ELEM+17), 50);
	ds_writeb((FIG_LIST_ELEM+18), 1);
	ds_writeb((FIG_LIST_ELEM+19), -1);

	FIG_add_to_list(-1);

	place_obj_on_cb(x, y, 127, 127, 0);
}


/* Combat / Kampf */

void spell_blitz(void)
{

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {
		/* cast a hero */

		/* set the spell target */
		ds_writed(SPELLTARGET,
	                (Bit32u)((RealPt)ds_readd(HEROS) + (host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) * SIZEOF_HERO));

		if (get_spelltarget() == get_spelluser()) {

			ds_writew(SPELL_SPECIAL_AECOST, 0);

			strcpy((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(112));
		} else {
			/* set the rounds counter */
			host_writeb(get_spelltarget() + HERO_BLIND, 3);

			/* prepare the message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(86),
				(char*)get_spelltarget() + HERO_NAME2);
		}
	} else {
		/* cast an enemy */

		/* set a pointer to the enemy */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* set the rounds counter */
		host_writeb(get_spelltarget_e() + ENEMY_SHEET_BLIND, 3);

		/* prepare the message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(85),
			(char*)Real2Host(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1)));
	}
}

void spell_ecliptifactus(void)
{
	signed short rounds;
	signed short ae;

	/* ask how many rounds */
	rounds = GUI_input(get_tx(87), 1);

	if (rounds != -1) {

		/* calculate the AE costs */
		ae = rounds * 2 + 5;

		if (host_readws(get_spelluser() + HERO_AE) >= ae) {
			/* set AP costs */
			ds_writew(SPELL_SPECIAL_AECOST, ae);
			/* enable the spell */
			host_writeb(get_spelluser() + HERO_ECLIPTIFACTUS, (signed char)rounds + 1);
			/* prepare the message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(88),
				(char*)(get_spelluser() + HERO_NAME2),
				(char*)Real2Host(GUI_get_ptr(host_readbs(get_spelluser() + HERO_SEX), 3)),
				rounds);
		} else {
			/* prepare the message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(607),
				(char*)get_spelluser() + HERO_NAME2);
			/* set costs to 0 */
			ds_writew(SPELL_SPECIAL_AECOST, 0);
		}
	} else {
		ds_writew(SPELL_SPECIAL_AECOST, -2);
	}
}

void spell_eisenrost(void)
{
	signed short id;

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {
		/* cast a hero */

		/* set the spell target */
		ds_writed(SPELLTARGET,
	                (Bit32u)((RealPt)ds_readd(HEROS) + (host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) * SIZEOF_HERO));

		if (get_spelltarget() == get_spelluser()) {

			ds_writew(SPELL_SPECIAL_AECOST, 0);

			strcpy((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(112));
		} else {
			/* get weapon id of the target */
			id = host_readws(get_spelltarget() + HERO_ITEM_RIGHT);

			if (!id) {
				/* no weapon in hand */
				ds_writew(SPELL_SPECIAL_AECOST, -2);
			} else {
				/* check if weapon is already broken */
				if (ks_broken(get_spelltarget() + HERO_ITEM_RIGHT)) {

					strcpy((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx(90));

				} else {

					if (host_readbs(get_spelltarget() + (HERO_ITEM_RIGHT + 6)) > 0) {
						/* set broken flag */
						or_ptr_bs(get_spelltarget() + (HERO_ITEM_RIGHT + 4), 0x01);
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_tx(92),
							(char*)Real2Host(GUI_names_grammar((signed short)0x8000, id, 0)),
							(char*)(get_spelltarget() + HERO_NAME2));
					} else {
						ds_writew(SPELL_SPECIAL_AECOST, -2);
					}
				}
			}
		}
	} else {
		/* set a pointer to the enemy */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* check if target is an animal */
		if (host_readbs(get_spelltarget_e() + ENEMY_SHEET_FLAGS) != 0)
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(89));
		} else {
			/* check if weapon is already broken */
			if (host_readbs(get_spelltarget_e() + ENEMY_SHEET_BROKEN) != 0) {
				strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx(90));
			} else {

				/* set weapon broken */
				host_writeb(get_spelltarget_e() + ENEMY_SHEET_BROKEN, 1);

				/* prepare message */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(91),
					(char*)Real2Host(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1)));
			}
		}
	}
}

void spell_fulminictus(void)
{
	signed short damage;

	if ((host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) &&
		get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) == get_spelluser()) {

		/* do not attack yourself */

		/* set costs to 0 */
		ds_writew(SPELL_SPECIAL_AECOST, 0);

		/* prepare message */
		strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx(112));
	} else {
		/* roll 3W6+0 damage */
		damage = dice_roll(3, 6, 0);

		/* add level to damage */
		damage += host_readbs(get_spelluser() + HERO_LEVEL);

		/* reduce damage if the spellcaster has not enough AE */
		if (host_readws(get_spelluser() + HERO_AE) < damage) {

			damage = host_readws(get_spelluser() + HERO_AE);
		}

		/* do the damage */
		FIG_do_spell_damage(damage);

		/* set costs to damage AE */
		ds_writew(SPELL_SPECIAL_AECOST, damage);
	}
}

void spell_ignifaxius(void)
{
	signed short rs_malus;
	signed short hero_pos;
	signed short slot;
	signed short mummy = 0;
	Bit8u *p_armour;
	signed short damage;
	signed short level;

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {

		if (get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) == get_spelluser()) {

			/* don't attack yourself */
			ds_writew(SPELL_SPECIAL_AECOST, 0);

			/* prepare message */
			strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx(112));
			return;;
		}
	}

	/* get spell level... */
	if ((ds_readws(AUTOFIGHT) == 0) && (host_readbs(get_spelluser() + HERO_NPC_ID) == 0)) {
		/* ... manual mode */

		/* prepare question of spell level */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(93),
			host_readbs(get_spelluser() + HERO_LEVEL) + 1);

		level = GUI_input(Real2Host(ds_readd(DTP2)), 2);

		if (level <= 0) {
			/* abort */
			/* terminate string */
			host_writeb(Real2Host(ds_readd(DTP2)), 0);
			ds_writew(SPELL_SPECIAL_AECOST, 0);
			return;
		}

		/* adjust wrong input */
		if ((host_readbs(get_spelluser() + HERO_LEVEL) + 1) < level) {
			level = host_readbs(get_spelluser() + HERO_LEVEL) + 1;
		}

	} else {
		/* ... autofight mode */
		level = host_readbs(get_spelluser() + HERO_LEVEL) + 1;
	}

	/* calculate: damage = level * W6 */
	damage = dice_roll(level, 6, 0);

	/* damage must not exceed AE of the spelluser */
	if (host_readws(get_spelluser() + HERO_AE) < damage) {
		damage = host_readws(get_spelluser() + HERO_AE);
	}

	/* damage doubles if the target is a mummy */
	if ((host_readbs(get_spelluser() + HERO_ENEMY_ID) >= 10) &&
		(host_readbs(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_GFX_ID) == 0x1e))
	{
		damage *= 2;
		mummy = 1;
	}

	/* do the damage to the target */
	FIG_do_spell_damage(damage);

	rs_malus = damage / 10;

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {

		/* target is a hero */

		hero_pos = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

		/* set the spell target */
		ds_writed(SPELLTARGET,
	                (Bit32u)((RealPt)ds_readd(HEROS) + hero_pos * SIZEOF_HERO));

		/* get a pointer to the armour */
		p_armour = get_spelltarget() + HERO_ITEM_BODY;

		if ((host_readws(p_armour) != 0) && (rs_malus != 0)) {

			/* adjust rs_malus */
			if ((host_readbs(p_armour + 7) + rs_malus) > ds_readbs(ARMORS_TABLE + host_readbs(get_itemsdat(host_readws(p_armour)) + 4) * 2)) {
				rs_malus = ds_readbs(ARMORS_TABLE + host_readbs(get_itemsdat(host_readws(p_armour)) + 4) * 2) - host_readbs(p_armour + 7);
			}

			/* add rs_malus to the armour */
			host_writebs(p_armour + 0x07,
				host_readbs(p_armour + 7) + rs_malus);
			/* subtract rs_malus from RS1 */
			host_writebs(get_spelltarget() + HERO_RS_BONUS1,
				host_readbs(get_spelltarget() + HERO_RS_BONUS1) - rs_malus);
		}

		/* get an AT/PA-Malus of -level / 2 for the current weapon and one hour */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1),
			get_spelltarget() + HERO_AT + host_readbs(get_spelltarget() + HERO_WP_CLASS),
			-level / 2, (signed char)hero_pos);

		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1),
			get_spelltarget() + HERO_PA + host_readbs(get_spelltarget() + HERO_WP_CLASS),
			-level / 2, (signed char)hero_pos);

	} else {
		/* target is a enemy */

		/* set a pointer to the enemy */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		host_writebs(get_spelltarget_e() + ENEMY_SHEET_RS,
			host_readbs(get_spelltarget_e() + ENEMY_SHEET_RS) - rs_malus);
		sub_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_AT, level / 2);
		sub_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_PA, level / 2);

	}

	/* terminate output string */
	host_writeb(Real2Host(ds_readd(DTP2)), 0);

	if (mummy != 0) {
		damage /= 2;
	}

	/* set spell costs */
	ds_writew(SPELL_SPECIAL_AECOST, damage);
}

void spell_plumbumbarum(void)
{
	signed short slot;
	signed short hero_pos;

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {

		/* target is a hero */

		hero_pos = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

		/* set the spell target */
		ds_writed(SPELLTARGET,
	                (Bit32u)((RealPt)ds_readd(HEROS) + hero_pos * SIZEOF_HERO));

		if (get_spelltarget() == get_spelluser()) {

			/* don't attack yourself */
			ds_writew(SPELL_SPECIAL_AECOST, 0);

			/* prepare message */
			strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx(112));

		} else {

			/* give a short AT-malus of -3 to the targets current weapon */
			slot = get_free_mod_slot();
			set_mod_slot(slot, 0x2d,
				get_spelltarget() + HERO_AT + host_readbs(get_spelltarget() + HERO_WP_CLASS),
				-3, (signed char)hero_pos);

			/* prepare the message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(94),
				(char*)get_spelltarget() + HERO_NAME2);
			}

		return;

	}

	/* target is a enemy */

	/* set a pointer to the enemy */
	ds_writed(SPELLTARGET_E,
		(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

	/* AT-malus of -3 (permanent) */
	sub_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_AT, 3);

	/* prepare the message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx(95),
		(char*)Real2Host(GUI_names_grammar((signed short)0x8001, host_readbs(get_spelltarget_e()), 1)));
}

void spell_radau(void)
{
	spell_kraehenruf();
}

void spell_saft_kraft(void)
{
	signed short target;
	signed short slot;
	signed short rounds;

	rounds = random_schick(20);

	/* get the index of the hero on whom the spell is cast */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	/* set a pointer to the target */
	ds_writed(SPELLTARGET,
		(Bit32u)((RealPt)ds_readd(HEROS) + SIZEOF_HERO * target));


	/* +5 on AT of the current weapon */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L,
		get_spelltarget() + HERO_AT + host_readbs(get_spelltarget() + HERO_WP_CLASS),
		5, (signed char)target);

	/* -5 on PA of the current weapon */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L,
		get_spelltarget() + HERO_PA + host_readbs(get_spelltarget() + HERO_WP_CLASS),
		-5, (signed char)target);

	/* TODO: this position is unknown */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L,
		get_spelltarget() + HERO_SAFTKRAFT, 5, (signed char)target);

	/* set ae costs */
	ds_writew(SPELL_SPECIAL_AECOST, rounds);

#ifdef M302de_ORIGINAL_BUGFIX
	char *p = (char*)get_tx(96);

	if (p[10] == 'L' && p[11] == 'E') {
		/* change "VERWANDLET" into "VERWANDELT" */
		p[10] = 'E';
		p[11] = 'L';
	}
#endif

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx(96),
		(char*)get_spelltarget() + HERO_NAME2);

}

void spell_scharfes_auge(void)
{
	signed short target;
	signed short slot;

	/* get the index of the hero on whom the spell is cast */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	/* set a pointer to the target */
	ds_writed(SPELLTARGET,
		(Bit32u)((RealPt)ds_readd(HEROS) + SIZEOF_HERO * target));

	/* all range skills are boosted + 3 */

	slot = get_free_mod_slot();

	set_mod_slot(slot, 3 * 9L, get_spelltarget() + (HERO_TA_FIGHT + 8), 3, (signed char)target);

	slot = get_free_mod_slot();

	set_mod_slot(slot, 3 * 9L, get_spelltarget() + (HERO_TA_FIGHT + 7), 3, (signed char)target);

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx(97),
		(char*)get_spelltarget() + HERO_NAME2);

}


/* Communication / Verstaendigung */
void spell_hexenblick(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Hexenblick\" ist nicht implementiert\n");
#endif
}

void spell_necropathia(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Necropathia\" ist nicht implementiert\n");
#endif
}

#if !defined(__BORLANDC__)
}
#endif
