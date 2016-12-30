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

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256  * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	if ((target_pos == 0x407 || target_pos == 0x409 || target_pos == 0x608) &&
		target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x04));

		ds_writew(X_TARGET, ds_writew(Y_TARGET, 1));
		DNG_inc_level();

	} else if (target_pos == 0x304 && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 0)
	{
		GUI_input(get_dtp(0x08), 15);

		if (!strcmp((char*)Real2Host(ds_readfp(TEXT_INPUT_BUF)),
				(char*)p_datseg + DNG03_STR_SPINNENNETZ))
		{
			GUI_output(get_dtp(0x10));

			ds_writew(X_TARGET, 9);
			ds_writew(Y_TARGET, 8);
			DNG_inc_level();

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) >= 7 &&
					host_readws(hero + HERO_AE) != 0)
				{
					sub_ae_splash(hero, random_schick(6));

				} else if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE)
				{
					sub_hero_le(hero, random_schick(6));
				}
			}
		}

	} else if (target_pos == 0xb06 && target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x10));

		ds_writew(X_TARGET, 1);
		ds_writew(Y_TARGET, 8);
		ds_writeb(DIRECTION, 3);

		DNG_update_pos();

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) >= 7 &&
				host_readws(hero + HERO_AE) != 0)
			{
				sub_ae_splash(hero, random_schick(6));

			} else if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE)
			{
				sub_hero_le(hero, random_schick(6));
			}
		}

	} else if ((target_pos == 0x60a || target_pos == 0x709) &&
			(target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)))
	{
		if ((target_pos == 0x60a && ds_readbs(DIRECTION) == 1) ||
			(target_pos == 0x709 && ds_readbs(DIRECTION) == 2))
		{
			DNG_update_pos();
			GUI_output(get_dtp(0x0c));

			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}

	} else if ((target_pos == 0xa09 || target_pos == 0x90e) &&
			ds_readb(DNG03_LEVER_TRAP) != 0 &&
			target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x14));

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, 10, 2) <= 0)
			{
				sprintf((char*)Real2Host(ds_readfp(DTP2)),
					(char*)get_dtp(0x18),
					(char*)hero + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

				GUI_output(Real2Host(ds_readfp(DTP2)));

				sub_hero_le(hero, dice_roll(1, 6, 3));
			}
		}

	} else if (target_pos == 0x50e &&
			ds_readbs(DIRECTION) != ds_readbs(0x2d7c) &&
			ds_readbs(DIRECTION) == 2)
	{
		GUI_input(get_dtp(0x1c), 15);

		if (!strcmp((char*)Real2Host(ds_readfp(TEXT_INPUT_BUF)),
				(char*)p_datseg + DNG03_STR_MACTANS))
		{
			GUI_output(get_dtp(0x10));

			ds_writew(X_TARGET, 3);
			ds_writew(Y_TARGET, 4);
			ds_writeb(DIRECTION, 2);
			DNG_update_pos();

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) >= 7 &&
					host_readws(hero + HERO_AE) != 0)
				{
					sub_ae_splash(hero, random_schick(6));

				} else if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE)
				{
					sub_hero_le(hero, random_schick(6));
				}
			}
		} else {
			ds_writeb(DIRECTION, 3);
		}

	} else if (target_pos == 0x90b &&
			ds_readbs(DIRECTION) == 3 &&
			target_pos != ds_readws(0x330e) &&
			ds_readbs(DIRECTION) != ds_readbs(0x2d7c))
	{
		/* LEVER: */
		if (GUI_bool(get_dtp(0x20)))
		{
			xor_ds_bs(DNG03_LEVER_CHEST11, 1);
			GUI_output(get_dtp(0x24));
		}

	} else if (target_pos == 0x130b &&
			ds_readbs(DIRECTION) == 1 &&
			(target_pos != ds_readws(0x330e) ||
			ds_readbs(DIRECTION) != ds_readbs(0x2d7c)))
	{
		if (GUI_bool(get_dtp(0x20)))
		{
			xor_ds_bs(DNG03_LEVER_TRAP, 1);
			GUI_output(get_dtp(0x24));
		}

	} else if ((target_pos == 0x1e0d || target_pos == 0x1d04 || target_pos == 0x1b02) &&
		target_pos != ds_readws(0x330e))
	{
		j = 0;

		if (test_skill(Real2Host(get_first_hero_available_in_group()), 50, 4) <= 0)
		{
			j++;
		}

		if ((hero = Real2Host(get_second_hero_available_in_group())) &&
			host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_skill(hero, 50, 4) <= 0)
		{
			j++;
		}

		hero = Real2Host(get_first_hero_available_in_group());

		if (j != 0 || test_attrib(hero, 4, 2) <= 0)
		{
			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_dtp(0x34),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			GUI_output(Real2Host(ds_readfp(DTP2)));

			sub_hero_le(hero, dice_roll(2, 6, 0));

			timewarp(MINUTES(20));

			if (host_readws(hero + HERO_ITEM_BODY) != 0)
			{
				l3 = ds_readbs(0x0877 + 2 * host_readbs(get_itemsdat(host_readws(hero + HERO_ITEM_BODY)) + 4)) -
 host_readbs(hero + HERO_ITEM_BODY + 7);

				l4 = (l3 > 3 ? 3 : (l3 > 0 ? l3 : 0));

				add_ptr_bs(hero + HERO_ITEM_BODY + 7, (unsigned char)l4);
				sub_ptr_bs(hero + HERO_RS_BONUS1, (unsigned char)l4);
			}
		}

		if ((hero = Real2Host(get_second_hero_available_in_group())) &&
			(j == 2 ||
			(host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_attrib(hero, 4, 2) <= 0)))
		{

			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_dtp(0x34),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			GUI_output(Real2Host(ds_readfp(DTP2)));

			sub_hero_le(hero, dice_roll(2, 6, 0));

			timewarp(MINUTES(20));

			if (host_readws(hero + HERO_ITEM_BODY) != 0)
			{
				l3 = ds_readbs(0x0877 + 2 * host_readbs(get_itemsdat(host_readws(hero + HERO_ITEM_BODY)) + 4)) -
					 host_readbs(hero + HERO_ITEM_BODY + 7);

				l4 = (l3 > 3 ? 3 : (l3 > 0 ? l3 : 0));

				add_ptr_bs(hero + HERO_ITEM_BODY + 7, (unsigned char)l4);
				sub_ptr_bs(hero + HERO_RS_BONUS1, (unsigned char)l4);
			}
		}

	} else if ((target_pos == 0x120a || target_pos == 0x130d) &&
		target_pos != ds_readws(0x330e))
	{
		ds_writews((0xd325 + 0), ds_writews((0xd325 + 6), 0x1208));
		ds_writews((0xd325 + 2), ds_writews((0xd325 + 4), 0x150d));

		if (!ds_readb(DNG03_HIGHPRIEST_KILLED))
		{
			ds_writeb(DNG03_HIGHPRIEST_KILLED, 14);
		}

		do_fight(ds_readb(DNG03_HIGHPRIEST_KILLED) == 14 ? 224 : 225);

	} else if ((target_pos == 0x1204 || target_pos == 0x1502) &&
		target_pos != ds_readws(0x330e))
	{
		ds_writews((0xd325 + 0), ds_writews((0xd325 + 2), 0x1702));
		ds_writews((0xd325 + 4), ds_writews((0xd325 + 6), 0x1206));

		if (!ds_readb(DNG03_HIGHPRIEST_KILLED))
		{
			ds_writeb(DNG03_HIGHPRIEST_KILLED, 16);
		}

		do_fight(ds_readb(DNG03_HIGHPRIEST_KILLED) == 16 ? 222 : 223);

	} else if (target_pos == 0x110c &&
			target_pos != ds_readws(0x330e) &&
			!ds_readb(DNG03_SANCTUM_SMASHED))
	{

		/* check if a hero in this group has crystals */
		i = get_first_hero_with_item(225) != -1 ? 0 : 1;

		do {
			j = GUI_radio(get_dtp(0x38), 2,
					get_dtp(0x3c),
					!i ? get_dtp(0x40) : get_dtp(0x74));
		} while (j == -1);

		if (j == 1)
		{
			GUI_output(get_dtp(0x48));
			ds_writeb(DNG03_SANCTUM_SMASHED, 1);

		} else if (j == 2 && !i)
		{
			GUI_output(get_dtp(0x44));

			ds_writews((0xd325 + 0),
				ds_writews((0xd325 + 2),
				ds_writews((0xd325 + 4),
				ds_writews((0xd325 + 6), 0x150d))));

			/* drop all crystals from the heros of that group */
			i = get_first_hero_with_item(225);

			do {
				hero = get_hero(i);

				drop_item(hero, get_item_pos(hero, 225), 1);

				i = get_first_hero_with_item(225);

			} while (i != -1);

			do_fight(252);
		}

	} else if (target_pos == 0x1101 &&
			target_pos != ds_readws(0x330e) &&
			!ds_readb(DNG03_SPIDEREGGS_BURNED))
	{
		do {
			j = GUI_radio(get_dtp(0x4c), 2,
					get_dtp(0x50),
					get_dtp(0x54));
		} while (j == -1);

		if (j == 1)
		{
			GUI_output(get_dtp(0x58));

			add_hero_ap_all(50);

			ds_writeb(DNG03_SPIDEREGGS_BURNED, 1);

			/* activate DEATHTRAP #2 with 15 steps */
			ds_writew(DEATHTRAP_STEPS, 15);
			ds_writew(DEATHTRAP, 2);
		}

	} else if (target_pos == 0x1a04 && target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x60));

	} else if (target_pos == 0x1a08 && target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x10));
		ds_writew(X_TARGET, 5);
		ds_writew(Y_TARGET, 14);
		DNG_dec_level();
		ds_writeb(DIRECTION, 0);

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) >= 7 &&
				host_readws(hero + HERO_AE) != 0)
			{
				sub_ae_splash(hero, random_schick(6));

			} else if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE)
			{
				sub_hero_le(hero, random_schick(6));
			}
		}

	} else if ((target_pos == 0x1201 && target_pos != ds_readws(0x330e) && GUI_bool(get_dtp(0x5c))) ||
			target_pos == 0x0008)
	{
		leave_dungeon();
		ds_writebs(CURRENT_TOWN, (signed char)ds_readws(0x4338));
		ds_writews(X_TARGET, ds_readws(0x433a));
		ds_writews(Y_TARGET, ds_readws(0x433c));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readws(0x433e) + 2) & 3);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x78),
			(char*)get_ltx(4 * (ds_readws(0x434a) + 0xeb)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(3));

		ds_writeb(0x4475, 3);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(0x330e, target_pos);

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
		answer = GUI_radio(get_dtp(0x28), 2,
					get_dtp(0x2c),
					get_dtp(0x30));
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

	/* count the crystals in the inventory of the leader */
	for (i = 7; i < 23; i++)
	{
		if (host_readws(hero + HERO_ITEM_HEAD + SIZEOF_KS_ITEM * i) == 225)
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
	GUI_output(get_ltx(0x828));
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
		GUI_output(get_dtp(0x64));

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

				if (test_attrib(hero, 4, mod) <= 0)
				{
					counter++;

					sprintf((char*)Real2Host(ds_readfp(DTP2)),
						(char*)get_dtp(0x6c),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readfp(DTP2)));

					sub_hero_le(hero, dice_roll(2, 6, 4));
				}
			}
		}

		if (counter == l_si)
		{
			/* end of game */
			GUI_output(get_dtp(0x68));

			ds_writew(0xc3c1, 1);
		} else {
			timewarp(HOURS(1));

			GUI_output(get_dtp(0x70));
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
