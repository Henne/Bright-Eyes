/*
 *      Rewrite of DSA1 v3.02_de functions of seg038 (Fight)
 *      Functions rewritten 3/6
 *
 *      Functions called rewritten 3/5
 *      Functions uncalled rewritten 0/1
*/

#include "schick.h"

#include "v302de.h"

#include "seg006.h"

namespace M302de {

/**
 * FIG_search_obj_on_cb - searches an object on the chessboard
 * @obj:	number of the object
 * @px:		pointer where x should be stored
 * @py:		pointer where y should be stored
 *
 * Returns 0 if the object was not found. If the object was
 * found it returns 1 and stores the coordinates at the pointers.
 */
unsigned short FIG_search_obj_on_cb(signed short obj, signed short *px, signed short *py)
{

	signed short x, y;

	for (x = 0; x < 25; x++)
		for (y = 0; y < 24; y++)
			if (get_cb_val(x, y) == obj) {
				*px = x;
				*py = y;
				return 1;
			}

	*px = x;
	*py = y;
	return 0;
}

void FIG_init_list_elem(signed short obj) {

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

}
