/**
 *	Rewrite of DSA1 v3.02_de functions of seg091 (dungeon: prem mine)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg091.cpp
 */

#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg003.h"
#include "seg007.h"
#include "seg025.h"
#include "seg047.h"
#include "seg091.h"
#include "seg092.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG13_handler(void)
{
	signed short pos;
	signed short tw_bak;
	Bit32s p_money;
	Bit8u *hero;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	pos = (ds_readbs(DUNGEON_LEVEL) << 12) + (ds_readws(X_TARGET) << 8) + ds_readws(Y_TARGET);

	hero = Real2Host(get_first_hero_available_in_group());

	if (!(ds_readds(DAY_TIMER) % MINUTES(5)) &&
		(pos == 0x501 || pos == 0x601 || pos == 0x701 || pos == 0x801) &&
		random_schick(100) <= 5)
	{
		hero = get_hero(get_random_hero());

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(0x48),
			(char*)hero + HERO_NAME2);

		GUI_output(Real2Host(ds_readd(DTP2)));

		sub_hero_le(hero, 2);
	}

	if (pos == 0x90d && pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG13_LANTERN_FLAG))
	{
		if (GUI_bool(get_tx(0x04)))
		{
			ds_writeb(DNG13_LANTERN_FLAG, 1);

			get_item(25, 1, 1);
		}

	} else if ((pos == 0x10c || pos == 0xe06 || pos == 0xe0d || pos == 0x301) &&
			pos != ds_readws(DNG_HANDLED_POS))
	{
		GUI_output(get_tx(0x08));

	} else if (pos == 0x20d && pos != ds_readws(DNG_HANDLED_POS))
	{
		GUI_dialog_na(60, get_tx(0x0c));

	} else if (pos == 0x30c && pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG13_unblock_passage(get_tx(0x10), p_datseg + DNG13_PASSAGE1_FLAG);

	} else if (pos == 0x409 &&
			 (pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)) &&
			ds_readbs(DIRECTION) == WEST)
	{
		GUI_output(get_tx(0x18));
		ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));

	} else if (pos == 0xa02 &&
			 (pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)) &&
			ds_readbs(DIRECTION) == EAST)
	{
		GUI_output(get_tx(0x18));
		ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));

	} else if (pos == 0x509 && pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG13_unblock_passage(get_tx(0x1c), p_datseg + DNG13_PASSAGE2_FLAG);

	} else if (pos == 0x209 && pos != ds_readws(DNG_HANDLED_POS))
	{
		loot_multi_chest(p_datseg + DNG13_CHEST_EQUIPS, get_tx(0x20));

	} else if (pos == 0xa06 && pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG13_unblock_passage(get_tx(0x1c), p_datseg + DNG13_PASSAGE3_FLAG);

	} else if (pos == 0xb05 && pos != ds_readws(DNG_HANDLED_POS))
	{
		seg092_06b4(0);

	} else if (pos == 0xa0a && pos != ds_readws(DNG_HANDLED_POS))
	{
		GUI_output(get_tx(0x30));

	} else if (pos == 0xe09 && pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG13_unblock_passage(get_tx(0x1c), p_datseg + DNG13_PASSAGE4_FLAG);

	} else if (pos == 0x703 && pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG13_unblock_passage(get_tx(0x1c), p_datseg + DNG13_PASSAGE5_FLAG);

	} else if (pos == 0x401 && pos != ds_readws(DNG_HANDLED_POS))
	{
		seg092_06b4(0);

	} else if (pos == 0x505 && pos != ds_readws(DNG_HANDLED_POS))
	{
		GUI_output(get_tx(0x3c));

		GUI_output(get_tx(0x40));

	} else if (pos == 0x407 && pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG13_MONEY_FLAG))
	{
		GUI_output(get_tx(0x44));

		ds_writeb(DNG13_MONEY_FLAG, 1);

		p_money = get_party_money();
		p_money += 41L;
		set_party_money(p_money);

	} else if ((pos == 0x70e || pos == 0x50c || pos == 0x108 || pos == 0x403 || pos == 0x705 || pos == 0xe01 || pos == 0x808) &&
			pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG13_collapsing_ceiling();

	} else if ((pos == 0xe0c || pos == 0x701) && pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG13_collapsing_ceiling_easy();

	} else if (pos == 0x70f && pos != ds_readws(DNG_HANDLED_POS))
	{
        /* AP depending on number of collapsed ceilings and lost heroes */
		add_hero_ap_all(ds_readb(DNG13_COLLAPSECOUNT) * (ds_readb(DNG13_HEROCOUNT) == count_heroes_in_group() ? 8 : 5));

		leave_dungeon();

		ds_writew(X_TARGET, 9);
		ds_writew(Y_TARGET, 1);
		ds_writeb(DIRECTION, 1);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(DNG_HANDLED_POS, pos);

	return 0;
}

/**
 * \brief   unblock a passage if its blocked
 *
 * \param   text        text for the output
 * \param   flag        pointer to the flag (0 = blocked / 1 = free)
 */
void DNG13_unblock_passage(Bit8u* text, Bit8u* flag)
{
	signed short has_items;

	/* check if passage is blocked */
	if (!host_readb(flag))
	{
		/* ask if the heros want to try */
		if (GUI_bool(text))
		{
			/* check if the group has ALL of these items:
				SHOVEL, HOE, CROWBAR, FRANCESCA
				SCHAUFEL, HACKE, BRECKEISEN, WURFBEIL
			*/

			has_items = 0;

			if (get_first_hero_with_item(73) != -1 &&
				get_first_hero_with_item(93) != -1 &&
				get_first_hero_with_item(26) != -1 &&
				get_first_hero_with_item(16) != -1)
			{
				has_items = 1;
			}

			/* if all items are available, it takes 2 hours instead of 6 */
			timewarp(!has_items ? HOURS(6) : HOURS(2));

			GUI_output(get_tx(0x14));

			/* mark this passage as free */
			host_writeb(flag, 1);

		} else {

			ds_writew(X_TARGET, ds_readws(X_TARGET_BAK));
			ds_writew(Y_TARGET, ds_readws(Y_TARGET_BAK));
		}
	}
}

void DNG13_corpse0(RealPt ptr)
{
	loot_corpse(ptr, get_tx(0x24), p_datseg + DNG13_CORPSE0_FLAG);
}

void DNG13_chest0(RealPt chest)
{
	RealPt bak;

	bak = (RealPt)host_readd(Real2Host(chest) + 0xb);
	host_writed(Real2Host(chest) + 0xb, (Bit32u)RealMake(datseg, DNG13_CHEST0_CONTENT));

	loot_chest(Real2Host(chest), get_tx(0x28), get_tx(0x2c));

	host_writed(Real2Host(chest) + 0xb, (Bit32u)bak);
}

void DNG13_corpse1(RealPt ptr)
{
	loot_corpse(ptr, get_tx(0x38), p_datseg + DNG13_CORPSE1_FLAG);
}

void DNG13_chest1(RealPt chest)
{
	RealPt bak;

	bak = (RealPt)host_readd(Real2Host(chest) + 0xb);
	host_writed(Real2Host(chest) + 0xb, (Bit32u)RealMake(datseg, DNG13_CHEST1_CONTENT));

	loot_chest(Real2Host(chest), get_tx(0x28), get_tx(0x2c));

	host_writed(Real2Host(chest) + 0xb, (Bit32u)bak);
}

/**
 * \brief   print introductions of dungeon #13 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG13_fight_intro(signed short fight_id)
{
	if (fight_id == FIGHTS_DPRE10_1)
	{
		GUI_output(get_tx(0x34));
	}
}

void DNG13_collapsing_ceiling(void)
{
	signed short i;
	signed short fails;
	signed short has_items;
	Bit8u *hero;

	inc_ds_bs_post(DNG13_COLLAPSECOUNT);

	hero = get_hero(0);
	for (i = fails = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_skill(hero, TA_SCHLEICHEN, -4) <= 0)
		{
			fails++;
		}
	}

	if (fails > 1)
	{
		/* at least two heros failed in the skill test */
		GUI_output(get_tx(0x4c));

		sub_group_le(dice_roll(1, 6, 4));

		GUI_output(get_tx(0x50));

		/* check if the group has ALL of these items:
			SHOVEL, HOE, CROWBAR, FRANCESCA
			SCHAUFEL, HACKE, BRECKEISEN, WURFBEIL
		*/

		has_items = 0;

		if (get_first_hero_with_item(73) != -1 &&
			get_first_hero_with_item(93) != -1 &&
			get_first_hero_with_item(26) != -1 &&
			get_first_hero_with_item(16) != -1)
		{
			has_items = 1;
		}

		/* if all items are available, it takes 6 hours instead of one day */
		timewarp(has_items == 0 ? HOURS(6) : DAYS(1));
	}
}

void DNG13_collapsing_ceiling_easy(void)
{
	signed short i;
	signed short fails;
	signed short has_items;
	Bit8u *hero;

	inc_ds_bs_post(DNG13_COLLAPSECOUNT);

	hero = get_hero(0);
	for (i = fails = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_skill(hero, TA_SCHLEICHEN, -1) <= 0)
		{
			fails++;
		}
	}

	if (fails > 1)
	{
		/* at least two heros failed in the skill test */
		GUI_output(get_tx(0x4c));

		sub_group_le(dice_roll(1, 6, 5));

		GUI_output(get_tx(0x50));

		/* check if the group has ALL of these items:
			SHOVEL, HOE, CROWBAR, FRANCESCA
			SCHAUFEL, HACKE, BRECKEISEN, WURFBEIL
		*/

		has_items = 0;

		if (get_first_hero_with_item(73) != -1 &&
			get_first_hero_with_item(93) != -1 &&
			get_first_hero_with_item(26) != -1 &&
			get_first_hero_with_item(16) != -1)
		{
			has_items = 1;
		}

		/* if all items are available, it takes 18 hours instead of 12 */
		timewarp(has_items == 0 ? HOURS(18) : HOURS(12));
	}
}

#if !defined(__BORLANDC__)
}
#endif
