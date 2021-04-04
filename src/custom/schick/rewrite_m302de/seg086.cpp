/**
 *	Rewrite of DSA1 v3.02_de functions of seg086 (dungeon: pirate cave on Manrek)
 *	Functions rewritten: 16/16 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg086.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg027.h"
#include "seg032.h"
#include "seg075.h"
#include "seg076.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG11_handler(void)
{
	signed short target_pos;
	signed short test_result;
	signed short answer;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;

	amap_ptr = p_datseg + DNG_MAP;
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	hero = Real2Host(get_first_hero_available_in_group());

	if (target_pos == 0x60c && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readb(DNG11_SOUP_SUPPLY) != 0)
	{
		if (GUI_radio(get_tx(1), 2, get_tx(2), get_tx(3)) == 1)
		{
			add_group_le(1);

			dec_ds_bs_post(DNG11_SOUP_SUPPLY);

			if (random_schick(100) <= 50)
			{
				ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), 0x82f))));
				do_fight(FIGHTS_F131_01B);
			}
		}

	} else if (target_pos == 0xb0e && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG_waterbarrel(p_datseg + DNG11_WATERBARREL1);

	} else if (target_pos == 0xc0e && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		DNG_waterbarrel(p_datseg + DNG11_WATERBARREL2);

	} else if (target_pos == 0x609 && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		answer = GUI_radio(get_tx(4), 6,
					get_tx(5),
					get_tx(6),
					get_tx(7),
					get_tx(8),
					get_tx(9),
					get_tx(10));

		ds_writeb(DNG11_LEVER_FLAG, 0);

		if (answer >= 1 && answer <= 5)
		{
			ds_writeb(DNG11_LEVER_FLAG, (signed char)answer);

			GUI_output(get_tx(11));
		}

	} else if (target_pos == 0x909)
	{
		if (target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readb(DNG11_EFFERD_HINT))
		{
			GUI_output(get_tx(12));

		} else if (ds_readb(DNG11_EFFERD_SACRIFICE) != 0 && !ds_readb(DNG11_EFFERD_HINT))
		{
			ds_writeb(DNG11_EFFERD_HINT, 1);
			ds_writeb(DNG11_UNKNOWN2_FLAG, ds_writeb(DNG11_EFFERD_SACRIFICE, 0));

			GUI_output(get_tx(13));
		}

	} else if (target_pos == 0xc0a && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readb(DNG11_FIRETRAP1_FLAG) != 0)
	{
		if (ds_readb(DNG11_EFFERD_HINT) != 0 || ds_readb(DNG11_UNKNOWN1_FLAG) != 0)
		{
			GUI_output(get_tx(15));

			dec_ds_bs_post(DNG11_FIRETRAP1_FLAG);

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					sub_hero_le(hero, test_attrib(hero, ATTRIB_GE, 4) <= 0 ? dice_roll(6, 6, 6) : dice_roll(3, 6, 3));
				}
			}
		}

	} else if (target_pos == 0xb08 && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readb(DNG11_FIRETRAP2_FLAG) != 0)
	{
		if (ds_readb(DNG11_UNKNOWN2_FLAG) != 0)
		{
			GUI_output(get_tx(15));

			dec_ds_bs_post(DNG11_FIRETRAP2_FLAG);

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					sub_hero_le(hero, test_attrib(hero, ATTRIB_GE, 4) <= 0 ? dice_roll(6, 6, 6) : dice_roll(3, 6, 3));
				}
			}
		}

	} else if (target_pos == 0xb03 && target_pos != ds_readws(DNG_HANDLED_POS) && ds_readbs(DIRECTION) == 1)
	{
		GUI_output(get_tx(17));

	} else if (((target_pos == 0xc03 && ds_readbs(DIRECTION) == 1) ||
			(target_pos == 0xb02 && ds_readbs(DIRECTION) == 0)) &&
			target_pos != ds_readws(DNG_HANDLED_POS))
	{
			GUI_output(get_tx(16));

	} else if ((target_pos == 0x600 || target_pos == 0x700 || target_pos == 0x800 || target_pos == 0x900) &&
			target_pos != ds_readws(DNG_HANDLED_POS))
	{
		/* boat. if entered, the complete group is drowned */
		if (GUI_radio(get_tx(18), 2, get_tx(19), get_tx(20)) == 1)
		{
			GUI_output(get_tx(21));
			GUI_output(get_tx(22));

			load_ani(18);
			init_ani(0);

			GUI_output(get_tx(23));

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					hero_disappear(hero, answer, -1);
				}
			}

			set_var_to_zero();

		} else {
			ds_writew(X_TARGET, ds_readws(X_TARGET_BAK));
			ds_writew(Y_TARGET, ds_readws(Y_TARGET_BAK));
		}

	} else if (target_pos == 0x80c &&
			(target_pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)) &&
			ds_readbs(DIRECTION) == 0 &&
			ds_readb(DNG11_SECRETDOOR1_FLAG) != 2)
	{
		if (ds_readb(DNG11_SECRETDOOR1_FLAG) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 8) > 0)
		{
			ds_writeb(DNG11_SECRETDOOR1_FLAG, 1);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(26),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)((ds_readb(DNG11_LEVER_FLAG) == 2 || ds_readb(DNG11_LEVER_FLAG) == 3) && (test_result = test_skill(hero, TA_SCHLOESSER, 5)) > 0 ? get_tx(27) : get_tx(28)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			strcat((char*)Real2Host(ds_readd(DTP2)), (char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			if (test_result > 0)
			{
				and_ptr_bs(amap_ptr + 0xb8, 0x0f);

				ds_writeb(DNG11_SECRETDOOR1_FLAG, 2);

				DNG_update_pos();
			}

			ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));
		}

	} else if (target_pos == 0x407 &&
			(target_pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)) &&
			ds_readbs(DIRECTION) == 1 &&
			ds_readb(DNG11_SECRETDOOR2_FLAG) != 2)
	{
		if (ds_readb(DNG11_SECRETDOOR2_FLAG) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 4) > 0)
		{
			ds_writeb(DNG11_SECRETDOOR2_FLAG, 1);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(26),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)((ds_readb(DNG11_LEVER_FLAG) == 1 || ds_readb(DNG11_LEVER_FLAG) == 3) && (test_result = test_skill(hero, TA_SCHLOESSER, 3)) > 0 ? get_tx(27) : get_tx(28)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			strcat((char*)Real2Host(ds_readd(DTP2)), (char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			if (test_result > 0)
			{
				and_ptr_bs(amap_ptr + 0x75, 0x0f);

				ds_writeb(DNG11_SECRETDOOR2_FLAG, 2);

				DNG_update_pos();
			}

			ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));
		}

	} else if (target_pos == 0x908 &&
			(target_pos != ds_readws(DNG_HANDLED_POS) || ds_readbs(DIRECTION) != ds_readbs(DIRECTION_BAK)) &&
			ds_readbs(DIRECTION) == 1 &&
			ds_readb(DNG11_SECRETDOOR3_FLAG) != 2)
	{
		if (ds_readb(DNG11_SECRETDOOR3_FLAG) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 6) > 0)
		{
			ds_writeb(DNG11_SECRETDOOR3_FLAG, 1);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(26),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)((ds_readb(DNG11_LEVER_FLAG) == 5 || ds_readb(DNG11_LEVER_FLAG) == 3) && (test_result = test_skill(hero, TA_SCHLOESSER, 5)) > 0 ? get_tx(27) : get_tx(28)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			strcat((char*)Real2Host(ds_readd(DTP2)), (char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			if (test_result > 0)
			{
				and_ptr_bs(amap_ptr + 0x8a, 0x0f);

				ds_writeb(DNG11_SECRETDOOR3_FLAG, 2);

				DNG_update_pos();
			}

			ds_writeb(DIRECTION_BAK, ds_readbs(DIRECTION));
		}

	} else if (target_pos == 0x80f && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		leave_dungeon();

		ds_writeb(CURRENT_TOWN, ds_readbs(TRV_DEST_REACHED));
		ds_writew(X_TARGET, ds_readws(ARRIVAL_X_TARGET));
		ds_writew(Y_TARGET, ds_readws(ARRIVAL_Y_TARGET));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readbs(ARRIVAL_DIRECTION) + 2) & 0x03);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx(29),
			(char*)get_ttx(ds_readw(TRV_DESTINATION) + 0xeb));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(3));

		ds_writeb(FADING_STATE, 3);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(DNG_HANDLED_POS, target_pos);

	return 0;
}

void DNG11_chest1(void)
{
	if (!ds_readb(DNG11_PROVIANT1_FLAG))
	{
		/* a chest with 50 food rations */
		GUI_output(get_tx(24));

		ds_writeb(DNG11_PROVIANT1_FLAG, 1);

	} else {

		/* an empty chest */
		GUI_output(get_ttx(522));
	}
}

void DNG11_chest2(void)
{
	if (!ds_readb(DNG11_PROVIANT2_FLAG))
	{
		/* a chest with 21 food rations */
		GUI_output(get_tx(25));

		ds_writeb(DNG11_PROVIANT2_FLAG, 1);

	} else {

		/* an empty chest */
		GUI_output(get_ttx(522));
	}
}

void DNG11_chest3(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST3));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest4(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST4));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest5(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST5));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest6(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST6));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest7(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST7));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest8(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST8));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest9(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST9));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest10(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST10));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest11(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST11));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest12(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST12));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest13(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST13));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest14(RealPt chest)
{
        RealPt ptr_bak;

        ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
        host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG11_CHEST14));
        loot_simple_chest(Real2Host(chest));
        host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

/**
 * \brief   print introductions of dungeon #11 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG11_fight_intro(signed short fight_id)
{
	if (fight_id == 127) {
		GUI_output(get_tx(14));
	}
}

#if !defined(__BORLANDC__)
}
#endif
