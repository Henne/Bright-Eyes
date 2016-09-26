/**
 *	Rewrite of DSA1 v3.02_de functions of seg081 (dungeon: cave2)
 *	Functions rewritten: 4/5
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg032.h"
#include "seg076.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
signed short DNG06_handler(void)
{
	register signed short i;
	signed short target_pos;
	signed short l3;
	signed short l4;
	signed short tw_bak;
	Bit32s p_money;
	Bit8u *hero;
	Bit8u *hero_first;
	Bit8u *hero_second;
	Bit8u *amap_ptr;

	amap_ptr = p_datseg + 0xbd95;
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	if (target_pos == 0xe05 && target_pos != ds_readws(0x330e) && !ds_readb(0x3cad))
	{
		if (GUI_bool(get_dtp(0x04)) && get_item(224, 1, 1))
		{
			ds_writeb(0x3cad, 1);
		}

	}

	if (target_pos == 0x909 && target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x8c));

	} else if (target_pos == 0x40d && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x08)))
		{
			strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(0x0c));

			if (!ds_readb(0x3ca9))
			{
				ds_writeb(0x3ca9, 1);

				hero = Real2Host(get_first_hero_available_in_group());

				i = dice_roll(3, 20, 0);

				add_party_money(100L * i);

				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					(char*)get_dtp(0x10),
					(char*)hero + HERO_NAME2,
					i);

				strcat((char*)Real2Host(ds_readd(DTP2)), (char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
			}

			GUI_output(Real2Host(ds_readd(DTP2)));
		}

	} else if (target_pos == 0x503 && target_pos != ds_readws(0x330e) && !ds_readb(0x3ca8))
	{
		if (GUI_bool(get_dtp(0x14)))
		{
			get_item(45, 1, 40);
			ds_writeb(0x3ca8, 1);
		}

	} else if (target_pos == 0x201 && target_pos != ds_readws(0x330e))
	{
		hero = get_hero(0);
		for (i = l3 = 0; i < 2; i++, hero += 0x6da)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
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
			(char*)(l3 == 1 ? p_datseg + 0x964f : p_datseg + 0x9657),
			(char*)hero_first + HERO_NAME2,
			(char*)hero_second + HERO_NAME2);

		strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(0x18));

		GUI_output(Real2Host(ds_readd(DTP2)));

		sub_hero_le(hero_first, random_schick(6));
		if (l3 > 1)
		{
			sub_hero_le(hero_second, random_schick(6));
		}

	} else if (target_pos == 0x907 && target_pos != ds_readws(0x330e) && !ds_readb(0x3ca7))
	{
		hero = get_hero(0);
		for (i = l3 = 0; i <= 6; i++, hero += 0x6da)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, 50, 5) > 0)
			{
				l3 = 1;
				break;
			}
		}

		if (l3 != 0)
		{
			if (GUI_bool(get_dtp(0x1c)))
			{
				GUI_output(get_dtp(0x20));

				hero = get_hero(0);
				for (i = l3 = 0; i <= 6; i++, hero += 0x6da)
				{
					if (host_readbs(hero + HERO_TYPE) != 0 &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero) &&
						test_skill(hero, 50, 5) > 0)
					{
						l3 = 1;
						break;
					}
				}

				if (l3 != 0 && GUI_bool(get_dtp(0x24)))
				{
					ds_writeb(0x3ca7, 1);

					GUI_output(get_dtp(0x28));
				}

				if (!ds_readb(0x3ca7))
				{
					DNG_fallpit_test(6);
				}
			} else {
				if (!ds_readb(0x3ca7))
				{
					DNG_fallpit_test(6);
				}
			}
		} else {
			if (!ds_readb(0x3ca7))
			{
				DNG_fallpit_test(6);
			}
		}

	} else if (target_pos == 0x807 && target_pos != ds_readws(0x330e) && ds_readb(0x3caa) != 2)
	{
		hero = get_hero(0);
		for (i = l3 = 0; i <= 6; i++, hero += 0x6da)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, 51, 10) > 0)
			{
				l3++;
			}
		}

		if (ds_readb(0x3caa) != 0 || l3 != 0)
		{
			ds_writeb(0x3caa, 1);

			if (GUI_bool(get_dtp(0x2c)))
			{
				ds_writeb(0x3caa, 2);

				get_item(195, 1, 1);

				/* TODO: This is not neccessary */
				hero = Real2Host(get_first_hero_available_in_group());

				add_party_money(3000L);
			}
		}

	} else if (target_pos == 0xb06 && target_pos != ds_readws(0x330e) && ds_readb(0x3cab) != 0)
	{
		if (div16(host_readb(amap_ptr + 0x1b)) != 1 ||
			div16(host_readb(amap_ptr + 0x62)) != 1 ||
			div16(host_readb(amap_ptr + 0x4b)) != 1)
		{
			dec_ds_bs_post(0x3cab);

			l3 = random_schick(5);

			hero = Real2Host(get_first_hero_available_in_group());

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)(l3 == 1 ? get_dtp(0x30) : get_dtp(0x34)),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, dice_roll(l3, 6, 0));
		}

	} else if (target_pos == 0x1d05 && target_pos != ds_readws(0x330e))
	{
		do {
			i = GUI_radio(get_dtp(0x38), 3,
					get_dtp(0x3c),
					get_dtp(0x40),
					get_dtp(0x44));
		} while (i == -1);

		hero = Real2Host(get_first_hero_available_in_group());

		if (i == 1 || i == 3)
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)(i == 1 ? get_dtp(0x48) : get_dtp(0x4c)),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 1)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, dice_roll(3, 6, 0));
		} else {

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += 0x6da)
			{
				if (host_readbs(hero + HERO_TYPE) != 0 &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero) &&
					test_skill(hero, 11, host_readbs(hero + HERO_RS_BONUS1)) <= 0)
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x50),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_hero_le(hero, dice_roll(3, 6, 0));
				}
			}
		}

	} else if (target_pos == 0x1102 && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x54)) && GUI_bool(get_dtp(0x58)))
		{
			for (i = l3 = 0; i < 6; i++)
			{
				if (ds_readws(GROUPS_X_TARGET + 2 * i) == 3 &&
					ds_readws(GROUPS_Y_TARGET + 2 * i) == 6 &&
					ds_readbs(CURRENT_GROUP) != i)
				{
					hero = get_hero(0);
					for (l4 = 0; l4 <= 6; l4++, hero += 0x6da)
					{
						if (host_readbs(hero + HERO_TYPE) != 0 &&
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
				GUI_output(get_dtp(0x60));

				ds_writeb(0x3cac, 1);
			} else {

				hero = Real2Host(get_first_hero_available_in_group());

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x5c),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_hero_le(hero, 2);
			}
		}

	} else if (target_pos == 0x1306 && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x54)) && GUI_bool(get_dtp(0x58)))
		{
			for (i = l3 = 0; i < 6; i++)
			{
				if (ds_readws(GROUPS_X_TARGET + 2 * i) == 1 &&
					ds_readws(GROUPS_Y_TARGET + 2 * i) == 2 &&
					ds_readbs(CURRENT_GROUP) != i)
				{
					hero = get_hero(0);
					for (l4 = 0; l4 <= 6; l4++, hero += 0x6da)
					{
						if (host_readbs(hero + HERO_TYPE) != 0 &&
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
				GUI_output(get_dtp(0x60));

				ds_writeb(0x3cac, 1);
			} else {

				hero = Real2Host(get_first_hero_available_in_group());

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x5c),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_hero_le(hero, 2);
			}
		}

	} else if (target_pos == 0x1508 && target_pos != ds_readws(0x330e) && !ds_readb(0x3cac))
	{
		hero = Real2Host(get_first_hero_available_in_group());

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x64),
			(char*)hero + HERO_NAME2);

		GUI_output(Real2Host(ds_readd(DTP2)));

		if (test_skill(hero, 11, 0) > 0 && test_skill(hero, 50, 0) > 0)
		{

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x68),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			GUI_output(Real2Host(ds_readd(DTP2)));

		} else {
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x6c),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, dice_roll(3, 6, 0));
		}

	} else if (target_pos == 0x170d && target_pos != ds_readws(0x330e) && !ds_readb(0x3cae))
	{
		if (GUI_bool(get_dtp(0x70)))
		{
			hero = Real2Host(get_first_hero_available_in_group());

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x74),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, 2);
		}

	} else if (target_pos == 0x160b && target_pos != ds_readws(0x330e) && !ds_readb(0x35f7))
	{
		GUI_output(get_dtp(0x90));

		p_money = get_party_money();
		p_money += 50000L;
		set_party_money(p_money);

		loot_multi_chest(p_datseg + 0x40cf, get_dtp(0x94));

		ds_writeb(0x35f7, 1);

		add_hero_ap_all(30);

	} else if (target_pos == 0x180e && target_pos != ds_readws(0x330e))
	{
		ds_writew(0xd325, ds_writew((0xd325 + 6), 0x163e));
		ds_writew((0xd325 + 4), ds_writew((0xd325 + 2), 0x1d0d));

		if (!do_fight(85))
		{
			ds_writeb(0x3cae, 1);
		}

	} else if (target_pos == 0xd0f && target_pos != ds_readws(0x330e))
	{
		leave_dungeon();

		ds_writeb(CURRENT_TOWN, ds_readbs(0x4338));
		ds_writew(X_TARGET, ds_readws(0x433a));
		ds_writew(Y_TARGET, ds_readws(0x433c));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readb(0x433e) + 2) & 3);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x98),
			(char*)get_ltx(4 * (ds_readws(0x434a) + 0xeb)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(2));

		ds_writeb(0x4475, 3);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(0x330e, target_pos);

	return 0;
}


/**
 * \brief print introductions of dungeon #6 fights
 *
 * \param fight_id	ID of the fight
 */
/* Borlandified and identical */
void DNG06_fight_intro(signed short fight_id)
{
	if (fight_id == FIGHTS_F094_22)
	{
		/* this is true all the time */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x78),
			(char*)(!ds_readb(0x3617) ? get_dtp(0x80) : get_dtp(0x7c)));

		GUI_output(Real2Host(ds_readd(DTP2)));
	}
}


/* Borlandified and identical */
void DNG06_chest1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x40bf));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}


/* Borlandified and identical */
void DNG06_chest2(RealPt chest)
{
	signed short i;
	Bit8u *hero;

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x84));

	hero = get_hero(0);
	for (i = 0; i <=6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) != 0 &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_skill(hero, 38, 0) > 0)
		{
			strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(0x88));
			break;
		}
	}

	GUI_output(Real2Host(ds_readd(DTP2)));
}


#if !defined(__BORLANDC__)
}
#endif
