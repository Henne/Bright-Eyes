/**
 *	Rewrite of DSA1 v3.02_de functions of seg077 (dungeon: the ship of death)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg077.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg003.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg032.h"
#include "seg075.h"
#include "seg076.h"
#include "seg092.h"
#include "seg095.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
#include "t_map.h"
#endif

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG01_handler(void)
{
	signed short target_pos;
	signed short i;
	Bit8u *hero;


	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	if (target_pos == 0x108)
	{
		DNG_fallpit_test(6);

	} else if (target_pos == 0x1309)
	{
		dec_ds_ws_post(X_TARGET);
		DNG_fallpit_test(6);

	} else if (target_pos == 0x1e05)
	{
		inc_ds_ws_post(Y_TARGET);
		DNG_fallpit_test(6);

	} else if (target_pos == 0x2706 || target_pos == 0x2709)
	{
		DNG_stub6();

	} else if (target_pos == 0x3e07)
	{
		DNG_fallpit_test(6);

	} else if (target_pos == 0x4706)
	{
		DNG_fallpit_test(6);
		inc_ds_ws_post(Y_TARGET);

	} else if (target_pos == 0x1d05 && target_pos != ds_readws(0x330e) && !ds_readbs(DNG01_SABRE_TAKEN))
	{
		sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
			(char*)get_ltx(0x840),
			(char*)Real2Host(GUI_names_grammar(0, 3, 0)),
			(char*)Real2Host(GUI_2f2(2, 3, 0)));

		/* ITEM: get a SABRE */
		if (GUI_bool(Real2Host(ds_readfp(TEXT_OUTPUT_BUF))) && get_item(3, 1, 1))
		{
			ds_writeb(DNG01_SABRE_TAKEN, 1);
		}

	} else if (target_pos == 0x3209 && target_pos != ds_readws(0x330e) && !ds_readbs(DNG01_CROSSBOW_TAKEN))
	{
		sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
			(char*)get_ltx(0x840),
			(char*)Real2Host(GUI_names_grammar(0, 12, 0)),
			(char*)Real2Host(GUI_2f2(2, 12, 0)));

		/* ITEM: get a CROSSBOW */
		if (GUI_bool(Real2Host(ds_readfp(TEXT_OUTPUT_BUF))) && get_item(12, 1, 1))
		{
			ds_writeb(DNG01_CROSSBOW_TAKEN, 1);
		}

	} else if (target_pos == 0x4209 && target_pos != ds_readws(0x330e) && !ds_readbs(DNG01_AMULET_TAKEN))
	{
		/* ITEM: a magic AMULET */
		if (GUI_bool(get_dtp(0x1c)) && get_item(174, 1, 1))
		{
			ds_writeb(DNG01_AMULET_TAKEN, 1);
			sub_ds_ds(GODS_ESTIMATION + 4 * 5, 100L);
		}

	} else if (target_pos == 0x4d06 && target_pos != ds_readws(0x330e))
	{
		seg092_06b4(0);

	} else if (ds_readbs((0xbd6e + 1)) == 2)
	{
		/* TRAP: heros may loose 2 LE with probability 10% */
		if (mod_day_timer(MINUTES(10)))
		{
			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (random_schick(100) <= 10 &&
					host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					sub_hero_le(hero, 2);

					sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
						(char*)get_dtp(0x30),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));
				}
			}
		}

	} else if (target_pos == 0x5108 && !ds_readbs(DNG01_ARDORA_FREED))
	{
		/* NPC: ARDORA */
		ds_writew(TEXTBOX_WIDTH, 7);

		load_in_head(21);

		if (GUI_dialogbox(ds_readfp(DTP2), get_ltx(0xbd0), get_dtp(0x20), 2,
				get_ltx(0x08), get_ltx(0x0c)) == 1)
		{
			ds_writew(TEXTBOX_WIDTH, 3);

			GUI_dialogbox(ds_readfp(DTP2), get_ltx(0xbd0), get_dtp(0x24), 0);

			if (host_readbs(get_hero(6) + HERO_TYPE) != HERO_TYPE_NONE)
			{
				remove_npc(host_readbs(get_hero(6) + HERO_NPC_ID) + 19,
						31,
						host_readbs(get_hero(6) + HERO_NPC_ID) + ARCHIVE_FILE_NSC_LTX,
						NULL, NULL);
			}

			add_npc(ARCHIVE_FILE_NSC_LTX + 4);
		} else {
			ds_writew(TEXTBOX_WIDTH, 3);

			GUI_dialogbox(ds_readfp(DTP2), get_ltx(0xbd0), get_dtp(0x28), 0);
		}

		ds_writeb(DNG01_ARDORA_FREED, 1);

		add_hero_ap_all(20);

	} else if (target_pos == 0x5e07 && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x2c)))
		{
			/* check if a ROPE LADDER or a ROPE is available */
			/* Original-Bug: Why not check for a mage with staffspell level >= 3? */
			if (get_first_hero_with_item(121) != -1 ||
				get_first_hero_with_item(32) != -1)
			{
				/* Original-Bug: better get_first_hero_available_in_group() */
				if (test_skill(get_hero(0), 10, 0) > 0)
				{
					ds_writeb(DNG_MAP + 32 * ds_readws(Y_TARGET) + ds_readws(X_TARGET), 0x50);
					DNG_dec_level();
					ds_writeb(DNG_MAP + 32 * ds_readws(Y_TARGET) + ds_readws(X_TARGET), 0x60);
					inc_ds_ws_post(Y_TARGET);
				}
			} else {
				GUI_output(get_ltx(0x844));
			}
		}

	} else if (target_pos == 0x280b && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x60)))
		{
			leave_dungeon();
			ds_writebs(CURRENT_TOWN, (signed char)ds_readws(TRV_DEST_REACHED));
			ds_writew(X_TARGET, ds_readws(0x433a));
			ds_writew(Y_TARGET, ds_readws(0x433c));
			ds_writeb(LOCATION, 0);
			ds_writeb(DIRECTION, (ds_readws(0x433e) + 2) & 3);

			GUI_output(get_dtp(0x3c));

			if (ds_readws(DEATHTRAP) == 1 && ds_readws(DEATHTRAP_STEPS) != 0)
			{
				load_ani(18);
				init_ani(1);
				set_var_to_zero();
				GUI_output(get_dtp(0x40));
			} else {
				GUI_output(get_dtp(0x44));
			}

			ds_writeb(0x45b8, 1);
			timewarp(HOURS(6));
			ds_writeb(0x4475, 3);
		}
	}

	ds_writew(0x330e, target_pos);

	return 0;
}

void DNG01_chest1_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG01_CHEST1_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG01_chest2_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG01_CHEST2_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG01_chest3_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG01_CHEST3_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG01_chest5_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG01_CHEST5_CONTENT));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG01_chest7_x1(RealPt chest)
{
	loot_corpse(chest, get_dtp(0x10), p_datseg + DNG01_CORPSE_LOOTED);
}

void DNG01_chest7_x2(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, DNG01_CHEST7_CONTENT));
	loot_chest(Real2Host(chest), get_dtp(0x14), get_dtp(0x18));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG01_chest6_x3(RealPt chest)
{
	/* ITEM: the GOLDEN KEY */
	get_item(219, 1, 1);

/* Original-Bug: The string 14 from SHIP.DTX needs a pointer to the name of the hero, not an integer.
*/
#ifdef M302de_ORIGINAL_BUGFIX
	sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
		(char*)get_dtp(0x38),
		(char*)Real2Host(get_first_hero_available_in_group()));
#else
	sprintf((char*)Real2Host(ds_readfp(TEXT_OUTPUT_BUF)),
		(char*)get_dtp(0x38), 10);
#endif
	print_msg_with_first_hero(Real2Host(ds_readfp(TEXT_OUTPUT_BUF)));

	ds_writeb(DNG01_KEY_TAKEN, 1);
}

void DNG01_chest6_x2(RealPt chest)
{
	GUI_output(get_dtp(0x0c));
}

void DNG01_chest6_x1(RealPt chest)
{
	if (!ds_readbs(DNG01_KEY_TAKEN))
	{
		GUI_input(get_dtp(0x08), 10);

		/* compare if the user wrote MARBO */
		if (!strcmp((char*)Real2Host(ds_readfp(TEXT_INPUT_BUF)),
				(char*)p_datseg + DNG01_STR_MARBO))
		{
			// correct answer
#if defined(__BORLANDC__)
			((void (*)(RealPt))((RealPt)host_readd(Real2Host(chest) + 11)))(chest);
#else
			t_map(chest, 11)(chest);
#endif

			// Original-Bug: enable the extra loot, here 10 AP
#ifdef M302de_ORIGINAL_BUGFIX
			ds_writew(GET_EXTRA_LOOT, 1);
#endif
		} else if ((RealPt)host_readd(Real2Host(chest) + 7) != 0){
#if defined(__BORLANDC__)
			((void (*)(void))((RealPt)host_readd(Real2Host(chest) + 7)))();
#else
			// wrong answer
			((treasure_trap)(t_map(chest, 7)))();
#endif
		}

	} else {
		GUI_output(get_ltx(0x828));
	}
}

void DNG01_chest0_x1(RealPt chest)
{
	if (!ds_readb(0x43a0) && GUI_bool(get_dtp(0x04)))
	{
		ds_writeb(0x43a0, 1);

		/* enable deathtrap */
		ds_writew(DEATHTRAP_STEPS, 30);
		ds_writew(DEATHTRAP, 1);

		/* fight the demon */
		ds_writew((0xd325 + 0), ds_writew((0xd325 + 2), ds_writew((0xd325 + 4), ds_writew((0xd325 + 6), 0x1d3a))));

		if (!do_fight(FIGHTS_SHIP10_1))
		{
			add_hero_ap_all(50);
		}

		/* this quest is done */
		ds_writeb(QUEST_DEADSHIP_DONE, 1);
	}
}

#if !defined(__BORLANDC__)
}
#endif
