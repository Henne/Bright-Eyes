/*
	Rewrite of DSA2 c1.02_de functions of seg037 (random, dice, min/max)
	Functions rewritten: 4/4 (complete)
*/

#include "schweif.h"

#include "c102de_seg037.h"

namespace C102de {

// TODO: These functions are just copies of the ones in schick. Use these rather than doubling the code.
static inline unsigned short my_rol16(unsigned short op, unsigned char count)
{
	return (op << count) | (op >> (16 - count));
}

/**
	random_schweif - generates a u16 random number
*/
unsigned short random_schweif(signed short val)
{
	signed short ax, bx, dx;

	if (val == 0)
		return 0;

	ax = ds_readw(0x869e);		/* get rand_seed */
	ax = ax ^ ds_readw(0xcb74);	/* XOR with rand_seed2 */
	ax = my_rol16(ax, 2);		/* ROL ax */
	ax = ax + ds_readw(0xcb74);	/* ADD rand_seed2 */
	ax = ax ^ ds_readw(0x869e);	/* XOR with rand_seed */
	ax = my_rol16(ax, 3);
	bx = ax;
	dx = (ax < 0) ? -1 : 0;		/* emulate CWD */
	ax = (ax ^ dx) - dx + 1 ;
	ds_writew(0x869e, ax);		/* update rand_seed */
	ax = bx;
	dx = (ax < 0) ? -1 : 0;		/* emulate CWD */
	ax = (ax ^ dx) - dx;
	dx = (ax < 0) ? -1 : 0;		/* emulate CWD */

	ax = ((dx << 16) | ax) % val;	/* emulate a dx_ax register */

	return ax + 1;
}


/**
 *	random_interval() - generates a u16 random number between lo and hi
 */
unsigned short random_interval(unsigned short lo, unsigned short hi) {

	return lo + random_schweif(hi - lo + 1) - 1;
}

signed short dice_roll(unsigned short n, unsigned short m, signed short x)
{
	signed short i;
	signed short sum = 0;

	for (i = 0; i < n; i++) {
		sum += random_schweif(m);
	}

	sum += x;

	if (sum < 0)
		sum = 0;

	return sum;
}

/**
 *	calc_damage_range() - calculate min/max damage of a weapon
 */
void calc_damage_range(unsigned short n, unsigned short m, short x,
				Bit8u *min, Bit8u *max)
{
	host_writew(min, n + x);
	host_writew(max, n * m + x);
}

}
