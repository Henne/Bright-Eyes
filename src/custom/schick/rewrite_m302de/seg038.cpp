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

struct vec2 {
	signed short x;
	signed short y;
};

struct dummy {
	struct vec2 o[4];
};

void FIG_backtrack(Bit8u *in_ptr, signed short target_x, signed short target_y,
			signed short bp_needed, signed char bp_avail,
			signed short arg6, signed short arg7, signed short arg8)
{
	signed short i;
	signed short bp_left;
	signed short lvar2;
	signed short lvar3;
	signed short obj_id;
	signed short bp_bak;
	signed short out_of_field = 0;
	signed short lvar7;
	signed short lvar8;
	signed short dir;
	signed short done;
	signed short min;
	signed short lvar12;
	signed short found_dir;
	Bit8u *ptr_cur;
	signed short x_bak;
	signed short y_bak;
	signed short found;
	struct dummy dst;
	Bit8u *ptr[4];
#if !defined(__BORLANDC__)
	for (int i = 0; i < 4; i++) {
		dst.o[i].x = host_readws(p_datseg + VIEWDIR_INVOFFSETS1 + 4 * i);
		dst.o[i].y = host_readws(p_datseg + VIEWDIR_INVOFFSETS1 + 4 * i + 2);
	}
#else
	dst = *((struct dummy*)(p_datseg + VIEWDIR_INVOFFSETS1));
#endif

	found = 0;
	min = 99;

	/* potential Original-Bug:
	 * found_dir is not initialized and may stay so in case that FIG_backtrack is called with equal target and hero/enemy position.
	 * See https://www.crystals-dsa-foren.de/showthread.php?tid=5383&pid=155007#pid155007
	 */
#ifdef M302de_ORIGINAL_BUGFIX
	found_dir = 0;
#endif

	memset(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), 0, 80);
	ptr[0] = Real2Host((RealPt)ds_readd(TEXT_OUTPUT_BUF));
	ptr[1] = Real2Host((RealPt)ds_readd(TEXT_OUTPUT_BUF)) + 20;
	ptr[2] = Real2Host((RealPt)ds_readd(TEXT_OUTPUT_BUF)) + 40;
	ptr[3] = Real2Host((RealPt)ds_readd(TEXT_OUTPUT_BUF)) + 60;

	obj_id = get_cb_val(target_x, target_y);

	if ((obj_id < 0) || (target_y < 0) || (target_y > 23) || (target_x < 0) || (target_x > 23)) {
		out_of_field = 1;
	}

	bp_bak = bp_needed;
	x_bak = target_x;
	y_bak = target_y;

	/* for each direction */
	for (i = 0; i < 4; i++) {

		bp_needed = bp_bak;
		target_x = x_bak;
		target_y = y_bak;
		dir = i;

		ptr_cur = ptr[i];
		bp_left = bp_needed;

		host_writeb(ptr_cur + bp_left, -1);
		bp_left--;
		bp_needed--;

		while (bp_needed >= 0) {

			done = 0;

			while (done == 0) {

				lvar3 = target_y + dst.o[dir].y;
				lvar2 = target_x + dst.o[dir].x;
				lvar8 = lvar3 + dst.o[dir].y;
				lvar7 = lvar2 + dst.o[dir].x;

				if ((lvar3 < 24) && (lvar3 >= 0) &&
					(lvar2 < 24) && (lvar2 >= 0))
				{
					obj_id = host_readbs(in_ptr + (25 * lvar3) + lvar2);

					if ((obj_id == bp_needed)	&&
						((!arg7 ) ||
							/* Original-Bug
							 * A fight with two-fielded enemies may freeze in an infinite loop here.
							 * The following check for space for the back part of a two-fielded monster is executed for every single backtracking step.
							 * However, in the function seg038 this check is not applied for the last step to the target in certain circumstances.
							 * Fix: don't apply the check in the last step, i.e. the first step in the backtracking.
							 * See discussion at https://www.crystals-dsa-foren.de/showthread.php?tid=5191&pid=165957#pid165957
							 */
#ifdef M302de_ORIGINAL_BUGFIX
						(arg7 &&
#else
						(bp_needed == bp_bak-1 &&
#endif
							((!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (lvar8 * 25) + lvar7)) ||
							(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (lvar8 * 25) + lvar7) == (arg8 + 10)) ||
							(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (lvar8 * 25) + lvar7) == (arg8 + 30))) &&
							(lvar8 < 24) && (lvar8 >= 0) && (lvar7 < 24) && (lvar7 >= 0))))
					{
						target_y = lvar3;
						target_x = lvar2;

						if (bp_avail <= bp_needed) {
							host_writeb(ptr_cur + bp_left, -1);
							bp_left--;
							found = 1;
						} else {
							host_writebs(ptr_cur + bp_left, (signed char)dir);
							bp_left--;
						}

						bp_needed--;
						done = 1;
					}
				}

				if (done == 0) {

					dir++;

					if  (dir == 4) {
						dir = 0;
					}
				}
			}
		}

		if ((bp_avail >= bp_bak) && (out_of_field == 0) && (arg6 < 6))
		{
#if !defined(__BORLANDC__)
			host_writeb(ptr_cur + bp_bak - 1, -1);
#else
			ptr_cur[bp_bak - 1] = -1;
#endif
		}

		if (host_readbs(ptr_cur) != -1) {

			lvar12 = FIG_count_smth((signed char*)ptr_cur);

			if (lvar12 < min) {

				found_dir = i;
				min = lvar12;

				if (lvar12 == 0) {
					break;
				}
			}
		}
	}

	memcpy(p_datseg + FIG_MOVE_PATHDIR, ptr[found_dir], 20);

	if (found != 0) {

		i = 0;

		while (ds_readbs(FIG_MOVE_PATHDIR + i++) != -1);
		ds_writeb(FIG_MOVE_PATHDIR + i, -2);
	}
}

//static
signed short FIG_count_smth(signed char *p)
{

	signed short i = 0;
	signed short count = 0;

	if (p[0] == -1) {
		return 99;
	}

	i++;
	while (p[i] != -1) {
		if (p[i - 1] != p[i]) {
			count++;
		}

		i++;
	}

	return count;
}

/**
 * \brief   TODO
 *
 * \param   in_ptr      pointer to hero or monster, depends on value of a4
 * \param   a1          no of hero or monster
 * \param   x_in        x-coordinate
 * \param   y_in        y-coordinate
 * \param   a4          fight_action
 * \return              {-1,1}
 */
signed short seg038(Bit8u *in_ptr, signed short a1, signed short x_in, signed short y_in, signed short a4)
{
	signed short l_si;
	signed short i;
	signed short l_var1 = 0;
	signed short l_var2 = 0;
	signed short l_var3;
	signed short l_var4;
	signed short l_var5;
	signed short l_var6;
	signed short l_var7;
	signed short l_var8;
	signed short l_var9;
	signed short l_var10;
	signed short l_var11;
	signed short l_var12;
	signed short l_var13;
	signed char obj_id;
	signed char l_var15;
	Bit8u *ptr2;
	Bit8u *hero_ptr;
	Bit8u *enemy_ptr;
	signed short l_var17;
	signed short l_var18;
	Bit8u *ptr3;
	signed char two_fields;
	signed short arr1[10];
	signed short arr2[10];
	signed short arr3[10];

	struct dummy dst;

#if !defined(__BORLANDC__)
	for (int i = 0; i < 4; i++) {
		dst.o[i].x = host_readws(p_datseg + VIEWDIR_OFFSETS7 + 4 * i);
		dst.o[i].y = host_readws(p_datseg + VIEWDIR_OFFSETS7 + 4 * i + 2);
	}
#else
	dst = *((struct dummy*)(p_datseg + VIEWDIR_OFFSETS7));
#endif

	ptr3 = NULL;
	two_fields = 0;

	if ((a4 == 0) || (a4 == 2) || (a4 == 4) || (a4 == 6) || (a4 == 7))
	{
		ptr3 = in_ptr;
		if (is_in_byte_array(host_readbs(ptr3 + 1), p_datseg + TWO_FIELDED_SPRITE_ID))
		{
			two_fields = 1;
		}
	}

	ptr2 = Real2Host(ds_readd(DTP2));
	ds_writed(CHESSBOARD_CPY, (Bit32u)((RealPt)ds_readd(DTP2) + 600));
	l_var3 = 1;
	memset(ptr2, -1, 600);
	host_writeb(ptr2 + (y_in * 25) + x_in, 0);
	memcpy(Real2Host(ds_readd(CHESSBOARD_CPY)), Real2Host(ds_readd(CHESSBOARD)), 600);

	for (i = 0; i < 10; i++) {
		arr1[i] = arr2[i] = arr3[i] = 0;
	}

	/* make the field where dead heros/enemies lie walkable */
	for (l_var7 = 0; l_var7 < 24; l_var7++) {
		for (l_var6 = 0; l_var6 < 24; l_var6++) {

			obj_id = host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var7 * 25) + l_var6);

			if (obj_id > 0) {
				if ((obj_id < 10) && (hero_dead(get_hero(obj_id - 1)) || hero_uncon(get_hero(obj_id - 1))))
				{
					host_writeb(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var7 * 25) + l_var6, 0);
				} else if ((obj_id >= 10) && (obj_id < 30) && (test_bit0(p_datseg + ((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + 49) + obj_id * SIZEOF_ENEMY_SHEET)))
				{
						host_writeb(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var7 * 25) + l_var6, 0);
				}
			}
		}
	}

	if ((a4 == 6) || (a4 == 8)) {

		for (i = 0; i <= 6; i++) {
			if ((a4 != 8) || (i != a1)) {

				hero_ptr = get_hero(i);

				if ((host_readbs(hero_ptr + HERO_TYPE) != HERO_TYPE_NONE) &&
					(host_readbs(hero_ptr + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
					!hero_dead(hero_ptr))
				{
					FIG_search_obj_on_cb(i + 1, &l_var6, &l_var7);

#if !defined(__BORLANDC__)
					/* BE-fix */
					l_var6 = host_readws((Bit8u*)&l_var6);
					l_var7 = host_readws((Bit8u*)&l_var7);
#endif
					for (l_var10 = 0; l_var10 < 4; l_var10++) {

						l_var17 = 0;
						l_var18 = 1;

						while (l_var17 == 0) {

							l_var5 = l_var7 + l_var18 * dst.o[l_var10].y;
							l_var4 = l_var6 + l_var18 * dst.o[l_var10].x;


							if ((l_var5 < 0) || (l_var5 > 23) ||
								/* Original-Bug: */
								 (l_var4 < 0) || (l_var5 > 23))
							{
								l_var17 = 1;
							} else {
								if (!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (25 * l_var5) + l_var4))
								{
									host_writeb(Real2Host(ds_readd(CHESSBOARD_CPY)) + (25 * l_var5) + l_var4, 9);
								} else {
									l_var17 = 1;
								}
							}
							l_var18++;
						}
					}
				}
			}
		}
	}

	if ((a4 == 7) || (a4 == 9)) {

		for (i = 0; i < 20; i++) {

			if ((a4 != 7) || (i != a1)) {

				enemy_ptr = p_datseg + ENEMY_SHEETS + i * SIZEOF_ENEMY_SHEET;

				if ((host_readbs(enemy_ptr + ENEMY_SHEET_MON_ID) != 0) && !enemy_dead(enemy_ptr)) {

					FIG_search_obj_on_cb(i + 10, &l_var6, &l_var7);

#if !defined(__BORLANDC__)
					/* BE-fix */
					l_var6 = host_readws((Bit8u*)&l_var6);
					l_var7 = host_readws((Bit8u*)&l_var7);
#endif
					for (l_var10 = 0; l_var10 < 4; l_var10++) {
						l_var17 = 0;
						l_var18 = 1;

						while (l_var17 == 0) {
							l_var5 = l_var7 + l_var18 * dst.o[l_var10].y;
							l_var4 = l_var6 + l_var18 * dst.o[l_var10].x;


							if ((l_var5 < 0) || (l_var5 > 23) ||
								/* Original-Bug: */
								 (l_var4 < 0) || (l_var5 > 23))
							{
								l_var17 =1;
							} else {
								if (!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (25 * l_var5) + l_var4))
								{
									host_writeb(Real2Host(ds_readd(CHESSBOARD_CPY)) + (25 * l_var5) + l_var4, 49);
								} else {
									l_var17 = 1;
								}
							}

							l_var18++;
						}
					}
				}
			}
		}
	}

	while ( (l_var1 == 0) && (l_var3 != 0) && (l_var2 < 50)) {
		l_var3 = 0;
		l_var2++;
		l_si = 0;

		for (l_var7 = 0; l_var7 < 24; l_var7++) {

			if (l_si == 10) {
				break;
			}

			for (l_var6 = 0; l_var6 < 24; l_var6++) {

				if (l_si == 10) {
					break;
				}

				if (host_readbs(ptr2 + (l_var7 * 25) + l_var6) == (l_var2 - 1)) {

					for (i = 0; i < 4; i++) {

						l_var5 = l_var7 + dst.o[i].y;
						l_var4 = l_var6 + dst.o[i].x;
						l_var9 = l_var7 - dst.o[i].y;
						l_var8 = l_var6 - dst.o[i].x;

						if ((l_var5 < 24) && (l_var5 >= 0) && (l_var4 < 24) && (l_var4 >= 0)) {

							obj_id = host_readbs(ptr2 + (l_var5 * 25) + l_var4);
							l_var15 = host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var5 * 25) + l_var4);

							if (obj_id < 0) {

								if (l_var15 == 0) {

									if (!NOT_NULL(ptr3) ||
										(NOT_NULL(ptr3) && (!two_fields ||
													((two_fields != 0) &&
														((!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var9 * 25) + l_var8)) ||
														(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var9 * 25) + l_var8))  == (a1 + 10)||
														(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var9 * 25) + l_var8)) == (a1 + 30)) &&
														((l_var9 < 24) && (l_var9 >= 0) && (l_var8 < 24) && (l_var8 >= 0))))))
									{
										host_writebs(ptr2 + (l_var5 * 25) + l_var4, (signed char)l_var2);
										l_var3 = 1;
									}
								} else {
									if (l_var15 < 0) {

										if ((a4 == 4) || (a4 == 5)) {
											arr3[l_si] = 1;
											arr1[l_si] = l_var4;
											arr2[l_si] = l_var5;
											l_si++;
											if (l_si == 10) {
												break;
											}
										} else {
											host_writeb(ptr2 + (l_var5 * 25) + l_var4, 100);
										}
									} else if (l_var15 == 124) {
										arr3[l_si] = 1;
										arr1[l_si] = l_var4;
										arr2[l_si] = l_var5;
										l_si++;
										if (l_si == 10) {
											break;
										}
									} else if (l_var15 >= 50) {
										host_writeb(ptr2 + (l_var5 * 25) + l_var4, 100);
									} else if (l_var15 < 10) {

										if (l_var15 == 9) {
											arr3[l_si] = 1;
											arr1[l_si] = l_var4;
											arr2[l_si] = l_var5;
											l_si++;
											if (l_si == 10) {
												break;
											}
										} else {
											if (((a4 == 0) || (a4 == 1)) && (!NOT_NULL(ptr3) ||
												(NOT_NULL(ptr3) && (!two_fields ||
															((two_fields != 0) &&
																((!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var9 * 25) + l_var8)) ||
																(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var9 * 25) + l_var8))  == (a1 + 10)||
																(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var9 * 25) + l_var8)) == (a1 + 30)) &&
																((l_var9 < 24) && (l_var9 >= 0) && (l_var8 < 24) && (l_var8 >= 0)))))))
											{
												arr3[l_si] = 1;
												arr1[l_si] = l_var4;
												arr2[l_si] = l_var5;
												l_si++;
												if (l_si == 10) {
													break;
												}
											}
										}
									} else if (l_var15 < 50) {
										if (l_var15 == 49) {
											arr3[l_si] = 1;
											arr1[l_si] = l_var4;
											arr2[l_si] = l_var5;
											l_si++;
											if (l_si == 10) {
												break;
											}
										} else {
											if (((a4 == 2) || (a4 == 3)) && (l_var15 < 30) && (!NOT_NULL(ptr3) ||
												(NOT_NULL(ptr3) && (!two_fields ||
															((two_fields != 0) &&
																((!host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var9 * 25) + l_var8)) ||
																(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var9 * 25) + l_var8))  == (a1 + 10)||
																(host_readbs(Real2Host(ds_readd(CHESSBOARD_CPY)) + (l_var9 * 25) + l_var8)) == (a1 + 30)) &&
																((l_var9 < 24) && (l_var9 >= 0) && (l_var8 < 24) && (l_var8 >= 0)))))))
											{
												arr3[l_si] = 1;
												arr1[l_si] = l_var4;
												arr2[l_si] = l_var5;
												l_si++;
												if (l_si == 10) {
													break;
												}
											}
										}
									}
								}
							}
						} else {
							if (((a4 == 4) || (a4 == 5)) &&
								((host_readbs(Real2Host(ds_readd(SCENARIO_BUF)) + 0x14) > 3) ||
									((host_readbs(Real2Host(ds_readd(SCENARIO_BUF)) + 0x14) <= 3) && ((l_var4 > 23) || (l_var5 > 23) || (l_var5 < 0)))))
							{
								arr3[l_si] = 1;
								arr1[l_si] = l_var4;
								arr2[l_si] = l_var5;
								l_si++;
								if (l_si == 10) {
									break;
								}
							}
						}
					}
				}
			}

		}

		if (l_si) {
			l_var1 = 1;
			l_var13 = 0;
			l_var12 = 99;

			for (i = 0; i < l_si; i++) {

				if ((a4 == 0) || (a4 == 2) || (a4 == 4) || (a4 == 6) || (a4 == 7)) {
					FIG_backtrack(ptr2, arr1[i], arr2[i], l_var2, host_readbs(in_ptr + ENEMY_SHEET_BP), a4, two_fields, a1);
				} else {
					FIG_backtrack(ptr2, arr1[i], arr2[i], l_var2, host_readbs(in_ptr + HERO_BP_LEFT), a4, two_fields, a1);
				}

				l_var11 = FIG_count_smth((signed char*)p_datseg + FIG_MOVE_PATHDIR);

				if ((l_var11 == 0)) {
					l_var13 = i;
					break;
				}

				if (l_var11 < l_var12) {
					l_var12 = l_var11;
					l_var13 = i;
				}
			}

			if ((a4 == 0) || (a4 == 2) || (a4 == 4) || (a4 == 6) || (a4 == 7)) {
				FIG_backtrack(ptr2, arr1[l_var13], arr2[l_var13], l_var2, host_readbs(in_ptr + ENEMY_SHEET_BP), a4, two_fields, a1);
			} else {
				FIG_backtrack(ptr2, arr1[l_var13], arr2[l_var13], l_var2, host_readbs(in_ptr + HERO_BP_LEFT), a4, two_fields, a1);
			}
		}
	}


	if (l_var1 != 0) {
		return 1;
	} else {
		return -1;
	}
}

#if !defined(__BORLANDC__)
}
#endif
