/**
 *	Rewrite of DSA1 v3.02_de functions of seg049 (group management)
 *	Functions rewritten: 8/9
 */

#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg028.h"
#include "seg029.h"
#include "seg047.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
int GRP_compare_heros(const void *p1, const void *p2)
{
	Bit8u *hero1, *hero2;

	hero1 = (Bit8u*)p1;
	hero2 = (Bit8u*)p2;

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) == ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) == ds_readbs(CURRENT_GROUP)))
	{
		if (host_readbs(hero1 + 0x8a) < host_readbs(hero2 + 0x8a))
		{
			return -1;
		} else {
			return 1;
		}
	}

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) == ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) != ds_readbs(CURRENT_GROUP)))
	{
		return -1;
	}

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) != ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) == ds_readbs(CURRENT_GROUP)))
	{
		return 1;
	}

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) != ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) != ds_readbs(CURRENT_GROUP)))
	{
		if (host_readbs(hero1 + 0x8a) < host_readbs(hero2 + 0x8a))
		{
			return -1;
		} else {
			return 1;
		}
	}

	if (!(host_readbs(hero1 + 0x21)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) == ds_readbs(CURRENT_GROUP)))
	{
		return 1;
	}

	if (!(host_readbs(hero1 + 0x21)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) != ds_readbs(CURRENT_GROUP)))
	{
		return -1;
	}

	if (!(host_readbs(hero1 + 0x21)) &&
		!(host_readbs(hero2 + 0x21)))
	{
		return 0;
	}

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) == ds_readbs(CURRENT_GROUP)) &&
		!(host_readbs(hero2 + 0x21)))
	{
		return -1;
	}

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) != ds_readbs(CURRENT_GROUP)) &&
		!(host_readbs(hero2 + 0x21)))
	{
		return 1;
	}

	return 0;
}

/* Borlandified and identical */
void GRP_sort_heros(void)
{
	signed short i;

	qsort((void*)get_hero(0), 6, 0x6da, GRP_compare_heros);

	for (i = 0; i < 6; i++) {
		host_writeb(get_hero(i) + 0x8a, i + 1);
	}
}

/* Borlandified and identical */
void GRP_save_pos(signed short group)
{
	signed short refresh = 0;

	if (group & 0x8000) {
		refresh = 1;
	}

	group &= 0x7fff;

	GRP_sort_heros();

	ds_writeb(0x2d3e + group, ds_readbs(DIRECTION));

	ds_writew(0x2d48 + group * 2, ds_readws(X_TARGET));
	ds_writew(0x2d54 + group * 2, ds_readws(Y_TARGET));

	ds_writeb(0x2d61 + group, ds_readbs(LOCATION));
	ds_writeb(0x2d68 + group, ds_readbs(CURRENT_TOWN));
	ds_writeb(0x2d6f + group, ds_readbs(DUNGEON_INDEX));
	ds_writeb(0x2d76 + group, ds_readbs(DUNGEON_LEVEL));
	ds_writeb(0x2d7d + group, ds_readbs(0x2d7c));

	ds_writew(0x2d87 + group * 2, ds_readws(0x2d83));
	ds_writew(0x2d93 + group * 2, ds_readws(0x2d85));

	ds_writeb(0x2da0 + group, ds_readbs(0x2d9f));
	ds_writeb(0x2da7 + group, ds_readbs(0x2da6));
	ds_writeb(0x2dae + group, ds_readbs(0x2dad));
	ds_writeb(0x2db5 + group, ds_readbs(0x2db4));

	if (!refresh) {
		draw_status_line();
	}
}

/* Borlandified and identical */
void GRP_split(void)
{
	signed short new_group;
	signed short not_empty;
	signed short answer;

	if (count_heroes_available_in_group() <= (host_readbs(get_hero(6) + 0x21) != 0 ? 2 : 1)) {

		GUI_output(get_ltx(0x808));
	} else {

		not_empty = 0;
		new_group = 0;

		while (ds_readbs(0x2d36 + new_group) != 0) {
			new_group++;
		}

		do {
			ds_writeb(0x64a2, 6);
			answer = select_hero_from_group(get_ltx(0x80c));

			if (answer == -1) {
				break;
			} else {

				not_empty = 1;
				host_writeb(get_hero(answer) + 0x87, (signed char)new_group);
				inc_ds_bs(0x2d36 + new_group);
				dec_ds_bs(0x2d36 + ds_readbs(CURRENT_GROUP));
			}

		} while	(count_heroes_available_in_group() > (host_readbs(get_hero(6) + 0x21) != 0 ? 2 : 1));

		if (not_empty) {
			GRP_save_pos(new_group);
		}
	}
}

/* Borlandified and identical */
void GRP_merge(void)
{
	signed short answer;
	signed short i;

	answer = can_merge_group();

	if (answer == -1) {

		GUI_output(get_ltx(0x810));
	} else {

		do {

			ds_writeb(0x2d3e + answer, (signed char)
				(ds_writew(0x2d48 + answer * 2,
				ds_writew(0x2d54 + answer * 2,
				ds_writebs(0x2d68 + answer,
				ds_writeb(0x2d6f + answer,
				ds_writeb(0x2d76 + answer,
				ds_writeb(0x2d7d + answer, (signed char)
				(ds_writew(0x2d87 + answer * 2,
				ds_writew(0x2d93 + answer * 2,
				ds_writebs(0x2da0 + answer,
				ds_writeb(0x2da7 + answer,
				ds_writeb(0x2dae + answer,
				ds_writeb(0x2db5 + answer, 0)))))))))))))));

			ds_writeb(0x2d36 + answer, 0);

			for (i = 0; i <= 6; i++) {

				if ((host_readbs(get_hero(i) + 0x21) != 0) &&
					host_readbs(get_hero(i) + 0x87) == answer)
				{
					host_writeb(get_hero(i) + 0x87, ds_readbs(CURRENT_GROUP));
					inc_ds_bs(0x2d36 + ds_readbs(CURRENT_GROUP));
				}
			}
			GRP_sort_heros();
			answer = can_merge_group();
		} while (answer != -1);

		draw_status_line();
	}
}

/* Borlandified and identical */
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

		if (ds_readbs(0x2d36 + group) != 0) {

			state = 0;

			for (i = 0; i < 6; i++) {

				if ((host_readbs(get_hero(i) + 0x21) != 0) &&
					(host_readbs(get_hero(i) + 0x87) == group) &&
					check_hero(get_hero(i)))
				{
					if (host_readbs(get_hero(i) + 0x9f) != 0) {
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
						GUI_output(get_ltx(0x90));
					} else {
						if ((state == 2) && GUI_bool(get_ltx(0xc14))) {
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

		if ( ( (ds_readbs(CURRENT_TOWN) != 0) && !ds_readbs(0x2d68 + group)) ||
			(!ds_readbs(CURRENT_TOWN) && (ds_readbs(0x2d68 + group) != 0)))
		{
			set_palette(p_datseg + 0x26c3, 0x00, 0x20);
			set_palette(p_datseg + 0x26c3, 0x80, 0x20);
			set_palette(p_datseg + 0x26c3, 0xa0, 0x20);
		}

		if ((ds_readbs(0x2d6f + group)) && (ds_readbs(0x2d6f + group) != ds_readbs(DUNGEON_INDEX)))
		{
			ds_writeb(0x2ca6, -1);
			ds_writew(0x2ccb, -1);
		}

		if ((ds_readbs(0x2d68 + group)) && (ds_readbs(0x2d68 + group) != ds_readbs(CURRENT_TOWN)))
		{
			ds_writeb(0x2ca7, -1);
			ds_writew(0x2ccb, -1);
		}

		/* save positions from the old group */
		ds_writeb(0x2d3e + ds_readbs(CURRENT_GROUP), ds_readbs(DIRECTION));
		ds_writew(0x2d48 + ds_readbs(CURRENT_GROUP) * 2, ds_readw(X_TARGET));
		ds_writew(0x2d54 + ds_readbs(CURRENT_GROUP) * 2, ds_readw(Y_TARGET));
		ds_writeb(0x2d61 + ds_readbs(CURRENT_GROUP), ds_readbs(LOCATION));
		ds_writeb(0x2d68 + ds_readbs(CURRENT_GROUP), ds_readbs(CURRENT_TOWN));
		ds_writeb(0x2d6f + ds_readbs(CURRENT_GROUP), ds_readbs(DUNGEON_INDEX));
		ds_writeb(0x2d76 + ds_readbs(CURRENT_GROUP), ds_readbs(DUNGEON_LEVEL));
		ds_writeb(0x2d7d + ds_readbs(CURRENT_GROUP), ds_readbs(0x2d7c));
		ds_writew(0x2d87 + ds_readbs(CURRENT_GROUP) * 2, ds_readw(0x2d83));
		ds_writew(0x2d93 + ds_readbs(CURRENT_GROUP) * 2, ds_readw(0x2d85));
		ds_writeb(0x2da0 + ds_readbs(CURRENT_GROUP), ds_readbs(0x2d9f));
		ds_writeb(0x2da7 + ds_readbs(CURRENT_GROUP), ds_readbs(0x2da6));
		ds_writeb(0x2dae + ds_readbs(CURRENT_GROUP), ds_readbs(0x2dad));
		ds_writeb(0x2db5 + ds_readbs(CURRENT_GROUP), ds_readbs(0x2db4));

		/* set positions for the new group */
		ds_writeb(CURRENT_GROUP, (signed char)group);
		ds_writeb(DIRECTION, ds_readb(0x2d3e + group));
		ds_writew(X_TARGET, ds_readw(0x2d48 + group * 2));
		ds_writew(Y_TARGET, ds_readw(0x2d54 + group * 2));
		ds_writeb(LOCATION, ds_readb(0x2d61 + group));
		ds_writeb(CURRENT_TOWN, ds_readb(0x2d68 + group));
		ds_writeb(DUNGEON_INDEX, ds_readb(0x2d6f + group));
		dng_level = ds_readbs(DUNGEON_LEVEL);
		ds_writeb(DUNGEON_LEVEL, ds_readbs(0x2d76 + group));

		if (dng_level != ds_readbs(DUNGEON_LEVEL)) {
			load_area_description(1);
		}

		ds_writeb(0x2d7c, ds_readb(0x2d7d + group));
		ds_writew(0x2d83, ds_readw(0x2d87 + group * 2));
		ds_writew(0x2d85, ds_readw(0x2d93 + group * 2));
		ds_writeb(0x2d9f, ds_readb(0x2da0 + group));
		ds_writeb(0x2da6, ds_readb(0x2da7 + group));
		ds_writeb(0x2dad, ds_readb(0x2dae + group));
		ds_writeb(0x2db4, ds_readb(0x2db5 + group));

		GRP_sort_heros();

		for (group = 0; group <= 6; group++) {
			ds_writeb(0x4219 + group, ds_writeb(0x4212 + group, 0));
		}

		ds_writew(0x2846, 1);
	}
}

struct dummy {
	char a[0x6da];
};

/* Borlandified and identical */
void GRP_swap_heros(void)
{
	signed short hero1_nr;
	signed short hero2_nr;
	signed char l2;
	signed char l3;
	signed char l4;
	signed char l5;
	signed char i;
	signed char tmp[0x6da];

	if ((ds_readw(0x29b4) == 0) || !ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP))) {
		return;
	}

	hero1_nr = select_hero_from_group(get_ltx(0x4b8));

	if ((hero1_nr != -1) && (hero1_nr < 6)) {

		hero2_nr = select_hero_from_group(get_ltx(0x4bc));

		if ((hero2_nr != -1) && (hero1_nr != hero2_nr) && (hero2_nr < 6)) {

			for (i = 0; i < 3; i++) {

				if (ds_readbs(0xe3be + i) == hero1_nr) {
					ds_writebs(0xe3be + i, (signed char)hero2_nr);
				}
			}

			/* save hero1 in tmp */
			*((struct dummy*)&tmp) = *((struct dummy*)get_hero(hero1_nr));

			l2 = ds_readbs(0xe3d6 + hero1_nr);
			l3 = ds_readbs(0xe3cf + hero1_nr);
			l4 = ds_readbs(0xe3c8 + hero1_nr);
			l5 = ds_readbs(0xe3c1 + hero1_nr);

			*((struct dummy*)get_hero(hero1_nr)) = *((struct dummy*)get_hero(hero2_nr));

			ds_writebs(0xe3d6 + hero1_nr, ds_readbs(0xe3d6 + hero2_nr));
			ds_writebs(0xe3cf + hero1_nr, ds_readbs(0xe3cf + hero2_nr));
			ds_writebs(0xe3c8 + hero1_nr, ds_readbs(0xe3c8 + hero2_nr));
			ds_writebs(0xe3c1 + hero1_nr, ds_readbs(0xe3c1 + hero2_nr));

			*((struct dummy*)get_hero(hero2_nr)) = *((struct dummy*)&tmp);

			ds_writebs(0xe3d6 + hero2_nr, l2);
			ds_writebs(0xe3cf + hero2_nr, l3);
			ds_writebs(0xe3c8 + hero2_nr, l4);
			ds_writebs(0xe3c1 + hero2_nr, l5);

			if (host_readbs(get_hero(hero1_nr) + 0x21)) {
				host_writebs(get_hero(hero1_nr) + 0x84, 100);
			}

			if (host_readbs(get_hero(hero2_nr) + 0x21)) {
				host_writebs(get_hero(hero2_nr) + 0x84, 100);
			}

			host_writeb(get_hero(hero1_nr) + 0x8a, hero1_nr + 1);
			host_writeb(get_hero(hero2_nr) + 0x8a, hero2_nr + 1);
		}
	}

	draw_status_line();
}

/* Borlandified and identical */
void GRP_move_hero(signed short src_pos)
{

	signed short dst_pos;
	Bit8u *src;
	Bit8u *dst;
	signed char bak1;
	signed char bak2;
	signed char bak3;
	signed char bak4;
	signed char i;

	/* dont move NPC */
	if (src_pos != 6) {

		ds_writew(0x2988, 157);
		ds_writew(0x298c, 157);
		ds_writew(0x298a, 1);
		ds_writew(0x298e, 260);

		update_mouse_cursor();

		ds_writew(0x2998, 1);
		ds_writew(0x299e, 157);

		ds_writew(0x299c, ds_readw(0x2d01 + 2 * src_pos));
		ds_writew(0xc011, ds_readw(0x299c));
		ds_writew(0xc013, ds_readw(0x299e));
		ds_writew(0xc015, ds_readw(0x299c) + 31);
		ds_writew(0xc017, ds_readw(0x299e) + 31);
		ds_writed(0xc019, ds_readd(0xd2f7));

		do_save_rect();

		ds_writew(0x2998, 0);

		refresh_screen_size();

		ds_writew(0x2998, 1);

		ds_writew(0xc011, ds_readw(0x2d01 + 2 * src_pos));
		ds_writew(0xc013, 157);
		ds_writew(0xc015, ds_readw(0x2d01 + 2 * src_pos) + 31);
		ds_writew(0xc017, 188);
		ds_writed(0xc019, ds_readd(0xd303));

		do_save_rect();

		ds_writew(0x2998, 0);

		update_mouse_cursor();

		ds_writew(0x29a0, ds_readw(0x299c));
		ds_writew(0x29a2, ds_readw(0x299e));

		while (ds_readw(0xc3d5) == 0) {

			if (ds_readw(0x29a4) != 0) {

				ds_writew(0x2998, 1);

				wait_for_vsync();

				ds_writew(0xc011, ds_readw(0x29a0));
				ds_writew(0xc013, ds_readw(0x29a2));
				ds_writew(0xc015, ds_readw(0x29a0) + 31);
				ds_writew(0xc017, ds_readw(0x29a2) + 31);
				ds_writed(0xc019, ds_readd(0xd2f7));

				do_pic_copy(0);

				ds_writew(0xc011, ds_readw(0x299c));
				ds_writew(0xc013, ds_readw(0x299e));
				ds_writew(0xc015, ds_readw(0x299c) + 31);
				ds_writew(0xc017, ds_readw(0x299e) + 31);
				ds_writed(0xc019, ds_readd(0xd2f7));

				do_save_rect();

				ds_writed(0xc019, ds_readd(0xd303));

				do_pic_copy(0);

				ds_writew(0x29a0, ds_readw(0x299c));
				ds_writew(0x29a2, ds_readw(0x299e));
				ds_writew(0x29a4, 0);
				ds_writew(0x2998, 0);
			}
		}

		ds_writew(0xc011, ds_readw(0x29a0));
		ds_writew(0xc013, ds_readw(0x29a2));
		ds_writew(0xc015, ds_readw(0x29a0) + 31);
		ds_writew(0xc017, ds_readw(0x29a2) + 31);
		ds_writed(0xc019, ds_readd(0xd2f7));

		do_pic_copy(0);

		dst_pos = 6;
		while (ds_readws(0x2d01 + --dst_pos * 2) > ds_readws(0x299c))
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
					if (ds_readbs(0xe3be + i) == src_pos) {
						ds_writeb(0xe3be + i, dst_pos);
					}
				}

				memcpy(Real2Host(ds_readd(0xd303)), src, 0x6da);

				bak1 = ds_readbs(0xe3d6 + src_pos);
				bak2 = ds_readbs(0xe3cf + src_pos);
				bak3 = ds_readbs(0xe3c8 + src_pos);
				bak4 = ds_readbs(0xe3c1 + src_pos);

				*((struct dummy*)src) = *((struct dummy*)dst);

				ds_writeb(0xe3d6 + src_pos, ds_readbs(0xe3d6 + dst_pos));
				ds_writeb(0xe3cf + src_pos, ds_readbs(0xe3cf + dst_pos));
				ds_writeb(0xe3c8 + src_pos, ds_readbs(0xe3c8 + dst_pos));
				ds_writeb(0xe3c1 + src_pos, ds_readbs(0xe3c1 + dst_pos));

				memcpy(dst, Real2Host(ds_readd(0xd303)), 0x6da);

				ds_writeb(0xe3d6 + dst_pos, bak1);
				ds_writeb(0xe3cf + dst_pos, bak2);
				ds_writeb(0xe3c8 + dst_pos, bak3);
				ds_writeb(0xe3c1 + dst_pos, bak4);

				host_writeb(src + 0x84, 100);
				host_writeb(dst + 0x84, 100);
				host_writeb(dst + 0x8a, src_pos + 1);
				host_writeb(src + 0x8a, dst_pos + 1);
			}
		}

		ds_writew(0xc3d5, 0);
		ds_writew(0x2988, 0);
		ds_writew(0x298c, 199);
		ds_writew(0x298a, 0);
		ds_writew(0x298e, 319);

		refresh_screen_size();
		draw_status_line();
	}
}

#if !defined(__BORLANDC__)
}
#endif
