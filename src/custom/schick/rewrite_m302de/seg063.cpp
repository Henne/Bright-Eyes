/*
 *	Rewrite of DSA1 v3.02_de functions of seg063 (harbour)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg063.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg029.h"
#include "seg030.h"
#include "seg049.h"
#include "seg059.h"
#include "seg063.h"
#include "seg064.h"
#include "seg075.h"
#include "seg097.h"
#include "seg119.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void passages_init(void)
{
	signed short si;
	signed short i;
	Bit8u *p = p_datseg + SEA_TRAVEL_PASSAGES;


	for (i = 0; i < 45; p += 8, i++) {

		host_writeb(p + 4, (unsigned char)random_interval(0, host_readbs(p + 3)));
		host_writeb(p + 7, (unsigned char)random_interval(70, 130));

		si = random_schick(100);

		if (!host_readbs(p + 5)) {

			/* Hochseerouten */
			host_writeb(p + 6, si <= 50 ? 0 : (si <= 80 ? 1 : (si <= 95 ? 2 : 3)));

		} else {

			/* Seerouten */
			host_writeb(p + 6, si <= 10 ? 4 : (si <= 40 ? 5 : (si <= 80 ? 6 : 7)));
		}

#if !defined(__BORLANDC__)
	D1_LOG("%16s - %16s: %d %d %d %d %d %d\n",
		(char*)get_ttx((host_readb(p + 0) + 0xeb) * 4),
		(char*)get_ttx((host_readb(p + 1) + 0xeb) * 4),
		host_readb(p + 2),
		host_readb(p + 3),
		host_readb(p + 4),
		host_readb(p + 5),
		host_readb(p + 6),
		host_readb(p + 7));
#endif
	}
}

struct dummy7 {
	signed short a[7];
};

void do_harbour(void)
{
	signed short l_si;
	signed short done;
	signed short answer;
	Bit8u *psg_ptr;
	Bit32s p_money;
	Bit8u *hero;
	signed char flag;
	Bit32s money;

	done = 0;
#if !defined(__BORLANDC__)
	struct dummy7 a = { { -2, 0, 5, 4, 3, 1, 0 } };
#else
	struct dummy7 a = *(struct dummy7*)(p_datseg + SEA_TRAVEL_SLEEPBONUS_TABLE1);
#endif
	flag = 1;

	ds_writew(CURRENT_DIRSIGN, ds_readws(TYPEINDEX));

	draw_loc_icons(4, 40, 42, 41, 8);
	ds_writew(REQUEST_REFRESH, 1);
	ds_writeb(TRAVEL_DETOUR, 0);

	do {

		if (ds_readw(REQUEST_REFRESH) != 0) {

			draw_main_screen();
			set_var_to_zero();
			load_ani(6);
			init_ani(0);
			ds_writew(REQUEST_REFRESH, 0);

			load_tx(ARCHIVE_FILE_HAFEN_LTX);

			if (flag != 0) {

				flag = 0;

				if (ds_readws(QUESTED_MONTHS) > 2 &&
					!ds_readbs((TREASURE_MAPS + 6)) &&
					ds_readb(INFORMER_FLAGS + INFORMER_SWAFNILD) &&
					random_schick(100) <= ds_readws(QUESTED_MONTHS) + 4)
				{
					/* meet SWAFNILD */
					do_talk(12, 0);

					if (ds_readb(SWAFNILD_TRAVELLED) != 0) {

						ds_writeb(SWAFNILD_TRAVELLED, 0);

						turnaround();

						return;
					}
				}
			}
		}

		handle_gui_input();

		if (ds_readw(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == 73) {

			answer = GUI_radio(get_tx(0x24), 4,
						get_tx(0x28), get_tx(0x2c),
						get_tx(0x30), get_tx(0x34)) - 1;

			if (answer != -2) {
				ds_writew(ACTION, answer + 129);
			}
		}

		if (ds_readws(ACTION) == 129) {

			answer = prepare_passages();

			if (answer == 0) {
				GUI_output(get_tx(0x3c));
			} else {

				/* select a destination */
				answer = GUI_radio(get_tx(0x38), (signed char)answer,
						get_ttx(4 * (ds_readb((SEA_TRAVEL_MENU_PASSAGES + 12 * 0 + 10)) + 235)),
						get_ttx(4 * (ds_readb((SEA_TRAVEL_MENU_PASSAGES + 12 * 1 + 10)) + 235)),
						get_ttx(4 * (ds_readb((SEA_TRAVEL_MENU_PASSAGES + 12 * 2 + 10)) + 235)),
						get_ttx(4 * (ds_readb((SEA_TRAVEL_MENU_PASSAGES + 12 * 3 + 10)) + 235)),
						get_ttx(4 * (ds_readb((SEA_TRAVEL_MENU_PASSAGES + 12 * 4 + 10)) + 235)),
						get_ttx(4 * (ds_readb((SEA_TRAVEL_MENU_PASSAGES + 12 * 5 + 10)) + 235)),
						get_ttx(4 * (ds_readb((SEA_TRAVEL_MENU_PASSAGES + 12 * 6 + 10)) + 235)),
						get_ttx(4 * (ds_readb((SEA_TRAVEL_MENU_PASSAGES + 12 * 7 + 10)) + 235)),
						get_ttx(4 * (ds_readb((SEA_TRAVEL_MENU_PASSAGES + 12 * 8 + 10)) + 235)),
						get_ttx(4 * (ds_readb((SEA_TRAVEL_MENU_PASSAGES + 12 * 9 + 10)) + 235))) - 1;

				if (answer != -2) {

					psg_ptr = p_datseg + 12 * answer + SEA_TRAVEL_MENU_PASSAGES;

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx(0x40),

						(char*)get_tx(4 * ds_readws(SEA_TRAVEL_TX_SHIP + 2 * host_readbs(psg_ptr + 9))),
						(char*)Real2Host(host_readds(psg_ptr + 0)),

						(char*)(!host_readbs(psg_ptr + 8) ? get_tx(0x14) : get_tx(0x18)),

						(char*)get_tx(4 * ds_readws(SEA_TRAVEL_TX_CLASS + 2 * ds_readbs(SEA_TRAVEL_QUALITY_TABLE + 4 * host_readbs(psg_ptr + 9)))),
						(char*)get_ttx(4 * (host_readb(psg_ptr + 10) + 235)),
						get_passage_travel_hours(host_readb(Real2Host(host_readd(psg_ptr + 4)) + 2), ds_readbs(SEA_TRAVEL_QUALITY_TABLE + 3 + 4 * host_readbs(psg_ptr + 9))),

						Real2Host(print_passage_price(ds_readbs(SEA_TRAVEL_QUALITY_TABLE + 2 + 4 * host_readbs(psg_ptr + 9)), Real2Host(host_readds(psg_ptr + 4)))));

					l_si = ds_readws(TEXTBOX_WIDTH);
					ds_writew(TEXTBOX_WIDTH, 5);

					answer = GUI_bool(Real2Host(ds_readd(DTP2)));

					ds_writew(TEXTBOX_WIDTH, l_si);

					if (answer != 0) {

						money = get_party_money();

						if (ds_readws(SEA_TRAVEL_PASSAGE_PRICE) > money) {

							GUI_output(get_ttx(0x644));

						} else {

							ds_writeb(SEA_TRAVEL_SLEEP_QUALITY, ds_readb(SEA_TRAVEL_QUALITY_TABLE + 4 * host_readbs(psg_ptr +9)));
							money -= ds_readws(SEA_TRAVEL_PASSAGE_PRICE);
							set_party_money(money);

							ds_writeb(SEA_TRAVEL_PSGBOOKED_TIMER, host_readb(psg_ptr + 8));
							ds_writeb(SEA_TRAVEL_PSGBOOKED_FLAG, 0xaa);
							ds_writeb(SEA_TRAVEL_PASSAGE_UNKN1, ds_readb(SEA_TRAVEL_PASSAGE_UNKN2));
							ds_writeb(SEA_TRAVEL_PASSAGE_ID, host_readb(psg_ptr + 11));

							GUI_output(host_readb(psg_ptr + 8) != 0 ? get_tx(0x48) : get_tx(0x44));
						}
					}
				}
			}

		} else if (ds_readws(ACTION) == 130) {

			if (ds_readds(DAY_TIMER) <= HOURS(6) || ds_readds(DAY_TIMER) >= HOURS(21)) {

				GUI_output(get_ttx(0x8e0));

			} else {

				load_in_head(11);

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(0x10),
									get_tx(0x00), 3,
									get_tx(0x4),
									get_tx(0x8),
									get_tx(0x0c));
				} while (answer == -1);

				p_money = get_party_money();

				if (p_money < 10L) {

					GUI_output(get_tx(0x20));

				} else if (answer == 1 || answer == 2) {

					p_money -= 10L;
					set_party_money(p_money);

					l_si  = answer;
					answer = get_next_passages(answer);

					if (answer != 0) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)(l_si == 1 ? get_tx(0x70) : get_tx(0x58)),
							(char*)(answer == 1 ? p_datseg + SEA_TRAVEL_STR_T : p_datseg + SEA_TRAVEL_STR_EN),
							(char*)(answer == 1 ? get_tx(0x5c) : get_tx(0x60)));

						l_si = 0;

						do {

							strcat((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(4 * (ds_readb((SEA_TRAVEL_MENU_PASSAGES + 10) + 12 * l_si++) + 235)));
							if (--answer) {

								strcat((char*)Real2Host(ds_readd(DTP2)),
									(char*)(answer >= 2 ? p_datseg + SEA_TRAVEL_STR_COMMA : get_tx(0x1c)));
							}

						} while (answer != 0);

						strcat((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_tx(0x64));

						GUI_output(Real2Host(ds_readd(DTP2)));

					} else {

						GUI_output(get_tx(0x68));
					}
				}
			}

		} else if (ds_readws(ACTION) == 131) {

			if (ds_readb(SEA_TRAVEL_PSGBOOKED_FLAG) != 0xaa) {

				GUI_output(get_tx(0x4c));

			} else if (ds_readb(SEA_TRAVEL_PSGBOOKED_TIMER) != 0) {

				GUI_output(ds_readbs(SEA_TRAVEL_PSGBOOKED_TIMER) == -1 ? get_tx(0x6c) : get_tx(0x50));

			} else {

				set_var_to_zero();

				GUI_output(get_tx(0x54));

				ds_writeb(TRAVEL_HEROKEEPING, 1);

				timewarp_until(HOURS(9));

				if (ds_readws(CHECK_DISEASE) != 0) {

					disease_effect();

					hero = get_hero(0);
					for (l_si = 0; l_si <= 6; l_si++, hero += SIZEOF_HERO) {

						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
						{
							GRP_hero_sleep(hero, a.a[ds_readbs(SEA_TRAVEL_SLEEP_QUALITY)]);

							host_writebs(hero + HERO_HUNGER,
								host_writebs(hero + HERO_THIRST, 0));
						}
					}
				}

				ds_writeb(TRAVEL_HEROKEEPING, 0);

				for (l_si = 0; l_si < 6; l_si++) {
					ds_writeb(FOOD_MESSAGE_SHOWN + l_si, 0);
				}

				load_map();

				ds_writews(CURRENT_ANI, -1);
				ds_writew(WALLCLOCK_UPDATE, 0);

				memmove(Real2Host(ds_readd(RENDERBUF_PTR)), Real2Host(ds_readd(TRAVEL_MAP_PTR)), 64000);
				map_effect(Real2Host(ds_readd(RENDERBUF_PTR)));

				wait_for_vsync();

				set_palette(Real2Host(ds_readd(TRAVEL_MAP_PTR)) + 64002, 0, 0x20);

				mod_clock_pos(ds_readbs(CURRENT_TOWN));

				set_audio_track(ARCHIVE_FILE_TERMS_XMI);

				ds_writew(WALLCLOCK_X, ds_readws(BASEPOS_X) + 120);
				ds_writew(WALLCLOCK_Y, ds_readws(BASEPOS_Y) + 87);
				ds_writew(WALLCLOCK_UPDATE, 1);

				sea_travel(ds_readb(SEA_TRAVEL_PASSAGE_ID), ds_readbs(SEA_TRAVEL_PASSAGES + 8 * ds_readb(SEA_TRAVEL_PASSAGE_ID)) == ds_readbs(CURRENT_TOWN) ? 0 : 1);
				passage_arrival();

				ds_writew(WALLCLOCK_UPDATE, ds_writew(BASEPOS_X, ds_writew(BASEPOS_Y, ds_writeb(SEA_TRAVEL_PSGBOOKED_FLAG, 0))));
				ds_writews(CURRENT_ANI, ds_writebs(CITY_AREA_LOADED, ds_writebs(PP20_INDEX, (signed char)0xff)));
				ds_writew(REQUEST_REFRESH, 1);
				ds_writeb(SHOW_TRAVEL_MAP, 0);

				if (!ds_readb(TRAVEL_DETOUR)) {

					ds_writebs(CURRENT_TOWN, (signed char)ds_readws(TRV_DEST_REACHED));
					ds_writew(X_TARGET_BAK, ds_readw(ARRIVAL_X_TARGET));
					ds_writew(Y_TARGET_BAK, ds_readw(ARRIVAL_Y_TARGET));
					ds_writeb(DIRECTION, (ds_readws(ARRIVAL_DIRECTION) + 2) & 3);

				} else {
					done = 1;
					DNG_enter_dungeon(1);
				}
			}

		} else if (ds_readws(ACTION) == 132) {
			done = 1;
		}

	} while (!done);

	l_si = load_archive_file(ARCHIVE_FILE_COMPASS);
	read_archive_file(l_si, Real2Host(ds_readd(BUFFER6_PTR)), 5000);
	bc_close(l_si);

	set_var_to_zero();

	if (!ds_readb(TRAVEL_DETOUR)) {

		copy_palette();
		turnaround();

	} else {

		ds_writeb(CURRENT_TOWN, 0);
	}
}

/**
 * \brief   * \param town_id	ID of the town
 */
void mod_clock_pos(signed short town_id)
{
	signed short val;
	signed short map_x;
	signed short map_y;

	map_x = ds_readws(TOWN_POSITIONS + 4 * town_id);
	map_y = ds_readws(TOWN_POSITIONS + 4 * town_id + 2);

	val = map_x >= 0 && map_x <= 159 ?
		(map_y >= 0 && map_y <= 99 ? 3 : 1) :
		(map_y >= 0 && map_y <= 99 ? 2 : 0);

	ds_writew(BASEPOS_X, !val || val == 2 ? -80 : 80);
	ds_writew(BASEPOS_Y, !val || val == 1 ? -40 : 40);
}

void sea_travel(signed short passage, signed short dir)
{
	signed short i;
	Bit8u *hero;
	RealPt ptr;
	Bit32s off;

#if !defined(__BORLANDC__)
	struct dummy7 a =  { { -2, 0, 5, 4, 3, 1, 0 } };
#else
	struct dummy7 a = *(struct dummy7*)(p_datseg + SEA_TRAVEL_SLEEPBONUS_TABLE2);
#endif

	ds_writeb(TRAVELING, 1);

	ds_writehp(SEA_TRAVEL_COURSES, (passage < 7 ? F_PADD(ds_readd(BUFFER9_PTR), 7600) : F_PADD(ds_readd(BUFFER9_PTR), 11400)));
	ds_writew(ROUTE_MOUSEHOVER, passage < 7 ? 7 : 38);
	ds_writew(SEA_TRAVEL_PASSAGE_NO, passage < 7 ? passage : passage - 7);

	off = host_readd(Real2Host(ds_readfp(SEA_TRAVEL_COURSES)) + 4 * ds_readw(SEA_TRAVEL_PASSAGE_NO));
	ds_writefp(ROUTE_COURSE_PTR, ds_readfp(SEA_TRAVEL_COURSES) + off + 4 * ds_readws(ROUTE_MOUSEHOVER));
	ptr = ds_readfp(FRAMEBUF_PTR);

	add_ds_fp(ROUTE_COURSE_PTR, 4);

	memset(Real2Host(ds_readd(TRV_TRACK_PIXEL_BAK)), 0xaa, 500);
	ds_writew(TRAVEL_SPEED, 10 * ds_readbs(SEA_TRAVEL_PASSAGE_UNKN1));
	ds_writew(ROUTE_TOTAL_STEPS, get_srout_len(Real2Host(ds_readfp(ROUTE_COURSE_PTR))));
	ds_writew(ROUTE_LENGTH, 100 * ds_readb(SEA_TRAVEL_PASSAGES + 2 + 8 * passage));
	ds_writew(ROUTE_DURATION, ds_readws(ROUTE_LENGTH) / ds_readws(TRAVEL_SPEED) * 60);
	ds_writew(ROUTE_TIMEDELTA, ds_readws(ROUTE_DURATION) / ds_readws(ROUTE_TOTAL_STEPS));
	ds_writew(ROUTE_STEPSIZE, ds_readws(ROUTE_LENGTH) / ds_readws(ROUTE_TOTAL_STEPS));

	if (ds_readw(ROUTE_STEPSIZE) == 0) {
		ds_writew(ROUTE_STEPSIZE, 1);
	}

	if (dir) {

		while (host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR))) != -1) {
			add_ds_fp(ROUTE_COURSE_PTR, 4);
		}

		sub_ds_fp(ROUTE_COURSE_PTR, 4);
	}

	ds_writefp(ROUTE_COURSE_START, ds_readfp(ROUTE_COURSE_PTR));
	ds_writew(ROUTE_DAYPROGRESS, 18 * (ds_readws(TRAVEL_SPEED) + ds_readws(TRAVEL_SPEED) / 10));

	if (passage <= 6 && ds_readb(QUEST_DEADSHIP) != 0 && !ds_readb(QUEST_DEADSHIP_DONE)) {

		if (ds_writews(PASSAGE_DEADSHIP_FLAG, random_schick(100) <= 20 ? 1 : 0)) {
			ds_writews(PASSAGE_DEADSHIP_TIME, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
		}
	} else {
		ds_writew(PASSAGE_DEADSHIP_FLAG, 0);
	}

	if (ds_writews(PASSAGE_OCTOPUS_FLAG, random_schick(100) <= 5 ? 1 : 0)) {

		ds_writews(PASSAGE_OCTOPUS_TIME, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
	}

	if (ds_writews(PASSAGE_PIRATES_FLAG, random_schick(100) <= 10 ? 1 : 0)) {

		ds_writews(PASSAGE_PIRATES_TIME, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
	}

	ds_writew(ROUTE_STEPCOUNT, ds_writew(ROUTE_PROGRESS, ds_writew(ROUTE_DAYPROGRESS, ds_writeb(TRAVEL_DETOUR, 0))));
	ds_writeb(TRAVEL_HEROKEEPING, 1);

	while (host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR)) + 2 * ds_writew(ROUTE_MOUSEHOVER, 0)) != -1 && !ds_readb(TRAVEL_DETOUR))
	{

		if (is_mouse_in_rect(host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR))) - 16,
					host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR)) + 2) - 16,
					host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR))) + 16,
					host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR)) + 2) + 16))
		{
			update_mouse_cursor();
			ds_writew(ROUTE_MOUSEHOVER, 1);
		}

		host_writeb(Real2Host(ds_readd(TRV_TRACK_PIXEL_BAK)) + ds_readws(ROUTE_STEPCOUNT),
			mem_readb(Real2Phys(ptr) + 320 * host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR)) + 2) + host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR)))));
		inc_ds_ws(ROUTE_STEPCOUNT);

		mem_writeb(Real2Phys(ptr) + 320 * host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR)) + 2) + host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR))), 0x1f);

		if (ds_readws(ROUTE_MOUSEHOVER) != 0) {
			refresh_screen_size();
		}

		for (ds_writew(TRV_I, 0); ds_readws(ROUTE_TIMEDELTA) / 2 > ds_readws(TRV_I); inc_ds_ws(TRV_I)) {

			handle_input();

			wait_for_vsync();

			timewarp(MINUTES(2));
		}

		add_ds_ws(ROUTE_PROGRESS, ds_readws(ROUTE_STEPSIZE));
		add_ds_ws(ROUTE_DAYPROGRESS, ds_readws(ROUTE_STEPSIZE));

		if (ds_readws(PASSAGE_DEADSHIP_FLAG) != 0 && ds_readws(ROUTE_DAYPROGRESS) >= ds_readws(PASSAGE_DEADSHIP_TIME) && !ds_readb(QUEST_DEADSHIP_DONE)) {
			enter_ghostship();
			ds_writew(PASSAGE_DEADSHIP_FLAG, 0);
		} else if (ds_readws(PASSAGE_OCTOPUS_FLAG) != 0 && ds_readws(ROUTE_DAYPROGRESS) >= ds_readws(PASSAGE_OCTOPUS_TIME) && !ds_readd(INGAME_TIMERS + 8)) {
			octopus_attack_wrapper();
			ds_writew(PASSAGE_OCTOPUS_FLAG, 0);
		} else if (ds_readws(PASSAGE_PIRATES_FLAG) != 0 && ds_readws(ROUTE_DAYPROGRESS) >= ds_readws(PASSAGE_PIRATES_TIME)) {
			pirates_attack_wrapper();
			ds_writew(PASSAGE_PIRATES_FLAG, 0);
		}

		if (ds_readws(CHECK_DISEASE) != 0 && !ds_readbs(CHECK_PARTY)) {

			disease_effect();

			ds_writeb(TRAVEL_BY_SHIP, 1);

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					GRP_hero_sleep(hero, a.a[ds_readbs(SEA_TRAVEL_SLEEP_QUALITY)]);
					host_writeb(hero + HERO_HUNGER, host_writebs(hero + HERO_THIRST, 0));
				}
			}

			ds_writeb(TRAVEL_BY_SHIP, 0);

		}

		if (ds_readws(REQUEST_REFRESH) != 0 && !ds_readb(TRAVEL_DETOUR)) {

			update_mouse_cursor();

			load_map();

			bc_memmove((RealPt)ds_readd(FRAMEBUF_PTR), (RealPt)ds_readd(TRAVEL_MAP_PTR), 64000);

			wait_for_vsync();

			set_palette(Real2Host(ds_readd(TRAVEL_MAP_PTR)) + 64002, 0, 0x20);

			set_audio_track(ARCHIVE_FILE_TERMS_XMI);

			ds_writew(TRV_I, 0);
			for (ds_writed(ROUTE_COURSE_PTR2, ds_readd(ROUTE_COURSE_START));
			     host_readb(Real2Host(ds_readd(TRV_TRACK_PIXEL_BAK)) + inc_ds_ws_post(TRV_I)) != 0xaa;
			     add_ds_fp(ROUTE_COURSE_PTR2, 2 * (!dir ? 2 : -2)))
			{
				mem_writeb(Real2Phys(ptr) + 320 * host_readws(Real2Host(ds_readd(ROUTE_COURSE_PTR2)) + 2) + host_readws(Real2Host(ds_readd(ROUTE_COURSE_PTR2))), 0x1f);
			}

			refresh_screen_size();

			ds_writew(WALLCLOCK_X, ds_readws(BASEPOS_X) + 120);
			ds_writew(WALLCLOCK_Y, ds_readws(BASEPOS_Y) + 87);
			ds_writew(WALLCLOCK_UPDATE, 1);
			ds_writew(REQUEST_REFRESH, 0);
		}

		add_ds_fp(ROUTE_COURSE_PTR, 2 * (!dir ? 2 : -2));
	}

	ds_writeb(TRAVEL_HEROKEEPING, 0);

	if (!ds_readb(TRAVEL_DETOUR)) {

		update_mouse_cursor();

		do {
			if (!dir) {
				sub_ds_fp(ROUTE_COURSE_PTR, 4);
			} else {
				add_ds_fp(ROUTE_COURSE_PTR, 4);
			}

			dec_ds_ws(ROUTE_STEPCOUNT);

			mem_writeb(Real2Phys(ptr) + 320 * host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR)) + 2) + host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR))),
				host_readb(Real2Host(ds_readd(TRV_TRACK_PIXEL_BAK)) + ds_readws(ROUTE_STEPCOUNT))
 );
		} while (host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR))) != -1);

		refresh_screen_size();
	}

	ds_writeb(TRAVELING, 0);
}

signed short get_srout_len(Bit8u *ptr)
{
	signed short i = 0;

	while (host_readws(ptr) != -1) {
		ptr += 4;
		i++;
	}

	return i;
}

#if !defined(__BORLANDC__)
}
#endif
