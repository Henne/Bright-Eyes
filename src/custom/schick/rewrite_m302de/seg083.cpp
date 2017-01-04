/**
 *	Rewrite of DSA1 v3.02_de functions of seg083 (dungeon: orc lair)
 *	Functions rewritten: 12/12 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg083.cpp
 */

#include "v302de.h"

#include <stdio.h>
#include <string.h>

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg032.h"
#include "seg047.h"
#include "seg049.h"
#include "seg075.h"
#include "seg076.h"
#include "seg083.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
#include "t_map.h"
#endif

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG08_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short tmp;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;

	amap_ptr = p_datseg + DNG_MAP;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	hero = Real2Host(get_first_hero_available_in_group());

	if (target_pos == 0x10a && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_00))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_00, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0x109 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_01))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_01, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0x108 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_02))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_02, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0x107 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_03))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_03, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0x106 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_04))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_04, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0x606 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_05))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_05, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0x60a && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_06))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_06, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0x707 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_07))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_07, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0x701 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_08))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_08, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0xb01 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_09))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_09, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0x704 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_10))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_10, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0xa05 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BED_11))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			ds_writeb(DNG08_BED_11, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == 0x805 && target_pos != ds_readws(0x330e) && ds_readb(DNG08_WATERBARREL))
	{
		DNG_waterbarrel(p_datseg + DNG08_WATERBARREL);

	} else 	if (target_pos == 0x306 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_TIMER1))
	{
		if (GUI_bool(get_dtp(0x14)))
		{
			GUI_output(get_dtp(0x18));

			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					ds_writeb(DNG08_TIMER1, 24);

					host_writebs(hero + HERO_HUNGER,
						host_writebs(hero + HERO_THIRST, 0));

					add_hero_le(hero, 2);

					tmp = get_free_mod_slot();

					set_mod_slot(tmp, DAYS(1), get_hero(i) + (HERO_ATTRIB + 3 * ATTRIB_CH), -1, (signed char)i);
				}
			}
		}

	} else if (target_pos == 0x809 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_TIMER2))
	{
		if (GUI_bool(get_dtp(0x14)))
		{
			GUI_output(get_dtp(0x18));

			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					ds_writeb(DNG08_TIMER2, 24);

					host_writebs(hero + HERO_HUNGER,
						host_writebs(hero + HERO_THIRST, 0));

					add_hero_le(hero, 2);

					tmp = get_free_mod_slot();

					set_mod_slot(tmp, DAYS(1), get_hero(i) + (HERO_ATTRIB + 3 * ATTRIB_CH), -1, (signed char)i);
				}
			}
		}

	} else if (target_pos == 0x101 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BALLISTA_DONE))
	{
		if (GUI_bool(get_dtp(0x1c)))
		{
			ds_writeb(DNG08_BALLISTA_DONE, 1);

			GUI_output(get_dtp(0x20));

			add_hero_ap_all(10);

			ds_writew((0xd325 + 0), ds_writew((0xd325 + 4), ds_writew((0xd325 + 6), 0x227)));
			ds_writew((0xd325 + 2), 0x613);

			do_fight(FIGHTS_F108_3B);
		}

	} else if (target_pos == 0x104 && target_pos != ds_readws(0x330e) && !ds_readb(DNG08_BUNDLES_DONE))
	{
		if (GUI_bool(get_dtp(0x24)))
		{
			ds_writeb(DNG08_BUNDLES_DONE, 1);

			GUI_output(get_dtp(0x28));

			add_hero_ap_all(10);

			ds_writew((0xd325 + 0), ds_writew((0xd325 + 4), ds_writew((0xd325 + 6), 0x227)));
			ds_writew((0xd325 + 2), 0x613);

			do_fight(FIGHTS_F108_3B);
		}


	} else if (target_pos == 0xb0e && target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x30));

	} else if (target_pos == 0xb0b &&
			 (target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)) &&
			ds_readbs(DIRECTION) == 3)
	{
		do {
			i = GUI_radio(get_dtp(0x34), 3,
					get_dtp(0x38),
					get_dtp(0x3c),
					get_dtp(0x40));
		} while (i == -1);

		if (i == 2)
		{
			GUI_output(get_dtp(0x44));

		} else if (i == 3)
		{
			hero = get_hero(select_hero_ok_forced(get_dtp(0x48)));

			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_dtp(0x4c),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readfp(DTP2)));

			and_ptr_bs(amap_ptr + 0xd8, 0x0f);
			or_ptr_bs(amap_ptr + 0xd8, 0x20);

			if (ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) > 1)
			{
				ds_writeb(0x2d7c, ds_readbs(DIRECTION));

				for (tmp = 0; ds_readbs(GROUP_MEMBER_COUNTS + tmp) != 0; tmp++);

				host_writebs(hero + HERO_GROUP_NO, (signed char)tmp);
				inc_ds_bs_post(GROUP_MEMBER_COUNTS + tmp);
				dec_ds_bs_post(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));

				GRP_save_pos(tmp);
			}

		} else {
			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}

	} else if ((target_pos == 0xb0a || target_pos == 0xb0c) && target_pos != ds_readws(0x330e))
	{
		for (i = tmp = 0; i < 6; i++)
		{
			if (ds_readws(GROUPS_X_TARGET + 2 * i) == 0x0b &&
				ds_readws(GROUPS_Y_TARGET + 2 * i) == 0x0b &&
				ds_readbs(CURRENT_GROUP) != i)
			{
				tmp = 1;
			}
		}

		if (tmp == 0)
		{
			and_ptr_bs(amap_ptr + 0xd8, 0x0f);
			or_ptr_bs(amap_ptr + 0xd8, 0x10);
		}

	} else if (target_pos == 0xa0c && target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x50));

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, TA_KLETTERN, 2) <= 0)
			{
				sprintf((char*)Real2Host(ds_readfp(DTP2)),
					(char*)get_dtp(0x54),
					(char*)hero + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));
				GUI_output(Real2Host(ds_readfp(DTP2)));

				sub_hero_le(hero, dice_roll(1, 6, 4));
			}
		}

	} else if (target_pos == 0x508 && target_pos != ds_readws(0x330e) &&
			ds_readbs(DIRECTION) != ds_readbs(0x2d7c) &&
			ds_readbs(DIRECTION) == 3 &&
			ds_readbs(DNG08_SECRET_DOOR) != 2)
	{
		if (ds_readbs(DNG08_SECRET_DOOR) != 0 ||
			test_skill((hero = Real2Host(get_first_hero_available_in_group())), 51, 1) > 0)
		{
			ds_writeb(DNG08_SECRET_DOOR, 1);

			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_dtp(0x74),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
				(char*)((tmp = test_skill(hero, TA_SCHLOESSER, 2)) > 0 ? get_dtp(0x78): get_dtp(0x7c)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat((char*)Real2Host(ds_readfp(DTP2)),
				(char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readfp(DTP2)));

			if (tmp > 0)
			{
				/* open the secret door */
				host_writeb(amap_ptr + 0x84, 0);
				ds_writeb(DNG08_SECRET_DOOR, 2);
				DNG_update_pos();
			}

			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}

	} else if (target_pos == 0x507 && target_pos != ds_readws(0x330e))
	{
		sprintf((char*)Real2Host(ds_readfp(DTP2)),
			(char*)get_dtp(0x58),
			(char*)(test_skill(hero, TA_GOETTER_KULTE, 4) <= 0 ? get_dtp(0x5c) : get_dtp(0x60)));

		GUI_output(Real2Host(ds_readfp(DTP2)));

	} else if (target_pos == 0x509 && target_pos != ds_readws(0x330e))
	{
		sprintf((char*)Real2Host(ds_readfp(DTP2)),
			(char*)get_dtp(0x58),
			(char*)(test_skill(hero, TA_GOETTER_KULTE, 6) <= 0 ? get_dtp(0x5c) : get_dtp(0x64)));

		GUI_output(Real2Host(ds_readfp(DTP2)));

	} else if (target_pos == 0xe && target_pos != ds_readws(0x330e))
	{
		leave_dungeon();

		ds_writebs(CURRENT_TOWN, (signed char)ds_readws(TRV_DEST_REACHED));
		ds_writews(X_TARGET, ds_readws(0x433a));
		ds_writews(Y_TARGET, ds_readws(0x433c));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readws(0x433e) + 2) & 3);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x84),
			(char*)get_ltx(4 * (ds_readws(TRV_DESTINATION) + 0xeb)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(3));

		ds_writeb(0x4475, 3);
	}


	ds_writew(TEXTBOX_WIDTH, tw_bak);

	ds_writew(0x330e, target_pos);

	return 0;

}

/* static */
void DNG08_search_bed(void)
{
	signed short counter;
	signed short money;
	signed short slot;
	Bit8u *hero;

	money = counter = 0;

	hero = Real2Host(get_first_hero_available_in_group());

	slot = get_free_mod_slot();

	set_mod_slot(slot, HOURS(6), hero + (HERO_ATTRIB + 3 * ATTRIB_CH), -2, (signed char)get_hero_index(hero));

	if (random_schick(100) <= 10)
	{
		sprintf((char*)Real2Host(ds_readfp(DTP2)),
			(char*)get_dtp(0x0c),
			(char*)hero + HERO_NAME2,
			(char*)Real2Host(GUI_names_grammar(2, 92, 0)));

		/* a BRANDY BOTTLE */
		get_item(92, 1, 1);

		GUI_output(Real2Host(ds_readfp(DTP2)));

		counter++;
	}

	if (random_schick(100) <= 10)
	{
		sprintf((char*)Real2Host(ds_readfp(DTP2)),
			(char*)get_dtp(0x0c),
			(char*)hero + HERO_NAME2,
			(char*)Real2Host(GUI_names_grammar(2, 14, 0)));

		/* a DAGGER */
		get_item(14, 1, 1);

		GUI_output(Real2Host(ds_readfp(DTP2)));

		counter++;
	}

	if (random_schick(100) <= 10)
	{
		money = random_schick(6);

		sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
			(char*)get_dtp(0x10),
			money);

		sprintf((char*)Real2Host(ds_readfp(DTP2)),
			(char*)get_dtp(0x0c),
			(char*)hero + HERO_NAME2,
			(char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));

		GUI_output(Real2Host(ds_readfp(DTP2)));

		add_party_money(money);

		counter++;
	}

	if (!counter)
	{
		GUI_output(get_dtp(0x08));
	}
}

void DNG08_chest1_func2(RealPt)
{
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());

	sprintf((char*)Real2Host(ds_readfp(DTP2)),
		(char*)get_dtp(0x2c),
		(char*)hero + HERO_NAME2,
		(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 1)));

	GUI_output(Real2Host(ds_readfp(DTP2)));

	/* 3W6 damage */
	sub_hero_le(hero, dice_roll(3, 6, 0));
}

void DNG08_chest0_func3(RealPt)
{
	if (!ds_readb(DNG08_CHEST0_LOOTED))
	{
		ds_writeb(DNG08_CHEST0_LOOTED, 1);

		GUI_output(get_dtp(0x80));

		/* get 40 FOOD PACKAGES */
		get_item(45, 1, 40);
	}
}

void DNG08_chest1_func3(RealPt)
{
	loot_multi_chest(p_datseg + DNG08_CHEST1_CONTENT, get_dtp(0x88));
}

void DNG08_chest2_func3(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG08_CHEST2_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG08_chest2_func1(RealPt chest)
{
	if (!ds_readb(DNG08_CHEST2_LOOTED))
	{
		if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_SPRACHEN, 2) > 0)
		{
			GUI_input(get_dtp(0x6c), 10);

			if (!strcmp((char*)Real2Host(ds_readfp(TEXT_INPUT_BUF)),
					(char*)p_datseg + DNG08_STR_TAIRACH))
			{
#if defined(__BORLANDC__)
			((void (*)(RealPt))((RealPt)host_readd(Real2Host(chest) + 11)))(chest);
#else
			t_map(chest, 11)(chest);
#endif

			} else if ((RealPt)host_readd(Real2Host(chest) + 7) != 0)
			{
#if defined(__BORLANDC__)
			((void (*)(void))((RealPt)host_readd(Real2Host(chest) + 7)))();
#else
			((treasure_trap)(t_map(chest, 7)))();
#endif
			}
		} else {
			GUI_output(get_dtp(0x68));
		}
	} else {
		GUI_input(get_ltx(0x828), 0);
	}
}

void DNG08_chest3_func1(RealPt chest)
{
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());

	if (get_first_hero_with_item(212) != -1 ||
		test_skill(hero, TA_SCHLOESSER, 5) > 0)
	{
		if (!(ds_readb(DNG08_CHEST35_LOOTED) & 1))
		{
			GUI_output(get_dtp(0x70));

			add_party_money(8530L);

			or_ds_bs(DNG08_CHEST35_LOOTED, 1);
		} else {
			GUI_output(get_ltx(0x828));
		}
	} else {
#if defined(__BORLANDC__)
			((void (*)(void))((RealPt)host_readd(Real2Host(chest) + 7)))();
#else
			((treasure_trap)(t_map(chest, 7)))();
#endif
	}
}

void DNG08_chest4_func1(RealPt chest)
{
	Bit8u *hero;
	hero = Real2Host(get_first_hero_available_in_group());

	if (get_first_hero_with_item(212) != -1 ||
		test_skill(hero, TA_SCHLOESSER, 5) > 0)
	{
#if defined(__BORLANDC__)
		((void (*)(RealPt))((RealPt)host_readd(Real2Host(chest) + 11)))(chest);
#else
		t_map(chest, 11)(chest);
#endif

	} else {
#if defined(__BORLANDC__)
		((void (*)(void))((RealPt)host_readd(Real2Host(chest) + 7)))();
#else
		((treasure_trap)(t_map(chest, 7)))();
#endif
	}
}

void DNG08_chest5_func1(RealPt chest)
{
	Bit8u *hero;
	hero = Real2Host(get_first_hero_available_in_group());

	if (get_first_hero_with_item(212) != -1 ||
		test_skill(hero, TA_SCHLOESSER, 5) > 0)
	{
#if defined(__BORLANDC__)
		((void (*)(RealPt))((RealPt)host_readd(Real2Host(chest) + 11)))(chest);
#else
		t_map(chest, 11)(chest);
#endif

	} else {
#if defined(__BORLANDC__)
		((void (*)(void))((RealPt)host_readd(Real2Host(chest) + 7)))();
#else
		((treasure_trap)(t_map(chest, 7)))();
#endif
	}
}

void DNG08_chest4_func3(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG08_CHEST4_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG08_chest5_func3(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG08_CHEST5_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);

	if (!(ds_readb(DNG08_CHEST35_LOOTED) & 2))
	{
		GUI_output(get_dtp(0x8c));

		add_party_money(2890L);

		or_ds_bs(DNG08_CHEST35_LOOTED, 2);
	}
}

#if !defined(__BORLANDC__)
}
#endif
