/**
 *	Rewrite of DSA1 v3.02_de functions of seg049 (group management)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg049.cpp
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg028.h"
#include "seg029.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg104.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

int GRP_compare_heros(const void *p1, const void *p2)
{
	Bit8u *hero1, *hero2;

	hero1 = (Bit8u*)p1;
	hero2 = (Bit8u*)p2;

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)))
	{
		if (host_readbs(hero1 + HERO_GROUP_POS) < host_readbs(hero2 + HERO_GROUP_POS))
		{
			return -1;
		} else {
			return 1;
		}
	}

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP)))
	{
		return -1;
	}

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)))
	{
		return 1;
	}

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP)))
	{
		if (host_readbs(hero1 + HERO_GROUP_POS) < host_readbs(hero2 + HERO_GROUP_POS))
		{
			return -1;
		} else {
			return 1;
		}
	}

	if (!(host_readbs(hero1 + HERO_TYPE)) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)))
	{
		return 1;
	}

	if (!(host_readbs(hero1 + HERO_TYPE)) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP)))
	{
		return -1;
	}

	if (!(host_readbs(hero1 + HERO_TYPE)) &&
		!(host_readbs(hero2 + HERO_TYPE)))
	{
		return 0;
	}

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
		!(host_readbs(hero2 + HERO_TYPE)))
	{
		return -1;
	}

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP)) &&
		!(host_readbs(hero2 + HERO_TYPE)))
	{
		return 1;
	}

	return 0;
}

void GRP_sort_heros(void)
{
	signed short i;

	qsort((void*)get_hero(0), 6, SIZEOF_HERO, GRP_compare_heros);

	for (i = 0; i < 6; i++) {
		host_writeb(get_hero(i) + HERO_GROUP_POS, i + 1);
	}
}

void GRP_save_pos(signed short group)
{
	signed short refresh = 0;

	if (group & 0x8000) {
		refresh = 1;
	}

	group &= 0x7fff;

	GRP_sort_heros();

	ds_writeb(GROUPS_DIRECTION + group, ds_readbs(DIRECTION));

	ds_writew(GROUPS_X_TARGET + group * 2, ds_readws(X_TARGET));
	ds_writew(GROUPS_Y_TARGET + group * 2, ds_readws(Y_TARGET));

	ds_writeb(GROUPS_LOCATION + group, ds_readbs(LOCATION));
	ds_writeb(GROUPS_TOWN + group, ds_readbs(CURRENT_TOWN));
	ds_writeb(GROUPS_DNG_INDEX + group, ds_readbs(DUNGEON_INDEX));
	ds_writeb(GROUPS_DNG_LEVEL + group, ds_readbs(DUNGEON_LEVEL));
	ds_writeb(GROUPS_DIRECTION_BAK + group, ds_readbs(DIRECTION_BAK));

	ds_writew(GROUPS_X_TARGET_BAK + group * 2, ds_readws(X_TARGET_BAK));
	ds_writew(GROUPS_Y_TARGET_BAK + group * 2, ds_readws(Y_TARGET_BAK));

	ds_writeb(GROUPS_LOCATION_BAK + group, ds_readbs(LOCATION_BAK));
	ds_writeb(GROUPS_TOWN_BAK + group, ds_readbs(CURRENT_TOWN_BAK));
	ds_writeb(GROUPS_DNG_INDEX_BAK + group, ds_readbs(DUNGEON_INDEX_BAK));
	ds_writeb(GROUPS_DNG_LEVEL_BAK + group, ds_readbs(DUNGEON_LEVEL_BAK));

	if (!refresh) {
		draw_status_line();
	}
}

void GRP_split(void)
{
	signed short new_group;
	signed short not_empty;
	signed short answer;

	if (count_heroes_available_in_group() <= (host_readbs(get_hero(6) + HERO_TYPE) != HERO_TYPE_NONE ? 2 : 1)) {

		GUI_output(get_ttx(514));
	} else {

		not_empty = 0;
		new_group = 0;

		while (ds_readbs(GROUP_MEMBER_COUNTS + new_group) != 0) {
			new_group++;
		}

		do {
			ds_writeb(HERO_SEL_EXCLUDE, 6);
			answer = select_hero_from_group(get_ttx(515));

			if (answer == -1) {
				break;
			} else {

				not_empty = 1;
				host_writeb(get_hero(answer) + HERO_GROUP_NO, (signed char)new_group);
				inc_ds_bs_post(GROUP_MEMBER_COUNTS + new_group);
				dec_ds_bs_post(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));
			}

		} while	(count_heroes_available_in_group() > (host_readbs(get_hero(6) + HERO_TYPE) != HERO_TYPE_NONE ? 2 : 1));

		if (not_empty) {
			GRP_save_pos(new_group);
		}
	}
}

void GRP_merge(void)
{
	signed short answer;
	signed short i;

	answer = can_merge_group();

	if (answer == -1) {

		GUI_output(get_ttx(516));
	} else {

		do {

			ds_writeb(GROUPS_DIRECTION + answer, (signed char)
				(ds_writew(GROUPS_X_TARGET + answer * 2,
				ds_writew(GROUPS_Y_TARGET + answer * 2,
				ds_writebs(GROUPS_TOWN + answer,
				ds_writeb(GROUPS_DNG_INDEX + answer,
				ds_writeb(GROUPS_DNG_LEVEL + answer,
				ds_writeb(GROUPS_DIRECTION_BAK + answer, (signed char)
				(ds_writew(GROUPS_X_TARGET_BAK + answer * 2,
				ds_writew(GROUPS_Y_TARGET_BAK + answer * 2,
				ds_writebs(GROUPS_LOCATION_BAK + answer,
				ds_writeb(GROUPS_TOWN_BAK + answer,
				ds_writeb(GROUPS_DNG_INDEX_BAK + answer,
				ds_writeb(GROUPS_DNG_LEVEL_BAK + answer, 0)))))))))))))));

			ds_writeb(GROUP_MEMBER_COUNTS + answer, 0);

			for (i = 0; i <= 6; i++) {

				if ((host_readbs(get_hero(i) + HERO_TYPE) != HERO_TYPE_NONE) &&
					host_readbs(get_hero(i) + HERO_GROUP_NO) == answer)
				{
					host_writeb(get_hero(i) + HERO_GROUP_NO, ds_readbs(CURRENT_GROUP));
					inc_ds_bs_post(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));
				}
			}
			GRP_sort_heros();
			answer = can_merge_group();
		} while (answer != -1);

		draw_status_line();
	}
}

void GRP_switch_to_next(signed short mode)
{
	signed short i;
	signed short state;

	signed short group;
	signed short done = 0;
	signed short dng_level;

	group = ds_readbs(CURRENT_GROUP);

	do {
		/* select next group */
		group++;

		if (group == 6) {
			group = 0;
		}

		if (ds_readbs(GROUP_MEMBER_COUNTS + group) != 0) {

			state = 0;

			for (i = 0; i < 6; i++) {

				if ((host_readbs(get_hero(i) + HERO_TYPE) != HERO_TYPE_NONE) &&
					(host_readbs(get_hero(i) + HERO_GROUP_NO) == group) &&
					check_hero(get_hero(i)))
				{
					if (host_readbs(get_hero(i) + HERO_JAIL) != 0) {
						/* hero is in prison */
						state = 2;
					} else {
						/* hero is free */
						state = 1;
					}
				}
			}

			if (!state || state == 2) {

				if (mode == 0) {

					if (!state) {
						GUI_output(get_ttx(36));
					} else {
						if ((state == 2) && GUI_bool(get_ttx(773))) {
							timewarp_until(0x7e90);
							done = 1;
						}
					}
				}

			} else {

				if (state || mode != 2) {
					done = 1;
				}
			}
		}

	} while (done == 0);

	if (ds_readbs(CURRENT_GROUP) != group) {

		if ( ( (ds_readbs(CURRENT_TOWN) != 0) && !ds_readbs(GROUPS_TOWN + group)) ||
			(!ds_readbs(CURRENT_TOWN) && (ds_readbs(GROUPS_TOWN + group) != 0)))
		{
			set_palette(p_datseg + 0x26c3, 0x00, 0x20);
			set_palette(p_datseg + 0x26c3, 0x80, 0x20);
			set_palette(p_datseg + 0x26c3, 0xa0, 0x20);
		}

		if ((ds_readbs(GROUPS_DNG_INDEX + group)) && (ds_readbs(GROUPS_DNG_INDEX + group) != ds_readbs(DUNGEON_INDEX)))
		{
			ds_writeb(DNG_AREA_LOADED, -1);
			ds_writew(AREA_PREPARED, -1);
		}

		if ((ds_readbs(GROUPS_TOWN + group)) && (ds_readbs(GROUPS_TOWN + group) != ds_readbs(CURRENT_TOWN)))
		{
			ds_writeb(CITY_AREA_LOADED, -1);
			ds_writew(AREA_PREPARED, -1);
		}

		/* save positions from the old group */
		ds_writeb(GROUPS_DIRECTION + ds_readbs(CURRENT_GROUP), ds_readbs(DIRECTION));
		ds_writew(GROUPS_X_TARGET + ds_readbs(CURRENT_GROUP) * 2, ds_readw(X_TARGET));
		ds_writew(GROUPS_Y_TARGET + ds_readbs(CURRENT_GROUP) * 2, ds_readw(Y_TARGET));
		ds_writeb(GROUPS_LOCATION + ds_readbs(CURRENT_GROUP), ds_readbs(LOCATION));
		ds_writeb(GROUPS_TOWN + ds_readbs(CURRENT_GROUP), ds_readbs(CURRENT_TOWN));
		ds_writeb(GROUPS_DNG_INDEX + ds_readbs(CURRENT_GROUP), ds_readbs(DUNGEON_INDEX));
		ds_writeb(GROUPS_DNG_LEVEL + ds_readbs(CURRENT_GROUP), ds_readbs(DUNGEON_LEVEL));
		ds_writeb(GROUPS_DIRECTION_BAK + ds_readbs(CURRENT_GROUP), ds_readbs(DIRECTION_BAK));
		ds_writew(GROUPS_X_TARGET_BAK + ds_readbs(CURRENT_GROUP) * 2, ds_readw(X_TARGET_BAK));
		ds_writew(GROUPS_Y_TARGET_BAK + ds_readbs(CURRENT_GROUP) * 2, ds_readw(Y_TARGET_BAK));
		ds_writeb(GROUPS_LOCATION_BAK + ds_readbs(CURRENT_GROUP), ds_readbs(LOCATION_BAK));
		ds_writeb(GROUPS_TOWN_BAK + ds_readbs(CURRENT_GROUP), ds_readbs(CURRENT_TOWN_BAK));
		ds_writeb(GROUPS_DNG_INDEX_BAK + ds_readbs(CURRENT_GROUP), ds_readbs(DUNGEON_INDEX_BAK));
		ds_writeb(GROUPS_DNG_LEVEL_BAK + ds_readbs(CURRENT_GROUP), ds_readbs(DUNGEON_LEVEL_BAK));

		/* set positions for the new group */
		ds_writeb(CURRENT_GROUP, (signed char)group);
		ds_writeb(DIRECTION, ds_readb(GROUPS_DIRECTION + group));
		ds_writew(X_TARGET, ds_readw(GROUPS_X_TARGET + group * 2));
		ds_writew(Y_TARGET, ds_readw(GROUPS_Y_TARGET + group * 2));
		ds_writeb(LOCATION, ds_readb(GROUPS_LOCATION + group));
		ds_writeb(CURRENT_TOWN, ds_readb(GROUPS_TOWN + group));
		ds_writeb(DUNGEON_INDEX, ds_readb(GROUPS_DNG_INDEX + group));
		dng_level = ds_readbs(DUNGEON_LEVEL);
		ds_writeb(DUNGEON_LEVEL, ds_readbs(GROUPS_DNG_LEVEL + group));

		if (dng_level != ds_readbs(DUNGEON_LEVEL)) {
			load_area_description(1);
		}

		ds_writeb(DIRECTION_BAK, ds_readb(GROUPS_DIRECTION_BAK + group));
		ds_writew(X_TARGET_BAK, ds_readw(GROUPS_X_TARGET_BAK + group * 2));
		ds_writew(Y_TARGET_BAK, ds_readw(GROUPS_Y_TARGET_BAK + group * 2));
		ds_writeb(LOCATION_BAK, ds_readb(GROUPS_LOCATION_BAK + group));
		ds_writeb(CURRENT_TOWN_BAK, ds_readb(GROUPS_TOWN_BAK + group));
		ds_writeb(DUNGEON_INDEX_BAK, ds_readb(GROUPS_DNG_INDEX_BAK + group));
		ds_writeb(DUNGEON_LEVEL_BAK, ds_readb(GROUPS_DNG_LEVEL_BAK + group));

		GRP_sort_heros();

		for (group = 0; group <= 6; group++) {
			ds_writeb(FOOD_MESSAGE + group, ds_writeb(UNCONSCIOUS_MESSAGE + group, 0));
		}

		ds_writew(REQUEST_REFRESH, 1);
	}
}

struct dummy {
	char a[SIZEOF_HERO];
};

void GRP_swap_heros(void)
{
	signed short hero1_no;
	signed short hero2_no;
	signed char l2;
	signed char l3;
	signed char l4;
	signed char l5;
	signed char i;
	signed char tmp[SIZEOF_HERO];

	if ((ds_readw(HEROSWAP_ALLOWED) == 0) || !ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
		return;
	}

	hero1_no = select_hero_from_group(get_ttx(302));

	if ((hero1_no != -1) && (hero1_no < 6)) {

		hero2_no = select_hero_from_group(get_ttx(303));

		if ((hero2_no != -1) && (hero1_no != hero2_no) && (hero2_no < 6)) {

			for (i = 0; i < 3; i++) {

				if (ds_readbs(WILDCAMP_GUARDS + i) == hero1_no) {
					ds_writebs(WILDCAMP_GUARDS + i, (signed char)hero2_no);
				}
			}

			/* save hero1 in tmp */
			*((struct dummy*)&tmp) = *((struct dummy*)get_hero(hero1_no));

			l2 = ds_readbs(WILDCAMP_GUARDSTATUS + hero1_no);
			l3 = ds_readbs(WILDCAMP_MAGICSTATUS + hero1_no);
			l4 = ds_readbs(WILDCAMP_REPLSTATUS + hero1_no);
			l5 = ds_readbs(WILDCAMP_HERBSTATUS + hero1_no);

			*((struct dummy*)get_hero(hero1_no)) = *((struct dummy*)get_hero(hero2_no));

			ds_writebs(WILDCAMP_GUARDSTATUS + hero1_no, ds_readbs(WILDCAMP_GUARDSTATUS + hero2_no));
			ds_writebs(WILDCAMP_MAGICSTATUS + hero1_no, ds_readbs(WILDCAMP_MAGICSTATUS + hero2_no));
			ds_writebs(WILDCAMP_REPLSTATUS + hero1_no, ds_readbs(WILDCAMP_REPLSTATUS + hero2_no));
			ds_writebs(WILDCAMP_HERBSTATUS + hero1_no, ds_readbs(WILDCAMP_HERBSTATUS + hero2_no));

			*((struct dummy*)get_hero(hero2_no)) = *((struct dummy*)&tmp);

			ds_writebs(WILDCAMP_GUARDSTATUS + hero2_no, l2);
			ds_writebs(WILDCAMP_MAGICSTATUS + hero2_no, l3);
			ds_writebs(WILDCAMP_REPLSTATUS + hero2_no, l4);
			ds_writebs(WILDCAMP_HERBSTATUS + hero2_no, l5);

			if (host_readbs(get_hero(hero1_no) + HERO_TYPE)) {
				host_writebs(get_hero(hero1_no) + HERO_ACTION_ID, FIG_ACTION_UNKNOWN2);
			}

			if (host_readbs(get_hero(hero2_no) + HERO_TYPE)) {
				host_writebs(get_hero(hero2_no) + HERO_ACTION_ID, FIG_ACTION_UNKNOWN2);
			}

			host_writeb(get_hero(hero1_no) + HERO_GROUP_POS, hero1_no + 1);
			host_writeb(get_hero(hero2_no) + HERO_GROUP_POS, hero2_no + 1);
		}
	}

	draw_status_line();
}

void GRP_move_hero(signed short src_pos)
{

	signed short dst_pos;
	Bit8u *src;
	Bit8u *dst;
	signed char src_guardstatus;
	signed char src_magicstatus;
	signed char src_replstatus;
	signed char src_herbstatus;
	signed char i;

	/* dont move NPC */
	if (src_pos != 6) {

		ds_writew(MOUSE_POSY_MIN, 157);
		ds_writew(MOUSE_POSY_MAX, 157);
		ds_writew(MOUSE_POSX_MIN, 1);
		ds_writew(MOUSE_POSX_MAX, 260);

		update_mouse_cursor();

		ds_writew(MOUSE_LOCKED, 1);
		ds_writew(MOUSE_POSY, 157);

		ds_writew(MOUSE_POSX, ds_readw(HERO_PIC_POSX + 2 * src_pos));
		ds_writew(PIC_COPY_X1, ds_readw(MOUSE_POSX));
		ds_writew(PIC_COPY_Y1, ds_readw(MOUSE_POSY));
		ds_writew(PIC_COPY_X2, ds_readw(MOUSE_POSX) + 31);
		ds_writew(PIC_COPY_Y2, ds_readw(MOUSE_POSY) + 31);
		ds_writed(PIC_COPY_SRC, ds_readd(ICON));

		do_save_rect();

		ds_writew(MOUSE_LOCKED, 0);

		refresh_screen_size();

		ds_writew(MOUSE_LOCKED, 1);

		ds_writew(PIC_COPY_X1, ds_readw(HERO_PIC_POSX + 2 * src_pos));
		ds_writew(PIC_COPY_Y1, 157);
		ds_writew(PIC_COPY_X2, ds_readw(HERO_PIC_POSX + 2 * src_pos) + 31);
		ds_writew(PIC_COPY_Y2, 188);
		ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));

		do_save_rect();

		ds_writew(MOUSE_LOCKED, 0);

		update_mouse_cursor();

		ds_writew(MOUSE_POSX_BAK, ds_readw(MOUSE_POSX));
		ds_writew(MOUSE_POSY_BAK, ds_readw(MOUSE_POSY));

		while (ds_readw(MOUSE1_EVENT2) == 0) {
#if !defined(__BORLANDC__)
			/* call DOSBOX to handle mouse ISR */
			wait_for_vsync();
#endif
			if (ds_readw(MOUSE_MOVED) != 0) {

				ds_writew(MOUSE_LOCKED, 1);

				wait_for_vsync();

				ds_writew(PIC_COPY_X1, ds_readw(MOUSE_POSX_BAK));
				ds_writew(PIC_COPY_Y1, ds_readw(MOUSE_POSY_BAK));
				ds_writew(PIC_COPY_X2, ds_readw(MOUSE_POSX_BAK) + 31);
				ds_writew(PIC_COPY_Y2, ds_readw(MOUSE_POSY_BAK) + 31);
				ds_writed(PIC_COPY_SRC, ds_readd(ICON));

				do_pic_copy(0);

				ds_writew(PIC_COPY_X1, ds_readw(MOUSE_POSX));
				ds_writew(PIC_COPY_Y1, ds_readw(MOUSE_POSY));
				ds_writew(PIC_COPY_X2, ds_readw(MOUSE_POSX) + 31);
				ds_writew(PIC_COPY_Y2, ds_readw(MOUSE_POSY) + 31);
				ds_writed(PIC_COPY_SRC, ds_readd(ICON));

				do_save_rect();

				ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));

				do_pic_copy(0);

				ds_writew(MOUSE_POSX_BAK, ds_readw(MOUSE_POSX));
				ds_writew(MOUSE_POSY_BAK, ds_readw(MOUSE_POSY));
				ds_writew(MOUSE_MOVED, 0);
				ds_writew(MOUSE_LOCKED, 0);
			}
		}

		ds_writew(PIC_COPY_X1, ds_readw(MOUSE_POSX_BAK));
		ds_writew(PIC_COPY_Y1, ds_readw(MOUSE_POSY_BAK));
		ds_writew(PIC_COPY_X2, ds_readw(MOUSE_POSX_BAK) + 31);
		ds_writew(PIC_COPY_Y2, ds_readw(MOUSE_POSY_BAK) + 31);
		ds_writed(PIC_COPY_SRC, ds_readd(ICON));

		do_pic_copy(0);

		dst_pos = 6;
		while (ds_readws(HERO_PIC_POSX + --dst_pos * 2) > ds_readws(MOUSE_POSX))
		{
			;
		}

		if (dst_pos < 0) {
			dst_pos = 0;
		}

		if ((src_pos != dst_pos) && (dst_pos != 6)) {

			dst = get_hero(dst_pos);
			src = get_hero(src_pos);

			if (!host_readbs(dst + 0x21) || (host_readbs(dst + 0x87) == ds_readbs(CURRENT_GROUP))) {

				for (i = 0; i < 3; i++) {
					if (ds_readbs(WILDCAMP_GUARDS + i) == src_pos) {
						ds_writebs(WILDCAMP_GUARDS + i, (signed char)dst_pos);
					}
				}

				memcpy(Real2Host(ds_readd(RENDERBUF_PTR)), src, SIZEOF_HERO);

				src_guardstatus = ds_readbs(WILDCAMP_GUARDSTATUS + src_pos);
				src_magicstatus = ds_readbs(WILDCAMP_MAGICSTATUS + src_pos);
				src_replstatus = ds_readbs(WILDCAMP_REPLSTATUS + src_pos);
				src_herbstatus = ds_readbs(WILDCAMP_HERBSTATUS + src_pos);

				*((struct dummy*)src) = *((struct dummy*)dst);

				ds_writeb(WILDCAMP_GUARDSTATUS + src_pos, ds_readbs(WILDCAMP_GUARDSTATUS + dst_pos));
				ds_writeb(WILDCAMP_MAGICSTATUS + src_pos, ds_readbs(WILDCAMP_MAGICSTATUS + dst_pos));
				ds_writeb(WILDCAMP_REPLSTATUS + src_pos, ds_readbs(WILDCAMP_REPLSTATUS + dst_pos));
				ds_writeb(WILDCAMP_HERBSTATUS + src_pos, ds_readbs(WILDCAMP_HERBSTATUS + dst_pos));

				memcpy(dst, Real2Host(ds_readd(RENDERBUF_PTR)), SIZEOF_HERO);

				ds_writeb(WILDCAMP_GUARDSTATUS + dst_pos, src_guardstatus);
				ds_writeb(WILDCAMP_MAGICSTATUS + dst_pos, src_magicstatus);
				ds_writeb(WILDCAMP_REPLSTATUS + dst_pos, src_replstatus);
				ds_writeb(WILDCAMP_HERBSTATUS + dst_pos, src_herbstatus);

				host_writeb(src + 0x84, 100);
				host_writeb(dst + 0x84, 100);
				host_writeb(dst + 0x8a, src_pos + 1);
				host_writeb(src + 0x8a, dst_pos + 1);
			}
		}

		ds_writew(MOUSE1_EVENT2, 0);
		ds_writew(MOUSE_POSY_MIN, 0);
		ds_writew(MOUSE_POSY_MAX, 199);
		ds_writew(MOUSE_POSX_MIN, 0);
		ds_writew(MOUSE_POSX_MAX, 319);

		refresh_screen_size();
		draw_status_line();
	}
}

void GRP_hero_sleep(Bit8u *hero, signed short quality)
{
	signed short le_regen;
	signed short ae_regen;
	signed short diff;
	signed short tmp;

	if (!hero_dead(hero) &&
		(host_readd(hero + HERO_STAFFSPELL_TIMER) == 0) &&
		(host_readbs(hero + HERO_RECIPE_TIMER) == 0))
	{

		if ((ds_readbs(TRAVEL_BY_SHIP) != 0) && (random_schick(100) < 10)) {
			/* chance of motion sickness is 9% */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(796),
				hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

		} else {

			if (!hero_is_diseased(hero) && !hero_is_poisoned(hero)) {

				if (host_readbs(hero + HERO_RUHE_KOERPER) > 0) {
					dec_ptr_bs(hero + HERO_RUHE_KOERPER);
				}

				if (!hero_busy(hero)) {

					le_regen = random_schick(6) + quality;
					ae_regen = random_schick(6) + quality;

					/* Original-Bug: next operator should be <= */
					if (le_regen < 0) {
						le_regen = 1;
					}

					/* Original-Bug: next operator should be <= */
					if (ae_regen < 0) {
						ae_regen = 1;
					}

					/* swap LE and AE */
					if ((host_readbs(hero + HERO_TYPE) >= 7) && (le_regen < ae_regen)) {
						tmp = ae_regen;
						ae_regen = le_regen;
						le_regen = tmp;
					}

					if (host_readbs(hero + HERO_RUHE_KOERPER) > 0) {
						le_regen += le_regen;
						ae_regen += ae_regen;
					}

					/* Do LE */

					diff = host_readws(hero + HERO_LE_ORIG) - host_readws(hero + HERO_LE);

					if ((diff != 0) && le_regen) {

						if (diff < le_regen) {
							le_regen = diff;
						}

						add_hero_le(hero, le_regen);

						strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_ttx(392));

						if (le_regen > 1) {
							strcat((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_ttx(393));
						}

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(319),
							hero + HERO_NAME2,
							le_regen,
							(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
						if (ds_readbs(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK) {
							GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
							delay_or_keypress(200);
						} else {
							if (ds_readbs(TRAVEL_BY_SHIP) != 0) {
								GUI_output(Real2Host(ds_readd(DTP2)));
							}
						}

					}

					if (host_readbs(hero + HERO_TYPE) >= 7) {

						diff = host_readws(hero + HERO_AE_ORIG) - host_readws(hero + HERO_AE);

						if ((diff != 0) && ae_regen) {

							if (diff < ae_regen) {
								ae_regen = diff;
							}

							add_hero_ae(hero, ae_regen);

							strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_ttx(392));

							if (ae_regen > 1) {
								strcat((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_ttx(393));
							}

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(320),
								hero + HERO_NAME2,
								ae_regen,
								(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

							if (ds_readbs(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK) {
								GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
								delay_or_keypress(200);
							} else {
								if (ds_readbs(TRAVEL_BY_SHIP) != 0) {
									GUI_output(Real2Host(ds_readd(DTP2)));
								}
							}
						}
					}
				} else {
					do_alchemy(hero, host_readbs(hero + HERO_RECIPE_ID), 0);
				}
			} else {
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(558),
					hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
