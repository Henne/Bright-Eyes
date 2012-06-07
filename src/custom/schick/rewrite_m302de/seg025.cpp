/**
 *	Rewrite of DSA1 v3.02_de functions of seg025 (locations)
 *	Functions rewritten: 2/15
 */

#include <string.h>

#include "schick.h"

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg097.h"

namespace M302de {

/* 0x100a */
/**
 * tumult() - party gets 1W6 LE damage
 */
void tumult(void)
{
	signed short bak;

	bak = ds_readw(0xbffd);
	ds_writew(0xbffd, 7);

	/* print message */
	GUI_output(get_ltx(0xbf0));

	/* each hero in the group looses 1W6 LE */
	sub_group_le(random_schick(6));

	if (ds_readb(CURRENT_TOWN) == 0x27 || ds_readb(CURRENT_TOWN) == 0x12 ||
		ds_readb(CURRENT_TOWN) == 0x1 || ds_readb(CURRENT_TOWN) == 0x11) {

			/* the guards */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0xbf4),
				(char*)get_ltx(0xbf8));
	} else {
			/* a mob */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0xbf4),
				(char*)get_ltx(0xbfc));
	}

	GUI_output(Real2Host(ds_readd(DTP2)));

	ds_writew(0xbffd, bak);
}

/* 0x114a */
void copy_palette(void)
{
	memcpy(Real2Host(ds_readd(0xd303)) + 0xfa00, Real2Host(ds_readd(0xce3b)), 0x60);
	ds_writeb(0x4475, 2);
}

}
