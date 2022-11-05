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
 * \brief   door logic
 *
 * \param   action: which menu option (icon) has been triggered?
 * ACTION_ID_ICON_7 = 135: might be 'open door', 'close door', 'smash door'. Can be distinguished by DNG_MENU_MODE
 * ACTION_ID_ICON_8 = 136: 'lockpicks'
 * ACTION_ID_ICON_9 = 137: 'foramen spell'
 */
void DNG_door(signed short action)
	/* is called from a single code position. DNG_MENU_MODE is one of 1 = DNG_MENU_MODE_OPEN_DOOR, 3 = DNG_MENU_MODE_CLOSE_DOOR, 5 = DNG_MENU_MODE_UNLOCK_DOOR */
{
	signed short x;
	signed short y;
	signed short pos;
	signed short l4;
	signed short spell_result;
	struct dummy5 *ptr_doors;
	Bit8u *hero;
	signed short hero_pos;
	signed short lockpick_pos;
	signed short lockpick_result;

	ptr_doors = (struct dummy5*)Real2Host(ds_readd(DUNGEON_DOORS_BUF));
	x = ds_readws(X_TARGET);
	y = ds_readws(Y_TARGET);

	switch (ds_readbs(DIRECTION))
	{
		case NORTH: y--; break;
		case EAST:  x++; break;
		case SOUTH: y++; break;
		case WEST:  x--; break;
	}

	pos = DNG_POS(ds_readbs(DUNGEON_LEVEL), x, y);

	do {

		if (host_readws((Bit8u*)ptr_doors + 0) == pos)
		{
#if !defined(__BORLANDC__)
			D1_INFO("Tuer: KK notwendig %d, SCHLOESSER mod = %d, FORAMEN mod = %d\n",
					host_readbs((Bit8u*)ptr_doors + 2),
					host_readbs((Bit8u*)ptr_doors + 3),
					host_readbs((Bit8u*)ptr_doors + 4));
#endif

			if (action == ACTION_ID_ICON_7)
			{
				/* 7th icon/menu option has been triggered. Might be 'open door', 'close door', 'smash door'. distinguish based on DNG_MENU_MODE */
				if (ds_readws(DNG_MENU_MODE) != DNG_MENU_MODE_UNLOCK_DOOR) {
					/* either 'open door' or 'close door' */

					if (ds_readb(DUNGEON_INDEX) == DUNGEONS_HYGGELIKS_RUINE && pos == DNG_POS(1,9,3) && ds_readb(DNG15_UNKNOWN_FLAG) != 0)
					{
						GUI_output(get_tx(18));

					} else if (!(ds_readb(DUNGEON_INDEX) == DUNGEONS_PIRATENHOEHLE && pos == DNG_POS(0,12,8) && ds_readb(DNG11_LEVER_FLAG) != 4) &&
							!(ds_readb(DUNGEON_INDEX) == DUNGEONS_DASPOTASCHATZ && pos == DNG_POS(0,11,1) && ds_readb(DNG06_PITDOOR_FLAG)) &&
							!(ds_readb(DUNGEON_INDEX) == DUNGEONS_ZWERGENFESTE && ds_readb(DNG12_WATERTRAP_WATER_RUNS)))
					{
						/* for this door works an IRON KEY [first type] */
						/* BEWARE: there are two types of IRON KEYs in the game */
						if (ds_readb(DUNGEON_INDEX) == DUNGEONS_PIRATENHOEHLE && pos == DNG_POS(0,12,8) && get_first_hero_with_item(ITEM_KEY_IRON_1) != -1)
						{
							or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), 0x02); /* effect: ......1. i.e. door is unlocked */
						}

						if (div16(host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y))) == DNG_TILE_CLOSED_DOOR) /* if 0001.... door is closed */
						{
							/* ASSERT */
							/*
							if (ds_readws(DNG_MENU_MODE) != DNG_MENU_MODE_OPEN_DOOR) {
								D1_INFO("FEHLER: DNG_MENU_MODE sollte DNG_MENU_MODE_OPEN_DOOR sein, stimmt aber nicht.\n");
							}
							*/
							l4 = host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y)) & 0x02; /* read bit 1: is door unlocked? */

							if (l4 != 0 || !host_readbs((Bit8u*)ptr_doors + 2))
							{
								/* door closed and unlocked -> open it */

								and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), 0x0f); /* clear higher 4 bits */
								or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), DNG_TILE_OPEN_DOOR << 4);
								ds_writeb(STEPTARGET_FRONT, host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y)));
								DNG_open_door();

								ds_writebs((NEW_MENU_ICONS + 6), ds_writebs((NEW_MENU_ICONS + 7), ds_writebs((NEW_MENU_ICONS + 8), MENU_ICON_NONE)));
								ds_writew(REDRAW_MENUICONS, 1);
								ds_writew(DNG_MENU_MODE, DNG_MENU_MODE_CLOSE_DOOR);
							} else {
								/* door closed and locked -> show icons for different opening methods */
								ds_writebs((NEW_MENU_ICONS + 6), MENU_ICON_SMASH_DOOR);
								ds_writebs((NEW_MENU_ICONS + 7), MENU_ICON_PICK_LOCK);
								ds_writebs((NEW_MENU_ICONS + 8), MENU_ICON_MAGIC);
								ds_writew(REDRAW_MENUICONS, 1);
								ds_writew(DNG_MENU_MODE, DNG_MENU_MODE_UNLOCK_DOOR);
							}

						} else if (div16(host_readbs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y))) == DNG_TILE_OPEN_DOOR) /* 0010.... i.e. door is open */
						{
							/* ASSERT */
							/*
							if (ds_readws(DNG_MENU_MODE) != DNG_MENU_MODE_CLOSE_DOOR) {
								D1_INFO("FEHLER: DNG_MENU_MODE sollte DNG_MENU_MODE_CLOSE_DOOR sein, stimmt aber nicht.\n");
							}
							*/

							/* the door is open -> close it */
							DNG_close_door();

							and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), 0x0f); /* clear higher 4 bits */
							or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), (DNG_TILE_CLOSED_DOOR << 4) + 0x02); /* +0x02: set bit 1 'unlocked' */

							ds_writeb(STEPTARGET_FRONT, host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y)));
							ds_writew(DNG_MENU_MODE, DNG_MENU_MODE_OPEN_DOOR);
						}
					}
				} else {
					/* smash door */
					play_voc(ARCHIVE_FILE_FX14_VOC);

					if (check_heroes_KK(host_readbs((Bit8u*)ptr_doors + 2)))
					{
						and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), 0x0f); /* clear higher 4 bits */
						or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), DNG_TILE_SMASHED_DOOR << 4);

						ds_writeb(STEPTARGET_FRONT, host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y)));
						ds_writew(DNG_REFRESH_DIRECTION, -1);
					}

					sub_hero_le(Real2Host(get_first_hero_available_in_group()), 1);

					if (get_second_hero_available_in_group())
					{
						sub_hero_le(Real2Host(get_second_hero_available_in_group()), 1);
					}
				}
			}

			if (action == ACTION_ID_ICON_8)
			{
				/* use lockpicks */

				hero = Real2Host(get_first_hero_available_in_group());

				if ((lockpick_pos = hero_has_lockpicks(hero)) != -1)
				{
					if (lockpick_pos != -2)
					{
						lockpick_result = test_skill(hero, TA_SCHLOESSER, host_readbs((Bit8u*)ptr_doors + 3));

						play_voc(ARCHIVE_FILE_FX11_VOC);

						inc_ds_ws(LOCKPICK_TRY_COUNTER);
						/* Original-Bug: Why is this a global variable? Should be related to the door. */

						if (lockpick_result == -99 || ds_readws(LOCKPICK_TRY_COUNTER) > 3)
						{
							/* LOCKPICK breaks if the test was unlucky,
								or when tried three times without moving */
							print_msg_with_first_hero(get_ttx(533));

							or_ptr_bs(hero + SIZEOF_INVENTORY * lockpick_pos + (HERO_INVENTORY + INVENTORY_FLAGS), 1); /* set 'broken' flag */

							ds_writew(LOCKPICK_TRY_COUNTER, 0);

						} else if (lockpick_result <= 0) {

							/* just failed */
							print_msg_with_first_hero(get_ttx(532));

						} else {
							/* success => the door opens */
							and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), 0x0f); /* clear higher 4 bits */
							or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), DNG_TILE_OPEN_DOOR << 4);
							/* note that the 'unlocked' flag ......1. is not explicitly set. It will be set if the party closes the door. */
							ds_writeb(STEPTARGET_FRONT, host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y)));
							DNG_open_door();

							add_hero_ap(hero, 1L); /* hero gets 1 AP for successful lock pick */

							ds_writebs((NEW_MENU_ICONS + 6), ds_writebs((NEW_MENU_ICONS + 7), ds_writebs((NEW_MENU_ICONS + 8), MENU_ICON_NONE)));
							ds_writew(REDRAW_MENUICONS, 1);
						}

					} else {
						/* all LOCKPICKS are broken */
						print_msg_with_first_hero(get_ttx(531));
					}
				} else {
					/* the leader has no LOCKPICKS */
					print_msg_with_first_hero(get_ttx(530));
				}
			}

			if (action == ACTION_ID_ICON_9)
			{
				/* use magic */
				hero_pos = select_hero_ok(get_ttx(317));

				if (hero_pos != -1)
				{
					hero = get_hero(hero_pos);

					if (host_readbs(hero + HERO_TYPE) < HERO_TYPE_WITCH)
					{
						/* not a spellcaster */
						GUI_output(get_ttx(330));
					} else {
						spell_result = test_spell(hero, SP_FORAMEN_FORAMINOR, host_readbs((Bit8u*)ptr_doors + 4));

						if (spell_result == -99)
						{
							/* unlucky => just print a message */
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(607),
								(char*)hero + HERO_NAME2);

							GUI_output(Real2Host(ds_readd(DTP2)));
						} else if (spell_result <= 0)
						{
							/* failed => half AE costs */
							sub_ae_splash(hero, get_spell_cost(SP_FORAMEN_FORAMINOR, 1));

							play_voc(ARCHIVE_FILE_FX17_VOC);
						} else {
							play_voc(ARCHIVE_FILE_FX17_VOC);

							sub_ae_splash(hero, get_spell_cost(SP_FORAMEN_FORAMINOR, 0));

							/* success => the door opens */
							and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), 0x0f); /* clear higher 4 bits */
							or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), DNG_TILE_OPEN_DOOR << 4);
							/* note that the 'unlocked' flag ......1. is not explicitly set. It will be set if the party closes the door. */
							ds_writeb(STEPTARGET_FRONT, host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y)));
							DNG_open_door();

							add_hero_ap(hero, 1L); /* hero gets 1 AP for successful lock pick */

							ds_writebs((NEW_MENU_ICONS + 6), ds_writebs((NEW_MENU_ICONS + 7), ds_writebs((NEW_MENU_ICONS + 8), MENU_ICON_NONE)));
							ds_writew(REDRAW_MENUICONS, 1);
						}
					}
				}
			}
		}

	} while (host_readws((Bit8u*)(ptr_doors++)) != -1);
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
 * \brief   fallpit logic for the current group in dungeons
 *
 * \param   max_damage  maximum damage if a hero drops in the fallpit
 */
void DNG_fallpit_test(signed short max_damage)
{
	signed short i;
	Bit8u *hero;

	play_voc(ARCHIVE_FILE_FX18_VOC);

	and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(ds_readws(X_TARGET), ds_readws(Y_TARGET)), 0x0f); /* clear higher 4 bits */
	or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(ds_readws(X_TARGET), ds_readws(Y_TARGET)), DNG_TILE_PIT << 4);

	if (ds_readb(DUNGEON_LIGHT) != 0)
	{
		/* light is on */
		GUI_output(get_ttx(517));

		/* drop one level down */
		DNG_inc_level();

		and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(ds_readws(X_TARGET), ds_readws(Y_TARGET)), 0x0f); /* clear higher 4 bits */
		or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(ds_readws(X_TARGET), ds_readws(Y_TARGET)), DNG_TILE_PIT_IN_CEILING << 4);
		/* effect: 0101.... */

		/* damage the heroes */
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

			and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(ds_readws(X_TARGET), ds_readws(Y_TARGET)), 0x0f); /* clear higher 4 bits */
			or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(ds_readws(X_TARGET), ds_readws(Y_TARGET)), DNG_TILE_PIT_IN_CEILING << 4);

			/* move one level up. */
			dec_ds_bs_post(DUNGEON_LEVEL);

			ds_writews(X_TARGET, ds_readws(X_TARGET_BAK));
			ds_writews(Y_TARGET, ds_readws(Y_TARGET_BAK));

			load_area_description(1);

			DNG_update_pos();
		} else {
			and_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(ds_readws(X_TARGET), ds_readws(Y_TARGET)), 0x0f); /* clear higher 4 bits */
			or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(ds_readws(X_TARGET), ds_readws(Y_TARGET)), DNG_TILE_PIT_IN_CEILING << 4);
			/* effect: 0101.... */
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

	ds_writeb((NEW_MENU_ICONS + 0), MENU_ICON_SPLIT_GROUP);
	old_value = ds_readbs((NEW_MENU_ICONS + 1));
	ds_writeb((NEW_MENU_ICONS + 1), ds_readbs(CAN_MERGE_GROUP) == -1 ? MENU_ICON_MERGE_GROUP_GRAYED : MENU_ICON_MERGE_GROUP);

	if (ds_readbs((NEW_MENU_ICONS + 1)) != old_value)
	{
		ds_writew(REDRAW_MENUICONS, 1);
	}

	ds_writeb((NEW_MENU_ICONS + 2), MENU_ICON_SWITCH_GROUP);
	ds_writeb((NEW_MENU_ICONS + 3), MENU_ICON_INFO);
	ds_writeb((NEW_MENU_ICONS + 4), MENU_ICON_MAP);
	ds_writeb((NEW_MENU_ICONS + 5), MENU_ICON_MAGIC);

	if (ds_readw(DNG_MENU_MODE) == DNG_MENU_MODE_PLAIN && ds_readb((NEW_MENU_ICONS + 6)) != MENU_ICON_CAMP)
	{
		ds_writeb((NEW_MENU_ICONS + 6), MENU_ICON_CAMP);
		ds_writew(REDRAW_MENUICONS, 1);
	}

	if (ds_readw(REQUEST_REFRESH) != 0)
	{
		draw_main_screen();
		GUI_print_loc_line(get_tx(0));
		ds_writew(REQUEST_REFRESH, ds_writew(REDRAW_MENUICONS, 0));
		ds_writew(DNG_REFRESH_X_TARGET, -1);
	}

	if (ds_readw(REDRAW_MENUICONS) != 0 && ds_readb(PP20_INDEX) == 0)
	{
		draw_icons();
		ds_writew(REDRAW_MENUICONS, 0);
	}

	if (ds_readbs(DIRECTION) != ds_readws(DNG_REFRESH_DIRECTION) ||
		ds_readws(X_TARGET) != ds_readws(DNG_REFRESH_X_TARGET) ||
		ds_readws(Y_TARGET) != ds_readws(DNG_REFRESH_Y_TARGET))
	{
		DNG_update_pos();
		set_automap_tiles(ds_readws(X_TARGET), ds_readws(Y_TARGET));
		DNG_fight();
	}

	if (ds_readws(X_TARGET) != ds_readws(X_TARGET_BAK) ||
		ds_readws(Y_TARGET) != ds_readws(Y_TARGET_BAK) ||
		ds_readbs(DNG_LEVEL_CHANGED) != 0)
	{
		ds_writeb(CAN_MERGE_GROUP, (unsigned char)can_merge_group());
		ds_writew(LOCKPICK_TRY_COUNTER, 0);
	}

	ds_writew(X_TARGET_BAK, ds_readws(X_TARGET));
	ds_writew(Y_TARGET_BAK, ds_readws(Y_TARGET));
	ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));

	handle_gui_input();

	if (ds_readw(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP)
	{
		tw_bak = ds_readws(TEXTBOX_WIDTH);
		ds_writew(TEXTBOX_WIDTH, 3);

		for (l_di = retval = 0; l_di < 9; l_di++)
		{
			if (ds_readbs(NEW_MENU_ICONS + l_di) != MENU_ICON_NONE)
			{
				retval++;
			}
		}

		l_di = GUI_radio(get_ttx(534), (signed char)retval,
					get_ttx(535),
					get_ttx(536),
					get_ttx(537),
					get_ttx(538),
					get_ttx(539),
					get_ttx(213),
					ds_readws(DNG_MENU_MODE) == DNG_MENU_MODE_PLAIN ? get_ttx(306) :(
						ds_readws(DNG_MENU_MODE) == DNG_MENU_MODE_OPEN_DOOR ? get_ttx(540) :(
						ds_readws(DNG_MENU_MODE) == DNG_MENU_MODE_CLOSE_DOOR ? get_ttx(787) :(
						ds_readws(DNG_MENU_MODE) == DNG_MENU_MODE_UNLOCK_DOOR ? get_ttx(542) :(
						ds_readws(DNG_MENU_MODE) == 4 ? get_ttx(555) : get_ttx(541))))),
					get_ttx(543),
					get_ttx(544)) - 1;

		if (l_di != -2)
		{
			ds_writew(ACTION, l_di + ACTION_ID_ICON_1);
		}

		ds_writew(TEXTBOX_WIDTH, tw_bak);
	}

	l_di = 0;

	if (ds_readws(ACTION) == ACTION_ID_ICON_1)
	{
		GRP_split();
		ds_writeb(CAN_MERGE_GROUP, (unsigned char)can_merge_group());

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_2)
	{
		/* merge groups or reach hands through the mirror */
		pos = DNG_POS(ds_readbs(DUNGEON_LEVEL), ds_readws(X_TARGET), ds_readws(Y_TARGET));

		if ((ds_readb(DUNGEON_INDEX) == DUNGEONS_HYGGELIKS_RUINE && pos == DNG_POS(1,8,1)) || pos == DNG_POS(1,8,5))
		{
			GUI_output(get_tx(33));

			ds_writeb(DNG15_REACHED_HANDS, 1);
		} else {
			GRP_merge();
#ifdef M302de_ORIGINAL_BUGFIX
			/* Original-Bug 19: After merging groups in a dungeon, the icon "merge groups" is not grayed.
			 * Compare to the corresponding code in city_step() in seg066.cpp, where the following line is present. */
			ds_writeb(CAN_MERGE_GROUP, -1);
#endif
			/* TODO: if a "dark" group was merged with a "lighted" group, make group "lighted". */
		}

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_3)
	{
		GRP_switch_to_next(0);

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_4)
	{
		game_options();

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_5)
	{
		show_automap();

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_6)
	{
		if (select_magic_user() > 0)
		{
			ds_writew(DNG_REFRESH_DIRECTION, -1);
		}

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_7 && ds_readw(DNG_MENU_MODE) == DNG_MENU_MODE_PLAIN)
	{
		ds_writeb(LOCATION, LOCATION_CITYCAMP);
		ds_writeb(CITYCAMP_CITY, 0); /* CITYCAMP takes place in dungeon */
		l_di = 1;

	} else if (ds_readws(ACTION) == ACTION_ID_LEFT)
	{
		update_direction(3);
		ds_writebs((NEW_MENU_ICONS + 6), ds_writebs((NEW_MENU_ICONS + 7), ds_writebs((NEW_MENU_ICONS + 8), MENU_ICON_NONE)));

	} else if (ds_readws(ACTION) == ACTION_ID_RIGHT)
	{
		update_direction(1);
		ds_writebs((NEW_MENU_ICONS + 6), ds_writebs((NEW_MENU_ICONS + 7), ds_writebs((NEW_MENU_ICONS + 8), MENU_ICON_NONE)));

	} else if (ds_readws(ACTION) == ACTION_ID_UP)
	{
		if ((l_si = div16(ds_readb(STEPTARGET_FRONT))) == DNG_TILE_SEMIPERMEABLE_WALL)
		{
			l_si = 1 << ds_readbs(DIRECTION);

			if (ds_readb(STEPTARGET_FRONT) & l_si & 0x0f)
				/* can only be entered if flag no. <direction> is set. */
			{
				DNG_timestep(1);
			}

		} else if ((l_si = div16(ds_readb(STEPTARGET_FRONT))) != DNG_TILE_WALL &&
				l_si != DNG_TILE_CLOSED_DOOR && /* closed door */
				l_si != DNG_TILE_REMOVABLE_WALL &&
				l_si != DNG_TILE_CHEST &&
				l_si != DNG_TILE_BLOCKER)
		{
			DNG_timestep(1);
		} else {
			no_way();
		}

	} else if (ds_readws(ACTION) == ACTION_ID_DOWN)
	{
		if ((l_si = div16(ds_readb(STEPTARGET_BACK))) != DNG_TILE_WALL &&
				l_si != DNG_TILE_CLOSED_DOOR &&
				l_si != DNG_TILE_REMOVABLE_WALL &&
				l_si != DNG_TILE_CHEST &&
				l_si != DNG_TILE_BLOCKER)
		{
			DNG_timestep(-1);
		} else {
			no_way();
		}

	} else if (ds_readws(ACTION) >= ACTION_ID_ICON_7 &&
			ds_readws(ACTION) <= ACTION_ID_ICON_9 &&
			ds_readbs((NEW_MENU_ICONS - ACTION_ID_ICON_1) + ds_readws(ACTION)) != -1)
	{
		if (ds_readw(DNG_MENU_MODE) == DNG_MENU_MODE_OPEN_DOOR || ds_readw(DNG_MENU_MODE) == DNG_MENU_MODE_CLOSE_DOOR || ds_readw(DNG_MENU_MODE) == DNG_MENU_MODE_UNLOCK_DOOR)
		{
			DNG_door(ds_readws(ACTION));
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_7 && ds_readw(DNG_MENU_MODE) == DNG_MENU_MODE_OPEN_CHEST)
		{
			seg092_06b4(1);

			if (ds_readw(GET_EXTRA_LOOT) != 0)
			{
				x = ds_readws(X_TARGET);
				y = ds_readws(Y_TARGET);

				switch (ds_readbs(DIRECTION))
				{
					case NORTH: y--; break;
					case EAST:  x++; break;
					case SOUTH: y++; break;
					case WEST:  x--; break;
				}

				or_ptr_bs(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y), 0x02);
				/* set bit 1 'unlocked' */
			}
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_7 && (!ds_readb(DNG15_LEVER_SOUTH) || !ds_readb(DNG15_LEVER_NORTH)))
		{
			DNG15_riddle();
		}
	}

	if (ds_readb(DUNGEON_INDEX) != DUNGEONS_NONE &&
		ds_readbs(DNG_AREA_LOADED) != -1 &&
		!l_di)
	{
		DNG_see_stairs();
		DNG_see_door();
		DNG_see_chest();
		DNG_see_lever();

#if defined(__BORLANDC__)
		dungeon_handler = (signed short (*)(void))ds_readd((DNG_HANDLERS-4) + 4 * ds_readbs(DUNGEON_INDEX));
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

	target_pos = DNG_POS(ds_readbs(DUNGEON_LEVEL), ds_readws(X_TARGET), ds_readws(Y_TARGET));

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

	if ((l_si = div16(ds_readb(STEPTARGET_FRONT))) == 1 || l_si == 2)
	{
		/* standing direct in front of a door with view to it */
		if (ds_readbs((NEW_MENU_ICONS + 6)) != MENU_ICON_OPEN_CLOSE_DOOR && ds_readbs((NEW_MENU_ICONS + 6)) != MENU_ICON_SMASH_DOOR)
		{
			ds_writebs((NEW_MENU_ICONS + 6), MENU_ICON_OPEN_CLOSE_DOOR);
			ds_writew(REDRAW_MENUICONS, 1);
		}

		if (ds_readbs((NEW_MENU_ICONS + 6)) != MENU_ICON_SMASH_DOOR)
		{
			ds_writew(DNG_MENU_MODE, l_si == 1 ? DNG_MENU_MODE_OPEN_DOOR : DNG_MENU_MODE_CLOSE_DOOR);
		}

	} else {
		if (ds_readbs((NEW_MENU_ICONS + 6)) != MENU_ICON_NONE &&
			(ds_readws(DNG_MENU_MODE) == DNG_MENU_MODE_OPEN_DOOR || ds_readws(DNG_MENU_MODE) == DNG_MENU_MODE_CLOSE_DOOR || ds_readws(DNG_MENU_MODE) == DNG_MENU_MODE_UNLOCK_DOOR))
		{
			/* standing two fields before a door with view to it */
			ds_writebs((NEW_MENU_ICONS + 6), ds_writebs((NEW_MENU_ICONS + 7), ds_writebs((NEW_MENU_ICONS + 8), MENU_ICON_NONE)));
			ds_writew(REDRAW_MENUICONS, 1);
			ds_writew(DNG_MENU_MODE, DNG_MENU_MODE_PLAIN);
		}
	}
}

void DNG_see_chest(void)
{
	if (div16(ds_readb(STEPTARGET_FRONT)) == 8)
	{
		/* standing direct in front of a treasure chest with view to it */
		if (ds_readbs((NEW_MENU_ICONS + 6)) != MENU_ICON_OPEN_CHEST)
		{
			ds_writebs((NEW_MENU_ICONS + 6), MENU_ICON_OPEN_CHEST);
			ds_writew(REDRAW_MENUICONS, 1);
			ds_writew(DNG_MENU_MODE, DNG_MENU_MODE_OPEN_CHEST);
		}

	} else {
		if (ds_readbs((NEW_MENU_ICONS + 6)) != MENU_ICON_NONE && ds_readws(DNG_MENU_MODE) == DNG_MENU_MODE_OPEN_CHEST)
		{
			/* standing two fields before a treasure chest with view to it */
			ds_writebs((NEW_MENU_ICONS + 6), ds_writebs((NEW_MENU_ICONS + 7), ds_writebs((NEW_MENU_ICONS + 8), MENU_ICON_NONE)));
			ds_writew(REDRAW_MENUICONS, 1);
			ds_writew(DNG_MENU_MODE, DNG_MENU_MODE_PLAIN);
		}
	}
}

void do_dungeon(void)
{
	signed short tw_bak;

	if (ds_readbs(DNG_AREA_LOADED) != ds_readbs(DUNGEON_INDEX) || ds_readws(AREA_PREPARED) != 0 || ds_readws(DNG_INIT_FLAG) != 0)
	{
		ds_writed(DNG_MAP_PTR, (Bit32u)RealMake(datseg, DNG_MAP));

		seg028_0555(0);

		set_audio_track(ARCHIVE_FILE_DUNGEON_XMI);

		ds_writed(DNG_GFXTAB, (Bit32u)RealMake(datseg, (!ds_readb(DUNGEON_GFX_STYLE) ? DNG_GFXTAB_WOOD : (ds_readb(DUNGEON_GFX_STYLE) == 1 ? DNG_GFXTAB_MARBLE : DNG_GFXTAB_STONE))));

		ds_writew(DNG_INIT_FLAG, 0);
		ds_writew(REQUEST_REFRESH, 1);
	}

	ds_writew(CURRENT_ANI, -1);

	ds_writeb(DUNGEON_INDEX_BAK, ds_readbs(DUNGEON_INDEX));

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

	target_pos = DNG_POS(ds_readbs(DUNGEON_LEVEL), ds_readws(X_TARGET), ds_readws(Y_TARGET));

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
			/* set positions of heroes which escape from the fight */
			ds_writew((FIG_FLEE_POSITION + 0), host_readws((Bit8u*)fight_ptr + 4));
			ds_writew((FIG_FLEE_POSITION + 2), host_readws((Bit8u*)fight_ptr + 6));
			ds_writew((FIG_FLEE_POSITION + 4), host_readws((Bit8u*)fight_ptr + 8));
			ds_writew((FIG_FLEE_POSITION + 6), host_readws((Bit8u*)fight_ptr + 10));

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
 * \brief   the group finds a barrel of water
 *
 * \param   unit_ptr    pointer to the number of water units
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
			(char*)get_ttx(781),
			host_readb(unit_ptr));

		answer = GUI_radio(Real2Host(ds_readd(DTP2)), 3,
						get_ttx(782),
						get_ttx(783),
						get_ttx(784));

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
					/* +9 means: round up */

					if (host_readb(unit_ptr) <= units_needed)
					{
						/* not enough units in the barrel for this hero */
						sub_ptr_bs(hero + HERO_THIRST, host_readb(unit_ptr) * 10);

						if (host_readbs(hero + HERO_THIRST) < 0)
						{
							host_writeb(hero + HERO_THIRST, 0);
						}

						host_writeb(unit_ptr, 0);

						GUI_output(get_ttx(785));

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
					for (item_pos = hero_refilled = 0; item_pos < NR_HERO_INVENTORY_SLOTS; item_pos++)
					{
						if (host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * item_pos) == ITEM_WATERSKIN)
						{
							units_needed = 0;

							if (inventory_half_empty(hero + HERO_INVENTORY + SIZEOF_INVENTORY * item_pos))
							{
								units_needed = 1;

							} else if (inventory_empty(hero + HERO_INVENTORY + SIZEOF_INVENTORY * item_pos))
							{
								units_needed = 2;
							}

							if (units_needed != 0)
							{
								hero_refilled = 1;

								/* refill waterskin */
#if !defined(__BORLANDC__)
								and_ptr_bs(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * item_pos, 0xfb); /* unset 'empty' flag */
								and_ptr_bs(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * item_pos, 0xfd); /* unset 'half_empty' flag */
#else
								(*(struct inventory_flags*)(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * item_pos)).half_empty =
									(*(struct inventory_flags*)(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * item_pos)).empty = 0;
#endif

								if (host_readb(unit_ptr) <= units_needed)
								{
									/* empty the barrel completely */
									GUI_output(get_ttx(785));
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
				GUI_output(get_ttx(786));
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

	target_pos = DNG_POS(ds_readbs(DUNGEON_LEVEL), ds_readws(X_TARGET), ds_readws(Y_TARGET));

	if (ds_readbs(DUNGEON_INDEX) == DUNGEONS_HYGGELIKS_RUINE &&
		(target_pos == DNG_POS(1,8,1) || target_pos == DNG_POS(1,8,5)) &&
		(!ds_readb(DNG15_LEVER_SOUTH) || !ds_readb(DNG15_LEVER_NORTH)))
	{
		if (ds_readbs((NEW_MENU_ICONS + 6)) == MENU_ICON_NONE)
		{
			ds_writeb((NEW_MENU_ICONS + 6), MENU_ICON_MOVE_LEVER);
			ds_writew(REDRAW_MENUICONS, 1);
			ds_writew(DNG_MENU_MODE, DNG_MENU_MODE_LEVER);
		}

	} else if (ds_readbs((NEW_MENU_ICONS + 6)) != MENU_ICON_NONE && ds_readw(DNG_MENU_MODE) == DNG_MENU_MODE_LEVER)
	{
			ds_writeb((NEW_MENU_ICONS + 6), MENU_ICON_NONE);
			ds_writew(REDRAW_MENUICONS, 1);
			ds_writew(DNG_MENU_MODE, DNG_MENU_MODE_PLAIN);
	}
}

#if !defined(__BORLANDC__)
}
#endif
