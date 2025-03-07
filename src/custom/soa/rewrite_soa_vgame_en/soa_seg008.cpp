/*
 *	Rewrite of SOA EN functions of seg008 (random)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 2.0
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y soa_seg008.cpp
 */

#if !defined(__BORLANDC__)
#include "dosbox.h"
#include "callback.h"
#include "dos_inc.h"
#include "soa.h"
#endif

//#include <stdlib.h>
//#include <math.h>
//#undef abs

#include "vgame_en.h"

#include "soa_seg008.h"

#if !defined(__BORLANDC__)
namespace SOA_VGAME_EN {
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
unsigned short random_interval(const unsigned short lo, const unsigned short hi)
{
	return ((lo + random_soa(hi - lo + 1)) - 1);
}

/**
 * \brief   generates a u16 random number
 */
signed int random_soa(const signed int val)
{
	int retval;

	if (val == 0) {
		return 0;
	}

	/* rand_seed XOR rand_seed2 */
	retval = ds_readw(RANDOM_SOA_SEED) ^ ds_readw(RANDOM_SOA_SEED2);
	retval = _rotl(retval, 2);		/* ROL retval */
	retval = (retval + ds_readw(RANDOM_SOA_SEED2)) ^ ds_readw(RANDOM_SOA_SEED);
	retval = _rotl(retval, 3);

	/* update rand_seed */
	ds_writew(RANDOM_SOA_SEED, retval);

	retval = abs(retval) % val;

	return ++retval;
}

/**
 * \brief   checks if val is in a word array
 */
unsigned short is_in_word_array(const signed short val, signed short *p)
{
	while (*p >= 0) {
		if (*p++ == val) return 1;
	}

	return 0;
}

/**
 * \brief   checks if val is in a byte array
 */
unsigned short is_in_byte_array(const signed char val, signed char *p)
{
	int i;

	for (i = 1; *p != -1; i++) {
		if (*(p++) == val)
			return i;
	}

	return 0;
}

#if !defined(__BORLANDC__)
}
#endif
