/*
 *      Rewrite of DSA1 v3.02_de functions of seg038 (Fight)
 *      Functions rewritten 5/6
 *
 *      Functions called rewritten 4/5
 *      Functions uncalled rewritten 1/1
*/

#include <string.h>

#include "v302de.h"

#include "seg006.h"
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

			lvar12 = FIG_count_smth(ptr_cur);

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
unsigned short FIG_count_smth(Bit8u *p) {

	unsigned short count, i;

	i = 0;
	count = 0;

	if (host_readb(p) == 0xff)
		return 99;

	while (host_readb(p + ++i) != 0xff) {
		if (host_readb(p + i - 1) != host_readb(p + i))
			count++;
	}

	return count;
}

#if !defined(__BORLANDC__)
}
#endif
