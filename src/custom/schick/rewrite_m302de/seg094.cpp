/**
 *	Rewrite of DSA1 v3.02_de functions of seg094 (travelmode)
 *	Functions rewritten: 1/11
 */

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg094.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void set_textbox_positions(signed short town_id)
{
	signed short x;
	signed short r_dx;
	signed short y;

	/* zero some global variables */
	ds_writew(0xe4a5, ds_writew(0xe4a3, ds_writew(0xe4ad, ds_writew(0xe4ab, 0))));


	x = ds_writews(0xe4b1, ds_readws(TOWN_POSITIONS + 4 * town_id));
	y = ds_writews(0xe4af, ds_readws(TOWN_POSITIONS + 4 * town_id + 2));

	r_dx = (x >= 0 && x <= 159) ?
			(y >= 0 && y <= 99 ? 3 : 1) :
			(y >= 0 && y <= 99 ? 2 : 0);

	ds_writew(0x2ca2, !r_dx || r_dx == 2 ? -80 : 80);
	ds_writew(0x2ca4, !r_dx || r_dx == 1 ? -40 : 40);
}

#if !defined(__BORLANDC__)
}
#endif
