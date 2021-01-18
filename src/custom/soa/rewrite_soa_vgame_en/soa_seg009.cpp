/*
 *	Rewrite of SOA en functions of seg009 (HW: VGA, TIMER)
 *	Functions rewritten: 2/45
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y -IAIL seg002.cpp
 *
 */
#include "dosbox.h"
#include "callback.h"
#include "dos_inc.h"

#include "soa.h"

#include "soa_seg000.h"

namespace SOA_VGAME_EN {

// Remark: written in inline asm with parameter
unsigned short swap_word(unsigned short inval)
{
	return (((inval & 0xff00) >> 8) | ((inval & 0xff) << 8));
}

// Remark: written in inline asm with parameter in reg_ax
unsigned short swap_word_fast(unsigned short inval)
{
	return (((inval & 0xff00) >> 8) | ((inval & 0xff) << 8));
}

}
