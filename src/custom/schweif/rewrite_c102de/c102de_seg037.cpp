/*
 *	Rewrite of DSA2 c1.02_de functions of seg037 (random, dice, min/max)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y SEG037.CPP
*/
// TODO: These functions are just copies of the ones in schick. Use these rather than doubling the code.

#include <stdlib.h>

#if !defined(__BORLANDC__)
#include "schweif.h"

#include "c102de_seg037.h"

#else
#include "c102de.h"

#include "seg037.h"
#endif


#if !defined(__BORLANDC__)
namespace C102de {
#endif

#if !defined(__BORLANDC__)
static inline
unsigned short _rotl(unsigned short op, unsigned char count) {
	return (op << count) | (op >> (16 - count));
}
static inline int __abs__(int a) { return abs(a); };
#endif

/**
	random_interval - generates a random number between lo and hi
*/
int random_interval(const int lo, const int hi)
{

	return lo + random_schweif(hi - lo + 1) - 1;
}

/**
	random_schweif - generates a random number
*/
int random_schweif(const int val)
{
	short retval;
	int tmp1, tmp2;

	if (val) {

		ds_writew(0x869e, __abs__(tmp1 = retval = (_rotl(retval = ((retval = (_rotl(retval = ds_readw(0x869e) ^ ds_readw(0xcb74), 2))) + ds_readw(0xcb74)) ^ ds_readw(0x869e), 3))) + 1);
		tmp2 = retval;
		retval = __abs__(retval) % val + 1;

	} else {
		retval = 0;
	}

	return retval;
}

/**
	dice_roll - rolls a dice: n*Wm+x
*/
int dice_roll(const int n, const int m, const int x)
{
	int i;
	int sum = 0;

	for (i = 0; i < n; i++) {
		sum += random_schweif(m);
	}

	sum += x;
	if (sum < 0)
		sum = 0;

	return sum;
}

/**
	calc_damage_range - calculate min/max damage of a weapon
*/
void calc_damage_range(const int n, const int m, const int x, Bit8u *min, Bit8u *max)
{
	host_writew(min, n + x);
	host_writew(max, n * m + x);
}

#if !defined(__BORLANDC__)
}
#endif
