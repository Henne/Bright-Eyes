/*
	Rewrite of DSA1 v3.02_de functions of seg109 (travel events 1 / 10)
	Functions rewritten: 2/30
*/

#include "schick.h"

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg097.h"

namespace M302de {

/* 0x4f2 */
/**
 * enter_hut_question() - question if you want to rest in a hut
 *
 * Returns the answer 0 = no, 1 = yes
 */
signed short enter_hut_question(void)
{
	signed short answer;

	load_ani(8);

	draw_main_screen();

	init_ani(0);

	answer = GUI_bool(get_dtp(0x60));

	set_var_to_zero();

	ds_writew(0x2846, 1);

	return answer;
}

/* The hunter Varnheim <-> Daspota */
void tevent_002(void)
{
	register signed short answer;

	/* load dialog head */
	load_in_head(0x0a);

	/* show dialogbox */
	do {
		answer = GUI_dialogbox(ds_readd(DTP2),
				NULL,
				get_city(0x00),
				3,
				get_city(0x04),
				get_city(0x08),
				get_city(0x0c));

	} while (answer == -1);

	/* talk to the hunter for 15 min */
	if (answer == 1 || answer == 2) {

		if (answer == 1)
			GUI_dialog_na(0, get_city(0x10));
		else
			GUI_dialog_na(0, get_city(0x14));

		timewarp(0x546);
	}
}

}
