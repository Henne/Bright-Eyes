/**
 *	Rewrite of DSA1 v3.02_de functions of seg034 (fight system)
 *	Functions rewritten: 1/6
 */

#include "v302de.h"

#include "seg007.h"
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

#if !defined(__BORLANDC__)
}
#endif
