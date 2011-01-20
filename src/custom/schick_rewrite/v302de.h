/*
 *	transitional functions for version 3.02_de
 *
 *	Here are function, which should make the transition from
 *	DOXBox emulation to native easier.
 *	These functions will only work with the version mentioned above.
 */

#include "schick.h"

#define HEROS		(0xbd34)
#define TEXT_LTX	(0xc3b5)
#define CHESSBOARD	(0xd852)
#define ITEMSDAT	(0xe22b)

static inline Bit8u *get_hero(unsigned short index) {
	return MemBase + Real2Phys(ds_readd(HEROS)) + index * 0x6da;
}

static inline Bit8u *get_ltx(unsigned short off) {
	return MemBase + Real2Phys(mem_readd(Real2Phys(ds_readd(TEXT_LTX) + off)));
}

static inline signed char get_cb_val(unsigned short x, unsigned short y) {
	return host_readb(MemBase + Real2Phys(ds_readd(CHESSBOARD)) + y * 25 + x);
}

static inline void set_cb_val(unsigned short x, unsigned short y, signed char val) {
	host_writeb(MemBase + Real2Phys(ds_readd(CHESSBOARD)) + y * 25 + x, val);
}

static inline Bit8u *get_itemsdat(unsigned short item) {
	return MemBase + Real2Phys(ds_readd(ITEMSDAT)) + item * 12;
}
