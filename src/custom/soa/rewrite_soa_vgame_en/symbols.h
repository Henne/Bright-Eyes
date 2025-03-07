/*
 *	symbol names for SOA EN
 *
 *	Here are the mappings from adresses in the datasegment
 *	to symbolic names of global variables.
 */

#ifndef SOA_SYMBOLS_H
#define SOA_SYMBOLS_H

#define RANDOM_SOA_SEED		(0x5F76)
#define P_CURRENT_CURSOR	(0x80f4)
#define RANDOM_SOA_SEED2	(0x80f8)
#define MOUSE_BACKBUFFER	(0xac36)	// ByteArray 16x16
#define VGA_MEMSTART		(0xad6c)	// RealPt

#endif
