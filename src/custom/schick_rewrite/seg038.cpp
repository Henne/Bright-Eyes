/*
 *      Rewrite of DSA1 v3.02_de functions of seg038 (Fight)
 *      Functions rewritten 1/6
 *
 *      Functions called rewritten 1/5
 *      Functions uncalled rewritten 0/1
*/

#include "schick.h"

#include "v302de.h"

/**
 * FIG_search_obj_on_cb - searches an object on the chessboard
 * @obj:	number of the object
 * @px:		pointer where x should be stored
 * @py:		pointer where y should be stored
 *
 * Returns 0 if the object was not found. If the object was
 * found it returns 1 and stores the coordinates at the pointers.
 */
unsigned short FIG_search_obj_on_cb(signed short obj, Bit8u *px, Bit8u* py) {

	unsigned short x, y;

	for (x = 0; x < 25; x++)
		for (y = 0; y < 24; y++)
			if (get_cb_val(x, y) == obj) {
				host_writew(px, x);
				host_writew(py, y);
				return 1;
			}

	host_writew(px, x);
	host_writew(py, y);
	return 0;
}
