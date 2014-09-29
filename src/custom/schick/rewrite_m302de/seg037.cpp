/*
 *	Rewrite of DSA1 v3.02_de functions of seg037 (fight: enemy attack)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg037.cpp
 *
*/

#include <string.h>

#include "v302de.h"

#include "seg005.h"
#include "seg006.h"
#include "seg007.h"
#include "seg032.h"
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

void seg037_00ae(Bit8u *enemy, signed short enemy_nr)
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
			/* BP - 2 */
			host_writeb(enemy + 0x23, host_readbs(enemy + 0x23) - 2);

		} else {
			p1 += copy_ani_stuff(p1, host_readws(p3 + (ds_readbs(0xd823 + i) + 0x08) * 2), 1);
			i++;
			/* BP - 1 */
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

	FIG_init_list_elem(enemy_nr + 10);
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

		if ( ((cb_val > 0) && (cb_val < 10) &&
			(!hero_dead(get_hero(cb_val - 1))) &&
			(!hero_unc(get_hero(cb_val - 1)))
			) || (
			(cb_val >= 10) && (cb_val < 30)) &&
				(!enemy_dead(p_datseg + 0xd0df + cb_val * 62))  &&
				(enemy_bb(p_datseg + 0xd0df + cb_val * 62))
			)
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
				if ( (
					(cb_val > 0) && (cb_val < 10) &&
						!hero_dead(get_hero(cb_val - 1)) &&
						!hero_unc(get_hero(cb_val - 1))
					) || (
					(cb_val >= 10) && (cb_val < 30) &&
						!enemy_dead(p_datseg + 0xd0df + 0x3e * cb_val) &&
						enemy_bb(p_datseg + 0xd0df + 0x3e * cb_val))
				)
				{
					can_attack = 1;
					done = 1;

				} else

				/* if field is not empty */
				if (cb_val != 0) {

					/* an enemy or another object */
					if ( (
						(cb_val >= 10) && (cb_val < 30) &&
							!enemy_dead(p_datseg + 0xd0df + 0x3e * cb_val)
						) || (
						(cb_val >= 0x32) &&
							!is_in_word_array(cb_val + 0xffce, (signed short*)(p_datseg + 0x5f46)))

					)
					{
							done = 1;
					}
				}

			} else if (mode == 1) {
				/* attack foe first */
				if ((cb_val >= 10) && (cb_val < 30) && !enemy_dead(p_datseg + 0xd0df + 0x3e * cb_val))
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

					if ( (
						(cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) &&
						!hero_unc(get_hero(cb_val - 1))
						) || (
						(cb_val >= 50) &&
							!is_in_word_array(cb_val + 0xffce, (signed short*)(p_datseg + 0x5f46))
						) || (
						(cb_val >= 10) && (cb_val < 30) &&
							!enemy_dead(p_datseg + 0xd0df + 0x3e * cb_val))
					)
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


signed short get_foe_attack_mode(signed short a1, signed short a2)
{
	signed short retval = 0;
	Bit8u *ptr = p_datseg + 0xf13 + a1 * 8;

	if (a2 == 0) {

		if ((host_readbs(ptr + 1) == 3) || (host_readbs(ptr + 1) == 2)) {
			retval = 2;
		} else {
			if (host_readbs(ptr + 1) == 1) {
				retval = 1;
			} else {
				retval = 3;
			}
		}

	} else {
		if (host_readbs(ptr + 1) == 3) {
			retval = 1;
		} else if (host_readbs(ptr + 1) == 0) {
			retval = 3;
		}
	}

	return retval;
}

struct coords {
	signed short x,y;
};

struct dummy {
	struct coords d[4];
};

signed short seg037_0791(Bit8u* enemy, signed short enemy_nr, signed short attack_foe, signed short x, signed short y)
{
	signed short available_spells;
	signed short l2;
	signed short done;
	signed short retval;
	signed short mode;
	signed short l6;
	signed short l7;
	struct dummy diff;
	signed short l_si;
	signed short l_di;

#if !defined(__BORLANDC__)
	diff.d[0].x = 0;
	diff.d[0].y = 1;
	diff.d[1].x = 0;
	diff.d[1].y = -1;
	diff.d[2].x = -1;
	diff.d[2].y = 0;
	diff.d[3].x = 0;
	diff.d[3].y = 1;
#else
	diff = *(struct dummy*)(p_datseg + 0x5fd8);
#endif

	retval = 0;

	available_spells = l_si = 0;
	while ((l_si < 5) && (ds_readbs(0xf8b + host_readbs(enemy + 0x25) * 5 + l_si++) != -1))
	{
		available_spells++;
	}

	done = 0;
	while ((done == 0) && (host_readbs(enemy + 0x23) > 0)) {

		l7 = 0;

		for (l_si = 0; l_si < available_spells; l_si++) {

			l2 = ds_readbs(0xf8b + host_readbs(enemy + 0x25) * 5 + l_si);

			if (ds_readbs(0xf15 + l2 * 8) == 1) {

				if (random_schick(100) < 75) {
					l7 = 1;
					break;
				}
			}
		}

		if (l7 == 0) {
			l2 = ds_readbs(0xf8b + host_readbs(enemy + 0x25) * 5 + random_interval(0, available_spells - 1));
		}

		host_writeb(enemy + 0x2d, 0);

		if ( (mode = get_foe_attack_mode(l2, attack_foe)) > 0) {

			if (mode == 3) {
				host_writeb(enemy + 0x2d, enemy_nr + 10);
				host_writeb(enemy + 0x2c, (signed char)l2);
				retval = 1;
				done = 1;
			} else {

				if (!ds_readbs(0xf15 + l2 * 8)) {

					while ((host_readbs(enemy + 0x23) != 0) && (done == 0)) {

						l_si = host_readbs(enemy + 0x27);
						l_di = 0;

						while (!host_readbs(enemy + 0x2d) && (l_di < 4)) {

							if (test_foe_melee_attack(x, y, diff.d[l_si].x, diff.d[l_si].y, mode)) {
								host_writeb(enemy + 0x2d, get_cb_val(x + diff.d[l_si].x, y + diff.d[l_si].y));
							}

							l_di++;
							if (++l_si == 4) {
								l_si = 0;
							}
						}

						if (host_readbs(enemy + 0x2d)) {
							host_writeb(enemy + 0x2c, (signed char)l2);
							retval = 1;
							done = 1;
						} else if (host_readbs(enemy + 0x23) > 0) {
							if (!enemy_cursed(enemy)) {

								if (mode == 1)
									l6 = seg038(enemy, enemy_nr, x, y, 2);
								else
									l6 = seg038(enemy, enemy_nr, x, y, 0);

								if (l6 != -1) {
									seg037_00ae(enemy, enemy_nr);
									FIG_search_obj_on_cb(enemy_nr + 10, &x, &y);

									if (host_readbs(enemy + 0x23) < 3) {
										host_writeb(enemy + 0x23, 0);
									}
								} else {
									host_writeb(enemy + 0x23, 0);
								}
							} else {
								host_writeb(enemy + 0x23, 0);
							}
						}
					}
				} else {

					while ((done == 0) && (host_readbs(enemy + 0x23) > 0)) {

						l_si = host_readbs(enemy + 0x27);
						l_di = 0;

						while (!host_readbs(enemy + 0x2d) && (l_di < 4)) {

							host_writeb(enemy + 0x2d, (signed char)test_foe_range_attack(x, y, l_si, mode));

							l_di++;
							if (++l_si == 4) {
								l_si = 0;
							}
						}

						if (host_readbs(enemy + 0x2d)) {
							host_writeb(enemy + 0x2c, (signed char)l2);
							retval = 1;
							done = 1;
						} else if (host_readbs(enemy + 0x23) > 0) {
							if (!enemy_cursed(enemy)) {

								if (mode == 1)
									l6 = seg038(enemy, enemy_nr, x, y, 7);
								else
									l6 = seg038(enemy, enemy_nr, x, y, 6);

								if (l6 != -1) {
									seg037_00ae(enemy, enemy_nr);
									FIG_search_obj_on_cb(enemy_nr + 10, &x, &y);

									if (host_readbs(enemy + 0x23) < 5) {
										host_writeb(enemy + 0x23, 0);
									}
								} else {
									host_writeb(enemy + 0x23, 0);
								}
							} else {
								host_writeb(enemy + 0x23, 0);
							}
						}
					}
				}
			}
		}
	}

#if !defined(__BORLANDC__)
	if (retval)
		D1_INFO("Zauberspruch %d\n", host_readbs(enemy + 0x2c));
	else
		D1_INFO("Kein Zauberspruch\n");
#endif

	return retval;
}


signed short seg037_0b3e(Bit8u* enemy, signed short enemy_nr, signed short attack_foe, signed short x, signed short y)
{

	signed short cnt;
	signed short done;
	signed short retval;
	signed short l4;
	signed short dir;

	retval = 0;

	done = 0;

	while ((done == 0) && (host_readbs(enemy + 0x23) > 0)) {

		/* reset the attackee ID */
		host_writeb(enemy + 0x2d, 0);

		while ( (done == 0) && (host_readbs(enemy + 0x23) > 0)) {

			dir = host_readbs(enemy + 0x27);
			cnt = 0;

			/* check clockwise for someone to attack */
			while ( !host_readbs(enemy + 0x2d) && (cnt < 4)) {

				host_writeb(enemy + 0x2d,
					(signed char)test_foe_range_attack(x, y, dir, attack_foe));
				cnt++;
				if (++dir == 4) {
					dir = 0;
				}
			}

			if (host_readbs(enemy + 0x2d) != 0) {
				/* found someone to attack */
				retval = 1;
				done = 1;
			} else if (host_readbs(enemy + 0x23) > 0) {

					if (!enemy_cursed(enemy)) {
						if (attack_foe == 0)
							l4 = seg038(enemy, enemy_nr, x, y, 6);
						else
							l4 = seg038(enemy, enemy_nr, x, y, 7);

						if (l4 != -1) {
							seg037_00ae(enemy, enemy_nr);
							FIG_search_obj_on_cb(enemy_nr + 10, &x, &y);

							if (host_readbs(enemy + 0x23) < 3) {
								host_writeb(enemy + 0x23, 0);
							}
						} else {
							host_writeb(enemy + 0x23, 0);
						}
					} else {
						host_writeb(enemy + 0x23, 0);
					}
			}
		}
	}

	return retval;
}


void enemy_turn(Bit8u *enemy, signed short enemy_nr, signed short x, signed short y)
{
	signed short l1;
	signed short attack_foe;
	signed short dir;
	signed short l3;
	signed short done;
	signed short l5;
	signed short l6;
	signed short l7;
	struct dummy diff;
	signed short l_di;

	done = 0;

#if !defined(__BORLANDC__)
	diff.d[0].x = 1;
	diff.d[0].y = 0;
	diff.d[1].x = 0;
	diff.d[1].y = -1;
	diff.d[2].x = -1;
	diff.d[2].y = 0;
	diff.d[3].x = 0;
	diff.d[3].y = 1;
#else
	diff = *(struct dummy*)(p_datseg + 0x5fe8);
#endif

	/* check if we are in a special fight */
	if (ds_readws(CURRENT_FIG_NR) == 180) {
		/* F064: fight against GORAH */

		if (host_readbs(enemy) == 0x46) {
			/* GORAH waits the first 5 rounds */

			if (ds_readws(FIGHT_ROUND) < 5) {
				host_writeb(enemy + 0x23, 0);
			}
		}

	} else if ((ds_readws(CURRENT_FIG_NR) == 189) &&
			(random_interval(8, 12) <= ds_readws(FIGHT_ROUND))) {
		/* F099: fight against four HARPIES */

			or_ptr_bs(enemy + 0x32, 4);

	} else if ((ds_readws(CURRENT_FIG_NR) == 191) &&
			(FIG_count_active_enemies() <= 3)) {
		/* F122: fight against 13 WOLVES */

			or_ptr_bs(enemy + 0x32, 4);

	} else if (ds_readws(CURRENT_FIG_NR) == 192) {
		/* F144: final fight */

		if (enemy_cursed(enemy)) {
			host_writeb(enemy + 0x23, 0);
		}
	}

	while ( (done == 0) && (host_readbs(enemy + 0x23) > 0)) {

		if (ds_readbs(0xe38e) != -1) {
			FIG_remove_from_list(ds_readbs(0xe38e), 0);
			ds_writebs(0xe38e, -1);
		}

		FIG_init_list_elem(enemy_nr + 10);

		draw_fight_screen_pal(0);

		host_writeb(enemy + 0x2d, 0);
		host_writeb(enemy + 0x2b, 1);

		/* should I flee */
		if (host_readbs(enemy + 0x3d) >= host_readws(enemy + 0x13)) {
#if !defined(__BORLANDC__)
			D1_INFO("Feind %d flieht\n", enemy_nr);
#endif
			or_ptr_bs(enemy + 0x32, 4);
		}

		/* chance of 4% that an illusion enemy disappears */
		if (random_schick(100) < 5) {
#if !defined(__BORLANDC__)
			if (enemy_illusion(enemy)) {
				D1_INFO("Feind %d verliert seinen Illusionszauber\n", enemy_nr);
			}
#endif
			and_ptr_bs(enemy + 0x31, 0xdf);
		}

		if (!enemy_bit10(enemy)) {

			attack_foe = 0;

			if (enemy_bb(enemy)) {
				attack_foe = 1;
			}

			/* enemy can cast spells and has AE >= 5 left */
			if ((host_readbs(enemy + 0x25) != -1) && (host_readws(enemy + 0x17) >= 5) &&
				(seg037_0791(enemy, enemy_nr, attack_foe, x, y)))
			{
#if !defined(__BORLANDC__)
				D1_INFO("Feind %d zaubert\n", enemy_nr);
#endif
				host_writeb(enemy + 0x2b, 4);

				/* adjust BP */
				host_writeb(enemy + 0x23, host_readbs(enemy + 0x23) -5);

				if (host_readbs(enemy + 0x23) < 0) {
					host_writeb(enemy + 0x23, 0);
				}
				return;
			}

			/* enemy has range weapons */
			if ( ((host_readbs(enemy + 0x37) > 0) || (host_readbs(enemy + 0x3a) > 0)) &&
				seg037_0b3e(enemy, enemy_nr, attack_foe, x, y))
			{
#if !defined(__BORLANDC__)
				D1_INFO("Feind %d greift mit Fernkampfwaffe an\n", enemy_nr);
#endif
				host_writeb(enemy + 0x2b, 15);

				/* adjust BP */
				host_writeb(enemy + 0x23, host_readbs(enemy + 0x23) -3);

				if (host_readbs(enemy + 0x23) < 0) {
					host_writeb(enemy + 0x23, 0);
				}
				return;
			}

			host_writeb(enemy + 0x2d, 0);
			dir = host_readbs(enemy + 0x27);
			l3 = 0;
			while (!(host_readbs(enemy + 0x2d)) && (l3 < 4)) {

				if (test_foe_melee_attack(x, y, diff.d[dir].x, diff.d[dir].y, attack_foe)) {

					l5 = 1;

					if (is_in_byte_array(host_readbs(enemy + 1), p_datseg + TWO_FIELDED_SPRITE_ID))
					{

						l_di = get_cb_val(x - diff.d[dir].x, y - diff.d[dir].y);

						if (l_di && (enemy_nr + 30 != l_di)) {

							if ((l_di < 0) || (l_di >= 50) || (l_di >= 30) ||
								((l_di > 0) && (l_di < 10) && !hero_dead(get_hero(l_di - 1))) ||
								((l_di < 30) && (l_di >= 10) && !(enemy_dead(p_datseg + 0xd0df + 0x3e * l_di))))
							{
								l5 = 0;
							}
						}
					}

					if (l5 != 0) {
						host_writeb(enemy + 0x2d, get_cb_val(x + diff.d[dir].x, y + diff.d[dir].y));
					}
				}

				l3++;
				if (++dir == 4) {
					dir = 0;
				}
			}
		}

		if (host_readbs(enemy + 0x2d) != 0) {
			host_writeb(enemy + 0x2b, 2);
			host_writeb(enemy + 0x23, host_readbs(enemy + 0x23) -3);
			done = 1;
		} else if (host_readbs(enemy + 0x23) > 0) {

			if (!enemy_cursed(enemy)) {

				if (enemy_bit10(enemy)) {
					l1 = seg038(enemy, enemy_nr, x, y, 4);
					host_writeb(enemy + 0x23, 0);
				} else {
					if (enemy_bb(enemy))
						l1 = seg038(enemy, enemy_nr, x, y, 2);
					else
						l1 = seg038(enemy, enemy_nr, x, y, 0);
				}

				if (l1 != -1) {
					l6 = x;
					l7 = y;
					seg037_00ae(enemy, enemy_nr);
					FIG_search_obj_on_cb(enemy_nr + 10, &x, &y);

					if ((l6 == x) && (l7 == y)) {
						host_writeb(enemy + 0x23, 0);
					}

					if (host_readbs(enemy + 0x23) < 3) {
						host_writeb(enemy + 0x23, 0);
					}

				} else {
					host_writeb(enemy + 0x23, 0);
				}
			} else {
				host_writeb(enemy + 0x23, 0);
			}

			host_writeb(enemy + 0x2b, 1);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
