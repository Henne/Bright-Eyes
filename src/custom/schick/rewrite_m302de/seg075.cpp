/**
 *	Rewrite of DSA1 v3.02_de functions of seg075 (dungeons generic)
 *	Functions rewritten: 20/20
 */

#include <string.h>
#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg003.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg027.h"
#include "seg028.h"
#include "seg047.h"
#include "seg049.h"
#include "seg075.h"
#include "seg096.h"
#include "seg097.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
signed short div16(unsigned char);
#endif

/* Borlandified and identical */
/* static */
void DNG_floor_ceil(void)
{
	struct nvf_desc nvf;
	signed short width, height;

	/* Load ceiling */
	nvf.dst = Real2Host(ds_readd(0xd303));
	nvf.src = Real2Host(ds_readd(0xd019));
	nvf.nr = 0;
	nvf.type = (!ds_readbs(0xe48c)) ? 3 : 5;
	nvf.width = (unsigned char*)&width;
	nvf.height = (unsigned char*)&height;
	process_nvf(&nvf);


	/* Load ceiling */
	nvf.dst = Real2Host(ds_readd(0xd303)) + 0x4030;
	nvf.src = Real2Host(ds_readd(0xd019));
	nvf.nr = (!(ds_readbs(DIRECTION)&1)) ? 1 : 2;
	nvf.type = (!ds_readbs(0xe48c)) ? 3 : 5;
	nvf.width = (unsigned char*)&width;
	nvf.height = (unsigned char*)&height;
	process_nvf(&nvf);

}

/* Borlandified and identical */
/* static */
void DNG_turn(void)
{
	signed short tmp;

	if ( ((tmp = div16(ds_readb(0xbd71))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd71, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd75))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd75, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd76))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd76, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd7c))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd7c, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd7d))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd7d, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd83))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd83, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd6e))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd77,
			ds_writeb(0xbd7d,
			ds_writeb(0xbd84, 0)));

		if ( ((tmp = div16(ds_readb(0xbd73))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd72,
				ds_writeb(0xbd78,
				ds_writeb(0xbd7e,
				ds_writeb(0xbd85, 0))));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd70))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd7b,
			ds_writeb(0xbd83,
			ds_writeb(0xbd8a, 0)));

		if ( ((tmp = div16(ds_readb(0xbd73))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd74,
				ds_writeb(0xbd7a,
				ds_writeb(0xbd82,
				ds_writeb(0xbd89, 0))));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd72))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd77,
			ds_writeb(0xbd7e,
			ds_writeb(0xbd84,
			ds_writeb(0xbd85, 0))));

		if ( ((tmp = div16(ds_readb(0xbd73))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd78, 0);
		}

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1)) {
				ds_writeb(0xbd78,
					ds_writeb(0xbd7f,
					ds_writeb(0xbd80, 0)));

				ds_writeb(0xbd86,
					ds_writeb(0xbd87, 0));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd73))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd79,
			ds_writeb(0xbd7f,
			ds_writeb(0xbd80, 0)));

		ds_writeb(0xbd81,
			ds_writeb(0xbd86,
			ds_writeb(0xbd87, 0)));

		ds_writeb(0xbd88, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd74))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd7b,
			ds_writeb(0xbd82,
			ds_writeb(0xbd89,
			ds_writeb(0xbd8a, 0))));

		if ( ((tmp = div16(ds_readb(0xbd73))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd79,
				ds_writeb(0xbd7a,
				ds_writeb(0xbd7f, 0)));

			ds_writeb(0xbd80,
				ds_writeb(0xbd81,
				ds_writeb(0xbd86, 0)));

			ds_writeb(0xbd87,
				ds_writeb(0xbd88, 0));

		}

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1)) {
				ds_writeb(0xbd7a,
					ds_writeb(0xbd80,
					ds_writeb(0xbd81, 0)));

				ds_writeb(0xbd87,
					ds_writeb(0xbd88, 0));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd77))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd84, 0);

		if ( ((tmp = div16(ds_readb(0xbd7f))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd7e,
				ds_writeb(0xbd85, 0));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd7b))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd8a, 0);

		if ( ((tmp = div16(ds_readb(0xbd81))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd82,
				ds_writeb(0xbd89, 0));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd78))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd77,
			ds_writeb(0xbd7e,
			ds_writeb(0xbd84,
			ds_writeb(0xbd85, 0))));

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd80, 0);
		}

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1) ||
			((tmp = div16(ds_readb(0xbd80))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd7f,
				ds_writeb(0xbd86,
				ds_writeb(0xbd87, 0)));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd7a))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd7b,
			ds_writeb(0xbd82,
			ds_writeb(0xbd89,
			ds_writeb(0xbd8a, 0))));

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd80, 0);
		}

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1) ||
			((tmp = div16(ds_readb(0xbd80))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd81,
				ds_writeb(0xbd87,
				ds_writeb(0xbd88, 0)));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd80,
			ds_writeb(0xbd87, 0));
	}

	if ( ((tmp = div16(ds_readb(0xbd7e))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd84, 0);

		if ( ((tmp = div16(ds_readb(0xbd86))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd85, 0);
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd82))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd8a, 0);

		if ( ((tmp = div16(ds_readb(0xbd88))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd89, 0);
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd80))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd87, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd80))) == 15) || (tmp == 1) ||
		((tmp = div16(ds_readb(0xbd87))) == 15) || (tmp == 1)) {

		if ( ((tmp = div16(ds_readb(0xbd7f))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd86, 0);
		}
		if ( ((tmp = div16(ds_readb(0xbd81))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd88, 0);
		}

	}
}

/* 0x56b */
/* Borlandified and identical */
void DNG_stub1(void)
{
	DNG_floor_ceil();
	move();
	DNG_turn();
	DNG_stub3();
	DNG_stub4();
	DNG_stub2();
	DNG_stub5();
}

/* 0x591 */
/* Borlandified and identical */
void DNG_stub2(void)
{
	signed short tmp;

	tmp = div16(ds_readb((0xbd6e + 1)));

	if ((tmp == 2) || (tmp == 9)) {

		if (div16(ds_readb((0xbd6e + 5))) == 15) {
			DNG_draw_walls( ((ds_readb(0x3616) == 1) ? 0x4e :
						((ds_readb(0x3616) == 2) ? 0x28 : 0x3e)),
					0, 0x36);
		}
	}
}

/* 0x5e5 */
/* Borlandified and identical */
void DNG_stub3(void)
{
	signed short i;
	signed short tmp;

	for (i = 21; i >= 0; i--) {

		tmp = div16(ds_readb(0xbd6e + i));

		ds_writeb(0xbd50 + i,
			(tmp == 6) ? ds_readb(0x9090 + i) :
				(tmp == 5) ? ds_readb(0x90a6 + i) :
				(tmp == 3) ? ds_readb(0x90d2 + i) :
				(tmp == 4) ? ds_readb(0x90bc + i) :
				(tmp == 7) ? ds_readb(0x90e8 + i) :
				(tmp == 1) ? ds_readb(0x90fe + i) :
				(tmp == 2) ? ds_readb(0x9114 + i) :
				(tmp == 9) ? ds_readb(0x912a + i) :
				(tmp == 8) ? ds_readb(0x9140 + i) :
				(tmp == 10) ? ds_readb(0x907a + i) :
				(tmp == 11) ? ds_readb(0x907a + i) :
				(tmp != 15) ? -1 : ds_readb(0x907a + i));
	}
}

/* 0x693 */
/* Borlandified and identical */
void DNG_draw_walls(signed short a1, signed short a2, signed short a3)
{
	signed short width;
	signed short height;
	signed short width2;
	signed short height2;
	signed short flag;
	Bit8u *dst_ptr;
	Bit8u *ptr2;
	struct nvf_desc nvf;

	flag = (a3 & 0x8000) ? 1 : 0;
	a3 &= 0x3fff;

	if ((a3 >= 8) && (a3 <= 13) && !ds_readbs(0xe48c)) {
		a3 += 6;
	} else {
		if ((a3 >= 14) && (a3 <= 19) && !ds_readbs(0xe48c)) {
			a3 -= 6;
		}
	}

	nvf.dst = dst_ptr = Real2Host(ds_readd(0xd303)) + 0x7530;
	nvf.src = Real2Host(ds_readd(0xd019));
	nvf.nr = a3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	nvf.type = 3;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	height = host_readws((Bit8u*)&height);
	width = host_readws((Bit8u*)&width);
#endif

	width2 = width;
	height2 = height;

	if ((a1 < 0) && ((a1 + width2) > 0)) {
		width2 += a1;
		dst_ptr += __abs__(a1);
		a1 = 0;
	}

	if ((a2 < 0) && ((a2 + height2) > 0)) {
		height2 -= a2;
		dst_ptr += __abs__(a2) * width;
		a2 = 0;
	}

	if ((a1 < 208) && (a2 < 135) && (a1 >= 0) && (a2 >= 0)) {

		if (a1 + width2 > 208) {
			width2 = 208 - a1;
		}

		if (a2 + height2 > 135) {
			height2 = 135 - a2;
		}

		ptr2 = Real2Host(ds_readd(0xd303)) + 208 * a2 + a1;

		if (!flag) {
			copy_solid(ptr2, dst_ptr, width2, height2, 208, width, 128);
		} else {
			copy_solid_permuted(ptr2, dst_ptr + width2 - 1, width2, height2, 208, width, 128);
		}
	}
}

/* 0x82e */
/* Borlandified and nearly identical */
void DNG_stub4(void)
{
	signed short l1;
	signed short l2;
	signed char l3;
	unsigned char l4;
	unsigned char l5;
	Bit8u *ptr;

	signed short i;
	signed short j;

	for (i = 0; i < 22; i++) {

		l3 = ds_readbs(0x92b6 + i);

		l5 = ds_readbs(0xbd6e + l3);

		l4 = (unsigned char)div16(l5);

		/* BC-TODO: access to a data structure */
		ptr = (l4 == 6) ? Real2Host(RealMake(datseg, l3 * 4 + 0x9206)) :
			(l4 == 5) ? p_datseg + 0x925e + l3 * 4 :
			(l4 == 4) ? p_datseg + 0x91ae + l3 * 4 :
			(l4 == 3) ? p_datseg + 0x91ae + l3 * 4 : p_datseg + 0x9156 + l3 * 4;


		l1 = host_readws(ptr);
		l2 = host_readws(ptr + 2);
		l3 = ds_readbs(0xbd50 + l3);

		if (l3 != -1) {

			ptr = Real2Host(ds_readd(0xe48d)) + (l3 - 1) * 18;

			if ((j = host_readws(ptr + 4)) != -1) {

				if ((j & 0x4000) &&
					((((l5 & 3) + 2) & 3) != ds_readbs(DIRECTION)))
				{
				} else {
					DNG_draw_walls(l1 + host_readws(ptr), l2 + host_readws(ptr + 2), j);
				}
			}

			if ((j = host_readws(ptr + 0xa)) != -1) {

				if ((j & 0x4000) &&
					((((l5 & 3) + 2) & 3) != ds_readbs(DIRECTION)))
				{
				} else {
					DNG_draw_walls(l1 + host_readws(ptr + 6), l2 + host_readws(ptr + 8), j);
				}
			}

			if ((j = host_readws(ptr + 0x10)) != -1) {

				if ((j & 0x4000) &&
					((((l5 & 3) + 2) & 3) != ds_readbs(DIRECTION)))
				{
				} else {
					DNG_draw_walls(l1 + host_readws(ptr + 0x0c), l2 + host_readws(ptr + 0x0e), j);
				}
			}
		}
	}
}

/* 0x9ef*/
/* Borlandified and identical */
void DNG_stub5(void)
{
	draw_compass();

	ds_writew(0xc011, ds_readw(0xce41));
	ds_writew(0xc013, ds_readw(0xce3f));
	ds_writew(0xc015, ds_readw(0xce41) + 207);
	ds_writew(0xc017, ds_readw(0xce3f) + 134);
	ds_writed(0xc019, ds_readd(0xd303));

	update_mouse_cursor();

	ds_writeb(0x45b8, 0);

	wait_for_vsync();

	do_pic_copy(1);

	DNG_lights();

	refresh_screen_size();
}

/* 0xa46 */

/**
 * is_staff_lvl2_in_group() - check for stafflevel >= in current group
 *
 * returns 0 = false, 1 = true
*/
/* Borlandified and identical */
signed short is_staff_lvl2_in_group(void)
{
	Bit8u *hero_i;
	signed short i;

	hero_i = get_hero(0);
	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

		if (host_readbs(hero_i + HERO_TYPE) &&
			(host_readbs(hero_i + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
			check_hero(hero_i) &&
			(host_readbs(hero_i + HERO_WAND) >= 2))
		{
			return 1;
		}
	}

	return 0;
}

/* 0xaaa*/
/* Borlandified and identical */
void DNG_lights(void)
{
	signed short l1;
	signed char l2;

	signed short i;

	if (div16(ds_readb((0xbd6e + 1))) != 11) {

		/* copy palette */
		memcpy(Real2Host(ds_readd(0xd2eb)), Real2Host(ds_readd(0xe404)), 0xc0);

		if (!(ds_readb((0xbd6e + 1)) & 1)) {

			if (ds_readd(INGAME_TIMERS + 0x24)) {
				l1 = 10;
			} else if (ds_readd(INGAME_TIMERS + 0x20) || is_staff_lvl2_in_group()) {
				l1 = 0;
			} else {
				if ( (l1 = get_max_light_time()) != -1) {
					l1 = 10 - l1;
				} else {
					l1 = 10;
				}
			}
		} else {
			if (ds_readd(INGAME_TIMERS + 0x24)) {
				l1 = 10;
			} else {
				l1 = 0;
			}
		}

		ds_writeb(DUNGEON_LIGHT, (l1 == 9) ? 1 : (l1 == 10) ? 2 : 0);
		l1 *= 3;

		for (i = 0; i < 0xc0; i++) {

			l2 = host_readbs(Real2Host(ds_readd(0xd2eb)) + i) - l1;

			if (l2 < 0) {
				l2 = 0;
			}

			host_writeb(Real2Host(ds_readd(0xd2eb)) + i, l2);
		}

		wait_for_vsync();
		set_palette(Real2Host(ds_readd(0xd2eb)), 0x80, 0x40);
	}
}

/* Borlandified and identical */
void DNG_timestep(signed short a1)
{
	signed short dir;

	timewarp(90);

	if ((ds_readws(DEATHTRAP) != 0) && !(dec_ds_ws(DEATHTRAP_STEPS))) {

		/* deathship / Totenschiff sinks */
		if (ds_readws(DEATHTRAP) == 1) {

			load_ani(18);
			init_ani(1);

			GUI_output(get_dtp(0x5c));
		}

		ds_writeb(DUNGEON_INDEX, 0);

		/* exit game */
		ds_writew(0xc3c1, 1);
	}

	if (ds_readw(0xc3c1) == 0) {

		dir = ds_readbs(DIRECTION);

		if (a1 == 1) {

			/* go forward */

			if (!dir) {
				dec_ds_ws(Y_TARGET);
			} else if (dir == 1) {
				inc_ds_ws(X_TARGET);
			} else if (dir == 2) {
				inc_ds_ws(Y_TARGET);
			} else {
				dec_ds_ws(X_TARGET);
			}
		} else {

			/* go backward */

			if (!dir) {
				inc_ds_ws(Y_TARGET);
			} else if (dir == 1) {
				dec_ds_ws(X_TARGET);
			} else if (dir == 2) {
				dec_ds_ws(Y_TARGET);
			} else {
				inc_ds_ws(X_TARGET);
			}
		}

		xor_ds_bs(0xe48c, 1);
	}
}

/* Borlandified and identical */
void DNG_update_pos(void)
{
	set_var_to_zero();

	DNG_stub1();

	ds_writew(0xe486, ds_readw(X_TARGET));
	ds_writew(0xe484, ds_readw(Y_TARGET));
	ds_writew(0xe482, ds_readbs(DIRECTION));

}

/* Borlandified and identical */
void DNG_inc_level(void)
{
	ds_writew(0x9312, 1);
	inc_ds_bs_post(DUNGEON_LEVEL);
	load_area_description(1);
	DNG_update_pos();

}

/* Borlandified and identical */
void DNG_dec_level(void)
{
	ds_writew(0x9312, 1);
	dec_ds_bs_post(DUNGEON_LEVEL);
	load_area_description(1);
	DNG_update_pos();

}

/* Borlandified and identical */
void DNG_open_door(void)
{
	signed short x;
	signed short y;
	signed short iters;
	signed short i;

	DNG_floor_ceil();
	move();
	ds_writebs((0xbd6e + 5), 32);
	DNG_turn();
	DNG_stub3();
	DNG_stub4();

	memmove(Real2Host(ds_readd(0xd303)) + 0x7530, Real2Host(ds_readd(0xd303)), 0x6db0);

	if (!ds_readb(0x3616)) {
		x = 45;
		y = 38;
		iters = 19;
	} else if (ds_readb(0x3616) == 1) {
		x = 47;
		y = 30;
		iters = 20;
	} else {
		x = 54;
		y = 44;
		iters = 17;
	}

	play_voc(0x12f);

	update_mouse_cursor();

	for (i = 0; i < iters; i++) {

		door_frame(31, x, y, (i * 4) + 1);

		DNG_stub5();

		memmove(Real2Host(ds_readd(0xd303)), Real2Host(ds_readd(0xd303)) + 0x7530, 0x6db0);
	}

	refresh_screen_size();
}

/* Borlandified and identical */
void DNG_close_door(void)
{
	signed short x;
	signed short y;
	signed short iters;
	signed short i;

	DNG_floor_ceil();
	move();
	DNG_turn();
	DNG_stub3();
	DNG_stub4();

	memmove(Real2Host(ds_readd(0xd303)) + 0x7530, Real2Host(ds_readd(0xd303)), 0x6db0);

	if (!ds_readb(0x3616)) {
		x = 45;
		y = 38;
		iters = 18;
	} else if (ds_readb(0x3616) == 1) {
		x = 47;
		y = 30;
		iters = 19;
	} else {
		x = 54;
		y = 44;
		iters = 16;
	}

	play_voc(0x12f);

	update_mouse_cursor();

	for (i = iters; i >= 0; i--) {

		door_frame(31, x, y, (i * 4) + 1);

		DNG_stub5();

		memmove(Real2Host(ds_readd(0xd303)), Real2Host(ds_readd(0xd303)) + 0x7530, 0x6db0);
	}

	refresh_screen_size();
}

/* Borlandified and identical */
void DNG_stub6(void)
{
	signed short l_si;
	signed short l_di;
	signed short l1;
	Bit8u *hero_auto;
	Bit8u *hero1;
	Bit8u *hero2;

	play_voc(0x132);

	if (ds_readb(DUNGEON_LIGHT) != 0) {

		if (DNG_fallpit(6)) {
			ds_writew(X_TARGET, ds_readw(0x2d83));
			ds_writew(Y_TARGET, ds_readw(0x2d85));
		}
	} else {

		if (((l_si = DNG_check_climb_tools()) != -1) &&
			((l_di = count_heroes_available_in_group()) > 1))
		{

			/* ropes oder staff */
			host_writeb(Real2Host(ds_readd(DTP2)), 0);

			if (l_si) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ltx(0xc00),
					get_hero(l_si - 1) + HERO_NAME2);
			}

			if (l_di == 2) {

				hero1 = Real2Host(get_first_hero_available_in_group());

				hero2 = Real2Host(get_second_hero_available_in_group());

				sprintf((char*)Real2Host(ds_readd(0xd2eb)),
					(char*)get_ltx(0xc04),
					hero1 + HERO_NAME2,
					hero2 + HERO_NAME2);

				strcat((char*)Real2Host(ds_readd(DTP2)),
					(char*)Real2Host(ds_readd(0xd2eb)));

				if (test_attrib(hero1, 4, 2) <= 0) {

					sprintf((char*)Real2Host(ds_readd(0xd2eb)),
						(char*)get_ltx(0xc08),
						hero1 + HERO_NAME2,
						hero2 + HERO_NAME2,
						l_si = random_schick(3) + 1);

					strcat((char*)Real2Host(ds_readd(DTP2)),
						(char*)Real2Host(ds_readd(0xd2eb)));

					sub_hero_le(hero2, l_si);
				}
			} else {

				hero_auto = get_hero(l1 = get_hero_KK_best());

				l_di--;

				l_si = 0;

				do {

					if (l_si == l1) {
						l_si++;
					}

					strcat((char*)Real2Host(ds_readd(DTP2)),
						(char*)(get_hero(l_si++) + HERO_NAME2));

					if (--l_di) {

						strcat((char*)Real2Host(ds_readd(DTP2)),
							(char*)((l_di >= 2) ? p_datseg + 0x92cc : p_datseg+ 0x92cf));
					}

				} while (l_di);

				sprintf((char*)Real2Host(ds_readd(0xd2eb)),
					(char*)get_ltx(0xc0c),
					hero_auto + 0x10,
					Real2Host(GUI_get_ptr(host_readbs(hero_auto + 0x22), 0)),
					Real2Host(GUI_get_ptr(host_readbs(hero_auto + 0x22), 2)));

				strcat((char*)Real2Host(ds_readd(DTP2)),
					(char*)Real2Host(ds_readd(0xd2eb)));

			}

			GUI_output(Real2Host(ds_readd(DTP2)));
		} else {

			sub_group_le(random_schick(5));
		}

		DNG_inc_level();
	}
}


/* Borlandified and identical */
signed short DNG_check_climb_tools(void)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);

	/* check for a mage with staffspell > 2 */
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if ((host_readbs(hero + HERO_TYPE) != 0) &&
			(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
			!hero_dead(hero) &&
			(host_readbs(hero + HERO_TYPE) == 9) &&
			(host_readbs(hero + HERO_WAND) > 2))
		{
			return i + 1;
		}
	}

	/* check for ladder or rope */
	return ((get_first_hero_with_item(0x79) != -1) ||
			(get_first_hero_with_item(0x20) != -1)) ? 0 : -1;
}

/* Borlandified and identical */
signed short DNG_fallpit(signed short a1)
{
	signed short l_si;
	signed short l_di;
	signed short l1;
	signed short l2;
	signed short retval;

	l2 = 0;
	retval = 0;

	ds_writew(0x9312, 1);
	l1 = random_schick(ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP)));

	if (ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP)) - 1 == l1) {
		l1 = ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP));
	}

	if (ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP)) != l1) {

		while (ds_readbs(0x2d36 + l2)) {
			l2++;
		}

		for (l_di = 0; l_di < l1; l_di++) {

			do {
				l_si = random_schick(7) - 1;

			} while ( (!host_readbs(get_hero(l_si) + HERO_TYPE)) ||
					(host_readbs(get_hero(l_si) + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP)) ||
					((l1 == 1) && (l_si == 6)));

			host_writeb(get_hero(l_si) + HERO_GROUP_NO, (unsigned char)l2);
			inc_ds_bs_post(0x2d36 + l2);
			dec_ds_bs_post(0x2d36 + ds_readbs(CURRENT_GROUP));
			sub_hero_le(get_hero(l_si), random_schick(a1));

		}

		GRP_save_pos(l2);
		ds_writeb(0x2d76 + l2, ds_readbs(DUNGEON_LEVEL) + 1);

		retval = 1;

	} else {

		l_si = 0;

		for (l_di = 0; l_di < l1; l_di++) {

			while (!host_readbs(get_hero(l_si) + HERO_TYPE) ||
				(host_readbs(get_hero(l_si) + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP)))
			{
				l_si++;
			}

			sub_hero_le(get_hero(l_si++), random_schick(a1));
		}

		DNG_inc_level();
	}

	return retval;
}

/* Borlandified and identical */
void DNG_enter_dungeon(signed short dungeon_id)
{
	signed short x_pos;
	signed short y_pos;
	signed short level;
	signed short dir;
	signed short i;
	Bit8u *ptr;

	switch (dungeon_id) {

		case 1:  x_pos = 9;  y_pos = 11;  dir = 0;  level = 2;  break;
		case 2:  x_pos = 1;  y_pos = 1;   dir = 2;  level = 0;  break;
		case 3:  x_pos = 1;  y_pos = 8;   dir = 1;  level = 0;  break;

#if !defined(__BORLANDC__)
		case 4:  x_pos = 7;  y_pos = 14;  dir = 0;  level = 0;  break;
#else
mark1:		case 4:  x_pos = 7;  y_pos = 14;  dir = 0;  level = 0;  break;
#endif

		case 5:  x_pos = 6;  y_pos = 14;  dir = 0;  level = 0;  break;
		case 6:  x_pos = 13; y_pos = 14;  dir = 0;  level = 0;  break;
		case 7:  x_pos = 1;  y_pos = 13;  dir = 0;  level = 0;  break;
		case 8:  x_pos = 1;  y_pos = 14;  dir = 1;  level = 0;  break;

#if !defined(__BORLANDC__)
		case 9:  x_pos = 7;  y_pos = 14;  dir = 0;  level = 0;  break;
#else
		case 9:  goto mark1;
mark2:			   goto mark1;
#endif

		case 10:  x_pos = 1;  y_pos = 3;   dir = 1;  level = 0;  break;

#if !defined(__BORLANDC__)
		case 11:  x_pos = 7;  y_pos = 14;  dir = 0;  level = 0;  break;
#else
		case 11:  goto mark2;
#endif

		case 12:  x_pos = 13; y_pos = 14;  dir = 0;  level = 0;  break;
		case 13: {
			x_pos = 7;
			y_pos = 14;
			dir = 0;
			level = 0;

			ds_writeb(0x434d, 0);
			ds_writeb(0x434e, (signed char)count_heroes_in_group());
			break;
		}
		case 14: {
			x_pos = 1;
			y_pos = 14;
			dir = 3;
			level = ds_writebs(LOCATION, 0);
			break;
		}
		case 15:  x_pos = 1;  y_pos = 11;   dir = 1;  level = 0;  break;
	}

	ds_writew(X_TARGET, x_pos);
	ds_writew(Y_TARGET, y_pos);
	ds_writeb(DIRECTION, (signed char)dir);
	ds_writeb(DUNGEON_LEVEL, (signed char)level);
	ds_writeb(DUNGEON_INDEX, (signed char)dungeon_id);
	ds_writebs(0x2d9f, ds_readbs(LOCATION));
	ds_writeb(0x2da6, ds_readb(CURRENT_TOWN));
	ds_writeb(LOCATION, ds_writeb(CURRENT_TOWN, 0));
	ds_writeb(0x2ca6, ds_writeb(0x2ca7, -1));

	if (dungeon_id == 14) {

		ptr = Real2Host(ds_readd(0xd303)) + 0x1f4;
		memset(Real2Host(ds_readd(0xd303)), 0, 0x120);
		memcpy(Real2Host(ds_readd(0xd303)) + 0x1f4, p_datseg + 0x3e53, 0x120);

		for (i = 0; i < 0x40; i++) {

			pal_fade(ptr, Real2Host(ds_readd(0xd303)));
			pal_fade(ptr + 0x60, Real2Host(ds_readd(0xd303)) + 0x60);
			pal_fade(ptr + 0xc0, Real2Host(ds_readd(0xd303)) + 0xc0);

			wait_for_vsync();

			set_palette(ptr, 0, 0x20);
			set_palette(ptr + 0x60, 0x80, 0x40);
		}

		do_fill_rect((RealPt)ds_readd(0xd303), 0, 0, 319, 199, 0);
		ds_writew(0xc011, 0);
		ds_writew(0xc013, 0);
		ds_writew(0xc015, 240);
		ds_writew(0xc017, 136);
		ds_writed(0xc019, ds_readd(0xd303));
		update_mouse_cursor();
		do_pic_copy(1);
		refresh_screen_size();
	}
}

#if !defined(__BORLANDC__)
}
#endif

