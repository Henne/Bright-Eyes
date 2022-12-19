/**
 *	Rewrite of DSA1 v3.02_de functions of seg075 (dungeon: common 1/2)
 *	Functions rewritten: 20/20 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg075.cpp
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

/* static */
void DNG_floor_ceil(void)
{
	struct nvf_desc nvf;
	signed short width, height;

	/* Load ceiling */
	nvf.dst = Real2Host(ds_readd(RENDERBUF_PTR));
	nvf.src = Real2Host(ds_readd(BUFFER9_PTR3));
	nvf.no = 0;
	nvf.type = (!ds_readbs(DNG_FLOOR_TEX)) ? 3 : 5;
	nvf.width = (unsigned char*)&width;
	nvf.height = (unsigned char*)&height;
	process_nvf(&nvf);


	/* Load ceiling */
	nvf.dst = Real2Host(ds_readd(RENDERBUF_PTR)) + 0x4030;
	nvf.src = Real2Host(ds_readd(BUFFER9_PTR3));
	nvf.no = (!(ds_readbs(DIRECTION)&1)) ? 1 : 2;
	nvf.type = (!ds_readbs(DNG_FLOOR_TEX)) ? 3 : 5;
	nvf.width = (unsigned char*)&width;
	nvf.height = (unsigned char*)&height;
	process_nvf(&nvf);

}

#if defined(__BORLANDC__)
static
#endif
void DNG_turn(void)
{
	signed short tmp;

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 3)))) == 15) || (tmp == 1)) {
		ds_writeb((VISUAL_FIELD_VALS + 3), 0);
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 7)))) == 15) || (tmp == 1)) {
		ds_writeb((VISUAL_FIELD_VALS + 7), 0);
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 8)))) == 15) || (tmp == 1)) {
		ds_writeb((VISUAL_FIELD_VALS + 8), 0);
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 14)))) == 15) || (tmp == 1)) {
		ds_writeb((VISUAL_FIELD_VALS + 14), 0);
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 15)))) == 15) || (tmp == 1)) {
		ds_writeb((VISUAL_FIELD_VALS + 15), 0);
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 21)))) == 15) || (tmp == 1)) {
		ds_writeb((VISUAL_FIELD_VALS + 21), 0);
	}

	if ( ((tmp = div16(ds_readb(VISUAL_FIELD_VALS))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 9),
			ds_writeb((VISUAL_FIELD_VALS + 15),
			ds_writeb((VISUAL_FIELD_VALS + 22), 0)));

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 5)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 4),
				ds_writeb((VISUAL_FIELD_VALS + 10),
				ds_writeb((VISUAL_FIELD_VALS + 16),
				ds_writeb((VISUAL_FIELD_VALS + 23), 0))));
		}
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 2)))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 13),
			ds_writeb((VISUAL_FIELD_VALS + 21),
			ds_writeb((VISUAL_FIELD_VALS + 28), 0)));

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 5)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 6),
				ds_writeb((VISUAL_FIELD_VALS + 12),
				ds_writeb((VISUAL_FIELD_VALS + 20),
				ds_writeb((VISUAL_FIELD_VALS + 27), 0))));
		}
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 4)))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 9),
			ds_writeb((VISUAL_FIELD_VALS + 16),
			ds_writeb((VISUAL_FIELD_VALS + 22),
			ds_writeb((VISUAL_FIELD_VALS + 23), 0))));

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 5)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 10), 0);
		}

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 11)))) == 15) || (tmp == 1)) {
				ds_writeb((VISUAL_FIELD_VALS + 10),
					ds_writeb((VISUAL_FIELD_VALS + 17),
					ds_writeb((VISUAL_FIELD_VALS + 18), 0)));

				ds_writeb((VISUAL_FIELD_VALS + 24),
					ds_writeb((VISUAL_FIELD_VALS + 25), 0));
		}
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 5)))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 11),
			ds_writeb((VISUAL_FIELD_VALS + 17),
			ds_writeb((VISUAL_FIELD_VALS + 18), 0)));

		ds_writeb((VISUAL_FIELD_VALS + 19),
			ds_writeb((VISUAL_FIELD_VALS + 24),
			ds_writeb((VISUAL_FIELD_VALS + 25), 0)));

		ds_writeb((VISUAL_FIELD_VALS + 26), 0);
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 6)))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 13),
			ds_writeb((VISUAL_FIELD_VALS + 20),
			ds_writeb((VISUAL_FIELD_VALS + 27),
			ds_writeb((VISUAL_FIELD_VALS + 28), 0))));

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 5)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 11),
				ds_writeb((VISUAL_FIELD_VALS + 12),
				ds_writeb((VISUAL_FIELD_VALS + 17), 0)));

			ds_writeb((VISUAL_FIELD_VALS + 18),
				ds_writeb((VISUAL_FIELD_VALS + 19),
				ds_writeb((VISUAL_FIELD_VALS + 24), 0)));

			ds_writeb((VISUAL_FIELD_VALS + 25),
				ds_writeb((VISUAL_FIELD_VALS + 26), 0));

		}

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 11)))) == 15) || (tmp == 1)) {
				ds_writeb((VISUAL_FIELD_VALS + 12),
					ds_writeb((VISUAL_FIELD_VALS + 18),
					ds_writeb((VISUAL_FIELD_VALS + 19), 0)));

				ds_writeb((VISUAL_FIELD_VALS + 25),
					ds_writeb((VISUAL_FIELD_VALS + 26), 0));
		}
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 9)))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 22), 0);

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 17)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 16),
				ds_writeb((VISUAL_FIELD_VALS + 23), 0));
		}
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 13)))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 28), 0);

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 19)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 20),
				ds_writeb((VISUAL_FIELD_VALS + 27), 0));
		}
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 10)))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 9),
			ds_writeb((VISUAL_FIELD_VALS + 16),
			ds_writeb((VISUAL_FIELD_VALS + 22),
			ds_writeb((VISUAL_FIELD_VALS + 23), 0))));

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 11)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 18), 0);
		}

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 11)))) == 15) || (tmp == 1) ||
			((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 18)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 17),
				ds_writeb((VISUAL_FIELD_VALS + 24),
				ds_writeb((VISUAL_FIELD_VALS + 25), 0)));
		}
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 12)))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 13),
			ds_writeb((VISUAL_FIELD_VALS + 20),
			ds_writeb((VISUAL_FIELD_VALS + 27),
			ds_writeb((VISUAL_FIELD_VALS + 28), 0))));

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 11)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 18), 0);
		}

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 11)))) == 15) || (tmp == 1) ||
			((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 18)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 19),
				ds_writeb((VISUAL_FIELD_VALS + 25),
				ds_writeb((VISUAL_FIELD_VALS + 26), 0)));
		}
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 11)))) == 15) || (tmp == 1)) {
		ds_writeb((VISUAL_FIELD_VALS + 18),
			ds_writeb((VISUAL_FIELD_VALS + 25), 0));
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 16)))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 22), 0);

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 24)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 23), 0);
		}
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 20)))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 28), 0);

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 26)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 27), 0);
		}
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 18)))) == 15) || (tmp == 1)) {

		ds_writeb((VISUAL_FIELD_VALS + 25), 0);
	}

	if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 18)))) == 15) || (tmp == 1) ||
		((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 25)))) == 15) || (tmp == 1)) {

		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 17)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 24), 0);
		}
		if ( ((tmp = div16(ds_readb((VISUAL_FIELD_VALS + 19)))) == 15) || (tmp == 1)) {

			ds_writeb((VISUAL_FIELD_VALS + 26), 0);
		}

	}
}

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

void DNG_stub2(void)
{
	signed short tmp;

	tmp = div16(ds_readb((VISUAL_FIELD_VALS + 1)));

	if ((tmp == 2) || (tmp == 9)) {

		if (div16(ds_readb((VISUAL_FIELD_VALS + 5))) == 15) {
			DNG_draw_walls( ((ds_readb(DUNGEON_GFX_STYLE) == 1) ? 0x4e :
						((ds_readb(DUNGEON_GFX_STYLE) == 2) ? 0x28 : 0x3e)),
					0, 0x36);
		}
	}
}

void DNG_stub3(void)
{
	signed short i;
	signed short tmp;

	for (i = 21; i >= 0; i--) {

		tmp = div16(ds_readb(VISUAL_FIELD_VALS + i));

		ds_writeb(VISUAL_FIELDS_TEX + i,
			(tmp == 6) ? ds_readb(DNG_STUB3_UNKN1 + i) :
				(tmp == 5) ? ds_readb(DNG_STUB3_UNKN2 + i) :
				(tmp == 3) ? ds_readb(DNG_STUB3_UNKN4 + i) :
				(tmp == 4) ? ds_readb(DNG_STUB3_UNKN3 + i) :
				(tmp == 7) ? ds_readb(DNG_STUB3_UNKN5 + i) :
				(tmp == 1) ? ds_readb(DNG_STUB3_UNKN6 + i) :
				(tmp == 2) ? ds_readb(DNG_STUB3_UNKN7 + i) :
				(tmp == 9) ? ds_readb(DNG_STUB3_UNKN8 + i) :
				(tmp == 8) ? ds_readb(DNG_STUB3_UNKN9 + i) :
				(tmp == 10) ? ds_readb(DNG_STUB3_UNKN0 + i) :
				(tmp == 11) ? ds_readb(DNG_STUB3_UNKN0 + i) :
				(tmp != 15) ? -1 : ds_readb(DNG_STUB3_UNKN0 + i));
	}
}

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

	if ((a3 >= 8) && (a3 <= 13) && !ds_readbs(DNG_FLOOR_TEX)) {
		a3 += 6;
	} else {
		if ((a3 >= 14) && (a3 <= 19) && !ds_readbs(DNG_FLOOR_TEX)) {
			a3 -= 6;
		}
	}

	nvf.dst = dst_ptr = Real2Host(ds_readd(RENDERBUF_PTR)) + 0x7530;
	nvf.src = Real2Host(ds_readd(BUFFER9_PTR3));
	nvf.no = a3;
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

		ptr2 = Real2Host(ds_readd(RENDERBUF_PTR)) + 208 * a2 + a1;

		if (!flag) {
			copy_solid(ptr2, dst_ptr, width2, height2, 208, width, 128);
		} else {
			copy_solid_permuted(ptr2, dst_ptr + width2 - 1, width2, height2, 208, width, 128);
		}
	}
}

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

		l3 = ds_readbs(DNG_STUB4_UNKN4 + i);

		l5 = ds_readbs(VISUAL_FIELD_VALS + l3);

		l4 = (unsigned char)div16(l5);

		ptr = (l4 == 6) ? Real2Host(RealMake(datseg, 4 * l3)) + DNG_STUB4_UNKN2 :
			(l4 == 5) ? p_datseg + DNG_STUB4_UNKN3 + l3 * 4 :
			(l4 == 4) ? p_datseg + DNG_STUB4_UNKN1 + l3 * 4 :
			(l4 == 3) ? p_datseg + DNG_STUB4_UNKN1 + l3 * 4 : p_datseg + DNG_STUB4_UNKN0 + l3 * 4;


		l1 = host_readws(ptr);
		l2 = host_readws(ptr + 2);
		l3 = ds_readbs(VISUAL_FIELDS_TEX + l3);

		if (l3 != -1) {

			ptr = Real2Host(ds_readd(DNG_GFXTAB)) + (l3 - 1) * 18;

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

void DNG_stub5(void)
{
	draw_compass();

	ds_writew(PIC_COPY_X1, ds_readw(ANI_POSX));
	ds_writew(PIC_COPY_Y1, ds_readw(ANI_POSY));
	ds_writew(PIC_COPY_X2, ds_readw(ANI_POSX) + 207);
	ds_writew(PIC_COPY_Y2, ds_readw(ANI_POSY) + 134);
	ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));

	update_mouse_cursor();

	ds_writeb(SPECIAL_SCREEN, 0);

	wait_for_vsync();

	do_pic_copy(1);

	DNG_lights();

	refresh_screen_size();
}

/**
 * \brief   check for stafflevel >= 2 in current group
 *
 * \return              0 = false, 1 = true
 */
signed short is_staff_lvl2_in_group(void)
{
	Bit8u *hero_i;
	signed short i;

	hero_i = get_hero(0);
	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

		if (host_readbs(hero_i + HERO_TYPE) &&
			(host_readbs(hero_i + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
			check_hero(hero_i) &&
			(host_readbs(hero_i + HERO_STAFFSPELL_LVL) >= 2))
		{
			return 1;
		}
	}

	return 0;
}

void DNG_lights(void)
{
	signed short l1;
	signed char l2;

	signed short i;

	if (div16(ds_readb((VISUAL_FIELD_VALS + 1))) != 11) {

		/* copy palette */
		memcpy(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), Real2Host(ds_readd(BUFFER11_PTR)), 0xc0);

		if (!(ds_readb((VISUAL_FIELD_VALS + 1)) & 1)) {

			if (ds_readd(INGAME_TIMERS + 4 * INGAME_TIMER_DARKNESS)) {
				l1 = 10;
			} else if (ds_readd(INGAME_TIMERS + 4 * INGAME_TIMER_FLIM_FLAM) || is_staff_lvl2_in_group()) {
				l1 = 0;
			} else {
				if ( (l1 = get_max_light_time()) != -1) {
					l1 = 10 - l1;
				} else {
					l1 = 10;
				}
			}
		} else {
			if (ds_readd(INGAME_TIMERS + 4 * INGAME_TIMER_DARKNESS)) {
				l1 = 10;
			} else {
				l1 = 0;
			}
		}

		ds_writeb(DUNGEON_LIGHT, (l1 == 9) ? 1 : (l1 == 10) ? 2 : 0);
		l1 *= 3;

		for (i = 0; i < 0xc0; i++) {

			l2 = host_readbs(Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + i) - l1;

			if (l2 < 0) {
				l2 = 0;
			}

			host_writeb(Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + i, l2);
		}

		wait_for_vsync();
		set_palette(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), 0x80, 0x40);
	}
}

/**
 * \brief   performs a step (forward or backward) in a dungeon. time advances by 1 minute.
 *
 * \param 	foward	1: forward step; -1: backward step
 */
void DNG_timestep(signed short forward)
{
	signed short dir;

	timewarp(MINUTES(1));

	if ((ds_readws(DEATHTRAP) != 0) && !(dec_ds_ws(DEATHTRAP_STEPS))) {
		/* oh oh: death trap is activated and there are no remaining steps in the dungeon... */

		if (ds_readws(DEATHTRAP) == 1) {
			/* It is the death trap on the Totenschiff. -> it sinks...  */

			load_ani(18);
			init_ani(1);

			GUI_output(get_tx(23));
		}

		ds_writeb(DUNGEON_INDEX, DUNGEONS_NONE);

		/* exit game */
		ds_writew(GAME_STATE, GAME_STATE_DEAD);
	}

	if (ds_readw(GAME_STATE) == GAME_STATE_MAIN) {

		dir = ds_readbs(DIRECTION);

		if (forward == 1) {

			/* go forward */

			if (!dir) {
				/* north */
				dec_ds_ws(Y_TARGET);
			} else if (dir == EAST) {
				/* east */
				inc_ds_ws(X_TARGET);
			} else if (dir == SOUTH) {
				/* south */
				inc_ds_ws(Y_TARGET);
			} else {
				/* west */
				dec_ds_ws(X_TARGET);
			}
		} else {

			/* go backward */

			if (!dir) {
				/* north */
				inc_ds_ws(Y_TARGET);
			} else if (dir == EAST) {
				/* east */
				dec_ds_ws(X_TARGET);
			} else if (dir == SOUTH) {
				/* south */
				dec_ds_ws(Y_TARGET);
			} else {
				/* west */
				inc_ds_ws(X_TARGET);
			}
		}

		xor_ds_bs(DNG_FLOOR_TEX, 1);
	}
}

void DNG_update_pos(void)
{
	set_var_to_zero();

	DNG_stub1();

	ds_writew(DNG_REFRESH_X_TARGET, ds_readw(X_TARGET));
	ds_writew(DNG_REFRESH_Y_TARGET, ds_readw(Y_TARGET));
	ds_writew(DNG_REFRESH_DIRECTION, ds_readbs(DIRECTION));

}

void DNG_inc_level(void)
{
	ds_writew(DNG_LEVEL_CHANGED, 1);
	inc_ds_bs_post(DUNGEON_LEVEL);
	load_area_description(1);
	DNG_update_pos();

}

void DNG_dec_level(void)
{
	ds_writew(DNG_LEVEL_CHANGED, 1);
	dec_ds_bs_post(DUNGEON_LEVEL);
	load_area_description(1);
	DNG_update_pos();

}

void DNG_open_door(void)
{
	signed short x;
	signed short y;
	signed short iters;
	signed short i;

	DNG_floor_ceil();
	move();
	ds_writebs((VISUAL_FIELD_VALS + 5), 32);
	DNG_turn();
	DNG_stub3();
	DNG_stub4();

	memmove(Real2Host(ds_readd(RENDERBUF_PTR)) + 0x7530, Real2Host(ds_readd(RENDERBUF_PTR)), 0x6db0);

	if (!ds_readb(DUNGEON_GFX_STYLE)) {
		x = 45;
		y = 38;
		iters = 19;
	} else if (ds_readb(DUNGEON_GFX_STYLE) == 1) {
		x = 47;
		y = 30;
		iters = 20;
	} else {
		x = 54;
		y = 44;
		iters = 17;
	}

	play_voc(ARCHIVE_FILE_FX15_VOC);

	update_mouse_cursor();

	for (i = 0; i < iters; i++) {

		door_frame(31, x, y, (i * 4) + 1);

		DNG_stub5();

		memmove(Real2Host(ds_readd(RENDERBUF_PTR)), Real2Host(ds_readd(RENDERBUF_PTR)) + 0x7530, 0x6db0);
	}

	refresh_screen_size();
}

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

	memmove(Real2Host(ds_readd(RENDERBUF_PTR)) + 0x7530, Real2Host(ds_readd(RENDERBUF_PTR)), 0x6db0);

	if (!ds_readb(DUNGEON_GFX_STYLE)) { /* dungeon graphics: wood */
		x = 45;
		y = 38;
		iters = 18;
	} else if (ds_readb(DUNGEON_GFX_STYLE) == 1) { /* dungeon graphics: marble */
		x = 47;
		y = 30;
		iters = 19;
	} else { /* dungeon graphics: stone */
		x = 54;
		y = 44;
		iters = 16;
	}

	play_voc(ARCHIVE_FILE_FX15_VOC);

	update_mouse_cursor();

	for (i = iters; i >= 0; i--) {

		door_frame(31, x, y, (i * 4) + 1);

		DNG_stub5();

		memmove(Real2Host(ds_readd(RENDERBUF_PTR)), Real2Host(ds_readd(RENDERBUF_PTR)) + 0x7530, 0x6db0);
	}

	refresh_screen_size();
}

void DNG_stub6(void)
{
	signed short l_si;
	signed short l_di;
	signed short l1;
	Bit8u *hero_auto;
	Bit8u *hero1;
	Bit8u *hero2;

	play_voc(ARCHIVE_FILE_FX18_VOC);

	if (ds_readb(DUNGEON_LIGHT) != 0) {

		if (DNG_fallpit(6)) {
			ds_writew(X_TARGET, ds_readw(X_TARGET_BAK));
			ds_writew(Y_TARGET, ds_readw(Y_TARGET_BAK));
		}
	} else {

		if (((l_si = DNG_check_climb_tools()) != -1) &&
			((l_di = count_heroes_available_in_group()) > 1))
		{

			/* ropes oder staff */
			host_writeb(Real2Host(ds_readd(DTP2)), 0);

			if (l_si) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(768),
					get_hero(l_si - 1) + HERO_NAME2);
			}

			if (l_di == 2) {

				hero1 = Real2Host(get_first_hero_available_in_group());

				hero2 = Real2Host(get_second_hero_available_in_group());

				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					(char*)get_ttx(769),
					hero1 + HERO_NAME2,
					hero2 + HERO_NAME2);

				strcat((char*)Real2Host(ds_readd(DTP2)),
					(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

				if (test_attrib(hero1, ATTRIB_GE, 2) <= 0) {

					sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_ttx(770),
						hero1 + HERO_NAME2,
						hero2 + HERO_NAME2,
						l_si = random_schick(3) + 1);

					strcat((char*)Real2Host(ds_readd(DTP2)),
						(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

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
							(char*)((l_di >= 2) ? p_datseg + DNG_STUB6_STR_COMMA : p_datseg+ DNG_STUB6_STR_AND));
					}

				} while (l_di);

				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					(char*)get_ttx(771),
					hero_auto + HERO_NAME2,
					Real2Host(GUI_get_ptr(host_readbs(hero_auto + HERO_SEX), 0)),
					Real2Host(GUI_get_ptr(host_readbs(hero_auto + HERO_SEX), 2)));

				strcat((char*)Real2Host(ds_readd(DTP2)),
					(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			}

			GUI_output(Real2Host(ds_readd(DTP2)));
		} else {

			sub_group_le(random_schick(5));
		}

		DNG_inc_level();
	}
}


signed short DNG_check_climb_tools(void)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);

	/* check for a mage with staffspell > 2 */
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
			!hero_dead(hero) && /* TODO: potential Original-Bug: What if petrified / unconscious etc.? Compare to is_staff_lvl2_in_group where check_hero is called */
			(host_readbs(hero + HERO_TYPE) == HERO_TYPE_MAGE) &&
			(host_readbs(hero + HERO_STAFFSPELL_LVL) > 2))
		{
			return i + 1;
		}
	}

	/* check for ladder or rope */
	return ((get_first_hero_with_item(ITEM_ROPE) != -1) ||
			(get_first_hero_with_item(ITEM_ROPE_LADDER) != -1)) ? 0 : -1;
}

signed short DNG_fallpit(signed short max_damage)
{
	signed short hero_id;
	signed short i;
	signed short nr_fallen_heroes;
	signed short new_group;
	signed short retval;

	new_group = 0;
	retval = 0;

	ds_writew(DNG_LEVEL_CHANGED, 1);
	nr_fallen_heroes = random_schick(ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)));

	/* If the result was rolled that all but one hero of the active group should fall down, all heroes will fall down.
	 * Reason probably: Avoid that the NPC gets separated into a single group (as he might be the single hero not falling down) */

	if (ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) - 1 == nr_fallen_heroes) {
		nr_fallen_heroes = ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));
	}

	if (ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) != nr_fallen_heroes) {
		/* only a part of the heroes of the active group falls down */

		/* find empty group */
		while (ds_readbs(GROUP_MEMBER_COUNTS + new_group)) {
			new_group++;
		}

		for (i = 0; i < nr_fallen_heroes; i++) {

			do {
				hero_id = random_schick(7) - 1;

			} while ( (!host_readbs(get_hero(hero_id) + HERO_TYPE)) ||
					(host_readbs(get_hero(hero_id) + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP)) ||
					((nr_fallen_heroes == 1) && (hero_id == 6))); /* avoid that the NPC gets separated into a single group */

			host_writeb(get_hero(hero_id) + HERO_GROUP_NO, (unsigned char)new_group);
			inc_ds_bs_post(GROUP_MEMBER_COUNTS + new_group);
			dec_ds_bs_post(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));
			sub_hero_le(get_hero(hero_id), random_schick(max_damage));
		}

		GRP_save_pos(new_group);
		ds_writeb(GROUPS_DNG_LEVEL + new_group, ds_readbs(DUNGEON_LEVEL) + 1);

		retval = 1;

	} else {

		hero_id = 0;

		for (i = 0; i < nr_fallen_heroes; i++) {

			while (!host_readbs(get_hero(hero_id) + HERO_TYPE) ||
				(host_readbs(get_hero(hero_id) + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP)))
			{
				hero_id++;
			}

			sub_hero_le(get_hero(hero_id++), random_schick(max_damage));
		}

		DNG_inc_level();
	}

	return retval;
}

void DNG_enter_dungeon(signed short dungeon_id)
{
	signed short x_pos;
	signed short y_pos;
	signed short level;
	signed short dir;
	signed short i;
	Bit8u *ptr;

	switch (dungeon_id) {

		case DUNGEONS_TOTENSCHIFF:  x_pos = 9;  y_pos = 11;  dir = NORTH;  level = 2;  break;
		case DUNGEONS_VERFALLENE_HERBERGE:  x_pos = 1;  y_pos = 1;   dir = SOUTH;  level = 0;  break;
		case DUNGEONS_SPINNENHOEHLE:  x_pos = 1;  y_pos = 8;   dir = EAST;  level = 0;  break;

#if !defined(__BORLANDC__)
		case DUNGEONS_WOLFSHOEHLE:  x_pos = 7;  y_pos = 14;  dir = NORTH;  level = 0;  break;
#else
mark1:		case DUNGEONS_WOLFSHOEHLE:  x_pos = 7;  y_pos = 14;  dir = NORTH;  level = 0;  break;
#endif

		case DUNGEONS_GOBLINHOEHLE:  x_pos = 6;  y_pos = 14;  dir = NORTH;  level = 0;  break;
		case DUNGEONS_DASPOTASCHATZ:  x_pos = 13; y_pos = 14;  dir = NORTH;  level = 0;  break;
		case DUNGEONS_RUINE_DES_SCHWARZMAGIERS:  x_pos = 1;  y_pos = 13;  dir = NORTH;  level = 0;  break;
		case DUNGEONS_ORKBEHAUSUNG:  x_pos = 1;  y_pos = 14;  dir = EAST;  level = 0;  break;

#if !defined(__BORLANDC__)
		case DUNGEONS_KULTSTAETTE_DES_NAMENLOSEN:  x_pos = 7;  y_pos = 14;  dir = NORTH;  level = 0;  break;
#else
		case DUNGEONS_KULTSTAETTE_DES_NAMENLOSEN:  goto mark1;
mark2:			   goto mark1;
#endif

		case DUNGEONS_DRACHENHORT:  x_pos = 1;  y_pos = 3;   dir = EAST;  level = 0;  break;

#if !defined(__BORLANDC__)
		case DUNGEONS_PIRATENHOEHLE:  x_pos = 7;  y_pos = 14;  dir = NORTH;  level = 0;  break;
#else
		case DUNGEONS_PIRATENHOEHLE:  goto mark2;
#endif

		case DUNGEONS_ZWERGENFESTE:  x_pos = 13; y_pos = 14;  dir = NORTH;  level = 0;  break;
		case DUNGEONS_VERLASSENE_MINE: {
			x_pos = 7;
			y_pos = 14;
			dir = NORTH;
			level = 0;

			ds_writeb(DNG13_COLLAPSECOUNT, 0);
			ds_writeb(DNG13_HEROCOUNT, (signed char)count_heroes_in_group());
			break;
		}
		case DUNGEONS_ZWINGFESTE: {
			x_pos = 1;
			y_pos = 14;
			dir = WEST;
			level = ds_writebs(LOCATION, 0);
			break;
		}
		case DUNGEONS_HYGGELIKS_RUINE:  x_pos = 1;  y_pos = 11;   dir = EAST;  level = 0;  break;
	}

	ds_writew(X_TARGET, x_pos);
	ds_writew(Y_TARGET, y_pos);
	ds_writeb(DIRECTION, (signed char)dir);
	ds_writeb(DUNGEON_LEVEL, (signed char)level);
	ds_writeb(DUNGEON_INDEX, (signed char)dungeon_id);
	ds_writebs(LOCATION_BAK, ds_readbs(LOCATION));
	ds_writeb(CURRENT_TOWN_BAK, ds_readb(CURRENT_TOWN));
	ds_writeb(LOCATION, ds_writeb(CURRENT_TOWN, TOWNS_NONE));
	ds_writeb(DNG_AREA_LOADED, ds_writeb(CITY_AREA_LOADED, -1));

	if (dungeon_id == DUNGEONS_ZWINGFESTE) {

		ptr = Real2Host(ds_readd(RENDERBUF_PTR)) + 0x1f4;
		memset(Real2Host(ds_readd(RENDERBUF_PTR)), 0, 0x120);
		memcpy(Real2Host(ds_readd(RENDERBUF_PTR)) + 0x1f4, p_datseg + PALETTE_FLOOR, 0x120);

		for (i = 0; i < 0x40; i++) {

			pal_fade(ptr, Real2Host(ds_readd(RENDERBUF_PTR)));
			pal_fade(ptr + 0x60, Real2Host(ds_readd(RENDERBUF_PTR)) + 0x60);
			pal_fade(ptr + 0xc0, Real2Host(ds_readd(RENDERBUF_PTR)) + 0xc0);

			wait_for_vsync();

			set_palette(ptr, 0, 0x20);
			set_palette(ptr + 0x60, 0x80, 0x40);
		}

		do_fill_rect((RealPt)ds_readd(RENDERBUF_PTR), 0, 0, 319, 199, 0);
		ds_writew(PIC_COPY_X1, 0);
		ds_writew(PIC_COPY_Y1, 0);
		ds_writew(PIC_COPY_X2, 240);
		ds_writew(PIC_COPY_Y2, 136);
		ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));
		update_mouse_cursor();
		do_pic_copy(1);
		refresh_screen_size();
	}
}

#if !defined(__BORLANDC__)
}
#endif

