/*
 *	Rewrite of DSA1 v3.02_de functions of seg101 (spells 3/3)
 *	Spells: Transformation / Transmutation
 *	Functions rewritten 9/26
 *
*/

#include "schick.h"

/* Transformation / Verwandlung */
void spell_adler() {
	/* triggers the "spell failed" messages */
	ds_writew(0xac0e, -2);
	D1_INFO("Zauberspruch \"Adler, Wolf und Hammerhai\" ist nicht implementiert\n");
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

void spell_erstarre() {
	D1_INFO("Zauberspruch \"Erstarre\" ist nicht implementiert\n");
}

void spell_schmelze() {
	D1_INFO("Zauberspruch \"Schmelze\" ist nicht implementiert\n");
}

void spell_sturmgebr() {
	D1_INFO("Zauberspruch \"Sturmgebruell\" ist nicht implementiert\n");
}
