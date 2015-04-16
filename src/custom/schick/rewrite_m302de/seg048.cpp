/*
 *      Rewrite of DSA1 v3.02_de functions of seg048 ()
 *      Functions rewritten: 1/3
 *
*/

#include "v302de.h"

#include "seg000.h"
#include "seg004.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void reset_item_selector(void)
{
	if (ds_readws(0x2c9b) < 3) {

		/* remove the previous border */
		do_border((RealPt)ds_readd(0xd2ff),
				ds_readws(0x64ce + 4 * ds_readbs(0x6370)),
				ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)),
				ds_readws(0x64ce + 4 * ds_readbs(0x6370)) + 17,
				ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)) + 17,
				30);

		/* mark the actual selected item */
		ds_writeb(0x636e, ds_writeb(0x6370, 7));

		/* set the new red border */
		do_border((RealPt)ds_readd(0xd2ff),
				ds_readws(0x64ce + 4 * ds_readbs(0x6370)),
				ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)),
				ds_readws(0x64ce + 4 * ds_readbs(0x6370)) + 17,
				ds_readws(0x64ce + 2 + 4 * ds_readbs(0x6370)) + 17,
				9);

		ds_writebs(0x6371, -1);
		ds_writebs(0x636f, 23);
	}

	ds_writed(0xcecb, ds_writed(0xe3ae, (Bit32u)RealMake(datseg, 0x2848)));
}

void seg048_00d0(signed short a1)
{
	DUMMY_WARNING();
}

void seg048_1498(void)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif
