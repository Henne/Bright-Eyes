/*
 *	Rewrite of SOA EN functions of seg001 (main, )
 *	Functions rewritten: 1/
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 2.0
 *	Call:		BCC.EXE -mlarge -O- -c -1 soa_seg001.cpp
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

signed short enter_sound_option(char *opts)
{
	signed short i;
	signed short key;

	do {
		i = 0;
		key = bc_bioskey(0) & 0xff;

		// opts = [1,2,3,0]
		while (opts[i] != 0) {
			if (opts[i++] == key) return i--;
		}
	} while (1);
}

#if !defined(__BORLANDC__)
}
#endif
