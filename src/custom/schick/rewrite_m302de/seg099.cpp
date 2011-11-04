/*
 *	Rewrite of DSA1 v3.02_de functions of seg099 (spells 1/3)
 *	Spells:		Dispell / Domination / Demonology / Elements /
 *			Movement / Healing / Clairvoyance
 *	Functions rewritten 3/39
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
	ds_writew(0xac02, -2);
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
