/**
 *	Rewrite of DSA1 v3.02_de functions of seg036 (Fight Hero KI)
 *	Functions rewritten: 10/10 (complete)
 *	Borlandified and identical
 *
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg036.cpp
 */

#include <string.h>

#include "v302de.h"

#include "seg001.h"
#include "seg002.h"
#include "seg005.h"
#include "seg006.h"
#include "seg007.h"
#include "seg032.h"
#include "seg036.h"
#include "seg038.h"
#include "seg039.h"
#include "seg041.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   copies an ani sequence
 *
 * \param   dst         destination pointer for data
 * \param   ani_no      * \param mode	DAT-File to use 3 = WEAPANI.DAT, else ANI.DAT
 * \return              the length of the sequence in bytes.
 */
#if defined(__BORLANDC__)
static
#endif
signed short KI_copy_ani_sequence(Bit8u *dst, signed short ani_no, signed short mode)
{
	Bit8u *p_datbuffer;
	Bit8u *p_datitem;
	signed char len;

	signed short i;
	signed short ani_max_no;


	/* set the right buffer */
	p_datbuffer = Real2Host(ds_readd(BUFFER_ANIDAT));

	/* This function is never calld with mode == 3 */
	if (mode == 3)
		p_datbuffer = Real2Host(ds_readd(BUFFER_WEAPANIDAT));

	/* read how many ani sequences are in the file */
	ani_max_no = host_readw(p_datbuffer);

	/* check if the desired ani_no is in the range */
	if (ani_no < 0)
		return 0;

	if (ani_no > ani_max_no)
		return 0;

	/* set p_datitem to the first (0) ani sequence */
	p_datitem = p_datbuffer;
	p_datitem += ani_max_no + 2;
	/* set len to the lenght first (0) ani sequence */
	len = host_readb(p_datbuffer + 2);

	/* forward to the desired ani sequence */
	for (i = 1; i <= ani_no; i++) {
		p_datitem += len;
		len = host_readb(p_datbuffer + i + 2);
	}

	p_datitem++;

	len = len - 2;

	/* copy the ani sequenecs to dst */
	for (i = 0; len > i; i++) {
		host_writeb(dst, host_readb(p_datitem));
		p_datitem++;
		dst++;
	}

	return len;
}

void seg036_00ae(Bit8u *hero, signed short hero_pos)
{
	signed short i;
	signed char dir1;
	signed char dir2;
	Bit8u *ptr1;
	signed char dir3;
	Bit8u *ptr2;

	ds_writeb(FIG_ANISHEETS, 0);
	ds_writeb((FIG_ANISHEETS + 242), host_readbs(hero + HERO_SPRITE_NO));

	ptr1 = p_datseg + (FIG_ANISHEETS + 1);
	ptr2 = Real2Host(ds_readd(GFX_ANI_INDEX + 4 * host_readbs(hero + HERO_SPRITE_NO)));

	i = 0;

	while (ds_readbs(FIG_MOVE_PATHDIR + i) != -1) {

		if (host_readbs(hero + HERO_VIEWDIR) != ds_readbs(FIG_MOVE_PATHDIR + i)) {

			dir2 = dir1 = -1;
			dir3 = host_readbs(hero + HERO_VIEWDIR);
			dir2 = dir3;
			dir3++;

			if (dir3 == 4) {
				dir3 = 0;
			}

			if (ds_readbs(FIG_MOVE_PATHDIR + i) != dir3) {

				dir1 = dir3;
				dir3++;

				if (dir3 == 4) {
					dir3 = 0;
				}

				if (ds_readbs(FIG_MOVE_PATHDIR + i) != dir3) {

					dir2 = host_readbs(hero + HERO_VIEWDIR) + 4;
					dir1 = -1;
				}
			}

			/* set heros looking direction */
			host_writeb(hero + HERO_VIEWDIR, ds_readbs(FIG_MOVE_PATHDIR + i));

			ptr1 += KI_copy_ani_sequence(ptr1, host_readws(ptr2 + dir2 * 2), 2);

			if (dir1 != -1) {
				ptr1 += KI_copy_ani_sequence(ptr1, host_readws(ptr2 + dir1 * 2), 2);
			}
		}

		if (ds_readbs(FIG_MOVE_PATHDIR + i) == ds_readbs((FIG_MOVE_PATHDIR+1) + i)) {
			ptr1 += KI_copy_ani_sequence(ptr1, host_readws(ptr2 + (ds_readbs(FIG_MOVE_PATHDIR + i) + 12) * 2), 2);
			i += 2;
			/* BP - 2 */
			host_writeb(hero + HERO_BP_LEFT, host_readbs(hero + HERO_BP_LEFT) - 2);
		} else {
			ptr1 += KI_copy_ani_sequence(ptr1, host_readws(ptr2 + (ds_readbs(FIG_MOVE_PATHDIR + i) + 8) * 2), 2);
			i++;
			/* BP - 1 */
			dec_ptr_bs(hero + HERO_BP_LEFT);
		}
	}

	host_writeb(ptr1, -1);
	FIG_call_draw_pic();
	FIG_remove_from_list(ds_readbs(FIG_CB_MAKRER_ID), 0);
	ds_writeb(FIG_CB_MAKRER_ID, -1);
	FIG_set_sheet(host_readbs(hero + HERO_FIGHTER_ID), 0);
	draw_fight_screen(0);
	memset(p_datseg + FIG_ANISHEETS, -1, 0xf3);
	FIG_init_list_elem(hero_pos + 1);
}

/**
 * \brief   changes the weapon of a hero
 *
 * \param   hero        pointer to the hero with a broken weapon
 * \return              1 if a weapon was found, 0 if the hero fights now with bare hands
 */
signed short KI_change_hero_weapon(Bit8u *hero)
{
	signed short pos;
	signed short has_new_weapon = 0;
	Bit8u *item_p;
	signed short item_id;
	Bit8u *ptr;

	for (pos = 7; pos < 23; pos++) {

		item_id = host_readws(hero + HERO_ITEM_HEAD + pos * 14);
		item_p = get_itemsdat(item_id);

		/* grab the first melee weapon on top of the knapsack,
		 * and exchange it with the broken weapon. */
		if (item_weapon(item_p) &&
			(host_readbs(item_p + 3) != 7) &&
			(host_readbs(item_p + 3) != 8) &&
			(host_readbs(item_p + 3) != 5))
		{
			move_item(3, pos, hero);
			has_new_weapon = 1;
			break;
		}
	}

	if (!has_new_weapon) {

		/* find a free slot, to get rid of the broken weapon */
		for (pos = 7; pos < 23; pos++) {
			if (host_readws(hero + HERO_ITEM_HEAD + pos * 14) == 0) {
				move_item(3, pos, hero);
				has_new_weapon = 2;
				break;
			}
		}

		/* if nothing helps, put it in the left hand */
		if (!has_new_weapon) {
			move_item(3, 4, hero);
		}

		has_new_weapon = 0;
	}

	ptr = Real2Host(FIG_get_ptr(host_readbs(hero + HERO_FIGHTER_ID)));
	host_writeb(ptr + 0x2, host_readbs(hero + HERO_VIEWDIR));
	host_writeb(ptr + 0xd, -1);
	draw_fight_screen_pal(0);
	host_writeb(hero + HERO_BP_LEFT, host_readbs(hero + HERO_BP_LEFT) - 2);

	return has_new_weapon;
}

/**
 * \brief   check if a neighbour can be attacked
 *
 * \param   start_x     X-Coordinate of the hero
 * \param   start_y     Y-Coordinate of the hero
 * \param   offset_x    X-Direction -1/0/+1
 * \param   offset_y    Y-Direction -1/0/+1
 * \param   mode        0 = Attack only Enemies / 1 = Attack heroes and enemies / 2 = Attack only Heros
 * \return              1 if an attack is possible in that mode, else 0.
 */
signed short KI_can_attack_neighbour(signed short start_x, signed short start_y,
			signed short offset_x, signed short offset_y,
			signed short mode)
{
	signed char target = get_cb_val(start_x + offset_x, start_y + offset_y);

	if (mode == 1) {
		/* target is hero or enemy */
		if ( ( (target > 0) && (target < 10) &&
			!hero_dead(get_hero(target - 1)) &&
			!hero_unc(get_hero(target - 1))
			) || (

			((target >= 10) && (target < 30) &&
				!enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + target * SIZEOF_ENEMY_SHEET) &&
				enemy_bb(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + target * SIZEOF_ENEMY_SHEET))))
		{
			return 1;
		} else {
			return 0;
		}

	} else if (!mode) {
		/* target is an enemy */
		if ((target >= 10) && (target < 30) &&
			!enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + target * SIZEOF_ENEMY_SHEET))
		{
			return 1;
		} else {
			return 0;
		}
	} else if (mode == 2) {
		/* target is a hero */
		if ((target > 0) && (target < 10) &&
			!hero_dead(get_hero(target - 1)) &&
			!hero_unc(get_hero(target - 1))) {

			return 1;
		} else {
			return 0;
		}
	}

	return 0;
}

/**
 * \brief   TODO
 *
 * \param   x           x-coordinate of the hero
 * \param   y           y-coordinate of the hero
 * \param   dir         looking direction of the hero
 * \param   cursed      0 = hero is not cursed, 1 = hero is cursed
 * \return              0 = no target found, fight-id of the target
 */
signed short KI_search_spell_target(signed short x, signed short y,
				signed short dir, signed short cursed)
{
	signed short x_diff;
	signed short y_diff;
	signed char obj_id;
	signed short done;
	signed short will_attack;

	done = 0;
	x_diff = 0;
	y_diff = 0;
	will_attack = 0;

	while (done == 0) {

		/* calculate the offset from direction */
		if (dir == 0) {
			x_diff++;
		} else if (dir == 1) {
			y_diff--;
		} else if (dir == 2) {
			x_diff--;
		} else {
			y_diff++;
		}

		/* check the field is on the chessboard */
		if ((y + y_diff < 0) || (y + y_diff > 23) ||
			(x + x_diff < 0) || (x + x_diff > 23))
		{
			done = 1;
			continue;
		}

		/* get the fight object ID of the object on that field */
		obj_id = get_cb_val(x + x_diff, y + y_diff);

		if (cursed == 1) {

			/* attack everyone */
			if ( ((obj_id > 0) && (obj_id < 10) &&
				!hero_dead(get_hero(obj_id - 1)) &&
				!hero_unc(get_hero(obj_id - 1))
				) || (
				(obj_id >= 10) && (obj_id < 30) &&
					!enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + obj_id * SIZEOF_ENEMY_SHEET) &&
					enemy_bb(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + obj_id * SIZEOF_ENEMY_SHEET))
				)
			{
				will_attack = 1;
				done = 1;

			} else if ( (obj_id != 0) && (((obj_id >= 10) && (obj_id < 30) &&
					!enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + obj_id * SIZEOF_ENEMY_SHEET)
					) || ((obj_id >= 50) &&
						!is_in_word_array(obj_id - 50, (signed short*)(p_datseg + CB_OBJ_NONOBSTACLE)))
					))
				{
					done = 1;
				}

		} else if (cursed == 0) {

			/* attack only enemies */
			if ((obj_id >= 10) && (obj_id < 30) && !enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + obj_id * SIZEOF_ENEMY_SHEET))
			{
				will_attack = 1;
				done = 1;
			} else if ((obj_id != 0) &&
					 (
#ifdef M302de_ORIGINAL_BUGFIX
						(obj_id > 0) &&
#endif
						 (((obj_id < 10) &&
						!hero_dead(get_hero(obj_id - 1)) &&
						!hero_unc(get_hero(obj_id - 1))
						) || (
							(obj_id >= 50) &&
							!is_in_word_array(obj_id - 50, (signed short*)(p_datseg + CB_OBJ_NONOBSTACLE))
						))
					)
				)
				{
					done = 1;
				}
		}
	}

	if (will_attack == 0) {
		return 0;
	} else {
		return obj_id;
	}
}

/**
 * \brief   selects a target to be attacked from hero with a spell
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position of the hero in the party
 * \param   cursed      0 = hero is not cursed, 1 = hero is cursed
 * \param   x           x-coordinate of the hero
 * \param   y           y-coordinate of the hero
 * \return              0 = no target found, 1 = target_found (long distance), 2 = target fount (short distance)
 */
signed short KI_select_spell_target(Bit8u *hero, signed short hero_pos, signed short cursed, signed short x, signed short y)
{
	signed short dir;
	signed short count;
	signed short done;
	signed short retval;
	signed short l5;
	signed short target_x;
	signed short target_y;

	retval = 0;
	done = 0;

	while ((done == 0) && (host_readbs(hero + HERO_BP_LEFT) > 0)) {

		/* reset target fight-id */
		host_writeb(hero + HERO_ENEMY_ID, 0);

		if (host_readbs(hero + HERO_BP_LEFT) >= 3) {

			dir = host_readbs(hero + HERO_VIEWDIR);

			count = 0;

			/* try to find a target clockwise from current direction */
			while (!host_readbs(hero + HERO_ENEMY_ID) && (count < 4)) {

				host_writebs(hero + HERO_ENEMY_ID, (signed char)KI_search_spell_target(x, y, dir, cursed));

				count++;

				if (++dir == 4) {
					dir = 0;
				}
			}
		}

		/* check if a target was found */
		if (host_readbs(hero + HERO_ENEMY_ID) != 0) {
			/* yes */

			FIG_search_obj_on_cb(host_readbs(hero + HERO_ENEMY_ID), &target_x, &target_y);

#if !defined(__BORLANDC__)
			/* BE-fix */
			target_x = host_readws((Bit8u*)&target_x);
			target_y = host_readws((Bit8u*)&target_y);
#endif

			if (calc_beeline(target_x, target_y, x, y) < 2) {
				retval = 2;
			} else {
				retval = 1;
			}

			done = 1;

		} else {
			/* try a more expensive search */

			if (!hero_unkn2(hero)) {

				if (cursed == 0) {
					l5 = seg038(hero, hero_pos, x, y, 9);
				} else {
					l5 = seg038(hero, hero_pos, x, y, 8);
				}

				if (l5 != -1) {
					seg036_00ae(hero, hero_pos);

					FIG_search_obj_on_cb(hero_pos + 1, &x, &y);

#if !defined(__BORLANDC__)
					/* BE-fix */
					x = host_readws((Bit8u*)&x);
					y = host_readws((Bit8u*)&y);
#endif
					if (host_readbs(hero + HERO_BP_LEFT) < 3) {
						/* set BP to 0 */
						host_writeb(hero + HERO_BP_LEFT, 0);
					}
				} else {
					/* set BP to 0 */
					host_writeb(hero + HERO_BP_LEFT, 0);
				}
			} else {
				/* set BP to 0 */
				host_writeb(hero + HERO_BP_LEFT, 0);
			}
		}
	}

	return retval;
}

/**
 * \brief   TODO
 *
 * \param   spell       spell index
 * \param   cursed      0 = hero is not cursed, 1 = hero is cursed
 * \return              TODO	{-1, 0, 1, 2}
 */
signed short KI_get_spell(signed short spell, signed short cursed)
{
	Bit8u *p;
	signed short retval = -1;

	/* make a pointer to the spell description */
	p = p_datseg + spell * 10 + SPELL_DESCRIPTIONS;

	if (cursed == 0) {
		if (host_readb(p + 7) == 2)
			retval = 1;
		else if (host_readb(p + 7) == 1 || host_readb(p + 7) == 3)
			retval = 0;
		else
			retval = 2;
	} else {
		if (host_readb(p + 7) == 3)
			retval = 1;
		else if (host_readb(p + 7) == 0)
			retval = 2;
	}

	return retval;
}

struct coord {
	signed short x;
	signed short y;
};

struct dummy {
	struct coord a[4];
};

/**
 * \brief   TODO
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position of the hero in the party
 * \param   cursed      0 = hero is not cursed, 1 = hero is cursed
 * \param   x           x-coordinate of the hero
 * \param   y           y-coordinate of the hero
 * \return              {0, 1}
 */
signed short seg036_8cf(Bit8u *hero, signed short hero_pos, signed short cursed, signed short x, signed short y)
{
	signed short l_si;
	signed short count;
	signed short spell;
	signed short done;
	signed short retval;
	signed short spell_mode;
	signed short l5;
	signed short decided;

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
	struct dummy a = *(struct dummy*)(p_datseg + VIEWDIR_OFFSETS3);
#endif

	retval = 0;
	done = 0;

	while ((done == 0) && (host_readbs(hero + HERO_BP_LEFT) > 0)) {

		decided = 0;

		for (l_si = 0; l_si <= 10; l_si++) {

			/* get a spell from an array */
			spell = ds_readbs(AF_SPELL_LIST + l_si);

			if ((ds_readbs((SPELL_DESCRIPTIONS + 8) + 10 * spell) == 1) && (random_schick(100) < 50))
			{
				decided = 1;
				break;

			} else if (random_schick(100) < 25) {
				decided = 1;
				break;
			}
		}

		if (decided == 0) {

			spell = ds_readbs(AF_SPELL_LIST + random_interval(0, 10));
		}

		/* reset the target of the hero */
		host_writebs(hero + HERO_ENEMY_ID, 0);

		if ((spell_mode = KI_get_spell(spell, cursed)) != -1) {

			if (spell_mode == 2) {

				/* set target to hero */
				host_writebs(hero + HERO_ENEMY_ID, hero_pos + 1);
				/* set spell */
				host_writebs(hero + HERO_SPELL_ID, (signed char)spell);
				retval = 1;
				done = 1;
			} else {

				if (!ds_readbs((SPELL_DESCRIPTIONS + 8) + 10 * spell)) {

					while ((host_readbs(hero + HERO_BP_LEFT) != 0) && (done == 0)) {

						l_si = host_readbs(hero + HERO_VIEWDIR);

						count = 0;
						while ((!host_readbs(hero + HERO_ENEMY_ID)) && (count < 4)) {

							if (KI_can_attack_neighbour(x, y, a.a[l_si].x, a.a[l_si].y, spell_mode)) {
								host_writebs(hero + HERO_ENEMY_ID, get_cb_val(x + a.a[l_si].x, y + a.a[l_si].y));
							}

							count++;

							if (++l_si == 4) {
								l_si = 0;
							}
						}

						if (host_readbs(hero + HERO_ENEMY_ID) != 0) {

							if (host_readbs(hero + HERO_BP_LEFT) >= 5) {
								/* enough BP */
								host_writeb(hero + HERO_SPELL_ID, (signed char)spell);
								retval = 1;
							} else {
								/* set BP to 0 */
								host_writeb(hero + HERO_BP_LEFT, 0);
							}

							done = 1;
						} else if (!hero_unkn2(hero)) {

							if (spell_mode == 0) {
								l5 = seg038(hero, hero_pos, x, y, 3);
							} else {
								l5 = seg038(hero, hero_pos, x, y, 1);
							}

							if (l5 != -1) {
								seg036_00ae(hero, hero_pos);
								FIG_search_obj_on_cb(hero_pos + 1, &x, &y);
#if !defined(__BORLANDC__)
								/* BE-fix */
								x = host_readws((Bit8u*)&x);
								y = host_readws((Bit8u*)&y);
#endif
							} else {
								/* set BP to 0 */
								host_writeb(hero + HERO_BP_LEFT, 0);
							}

						} else {
							/* set BP to 0 */
							host_writeb(hero + HERO_BP_LEFT, 0);
						}
					}
				} else {
					while ((done == 0) && (host_readbs(hero + HERO_BP_LEFT) > 0)) {

						l_si = host_readbs(hero + HERO_VIEWDIR);

						count = 0;

						while ((!host_readbs(hero + HERO_ENEMY_ID)) && (count < 4)) {

							host_writebs(hero + HERO_ENEMY_ID, (signed char)KI_search_spell_target(x, y, l_si, spell_mode));

							count++;

							if (++l_si == 4) {
								l_si = 0;
							}
						}

						if (host_readbs(hero + HERO_ENEMY_ID) != 0) {

							if (host_readbs(hero + HERO_BP_LEFT) >= 5) {
								/* enough BP */
								host_writeb(hero + HERO_SPELL_ID, (signed char)spell);
								retval = 1;
							} else {
								/* set BP to 0 */
								host_writeb(hero + HERO_BP_LEFT, 0);
							}

							done = 1;
						} else if (!hero_unkn2(hero)) {

							if (spell_mode == 0) {
								l5 = seg038(hero, hero_pos, x, y, 9);
							} else {
								l5 = seg038(hero, hero_pos, x, y, 8);
							}

							if (l5 != -1) {
								seg036_00ae(hero, hero_pos);
								FIG_search_obj_on_cb(hero_pos + 1, &x, &y);
#if !defined(__BORLANDC__)
								/* BE-fix */
								x = host_readws((Bit8u*)&x);
								y = host_readws((Bit8u*)&y);
#endif
							} else {
								/* set BP to 0 */
								host_writeb(hero + HERO_BP_LEFT, 0);
							}

						} else {
							/* set BP to 0 */
							host_writeb(hero + HERO_BP_LEFT, 0);
						}
					}
				}
			}
		}
	}

	return retval;
}

/**
 * \brief   count the other heros in the current group
 *
 * \param   hero_pos    position of the calling hero
 * \return              number of heros in the group - 1
 */
signed short KI_count_heros(signed short hero_pos)
{
	signed short cnt = 0;
	signed short i;

	/* for each hero in this group */
	for (i = 0; ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) > i; i++) {

		if ((i != hero_pos) && check_hero(get_hero(i))) {
			cnt++;
		}
	}

	return cnt;
}

/**
 * \brief   TODO
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position of the hero in the party
 * \param   x           x-coordinate of the hero
 * \param   y           y-coordinate of the hero
 */
void KI_hero(Bit8u *hero, signed short hero_pos, signed short x, signed short y)
{
	signed short l_di;
	signed short l1;
	signed short done;
	signed short l4;
	signed short l5;
	signed short x_bak;
	signed short y_bak;
	signed short l8;
	signed short hero_x;
	signed short hero_y;

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
	struct dummy a = *(struct dummy*)(p_datseg + VIEWDIR_OFFSETS4);
#endif

	done = 0;
	l5 = 1;
	host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);
	if (host_readbs(hero + HERO_NPC_ID) > 0) {

		if (host_readbs(hero + HERO_NPC_ID) == NPC_NARIELL) {
			/* equip LONGBOW and ARROWS in the first round,
			 * if the hero has them in the inventory */
			if ((ds_readws(FIGHT_ROUND) == 0) &&
				(host_readws(hero + HERO_ITEM_RIGHT) != ITEM_LONGBOW) &&
				(get_item_pos(hero, ITEM_ARROWS) != -1) &&
				(get_item_pos(hero, ITEM_LONGBOW) != -1))
			{
				move_item(3, get_item_pos(hero, ITEM_LONGBOW), hero);

				if (host_readws(hero + HERO_ITEM_LEFT) != ITEM_ARROWS) {
					move_item(4, get_item_pos(hero, ITEM_ARROWS), hero);
				}
			}

		} else if (host_readbs(hero + HERO_NPC_ID) == NPC_HARIKA) {

			if (host_readws(hero + HERO_LE) <= 12) {

				/* equip LONGBOW and ARROWS in the first round,
				 * if the hero has them in the inventory */
				if ((ds_readws(FIGHT_ROUND) == 0) &&
					(host_readws(hero + HERO_ITEM_RIGHT) != ITEM_LONGBOW) &&
					(get_item_pos(hero, ITEM_ARROWS) != -1) &&
					(get_item_pos(hero, ITEM_LONGBOW) != -1))
				{
					move_item(3, get_item_pos(hero, ITEM_LONGBOW), hero);

					if (host_readws(hero + HERO_ITEM_LEFT) != ITEM_ARROWS) {
						move_item(4, get_item_pos(hero, ITEM_ARROWS), hero);
					}
				} else if (FIG_get_range_weapon_type(hero) == -1)
				{
					host_writebs(hero + HERO_ACTION_ID, FIG_ACTION_FLEE);
				}
			}

		} else if (host_readbs(hero + HERO_NPC_ID) == NPC_CURIAN) {

			if ((host_readws(hero + HERO_LE) < 10) &&
				(host_readws(hero + HERO_AE) < 10))
			{
				host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_FLEE);
			}

		} else if (host_readbs(hero + HERO_NPC_ID) == NPC_ARDORA) {

			if (host_readws(hero + HERO_LE) < 8)
			{
				host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_FLEE);
			}

		} else if (host_readbs(hero + HERO_NPC_ID) == NPC_GARSVIK) {

			if (!KI_count_heros(hero_pos)) {
				host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_FLEE);
			}

		} else if (host_readbs(hero + HERO_NPC_ID) == NPC_ERWO) {

			if (host_readws(hero + HERO_LE) < 15)
			{
				host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_FLEE);
			}

		}

		if (FIG_get_first_active_hero() == 6) {
			host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_FLEE);
		}

		if (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_FLEE) {

			FIG_search_obj_on_cb(hero_pos + 1, &hero_x, &hero_y);

#if !defined(__BORLANDC__)
			/* BE-fix */
			hero_x = host_readws((Bit8u*)&hero_x);
			hero_y = host_readws((Bit8u*)&hero_y);
#endif
			if ((hero_x - 1 >= 0) && (hero_x + 1 <= 25) &&
				(hero_y -1 >= 0) && (hero_y + 1 <= 25) &&
				(get_cb_val(hero_x, hero_y + 1) != 0) &&
				(get_cb_val(hero_x, hero_y - 1) != 0) &&
				(get_cb_val(hero_x + 1, hero_y) != 0) &&
				(get_cb_val(hero_x - 1, hero_y) != 0))
			{
				host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);
			}
		}
	}

	if (host_readbs(hero + HERO_BP_LEFT) == 1) {
		/* set BP to 0 */
		host_writebs(hero + HERO_BP_LEFT, 0);
	}

	while ((done == 0) && (host_readbs(hero + HERO_BP_LEFT) > 0)) {

		seg001_02c4();

		if ((host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_FLEE) && (host_readbs(hero + HERO_BP_LEFT) > 0)) {

			if (!hero_unkn2(hero)) {

				l4 = seg038(hero, hero_pos, x, y, 5);

				if (l4 != -1) {

					x_bak = x;
					y_bak = y;

					seg036_00ae(hero, hero_pos);

					host_writeb(hero + HERO_ENEMY_ID, 0);

					if (FIG_search_obj_on_cb(hero_pos + 1, &x, &y)) {

#if !defined(__BORLANDC__)
						/* BE-fix */
						x = host_readws((Bit8u*)&x);
						y = host_readws((Bit8u*)&y);
#endif
						host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);

						if ((x_bak == x) && (y_bak == y)) {

							host_writeb(hero + HERO_BP_LEFT, 0);
						}
					} else {
						host_writeb(hero + HERO_BP_LEFT, 0);
					}

					if (host_readbs(hero + HERO_BP_LEFT) < 3) {
						host_writeb(hero + HERO_BP_LEFT, 0);
					}

				} else {
					host_writeb(hero + HERO_BP_LEFT, 0);
				}
			} else {
				host_writeb(hero + HERO_BP_LEFT, 0);
			}

		} else {

			if ((host_readbs(hero + HERO_TYPE) >= 7) &&		/* magic user */
				(host_readws(hero + HERO_AE) > 10) &&	/* AE > 10 */
				(l5 != 0) &&
				(ds_readws(CURRENT_FIG_NO) != 192) &&	/* not in the final fight */
				(ds_readbs(AUTOFIGHT_MAGIC) != 0))		/* ??? a bool variable, maybe autofight magic */
			{
				if (seg036_8cf(hero, hero_pos, hero_cursed(hero), x, y)) {

					host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_SPELL);
					host_writeb(hero + HERO_BP_LEFT, 0);

				} else {
					l5 = 0;
				}
			}

			if ((host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_MOVE) && (host_readbs(hero + HERO_BP_LEFT) > 0)) {

				if (FIG_get_range_weapon_type(hero) != -1) {

					if (range_attack_check_ammo(hero, 2)) {

						l8 = KI_select_spell_target(hero, hero_pos, hero_cursed(hero), x, y);

						if (l8 != 0) {
							if (l8 == 2) {
								if (!KI_change_hero_weapon(hero)) {
									done = 1;
								}
							} else {
								host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_RANGE_ATTACK);
							}
						}

						/* set BP to 0 */
						host_writeb(hero + HERO_BP_LEFT, 0);
					} else {
						if (!KI_change_hero_weapon(hero)) {
							done = 1;
						}
					}
				} else {
					host_writeb(hero + HERO_ENEMY_ID, 0);

					if (host_readbs(hero + HERO_BP_LEFT) >= 3) {

						l_di = host_readbs(hero + HERO_VIEWDIR);
						l1 = 0;

						while (!host_readbs(hero + HERO_ENEMY_ID) && (l1 < 4)) {

							if (KI_can_attack_neighbour(x, y, a.a[l_di].x, a.a[l_di].y, hero_cursed(hero)))
							{
								host_writeb(hero + HERO_ENEMY_ID, get_cb_val(x + a.a[l_di].x, y + a.a[l_di].y));
							}

							l1++;
							if (++l_di == 4) {
								l_di = 0;
							}
						}
					}

					if (host_readbs(hero + HERO_ENEMY_ID) != 0) {

						host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_ATTACK);
						host_writeb(hero + HERO_BP_LEFT, 0);

					} else {

						if (!hero_unkn2(hero)) {

							if (!hero_cursed(hero)) {
								l4  = seg038(hero , hero_pos, x, y, 3);
							} else {
								l4  = seg038(hero , hero_pos, x, y, 1);
							}

							if (l4 != -1) {
								x_bak = x;
								y_bak = y;

								seg036_00ae(hero, hero_pos);

								host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);
								host_writeb(hero + HERO_ENEMY_ID, 0);

								FIG_search_obj_on_cb(hero_pos + 1, &x, &y);

#if !defined(__BORLANDC__)
								/* BE-fix */
								x = host_readws((Bit8u*)&x);
								y = host_readws((Bit8u*)&y);
#endif
								if ((x_bak == x) && (y_bak == y)) {
									host_writeb(hero + HERO_BP_LEFT, 0);
								}

								if (host_readbs(hero + HERO_BP_LEFT) < 3) {
									host_writeb(hero + HERO_BP_LEFT, 0);
								}
							} else {
								host_writeb(hero + HERO_BP_LEFT, 0);
							}
						} else {
							host_writeb(hero + HERO_BP_LEFT, 0);
						}
					}
				}
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
