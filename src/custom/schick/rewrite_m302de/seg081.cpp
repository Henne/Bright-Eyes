/**
 *	Rewrite of DSA1 v3.02_de functions of seg081 (dungeon: cave2)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg081.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg032.h"
#include "seg049.h"
#include "seg075.h"
#include "seg076.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG06_handler(void)
{
	signed short i;
	signed short target_pos;
	signed short l3;
	signed short l4;
	signed short tw_bak;
	Bit32s p_money;
	Bit8u *hero;
	Bit8u *hero_first;
	Bit8u *hero_second;
	Bit8u *amap_ptr;

	amap_ptr = p_datseg + DNG_MAP;
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	if (target_pos == 0xe05 && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG06_BOOK_FLAG))
	{
		if (GUI_bool(get_tx(1)) && get_item(224, 1, 1))
		{
			ds_writeb(DNG06_BOOK_FLAG, 1);
		}

	}

	if (target_pos == 0x909 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		GUI_output(get_tx(35));

	} else if (target_pos == 0x40d && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		if (GUI_bool(get_tx(2)))
		{
			strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx(3));

			if (!ds_readb(DNG06_MONEY_FLAG))
			{
				ds_writeb(DNG06_MONEY_FLAG, 1);

				hero = Real2Host(get_first_hero_available_in_group());

				i = dice_roll(3, 20, 0);

				add_party_money(100L * i);

				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					(char*)get_tx(4),
					(char*)hero + HERO_NAME2,
					i);

				strcat((char*)Real2Host(ds_readd(DTP2)), (char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
			}

			GUI_output(Real2Host(ds_readd(DTP2)));
		}

	} else if (target_pos == 0x503 && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG06_PROVIANT_FLAG))
	{
		if (GUI_bool(get_tx(5)))
		{
			get_item(45, 1, 40);
			ds_writeb(DNG06_PROVIANT_FLAG, 1);
		}

	} else if (target_pos == 0x201 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		hero = get_hero(0);
		for (i = l3 = 0; i < 2; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero))
			{
				l3++;
			}
		}

		hero_first = Real2Host(get_first_hero_available_in_group());
		if (l3 > 1)
		{
			hero_second = Real2Host(get_second_hero_available_in_group());
		}

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)(l3 == 1 ? p_datseg + STR_S_WAR : p_datseg + STR_S_UND_S_WAREN),
			(char*)hero_first + HERO_NAME2,
			(char*)hero_second + HERO_NAME2);

		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx(6));

		GUI_output(Real2Host(ds_readd(DTP2)));

		sub_hero_le(hero_first, random_schick(6));
		if (l3 > 1)
		{
			sub_hero_le(hero_second, random_schick(6));
		}

	} else if (target_pos == 0x907 && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG06_PITDOOR_FLAG))
	{
		hero = get_hero(0);
		for (i = l3 = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, TA_GEFAHRENSINN, 5) > 0)
			{
				l3 = 1;
				break;
			}
		}

		if (l3 != 0)
		{
			if (GUI_bool(get_tx(7)))
			{
				GUI_output(get_tx(8));

				hero = get_hero(0);
				for (i = l3 = 0; i <= 6; i++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero) &&
						test_skill(hero, TA_GEFAHRENSINN, 5) > 0)
					{
						l3 = 1;
						break;
					}
				}

				if (l3 != 0 && GUI_bool(get_tx(9)))
				{
					ds_writeb(DNG06_PITDOOR_FLAG, 1);

					GUI_output(get_tx(10));
				}

				if (!ds_readb(DNG06_PITDOOR_FLAG))
				{
					DNG_fallpit_test(6);
				}
			} else {
				if (!ds_readb(DNG06_PITDOOR_FLAG))
				{
					DNG_fallpit_test(6);
				}
			}
		} else {
			if (!ds_readb(DNG06_PITDOOR_FLAG))
			{
				DNG_fallpit_test(6);
			}
		}

	} else if (target_pos == 0x807 && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readb(DNG06_GOLDKEY_FLAG) != 2)
	{
		hero = get_hero(0);
		for (i = l3 = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, TA_SINNESSCHAERFE, 10) > 0)
			{
				l3++;
			}
		}

		if (ds_readb(DNG06_GOLDKEY_FLAG) != 0 || l3 != 0)
		{
			ds_writeb(DNG06_GOLDKEY_FLAG, 1);

			if (GUI_bool(get_tx(11)))
			{
				ds_writeb(DNG06_GOLDKEY_FLAG, 2);

				get_item(195, 1, 1);

				/* TODO: This is not neccessary */
				hero = Real2Host(get_first_hero_available_in_group());

				add_party_money(3000L);
			}
		}

	} else if (target_pos == 0xb06 && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readb(DNG06_COUNTDOWN_FLAG) != 0)
	{
		if (div16(host_readb(amap_ptr + 0x1b)) != 1 ||
			div16(host_readb(amap_ptr + 0x62)) != 1 ||
			div16(host_readb(amap_ptr + 0x4b)) != 1)
		{
			dec_ds_bs_post(DNG06_COUNTDOWN_FLAG);

			l3 = random_schick(5);

			hero = Real2Host(get_first_hero_available_in_group());

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)(l3 == 1 ? get_tx(12) : get_tx(13)),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, dice_roll(l3, 6, 0));
		}

	} else if (target_pos == 0x1d05 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		do {
			i = GUI_radio(get_tx(14), 3,
					get_tx(15),
					get_tx(16),
					get_tx(17));
		} while (i == -1);

		hero = Real2Host(get_first_hero_available_in_group());

		if (i == 1 || i == 3)
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)(i == 1 ? get_tx(18) : get_tx(19)),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 1)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, dice_roll(3, 6, 0));
		} else {

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero) &&
					test_skill(hero, TA_KOERPERBEH, host_readbs(hero + HERO_RS_BONUS1)) <= 0)
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx(20),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_hero_le(hero, dice_roll(3, 6, 0));
				}
			}
		}

	} else if (target_pos == 0x1102 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* Hole in the wall: want to reach into it? want to move the lever? */
		if (GUI_bool(get_tx(21)) && GUI_bool(get_tx(22)))
		{
			for (i = l3 = 0; i < 6; i++)
			{
				if (ds_readws(GROUPS_X_TARGET + 2 * i) == 3 &&
					ds_readws(GROUPS_Y_TARGET + 2 * i) == 6 &&
					ds_readbs(CURRENT_GROUP) != i)
				{
					hero = get_hero(0);
					for (l4 = 0; l4 <= 6; l4++, hero += SIZEOF_HERO)
					{
						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == i)
						{
							l3 = 1;
							break;
						}
					}
				}
			}

			if (l3 != 0)
			{
				/* some heroes are in the right position: hear gentle click */
				GUI_output(get_tx(24));

				ds_writeb(DNG06_LEVER_FLAG, 1);
			} else {
				/* liquid from ceiling causes damage */
				hero = Real2Host(get_first_hero_available_in_group());

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(23),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_hero_le(hero, 2);
			}
		}

	} else if (target_pos == 0x1306 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* Hole in the wall: want to reach into it? want to move the lever? */
		if (GUI_bool(get_tx(21)) && GUI_bool(get_tx(22)))
		{
			for (i = l3 = 0; i < 6; i++)
			{
				if (ds_readws(GROUPS_X_TARGET + 2 * i) == 1 &&
					ds_readws(GROUPS_Y_TARGET + 2 * i) == 2 &&
					ds_readbs(CURRENT_GROUP) != i)
				{
					hero = get_hero(0);
					for (l4 = 0; l4 <= 6; l4++, hero += SIZEOF_HERO)
					{
						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == i)
						{
							l3 = 1;
							break;
						}
					}
				}
			}

			if (l3 != 0)
			{
				/* some heroes are in the right position: hear gentle click */
				GUI_output(get_tx(24));

				ds_writeb(DNG06_LEVER_FLAG, 1);
			} else {
				/* liquid from ceiling causes damage */
				hero = Real2Host(get_first_hero_available_in_group());

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(23),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_hero_le(hero, 2);
			}
		}

	} else if (target_pos == 0x1508 && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG06_LEVER_FLAG))
	{
		hero = Real2Host(get_first_hero_available_in_group());

		/* large disk comes down from ceiling */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(25),
			(char*)hero + HERO_NAME2);

		GUI_output(Real2Host(ds_readd(DTP2)));

		if (test_skill(hero, TA_KOERPERBEH, 0) > 0 && test_skill(hero, TA_GEFAHRENSINN, 0) > 0)
		{
			/* evasion succeeds */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(26),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			GUI_output(Real2Host(ds_readd(DTP2)));

		} else {
			/* disk causes heavy damage */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(27),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, dice_roll(3, 6, 0));
		}

	} else if (target_pos == 0x170d && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG06_FIGHT19_FLAG))
	{
		if (GUI_bool(get_tx(28)))
		{
			hero = Real2Host(get_first_hero_available_in_group());

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(29),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, 2);
		}

	} else if (target_pos == 0x160b && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG06_DASPTREAS_FOUND))
	{
		/* treasure of Daspota found */
		GUI_output(get_tx(36));

		p_money = get_party_money();
		p_money += 50000L;
		set_party_money(p_money);

		loot_multi_chest(p_datseg + DNG06_CHEST_DASPTREAS, get_tx(37));

		ds_writeb(DNG06_DASPTREAS_FOUND, 1);

		add_hero_ap_all(30);

	} else if (target_pos == 0x180e && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		ds_writew(FIG_FLEE_POSITION, ds_writew((FIG_FLEE_POSITION + 6), 0x163e));
		ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 2), 0x1d0d));

		if (!do_fight(FIGHTS_F094_19))
		{
			ds_writeb(DNG06_FIGHT19_FLAG, 1);
		}

	} else if (target_pos == 0xd0f && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		leave_dungeon();

		ds_writeb(CURRENT_TOWN, ds_readbs(TRV_DEST_REACHED));
		ds_writew(X_TARGET, ds_readws(ARRIVAL_X_TARGET));
		ds_writew(Y_TARGET, ds_readws(ARRIVAL_Y_TARGET));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readb(ARRIVAL_DIRECTION) + 2) & 3);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(38),
			(char*)get_ttx(ds_readws(TRV_DESTINATION) + 0xeb));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(2));

		ds_writeb(FADING_STATE, 3);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(DNG_HANDLED_POS, target_pos);

	return 0;
}


/**
 * \brief   print introductions of dungeon #6 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG06_fight_intro(signed short fight_id)
{
	if (fight_id == FIGHTS_F094_22)
	{
		/* this is true all the time */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(30),
			(char*)(!ds_readb(ESTORIK_KNOWN) ? get_tx(32) : get_tx(31)));

		GUI_output(Real2Host(ds_readd(DTP2)));
	}
}


void DNG06_chest1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG06_CHEST1_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}


void DNG06_chest2(RealPt chest)
{
	signed short i;
	Bit8u *hero;

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_tx(33));

	hero = get_hero(0);
	for (i = 0; i <=6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_skill(hero, TA_LESEN, 0) > 0)
		{
			strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx(34));
			break;
		}
	}

	GUI_output(Real2Host(ds_readd(DTP2)));
}

/**
 * \brief   handles the pit in Kultstaette des Namenlosen, level 2, square (3,12)
 *
 * 	heroes in the pit are separated into a new group.
 * 	This group will have an entry GROUPS_DNG_LEVEL = 2,
 * 	which indicates that it is in the pit.
 */

void DNG09_pitfall(void)
{
	signed short i;
	signed short l3;
	Bit8u *hero;
	Bit8u *hero_first;
	Bit8u *hero_second;

	hero = Real2Host(get_first_hero_available_in_group());

	if (!ds_readb(DNG09_PIT_FLAG))
	{
		for (i = l3 = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, TA_GEFAHRENSINN, 4) > 0)
				/* TODO: potential Original-Bug: Why should 'petrified' or 'uncouscious' (or maybe other properties ) be o.k. here?? */
			{
				l3 = 1;
			}
		}

		if (l3 != 0)
		{
			GUI_output(get_tx(29));

		} else {
			hero_first = Real2Host(get_first_hero_available_in_group());
			hero_second = Real2Host(get_second_hero_available_in_group());

			if (ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) >= 2)
			{
				/* the current group has at least two heroes */

				/* print message */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(30),
					(char*)hero_first + HERO_NAME2,
					(char*)hero_second + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				/* each of these two heroes looses 3W6+3 LE */
				sub_hero_le(hero_first, dice_roll(3, 6, 3));
				sub_hero_le(hero_second, dice_roll(3, 6, 3));

				/* find an empty group */
				l3 = 0;
				while (ds_readb(GROUP_MEMBER_COUNTS + l3) != 0) l3++;

				/* put these heroes in empty group */
				host_writeb(hero_first + HERO_GROUP_NO, (signed char)l3);
				host_writeb(hero_second + HERO_GROUP_NO, (signed char)l3);
				add_ds_bs(GROUP_MEMBER_COUNTS + l3, 2);
				sub_ds_bs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP), 2);

				GRP_save_pos(l3);

				ds_writeb(GROUPS_DNG_LEVEL + l3, ds_readbs(DUNGEON_LEVEL) + 1);

				ds_writeb(DNG09_PIT_FLAG, 2);
			} else {
				/* the current group has only one hero */

				/* print message */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(31),
					(char*)hero_first + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero_first + HERO_SEX), 0)));

				GUI_output(Real2Host(ds_readd(DTP2)));

				/* this hero looses 3W6+3 LE */
				sub_hero_le(hero_first, dice_roll(3, 6, 3));

				/* find an empty group */
				l3 = 0;
				while (ds_readb(GROUP_MEMBER_COUNTS + l3) != 0) l3++;

				/* put this hero in an empty group */
				host_writeb(hero_first + HERO_GROUP_NO, (signed char)l3);
				inc_ds_bs_post(GROUP_MEMBER_COUNTS + l3);
				dec_ds_bs_post(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));

				GRP_save_pos(l3);

				ds_writeb(GROUPS_DNG_LEVEL + l3, ds_readbs(DUNGEON_LEVEL) + 1);

				ds_writeb(DNG09_PIT_FLAG, 1);
			}
		}

	} else if (GUI_bool(get_tx(33))) {

		if ((i = DNG_check_climb_tools()) != -1)
		{
			l3 = group_count_item(121);
			l3 += group_count_item(32);

			if (l3 >= 2 || (l3 == 1 && i))
			{
				host_writeb(Real2Host(ds_readd(DTP2)), 0);

				if (i)
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(768),
						(char*)get_hero(i - 1) + HERO_NAME2);
				}

				strcat((char*)Real2Host(ds_readd(DTP2)),
					(char*)(ds_readbs(DNG09_PIT_FLAG) == 2 ? get_tx(36) : get_tx(37)));

				GUI_output(Real2Host(ds_readd(DTP2)));

				for (i = 0; i < 6; i++)
				{
					if (ds_readbs(GROUPS_DNG_INDEX + i) == 9 && ds_readbs(GROUPS_DNG_LEVEL + i) == 2)
					{
						ds_writeb(GROUPS_DNG_LEVEL + i, 1);
					}
				}

				GRP_merge();

				ds_writeb(DNG09_PIT_FLAG, 0);
			} else {
				GUI_output(get_tx(35));
			}
		} else {
			GUI_output(get_tx(34));
		}
	}
}


#if !defined(__BORLANDC__)
}
#endif
