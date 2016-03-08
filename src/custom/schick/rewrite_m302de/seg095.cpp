/*
 *	Rewrite of DSA1 v3.02_de functions of seg095 (NPCs)
 *	Functions rewritten: 10/10 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg095.cpp
*/

#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg026.h"
#include "seg028.h"
#include "seg029.h"
#include "seg050.h"
#include "seg095.h"
#include "seg097.h"
#include "seg120.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

unsigned short npc_meetings(unsigned short type_index)
{

	/* check if an NPC is in the party and if we
		already had an NPC conversation here */
	if (!host_readbs(get_hero(6) + HERO_TYPE) &&
		(type_index != ds_readw(0x346e))) {

		ds_writew(0x346e, type_index);

		/* Nariell */
		if (ds_readb(CURRENT_TOWN) == 0x17 &&
			ds_readw(TYPEINDEX) == 0x30 &&
			!ds_readb(0x3601 + 1)) {

			npc_nariell();
		} else

		/* Harika */
		if (ds_readb(CURRENT_TOWN) == 0x12 &&
			ds_readw(TYPEINDEX) == 0x28 &&
			!ds_readb(0x3601 + 2)) {

			npc_harika();
		} else

		/* Curian */
		if (ds_readb(CURRENT_TOWN) == 0x1d &&
			ds_readw(TYPEINDEX) == 0x3c &&
			!ds_readb(0x3601 + 3)) {

			npc_curian();
		} else

		/* Ardora */
		if (ds_readb(CURRENT_TOWN) == 0x1 &&
			ds_readw(TYPEINDEX) == 0x6 &&
			!ds_readb(0x3601 + 4) &&
			ds_readb(0x3317) != 0) {

			npc_ardora();
		} else

		/* Garsvik */
		if (ds_readb(CURRENT_TOWN) == 0x27 &&
			ds_readw(TYPEINDEX) == 0x4a &&
			!ds_readb(0x3601 + 5)) {

			npc_garsvik();
		} else

		/* Erwo */
		if (ds_readb(CURRENT_TOWN) == 0x30 &&
			ds_readw(TYPEINDEX) == 0x57 &&
			!ds_readb(0x3601 + 6)) {

			npc_erwo();
		} else {
			return 0;
		}

		return 1;
	} else {
		return 0;
	}
}

void npc_farewell(void)
{
	Bit8u *hero_i;
	signed short i;
	signed short tmp;

	/* no NPC there */
	if (host_readb(get_hero(6) + HERO_TYPE) == 0)
		return;

	/* no NPC in that group */
	if (host_readb(get_hero(6) + HERO_GROUP_NO) != ds_readb(CURRENT_GROUP))
		return;

	/* The NPC will be removed after 99 Months ingame time. Weird! */
	if (check_hero(get_hero(6)) == 0 && ds_readws(NPC_MONTHS) < 99)
		return;

	tmp = ds_readw(0x26bf);
	load_buffer_1(ARCHIVE_FILE_NSC_LTX);

	switch (host_readbs(get_hero(6) + HERO_NPC_ID)) {
		/* Nariell */
		case 1: {
			if (ds_readws(NPC_MONTHS) >= 2)
				remove_npc(0x14, 0x1f, 0xe2,
					get_ltx(0xbc4), get_dtp(0x24));
			break;
		}
		/* Harika */
		case 2: {
			if (ds_readws(NPC_MONTHS) >= 2) {
				if (ds_readws(NPC_MONTHS) >= 99 ||
					ds_readb(CURRENT_TOWN) == 1 ||
					ds_readb(CURRENT_TOWN) == 0x12 ||
					ds_readb(CURRENT_TOWN) == 0x27 ||
					ds_readb(CURRENT_TOWN) == 0x11) {

					remove_npc(0x16, 0x1f, 0xe3,
						get_ltx(0xbc8), get_dtp(0x4c));

					hero_i = get_hero(0);
					for (i = 0; i < 6; i++, hero_i += SIZEOF_HERO) {
						if (host_readb(hero_i + HERO_TYPE) &&
							(host_readb(hero_i + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP)) &&
							(!hero_dead(hero_i)))
						{

							/* try to increase sneaking */
							inc_skill_novice(hero_i, 0xd);
						}
					}
				}
			}
			break;
		}
		/* Curian */
		case 3: {
			if (ds_readws(NPC_MONTHS) >= 6)
				remove_npc(0x19, 0x40, 0xe4,
					get_ltx(0xbcc), get_dtp(0x74));
			break;
		}
		/* Ardora */
		case 4: {
			if (ds_readws(NPC_MONTHS) >= 1)
				remove_npc(0x15, 0x1f, 0xe5,
					get_ltx(0xbd0), get_dtp(0xac));
			break;
		}
		/* Garsvik */
		case 5: {
			if (ds_readws(NPC_MONTHS) >= 2)
				remove_npc(0x17, 0x1f, 0xe6,
					get_ltx(0xbd4), get_dtp(0xd4));
			break;
		}
		/* Erwo */
		case 6: {
			if (ds_readws(NPC_MONTHS) >= 2)
				remove_npc(0x18, 0x1f, 0xe7,
					get_ltx(0xbd8), get_dtp(0xfc));
			break;
		}
	}

	if (tmp != -1 && tmp != 0xe1)
		load_buffer_1(tmp);
}

//static
void npc_nariell(void)
{
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(0x14);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_ltx(0xbc4), get_dtp(0x00),
				3,
				get_dtp(0x04), get_dtp(0x08),
				get_dtp(0x0c));
	} while (answer == -1);

	/* process the answer */
	if (answer == 1) {
		GUI_dialogbox((RealPt)ds_readd(DTP2),
			get_ltx(0xbc4), get_dtp(0x10), 0);
	} else {
		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_ltx(0xbc4),
					(answer == 2) ? get_dtp(0x14): get_dtp(0x18),
					2, get_dtp(0x1c),
					get_dtp(0x20));
		} while (answer == -1);

		/* add nariell */
		if (answer == 2)
			add_npc(0xe2);
	}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

//static
void npc_harika(void)
{
	long money;
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(0x16);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_ltx(0xbc8), get_dtp(0x28),
				3,
				get_dtp(0x2c), get_dtp(0x30),
				get_dtp(0x34));
	} while (answer == -1);

	if (answer == 1) {
		GUI_dialogbox((RealPt)ds_readd(DTP2), get_ltx(0xbc8),
			get_dtp(0x38), 0);
	} else if (answer == 2) {
		money = get_party_money();

		answer = (money >= 2000) ? 2 : 1;

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_ltx(0xbc8), get_dtp(0x3c),
					answer,
					get_dtp(0x44), get_dtp(0x48));
		} while (answer == -1);

		/* hire her for 20D */
		if (answer == 2) {
			/* subtract 20D */
			money -= 2000;
			set_party_money(money);

			/* add her to the party */
			add_npc(0xe3);
		}
	} else {
		money = get_party_money();

		answer = (money >= 1500) ? 2 : 1;

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_ltx(0xbc8), get_dtp(0x40),
					answer,
					get_dtp(0x44), get_dtp(0x48));
		} while (answer == -1);

		/* hire her for 15D */
		if (answer == 2) {
			/* subtract 15D */
			money -= 1500;
			set_party_money(money);

			/* add her to the party */
			add_npc(0xe3);
		}
	}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

void npc_curian(void)
{
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(0x19);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_ltx(0xbcc), get_dtp(0x50),
				3,
				get_dtp(0x54), get_dtp(0x58),
				get_dtp(0x5c));
	} while (answer == -1);

	if (answer == 1) {
		GUI_dialogbox((RealPt)ds_readd(DTP2), get_ltx(0xbcc),
			get_dtp(0x60), 0);
	} else {
		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_ltx(0xbcc),
					(answer == 2) ? get_dtp(0x64): get_dtp(0x68),
					2,
					get_dtp(0x6c), get_dtp(0x70));
		} while (answer == -1);

		/* add NPC */
		if (answer == 2)
			add_npc(0xe4);

	}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

//static
void npc_ardora(void)
{
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(0x15);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_ltx(0xbd0), get_dtp(0x78),
				3,
				get_dtp(0x7c), get_dtp(0x80),
				get_dtp(0x84));
	} while (answer == -1);

	if (answer == 1) {
		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_ltx(0xbd0), get_dtp(0x88),
					2,
					get_dtp(0x8c), get_dtp(0x90));
		} while (answer == -1);

		if (answer == 2) {
			GUI_dialogbox((RealPt)ds_readd(DTP2), get_ltx(0xbd0),
				get_dtp(0x94), 0);
		} else {
			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_ltx(0xbd0), get_dtp(0x9c),
						2,
						get_dtp(0xa0), get_dtp(0xa4));
			} while (answer == -1);

			if (answer == 1) {
				add_npc(0xe5);
			} else {
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_ltx(0xbd0),
					get_dtp(0xa8), 0);
			}
		}
	} else {
		if (answer == 2) {
			GUI_dialogbox((RealPt)ds_readd(DTP2), get_ltx(0xbd0),
				get_dtp(0x98), 0);
		}

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_ltx(0xbd0),
					get_dtp(0x9c), 2,
					get_dtp(0xa0), get_dtp(0xa4));
		} while (answer == -1);

		if (answer == 1) {
			add_npc(0xe5);
		} else {
			GUI_dialogbox((RealPt)ds_readd(DTP2), get_ltx(0xbd0),
				get_dtp(0xa8), 0);
		}
	}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

//static
void npc_garsvik(void)
{
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(0x17);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_ltx(0xbd4), get_dtp(0xb0),
				3,
				get_dtp(0xb4), get_dtp(0xb8),
				get_dtp(0xbc));
	} while (answer == -1);

	if (answer == 1) {
		GUI_dialogbox((RealPt)ds_readd(DTP2), get_ltx(0xbd4),
			get_dtp(0xc0), 0);
	} else {
		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_ltx(0xbd4),
					(answer == 2) ? get_dtp(0xc4): get_dtp(0xc8),
					2,
					get_dtp(0xcc), get_dtp(0xd0));
		} while (answer == -1);

		/* add NPC */
		if (answer == 2)
			add_npc(0xe6);

	}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

//static
void npc_erwo(void)
{
	signed short answer;

	/* load NSC.LTX */
	load_buffer_1(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(0x18);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_ltx(0xbd8), get_dtp(0xd8),
				3,
				get_dtp(0xdc), get_dtp(0xe0),
				get_dtp(0xe4));
	} while (answer == -1);

	if (answer == 1) {
		GUI_dialogbox((RealPt)ds_readd(DTP2), get_ltx(0xbd8),
			get_dtp(0xe8), 0);
	} else {
		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_ltx(0xbd8),
					(answer == 2) ? get_dtp(0xec): get_dtp(0xf0),
					2,
					get_dtp(0xf4), get_dtp(0xf8));
		} while (answer == -1);

		/* add NPC */
		if (answer == 2)
			add_npc(0xe7);

		}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

void remove_npc(signed short head_index, signed char days,
		signed short index, Bit8u* name, Bit8u *text)
{

	if (ds_readb(0x4475) != 0)
		refresh_colors();

	/* reset NPCs groups position */
	/* TODO: this is bogus, since memset() will come */
	host_writeb(get_hero(6) + HERO_GROUP_POS, 0);

	/* save the NPC */
	save_npc(index);


	/* print farewell message if the NPC has and can */
	if (NOT_NULL(text)) {
		if (check_hero(get_hero(6)) != 0) {
			load_in_head(head_index);
			GUI_dialogbox((RealPt)ds_readd(DTP2), name, text, 0);
		}
	}

	/* clear the NPC from memory */
	memset(get_hero(6), 0, SIZEOF_HERO);

	/* dec group counter */
	dec_ds_bs_post(0x2d36 + ds_readbs(CURRENT_GROUP));

	/* dec global hero counter */
	dec_ds_bs_post(0x2d3c);

	ds_writeb(0x46df, 1);

	/* TODO:	check_hero() will now, after memset() return 0,
			so the parameter days is useless */
	if (check_hero(get_hero(6)) != 0)
		ds_writeb(0x3520 + index, days);
	else
		ds_writeb(0x3520 + index, -1);

}


void add_npc(signed short index)
{
	load_npc(index);

	/* overwrite the picture of the NPC with one from IN_HEAD.NVF */
	memcpy(get_hero(6) + HERO_PORTRAIT, Real2Host(ds_readd(DTP2)), 0x400);

	/* increment heros in that group */
	inc_ds_bs_post(0x2d36 + ds_readbs(CURRENT_GROUP));

	/* increment heros */
	inc_ds_bs_post(0x2d3c);

	/* reset the months the NPC is in the group */
	ds_writew(NPC_MONTHS, 0);

	/* set a number to deciede between the NPCs (1-6) */
	host_writeb(get_hero(6) + HERO_NPC_ID, index - 0xe1);

	/* set the group the NPC contains in */
	host_writeb(get_hero(6) + HERO_GROUP_NO, ds_readb(CURRENT_GROUP));

	draw_status_line();
}

#if !defined(__BORLANDC__)
}
#endif
