/**
 *	Rewrite of DSA1 v3.02_de functions of seg086 (dungeon: piratecave)
 *	Functions rewritten: 2/16
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg027.h"
#include "seg032.h"
#include "seg075.h"
#include "seg076.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
signed short DNG11_handler(void)
{
	signed short target_pos;
	signed short test_result;
	signed short answer;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;

	amap_ptr = p_datseg + 0xbd95;
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	hero = Real2Host(get_first_hero_available_in_group());

	if (target_pos == 0x60c && target_pos != ds_readws(0x330e) && ds_readb(0x43a2) != 0)
	{
		if (GUI_radio(get_dtp(0x04), 2, get_dtp(0x08), get_dtp(0x0c)) == 1)
		{
			add_group_le(1);

			dec_ds_bs_post(0x43a2);

			if (random_schick(100) <= 50)
			{
				ds_writew((0xd325 + 0), ds_writew((0xd325 + 2), ds_writew((0xd325 + 4), ds_writew((0xd325 + 6), 0x82f))));
				do_fight(FIGHTS_F131_01B);
			}
		}

	} else if (target_pos == 0xb0e && target_pos != ds_readws(0x330e))
	{
		DNG_waterbarrel(p_datseg + 0x415d);

	} else if (target_pos == 0xc0e && target_pos != ds_readws(0x330e))
	{
		DNG_waterbarrel(p_datseg + 0x415e);

	} else if (target_pos == 0x609 && target_pos != ds_readws(0x330e))
	{
		answer = GUI_radio(get_dtp(0x10), 6,
					get_dtp(0x14),
					get_dtp(0x18),
					get_dtp(0x1c),
					get_dtp(0x20),
					get_dtp(0x24),
					get_dtp(0x28));

		ds_writeb(0x4168, 0);

		if (answer >= 1 && answer <= 5)
		{
			ds_writeb(0x4168, (signed char)answer);

			GUI_output(get_dtp(0x2c));
		}

	} else if (target_pos == 0x909)
	{
		if (target_pos != ds_readws(0x330e) && !ds_readb(EFFERD_HINT))
		{
			GUI_output(get_dtp(0x30));

		} else if (ds_readb(EFFERD_SACRIFICE) != 0 && !ds_readb(EFFERD_HINT))
		{
			ds_writeb(EFFERD_HINT, 1);
			ds_writeb(0x4163, ds_writeb(EFFERD_SACRIFICE, 0));

			GUI_output(get_dtp(0x34));
		}

	} else if (target_pos == 0xc0a && target_pos != ds_readws(0x330e) && ds_readb(0x4162) != 0)
	{
		if (ds_readb(EFFERD_HINT) != 0 || ds_readb(0x4161) != 0)
		{
			GUI_output(get_dtp(0x3c));

			dec_ds_bs_post(0x4162);

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != 0 &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					sub_hero_le(hero, test_attrib(hero, 4, 4) <= 0 ? dice_roll(6, 6, 6) : dice_roll(3, 6, 3));
				}
			}
		}

	} else if (target_pos == 0xb08 && target_pos != ds_readws(0x330e) && ds_readb(0x4164) != 0)
	{
		if (ds_readb(0x4163) != 0)
		{
			GUI_output(get_dtp(0x3c));

			dec_ds_bs_post(0x4164);

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != 0 &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{
					sub_hero_le(hero, test_attrib(hero, 4, 4) <= 0 ? dice_roll(6, 6, 6) : dice_roll(3, 6, 3));
				}
			}
		}

	} else if (target_pos == 0xb03 && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 1)
	{
		GUI_output(get_dtp(0x44));

	} else if (((target_pos == 0xc03 && ds_readbs(DIRECTION) == 1) ||
			(target_pos == 0xb02 && ds_readbs(DIRECTION) == 0)) &&
			target_pos != ds_readws(0x330e))
	{
			GUI_output(get_dtp(0x40));

	} else if ((target_pos == 0x600 || target_pos == 0x700 || target_pos == 0x800 || target_pos == 0x900) &&
			target_pos != ds_readws(0x330e))
	{
		if (GUI_radio(get_dtp(0x48), 2, get_dtp(0x4c), get_dtp(0x50)) == 1)
		{
			GUI_output(get_dtp(0x54));
			GUI_output(get_dtp(0x58));

			load_ani(18);
			init_ani(0);

			GUI_output(get_dtp(0x5c));

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != 0 &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					hero_disappear(hero, answer, -1);
				}
			}

			set_var_to_zero();

		} else {
			ds_writew(X_TARGET, ds_readws(0x2d83));
			ds_writew(Y_TARGET, ds_readws(0x2d85));
		}

	} else if (target_pos == 0x80c &&
			(target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)) &&
			ds_readbs(DIRECTION) == 0 &&
			ds_readb(0x4165) != 2)
	{
		if (ds_readb(0x4165) != 0 || test_skill(hero, 51, 8) > 0)
		{
			ds_writeb(0x4165, 1);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x68),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)((ds_readb(0x4168) == 2 || ds_readb(0x4168) == 3) && (test_result = test_skill(hero, 48, 5)) > 0 ? get_dtp(0x6c) : get_dtp(0x70)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			strcat((char*)Real2Host(ds_readd(DTP2)), (char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			if (test_result > 0)
			{
				and_ptr_bs(amap_ptr + 0xb8, 0x0f);

				ds_writeb(0x4165, 2);

				DNG_update_pos();
			}

			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}

	} else if (target_pos == 0x407 &&
			(target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)) &&
			ds_readbs(DIRECTION) == 1 &&
			ds_readb(0x4166) != 2)
	{
		if (ds_readb(0x4166) != 0 || test_skill(hero, 51, 4) > 0)
		{
			ds_writeb(0x4166, 1);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x68),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)((ds_readb(0x4168) == 1 || ds_readb(0x4168) == 3) && (test_result = test_skill(hero, 48, 3)) > 0 ? get_dtp(0x6c) : get_dtp(0x70)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			strcat((char*)Real2Host(ds_readd(DTP2)), (char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			if (test_result > 0)
			{
				and_ptr_bs(amap_ptr + 0x75, 0x0f);

				ds_writeb(0x4166, 2);

				DNG_update_pos();
			}

			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}

	} else if (target_pos == 0x908 &&
			(target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)) &&
			ds_readbs(DIRECTION) == 1 &&
			ds_readb(0x4167) != 2)
	{
		if (ds_readb(0x4167) != 0 || test_skill(hero, 51, 6) > 0)
		{
			ds_writeb(0x4167, 1);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0x68),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)((ds_readb(0x4168) == 5 || ds_readb(0x4168) == 3) && (test_result = test_skill(hero, 48, 5)) > 0 ? get_dtp(0x6c) : get_dtp(0x70)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			strcat((char*)Real2Host(ds_readd(DTP2)), (char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			if (test_result > 0)
			{
				and_ptr_bs(amap_ptr + 0x8a, 0x0f);

				ds_writeb(0x4167, 2);

				DNG_update_pos();
			}

			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}

	} else if (target_pos == 0x80f && target_pos != ds_readws(0x330e))
	{
		leave_dungeon();

		ds_writeb(CURRENT_TOWN, ds_readbs(0x4338));
		ds_writew(X_TARGET, ds_readws(0x433a));
		ds_writew(Y_TARGET, ds_readws(0x433c));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readbs(0x433e) + 2) & 0x03);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0x74),
			(char*)get_ltx(4 * (ds_readw(0x434a) + 0xeb)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(3));

		ds_writeb(0x4475, 3);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(0x330e, target_pos);

	return 0;
}

/**
 * \brief print introductions of dungeon #11 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG11_fight_intro(signed short fight_id)
{
	if (fight_id == 127) {
		GUI_output(get_dtp(0x38));
	}
}

#if !defined(__BORLANDC__)
}
#endif
