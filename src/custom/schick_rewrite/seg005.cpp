/*
 *	Rewrite of DSA1 v3.02_de functions of seg005 (fight)
 *	Functions rewritten: 1/9
 */

#include "schick.h"

//static
void set_delay_timer() {
	/* set delay_timer to delay_factor */
	ds_writew(0xe2d0, ds_readw(0x4b66));
}

