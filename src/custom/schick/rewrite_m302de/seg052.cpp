/**
 *	Rewrite of DSA1 v3.02_de functions of seg052 (citycamp)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg052.cpp
 */
#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg027.h"
#include "seg029.h"
#include "seg032.h"
#include "seg047.h"
#include "seg049.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief	camp in a city or a dungeon
 */
void do_citycamp(void)
{
	signed short l_si;
	signed short l_di;
	signed short done;
	signed short answer;
	signed short l3;
	RealPt hero;
	signed char hours;
	signed short l5;
	signed short l6;
	signed short l7;
	signed char l8;

	done = 0;

	l3 = ds_writew(0x2846, 1);

	for (l_si = 0; l_si <= 6; l_si++) {
		ds_writeb(0xe3e1 + l_si, ds_writeb(0xe3e8 + l_si, 0));
	}

	for (l_si = 0; l_si < 3; l_si++) {
		ds_writeb(0xe3de + l_si, -1);
	}

	draw_loc_icons(5, 9, 25, 11, 17, 8);

	while (done == 0) {

		if (ds_readw(0x2846) != 0) {
			draw_main_screen();
			set_var_to_zero();
			load_ani(36);
			init_ani(0);
			GUI_print_loc_line(get_ltx(0x4c8));
			set_audio_track(ARCHIVE_FILE_CAMP_XMI);
			ds_writew(0x2846, l3 = 0);
		}

		if (l3 != 0) {
			GUI_print_loc_line(get_ltx(0x4c8));
			l3 = 0;
		}

		handle_gui_input();

		if (ds_readws(0xc3d3) != 0 || ds_readws(ACTION) == 73) {

			/* open citycamp radio menu */
			answer = GUI_radio(get_ltx(0x4cc), 5,
						get_ltx(0x4d0),
						get_ltx(0x350),
						get_ltx(0x4d8),
						get_ltx(0x4f0),
						get_ltx(0x75c)) - 1;

			/* set action on a valid answer */
			if (answer != -2) {
				ds_writew(ACTION, answer + 129);
			}
		}

		if (ds_readws(ACTION) == 129) {

			answer = -1;

			for (l_si = 0; l_si <= 6; l_si++) {

				if (!ds_readbs(0xe3e1 + l_si) && check_hero(get_hero(l_si))) {
					ds_writeb(0xe3e8 + l_si, 0);
					answer = 0;
				}
			}

			if (answer == -1) {
				GUI_output(get_ltx(0x530));
			} else {

				for (l_si = 0; l_si < 3; l_si++) {

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x504),
						l_si + 1);

					do {

						answer = select_hero_ok(Real2Host(ds_readd(DTP2)));

						if (answer != -1 && ds_readbs(0xe3e1 + answer) != 0) {
							GUI_output(get_ltx(0x52c));
							answer = -1;
						}

						if (answer != -1 && hero_busy(get_hero(answer))) {
							GUI_output(get_ltx(0xb68));
							answer = -1;
						}

					} while (answer == -1);

					inc_ds_bs_post(0xe3e8 + answer);
					ds_writebs(0xe3de + l_si, (signed char)answer);
				}
			}

		} else if (ds_readws(ACTION) == 130) {

			GUI_use_talent2(0, get_ltx(0x62c));

		} else if (ds_readws(ACTION) == 131) {

			answer = select_hero_ok(get_ltx(0x4f4));

			if (answer != -1 && hero_busy(get_hero(answer))) {
				GUI_output(get_ltx(0xb68));
				answer = -1;
			}

			if (answer != -1) {

				hero = (RealPt)ds_readd(HEROS) + SIZEOF_HERO * answer;

				if (host_readbs(Real2Host(hero) + HERO_TYPE) >= 7) {

					if (ds_readb(0xe3e8 + answer) != 0) {
						GUI_output(get_ltx(0x52c));
					} else {
						if (ds_readb(0xe3e1 + answer) != 0) {
							GUI_output(get_ltx(0x538));
						} else {
							ds_writebs(0xe3e1 + answer, (signed char)use_magic(hero));
						}
					}
				} else {
					GUI_output(get_ltx(0x528));
				}
			}
		} else if (ds_readws(ACTION) == 132) {

			if (GUI_bool(get_ltx(0x4f8))) {

				hours = (signed char)GUI_input(get_ltx(0xd04), 1);

				if (hours > 0) {
					l5 = hours / 3;
					l6 = l5;
					l_di = 0;

					if (ds_readws(0x434f) == -1) {
						if ((ds_readbs(0xe3de) == -1 ? 4 * hours : hours) > random_schick(100)) {
							ds_writews(0x434f, random_schick(3) - 1);
						}
					}

					l8 = 0;
					l7 = hours;

					if (ds_readbs(0xe3de + l_di) != -1) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0xc18),
							(char*)get_hero(ds_readbs(0xe3de + l_di)) + 0x10);

						GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
					}

					do {
						ds_writeb(FOOD_MOD, 1);
						timewarp(HOURS(1));
						ds_writeb(FOOD_MOD, 0);

						l6--;
						l8++;
						l7--;

						if (l_di == ds_readws(0x434f) && (l5 / 2) >= l6) {
							done = 1;
						}

						if (l6 == 0 && l_di < 2) {

							l6 = l5;
							l_di++;

							if (ds_readbs(0xe3de + l_di) != -1) {

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_ltx(0xc18),
									(char*)get_hero(ds_readbs(0xe3de + l_di)) + 0x10);

								GUI_print_loc_line(Real2Host(ds_readd(DTP2)));
							}
						}

					} while (l7 > 0 && done == 0);

					if (done != 0) {

						ds_writew(0x434f, -1);

						if (ds_readb(0xbd27) == 0) {
							/* in a dungeon */

							ds_writeb(0x26ac, 1);
							ds_writew(0x26c1, 1);

							do_fight(ds_readws((DCAMPFIGHTS-2) + 2 * random_schick(4)));

							if (ds_readws(0xc3c1) == 0) {
								draw_main_screen();
								set_var_to_zero();
								load_ani(36);
								init_ani(0);
								GUI_print_loc_line(get_ltx(0x4c8));
								set_audio_track(ARCHIVE_FILE_CAMP_XMI);
								ds_writew(0x2846, l3 = 0);
							}

						} else {
							/* in a city */
							done = 0;
							loose_random_item(get_hero(get_random_hero()), 100, get_ltx(0xd00));
						}

						if (l7 > 0) {
							ds_writeb(FOOD_MOD, 1);
							timewarp(HOURS(l7));
							ds_writeb(FOOD_MOD, 0);
						}
					}

					if (done == 0) {

						hero = (RealPt)ds_readd(HEROS);
						for (l_si = 0; l_si <= 6; l_si++, hero += SIZEOF_HERO) {

							if (host_readbs(Real2Host(hero) + HERO_TYPE) != 0 &&
								host_readbs(Real2Host(hero) + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								ds_readbs(0xe3e8 + l_si) < 2 &&
								ds_readbs(0xe3e1 + l_si) != 1)
							{
								GRP_hero_sleep(Real2Host(hero), hours - 10);
							}
						}
					}
				}
				done = 1;
			}

		} else if (ds_readws(ACTION) == 133) {
			done = 1;
		}
	}

	ds_writeb(0x2d9f, 0);
	turnaround();
}

#if !defined(__BORLANDC__)
}
#endif
