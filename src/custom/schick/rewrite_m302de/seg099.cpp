/*
 *	Rewrite of DSA1 v3.02_de functions of seg099 (spells 1/3)
 *	Spells:		Dispell / Domination / Demonology / Elements /
 *			Movement / Healing / Clairvoyance
 *	Functions rewritten 1/39
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

}
