/**
 *	Rewrite of DSA1 v3.02_de functions of seg074 (automap)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg074.cpp
 */

#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg029.h"
#if !defined(__BORLANDC__)
#include "seg066.h"
#else
/* BC: only the same code is produced, when this prototype is known */
signed short get_border_index(unsigned short);
#endif
#include "seg074.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void show_automap(void)
{
	signed short l_si;
	signed short l_di;
	signed short loc_bak;
	signed short done;
	signed short dungeon;
	signed short town;
	signed short textbox_width_bak;

	if (!ds_readb(DUNGEON_LIGHT) ||
		((ds_readbs(DUNGEON_LIGHT) == 1) && (test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 6) > 0)))
	{

		ds_writeb(SPECIAL_SCREEN, 1);

		dungeon = ds_readbs(DUNGEON_INDEX);
		town = ds_readbs(CURRENT_TOWN);

		ds_writeb(CURRENT_TOWN, ds_writeb(DUNGEON_INDEX, 0));

		textbox_width_bak = ds_readw(TEXTBOX_WIDTH);
		ds_writew(TEXTBOX_WIDTH, 3);

		l_si = (ds_readb(DNG_MAP_SIZE) == 16) ? 0 :
				((ds_readws(X_TARGET) - 8 < 0) ? 0 :
				((ds_readws(X_TARGET) - 8 > 15) ? 16 : ds_readws(X_TARGET) - 8));

		ds_writeb(CURRENT_TOWN, (signed char)town);
		ds_writeb(DUNGEON_INDEX, (signed char)dungeon);

		ds_writew(REQUEST_REFRESH, 1);

		do {

			if (ds_readw(REQUEST_REFRESH) != 0) {
				loc_bak = ds_readbs(LOCATION);
				ds_writeb(LOCATION, 1);

				draw_main_screen();
				ds_writeb(LOCATION, (signed char)loc_bak);

				if (ds_readb(DNG_MAP_SIZE) == 16) {
					draw_loc_icons(1, MENU_ICON_LEAVE);
				} else {
					draw_loc_icons(3, MENU_ICON_SCROLL_LEFT, MENU_ICON_SCROLL_RIGHT, MENU_ICON_LEAVE);
				}

				render_automap(l_si);
				clear_ani_pal();
				draw_automap_to_screen();
				set_ani_pal(p_datseg + PALETTE_FIGHT2);
				ds_writew(REQUEST_REFRESH, 0);
			}

			done = 0;

			handle_gui_input();

			if ((ds_readw(MOUSE2_EVENT) != 0) || (ds_readw(ACTION) == ACTION_ID_PAGE_UP)) {

				if (ds_readb(DNG_MAP_SIZE) == 16) {
					l_di = GUI_radio(get_ttx(612), 1, get_ttx(613)) - 1;
				} else {
					ds_writew(MENU_DEFAULT_SELECT, 2);
					l_di = GUI_radio(get_ttx(612), 3,
								 get_ttx(614),
								 get_ttx(615),
								 get_ttx(613)) - 1;
				}

				if (l_di != -2) {
					ds_writew(ACTION, l_di + ACTION_ID_ICON_1);
				}
			}

			if (ds_readb(DNG_MAP_SIZE) != 16) {

				if ((ds_readws(ACTION) == ACTION_ID_ICON_1) || (ds_readws(ACTION) == ACTION_ID_LEFT)) {

					if (l_si > 0) {
						render_automap(--l_si);
						draw_automap_to_screen();
					}
				}

				if ((ds_readws(ACTION) == ACTION_ID_ICON_2) || (ds_readws(ACTION) == ACTION_ID_RIGHT)) {

					if (l_si < 16) {
						render_automap(++l_si);
						draw_automap_to_screen();
					}
				}
			}

			if (((ds_readws(ACTION) == ACTION_ID_ICON_1) && (ds_readb(DNG_MAP_SIZE) == 16)) ||
				((ds_readws(ACTION) == ACTION_ID_ICON_3) && (ds_readb(DNG_MAP_SIZE) != 16)))
			{
				done = 1;
			}
		} while (done == 0);

		ds_writew(TEXTBOX_WIDTH, textbox_width_bak);
		ds_writew(REQUEST_REFRESH, 1);

		clear_ani_pal();

	} else {
		GUI_output(get_ttx(772));
	}
}

unsigned short is_discovered(signed short x, signed short y)
{
	return ds_readb(AUTOMAP_BUF + (4 * y) + (x >> 3)) & (ds_readb(AUTOMAP_BITMASK + (x & 7)));
}

/**
 * \brief   get value of automap field from small (16x16) maps
 *
 * \param   x           x-coordinate
 * \param   y           y-coordinate
 * \return              value of the field at (x,y)
 */
unsigned short get_mapval_small(signed short x, signed short y)
{
	Bit8u *map = p_datseg + DNG_MAP;

	return host_readb(map + 16 * y + x);
}

/**
 * \brief   get value of automap field from small (32x16) maps
 *
 * \param   x           x-coordinate
 * \param   y           y-coordinate
 * \return              value of the field at (x,y)
 */
unsigned short get_mapval_large(signed short x, signed short y)
{
	Bit8u *map = p_datseg + DNG_MAP;

	return host_readb(map + 32 * y + x);
}

/**
 * \brief   checks if the group is in prison
 *
 * \param   group_no    number of the group
 * \return              the value of the "in_prison" flag
 */
signed short is_group_in_prison(signed short group_no)
{
	Bit8u *hero = get_hero(0);
	signed short i;

	for (i = 0; i < 6; i++, hero += SIZEOF_HERO) {

		if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readbs(hero + HERO_GROUP_NO) == group_no))
		{
			return host_readbs(hero + HERO_JAIL);
		}
	}

	return 0;
}

/**
 * \brief   render the automap in RENDERBUF
 *
 * \param   x_off           x offset for vertical scroll
 */
void render_automap(signed short x_off)
{
	signed short tile_type;
	signed short group_i;
	signed short x;
	signed short y;
	signed short entrance_dir;

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 6);
	ds_writew(PIC_COPY_Y2, 6);

	/* set buffer to 0 */
	memset(Real2Host(ds_readd(RENDERBUF_PTR)), 0, 64000);

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (is_discovered(x + x_off, y)) {

				if (ds_readbs(DUNGEON_INDEX) != DUNGEONS_NONE) {
					/* in dungeon */

					tile_type = div16(get_mapval_small(x, y));

					draw_automap_square(x, y,
						(tile_type <= DNG_TILE_CORRIDOR)? MAP_TILE_DARK_GREY :
							(((tile_type == DNG_TILE_CLOSED_DOOR) || (tile_type == DNG_TILE_SMASHED_DOOR) || tile_type == DNG_TILE_OPEN_DOOR) ? MAP_TILE_DARK_RED :
							((tile_type == DNG_TILE_STAIR_UP) ? MAP_TILE_LIGHT_BLUE :
							((tile_type == DNG_TILE_STAIR_DOWN) ? MAP_TILE_BLUE :
							((tile_type == DNG_TILE_PIT_IN_CEILING) ? MAP_TILE_BRIGHT_GREEN :
							((tile_type == DNG_TILE_CHEST) ? MAP_TILE_BROWN :
							((tile_type == DNG_TILE_PIT) ? MAP_TILE_DARK_GREEN : MAP_TILE_RED)))))), -1);

				} else {
					/* in a town */

					if (!(tile_type = get_maploc(x + x_off, y))) {
						tile_type = get_border_index((ds_readb(DNG_MAP_SIZE) == 16) ?
										get_mapval_small(x, y) :
										get_mapval_large(x + x_off, y));
					}

					draw_automap_square(x, y,
						(tile_type <= TOWN_TILE_STREET)? MAP_TILE_DARK_GREY :
							((tile_type == TOWN_TILE_WATER) ? MAP_TILE_BLUE :
							((tile_type == TOWN_TILE_GRASS) ? MAP_TILE_GREEN :
							((tile_type == TOWN_TILE_SIGNPOST) ? MAP_TILE_RED :
							((tile_type == TOWN_TILE_TEMPLE) ? MAP_TILE_ORANGE :
							((tile_type == TOWN_TILE_INN_OR_TAVERN) ? MAP_TILE_LIGHT_BLUE :
							((tile_type == TOWN_TILE_MERCHANT) ? MAP_TILE_GREY : /* The lightouse on Runin is also displayed in Gray */
							((tile_type == TOWN_TILE_SMITH) ? MAP_TILE_DARK_GREEN :
							((tile_type == TOWN_TILE_HEALER) ? MAP_TILE_PINK :
							((tile_type == TOWN_TILE_BLACK_FINGER) ? MAP_TILE_DARK_PURPLE :
							(((tile_type >= TOWN_TILE_HOUSE_1) && (tile_type <= TOWN_TILE_HOUSE_4)) ? MAP_TILE_DARK_RED : MAP_TILE_BLACK)))))))))), -1);

					if ((tile_type != TOWN_TILE_STREET) && (tile_type != TOWN_TILE_GRASS) && (tile_type != TOWN_TILE_WATER) && (tile_type != TOWN_TILE_SIGNPOST)) {

						entrance_dir = (ds_readb(DNG_MAP_SIZE) == 16) ?
										get_mapval_small(x, y) :
										get_mapval_large(x + x_off, y);

						entrance_dir &= 3;
						draw_automap_entrance(x, y, entrance_dir);
					}
				}
			}
		}
	}

#ifndef M302de_ORIGINAL_BUGFIX
	/* draw yellow arrow at the position of the active group */

	/* Original-Bug 31: If there is another group on the same map square as the active group, the automap will mark that square by a purple arrow (for the other group) instead of a yellow one (for the active group).
	 * The reason is that the yellow arrow will be overdrawn by the purple ones, which are drawn later. */
	if (((ds_readws(X_TARGET) - x_off) >= 0) && ((ds_readws(X_TARGET) - x_off) <= 16)) { /* shouldn't this always be true? */

		draw_automap_square(ds_readws(X_TARGET) - x_off, ds_readws(Y_TARGET),
					MAP_TILE_YELLOW_ARROW, ds_readbs(DIRECTION));
	}
#endif

	/* draw purple arrows at the positions of other groups */
	for (group_i = 0; group_i < 6; group_i++) {

		if ((ds_readbs(CURRENT_GROUP) != group_i) &&
			(ds_readbs(GROUP_MEMBER_COUNTS + group_i) > 0) &&
			(ds_readb(GROUPS_DNG_LEVEL + group_i) == ds_readbs(DUNGEON_LEVEL)) &&
			(ds_readb(GROUPS_TOWN + group_i) == ds_readbs(CURRENT_TOWN)) &&
			(ds_readb(GROUPS_DNG_INDEX + group_i) == ds_readbs(DUNGEON_INDEX)) &&
			!is_group_in_prison(group_i) &&
			(ds_readws(GROUPS_X_TARGET + 2 * group_i) - x_off >= 0) &&
			(ds_readws(GROUPS_X_TARGET + 2 * group_i) - x_off <= 16))
		{
			draw_automap_square(ds_readws(GROUPS_X_TARGET + 2 * group_i) - x_off,
					ds_readws(GROUPS_Y_TARGET + 2 * group_i),
					MAP_TILE_PURPLE_ARROW,
					ds_readbs(GROUPS_DIRECTION + group_i));
		}
	}

#ifdef M302de_ORIGINAL_BUGFIX
	/* draw yellow arrow at the position of the active group */

	/* Original-Bug 31: see above.
	 * Fix: Move the code block drawing the yellow arrow after the one drawing the purple arrows. */
	if (((ds_readws(X_TARGET) - x_off) >= 0) && ((ds_readws(X_TARGET) - x_off) <= 16)) { /* shouldn't this always be true? */

		draw_automap_square(ds_readws(X_TARGET) - x_off, ds_readws(Y_TARGET),
					MAP_TILE_YELLOW_ARROW, ds_readbs(DIRECTION));
	}
#endif

	/* In the target selector screen of the Transversalis spell, mark the target with a cross */
	if (((ds_readws(AUTOMAP_SELX) - x_off) >= 0) && ((ds_readws(AUTOMAP_SELX) - x_off) <= 16)) {

		draw_automap_square(ds_readws(AUTOMAP_SELX) - x_off,	ds_readws(AUTOMAP_SELY), MAP_TILE_CROSS, -1);
	}
}

/**
 * \brief   draws a building on the automap
 *
 * \param   x           x-coordinate on the automap
 * \param   y           y-coordiante on the automap
 * \param   color       the color
 * \param   dir         direction of the entrance, -1 for none
 */
void draw_automap_square(signed short x, signed short y, signed short color, signed short dir)
{
	signed short i;
	unsigned short offset_y;
	RealPt p_img_tile;
	signed char tile[50];

	offset_y = y;
	offset_y <<= 3;
	offset_y *= 320;

	p_img_tile = (RealPt)ds_readd(RENDERBUF_PTR) + offset_y + 8 * x + 0xca8;

	for (i = 0; i < 49; i++) {
		tile[i] = (signed char)color;
	}

	if ((color == 4) || (color == 16)) {

		if (dir == 0) {

			memcpy(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), p_datseg + AUTOMAP_TILE_ARROWUP, 49);

		} else if (dir == 2) {

			for (i = 0; i < 49; i++) {
				host_writeb(Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + i, ds_readb(AUTOMAP_TILE_ARROWUP + (48 - i)));
			}

		} else if (dir == 1) {

			memcpy(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), p_datseg + AUTOMAP_TILE_ARROWRIGHT, 49);

		} else {

			for (i = 0; i < 49; i++) {
				host_writeb(Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + i, ds_readb(AUTOMAP_TILE_ARROWRIGHT + (48 - i)));
			}
		}

		for (i = 0; i < 49; i++) {
			if (!host_readbs(Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + i)) {
				tile[i] = 0;
			}
		}
	}

	if (color == 7) {

		for (i = 0; i < 49; i++) {
			if (!ds_readb(AUTOMAP_TILE_CROSS + i)) {
				tile[i] = 0;
			} else {
				tile[i] = (signed char)color;
			}
		}
	}

	ds_writed(PIC_COPY_DST, (Bit32u)p_img_tile);

#if !defined(__BORLANDC__)
	/* need 50 bytes on the DOSBox-Stack */
	reg_esp -= 50;

	/* make a pointer to this position */
	Bit8u *p = Real2Host(RealMake(SegValue(ss), reg_sp));

	/* copy the tile from host stack to DOSBox stack */
	for (i = 0; i < 50; i++, p++) {
		host_writeb(p, tile[i]);
	}

	/* save the pointer as the graphic source */
	ds_writed(PIC_COPY_SRC, (Bit32u)RealMake(SegValue(ss), reg_sp));

	/* free 50 bytes */
	reg_esp += 50;
#else
	ds_writed(PIC_COPY_SRC, (Bit32u)&tile);
#endif
	/* */
	do_pic_copy(0);

}

/**
 * \brief   draws the entrance of a automap building
 *
 * \param   x           x-coordinate of the building
 * \param   y           y-coordinate of the building
 * \param   dir         direction of the entrance, 0 = NORTH, 1 = EAST,...
 */
void draw_automap_entrance(signed short x, signed short y, signed short dir)
{
	unsigned short offset_y = y;
	signed short d = dir;
	signed short skipsize;
	Bit8u *p_img_tile;

	offset_y <<= 3;
	offset_y *= 320;

	p_img_tile = Real2Host(ds_readd(RENDERBUF_PTR)) + offset_y + 8 * x + 0xca8;

	if (!d) {
		p_img_tile += 2;
		skipsize = 1;
	} else if (d == 1) {
		p_img_tile += 646;
		skipsize = 320;
	} else if (d == 2) {
		p_img_tile += 1922;
		skipsize = 1;
	} else {
		p_img_tile += 640;
		skipsize = 320;
	}

	/* set 3 pixel to black */
	host_writeb(p_img_tile, 0);
	host_writeb(p_img_tile + skipsize, 0);
	host_writeb(p_img_tile + skipsize + skipsize, 0);
}

struct coords {
	signed short x, y;
};

struct dummy {
	struct coords a[2];
};

void draw_automap_to_screen(void)
{
	struct dummy bak;

	/* save screen coordinates */
	bak = *(struct dummy*)(p_datseg + PIC_COPY_DS_RECT);

	/* set the screen coordinates */
	ds_writew((PIC_COPY_DS_RECT + 2), ds_writew(PIC_COPY_DS_RECT, 0));
	ds_writew((PIC_COPY_DS_RECT + 6), ds_readw(ANI_POSX) + 208);
	ds_writew((PIC_COPY_DS_RECT + 4), ds_readw(ANI_POSY) + 135);

	ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 319);
	ds_writew(PIC_COPY_Y2, 134);

	ds_writed(PIC_COPY_DST, (Bit32u)((RealPt)ds_readd(FRAMEBUF_PTR) + ds_readws(ANI_POSX) + 320 * ds_readws(ANI_POSY)));

	update_mouse_cursor();

	ds_writew(PIC_COPY_FLAG, 1);
	do_pic_copy(1);
	ds_writew(PIC_COPY_FLAG, 0);

	refresh_screen_size();

	ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

	/* restore screen coordinates */
	*(struct dummy*)(p_datseg + PIC_COPY_DS_RECT) = bak;
}

/**
 * \brief   select a teleport destination
 *
 * \return              costs for teleport in AE
 */
signed short select_teleport_dest(void)
{
	signed short l_si;
	signed short l_di;

	signed short answer;
	signed short done;
	signed short dungeon;
	signed short town;
	signed short ae_costs;
	signed short textbox_width_bak;

	draw_main_screen();

	dungeon = ds_readbs(DUNGEON_INDEX);
	town = ds_readbs(CURRENT_TOWN);
	ds_writeb(CURRENT_TOWN, ds_writeb(DUNGEON_INDEX, 0));

	l_si = ((ds_readb(DNG_MAP_SIZE) == 16) ? 0 :
			((ds_readws(X_TARGET) - 8 < 0) ? 0 :
			((ds_readws(X_TARGET) - 8 > 15) ? 16 : ds_readws(X_TARGET) - 8)));

	ds_writew(AUTOMAP_SELX, ds_readws(X_TARGET));
	ds_writew(AUTOMAP_SELY, ds_readws(Y_TARGET));
	ds_writeb(DUNGEON_INDEX, (signed char)dungeon);
	ds_writeb(CURRENT_TOWN, (signed char)town);
	textbox_width_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);

	render_automap(l_si);

	clear_ani_pal();

	draw_automap_to_screen();

	set_ani_pal(p_datseg + PALETTE_FIGHT2);

	if (ds_readb(DNG_MAP_SIZE) == 16) {
		draw_loc_icons(1, MENU_ICON_MAGIC);
	} else {
		draw_loc_icons(3, MENU_ICON_SCROLL_LEFT, MENU_ICON_SCROLL_RIGHT, MENU_ICON_MAGIC);
	}

	done = 0;

	do {
		handle_input();

		if ((ds_readw(MOUSE2_EVENT) != 0) || (ds_readw(ACTION) == ACTION_ID_PAGE_UP)) {

			if (ds_readb(DNG_MAP_SIZE) == 16) {
				answer = GUI_radio(get_ttx(616), 1, get_ttx(617)) - 1;
			} else {
				answer = GUI_radio(get_ttx(616), 3,
							 get_ttx(614),
							 get_ttx(615),
							 get_ttx(617)) - 1;
			}

			if (answer != -2) {
				ds_writew(ACTION, answer + ACTION_ID_ICON_1);
			}
		}

		if ((ds_readw(ACTION) == ACTION_ID_LEFT) &&
			(ds_readws(AUTOMAP_SELX) > 0) &&
			is_discovered(ds_readws(AUTOMAP_SELX) - 1, ds_readws(AUTOMAP_SELY)))
		{
			dec_ds_ws(AUTOMAP_SELX);
			render_automap(l_si);
			draw_automap_to_screen();

		} else if ((ds_readw(ACTION) == ACTION_ID_UP) &&
			(ds_readws(AUTOMAP_SELY) > 0) &&
			is_discovered(ds_readws(AUTOMAP_SELX), ds_readws(AUTOMAP_SELY) - 1))
		{
			dec_ds_ws(AUTOMAP_SELY);
			render_automap(l_si);
			draw_automap_to_screen();

		} else if ((ds_readw(ACTION) == ACTION_ID_RIGHT) &&
			(ds_readb(DNG_MAP_SIZE) - 1 > ds_readws(AUTOMAP_SELX)) &&
			is_discovered(ds_readws(AUTOMAP_SELX) + 1, ds_readws(AUTOMAP_SELY)))
		{
			inc_ds_ws(AUTOMAP_SELX);
			render_automap(l_si);
			draw_automap_to_screen();

		} else if ((ds_readw(ACTION) == ACTION_ID_DOWN) &&
			(ds_readws(AUTOMAP_SELY) < 16) &&
			is_discovered(ds_readws(AUTOMAP_SELX), ds_readws(AUTOMAP_SELY) + 1))
		{
			inc_ds_ws(AUTOMAP_SELY);
			render_automap(l_si);
			draw_automap_to_screen();
		}

		if (ds_readb(DNG_MAP_SIZE) != 16) {

			if ((ds_readw(ACTION) == ACTION_ID_ICON_1) && (l_si > 0)) {
				render_automap(--l_si);
				draw_automap_to_screen();
			}

			if ((ds_readw(ACTION) == ACTION_ID_ICON_2) && (l_si < 16)) {
				render_automap(++l_si);
				draw_automap_to_screen();
			}
		}

		if (((ds_readw(ACTION) == ACTION_ID_ICON_1) && (ds_readb(DNG_MAP_SIZE) == 16)) ||
			((ds_readw(ACTION) == ACTION_ID_ICON_3) && (ds_readb(DNG_MAP_SIZE) != 16)))
		{
			done = 1;
		}


	} while (done == 0);

	l_di = (ds_readb(DNG_MAP_SIZE) == 16) ?
		get_mapval_small(ds_readws(AUTOMAP_SELX), ds_readws(AUTOMAP_SELY)) :
		get_mapval_large(ds_readws(AUTOMAP_SELX), ds_readws(AUTOMAP_SELY));

	if (ds_readbs(CURRENT_TOWN) != TOWNS_NONE) {
		l_di = get_border_index(l_di);
	} else {
		l_di = div16(l_di);
	}

	ae_costs = 0;

	if ((ds_readws(AUTOMAP_SELX) == ds_readws(X_TARGET)) &&
		(ds_readws(AUTOMAP_SELY) == ds_readws(Y_TARGET)))
	{
		ae_costs = 0;
		host_writeb(Real2Host(ds_readd(DTP2)), 0);

	} else if (((ds_readbs(DUNGEON_INDEX) != 0) && (l_di == 15)) ||
			((ds_readbs(CURRENT_TOWN) != TOWNS_NONE) && (((l_di >= 2) && (l_di <= 5)) ||
			(l_di == 6))))
	{
		strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(611));
		ae_costs = -2;
	} else {
		host_writeb(Real2Host(ds_readd(DTP2)), 0);
		ae_costs = 15;
	}

	ds_writew(TEXTBOX_WIDTH, textbox_width_bak);
	ds_writew(REQUEST_REFRESH, 1);

	return ae_costs;
}

/**
 * \brief   get location type at given position on map
 *
 * \param   x           x-coordinate of the building
 * \param   y           y-coordinate of the building
 * \return              type of location
 */
signed short get_maploc(signed short x, signed short y)
{
	Bit8u *p_loc;
	unsigned short pos_xy = (x << 8) + y;

	if (ds_readbs(CURRENT_TOWN) == TOWNS_THORWAL) {

		if (pos_xy == (4 << 8) + 13) {
			return 13;
		} else if (pos_xy == (5 << 8) + 2) {
			return 8;
		} else if ((pos_xy == ( 5 << 8) + 1)
		        || (pos_xy == ( 5 << 8) + 4) || (pos_xy == ( 3 << 8) +  6)
		        || (pos_xy == (13 << 8) + 8) || (pos_xy == (20 << 8) + 11)
		        || (pos_xy == ( 5 << 8) + 5) || (pos_xy == ( 3 << 8) + 10))
		{
			return 9;
		}
	} else if (ds_readbs(CURRENT_TOWN) == TOWNS_PREM) {
		if (pos_xy == (28 << 8) + 9) {
			return 9;
		}
	} else if (ds_readbs(CURRENT_TOWN) == TOWNS_GUDDASUNDEN) {
		if (pos_xy == (1 << 8) + 14) {
			return 8;
		}
	}

	p_loc = p_datseg + LOCATIONS_TAB;

	do {

		if (host_readws(p_loc) == pos_xy) {

			if (host_readbs(p_loc + 2) == 2) {
				return 1;
			}
			if (host_readbs(p_loc + 2) == 5) {
				return 10;
			}
			if (host_readbs(p_loc + 2) == 8) {
				return 11;
			}
			if ((host_readbs(p_loc + 2) == 3) || (host_readbs(p_loc + 2) == 7)) {
				return 9;
			}
			if (host_readbs(p_loc + 2) == 4) {
				return 12;
			}
		}

		p_loc += 6;
	} while (host_readws(p_loc) != -1);

	return 0;
}

#if !defined(__BORLANDC__)
}
#endif
