/*
 *	Rewrite of DSA1 v3.02_de functions of seg032 (fight)
 *	Functions rewritten 1/12
*/

#include "schick.h"

/**
 *	FIG_set_cb_field - sets an object to a chessboard position
 *	@row:		the row on the chessboard (iso_y)
 *	@col:		the column on the chessboard (iso_y)
 *	@object:	number of the object
 *			0=free, 1-6=heroes, >10=enemys,
 *			>50walls, trees, etc
 *			-3 = flee
 *
 *	This function is called multiple timeswhen an entrant in a fight
 *	is moving. The initial placements of static objects are also done
 *	with this function.
 */
void FIG_set_cb_field(signed short row, signed short col, signed char object) {
	if (row < 0 || row > 24 || col < 0 || col > 24)
		return;

	host_writeb(MemBase + Real2Phys(ds_readd(0xd852)) + row * 25 + col,
		object);
}
