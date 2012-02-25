/*
 *	Rewrite of DSA1 v3.02_de functions of seg099 (spells 1/3)
 *	Spells:		Dispell / Domination / Demonology / Elements /
 *			Movement / Healing / Clairvoyance
 *	Functions rewritten 5/39
 *
*/

#include "schick.h"

#include "string.h"

#include "v302de.h"

#include "seg002.h"
#include "seg096.h"
#include "seg097.h"

namespace M302de {

void spell_destructibo()
{
        D1_INFO("Zauberspruch \"Destructibo\" ist nicht implementiert\n");
	ds_writew(0xac0e, -2);
}

void spell_gardanium()
{
	signed short answer;

	/* prepare a question */
	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)get_dtp(0x8), (char*)(get_spelluser() + 0x10));

	/* ask and get the answer */
	answer = GUI_input(Real2Host(ds_readd(0xd2f3)), 2);

	/* clear the textbuffer */
	host_writeb(Real2Host(ds_readd(0xd2f3)), 0);

	if (answer <= 0) {
		ds_writew(0xac0e, 0);
		return;
	}

	if (host_readw(get_spelluser() + 0x64) >= answer) {
		/* enough AE */

		/* TODO: this adds the AE to a variable no one reads */
		ds_writew(0x333e, ds_readw(0x333e) + answer);
		/* set AE costs */
		ds_writew(0xac0e, answer);
		/* prepare the message */
		strcpy((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(0x1c));
	} else {
		/* not enough AE */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_ltx(0x97c), (char*)get_spelluser() + 0x10);
		/* set AE costs */
		ds_writew(0xac0e, 0);
	}
}

void spell_boeser_blick()
{
	/* set attacked foe */
	ds_writed(0xe5b4,
		RealMake(datseg, host_readb(get_spelluser() + 0x86) * 0x3e + 0xd0df));

	/* this spell does not work on all kind of sleletons */
	if (host_readb(Real2Host(ds_readd(0xe5b4)) + 1) == 0x1c) {
		ds_writew(0xac0e, -2);
	} else {
		/* set "Boeser Blick" Flag */
		host_writeb(Real2Host(ds_readd(0xe5b4)) + 0x32,
			host_readb(Real2Host(ds_readd(0xe5b4)) + 0x32) | 2);

		/* set number of attacks to 2 */
		host_writeb(Real2Host(ds_readd(0xe5b4)) + 0x1b, 2);

		/* prepare message */
		sprintf((char*)Real2Host(ds_readd(0xd2f3)),
			(char*)get_dtp(0x28),
			(char*)Real2Host(GUI_names_grammar(0x8000, host_readb(Real2Host(ds_readd(0xe5b4))), 1)));

	}
}

void spell_grosse_gier()
{
	D1_INFO("Zauberspruch \"Grosse Gier\" ist nicht implementiert\n");
	ds_writew(0xac0e, -2);
}

void spell_grosse_ver()
{
	ds_writed(0xe5b4,
		RealMake(datseg, host_readb(get_spelluser() + 0x86) * 0x3e + 0xd0df));

	/* this spell does not work on all kind of sleletons */
	if (host_readb(Real2Host(ds_readd(0xe5b4)) + 1) == 0x1c) {
		ds_writew(0xac0e, -2);
		return;
	}

	/* cost is 8 AP */
	ds_writew(0xac0e, 8);

	/* Sub -2 from AT */
	host_writeb(Real2Host(ds_readd(0xe5b4)) + 0x1c,
		host_readb(Real2Host(ds_readd(0xe5b4)) + 0x1c) - 2);

	/* Sub -2 from PAT */
	host_writeb(Real2Host(ds_readd(0xe5b4)) + 0x1d,
		host_readb(Real2Host(ds_readd(0xe5b4)) + 0x1d) - 2);

}

}
