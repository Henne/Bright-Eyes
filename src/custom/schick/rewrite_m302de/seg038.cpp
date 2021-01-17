/*
 *      Rewrite of DSA1 v3.02_de functions of seg038 (Fight)
 *      Functions rewritten: 6/6
 *
 *      Functions called rewritten 5/5
 *      Functions uncalled rewritten 1/1
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg038.cpp
 */

#include <string.h>

#include "v302de.h"

#include "seg006.h"
#include "seg007.h"
#include "seg038.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   searches an object on the chessboard
 *
 * \param   obj         number of the object
 * \param   px          pointer where x should be stored
 * \param   py          pointer where y should be stored
 * \return              0 if the object was not found. If the object was
 * found it returns 1 and stores the coordinates at the pointers.
 */
unsigned short FIG_search_obj_on_cb(signed short obj, signed short *px, signed short *py)
{
	/* This is the readable version */
	/*
	for (*px = 0; *px < 25; ++*px) {
		for (*py = 0; *py < 24; ++*py) {
			if (get_cb_val(*px, *py) == obj) {
				return 1;
			}
		}
	}
	*/

	/* This is the Endian save version */
	for (host_writew((Bit8u*)px, 0); host_readws((Bit8u*)px) < 25; inc_ptr_ws((Bit8u*)px)) {
		for (host_writew((Bit8u*)py, 0); host_readws((Bit8u*)py) < 24; inc_ptr_ws((Bit8u*)py)) {
			if (get_cb_val(host_readws((Bit8u*)px), host_readws((Bit8u*)py)) == obj) {
				return 1;
			}
		}
	}

	return 0;
}

void FIG_init_list_elem(signed short obj)
{

	signed short x, y;

	FIG_search_obj_on_cb(obj, &x, &y);

#if !defined(__BORLANDC__)
	/* BE-fix */
	x = host_readws((Bit8u*)&x);
	y = host_readws((Bit8u*)&y);
#endif

	/* This initializes a FIGHTER structure at DS:FIG_LIST_ELEM */
	ds_writew(FIG_LIST_ELEM, 0);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_NVF_NO), 0);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_CBX), (signed char)x);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_CBY), (signed char)y);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_OFFSETX), 0);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_OFFSETY), 4);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_HEIGHT), 11);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_WIDTH), 22);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_X1), 0);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_Y1), 0);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_X2), 21);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_Y2), 10);

	ds_writeb((FIG_LIST_ELEM + FIGHTER_MONSTER), 0);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_RELOAD), 0);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_WSHEET), -1);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_SHEET), -1);

	ds_writed((FIG_LIST_ELEM + FIGHTER_GFXBUF), ds_readd(FIG_CB_MARKER_BUF));
	ds_writeb((FIG_LIST_ELEM + FIGHTER_Z), 0);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_VISIBLE), 1);
	ds_writeb((FIG_LIST_ELEM + FIGHTER_TWOFIELDED), -1);

	ds_writeb(FIG_CB_MAKRER_ID, FIG_add_to_list(-1));
}

void FIG_unused(signed short a1, signed short a2,  Bit8u *p1, Bit8u *p2)
{
	signed short loc1 = 10;
	signed short loc2 = 118;

	host_writew(p2, ((loc2 - a2) + ((a1 - loc1) / 2)) / 10);
	host_writew(p1, (a1 - loc1) / 10 - host_readws(p2));

}

struct coordinates {
	signed short x;
	signed short y;
};

struct dummy {
	struct coordinates o[4];
};

/**
 * \brief  Computes path via backtracking on the distance table
 *
 * \param   dist_table_ptr    pointer to the distance table
 * \param   target_x          x-coordinate of the target
 * \param   target_y          y_coordinate of the target
 * \param   dist              distance to the target
 * \param   bp_avail          left BP of the fighter
 * \param   mode              mode (see FIG_find_path_to_target)
 * \param   two_squares       fighter occupies two squares on the map (wolves, dogs, lions)
 */
void FIG_find_path_to_target_backtrack(Bit8u *dist_table_ptr, signed short target_x, signed short target_y,
			signed short dist, signed char bp_avail,
			signed short mode, signed short two_squares, signed short fighter_id)
{
	signed short i;
	signed short dist_duplicate; /* duplicates the dist variable. apparently redundant */
	signed short backtrack_x;
	signed short backtrack_y;
	signed short cb_or_dist_entry; /* used for both a chessboard entry and as a distance table entry */
	signed short dist_bak;
	signed short target_is_escape_square = 0;
	signed short tail_x;
	signed short tail_y;
	signed short dir;
	signed short success;
	signed short lowest_nr_dir_changes;
	signed short nr_dir_changes;
	signed short best_dir;
	Bit8u *path_cur;
	signed short x_bak;
	signed short y_bak;
	signed short target_out_of_reach; /* will be set to 1 if the target is out of reach with avail_bp steps. Redundant, as this could simply be tested by (avail_bp < dist). */
	struct dummy inverse_coordinate_offset;
	Bit8u *path_table[4];

#if !defined(__BORLANDC__)
	for (int i = 0; i < 4; i++) {
		inverse_coordinate_offset.o[i].x = host_readws(p_datseg + VIEWDIR_INVOFFSETS1 + 4 * i);
		inverse_coordinate_offset.o[i].y = host_readws(p_datseg + VIEWDIR_INVOFFSETS1 + 4 * i + 2);
	}
#else
	inverse_coordinate_offset = *((struct dummy*)(p_datseg + VIEWDIR_INVOFFSETS1));
#endif

	target_out_of_reach = 0;
	lowest_nr_dir_changes = 99;

	memset(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), 0, 80);
	path_table[0] = Real2Host((RealPt)ds_readd(TEXT_OUTPUT_BUF));
	path_table[1] = Real2Host((RealPt)ds_readd(TEXT_OUTPUT_BUF)) + 20;
	path_table[2] = Real2Host((RealPt)ds_readd(TEXT_OUTPUT_BUF)) + 40;
	path_table[3] = Real2Host((RealPt)ds_readd(TEXT_OUTPUT_BUF)) + 60;

	cb_or_dist_entry = get_cb_val(target_x, target_y); /* possibly reads out of the boundary of the chessboard. not critical, as the following condition is always true for coordinates (target_x, target_y) out of the map. */

	if ((cb_or_dist_entry < 0) || (target_y < 0) || (target_y > 23) || (target_x < 0) || (target_x > 23)) {
		target_is_escape_square = 1;
	}

	dist_bak = dist;
	x_bak = target_x;
	y_bak = target_y;

	/* potential Original-Bug:
	 * best_dir is not initialized and may stay so in case that FIG_find_path_to_target_backtrack is called with equal target and hero/enemy position.
	 * See https://www.crystals-dsa-foren.de/showthread.php?tid=5383&pid=155007#pid155007
	 */
#ifdef M302de_ORIGINAL_BUGFIX
	best_dir = 0;
#endif

	/* the following appears to be a simplistic way to produce paths with few (but not necessarily the fewest possible number of) direction changes.
	 * the value i in the following loop is the "preferred" direction. The backtracking always tries this direction first. */
	for (i = 0; i < 4; i++) {

		dist = dist_bak;
		target_x = x_bak;
		target_y = y_bak;
		dir = i; /* start with the preferred direction */

		path_cur = path_table[i];
		dist_duplicate = dist;

		host_writeb(path_cur + dist_duplicate, -1);
		dist_duplicate--;
		dist--;

		while (dist >= 0) {

			success = 0;

			while (success == 0) {

				backtrack_y = target_y + inverse_coordinate_offset.o[dir].y;
				backtrack_x = target_x + inverse_coordinate_offset.o[dir].x;
				tail_y = backtrack_y + inverse_coordinate_offset.o[dir].y;
				tail_x = backtrack_x + inverse_coordinate_offset.o[dir].x;

				if ((backtrack_y < 24) && (backtrack_y >= 0) &&
					(backtrack_x < 24) && (backtrack_x >= 0))
				{
					cb_or_dist_entry = host_readbs(dist_table_ptr + (25 * backtrack_y) + backtrack_x);

					if ((cb_or_dist_entry == dist)	&&
						((!two_squares) || (
							/* Original-Bug
							 * A fight with two-squares enemies may freeze in an infinite loop here.
							 * The following check for space for the tail part of a two-squares monster is executed for every single backtracking step.
							 * However, in the function FIG_find_path_to_target this check is not applied for the last step to the target in certain circumstances.
							 * Fix: don't apply the check in the last step, i.e. the first step in the backtracking.
							 * See discussion at https://www.crystals-dsa-foren.de/showthread.php?tid=5191&pid=165957#pid165957
							 */
#ifdef M302de_ORIGINAL_BUGFIX
							dist == dist_bak-1 &&
#else
							two_squares &&
#endif
							((!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x)) || /* square is empty */
							(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x) == (fighter_id + 10)) || /* head of active enemy is on square */
							(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x) == (fighter_id + 30))) && /* tail of active enemy is on square */
							(tail_y < 24) && (tail_y >= 0) && (tail_x < 24) && (tail_x >= 0))))
					{
						target_y = backtrack_y;
						target_x = backtrack_x;

						if (bp_avail <= dist) {
						    	/* if the old square (target_x, target_y) (before updating to (backtrack_x, backtrack_y)) cannot be reached with the available BP, write -1 to the path */
							host_writeb(path_cur + dist_duplicate, -1);
							dist_duplicate--;
							target_out_of_reach = 1;
						} else {
						    	/* otherwise, write the found direction to the path */
							host_writebs(path_cur + dist_duplicate, (signed char)dir);
							dist_duplicate--;
						}

						dist--;
						success = 1;
					}
				}

				if (success == 0) {

					dir++;

					if  (dir == 4) {
						dir = 0;
					}
				}
			}
		}

		if ((bp_avail >= dist_bak) && (target_is_escape_square == 0) && (mode < 6))
		/* if mode < 6, i.e. the mode is melee attack or flee, and the target is an escape square which can be reached with the available BP.
		 * As the target square is always an escape square for the flee mode, this should be logically equivalent to (bp_avail >= dist_bak) && (mode < 4) */
		{
		    	/* remove the last step from the path (as for melee attack, the target should not be entered) */
#if !defined(__BORLANDC__)
			host_writeb(path_cur + dist_bak - 1, -1);
#else
			path_cur[dist_bak - 1] = -1;
#endif
		}

		if (host_readbs(path_cur) != -1) {

			nr_dir_changes = FIG_count_direction_changes((signed char*)path_cur);

			if (nr_dir_changes < lowest_nr_dir_changes) {

				best_dir = i;
				lowest_nr_dir_changes = nr_dir_changes;

				if (nr_dir_changes == 0) {
					break;
				}
			}
		}
	}

	memcpy(p_datseg + FIG_MOVE_PATHDIR, path_table[best_dir], 20);

	if (target_out_of_reach != 0) {

		i = 0;

		while (ds_readbs(FIG_MOVE_PATHDIR + i++) != -1);
		ds_writeb(FIG_MOVE_PATHDIR + i, -2);
		/* replace the first -1 entry in the path by -2 */
	}
}

//static
signed short FIG_count_direction_changes(signed char *path_ptr)
{

	signed short i = 0;
	signed short count = 0;

	if (path_ptr[0] == -1) {
		return 99;
	}

	i++;
	while (path_ptr[i] != -1) {
		if (path_ptr[i - 1] != path_ptr[i]) {
			count++;
		}

		i++;
	}

	return count;
}

/**
 * \brief  Search target for fighter and compute path. 
 *
 * Searches for a target (depending on given mode) for a fighter and computes a path to it. The computed path is written at FIG_MOVE_PATHDIR in the data segment.
 *
 * \param   fighter_ptr    pointer to a fighter (hero or enemy, depending on mode)
 * \param   fighter_id     id of fighter. hero: range 1..9 or; monster: range 10..29.
 * \param   x_in           x-coordinate of fighter
 * \param   y_in           y-coordinate of fighter
 * \param   mode           0: enemy to hero melee / 1: hero to hero melee / 2: enemy to enemy melee / 3: hero to enemy melee / 4: enemy is fleeing / 5: hero is fleeing / 6: enemy to hero ranged /  7: enemy to enemy ranged / 8: hero to hero ranged / 9: hero to enemy ranged / 10: hero movement (target labeled with 124 on the chess board)
 * \return                 1: reachable target found, path written / -1: no reachable target found.
 */
signed short FIG_find_path_to_target(Bit8u *fighter_ptr, signed short fighter_id, signed short x_in, signed short y_in, signed short mode)
{
	signed short nr_targets_reached;
	signed short i;
	signed short target_reached = 0;
	signed short dist = 0;
	signed short new_squares_reached;
	signed short new_x;
	signed short new_y;
	signed short x;
	signed short y;
	signed short tail_x;
	signed short tail_y;
	signed short dir;
	signed short nr_dir_changes;
	signed short lowest_nr_dir_changes;
	signed short best_target;
	signed char cb_or_dist_entry; /* used for both a chessboard entry and as a distance table entry */
	signed char cb_entry;
	Bit8u *dist_table_ptr;
	Bit8u *hero_ptr;
	Bit8u *enemy_ptr;
	signed short done;
	signed short ranged_dist;
	Bit8u *fighter_enemy_ptr; /* not needed, in principal. is only used for tests with NOT_NULL at a few places to determine wether the fighter is an enemy. could also be done based on mode. */
	signed char two_squares;
	signed short target_reached_x[10];
	signed short target_reached_y[10];
	signed short unused[10]; /* array gets only written, but never read */

	struct dummy coordinate_offset;

#if !defined(__BORLANDC__)
	for (int i = 0; i < 4; i++) {
		coordinate_offset.o[i].x = host_readws(p_datseg + VIEWDIR_OFFSETS7 + 4 * i);
		coordinate_offset.o[i].y = host_readws(p_datseg + VIEWDIR_OFFSETS7 + 4 * i + 2);
	}
#else
	coordinate_offset = *((struct dummy*)(p_datseg + VIEWDIR_OFFSETS7));
#endif

	fighter_enemy_ptr = NULL;
	two_squares = 0;

	if ((mode == 0) || (mode == 2) || (mode == 4) || (mode == 6) || (mode == 7)) /* fighter is an enemy */
	{
		fighter_enemy_ptr = fighter_ptr;
		if (is_in_byte_array(host_readbs(fighter_enemy_ptr + 1), p_datseg + TWO_FIELDED_SPRITE_ID))
		{
			two_squares = 1;
		}
	}

	dist_table_ptr = Real2Host(ds_readd(DTP2));
	ds_writed(CHESSBOARD_CPY, (Bit32u)((RealPt)ds_readd(DTP2) + 600));
	new_squares_reached = 1;
	memset(dist_table_ptr, -1, 600);
	host_writeb(dist_table_ptr + (y_in * 25) + x_in, 0);
	memcpy(Real2Host(ds_readd(CHESSBOARD_CPY)), Real2Host(ds_readd(CHESSBOARD)), 600);

	for (i = 0; i < 10; i++) {
		target_reached_x[i] = target_reached_y[i] = unused[i] = 0;
	}

	/* make squares with dead heros/enemies walkable */
	for (y = 0; y < 24; y++) {
		for (x = 0; x < 24; x++) {

			cb_or_dist_entry = host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (y * 25) + x);

			if (cb_or_dist_entry > 0) {
				if ((cb_or_dist_entry < 10) && (hero_dead(get_hero(cb_or_dist_entry - 1)) || hero_uncon(get_hero(cb_or_dist_entry - 1))))
				{
					host_writeb(Real2Host(ds_readd(CHESSBOARD_CPY)) + (y * 25) + x, 0);
				} else if ((cb_or_dist_entry >= 10) && (cb_or_dist_entry < 30) && (test_bit0(p_datseg + ((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_STATUS1) + cb_or_dist_entry * SIZEOF_ENEMY_SHEET)))
				{
						host_writeb(Real2Host(ds_readd(CHESSBOARD_CPY)) + (y * 25) + x, 0);
				}
			}
		}
	}

	if ((mode == 6) || (mode == 8)) { /* ranged attack to some hero. mark possible base squares for launching the attack. */

		for (i = 0; i <= 6; i++) { /* loop over all heros */
			if ((mode != 8) || (i != fighter_id)) { /* hero should not attack himself */

				hero_ptr = get_hero(i);

				if ((host_readbs(hero_ptr + HERO_TYPE) != HERO_TYPE_NONE) &&
					(host_readbs(hero_ptr + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
					!hero_dead(hero_ptr))
				{ /* hero_ptr points to an actual alive hero in the current group */
					FIG_search_obj_on_cb(i + 1, &x, &y);

#if !defined(__BORLANDC__)
					/* BE-fix */
					x = host_readws((Bit8u*)&x);
					y = host_readws((Bit8u*)&y);
#endif
					for (dir = 0; dir < 4; dir++) {

						done = 0;
						ranged_dist = 1;

						while (done == 0) {

							new_y = y + ranged_dist * coordinate_offset.o[dir].y;
							new_x = x + ranged_dist * coordinate_offset.o[dir].x;


							if ((new_y < 0) || (new_y > 23) || (new_x < 0) ||
								/* Original-Bug: */
#ifdef M302de_ORIGINAL_BUGFIX
									(new_x > 23)
#else
									(new_y > 23)
#endif
								)
							{
								/* search passed the border of the map */
								done = 1;
							} else {
								if (!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (25 * new_y) + new_x))
								{
									/* sqare empty and may be used as base square for launching the ranged attack. set target marker */
									host_writeb(Real2Host(ds_readd(CHESSBOARD_CPY)) + (25 * new_y) + new_x, 9);
								} else {
									/* square blocked */
									done = 1;
								}
							}
							ranged_dist++;
						}
					}
				}
			}
		}
	}

	if ((mode == 7) || (mode == 9)) { /* ranged attack to some enemy. mark possible base squares for launching the attack */

		for (i = 0; i < 20; i++) { /* loop over all enemies */

			if ((mode != 7) || (i != fighter_id)) { /* enemy shoud not attack himself */

				enemy_ptr = p_datseg + ENEMY_SHEETS + i * SIZEOF_ENEMY_SHEET;

				if ((host_readbs(enemy_ptr + ENEMY_SHEET_MON_ID) != 0) && !enemy_dead(enemy_ptr)) {

					FIG_search_obj_on_cb(i + 10, &x, &y);

#if !defined(__BORLANDC__)
					/* BE-fix */
					x = host_readws((Bit8u*)&x);
					y = host_readws((Bit8u*)&y);
#endif
					for (dir = 0; dir < 4; dir++) {
						done = 0;
						ranged_dist = 1;

						while (done == 0) {
							new_y = y + ranged_dist * coordinate_offset.o[dir].y;
							new_x = x + ranged_dist * coordinate_offset.o[dir].x;

							if ((new_y < 0) || (new_y > 23) || (new_x < 0) ||
								/* Original-Bug: */
#ifdef M302de_ORIGINAL_BUGFIX
									(new_x > 23)
#else
									(new_y > 23)
#endif
								)
							{
								/* search passed the border of the map */
								done =1;
							} else {
								if (!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (25 * new_y) + new_x))
								{
									/* sqare empty and may be used as base square for launching the ranged attack. set target marker */
									host_writeb(Real2Host(ds_readd(CHESSBOARD_CPY)) + (25 * new_y) + new_x, 49);
								} else {
									/* square blocked */
									done = 1;
								}
							}

							ranged_dist++;
						}
					}
				}
			}
		}
	}

	/* breadth-first search to fill the distance table 
	 * search stops if for the last distance, a target was found, no new reachable squares have been found, or if the new distance will be as large as 50.
	 * Moreover, the search will stop with a break if for the current distance, 10 targets have been found. */
	while ( (target_reached == 0) && (new_squares_reached != 0) && (dist < 50)) {
		new_squares_reached = 0;
		dist++;
		nr_targets_reached = 0;

		for (y = 0; y < 24; y++) {

			if (nr_targets_reached == 10) {
				break;
			}

			for (x = 0; x < 24; x++) {

				if (nr_targets_reached == 10) {
					break;
				}

				if (host_readbs(dist_table_ptr + (y * 25) + x) == (dist - 1)) {

					for (i = 0; i < 4; i++) {

						new_y = y + coordinate_offset.o[i].y;
						new_x = x + coordinate_offset.o[i].x;
						tail_y = y - coordinate_offset.o[i].y; /* for movement of two-squares fighters */
						tail_x = x - coordinate_offset.o[i].x; /* for movement of two-squares fighters */

						if ((new_y < 24) && (new_y >= 0) && (new_x < 24) && (new_x >= 0)) {

							cb_or_dist_entry = host_readbs(dist_table_ptr + (new_y * 25) + new_x);
							cb_entry = host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (new_y * 25) + new_x);

							if (cb_or_dist_entry < 0) { /* square has not been reached before */

								if (cb_entry == 0) { /* square is empty */

									if (!NOT_NULL(fighter_enemy_ptr) || 
										(NOT_NULL(fighter_enemy_ptr) && (!two_squares ||
													((two_squares != 0) &&
														((!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x)) || /* square is empty */
														(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x))  == (fighter_id + 10)|| /* head of active enemy is on square */
														(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x)) == (fighter_id + 30)) && /* tail of active enemy is on square */
														((tail_y < 24) && (tail_y >= 0) && (tail_x < 24) && (tail_x >= 0))))))
									{
										host_writebs(dist_table_ptr + (new_y * 25) + new_x, (signed char)dist);
										new_squares_reached = 1;
									}
								} else {
									if (cb_entry < 0) { /* escape square */

										if ((mode == 4) || (mode == 5)) { /* fighter is fleeing */
											unused[nr_targets_reached] = 1;
											target_reached_x[nr_targets_reached] = new_x;
											target_reached_y[nr_targets_reached] = new_y;
											nr_targets_reached++;
											if (nr_targets_reached == 10) {
												break;
											}
										} else {
											host_writeb(dist_table_ptr + (new_y * 25) + new_x, 100); /* for all other modes: square is blocked */
										}
									} else if (cb_entry == 124) { /* target marker for hero movement (implies mode == 10 (hero movement)) */
										unused[nr_targets_reached] = 1;
										target_reached_x[nr_targets_reached] = new_x;
										target_reached_y[nr_targets_reached] = new_y;
										nr_targets_reached++;
										if (nr_targets_reached == 10) {
											break;
										}
									} else if (cb_entry >= 50) { /* square is blocked */
										host_writeb(dist_table_ptr + (new_y * 25) + new_x, 100);
									} else if (cb_entry < 10) {

										if (cb_entry == 9) { /* target marker for ranged attack to some hero (implies mode == 6 or mode == 8) */
											unused[nr_targets_reached] = 1;
											target_reached_x[nr_targets_reached] = new_x;
											target_reached_y[nr_targets_reached] = new_y;
											nr_targets_reached++;
											if (nr_targets_reached == 10) {
												break;
											}
										} else { /* some hero on square */
											if (((mode == 0) || (mode == 1)) /* melee attack */
												&& (!NOT_NULL(fighter_enemy_ptr) || (NOT_NULL(fighter_enemy_ptr) && (!two_squares ||
															((two_squares != 0) &&
																((!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x)) ||
																(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x))  == (fighter_id + 10)||
																(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x)) == (fighter_id + 30)) &&
																((tail_y < 24) && (tail_y >= 0) && (tail_x < 24) && (tail_x >= 0)))))))
											{
												unused[nr_targets_reached] = 1;
												target_reached_x[nr_targets_reached] = new_x;
												target_reached_y[nr_targets_reached] = new_y;
												nr_targets_reached++;
												if (nr_targets_reached == 10) {
													break;
												}
											} /* if a two-squares monster cannot attack, because there is no space for the tail, the entry in dist_table stays at -1.
											     In this way, later the hero may be considered again as a target from a different direction. */
										}
									} else if (cb_entry < 50) {
										if (cb_entry == 49) { /* target marker for ranged attack to enemy (implies mode == 7 or mode == 9) */
											unused[nr_targets_reached] = 1;
											target_reached_x[nr_targets_reached] = new_x;
											target_reached_y[nr_targets_reached] = new_y;
											nr_targets_reached++;
											if (nr_targets_reached == 10) {
												break;
											}
										} else { /* enemy on square */
											if (((mode == 2) || (mode == 3)) && /* melee attack */
												(cb_entry < 30) && (!NOT_NULL(fighter_enemy_ptr) || (NOT_NULL(fighter_enemy_ptr) && (!two_squares ||
															((two_squares != 0) &&
																((!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x)) ||
																(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x))  == (fighter_id + 10)||
																(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (tail_y * 25) + tail_x)) == (fighter_id + 30)) &&
																((tail_y < 24) && (tail_y >= 0) && (tail_x < 24) && (tail_x >= 0)))))))
											{
												unused[nr_targets_reached] = 1;
												target_reached_x[nr_targets_reached] = new_x;
												target_reached_y[nr_targets_reached] = new_y;
												nr_targets_reached++;
												if (nr_targets_reached == 10) {
													break;
												}
											} /* if a two-squares monster cannot attack, because there is no space for the tail, the entry in dist_table stays at -1.
											     In this way, later the enemy may be considered again as a target from a different direction. */

										}
									}
								}
							}
						} else {
							if (((mode == 4) || (mode == 5)) && /* fighter is fleeing */
								((host_readbs(Real2Host(ds_readd(SCENARIO_BUF)) + 0x14) > 3) ||
									((host_readbs(Real2Host(ds_readd(SCENARIO_BUF)) + 0x14) <= 3) && ((new_x > 23) || (new_y > 23) || (new_y < 0)))))
							{
								unused[nr_targets_reached] = 1;
								target_reached_x[nr_targets_reached] = new_x;
								target_reached_y[nr_targets_reached] = new_y;
								nr_targets_reached++;
								if (nr_targets_reached == 10) {
									break;
								}
							}
						}
					}
				}
			}

		}

		if (nr_targets_reached) {
			target_reached = 1;
			best_target = 0;
			lowest_nr_dir_changes = 99;

			/* find target whose path has the lowest number of direction changes */
			for (i = 0; i < nr_targets_reached; i++) {

				if ((mode == 0) || (mode == 2) || (mode == 4) || (mode == 6) || (mode == 7)) { /* fighter is an enemy */
					FIG_find_path_to_target_backtrack(dist_table_ptr, target_reached_x[i], target_reached_y[i], dist, host_readbs(fighter_ptr + ENEMY_SHEET_BP), mode, two_squares, fighter_id);
				} else { /* fighter is a hero */
					FIG_find_path_to_target_backtrack(dist_table_ptr, target_reached_x[i], target_reached_y[i], dist, host_readbs(fighter_ptr + HERO_BP_LEFT), mode, two_squares, fighter_id);
				}

				nr_dir_changes = FIG_count_direction_changes((signed char*)p_datseg + FIG_MOVE_PATHDIR);

				if ((nr_dir_changes == 0)) {
					best_target = i;
					break;
				}

				if (nr_dir_changes < lowest_nr_dir_changes) {
					lowest_nr_dir_changes = nr_dir_changes;
					best_target = i;
				}
			}

			if ((mode == 0) || (mode == 2) || (mode == 4) || (mode == 6) || (mode == 7)) { /* fighter is an enemy */
				FIG_find_path_to_target_backtrack(dist_table_ptr, target_reached_x[best_target], target_reached_y[best_target], dist, host_readbs(fighter_ptr + ENEMY_SHEET_BP), mode, two_squares, fighter_id);
			} else { /* fighter is a hero */
				FIG_find_path_to_target_backtrack(dist_table_ptr, target_reached_x[best_target], target_reached_y[best_target], dist, host_readbs(fighter_ptr + HERO_BP_LEFT), mode, two_squares, fighter_id);
			}
		}
	}


	if (target_reached != 0) {
		return 1;
	} else {
		return -1;
	}
}

#if !defined(__BORLANDC__)
}
#endif
