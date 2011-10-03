/*
	Rewrite of DSA1 Generator v1.05_de seg003 (random)
	Functions rewritten: complete
*/

#include <stdlib.h>

#include "schick.h"

#include "g105de_seg003.h"

namespace G105de {

static Bit16u rand_seed = 0x327b;

static inline
unsigned short my_rol16(unsigned short op, unsigned char count) {
	return (op << count) | (op >> (16 - count));
}

/**
	random_interval_gen - generates a u16 random number between lo and hi
*/
unsigned short random_interval_gen(unsigned short lo, unsigned short hi) {

	return lo + random_gen(hi - lo + 1) - 1;
}

/**
	random_gen - generates a u16 random number
*/
unsigned short random_gen(short val) {

	Bit16u si, ax;

	if (val == 0)
		return 0;

	si = rand_seed ^ ds_readw(0x458f);
	ax = my_rol16(si, 2) + ds_readw(0x458f);
	ax = ax ^ rand_seed;
	si = my_rol16(ax, 3);

	rand_seed = si;		/* update rand_seed */
	si = abs(si) % val;

	return si + 1;
}

unsigned short is_in_word_array(unsigned short val, Bit8u *p)
{

	while (host_readw(p) != 0) {
		p += 2;

		if (host_readw(p) == val)
			return 1;
	}

	return 0;
}

}
