/**
 *	Rewrite of DSA1 v3.02_de functions of seg034 (fight system)
 *	Functions rewritten: 3/6
 */

#include "v302de.h"

#include "seg002.h"
#include "seg006.h"
#include "seg007.h"
#include "seg032.h"
#include "seg039.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief	TODO
 * \param x_hero	x-coordinate of the hero
 * \param y_hero	y-coordinate of the hero
 * \param x		x-coordinate of the last position
 * \param y		y-coordinate of the last position
 * \param x_diff	x-coordinate difference {-1, 0, 1}
 * \param y_diff	y-coordinate difference {-1, 0, 1}
 * \param max_range	maximal range for the weapon
 *
 * \return	TODO {0, 1}
 **/
/* Borlandified and identical */
signed short seg034_000(signed short x_hero, signed short y_hero,
			signed short x, signed short y,
			signed short x_diff, signed short y_diff,
			signed short max_range)
{
	signed short fight_id_target;
	signed short fight_id;
	signed short beeline;

	fight_id = get_cb_val(x, y);
	fight_id_target = get_cb_val(x + x_diff, y + y_diff);

	beeline = calc_beeline(x + x_diff, y + y_diff, x_hero, y_hero);

	if ((fight_id != 0) && (calc_beeline(x, y, x_hero, y_hero) < beeline) && (beeline <= max_range)) {

		if ((x_hero == x) && (y_hero == y)) {

			if ((fight_id_target < 50) || ((fight_id_target >= 50) && is_in_word_array(fight_id_target - 50, (signed short*)(p_datseg + 0x5f46))))
			{
				return 1;
			} else {
				return 0;
			}

		} else if (((fight_id >= 50) ||
				((fight_id >= 10) && (fight_id < 30) && test_bit0(p_datseg + 0xd110 + 62 * fight_id)) ||
				((fight_id >= 30) && (fight_id < 50) && test_bit0(p_datseg + 0xcc38 + 62 * fight_id)) ||
				((fight_id < 10) && hero_dead(get_hero(fight_id - 1))))
				&&
				((fight_id_target >= 0) &&
				 ((fight_id_target < 50) || ((fight_id_target >= 50) && is_in_word_array(fight_id_target - 50, (signed short*)(p_datseg + 0x5f46))))))
			{

				if (((((x_diff == 1) || (x_diff == -1)) && (y_hero != y))) ||
					((((y_diff == 1) || (y_diff == -1)) && (x_hero != x))))
				{
					return 0;
				} else {
					return 1;
				}

			} else {
				return 0;
			}
	}

	if (x_diff == 1) {
		if ((fight_id_target >= 0) &&
			((fight_id_target < 50) || ((fight_id_target >= 50) && is_in_word_array(fight_id_target - 50, (signed short*)(p_datseg + 0x5f46))))
			&& ((x < 23) && (y == y_hero) && (calc_beeline(x_hero, y_hero, x + 1, y) <= max_range)))
		{
			return 1;
		} else {
			return 0;
		}

		return 0;
	}

	if (x_diff == -1) {
		if ((fight_id_target >= 0) &&
			((fight_id_target < 50) || ((fight_id_target >= 50) && is_in_word_array(fight_id_target - 50, (signed short*)(p_datseg + 0x5f46))))
			&& ((x > 0) && (y == y_hero) && (calc_beeline(x_hero, y_hero, x - 1, y) <= max_range)))
		{
			return 1;
		} else {
			return 0;
		}

		return 0;

	}

	if (y_diff == 1) {
		if ((fight_id_target >= 0) &&
			((fight_id_target < 50) || ((fight_id_target >= 50) && is_in_word_array(fight_id_target - 50, (signed short*)(p_datseg + 0x5f46))))
			&& ((y < 23) && (x == x_hero) && (calc_beeline(x_hero, y_hero, x, y + 1) <= max_range)))
		{
			return 1;
		} else {
			return 0;
		}

		return 0;

	}

	if (y_diff == -1) {
		if ((fight_id_target >= 0) &&
			((fight_id_target < 50) || ((fight_id_target >= 50) && is_in_word_array(fight_id_target - 50, (signed short*)(p_datseg + 0x5f46))))
			&& ((y > 0) && (x == x_hero) && (calc_beeline(x_hero, y_hero, x, y - 1) <= max_range)))
		{
			return 1;
		} else {
			return 0;
		}
	}

	return 0;
}

/**
 * \brief	TODO
 *
 * \param	px
 * \param	py
 * \param max_range	maximal range for the weapon
 *
 * \return	fight-id of the target
 */
/* Borlandified and identical */
signed char seg034_2e3(Bit8u *px, Bit8u *py, const signed short max_range)
{
	signed short x_diff;
	signed short y_diff;
	signed short x;
	signed short y;
	signed short fight_id;
	signed short x_screen;
	signed short y_screen;
	signed short from_kbd;
	/* TODO: the next two variables are constants */
	signed short l8 = 9;
	signed short l9 = 116;

	ds_writew(0xc3d1, ds_writew(0xc3d3, 0));

	update_mouse_cursor();

	ds_writew(0x29a0, ds_writew(0x299c, x_screen = l8 + 10 * (host_readws(px) + host_readws(py))));

	ds_writew(0x29a2, ds_writew(0x299e, y_screen = l9 + 5 * (host_readws(px) - host_readws(py))));

	mouse_move_cursor(ds_readws(0x299c), ds_readws(0x299e));

	refresh_screen_size();

	x = host_readws(px);
	y = host_readws(py);

	if (ds_readbs(0xe38f) != -1) {
		FIG_remove_from_list(ds_readbs(0xe38f), 0);
		ds_writebs(0xe38f, -1);
	}

	ds_writew(0xe066, 0);
	ds_writeb(0xe068, 0);
	ds_writeb(0xe069, host_readbs(px));
	ds_writeb(0xe06a, host_readbs(py));
	ds_writeb(0xe06b, 0);
	ds_writeb(0xe06c, 4);
	ds_writeb(0xe06d, 11);
	ds_writeb(0xe06e, 22);
	ds_writeb(0xe06f, 0);
	ds_writeb(0xe070, 0);
	ds_writeb(0xe071, 21);
	ds_writeb(0xe072, 10);
	ds_writeb(0xe07b, 0);
	ds_writeb(0xe073, 0);
	ds_writeb(0xe075, -1);
	ds_writeb(0xe074, -1);
	ds_writed(0xe07d, ds_readd(0xd85e));
	ds_writeb(0xe077, 1);
	ds_writeb(0xe078, 1);
	ds_writeb(0xe079, -1);

	ds_writeb(0xe38f, FIG_add_to_list(-1));

	draw_fight_screen_pal(0);

	do {
		handle_input();

		if ((ds_readws(ACTION) == 1) || (ds_readws(0xc3d3) != 0)) {

			/* cancel */

			ds_writew(0xc3d3, 0);

			FIG_remove_from_list(ds_readbs(0xe38f), 0);

			ds_writeb(0xe38f, -1);

			draw_fight_screen_pal(0);

			return -99;
		}

		from_kbd = 0;

		if ((ds_readws(ACTION) == 72) ||
			(ds_readws(ACTION) == 80) ||
			(ds_readws(ACTION) == 77) ||
			(ds_readws(ACTION) == 75))
		{
			from_kbd = 1;
		} else {

			x_diff = ds_readws(0x299c) - x_screen;
			y_diff = ds_readws(0x299e) - y_screen;

			if (((y_diff > 0) && (x_diff <= -10)) ||
				((x_diff < 0) && (y_diff >= 5)))
			{
				ds_writew(ACTION, 80);

			} else if (((y_diff < 0) && (x_diff >= 10)) ||
					((x_diff > 0) && (y_diff <= -5)))
			{
				ds_writew(ACTION, 72);

			} else if (((y_diff > 0) && (x_diff >= 10)) ||
					((x_diff > 0) && (y_diff >= 5)))
			{
				ds_writew(ACTION, 77);

			} else if (((y_diff < 0) && (x_diff <= -10)) ||
					((x_diff < 0) && (y_diff <= -5)))
			{
				ds_writew(ACTION, 75);
			}
		}

		if (ds_readws(0xc3d1) != 0) {
			ds_writew(ACTION, 28);
			ds_writew(0xc3d1, 0);
		}

		if (ds_readws(ACTION) == 77) {

			if (seg034_000(x, y, host_readws(px), host_readws(py), 1, 0, max_range)) {
				inc_ptr_ws(px);
			}
		} else if (ds_readws(ACTION) == 75) {
			if (seg034_000(x, y, host_readws(px), host_readws(py), -1, 0, max_range)) {
				dec_ptr_ws(px);
			}
		} else if (ds_readws(ACTION) == 72) {
			if (seg034_000(x, y, host_readws(px), host_readws(py), 0, 1, max_range)) {
				inc_ptr_ws(py);
			}
		} else if (ds_readws(ACTION) == 80) {
			if (seg034_000(x, y, host_readws(px), host_readws(py), 0, -1, max_range)) {
				dec_ptr_ws(py);
			}
		}

		if ((ds_readbs(0xe069) != host_readws(px)) || (ds_readbs(0xe06a) != host_readws(py))) {

			update_mouse_cursor();

			x_screen = l8 + 10 * (host_readws(px) + host_readws(py));
			y_screen = l9 + 5 * (host_readws(px) - host_readws(py));

			if (from_kbd != 0) {
				ds_writew(0x29a0, ds_writew(0x299c, x_screen));
				ds_writew(0x29a2, ds_writew(0x299e, y_screen));

				mouse_move_cursor(ds_readws(0x299c), ds_readws(0x299e));
			}

			refresh_screen_size();
			FIG_call_draw_pic();
			fight_id = get_cb_val(host_readws(px), host_readws(py));

			if ((fight_id > 0) && (fight_id < 50)) {

				if (fight_id < 10) {
					FIG_draw_char_pic(1, fight_id);
				} else {

					if (fight_id >= 30) {
						FIG_draw_enemy_pic(1, fight_id - 20);
					} else {
						FIG_draw_enemy_pic(1, fight_id);
					}
				}
			}

			FIG_remove_from_list(ds_readbs(0xe38f), 1);

			ds_writeb(0xe069, host_readws(px));
			ds_writeb(0xe06a, host_readws(py));

			FIG_add_to_list(ds_readbs(0xe38f));
			FIG_draw_figures();
			FIG_set_gfx();
		}

	} while (ds_readws(ACTION) != 28);

	FIG_remove_from_list(ds_readbs(0xe38f), 0);

	ds_writeb(0xe38f, -1);

	draw_fight_screen_pal(0);

	return get_cb_val(host_readws(px), host_readws(py));
}

struct coord {
	signed short x;
	signed short y;
};

struct dummy {
	struct coord a[4];
};

/* Borlandified and identical */
void seg034_718(signed short x, signed short y, Bit8u *px, Bit8u *py, signed short dir, signed short mode)
{
	signed short new_dir;
	signed short dist;
	signed short new_x;
	signed short new_y;
	signed char done;

#if !defined(__BORLANDC__)
        struct dummy a;
        a.a[0].x = 1;
        a.a[0].y = 0;
        a.a[1].x = 0;
        a.a[1].y = -1;
        a.a[2].x = -1;
        a.a[2].y = 0;
        a.a[3].x = 0;
        a.a[3].y = 1;
#else
        struct dummy a = *(struct dummy*)(p_datseg + 0x5f7c);
#endif

	done = 0;

	host_writew(px, x);
	host_writew(py, y);

	if (get_cb_val(x, y) == 0) {

		if (mode == 0) {
			return;
		}

		if (get_cb_val(x - a.a[dir].x, y - a.a[dir].y) == 0) {
			return;
		}
	}

	dist = 1;

	while (!done) {

		for (new_dir = 0; new_dir < 4; new_dir++) {

			new_x = host_readws(px) + a.a[new_dir].x * dist;
			new_y = host_readws(py) + a.a[new_dir].y * dist;

			if ((new_x >= 0) && (new_x < 24) && (new_y >= 0) && (new_y < 24) &&
				!get_cb_val(new_x, new_y))
			{

				if ((mode == 0) ||
					(!get_cb_val(new_x - a.a[dir].x, new_y - a.a[dir].y)))
				{

					done = 1;
					host_writew(px, new_x);
					host_writew(py, new_y);
					break;
				}
			}
		}

		dist++;
	}
}

#if !defined(__BORLANDC__)
}
#endif
