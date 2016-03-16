/**
 *	Rewrite of DSA1 v3.02_de functions of seg110 (travel events 2 / 10)
 *	Functions rewritten: 24/35
 */

#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg003.h"
#include "seg007.h"
#include "seg026.h"
#include "seg032.h"
#include "seg047.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#include "seg110.h"
#include "seg117.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void tevent_011(void)
{
	signed short answer;
	Bit8u *hero;

	if ((test_skill((hero = Real2Host(get_first_hero_available_in_group())), 51, 8) > 0 && !ds_readb(0x3da6)) ||
		ds_readb(0x3da6) != 0)
	{
		ds_writeb(0x3da6, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0x60),
			(char*)hero + HERO_NAME2,
			(char*)hero + HERO_NAME2);

		do {
			answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
						get_city(0x64),
						get_city(0x68));
		} while (answer == -1);

		if (answer == 2)
		{
			do {
				answer = GUI_radio(get_city(0x6c), 3,
							get_city(0x70),
							get_city(0x74),
							get_city(0x78));
			} while (answer == -1);

			if (answer == 1)
			{
				/* just swim */
				TRV_swim2(0, 5);
				TRV_a_path();

			} else if (answer == 2)
			{
				/* build a raft */
				timewarp(HOURS(1));
				TRV_swim2(0, 0);
				TRV_a_path();
			}
		}
	}
}

/**
 * \brief	all members of the current group try to swim
 * \param	mod	modificator for the swim test
 * \param	percent probability to loose an item
 *
 * In seg112.cpp exists a similiar function called TRV_swim() for that purpose.
 */
/* should be static */
/* Borlandified and identical */
void TRV_swim2(signed char mod, signed short percent)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) != 0 &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero))
		{
			/* a swim test */
			if (test_skill(hero, 14, (signed char)mod) > 0)
			{
				/* skill test succeeded */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x7c),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			} else {
				/* skill test failed */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x80),
					(char*)hero + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

				GUI_output(Real2Host(ds_readd(DTP2)));

				hero_disease_test(hero, 2, 20 - (host_readbs(hero + HERO_KK) + host_readbs(hero + HERO_KK_MOD)));

				if (percent)
				{
					loose_random_item(hero, percent, get_ltx(0x7e8));
				}

				sub_hero_le(hero, random_schick(5));
			}
		}
	}

}

/* a path between SERSKE and PEILINEN */
/* Borlandified and identical */
/* should be static */
void TRV_a_path(void)
{
	signed short answer;

	do {
		answer = GUI_radio(get_city(0x84), 2,
					get_city(0x88),
					get_city(0x8c));
	} while (answer == -1);

	if (answer == 1)
	{
		/* follow the path */
		ds_writeb(0xe4a2, (ds_readb(CURRENT_TOWN) == 4 ? 2 : 4));
		ds_writeb(0x4333, 1);
	} else {
		/* swim back */

		do {
			answer = GUI_radio(get_city(0x90), 2,
						get_city(0x70),
						get_city(0x74));
		} while (answer == -1);

		if (answer == 1)
		{
			/* just swim */
			TRV_swim2(0, 5);

		/* TODO: this test is bogus, just an else case */
		} else if (answer == 2)
		{
			/* build a raft */
			timewarp(HOURS(1));
			TRV_swim2(0, 0);
		}

		GUI_output(get_city(0x94));
	}
}

/* Borlandified and identical */
void tevent_013(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 29, 3) > 0 && !ds_readb(0x3da7)) ||
		ds_readb(0x3da7) != 0)
	{
		ds_writeb(0x66d0, 123);
		TRV_found_herb_place(0);
		ds_writeb(0x66d0, -1);
		ds_writeb(0x3da7, 1);
	}
}

/* Borlandified and identical */
void tevent_014(void)
{
	signed short answer;

	if (!ds_readb(0x3da8))
	{
		load_in_head(55);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						get_city(0xac), 2,
						get_city(0xb0),
						get_city(0xb4));
		} while (answer == -1);

		if (answer == 1)
		{
			/* examine the corpse */
			loot_corpse((RealPt)RealMake(datseg, 0xb13e), get_city(0xb8), p_datseg + 0x3da8);
		}
	}
}

/* Borlandified and identical */
void tevent_014_chest(RealPt chest)
{
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x3e27));

	loot_chest(Real2Host(chest), get_city(0xbc), get_city(0xc0));
}

/* Borlandified and identical */
void tevent_015(void)
{
	TRV_found_inn(44, 67);
}

/* Borlandified and identical */
void tevent_017(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 2) > 0 && !ds_readb(0x3daa)) ||
		ds_readb(0x3daa) != 0)
	{
		TRV_found_replenish_place(0);
		ds_writeb(0x3daa, 1);
	}
}

/* Borlandified and identical */
void tevent_020(void)
{
	signed short answer;
	Bit8u *hero;

	if ((test_skill((hero = Real2Host(get_first_hero_available_in_group())), 51, 7) > 0 && !ds_readb(0x3dab)) ||
		ds_readb(0x3dab) != 0)
	{
		ds_writeb(0x3dab, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0xc4),
			(char*)hero + HERO_NAME2);

		do {
			answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
						get_city(0xc8),
						get_city(0xcc));
		} while (answer == -1);

		if (answer == 1)
		{
			do {
				answer = GUI_radio(get_city(0xd0), 2,
							get_city(0xd4),
							get_city(0xd8));
			} while (answer == -1);

			if (answer == 1)
			{
				ds_writeb(0xe4a2, (ds_readb(CURRENT_TOWN) == 4 ? 1 : 3));
				ds_writeb(0x4333, 1);
			}
		}

		timewarp(MINUTES(15));
	}
}

/* Borlandified and identical */
void tevent_021(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 29, 8) > 0 && !ds_readb(0x3dac)) ||
		ds_readb(0x3dac) != 0)
	{
		ds_writeb(0x66d0, 126);
		TRV_found_herb_place(1);
		ds_writeb(0x66d0, -1);
		ds_writeb(0x3dac, 1);
	}
}

/* Borlandified and identical */
void tevent_022(void)
{
	signed short answer;

	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 0) > 0 && !ds_readb(0x3dad)) ||
		ds_readb(0x3dad) != 0)
	{
		ds_writeb(0x3dad, 1);
		if (TRV_found_camp_place(0))
		{
			if ((test_skill(Real2Host(get_first_hero_available_in_group()), 26, 5) > 0 && !ds_readb(0x3dae)) ||
				ds_readb(0x3dae) != 0)
			{
				do {
					answer = GUI_radio(get_city(0xec), 2,
								get_city(0xf0),
								get_city(0xf4));
				} while (answer == -1);

				if (answer == 1)
				{
					TRV_hunt_generic(25, 62, -2, 2, 8, 3, 7, 2, 4, 30, 6);
					ds_writeb(0x3dae, 1);
				}
			}
		}
	}
}

/* Borlandified and identical */
void tevent_024(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 2) > 0 && !ds_readb(0x3daf)) ||
		ds_readb(0x3daf) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(0x3daf, 1);
	}
}

/* Borlandified and identical */
void tevent_025(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 1) > 0 && !ds_readb(0x3db0)) ||
		ds_readb(0x3db0) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(0x3db0, 1);
	}
}

/* Borlandified and identical */
void tevent_026(void)
{
	TRV_cross_a_ford(get_city(0x120), 60, 0);
}

/* Borlandified and identical */
void tevent_027(void)
{
	TRV_cross_a_ford(get_city(0x124), 30, 0);
}

/* Borlandified and identical */
void tevent_028(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 2) > 0 && !ds_readb(0x3db1)) ||
		ds_readb(0x3db1) != 0)
	{
		TRV_found_camp_place(1);
		ds_writeb(0x3db1, 1);
	}
}

/* Borlandified and identical */
void tevent_029(void)
{
	signed short i;
	signed short item_pos;
	Bit8u *hero;

	load_in_head(51);

	GUI_dialog_na(0, get_city(0x128));
	GUI_dialog_na(0, get_city(0x12c));

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		hero_disease_test(hero, 2, 20 - (host_readbs(hero + HERO_KK) + host_readbs(hero + HERO_KK_MOD)));
	}

	if (test_skill(Real2Host(get_first_hero_available_in_group()), 28, 3) > 0)
	{
		/* skill test succeeded */
		timewarp(HOURS(3));

		GUI_dialog_na(0, get_city(0x130));
	} else {
		/* skill test failed */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero))
			{
				sub_hero_le(hero, 2);

				item_pos = get_item_pos(hero, 45);

				if (item_pos != -1)
				{
					/* hero looses the first set of FOOD PACKAGES */
					drop_item(hero, item_pos, host_readws(hero + HERO_ITEM_HEAD + 2 + 14 * item_pos));
				}

				item_pos = hero_count_item(hero, 30);

				if (item_pos)
				{
					/* hero looses the first WATERSKIN */
					drop_item(hero, get_item_pos(hero, 30), item_pos - 1);
				}
			}
		}

		timewarp(HOURS(5));

		GUI_dialog_na(0, get_city(0x134));
	}
}

/* Borlandified and identical */
void tevent_030(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 0) > 0 && !ds_readb(0x3db2)) ||
		ds_readb(0x3db2) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(0x3db2, 1);
	}
}

/* Borlandified and identical */
void tevent_031(void)
{
	signed short answer;

	if (ds_readb(FIND_HYGGELIK))
	{
		search_ruin1();

	} else if (!ds_readb(0x3db3))
	{
		load_in_head(49);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						get_city(0x00), 2,
						get_city(0x04),
						get_city(0x08));
		} while (answer == -1);

		if (answer == 2)
		{
			if (test_skill(Real2Host(get_first_hero_available_in_group()), 37, 7) <= 0)
			{
				/* skill test failed */
				ds_writeb(FIG_INITIATIVE, 1);
				do_fight(FIGHTS_F031);
			}
		} else {
			do_fight(FIGHTS_F031);
		}

		ds_writeb(0x3db3, 1);
	}
}

/* Borlandified and identical */
void tevent_032(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 2) > 0 && !ds_readb(0x3db4)) ||
		ds_readb(0x3db4) != 0)
	{
		ds_writeb(0x3db4, 1);
		if ((test_skill(Real2Host(get_first_hero_available_in_group()), 29, 5) > 0 && !ds_readb(0x3db5)) ||
			ds_readb(0x3db5) != 0)
		{
			ds_writeb(0x3db5, 1);

			ds_writeb(0x66d0, 62);
			TRV_found_camp_place(2);
			ds_writeb(0x66d0, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

/* Borlandified and identical */
void tevent_033(void)
{
	signed short answer;

	load_in_head(9);

	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
					get_city(0x0c), 3,
					get_city(0x10),
					get_city(0x14),
					get_city(0x18));
	} while (answer == -1);

	if (answer == 1 || answer == 2)
	{
		timewarp(MINUTES(15));

		GUI_dialog_na(0, (Bit8u*)(answer == 1 ? get_city(0x1c) : get_city(0x20)));
	}
}

/* Borlandified and identical */
void tevent_034(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 2) > 0 && !ds_readb(0x3db6)) ||
		ds_readb(0x3db6) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(0x3db6, 1);
	}
}

/* Borlandified and identical */
void tevent_035(void)
{
	signed short answer;

	if (!ds_readb(0x3db7))
	{
		do {
			answer = GUI_radio(get_city(0x24), 2,
						get_city(0x28),
						get_city(0x2c));
		} while (answer == -1);

		if (answer == 2)
		{
			if (test_skill(Real2Host(get_first_hero_available_in_group()), 37, 8) <= 0)
			{
				/* skill test failed */
				ds_writeb(FIG_INITIATIVE, 1);
				do_fight(FIGHTS_F035);
			}
		} else {
			do_fight(FIGHTS_F035);
		}

		ds_writeb(0x3db7, 1);
	}
}

/* Borlandified and identical */
void tevent_036(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 3) > 0 && !ds_readb(0x3db8)) ||
		ds_readb(0x3db8) != 0)
	{
		ds_writeb(0x3db8, 1);

		if ((test_skill(Real2Host(get_first_hero_available_in_group()), 29, 6) > 0 && !ds_readb(0x3db9)) ||
			ds_readb(0x3db9) != 0)
		{
			ds_writeb(0x3db9, 1);

			ds_writeb(0x66d0, 63);
			TRV_found_camp_place(2);
			ds_writeb(0x66d0, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
