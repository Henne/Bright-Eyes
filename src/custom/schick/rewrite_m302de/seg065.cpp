/**
 *	Rewrite of DSA1 v3.02_de functions of seg065 (special animations)
 *	Functions rewritten: 1/9
 */

#include "v302de.h"

#include "seg004.h"
#include "seg025.h"
#include "seg027.h"
#include "seg029.h"
#include "seg055.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void do_market(void)
{
	signed short done;
	signed short answer;
	signed short type_bak;
	signed short dir_bak;
	signed short bak1;

	done = 0;
	ds_writew(0x2846, 1);
	bak1 = ds_readbs(0x2d7c);
	dir_bak = ds_readbs(DIRECTION);

	do {

		if (ds_readw(0x2846) != 0) {
			draw_main_screen();
			set_var_to_zero();
			load_ani(16);
			init_ani(0);
			ds_writew(0x2846, 0);
		}

		answer = GUI_radio(get_ltx(0xaa0), 4,
					get_ltx(0xa90), get_ltx(0xa94),
					get_ltx(0xa98), get_ltx(0x994));

		if (answer == 4 || answer == -1) {
			done = 1;
		} else {

			/* set up parameters for this merchant */
			ds_writeb(0x6870 + 90 * 9 + 0, ds_readb(0x70aa + 8 * ds_readws(TYPEINDEX)));
			ds_writeb(0x6870 + 90 * 9 + 2, ds_readb(0x70ae + 8 * ds_readws(TYPEINDEX)));
			ds_writeb(0x6870 + 90 * 9 + 1, answer);
			type_bak = ds_readws(TYPEINDEX);
			ds_writew(TYPEINDEX, 90);

			/* visit this merchant */
			do_merchant();

			/* change back to market */
			ds_writeb(LOCATION, 9);

			/* clean up */
			ds_writew(TYPEINDEX, type_bak);
			ds_writeb(0x2d7c, bak1);
			ds_writeb(DIRECTION, dir_bak);
			ds_writeb(0x6870 + 90 * 9 + 0, 0);
			ds_writeb(0x6870 + 90 * 9 + 2, 0);
			ds_writeb(0x6870 + 90 * 9 + 1, 0);
		}

	} while (!done);

	ds_writeb(LOCATION, 0);
	copy_palette();
}

#if !defined(__BORLANDC__)
}
#endif
