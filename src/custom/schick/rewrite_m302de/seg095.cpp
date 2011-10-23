/*
	Rewrite of DSA1 v3.02_de functions of seg095 (NPCs)
	Functions rewritten: 7/10
*/

#include <string.h>

#include "schick.h"
#include "v302de.h"

#include "seg002.h"
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

//static
void npc_harika()
{
	unsigned int money;
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(0xe1);

	/* load head */
	load_in_head(0x16);

	/* show dialog window */
	do {
		answer = GUI_dialogbox(ds_readd(0xd2f3),
				get_ltx(0xbc8), get_dtp(0x28),
				3,
				get_dtp(0x2c), get_dtp(0x30),
				get_dtp(0x34));
	} while (answer == -1);

	if (answer == 1) {
		GUI_dialogbox(ds_readd(0xd2f3), get_ltx(0xbc8),
			get_dtp(0x38), 0);
	} else if (answer == 2) {
		money = get_party_money();

		if (money >= 2000)
			answer = 2;
		else
			answer = 1;

		do {
			answer = GUI_dialogbox(ds_readd(0xd2f3),
					get_ltx(0xbc8), get_dtp(0x3c),
					answer,
					get_dtp(0x44), get_dtp(0x48));
		} while (answer == -1);

		/* hier her for 20D */
		if (answer == 2) {
			/* subtract 20D */
			money -= 2000;
			set_party_money(money);

			/* add her to the party */
			add_npc(0xe3);
		}
	} else {
		money = get_party_money();

		if (money >= 1500)
			answer = 2;
		else
			answer = 1;

		do {
			answer = GUI_dialogbox(ds_readd(0xd2f3),
					get_ltx(0xbc8), get_dtp(0x40),
					answer,
					get_dtp(0x44), get_dtp(0x48));
		} while (answer == -1);

		/* hier her for 15D */
		if (answer == 2) {
			/* subtract 15D */
			money -= 1500;
			set_party_money(money);

			/* add her to the party */
			add_npc(0xe3);
		}
	}


	/* load TAVERN.TLK */
	load_tlk(0x82);
}

//static
void npc_ardora()
{
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(0xe1);

	/* load head */
	load_in_head(0x15);

	/* show dialog window */
	do {
		answer = GUI_dialogbox(ds_readd(0xd2f3),
				get_ltx(0xbd0), get_dtp(0x78),
				3,
				get_dtp(0x7c), get_dtp(0x80),
				get_dtp(0x84));
	} while (answer == -1);

	if (answer == 1) {
		do {
			answer = GUI_dialogbox(ds_readd(0xd2f3),
					get_ltx(0xbd0), get_dtp(0x88),
					2,
					get_dtp(0x8c), get_dtp(0x90));
		} while (answer == -1);

		if (answer == 2) {
			GUI_dialogbox(ds_readd(0xd2f3), get_ltx(0xbd0),
				get_dtp(0x94), 0);
		} else {
			do {
				answer = GUI_dialogbox(ds_readd(0xd2f3),
						get_ltx(0xbd0), get_dtp(0x9c),
						2,
						get_dtp(0xa0), get_dtp(0xa4));
			} while (answer == -1);

			if (answer == 1) {
				add_npc(0xe5);
			} else {
				GUI_dialogbox(ds_readd(0xd2f3), get_ltx(0xbd0),
					get_dtp(0xa8), 0);
			}
		}
	} else {
		if (answer == 2) {
			GUI_dialogbox(ds_readd(0xd2f3), get_ltx(0xbd0),
				get_dtp(0x98), 0);
		}

		do {
			answer = GUI_dialogbox(ds_readd(0xd2f3), get_ltx(0xbd0),
					get_dtp(0x9c), 2,
					get_dtp(0xa0), get_dtp(0xa4));
		} while (answer == -1);

		if (answer == 1) {
			add_npc(0xe5);
		} else {
			GUI_dialogbox(ds_readd(0xd2f3), get_ltx(0xbd0),
				get_dtp(0xa8), 0);
		}
	}

	/* load TAVERN.TLK */
	load_tlk(0x82);
}

void npc_curian()
{
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(0xe1);

	/* load head */
	load_in_head(0x19);

	/* show dialog window */
	do {
		answer = GUI_dialogbox(ds_readd(0xd2f3),
				get_ltx(0xbcc), get_dtp(0x50),
				3,
				get_dtp(0x54), get_dtp(0x58),
				get_dtp(0x5c));
	} while (answer == -1);

	if (answer == 1) {
		GUI_dialogbox(ds_readd(0xd2f3), get_ltx(0xbcc),
			get_dtp(0x60), 0);
	} else {
		do {
			if (answer == 2)
				answer = GUI_dialogbox(ds_readd(0xd2f3),
						get_ltx(0xbcc), get_dtp(0x64),
						2,
						get_dtp(0x6c), get_dtp(0x70));
			else
				answer = GUI_dialogbox(ds_readd(0xd2f3),
						get_ltx(0xbcc), get_dtp(0x68),
						2,
						get_dtp(0x6c), get_dtp(0x70));
		} while (answer == -1);

	/* add NPC */
	if (answer == 2)
		add_npc(0xe4);

	}

	/* load TAVERN.TLK */
	load_tlk(0x82);
}

//static
void npc_garsvik()
{
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(0xe1);

	/* load head */
	load_in_head(0x17);

	/* show dialog window */
	do {
		answer = GUI_dialogbox(ds_readd(0xd2f3),
				get_ltx(0xbd4), get_dtp(0xb0),
				3,
				get_dtp(0xb4), get_dtp(0xb8),
				get_dtp(0xbc));
	} while (answer == -1);

	if (answer == 1) {
		GUI_dialogbox(ds_readd(0xd2f3), get_ltx(0xbd4),
			get_dtp(0xc0), 0);
	} else {
		do {
			if (answer == 2)
				answer = GUI_dialogbox(ds_readd(0xd2f3),
						get_ltx(0xbd4), get_dtp(0xc4),
						2,
						get_dtp(0xcc), get_dtp(0xd0));
			else
				answer = GUI_dialogbox(ds_readd(0xd2f3),
						get_ltx(0xbd4), get_dtp(0xc8),
						2,
						get_dtp(0xcc), get_dtp(0xd0));
		} while (answer == -1);

	/* add NPC */
	if (answer == 2)
		add_npc(0xe6);

	}

	/* load TAVERN.TLK */
	load_tlk(0x82);
}

//static
void npc_erwo()
{
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(0xe1);

	/* load head */
	load_in_head(0x18);

	/* show dialog window */
	do {
		answer = GUI_dialogbox(ds_readd(0xd2f3),
				get_ltx(0xbd8), get_dtp(0xd8),
				3,
				get_dtp(0xdc), get_dtp(0xe0),
				get_dtp(0xe4));
	} while (answer == -1);

	if (answer == 1) {
		GUI_dialogbox(ds_readd(0xd2f3), get_ltx(0xbd8),
			get_dtp(0xe8), 0);
	} else {
		do {
			if (answer == 2)
				answer = GUI_dialogbox(ds_readd(0xd2f3),
						get_ltx(0xbd8), get_dtp(0xec),
						2,
						get_dtp(0xf4), get_dtp(0xf8));
			else
				answer = GUI_dialogbox(ds_readd(0xd2f3),
						get_ltx(0xbd8), get_dtp(0xf0),
						2,
						get_dtp(0xf4), get_dtp(0xf8));
		} while (answer == -1);

	/* add NPC */
	if (answer == 2)
		add_npc(0xe7);

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
