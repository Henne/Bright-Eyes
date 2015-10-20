/**
 *	Rewrite of DSA1 v3.02_de functions of seg066 (city)
 *	Functions rewritten: 6/18
 */

#include <stdlib.h>

#include "v302de.h"

#include "seg002.h"
#include "seg003.h"
#include "seg004.h"
#include "seg008.h"
#include "seg025.h"
#include "seg027.h"
#include "seg029.h"
#include "seg030.h"
#include "seg032.h"
#include "seg066.h"
#include "seg092.h"
#include "seg096.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
signed short get_border_index(unsigned char);
#endif

/* Borlandified and nearly identical */
signed short enter_location(signed short town_id)
{
	signed short map_pos;
	signed short b_index;
	Bit8u *ptr;

	if (town_id == 40) {
		return enter_location_daspota();
	}

	map_pos = 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);
	ptr = p_datseg + 0xc025;
	ds_writeb(0xe10c, 0);

	do {
		if (host_readws(ptr) == map_pos) {

			/* found the location */
			ds_writeb(0x2d9f, 0);
			ds_writeb(LOCATION, host_readb(ptr + 2));
			ds_writew(TYPEINDEX, host_readb(ptr + 3));
			ds_writew(CITYINDEX, host_readw(ptr + 4));

			if (ds_readb(LOCATION) == 9) {
				ds_writeb(LOCATION, 0);
				ds_writeb(0xe10c, 1);
			}

			return 1;
		}

		ptr += 6;

	} while (host_readws(ptr) != -1);

	move();

	/* TODO: this assignment differs */
	if ((b_index = get_border_index(ds_readb((0xbd6e + 1)) & 0xff)) >= 2 && b_index <= 5) {

		ds_writeb(0x2d9f, 0);
		ds_writew(CITYINDEX, ds_readb(0x71c9 + town_id));

		if (!((ds_readbs(DIRECTION) + ds_readws(X_TARGET) + ds_readws(Y_TARGET)) & 1)) {
			ds_writeb(LOCATION, 10);
		} else {
			ds_writeb(LOCATION, 16);
			inc_ds_ws(CITYINDEX);
		}

		return 1;
	}

	return 0;
}

/* Borlandified and nearly identical */
signed short enter_location_daspota(void)
{
	signed short map_pos;
	signed short b_index;
	Bit8u *ptr;

	if (ds_readws(0xc3c1) == 7) {
		return 1;
	}

	map_pos = 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);
	ptr = p_datseg + 0xc025;
	ds_writeb(0xe10c, 0);

	do {

		if (host_readws(ptr) == map_pos) {

			ds_writew(TYPEINDEX, host_readb(ptr + 3));

			if (host_readb(ptr + 2) != 12) {

				GUI_print_loc_line(get_dtp(4 * host_readw(ptr + 4)));

				if (!ds_readb(0x331f + host_readw(ptr + 4))) {

					do_talk(host_readbs(ptr + 2), host_readb(ptr + 3) - 1);

					if (!ds_readb(0x331f + host_readw(ptr + 4))) {
						turnaround();
						return 1;
					}
				}

				draw_main_screen();
				set_var_to_zero();

				load_ani(10);
				GUI_print_loc_line(get_dtp(4 * host_readw(ptr + 4)));
				init_ani(0);

				if (ds_readd(0x71fa + 4 * host_readw(ptr + 4))) {

					loot_multi_chest(Real2Host((RealPt)ds_readd(0x71fa + 4 * host_readw(ptr + 4))), get_dtp(0x54));

				} else {

					do {
						handle_gui_input();
					} while (ds_readws(ACTION) == 0 && ds_readws(0xc3d5) == 0);

					ds_writew(0xc3d5, 0);
				}

				set_var_to_zero();

				if (host_readw(ptr + 4) == 6) {
					do_fight(200);
				} else if (host_readw(ptr + 4 ) == 12) {
					do_fight(207);
				}

				turnaround();

			} else {
				ds_writeb(0x2d9f, 0);
				ds_writeb(LOCATION, host_readbs(ptr + 2));
				ds_writew(CITYINDEX, host_readw(ptr + 4));
			}

			return 1;
		}

		ptr += 6;

	} while (host_readws(ptr) != -1);

	move();

	/* TODO: this assignment differs */
	if ((b_index = get_border_index(ds_readb((0xbd6e + 1)) & 0xff)) >= 2 && b_index <= 5) {

		ds_writeb(0x2d9f, 0);
		ds_writeb(LOCATION, 10);
		ds_writew(CITYINDEX, 19);
		return 1;
	}

	return 0;
}

void TLK_eremit(signed short state)
{
	signed short i;
	Bit8u *hero;

	if (!state) {
		ds_writew(0xe30e, ds_readb(0x3615) != 0 ? 1 : 2);
	} else if (state == 6) {

		hero = get_hero(0);
		for (i = 0 ; i <= 6; i++, hero += 0x6da) {

			/* remove hunger and thirst */
			host_writeb(hero + 0x7f, host_writebs(hero + 0x80, 0));

			/* heal all wounds */
			add_hero_le(hero, host_readws(hero + 0x5e));
		}

	} else if (state == 10) {
		/* group learns about two places to rest */
		ds_writeb(0xe309, ds_writeb(0x3e08, 1));
	} else if (state == 13) {
		ds_writeb(0x3615, 1);
	} else if (state == 14) {
		timewarp(MINUTES(30));
	}
}

void do_town(void)
{
	DUMMY_WARNING();
}

/* 0xb73 */
#if !defined(__BORLANDC__)
signed short get_border_index(unsigned short val)
#else
signed short get_border_index(unsigned char val)
#endif
{
	signed short i;

	i = 0;
	while (ds_readb(0x7246 + i) < val) {
		i++;
	}

	ds_writew(0xe412, (((val & 3) + 4) - ds_readbs(DIRECTION)) & 3);

	if (i == 0)
		i = 1;

	i--;

	return i;

}

/* 0xf62 */
void load_city_texture(signed short v1, signed short v2, signed short nvf_nr,
		signed short v4)
{

	struct nvf_desc nvf;
	Bit8u *dst;
	Bit8u *src;
	signed short direction;
	signed short copy_height;
	signed short copy_width;
	signed short height;
	signed short width;

	direction = (nvf_nr & 0x8000 ? 1: 0);
	nvf_nr &= 0x3fff;

	v4 -= 184;

	nvf.dst = src = Real2Host(ds_readd(0xd303)) + 0x7530;
	nvf.src = Real2Host(ds_readd(0xe414 + v4 * 4));

	if (v4 == 48 || v4 == 49) {
		nvf.src = Real2Host(ds_readd(0xd2b5));
	}

	nvf.nr = nvf_nr;
	nvf.type = (direction == 0 ? 3: 5);
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	height = host_readws((Bit8u*)&height);
	width = host_readws((Bit8u*)&width);
#endif

	copy_width = width;
	copy_height = height;

	if (v1 < 0) {
		if (v1 + copy_width > 0) {
			copy_width += v1;
			src += abs(v1);
			v1 = 0;
		}
	}

	if (v2 < 0) {
		if (v2 + copy_height > 0) {
			copy_height -= v2;
			src += abs(v2) * width;
			v2 = 0;
		}
	}


	if (v1 < 208 && v2 < 135 && v1 >= 0 && v2 >= 0) {

		if (copy_width + v1 > 208) {
			copy_width = 208 - v1;
		}

		if (copy_height + v2 > 135) {
			copy_height = 135 - v2;
		}

		dst = Real2Host(ds_readd(0xd303)) + v2 * 208 + v1;

		copy_solid(dst, src, copy_width, copy_height, 208, width,
			v4 == 0 ? 0 : 128);
	}
}

/* 0x172b */
void seg066_172b(void)
{
	signed short l_si;
	signed short l_di;
	Bit8u *ptr = p_datseg + 0xbd95;

	ds_writeb(0xe400, ds_writeb(0xe401, ds_writeb(0xe402, ds_writeb(0xe403, 0))));

	for (l_di = 0; ds_readb(0xbd94) * 16 > l_di; l_di++) {

		l_si = get_border_index(host_readb(ptr + l_di));

		if (l_si == 2) {
			inc_ds_bs_post(0xe400);
		} else if (l_si == 3) {
			inc_ds_bs_post(0xe401);
		} else if ((l_si == 4) || (l_si == 1)) {
			inc_ds_bs_post(0xe402);
		} else if (l_si == 5) {
			inc_ds_bs_post(0xe403);
		}
	}

	l_di = 5;
	l_si = 2000;

	if (ds_readb(0xe400) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 0));
	}

	if (ds_readb(0xe401) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 1));
	}

	if (ds_readb(0xe402) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 2));
	}

	if (ds_readb(0xe403) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 3));
	}

	ds_writeb(0xe400 + l_di, 0);
}

#if !defined(__BORLANDC__)
}
#endif

