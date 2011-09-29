/*
 *	Rewrite of DSA1 v3.02_de functions of seg101 (spells 3/3)
 *	Spells: Transformation / Transmutation
 *	Functions rewritten 25/26
 *
*/

#include "schick.h"

#include "string.h"

#include "v302de.h"

#include "seg002.h"
#include "seg096.h"
#include "seg097.h"

namespace M302de {

/* Transformation / Verwandlung */
void spell_adler() {
	/* triggers the "spell failed" messages */
	ds_writew(0xac0e, -2);
	D1_INFO("Zauberspruch \"Adler, Wolf und Hammerhai\" ist nicht implementiert\n");
}

void spell_arcano() {

	Bit8u *tp;
	unsigned short slot;
	signed short target;

	/* get the spell target */
	target = (signed char)host_readb(get_spelluser() + 0x86) - 1;

	ds_writed(0xe5b8, ds_readd(HEROS) + target * 0x6da);
	tp = Real2Host(ds_readd(0xe5b8));

	/* get a free mod_slot */
	slot = get_free_mod_slot();

	/* MR + 2 for 1 h */
	set_mod_slot(slot, 0x1518, tp + 0x66, 2, target);

	/* "Die Magieresistenz von %s steigt um 2 Punkte." */
	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_dtp(98 * 4),
		(char*)tp + 0x10);
}

void spell_armatrutz()
{
	unsigned short max_boni, pos, slot;
	signed short boni;

	max_boni = 0;

	/* calc the maximal RS boni */
	/* Original-Bug: you can get one RS point more that you have AE for */
	while (max_boni * max_boni < host_readw(get_spelluser() + 0x64)) {
		max_boni++;
	}

	/* the maximum boni is 9 */
	if (max_boni > 9)
		max_boni = 9;

	/* aks the user which boni he wants */
	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_dtp(0x18c), max_boni);
	boni = GUI_input(Real2Host(ds_readd(0xd2f3)), 1);

	/* fix wrong input */
	if (boni > max_boni)
		boni = max_boni;

	if (boni != -1) {

		pos = get_hero_index(get_spelluser());
		ds_writew(0xac0e, boni * boni);
		slot = get_free_mod_slot();
		set_mod_slot(slot, 450, get_spelluser() + 0x30, boni, pos);

		/* prepare output message */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(0x190), (char*)get_spelluser() + 0x10, boni);

	} else {
		/* spell canceled */
		/* set AE to 0 */
		ds_writew(0xac0e, 0);
		/* avoid the textbox output */
		host_writeb(Real2Host(ds_readd(0xd2f3)), 0);
	}
}

void spell_inc_ch() {

	Bit8u *tp;
	unsigned short slot;
	signed short target;

	/* get the spell target */
	target = (signed char)host_readb(get_spelluser() + 0x86) - 1;

	ds_writed(0xe5b8, ds_readd(HEROS) + target * 0x6da);
	tp = Real2Host(ds_readd(0xe5b8));

	/* check if the target is the spelluser */
	if (tp == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if CH was already increased */
	if (host_readb(tp + 0x3b) > host_readb(tp + 0x3a)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(113 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(414 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* CH + 2 for 2 hours */
		set_mod_slot(slot, 0x2a30, tp + 0x3b, 2, target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(101 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(414 * 4));
	}
}

void spell_feuerbann()
{
	unsigned short slot, pos;

	/* check if spell is already activated */
	if (host_readb(get_spelluser() + 0x99) == 0) {

		pos = get_hero_index(get_spelluser());

		slot = get_free_mod_slot();

		/* Duration = Level * 12 min */
		set_mod_slot(slot, host_readb(get_spelluser() + 0x27) * 450,
			get_spelluser() + 0x99, 1, pos);

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(0x198),
			(char*)get_spelluser() + 0x10);
	} else {
		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);
	}
}

void spell_inc_ff() {

	Bit8u *tp;
	unsigned short slot;
	signed short target;

	/* get the spell target */
	target = (signed char)host_readb(get_spelluser() + 0x86) - 1;

	ds_writed(0xe5b8, ds_readd(HEROS) + target * 0x6da);
	tp = Real2Host(ds_readd(0xe5b8));

	/* check if the target is the spelluser */
	if (tp == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if FF was already increased */
	if (host_readb(tp + 0x3e) > host_readb(tp + 0x3d)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(113 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(415 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* FF + 2 for 2 hours */
		set_mod_slot(slot, 0x2a30, tp + 0x3e, 2, target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(101 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(415 * 4));
	}
}

void spell_inc_ge() {

	Bit8u *tp;
	unsigned short slot;
	signed short target;

	/* get the spell target */
	target = (signed char)host_readb(get_spelluser() + 0x86) - 1;

	ds_writed(0xe5b8, ds_readd(HEROS) + target * 0x6da);
	tp = Real2Host(ds_readd(0xe5b8));

	/* check if the target is the spelluser */
	if (tp == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if GE was already increased */
	if (host_readb(tp + 0x41) > host_readb(tp + 0x40)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(113 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(416 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* GE + 2 for 2 hours */
		set_mod_slot(slot, 0x2a30, tp + 0x41, 2, target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(101 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(416 * 4));
	}
}

void spell_inc_in() {

	Bit8u *tp;
	unsigned short slot;
	signed short target;

	/* get the spell target */
	target = (signed char)host_readb(get_spelluser() + 0x86) - 1;

	ds_writed(0xe5b8, ds_readd(HEROS) + target * 0x6da);
	tp = Real2Host(ds_readd(0xe5b8));

	/* check if the target is the spelluser */
	if (tp == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if IN was already increased */
	if (host_readb(tp + 0x44) > host_readb(tp + 0x43)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(113 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(417 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* IN + 2 for 2 hours */
		set_mod_slot(slot, 0x2a30, tp + 0x44, 2, target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(101 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(417 * 4));
	}
}

void spell_inc_kk() {

	Bit8u *tp;
	unsigned short slot;
	signed short target;

	/* get the spell target */
	target = (signed char)host_readb(get_spelluser() + 0x86) - 1;

	ds_writed(0xe5b8, ds_readd(HEROS) + target * 0x6da);
	tp = Real2Host(ds_readd(0xe5b8));

	/* check if the target is the spelluser */
	if (tp == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if KK was already increased */
	if (host_readb(tp + 0x47) > host_readb(tp + 0x46)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(113 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(418 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* IN + 2 for 2 hours */
		set_mod_slot(slot, 0x2a30, tp + 0x47, 2, target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(101 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(418 * 4));
	}
}

void spell_inc_kl() {

	Bit8u *tp;
	unsigned short slot;
	signed short target;

	/* get the spell target */
	target = (signed char)host_readb(get_spelluser() + 0x86) - 1;

	ds_writed(0xe5b8, ds_readd(HEROS) + target * 0x6da);
	tp = Real2Host(ds_readd(0xe5b8));

	/* check if the target is the spelluser */
	if (tp == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if KL was already increased */
	if (host_readb(tp + 0x38) > host_readb(tp + 0x37)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(113 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(413 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* KL + 2 for 2 hours */
		set_mod_slot(slot, 0x2a30, tp + 0x38, 2, target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(101 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(413 * 4));
	}
}

void spell_inc_mu() {

	Bit8u *tp;
	unsigned short slot;
	signed short target;

	/* get the spell target */
	target = (signed char)host_readb(get_spelluser() + 0x86) - 1;

	ds_writed(0xe5b8, ds_readd(HEROS) + target * 0x6da);
	tp = Real2Host(ds_readd(0xe5b8));

	/* check if the target is the spelluser */
	if (tp == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if MU was already increased */
	if (host_readb(tp + 0x35) > host_readb(tp + 0x34)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(113 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(412 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* MU + 2 for 2 hours */
		set_mod_slot(slot, 0x2a30, tp + 0x35, 2, target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(101 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(412 * 4));
	}
}

void spell_mutabili() {
	/* triggers the "spell failed" messages */
	ds_writew(0xac0e, -2);
	D1_INFO("Zauberspruch \"Mutabili\" ist nicht implementiert\n");
}

void spell_paral()
{

	if (host_readb(get_spelluser() + 0x86) >= 10) {
		/* cast an enemy */
		ds_writed(0xe5b4, RealMake(datseg, 0xd0df + host_readb(get_spelluser() + 0x86) * 62));

		host_writeb(Real2Host(ds_readd(0xe5b4)) + 0x31,
			host_readb(Real2Host(ds_readd(0xe5b4)) + 0x31) | 0x04);

		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(0x19c),
			(char*)Real2Host(GUI_names_grammar(0x8000, host_readb(Real2Host(ds_readd(0xe5b4))), 1)));
	} else {
		/* cast a hero */
		/* TODO: the first check can be removed, cause it would not give a message */
		if (get_spelluser() != get_spelltarget()) {

			/* set the target  */
			ds_writed(SPELLTARGET, ds_readd(HEROS) + (host_readb(get_spelluser() + 0x86) - 1) * 0x6da);

			/* check again */
			if (get_spelluser() == get_spelltarget()) {

				/* never cast yourself */
				ds_writew(0xac0e, 0);

				strcpy((char*)Real2Host(ds_readd(0xd2f3)),
					(char*)get_dtp(0x1c0));
			} else {
				/* set the hero to stoned */
				host_writeb(get_spelltarget() + 0xaa, host_readb(get_spelltarget() + 0xaa) | 0x4);

				/* prepare message */
				sprintf((char*)Real2Host(ds_readd(0xd2f3)),
					(char*)get_dtp(0x19c),
					(char*)get_spelltarget() + 0x10);
			}
		} else {
			/* set AE to 0 */
			ds_writew(0xac0e, 0);
		}
	}

}

void spell_salander()
{
	Bit16s ae_cost;

	/* set a pointer */
	ds_writed(0xe5b4, RealMake(datseg, 0xd0df + (signed char)host_readb(get_spelluser() + 0x86) * 62));

	/* read a value from that struct */
	ae_cost = (signed char)host_readb(Real2Host(ds_readd(0xe5b4)) + 0x19) * 3;

	/* set the minimal astral cost to 25 AE */
	if (ae_cost < 25)
		ae_cost = 25;

	if (host_readw(get_spelluser() + 0x64) >= ae_cost) {

		host_writeb(Real2Host(ds_readd(0xe5b4)) + 0x31,
			host_readb(Real2Host(ds_readd(0xe5b4)) + 0x31) | 0x40);


		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(0x1a0),
			(char*)Real2Host(GUI_names_grammar(0x8000, host_readb(Real2Host(ds_readd(0xe5b4))), 1)));

		/* set AE cost */
		ds_writew(0xac0e, ae_cost);
	} else {
		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_ltx(0x97c),
			(char*)Real2Host(ds_readd(0xe5bc) + 0x10));

		/* no AE cost */
		ds_writew(0xac0e, 0);
	}
}

void spell_see() {
	D1_INFO("Zauberspruch \"See und Fluss\" ist nicht implementiert\n");
}

void spell_visibili()
{
	unsigned short pos, slot;
	signed short rounds;

	/* ask the user how many rounds he wants to be invisible */
	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_dtp(0x1a4), (char*)get_spelluser() + 0x10);
	rounds = GUI_input(Real2Host(ds_readd(0xd2f3)), 2);

	/* the spell has also no effect if it is already active */
	if ((rounds <= 0) || (host_readb(get_spelluser() + 0x9a) != 0)) {

		/* set AE to 0 */
		ds_writew(0xac0e, 0);
		/* clear output string */
		host_writeb(Real2Host(ds_readd(0xd2f3)), 0);

		return;
	}

	/* check if the hero has enough AE */
	if (rounds * 5 <= host_readw(get_spelluser() + 0x64)) {

		ds_writew(0xac0e, rounds * 5);
		pos = get_hero_index(get_spelluser());
		slot = get_free_mod_slot();
		set_mod_slot(slot, rounds * 450, get_spelluser() + 0x9a, 1, pos);
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(0x1a8),
			(char*)get_spelluser() + 0x10,
			(char*)Real2Host(GUI_get_ptr(host_readb(get_spelluser() + 0x22), 0)));
	} else {
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_ltx(0x97c),
			(char*)get_spelluser() + 0x10);

		ds_writew(0xac0e, 0);
	}

}

/* Transmutation / Veraenderung */

void spell_abvenenum() {
	D1_INFO("Zauberspruch \"Abvenenum\" ist nicht implementiert\n");
}

void spell_aeolitus() {
	D1_INFO("Zauberspruch \"Aeolitus\" ist nicht implementiert\n");
}

void spell_claudibus() {
	D1_INFO("Zauberspruch \"Claudibus\" ist nicht implementiert\n");
}

void spell_dunkelheit() {

	signed char level = host_readb(get_spelluser() + 0x27);

	/* set dunkelheit duration (level + 3) hours */
	ds_writed(0x2dc4 + 0x24, (level + 3) * 0x1518);

	/* copy message text */
	strcpy((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_dtp(109 * 4));

}

void spell_erstarre() {
	D1_INFO("Zauberspruch \"Erstarre\" ist nicht implementiert\n");
}

void spell_flimflam() {

	signed char level = host_readb(get_spelluser() + 0x27);

	/* set flim flam duration (level + 3) hours */
	ds_writed(0x2dc4 + 0x20, (level + 3) * 0x1518);

	/* copy message text */
	strcpy((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_dtp(110 * 4));

}

void spell_schmelze() {
	D1_INFO("Zauberspruch \"Schmelze\" ist nicht implementiert\n");
}

void spell_silentium() {

	Bit8u *hero;
	unsigned short i;
	unsigned short slot;

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero += 0x6da) {
		/* check class */
		if (host_readb(hero + 0x21) == 0)
			continue;
		/* check group */
		if (host_readb(hero + 0x87) != ds_readb(0x2d35))
			continue;
		/* check dead */
		if (host_readb(hero + 0xaa) & 1)
			continue;

		/* get a free mod_slot */
		slot = get_free_mod_slot();
		/* skill stealth + 10 for 12 minutes */
		set_mod_slot(slot, 0x1c2, hero + 0x115, 10, i);
	}

	/* set AP cost */
	ds_writew(0xac0e, 5);

	/* copy message text */
	strcpy((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_dtp(111 * 4));
}

void spell_sturmgebr() {
	D1_INFO("Zauberspruch \"Sturmgebruell\" ist nicht implementiert\n");
}

}
