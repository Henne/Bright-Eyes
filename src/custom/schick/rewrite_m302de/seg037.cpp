/*
 *	Rewrite of DSA1 v3.02_de functions of seg037 (fight helper)
 *	Functions rewritten: 4/8
 *
*/

#include <string.h>

#include "v302de.h"

#include "seg005.h"
#include "seg006.h"
#include "seg007.h"
#include "seg038.h"

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

/* Borlandified an identical */
signed short copy_ani_stuff(Bit8u *dst, signed short nr, signed short mode)
{
	Bit8u *buffer, *src;
	signed char retval;
	signed short i;
	signed short max_nr;

	/* ANI.DAT */
	buffer = Real2Host(ds_readd(BUFFER_ANIDAT));

	/* WEAPANI.DAT */
	if (mode == 3)
		buffer = Real2Host(ds_readd(BUFFER_WEAPANIDAT));

	max_nr = host_readw(buffer);

	/* Sanity check of nr */
	if (nr < 0)
		return 0;

	if (nr > max_nr)
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

/* Borlandified and identical */
void seg037_00ae(Bit8u *enemy, signed short a2)
{
	signed char b1;
	signed char b2;
	signed char b3;
	Bit8u *p1;
	Bit8u *p2;
	Bit8u *p3;

	signed short i;

	ds_writeb(0xd9c1, 0);
	ds_writeb(0xdab3, host_readbs(enemy+1));
	p1 = p_datseg + 0xd9c2;

	i = 0;
	p3 = Real2Host(ds_readd(0x2555 + host_readbs(enemy + 1) * 4));

	while (ds_readbs(0xd823 + i) != -1) {

		if (host_readbs(enemy + 0x27) != ds_readbs(0xd823 + i)) {

			b2 = b1 = -1;
			b3 = host_readbs(enemy + 0x27);
			b2 = b3;

			b3++;

			if (b3 == 4) {
				b3 = 0;
			}

			if (ds_readbs(0xd823 + i) != b3) {

				b1 = b3;
				b3++;

				if (b3 == 4) {
					b3 = 0;
				}

				if (ds_readbs(0xd823 + i) != b3) {
					b2 = host_readbs(enemy + 0x27) + 4;
					b1 = -1;
				}

			}

			host_writeb(enemy + 0x27, ds_readbs(0xd823 + i));

			p1 += copy_ani_stuff(p1, host_readws(p3 + b2 * 2), 1);

			if (b1 != -1) {

				p1 += copy_ani_stuff(p1, host_readws(p3 + b1 * 2), 1);
			}
		}

		if (ds_readbs(0xd823 + i) == ds_readbs(0xd824 + i)) {

			p1 += copy_ani_stuff(p1, host_readws(p3 + (ds_readbs(0xd823 + i) + 0x0c) * 2), 1);
			i += 2;
			host_writeb(enemy + 0x23, host_readbs(enemy + 0x23) - 2);

		} else {
			p1 += copy_ani_stuff(p1, host_readws(p3 + (ds_readbs(0xd823 + i) + 0x08) * 2), 1);
			i++;
			dec_ptr_bs(enemy + 0x23);
		}
	}

	/* terminate array */
	host_writeb(p1, -1);

	FIG_call_draw_pic();

	FIG_remove_from_list(ds_readbs(0xe38e), 0);

	ds_writeb(0xe38e, -1);

	FIG_set_0e(host_readbs(enemy + 0x26), 1);

	if (is_in_byte_array(host_readbs(enemy + 1), p_datseg + 0x25f9)) {

		memcpy(p_datseg + 0xdba7, p_datseg + 0xd9c1, 0xf3);

		p2 = Real2Host(FIG_get_ptr(host_readbs(enemy + 0x26)));

		FIG_set_0e(ds_readbs(0xe35a + host_readbs(p2 + 0x13)), 3);
	}

	/* draw_fight_screen */
	draw_fight_screen(0);

	memset(p_datseg + 0xd9c1, -1, 0xf3);
	memset(p_datseg + 0xdba7, -1, 0xf3);

	FIG_init_list_elem(a2 + 10);
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

		if ( ((cb_val > 0) && (cb_val < 10) && (!hero_dead(get_hero(cb_val - 1))) && (!hero_unc(get_hero(cb_val - 1)))) ||

			((cb_val >= 10) && (cb_val < 30)) && (!enemy_dead(p_datseg + 0xd0df + cb_val * 62))  &&
				(!enemy_bb(p_datseg + 0xd0df + cb_val * 62)))
		{
			return 1;
		} else {
			return 0;
		}

	} else if (mode == 1) {

		/* is a living enemy */
		if ((cb_val >= 10) && (cb_val < 30) && (!enemy_dead(p_datseg + 0xd0df + cb_val * 62)))
		{
			return 1;
		} else {
			return 0;
		}

	} else if (mode == 2) {

		/* is a living, conscious hero */
		if ((cb_val > 0) && (cb_val < 10) && (!hero_dead(get_hero(cb_val - 1))) && (!hero_unc(get_hero(cb_val - 1))))
		{
			return 1;
		} else {
			return 0;
		}
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
 * The return value is 0 if theres nothing to attack in that direction
 * or the ID of the attackee.
 */

/*
 * Original-Bug: range attack of foes is possible with direct contact
*/

signed short test_foe_range_attack(signed short x, signed short y, const signed short dir, signed short mode)
{
	signed char cb_val;
	signed short dy;	/* run variables in dir */
	signed short can_attack;
	register signed short done;
	register signed short di;	/* run variables in dir */

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
		} else {

			/* get value from current field */
			cb_val = get_cb_val(x + di, y + dy);

			if (mode == 0) {
				/* hero or enemy reacheable from enemies position */
				if (((cb_val > 0) && (cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) && !hero_unc(get_hero(cb_val - 1))) ||
					((cb_val >= 10) && (cb_val < 30) && !(ds_readb(0xd0df + 0x3e * cb_val + 0x31) & 1) &&
					(enemy_bb(p_datseg + 0xd0df + 0x3e * cb_val))))

				{
					can_attack = 1;
					done = 1;

				} else

				/* if field is not empty */
				if (cb_val != 0) {

					/* an enemy or another object */
					if ((cb_val >= 10 && cb_val < 30 && !(ds_readb(0xd0df + 0x3e * cb_val + 0x31) & 1))

						|| (cb_val >= 0x32 && !is_in_word_array(cb_val + 0xffce, (signed short*)(p_datseg + 0x5f46))))
					{
							done = 1;
					}
				}

			} else if (mode == 1) {
				/* attack foe first */
				if ((cb_val >= 10) && (cb_val < 30) && !(ds_readb(0xd0df + 0x3e * cb_val + 0x31) & 1))
				{
					can_attack = 1;
					done = 1;
				} else
				/* skip zeros */
				if (cb_val != 0) {

#ifdef M302de_ORIGINAL_BUGFIX
					/* Original-Bugfix: the next if assumes
						that a negative cb_val is a hero -> SEGFAULT*/
					if (cb_val < 0) {
						done = 1;
					} else
#endif

					/* handle heroes or walls */
					if (((cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) && !hero_unc(get_hero(cb_val - 1))) ||
						((cb_val >= 50) && !is_in_word_array(cb_val + 0xffce, (signed short*)(p_datseg + 0x5f46))))
					{
						done = 1;
					}
				}

			} else if (mode == 2) {
				/* attack hero */
				if ((cb_val > 0) && (cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) && !hero_unc(get_hero(cb_val - 1)))
				{
					can_attack = 1;
					done = 1;
				} else

				/* skip zeros */
				if (cb_val != 0) {

#ifdef M302de_ORIGINAL_BUGFIX
					if (cb_val < 0) {
						done = 1;
					} else
#endif

					if (((cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) && !hero_unc(get_hero(cb_val - 1))) ||
						((cb_val >= 50) && !is_in_word_array(cb_val + 0xffce, (signed short*)(p_datseg + 0x5f46))) ||
						((cb_val >= 10) && (cb_val < 30) && !(ds_readb(0xd0df + 0x3e * cb_val + 0x31)&1)))
					{
						done = 1;
					}
				}

			}
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
