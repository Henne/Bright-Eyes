/**
 *	Rewrite of DSA1 v3.02_de functions of seg088 (dungeon: thorwal 2 / 2)
 *	Functions rewritten: 12/12 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg088.cpp
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


void DNG14_dive(signed short diver_pos, signed char mod, signed short dest_x)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (i != diver_pos &&
			host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero))
		{

			if (test_skill(hero, TA_SCHWIMMEN, mod) <= 0) {
				/* swimming failed */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0xa8),
					(char*)hero + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

				GUI_output(Real2Host(ds_readd(DTP2)));

				/* loose 2W6 LE */
				sub_hero_le(hero, dice_roll(2, 6, 0));
			} else {
				/* swimming succeeded */

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0xac),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}
	}

	ds_writew(X_TARGET, dest_x);
	ds_writews(DNG_REFRESH_DIRECTION, -1);

}

/**
 * \brief print introductions of dungeon #14 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG14_fight_intro(signed short fight_id)
{
	if ((fight_id == 172) || (fight_id == 174)) {
		GUI_output(get_dtp(0xe0));
	}
}

void DNG14_chest_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG14_CHEST_X1));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x2(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG14_CHEST_X2));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x3(RealPt chest)
{
	loot_multi_chest(p_datseg + DNG14_CHEST_X3, get_dtp(0xf8));
}

void DNG14_chest_x4(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG14_CHEST_X4));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x5(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG14_CHEST_X5));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x6(RealPt chest)
{
	RealPt ptr_bak;
	unsigned char x;
	unsigned char y;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG14_CHEST_X6));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);

	if (ds_readb(DNG14_FULLMAP_FLAG) != 0) {

		ds_writeb(DNG14_FULLMAP_FLAG, 0);

		GUI_output(get_dtp(0x100));

		for (x = 0; x < 16; x++) {
			for (y = 0; y < 16; y++) {
				set_automap_tile(x, y);
			}
		}
	}
}

void DNG14_chest_x7(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG14_CHEST_X7));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x8(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG14_CHEST_X8));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x9(void)
{
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());
	print_msg_with_first_hero(get_dtp(0xf4));
	sub_hero_le(hero, dice_roll(2, 6, 0));
}

void DNG15_riddle(void)
{
	signed short i;
	signed short l_di;
	signed short pos;
	signed short tw_bak;
	Bit8u *ptr;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 8);

	ptr = p_datseg + DNG_MAP;

	pos = (ds_readbs(DUNGEON_LEVEL) << 12) + (ds_readws(X_TARGET) << 8) + ds_readws(Y_TARGET);

	/* check if the other group is in position */
	for (i = l_di = 0; i < 6; i++) {

		if (pos == 0x1801 && ds_readws(GROUPS_X_TARGET + 2 * i) == 8 &&
			ds_readws(GROUPS_Y_TARGET + 2 * i) == 5 && ds_readbs(CURRENT_GROUP) != i)
		{
			l_di = 1;
		}

		if (pos == 0x1805 && ds_readws(GROUPS_X_TARGET + 2 * i) == 8 &&
			ds_readws(GROUPS_Y_TARGET + 2 * i) == 1 && ds_readbs(CURRENT_GROUP) != i)
		{
			l_di = 1;
		}
	}

	if (!l_di || !ds_readb(DNG15_REACHED_HANDS)) {

		/* INFO: you hear a soft cracking noise */
		GUI_output(get_dtp(0x70));

	} else {

		/* ask which lever should be pulled */
		do {
			i = GUI_radio(get_dtp(0x74), 2,
					get_dtp(0x78),
					get_dtp(0x7c));
		} while (i == -1);

		if (i == 2) {
			/* pull on the other side */

			/* set the corresponding lever */
			if (ds_readws(Y_TARGET) == 1) {
				ds_writeb(DNG15_LEVER_SOUTH, 1);
			} else if (ds_readws(Y_TARGET) == 5) {
				ds_writeb(DNG15_LEVER_NORTH, 1);
			}

			if (ds_readb(DNG15_LEVER_SOUTH) != 0 && ds_readb(DNG15_LEVER_NORTH) != 0)
			{
				/* riddle solved: remove the door from the map */
				GUI_output(get_dtp(0x80));

				host_writeb(ptr + 0x39, 0x20);

				add_hero_ap_all(10);
			} else {
				/* INFO: you hear a soft cracking noise */
				GUI_output(get_dtp(0x70));
			}
		} else {
			/* pull on the this side */
			/* INFO: you hear a soft cracking noise */
			GUI_output(get_dtp(0x70));
		}
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
}

#if !defined(__BORLANDC__)
}
#endif
