/*
	Rewrite of DSA1 v3.02_de functions of seg095 (NPCs)
	Functions rewritten: 2/10
*/

#include <string.h>

#include "schick.h"
#include "v302de.h"

#include "seg026.h"
#include "seg028.h"
#include "seg029.h"
#include "seg095.h"
#include "seg097.h"

namespace M302de {

//static
void npc_nariell()
{
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(0xe1);

	/* load head */
	load_in_head(0x14);

	/* show dialog window */
	do {
		answer = GUI_dialogbox(ds_readd(0xd2f3),
				get_ltx(0xbc4), get_dtp(0x00),
				3,
				get_dtp(0x04), get_dtp(0x08),
				get_dtp(0x0c));
	} while (answer == -1);

	/* process the answer */
	if (answer == 1) {
		GUI_dialogbox(ds_readd(0xd2f3),
			get_ltx(0xbc4), get_dtp(0x10), 0);
	} else {
		do {
			if (answer == 2)
				answer = GUI_dialogbox(ds_readd(0xd2f3),
						get_ltx(0xbc4), get_dtp(0x14),
						2, get_dtp(0x1c),
						get_dtp(0x20));
			else
				answer = GUI_dialogbox(ds_readd(0xd2f3),
						get_ltx(0xbc4), get_dtp(0x18),
						2, get_dtp(0x1c),
						get_dtp(0x20));
		} while (answer == -1);

		/* add nariell */
		if (answer == 2)
			add_npc(0xe2);
	}

	/* load TAVERN.TLK */
	load_tlk(0x82);

}

void add_npc(signed short index)
{
	load_npc(index);

	memcpy(get_hero(0) + 0x2bf6, Real2Host(ds_readd(0xd2f3)), 0x400);

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
