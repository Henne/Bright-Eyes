/*
 *	Rewrite of DSA1 v3.02_de functions of seg101 (spells 3/3)
 *	Spells: Transformation / Transmutation
 *	Functions rewritten 13/26
 *
*/

#include "schick.h"

#include "string.h"

#include "v302de.h"

#include "seg002.h"

/* Transformation / Verwandlung */
void spell_adler() {
	/* triggers the "spell failed" messages */
	ds_writew(0xac0e, -2);
	D1_INFO("Zauberspruch \"Adler, Wolf und Hammerhai\" ist nicht implementiert\n");
}

void spell_inc_mu() {

	Bit8u *tp;
	unsigned short slot;
	signed short target;

	/* get the spell target */
	target = (signed char)host_readb(get_spelluser() + 0x86) - 1;

	ds_writed(0xe5b8, ds_readd(HEROS) + target * 0x6da);
	tp = MemBase + Real2Phys(ds_readd(0xe5b8));

	/* check if the target is the spelluser */
	if (tp == get_spelluser()) {

		/* set AP costs to 0 */
		ds_writew(0xac0e, 0);

		/* copy message text */
		strcpy((char*)MemBase + Real2Phys(ds_readd(0xd2f3)),
			(char*)get_dtp(112 * 4));

		return;
	}

	/* check if MU was already increased */
	if (host_readb(tp + 0x35) > host_readb(tp + 0x34)) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf((char*)MemBase + Real2Phys(ds_readd(0xd2f3)),
			(char*)get_dtp(113 * 4),
			(char*)tp + 0x10,
			(char*)get_ltx(412 * 4));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* MU + 2 for 2 hours */
		set_mod_slot(slot, 0x2a30, tp + 0x35, 2, target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf((char*)MemBase + Real2Phys(ds_readd(0xd2f3)),
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

void spell_see() {
	D1_INFO("Zauberspruch \"See und Fluss\" ist nicht implementiert\n");
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
	strcpy((char*)MemBase + Real2Phys(ds_readd(0xd2f3)),
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
	strcpy((char*)MemBase + Real2Phys(ds_readd(0xd2f3)),
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
	strcpy((char*)MemBase + Real2Phys(ds_readd(0xd2f3)),
		(char*)get_dtp(111 * 4));
}

void spell_sturmgebr() {
	D1_INFO("Zauberspruch \"Sturmgebruell\" ist nicht implementiert\n");
}
