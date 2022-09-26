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
 * \brief   check if hero can healed by a healer
 *
 * \param   hero        pointer to hero
 * \return              1 if hero can be healed, else 0.
 */
unsigned short is_hero_healable(Bit8u *hero)
{

	if (hero_dead(hero) || hero_petrified(hero)) {
		/* this hero can not be helped */
		GUI_output(get_ttx(778));
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

#if !defined(__BORLANDC__)
	const Bit8u typi = ds_readb(TYPEINDEX);
	const Bit8s h_price = ds_readb(HEALER_DESCR_TABLE + 2 * typi);
	const Bit8u h_qual = ds_readb(HEALER_DESCR_TABLE + 1 + 2 * typi);

	D1_INFO("Heiler: %2d\n", typi);
	D1_INFO("\tPreis: %d%% [50,150]\n", h_price + 100);
	D1_INFO("\tQualitaet: %2d [1,15]\n", h_qual);
#endif

	v6 = ds_writew(REQUEST_REFRESH, 1);
	info = p_datseg + HEALER_DESCR_TABLE + ds_readw(TYPEINDEX) * 2;
	draw_loc_icons(4, MENU_ICON_HEAL_WOUNDS, MENU_ICON_HEAL_DISEASE, MENU_ICON_HEAL_POISON, MENU_ICON_LEAVE);

	while (leave_healer == 0) {

		if (ds_readw(REQUEST_REFRESH) != 0) {
			draw_main_screen();
			set_var_to_zero();
			load_ani(23);
			init_ani(0);

			GUI_print_loc_line(get_tx(ds_readw(CITYINDEX)));

			set_audio_track(ARCHIVE_FILE_HEALER_XMI);

			ds_writew(REQUEST_REFRESH, v6 = 0);

			if (!motivation) {

				motivation = 1;

				/* from 9.00 pm to 6.00 am the healer gets unkind */
				if (ds_readds(DAY_TIMER) > 0x1baf8 || ds_readds(DAY_TIMER) < 0x7e90) {
					GUI_output(get_ttx(484));
					motivation = 2;
				}
			}
		}

		if (v6 != 0) {
			GUI_print_loc_line(get_tx(ds_readw(CITYINDEX)));
			v6 = 0;
		}

		handle_gui_input();

		if (ds_readw(MOUSE2_EVENT) != 0 || ds_readw(ACTION) == ACTION_ID_PAGE_UP) {

			ds_writew(TEXTBOX_WIDTH, 4);

			answer = GUI_radio(get_ttx(459), 4,
						get_ttx(455),
						get_ttx(456),
						get_ttx(457),
						get_ttx(458)) - 1;
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
			answer = select_hero_from_group(get_ttx(460));
			if (answer != - 1) {

				hero = get_hero(answer);

				if (is_hero_healable(hero)) {

					/* LEmax >= LE and no permanent LEdamage */
					if (host_readws(hero + HERO_LE) >= host_readws(hero + HERO_LE_ORIG)
						&& !host_readbs(hero + HERO_LE_MOD)) {

						/* Hero seems OK */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(461),
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
							(char*)get_ttx(464),
							(char*)(hero + HERO_NAME2),
							price);

						if (GUI_bool(Real2Host(ds_readd(DTP2)))) {
							price *= 10;

							if (money < price) {
								GUI_output(get_ttx(401));
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
									(char*)get_ttx(467),
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
			answer = select_hero_from_group(get_ttx(460));
			if (answer != -1) {
				hero = get_hero(answer);

				if (is_hero_healable(hero)) {
					disease = hero_is_diseased(hero);

					if (!disease) {
						/* Hero is not diseased */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(462),
							(char*)(hero + HERO_NAME2));

						GUI_output(Real2Host(ds_readd(DTP2)));
					} else {
						/* calculate price */
						price = ds_readws(DISEASE_PRICES + disease * 2) * 10;

						price += (host_readbs(info) * price) / 100;

						if (motivation == 2)
							price *= 2;

						/* prepare output */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(465),
							(char*)(hero + HERO_NAME2),
							(char*)get_ttx(disease + 0x193),
							price);

						if (GUI_bool(Real2Host(ds_readd(DTP2)))) {
							price *= 10;

							if (money < price) {
								GUI_output(get_ttx(401));
							} else {
								/* let pass some time */
								timewarp(5400);

								if (random_schick(100) <= (120 - host_readbs(info + 1) * 10) + ds_readws(DISEASE_DELAYS + disease * 2)) {
									/* heal the disease */
									host_writeb(hero + (HERO_ILLNESS) + disease * SIZEOF_HERO_ILLNESS, 1);
									host_writeb(hero + (HERO_ILLNESS + 1) + disease * SIZEOF_HERO_ILLNESS, 0);

									/* prepare output */
									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ttx(467),
										(char*)(hero + HERO_NAME2));

									GUI_output(Real2Host(ds_readd(DTP2)));
								} else {
									price /= 2;
									GUI_output(get_ttx(468));
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
			answer = select_hero_from_group(get_ttx(460));
			if (answer != -1) {
				hero = get_hero(answer);

				if (is_hero_healable(hero)) {
					poison = hero_is_poisoned(hero);

					if (poison == 0) {
						/* Hero is not poisoned */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(463),
							(char*)(hero + HERO_NAME2));

						GUI_output(Real2Host(ds_readd(DTP2)));
					} else {
						/* calculate price */
						price = ds_readws(POISON_PRICES + poison * 2) * 20;
						price += (host_readbs(info) * price) / 100;
						if (motivation == 2)
							price *= 2;

						/* prepare output */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(466),
							price,
							(char*)(hero + HERO_NAME2));

						if (GUI_bool(Real2Host(ds_readd(DTP2)))) {
							price *= 10;

							if (money < price) {
								GUI_output(get_ttx(401));
							} else {
								timewarp(5400);

								if (random_schick(100) <= (120 - host_readbs(info + 1) * 5) + ds_readws(POISON_DELAYS + poison * 2)) {
									/* cure the poison */
									host_writeb(hero + (HERO_POISON + 1) + poison * SIZEOF_HERO_POISON, 0);
									host_writeb(hero + (HERO_POISON) + poison * SIZEOF_HERO_POISON, 1);

									/* prepare output */
									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ttx(467),
										(char*)(hero + HERO_NAME2));

									GUI_output(Real2Host(ds_readd(DTP2)));
								} else {
									price /= 2;
									GUI_output(get_ttx(468));
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
	leave_location();

}

#if !defined(__BORLANDC__)
}
#endif
