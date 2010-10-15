#include <stdio.h>

#include "mem.h"

#include "../schick.h"
#include "seg007.h"

static inline
unsigned short my_rol16(unsigned short op, unsigned char count) {
	return (op << count) | (op >> (16 - count));
}

/**
	random_interval - generates a u16 random number between lo and hi
*/
unsigned short random_interval(unsigned short lo, unsigned short hi) {

	return lo + random_schick(hi - lo + 1) - 1;
}

/**
	random_schick - generates a u16 random number
*/
unsigned short random_schick(short val) {
	short ax, bx, dx;

	if (val == 0)
		return 0;

	ax = real_readw(datseg, 0x4ba0);	/* get rand_seed */
	ax = ax ^ real_readw(datseg, 0xc3bf);	/* XOR with rand_seed2 */
	ax = my_rol16(ax, 2);			/* ROL ax */
	ax = ax + real_readw(datseg, 0xc3bf);	/* ADD rand_seed2 */
	ax = ax ^ real_readw(datseg, 0x4ba0);	/* XOR with rand_seed */
	ax = my_rol16(ax, 3);
	bx = ax;
	dx = (ax < 0) ? -1 : 0;			/* emulate CWD */
	ax = (ax ^ dx) - dx + 1 ;
	real_writew(datseg, 0x4ba0, ax);	/* update rand_seed */
	ax = bx;
	dx = (ax < 0) ? -1 : 0;			/* emulate CWD */
	ax = (ax ^ dx) - dx;
	dx = (ax < 0) ? -1 : 0;			/* emulate CWD */

	ax = ((dx << 16) | ax) % val;		/* emulate a dx_ax register */

	return ax + 1;
};

/**
	dice_roll - rolls a dice: n*Wm+x
*/

short dice_roll(unsigned short n, unsigned short m, short x) {
	unsigned short i;
	unsigned short sum = 0;

	for (i=0; i < n; i++)
		sum += random_schick(m);

	return sum + x;
}

/**
	calc_damage_range - calculate min/max damage of a weapon
*/
void calc_damage_range(unsigned short n, unsigned short m, short x, Bit8u *min, Bit8u *max) {
	host_writew(min, n+x);
	host_writew(max, n*m+x);
}

