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
		(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser() + 0x86) * 62));

	damage_range_template(host_readws(get_spelltarget_e() + 0x1e),
		(Bit8u*)&min, (Bit8u*)&max);

	min = min * 8 / 10;
	max = max * 8 / 10;

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x64),
		Real2Host(GUI_name_singular(get_monname(host_readbs(get_spelltarget_e())))),
		host_readbs(get_spelltarget_e() + 0x29),	/* Level */
		host_readbs(get_spelltarget_e() + 0x1c),	/* AT */
		host_readbs(get_spelltarget_e() + 0x1d),	/* PA */
		host_readbs(get_spelltarget_e() + 0x2),		/* RS */
		host_readbs(get_spelltarget_e() + 0x1b),	/* Attacks */
		(host_readbs(get_spelltarget_e() + 0x1b) > 1) ?
			get_dtp(0x68) : get_dtp(0x6c),
		min,							/* TPmin */
		max,							/* TPmax */
		host_readws(get_spelltarget_e() + 0x13),	/* LE */
		host_readws(get_spelltarget_e() + 0x11),	/* LEmax */
		host_readws(get_spelltarget_e() + 0x17),	/* AE */
		host_readws(get_spelltarget_e() + 0x15));	/* AEmax */
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
			(char*)get_dtp(0x7c));

		for (i = 0; count - 1 > i; i++) {
			sprintf((char*)Real2Host(ds_readd(BUFFER4_PTR)),
				(char*)get_dtp(0x84),		/* "%d %s" */
				arr[i][1],
				(char*)Real2Host(GUI_names_grammar(((arr[i][1] > 1)? 4 : 0) + 0x4000,
									arr[i][0], 1)));
			strcat((char*)Real2Host(ds_readd(DTP2)),
				(char*)Real2Host(ds_readd(BUFFER4_PTR)));

			if (count - 2 > i) {
				strcat((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x70));		/* "," */
			}
		}

		if (count > 1) {
			strcat((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x74));		/* "AND" */
		}

		sprintf((char*)Real2Host(ds_readd(BUFFER4_PTR)),
			(char*)get_dtp(0x84),
			arr[count - 1][1],	/* TODO: this field access produces other code */
			(char*)Real2Host(GUI_names_grammar((arr[count - 1][1] > 1 ? 4 : 0) + 0x4000,
								arr[count - 1][0], 1)));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)Real2Host(ds_readd(BUFFER4_PTR)));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x78));			/* "." */
	} else {
		/* no more hidden enemies */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x80));
	}
}

void spell_odem_arcanum(void)
{
	signed short pos;
	signed short id;

	pos = select_item_to_drop(get_spelluser());

	id = host_readws(get_spelluser() + pos * 14 + 0x196);

	if (id) {

		if (ks_magic_hidden(get_spelluser() + pos * 14 + 0x196)) {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x144),
				(char*)Real2Host(GUI_names_grammar((signed short)0x8000, id, 0)));

			/* set known flag */
			or_ptr_bs(get_spelluser() + pos * 14 + 0x19a, 0x80);

		} else {
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x148),
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
				if ((ds_readb(0xbd94) - 1 >= ds_readws(X_TARGET) + x) && (ds_readws(Y_TARGET) + y <= 15)) {
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
	or_ptr_bs(get_spelluser() + 0xaa, 0x10);

	/* prepare the message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x14c),
		(char*)get_spelluser() + 0x10,
		(char*)Real2Host(GUI_get_ptr(host_readbs(get_spelluser() + 0x22), 0)));

}

void spell_duplicatus(void)
{
	/* set the flag for this spell */
	or_ptr_bs(get_spelluser() + 0xab, 0x04);

	/* prepare the message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x150),
		(char*)get_spelluser() + 0x10,
		(char*)Real2Host(GUI_get_ptr(host_readbs(get_spelluser() + 0x22), 0)));

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
	signed short nr;
	signed short height;
	signed short width;
	struct nvf_desc nvf;

	if (ds_readds(0xe370) < 0x240) {
		ds_writew(0xac0e, -2);
		return;
	}

	ptr = Real2Host(FIG_get_ptr(host_readbs(get_spelluser() + 0x81)));
	x = host_readbs(ptr + 3);
	y = host_readbs(ptr + 4);

	if (!host_readbs(get_spelluser() + 0x82)) {
		x++;
	}
	if (host_readbs(get_spelluser() + 0x82) == 1) {
		y--;
	}
	if (host_readbs(get_spelluser() + 0x82) == 2) {
		x--;
	}
	if (host_readbs(get_spelluser() + 0x82) == 3) {
		y++;
	}

	if (get_cb_val(x, y) != 0) {
		ds_writew(0xac0e, -2);
		return;
	}

	nr = 24;

	if (NOT_NULL(Real2Host(ds_readd(0xacc8)))) {
		rp = (RealPt)ds_readd(0xacc8);
		/* TODO: graphic bug if cast more than once */
	} else {
		rp = (RealPt)ds_readd(0xd86e);
		nvf.dst = Real2Host(rp);
		nvf.src = Real2Host(ds_readd(SPELLOBJ_NVF_BUF));
		nvf.nr = nr;
		nvf.type = 0;
		nvf.width = (Bit8u*)&width;
		nvf.height = (Bit8u*)&height;
		process_nvf(&nvf);

#if !defined(__BORLANDC__)
		/* BE-fix */
		width = host_readws((Bit8u*)&width);
		height = host_readws((Bit8u*)&height);
#endif

		ds_writed(0xacc8, ds_readd(0xd86e));

		/* move pointer further */
		add_ds_ws(0xd86e, width * height + 8);

		sub_ds_ds(0xe370, width * height + 8L);
	}

	ds_writew(FIG_LIST_ELEM, 0);
	ds_writeb(0xe068, 127);
	ds_writeb(0xe069, (signed char)x);
	ds_writeb(0xe06a, (signed char)y);
	ds_writeb(0xe06b, 0);
	ds_writeb(0xe06c, 0);
	ds_writeb(0xe06d, (signed char)height);
	ds_writeb(0xe06e, (signed char)width);
	ds_writeb(0xe06f, 0);
	ds_writeb(0xe070, 0);
	ds_writebs(0xe071, (signed char)(width) - 1);
	ds_writebs(0xe072, (signed char)(height) - 1);
	ds_writeb(0xe073, 0);
	ds_writeb(0xe075, -1);
	ds_writeb(0xe074, -1);
	ds_writed(0xe07d, (Bit32u)rp);
	ds_writeb(0xe077, 50);
	ds_writeb(0xe078, 1);
	ds_writeb(0xe079, -1);

	FIG_add_to_list(-1);

	place_obj_on_cb(x, y, 127, 127, 0);
}


/* Combat / Kampf */

void spell_blitz(void)
{

	if (host_readbs(get_spelluser() + 0x86) < 10) {
		/* cast a hero */

		/* set the spell target */
		ds_writed(SPELLTARGET,
	                (Bit32u)((RealPt)ds_readd(HEROS) + (host_readbs(get_spelluser() + 0x86) - 1) * SIZEOF_HERO));

		if (get_spelltarget() == get_spelluser()) {

			ds_writew(0xac0e, 0);

			strcpy((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x1c0));
		} else {
			/* set the rounds counter */
			host_writeb(get_spelltarget() + 0x96, 3);

			/* prepare the message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x158),
				(char*)get_spelltarget() + 0x10);
		}
	} else {
		/* cast an enemy */

		/* set a pointer to the enemy */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser() + 0x86) * 62));

		/* set the rounds counter */
		host_writeb(get_spelltarget_e() + 0x2f, 3);

		/* prepare the message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x154),
			(char*)Real2Host(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1)));
	}
}

void spell_ecliptifactus(void)
{
	signed short rounds;
	signed short ae;

	/* ask how many rounds */
	rounds = GUI_input(get_dtp(0x15c), 1);

	if (rounds != -1) {

		/* calculate the AE costs */
		ae = rounds * 2 + 5;

		if (host_readws(get_spelluser() + 0x64) >= ae) {
			/* set AP costs */
			ds_writew(0xac0e, ae);
			/* enable the spell */
			host_writeb(get_spelluser() + 0x97, (signed char)rounds + 1);
			/* prepare the message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x160),
				(char*)(get_spelluser() + 0x10),
				(char*)Real2Host(GUI_get_ptr(host_readbs(get_spelluser() + 0x22), 3)),
				rounds);
		} else {
			/* prepare the message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0x97c),
				(char*)get_spelluser() + 0x10);
			/* set costs to 0 */
			ds_writew(0xac0e, 0);
		}
	} else {
		ds_writew(0xac0e, -2);
	}
}

void spell_eisenrost(void)
{
	signed short id;

	if (host_readbs(get_spelluser() + 0x86) < 10) {
		/* cast a hero */

		/* set the spell target */
		ds_writed(SPELLTARGET,
	                (Bit32u)((RealPt)ds_readd(HEROS) + (host_readbs(get_spelluser() + 0x86) - 1) * SIZEOF_HERO));

		if (get_spelltarget() == get_spelluser()) {

			ds_writew(0xac0e, 0);

			strcpy((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x1c0));
		} else {
			/* get weapon id of the target */
			id = host_readws(get_spelltarget() + 0x1c0);

			if (!id) {
				/* no weapon in hand */
				ds_writew(0xac0e, -2);
			} else {
				/* check if weapon is already broken */
				if (ks_broken(get_spelltarget() + 0x1c0)) {

					strcpy((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x168));

				} else {

					if (host_readbs(get_spelltarget() + 0x1c6) > 0) {
						/* set broken flag */
						or_ptr_bs(get_spelltarget() + 0x1c4, 0x01);
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_dtp(0x170),
							(char*)Real2Host(GUI_names_grammar((signed short)0x8000, id, 0)),
							(char*)(get_spelltarget() + 0x10));
					} else {
						ds_writew(0xac0e, -2);
					}
				}
			}
		}
	} else {
		/* set a pointer to the enemy */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser() + 0x86) * 62));

		/* check if target is an animal */
		if (host_readbs(get_spelltarget_e() + 0x36) != 0) {
			sprintf((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(0x164));
		} else {
			/* check if weapon is already broken */
			if (host_readbs(get_spelltarget_e() + 0x30) != 0) {
				strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(0x168));
			} else {

				/* set weapon broken */
				host_writeb(get_spelltarget_e() + 0x30, 1);

				/* prepare message */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x16c),
					(char*)Real2Host(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1)));
			}
		}
	}
}

void spell_fulminictus(void)
{
	signed short damage;

	if ((host_readbs(get_spelluser() + 0x86) < 10) &&
		get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) == get_spelluser()) {

		/* do not attack yourself */

		/* set costs to 0 */
		ds_writew(0xac0e, 0);

		/* prepare message */
		strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(0x1c0));
	} else {
		/* roll 3W6+0 damage */
		damage = dice_roll(3, 6, 0);

		/* add level to damage */
		damage += host_readbs(get_spelluser() + 0x27);

		/* reduce damage if the spellcaster has not enough AE */
		if (host_readws(get_spelluser() + 0x64) < damage) {

			damage = host_readws(get_spelluser() + 0x64);
		}

		/* do the damage */
		FIG_do_spell_damage(damage);

		/* set costs to damage AE */
		ds_writew(0xac0e, damage);
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

	if (host_readbs(get_spelluser() + 0x86) < 10) {

		if (get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) == get_spelluser()) {

			/* don't attack yourself */
			ds_writew(0xac0e, 0);

			/* prepare message */
			strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(0x1c0));
			return;;
		}
	}

	/* get spell level... */
	if ((ds_readws(AUTOFIGHT) == 0) && (host_readbs(get_spelluser() + 0x89) == 0)) {
		/* ... manual mode */

		/* prepare question of spell level */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x174),
			host_readbs(get_spelluser() + 0x27) + 1);

		level = GUI_input(Real2Host(ds_readd(DTP2)), 2);

		if (level <= 0) {
			/* abort */
			/* terminate string */
			host_writeb(Real2Host(ds_readd(DTP2)), 0);
			ds_writew(0xac0e, 0);
			return;
		}

		/* adjust wrong input */
		if ((host_readbs(get_spelluser() + 0x27) + 1) < level) {
			level = host_readbs(get_spelluser() + 0x27) + 1;
		}

	} else {
		/* ... autofight mode */
		level = host_readbs(get_spelluser() + 0x27) + 1;
	}

	/* calculate: damage = level * W6 */
	damage = dice_roll(level, 6, 0);

	/* damage must not exceed AE of the spelluser */
	if (host_readws(get_spelluser() + 0x64) < damage) {
		damage = host_readws(get_spelluser() + 0x64);
	}

	/* damage doubles if the target is a mummy */
	if ((host_readbs(get_spelluser() + 0x86) >= 10) &&
		(host_readbs(p_datseg + host_readbs(get_spelluser() + 0x86) * 62 + 0xd0df + 1) == 0x1e))
	{
		damage *= 2;
		mummy = 1;
	}

	/* do the damage to the target */
	FIG_do_spell_damage(damage);

	rs_malus = damage / 10;

	if (host_readbs(get_spelluser() + 0x86) < 10) {

		/* target is a hero */

		hero_pos = host_readbs(get_spelluser() + 0x86) - 1;

		/* set the spell target */
		ds_writed(SPELLTARGET,
	                (Bit32u)((RealPt)ds_readd(HEROS) + hero_pos * SIZEOF_HERO));

		/* get a pointer to the armour */
		p_armour = get_spelltarget() + 0x1b2;

		if ((host_readws(p_armour) != 0) && (rs_malus != 0)) {

			/* adjust rs_malus */
			if ((host_readbs(p_armour + 7) + rs_malus) > ds_readbs(0x877 + host_readbs(get_itemsdat(host_readws(p_armour)) + 4) * 2)) {
				rs_malus = ds_readbs(0x877 + host_readbs(get_itemsdat(host_readws(p_armour)) + 4) * 2) - host_readbs(p_armour + 7);
			}

			/* add rs_malus to the armour */
			host_writebs(p_armour + 0x07,
				host_readbs(p_armour + 7) + rs_malus);
			/* subtract rs_malus from RS1 */
			host_writebs(get_spelltarget() + 0x30,
				host_readbs(get_spelltarget() + 0x30) - rs_malus);
		}

		/* get an AT/PA-Malus of -level / 2 for the current weapon and one hour */
		slot = get_free_mod_slot();
		set_mod_slot(slot, 0x1518,
			get_spelltarget() + 0x68 + host_readbs(get_spelltarget() + 0x78),
			-level / 2, (signed char)hero_pos);

		slot = get_free_mod_slot();
		set_mod_slot(slot, 0x1518,
			get_spelltarget() + 0x6f + host_readbs(get_spelltarget() + 0x78),
			-level / 2, (signed char)hero_pos);

	} else {
		/* target is a enemy */

		/* set a pointer to the enemy */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser() + 0x86) * 62));

		host_writebs(get_spelltarget_e() + 0x2,
			host_readbs(get_spelltarget_e() + 0x2) - rs_malus);
		sub_ptr_bs(get_spelltarget_e() + 0x1c, level / 2);
		sub_ptr_bs(get_spelltarget_e() + 0x1d, level / 2);

	}

	/* terminate output string */
	host_writeb(Real2Host(ds_readd(DTP2)), 0);

	if (mummy != 0) {
		damage /= 2;
	}

	/* set spell costs */
	ds_writew(0xac0e, damage);
}

void spell_plumbumbarum(void)
{
	signed short slot;
	signed short hero_pos;

	if (host_readbs(get_spelluser() + 0x86) < 10) {

		/* target is a hero */

		hero_pos = host_readbs(get_spelluser() + 0x86) - 1;

		/* set the spell target */
		ds_writed(SPELLTARGET,
	                (Bit32u)((RealPt)ds_readd(HEROS) + hero_pos * SIZEOF_HERO));

		if (get_spelltarget() == get_spelluser()) {

			/* don't attack yourself */
			ds_writew(0xac0e, 0);

			/* prepare message */
			strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(0x1c0));

		} else {

			/* give a short AT-malus of -3 to the targets current weapon */
			slot = get_free_mod_slot();
			set_mod_slot(slot, 0x2d,
				get_spelltarget() + 0x68 + host_readbs(get_spelltarget() + 0x78),
				-3, (signed char)hero_pos);

			/* prepare the message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x178),
				(char*)get_spelltarget() + 0x10);
			}

		return;

	}

	/* target is a enemy */

	/* set a pointer to the enemy */
	ds_writed(SPELLTARGET_E,
		(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser() + 0x86) * 62));

	/* AT-malus of -3 (permanent) */
	sub_ptr_bs(get_spelltarget_e() + 0x1c, 3);

	/* prepare the message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x17c),
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
	target = host_readbs(get_spelluser() + 0x86) - 1;

	/* set a pointer to the target */
	ds_writed(SPELLTARGET,
		(Bit32u)((RealPt)ds_readd(HEROS) + SIZEOF_HERO * target));


	/* +5 on AT of the current weapon */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L,
		get_spelltarget() + 0x68 + host_readbs(get_spelltarget() + 0x78),
		5, (signed char)target);

	/* -5 on PA of the current weapon */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L,
		get_spelltarget() + 0x6f + host_readbs(get_spelltarget() + 0x78),
		-5, (signed char)target);

	/* TODO: this position is unknown */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L,
		get_spelltarget() + HERO_SAFTKRAFT, 5, (signed char)target);

	/* set ae costs */
	ds_writew(0xac0e, rounds);

#ifdef M302de_ORIGINAL_BUGFIX
	char *p = (char*)get_dtp(96 * 4);

	if (p[10] == 'L' && p[11] == 'E') {
		/* change "VERWANDLET" into "VERWANDELT" */
		p[10] = 'E';
		p[11] = 'L';
	}
#endif

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(96 * 4),
		(char*)get_spelltarget() + 0x10);

}

void spell_scharfes_auge(void)
{
	signed short target;
	signed short slot;

	/* get the index of the hero on whom the spell is cast */
	target = host_readbs(get_spelluser() + 0x86) - 1;

	/* set a pointer to the target */
	ds_writed(SPELLTARGET,
		(Bit32u)((RealPt)ds_readd(HEROS) + SIZEOF_HERO * target));

	/* all range talents are boosted + 3 */

	slot = get_free_mod_slot();

	set_mod_slot(slot, 27, Real2Host(ds_readd(SPELLTARGET)) + 0x110, 3, (signed char)target);

	slot = get_free_mod_slot();

	set_mod_slot(slot, 27, Real2Host(ds_readd(SPELLTARGET)) + 0x10f, 3, (signed char)target);

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(97 * 4),
		(char*)Real2Host(ds_readd(SPELLTARGET)) + 0x10);

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
