/*
	Rewrite of DSA1 v3.02_de functions of seg007 (random, dice, min/max)
	Functions rewritten: complete
*/
#include <stdlib.h>

#include "mem.h"

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"

#include "seg007.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if !defined(__BORLANDC__)
static inline
unsigned short _rotl(unsigned short op, unsigned char count) {
	return (op << count) | (op >> (16 - count));
}
#endif

/**
	random_interval - generates a random number between lo and hi
*/
/* Borlandified and identical */
int random_interval(const int lo, const int hi)
{

	return lo + random_schick(hi - lo + 1) - 1;
}

/**
	random_schick - generates a u16 random number
*/
int random_schick(const int val)
{
	register short ax, bx, dx;

	if (val == 0)
		return 0;

	ax = ds_readw(0x4ba0);		/* get rand_seed */
	ax = ax ^ ds_readw(0xc3bf);	/* XOR with rand_seed2 */
	ax = _rotl(ax, 2);		/* ROL ax */
	ax = ax + ds_readw(0xc3bf);	/* ADD rand_seed2 */
	ax = ax ^ ds_readw(0x4ba0);	/* XOR with rand_seed */
	ax = _rotl(ax, 3);
	bx = ax;
	dx = (ax < 0) ? -1 : 0;		/* emulate CWD */
	ax = (ax ^ dx) - dx + 1 ;
	ds_writew(0x4ba0, ax);		/* update rand_seed */
	ax = bx;
	dx = (ax < 0) ? -1 : 0;		/* emulate CWD */
	ax = (ax ^ dx) - dx;
	dx = (ax < 0) ? -1 : 0;		/* emulate CWD */

	ax = ((dx << 16) | ax) % val;	/* emulate a dx_ax register */

	return ax + 1;
}

/**
	dice_roll - rolls a dice: n*Wm+x
*/
/* Borlandified and identical */
int dice_roll(const int n, const int m, const int x)
{
	int sum = 0;
	int i;

	for (i = 0; i < n; i++) {
		sum += random_schick(m);
	}

	sum += x;

	return sum;
}

/**
	calc_damage_range - calculate min/max damage of a weapon
*/
/* Borlandified and identical */
void calc_damage_range(const int n, const int m, const int x, Bit8u *min, Bit8u *max)
{
	host_writew(min, n+x);
	host_writew(max, n*m+x);
}

/**
	is_in_word_array - checks if val is in a word array
*/
/* Borlandified and identical */
int is_in_word_array(const int val, signed short *p)
{

	int i;

	for (i = 1; host_readws((Bit8u*)p) >= 0; i++) {
		if (host_readws((Bit8u*)(p++)) == val)
			return i;
	}

	return 0;
}

/**
	is_in_byte_array - checks if val is in a byte array
*/
/* Borlandified and identical */
int is_in_byte_array(const signed char val, Bit8u *p)
{
	int i;

	for (i = 1; host_readbs(p) != -1; i++) {
		if (host_readbs(p++) == val)
			return i;
	}

	return 0;
}

/**
	dice_template - rolls a dice from enemy templates
*/
/* Borlandified and identical */
int dice_template(const unsigned short val)
{
	signed short n;
	signed short m;
	signed char x;
	signed short i, sum = 0;

	/* get dice formula n*Wm+x */
	n = _rotl(val & 0xf000, 4);

	i =_rotl(val & 0x0f00, 8);

	m = (i == 1) ? 6 : ((i == 2) ? 20 : ((i == 3) ? 3 : 4));

	x = (signed char)val;

	/* roll the dices */
	for (i = 0; i < n; i++)
		sum += random_schick(m);

	sum += x;

	return sum;
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

#if !defined(__BORLANDC__)
}
#endif
