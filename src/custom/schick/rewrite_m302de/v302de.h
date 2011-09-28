/*
 *	transitional functions for version 3.02_de
 *
 *	Here are function, which should make the transition from
 *	DOXBox emulation to native easier.
 *	These functions will only work with the version mentioned above.
 */

#include "schick.h"

#define FNAMES		(0x4c8c)
#define HEROS		(0xbd34)
#define DIALOG_TEXT	(0xc3b1)
#define TEXT_LTX	(0xc3b5)
#define CHESSBOARD	(0xd852)
#define ITEMSDAT	(0xe22b)
#define SPELLTARGET	(0xe5b8)
#define SPELLUSER	(0xe5bc)

static inline Bit8u *get_hero(unsigned short index) {
	return MemBase + Real2Phys(ds_readd(HEROS)) + index * 0x6da;
}

static inline Bit8u *get_spelluser() {
	return MemBase + Real2Phys(ds_readd(SPELLUSER));
}

static inline Bit8u *get_spelltarget() {
	return MemBase + Real2Phys(ds_readd(SPELLTARGET));
}

static inline Bit8u *get_fname(unsigned short off) {
	return MemBase + Real2Phys(ds_readd(FNAMES + off * 4));
}

static inline Bit8u *get_ltx(unsigned short off) {
	return MemBase + Real2Phys(mem_readd(Real2Phys(ds_readd(TEXT_LTX) + off)));
}

static inline Bit8u *get_dtp(unsigned short off) {
	return MemBase + Real2Phys(mem_readd(Real2Phys(ds_readd(DIALOG_TEXT) + off)));
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
