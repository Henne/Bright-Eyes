/**
 *	Rewrite of DSA1 v3.02_de functions of seg089 (dungeon: ruined castle)
 *	Functions rewritten: 12/12 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg089.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg003.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg030.h"
#include "seg032.h"
#include "seg065.h"
#include "seg075.h"
#include "seg076.h"
#include "seg089.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG15_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short dir;
	signed short tmp;
	signed short tw_bak;
	Bit8u *hero;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	dir = ds_readbs(DIRECTION);

	hero = Real2Host(get_first_hero_available_in_group());

	if ((((target_pos == 0x603 || target_pos == 0x606 || target_pos == 0x609) && dir == 1) ||
		((target_pos == 0xc0a || target_pos == 0x80a) && dir == 0)) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* INFO: a large hall */
		GUI_output(get_tx(0x04));

	} else if (((target_pos == 0x304 && dir == 0) || (target_pos == 0x403 && dir == 3)) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* INFO: entering the tower */
		GUI_output(get_tx(0x08));

	} else if ((target_pos == 0x101 || target_pos == 0x201 || target_pos == 0x301 || target_pos == 0x401 ||
			target_pos == 0x102 || target_pos == 0x202 || target_pos == 0x302 || target_pos == 0x402 ||
			target_pos == 0x103 || target_pos == 0x203 || target_pos == 0x303 || target_pos == 0x403 ||
			target_pos == 0x104 || target_pos == 0x204 || target_pos == 0x304 || target_pos == 0x404) &&
			target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* TRAP: light wounds */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_attrib(hero, ATTRIB_GE, -3) <= 0)
			{
				tmp = random_schick(3);

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)(tmp == 3 ? get_tx(0x0c) : (tmp == 2 ? get_tx(0x10) : get_tx(0x14))),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_hero_le(hero, 1);
			}
		}

	} else if ((target_pos == 0xa01 || target_pos == 0xb01 || target_pos == 0xb02 ||
			target_pos == 0xc02 || target_pos == 0xd02 || target_pos == 0xe03) &&
			target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* debris, with wounds and no hidden ladder */
		DNG15_wounds_and_ladders(get_tx(0x24), 1, 0);

	} else if ((target_pos == 0xb00 || target_pos == 0xc00 || target_pos == 0xd00 ||
			target_pos == 0xf00 || target_pos == 0xf01 || target_pos == 0xe00) &&
			target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* debris, no wounds and no hidden ladder */
		DNG15_wounds_and_ladders(get_tx(0x38), 0, 0);

	} else if (target_pos == 0xe02 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* debris, with wounds and hidden ladder */
		DNG15_wounds_and_ladders(get_tx(0x3c), 1, 1);

	} else if ((target_pos == 0xb01 || target_pos == 0xc01 || target_pos == 0xd01 ||
			target_pos == 0xe01) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* debris, with wounds and no hidden ladder */
		DNG15_wounds_and_ladders(get_tx(0x40), 1, 0);

	} else if (target_pos == 0x901 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 0));

	} else if (target_pos == 0xe04 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 1));

	} else if (target_pos == 0x1e02 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* LADDER: upwards */
		if (GUI_bool(get_tx(0x44)))
		{
			DNG_dec_level();
			ds_writeb(DIRECTION, 3);
			dec_ds_ws_post(X_TARGET);
		}

	} else if (target_pos == 0x1801 &&
			(target_pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION_BAK) != dir) &&
			(!ds_readb(DNG15_LEVER_SOUTH) || !ds_readb(DNG15_LEVER_NORTH)))
	{
		tmp = dir;

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(0x5c),
			(char*)(tmp == 0 ? get_tx(0x58) :
				(tmp == 2 ? get_tx(0x54) :
				(tmp == 3 ? get_tx(0x50) : get_tx(0x4c)))));

		for (i = tmp = 0; i < 6; i++)
		{
			if (ds_readw(GROUPS_X_TARGET + 2 * i) == 8 &&
				ds_readw(GROUPS_Y_TARGET + 2 * i) == 5 &&
				ds_readbs(CURRENT_GROUP) != i)
			{
				tmp = 1;
			}
		}

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)(tmp == 0 ? get_tx(0x60) : get_tx(0x64)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		ds_writeb(DIRECTION_BAK, (signed char)dir);

	} else if (target_pos == 0x1805 &&
			(target_pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION_BAK) != dir) &&
			(!ds_readb(DNG15_LEVER_SOUTH) || !ds_readb(DNG15_LEVER_NORTH)))
	{
		tmp = dir;

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(0x5c),
			(char*)(tmp == 0 ? get_tx(0x54) :
				(tmp == 2 ? get_tx(0x58) :
				(tmp == 3 ? get_tx(0x4c) : get_tx(0x50)))));

		for (i = tmp = 0; i < 6; i++)
		{
			if (ds_readw(GROUPS_X_TARGET + 2 * i) == 8 &&
				ds_readw(GROUPS_Y_TARGET + 2 * i) == 1 &&
				ds_readbs(CURRENT_GROUP) != i)
			{
				tmp = 1;
			}
		}

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)(tmp == 0 ? get_tx(0x60) : get_tx(0x64)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		ds_writeb(DIRECTION_BAK, (signed char)dir);

	} else if (target_pos == 0x1802 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* check if another group stands on the other side */
		for (i = tmp = 0; i < 6; i++)
		{
			if (ds_readw(GROUPS_X_TARGET + 2 * i) == 8 &&
				ds_readw(GROUPS_Y_TARGET + 2 * i) == 5 &&
				ds_readbs(CURRENT_GROUP) != i)
			{
				tmp = 1;
			}
		}

		if (tmp == 0)
		{
			/* go through the mirror */
			GUI_output(get_tx(0x68));
			ds_writew(Y_TARGET, 5);
			DNG_update_pos();
		} else {
			/* stay here */
			GUI_output(get_tx(0x6c));
			ds_writew(Y_TARGET, ds_readw(Y_TARGET_BAK));
		}

	} else if (target_pos == 0x1804 && target_pos != ds_readws(DNG_HANDLED_POS))
	{

		/* check if another group stands on the other side */
		for (i = tmp = 0; i < 6; i++)
		{
			if (ds_readw(GROUPS_X_TARGET + 2 * i) == 8 &&
				ds_readw(GROUPS_Y_TARGET + 2 * i) == 1 &&
				ds_readbs(CURRENT_GROUP) != i)
			{
				tmp = 1;
			}
		}

		if (tmp == 0)
		{
			/* go through the mirror */
			GUI_output(get_tx(0x68));
			ds_writew(Y_TARGET, 1);
			DNG_update_pos();
		} else {
			/* stay here */
			GUI_output(get_tx(0x6c));
			ds_writew(Y_TARGET, ds_readw(Y_TARGET_BAK));
		}

	} else if (target_pos == 0x110e && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		loot_multi_chest(p_datseg + DNG15_CHEST_EQUIPS, get_tx(0x98));

	} else if (target_pos == 0x110b && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* INFO: wooden beams */
		GUI_output(get_tx(0x9c));

	} else if (target_pos == 0x130a && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 2));

	} else if (target_pos == 0x240a && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG15_TOOK_HOE))
	{
		/* ITEM: a HOE */
		if (GUI_bool(get_tx(0xc8)))
		{
			GUI_output(get_tx(0xcc));

			if (get_item(93, 1, 1) != -1)
			{
				ds_writeb(DNG15_TOOK_HOE, 1);
			}
		}

	} else if ((target_pos == 0x2109 || target_pos == 0x230d) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* INFO: end of the corridor */
		GUI_output(get_tx(0xd0));

	} else if (target_pos == 0x230c && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 3));

	} else if (target_pos == 0x230b && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 4));

	} else if (target_pos == 0x2209 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 5));

	} else if (target_pos == 0x2703 && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb((DNG15_CURSED_MONEY_A + 0)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 0));

	} else if (target_pos == 0x2606 && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb((DNG15_CURSED_MONEY_A + 1)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 1));

	} else if (target_pos == 0x280b && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb((DNG15_CURSED_MONEY_A + 2)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 2));

	} else if (target_pos == 0x2907 && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb((DNG15_CURSED_MONEY_A + 5)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 5));

	} else if (target_pos == 0x2501 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 11));

	} else if (target_pos == 0x2903 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 12));

	} else if (target_pos == 0x2902 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 13));

	} else if (target_pos == 0x2302 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 15));

	} else if (target_pos == 0x2303 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 16));

	} else if (target_pos == 0x2304 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 17));

	} else if (target_pos == 0x310c && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 6));

	} else if (target_pos == 0x310d && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 7));

	} else if (target_pos == 0x310e && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 8));

	} else if (target_pos == 0x320d && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 9));

	} else if (target_pos == 0x320e && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 10));

	} else if (target_pos == TEVENT128_FLAG && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb((DNG15_CURSED_MONEY_A + 3)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 3));

	} else if (target_pos == 0x3b05 && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb((DNG15_CURSED_MONEY_A + 4)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 4));

	} else if (target_pos == 0x3b02 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* FIGHT: */
		if (random_schick(100) < 10)
		{
			ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 2), ds_writew((0xd325 + 6), 0x3932)));
			ds_writew((FIG_FLEE_POSITION + 4), 0x3d25);

			if (!do_fight(FIGHTS_DFIN26))
			{
				ds_writeb(DNG15_UNDEAD_FIGHT, 1);
			}
		}

	} else if (target_pos == 0x3d03 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* FIGHT: */
		if (random_schick(100) < 10)
		{
			ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 2), ds_writew((0xd325 + 4), ds_writew((0xd325 + 6), 0x3d25))));

			if (!do_fight(FIGHTS_DFIN26))
			{
				ds_writeb(DNG15_UNDEAD_FIGHT, 1);
			}
		}

	} else if ((target_pos == 0x2809 || target_pos == 0x2909) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* FIGHT: */
		ds_writew((FIG_FLEE_POSITION + 0), 0x2907);
		ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), 0x2c1b));
		ds_writew((FIG_FLEE_POSITION + 6), 0x2639);

		do_fight(!ds_readb(DNG15_UNDEAD_FIGHT) ? 134 : 133);

	} else if ((target_pos == 0x3d05 || target_pos == 0x3905) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* FIGHT: */
		if (ds_readb(DNG15_UNDEAD_FIGHT) != 2)
		{
			ds_writew((FIG_FLEE_POSITION + 0), 0x03d03);
			ds_writew((FIG_FLEE_POSITION + 2), 0x03e15);
			ds_writew((FIG_FLEE_POSITION + 4), 0x03a2a);
			ds_writew((FIG_FLEE_POSITION + 6), 0x03903);

			if (!do_fight(!ds_readb(DNG15_UNDEAD_FIGHT) ? 137 : 136))
			{
				ds_writeb(DNG15_UNDEAD_FIGHT, 2);
			}
		}

	} else if ((target_pos == 0x3a0a || target_pos == 0x3d0a || target_pos == 0x3c0e) &&
			target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* QUEST: hyggelik */
		if (ds_readb(DNG15_TOOK_CURSED_MONEY) != 0)
		{
			/* you are cursed */
			do_talk(19, 1);

		} else if (!ds_readb(GOT_GRIMRING)) {

			/* fight the zombies */
			ds_writew((FIG_FLEE_POSITION + 0),
				ds_writew((FIG_FLEE_POSITION + 2),
				ds_writew((FIG_FLEE_POSITION + 4),
				ds_writew((FIG_FLEE_POSITION + 6), 0x3a0a))));

			if (!do_fight(FIGHTS_DFIN28))
			{
				/* talk with hyggelik */
				draw_main_screen();
				DNG_update_pos();
				do_talk(19, 0);
				update_mouse_cursor();
				show_hyggelik_ani();

				if (1) { } else { }

				/* group gets GRIMRING */
				do { ; } while (!get_item(181, 1, 1));

				ds_writeb(GOT_GRIMRING, 1);

				add_hero_ap_all(50);

				ds_writeb(DNG_AREA_LOADED, (signed char)ds_writews(AREA_PREPARED, -1));
				ds_writeb(PP20_INDEX, -2);

				draw_main_screen();
				refresh_screen_size();
			}
		}

	} else if (target_pos == 0x0b && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* EXIT: may be blocked if cursed money has been taken */
		if (ds_readb(DNG15_TOOK_CURSED_MONEY) != 0)
		{
			GUI_output(get_tx(0x18));

			ds_writew(X_TARGET, ds_readws(X_TARGET_BAK));
			ds_writew(Y_TARGET, ds_readws(Y_TARGET_BAK));
		} else {
			leave_dungeon();

			ds_writeb(CURRENT_TOWN, ds_readbs(TRV_DEST_REACHED));
			ds_writew(X_TARGET, ds_readws(ARRIVAL_X_TARGET));
			ds_writew(Y_TARGET, ds_readws(ARRIVAL_Y_TARGET));
			ds_writeb(LOCATION, 0);
			ds_writeb(DIRECTION, (ds_readbs(ARRIVAL_DIRECTION) + 2) & 0x03);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(0xdc),
				(char*)get_ttx(4 * (ds_readw(TRV_DESTINATION) + 0xeb)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			timewarp(HOURS(2));

			ds_writeb(FADING_STATE, 3);
		}
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(DNG_HANDLED_POS, target_pos);

	return 0;
}

/**
 * \brief   heros may get small wounds
 *
 *          Each alive hero in the group makes a GE-3 test.
 *          If it fails he looses 1 LE.
 */
void DNG15_small_wounds(void)
{
	signed short i;
	signed short randval;
	Bit8u *hero;

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_attrib(hero, ATTRIB_GE, -3) <= 0)
		{
			randval = random_schick(3);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)(randval == 3 ? get_tx(0x0c) :
						(randval == 2 ? get_tx(0x1c) : get_tx(0x20))),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, 1);
		}
	}
}

/**
 * \brief   group find debris
 *
 *          The group may find a ladder to descent to the next dungeon level.
 *
 * \param   ladder      (0 = no ladder, 1 = ladder)
 */
void DNG15_debris(signed short ladder)
{
	if (GUI_bool(get_tx(0x28)))
	{
		if (ladder == 0)
		{
			GUI_output(get_tx(0x2c));
		} else {
			GUI_output(get_tx(0x30));

			if (GUI_bool(get_tx(0x34)))
			{
				ds_writeb(DIRECTION, 3);
				dec_ds_ws(X_TARGET);
				DNG_inc_level();
			}
		}
	}
}

void DNG15_empty_chest(RealPt chest)
{
	GUI_output(get_ttx(0x828));
}

void DNG15_rotten_clothes_chest(RealPt chest)
{
	GUI_output(get_tx(0x88));
}

void DNG15_smelling_chest(RealPt chest)
{
	Bit8u *hero;
	if (GUI_bool(get_tx(0x8c)))
	{
		hero = get_hero(get_random_hero());

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(0x90),
			(char*)hero + HERO_NAME2);
		GUI_output(Real2Host(ds_readd(DTP2)));

		sub_hero_le(hero, 4);
	}
}

void DNG15_figures_chest(RealPt chest)
{
	signed short tw_bak;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);
	GUI_output(get_tx(0x94));
	ds_writew(TEXTBOX_WIDTH, tw_bak);
}

void DNG15_cursed_money_chest(RealPt chest)
{
	Bit32s p_money;
	signed short tw_bak;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	if (GUI_bool(get_tx(0xd8)))
	{
		/* You are cursed ... */
		ds_writeb(DNG15_TOOK_CURSED_MONEY, 1);

		/* ... but get much money and ... */
		p_money = get_party_money();
		p_money += 18928L;
		set_party_money(p_money);

		/* ... 50 GOLD JEWELRY. */
		get_item(74, 1, 50);
	}
	ds_writew(TEXTBOX_WIDTH, tw_bak);
}

void DNG15_collapsing_ceiling(Bit8u* ptr)
{
	signed short i;
	signed short cnt;
	Bit8u *hero;

	hero = get_hero(0);

	switch (host_readb(ptr))
	{
		case 1:
		{
			/* rotting beams */
			GUI_output(get_tx(0xa0));

			/* count failed GE-3 test */
			for (i = cnt = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero) &&
					test_attrib(hero, ATTRIB_GE, -3) <= 0)
				{
					cnt++;
				}
			}

			if (cnt >= 2)
			{
				/* if more that one hero failed, the ceiling craks */
				inc_ptr_bs(ptr);

				GUI_output(get_tx(0xa4));
			}
			break;
		}
		case 2:
		{
			/* the beams crash imediately */
			GUI_output(get_tx(0xa8));
			inc_ptr_bs(ptr);
			GUI_output(get_tx(0xac));

			/* each hero gets 1W6 damage on a failed GE test */
			for (i = cnt = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero) &&
					test_attrib(hero, ATTRIB_GE, 0) <= 0)
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx(0xb0),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_hero_le(hero, random_schick(6));
				}
			}

			/* way is blocked */
			ds_writew(X_TARGET, ds_readws(X_TARGET_BAK));
			ds_writew(Y_TARGET, ds_readws(Y_TARGET_BAK));
			break;
		}
		case 4:
		{
			if (random_schick(100) >= 65)
			{
				/* the way is already cleared */
				GUI_output(get_tx(0xc0));
			} else {
				dec_ptr_bs(ptr);

				if (GUI_bool(get_tx(0xc4)))
				{
					DNG15_clear_way(ptr);
				}
			}
				break;
		}
	}
}

void DNG15_clear_way(Bit8u* ptr)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);
	i = 0;

	/* With all of the following items SHOVEL, HOE, CROWBAR, FRANCESCA ...*/
	if ((get_first_hero_with_item(73) != -1) &&
		(get_first_hero_with_item(93) != -1) &&
		(get_first_hero_with_item(26) != -1) &&
		(get_first_hero_with_item(16) != -1))
	{
		i = 1;
	}

	/* ... the group saves 30 minutes ingame time. */
	timewarp(!i ? MINUTES(30) : HOURS(1));

	/* With aprobability of 60% the ceiling crashes down. */
	if (random_schick(100) <= 60)
	{
		GUI_output(get_tx(0xb8));

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_attrib(hero, ATTRIB_GE, 0) <= 0)
			{
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(0xb0),
					(char*)hero + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_hero_le(hero, random_schick(6));
			}
		}

		ds_writew(X_TARGET, ds_readws(X_TARGET_BAK));
		ds_writew(Y_TARGET, ds_readws(Y_TARGET_BAK));
	} else {
		inc_ptr_bs(ptr);
		GUI_output(get_tx(0xbc));
	}
}

void DNG15_cursed_money(Bit8u* ptr)
{
	Bit32s p_money;

	if (GUI_bool(get_tx(0xd4)))
	{
		host_writebs(ptr, ds_writeb(DNG15_TOOK_CURSED_MONEY, 1));

		p_money = get_party_money();
		p_money += random_interval(10, 25) * 100;
		set_party_money(p_money);
	}
}

/**
 * \brief   * \param text		pointer to the output text
 *
 * \param   wounds      group may get wounds (0/1)
 * \param   ladder      here is a ladder to the next level
 */
void DNG15_wounds_and_ladders(Bit8u* text, signed short wounds, signed short ladder)
{
	GUI_output(text);

	if (wounds != 0)
	{
		DNG15_small_wounds();
	}

	DNG15_debris(ladder);
}

#if !defined(__BORLANDC__)
}
#endif
