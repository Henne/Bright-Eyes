/**
 *	Rewrite of DSA1 v3.02_de functions of seg120 (misc)
 *	Functions rewritten: 1/11
 */

#include "schick.h"

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg028.h"
#include "seg063.h"

namespace M302de {

void refresh_colors()
{
	set_color(p_datseg + 0xb22d, 0);
	set_color(p_datseg + 0xb22a, 0xff);
	set_palette(p_datseg + 0xb230, 0xd8, 8);

	set_palette(p_datseg + 0xb248, 0xc8, 3);
	set_palette(p_datseg + 0xb251, 0x40, 0x20);
	set_palette(p_datseg + 0xb2b1, 0x20, 0x20);
	set_palette(p_datseg + 0x2723, 0x60, 0x20);

}

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
	ds_writeb(TYPEINDEX, 1);
	ds_writew(0x2d83, 9);
	ds_writew(0x2d85, 9);
	ds_writew(X_TARGET, 9);
	ds_writew(Y_TARGET, 8);
	ds_writeb(0x2d7c, 0);
	ds_writeb(DIRECTION, 0);
	ds_writeb(DUNGEON_INDEX, 0);

	/* var = current_town = 1; */
	ds_writeb(CURRENT_TOWN, 1);
	ds_writeb(0x2da6, 1);

	ds_writew(0xbffd, 3);

	/* timer */
	ds_writed(DAY_TIMER, 24 * 0x1518 - 1);
	timewarp_until(1);
	ds_writed(DAY_TIMER,  8 * 0x1518);
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

}
