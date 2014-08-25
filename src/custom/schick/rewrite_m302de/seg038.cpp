/*
 *      Rewrite of DSA1 v3.02_de functions of seg038 (Fight)
 *      Functions rewritten 6/6
 *
 *      Functions called rewritten 5/5
 *      Functions uncalled rewritten 1/1
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
 * FIG_search_obj_on_cb - searches an object on the chessboard
 * @obj:	number of the object
 * @px:		pointer where x should be stored
 * @py:		pointer where y should be stored
 *
 * Returns 0 if the object was not found. If the object was
 * found it returns 1 and stores the coordinates at the pointers.
 */
/* Borlandified and identical */
unsigned short FIG_search_obj_on_cb(signed short obj, signed short *px, signed short *py)
{


	for (*px = 0; *px < 25; ++*px) {
		for (*py = 0; *py < 24; ++*py) {
			if (get_cb_val(*px, *py) == obj) {
				return 1;
			}
		}
	}

	return 0;
}

/* Borlandified and identical */
void FIG_init_list_elem(signed short obj)
{

	signed short x, y;

	FIG_search_obj_on_cb(obj, &x, &y);

	/* This initializes a 35 byte structure at DS:0x3066 */
	ds_writew(0xe066, 0);
	ds_writeb(0xe068, 0);
	ds_writeb(0xe069, (signed char)x);
	ds_writeb(0xe06a, (signed char)y);
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

	ds_writed(0xe07d, ds_readd(0xd862));
	ds_writeb(0xe077, 0);
	ds_writeb(0xe078, 1);
	ds_writeb(0xe079, -1);

	ds_writeb(0xe38e, FIG_add_to_list(-1));
}

/* Borlandified and identical */
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

/* Borlandified and identical */
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
	struct_copy((Bit8u*)&dst, p_datseg + 0x5ff8, 16);
#else
	dst = *((struct dummy*)(p_datseg + 0x5ff8));
#endif

	found = 0;
	min = 99;

	memset(Real2Host(ds_readd(0xd2eb)), 0, 80);
	ptr[0] = Real2Host((RealPt)ds_readd(0xd2eb));
	ptr[1] = Real2Host((RealPt)ds_readd(0xd2eb)) + 20;
	ptr[2] = Real2Host((RealPt)ds_readd(0xd2eb)) + 40;
	ptr[3] = Real2Host((RealPt)ds_readd(0xd2eb)) + 60;

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
						(arg7 &&
							((!host_readbs(Real2Host(ds_readd(0xe356)) + (lvar8 * 25) + lvar7)) ||
							(host_readbs(Real2Host(ds_readd(0xe356)) + (lvar8 * 25) + lvar7) == (arg8 + 10)) ||
							(host_readbs(Real2Host(ds_readd(0xe356)) + (lvar8 * 25) + lvar7) == (arg8 + 30))) &&
							(lvar8 < 24) && (lvar8 >= 0) && (lvar7 < 24) && (lvar7 >= 0))))
					{
						target_y = lvar3;
						target_x = lvar2;

						if (bp_avail <= bp_needed) {
							host_writeb(ptr_cur + bp_left, -1);
							bp_left--;
							found = 1;
						} else {
							host_writeb(ptr_cur + bp_left, dir);
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

	memcpy(p_datseg + 0xd823, ptr[found_dir], 20);

	if (found != 0) {

		i = 0;

		while (ds_readbs(0xd823 + i++) != -1);
		ds_writeb(0xd823 + i, -2);
	}
}

//static
/* Borlandified and identical */
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

signed short seg038(Bit8u *in_ptr, signed short a1, signed short x_in, signed short y_in, signed char a4)
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
	struct_copy((Bit8u*)&dst, p_datseg + 0x6008, 16);
#else
	dst = *((struct dummy*)(p_datseg + 0x6008));
#endif

	ptr3 = NULL;
	two_fields = 0;

	if ((a4 == 0) || (a4 == 2) || (a4 == 4) || (a4 == 6) || (a4 == 7))
	{
		ptr3 = in_ptr;
		if (is_in_byte_array(host_readbs(ptr3 + 1), p_datseg + 0x25f9))
		{
			two_fields = 1;
		}
	}

	ptr2 = Real2Host(ds_readd(DTP2));
	ds_writed(0xe356, (Bit32u)((RealPt)ds_readd(DTP2) + 600));
	l_var3 = 1;
	memset(ptr2, -1, 600);
	host_writeb(ptr2 + (y_in * 25) + x_in, 0);
	memcpy(Real2Host(ds_readd(0xe356)), Real2Host(ds_readd(CHESSBOARD)), 600);

	for (i = 0; i < 10; i++) {
		arr1[i] = arr2[i] = arr3[i] = 0;
	}

	/* make the field where dead heros/enemies lie walkable */
	for (l_var7 = 0; l_var7 < 24; l_var7++) {
		for (l_var6 = 0; l_var6 < 24; l_var6++) {

			obj_id = host_readbs(Real2Host(ds_readd(0xe356)) + (l_var7 * 25) + l_var6);

			if (obj_id > 0) {
				if ((obj_id < 10) && (hero_dead(get_hero(obj_id - 1)) || hero_unc(get_hero(obj_id - 1))))
				{
					host_writeb(Real2Host(ds_readd(0xe356)) + (l_var7 * 25) + l_var6, 0);
				} else if ((obj_id >= 10) && (obj_id < 30) && ((ds_readbs(0xd110 + obj_id * 62) & 1)))
				{
#if defined(__BORLANDC__)
	asm {  db 0xe9, 0x00, 0x00; nop; }
#endif
						host_writeb(Real2Host(ds_readd(0xe356)) + (l_var7 * 25) + l_var6, 0);
				}
			}
		}
	}

	if ((a4 == 6) || (a4 == 8)) {

		for (i = 0; i <= 6; i++) {
			if ((a4 != 8) || (i != a1)) {

					hero_ptr = get_hero(i);

					if ((host_readbs(hero_ptr + 0x21) != 0) &&
						(host_readbs(hero_ptr + 0x87) == ds_readbs(CURRENT_GROUP)) &&
						!hero_dead(hero_ptr))
					{
						FIG_search_obj_on_cb(i + 1, &l_var6, &l_var7);

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
									if (!host_readbs(Real2Host(ds_readd(0xe356)) + (25 * l_var5) + l_var4))
									{
										host_writeb(Real2Host(ds_readd(0xe356)) + (25 * l_var5) + l_var4, 9);
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

				enemy_ptr = p_datseg + ENEMY_SHEETS + i * 62;

				if ((host_readbs(enemy_ptr) != 0) && !enemy_dead(enemy_ptr)) {

					FIG_search_obj_on_cb(i + 10, &l_var6, &l_var7);
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
								if (!host_readbs(Real2Host(ds_readd(0xe356)) + (25 * l_var5) + l_var4))
								{
									host_writeb(Real2Host(ds_readd(0xe356)) + (25 * l_var5) + l_var4, 49);
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
							l_var15 = host_readbs(Real2Host(ds_readd(0xe356)) + (l_var5 * 25) + l_var4);

							if (obj_id < 0) {

								if (l_var15 == 0) {

									if (!NOT_NULL(ptr3) ||
										(NOT_NULL(ptr3) && (!two_fields ||
													((two_fields != 0) &&
														((!host_readbs(Real2Host(ds_readd(0xe356)) + (l_var9 * 25) + l_var8)) ||
														(host_readbs(Real2Host(ds_readd(0xe356)) + (l_var9 * 25) + l_var8))  == (a1 + 10)||
														(host_readbs(Real2Host(ds_readd(0xe356)) + (l_var9 * 25) + l_var8)) == (a1 + 30)) &&
														((l_var9 < 24) && (l_var9 >= 0) && (l_var8 < 24) && (l_var8 >= 0))))))
									{
										host_writeb(ptr2 + (l_var5 * 25) + l_var4, l_var2);
										l_var3 = 1;
									}
								/* DONE: 0x9ba - 0xa63 */
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
										/* DONE: 0xa66 - 0xac1 */
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
																((!host_readbs(Real2Host(ds_readd(0xe356)) + (l_var9 * 25) + l_var8)) ||
																(host_readbs(Real2Host(ds_readd(0xe356)) + (l_var9 * 25) + l_var8))  == (a1 + 10)||
																(host_readbs(Real2Host(ds_readd(0xe356)) + (l_var9 * 25) + l_var8)) == (a1 + 30)) &&
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
										/* DONE: 0xb64 - 0xc38 */
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
																((!host_readbs(Real2Host(ds_readd(0xe356)) + (l_var9 * 25) + l_var8)) ||
																(host_readbs(Real2Host(ds_readd(0xe356)) + (l_var9 * 25) + l_var8))  == (a1 + 10)||
																(host_readbs(Real2Host(ds_readd(0xe356)) + (l_var9 * 25) + l_var8)) == (a1 + 30)) &&
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
										/* DONE: 0xc3b - 0xd5e */
								}
							}
						} else {
							if (((a4 == 4) || (a4 == 5)) &&
								((host_readbs(Real2Host(ds_readd(0xbd2c)) + 0x14) > 3) ||
									((host_readbs(Real2Host(ds_readd(0xbd2c)) + 0x14) <= 3) && ((l_var4 > 23) || (l_var5 > 23) || (l_var5 < 0)))))
							{
								arr3[l_si] = 1;
								arr1[l_si] = l_var4;
								arr2[l_si] = l_var5;
								l_si++;
								if (l_si == 10) {
									break;
								}
							}
							/* TODO: 0xd60 - 0xddd */
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
					FIG_backtrack(ptr2, arr1[i], arr2[i], l_var2, host_readbs(in_ptr + 0x23), a4, two_fields, a1);
				} else {
					FIG_backtrack(ptr2, arr1[i], arr2[i], l_var2, host_readbs(in_ptr + 0x33), a4, two_fields, a1);
				}

				l_var11 = FIG_count_smth((signed char*)p_datseg + 0xd823);

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
				FIG_backtrack(ptr2, arr1[l_var13], arr2[l_var13], l_var2, host_readbs(in_ptr + 0x23), a4, two_fields, a1);
			} else {
				FIG_backtrack(ptr2, arr1[l_var13], arr2[l_var13], l_var2, host_readbs(in_ptr + 0x33), a4, two_fields, a1);
			}
		/* DONE: 0xde9 - 0xf27 */
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
