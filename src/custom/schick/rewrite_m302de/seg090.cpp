/**
 *	Rewrite of DSA1 v3.02_de functions of seg090 (dungeon: oberorken mine)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg090.cpp
 *
 */

#include "v302de.h"

#include <stdio.h>

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg075.h"
#include "seg076.h"
#include "seg090.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG_handler_oberorken(void)
{
	signed short target_pos;
	signed short i;
	signed short tw_bak;
	Bit32s money;
	Bit8u *hero;
	Bit8u *ptr;

	ptr = p_datseg + 0xbd95;
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writews(TEXTBOX_WIDTH, 7);
	target_pos = (ds_readbs(DUNGEON_LEVEL) << 12)
			+ (ds_readws(X_TARGET) << 8)
			+ ds_readws(Y_TARGET);

	hero = Real2Host(get_first_hero_available_in_group());

	if (ds_readds(DAY_TIMER) % 450 == 0) {
		/* TODO: buggy timer for cave in */

		if (ds_readbs(0x3f9b) > 0) dec_ds_bs(0x3f9b);
		if (ds_readbs(0x3f9c) > 0) dec_ds_bs(0x3f9c);
		if (ds_readbs(0x3f9d) > 0) dec_ds_bs(0x3f9d);
		if (ds_readbs(0x3f9e) > 0) dec_ds_bs(0x3f9e);
	}

	if (target_pos == 0x1608 && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 1 && ds_readbs(0x3fa1) != 0) {
		/* secret door from water trap */
#if !defined(__BORLANDC__)
		D1_INFO("Geheimtuere\n");
#endif
		if (test_skill(hero, 0x33, 6) > 0) {

			GUI_output(get_dtp(0x54));

			and_ptr_bs(ptr + 0x87, 0xf);
			/* turn off water trap */
			ds_writeb(0x3fa6, 0);
			ds_writeb(0x3fa1, 0);

		} else {
			/* door not detected */
			if (GUI_bool(get_dtp(0x40))) GUI_output(get_dtp(0x44));
		}
	} else {

		if ((	target_pos == 0x1107 || target_pos == 0x1207 || target_pos == 0x1307 ||
			target_pos == 0x1407 || target_pos == 0x1507 || target_pos == 0x1607 ||
			target_pos == 0x1108 || target_pos == 0x1208 || target_pos == 0x1308 ||
			target_pos == 0x1408 || target_pos == 0x1508 || target_pos == 0x1608 ||
			target_pos == 0x1109 || target_pos == 0x1209 || target_pos == 0x1309 ||
			target_pos == 0x1409 || target_pos == 0x1509 || target_pos == 0x1609 )
			&& ds_readbs(0x3fa6) != 0)
		{
			/* water trap room, activate */
			ds_writeb(0x3fa1, 1);

			if (ds_readds(0x3fa2) / 450L != ds_readws(0x9d45)) {

				ds_writews(0x9d45, (signed short)(ds_readds(0x3fa2) / 450L));

				/* warning according to water level */
				GUI_output(ds_readds(0x3fa2) == 0 ? get_dtp(0x50) :
						(ds_readds(0x3fa2) <= 900 ? get_dtp(0x4c) : get_dtp(0x48)));

				if (ds_readds(0x3fa2) == 0) {
					/* time is up, drown party */
					hero = get_hero(0);
					for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

						if (host_readbs(hero + 0x21) != 0 &&
							host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
							!hero_dead(hero))
						{
							hero_disappear(hero, i, -1);
						}
					}
				} else if (ds_readds(0x3fa2) <= 3600) {
					/* NPC will find secret door */

					if (is_hero_available_in_group(get_hero(6))) {

						/* prepare a message with the name of the NPC */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_dtp(0x58),
								(char*)get_hero(6) + 0x10);

						GUI_output(Real2Host(ds_readd(DTP2)));

						while (ds_readws(X_TARGET) != 6) {

							add_ds_ws(X_TARGET,
								ds_readws(X_TARGET) > 6 ? -1 : 1);

							DNG_update_pos();
						}


						while (ds_readws(Y_TARGET) != 8) {

							add_ds_ws(Y_TARGET,
								ds_readws(Y_TARGET) > 8 ? -1 : 1);

							DNG_update_pos();
						}

						and_ptr_bs(ptr + 0x87, 0xf);
						/* turn off water trap */
						ds_writeb(0x3fa6, 0);
						ds_writeb(0x3fa1, 0);
						DNG_update_pos();
					}
				}
			}
		} else {
			/* clear water */
			ds_writeb(0x3fa1, 0);
			/* reset countdown */
			ds_writed(0x3fa2, 0x1199);
		}
	}

	if (target_pos == 0x0d09 && target_pos != ds_readws(0x330e) && !ds_readbs(0x3f9b)) {
		/* 1. tunnel block */
		DNG_clear_corridor(p_datseg + 0x3f9b);
	} else if (target_pos == 0x0e08 && target_pos != ds_readws(0x330e) && !ds_readbs(0x3f9c)) {
		/* 2. tunnel block */
		DNG_clear_corridor(p_datseg + 0x3f9c);
	} else if (target_pos == 0x0d07 && target_pos != ds_readws(0x330e) && !ds_readbs(0x3f9d)) {
		/* 3. tunnel block */
		DNG_clear_corridor(p_datseg + 0x3f9d);
	} else if (target_pos == 0x0e05 && target_pos != ds_readws(0x330e) && !ds_readbs(0x3f9e)) {
		/* 4. tunnel block */
		DNG_clear_corridor(p_datseg + 0x3f9e);
	} else if (target_pos == 0x0804 && target_pos != ds_readws(0x330e)) {

		/* upper Ingerimm idol */
		GUI_output(get_dtp(0x0c));

		if (GUI_bool(get_dtp(0x10))) {

			do {
				i = GUI_radio(get_dtp(0x14), 3,
						get_dtp(0x18), get_dtp(0x1c), get_dtp(0x20));
			} while (i == -1);

			if (i == 1) {
				/* lift arm */
#if !defined(__BORLANDC__)
				D1_INFO("nichts passiert\n");
#endif
				GUI_output(get_dtp(0x24));
			} else if (i == 2) {
				/* press arm */
#if !defined(__BORLANDC__)
				D1_INFO("Einsturz des Tunnels verhindern (buggy)\n");
#endif

				GUI_output(get_dtp(0x24));

				if (ds_readbs(0x3f9b) > 0) ds_writeb(0x3f9b, -1);
				if (ds_readbs(0x3f9c) > 0) ds_writeb(0x3f9c, -1);
				if (ds_readbs(0x3f9d) > 0) ds_writeb(0x3f9d, -1);
				if (ds_readbs(0x3f9e) > 0) ds_writeb(0x3f9e, -1);
			} else if (i == 3) {
				/* sacrifice gold */

				GUI_output(get_dtp(0x28));

				money = get_party_money();

				if (money >= 100) {
					money -= 100;
					set_party_money(money);
				}
#if !defined(__BORLANDC__)
				D1_INFO("nichts passiert\n");
#endif
			}
		}
	} else if (target_pos == 0x0503 && target_pos != ds_readws(0x330e)) {
		/* trap door */
		GUI_output(get_dtp(0x2c));
		DNG_fallpit(6);
	} else if (target_pos == 0x0406 && target_pos != ds_readws(0x330e)) {
		/* trap door */
		GUI_output(get_dtp(0x2c));
		ds_writews(X_TARGET, 1);
		ds_writews(Y_TARGET, 3);
		DNG_inc_level();
	} else if (target_pos == 0x060d && target_pos != ds_readws(0x330e)) {
		/* bolt trap */
		print_msg_with_first_hero(get_dtp(0x34));
		sub_hero_le(hero, random_schick(6));
	} else if (target_pos == 0x120e && target_pos != ds_readws(0x330e) && !ds_readb(INGERIMM_HINT)) {
		/* lower Ingerimm idol */
#if !defined(__BORLANDC__)
		D1_INFO("Untere Ingerimstatue\n");
#endif
		GUI_output(get_dtp(0x38));
	} else if (target_pos == 0x120e) {
#if !defined(__BORLANDC__)
		D1_INFO("Test auf Ingerimm-Opfer\n");
#endif
		if (ds_readb(INGERIMM_SACRIFICE) != 0 && !ds_readb(INGERIMM_HINT)) {
			/* hint to secret door */
			ds_writeb(INGERIMM_HINT, 1);
			ds_writeb(INGERIMM_SACRIFICE, 0);
			GUI_output(get_dtp(0x3c));
		}
	} else if (target_pos == 0x130a && target_pos != ds_readws(0x330e)) {
#if !defined(__BORLANDC__)
		D1_INFO("In Wasserfalle gefangen\n");
#endif
		if (div16(host_readb(ptr + 0xb3)) == 2) {
			and_ptr_bs(ptr + 0xb3, 0xf);
			or_ptr_bs(ptr + 0xb3, 0x10);
		}
	} else if (target_pos == 0x1307 && target_pos != ds_readws(0x330e)) {
#if !defined(__BORLANDC__)
		D1_INFO("In Wasserfalle gefangen\n");
#endif
		if (div16(host_readb(ptr + 0x63)) == 2) {
			and_ptr_bs(ptr + 0x63, 0xf);
			or_ptr_bs(ptr + 0x63, 0x10);
		}
	} else if (target_pos == 0x1108 && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 3) {
		/* water source */

		if (GUI_bool(get_dtp(0x40))) {
			GUI_output(get_dtp(0x44));
		}
	} else if (target_pos == 0x1302 && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 2) {
#if !defined(__BORLANDC__)
		D1_INFO("Rueckwaerts gehen\n");
#endif
		GUI_output(get_dtp(0x5c));
		ds_writew(X_TARGET, 1);
		ds_writew(Y_TARGET, 3);
		ds_writeb(DIRECTION, 3);
		DNG_update_pos();
	} else if (target_pos == 0x1203 && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 1) {
#if !defined(__BORLANDC__)
		D1_INFO("Rueckwaerts gehen\n");
#endif
		GUI_output(get_dtp(0x5c));
		ds_writew(X_TARGET, 5);
		ds_writew(Y_TARGET, 3);
		ds_writeb(DIRECTION, 1);
		DNG_update_pos();
	} else if (target_pos == 0x1403 && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 3) {
#if !defined(__BORLANDC__)
		D1_INFO("Rueckwaerts gehen\n");
#endif
		GUI_output(get_dtp(0x5c));
		ds_writew(X_TARGET, 3);
		ds_writew(Y_TARGET, 1);
		ds_writeb(DIRECTION, 0);
		DNG_update_pos();
	} else if (target_pos == 0x1b06 && target_pos != ds_readws(0x330e)) {
#if !defined(__BORLANDC__)
		D1_INFO("Illusionswand und Grube\n");
#endif

		if (GUI_bool(get_dtp(0x60))) {
			if (ds_readb(0x3fa8) != 0) {

				if (inc_ds_ws(0x9d43) < 3) {
					/* fall into pit */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x64),
						(char*)hero + 0x10,
						Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)),
						Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 2)));
				} else {
					/* detect obstacle */
					/* TODO: buggy output */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x7c),
						(char*)hero + 0x10,
						Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)),
						Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 2)),
						Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)),
						Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 2)));
				}

				/* try to break through */
				sub_ds_bs(0x3fa7, dice_roll(4, 6, 2));

				if (ds_readbs(0x3fa7) < 0) {
					/* obstacle broken */
					ds_writebs(0x3fa8, 0);

					/* TODO: buggy output*/
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x80),
						(char*)hero + 0x10);
				}

				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_hero_le(hero, random_schick(4));
				ds_writew(X_TARGET, ds_readw(0x2d83));
				ds_writew(Y_TARGET, ds_readw(0x2d85));
			} else {
				/* pass */
				GUI_output(get_dtp(0x68));
			}
		} else {
			ds_writew(X_TARGET, ds_readw(0x2d83));
			ds_writew(Y_TARGET, ds_readw(0x2d85));
		}
	} else if (target_pos == 0x1e03 && target_pos != ds_readws(0x330e) && ds_readb(0x3fa9) != 0) {
		/* spear trap */

		if (test_skill(hero, 0x33, 2) > 0) {

			if (GUI_bool(get_dtp(0x70))) {
				if (test_skill(hero, 0x30, 0) <= 0) {
					/* defusing trap failed */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x74),
						(char*)hero + 0x10);
					sub_hero_le(hero , dice_roll(3, 6, 0));
				} else {
					/* trap defused */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x78),
						(char*)hero + 0x10,
						(char*)hero + 0x10);
					ds_writeb(0x3fa9, 0);
					add_hero_ap(hero, 10);
				}
				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		} else {
			/* sprung trap */
			GUI_output(get_dtp(0x6c));
			sub_group_le(dice_roll(3, 6, 0));
		}
	} else if (target_pos == 0x0d0f && target_pos != ds_readws(0x330e)) {
		/* exit mine */
		leave_dungeon();
		ds_writew(X_TARGET, 1);
		ds_writew(Y_TARGET, 6);
		ds_writeb(DIRECTION, 0);
	}

	ds_writews(TEXTBOX_WIDTH, tw_bak);
	ds_writews(0x330e, target_pos);

	return 0;
}

void DNG_clear_corridor(Bit8u *ptr)
{
	/* ask if the corridor should be cleared */
	if (GUI_bool(get_dtp(0x4))) {

		/* clear the corridor */

		GUI_output(get_dtp(0x8));

		timewarp(0x1fa40L);

		host_writebs(ptr, -1);
	} else {
		ds_writew(X_TARGET, ds_readws(0x2d83));
		ds_writew(Y_TARGET, ds_readws(0x2d85));
	}
}

void DNG_oberorken_chest(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 0x0b);

	host_writed(Real2Host(chest) + 0x0b, (Bit32u)RealMake(datseg, 0x3faa));

	loot_simple_chest(Real2Host(chest));

	host_writed(Real2Host(chest) + 0x0b, (Bit32u)ptr_bak);
}

/**
 * \brief print introductions of dungeon #12 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG12_fight_intro(signed short fight_id)
{
	if (fight_id == 141) {
		GUI_output(get_dtp(0x30));
	}
}

#if !defined(__BORLANDC__)
}
#endif
