/**
 *	Rewrite of DSA1 v3.02_de functions of seg093 (travelmode)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg093.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg025.h"
#include "seg028.h"
#include "seg075.h"
#include "seg094.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short do_travel_mode(void)
{
	signed short l_di;
	signed short i;
	signed short answer;
	signed short l4;
	signed short route_id;
	Bit8u *dir_sign_ptr;
	signed short l6;
	signed short l7;
	signed short bak1;
	signed short tw_bak;
	Bit8u *destinations_tab[6];

	bak1 = ds_readws(WALLCLOCK_UPDATE);
	ds_writeb(ROUTE59_FLAG, (unsigned char)ds_writew(WALLCLOCK_UPDATE, (unsigned short)ds_writeb(TRAVEL_DETOUR, 0)));
	ds_writeb(CURRENT_TOWN, ds_readbs(TYPEINDEX));

	update_mouse_cursor();

	for (i = 0; i < 6; i++)
	{
		ds_writeb(FOOD_MESSAGE_SHOWN + i, 0);
	}

	if (ds_readb(TRAVEL_MAP_LOADED) != ds_readb(SHOW_TRAVEL_MAP))
	{
		load_map();
	}

	memmove((void*)Real2Host(ds_readd(RENDERBUF_PTR)), (void*)Real2Host(ds_readd(TRAVEL_MAP_PTR)), 64000);

	map_effect(Real2Host(ds_readd(RENDERBUF_PTR)));

	wait_for_vsync();

	set_palette(Real2Host(ds_readd(TRAVEL_MAP_PTR)) + 64000 + 2, 0, 0x20);

	refresh_screen_size();

	prepare_map_marker();

	ds_writew(MOUSE1_EVENT1, 0);

	dir_sign_ptr = p_datseg + DIRECTION_SIGNS;

	do {
		if (ds_readws(REQUEST_REFRESH) != 0)
		{
			update_mouse_cursor();

			if (ds_readb(TRAVEL_MAP_LOADED) != ds_readb(SHOW_TRAVEL_MAP))
			{
				load_map();
			}

			wait_for_vsync();

			bc_memmove((RealPt)ds_readd(FRAMEBUF_PTR), (RealPt)ds_readd(TRAVEL_MAP_PTR), 64000);

			wait_for_vsync();

			set_palette(Real2Host(ds_readd(TRAVEL_MAP_PTR)) + 64000 + 2, 0, 0x20);

			refresh_screen_size();

			ds_writew(REQUEST_REFRESH, 0);
		}

		if (host_readbs(dir_sign_ptr) == ds_readbs(CURRENT_TOWN) && host_readb(dir_sign_ptr + 1) == ds_readw(CURRENT_DIRSIGN))
		{
			while (1) {
				handle_input();

				if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP)
				{
					i = 0;
					while ((l_di = host_readb(Real2Host(host_readd(dir_sign_ptr + 2)) + i)) != 255)
					{
						destinations_tab[i] = get_ttx(235 + ds_writebs(TRV_MENU_TOWNS + i,
						    (answer = ds_readb((ROUTES_TAB - 9) + 9 * l_di)) != ds_readbs(CURRENT_TOWN) ?
						    (signed char) answer : ds_readbs((ROUTES_TAB - 9 + 1) + 9 * l_di)));

						i++;
					}

					ds_writeb(TRV_MENU_TOWNS + i, ds_readbs(CURRENT_TOWN));
					destinations_tab[i] = get_ttx(613);
					i++;

					sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_ttx(545),
						(char*)get_ttx(235 + ds_readbs(CURRENT_TOWN)));

					tw_bak = ds_readws(TEXTBOX_WIDTH);
					ds_writew(TEXTBOX_WIDTH, 4);

					answer = GUI_radio(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (signed char)i,
								destinations_tab[0],
								destinations_tab[1],
								destinations_tab[2],
								destinations_tab[3],
								destinations_tab[4],
								destinations_tab[5]) - 1;

					ds_writew(TEXTBOX_WIDTH, tw_bak);

					ds_writew(CURRENT_TOWN_ANIX, 0);

					set_and_spin_lock();

					if (i - 1 == answer || answer == -2)
					{
						ds_writeb(SHOW_TRAVEL_MAP, 0);
						ds_writeb(DIRECTION, (ds_readb(DIRECTION) + 2) & 3);
						break;
					}

					route_id = host_readb(Real2Host(host_readd(dir_sign_ptr + 2)) + answer);
					ds_writew(TRV_DESTINATION, ds_readbs(TRV_MENU_TOWNS + answer));

					if (!get_current_season() &&
						(route_id == 31 || route_id == 41 || route_id == 47 || route_id == 48 || route_id == 49))
					{
						/* routes not available in winter */
						GUI_input(get_tx(69), 0);
						break;
					}

					ds_writew(WALLCLOCK_X, ds_readws(BASEPOS_X) + 120);
					ds_writew(WALLCLOCK_Y, ds_readws(BASEPOS_Y) + 87);

					ds_writew(WALLCLOCK_UPDATE, 1);

					TM_func1(host_readb(Real2Host(host_readd(dir_sign_ptr + 2)) + answer),
						(ds_readbs((ROUTES_TAB - 9) + 9 * host_readb(Real2Host(host_readd(dir_sign_ptr + 2)) + answer)) == ds_readbs(CURRENT_TOWN) ? 0 : 1));
					ds_writew(WALLCLOCK_UPDATE, 0);

					if (ds_readb(ROUTE59_FLAG) != 0)
					{
						TM_func9();
					}

					if (ds_readw(TRV_RETURN) == 2)
					{
						if (ds_readb(TRAVEL_DETOUR) != 0 && ds_readb(TRAVEL_DETOUR) != 99)
						{
							DNG_enter_dungeon(ds_readb(TRAVEL_DETOUR));
						}
						break;
					}

					TM_enter_target_town();

					if (!ds_readb(TRAVEL_DETOUR) && ds_readw(GAME_STATE) == GAME_STATE_MAIN)
					{
						ds_writeb(CURRENT_TOWN, (signed char)ds_readw(TRV_DEST_REACHED));
						ds_writew(X_TARGET_BAK, ds_readw(ARRIVAL_X_TARGET));
						ds_writew(Y_TARGET_BAK, ds_readw(ARRIVAL_Y_TARGET));
						ds_writeb(DIRECTION, (ds_readb(ARRIVAL_DIRECTION) + 2) & 3);

					} else if (ds_readw(GAME_STATE) == GAME_STATE_MAIN && ds_readb(TRAVEL_DETOUR) != 99)
					{
						DNG_enter_dungeon(ds_readb(TRAVEL_DETOUR));
					}

					break;

				} else if (ds_readw(MOUSE1_EVENT1) != 0 )
				{

					for (i = 0, l4 = -1; i < 52; i++)
					{
						if (is_mouse_in_rect(l_di - 4,
									answer - 4,
							    (l_di = ds_readws(TOWN_POSITIONS + 4 * i)) + 4,
							    (answer = ds_readws((TOWN_POSITIONS + 2) + 4 * i)) + 4))
						{
							l4 = i;
							break;
						}
					}

					if (l4 == -1 && (l_di = get_mouse_action(ds_readws(MOUSE_POSX), ds_readws(MOUSE_POSY), p_datseg + ACTION_TABLE_TRAVELMAP)))
					{
						l4 = l_di + 51;
					}

					if (l4 != -1)
					{
						answer = ds_readws(CURRENT_TOWN_ANIX);
						ds_writew(CURRENT_TOWN_ANIX, 0);
						l6 = ds_readws(BASEPOS_X);
						l7 = ds_readws(BASEPOS_Y);
						ds_writew(BASEPOS_Y, 0);
						ds_writew(BASEPOS_X, (ds_readws(MOUSE_POSX) >= 0 && ds_readws(MOUSE_POSX) <= 159 ? 80 : -80));

						set_and_spin_lock();

						GUI_input(get_tx(l4), 0);

						ds_writew(BASEPOS_X, l6);
						ds_writew(BASEPOS_Y, l7);
						ds_writew(CURRENT_TOWN_ANIX, answer);
					}

					ds_writew(MOUSE1_EVENT1, 0);
				}
			}
			break;
		}

		dir_sign_ptr += 6;

	} while (host_readb(dir_sign_ptr) != 255);

	ds_writew(CURRENT_TOWN_ANIX, ds_writew(CURRENT_TOWN_ANIY, ds_writew(SELECTED_TOWN_ANIX, ds_writew(SELECTED_TOWN_ANIY, 0))));

	i = load_archive_file(ARCHIVE_FILE_COMPASS);
	read_archive_file(i, Real2Host(ds_readd(BUFFER6_PTR)), 5000);
	bc_close(i);

	ds_writeb(SHOW_TRAVEL_MAP, (signed char)ds_writew(BASEPOS_X, ds_writew(BASEPOS_Y, ds_writew(CURRENT_TOWN_OVER, ds_writew(TRV_MENU_SELECTION, 0)))));

	if (!ds_readb(TRAVEL_DETOUR))
	{
		ds_writew(WALLCLOCK_UPDATE, 0);
		leave_location();

	} else if (ds_readb(TRAVEL_DETOUR) != 99)
	{
		ds_writeb(CURRENT_TOWN, TOWNS_NONE);
	}

	if (ds_readb(PP20_INDEX) == 5)
	{
		memset((void*)Real2Host(ds_readd(DTP2)), 0, 0xc0);

		memcpy((void*)(Real2Host(ds_readd(DTP2)) + 0xc0),
			(void*)(Real2Host(ds_readd(TRAVEL_MAP_PTR)) + 64000 + 2), 0x60);

		memcpy((void*)(Real2Host(ds_readd(DTP2)) + 0x120),
			(void*)(p_datseg + PALETTE_SPECIAL), 0x60);

		for (i = 0; i < 64; i++)
		{
			pal_fade(Real2Host(ds_readd(DTP2)) + 0xc0, Real2Host(ds_readd(DTP2)));
			pal_fade(Real2Host(ds_readd(DTP2)) + 0x120, Real2Host(ds_readd(DTP2)) + 0x60);
			wait_for_vsync();
			set_palette(Real2Host(ds_readd(DTP2)) + 0xc0, 0x00, 0x20);
			set_palette(Real2Host(ds_readd(DTP2)) + 0x120, 0xe0, 0x20);
		}

		wait_for_vsync();
		set_palette(Real2Host(ds_readd(DTP2)), 0x80, 0x40);
		set_palette(Real2Host(ds_readd(DTP2)), 0x00, 0x20);

		do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 0, 0, 319, 199, 0);
	}

	ds_writew(CURRENT_ANI, ds_writebs(CITY_AREA_LOADED, ds_writebs(PP20_INDEX, -1)));
	ds_writew(REQUEST_REFRESH, (unsigned short)ds_writeb(FADING_STATE, 1));
	ds_writew(WALLCLOCK_UPDATE, bak1);

	return 0;
}

#if !defined(__BORLANDC__)
}
#endif
