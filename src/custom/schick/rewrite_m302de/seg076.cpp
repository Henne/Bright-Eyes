/**
 *	Rewrite of DSA1 v3.02_de functions of seg076 (dungeon: common 2/2)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg076.cpp
 */

#if !defined(__BORLANDC__)
#include <stdlib.h>
#endif

#include <stdio.h>

#include "v302de.h"

/* we need an own prototype of div16 */
#define DONT_DEF_DIV16

#include "seg000.h"
#include "seg002.h"
#include "seg003.h"
#include "seg007.h"
#include "seg025.h"
#include "seg028.h"
#include "seg032.h"
#include "seg029.h"
#include "seg047.h"
#include "seg049.h"
#include "seg074.h"
#include "seg075.h"
#include "seg076.h"
#include "seg077.h"
#include "seg078.h"
#include "seg079.h"
#include "seg080.h"
#include "seg081.h"
#include "seg082.h"
#include "seg083.h"
#include "seg084.h"
#include "seg085.h"
#include "seg086.h"
#include "seg087.h"
#include "seg088.h"
#include "seg089.h"
#include "seg090.h"
#include "seg091.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"

#if defined(__BORLANDC__)
signed short div16(signed char);
#endif

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct dummy5 {
	unsigned char a[5];
};

#if !defined(__BORLANDC__)
static signed short (*DNG_handler[])(void) = {
	NULL,
	DNG01_handler,
	DNG02_handler,
	DNG03_handler,
	DNG04_handler,
	DNG05_handler,
	DNG06_handler,
	DNG07_handler,
	DNG08_handler,
	DNG09_handler,
	DNG10_handler,
	DNG11_handler,
	DNG12_handler,
	DNG13_handler,
	DNG14_handler,
	DNG15_handler,
};
#endif

/**
 * \brief	opening doors logic
 * \param	action	how to try to open the door (smash / lockpicks / magic)
 */
void DNG_door(signed short action)
{
	signed short x;
	signed short y;
	signed short pos;
	signed short l4;
	signed short spell_result;
	struct dummy5 *ptr;
	Bit8u *hero;
	signed short hero_pos;
	signed short lockpick_pos;
	signed short lockpick_result;

	ptr = (struct dummy5*)Real2Host(ds_readd(DUNGEON_DOORS_BUF));
	x = ds_readws(X_TARGET);
	y = ds_readws(Y_TARGET);

	switch (ds_readbs(DIRECTION))
	{
		case 0: y--; break;
		case 1:	x++; break;
		case 2: y++; break;
		case 3: x--; break;
	}

	pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * x + y;

	do {

		if (host_readws((Bit8u*)ptr + 0) == pos)
		{
#if !defined(__BORLANDC__)
			D1_INFO("Tuer: KK notwendig %d, SCHLOESSER mod = %d, FORAMEN mod = %d\n",
					host_readbs((Bit8u*)ptr + 2),
					host_readbs((Bit8u*)ptr + 3),
					host_readbs((Bit8u*)ptr + 4));
#endif

			if (action == 135)
			{
				/* smash door */
				if (ds_readws(0xd011) != 5)
				{

					if (ds_readb(DUNGEON_INDEX) == 15 && pos == 0x1903 && ds_readb(0x41c7) != 0)
					{
						GUI_output(get_dtp(0x48));

					} else if (!(ds_readb(DUNGEON_INDEX) == 11 && pos == 0xc08 && ds_readb(0x4168) != 4) &&
							!(ds_readb(DUNGEON_INDEX) == 6 && pos == 0xb01 && ds_readb(DNG06_PITDOOR_FLAG)) &&
							!(ds_readb(DUNGEON_INDEX) == 12 && ds_readb(DNG12_WATERTRAP_WATER_RUNS)))
					{
						/* for this door works an IRON KEY */
						/* BEWARE: there are two of them */
						if (ds_readb(DUNGEON_INDEX) == 11 && pos == 0xc08 && get_first_hero_with_item(209) != -1)
						{
							or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x02);
						}

						if (div16(host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x)) == 1)
						{
							/* the door is closed -> open it */
							l4 = host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x) & 0x02;

							if (l4 != 0 || !host_readbs((Bit8u*)ptr + 2))
							{
								and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x0f);
								or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x20);
								ds_writeb(0xbd4d, host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x));
								DNG_open_door();

								ds_writebs((0xbd38 + 6), ds_writebs((0xbd38 + 7), ds_writebs((0xbd38 + 8), -1)));
								ds_writew(0xd013, 1);
								ds_writew(0xd011, 3);
							} else {
								ds_writebs((0xbd38 + 6), 0x22);
								ds_writebs((0xbd38 + 7), 0x23);
								ds_writebs((0xbd38 + 8), 0x0b);
								ds_writew(0xd013, 1);
								ds_writew(0xd011, 5);
							}

						} else if (div16(host_readbs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x)) == 2)
						{

							/* the door is open -> close it */
							DNG_close_door();

							and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x0f);
							or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x12);

							ds_writeb(0xbd4d, host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x));
							ds_writew(0xd011, 1);
						}
					}
				} else {
					play_voc(ARCHIVE_FILE_FX14_VOC);

					if (check_heros_KK(host_readbs((Bit8u*)ptr + 2)))
					{
						and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x0f);
						or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x90);

						ds_writeb(0xbd4d, host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x));
						ds_writew(DNG_REFRESH_DIRECTION, -1);
					}

					sub_hero_le(Real2Host(get_first_hero_available_in_group()), 1);

					if (get_second_hero_available_in_group())
					{
						sub_hero_le(Real2Host(get_second_hero_available_in_group()), 1);
					}
				}
			}

			if (action == 136)
			{
				/* use lockpicks */

				hero = Real2Host(get_first_hero_available_in_group());

				if ((lockpick_pos = hero_has_lockpicks(hero)) != -1)
				{
					if (lockpick_pos != -2)
					{
						lockpick_result = test_skill(hero, TA_SCHLOESSER, host_readbs((Bit8u*)ptr + 3));

						play_voc(ARCHIVE_FILE_FX11_VOC);

						inc_ds_ws(LOCKPICK_TRY_COUNTER);

						if (lockpick_result == -99 || ds_readws(LOCKPICK_TRY_COUNTER) > 3)
						{
							/* LOCKPICK breaks if the test was unlucky,
								or when tried three times without moving */
							print_msg_with_first_hero(get_ltx(0x854));

							or_ptr_bs(hero + 14 * lockpick_pos + (HERO_ITEM_HEAD + 4), 1);

							ds_writew(LOCKPICK_TRY_COUNTER, 0);

						} else if (lockpick_result <= 0) {

							/* just failed */
							print_msg_with_first_hero(get_ltx(0x850));

						} else {
							/* success => the door opens */
							and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x0f);
							or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x20);
							ds_writeb(0xbd4d, host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x));
							DNG_open_door();

							add_hero_ap(hero, 1L);

							ds_writebs((0xbd38 + 6), ds_writebs((0xbd38 + 7), ds_writebs((0xbd38 + 8), -1)));
							ds_writew(0xd013, 1);
						}

					} else {
						/* all LOCKPICKS are broken */
						print_msg_with_first_hero(get_ltx(0x84c));
					}
				} else {
					/* the leader has no LOCKPICKS */
					print_msg_with_first_hero(get_ltx(0x848));
				}
			}

			if (action == 137)
			{
				/* use magic */
				hero_pos = select_hero_ok(get_ltx(0x4f4));

				if (hero_pos != -1)
				{
					hero = get_hero(hero_pos);

					if (host_readbs(hero + HERO_TYPE) < 7)
					{
						GUI_output(get_ltx(0x528));
					} else {
						spell_result = test_spell(hero, 28, host_readbs((Bit8u*)ptr + 4));

						if (spell_result == -99)
						{
							/* unlucky => just print a message */
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ltx(0x97c),
								(char*)hero + HERO_NAME2);

							GUI_output(Real2Host(ds_readd(DTP2)));
						} else if (spell_result <= 0)
						{
							/* failed => half AE costs */
							sub_ae_splash(hero, get_spell_cost(28, 1));

							play_voc(ARCHIVE_FILE_FX17_VOC);
						} else {
							play_voc(ARCHIVE_FILE_FX17_VOC);

							sub_ae_splash(hero, get_spell_cost(28, 0));

							/* success => the door opens */
							and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x0f);
							or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x20);
							ds_writeb(0xbd4d, host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x));
							DNG_open_door();

							add_hero_ap(hero, 1L);

							ds_writebs((0xbd38 + 6), ds_writebs((0xbd38 + 7), ds_writebs((0xbd38 + 8), -1)));
							ds_writew(0xd013, 1);
						}
					}
				}
			}
		}

	} while (host_readws((Bit8u*)(ptr++)) != -1);
}

void print_msg_with_first_hero(Bit8u *msg)
{
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());

	sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
		(char*)msg, (char*)hero + HERO_NAME2);

	GUI_input(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), 0);
	return;
}

/**
 * \brief		fallpit logic for the current group in dungeons
 * \param max_damage	maximum damage if a hero drops in the fallpit
 */
void DNG_fallpit_test(signed short max_damage)
{
	signed short i;
	Bit8u *hero;

	play_voc(ARCHIVE_FILE_FX18_VOC);

	and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (ds_readws(Y_TARGET) << 4) + ds_readws(X_TARGET), 0x0f);
	or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (ds_readws(Y_TARGET) << 4) + ds_readws(X_TARGET), 0x60);

	if (ds_readb(DUNGEON_LIGHT) != 0)
	{
		/* light is on */
		GUI_output(get_ltx(0x814));

		/* drop one level down */
		DNG_inc_level();

		and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (ds_readws(Y_TARGET) << 4) + ds_readws(X_TARGET), 0x0f);
		or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (ds_readws(Y_TARGET) << 4) + ds_readws(X_TARGET), 0x50);

		/* damage the heros */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			/* TODO: need to check if the hero is dead ? */
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
			{
				sub_hero_le(hero, random_schick(max_damage));
			}
		}
	} else {
		/* light is off */
		if (DNG_fallpit(max_damage))
		{
			/* drop one level down */
			inc_ds_bs_post(DUNGEON_LEVEL);
			load_area_description(1);

			and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (ds_readws(Y_TARGET) << 4) + ds_readws(X_TARGET), 0x0f);
			or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (ds_readws(Y_TARGET) << 4) + ds_readws(X_TARGET), 0x50);

			/* move one level up. Why? */
			dec_ds_bs_post(DUNGEON_LEVEL);

			ds_writews(X_TARGET, ds_readws(0x2d83));
			ds_writews(Y_TARGET, ds_readws(0x2d85));

			load_area_description(1);

			DNG_update_pos();
		} else {
			and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (ds_readws(Y_TARGET) << 4) + ds_readws(X_TARGET), 0x0f);
			or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (ds_readws(Y_TARGET) << 4) + ds_readws(X_TARGET), 0x50);
		}
	}
}

signed short DNG_step(void)
{
	signed short l_si;
	signed short l_di;
	signed short (*dungeon_handler)(void);
	signed short retval;
	signed short tw_bak;
	signed short old_value;
	signed short x;
	signed short y;
	signed short pos;

	ds_writeb((0xbd38 + 0), 0x0c);
	old_value = ds_readbs((0xbd38 + 1));
	ds_writeb((0xbd38 + 1), ds_readbs(CAN_MERGE_GROUP) == -1 ? 0x2d : 0x0f);

	if (ds_readbs((0xbd38 + 1)) != old_value)
	{
		ds_writew(0xd013, 1);
	}

	ds_writeb((0xbd38 + 2), 0x1d);
	ds_writeb((0xbd38 + 3), 0x25);
	ds_writeb((0xbd38 + 4), 0x27);
	ds_writeb((0xbd38 + 5), 0x0b);

	if (ds_readw(0xd011) == 0 && ds_readb((0xbd38 + 6)) != 0x36)
	{
		ds_writeb((0xbd38 + 6), 0x36);
		ds_writew(0xd013, 1);
	}

	if (ds_readw(REQUEST_REFRESH) != 0)
	{
		draw_main_screen();
		GUI_print_loc_line(get_dtp(0x00));
		ds_writew(REQUEST_REFRESH, ds_writew(0xd013, 0));
		ds_writew(DNG_REFRESH_X_TARGET, -1);
	}

	if (ds_readw(0xd013) != 0 && ds_readb(PP20_INDEX) == 0)
	{
		draw_icons();
		ds_writew(0xd013, 0);
	}

	if (ds_readbs(DIRECTION) != ds_readws(DNG_REFRESH_DIRECTION) ||
		ds_readws(X_TARGET) != ds_readws(DNG_REFRESH_X_TARGET) ||
		ds_readws(Y_TARGET) != ds_readws(DNG_REFRESH_Y_TARGET))
	{
		DNG_update_pos();
		set_automap_tiles(ds_readws(X_TARGET), ds_readws(Y_TARGET));
		DNG_fight();
	}

	if (ds_readws(X_TARGET) != ds_readws(0x2d83) ||
		ds_readws(Y_TARGET) != ds_readws(0x2d85) ||
		ds_readbs(0x9312) != 0)
	{
		ds_writeb(CAN_MERGE_GROUP, (unsigned char)can_merge_group());
		ds_writew(LOCKPICK_TRY_COUNTER, 0);
	}

	ds_writew(0x2d83, ds_readws(X_TARGET));
	ds_writew(0x2d85, ds_readws(Y_TARGET));
	ds_writeb(0x2d7c, ds_readbs(DIRECTION));

	handle_gui_input();

	if (ds_readw(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == 73)
	{
		tw_bak = ds_readws(TEXTBOX_WIDTH);
		ds_writew(TEXTBOX_WIDTH, 3);

		for (l_di = retval = 0; l_di < 9; l_di++)
		{
			if (ds_readbs(0xbd38 + l_di) != -1)
			{
				retval++;
			}
		}

		l_di = GUI_radio(get_ltx(0x858), (signed char)retval,
					get_ltx(0x85c),
					get_ltx(0x860),
					get_ltx(0x864),
					get_ltx(0x868),
					get_ltx(0x86c),
					get_ltx(0x354),
					ds_readws(0xd011) == 0 ? get_ltx(0x4c8) :(
						ds_readws(0xd011) == 1 ? get_ltx(0x870) :(
						ds_readws(0xd011) == 3 ? get_ltx(0xc4c) :(
						ds_readws(0xd011) == 5 ? get_ltx(0x878) :(
						ds_readws(0xd011) == 4 ? get_ltx(0x8ac) : get_ltx(0x874))))),
					get_ltx(0x87c),
					get_ltx(0x880)) - 1;

		if (l_di != -2)
		{
			ds_writew(ACTION, l_di + 129);
		}

		ds_writew(TEXTBOX_WIDTH, tw_bak);
	}

	l_di = 0;

	if (ds_readws(ACTION) == 129)
	{
		GRP_split();
		ds_writeb(CAN_MERGE_GROUP, (unsigned char)can_merge_group());

	} else if (ds_readws(ACTION) == 130)
	{
		/* merge groups or reach hands through the mirror */
		pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

		if ((ds_readb(DUNGEON_INDEX) == 15 && pos == 0x1801) || pos == 0x1805)
		{
			GUI_output(get_dtp(0x84));

			ds_writeb(DNG15_REACHED_HANDS, 1);
		} else {
			GRP_merge();
		}

	} else if (ds_readws(ACTION) == 131)
	{
		GRP_switch_to_next(0);

	} else if (ds_readws(ACTION) == 132)
	{
		game_options();

	} else if (ds_readws(ACTION) == 133)
	{
		show_automap();

	} else if (ds_readws(ACTION) == 134)
	{
		if (select_magic_user() > 0)
		{
			ds_writew(DNG_REFRESH_DIRECTION, -1);
		}

	} else if (ds_readws(ACTION) == 135 && ds_readw(0xd011) == 0)
	{
		ds_writeb(LOCATION, LOCATION_CITYCAMP);
		ds_writeb(0xbd27, 0);
		l_di = 1;

	} else if (ds_readws(ACTION) == 75)
	{
		update_direction(3);
		ds_writebs((0xbd38 + 6), ds_writebs((0xbd38 + 7), ds_writebs((0xbd38 + 8), -1)));

	} else if (ds_readws(ACTION) == 77)
	{
		update_direction(1);
		ds_writebs((0xbd38 + 6), ds_writebs((0xbd38 + 7), ds_writebs((0xbd38 + 8), -1)));

	} else if (ds_readws(ACTION) == 72)
	{
		if ((l_si = div16(ds_readb(0xbd4d))) == 11)
		{
			l_si = 1 << ds_readbs(DIRECTION);

			if (ds_readb(0xbd4d) & l_si & 0x0f)
			{
				DNG_timestep(1);
			}

		} else if ((l_si = div16(ds_readb(0xbd4d))) != 15 &&
				l_si != 1 &&
				l_si != 10 &&
				l_si != 8 &&
				l_si != 7)
		{
			DNG_timestep(1);
		} else {
			no_way();
		}

	} else if (ds_readws(ACTION) == 80)
	{
		if ((l_si = div16(ds_readb(0xbd4e))) != 15 &&
				l_si != 1 &&
				l_si != 10 &&
				l_si != 8 &&
				l_si != 7)
		{
			DNG_timestep(-1);
		} else {
			no_way();
		}

	} else if (ds_readws(ACTION) >= 135 &&
			ds_readws(ACTION) <= 137 &&
			ds_readbs(0xbcb7 + ds_readws(ACTION)) != -1)
	{
		if (ds_readw(0xd011) == 1 || ds_readw(0xd011) == 3 || ds_readw(0xd011) == 5)
		{
			DNG_door(ds_readws(ACTION));
		} else if (ds_readws(ACTION) == 135 && ds_readw(0xd011) == 2)
		{
			seg092_06b4(1);

			if (ds_readw(GET_EXTRA_LOOT) != 0)
			{
				x = ds_readws(X_TARGET);
				y = ds_readws(Y_TARGET);

				switch (ds_readbs(DIRECTION))
				{
					case 0:	y--; break;
					case 1:	x++; break;
					case 2:	y++; break;
					case 3:	x--; break;
				}

				or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + (y << 4) + x, 0x02);
			}
		} else if (ds_readws(ACTION) == 135 && (!ds_readb(DNG15_LEVER_SOUTH) || !ds_readb(DNG15_LEVER_NORTH)))
		{
			DNG15_riddle();
		}
	}

	if (ds_readb(DUNGEON_INDEX) != 0 &&
		ds_readbs(DNG_AREA_LOADED) != -1 &&
		!l_di)
	{
		DNG_see_stairs();
		DNG_see_door();
		DNG_see_chest();
		DNG_see_lever();

#if defined(__BORLANDC__)
		dungeon_handler = (signed short (*)(void))ds_readd(0x92d2 + 4 * ds_readbs(DUNGEON_INDEX));
#else
		dungeon_handler = DNG_handler[ds_readbs(DUNGEON_INDEX)];
#endif
		retval = dungeon_handler();
	}

	return 0;
}

struct stair_struct {
	signed short pos;
	signed char target_x;
	signed char target_y;
};

void DNG_see_stairs(void)
{
	signed short target_pos;
	stair_struct *stair_ptr;
	stair_ptr = (stair_struct*)Real2Host(ds_readd(DUNGEON_STAIRS_BUF));

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

#if !defined(__BORLANDC__)
	if (sizeof(stair_struct) != 4)
	{
		D1_INFO("sizeof(stair_struct) = %d\n", sizeof(stair_struct));
		exit(-1);
	}
#endif

	do {
		if (host_readws((Bit8u*)stair_ptr) == target_pos)
		{
			/* found the current stairs */
			ds_writew(X_TARGET, host_readbs((Bit8u*)stair_ptr + 2) & 0x0f);
			ds_writew(Y_TARGET, host_readbs((Bit8u*)stair_ptr + 3) & 0x0f);
			ds_writeb(DIRECTION, host_readbs((Bit8u*)stair_ptr + 3) >> 4);

			if (host_readbs((Bit8u*)stair_ptr + 2) & 0x80)
			{
				/* downstairs */
				if (host_readbs((Bit8u*)stair_ptr + 2) & 0x40)
				{
					inc_ds_bs_post(DUNGEON_LEVEL);
				}

				DNG_inc_level();

			} else {
				/* upstairs */
				if (host_readbs((Bit8u*)stair_ptr + 2) & 0x40)
				{
					dec_ds_bs_post(DUNGEON_LEVEL);
				}

				DNG_dec_level();

			}

			set_automap_tiles(ds_readws(X_TARGET), ds_readws(Y_TARGET));

			break;
		}

	} while (host_readws((Bit8u*)stair_ptr++) != -1);
}

void DNG_see_door(void)
{
	signed short l_si;

	if ((l_si = div16(ds_readb(0xbd4d))) == 1 || l_si == 2)
	{
		/* standing direct in front of a door with view to it */
		if (ds_readbs((0xbd38 + 6)) != 0x21 && ds_readbs((0xbd38 + 6)) != 0x22)
		{
			ds_writebs((0xbd38 + 6), 0x21);
			ds_writew(0xd013, 1);
		}

		if (ds_readbs((0xbd38 + 6)) != 0x22)
		{
			ds_writew(0xd011, l_si == 1 ? 1 : 3);
		}

	} else {
		if (ds_readbs((0xbd38 + 6)) != -1 &&
			(ds_readws(0xd011) == 1 || ds_readws(0xd011) == 3 || ds_readws(0xd011) == 5))
		{
			/* standing two fields before a door with view to it */
			ds_writebs((0xbd38 + 6), ds_writebs((0xbd38 + 7), ds_writebs((0xbd38 + 8), -1)));
			ds_writew(0xd013, 1);
			ds_writew(0xd011, 0);
		}
	}
}

void DNG_see_chest(void)
{
	if (div16(ds_readb(0xbd4d)) == 8)
	{
		/* standing direct in front of a treasure chest with view to it */
		if (ds_readbs((0xbd38 + 6)) != 0x26)
		{
			ds_writebs((0xbd38 + 6), 0x26);
			ds_writew(0xd013, 1);
			ds_writew(0xd011, 2);
		}

	} else {
		if (ds_readbs((0xbd38 + 6)) != -1 && ds_readws(0xd011) == 2)
		{
			/* standing two fields before a treasure chest with view to it */
			ds_writebs((0xbd38 + 6), ds_writebs((0xbd38 + 7), ds_writebs((0xbd38 + 8), -1)));
			ds_writew(0xd013, 1);
			ds_writew(0xd011, 0);
		}
	}
}

void do_dungeon(void)
{
	signed short tw_bak;

	if (ds_readbs(DNG_AREA_LOADED) != ds_readbs(DUNGEON_INDEX) || ds_readws(AREA_PREPARED) != 0 || ds_readws(0xd00f) != 0)
	{
		ds_writed(DNG_MAP_PTR, (Bit32u)RealMake(datseg, DNG_MAP));

		seg028_0555(0);

		set_audio_track(ARCHIVE_FILE_DUNGEON_XMI);

		ds_writed(0xe48d, (Bit32u)RealMake(datseg, (!ds_readb(DUNGEON_TYPE) ? 0x7dea : (ds_readb(DUNGEON_TYPE) == 1 ? 0x8a4a : 0x841a))));

		ds_writew(0xd00f, 0);
		ds_writew(REQUEST_REFRESH, 1);
	}

	ds_writew(CURRENT_ANI, -1);

	ds_writeb(0x2dad, ds_readbs(DUNGEON_INDEX));

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	DNG_step();

	ds_writew(TEXTBOX_WIDTH, tw_bak);
}

struct fight_struct {
	signed short pos;
	signed short fight_id;
	signed short unkn1;
	signed short unkn2;
	signed short unkn3;
	signed short unkn4;
	signed short ap;
};

void DNG_fight(void)
{
	signed short target_pos;
	struct fight_struct *fight_ptr;

	fight_ptr = (struct fight_struct*)Real2Host(ds_readd(DUNGEON_FIGHTS_BUF));

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

#if !defined(__BORLANDC__)
	if (sizeof(fight_struct) != 14)
	{
		D1_INFO("sizeof(fight_struct) = %d\n", sizeof(fight_struct));
		exit(-1);
	}
#endif
	do {
		if (host_readws((Bit8u*)fight_ptr + 0) == target_pos)
		{
			/* set positions of heros which escape from the fight */
			ds_writew((0xd325 + 0), host_readws((Bit8u*)fight_ptr + 4));
			ds_writew((0xd325 + 2), host_readws((Bit8u*)fight_ptr + 6));
			ds_writew((0xd325 + 4), host_readws((Bit8u*)fight_ptr + 8));
			ds_writew((0xd325 + 6), host_readws((Bit8u*)fight_ptr + 10));

			/* execute the fight */
			if (!do_fight(host_readws((Bit8u*)fight_ptr + 2)))
			{
				add_hero_ap_all(host_readws((Bit8u*)fight_ptr + 12));
			}

			/* play the music for the DUNGEON again */
			set_audio_track(ARCHIVE_FILE_DUNGEON_XMI);
		}

	} while (host_readws((Bit8u*)++fight_ptr) != -1);
}

/**
 * \brief		the group finds a barrel of water
 * \param unit_ptr	pointer to the number of water units
 */
void DNG_waterbarrel(Bit8u *unit_ptr)
{
	signed short item_pos;
	signed short l_di;
	signed short answer;
	signed short units_needed;
	signed short hero_refilled;
	signed short hero_refilled_counter;
	signed short done;
	Bit8u *hero;

	done = 0;

	/* TODO: check the value of unit_ptr first and skip if *unit_ptr <= 0*/

	do {
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0xc34),
			host_readb(unit_ptr));

		answer = GUI_radio(Real2Host(ds_readd(DTP2)), 3,
						get_ltx(0xc38),
						get_ltx(0xc3c),
						get_ltx(0xc40));

		if (answer == 1)
		{
			/* drink */
			hero = get_hero(0);
			for (l_di = 0; l_di <= 6; l_di++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					/* 1 unit of water <=> 10 Points of thirst */

					units_needed = (host_readbs(hero + HERO_THIRST) + 9) / 10;

					if (host_readb(unit_ptr) <= units_needed)
					{
						/* not enough units in the barrel for this hero */
						sub_ptr_bs(hero + HERO_THIRST, host_readb(unit_ptr) * 10);

						if (host_readbs(hero + HERO_THIRST) < 0)
						{
							host_writeb(hero + HERO_THIRST, 0);
						}

						host_writeb(unit_ptr, 0);

						GUI_output(get_ltx(0xc44));

						break;
					} else {
						/* this hero quenches his/her thirst completely */
						sub_ptr_bs(unit_ptr, (unsigned char)units_needed);
						host_writeb(hero + HERO_THIRST, 0);
					}
				}
			}

		} else	if (answer == 2)
		{
			/* replenish WATERSKINS */
			hero = get_hero(0);
			for (hero_refilled_counter = l_di = 0; l_di <= 6; l_di++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					for (item_pos = hero_refilled = 0; item_pos < 23; item_pos++)
					{
						if (host_readws(hero + HERO_ITEM_HEAD + 14 * item_pos) == 30)
						{
							units_needed = 0;

							if (ks_half_empty(hero + HERO_ITEM_HEAD + 14 * item_pos))
							{
								units_needed = 1;

							} else if (ks_empty(hero + HERO_ITEM_HEAD + 14 * item_pos))
							{
								units_needed = 2;
							}

							if (units_needed != 0)
							{
								hero_refilled = 1;

								/* reset empty and half_empty bits of the knapsack item status */
#if !defined(__BORLANDC__)
								and_ptr_bs(hero + HERO_ITEM_HEAD + 0x04 + 14 * item_pos, 0xfb);
								and_ptr_bs(hero + HERO_ITEM_HEAD + 0x04 + 14 * item_pos, 0xfd);
#else
								(*(struct knapsack_status*)(hero + HERO_ITEM_HEAD + 4 + 14 * item_pos)).half_empty =
									(*(struct knapsack_status*)(hero + HERO_ITEM_HEAD + 4 + 14 * item_pos)).empty = 0;
#endif

								if (host_readb(unit_ptr) <= units_needed)
								{
									/* empty the barrel completely */
									GUI_output(get_ltx(0xc44));
									host_writeb(unit_ptr, 0);

								} else {
									/* remove units from the barrel */
									sub_ptr_bs(unit_ptr, (unsigned char)units_needed);
								}
							}
						}
					}

					if (hero_refilled != 0)
					{
						hero_refilled_counter++;
					}
				}
			}

			/* print a message if no hero used the barrel */
			if (hero_refilled_counter == 0)
			{
				GUI_output(get_ltx(0xc48));
			}

			done = 1;
		} else {
			/* stay away or pushed ESC */
			done = 1;
		}

	} while (!done);

}

void DNG_see_lever(void)
{
	signed short target_pos;

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	if (ds_readbs(DUNGEON_INDEX) == 15 &&
		(target_pos == 0x1801 || target_pos == 0x1805) &&
		(!ds_readb(DNG15_LEVER_SOUTH) || !ds_readb(DNG15_LEVER_NORTH)))
	{
		if (ds_readbs((0xbd38 + 6)) == -1)
		{
			ds_writeb((0xbd38 + 6), 46);
			ds_writew(0xd013, 1);
			ds_writew(0xd011, 4);
		}

	} else if (ds_readbs((0xbd38 + 6)) != -1 && ds_readw(0xd011) == 4)
	{
			ds_writeb((0xbd38 + 6), -1);
			ds_writew(0xd013, 1);
			ds_writew(0xd011, 0);
	}
}

#if !defined(__BORLANDC__)
}
#endif
