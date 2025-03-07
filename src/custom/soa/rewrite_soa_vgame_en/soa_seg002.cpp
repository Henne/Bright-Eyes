/*
 *	Rewrite of SOA EN functions of seg002 (mouse)
 *	Functions rewritten: 2/ (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 2.0
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y soa_seg002.cpp
 */

#if !defined(__BORLANDC__)
#include "dosbox.h"
#include "callback.h"
#include "dos_inc.h"
#include "paging.h"

#include "soa.h"
#endif

#include "symbols.h"
#include "vgame_en.h"

#include "soa_seg000.h"

#if !defined(__BORLANDC__)
namespace SOA_VGAME_EN {
#endif

void draw_mouse_cursor()
{
	char Y, X;
	RealPt vgaptr;
	signed short *mouse_cursor;
	int rangeX, rangeY;
	int diffX, diffY;
	signed short mask;

	vgaptr = (RealPt)ds_readd(VGA_MEMSTART);
	mouse_cursor = (signed short*)Real2Host(ds_readd(P_CURRENT_CURSOR)) + (32 / 2);

	rangeX = ds_readw(0x05d7) - ds_readw(0x05e1);
	rangeY = ds_readw(0x05d9) - ds_readw(0x05e3);
	diffX = diffY = 16;

	if (rangeX > 304) diffX = 320 - rangeX;
	if (rangeY > 184) diffY = 200 - rangeY;

	vgaptr += rangeY * 320 + rangeX;

	for (Y = 0; Y < diffY; Y++) {
		mask = host_readw((Bit8u*)mouse_cursor++);
		for (X = 0; X < diffX; X++) {
			if ((0x8000 >> X) & mask) {
				mem_writeb(Real2Phys(vgaptr) + X, 0xff);
			}
		}
		vgaptr += 320;
	}
}

void save_mouse_bg()
{
	RealPt vgaptr;
	int rangeX, rangeY;
	int diffX, diffY;
	int Y, X;

	vgaptr = (RealPt)ds_readd(VGA_MEMSTART);

	rangeX = ds_readw(0x05d7) - ds_readw(0x05e1);
	rangeY = ds_readw(0x05d9) - ds_readw(0x05e3);
	diffX = diffY = 16;

	if (rangeX > 304) diffX = 320 - rangeX;
	if (rangeY > 184) diffY = 200 - rangeY;

	vgaptr += rangeY * 320 + rangeX;

	for (Y = 0; Y < diffY; Y++) {
		for (X = 0; X < diffX; X++) {
			ds_writeb(MOUSE_BACKBUFFER + 16 * Y + X, mem_readb(Real2Phys(vgaptr) + X));
		}
		vgaptr += 320;
	}
}

void restore_mouse_bg()
{
	RealPt vgaptr;
	int rangeX, rangeY;
	int diffX, diffY;
	int Y, X;

	vgaptr = (RealPt)ds_readd(VGA_MEMSTART);

	rangeX = ds_readw(0x05db) - ds_readw(0x05e5);
	rangeY = ds_readw(0x05dd) - ds_readw(0x05e7);
	diffX = diffY = 16;

	if (rangeX > 304) diffX = 320 - rangeX;
	if (rangeY > 184) diffY = 200 - rangeY;

	vgaptr += rangeY * 320 + rangeX;

	for (Y = 0; Y < diffY; Y++) {
		for (X = 0; X < diffX; X++) {
			mem_writeb(Real2Phys(vgaptr) + X, ds_readb(MOUSE_BACKBUFFER + 16 * Y + X));
		}
		vgaptr += 320;
	}
}

void wait_for_keyboard()
{
	while (bc_bioskey(1) != 0) bc_bioskey(0);
}

#if !defined(__BORLANDC__)
}
#endif
