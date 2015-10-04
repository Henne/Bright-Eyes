/**
 *	Rewrite of DSA1 v3.02_de functions of seg059 (tavern)
 *	Functions rewritten: 1/4
 */

#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg025.h"
#include "seg027.h"
#include "seg029.h"
#include "seg047.h"
#include "seg060.h"
#include "seg073.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void do_tavern(void)
{
	signed short i;
	signed short done;
	signed short answer;
	signed short l3;
	Bit32s p_money_before;
	Bit32s p_money_after;
	Bit8u *tav_ptr;
	signed short bonus;
	time_t timeval;

	done = 0;
	tav_ptr = p_datseg + 0x6c84 + 4 * ds_readws(TYPEINDEX);

	GUI_print_loc_line(get_dtp(4 * ds_readws(CITYINDEX)));

	ds_writew(0x2846, 1);

	if (host_readws(tav_ptr) >= 6 && host_readws(tav_ptr) <= 13) {

		if (ds_readds(DAY_TIMER) < HOURS(11) && ds_readds(DAY_TIMER) > HOURS(3)) {

			GUI_output(get_ltx(0xc84));
			turnaround();
			return;
		}

	} else if (ds_readds(DAY_TIMER) < HOURS(16) && ds_readds(DAY_TIMER) > HOURS(3)) {

			GUI_output(get_ltx(0x784));
			turnaround();
			return;
	}

	draw_loc_icons(ds_readws(COMBO_MODE) == 0 ? 4 : 5, 21, 13, 25, 8, 49);

	while (!done) {

		if (ds_readw(0x2846) != 0) {

			draw_main_screen();
			set_var_to_zero();
			load_ani(27);
			init_ani(0);
			GUI_print_loc_line(get_dtp(4 * ds_readws(CITYINDEX)));
			set_audio_track(147);
			ds_writew(0x2846, 0);
		}

		handle_gui_input();

		if (ds_readbs(0x318a + ds_readws(TYPEINDEX)) != 0) {

			GUI_output(get_ltx(0x760));
			done = 1;
			ds_writew(0xc3d3, ds_writew(ACTION, 0));
		}

		if (ds_readds(DAY_TIMER) < HOURS(11) && ds_readds(DAY_TIMER) > HOURS(3)) {

			GUI_output(get_ltx(0x24));
			done = 1;
			ds_writew(0xc3d3, ds_writew(ACTION, 0));
		}

		if (ds_readw(0xc3d3) != 0 || ds_readws(ACTION) == 73) {

			answer = GUI_radio(get_ltx(0x754), ds_readw(COMBO_MODE) == 0 ? 4 : 5,
						get_ltx(0x55c),
						get_ltx(0x758),
						get_ltx(0x350),
						get_ltx(0x75c),
						get_ltx(0xce0)) - 1;

			if (answer != -2) {
				ds_writew(ACTION, answer + 129);
			}
		}

		if (ds_readws(ACTION) == 129) {
			/* TALK */

			p_money_before = get_party_money();
			talk_tavern();
			p_money_after = get_party_money();

			if (p_money_before != p_money_after) {

				make_valuta_str((char*)Real2Host(ds_readd(0xd2eb)), p_money_before - p_money_after);

				sprintf((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0xce4),
					(char*)Real2Host(ds_readd(0xd2eb)));

				GUI_output(Real2Host(ds_readd(DTP2)));
			}

			if (ds_readb(0x360f) != 0) {

				tavern_follow_informer();
			}

			ds_writew(0x2846, done = 1);
			ds_writew(COMBO_MODE, 0);

		} else if (ds_readws(ACTION) == 130) {
			/* EAT AND DRINK */

			p_money_after = count_heroes_in_group() * (6 - host_readws(tav_ptr) / 4);

			p_money_after += host_readws(tav_ptr + 2) * p_money_after / 100;
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0x764),
				(signed short)p_money_after);

			if (GUI_bool(Real2Host(ds_readd(DTP2)))) {

				GUI_output(host_readws(tav_ptr) < 5 ? get_ltx(0x76c) : (
						host_readws(tav_ptr) < 15 ? get_ltx(0x770) : get_ltx(0x774)));

				timewarp(HOURS(1));

				for (i = 0; i <= 6; i++) {

					ds_writeb(FOOD_MESSAGE + i, ds_writeb(0x26a4 + i, 0));

					if (host_readbs(get_hero(i) + 0x21) != 0 &&
						host_readbs(get_hero(i) + 0x87) == ds_readbs(CURRENT_GROUP))
					{

						l3 = (21 - host_readws(tav_ptr)) * 20;
						if (l3 > 100) {
							l3 = 100;
						}

						sub_ptr_bs(get_hero(i) + 0x7f, l3);

						if (host_readbs(get_hero(i) + 0x7f) < 0) {
							host_writebs(get_hero(i) + 0x7f, 0);
						}

						l3 = (21 - host_readws(tav_ptr)) * 30;
						if (l3 > 100) {
							l3 = 100;
						}

						sub_ptr_bs(get_hero(i) + 0x80, l3);

						if (host_readbs(get_hero(i) + 0x80) < 0) {
							host_writebs(get_hero(i) + 0x80, 0);
						}
					}
				}


				p_money_before = get_party_money();
				p_money_after *= 10L;

				if (p_money_before < p_money_after) {

					GUI_output(get_ltx(0x768));

					ds_writeb(0x318a + ds_readws(TYPEINDEX), 1);
					done = 1;
					ds_writew(COMBO_MODE, 0);

				} else {

					p_money_before -= p_money_after;
					set_party_money(p_money_before);
				}
			}

		} else if (ds_readws(ACTION) == 131) {
			/* USE TALENT */

			bc_time(&timeval);

			bonus = (timeval - ds_readds(0xe2d6)) > 120 ? 0 : 50;

			if (GUI_use_talent2(bonus, get_ltx(0x62c)) == -1) {
				done = 1;
				ds_writew(COMBO_MODE, 0);
			}

		} else if (ds_readws(ACTION) == 132) {
			/* LEAVE */

			done = 1;
			ds_writew(COMBO_MODE, 0);

		} else if (ds_readws(ACTION) == 133) {
			/* VISIT INN */

			if (ds_readws(COMBO_MODE) != 0) {

				done = 1;
				ds_writew(COMBO_MODE, 2);
			}
		}
	}

	copy_palette();
	turnaround();
}

#if !defined(__BORLANDC__)
}
#endif
