/**
 *	Rewrite of DSA1 v3.02_de functions of seg082 (dungeon: mageruin)
 *	Functions rewritten: 2/2 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg082.cpp
 */

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg032.h"
#include "seg047.h"
#include "seg075.h"
#include "seg076.h"
#include "seg092.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG07_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;
	signed short spell_result;
	signed short lockpick_pos;
	signed short skill_result;

	amap_ptr = p_datseg + DNG_MAP;
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	hero = Real2Host(get_first_hero_available_in_group());

	if (target_pos == 0xd02 && target_pos != ds_readws(0x330e))
	{
		if (div16(host_readb(amap_ptr + 0x2d)) != 3)
		{
			do {
				i = GUI_radio(get_dtp(0x20), 3,
							get_dtp(0x24),
							get_dtp(0x28),
							get_dtp(0x2c));
			} while (i == -1);

			if (i == 1)
			{
				if ((lockpick_pos = hero_has_lockpicks(hero)) != -1)
				{
					if (lockpick_pos != -2)
					{
						skill_result = test_skill(hero, 48, 7);

						if (skill_result == -99) {

							print_msg_with_first_hero(get_ltx(0x854));
							or_ptr_bs(hero + HERO_ITEM_HEAD + 4 + 14 * lockpick_pos, 0x01);
							ds_writew((0xd325 + 0),
								ds_writew((0xd325 + 2),
								ds_writew((0xd325 + 4),
								ds_writew((0xd325 + 6), target_pos))));

							do_fight(FIGHTS_F100_03);

						} else if (skill_result <= 0) {

							print_msg_with_first_hero(get_ltx(0x850));
							ds_writew((0xd325 + 0),
								ds_writew((0xd325 + 2),
								ds_writew((0xd325 + 4),
								ds_writew((0xd325 + 6), target_pos))));

							do_fight(FIGHTS_F100_03);

						} else {

							and_ptr_bs(amap_ptr + 0x2d, 0x0f);
							or_ptr_bs(amap_ptr + 0x2d, 0x30);

							add_hero_ap(hero, 1L);
						}

					} else {
						print_msg_with_first_hero(get_ltx(0x84c));
					}
				} else {
					print_msg_with_first_hero(get_ltx(0x848));
				}

			} else if (i == 2)
			{

				hero = get_hero(select_hero_ok_forced(get_ltx(0x4f4)));

				if (host_readbs(hero + HERO_TYPE) < 7)
				{
					GUI_output(get_ltx(0x528));
				} else {
					spell_result = test_spell(hero, 28, 5);

					if (spell_result > 0)
					{
						sub_ae_splash(hero, get_spell_cost(28, 0));

						and_ptr_bs(amap_ptr + 0x2d, 0x0f);
						or_ptr_bs(amap_ptr + 0x2d, 0x30);

						add_hero_ap(hero, 1L);

					} else if (spell_result != -99)
					{

						sub_ae_splash(hero, get_spell_cost(28, 1));

						ds_writew((0xd325 + 0),
							ds_writew((0xd325 + 2),
							ds_writew((0xd325 + 4),
							ds_writew((0xd325 + 6), target_pos))));

						do_fight(FIGHTS_F100_03);
					} else
					{
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x97c),
							(char*)hero + HERO_NAME2);

						GUI_output(Real2Host(ds_readd(DTP2)));
					}
				}
			}

			ds_writew(X_TARGET, ds_readws(0x2d83));
			ds_writew(Y_TARGET, ds_readws(0x2d85));
			ds_writew(DNG_REFRESH_DIRECTION, -1);

		} else {

			inc_ds_ws(X_TARGET);
			DNG_inc_level();
		}

	} else if (target_pos == 0x1c03 && target_pos != ds_readws(0x330e) && ds_readb((TREASURE_MAPS + 1)) != 1)
	{
		GUI_output(get_dtp(0x04));

	} else if (target_pos == 0x1d02 && target_pos != ds_readws(0x330e))
	{

		inc_ds_ws(X_TARGET);

		DNG_dec_level();

	} else if (target_pos == 0x1601 && target_pos != ds_readws(0x330e) && ds_readb(DNG07_MUELIXIER_FLAG) != 2)
	{
		if (!ds_readb(DNG07_MUELIXIER_FLAG))
		{
			ds_writeb(DNG07_MUELIXIER_FLAG, 1);

			GUI_output(get_dtp(0x08));

			if (GUI_bool(get_dtp(0x0c)))
			{
				get_item(147, 1, 1);

				ds_writeb(DNG07_MUELIXIER_FLAG, 2);
			}

			/* ORIGINAL-BUG: forgot to set hero */
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readb(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					add_ptr_bs(hero + HERO_MU, 3);
					or_ptr_bs(hero + HERO_STATUS2, 0x80);
				}
			}
		} else {
			if (GUI_bool(get_dtp(0x14)))
			{
				get_item(147, 1, 1);

				ds_writeb(DNG07_MUELIXIER_FLAG, 2);
			}
		}

	} else if ((target_pos == 0x230d || target_pos == 0x220d) && target_pos != ds_readws(0x330e) && !ds_readb(DNG07_FLICKER_FLAG))
	{
		ds_writeb(DNG07_FLICKER_FLAG, 1);

		GUI_output(get_dtp(0x10));

	} else if (target_pos == 0x210b && target_pos != ds_readws(0x330e) && !ds_readb(DNG07_ANTIMUELIXIER_FLAG))
	{
		if (GUI_bool(get_dtp(0x14)))
		{
			get_item(226, 1, 1);

			ds_writeb(DNG07_ANTIMUELIXIER_FLAG, 1);
		}

	} else if (target_pos == 0x2a01 && target_pos != ds_readws(0x330e))
	{
		if (!do_fight(FIGHTS_F100_13))
		{
			if (ds_readb((TREASURE_MAPS + 1)) == 2)
			{
				ds_writeb(TMAP_DOUBLE2, 1);
			}

			ds_writeb((TREASURE_MAPS + 1), 1);

			add_hero_ap_all(10L);

			show_treasure_map();
		}

	} else if (target_pos == 0x2e0d && target_pos != ds_readws(0x330e))
	{
		ds_writew(X_TARGET, 7);

		ds_writeb(DIRECTION, (ds_readbs(DIRECTION) + 2) & 3);

	} else if (target_pos == 0x280d && target_pos != ds_readws(0x330e))
	{
		ds_writew(X_TARGET, 13);

		ds_writeb(DIRECTION, (ds_readbs(DIRECTION) + 2) & 3);

	} else if (target_pos == 0x2102 && target_pos != ds_readws(0x330e) && !ds_readb(DNG07_POISON_FLAG))
	{
		if (GUI_bool(get_dtp(0x18)))
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x1c),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			ds_writeb(DNG07_POISON_FLAG, 1);

			ds_writed(DNG07_POISON_TIMER, DAYS(1));
		}
	} else if (target_pos == 0x10e && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x3c)))
		{
			/* ORIGINAL-BUG: forgot to set hero */
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readb(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					hero_dummy6(hero))
				{
					sub_ptr_bs(hero + HERO_MU, 3);
					and_ptr_bs(hero + HERO_STATUS2, 0x7f);
				}
			}

			leave_dungeon();

			ds_writeb(CURRENT_TOWN, ds_readbs(TRV_DEST_REACHED));
			ds_writew(X_TARGET, ds_readws(0x433a));
			ds_writew(Y_TARGET, ds_readws(0x433c));
			ds_writeb(LOCATION, 0);
			ds_writeb(DIRECTION, (ds_readb(0x433e) + 2) & 3);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x38),
				(char*)get_ltx(4 * (ds_readws(TRV_DESTINATION) + 0xeb)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			timewarp(HOURS(3));

			ds_writeb(0x4475, 3);
		} else
		{
			ds_writew(Y_TARGET, ds_readws(0x2d85));
			ds_writew(X_TARGET, ds_readws(0x2d83));
		}

	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(0x330e, target_pos);

	return 0;
}

void DNG09_statues(signed short prob, signed short bonus)
{
	signed short i;
	signed short randval;
	Bit8u *hero;
	Bit8u *amap_ptr;

	hero = Real2Host(get_first_hero_available_in_group());

	amap_ptr = p_datseg + DNG_MAP;

	if (host_readbs(amap_ptr + 16 * ds_readws(Y_TARGET) + ds_readws(X_TARGET)) == 4)
	{
		/* TODO: no forced decision here ? */
		i = GUI_radio(get_dtp(0x10), 3,
				get_dtp(0x14),
				get_dtp(0x18),
				get_dtp(0x1c));

		if (i == 1)
		{
			/* praise the nameless god */
			if (random_schick(100) <= prob)
			{
				if (random_schick(100) < 50 &&
					!hero_dummy4(hero) &&
					!ds_readb(0x439f))
				{
					/* increase one attribute of the leader permanently */
					randval = random_schick(7) - 1;

					inc_ptr_bs(hero + HERO_MU_ORIG + 3 * randval);
					inc_ptr_bs(hero + HERO_MU + 3 * randval);

					/* ... but the twelfe won't grand miracles */
					or_ptr_bs(hero + 0xab, 0x20);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x20),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));
				} else {
					/* loose 1W6 LE */
					sub_hero_le(hero, random_schick(6));

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x24),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			}

		} else if (i == 2) {

			/* destroy the statue */

			/* remove the statue from the map */
			and_ptr_bs(amap_ptr + 16 * ds_readws(Y_TARGET) + ds_readws(X_TARGET), 0xfb);

			GUI_output(get_dtp(0x28));

			/* increase estimation */
			for (i = 0; i < 14; i++)
			{
				add_ds_ds((GODS_ESTIMATION + 4 * i), bonus);
			}

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readb(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					/* the twelfe will grant miracles again */
					and_ptr_bs(hero + HERO_STATUS2, 0xdf);
				}
			}

			/* all staues can't be used anymore */
			ds_writeb(0x439f, 1);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
