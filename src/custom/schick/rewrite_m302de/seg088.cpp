/**
 *	Rewrite of DSA1 v3.02_de functions of seg088 (dungeon: thorwal 2 / 2)
 *	Functions rewritten: 11/12
 */
#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg076.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif


/* Borlandified and identical */
void DNG14_dive(signed short diver_pos, signed char mod, signed short dest_x)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += 0x6da)
	{
		if (i != diver_pos &&
			host_readbs(hero + 0x21) != 0 &&
			host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero))
		{

			if (test_skill(hero, 14, mod) <= 0) {
				/* swimming failed */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0xa8),
					(char*)hero + 0x10,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)),
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)));

				GUI_output(Real2Host(ds_readd(DTP2)));

				/* loose 2W6 LE */
				sub_hero_le(hero, dice_roll(2, 6, 0));
			} else {
				/* swimming succeeded */

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0xac),
					(char*)hero + 0x10);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}
	}

	ds_writew(X_TARGET, dest_x);
	ds_writews(0xe482, -1);

}

/**
 * \brief print introductions of dungeon #14 fights
 *
 * \param fight_id	ID of the fight
 */
/* Borlandified and identical */
void DNG14_fight_intro(signed short fight_id)
{
	if ((fight_id == 172) || (fight_id == 174)) {
		GUI_output(get_dtp(0xe0));
	}
}

/* Borlandified and identical */
void DNG14_chest_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x3fe4));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/* Borlandified and identical */
void DNG14_chest_x2(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x3feb));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/* Borlandified and identical */
void DNG14_chest_x3(RealPt chest)
{
	loot_multi_chest(p_datseg + 0x3ffb, get_dtp(0xf8));
}

/* Borlandified and identical */
void DNG14_chest_x4(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x4006));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/* Borlandified and identical */
void DNG14_chest_x5(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x4014));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/* Borlandified and identical */
void DNG14_chest_x6(RealPt chest)
{
	RealPt ptr_bak;
	unsigned char x;
	unsigned char y;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x401e));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);

	if (ds_readb(0x403a) != 0) {

		ds_writeb(0x403a, 0);

		GUI_output(get_dtp(0x100));

		for (x = 0; x < 16; x++) {
			for (y = 0; y < 16; y++) {
				set_automap_tile(x, y);
			}
		}
	}
}

/* Borlandified and identical */
void DNG14_chest_x7(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x4026));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/* Borlandified and identical */
void DNG14_chest_x8(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x402d));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/* Borlandified and identical */
void DNG14_chest_x9(void)
{
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());
	print_msg_with_first_hero(get_dtp(0xf4));
	sub_hero_le(hero, dice_roll(2, 6, 0));
}

#if !defined(__BORLANDC__)
}
#endif
