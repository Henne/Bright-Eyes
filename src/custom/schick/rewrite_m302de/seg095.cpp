/*
	Rewrite of DSA1 v3.02_de functions of seg095 (NPCs)
	Functions rewritten: 1/10
*/

#include <string.h>

#include "schick.h"
#include "v302de.h"

#include "seg028.h"
#include "seg029.h"

namespace M302de {

void add_npc(signed short index)
{
	load_npc(index);

	/* we could copy is from the hero array, too */
#if 0
	memcpy(get_hero(0) + 0x2bf6, Real2Host(ds_readd(0xd2f3)), 0x400);
#endif

	/* increment heros in that group */
	ds_writeb(0x2d36 + ds_readb(0x2d35),
		ds_readb(0x2d36 + ds_readb(0x2d35)) + 1);

	/* increment heros */
	ds_writew(0x2d3c, ds_readw(0x2d3c) + 1);

	ds_writew(0x3470, 0);

	host_writeb(get_hero(0) + 0x29a5, index + 31);
	host_writeb(get_hero(0) + 0x29a3, ds_readb(0x2d35));

	draw_status_line();
}

}
