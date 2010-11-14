/*
	Rewrite of DSA1 v3.02_de functions of seg007 (random, dice, min/max)
	Functions rewritten: complete
*/
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

/**
	is_in_word_array - checks if val is in a word array
*/
unsigned short is_in_word_array(unsigned short val, Bit8u *p) {

	unsigned short i;
	Bit8u *p_tmp;

	for (i = 1; (short)host_readw(p) >= 0; i++) {
		p_tmp = p;
		p += 2;
		if (host_readw(p_tmp) == val)
			return i;
	}

	return 0;
}

/**
	is_in_byte_array - checks if val is in a byte array
*/
unsigned short is_in_byte_array(char val, Bit8u *p) {

	unsigned short i;
	Bit8u *p_tmp;

	for (i = 1; (signed char)host_readb(p) != -1; i++) {
		p_tmp = p;
		p += 1;
		if (host_readb(p_tmp) == val)
			return i;
	}

	return 0;
}

/**
	dice_template - rolls a dice from enemy templates
*/
short dice_template(unsigned short val) {
	unsigned short n, m;
	char x;
	unsigned short i, sum = 0;

	/* get dice formula n*Wm+x */
	n = (val & 0xf000) >> 12;

	switch ((val & 0x0f00) >> 8) {
		case 1:	m = 6;
			break;
		case 2: m = 20;
			break;
		case 3: m = 3;
			break;
		default:
			m = 4;
	}

	x = (val & 0xff);

	/* roll the dices */
	for (i = 0; i < n; i++)
		sum += random_schick(m);

	return sum + x;
}

/**
	damage_range_template - writes damage range from enemy templates to mem
*/
void damage_range_template(unsigned short val, Bit8u *min, Bit8u *max) {
	unsigned short n, m;
	char x;
	unsigned short i;

	/* get dice formula n*Wm+x */
	n = (val & 0xf000) >> 12;

	switch ((val & 0x0f00) >> 8) {
		case 1:	m = 6;
			break;
		case 2: m = 20;
			break;
		case 3: m = 3;
			break;
		default:
			m = 4;
	}

	x = (val & 0xff);

	/* set vars to 0 */
	host_writew(max, 0);
	host_writew(min, 0);

	for (i = 0; i < n; i++) {
		host_writew(min, host_readw(min) + 1); /* *min++; */
		host_writew(max, host_readw(max) + m); /* *max += m; */
	}

	host_writew(min, host_readw(min) + x);
	host_writew(max, host_readw(max) + x);
}
