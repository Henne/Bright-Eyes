/**
 *	Rewrite of DSA1 v3.02_de functions of seg117 (2xTravelevent, hunt and helpers)
 *	Functions rewritten: 2/16
 */

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"

#include "seg004.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* static */
void pause_traveling(signed short ani_nr)
{

	ds_writeb(0xe5d2, 1);

	load_ani(ani_nr);

	draw_main_screen();

	init_ani(0);

	load_city_ltx(0xf0);

	ds_writew(0xe5d9, ds_readw(0x2ca2));
	ds_writew(0xe5d7, ds_readw(0x2ca4));
	ds_writew(0xe5d5, ds_readw(0xbffd));
	ds_writew(0xe5d3, ds_readw(0xe113));

	ds_writeb(0xb132, 1);

	/* c = b = a = 0 */
	ds_writew(0xe113, 0);
	ds_writew(0x2ca2, 0);
	ds_writeb(TRAVELING, 0);

	ds_writew(0x2ca4, ani_nr == 21 ? 60: 70);
	ds_writew(0xbffd, 9);

}

/* static */
void resume_traveling(void)
{
	ds_writew(0x2ca2, ds_readw(0xe5d9));
	ds_writew(0x2ca4, ds_readw(0xe5d7));
	ds_writew(0xbffd, ds_readw(0xe5d5));
	ds_writew(0xe113, ds_readw(0xe5d3));

	set_var_to_zero();

	/* b = a = 1 */
	ds_writeb(TRAVELING, 1);
	ds_writew(0x2846, 1);

	ds_writeb(0xe5d2, 0);
	ds_writeb(0xb132, 0);

}

#if !defined(__BORLANDC__)
}
#endif

