/*
 *	Rewrite of SOA EN functions of seg008 (random)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y soa_seg008.cpp
 */


#include "dosbox.h"
#include "callback.h"
#include "dos_inc.h"

#include <stdlib.h>

#include "soa.h"
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
	return lo + random_soa(hi - lo + 1) - 1;
}

/**
 * \brief   generates a u16 random number
 */
unsigned short random_soa(const unsigned short val)
{
	signed short retval;

	if (val == 0) {
		return 0;
	}

	/* rand_seed XOR rand_seed2 */
	retval = ds_readw(RANDOM_SOA_SEED) ^ ds_readw(RANDOM_SOA_SEED2);
	retval = _rotl(retval, 2);		/* ROL retval */
	retval = (retval + ds_readw(RANDOM_SOA_SEED2)) ^ ds_readw(RANDOM_SOA_SEED);
	retval = _rotl(retval, 3);

	/* update rand_seed */
	ds_writew(RANDOM_SOA_SEED, __abs__(retval) + 1);

	retval = __abs__(retval) % val;

	return ++retval;
}

/**
 * \brief   checks if val is in a word array
 */
unsigned short is_in_word_array(const signed short val, signed short *p)
{
	int i;

	for (i = 1; *p >= 0; i++) {
		if (*(p++) == val)
			return i;
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
