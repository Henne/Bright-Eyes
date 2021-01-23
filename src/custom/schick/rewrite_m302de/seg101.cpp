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
	ds_writew(SPELL_SPECIAL_AECOST, -2);
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
		(char*)get_tx(98),
		(char*)Real2Host(ds_readd(SPELLTARGET)) + HERO_NAME2);
}

void spell_armatrutz(void)
{
	signed short max_bonus;
	signed short pos;
	signed short bonus;
	signed short slot;

	max_bonus = 0;

	/* calc the maximal RS bonus */
#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug: you can get one RS point more that you have AE for */
	while (max_bonus * max_bonus < host_readws(get_spelluser() + HERO_AE)) {
		max_bonus++;
	}
#else
	while (max_bonus * max_bonus <= host_readws(get_spelluser() + HERO_AE)) {
		max_bonus++;
	}
	--max_bonus;
#endif;

	/* the maximum bonus is 9 */
	if (max_bonus > 9)
		max_bonus = 9;

	/* aks the user which bonus he wants */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx(99), max_bonus);
	bonus = GUI_input(Real2Host(ds_readd(DTP2)), 1);

	/* fix wrong input */
	if (bonus > max_bonus)
		bonus = max_bonus;

	if (bonus != -1) {

		pos = get_hero_index(get_spelluser());
		ds_writew(SPELL_SPECIAL_AECOST, bonus * bonus);
		slot = get_free_mod_slot();
		set_mod_slot(slot, MINUTES(5),
			get_spelluser() + HERO_RS_BONUS1,
			(signed char)bonus, (signed char)pos);

		/* prepare output message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(100),
			(char*)get_spelluser() + HERO_NAME2,
			bonus);

	} else {
		/* spell canceled */
		/* set AE to 0 */
		ds_writew(SPELL_SPECIAL_AECOST, 0);
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
		ds_writew(SPELL_SPECIAL_AECOST, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(112));

		return;
	}

	/* check if CH was already increased */
	if (host_readbs(get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_CH)) > host_readbs(get_spelltarget() + (HERO_ATTRIB_ORIG + 3 * ATTRIB_CH))) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(113),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(414));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* CH + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_CH), 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(101),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(414));
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
			(char*)get_tx(102),
			(char*)get_spelluser() + HERO_NAME2);
	} else {
		/* set AP costs to 0 */
		ds_writew(SPELL_SPECIAL_AECOST, 0);
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
		ds_writew(SPELL_SPECIAL_AECOST, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(112));

		return;
	}

	/* check if FF was already increased */
	if (host_readbs(get_spelltarget() + 0x3e) > host_readbs(get_spelltarget() + (HERO_ATTRIB_ORIG + 3 * ATTRIB_FF))) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(113),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(415));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* FF + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + 0x3e, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(101),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(415));
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
		ds_writew(SPELL_SPECIAL_AECOST, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(112));

		return;
	}

	/* check if GE was already increased */
	if (host_readbs(get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_GE)) > host_readbs(get_spelltarget() + (HERO_ATTRIB_ORIG + 3 * ATTRIB_GE))) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(113),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(416));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* GE + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_GE), 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(101),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(416));
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
		ds_writew(SPELL_SPECIAL_AECOST, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(112));

		return;
	}

	/* check if IN was already increased */
	if (host_readbs(get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_IN)) > host_readbs(get_spelltarget() + (HERO_ATTRIB_ORIG + 3 * ATTRIB_IN))) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(113),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(417));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* IN + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_IN), 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(101),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(417));
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
		ds_writew(SPELL_SPECIAL_AECOST, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(112));

		return;
	}

	/* check if KK was already increased */
	if (host_readbs(get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_KK)) > host_readbs(get_spelltarget() + (HERO_ATTRIB_ORIG + 3 * ATTRIB_KK))) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(113),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(418));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* IN + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_KK), 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(101),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(418));
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
		ds_writew(SPELL_SPECIAL_AECOST, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(112));

		return;
	}

	/* check if KL was already increased */
	if (host_readbs(get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_KL)) > host_readbs(get_spelltarget() + (HERO_ATTRIB_ORIG + 3 * ATTRIB_KL))) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(113),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(413));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* KL + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_KL), 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(101),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(413));
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
		ds_writew(SPELL_SPECIAL_AECOST, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(112));

		return;
	}

	/* check if MU was already increased */
	if (host_readbs(get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_MU)) > host_readbs(get_spelltarget() + (HERO_ATTRIB_ORIG + 3 * ATTRIB_MU))) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(113),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(412));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* MU + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), get_spelltarget() + (HERO_ATTRIB + 3 * ATTRIB_MU), 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(101),
			(char*)get_spelltarget() + HERO_NAME2,
			(char*)get_ttx(412));
	}
}

void spell_mutabili(void)
{
	/* triggers the "spell failed" messages */
	ds_writew(SPELL_SPECIAL_AECOST, -2);
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Mutabili\" ist nicht implementiert\n");
#endif
}

void spell_paralue(void)
{

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) >= 10) {
		/* cast an enemy */

		/* BC-TODO: calculation of ptr could be better */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* set the enemy to petrified */
		or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_STATUS1, 0x04);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(103),
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
				ds_writew(SPELL_SPECIAL_AECOST, 0);

				strcpy((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(112));
			} else {
				/* set the hero to petrified */
				or_ptr_bs(get_spelltarget() + HERO_STATUS1, 0x4);

				/* prepare message */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(103),
					(char*)get_spelltarget() + HERO_NAME2);
			}
		} else {
			/* set AE to 0 */
			ds_writew(SPELL_SPECIAL_AECOST, 0);
		}
	}
}

/**
 * \brief   Salander Mutanderer hero spell
 *
 * Turns the target into a mushroom
 */
void spell_salander(void)
{
	signed short ae_cost;

	/* BC-TODO: calculation of ptr could be better */
	/* set a pointer */
	ds_writed(SPELLTARGET_E, (Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

	/* read a value from that struct */
	ae_cost = host_readbs(get_spelltarget_e() + ENEMY_SHEET_MR) * 3;

	/* set the minimal astral cost to 25 AE */
	if (ae_cost < 25)
		ae_cost = 25;

	if (host_readws(get_spelluser() + HERO_AE) >= ae_cost) {

		or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_STATUS1, 0x40); /* sets 'mushroom' status bit */

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(104),
			(char*)Real2Host(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1)));

		/* set AE cost */
		ds_writew(SPELL_SPECIAL_AECOST, ae_cost);
	} else {
		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(607),
			(char*)Real2Host(ds_readd(SPELLUSER)) + HERO_NAME2);

		/* no AE cost */
		ds_writew(SPELL_SPECIAL_AECOST, 0);
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
		(char*)get_tx(105), (char*)get_spelluser() + HERO_NAME2);
	rounds = GUI_input(Real2Host(ds_readd(DTP2)), 2);

	/* the spell has also no effect if it is already active */
	if ((rounds <= 0) || (host_readb(get_spelluser() + HERO_INVISIBLE) != 0)) {

		/* set AE to 0 */
		ds_writew(SPELL_SPECIAL_AECOST, 0);
		/* clear output string */
		host_writeb(Real2Host(ds_readd(DTP2)), 0);

		return;
	}

	/* check if the hero has enough AE */
	if (rounds * 5 <= host_readws(get_spelluser() + HERO_AE)) {

		ds_writew(SPELL_SPECIAL_AECOST, rounds * 5);
		pos = (signed short)get_hero_index(get_spelluser());
		slot = get_free_mod_slot();
		set_mod_slot(slot, (Bit32s)rounds * MINUTES(5), get_spelluser() + HERO_INVISIBLE, 1, (signed char)pos);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(106),
			(char*)get_spelluser() + HERO_NAME2,
			(char*)Real2Host(GUI_get_ptr(host_readbs(get_spelluser() + HERO_SEX), 0)));
	} else {
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(607),
			(char*)get_spelluser() + HERO_NAME2);

		ds_writew(SPELL_SPECIAL_AECOST, 0);
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

	ds_writew(SPELL_SPECIAL_AECOST, 0);

	if (ds_readw(LIGHT_TYPE) == IS_TORCH) {
		torch_pos = get_item_pos(get_spelluser(), ITEM_TORCH_OFF);
	} else {
		if (ds_readw(LIGHT_TYPE) == IS_LANTERN) {
		} else {
			torch_pos = get_item_pos(get_spelluser(), ITEM_TORCH_OFF);
		}

		lantern_pos = get_item_pos(get_spelluser(), ITEM_LANTERN_OFF);
	}

	if (torch_pos != -1) {
		if (lantern_pos != -1) {
			/* lantern and torch are available, must decide */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(107),
				(char*)get_spelluser() + HERO_NAME2);

			answer = GUI_radio(Real2Host(ds_readd(DTP2)),
					2,
					(char*)Real2Host(GUI_names_grammar(0x4000, ITEM_TORCH_OFF, 0)),
					(char*)Real2Host(GUI_names_grammar(0x4000, ITEM_LANTERN_OFF, 0)));

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
		host_writew(get_spelluser() + HERO_ITEM_HEAD + torch_pos * 14, ITEM_TORCH_ON);

		/* set counter to 10 */
		host_writeb(get_spelluser() + HERO_ITEM_HEAD  + 8 + torch_pos * 14, 10);

		/* set AP cost */
		ds_writew(SPELL_SPECIAL_AECOST, random_schick(20));

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(108),
			get_spelluser() + HERO_NAME2);

	} else if (lantern_pos != -1) {

		/* get position of oil */
		oil_pos = get_item_pos(get_spelluser(), ITEM_OIL);

		if (oil_pos != -1) {

			/* change torch to burning lantern */
			host_writew(get_spelluser() + HERO_ITEM_HEAD + lantern_pos * 14, ITEM_LANTERN_ON);

			/* set counter to 100 */
			host_writeb(get_spelluser() + HERO_ITEM_HEAD  + 8 + lantern_pos * 14, 100);

			/* drop one oil flask */
			drop_item(get_spelluser(), oil_pos, 1);

			/* give bronze flask */
			give_hero_new_item(get_spelluser(), ITEM_FLASK_BRONZE, 0, 1);

			/* set AP cost */
			ds_writew(SPELL_SPECIAL_AECOST, random_schick(20));

			/* prepare message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(119),
				get_spelluser() + HERO_NAME2);
		} else {
			/* prepare message */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(120),
				get_spelluser() + HERO_NAME2);
		}
	} else {
		/* neither torch nor lantern */

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(121),
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
		(char*)get_tx(109));

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
		(char*)get_tx(110));

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
		if ((host_readb(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
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
	ds_writew(SPELL_SPECIAL_AECOST, 5);

	/* copy message text */
	strcpy((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx(111));
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
