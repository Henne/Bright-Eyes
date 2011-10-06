/*
 *	Rewrite of DSA1 v3.02_de functions of seg100 (spells 2/3)
 *	Spells: Clairvoyance / Illusion / Combat / Communication
 *	Functions rewritten 8/20
 *
*/

#include "schick.h"

#include "string.h"

#include "v302de.h"

#include "seg002.h"
#include "seg096.h"
#include "seg097.h"

namespace M302de {

/* Clairvoyance / Hellsicht */

void spell_sensibar()
{
        D1_INFO("Zauberspruch \"Sensibar\" ist nicht implementiert\n");
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
        D1_INFO("Zauberspruch \"Harmlos\" ist nicht implementiert\n");
}

/* Combat / Kampf */
void spell_blitz()
{

	if (host_readb(get_spelluser() + 0x86) < 10) {
		/* cast a hero */

		/* set the spell target */
		ds_writed(0xe5b8,
			ds_readd(0xbd34) + (host_readb(get_spelluser() + 0x86) - 1) * 0x6da);

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
		ds_writed(0xe5b4,
			RealMake(datseg, 0xd0df + host_readb(get_spelluser() + 0x86) * 62));

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
				(char*)Real2Host(GUI_get_ptr(host_readb(get_spelluser() + 0x22), 3)),
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


/* Communication / Verstaendigung */
void spell_hexenblick()
{
        D1_INFO("Zauberspruch \"Hexenblick\" ist nicht implementiert\n");
}

void spell_necropathia()
{
        D1_INFO("Zauberspruch \"Necropathia\" ist nicht implementiert\n");
}

}
