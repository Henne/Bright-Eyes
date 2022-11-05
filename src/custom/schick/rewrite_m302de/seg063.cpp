/*
 *	Rewrite of DSA1 v3.02_de functions of seg063 (harbor)
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
	Bit8u *p = p_datseg + SEA_ROUTES;


	for (i = 0; i < NR_SEA_ROUTES; p += SIZEOF_SEA_ROUTE, i++) {

		host_writeb(p + SEA_ROUTE_PASSAGE_TIMER, (unsigned char)random_interval(0, host_readbs(p + SEA_ROUTE_FREQUENCY)));
		host_writeb(p + SEA_ROUTE_PASSAGE_PRICE_MOD, (unsigned char)random_interval(70, 130));

		si = random_schick(100);

		if (!host_readbs(p + SEA_ROUTE_COSTAL_ROUTE)) {
			/* high seas route */
			host_writeb(p + SEA_ROUTE_PASSAGE_SHIP_TYPE, si <= 50 ? SHIP_TYPE_LANGSCHIFF_HIGH_SEAS : (si <= 80 ? SHIP_TYPE_KARRACKE : (si <= 95 ? SHIP_TYPE_SCHNELLSEGLER : SHIP_TYPE_SCHNELLSEGLER_LUXURY)));

		} else {

			/* costal route */
			host_writeb(p + SEA_ROUTE_PASSAGE_SHIP_TYPE, si <= 10 ? SHIP_TYPE_LANGSCHIFF_COSTAL : (si <= 40 ? SHIP_TYPE_KUESTENSEGLER : (si <= 80 ? SHIP_TYPE_KUTTER : SHIP_TYPE_FISCHERBOOT)));
		}

#if !defined(__BORLANDC__)
	D1_LOG("%16s - %16s: %d %d %d %d %d %d\n",
		(char*)get_ttx(host_readb(p + SEA_ROUTE_TOWN_1) + 0xeb),
		(char*)get_ttx(host_readb(p + SEA_ROUTE_TOWN_2) + 0xeb),
		host_readb(p + SEA_ROUTE_DISTANCE),
		host_readb(p + SEA_ROUTE_FREQUENCY),
		host_readb(p + SEA_ROUTE_PASSAGE_TIMER),
		host_readb(p + SEA_ROUTE_COSTAL_ROUTE),
		host_readb(p + SEA_ROUTE_PASSAGE_SHIP_TYPE),
		host_readb(p + SEA_ROUTE_PASSAGE_PRICE_MOD));
#endif
	}
}

struct dummy7 {
	signed short a[7];
};

void do_harbor(void)
{
	signed short i; /* diverse usage */
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

	draw_loc_icons(4, MENU_ICON_BOOK_SHIP_PASSAGE, MENU_ICON_HARBOR_MASTER, MENU_ICON_BOARD_SHIP, MENU_ICON_LEAVE);
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

				if (
					ds_readws(QUESTED_MONTHS) > 2 &&
					!ds_readbs((TREASURE_MAPS + 6)) &&
					ds_readb(INFORMER_FLAGS + INFORMER_SWAFNILD) &&
					random_schick(100) <= ds_readws(QUESTED_MONTHS) + 4
				) {
					/* meet SWAFNILD */
					do_talk(12, 0);

					if (ds_readb(SWAFNILD_TRAVELED) != 0) {

						ds_writeb(SWAFNILD_TRAVELED, 0);

						leave_location();

						return;
					}
				}
			}
		}

		handle_gui_input();

		if (ds_readw(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

			answer = GUI_radio(get_tx(9), 4,
						get_tx(10), get_tx(11),
						get_tx(12), get_tx(13)) - 1;

			if (answer != -2) {
				ds_writew(ACTION, answer + ACTION_ID_ICON_1);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_1) {
			/* book passage */

			answer = prepare_passages();

			if (answer == 0) {
				GUI_output(get_tx(15));
			} else {

				/* select a destination */
				answer = GUI_radio(get_tx(14), (signed char)answer,
						get_ttx(ds_readb((HARBOR_OPTIONS + SIZEOF_HARBOR_OPTION * 0 + HARBOR_OPTION_DESTINATION)) + 235),
						get_ttx(ds_readb((HARBOR_OPTIONS + SIZEOF_HARBOR_OPTION * 1 + HARBOR_OPTION_DESTINATION)) + 235),
						get_ttx(ds_readb((HARBOR_OPTIONS + SIZEOF_HARBOR_OPTION * 2 + HARBOR_OPTION_DESTINATION)) + 235),
						get_ttx(ds_readb((HARBOR_OPTIONS + SIZEOF_HARBOR_OPTION * 3 + HARBOR_OPTION_DESTINATION)) + 235),
						get_ttx(ds_readb((HARBOR_OPTIONS + SIZEOF_HARBOR_OPTION * 4 + HARBOR_OPTION_DESTINATION)) + 235),
						get_ttx(ds_readb((HARBOR_OPTIONS + SIZEOF_HARBOR_OPTION * 5 + HARBOR_OPTION_DESTINATION)) + 235),
						get_ttx(ds_readb((HARBOR_OPTIONS + SIZEOF_HARBOR_OPTION * 6 + HARBOR_OPTION_DESTINATION)) + 235),
						get_ttx(ds_readb((HARBOR_OPTIONS + SIZEOF_HARBOR_OPTION * 7 + HARBOR_OPTION_DESTINATION)) + 235),
						get_ttx(ds_readb((HARBOR_OPTIONS + SIZEOF_HARBOR_OPTION * 8 + HARBOR_OPTION_DESTINATION)) + 235),
						get_ttx(ds_readb((HARBOR_OPTIONS + SIZEOF_HARBOR_OPTION * 9 + HARBOR_OPTION_DESTINATION)) + 235)) - 1;

				if (answer != -2) {

					psg_ptr = p_datseg + SIZEOF_HARBOR_OPTION * answer + HARBOR_OPTIONS;

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx(16),

						(char*)get_tx(ds_readws(SEA_TRAVEL_TX_SHIP + 2 * host_readbs(psg_ptr + HARBOR_OPTION_SHIP_TYPE))), /* Fischerboot, Schnellsegler etc. */
						(char*)Real2Host(host_readds(psg_ptr + HARBOR_OPTION_SHIP_NAME_PTR)),

						(char*)(!host_readbs(psg_ptr + HARBOR_OPTION_SHIP_TIMER) ? get_tx(5) : get_tx(6)), /* today or tomorrow */

						(char*)get_tx(ds_readws(PASSAGE_TYPE_TO_NAME + 2 * ds_readbs(SHIP_TABLE + SHIP_TABLE_PASSAGE_TYPE + SIZEOF_SHIP_TABLE_ENTRY * host_readbs(psg_ptr + HARBOR_OPTION_SHIP_TYPE)))), /* Kabinenpassage etc. */
						(char*)get_ttx(host_readb(psg_ptr + HARBOR_OPTION_DESTINATION) + 235),
#ifdef __BORLANDC__
						get_passage_travel_hours(host_readb(Real2Host(host_readd(psg_ptr + HARBOR_OPTION_ROUTE_PTR)) + SEA_ROUTE_DISTANCE), ds_readbs(SHIP_TABLE + SHIP_TABLE_BASE_SPEED + SIZEOF_SHIP_TABLE_ENTRY * host_readbs(psg_ptr + HARBOR_OPTION_SHIP_TYPE))),
#else
						/* when compiled with gcc, occasionally passage times of 0 hours do show up. (which does not happen in the original game!!)
						 * I observed that within the function get_prassage_travel_hours(..), computations with negative numbers might happen and lead to this bug.
						 * The following line fixes this. However, it will lead to incompatible binaries when compiled with the original 1992 BCC compiler
						 * This incompatibility of the behavior gcc vs. BCC is a bit scary.
						 * A better understanding is urgently needed... */
						get_passage_travel_hours(host_readb(Real2Host(host_readd(psg_ptr + HARBOR_OPTION_ROUTE_PTR)) + SEA_ROUTE_DISTANCE), (unsigned char)ds_readbs(SHIP_TABLE + SHIP_TABLE_BASE_SPEED + SIZEOF_SHIP_TABLE_ENTRY * host_readbs(psg_ptr + HARBOR_OPTION_SHIP_TYPE))),
#endif
						Real2Host(print_passage_price(ds_readbs(SHIP_TABLE + SHIP_TABLE_BASE_PRICE_PER_DISTANCE + SIZEOF_SHIP_TABLE_ENTRY * host_readbs(psg_ptr + HARBOR_OPTION_SHIP_TYPE)), Real2Host(host_readds(psg_ptr + HARBOR_OPTION_ROUTE_PTR)))));

					i = ds_readws(TEXTBOX_WIDTH);
					ds_writew(TEXTBOX_WIDTH, 5);

					answer = GUI_bool(Real2Host(ds_readd(DTP2)));

					ds_writew(TEXTBOX_WIDTH, i);

					if (answer != 0) {

						money = get_party_money();

						if (ds_readws(SEA_TRAVEL_PASSAGE_PRICE) > money) {
							/* party does not have enough money to pay the ship passage */

							GUI_output(get_ttx(401));

						} else {

							ds_writeb(SEA_TRAVEL_SLEEP_QUALITY, ds_readb(SHIP_TABLE + SHIP_TABLE_PASSAGE_TYPE + SIZEOF_SHIP_TABLE_ENTRY * host_readbs(psg_ptr + HARBOR_OPTION_SHIP_TYPE)));
							money -= ds_readws(SEA_TRAVEL_PASSAGE_PRICE);
							set_party_money(money);

							ds_writeb(SEA_TRAVEL_PSGBOOKED_TIMER, host_readb(psg_ptr + HARBOR_OPTION_SHIP_TIMER));
							ds_writeb(SEA_TRAVEL_PSGBOOKED_FLAG, 0xaa);

							ds_writeb(SEA_TRAVEL_PASSAGE_SPEED1, ds_readb(SEA_TRAVEL_PASSAGE_SPEED2)); /* speed in [100m per hour] */
							/* Now ..._SPEED1 is the lower byte of ..._SPEED2 */
							/* not clear why two variables ..._SPEED1 and ..._SPEED2 are used. */
							/* In my opinion, a single variable would be enough (and then there would not be the need to copy the value around) */

							ds_writeb(SEA_TRAVEL_PASSAGE_ID, host_readb(psg_ptr + HARBOR_OPTION_ROUTE_ID));

							GUI_output(host_readb(psg_ptr + HARBOR_OPTION_SHIP_TIMER) != 0 ? get_tx(18) : get_tx(17));
							/* ship leaving tomorrow or today */
						}
					}
				}
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_2) {
			/* Hafenmeister */

			if (ds_readds(DAY_TIMER) <= HOURS(6) || ds_readds(DAY_TIMER) >= HOURS(21)) {

				GUI_output(get_ttx(568));

			} else {

				load_in_head(11);

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(4),
									get_tx(0), 3,
									get_tx(1), /* Ankommende Schiffe erfragen */
									get_tx(2), /* Saemtliche Routen erfragen */
									get_tx(3)  /* Den Geldhai verlassen */
					);
				} while (answer == -1);

				p_money = get_party_money();

				if (p_money < 10L) {

					GUI_output(get_tx(8));

				} else if (answer == 1 || answer == 2) {
					/* answer == 1: Ankommende Schiffe erfragen (next 2 days)
					 * answer == 2: Saemtliche Routern erfragen */

					p_money -= 10L;
					set_party_money(p_money);

					i  = answer;
					answer = get_next_passages(answer);
					/* answer now: number of passages reported by Hafenmeister */

					if (answer != 0) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)(i == 1 ? get_tx(28) : get_tx(22)),
							(char*)(answer == 1 ? p_datseg + SEA_TRAVEL_STR_T : p_datseg + SEA_TRAVEL_STR_EN),
							(char*)(answer == 1 ? get_tx(23) : get_tx(24)));

						i = 0;

						do {

							strcat((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(ds_readb((HARBOR_OPTIONS + HARBOR_OPTION_DESTINATION) + SIZEOF_HARBOR_OPTION * i++) + 235));
							if (--answer) {

								strcat((char*)Real2Host(ds_readd(DTP2)),
									(char*)(answer >= 2 ? p_datseg + SEA_TRAVEL_STR_COMMA : get_tx(7)));
							}

						} while (answer != 0);

						strcat((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_tx(25));

						GUI_output(Real2Host(ds_readd(DTP2)));

					} else {

						GUI_output(get_tx(26));
						/* "In den naechsten Tagen wird wohl kein Schiff kommen [...]" */
					}
				}
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_3) {
			/* enter booked ship */

			if (ds_readb(SEA_TRAVEL_PSGBOOKED_FLAG) != 0xaa) {
				/* no ship booked... */

				GUI_output(get_tx(19));

			} else if (ds_readb(SEA_TRAVEL_PSGBOOKED_TIMER) != 0) {

				GUI_output(ds_readbs(SEA_TRAVEL_PSGBOOKED_TIMER) == -1 ?
					get_tx(27) : /* SEA_TRAVEL_PSGBOOKED_TIMER == -1 -> "Zu spaet! Das Schiff, fuer das ihr gebucht wart ist leider ohne euch losgefahren!" */
					get_tx(20)   /* SEA_TRAVEL_PSGBOOKED_TIMER == +1 -> "Die Matrosen lassen euch das Schiff noch nicht besteigen. Kommt morgen wieder..." */
				);

			} else {

				set_var_to_zero();

				GUI_output(get_tx(21));
				/* "Ihr begebt euch an Bord. <Macht's euch gemuetlich!> brummelt der Kapitaen. <Wir legen bald ab!>" */

				ds_writeb(TRAVEL_HEROKEEPING, 1);

				timewarp_until_time_of_day(HOURS(9));

				/* CHECK_DISEASE is set to 1 each midnight in timers_daily() */
				if (ds_readws(CHECK_DISEASE) != 0) {
					/* effectively, this branch is reached if the ship was entered before midnight. */

					disease_effect();
					/* in this call, CHECK_DISEASE is reset to 0 */

					hero = get_hero(0);
					for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
						{
							GRP_hero_sleep(hero, a.a[ds_readbs(SEA_TRAVEL_SLEEP_QUALITY)]);

							host_writebs(hero + HERO_HUNGER, host_writebs(hero + HERO_THIRST, 0));
							/* on the ship, food and drinks are served. */
						}
					}
				}

				ds_writeb(TRAVEL_HEROKEEPING, 0);

				for (i = 0; i < 6; i++) {
					ds_writeb(FOOD_MESSAGE_SHOWN + i, 0);
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

				sea_travel(ds_readb(SEA_TRAVEL_PASSAGE_ID), ds_readbs(SEA_ROUTES + SIZEOF_SEA_ROUTE * ds_readb(SEA_TRAVEL_PASSAGE_ID)) == ds_readbs(CURRENT_TOWN) ? 0 : 1);
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
					DNG_enter_dungeon(DUNGEONS_TOTENSCHIFF);
				}
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_4) {
			done = 1;
		}

	} while (!done);

	i = load_archive_file(ARCHIVE_FILE_COMPASS);
	read_archive_file(i, Real2Host(ds_readd(BUFFER6_PTR)), 5000);
	bc_close(i);

	set_var_to_zero();

	if (!ds_readb(TRAVEL_DETOUR)) {

		copy_palette();
		leave_location();

	} else {

		ds_writeb(CURRENT_TOWN, TOWNS_NONE);
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

	map_x = ds_readws((TOWN_POSITIONS-4) + 4 * town_id);
	map_y = ds_readws((TOWN_POSITIONS-4) + 4 * town_id + 2);

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

	/* high seas routes have id 0..6, costal routes id 7..44 */

	/* if high seas route, write 7 (total number of high seas routes)
	 * if costal route, write 38 (total number of costal routes) */
	ds_writew(ROUTE_MOUSEHOVER, passage < 7 ? 7 : 38);

	/* convert costal route ids to range 0..37 */
	ds_writew(SEA_TRAVEL_PASSAGE_NO, passage < 7 ? passage : passage - 7);

	off = host_readd(Real2Host(ds_readfp(SEA_TRAVEL_COURSES)) + 4 * ds_readw(SEA_TRAVEL_PASSAGE_NO));
	ds_writefp(ROUTE_COURSE_PTR, ds_readfp(SEA_TRAVEL_COURSES) + off + 4 * ds_readws(ROUTE_MOUSEHOVER));
	ptr = ds_readfp(FRAMEBUF_PTR);

	add_ds_fp(ROUTE_COURSE_PTR, 4);

	memset(Real2Host(ds_readd(TRV_TRACK_PIXEL_BAK)), 0xaa, 500);
	ds_writew(TRAVEL_SPEED, 10 * ds_readbs(SEA_TRAVEL_PASSAGE_SPEED1)); /* speed [unit: 10m per hour] */
	ds_writew(ROUTE_TOTAL_STEPS, get_srout_len(Real2Host(ds_readfp(ROUTE_COURSE_PTR)))); /* a step for each pixel on the map. */
	ds_writew(ROUTE_LENGTH, 100 * ds_readb(SEA_ROUTES + SEA_ROUTE_DISTANCE + SIZEOF_SEA_ROUTE * passage)); /* length of sea route [unit: 10m] */
	ds_writew(ROUTE_DURATION, ds_readws(ROUTE_LENGTH) / ds_readws(TRAVEL_SPEED) * 60); /* duration [unit: minutes] */
	ds_writew(ROUTE_TIMEDELTA, ds_readws(ROUTE_DURATION) / ds_readws(ROUTE_TOTAL_STEPS)); /* duration of each step [unit: minutes] */
	ds_writew(ROUTE_STEPSIZE, ds_readws(ROUTE_LENGTH) / ds_readws(ROUTE_TOTAL_STEPS)); /* length of a single step [unit: 10m] */

	if (ds_readw(ROUTE_STEPSIZE) == 0) {
		ds_writew(ROUTE_STEPSIZE, 1);
	}
#if !defined(__BORLANDC__)
	D1_INFO("Schiffspassage gestartet. Entfernung: %d0 Schritt. Geschwindigkeit: %d0 Schritt/h. Dauer (lt. Hafen): %d min. Dauer (real): %d min.\n", ds_readw(ROUTE_LENGTH), ds_readw(TRAVEL_SPEED), ds_readw(ROUTE_DURATION),ds_readws(ROUTE_TOTAL_STEPS)*2*(ds_readw(ROUTE_TIMEDELTA)/2));
	D1_INFO_VERBOSE("#Pixel = %d, Entfernung/Pixel: %d0 Schritt, Dauer/Pixel: %d min\n", ds_readw(ROUTE_TOTAL_STEPS), ds_readw(ROUTE_STEPSIZE), ds_readw(ROUTE_TIMEDELTA));
#endif

	if (dir) {
		/* for reverse direction, point ROUTE_COURSE_PTR to end of route */

		while (host_readws(Real2Host(ds_readfp(ROUTE_COURSE_PTR))) != -1) {
			add_ds_fp(ROUTE_COURSE_PTR, 4);
		}

		sub_ds_fp(ROUTE_COURSE_PTR, 4);
	}

	ds_writefp(ROUTE_COURSE_START, ds_readfp(ROUTE_COURSE_PTR));

	ds_writew(ROUTE_DAYPROGRESS, 18 * (ds_readws(TRAVEL_SPEED) + ds_readws(TRAVEL_SPEED) / 10));
	/* this is 19.8h * TRAVEL_SPEED, which is the distance [unit: 10m] the ship travels in 19.8 h.
	 * It is used as upper bound for the position of the random encounters. */

	if (passage <= 6 && ds_readb(QUEST_DEADSHIP) != 0 && !ds_readb(QUEST_DEADSHIP_DONE)) {
		/* only on high seas routes */

		if (ds_writews(PASSAGE_DEADSHIP_FLAG, random_schick(100) <= 20 ? 1 : 0)) {
			ds_writews(PASSAGE_DEADSHIP_POSITION, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
#if !defined(__BORLANDC__)
			D1_INFO("Totenschiff wurde bei %o0 Schritt aktiviert!\n", ds_readws(PASSAGE_DEADSHIP_POSITION));
#endif
		}
	} else {
		ds_writew(PASSAGE_DEADSHIP_FLAG, 0);
	}

	if (ds_writews(PASSAGE_OCTOPUS_FLAG, random_schick(100) <= 5 ? 1 : 0)) {

		ds_writews(PASSAGE_OCTOPUS_POSITION, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
#if !defined(__BORLANDC__)
		D1_INFO("Krakenmolch wurde bei %o0 Schritt aktiviert!\n", ds_readws(PASSAGE_OCTOPUS_POSITION));
#endif
	}

	if (ds_writews(PASSAGE_PIRATES_FLAG, random_schick(100) <= 10 ? 1 : 0)) {

		ds_writews(PASSAGE_PIRATES_POSITION, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
#if !defined(__BORLANDC__)
		D1_INFO("Piratenangriff wurde bei %o0 Schritt aktiviert!\n", ds_readws(PASSAGE_PIRATES_POSITION));
#endif
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

		/* the following loop will be executed Floor(ROUTE_TIMEDELTA / 2) times.
		 * therefore, 2 * Floor(ROUTE_TIMEDELTA / 2) minutes ingame times will pass. */
		for (ds_writew(TRV_I, 0); ds_readws(ROUTE_TIMEDELTA) / 2 > ds_readws(TRV_I); inc_ds_ws(TRV_I)) {

			handle_input();

			wait_for_vsync();

			timewarp(MINUTES(2));
		}

		add_ds_ws(ROUTE_PROGRESS, ds_readws(ROUTE_STEPSIZE));
		add_ds_ws(ROUTE_DAYPROGRESS, ds_readws(ROUTE_STEPSIZE));

#if !defined(__BORLANDC__)
		D1_LOG("%d0 Schritt zurueckgelegt.\n",ds_readws(ROUTE_DAYPROGRESS));
#endif

		if (ds_readws(PASSAGE_DEADSHIP_FLAG) != 0 && ds_readws(ROUTE_DAYPROGRESS) >= ds_readws(PASSAGE_DEADSHIP_POSITION) && !ds_readb(QUEST_DEADSHIP_DONE)) {
			prolog_ghostship();
			/* within the call prolog_ghostship(), the party can decide if they enter the Totenschiff.
			 * In that case, TRAVEL_DETOUR is set to DUNGEONS_TOTENSCHIFF (instead of 0) */

			ds_writew(PASSAGE_DEADSHIP_FLAG, 0);
		} else if (ds_readws(PASSAGE_OCTOPUS_FLAG) != 0 && ds_readws(ROUTE_DAYPROGRESS) >= ds_readws(PASSAGE_OCTOPUS_POSITION) && !ds_readd(INGAME_TIMERS + 4 * INGAME_TIMER_EFFERD_SAFE_PASSAGE)) {
			octopus_attack_wrapper();
			ds_writew(PASSAGE_OCTOPUS_FLAG, 0);
		} else if
#ifndef M302de_ORIGINAL_BUGFIX
			/* Original-Bug 34:
			 * There is an Efferd miracle with the text "Efferd gewaehrt euch seinen Schutz auf Wasser.".
			 * For sea traveling, it prevents octopus encounters. However, pirate encounters are still possible, which feels wrong. */
			(ds_readws(PASSAGE_PIRATES_FLAG) != 0 && ds_readws(ROUTE_DAYPROGRESS) >= ds_readws(PASSAGE_PIRATES_POSITION))
#else
			(ds_readws(PASSAGE_PIRATES_FLAG) != 0 && ds_readws(ROUTE_DAYPROGRESS) >= ds_readws(PASSAGE_PIRATES_POSITION) && !ds_readd(INGAME_TIMERS + 4 * INGAME_TIMER_EFFERD_SAFE_PASSAGE))
#endif
		{
			pirates_attack_wrapper();
			ds_writew(PASSAGE_PIRATES_FLAG, 0);
		}

		/* CHECK_DISEASE is set to 1 each midnight in timers_daily() */
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
