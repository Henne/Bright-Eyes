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
	signed short handle;
	signed short l3;
	signed short l4;
	signed short l5;
	Bit8u *ptr;
	signed short l6;
	signed short l7;
	signed short bak1;
	signed short tw_bak;
	Bit8u *array[6];

	bak1 = ds_readws(WALLCLOCK_UPDATE);
	ds_writeb(0xe4a2, (unsigned char)ds_writew(WALLCLOCK_UPDATE, (unsigned short)ds_writeb(0x4333, 0)));
	ds_writeb(CURRENT_TOWN, ds_readbs(TYPEINDEX));

	update_mouse_cursor();

	for (handle = 0; handle < 6; handle++)
	{
		ds_writeb(FOOD_MESSAGE_SHOWN + handle, 0);
	}

	if (ds_readb(0x2ca8) != ds_readb(TRAVELING))
	{
		load_map();
	}

	memmove((void*)Real2Host(ds_readd(BUFFER1_PTR)), (void*)Real2Host(ds_readd(0x432e)), 64000);

	map_effect(Real2Host(ds_readd(BUFFER1_PTR)));

	wait_for_vsync();

	set_palette(Real2Host(ds_readd(0x432e)) + 64000 + 2, 0, 0x20);

	refresh_screen_size();

	prepare_map_marker();

	ds_writew(MOUSE1_EVENT1, 0);

	ptr = p_datseg + 0xa0b4;

	do {
		if (ds_readws(REQUEST_REFRESH) != 0)
		{
			update_mouse_cursor();

			if (ds_readb(0x2ca8) != ds_readb(TRAVELING))
			{
				load_map();
			}

			wait_for_vsync();

			bc_memmove((RealPt)ds_readd(FRAMEBUF_PTR), (RealPt)ds_readd(0x432e), 64000);

			wait_for_vsync();

			set_palette(Real2Host(ds_readd(0x432e)) + 64000 + 2, 0, 0x20);

			refresh_screen_size();

			ds_writew(REQUEST_REFRESH, 0);
		}

		if (host_readbs(ptr) == ds_readbs(CURRENT_TOWN) && host_readb(ptr + 1) == ds_readw(0x4334))
		{
			while (1) {
				handle_input();

				if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == 73)
				{
					handle = 0;
					while ((l_di = host_readb(Real2Host(host_readd(ptr + 2)) + handle)) != 255)
					{
						array[handle] = get_ltx(4 * (0xeb + ds_writebs(TRV_MENU_TOWNS + handle, (l3 = ds_readb(0x9dbd + 9 * l_di)) != ds_readbs(CURRENT_TOWN) ? (signed char) l3 : ds_readbs(0x9dbe + 9 * l_di))));

						handle++;
					}

					ds_writeb(TRV_MENU_TOWNS + handle, ds_readbs(CURRENT_TOWN));
					array[handle] = get_ltx(0x994);
					handle++;

					sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_ltx(0x884),
						(char*)get_ltx(4 * (0xeb + ds_readbs(CURRENT_TOWN))));

					tw_bak = ds_readws(TEXTBOX_WIDTH);
					ds_writew(TEXTBOX_WIDTH, 4);

					l3 = GUI_radio(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (signed char)handle,
								array[0],
								array[1],
								array[2],
								array[3],
								array[4],
								array[5]) - 1;

					ds_writew(TEXTBOX_WIDTH, tw_bak);

					ds_writew(CURRENT_TOWN_ANIX, 0);

					set_and_spin_lock();

					if (handle - 1 == l3 || l3 == -2)
					{
						ds_writeb(TRAVELING, 0);
						ds_writeb(DIRECTION, (ds_readb(DIRECTION) + 2) & 3);
						break;
					}

					l5 = host_readb(Real2Host(host_readd(ptr + 2)) + l3);
					ds_writew(TRV_DESTINATION, ds_readbs(TRV_MENU_TOWNS + l3));

					if (!get_current_season() &&
						(l5 == 31 || l5 == 41 || l5 == 47 || l5 == 48 || l5 == 49))
					{
						GUI_input(get_dtp(0x114), 0);
						break;
					}

					ds_writew(WALLCLOCK_X, ds_readws(BASEPOS_X) + 120);
					ds_writew(WALLCLOCK_Y, ds_readws(BASEPOS_Y) + 87);

					ds_writew(WALLCLOCK_UPDATE, 1);

					TM_func1(host_readb(Real2Host(host_readd(ptr + 2)) + l3),
						(ds_readbs(0x9dbd + 9 * host_readb(Real2Host(host_readd(ptr + 2)) + l3)) == ds_readbs(CURRENT_TOWN) ? 0 : 1));
					ds_writew(WALLCLOCK_UPDATE, 0);

					if (ds_readb(0xe4a2) != 0)
					{
						TM_func9();
					}

					if (ds_readw(TRV_RETURN) == 2)
					{
						if (ds_readb(0x4333) != 0 && ds_readb(0x4333) != 99)
						{
							DNG_enter_dungeon(ds_readb(0x4333));
						}
						break;
					}

					TM_enter_target_town();

					if (!ds_readb(0x4333) && ds_readw(GAME_STATE) == GAME_STATE_MAIN)
					{
						ds_writeb(CURRENT_TOWN, (signed char)ds_readw(TRV_DEST_REACHED));
						ds_writew(X_TARGET_BAK, ds_readw(0x433a));
						ds_writew(Y_TARGET_BAK, ds_readw(0x433c));
						ds_writeb(DIRECTION, (ds_readb(0x433e) + 2) & 3);

					} else if (ds_readw(GAME_STATE) == GAME_STATE_MAIN && ds_readb(0x4333) != 99)
					{
						DNG_enter_dungeon(ds_readb(0x4333));
					}

					break;

				} else if (ds_readw(MOUSE1_EVENT1) != 0 )
				{

					for (handle = 0, l4 = -1; handle < 52; handle++)
					{
						if (is_mouse_in_rect(l_di - 4,
									l3 - 4,
							    (l_di = ds_readws((TOWN_POSITIONS + 4) + 4 * handle)) + 4,
							    (l3 = ds_readws((TOWN_POSITIONS + 6) + 4 * handle)) + 4))
						{
							l4 = handle;
							break;
						}
					}

					if (l4 == -1 && (l_di = get_mouse_action(ds_readws(MOUSE_POSX), ds_readws(MOUSE_POSY), p_datseg + 0xa50f)))
					{
						l4 = l_di + 51;
					}

					if (l4 != -1)
					{
						l3 = ds_readws(CURRENT_TOWN_ANIX);
						ds_writew(CURRENT_TOWN_ANIX, 0);
						l6 = ds_readws(BASEPOS_X);
						l7 = ds_readws(BASEPOS_Y);
						ds_writew(BASEPOS_Y, 0);
						ds_writew(BASEPOS_X, (ds_readws(MOUSE_POSX) >= 0 && ds_readws(MOUSE_POSX) <= 159 ? 80 : -80));

						set_and_spin_lock();

						GUI_input(get_dtp(4 * l4), 0);

						ds_writew(BASEPOS_X, l6);
						ds_writew(BASEPOS_Y, l7);
						ds_writew(CURRENT_TOWN_ANIX, l3);
					}

					ds_writew(MOUSE1_EVENT1, 0);
				}
			}
			break;
		}

		ptr += 6;

	} while (host_readb(ptr) != 255);

	ds_writew(CURRENT_TOWN_ANIX, ds_writew(CURRENT_TOWN_ANIY, ds_writew(SELECTED_TOWN_ANIX, ds_writew(SELECTED_TOWN_ANIY, 0))));

	handle = load_archive_file(ARCHIVE_FILE_COMPASS);
	read_archive_file(handle, Real2Host(ds_readd(BUFFER6_PTR)), 5000);
	bc_close(handle);

	ds_writeb(TRAVELING, (signed char)ds_writew(BASEPOS_X, ds_writew(BASEPOS_Y, ds_writew(CURRENT_TOWN_OVER, ds_writew(TRV_MENU_SELECTION, 0)))));

	if (!ds_readb(0x4333))
	{
		ds_writew(WALLCLOCK_UPDATE, 0);
		turnaround();

	} else if (ds_readb(0x4333) != 99)
	{
		ds_writeb(CURRENT_TOWN, 0);
	}

	if (ds_readb(PP20_INDEX) == 5)
	{
		memset((void*)Real2Host(ds_readd(DTP2)), 0, 0xc0);

		memcpy((void*)(Real2Host(ds_readd(DTP2)) + 0xc0),
			(void*)(Real2Host(ds_readd(0x432e)) + 64000 + 2), 0x60);

		memcpy((void*)(Real2Host(ds_readd(DTP2)) + 0x120),
			(void*)(p_datseg + 0x27e3), 0x60);

		for (handle = 0; handle < 64; handle++)
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
	ds_writew(REQUEST_REFRESH, (unsigned short)ds_writeb(0x4475, 1));
	ds_writew(WALLCLOCK_UPDATE, bak1);

	return 0;
}

#if !defined(__BORLANDC__)
}
#endif
