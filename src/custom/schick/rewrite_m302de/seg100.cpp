/*
 *	Rewrite of DSA1 v3.02_de functions of seg100 (spells 2/3)
 *	Spells: Clairvoyance / Illusion / Combat / Communication
 *	Functions rewritten 15/20
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg099.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Clairvoyance / Hellsicht */

/* Borlandified and identical */
void spell_eigenschaften(void)
{
	signed short min;
	signed short max;

	ds_writed(SPELLTARGET_E,
		(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser() + 0x86) * 62));

	damage_range_template(host_readws(Real2Host(ds_readd(SPELLTARGET_E)) + 0x1e),
		(Bit8u*)&min, (Bit8u*)&max);

	min = min * 8 / 10;
	max = max * 8 / 10;

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x64),
		Real2Host(GUI_name_singular(get_monname(host_readbs(Real2Host(ds_readd(SPELLTARGET_E)))))),
		host_readbs(Real2Host(ds_readd(SPELLTARGET_E)) + 0x29),	/* Level */
		host_readbs(Real2Host(ds_readd(SPELLTARGET_E)) + 0x1c),	/* AT */
		host_readbs(Real2Host(ds_readd(SPELLTARGET_E)) + 0x1d),	/* PA */
		host_readbs(Real2Host(ds_readd(SPELLTARGET_E)) + 0x2),		/* RS */
		host_readbs(Real2Host(ds_readd(SPELLTARGET_E)) + 0x1b),	/* Attacks */
		(host_readbs(Real2Host(ds_readd(SPELLTARGET_E)) + 0x1b) > 1) ?
			get_dtp(0x68) : get_dtp(0x6c),
		min,							/* TPmin */
		max,							/* TPmax */
		host_readws(Real2Host(ds_readd(SPELLTARGET_E)) + 0x13),	/* LE */
		host_readws(Real2Host(ds_readd(SPELLTARGET_E)) + 0x11),	/* LEmax */
		host_readws(Real2Host(ds_readd(SPELLTARGET_E)) + 0x17),	/* AE */
		host_readws(Real2Host(ds_readd(SPELLTARGET_E)) + 0x15));	/* AEmax */
}

/* Borlandified and identical */
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

		if (host_readbs(Real2Host(ds_readd(0xbd28)) + 5 * i + 0x1a) != 0) {

			id = host_readbs(Real2Host(ds_readd(0xbd28)) + 5 * i + 0x16);

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
			sprintf((char*)Real2Host(ds_readd(0xd2eb)),
				(char*)get_dtp(0x84),		/* "%d %s" */
				arr[i][1],
				(char*)Real2Host(GUI_names_grammar(((arr[i][1] > 1)? 4 : 0) + 0x4000,
									arr[i][0], 1)));
			strcat((char*)Real2Host(ds_readd(DTP2)),
				(char*)Real2Host(ds_readd(0xd2eb)));

			if (count - 2 > i) {
				strcat((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x70));		/* "," */
			}
		}

		if (count > 1) {
			strcat((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x74));		/* "AND" */
		}

		sprintf((char*)Real2Host(ds_readd(0xd2eb)),
			(char*)get_dtp(0x84),
			arr[count - 1][1],	/* TODO: this field access produces other code */
			(char*)Real2Host(GUI_names_grammar((arr[count - 1][1] > 1 ? 4 : 0) + 0x4000,
								arr[count - 1][0], 1)));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)Real2Host(ds_readd(0xd2eb)));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x78));			/* "." */
	} else {
		/* no more hidden enemies */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x80));
	}
}

/* Borlandified and identical */
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
				(char*)Real2Host(GUI_names_grammar(0x8000, id, 0)));

			/* set known flag */
			or_ptr_bs(get_spelluser() + pos * 14 + 0x19a, 0x80);

		} else {
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x148),
				(char*)Real2Host(GUI_names_grammar(0x8000, id, 0)));
		}
	}
}

/* Borlandified and identical */
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

/* Borlandified and identical */
void spell_sensibar(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Sensibar\" ist nicht implementiert\n");
#endif
}

/* Illusion */
/* Borlandified and identical */
void spell_chamaelioni(void)
{
	/* set the flag for this spell */
	or_ptr_bs(get_spelluser() + 0xaa, 0x10);

	/* prepare the message */
	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_dtp(0x14c),
		(char*)get_spelluser() + 0x10,
		(char*)Real2Host(GUI_get_ptr(host_readbs(get_spelluser() + 0x22), 0)));

}

/* Borlandified and identical */
void spell_duplicatus(void)
{
	/* set the flag for this spell */
	or_ptr_bs(get_spelluser() + 0xab, 0x04);

	/* prepare the message */
	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_dtp(0x150),
		(char*)get_spelluser() + 0x10,
		(char*)Real2Host(GUI_get_ptr(host_readbs(get_spelluser() + 0x22), 0)));

}

/* Borlandified and identical */
void spell_harmlos(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Harmlos\" ist nicht implementiert\n");
#endif
}

/* Combat / Kampf */
void spell_blitz()
{

	if (host_readb(get_spelluser() + 0x86) < 10) {
		/* cast a hero */

		/* set the spell target */
		ds_writed(0xe5b8,
			ds_readd((HEROS) + (host_readb(get_spelluser() + 0x86) - 1) * 0x6da));

		/* do not cast yourself */
		if (get_spelltarget() == get_spelluser()) {

			ds_writeb(0xac0e, 0);

			strcpy((char*)Real2Host(ds_readd(0xd2f3)),
				(char*)get_dtp(0x1c0));
		} else {
			/* set the rounds counter */
			host_writeb(get_spelltarget() + 0x96, 3);

			/* prepare the message */
			sprintf((char*)Real2Host(ds_readd(0xd2f3)),
				(char*)get_dtp(0x158),
				(char*)get_spelltarget() + 0x10);
		}
	} else {
		/* cast an enemy */

		/* set a pointer to the enemy */
#if !defined(__BORLANDC__)
		ds_writed(SPELLTARGET_E,
			RealMake(datseg, 0xd0df + host_readb(get_spelluser() + 0x86) * 62));
#else
		ds_writed(SPELLTARGET_E,
			(Bit32u)MK_FP(datseg, 0xd0df + host_readb(get_spelluser() + 0x86) * 62));
#endif

		/* set the rounds counter */
		host_writeb(Real2Host(ds_readd(SPELLTARGET_E)) + 0x2f, 3);

		/* prepare the message */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(0x154),
			(char*)Real2Host(GUI_names_grammar(0x8000, host_readb(Real2Host(ds_readd(SPELLTARGET_E))), 1)));
	}
}

void spell_ecliptifactus()
{
	signed short rounds;
	unsigned short ae;

	/* ask how many rounds */
	rounds = GUI_input(get_dtp(0x15c), 1);

	if (rounds != -1) {

		/* calculate the AE costs */
		ae = rounds * 2 + 5;

		if (host_readw(get_spelluser() + 0x64) >= ae) {
			/* set AP costs */
			ds_writew(0xac0e, ae);
			/* enable the spell */
			host_writeb(get_spelluser() + 0x97, rounds + 1);
			/* prepare the message */
			sprintf((char*)Real2Host(ds_readd(0xd2f3)),
				(char*)get_dtp(0x160),
				(char*)(get_spelluser() + 0x10),
				(char*)Real2Host(GUI_get_ptr(host_readb(get_spelluser() + 0x22), 2)),
				rounds);
		} else {
			/* prepare the message */
			sprintf((char*)Real2Host(ds_readd(0xd2f3)),
				(char*)get_ltx(0x97e),
				(char*)get_spelluser() + 0x10);
			/* set costs to 0 */
			ds_writew(0xac0e, 0);
		}
	} else {
		ds_writew(0xac0e, -2);
	}

}

void spell_fulminictus(void)
{
	signed short damage;

	if ((host_readb(get_spelluser() + 0x86) < 10) &&
		get_hero(host_readb(get_spelluser() + 0x86) - 1) == get_spelluser()) {

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
	ds_writed(SPELLTARGET, ds_readd(HEROS) + 0x6da * target);


	/* +5 on AT of the current weapon */
	slot = get_free_mod_slot();

	set_mod_slot(slot, abs(rounds) * 9,
		Real2Host(ds_readd(SPELLTARGET)) + 0x68 + host_readbs(Real2Host(ds_readd(SPELLTARGET)) + 0x78),
		5, (signed char)target);

	/* -5 on PA of the current weapon */
	slot = get_free_mod_slot();

	set_mod_slot(slot, abs(rounds) * 9,
		Real2Host(ds_readd(SPELLTARGET)) + 0x6f + host_readbs(Real2Host(ds_readd(SPELLTARGET)) + 0x78),
		-5, (signed char)target);

	/* TODO: this position is unknown */
	slot = get_free_mod_slot();

	set_mod_slot(slot, abs(rounds) * 9, Real2Host(ds_readd(SPELLTARGET)) + 0x98, 5, (signed char)target);

	/* set ae costs */
	ds_writew(0xac0e, rounds);

	/* prepare message */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(96 * 4),
		(char*)Real2Host(ds_readd(SPELLTARGET)) + 0x10);

}

void spell_scharfes_auge(void)
{
	signed short target;
	signed short slot;

	/* get the index of the hero on whom the spell is cast */
	target = host_readbs(get_spelluser() + 0x86) - 1;

	/* set a pointer to the target */
	ds_writed(SPELLTARGET, ds_readd(HEROS) + 0x6da * target);

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
void spell_hexenblick()
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Hexenblick\" ist nicht implementiert\n");
#endif
}

void spell_necropathia()
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Necropathia\" ist nicht implementiert\n");
#endif
}

#if !defined(__BORLANDC__)
}
#endif
