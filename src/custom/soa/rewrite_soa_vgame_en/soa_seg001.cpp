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
#include "soa_seg001.h"
#include "soa_seg002.h"

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


void refresh_screen_size(void)
{
	refresh_screen_size1();
}

void refresh_screen_size1(void)
{
	/* check lock */
	if (ds_readw(MOUSE_LOCKED) == 0) {

		inc_ds_ws(MOUSE_REFRESH_FLAG);

		if (ds_readw(MOUSE_REFRESH_FLAG) == 0) {

			/* get lock */
			ds_writew(MOUSE_LOCKED, 1);

			if (ds_readws(MOUSE_POSX) < ds_readws(MOUSE_POINTER_OFFSETX))
				ds_writew(MOUSE_POSX, ds_readw(MOUSE_POINTER_OFFSETX));

			if (ds_readws(MOUSE_POSX) > 315)
				ds_writew(MOUSE_POSX, 315);

			if (ds_readws(MOUSE_POSY) < ds_readws(MOUSE_POINTER_OFFSETY))
				ds_writew(MOUSE_POSY, ds_readw(MOUSE_POINTER_OFFSETY));

			if (ds_readws(MOUSE_POSY) > 195)
				ds_writew(MOUSE_POSY, 195);

			save_mouse_bg();
			ds_writew(MOUSE_POSX_BAK, ds_readw(MOUSE_POSX));
			ds_writew(MOUSE_POSY_BAK, ds_readw(MOUSE_POSY));
			ds_writew(MOUSE_POINTER_OFFSETX_BAK, ds_readw(MOUSE_POINTER_OFFSETX));
			ds_writew(MOUSE_POINTER_OFFSETY_BAK, ds_readw(MOUSE_POINTER_OFFSETY));
			draw_mouse_cursor();

			/* put lock */
			ds_writew(MOUSE_LOCKED, 0);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
