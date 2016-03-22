/*
 *	Rewrite of DSA1 v3.02_de functions of seg101 (spells 3/3)
 *	Spells: Transformation / Transmutation
 *	Functions rewritten: 26/26 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg101.cpp
 *
*/

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg096.h"
#include "seg097.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Transformation / Verwandlung */
void spell_adler(void)
{
	/* triggers the "spell failed" messages */
	ds_writew(0xac0e, -2);
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Adler, Wolf und Hammerhai\" ist nicht implementiert\n");
#endif
}

void spell_arcano(void)
{
	signed short target;
	unsigned short slot;

	/* get the spell target */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	ds_writed(SPELLTARGET, (Bit32u)((RealPt)ds_readd(HEROS) + target * SIZEOF_HERO));


	/* get a free mod_slot */
	slot = get_free_mod_slot();

	/* MR + 2 for 1 h */
	set_mod_slot(slot, HOURS(1), get_spelltarget() + HERO_MR, 2,
			(signed char)target);

	/* "Die Magieresistenz von %s steigt um 2 Punkte." */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(98 * 4),
		(char*)Real2Host((RealPt)ds_readd(SPELLTARGET) + 0x10));
}

void spell_armatrutz(void)
{
	signed short max_boni;
	signed short pos;
	signed short boni;
	signed short slot;

	max_boni = 0;

	/* calc the maximal RS boni */
	/* Original-Bug: you can get one RS point more that you have AE for */
	while (max_boni * max_boni < host_readws(get_spelluser() + HERO_AE)) {
		max_boni++;
	}

	/* the maximum boni is 9 */
	if (max_boni > 9)
		max_boni = 9;

	/* aks the user which boni he wants */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x18c), max_boni);
	boni = GUI_input(Real2Host(ds_readd(DTP2)), 1);

	/* fix wrong input */
	if (boni > max_boni)
		boni = max_boni;

	if (boni != -1) {

		pos = get_hero_index(get_spelluser());
		ds_writew(0xac0e, boni * boni);
		slot = get_free_mod_slot();
		set_mod_slot(slot, MINUTES(5), get_spelluser() + HERO_RS_BONUS1,
			(signed char)boni, (signed char)pos);

		/* prepare output message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x190), (char*)get_spelluser() + HERO_NAME2, boni);

	} else {
		/* spell canceled */
		/* set AE to 0 */
		ds_writew(0xac0e, 0);
		/* avoid the textbox output */
		host_writeb(Real2Host(ds_readd(DTP2)), 0);
	}
}

void spell_inc_ch(void)
{

	signed short target;
	signed short slot;

	/* get the spell target */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	ds_writed(SPELLTARGET, (Bit32u)((RealPt)ds_readd(HEROS) + target * SIZEOF_HERO));

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if CH was already increased */
	if (host_readbs(get_spelltarget() + HERO_CH) > host_readbs(get_spelltarget() + HERO_CH_ORIG)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(113 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(414 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* CH + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + HERO_CH, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(101 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(414 * 4));
	}
}

void spell_feuerbann(void)
{
	signed short target;
	signed short slot;

	/* check if spell is already activated */
	if (!host_readbs(get_spelluser() + HERO_FIREBAN)) {

		target = get_hero_index(get_spelluser());

		slot = get_free_mod_slot();

		/* Duration = Level * 12 min */
		set_mod_slot(slot, host_readbs(get_spelluser() + HERO_LEVEL) * MINUTES(5),
			get_spelluser() + HERO_FIREBAN, 1, (signed char)target);

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x198),
			(char*)get_spelluser() + HERO_NAME2);
	} else {
		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);
	}
}

void spell_inc_ff(void)
{

	signed short target;
	signed short slot;

	/* get the spell target */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	ds_writed(SPELLTARGET, (Bit32u)((RealPt)ds_readd(HEROS) + target * SIZEOF_HERO));

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if FF was already increased */
	if (host_readbs(get_spelltarget() + 0x3e) > host_readbs(get_spelltarget() + HERO_FF_ORIG)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(113 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(415 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* FF + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + 0x3e, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(101 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(415 * 4));
	}
}

void spell_inc_ge(void)
{

	signed short target;
	signed short slot;

	/* get the spell target */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	ds_writed(SPELLTARGET, (Bit32u)((RealPt)ds_readd(HEROS) + target * SIZEOF_HERO));

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if GE was already increased */
	if (host_readbs(get_spelltarget() + HERO_GE) > host_readbs(get_spelltarget() + HERO_GE_ORIG)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(113 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(416 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* GE + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + HERO_GE, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(101 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(416 * 4));
	}
}

void spell_inc_in(void)
{

	signed short target;
	signed short slot;

	/* get the spell target */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	ds_writed(SPELLTARGET, (Bit32u)((RealPt)ds_readd(HEROS) + target * SIZEOF_HERO));

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if IN was already increased */
	if (host_readbs(get_spelltarget() + HERO_IN) > host_readbs(get_spelltarget() + HERO_IN_ORIG)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(113 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(417 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* IN + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + HERO_IN, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(101 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(417 * 4));
	}
}

void spell_inc_kk(void)
{

	signed short target;
	signed short slot;

	/* get the spell target */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	ds_writed(SPELLTARGET, (Bit32u)((RealPt)ds_readd(HEROS) + target * SIZEOF_HERO));

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if KK was already increased */
	if (host_readbs(get_spelltarget() + HERO_KK) > host_readbs(get_spelltarget() + HERO_KK_ORIG)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(113 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(418 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* IN + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + HERO_KK, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(101 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(418 * 4));
	}
}

void spell_inc_kl(void)
{

	signed short target;
	signed short slot;

	/* get the spell target */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	ds_writed(SPELLTARGET, (Bit32u)((RealPt)ds_readd(HEROS) + target * SIZEOF_HERO));

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if KL was already increased */
	if (host_readbs(get_spelltarget() + HERO_KL) > host_readbs(get_spelltarget() + HERO_KL_ORIG)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(113 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(413 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* KL + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + HERO_KL, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(101 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(413 * 4));
	}
}

void spell_inc_mu(void)
{

	signed short target;
	signed short slot;

	/* get the spell target */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	ds_writed(SPELLTARGET, (Bit32u)((RealPt)ds_readd(HEROS) + target * SIZEOF_HERO));

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if MU was already increased */
	if (host_readbs(get_spelltarget() + HERO_MU) > host_readbs(get_spelltarget() + HERO_MU_ORIG)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(113 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(412 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* MU + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + HERO_MU, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(101 * 4),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ltx(412 * 4));
	}
}

void spell_mutabili(void)
{
	/* triggers the "spell failed" messages */
	ds_writew(0xac0e, -2);
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Mutabili\" ist nicht implementiert\n");
#endif
}

void spell_paral(void)
{

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) >= 10) {
		/* cast an enemy */

		/* BC-TODO: calculation of ptr could be better */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* set the enemy to stoned */
		or_ptr_bs(get_spelltarget_e() + 0x31, 0x04);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x19c),
			(char*)Real2Host(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1)));
	} else {
		/* cast a hero */
		/* TODO: the first check can be removed, cause it would not give a message */
		if (get_spelltarget() != get_spelluser()) {

			/* set the target  */
			ds_writed(SPELLTARGET,
				(Bit32u)((RealPt)ds_readd(HEROS) + (host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) * SIZEOF_HERO));

			/* check again */
			if (get_spelltarget() == get_spelluser()) {

				/* never cast yourself */
				ds_writew(0xac0e, 0);

				strcpy((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x1c0));
			} else {
				/* set the hero to stoned */
				or_ptr_bs(get_spelltarget() + 0xaa, 0x4);

				/* prepare message */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x19c),
					(char*)get_spelltarget() + 0x10);
			}
		} else {
			/* set AE to 0 */
			ds_writew(0xac0e, 0);
		}
	}
}

void spell_salander(void)
{
	signed short ae_cost;

	/* BC-TODO: calculation of ptr could be better */
	/* set a pointer */
	ds_writed(SPELLTARGET_E, (Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

	/* read a value from that struct */
	ae_cost = host_readbs(get_spelltarget_e() + 0x19) * 3;

	/* set the minimal astral cost to 25 AE */
	if (ae_cost < 25)
		ae_cost = 25;

	if (host_readws(get_spelluser() + HERO_AE) >= ae_cost) {

		or_ptr_bs(get_spelltarget_e() + 0x31, 0x40);

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x1a0),
			(char*)Real2Host(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1)));

		/* set AE cost */
		ds_writew(0xac0e, ae_cost);
	} else {
		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0x97c),
			(char*)Real2Host(ds_readd(SPELLUSER)) + 0x10);

		/* no AE cost */
		ds_writew(0xac0e, 0);
	}
}

void spell_see(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"See und Fluss\" ist nicht implementiert\n");
#endif
}

void spell_visibili(void)
{
	signed short slot;
	signed short pos;
	signed short rounds;

	/* ask the user how many rounds he wants to be invisible */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x1a4), (char*)get_spelluser() + HERO_NAME2);
	rounds = GUI_input(Real2Host(ds_readd(DTP2)), 2);

	/* the spell has also no effect if it is already active */
	if ((rounds <= 0) || (host_readb(get_spelluser() + HERO_INVISIBLE) != 0)) {

		/* set AE to 0 */
		ds_writew(0xac0e, 0);
		/* clear output string */
		host_writeb(Real2Host(ds_readd(DTP2)), 0);

		return;
	}

	/* check if the hero has enough AE */
	if (rounds * 5 <= host_readws(get_spelluser() + HERO_AE)) {

		ds_writew(0xac0e, rounds * 5);
		pos = (signed short)get_hero_index(get_spelluser());
		slot = get_free_mod_slot();
		set_mod_slot(slot, (Bit32s)rounds * MINUTES(5), get_spelluser() + HERO_INVISIBLE, 1, (signed char)pos);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x1a8),
			(char*)get_spelluser() + HERO_NAME2,
			(char*)Real2Host(GUI_get_ptr(host_readbs(get_spelluser() + HERO_SEX), 0)));
	} else {
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0x97c),
			(char*)get_spelluser() + HERO_NAME2);

		ds_writew(0xac0e, 0);
	}

}

/* Transmutation / Veraenderung */

void spell_abvenenum(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Abvenenum\" ist nicht implementiert\n");
#endif
}

void spell_aeolitus(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Aeolitus\" ist nicht implementiert\n");
#endif
}

void spell_brenne(void)
{
	signed short oil_pos;
	signed short answer;

	signed short torch_pos;
	signed short lantern_pos;

	torch_pos = -1;
	lantern_pos = -1;

	ds_writew(0xac0e, 0);

	if (ds_readw(LIGHT_TYPE) == 1) {
		torch_pos = get_item_pos(get_spelluser(), 0x41);
	} else {
		if (ds_readw(LIGHT_TYPE) == 2) {
		} else {
			torch_pos = get_item_pos(get_spelluser(), 0x41);
		}

		lantern_pos = get_item_pos(get_spelluser(), 0x19);
	}

	if (torch_pos != -1) {
		if (lantern_pos != -1) {
			/* lantern and torch are available, must decide */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(107 * 4),
				(char*)get_spelluser() + HERO_NAME2);

			answer = GUI_radio(Real2Host(ds_readd(DTP2)),
					2,
					(char*)Real2Host(GUI_names_grammar(0x4000, 0x41, 0)),
					(char*)Real2Host(GUI_names_grammar(0x4000, 0x19, 0)));

			if (answer == -1) {
				/* abort */
				torch_pos = lantern_pos = -1;
			} else if (answer == 1) {
				lantern_pos = -1;
			} else {
				torch_pos = -1;
			}
		}
	}

	if (torch_pos != -1) {

		/* change torch to burning torch */
		host_writew(get_spelluser() + HERO_ITEM_HEAD + torch_pos * 14, 0x16);

		/* set counter to 10 */
		host_writeb(get_spelluser() + HERO_ITEM_HEAD  + 8 + torch_pos * 14, 10);

		/* set AP cost */
		ds_writew(0xac0e, random_schick(20));

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(108 * 4),
			get_spelluser() + HERO_NAME2);

	} else if (lantern_pos != -1) {

		/* get position of oil */
		oil_pos = get_item_pos(get_spelluser(), 0x29);

		if (oil_pos != -1) {

			/* change torch to burning lantern */
			host_writew(get_spelluser() + HERO_ITEM_HEAD + lantern_pos * 14, 0xf9);

			/* set counter to 100 */
			host_writeb(get_spelluser() + HERO_ITEM_HEAD  + 8 + lantern_pos * 14, 100);

			/* drop one oil flask */
			drop_item(get_spelluser(), oil_pos, 1);

			/* give bronze flask */
			give_hero_new_item(get_spelluser(), 0x2a, 0, 1);

			/* set AP cost */
			ds_writew(0xac0e, random_schick(20));

			/* prepare message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(119 * 4),
				get_spelluser() + HERO_NAME2);
		} else {
			/* prepare message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(120 * 4),
				get_spelluser() + HERO_NAME2);
		}
	} else {
		/* neither torch nor lantern */

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(121 * 4),
			get_spelluser() + HERO_NAME2);
	}
}

void spell_claudibus(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Claudibus\" ist nicht implementiert\n");
#endif
}

void spell_dunkelheit(void)
{


	/* set dunkelheit duration (level + 3) hours */
	ds_writed(INGAME_TIMERS + 0x24,
		(Bit32s)(host_readbs(get_spelluser() + HERO_LEVEL) + 3) * 0x1518);

	/* copy message text */
	strcpy((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(109 * 4));

}

void spell_erstarre(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Erstarre\" ist nicht implementiert\n");
#endif
}

void spell_flimflam(void)
{


	/* set flim flam duration (level + 3) hours */
	ds_writed(INGAME_TIMERS + 0x20,
		(Bit32s)(host_readbs(get_spelluser() + HERO_LEVEL) + 3) * 0x1518);

	/* copy message text */
	strcpy((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(110 * 4));

}

void spell_schmelze(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Schmelze\" ist nicht implementiert\n");
#endif
}

void spell_silentium(void)
{

	signed short i;
	signed short slot;
	Bit8u *hero;

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {
		/* check class */
		if ((host_readb(hero + HERO_TYPE) != 0) &&
			/* check group */
			(host_readb(hero + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP)) &&
			/* check dead */
			!(hero_dead(hero))) {

			/* get a free mod_slot */
			slot = get_free_mod_slot();
			/* skill stealth + 10 for 5 minutes */
			set_mod_slot(slot, MINUTES(5), hero + (HERO_TA_BODY+4), 10, (signed char)i);
		}
	}

	/* set AP cost */
	ds_writew(0xac0e, 5);

	/* copy message text */
	strcpy((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(111 * 4));
}

void spell_sturmgebr(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Sturmgebruell\" ist nicht implementiert\n");
#endif
}

#if !defined(__BORLANDC__)
}
#endif
