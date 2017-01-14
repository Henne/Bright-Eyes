/**
 *	Rewrite of DSA1 v3.02_de functions of seg110 (travel events 2 / 10)
 *	Functions rewritten: 35/35 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg110.cpp
 */

#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg003.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
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

void tevent_011(void)
{
	signed short answer;
	Bit8u *hero;

	if ((test_skill((hero = Real2Host(get_first_hero_available_in_group())), 51, 8) > 0 && !ds_readb(TEVENT011_FLAG)) ||
		ds_readb(TEVENT011_FLAG) != 0)
	{
		ds_writeb(TEVENT011_FLAG, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx2(0x60),
			(char*)hero + HERO_NAME2,
			(char*)hero + HERO_NAME2);

		do {
			answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
						get_tx2(0x64),
						get_tx2(0x68));
		} while (answer == -1);

		if (answer == 2)
		{
			do {
				answer = GUI_radio(get_tx2(0x6c), 3,
							get_tx2(0x70),
							get_tx2(0x74),
							get_tx2(0x78));
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
 * \brief   all members of the current group try to swim
 *
 *          In seg112.cpp exists a similiar function called TRV_swim() for that purpose.
 *
 * \param   mod         modificator for the swim test
 * \param   percent     probability to loose an item
 */
/* should be static */
void TRV_swim2(signed char mod, signed short percent)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero))
		{
			/* a swim test */
			if (test_skill(hero, TA_SCHWIMMEN, (signed char)mod) > 0)
			{
				/* skill test succeeded */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx2(0x7c),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			} else {
				/* skill test failed */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx2(0x80),
					(char*)hero + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

				GUI_output(Real2Host(ds_readd(DTP2)));

				hero_disease_test(hero, 2, 20 - (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK))));

				if (percent)
				{
					loose_random_item(hero, percent, get_ttx(0x7e8));
				}

				sub_hero_le(hero, random_schick(5));
			}
		}
	}

}

/* Path east from the route Rovamund-Peilinen. */
/* should be static */
void TRV_a_path(void)
{
	signed short answer;

	do {
		answer = GUI_radio(get_tx2(0x84), 2,
					get_tx2(0x88),
					get_tx2(0x8c));
	} while (answer == -1);

	if (answer == 1)
	{
		/* follow the path */
		ds_writeb(ROUTE59_FLAG, (ds_readb(CURRENT_TOWN) == TOWNS_PEILINEN ? 2 : 4));
		ds_writeb(TRAVEL_DETOUR, 1);
	} else {
		/* swim back */

		do {
			answer = GUI_radio(get_tx2(0x90), 2,
						get_tx2(0x70),
						get_tx2(0x74));
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

		GUI_output(get_tx2(0x94));
	}
}

void tevent_013(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 3) > 0 && !ds_readb(TEVENT013_FLAG)) ||
		ds_readb(TEVENT013_FLAG) != 0)
	{
		ds_writeb(GATHER_HERBS_SPECIAL, 123);
		TRV_found_herb_place(0);
		ds_writeb(GATHER_HERBS_SPECIAL, -1);
		ds_writeb(TEVENT013_FLAG, 1);
	}
}

void tevent_014(void)
{
	signed short answer;

	if (!ds_readb(TEVENT014_FLAG))
	{
		load_in_head(55);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						get_tx2(0xac), 2,
						get_tx2(0xb0),
						get_tx2(0xb4));
		} while (answer == -1);

		if (answer == 1)
		{
			/* examine the corpse */
			loot_corpse((RealPt)RealMake(datseg, TEVENT014_CORPSE), get_tx2(0xb8), p_datseg + TEVENT014_FLAG);
		}
	}
}

void tevent_014_chest(RealPt chest)
{
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, TEVENT014_CHEST));

	loot_chest(Real2Host(chest), get_tx2(0xbc), get_tx2(0xc0));
}

void tevent_015(void)
{
	TRV_found_inn(44, 67);
}

void tevent_017(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT017_FLAG)) ||
		ds_readb(TEVENT017_FLAG) != 0)
	{
		TRV_found_replenish_place(0);
		ds_writeb(TEVENT017_FLAG, 1);
	}
}

/* Path west from the route Kravik-Skelellen. */
void tevent_020(void)
{
	signed short answer;
	Bit8u *hero;

	if ((test_skill((hero = Real2Host(get_first_hero_available_in_group())), TA_SINNESSCHAERFE, 7) > 0 && !ds_readb(TEVENT020_FLAG)) ||
		ds_readb(TEVENT020_FLAG) != 0)
	{
		ds_writeb(TEVENT020_FLAG, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx2(0xc4),
			(char*)hero + HERO_NAME2);

		do {
			answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
						get_tx2(0xc8),
						get_tx2(0xcc));
		} while (answer == -1);

		if (answer == 1)
		{
			do {
				answer = GUI_radio(get_tx2(0xd0), 2,
							get_tx2(0xd4),
							get_tx2(0xd8));
			} while (answer == -1);

			if (answer == 1)
			{
			    /* TODO: Original-Bug: CURRENT_TOWN is either Kravik or Skelellen. */
				ds_writeb(ROUTE59_FLAG, (ds_readb(CURRENT_TOWN) == TOWNS_PEILINEN ? 1 : 3));
				ds_writeb(TRAVEL_DETOUR, 1);
			}
		}

		timewarp(MINUTES(15));
	}
}

void tevent_021(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 8) > 0 && !ds_readb(TEVENT021_FLAG)) ||
		ds_readb(TEVENT021_FLAG) != 0)
	{
		ds_writeb(GATHER_HERBS_SPECIAL, 126);
		TRV_found_herb_place(1);
		ds_writeb(GATHER_HERBS_SPECIAL, -1);
		ds_writeb(TEVENT021_FLAG, 1);
	}
}

void tevent_022(void)
{
	signed short answer;

	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 0) > 0 && !ds_readb(TEVENT022_FLAG)) ||
		ds_readb(TEVENT022_FLAG) != 0)
	{
		ds_writeb(TEVENT022_FLAG, 1);
		if (TRV_found_camp_place(0))
		{
			if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_FAEHRTENSUCHEN, 5) > 0 && !ds_readb(TEVENT022_TRACK_FLAG)) ||
				ds_readb(TEVENT022_TRACK_FLAG) != 0)
			{
				do {
					answer = GUI_radio(get_tx2(0xec), 2,
								get_tx2(0xf0),
								get_tx2(0xf4));
				} while (answer == -1);

				if (answer == 1)
				{
					TRV_hunt_generic(25, 62, -2, 2, 8, 3, 7, 2, 4, 30, 6);
					ds_writeb(TEVENT022_TRACK_FLAG, 1);
				}
			}
		}
	}
}

void tevent_024(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT024_FLAG)) ||
		ds_readb(TEVENT024_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT024_FLAG, 1);
	}
}

void tevent_025(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 1) > 0 && !ds_readb(TEVENT025_FLAG)) ||
		ds_readb(TEVENT025_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT025_FLAG, 1);
	}
}

void tevent_026(void)
{
	TRV_cross_a_ford(get_tx2(0x120), 60, 0);
}

void tevent_027(void)
{
	TRV_cross_a_ford(get_tx2(0x124), 30, 0);
}

void tevent_028(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT028_FLAG)) ||
		ds_readb(TEVENT028_FLAG) != 0)
	{
		TRV_found_camp_place(1);
		ds_writeb(TEVENT028_FLAG, 1);
	}
}

void tevent_029(void)
{
	signed short i;
	signed short item_pos;
	Bit8u *hero;

	load_in_head(51);

	GUI_dialog_na(0, get_tx2(0x128));
	GUI_dialog_na(0, get_tx2(0x12c));

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		hero_disease_test(hero, 2, 20 - (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK))));
	}

	if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 3) > 0)
	{
		/* skill test succeeded */
		timewarp(HOURS(3));

		GUI_dialog_na(0, get_tx2(0x130));
	} else {
		/* skill test failed */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
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

		GUI_dialog_na(0, get_tx2(0x134));
	}
}

void tevent_030(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 0) > 0 && !ds_readb(TEVENT030_FLAG)) ||
		ds_readb(TEVENT030_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT030_FLAG, 1);
	}
}

void tevent_031(void)
{
	signed short answer;

	if (ds_readb(FIND_HYGGELIK))
	{
		search_ruin1();

	} else if (!ds_readb(TEVENT031_FLAG))
	{
		load_in_head(49);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						get_tx2(0x00), 2,
						get_tx2(0x04),
						get_tx2(0x08));
		} while (answer == -1);

		if (answer == 2)
		{
			if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_KRIEGSKUNST, 7) <= 0)
			{
				/* skill test failed */
				ds_writeb(FIG_INITIATIVE, 1);
				do_fight(FIGHTS_F031);
			}
		} else {
			do_fight(FIGHTS_F031);
		}

		ds_writeb(TEVENT031_FLAG, 1);
	}
}

void tevent_032(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT032_FLAG)) ||
		ds_readb(TEVENT032_FLAG) != 0)
	{
		ds_writeb(TEVENT032_FLAG, 1);
		if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 5) > 0 && !ds_readb(TEVENT032_HERB_FLAG)) ||
			ds_readb(TEVENT032_HERB_FLAG) != 0)
		{
			ds_writeb(TEVENT032_HERB_FLAG, 1);

			ds_writeb(GATHER_HERBS_SPECIAL, 62);
			TRV_found_camp_place(2);
			ds_writeb(GATHER_HERBS_SPECIAL, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

void tevent_033(void)
{
	signed short answer;

	load_in_head(9);

	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
					get_tx2(0x0c), 3,
					get_tx2(0x10),
					get_tx2(0x14),
					get_tx2(0x18));
	} while (answer == -1);

	if (answer == 1 || answer == 2)
	{
		timewarp(MINUTES(15));

		GUI_dialog_na(0, (Bit8u*)(answer == 1 ? get_tx2(0x1c) : get_tx2(0x20)));
	}
}

void tevent_034(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT034_FLAG)) ||
		ds_readb(TEVENT034_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT034_FLAG, 1);
	}
}

void tevent_035(void)
{
	signed short answer;

	if (!ds_readb(TEVENT035_FLAG))
	{
		do {
			answer = GUI_radio(get_tx2(0x24), 2,
						get_tx2(0x28),
						get_tx2(0x2c));
		} while (answer == -1);

		if (answer == 2)
		{
			if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_KRIEGSKUNST, 8) <= 0)
			{
				/* skill test failed */
				ds_writeb(FIG_INITIATIVE, 1);
				do_fight(FIGHTS_F035);
			}
		} else {
			do_fight(FIGHTS_F035);
		}

		ds_writeb(TEVENT035_FLAG, 1);
	}
}

void tevent_036(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 3) > 0 && !ds_readb(TEVENT036_FLAG)) ||
		ds_readb(TEVENT036_FLAG) != 0)
	{
		ds_writeb(TEVENT036_FLAG, 1);

		if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 6) > 0 && !ds_readb(TEVENT036_HERB_FLAG)) ||
			ds_readb(TEVENT036_HERB_FLAG) != 0)
		{
			ds_writeb(TEVENT036_HERB_FLAG, 1);

			ds_writeb(GATHER_HERBS_SPECIAL, 63);
			TRV_found_camp_place(2);
			ds_writeb(GATHER_HERBS_SPECIAL, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

void tevent_039(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 4) > 0 && !ds_readb(TEVENT039_FLAG)) ||
		ds_readb(TEVENT039_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT039_FLAG, 1);
	}
}

void tevent_040(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 4) > 0 && !ds_readb(TEVENT040_FLAG)) ||
		ds_readb(TEVENT040_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT040_FLAG, 1);
	}
}

void tevent_041(void)
{
	signed short answer;

	TRV_barrier(12);

	if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_FAEHRTENSUCHEN, 0) > 0 && !ds_readb(TEVENT041_FLAG))
	{
		do {
			answer = GUI_radio(get_tx2(0x50), 2,
						get_tx2(0x54),
						get_tx2(0x58));
		} while (answer == -1);

		if (answer == 1)
		{
			TRV_hunt_generic(21, 23, -2, 2, 8, 3, 7, 2, 4, 15, 8);

			ds_writeb(TEVENT041_FLAG, 1);
		}
	}
}

void tevent_042(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT042_FLAG)) ||
		ds_readb(TEVENT042_FLAG) != 0)
	{
		TRV_found_replenish_place(0);
		ds_writeb(TEVENT042_FLAG, 1);
	}
}

void tevent_043(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 3) > 0 && !ds_readb(TEVENT043_FLAG)) ||
		ds_readb(TEVENT043_FLAG) != 0)
	{
		TRV_found_replenish_place(0);
		ds_writeb(TEVENT043_FLAG, 1);
	}
}

/* dying adventuress */
void tevent_044(void)
{
	signed short answer;
	Bit8u *hero;

	if (!ds_readb(TEVENT094_FLAG))
	{
		ds_writeb(TEVENT094_FLAG, 1);

		load_in_head(1);

		hero = Real2Host(get_first_hero_available_in_group());

		sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
			(char*)get_tx2(0x84),
			(char*)hero + HERO_NAME2,
			(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						Real2Host(ds_readd(DTP2)) + 0x400, 2,
						get_tx2(0x88),
						get_tx2(0x8c));
		} while (answer == -1);

		if (answer == 2)
		{
			/* bury her */
			add_ds_ds(GODS_ESTIMATION + 4 * 5, 50L);

			timewarp(HOURS(1));

			add_hero_ap_all(10);
		}
	}
}

/* a farmer */
void tevent_045(void)
{
	signed short answer;

	if (ds_readw(TRV_DESTINATION) == 40)
	{
		load_in_head(11);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						get_tx2(0x90), 3,
						get_tx2(0x94),
						get_tx2(0x98),
						get_tx2(0x9c));
		} while (answer == -1);

		if (answer == 1 || answer == 2)
		{
			timewarp(MINUTES(15));

			GUI_dialog_na(0, (Bit8u*)(answer == 1 ? get_tx2(0xa0) : get_tx2(0xa4)));
		}
	}
}

/* abandoned inn */
void tevent_046(void)
{
	signed short answer;
	signed short enter_inn;
	Bit8u *hero;

	enter_inn = 0;

	/* pause traveling */
	ds_writeb(EVENT_ANI_BUSY, 1);

	load_ani(12);
	draw_main_screen();
	init_ani(0);

	do {
		answer = GUI_radio(get_tx2(0xa8), 2,
					get_tx2(0xac),
					get_tx2(0xb0));
	} while (answer == -1);

	if (answer == 1)
	{
		/* make a camp */
		ds_writew(CAMP_INCIDENT, 1);
		ds_writeb(LOCATION, LOCATION_WILDCAMP);
		do_location();
		ds_writeb(LOCATION, 0);
		TRV_load_textfile(-1);

		hero = (ds_readws(CAMP_INCIDENT) != -1 ? get_hero(ds_readw(CAMP_INCIDENT)) : Real2Host(get_first_hero_available_in_group()));
		ds_writew(CAMP_INCIDENT, -1);

		if (test_skill(hero, TA_SINNESSCHAERFE, 0) > 0)
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx2(0xb4),
				(char*)hero + HERO_NAME2,
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			do {
				answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
							get_tx2(0xb8),
							get_tx2(0xbc));
			} while (answer == -1);

			if (answer == 1)
			{
				ds_writeb(TRAVEL_DETOUR, 2);
				enter_inn = 1;
			}
		}

		if (!enter_inn)
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx2(0xc0),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			ds_writew(FIG_DISCARD, 1);

			TRV_fight_event(FIGHTS_F046, 46);

			do {
				answer = GUI_radio(get_tx2(0xc4), 2,
							get_tx2(0xc8),
							get_tx2(0xcc));
			} while (answer == -1);

			if (answer == 1)
			{
				ds_writeb(TRAVEL_DETOUR, 2);
			}
		}
	}

	set_var_to_zero();
	/* resume traveling */
	ds_writeb(EVENT_ANI_BUSY, 0);
	ds_writew(REQUEST_REFRESH, 1);
}

void tevent_048(void)
{
	signed short answer;

	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 3) > 0 && !ds_readb(TEVENT048_FLAG)) ||
		ds_readb(TEVENT048_FLAG) != 0)
	{
		ds_writeb(TEVENT048_FLAG, 1);

		if (TRV_found_camp_place(0))
		{
			if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_FAEHRTENSUCHEN, 3) > 0 && !ds_readb(TEVENT048_TRACK_FLAG)) ||
				ds_readb(TEVENT048_TRACK_FLAG) != 0)
			{
				do {
					answer = GUI_radio(get_tx2(0xd0), 2,
								get_tx2(0xd4),
								get_tx2(0xd8));
				} while (answer == -1);

				if (answer == 1)
				{
					TRV_hunt_generic(21, 55, -2, 2, 8, 3, 5, 2, 4, 25, 12);
					ds_writeb(TEVENT048_TRACK_FLAG, 1);
				}
			}
		}
	}
}

void tevent_049(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT049_FLAG)) ||
		ds_readb(TEVENT049_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT049_FLAG, 1);
	}
}

void tevent_050(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 3) > 0 && !ds_readb(TEVENT050_FLAG)) ||
		ds_readb(TEVENT050_FLAG) != 0)
	{
		ds_writeb(TEVENT050_FLAG, 1);

		if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 4) > 0 && !ds_readb(TEVENT050_HERB_FLAG)) ||
			ds_readb(TEVENT050_HERB_FLAG) != 0)
		{
			TRV_found_replenish_place(ds_writeb(TEVENT050_HERB_FLAG, 1));
		} else {
			TRV_found_replenish_place(0);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
