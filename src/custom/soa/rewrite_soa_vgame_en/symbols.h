/*
 *	symbol names for SOA EN
 *
 *	Here are the mappings from adresses in the datasegment
 *	to symbolic names of global variables.
 */

#ifndef SOA_SYMBOLS_H
#define SOA_SYMBOLS_H

#define DEFAULT_MOUSE_CURSOR	(0x054b)
#define MOUSE_LOCKED		(0x05d3)	// Bit16
#define MOUSE_REFRESH_FLAG	(0x05d5)	// Bit16s
#define MOUSE_POSX		(0x05d7)	// Bit16s
#define MOUSE_POSY		(0x05d9)	// Bit16s
#define MOUSE_POSX_BAK		(0x05db)	// Bit16s
#define MOUSE_POSY_BAK		(0x05dd)	// Bit16s
#define MOUSE_MOVED		(0x05df)	// Bit16s

#define MOUSE_POINTER_OFFSETX		(0x05e1)
#define MOUSE_POINTER_OFFSETY		(0x05e3)
#define MOUSE_POINTER_OFFSETX_BAK	(0x05e5)
#define MOUSE_POINTER_OFFSETY_BAK	(0x05e7)

#define RANDOM_SOA_SEED		(0x5F76)
#define P_CURRENT_CURSOR	(0x80f4)
#define RANDOM_SOA_SEED2	(0x80f8)
#define P_LAST_CURSOR		(0x8dfc)	// RealPt
#define MOUSE_BACKBUFFER	(0xac36)	// ByteArray 16x16
#define VGA_MEMSTART		(0xad6c)	// RealPt

#endif
