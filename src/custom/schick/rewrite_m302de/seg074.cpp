/**
 *	Rewrite of DSA1 v3.02_de functions of seg074 (automap)
 *	Functions rewritten: 3/11
 */

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg029.h"
#include "seg074.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
/* Borlandified and identical */
void show_automap(void)
{
	signed short l_si;
	signed short l_di;
	signed short loc_bak;
	signed short done;
	signed short dungeon;
	signed short town;
	signed short bak;

	if (!ds_readb(DUNGEON_LIGHT) ||
		((ds_readbs(DUNGEON_LIGHT) == 1) && (test_skill(get_first_hero_available_in_group(), 28, 6) > 0)))
	{

		ds_writeb(0x45b8, 1);

		dungeon = ds_readbs(DUNGEON_INDEX);
		town = ds_readbs(CURRENT_TOWN);

		ds_writeb(CURRENT_TOWN, ds_writeb(DUNGEON_INDEX, 0));

		bak = ds_readw(0xbffd);
		ds_writew(0xbffd, 3);

		l_si = (ds_readb(0xbd94) == 16) ? 0 :
				((ds_readws(X_TARGET) - 8 < 0) ? 0 :
				((ds_readws(X_TARGET) - 8 > 15) ? 16 : ds_readws(X_TARGET) - 8));

		ds_writeb(CURRENT_TOWN, (signed char)town);
		ds_writeb(DUNGEON_INDEX, (signed char)dungeon);

		ds_writew(0x2846, 1);

		do {

			if (ds_readw(0x2846) != 0) {
				loc_bak = ds_readbs(LOCATION);
				ds_writeb(LOCATION, 1);

				draw_main_screen();
				ds_writeb(LOCATION, (signed char)loc_bak);

				if (ds_readb(0xbd94) == 16) {
					draw_loc_icons(1, 8);
				} else {
					draw_loc_icons(3, 27, 26, 8);
				}

				seg074_305(l_si);
				clear_ani_pal();
				seg074_7b2();
				set_ani_pal(p_datseg + 0x7d0e);
				ds_writew(0x2846, 0);
			}

			done = 0;

			handle_gui_input();

			if ((ds_readw(0xc3d3) != 0) || (ds_readw(ACTION) == 73)) {

				if (ds_readb(0xbd94) == 16) {
					l_di = GUI_radio(get_ltx(0x990), 1, get_ltx(0x994)) - 1;
				} else {
					ds_writew(0xe5ac, 2);
					l_di = GUI_radio(get_ltx(0x990), 3,
								 get_ltx(0x998),
								 get_ltx(0x99c),
								 get_ltx(0x994)) - 1;
				}

				if (l_di != -2) {
					ds_writew(ACTION, l_di + 129);
				}
			}

			if (ds_readb(0xbd94) != 16) {

				if ((ds_readws(ACTION) == 129) || (ds_readws(ACTION) == 75)) {

					if (l_si > 0) {
						seg074_305(--l_si);
						seg074_7b2();
					}
				}

				if ((ds_readws(ACTION) == 130) || (ds_readws(ACTION) == 77)) {

					if (l_si < 16) {
						seg074_305(++l_si);
						seg074_7b2();
					}
				}
			}

			if (((ds_readws(ACTION) == 129) && (ds_readb(0xbd94) == 16)) ||
				((ds_readws(ACTION) == 131) && (ds_readb(0xbd94) != 16)))
			{
				done = 1;
			}
		} while (done == 0);

		ds_writew(0xbffd, bak);
		ds_writew(0x2846, 1);

		clear_ani_pal();

	} else {
		GUI_output(get_ltx(0xc10));
	}
}
#endif

/* Borlandified and identical */
unsigned short is_discovered(signed short x, signed short y)
{
	return ds_readb(0xe442 + (4 * y) + (x >> 3)) & (ds_readb(0x7d4a + (x & 7)));
}

/**
 * \brief	get value of automap field from small (16x16) maps
 *
 * \param	x	x-coordinate
 * \param	y	y-coordinate
 *
 * \return	value of the field at (x,y)
 */
/* Borlandified and identical */
unsigned short get_mapval_small(signed short x, signed short y)
{
	Bit8u *map = p_datseg + 0xbd95;

	return host_readb(map + 16 * y + x);
}

#if defined(__BORLANDC__)
void seg074_305(signed short a1)
{
}

void seg074_7b2(void)
{
}

#endif


/* TODO: stub function for transversalis */
signed short select_teleport_dest(void)
{
	return -1;
}

#if !defined(__BORLANDC__)
}
#endif
