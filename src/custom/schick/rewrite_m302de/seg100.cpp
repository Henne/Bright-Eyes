/*
 *	Rewrite of DSA1 v3.02_de functions of seg100 (spells 2/3)
 *	Spells: Clairvoyance / Illusion / Combat / Communication
 *	Functions rewritten 13/20
 *
*/

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg099.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Clairvoyance / Hellsicht */

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

void spell_sensibar()
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Sensibar\" ist nicht implementiert\n");
#endif
}

/* Illusion */
void spell_chamaelioni()
{
	/* set the flag for this spell */
	host_writeb(get_spelluser() + 0xaa,
		host_readb(get_spelluser() + 0xaa) | 0x10);

	/* prepare the message */
	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_dtp(0x14c),
		(char*)get_spelluser() + 0x10,
		(char*)Real2Host(GUI_get_ptr(host_readb(get_spelluser() + 0x22), 0)));

}

void spell_duplicatus()
{
	/* set the flag for this spell */
	host_writeb(get_spelluser() + 0xab,
		host_readb(get_spelluser() + 0xab) | 4);

	/* prepare the message */
	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_dtp(0x150),
		(char*)get_spelluser() + 0x10,
		(char*)Real2Host(GUI_get_ptr(host_readb(get_spelluser() + 0x22), 0)));

}

void spell_harmlos()
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
		ds_writed(0xe5b4,
			RealMake(datseg, 0xd0df + host_readb(get_spelluser() + 0x86) * 62));
#else
		ds_writed(0xe5b4,
			(Bit32u)MK_FP(datseg, 0xd0df + host_readb(get_spelluser() + 0x86) * 62));
#endif

		/* set the rounds counter */
		host_writeb(Real2Host(ds_readd(0xe5b4)) + 0x2f, 3);

		/* prepare the message */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(0x154),
			(char*)Real2Host(GUI_names_grammar(0x8000, host_readb(Real2Host(ds_readd(0xe5b4))), 1)));
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
