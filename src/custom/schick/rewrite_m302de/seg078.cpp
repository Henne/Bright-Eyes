/**
 *	Rewrite of DSA1 v3.02_de functions of seg078 (dungeon: a tumbledown inn)
 *	Functions rewritten: 10/10 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1- Yo seg078.cpp
 */

#include "v302de.h"

#include <stdio.h>
#include <string.h>

#include "seg000.h"
#include "seg002.h"
#include "seg003.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg032.h"
#include "seg075.h"
#include "seg076.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg104.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG02_handler(void)
{
	signed short target_pos;
	signed short mod_slot;
	signed short i;
	signed short hero_weight;
	Bit32s weight_sum;
	Bit8u *hero;
	Bit8u *amap_ptr;
	signed char flag;

	target_pos = DNG_POS(ds_readbs(DUNGEON_LEVEL), ds_readws(X_TARGET), ds_readws(Y_TARGET));

	amap_ptr = p_datseg + DNG_MAP;

	if ((target_pos == DNG_POS(0,1,2) || target_pos == DNG_POS(0,2,1)))
	{
		if (ds_readds(DAY_TIMER) >= HOURS(8) && ds_readds(DAY_TIMER) <= HOURS(20))
		{
		} else {

			ds_writew((FIG_FLEE_POSITION + 0),
				ds_writew((FIG_FLEE_POSITION + 2),
				ds_writew((FIG_FLEE_POSITION + 4),
				ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(0,2,6,NORTH)))));

			do_fight(FIGHTS_F046_01);
		}

	} else if (target_pos == DNG_POS(0,3,6) && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readbs(DIRECTION) == SOUTH)
	{
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(4));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			 (char*)((test_spell_group(SP_ODEM_ARCANUM, 0) == 0) ? get_tx(5) : get_tx(6)));

		GUI_output(Real2Host(ds_readd(DTP2)));

	} else if (target_pos == DNG_POS(0,4,10) && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readbs(DIRECTION) == SOUTH)
	{
		GUI_output(get_tx(7));

	} else if ((target_pos == DNG_POS(0,7,13) || target_pos == DNG_POS(1,6,5) || target_pos == DNG_POS(1,4,14)) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		if (test_skill((hero = Real2Host(get_first_hero_available_in_group())), TA_SINNESSCHAERFE, 6) > 0)
		{
			GUI_output(get_tx(8));

		} else {
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(9),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			flag = 0;

			while (host_readws(hero + HERO_LE) > 10 && !flag)
			{
				/* KK+4 */
				if (test_attrib(hero, ATTRIB_KK, 4) <= 0)
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx(10),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_hero_le(hero, 1);

				} else {
					flag = 1;
				}
			}

			if (!flag)
			{
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(11),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_hero_le(hero, (host_readws(hero + HERO_LE) > 6 ? random_schick(6) : host_readws(hero + HERO_LE) - 1));

			}
		}

	} else if (target_pos == DNG_POS(0,6,10))
	{
		DNG_fallpit_test(2);

	} else if (target_pos == DNG_POS(0,14,14) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		hero = Real2Host(get_first_hero_available_in_group());
		weight_sum = get_hero_weight(hero);

		if ((hero = Real2Host(get_second_hero_available_in_group())))
		{
			weight_sum += get_hero_weight(hero);
		}

		if (weight_sum >= 7000L)
		{
			DNG_fallpit(6);
		}

	} else if ((target_pos == DNG_POS(0,10,12) || target_pos == DNG_POS(0,10,10)) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		hero = get_hero(0);
		for (i = (signed short)(weight_sum = 0); i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
			{
				weight_sum += get_hero_weight(hero);
			}
		}

		for (i = 0; i < 6; i++)
		{
			if (ds_readws(GROUPS_X_TARGET + 2 * i) == 10 &&
				ds_readws(GROUPS_Y_TARGET + 2 * i) == 11 &&
				ds_readbs(GROUPS_DNG_LEVEL + i) == ds_readbs(DUNGEON_LEVEL) &&
				ds_readbs(GROUPS_DNG_INDEX + i) == ds_readbs(DUNGEON_INDEX))
			{
				weight_sum = 0L;
			}
		}

		hero_weight = get_hero_weight(Real2Host(get_heaviest_hero()));

		host_writebs(amap_ptr + MAP_POS(10,11), (hero_weight >= weight_sum ? 0 : MAP_POS(0,15)));

		play_voc(305);

	} else if ((target_pos == DNG_POS(0,10,13) || target_pos == DNG_POS(0,10,9)) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		host_writeb(amap_ptr + MAP_POS(10,11), 0);

		play_voc(305);

	} else if ((target_pos == DNG_POS(0,7,6) || target_pos == DNG_POS(0,8,6) || target_pos == DNG_POS(0,8,8)))
	{
		if (ds_readb(DNG02_SPHERE_ACTIVE) != 0)
		{
			/* Sphaerenriss */
			GUI_output(get_tx(41));

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
					!hero_dead(hero)) /* TODO: Why !dead? */
				{
					hero_disappear(hero, i, -1);
				}
			}

		} else {
			hero = get_hero(0);
			for (i = mod_slot = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
					!hero_dead(hero) &&
					!hero_seen_phantom(hero))
				{
					mod_slot = get_free_mod_slot();
					set_mod_slot(mod_slot, HOURS(5), hero + (HERO_ATTRIB + 3 * ATTRIB_MU), -3, (signed char)i);
					{
						hero_seen_phantom_set(hero, (mod_slot = 1)); /* set 'seen_phantom' flag */
					}
				}
			}

			if (mod_slot != 0)
			{
				GUI_output(get_tx(12));
			}
		}

	} else if (target_pos == DNG_POS(0,7,1) && !ds_readb(DNG02_RAVEN_DESTROYED) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		if (GUI_bool(get_tx(13)))
		{
			/* destroy the raven */
			ds_writeb(DNG02_RAVEN_DESTROYED, 1);

			sub_group_le(random_schick(20));

			GUI_output(get_tx(14));

			host_writebs(amap_ptr + MAP_POS(5,2), 0);
		}

	} else if (target_pos == DNG_POS(0,11,6) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		hero = get_hero(0);
		for (i = (signed short)(weight_sum = 0); i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
			{
				weight_sum += get_hero_weight(hero);
			}
		}

		if (weight_sum >= 4000)
		{
			GUI_output(get_tx(15));

			host_writeb(amap_ptr + MAP_POS(11,8), 0);
		}

	} else if ((target_pos == DNG_POS(0,10,6) || target_pos == DNG_POS(0,11,5)) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		for (i = 0; i < 6; i++)
		{
			weight_sum = 0;

			if (ds_readws(GROUPS_X_TARGET + 2 * i) == 11 &&
				ds_readws(GROUPS_Y_TARGET + 2 * i) == 6 &&
				ds_readbs(GROUPS_DNG_LEVEL + i) == ds_readbs(DUNGEON_LEVEL) &&
				ds_readbs(GROUPS_DNG_INDEX + i) == ds_readbs(DUNGEON_INDEX))
			{
				hero = get_hero(0);
				for (mod_slot = 0; mod_slot <= 6; mod_slot++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == i)
					{
						weight_sum += get_hero_weight(hero);
					}
				}

				break;

			} else if (ds_readws(GROUPS_X_TARGET + 2 * i) == 11 &&
					ds_readws(GROUPS_Y_TARGET + 2 * i) == 8 &&
					ds_readbs(GROUPS_DNG_LEVEL + i) == ds_readbs(DUNGEON_LEVEL) &&
					ds_readbs(GROUPS_DNG_INDEX + i) == ds_readbs(DUNGEON_INDEX))
			{
				weight_sum = 5000L;
			}
		}

		host_writeb(amap_ptr + MAP_POS(11,8), (weight_sum >= 4000L ? (DNG_TILE_CORRIDOR << 4) : (DNG_TILE_WALL << 4)));

	} else if (target_pos == DNG_POS(1,12,5) && ds_readbs(DIRECTION) == NORTH &&
			 (target_pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)))
	{
		ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));

		GUI_output(get_tx(16));

	} else if (target_pos == DNG_POS(1,4,5) && ds_readbs(DIRECTION) == SOUTH &&
			 (target_pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)))
	{
		ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));

		GUI_output(get_tx(17));

	} else if (target_pos == DNG_POS(1,7,1) && !ds_readb(DNG02_SPHERE_KNOWN))
	{
		load_in_head(43);

		GUI_dialogbox(ds_readfp(DTP2), NULL, get_tx(18), 0);

		ds_writeb(DNG02_SPHERE_TIMER, 7);
		ds_writeb(DNG02_SPHERE_KNOWN, 1);

	} else if ((target_pos == DNG_POS(1,1,11) || target_pos == DNG_POS(1,1,8)) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		GUI_output(get_tx(22));

	} else if (target_pos == DNG_POS(1,3,7) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		if (GUI_bool(get_tx(23)))
		{
			GUI_output(get_tx(24));
		}

	} else if (target_pos == DNG_POS(1,8,9) && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG02_APPARATURE_DESTROYED))
	{
		/* petrification trap */
		GUI_output(get_tx(25));

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				host_readbs(hero + HERO_MR) < 8)
			{
				or_ptr_bs(hero + HERO_FLAGS1, 0x04); /* set 'petrified' flag */

				sprintf((char*)Real2Host(ds_readfp(DTP2)),
					(char*)get_tx(42),
					(char*)hero + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readfp(DTP2)));
			}
		}

		check_group();

	} else if (target_pos == DNG_POS(1,13,11) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		do {
			i = GUI_radio(get_tx(26), 2,
					get_tx(27),
					get_tx(28));
		} while (i == -1);

		if (i == 1)
		{
			DNG_dec_level();
			target_pos = DNG_POS(0,13,11);

			do {
				i = GUI_radio(get_tx(29), 2,
						get_tx(30),
						get_tx(31));
			} while (i == -1);

			if (i == 2)
			{
				DNG_inc_level();
				target_pos = DNG_POS(1,13,11);
			}
		}

	} else if (target_pos == DNG_POS(0,13,11) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		do {
			i = GUI_radio(get_tx(43), 2,
					get_tx(27),
					get_tx(28));
		} while (i == -1);

		if (i == 1)
		{
			DNG_inc_level();
			target_pos = DNG_POS(1,13,11);

			do {
				i = GUI_radio(get_tx(26), 2,
						get_tx(27),
						get_tx(28));
			} while (i == -1);

			if (i == 1)
			{
				DNG_dec_level();
				target_pos = DNG_POS(0,13,11);
			}
		}

	} else if ((target_pos == DNG_POS(0,12,11) || target_pos == DNG_POS(0,14,11) || target_pos == DNG_POS(0,13,10) || target_pos == DNG_POS(0,13,12)) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		ds_writew((FIG_FLEE_POSITION + 0),
			ds_writew((FIG_FLEE_POSITION + 2),
			ds_writew((FIG_FLEE_POSITION + 4),
			ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(0,12,11,NORTH)))));

		if (!do_fight(FIGHTS_F046_31))
		{
			ds_writeb(ALWAYS_ONE2, 1);

			add_hero_ap_all(30);
		}

	} else if (target_pos == DNG_POS(1,1,1) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		seg092_06b4(0);

	} else if (target_pos == DNG_POS(1,9,14) && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readbs(DIRECTION) == EAST)
	{
		GUI_output(get_tx(35));

	} else if (target_pos == DNG_POS(0,1,8) &&
			(target_pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)) &&
			ds_readbs(DIRECTION) == SOUTH &&
			ds_readb(DNG02_SECRET_DOOR1) != 2)
	{
		/* Original-Bug: this should be the leader, not hero no 0 */
		hero = get_hero(0);

		if (ds_readb(DNG02_SECRET_DOOR1) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 6) > 0)
		{
			ds_writeb(DNG02_SECRET_DOOR1, 1);

			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_tx(37),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
				(char*)((i = test_skill(hero, TA_SCHLOESSER, 4)) > 0 ? get_tx(39) : get_tx(38)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat((char*)Real2Host(ds_readfp(DTP2)),
				(char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readfp(DTP2)));

			if (i > 0)
			{
				/* unlike other similar code positions, the lower 4 bits of the map entry are preserved here. Is there a reason? */
				and_ptr_bs(amap_ptr + MAP_POS(1,9), (DNG_TILE_CORRIDOR << 4) + 0x0f);
				ds_writeb(DNG02_SECRET_DOOR1, 2);
				DNG_update_pos();
			}

			ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));
		}

	} else if (target_pos == DNG_POS(0,4,8) &&
			(target_pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)) &&
			ds_readbs(DIRECTION) == SOUTH &&
			ds_readb(DNG02_SECRET_DOOR2) != 2)
	{
		/* Original-Bug: this should be the leader, not hero no 0 */
		hero = get_hero(0);

		if (ds_readb(DNG02_SECRET_DOOR2) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 2) > 0)
		{
			ds_writeb(DNG02_SECRET_DOOR2, 1);

			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_tx(37),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
				(char*)((i = test_skill(hero, TA_SCHLOESSER, 2)) > 0 ? get_tx(39) : get_tx(38)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat((char*)Real2Host(ds_readfp(DTP2)),
				(char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readfp(DTP2)));

			if (i > 0)
			{
				host_writeb(amap_ptr + MAP_POS(4,9), DNG_TILE_CORRIDOR << 4);
				ds_writeb(DNG02_SECRET_DOOR2, 2);
				DNG_update_pos();
			}

			ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));
		}

	} else if (((target_pos == DNG_POS(1,4,9) && ds_readbs(DIRECTION) == EAST) ||
			(target_pos == DNG_POS(1,6,9) && ds_readbs(DIRECTION) == WEST)) &&
			(target_pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)) &&
			ds_readb(DNG02_SECRET_DOOR3) != 2)
	{
		hero = Real2Host(get_first_hero_available_in_group());

		if (ds_readb(DNG02_SECRET_DOOR3) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 5) > 0)
		{
			ds_writeb(DNG02_SECRET_DOOR3, 1);

			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_tx(37),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
				(char*)((i = test_skill(hero, TA_SCHLOESSER, 4)) > 0 ? get_tx(39) : get_tx(38)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat((char*)Real2Host(ds_readfp(DTP2)),
				(char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));

			if (i > 0)
			{
				host_writeb(amap_ptr + MAP_POS(5,9), DNG_TILE_CORRIDOR << 4);
				ds_writeb(DNG02_SECRET_DOOR3, 2);
				DNG_update_pos();
			}

			i = random_schick(6) + 4;

			sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
				(char*)get_tx(40),
				(char*)hero + HERO_NAME2,
				i);

			strcat((char*)Real2Host(ds_readfp(DTP2)),
				(char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readfp(DTP2)));

			sub_hero_le(hero, i);

			ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));
		}

	} else if (target_pos == DNG_POS(0,1,0) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		leave_dungeon();
		ds_writebs(CURRENT_TOWN, (signed char)ds_readws(TRV_DEST_REACHED));
		ds_writews(X_TARGET, ds_readws(ARRIVAL_X_TARGET));
		ds_writews(Y_TARGET, ds_readws(ARRIVAL_Y_TARGET));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readws(ARRIVAL_DIRECTION) + 2) & 3);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(44),
			(char*)get_ttx(ds_readws(TRV_DESTINATION) + 0xeb));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(3));

		ds_writeb(FADING_STATE, 3);

	}

	ds_writews(DNG_HANDLED_POS, target_pos);

	return 0;
}

void DNG02_chest00_func3(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG02_CHEST00_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG02_chest01_func3(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG02_CHEST01_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG02_chest02_func3(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG02_CHEST02_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG02_chest03_func3(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG02_CHEST03_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG02_chest04_func3(RealPt)
{
	signed short i;
	signed short answer;
	signed short tw_bak;
	Bit8u *hero;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	if (!ds_readb(DNG02_APPARATURE_DESTROYED))
	{
		do {
			answer = GUI_radio(get_tx(1), 2,
						get_tx(2),
						get_tx(3));
		} while (answer == -1);

		if (answer == 1)
		{
			chest_petrified();
		} else {
			ds_writeb(DNG02_APPARATURE_DESTROYED, 1);

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					sub_hero_le(hero, random_schick(6));
				}
			}
		}
	} else {
		GUI_output(get_ttx(522));
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
}

void DNG02_chest05_func3(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG02_CHEST05_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG02_chest06_func1(RealPt chest)
{
	loot_corpse(chest, get_tx(19), p_datseg + DNG02_CORPSE);
}

void DNG02_chest06_func3(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG02_CHEST06_CONTENT));
	loot_chest(Real2Host(chest), get_tx(20), get_tx(21));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/**
 * \brief   print introductions of dungeon #2 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG02_fight_intro(signed short fight_id)
{
	if (fight_id == 62) {
		GUI_output(get_tx(32));
	} else if (fight_id == 66) {
		GUI_output(get_tx(33));
	} else if (fight_id == 68) {
		GUI_output(get_tx(34));
	} else if (fight_id == 72) {
		GUI_output(get_tx(36));
	}
}

#if !defined(__BORLANDC__)
}
#endif
