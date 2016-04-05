/**
 *	Rewrite of DSA1 v3.02_de functions of seg080 (dungeon: wolvcave, cave)
 *	Functions rewritten: 3/9
 */

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg003.h"
#include "seg007.h"
#include "seg025.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
/**
 * \brief	dungeon handler of the wolfcave
 */
/* Borlandified and identical */
signed short DNG04_handler(void)
{
	signed short pos;
	signed short i;
	signed short tw_bak;
	Bit8u *hero;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

        pos = (ds_readbs(DUNGEON_LEVEL) << 12) + (ds_readws(X_TARGET) << 8) + ds_readws(Y_TARGET);

	if (pos == 0x607 && pos != ds_readws(0x330e) && !ds_readb(0x3ca0))
	{
		/* DNG04_corpse0 */
		seg092_06b4(0);

	} else if (pos == 0x808 && pos != ds_readws(0x330e) && !ds_readb(0x3ca3))
	{
		/* do you want to grab into the gap ? */
		do {
			i = GUI_radio(get_dtp(0x08), 2,
						get_dtp(0x0c),
						get_dtp(0x10));
		} while (i == -1);

		if (i == 2)
		{
			hero = Real2Host(get_first_hero_available_in_group());

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x14),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			sub_hero_le(hero, 2);

			/* get a magic AMULET */
			get_item(175, 1, 1);

			ds_writeb(0x3ca3, 1);
		}

	} else if (pos == 0x50b && pos != ds_readws(0x330e) && !ds_readb(0x3ca4))
	{
		/* do you want the SILVER HELMET? */
		if (GUI_bool(get_dtp(0x1c)))
		{
			/* get a SILVER HELMET */
			if (get_item(213, 1, 1))
			{
				/* mark SILVER HELMET as taken */
				ds_writeb(0x3ca4, 1);
			}
		}

	} else if (pos == 0x10c && pos != ds_readws(0x330e))
	{
		/* DNG04_corpse1 */
		seg092_06b4(0);

	} else if (pos == 0x803 && pos != ds_readws(0x330e))
	{
		/* the ceiling may drop on your head */
		GUI_output(get_dtp(0x24));

		hero = get_hero(0);
		for (i = 0; i <= 6 ; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, 13, 2) <= 0)
			{
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x28),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_group_le(dice_roll(2, 6, 0));

				break;
			}
		}

	} else if (pos == 0x703 && pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x2c));

	} else if (pos == 0x505 && pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x30));

	} else if (pos == 0xa0d && pos != ds_readws(0x330e))
	{
		/* dig into a heap of dirt? */
		if (GUI_bool(get_dtp(0x34)) && GUI_bool(get_dtp(0x38)))
		{
			GUI_output(get_dtp(0x3c));

			i = get_free_mod_slot();

			/* Original-BUG: assumption the leader is at pos 0 */
			/* CH-5 for 1 day */
			set_mod_slot(i, DAYS(1), get_hero(0) + HERO_CH, -5, 0);
		}

	} else if (pos == 0xe0e && pos != ds_readws(0x330e))
	{
		/* the ceiling may drop on your head */
		GUI_output(get_dtp(0x24));

		hero = get_hero(0);
		for (i = 0; i <= 6 ; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, 13, 4) <= 0)
			{
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x28),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_group_le(dice_roll(1, 6, 2));

				break;
			}
		}

	} else if (pos == 0xa06 && pos != ds_readws(0x330e) && !ds_readb(0x3ca5))
	{
		/* do you want a LANTERN? */
		if (GUI_bool(get_dtp(0x40)))
		{
			/* get a LANTERN */
			if (get_item(25, 1, 1))
			{
				/* mark LANTERN as taken */
				ds_writeb(0x3ca5, 1);
			}
		}

	} else if (pos == 0xa02 && pos != ds_readws(0x330e))
	{
		/* a very deep gap */
		i = 1;
		hero = Real2Host(get_first_hero_available_in_group());

		if (!ds_readb(0x3ca6) || test_attrib(hero, 4, 0) > 0)
		{
			ds_writeb(0x3ca6, 1);

			i = GUI_bool(get_dtp(0x44));
		}

		if (i)
		{
			if (test_attrib(hero, 4, 0) > 0)
			{
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x48),
					(char*)hero + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)),
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

				GUI_output(Real2Host(ds_readd(DTP2)));

			} else {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x4c),
					(char*)hero + HERO_NAME2,
					(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				hero_disappear(hero, 0, -1);
			}
		}
	}

	if (pos == 0x601 && pos != ds_readws(0x330e) && !ds_readb(0x3ca2))
	{
		/* DNG04_corpse2 */
		seg092_06b4(0);

	} else if ((pos == 0xd06 || pos == 0xe06 || pos == 0xe05) && pos != ds_readws(0x330e))
	{
		/* Probability of 5% to get bitten by a rat */
		if (mod_day_timer(MINUTES(10)))
		{
			hero = get_hero(0);
			for (i = 0; i <= 6 ; i++, hero += SIZEOF_HERO)
			{
				if (random_schick(100) <= 5 &&
					host_readbs(hero + HERO_TYPE) != 0 &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					sub_hero_le(hero, 2);

					sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_dtp(0x54),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
				}
			}
		}

	} else if (ds_readb((0xbd6e + 1)) == 2)
	{
		/* Probability of 10% to get bitten by a rat */
		if (mod_day_timer(MINUTES(10)))
		{
			hero = get_hero(0);
			for (i = 0; i <= 6 ; i++, hero += SIZEOF_HERO)
			{
				/* Original-Bug: forgot to check if the hero is dead */
				if (random_schick(100) <= 10 &&
					host_readbs(hero + HERO_TYPE) != 0 &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					sub_hero_le(hero, 2);

					sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_dtp(0x64),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
				}
			}
		}

	} else if (pos == 0x70f && pos != ds_readws(0x330e))
	{
		/* the exit of this dungeon */
		leave_dungeon();

		ds_writeb(CURRENT_TOWN, ds_readb(0x4338));
		ds_writew(X_TARGET, ds_readws(0x433a));
		ds_writew(Y_TARGET, ds_readws(0x433c));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readb(0x433e) + 2) & 0x03);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x68),
			(char*)get_ltx(4 * (ds_readws(0x434a) + 0xeb)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(2));

		ds_writeb(0x4475, 3);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(0x330e, pos);

	return 0;
}
#endif

/* Borlandified and identical */
void DNG04_corpse0(RealPt ptr)
{
	loot_corpse(ptr, get_dtp(0x04), p_datseg + 0x3ca0);
}

/**
 * \brief print introductions of dungeon #5 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG5_fight_intro(signed short fight_id)
{
	if (fight_id == 76) {
		GUI_output(get_dtp(0x28));
	}
}

#if !defined(__BORLANDC__)
}
#endif
