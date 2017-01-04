/**
 *	Rewrite of DSA1 v3.02_de functions of seg085 (dungeon: cave4)
 *	Functions rewritten: 7/7 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg085.cpp
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
#include "seg032.h"
#include "seg075.h"
#include "seg076.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG10_handler(void)
{
	signed short target_pos;
	signed short answer;
	signed short result;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;
	Bit32s p_money;

	amap_ptr = p_datseg + DNG_MAP;
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	hero = Real2Host(get_first_hero_available_in_group());

	if ((target_pos == 0x101 || target_pos == 0x302) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* TRAP: a hole in a wall; leader gets 2 LE damage */
		if (GUI_bool(get_dtp(0x04)))
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x08),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, 2);
		}

	} else if (target_pos == 0x502 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* a dead dwarf */
		seg092_06b4(0);

	} else if (target_pos == 0x801 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* another hole in a wall with a lever for a trap */
		if ((ds_readb(DNG10_LEVER_FOUND) != 0) || test_skill(hero, TA_SINNESSCHAERFE, 7) > 0)
		{
			or_ds_bs(DNG10_LEVER_FOUND, 1);

			if (GUI_bool((!(ds_readb(DNG10_LEVER_FOUND) & 2) ? get_dtp(0x18) : get_dtp(0xa4))))
			{
				if (GUI_bool(get_dtp(0x1c)))
				{
					or_ds_bs(DNG10_LEVER_FOUND, 2);
					xor_ds_bs(DNG10_LEVER_STATE, 1);

					GUI_output(get_dtp(0x20));
				}
			}
		}

	} else if ((target_pos == 0x804 && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG10_LEVER_STATE)) ||
			(target_pos == 0x704 && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readb(DNG10_LEVER_STATE) != 0))
	{
		/* TRAP: terrible pain; radom hero gets 3W6+4 LE damage */
		hero = get_hero(get_random_hero());

		answer = dice_roll(3, 6, 4);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x24),
			(char*)hero + HERO_NAME2);

		/* check if the hero will survive */
		if (host_readws(hero + HERO_LE) > answer)
		{
			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)get_dtp(0x28),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat((char*)Real2Host(ds_readd(DTP2)),
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
		}

		GUI_output(Real2Host(ds_readd(DTP2)));

		sub_hero_le(hero, answer);

	} else if (target_pos == 0x30a &&
			(target_pos != ds_readws(DNG_HANDLED_POS) || ds_readb(DIRECTION) != ds_readb(DIRECTION_BAK)) &&
			ds_readbs(DIRECTION) == 3)
	{
		/* TRAP: a loose stone in a wall */
		if (ds_readb(DNG10_HOLE_STATE) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 5) > 0)
		{
			/* Original-Bug: ??? */
			ds_writeb(DNG10_HOLE_STATE, 1);

			if (ds_readb(DNG10_HOLE_STATE) == 2 || GUI_bool(get_dtp(0x18)))
			{
				ds_writeb(DNG10_HOLE_STATE, 2);

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x2c),
					(char*)hero + HERO_NAME2);

				if (GUI_bool(Real2Host(ds_readd(DTP2))))
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x30),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));

					result = random_schick(6);

					add_ds_bs(DNG10_HOLE_DAMAGE, result);

					sub_hero_le(hero, result);

					if (ds_readb(DNG10_HOLE_DAMAGE) >= 7)
					{
						/* the secret wall can now be passed forward */
						host_writeb(amap_ptr + 0x73, 0xbf);
					}
				}

				ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));

			} else {
			}

			ds_writew(DNG_HANDLED_POS, target_pos);
		}
	} else if (target_pos == 0x108 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* TRAP: a floorplate */
		if (ds_readb(DNG10_FLOORPLATE_FOUND) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 5) > 0)
		{
			ds_writeb(DNG10_FLOORPLATE_FOUND, 1);

			/* Original-Bug: ???*/
			/* Damage only happens here when the leader of the group tries to disable this trap.
			   If the trap is not found or left alone nobody gets damaged. Weird! */
			if (GUI_bool(get_dtp(0x34)) && test_skill(hero, TA_SCHLOESSER, 7) <= 0)
			{
				if (ds_readb(DNG10_FLOORPLATE_LOADS) != 0)
				{

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x38),
						(char*)hero + HERO_NAME2);

					dec_ds_bs_post(DNG10_FLOORPLATE_LOADS);

					sub_hero_le(hero, dice_roll(3, 6, 0));
				} else {
					strcpy((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x3c));
				}

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}

	} else if (target_pos == 0x10c && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* INFO: you see three holes in the wall */
		GUI_output(get_dtp(0x40));

	} else if (target_pos == 0x30e && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* INFO: collection bowl */
		if (GUI_bool(get_dtp(0x44)))
		{
			answer = GUI_input(get_dtp(0x48), 6) * 10;

			if (get_party_money() < answer)
			{
				GUI_output(get_ltx(0x644));
			} else {
				GUI_output( (answer >= 1000) ? get_dtp(0x50) : get_dtp(0x4c));

				set_party_money( get_party_money() - answer);
			}
		}

	} else if (target_pos == 0x60c && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readbs(DIRECTION) == 2)
	{
		/* INFO: an empty room */
		GUI_output(get_dtp(0x54));

	} else if (target_pos == 0xa0d && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG10_HESHTOT))
	{
		/* FIGHT: scared heshtot */
		if (GUI_bool(get_dtp(0x58)))
		{
			ds_writew((0xd325 + 0), ds_writew((0xd325 + 2), ds_writew((0xd325 + 4), ds_writew((0xd325 + 6), 0xa0d))));
			ds_writew(DNG_HANDLED_POS, 0);

			if (!do_fight(FIGHTS_F129_17))
			{
				ds_writeb(DNG10_HESHTOT, 1);
			}
		} else {
			GUI_output(get_dtp(0x5c));
			sub_group_le(random_schick(6));

			ds_writew(X_TARGET, 1);
			ds_writew(Y_TARGET, 3);
			ds_writeb(DIRECTION, 3);
			DNG_update_pos();
		}

	} else if ((target_pos == 0x1e08 || target_pos == 0x1e09 || target_pos == 0x1e0a || target_pos == 0x1e07 ||
			target_pos == 0x1e06 || target_pos == 0x1e05) &&
			target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* TRAP: column of fire, probability 10%, damage 1W6 for each hero in the party */
		if (random_schick(100) <= 10)
		{
			GUI_output(get_dtp(0x60));

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					sub_hero_le(hero, random_schick(6));
				}
			}
		}
	} else if (target_pos == 0x1e02 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* LEVER: enables/disables fight 111, leader get 2 LE damage */
		if (GUI_bool(get_dtp(0x64)))
		{
			xor_ds_bs(DNG10_MUMMY_LEVER, 1);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x68),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, 2);
		}

	} else if ((target_pos == 0x1a03 || target_pos == 0x1703) && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readb(DNG10_MUMMY_LEVER) != 0)
	{
		/* FIGHT: four mummies again and again */
		ds_writew((0xd325 + 0), ds_writew((0xd325 + 6), 0x1533));
		ds_writew((0xd325 + 2), ds_writew((0xd325 + 4), 0x1c13));
		ds_writew(FIG_DISCARD, 1);
		do_fight(FIGHTS_F129_21);

	} else if (target_pos == 0x110c && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readbs(DIRECTION) == 2)
	{
		/* INFO: glowing walls */
		GUI_output(get_dtp(0x6c));

	} else if (target_pos == 0x190c && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readb(DNG10_DRAGON_QUEST) != 0)
	{
		/* FIGHT: get PLATINKEY for the dragon */
		ds_writew((0xd325 + 0), ds_writew((0xd325 + 6), 0x190a));
		ds_writew((0xd325 + 2), ds_writew((0xd325 + 4), 0x190a));
		do_fight(FIGHTS_F129_29);

	} else if (target_pos == 0x2c0c && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* QUEST: the dragon */
		/* TIP: plunder sucessfully, do the quest and get the reward */
		if (!ds_readb(DNG10_DRAGON_QUEST))
		{
			load_in_head(58);

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70),
						get_dtp(0x74), 2,
						get_dtp(0x78), get_dtp(0x7c));
			} while (answer == -1);

			if (answer == 1)
			{
				/* try to fight the dragon */
				ds_writeb(DNG10_DRAGON_QUEST, 1);

				if (GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70),
						get_dtp(0x80), 2,
						get_ltx(0x08), get_ltx(0x0c)) == 1)
				{
					GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x84), 0);
				} else {
					GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x88), 0);

					sub_group_le(5000);
				}
			} else {
				/* try to plunder the hoard */
				hero = get_hero(0);

				for (answer = result = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero) &&
						test_skill(hero, TA_SCHLEICHEN, host_readbs(hero + HERO_RS_BONUS1) + 3) <= 0)
					{
						result++;
					}
				}

				if (result == 0 && !ds_readb(DNG10_HOARD_PLUNDERED))
				{
					ds_writeb(DNG10_HOARD_PLUNDERED, 1);

					GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x8c), 0);

					/* 2x HEALING POTION, MAGIC POTION, THROWING DAGGER and ...*/
					get_item(145, 1, 2);
					get_item(154, 1, 1);
					get_item(218, 1, 1);

					/* 200 Ducats */
					p_money = get_party_money();
					p_money += 20000L;
					set_party_money(p_money);

				} else {
					ds_writeb(DNG10_DRAGON_QUEST, 1);

					if (GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x80), 2,
								get_ltx(0x08), get_ltx(0x0c)) == 1)
					{
						GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x84), 0);
					} else {
						GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x88), 0);
						sub_group_le(5000);
					}
				}
			}

		} else if (ds_readb(DNG10_DRAGON_QUEST) == 1) {

			load_in_head(58);

			ds_writew(X_TARGET, ds_writew(Y_TARGET, 12));

			if ((answer = get_first_hero_with_item(201)) != -1)
			{
				hero = get_hero(answer);
				result = get_item_pos(hero, 201);
				drop_item(hero, result, 1);

				GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x90), 0);
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x94), 0);

				/* 2x HEALING POTION, MAGIC POTION, THROWING DAGGER, CRYSTAL BALL and ...*/
				get_item(145, 1, 2);
				get_item(154, 1, 1);
				get_item(218, 1, 1);
				get_item(70, 1, 1);

				/* ... 200 Ducats and ... */
				p_money = get_party_money();
				p_money += 20000L;
				set_party_money(p_money);

				/* ... 50 AP */
				add_hero_ap_all(50);

				ds_writeb(DNG10_DRAGON_QUEST, 2);
			} else {
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x98), 0);
			}
		}

	} else if (target_pos == 0x2005 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* INFO: some fools you are */
		GUI_output(get_dtp(0x9c));

	} else if (target_pos == 0x03 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		leave_dungeon();

		ds_writeb(CURRENT_TOWN, ds_readbs(TRV_DEST_REACHED));
		ds_writew(X_TARGET, ds_readws(0x433a));
		ds_writew(Y_TARGET, ds_readws(0x433c));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readbs(0x433e) + 2) & 0x03);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0xa0),
			(char*)get_ltx(4 * (ds_readw(TRV_DESTINATION) + 0xeb)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(3));

		ds_writeb(0x4475, 3);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(DNG_HANDLED_POS, target_pos);

	return 0;
}

void DNG10_chest0_x1(RealPt ptr)
{
	loot_corpse(ptr, get_dtp(0x0c), p_datseg + DNG10_CORPSE_LOOTED);
}

void DNG10_chest0_x2(RealPt chest)
{
	RealPt ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG10_CHEST0_CONTENT));
	loot_chest(Real2Host(chest), get_dtp(0x10), get_dtp(0x14));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG10_chest1_x1(RealPt chest)
{
	RealPt ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG10_CHEST1_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG10_chest2_x1(RealPt chest)
{
	RealPt ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG10_CHEST2_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG10_chest3_x1(RealPt chest)
{
	RealPt ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG10_CHEST3_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG10_chest4_x1(RealPt chest)
{
	RealPt ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG10_CHEST4_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

#if !defined(__BORLANDC__)
}
#endif
