/**
 *	Rewrite of DSA1 v3.02_de functions of seg078 (dungeon: inn)
 *	Functions rewritten: 2/10
 */

#include "v302de.h"

#include <stdio.h>
#include <string.h>

#include "seg002.h"
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

/* Borlandified and identical */
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

	target_pos = (ds_readbs(DUNGEON_LEVEL) << 12)
			+ (ds_readws(X_TARGET) << 8)
			+ ds_readws(Y_TARGET);

	amap_ptr = p_datseg + 0xbd95;

	if ((target_pos == 0x102 || target_pos == 0x201))
	{
		if (ds_readds(DAY_TIMER) >= HOURS(8) && ds_readds(DAY_TIMER) <= HOURS(20))
		{
		} else {

			ds_writew((0xd325 + 0),
				ds_writew((0xd325 + 2),
				ds_writew((0xd325 + 4),
				ds_writew((0xd325 + 6), 0x206))));

			do_fight(56);
		}

	} else if (target_pos == 0x306 && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 2)
	{
		strcpy((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x10));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			 (char*)((test_spell_group(42, 0) == 0) ? get_dtp(0x14) : get_dtp(0x18)));

		GUI_output(Real2Host(ds_readd(DTP2)));

	} else if (target_pos == 0x40a && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 2)
	{
		GUI_output(get_dtp(0x1c));

	} else if ((target_pos == 0x70d || target_pos == 0x1605 || target_pos == 0x140e) && target_pos != ds_readws(0x330e))
	{
		if (test_skill((hero = Real2Host(get_first_hero_available_in_group())), 51, 6) > 0)
		{
			GUI_output(get_dtp(0x20));

		} else {
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x24),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			flag = 0;

			while (host_readws(hero + HERO_LE) > 10 && !flag)
			{
				/* KK+4 */
				if (test_attrib(hero, 6, 4) <= 0)
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x28),
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
					(char*)get_dtp(0x2c),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_hero_le(hero, (host_readws(hero + HERO_LE) > 6 ? random_schick(6) : host_readws(hero + HERO_LE) - 1));

			}
		}

	} else if (target_pos == 0x60a)
	{
		DNG_fallpit_test(2);

	} else if (target_pos == 0xe0e && target_pos != ds_readws(0x330e))
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

	} else if ((target_pos == 0xa0c || target_pos == 0xa0a) && target_pos != ds_readws(0x330e))
	{
		hero = get_hero(0);
		for (i = weight_sum = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
			{
				weight_sum += get_hero_weight(hero);
			}
		}

		for (i = 0; i < 6; i++)
		{
			if (ds_readws(GROUPS_X_TARGET + 2 * i) == 0x0a &&
				ds_readws(GROUPS_Y_TARGET + 2 * i) == 0x0b &&
				ds_readbs(GROUPS_DNG_LEVEL + i) == ds_readbs(DUNGEON_LEVEL) &&
				ds_readbs(GROUPS_DNG_INDEX + i) == ds_readbs(DUNGEON_INDEX))
			{
				weight_sum = 0L;
			}
		}

		hero_weight = get_hero_weight(Real2Host(get_heaviest_hero()));

		host_writebs(amap_ptr + 0xba, (hero_weight >= weight_sum ? 0 : 0xf0));

		play_voc(305);

	} else if ((target_pos == 0xa0d || target_pos == 0xa09) && target_pos != ds_readws(0x330e))
	{
		host_writeb(amap_ptr + 0xba, 0);

		play_voc(305);

	} else if ((target_pos == 0x706 || target_pos == 0x806 || target_pos == 0x808))
	{
		if (ds_readb(SPHERE_ACTIVE) != 0)
		{
			GUI_output(get_dtp(0xa4));

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != 0 &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					hero_disappear(hero, i, -1);
				}
			}

		} else {
			hero = get_hero(0);
			for (i = mod_slot = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != 0 &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
					!hero_dead(hero) &&
					!hero_dummy3(hero))
				{
					mod_slot = get_free_mod_slot();
					set_mod_slot(mod_slot, HOURS(5), hero + HERO_MU, -3, (signed char)i);
					{
						hero_dummy3_set(hero, (mod_slot = 1));
					}
				}
			}

			if (mod_slot != 0)
			{
				GUI_output(get_dtp(0x30));
			}
		}

	} else if (target_pos == 0x701 && !ds_readb(0x3c99) && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x34)))
		{
			/* destroy the raven */
			ds_writeb(0x3c99, 1);

			sub_group_le(random_schick(20));

			GUI_output(get_dtp(0x38));

			host_writebs(amap_ptr + 0x25, 0);
		}

	} else if (target_pos == 0xb06 && target_pos != ds_readws(0x330e))
	{
		hero = get_hero(0);
		for (i = weight_sum = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
			{
				weight_sum += get_hero_weight(hero);
			}
		}

		if (weight_sum >= 4000)
		{
			GUI_output(get_dtp(0x3c));

			host_writeb(amap_ptr + 0x8b, 0);
		}

	} else if ((target_pos == 0xa06 || target_pos == 0xb05) && target_pos != ds_readws(0x330e))
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
					if (host_readbs(hero + HERO_TYPE) != 0 &&
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

		host_writeb(amap_ptr + 0x8b, (weight_sum >= 4000L ? 0x00 : 0xf0));

	} else if (target_pos == 0x1c05 && ds_readbs(DIRECTION) == 0 &&
			 (target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)))
	{
		ds_writeb(0x2d7c, ds_readbs(DIRECTION));

		GUI_output(get_dtp(0x40));

	} else if (target_pos == 0x1405 && ds_readbs(DIRECTION) == 2 &&
			 (target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)))
	{
		ds_writeb(0x2d7c, ds_readbs(DIRECTION));

		GUI_output(get_dtp(0x44));

	} else if (target_pos == 0x1701 && !ds_readb(0x3c94))
	{
		load_in_head(43);

		GUI_dialogbox(ds_readfp(DTP2), NULL, get_dtp(0x48), 0);

		ds_writeb(SPHERE_TIMER, 7);
		ds_writeb(0x3c94, 1);

	} else if ((target_pos == 0x110b || target_pos == 0x1108) && target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x58));

	} else if (target_pos == 0x1307 && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x5c)))
		{
			GUI_output(get_dtp(0x60));
		}

	} else if (target_pos == 0x1809 && target_pos != ds_readws(0x330e) && !ds_readb(0x3c96))
	{
		GUI_output(get_dtp(0x64));

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				host_readbs(hero + HERO_MR) < 8)
			{
				or_ptr_bs(hero + HERO_STATUS1, 0x04);

				sprintf((char*)Real2Host(ds_readfp(DTP2)),
					(char*)get_dtp(0xa8),
					(char*)hero + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readfp(DTP2)));
			}
		}

		seg002_57f1();

	} else if (target_pos == 0x1d0b && target_pos != ds_readws(0x330e))
	{
		do {
			i = GUI_radio(get_dtp(0x68), 2,
					get_dtp(0x6c),
					get_dtp(0x70));
		} while (i == -1);

		if (i == 1)
		{
			DNG_dec_level();
			target_pos = 0xd0b;

			do {
				i = GUI_radio(get_dtp(0x74), 2,
						get_dtp(0x78),
						get_dtp(0x7c));
			} while (i == -1);

			if (i == 2)
			{
				DNG_inc_level();
				target_pos = 0x1d0b;
			}
		}

	} else if (target_pos == 0xd0b && target_pos != ds_readws(0x330e))
	{
		do {
			i = GUI_radio(get_dtp(0xac), 2,
					get_dtp(0x6c),
					get_dtp(0x70));
		} while (i == -1);

		if (i == 1)
		{
			DNG_inc_level();
			target_pos = 0x1d0b;

			do {
				i = GUI_radio(get_dtp(0x68), 2,
						get_dtp(0x6c),
						get_dtp(0x70));
			} while (i == -1);

			if (i == 1)
			{
				DNG_dec_level();
				target_pos = 0xd0b;
			}
		}

	} else if ((target_pos == 0xc0b || target_pos == 0xe0b || target_pos == 0xd0a || target_pos == 0xd0c) && target_pos != ds_readws(0x330e))
	{
		ds_writew((0xd325 + 0),
			ds_writew((0xd325 + 2),
			ds_writew((0xd325 + 4),
			ds_writew((0xd325 + 6), 0xc0b))));

		if (!do_fight(73))
		{
			ds_writeb(0x35f3, 1);

			add_hero_ap_all(30);
		}

	} else if (target_pos == 0x1101 && target_pos != ds_readws(0x330e))
	{
		seg092_06b4(0);

	} else if (target_pos == 0x190e && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 1)
	{
		GUI_output(get_dtp(0x8c));

	} else if (target_pos == 0x108 &&
			(target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)) &&
			ds_readbs(DIRECTION) == 2 &&
			ds_readb(0x3c9a) != 2)
	{
		/* Original-Bug: this should be the leader, not hero nr 0 */
		hero = get_hero(0);

		if (ds_readb(0x3c9a) != 0 || test_skill(hero, 51, 6) > 0)
		{
			ds_writeb(0x3c9a, 1);

			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_dtp(0x94),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
				(char*)((i = test_skill(hero, 48, 4)) > 0 ? get_dtp(0x9c) : get_dtp(0x98)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat((char*)Real2Host(ds_readfp(DTP2)),
				(char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readfp(DTP2)));

			if (i > 0)
			{
				and_ptr_bs(amap_ptr + 0x91, 0x0f);
				ds_writeb(0x3c9a, 2);
				DNG_update_pos();
			}

			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}

	} else if (target_pos == 0x408 &&
			(target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)) &&
			ds_readbs(DIRECTION) == 2 &&
			ds_readb(0x3c9b) != 2)
	{
		/* Original-Bug: this should be the leader, not hero nr 0 */
		hero = get_hero(0);

		if (ds_readb(0x3c9b) != 0 || test_skill(hero, 51, 2) > 0)
		{
			ds_writeb(0x3c9b, 1);

			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_dtp(0x94),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
				(char*)((i = test_skill(hero, 48, 2)) > 0 ? get_dtp(0x9c) : get_dtp(0x98)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat((char*)Real2Host(ds_readfp(DTP2)),
				(char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readfp(DTP2)));

			if (i > 0)
			{
				host_writeb(amap_ptr + 0x94, 0x00);
				ds_writeb(0x3c9b, 2);
				DNG_update_pos();
			}

			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}

	} else if (((target_pos == 0x1409 && ds_readbs(DIRECTION) == 1) ||
			(target_pos == 0x1609 && ds_readbs(DIRECTION) == 3)) &&
			(target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)) &&
			ds_readb(0x3c9c) != 2)
	{
		hero = Real2Host(get_first_hero_available_in_group());

		if (ds_readb(0x3c9c) != 0 || test_skill(hero, 51, 5) > 0)
		{
			ds_writeb(0x3c9c, 1);

			sprintf((char*)Real2Host(ds_readfp(DTP2)),
				(char*)get_dtp(0x94),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
				(char*)((i = test_skill(hero, 48, 4)) > 0 ? get_dtp(0x9c) : get_dtp(0x98)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat((char*)Real2Host(ds_readfp(DTP2)),
				(char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));

			if (i > 0)
			{
				host_writeb(amap_ptr + 0x95, 0x00);
				ds_writeb(0x3c9c, 2);
				DNG_update_pos();
			}

			i = random_schick(6) + 4;

			sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
				(char*)get_dtp(0xa0),
				(char*)hero + HERO_NAME2,
				i);

			strcat((char*)Real2Host(ds_readfp(DTP2)),
				(char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readfp(DTP2)));

			sub_hero_le(hero, i);

			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}

	} else if (target_pos == 0x100 && target_pos != ds_readws(0x330e))
	{
		leave_dungeon();
		ds_writebs(CURRENT_TOWN, (signed char)ds_readws(0x4338));
		ds_writews(X_TARGET, ds_readws(0x433a));
		ds_writews(Y_TARGET, ds_readws(0x433c));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readws(0x433e) + 2) & 3);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0xb0),
			(char*)get_ltx(4 * (ds_readws(0x434a) + 0xeb)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(3));

		ds_writeb(0x4475, 3);

	}

	ds_writews(0x330e, target_pos);

	return 0;
}

/**
 * \brief print introductions of dungeon #2 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG2_fight_intro(signed short fight_id)
{
	if (fight_id == 62) {
		GUI_output(get_dtp(0x80));
	} else if (fight_id == 66) {
		GUI_output(get_dtp(0x84));
	} else if (fight_id == 68) {
		GUI_output(get_dtp(0x88));
	} else if (fight_id == 72) {
		GUI_output(get_dtp(0x90));
	}
}

#if !defined(__BORLANDC__)
}
#endif
