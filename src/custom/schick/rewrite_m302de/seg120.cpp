/**
 *	Rewrite of DSA1 v3.02_de functions of seg120 (misc)
 *	Functions rewritten: 3/11
 */

#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg001.h"
#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg010.h"
#include "seg028.h"
#include "seg063.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void refresh_colors(void)
{
	set_color(p_datseg + 0xb22d, 0);
	set_color(p_datseg + 0xb22a, 0xff);
	set_palette(p_datseg + 0xb230, 0xd8, 8);

	set_palette(p_datseg + 0xb248, 0xc8, 3);
	set_palette(p_datseg + 0xb251, 0x40, 0x20);
	set_palette(p_datseg + 0xb2b1, 0x20, 0x20);
	set_palette(p_datseg + 0x2723, 0x60, 0x20);

}

/* Borlandified and nearly identical, see BC-TODO */
void init_game_state(void)
{
	signed short i;

	refresh_colors();

	ds_writew(0xd2cb, 0xc8);
	ds_writew(0xd2cd, 0xc9);
	ds_writew(0xd2cf, 0xca);

	set_to_ff();

	for (i = 0; i < 0xfe; i++) {
		ds_writeb(0xe12d + i, 0);
	}

	ds_writeb(0x2d9f, 0);
	/* Travia Temple in Thorwal */
	ds_writeb(LOCATION, 2);
	ds_writew(TYPEINDEX, 1);
	ds_writew(0x2d83, 9);
	ds_writew(0x2d85, 9);
	ds_writew(X_TARGET, 9);
	ds_writew(Y_TARGET, 8);
	ds_writeb(0x2d7c, 0);
	ds_writeb(DIRECTION, 0);
	ds_writeb(DUNGEON_INDEX, 0);

	/* BC-TODO: var = current_town = 1; */
	ds_writeb(CURRENT_TOWN, 1);
	ds_writeb(0x2da6, 1);

	ds_writew(0xbffd, 3);

	/* timer */
	/* ds_writed(DAY_TIMER, 24 * 0x1518 - 1);*/
	ds_writed(DAY_TIMER, 0x1fa3f);
	timewarp_until(1);
	/* ds_writed(DAY_TIMER,  8 * 0x1518); */
	ds_writed(DAY_TIMER,  0xa8c0);
	ds_writeb(DAY_OF_WEEK, 4);
	ds_writeb(DAY_OF_MONTH, 17);
	ds_writeb(MONTH, 1);
	ds_writeb(YEAR, 15);

	ds_writed(0xc00d, ds_readd(0xd2ff));

	load_objects_nvf();
	passages_init();

	ds_writew(CURRENT_ANI, -1);
	ds_writew(0xe113, 1);

	ds_writed(0xbff9, ds_readd(0xd303));
	load_splashes();
}

void cleanup_game(void)
{
	struct ffblk blk;
	signed short l_si;
	signed short l_di;

	/* disable AIL */
	exit_AIL();

	/* disable CD-Audio */
	if (ds_readw(0x95) != 0) {
		CD_audio_stop();
	}

	/* free EMS memory */
	if (ds_readb(0x26ab) != 0) {

		for (l_si = 0; l_si < 37; l_si++) {
			if (host_readw(Real2Host(ds_readd(0xe121)) + l_si * 8) != 0) {
				EMS_free_pages(host_readw(Real2Host(ds_readd(0xe121)) + l_si * 8) + 2);
			}
		}

		/* free male and female figures */
		for (l_si = 0; l_si < 43; l_si++) {

			if ((host_readw(Real2Host(ds_readd(0xe11d)) + l_si * 12) != 0) &&
				((host_readw(Real2Host(ds_readd(0xe11d)) + l_si * 12 + 6) != 0)))
			{
				EMS_free_pages(host_readw(Real2Host(ds_readd(0xe11d)) + l_si * 12) + 6);
			}

			if ((host_readw(Real2Host(ds_readd(0xe119)) + l_si * 12) != 0) &&
				((host_readw(Real2Host(ds_readd(0xe119)) + l_si * 12 + 6) != 0)))
			{
				EMS_free_pages(host_readw(Real2Host(ds_readd(0xe119)) + l_si * 12) + 6);
			}
		}

		/* free monster figures */
		for (l_si = 0; l_si < 36; l_si++) {

			if ((host_readw(Real2Host(ds_readd(0xe115)) + l_si * 12) != 0) &&
				((host_readw(Real2Host(ds_readd(0xe115)) + l_si * 12 + 6) != 0)))
			{
				EMS_free_pages(host_readw(Real2Host(ds_readd(0xe115)) + l_si * 12) + 6);
			}
		}

		if (ds_readw(0xbd92) != 0) {
			EMS_free_pages(ds_readw(0xbd92));
		}

		/* free map memory */
		if (ds_readw(0xbd90) != 0) {
			EMS_free_pages(ds_readw(0xbd90));
		}
	}

	/* delete all files in TEMP */

	sprintf((char*)Real2Host(ds_readd(0xd2eb)),
		(char*)Real2Host(ds_readd(0x4c88)),	/* contains "TEMP\\%s" */
		(char*)p_datseg + 0xb4b9);		/* contains "*.*" */

	l_di = bc_findfirst((RealPt)ds_readd(0xd2eb), &blk, 0);

	if (l_di == 0) {
		do {
			/* delete each found file */
			sprintf((char*)Real2Host(ds_readd(0xd2eb)),
				(char*)Real2Host(ds_readd(0x4c88)),	/* contains "TEMP\\%s" */
				((char*)(&blk)) + 30);			/* contains a filename */

			bc_unlink((RealPt)ds_readd(0xd2eb));

			l_di = bc_findnext(&blk);
		} while (!l_di);
	}

	/* misc cleanups */
	update_mouse_cursor();
	disable_mouse();
	reset_timer();
	schick_reset_video();
	bc_clrscr();
}

#if !defined(__BORLANDC__)
}
#endif
