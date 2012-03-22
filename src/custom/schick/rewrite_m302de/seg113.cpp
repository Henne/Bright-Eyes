/*
	Rewrite of DSA1 v3.02_de functions of seg113 (travel events)
	Functions rewritten: 1/22
*/

#include "schick.h"

#include "schick_common.h"
#include "v302de.h"

#include "seg004.h"
#include "seg026.h"
#include "seg028.h"
#include "seg029.h"
#include "seg097.h"

namespace M302de {

/* 0x900 */

void hero_disappear(Bit8u *hero, unsigned short pos, signed char type)
{

	/* decrement the number of heroes */
	ds_writeb(0x2d3c, ds_readb(0x2d3c) - 1);

	/* load a new savegame if no hero is present */
	if (ds_readb(0x2d3c) == 0)
		ds_writew(0xc3c1, 1);

	/* decrement group counter */
	ds_writeb(0x2d36 + ds_readb(0x2d35),
		ds_readb(0x2d36 + ds_readb(0x2d35)) - 1);

	/* write type to character sheet */
	host_writeb(hero + 0x88, type);

	/* reset position in group */
	host_writeb(hero + 0x8a, 0);

	if (pos == 6) {
		/* NPC */
		save_npc(0xe2 + host_readb(hero + 0x89));

		/* reset NPC timer */
		ds_writeb(0x3602 + host_readb(hero + 0x89), 0xff);
	} else {
		/* Regular Hero */
		write_chr_temp(pos);
	}

	/* set typus to 0 */
	host_writeb(hero + 0x21, 0);

	if (type != -2) {
		draw_main_screen();
		init_ani(2);
		ds_writew(0x2846, 1);
	}

	/* set flag to check all heros */
	ds_writeb(CHECK_PARTY, 1);
}

}
