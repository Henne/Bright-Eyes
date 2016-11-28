/**
 *	Rewrite of DSA1 v3.02_de functions of seg085 (dungeon: cave4)
 *	Functions rewritten: 7/7 (complete)
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

/* Borlandified and identical */
signed short DNG10_handler(void)
{
	signed short target_pos;
	signed short answer;
	signed short result;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;
	Bit32s p_money;

	amap_ptr = p_datseg + 0xbd95;
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	hero = Real2Host(get_first_hero_available_in_group());

	if ((target_pos == 0x101 || target_pos == 0x302) && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x04)))
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x08),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, 2);
		}

	} else if (target_pos == 0x502 && target_pos != ds_readws(0x330e))
	{
		seg092_06b4(0);

	} else if (target_pos == 0x801 && target_pos != ds_readws(0x330e))
	{
		if ((ds_readb(0x4141) != 0) || test_skill(hero, 51, 7) > 0)
		{
			or_ds_bs(0x4141, 1);

			if (GUI_bool((!(ds_readb(0x4141) & 2) ? get_dtp(0x18) : get_dtp(0xa4))))
			{
				if (GUI_bool(get_dtp(0x1c)))
				{
					or_ds_bs(0x4141, 2);
					xor_ds_bs(0x4143, 1);

					GUI_output(get_dtp(0x20));
				}
			}
		}
	} else if ((target_pos == 0x804 && target_pos != ds_readws(0x330e) && !ds_readb(0x4143)) ||
			(target_pos == 0x704 && target_pos != ds_readws(0x330e) && ds_readb(0x4143) != 0))
	{
		hero = get_hero(get_random_hero());

		answer = dice_roll(3, 6, 4);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x24),
			(char*)hero + HERO_NAME2);

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
			(target_pos != ds_readws(0x330e) || ds_readb(DIRECTION) != ds_readb(0x2d7c)) &&
			ds_readbs(DIRECTION) == 3)
	{
		if (ds_readb(0x4142) != 0 || test_skill(hero, 51, 5) > 0)
		{
			/* Original-Bug: ??? */
			ds_writeb(0x4142, 1);

			if (ds_readb(0x4142) == 2 || GUI_bool(get_dtp(0x18)))
			{
				ds_writeb(0x4142, 2);

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

					add_ds_bs(0x4144, result);

					sub_hero_le(hero, result);

					if (ds_readb(0x4144) >= 7)
					{
						host_writebs(amap_ptr + 0x73, 0xbf);
					}
				}

				ds_writeb(0x2d7c, ds_readbs(DIRECTION));

			} else {
			}

			ds_writew(0x330e, target_pos);
		}
	} else if (target_pos == 0x108 && target_pos != ds_readws(0x330e))
	{
		if (ds_readb(0x4145) != 0 || test_skill(hero, 51, 5) > 0)
		{
			/* Original-Bug: ??? */
			ds_writeb(0x4145, 1);

			if (GUI_bool(get_dtp(0x34)) && test_skill(hero, 48, 7) <= 0)
			{
				if (ds_readb(0x4146) != 0)
				{

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x38),
						(char*)hero + HERO_NAME2);

					dec_ds_bs_post(0x4146);

					sub_hero_le(hero, dice_roll(3, 6, 0));
				} else {
					strcpy((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x3c));
				}

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}
	} else if (target_pos == 0x10c && target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x40));

	} else if (target_pos == 0x30e && target_pos != ds_readws(0x330e))
	{
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
	} else if (target_pos == 0x60c && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 2)
	{
		GUI_output(get_dtp(0x54));

	} else if (target_pos == 0xa0d && target_pos != ds_readws(0x330e) && !ds_readb(0x4148))
	{
		if (GUI_bool(get_dtp(0x58)))
		{
			ds_writew((0xd325 + 0), ds_writew((0xd325 + 2), ds_writew((0xd325 + 4), ds_writew((0xd325 + 6), 0xa0d))));
			ds_writew(0x330e, 0);

			if (!do_fight(253))
			{
				ds_writeb(0x4148, 1);
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
			target_pos != ds_readws(0x330e))
	{
		if(random_schick(100) <= 10)
		{
			GUI_output(get_dtp(0x60));

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != 0 &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					sub_hero_le(hero, random_schick(6));
				}
			}
		}
	} else if (target_pos == 0x1e02 && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x64)))
		{
			xor_ds_bs(0x4147, 1);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x68),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, 2);
		}
	} else if ((target_pos == 0x1a03 || target_pos == 0x1703) && target_pos != ds_readws(0x330e) && ds_readb(0x4147) != 0)
	{
			ds_writew((0xd325 + 0), ds_writew((0xd325 + 6), 0x1533));
			ds_writew((0xd325 + 2), ds_writew((0xd325 + 4), 0x1c13));
			ds_writew(FIG_DISCARD, 1);
			do_fight(111);

	} else if (target_pos == 0x110c && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 2)
	{
		GUI_output(get_dtp(0x6c));

	} else if (target_pos == 0x190c && target_pos != ds_readws(0x330e) && ds_readb(0x4149) != 0)
	{
			ds_writew((0xd325 + 0), ds_writew((0xd325 + 6), 0x190a));
			ds_writew((0xd325 + 2), ds_writew((0xd325 + 4), 0x190a));
			do_fight(117);

	} else if (target_pos == 0x2c0c && target_pos != ds_readws(0x330e))
	{
		if (!ds_readb(0x4149))
		{
			load_in_head(58);

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70),
						get_dtp(0x74), 2,
						get_dtp(0x78), get_dtp(0x7c));
			} while (answer == -1);

			if (answer == 1)
			{
				ds_writeb(0x4149, 1);

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
				hero = get_hero(0);

				for (answer = result = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != 0 &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero) &&
						test_skill(hero, 13, host_readbs(hero + HERO_RS_BONUS1) + 3) <= 0)
					{
						result++;
					}
				}

				if (result == 0 && !ds_readb(0x414a))
				{
					ds_writeb(0x414a, 1);

					GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x8c), 0);

					get_item(145, 1, 2);
					get_item(154, 1, 1);
					get_item(218, 1, 1);

					p_money = get_party_money();
					p_money += 20000L;
					set_party_money(p_money);

				} else {
					ds_writeb(0x4149, 1);

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

		} else if (ds_readb(0x4149) == 1) {

			load_in_head(58);

			ds_writew(X_TARGET, ds_writew(Y_TARGET, 12));

			if ((answer = get_first_hero_with_item(201)) != -1)
			{
				hero = get_hero(answer);
				result = get_item_pos(hero, 201);
				drop_item(hero, result, 1);

				GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x90), 0);
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x94), 0);

				get_item(145, 1, 2);
				get_item(154, 1, 1);
				get_item(218, 1, 1);
				get_item(70, 1, 1);

				p_money = get_party_money();
				p_money += 20000L;
				set_party_money(p_money);

				add_hero_ap_all(50);

				ds_writeb(0x4149, 2);
			} else {
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x70), get_dtp(0x98), 0);
			}
		}

	} else if (target_pos == 0x2005 && target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x9c));

	} else if (target_pos == 0x03 && target_pos != ds_readws(0x330e))
	{
		leave_dungeon();

		ds_writeb(CURRENT_TOWN, ds_readbs(0x4338));
		ds_writew(X_TARGET, ds_readws(0x433a));
		ds_writew(Y_TARGET, ds_readws(0x433c));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readbs(0x433e) + 2) & 0x03);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0xa0),
			(char*)get_ltx(4 * (ds_readw(0x434a) + 0xeb)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(3));

		ds_writeb(0x4475, 3);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(0x330e, target_pos);

	return 0;
}

/* Borlandified and identical */
void DNG10_chest0_x1(RealPt ptr)
{
	loot_corpse(ptr, get_dtp(0x0c), p_datseg + 0x4140);
}

/* Borlandified and identical */
void DNG10_chest0_x2(RealPt chest)
{
	RealPt ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x414b));
	loot_chest(Real2Host(chest), get_dtp(0x10), get_dtp(0x14));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/* Borlandified and identical */
void DNG10_chest1_x1(RealPt chest)
{
	RealPt ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x4150));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/* Borlandified and identical */
void DNG10_chest2_x1(RealPt chest)
{
	RealPt ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x4154));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/* Borlandified and identical */
void DNG10_chest3_x1(RealPt chest)
{
	RealPt ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x4158));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/* Borlandified and identical */
void DNG10_chest4_x1(RealPt chest)
{
	RealPt ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x415b));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

#if !defined(__BORLANDC__)
}
#endif
