/*
 *	Rewrite of DSA1 v3.02_de functions of seg005 (fight)
 *	Functions rewritten: 5/9
 */

#include "schick.h"

#include "seg096.h"

/**
 *	FIG_set_star_color - set the color of the star in fights
 *	@ptr:	pointer to the star template
 *	@count: number of bytes the star has
 *	@color: 1=red/2=green/3=blue/4=yellow/11=darkbrown/12=lightbrown/13=pink
 *
 *	Sets the color of the star which shows fight activities,
 *	like damage, in fights.
 */
void FIG_set_star_color(PhysPt ptr, unsigned short count, unsigned char color) {

	color += 0x80;
	for (; --count; ptr++) {
		if (mem_readb(ptr))
			mem_writeb(ptr, color);
	}
}

/*
 *	FIG_name_3rd_case - returns name in the 3rd german case
 *	@type:	1 = enemy / 2 = hero
 *	@pos:	position
 *
 *	Would return "einem Magier" if the enemy is a "Magier".
*/
//static
RealPt FIG_name_3rd_case(unsigned short type, unsigned short pos) {

	if (type == 2)
		return ds_readd(0xbd34) + pos * 0x6da + 0x10;
	else
		return GUI_names_grammar(3, pos, 1);
}

/*
 *	FIG_name_4th_case - returns name in the 4th german case
 *	@type:	1 = enemy / 2 = hero
 *	@pos:	position
 *
 *	Would return "einen Magier" if the enemy is a "Magier".
*/
//static
RealPt FIG_name_4th_case(unsigned short type, unsigned short pos) {

	if (type == 2)
		return ds_readd(0xbd34) + pos * 0x6da + 0x10;
	else
		return GUI_names_grammar(2, pos, 1);
}

/*
 *	FIG_name_1st_case - returns name in the 1st german case
 *	@type:	1 = enemy / 2 = hero
 *	@pos:	position
 *
 *	Would return "ein Magier" if the enemy is a "Magier".
*/
//static
RealPt FIG_name_1st_case(unsigned short type, unsigned short pos) {

	if (type == 2)
		return ds_readd(0xbd34) + pos * 0x6da + 0x10;
	else
		return GUI_names_grammar(0, pos, 1);
}

//static
void set_delay_timer() {
	/* set delay_timer to delay_factor */
	ds_writew(0xe2d0, ds_readw(0x4b66));
}

