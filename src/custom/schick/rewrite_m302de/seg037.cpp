/*
        Rewrite of DSA1 v3.02_de functions of seg037 (fight helper)
        Functions rewritten: 1/8
*/

#include "schick.h"
#include "v302de.h"

#include "seg007.h"

namespace M302de {

/**
 * test_foe_range_attack() - checks if range attack is possible
 * @x:	x - coordinate of attacker
 * @y:	y - coordinate of attacker
 * @dir:	looking direction
 * @mode:	0 = common, 1 = attack enemies, 2 = ?
 *
 * The return value is 0 if theres nothing to attack in taht direction
 * or the ID of the attackee.
 */

/*
 * Original-Bug: range attack of foes is possible with direct contacti
*/

signed short test_foe_range_attack(signed short x, signed short y, signed short dir, signed short mode)
{
	signed char cb_val;
	signed short dy = 0, di = 0;	/* run variables in dir */
	signed short can_attack;
	signed short done;

	done = 0;
	can_attack = 0;

	while (done == 0) {

		/* go one field further */
		switch (dir) {
		case 0:	di++; break;	/* RIGHT-BOTTOM */
		case 1: dy--; break;	/* LEFT-BOTTOM */
		case 2: di--; break;	/* LEFT-UP */
		default: dy++;		/* RIGHT-UP */
		}

		/* out of chessboard */
		if (y + dy < 0 || y + dy > 23 || x + di < 0 || x + di > 23) {
			done = 1;
			continue;
		}

		/* get value from current field */
		cb_val = get_cb_val(x + di, y + dy);

		if (mode == 0) {
			/* hero or enemy reacheable from enemies position */
			if ((cb_val > 0 && cb_val < 10 &&
				!hero_dead(get_hero(cb_val - 1)) &&
				!hero_unc(get_hero(cb_val - 1))) ||

				(cb_val >= 10 && cb_val < 30 &&
				(ds_readb(0xd0df + 0x3e * cb_val + 0x31)&1)== 0 &&
				((ds_readb(0xd0df + 0x3e * cb_val + 0x32) >> 1)&1)))

			{
				can_attack = 1;
				done = 1;

				if (cb_val < 10)
					D1_INFO("Attack hero %s\n",
						(char*)get_hero(cb_val - 1) + 0x10);
				else
					D1_INFO("Attack foe\n");

				continue;
			}

			/* empty field */
			if (cb_val == 0)
				continue;

			/* not an enemy */
			if (cb_val < 10 || cb_val >= 30 ||
				(ds_readb(0xd0df + 0x3e * cb_val + 0x31)&1) != 0) {
					if (cb_val >= 0x32 &&
						!is_in_word_array(cb_val + 0xffce, p_datseg + 0x5f46)) {
						D1_INFO("Reached a %d\n", cb_val);
						done = 1;
					}
				}
		} else if (mode == 1) {
			/* attack foe first */
			if (cb_val >= 10 && cb_val < 30 &&
				(ds_readb(0xd0df + 0x3e * cb_val + 0x31) & 1) == 0) {
				can_attack = 1;
				done = 1;
				continue;
			}
			/* skip zeros */
			if (cb_val == 0)
				continue;


			/* Original-Bugfix: the next if assumes
					that a negative cb_val is a hero -> SEGFAULT*/
			if (cb_val < 0) {
				done = 1;
				continue;
			}

			/* handle heroes or walls */
			if (cb_val >= 10 ||
				hero_dead(get_hero(cb_val - 1)) ||
				hero_unc(get_hero(cb_val - 1))) {

				if (cb_val < 50)
					continue;
				if (is_in_word_array(cb_val + 0xffce, p_datseg + 0x5f46))
					continue;
			}
			done = 1;
			continue;
		} else if (mode == 2) {
			/* attack hero */
			if (cb_val > 0 && cb_val < 10 &&
				!hero_dead(get_hero(cb_val - 1)) &&
				!hero_unc(get_hero(cb_val - 1))) {
					can_attack = 1;
					done = 1;
					continue;
			}

			/* skip zeros */
			if (cb_val == 0)
				continue;

			if (cb_val >= 10 ||
				hero_dead(get_hero(cb_val - 1)) ||
				hero_unc(get_hero(cb_val - 1))) {

				if (cb_val >= 50 && is_in_word_array(cb_val + 0xffce, p_datseg + 0x5f46))
					{
						if (cb_val < 10)
							continue;
						if (cb_val >= 30)
							continue;
						if ((ds_readb(0xd0df + 0x3e * cb_val + 0x31)&1) != 0)
							continue;
					}
			}

			done = 1;
			continue;
		}
	}

	if (can_attack != 0)
		return cb_val;
	else
		return 0;
}

}
