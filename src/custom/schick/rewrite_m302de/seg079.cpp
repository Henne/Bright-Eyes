/**
 *	Rewrite of DSA1 v3.02_de functions of seg079 (dungeon: cave of the spiders)
 *	Functions rewritten: 15/15 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg079.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg032.h"
#include "seg075.h"
#include "seg079.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG03_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short j;
	signed short tw_bak;
	Bit8u *hero;
	signed short l3;
	signed short l4;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	target_pos = DNG_POS(ds_readbs(DUNGEON_LEVEL), ds_readws(X_TARGET), ds_readws(Y_TARGET));

	if ((target_pos == DNG_POS(0,4,7) || target_pos == DNG_POS(0,4,9) || target_pos == DNG_POS(0,6,8)) &&
		target_pos != ds_readws(DNG_HANDLED_POS))
	{
		GUI_output(get_tx(1));

		ds_writew(X_TARGET, ds_writew(Y_TARGET, 1));
		DNG_inc_level();

	} else if (target_pos == DNG_POS(0,3,4) && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readbs(DIRECTION) == NORTH)
	{
		GUI_input(get_tx(2), 15);

		if (!strcmp((char*)Real2Host(ds_readfp(TEXT_INPUT_BUF)),
				(char*)p_datseg + DNG03_STR_SPINNENNETZ))
		{
			GUI_output(get_tx(4));

			ds_writew(X_TARGET, 9);
			ds_writew(Y_TARGET, 8);
			DNG_inc_level();

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) >= HERO_TYPE_WITCH &&
					host_readws(hero + HERO_AE) != 0)
				{
					sub_ae_splash(hero, random_schick(6));

				} else if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE)
				{
					sub_hero_le(hero, random_schick(6));
				}
			}
		}

	} else if (target_pos == DNG_POS(0,11,6) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		GUI_output(get_tx(4));

		ds_writew(X_TARGET, 1);
		ds_writew(Y_TARGET, 8);
		ds_writeb(DIRECTION, WEST);

		DNG_update_pos();

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) >= HERO_TYPE_WITCH &&
				host_readws(hero + HERO_AE) != 0)
			{
				sub_ae_splash(hero, random_schick(6));

			} else if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE)
			{
				sub_hero_le(hero, random_schick(6));
			}
		}

	} else if ((target_pos == DNG_POS(0,6,10) || target_pos == DNG_POS(0,7,9)) &&
			(target_pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)))
	{
		if ((target_pos == DNG_POS(0,6,10) && ds_readbs(DIRECTION) == EAST) ||
			(target_pos == DNG_POS(0,7,9) && ds_readbs(DIRECTION) == SOUTH))
		{
			DNG_update_pos();
			GUI_output(get_tx(3));

			ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));
		}

	} else if ((target_pos == DNG_POS(0,10,9) || target_pos == DNG_POS(0,9,14)) &&
			ds_readb(DNG03_LEVER_TRAP) != 0 &&
			target_pos != ds_readws(DNG_HANDLED_POS))
	{
		GUI_output(get_tx(5));

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, TA_KLETTERN, 2) <= 0)
			{
				sprintf((char*)Real2Host(ds_readfp(DTP2)),
					(char*)get_tx(6),
					(char*)hero + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

				GUI_output(Real2Host(ds_readfp(DTP2)));

				sub_hero_le(hero, dice_roll(1, 6, 3));
			}
		}

	} else if (target_pos == DNG_POS(0,5,14) &&
			ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK) &&
			ds_readbs(DIRECTION) == SOUTH)
	{
		GUI_input(get_tx(7), 15);

		if (!strcmp((char*)Real2Host(ds_readfp(TEXT_INPUT_BUF)),
				(char*)p_datseg + DNG03_STR_MACTANS))
		{
			GUI_output(get_tx(4));

			ds_writew(X_TARGET, 3);
			ds_writew(Y_TARGET, 4);
			ds_writeb(DIRECTION, SOUTH);
			DNG_update_pos();

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) >= HERO_TYPE_WITCH &&
					host_readws(hero + HERO_AE) != 0)
				{
					sub_ae_splash(hero, random_schick(6));

				} else if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE)
				{
					sub_hero_le(hero, random_schick(6));
				}
			}
		} else {
			ds_writeb(DIRECTION, WEST);
		}

	} else if (target_pos == DNG_POS(0,9,11) &&
			ds_readbs(DIRECTION) == WEST &&
			target_pos != ds_readws(DNG_HANDLED_POS) &&
			ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK))
	{
		/* LEVER: */
		if (GUI_bool(get_tx(8)))
		{
			xor_ds_bs(DNG03_LEVER_CHEST11, 1);
			GUI_output(get_tx(9));
		}

	} else if (target_pos == DNG_POS(1,3,11) &&
			ds_readbs(DIRECTION) == EAST &&
			(target_pos != ds_readws(DNG_HANDLED_POS) ||
			ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)))
	{
		if (GUI_bool(get_tx(8)))
		{
			xor_ds_bs(DNG03_LEVER_TRAP, 1);
			GUI_output(get_tx(9));
		}

	} else if ((target_pos == DNG_POS(1,14,13) || target_pos == DNG_POS(1,13,4) || target_pos == DNG_POS(1,11,2)) &&
		target_pos != ds_readws(DNG_HANDLED_POS))
	{
		j = 0;

		if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_GEFAHRENSINN, 4) <= 0)
		{
			j++;
		}

		if ((hero = Real2Host(get_second_hero_available_in_group())) &&
			host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_skill(hero, TA_GEFAHRENSINN, 4) <= 0)
		{
			j++;
		}

		hero = Real2Host(get_first_hero_available_in_group());

		if (j != 0 || test_attrib(hero, ATTRIB_GE, 2) <= 0)
		{
			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_tx(13),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			GUI_output(Real2Host(ds_readfp(DTP2)));

			sub_hero_le(hero, dice_roll(2, 6, 0));

			timewarp(MINUTES(20));

			if (host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY + INVENTORY_ITEM_ID) != ITEM_NONE)
			{
				/* RS of the equipped body armour gets degraded by 3, but not below 0 */
				l3 = ds_readbs(ARMORS_TABLE + 2 * host_readbs(get_itemsdat(host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY)) + 4))
				    - host_readbs(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY + INVENTORY_RS_LOST);

				l4 = (l3 > 3 ? 3 : (l3 > 0 ? l3 : 0));

				add_ptr_bs(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY + INVENTORY_RS_LOST, (unsigned char)l4);
				sub_ptr_bs(hero + HERO_RS_BONUS1, (unsigned char)l4);
			}
		}

		if ((hero = Real2Host(get_second_hero_available_in_group())) &&
			(j == 2 ||
			(host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_attrib(hero, ATTRIB_GE, 2) <= 0)))
		{

			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_tx(13),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			GUI_output(Real2Host(ds_readfp(DTP2)));

			sub_hero_le(hero, dice_roll(2, 6, 0));

			timewarp(MINUTES(20));

			if (host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY + INVENTORY_ITEM_ID) != ITEM_NONE)
			{
				/* RS of the equipped body armour gets degraded by 3, but not below 0 */
				l3 = ds_readbs(ARMORS_TABLE + 2 * host_readbs(get_itemsdat(host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY)) + 4))
				    - host_readbs(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY + INVENTORY_RS_LOST);

				l4 = (l3 > 3 ? 3 : (l3 > 0 ? l3 : 0));

				add_ptr_bs(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY + INVENTORY_RS_LOST, (unsigned char)l4);
				sub_ptr_bs(hero + HERO_RS_BONUS1, (unsigned char)l4);
			}
		}

	} else if ((target_pos == DNG_POS(1,2,10) || target_pos == DNG_POS(1,3,13)) &&
		target_pos != ds_readws(DNG_HANDLED_POS))
	{
		ds_writews((FIG_FLEE_POSITION + 0), ds_writews((FIG_FLEE_POSITION + 6), DNG_POS_DIR(1,2,8,NORTH)));
		ds_writews((FIG_FLEE_POSITION + 2), ds_writews((FIG_FLEE_POSITION + 4), DNG_POS_DIR(1,5,13,NORTH)));

		if (!ds_readb(DNG03_HIGHPRIEST_KILLED))
		{
			ds_writeb(DNG03_HIGHPRIEST_KILLED, 14);
		}

		do_fight(ds_readb(DNG03_HIGHPRIEST_KILLED) == 14 ? 224 : 225);

	} else if ((target_pos == DNG_POS(1,2,4) || target_pos == DNG_POS(1,5,2)) &&
		target_pos != ds_readws(DNG_HANDLED_POS))
	{
		ds_writews((FIG_FLEE_POSITION + 0), ds_writews((FIG_FLEE_POSITION + 2), DNG_POS_DIR(1,7,2,NORTH)));
		ds_writews((FIG_FLEE_POSITION + 4), ds_writews((FIG_FLEE_POSITION + 6), DNG_POS_DIR(1,2,6,NORTH)));

		if (!ds_readb(DNG03_HIGHPRIEST_KILLED))
		{
			ds_writeb(DNG03_HIGHPRIEST_KILLED, 16);
		}

		do_fight(ds_readb(DNG03_HIGHPRIEST_KILLED) == 16 ? 222 : 223);

	} else if (target_pos == DNG_POS(1,1,12) &&
			target_pos != ds_readws(DNG_HANDLED_POS) &&
			!ds_readb(DNG03_SANCTUM_SMASHED))
	{

		/* check if a hero in this group has crystals */
		i = get_first_hero_with_item(ITEM_CRYSTAL) != -1 ? 0 : 1;

		do {
			j = GUI_radio(get_tx(14), 2,
					get_tx(15),
					!i ? get_tx(16) : get_tx(29));
		} while (j == -1);

		if (j == 1)
		{
			GUI_output(get_tx(18));
			ds_writeb(DNG03_SANCTUM_SMASHED, 1);

		} else if (j == 2 && !i)
		{
			GUI_output(get_tx(17));

			ds_writews((FIG_FLEE_POSITION + 0),
				ds_writews((FIG_FLEE_POSITION + 2),
				ds_writews((FIG_FLEE_POSITION + 4),
				ds_writews((FIG_FLEE_POSITION + 6), DNG_POS_DIR(1,5,13,NORTH)))));

			/* drop all crystals from the heroes of that group */
			i = get_first_hero_with_item(ITEM_CRYSTAL);

			do {
				hero = get_hero(i);

				drop_item(hero, get_item_pos(hero, ITEM_CRYSTAL), 1);

				i = get_first_hero_with_item(ITEM_CRYSTAL);

			} while (i != -1);

			do_fight(FIGHTS_F051_14C);
		}

	} else if (target_pos == DNG_POS(1,1,1) &&
			target_pos != ds_readws(DNG_HANDLED_POS) &&
			!ds_readb(DNG03_SPIDEREGGS_BURNED))
	{
		do {
			j = GUI_radio(get_tx(19), 2,
					get_tx(20),
					get_tx(21));
		} while (j == -1);

		if (j == 1)
		{
			GUI_output(get_tx(22));

			add_hero_ap_all(50);

			ds_writeb(DNG03_SPIDEREGGS_BURNED, 1);

			/* activate DEATHTRAP #2 with 15 steps */
			ds_writew(DEATHTRAP_STEPS, 15);
			ds_writew(DEATHTRAP, 2);
		}

	} else if (target_pos == DNG_POS(1,10,4) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		GUI_output(get_tx(24));

	} else if (target_pos == DNG_POS(1,10,8) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		GUI_output(get_tx(4));
		ds_writew(X_TARGET, 5);
		ds_writew(Y_TARGET, 14);
		DNG_dec_level();
		ds_writeb(DIRECTION, NORTH);

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) >= HERO_TYPE_WITCH &&
				host_readws(hero + HERO_AE) != 0)
			{
				sub_ae_splash(hero, random_schick(6));

			} else if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE)
			{
				sub_hero_le(hero, random_schick(6));
			}
		}

	} else if ((target_pos == DNG_POS(1,2,1) && target_pos != ds_readws(DNG_HANDLED_POS) && GUI_bool(get_tx(23))) ||
			target_pos == DNG_POS(0,0,8))
	{
		leave_dungeon();
		ds_writebs(CURRENT_TOWN, (signed char)ds_readws(TRV_DEST_REACHED));
		ds_writews(X_TARGET, ds_readws(ARRIVAL_X_TARGET));
		ds_writews(Y_TARGET, ds_readws(ARRIVAL_Y_TARGET));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readws(ARRIVAL_DIRECTION) + 2) & 3);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(30),
			(char*)get_ttx(ds_readws(TRV_DESTINATION) + 0xeb));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(3));

		ds_writeb(FADING_STATE, 3);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(DNG_HANDLED_POS, target_pos);

	return 0;
}

void DNG03_chest00_func3(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG03_CHEST00_CONTENT));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG03_chest01_func3(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG03_CHEST01_CONTENT));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG03_chest02_func3(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG03_CHEST02_CONTENT));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG03_chest03_func3(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG03_CHEST03_CONTENT));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG03_chest04_func3(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG03_CHEST04_CONTENT));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG03_chest05_func3(RealPt)
{
	signed short answer;
	do {
		answer = GUI_radio(get_tx(10), 2,
					get_tx(11),
					get_tx(12));
	} while (answer == -1);

	if (answer == 1)
	{
		ds_writew(X_TARGET, 10);
		ds_writew(Y_TARGET, 12);
		DNG_inc_level();
	}
}

void DNG03_chest06_func3(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG03_CHEST06_CONTENT));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG03_chest07_func3(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG03_CHEST07_CONTENT));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG03_chest08_func3(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG03_CHEST08_CONTENT));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG03_chest09_func3(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG03_CHEST09_CONTENT));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG03_chest12_func3(RealPt chest)
{
	RealPt ptr_bak;
	signed char crystals;
	signed char i;
	Bit8u *hero;

	crystals = 0;

	hero = Real2Host(get_first_hero_available_in_group());

	/* count the crystals in the knapsack of the leader */
	for (i = HERO_INVENTORY_SLOT_KNAPSACK_1; i < NR_HERO_INVENTORY_SLOTS; i++)
	{
		if (host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * i) == ITEM_CRYSTAL)
		{
			crystals++;
		}
	}

	/* this chest can only be opened with a leader with 4 crystals */
	if (crystals == 4)
	{
		ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
		host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG03_CHEST12_CONTENT));
		loot_simple_chest(Real2Host(chest));
		host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);

	} else {
		DNG03_chest12_func2();
	}
}

void DNG03_chest10_func3(RealPt)
{
	GUI_output(get_ttx(522));
}

void DNG03_chest11_func3(RealPt)
{
	signed short l_si;
	signed short l_di;
	signed short counter;
	signed short mod;
	Bit8u *hero;

	if (!ds_readb(DNG03_LEVER_CHEST11))
	{
		GUI_output(get_tx(25));

	} else {
		hero = get_hero(0);
		for (l_di = counter = l_si = 0; l_di <= 6; l_di++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero))
			{
				l_si++;

				mod = l_si < 2 ? 6 : (l_si < 4 ? 2 : -2);

				if (test_attrib(hero, ATTRIB_GE, mod) <= 0)
				{
					counter++;

					sprintf((char*)Real2Host(ds_readfp(DTP2)),
						(char*)get_tx(27),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readfp(DTP2)));

					sub_hero_le(hero, dice_roll(2, 6, 4));
				}
			}
		}

		if (counter == l_si)
		{
			/* end of game */
			GUI_output(get_tx(26));

			ds_writew(GAME_STATE, GAME_STATE_DEAD);
		} else {
			timewarp(HOURS(1));

			GUI_output(get_tx(28));
		}
	}
}

void DNG03_chest12_func2(void)
{
	if (ds_readb(DNG03_CHEST12_LOADS) != 0)
	{
		dec_ds_bs_post(DNG03_CHEST12_LOADS);

		chest_poisoned2();
	}
}

#if !defined(__BORLANDC__)
}
#endif
