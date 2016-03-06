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
	Bit8u *p = p_datseg + 0x6f00;


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
		(char*)get_ltx((host_readb(p + 0) + 0xeb) * 4),
		(char*)get_ltx((host_readb(p + 1) + 0xeb) * 4),
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
	Bit8u *ptr;
	Bit32s p_money;
	Bit8u *hero;
	signed char flag;
	Bit32s money;

	done = 0;
#if !defined(__BORLANDC__)
	struct dummy7 a = { { -2, 0, 5, 4, 3, 1, 0 } };
#else
	struct dummy7 a = *(struct dummy7*)(p_datseg + 0x7071);
#endif
	flag = 1;

	ds_writew(0x4334, ds_readws(TYPEINDEX));

	draw_loc_icons(4, 40, 42, 41, 8);
	ds_writew(0x2846, 1);
	ds_writeb(0x4333, 0);

	do {

		if (ds_readw(0x2846) != 0) {

			draw_main_screen();
			set_var_to_zero();
			load_ani(6);
			init_ani(0);
			ds_writew(0x2846, 0);

			load_buffer_1(133);

			if (flag != 0) {

				flag = 0;

				if (ds_readws(QUESTED_MONTHS) > 2 &&
					!ds_readbs((TREASURE_MAPS + 6)) &&
					ds_readb(INFORMER_SWAFNILD) &&
					random_schick(100) <= ds_readws(QUESTED_MONTHS) + 4)
				{
					/* meet SWAFNILD */
					do_talk(12, 0);

					if (ds_readb(0x7c9c) != 0) {

						ds_writeb(0x7c9c, 0);

						turnaround();

						return;
					}
				}
			}
		}

		handle_gui_input();

		if (ds_readw(0xc3d3) != 0 || ds_readws(ACTION) == 73) {

			answer = GUI_radio(get_dtp(0x24), 4,
						get_dtp(0x28), get_dtp(0x2c),
						get_dtp(0x30), get_dtp(0x34)) - 1;

			if (answer != -2) {
				ds_writew(ACTION, answer + 129);
			}
		}

		if (ds_readws(ACTION) == 129) {

			answer = prepare_passages();

			if (answer == 0) {
				GUI_output(get_dtp(0x3c));
			} else {

				/* select a destination */
				answer = GUI_radio(get_dtp(0x38), (signed char)answer,
						get_ltx(4 * (ds_readb((0x42b2 + 12 * 0 + 10)) + 235)),
						get_ltx(4 * (ds_readb((0x42b2 + 12 * 1 + 10)) + 235)),
						get_ltx(4 * (ds_readb((0x42b2 + 12 * 2 + 10)) + 235)),
						get_ltx(4 * (ds_readb((0x42b2 + 12 * 3 + 10)) + 235)),
						get_ltx(4 * (ds_readb((0x42b2 + 12 * 4 + 10)) + 235)),
						get_ltx(4 * (ds_readb((0x42b2 + 12 * 5 + 10)) + 235)),
						get_ltx(4 * (ds_readb((0x42b2 + 12 * 6 + 10)) + 235)),
						get_ltx(4 * (ds_readb((0x42b2 + 12 * 7 + 10)) + 235)),
						get_ltx(4 * (ds_readb((0x42b2 + 12 * 8 + 10)) + 235)),
						get_ltx(4 * (ds_readb((0x42b2 + 12 * 9 + 10)) + 235))) - 1;

				if (answer != -2) {

					ptr = p_datseg + 12 * answer + 0x42b2;

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x40),

						(char*)get_dtp(4 * ds_readws(0x6ef0 + 2 * host_readbs(ptr + 9))),
						(char*)Real2Host(host_readds(ptr + 0)),

						(char*)(!host_readbs(ptr + 8) ? get_dtp(0x14) : get_dtp(0x18)),

						(char*)get_dtp(4 * ds_readws(0x6ec2 + 2 * ds_readbs(0x6ed0 + 4 * host_readbs(ptr + 9)))),
						(char*)get_ltx(4 * (host_readb(ptr + 10) + 235)),
						get_passage_travel_hours(host_readb(Real2Host(host_readd(ptr + 4)) + 2), ds_readbs(0x6ed0 + 3 + 4 * host_readbs(ptr + 9))),

						Real2Host(print_passage_price(ds_readbs(0x6ed0 + 2 + 4 * host_readbs(ptr + 9)), Real2Host(host_readds(ptr + 4)))));

					l_si = ds_readws(TEXTBOX_WIDTH);
					ds_writew(TEXTBOX_WIDTH, 5);

					answer = GUI_bool(Real2Host(ds_readd(DTP2)));

					ds_writew(TEXTBOX_WIDTH, l_si);

					if (answer != 0) {

						money = get_party_money();

						if (ds_readws(0x432a) > money) {

							GUI_output(get_ltx(0x644));

						} else {

							ds_writeb(0xe3fa, ds_readb(0x6ed0 + 4 * host_readbs(ptr +9)));
							money -= ds_readws(0x432a);
							set_party_money(money);

							ds_writeb(0x42af, host_readb(ptr + 8));
							ds_writeb(0x42ae, 0xaa);
							ds_writeb(0x42b0 ,ds_readb(0x432c));
							ds_writeb(0x42b1, host_readb(ptr + 11));

							GUI_output(host_readb(ptr + 8) != 0 ? get_dtp(0x48) : get_dtp(0x44));
						}
					}
				}
			}

		} else if (ds_readws(ACTION) == 130) {

			if (ds_readds(DAY_TIMER) <= HOURS(6) || ds_readds(DAY_TIMER) >= HOURS(21)) {

				GUI_output(get_ltx(0x8e0));

			} else {

				load_in_head(11);

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_dtp(0x10),
									get_dtp(0x00), 3,
									get_dtp(0x4),
									get_dtp(0x8),
									get_dtp(0x0c));
				} while (answer == -1);

				p_money = get_party_money();

				if (p_money < 10L) {

					GUI_output(get_dtp(0x20));

				} else if (answer == 1 || answer == 2) {

					p_money -= 10L;
					set_party_money(p_money);

					l_si  = answer;
					answer = get_next_passages(answer);

					if (answer != 0) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)(l_si == 1 ? get_dtp(0x70) : get_dtp(0x58)),
							(char*)(answer == 1 ? p_datseg + 0x708d : p_datseg + 0x708f),
							(char*)(answer == 1 ? get_dtp(0x5c) : get_dtp(0x60)));

						l_si = 0;

						do {

							strcat((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ltx(4 * (ds_readb(0x42bc + 12 * l_si++) + 235)));
							if (--answer) {

								strcat((char*)Real2Host(ds_readd(DTP2)),
									(char*)(answer >= 2 ? p_datseg + 0x7092 : get_dtp(0x1c)));
							}

						} while (answer != 0);

						strcat((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_dtp(0x64));

						GUI_output(Real2Host(ds_readd(DTP2)));

					} else {

						GUI_output(get_dtp(0x68));
					}
				}
			}

		} else if (ds_readws(ACTION) == 131) {

			if (ds_readb(0x42ae) != 0xaa) {

				GUI_output(get_dtp(0x4c));

			} else if (ds_readb(0x42af) != 0) {

				GUI_output(ds_readbs(0x42af) == -1 ? get_dtp(0x6c) : get_dtp(0x50));

			} else {

				set_var_to_zero();

				GUI_output(get_dtp(0x54));

				ds_writeb(0x4497, 1);

				timewarp_until(HOURS(9));

				if (ds_readws(CHECK_DISEASE) != 0) {

					disease_effect();

					hero = get_hero(0);
					for (l_si = 0; l_si <= 6; l_si++, hero += SIZEOF_HERO) {

						if (host_readbs(hero + HERO_TYPE) != 0 &&
							host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
						{
							GRP_hero_sleep(hero, a.a[ds_readbs(0xe3fa)]);

							host_writebs(hero + HERO_HUNGER,
								host_writebs(hero + HERO_THIRST, 0));
						}
					}
				}

				ds_writeb(0x4497, 0);

				for (l_si = 0; l_si < 6; l_si++) {
					ds_writeb(0x26a4 + l_si, 0);
				}

				load_map();

				ds_writews(CURRENT_ANI, -1);
				ds_writew(0xe113, 0);

				memmove(Real2Host(ds_readd(0xd303)), Real2Host(ds_readd(0x432e)), 64000);
				map_effect(Real2Host(ds_readd(0xd303)));

				wait_for_vsync();

				set_palette(Real2Host(ds_readd(0x432e)) + 64002, 0, 0x20);

				mod_clock_pos(ds_readbs(CURRENT_TOWN));

				set_audio_track(145);

				ds_writew(WALLCLOCK_X, ds_readws(0x2ca2) + 120);
				ds_writew(WALLCLOCK_Y, ds_readws(0x2ca4) + 87);
				ds_writew(0xe113, 1);

				sea_travel(ds_readb(0x42b1), ds_readbs(0x6f00 + 8 * ds_readb(0x42b1)) == ds_readbs(CURRENT_TOWN) ? 0 : 1);
				passage_arrival();

				ds_writew(0xe113, ds_writew(0x2ca2, ds_writew(0x2ca4, ds_writeb(0x42ae, 0))));
				ds_writews(CURRENT_ANI, ds_writebs(0x2ca7, ds_writebs(0x2845, -1)));
				ds_writew(0x2846, 1);
				ds_writeb(TRAVELING, 0);

				if (!ds_readb(0x4333)) {

					ds_writebs(CURRENT_TOWN, (signed char)ds_readws(0x4338));
					ds_writew(0x2d83, ds_readw(0x433a));
					ds_writew(0x2d85, ds_readw(0x433c));
					ds_writeb(DIRECTION, (ds_readws(0x433e) + 2) & 3);

				} else {
					done = 1;
					DNG_enter_dungeon(1);
				}
			}

		} else if (ds_readws(ACTION) == 132) {
			done = 1;
		}

	} while (!done);

	l_si = load_archive_file(18);
	read_archive_file(l_si, Real2Host(ds_readd(0xd2b1)), 5000);
	bc_close(l_si);

	set_var_to_zero();

	if (!ds_readb(0x4333)) {

		copy_palette();
		turnaround();

	} else {

		ds_writeb(CURRENT_TOWN, 0);
	}
}

/**
 * \brief
 * \param town_id	ID of the town
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

	ds_writew(0x2ca2, !val || val == 2 ? -80 : 80);
	ds_writew(0x2ca4, !val || val == 1 ? -40 : 40);
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
	struct dummy7 a = *(struct dummy7*)(p_datseg + 0x707f);
#endif

	ds_writeb(0xa842, 1);

	ds_writed(0x4266, (Bit32u)(passage < 7 ? F_PADD(ds_readd(0xc3db), 7600) : F_PADD(ds_readd(0xc3db), 11400)));
	ds_writew(0x4236, passage < 7 ? 7 : 38);
	ds_writew(0x423e, passage < 7 ? passage : passage - 7);

	off = host_readd(Real2Host(ds_readd(0x4266)) + 4 * ds_readw(0x423e));
	ds_writed(0x425a, (Bit32u)((RealPt)ds_readd(0x4266) + off + 4 * ds_readws(0x4236)));
	ptr = (RealPt)ds_readd(0xd2ff);

	add_ds_ws(0x425a, 4);

	memset(Real2Host(ds_readd(0xd299)), 0xaa, 500);
	ds_writew(0x424c, 10 * ds_readbs(0x42b0));
	ds_writew(0x422e, get_srout_len(Real2Host(ds_readd(0x425a))));
	ds_writew(0x4230, 100 * ds_readb(0x6f00 + 2 + 8 * passage));
	ds_writew(0x4232, ds_readws(0x4230) / ds_readws(0x424c) * 60);
	ds_writew(0x4234, ds_readws(0x4232) / ds_readws(0x422e));
	ds_writew(0x423a, ds_readws(0x4230) / ds_readws(0x422e));

	if (ds_readw(0x423a) == 0) {
		ds_writew(0x423a, 1);
	}

	if (dir) {

		while (host_readws(Real2Host(ds_readd(0x425a))) != -1) {
			add_ds_ws(0x425a, 4);
		}

		sub_ds_ws(0x425a, 4);
	}

	ds_writed(0x425e, ds_readd(0x425a));
	ds_writew(0x423c, 18 * (ds_readws(0x424c) + ds_readws(0x424c) / 10));

	if (passage <= 6 && ds_readb(QUEST_DEADSHIP) != 0 && !ds_readb(QUEST_DEADSHIP_DONE)) {

		if (ds_writews(0x424e, random_schick(100) <= 20 ? 1 : 0)) {
			ds_writews(0x4250, random_schick(ds_readws(0x423c)));
		}
	} else {
		ds_writew(0x424e, 0);
	}

	if (ds_writews(0x4252, random_schick(100) <= 5 ? 1 : 0)) {

		ds_writews(0x4254, random_schick(ds_readws(0x423c)));
	}

	if (ds_writews(0x4256, random_schick(100) <= 10 ? 1 : 0)) {

		ds_writews(0x4258, random_schick(ds_readws(0x423c)));
	}

	ds_writew(0x422a, ds_writew(0x4238, ds_writew(0x423c, ds_writeb(0x4333, 0))));
	ds_writeb(0x4497, 1);

	while (host_readws(Real2Host(ds_readd(0x425a)) + 2 * ds_writew(0x4236, 0)) != -1 && !ds_readb(0x4333))
	{

		if (is_mouse_in_rect(host_readws(Real2Host(ds_readd(0x425a))) - 16,
					host_readws(Real2Host(ds_readd(0x425a)) + 2) - 16,
					host_readws(Real2Host(ds_readd(0x425a))) + 16,
					host_readws(Real2Host(ds_readd(0x425a)) + 2) + 16))
		{
			update_mouse_cursor();
			ds_writew(0x4236, 1);
		}

		host_writeb(Real2Host(ds_readd(0xd299)) + ds_readws(0x422a),
			mem_readb(Real2Phys(ptr) + 320 * host_readws(Real2Host(ds_readd(0x425a)) + 2) + host_readws(Real2Host(ds_readd(0x425a)))));
		inc_ds_ws(0x422a);

		mem_writeb(Real2Phys(ptr) + 320 * host_readws(Real2Host(ds_readd(0x425a)) + 2) + host_readws(Real2Host(ds_readd(0x425a))), 0x1f);

		if (ds_readws(0x4236) != 0) {
			refresh_screen_size();
		}

		for (ds_writew(0x4228, 0); ds_readws(0x4234) / 2 > ds_readws(0x4228); inc_ds_ws(0x4228)) {

			handle_input();

			wait_for_vsync();

			timewarp(MINUTES(2));
		}

		add_ds_ws(0x4238, ds_readws(0x423a));
		add_ds_ws(0x423c, ds_readws(0x423a));

		if (ds_readws(0x424e) != 0 && ds_readws(0x423c) >= ds_readws(0x4250) && !ds_readb(QUEST_DEADSHIP_DONE)) {
			enter_ghostship();
			ds_writew(0x424e, 0);
		} else if (ds_readws(0x4252) != 0 && ds_readws(0x423c) >= ds_readws(0x4254) && !ds_readd(INGAME_TIMERS + 8)) {
			octopus_attack_wrapper();
			ds_writew(0x4252, 0);
		} else if (ds_readws(0x4256) != 0 && ds_readws(0x423c) >= ds_readws(0x4258)) {
			pirates_attack_wrapper();
			ds_writew(0x4256, 0);
		}

		if (ds_readws(CHECK_DISEASE) != 0 && !ds_readbs(CHECK_PARTY)) {

			disease_effect();

			ds_writeb(TRAVEL_BY_SHIP, 1);

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if (host_readbs(hero + HERO_TYPE) != 0 &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					GRP_hero_sleep(hero, a.a[ds_readbs(0xe3fa)]);
					host_writeb(hero + HERO_HUNGER, host_writebs(hero + HERO_THIRST, 0));
				}
			}

			ds_writeb(TRAVEL_BY_SHIP, 0);

		}

		if (ds_readws(0x2846) != 0 && !ds_readb(0x4333)) {

			update_mouse_cursor();

			load_map();

			bc_memmove((RealPt)ds_readd(0xd2ff), (RealPt)ds_readd(0x432e), 64000);

			wait_for_vsync();

			set_palette(Real2Host(ds_readd(0x432e)) + 64002, 0, 0x20);

			set_audio_track(145);

			ds_writew(0x4228, 0);
			for (ds_writed(0x4262, ds_readd(0x425e)); host_readb(Real2Host(ds_readd(0xd299)) + inc_ds_ws_post(0x4228)) != 0xaa; add_ds_ws(0x4262, 2 * (!dir ? 2 : -2)))
			{
				mem_writeb(Real2Phys(ptr) + 320 * host_readws(Real2Host(ds_readd(0x4262)) + 2) + host_readws(Real2Host(ds_readd(0x4262))), 0x1f);
			}

			refresh_screen_size();

			ds_writew(WALLCLOCK_X, ds_readws(0x2ca2) + 120);
			ds_writew(WALLCLOCK_Y, ds_readws(0x2ca4) + 87);
			ds_writew(0xe113, 1);
			ds_writew(0x2846, 0);
		}

		add_ds_ws(0x425a, 2 * (!dir ? 2 : -2));
	}

	ds_writeb(0x4497, 0);

	if (!ds_readb(0x4333)) {

		update_mouse_cursor();

		do {
			if (!dir) {
				sub_ds_ws(0x425a, 4);
			} else {
				add_ds_ws(0x425a, 4);
			}

			dec_ds_ws(0x422a);

			mem_writeb(Real2Phys(ptr) + 320 * host_readws(Real2Host(ds_readd(0x425a)) + 2) + host_readws(Real2Host(ds_readd(0x425a))),
				host_readb(Real2Host(ds_readd(0xd299)) + ds_readws(0x422a))
 );
		} while (host_readws(Real2Host(ds_readd(0x425a))) != -1);

		refresh_screen_size();
	}

	ds_writeb(0xa842, 0);
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
