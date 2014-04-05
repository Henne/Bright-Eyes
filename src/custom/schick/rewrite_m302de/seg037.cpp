/*
        Rewrite of DSA1 v3.02_de functions of seg037 (fight helper)
        Functions rewritten: 3/8
*/

#include "v302de.h"

#include "seg007.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * copy_ani_stuff() - copies something from ANI.DAT
 * @dst:	destination
 * @nr:		which record to copy
 * @mode:	3 for WEAPANI.DAT, anything else is ANI.DAT
 *
 * Returns the number of copied bytes.
 */

signed short copy_ani_stuff(Bit8u *dst, signed short nr, signed short mode)
{
	Bit8u *buffer, *src;
	signed short retval;
	signed short max_nr;
	signed short i;

	/* ANI.DAT */
	buffer = Real2Host(ds_readd(BUFFER_ANIDAT));

	/* WEAPANI.DAT */
	if (mode == 3)
		buffer = Real2Host(ds_readd(BUFFER_WEAPANIDAT));

	max_nr = host_readw(buffer);

	/* Sanity check of nr */
	if (nr < 0 || nr > max_nr)
		return 0;

	/* set src to the requested data entry */
	src = buffer;
	src += max_nr + 2;

	retval = host_readb(buffer + 2);

	for (i = 1; i <= nr; i++) {
		src += retval;
		retval = host_readb(buffer + i + 2);
	}

	src++;

	retval = retval - 2;

	/* copy some bytes from ANI.DAT */

	for (i = 0; retval > i; i++) {
		host_writeb(dst, host_readb(src));
		src++;
		dst++;
	}

	/* return the number of copied bytes */

	return retval;
}


/**
 * test_foe_melee_attack() - checks if range attack is possible
 * @x:		x - coordinate of attacker
 * @y:		y - coordinate of attacker
 * @dx:		delta to x (looking direction)
 * @dy:		delta to y (looking direction)
 * @mode:	0 = common, 1 = attack enemies only, 2 = attack heroes only
 *
 * The return value is 0 if theres nothing to attack else 1
 */
unsigned short test_foe_melee_attack(signed short x, signed short y,
		signed short dx, signed short dy, signed short mode)
{
	signed char cb_val;

	cb_val = get_cb_val(x + dx, y + dy);

	if (mode == 0) {

		/* is a hero */
		if ((cb_val > 0) && (cb_val < 10))
			/* hero is not dead */
			if (!hero_dead(get_hero(cb_val - 1)))
				/* hero is not unconscious */
				if (!hero_unc(get_hero(cb_val - 1)))
					return 1;

		/* is an enemy */
		if ((cb_val >= 10) && (cb_val < 30))
			/* enemy is alive */
			if ((ds_readb(0xd0df + cb_val * 62 + 0x31) & 1) == 0)
				/* is under "Boeser Blick"-Spell */
				if (((ds_readb(0xd0df + cb_val * 62 + 0x32) >> 1) & 1) == 1)
					return 1;

		return 0;

	} else if (mode == 1) {

		/* is an enemy */
		if ((cb_val >= 10) && (cb_val < 30))
			/* enemy is alive */
			if ((ds_readb(0xd0df + cb_val * 62 + 0x31) & 1) == 0)
					return 1;

		return 0;

	} else if (mode == 2) {

		/* is a hero */
		if ((cb_val > 0) && (cb_val < 10))
			/* hero is not dead */
			if (!hero_dead(get_hero(cb_val - 1)))
				/* hero is not unconscious */
				if (!hero_unc(get_hero(cb_val - 1)))
					return 1;
	}


	return 0;
}

/**
 * test_foe_range_attack() - checks if range attack is possible
 * @x:	x - coordinate of attacker
 * @y:	y - coordinate of attacker
 * @dir:	looking direction
 * @mode:	0 = common, 1 = attack enemies only, 2 = attack heroes only
 *
 * The return value is 0 if theres nothing to attack in taht direction
 * or the ID of the attackee.
 */

/*
 * Original-Bug: range attack of foes is possible with direct contacti
*/

signed short test_foe_range_attack(signed short x, signed short y, const signed short dir, signed short mode)
{
	signed char cb_val;
	signed short dy;	/* run variables in dir */
	signed short di;	/* run variables in dir */
	signed short can_attack;
	signed short done;

	done = 0;
	di = 0;
	dy = 0;
	can_attack = 0;

	while (!done) {

		/* go one field further */
		if (dir == 0) {		/* RIGHT-BOTTOM */
			di++;
		} else if (dir == 1) {	/* LEFT-BOTTOM */
			dy--;
		} else if (dir == 2) {	/* LEFT-UP */
			di--;
		} else {		/* RIGHT-UP */
			dy++;
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
				!(ds_readb(0xd0df + 0x3e * cb_val + 0x31) & 1) &&
				((ds_readb(0xd0df + 0x3e * cb_val + 0x32) >> 1) & 1)))

			{
				can_attack = 1;
				done = 1;

#if !defined(__BORLANDC__)
				if (cb_val < 10)
					D1_LOG("Attack hero %s\n",
						(char*)get_hero(cb_val - 1) + 0x10);
				else
					D1_LOG("Attack foe\n");
#endif

				continue;
			}

			/* if field is not empty */
			if (cb_val != 0) {

				/* an enemy or another object */
				if ((cb_val >= 10 && cb_val < 30 &&
					!(ds_readb(0xd0df + 0x3e * cb_val + 0x31) & 1))

					|| (cb_val >= 0x32 && !is_in_word_array(cb_val + 0xffce, (signed short*)(p_datseg + 0x5f46)))) {
#if !defined(__BORLANDC__)
						D1_LOG("Reached a %d\n", cb_val);
#endif
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
#ifdef M302de_ORIGINAL_BUGFIX
			if (cb_val < 0) {
				done = 1;
				continue;
			}
#endif

			/* handle heroes or walls */
			if (cb_val >= 10 ||
				hero_dead(get_hero(cb_val - 1)) ||
				hero_unc(get_hero(cb_val - 1))) {

				if (cb_val < 50)
					continue;
				if (is_in_word_array(cb_val + 0xffce, (signed short*)(p_datseg + 0x5f46)))
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

#ifdef M302de_ORIGINAL_BUGFIX
			if (cb_val < 0) {
				done = 1;
				continue;
			}
#endif

			if (cb_val >= 10 ||
				hero_dead(get_hero(cb_val - 1)) ||
				hero_unc(get_hero(cb_val - 1))) {

				if (cb_val >= 50 && is_in_word_array(cb_val + 0xffce, (signed short*)(p_datseg + 0x5f46)))
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

	if (can_attack == 0)
		return 0;
	else
		return cb_val;
}

#if !defined(__BORLANDC__)
}
#endif
