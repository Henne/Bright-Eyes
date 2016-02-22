/*
 *	Rewrite of DSA1 v3.02_de functions of seg053 (healer)
 *	Functions rewritten: 2/2 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg053.cpp
*/

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg027.h"
#include "seg029.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * is_hero_healable() - check if hero can healed by a healer
 * @hero:	pointer to hero
 *
 * Returns 1 if hero can be healed, else 0.
 */
unsigned short is_hero_healable(Bit8u *hero)
{

	if (hero_dead(hero) || hero_stoned(hero)) {
		/* this hero can not be helped */
		GUI_output(get_ltx(0xc28));
		return 0;
	} else {
		return 1;
	}
}

void do_healer(void)
{
	signed char motivation;
	signed short leave_healer;
	signed short v6;
	Bit8u *hero;
	signed long money;
	signed long price;
	Bit8u *info;
	signed short poison;

	signed short answer;
	signed short disease;

	motivation = 0;
	leave_healer = 0;

	v6 = ds_writew(0x2846, 1);
	info = p_datseg + 0x66ea + ds_readw(TYPEINDEX) * 2;
	draw_loc_icons(4, 0x1e, 0x1f, 0x20, 8);

	while (leave_healer == 0) {

		if (ds_readw(0x2846) != 0) {
			draw_main_screen();
			set_var_to_zero();
			load_ani(23);
			init_ani(0);

			GUI_print_loc_line(get_dtp(ds_readw(CITYINDEX) * 4));

			set_audio_track(ARCHIVE_FILE_HEALER_XMI);

			ds_writew(0x2846, v6 = 0);

			if (!motivation) {

				motivation = 1;

				/* from 9.00 pm to 6.00 am the healer gets unkind */
				if (ds_readds(DAY_TIMER) > 0x1baf8 || ds_readds(DAY_TIMER) < 0x7e90) {
					GUI_output(get_ltx(0x790));
					motivation = 2;
				}
			}
		}

		if (v6 != 0) {
			GUI_print_loc_line(get_dtp(ds_readw(CITYINDEX) * 4));
			v6 = 0;
		}

		handle_gui_input();

		if (ds_readw(0xc3d3) != 0 || ds_readw(ACTION) == 0x49) {

			ds_writew(TEXTBOX_WIDTH, 4);

			answer = GUI_radio(get_ltx(0x72c), 4,
						get_ltx(0x71c),
						get_ltx(0x720),
						get_ltx(0x724),
						get_ltx(0x728)) - 1;
			ds_writew(TEXTBOX_WIDTH, 3);

			if (answer != -2) {
				ds_writew(ACTION, answer + 0x81);
			}
		}

		if (ds_readw(ACTION) == 0x84) {
			leave_healer = 1;
			continue;
		}

		if (ds_readw(ACTION) == 0x81) {

			/* Heal Wounds */

			money = get_party_money();
			answer = select_hero_from_group(get_ltx(0x730));
			if (answer != - 1) {

				hero = get_hero(answer);

				if (is_hero_healable(hero)) {

					/* LEmax >= LE and no permanent LEdamage */
					if (host_readws(hero + HERO_LE) >= host_readws(hero + HERO_LE_ORIG)
						&& !host_readbs(hero + HERO_LE_MOD)) {

						/* Hero seems OK */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x734),
							(char*)(hero + HERO_NAME2));
						GUI_output(Real2Host(ds_readd(DTP2)));
					} else {

						/* calculate price */
						price = host_readbs(hero + HERO_LE_MOD) * 50;
						price += (host_readws(hero + HERO_LE_ORIG) + host_readbs(hero + HERO_LE_MOD) - host_readws(hero + HERO_LE)) * 5;
						price += (host_readbs(info) * price) / 100;

						if (motivation == 2)
							price *= 2;

						/* ask */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x740),
							(char*)(hero + HERO_NAME2),
							price);

						if (GUI_bool(Real2Host(ds_readd(DTP2)))) {
							price *= 10;

							if (money < price) {
								GUI_output(get_ltx(0x644));
							} else {
								/* remove money */
								money -= price;
								set_party_money(money);

								/* heal permanent damage TODO:LE += */
								add_ptr_ws(hero + HERO_LE_ORIG, host_readbs(hero + HERO_LE_MOD));
								host_writeb(hero + HERO_LE_MOD, 0);

								/* let pass some time */
								timewarp(90 * (signed long)(host_readws(hero + HERO_LE_ORIG) - host_readws(hero + HERO_LE)));

								/* heal LE */
								add_hero_le(hero, host_readws(hero + HERO_LE_ORIG));

								/* prepare output */
								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_ltx(0x74c),
									(char*)(hero + HERO_NAME2));

								GUI_output(Real2Host(ds_readd(DTP2)));
							}
						}
					}
				}
			}

		} else if (ds_readw(ACTION) == 0x82) {
			/* Cure Disease */
			money = get_party_money();
			answer = select_hero_from_group(get_ltx(0x730));
			if (answer != -1) {
				hero = get_hero(answer);

				if (is_hero_healable(hero)) {
					disease = hero_is_diseased(hero);

					if (!disease) {
						/* Hero is not diseased */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x738),
							(char*)(hero + HERO_NAME2));

						GUI_output(Real2Host(ds_readd(DTP2)));
					} else {
						/* calculate price */
						price = ds_readws(0x2c50 + disease * 2) * 10;

						price += (host_readbs(info) * price) / 100;

						if (motivation == 2)
							price *= 2;

						/* prepare output */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x744),
							(char*)(hero + HERO_NAME2),
							(char*)get_ltx((disease + 0x193) * 4),
							price);

						if (GUI_bool(Real2Host(ds_readd(DTP2)))) {
							price *= 10;

							if (money < price) {
								GUI_output(get_ltx(0x644));
							} else {
								/* let pass some time */
								timewarp(5400);

								if (random_schick(100) <= (120 - host_readbs(info + 1) * 10) + ds_readws(0x2c60 + disease * 2)) {
									/* heal the disease */
									host_writeb(hero + HERO_ILLNESS_EMPTY + disease * 5, 1);
									host_writeb(hero + 0xaf + disease * 5, 0);

									/* prepare output */
									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ltx(0x74c),
										(char*)(hero + HERO_NAME2));

									GUI_output(Real2Host(ds_readd(DTP2)));
								} else {
									price /= 2;
									GUI_output(get_ltx(0x750));
								}

								/* remove money */
								money -= price;
								set_party_money(money);
							}
						}
					}
				}
			}
		} else if (ds_readw(ACTION) == 0x83) {
			/* Heal Poison */
			money = get_party_money();
			answer = select_hero_from_group(get_ltx(0x730));
			if (answer != -1) {
				hero = get_hero(answer);

				if (is_hero_healable(hero)) {
					poison = hero_is_poisoned(hero);

					if (poison == 0) {
						/* Hero is not poisoned */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x73c),
							(char*)(hero + HERO_NAME2));

						GUI_output(Real2Host(ds_readd(DTP2)));
					} else {
						/* calculate price */
						price = ds_readws(0x2c70 + poison * 2) * 20;
						price += (host_readbs(info) * price) / 100;
						if (motivation == 2)
							price *= 2;

						/* prepare output */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x748),
							price,
							(char*)(hero + HERO_NAME2));

						if (GUI_bool(Real2Host(ds_readd(DTP2)))) {
							price *= 10;

							if (money < price) {
								GUI_output(get_ltx(0x644));
							} else {
								timewarp(5400);

								if (random_schick(100) <= (120 - host_readbs(info + 1) * 5) + ds_readws(0x2c84 + poison * 2)) {
									/* cure the poison */
									host_writeb(hero + 0xd7 + poison * 5, 0);
									host_writeb(hero + HERO_POISON_EMPTY + poison * 5, 1);

									/* prepare output */
									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ltx(0x74c),
										(char*)(hero + HERO_NAME2));

									GUI_output(Real2Host(ds_readd(DTP2)));
								} else {
									price /= 2;
									GUI_output(get_ltx(0x750));
								}

								money -= price;
								set_party_money(money);
							}
						}
					}
				}
			}
		}
	}

	copy_palette();
	turnaround();

}

#if !defined(__BORLANDC__)
}
#endif
