/*
 *	Rewrite of DSA1 v3.02_de functions of seg100 (spells 2/3)
 *	Spells: Clairvoyance / Illusion / Combat / Communication
 *	Functions rewritten 5/20
 *
*/

#include "schick.h"

#include "string.h"

#include "v302de.h"

#include "seg002.h"
#include "seg096.h"
#include "seg097.h"

namespace M302de {

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
