/**
 *	Rewrite of DSA1 v3.02_de functions of seg049 (group management)
 *	Functions rewritten: 3/9
 */

#include <stdlib.h>

#include "v302de.h"

#include "seg029.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
int GRP_compare_heros(const void *p1, const void *p2)
{
	Bit8u *hero1, *hero2;

	hero1 = (Bit8u*)p1;
	hero2 = (Bit8u*)p2;

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) == ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) == ds_readbs(CURRENT_GROUP)))
	{
		if (host_readbs(hero1 + 0x8a) < host_readbs(hero2 + 0x8a))
		{
			return -1;
		} else {
			return 1;
		}
	}

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) == ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) != ds_readbs(CURRENT_GROUP)))
	{
		return -1;
	}

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) != ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) == ds_readbs(CURRENT_GROUP)))
	{
		return 1;
	}

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) != ds_readbs(CURRENT_GROUP)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) != ds_readbs(CURRENT_GROUP)))
	{
		if (host_readbs(hero1 + 0x8a) < host_readbs(hero2 + 0x8a))
		{
			return -1;
		} else {
			return 1;
		}
	}

	if (!(host_readbs(hero1 + 0x21)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) == ds_readbs(CURRENT_GROUP)))
	{
		return 1;
	}

	if (!(host_readbs(hero1 + 0x21)) &&
		(host_readbs(hero2 + 0x21) != 0) &&
		(host_readbs(hero2 + 0x87) != ds_readbs(CURRENT_GROUP)))
	{
		return -1;
	}

	if (!(host_readbs(hero1 + 0x21)) &&
		!(host_readbs(hero2 + 0x21)))
	{
		return 0;
	}

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) == ds_readbs(CURRENT_GROUP)) &&
		!(host_readbs(hero2 + 0x21)))
	{
		return -1;
	}

	if ((host_readbs(hero1 + 0x21) != 0) &&
		(host_readbs(hero1 + 0x87) != ds_readbs(CURRENT_GROUP)) &&
		!(host_readbs(hero2 + 0x21)))
	{
		return 1;
	}

	return 0;
}

/* Borlandified and identical */
void GRP_sort_heros(void)
{
	signed short i;

	qsort((void*)get_hero(0), 6, 0x6da, GRP_compare_heros);

	for (i = 0; i < 6; i++) {
		host_writeb(get_hero(i) + 0x8a, i + 1);
	}
}

/* Borlandified and identical */
void GRP_save_pos(signed short group)
{
	signed short refresh = 0;

	if (group & 0x8000) {
		refresh = 1;
	}

	group &= 0x7fff;

	GRP_sort_heros();

	ds_writeb(0x2d3e + group, ds_readbs(DIRECTION));

	ds_writew(0x2d48 + group * 2, ds_readws(X_TARGET));
	ds_writew(0x2d54 + group * 2, ds_readws(Y_TARGET));

	ds_writeb(0x2d61 + group, ds_readbs(LOCATION));
	ds_writeb(0x2d68 + group, ds_readbs(CURRENT_TOWN));
	ds_writeb(0x2d6f + group, ds_readbs(DUNGEON_INDEX));
	ds_writeb(0x2d76 + group, ds_readbs(DUNGEON_LEVEL));
	ds_writeb(0x2d7d + group, ds_readbs(0x2d7c));

	ds_writew(0x2d87 + group * 2, ds_readws(0x2d83));
	ds_writew(0x2d93 + group * 2, ds_readws(0x2d85));

	ds_writeb(0x2da0 + group, ds_readbs(0x2d9f));
	ds_writeb(0x2da7 + group, ds_readbs(0x2da6));
	ds_writeb(0x2dae + group, ds_readbs(0x2dad));
	ds_writeb(0x2db5 + group, ds_readbs(0x2db4));

	if (!refresh) {
		draw_status_line();
	}
}

#if !defined(__BORLANDC__)
}
#endif
