/*
 *	Rewrite of DSA1 v3.02_de functions of seg007 (random, dice, min/max)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg007.cpp
 */

#include <stdlib.h>

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
 * \brief   generates a random number between lo and hi
 */
int random_interval(const int lo, const int hi)
{

	return lo + random_schick(hi - lo + 1) - 1;
}

/**
 * \brief   generates a u16 random number
 */
int random_schick(const int val)
{
	signed short retval;


	if (val == 0) {
		return 0;
	}

	/* rand_seed XOR rand_seed2 */
	retval = ds_readw(RANDOM_SCHICK_SEED) ^ ds_readw(RANDOM_SCHICK_SEED2);
	retval = _rotl(retval, 2);		/* ROL retval */
	retval = (retval + ds_readw(RANDOM_SCHICK_SEED2)) ^ ds_readw(RANDOM_SCHICK_SEED);
	retval = _rotl(retval, 3);

	/* update rand_seed */
	ds_writew(RANDOM_SCHICK_SEED, __abs__(retval) + 1);

	retval = __abs__(retval) % val;

	return ++retval;
}

/**
 * \brief   rolls a dice: n*Wm+x
 */
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
 * \brief   calculate min/max damage of a weapon
 */
void calc_damage_range(const int n, const int m, const int x, Bit8u *min, Bit8u *max)
{
	host_writew(min, n+x);
	host_writew(max, n*m+x);
}

/**
 * \brief   checks if val is in a word array
 */
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
 * \brief   checks if val is in a byte array
 */
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
 * \brief   rolls a dice from enemy templates
 */
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
 * \brief   writes damage range from enemy templates to mem
 */
void damage_range_template(unsigned short val, Bit8u *min, Bit8u *max)
{
	signed short n, m;
	signed char x;
	signed short i;
	signed short tmp = val;

	/* get dice formula n*Wm+x */
	n = _rotl(tmp & 0xf000, 4);

	i =_rotl(tmp & 0x0f00, 8);

	m = (i == 1) ? 6 : ((i == 2) ? 20 : ((i == 3) ? 3 : 4));

	x = (signed char)tmp;

	/* set vars to 0 */
	host_writew(min, host_writews(max, 0));

	for (i = 0; i < n; i++) {
		inc_ptr_ws(min);	/* *min++; */
		add_ptr_ws(max, m);	/* *max += m; */
	}

	add_ptr_ws(min, x);
	add_ptr_ws(max, x);
}

#if !defined(__BORLANDC__)
}
#endif
