/**
 *	Rewrite of DSA1 v3.02_de functions of seg094 (travelmode)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1- Yo seg094.cpp
 */

#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg025.h"
#include "seg028.h"
#include "seg094.h"
#include "seg097.h"
#include "seg109.h"
#include "seg117.h"
#include "seg119.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void prepare_map_marker(void)
{
	signed short i;
	signed short fd;
	signed short dummy;
	struct nvf_desc nvf;

	/* load OBJECTS.NVF */
	fd = load_archive_file(ARCHIVE_FILE_OBJECTS_NVF);

	read_archive_file(fd, Real2Host(ds_readd(BUFFER6_PTR)), 2000);

	bc_close(fd);


	for (i = 0; i < 10; i++) {
		nvf.dst = Real2Host(ds_readd(BUFFER6_PTR)) + 100 * i + 1000;
		nvf.src = Real2Host(ds_readd(BUFFER6_PTR));
		nvf.nr = i;
		nvf.type = 0;
		nvf.width = (Bit8u*)&dummy;
		nvf.height = (Bit8u*)&dummy;

		process_nvf(&nvf);
	}

	set_textbox_positions(ds_readbs(CURRENT_TOWN));
}

void set_textbox_positions(signed short town_id)
{
	signed short x;
	signed short r_dx;
	signed short y;

	/* zero some global variables */
	ds_writew(0xe4a5, ds_writew(0xe4a3, ds_writew(0xe4ad, ds_writew(0xe4ab, 0))));


	x = ds_writews(0xe4b1, ds_readws(TOWN_POSITIONS + 4 * town_id));
	y = ds_writews(0xe4af, ds_readws(TOWN_POSITIONS + 4 * town_id + 2));

	r_dx = (x >= 0 && x <= 159) ?
			(y >= 0 && y <= 99 ? 3 : 1) :
			(y >= 0 && y <= 99 ? 2 : 0);

	ds_writew(0x2ca2, !r_dx || r_dx == 2 ? -80 : 80);
	ds_writew(0x2ca4, !r_dx || r_dx == 1 ? -40 : 40);
}

/**
 * \brief - ???
 * \param route_nr	number of the route
 * \param backwards	0 = travel the route forwards, 1 = travel backwards
 */
void TM_func1(signed short route_nr, signed short backwards)
{
	RealPt fb_start;
	Bit8u *hero;
	Bit8u *ptr;
	signed short bak1;
	signed short bak2;
	signed short l5;
	signed short answer;

	ds_writeb(SEA_TRAVEL, 1);
	l5 = -1;
	ds_writefp(0x425a, (RealPt)(F_PADD(F_PADD(ds_readd(BUFFER9_PTR), host_readws(Real2Host(ds_readd(BUFFER9_PTR)) + 4 * (route_nr - 1))), 0xec)));
	fb_start = ds_readfp(0xd2ff);
	add_ds_fp(0x425a, 4);

	memset((void*)Real2Host(ds_readfp(0xd299)), 170, 500);
	ds_writefp(0x426e, (RealPt)RealMake(datseg, 0x9dbd + 9 * route_nr));
	ds_writew(0x424c, 166);
	ds_writew(0x422e, TM_get_track_length(Real2Host(ds_readfp(0x425a))));
	ds_writew(0x4230, host_readb(Real2Host(ds_readd(0x426e)) + 2) * 100);
	ds_writew(0x4232, ds_readws(0x4230) / (ds_readws(0x424c) + host_readbs(Real2Host(ds_readd(0x426e)) + 3) * ds_readws(0x424c) / 10) * 60);
	ds_writew(0x4234, ds_readws(0x4232) / ds_readws(0x422e));
	ds_writew(0x423a, ds_readws(0x4230) / ds_readws(0x422e));

	if (ds_readws(0x423a) == 0)
	{
		ds_writew(0x423a, 1);
	}

	if (backwards)
	{
		while (host_readws(Real2Host(ds_readfp(0x425a))) != -1)
		{
			add_ds_fp(0x425a, 4);
		}
		sub_ds_fp(0x425a, 4);
	}

	memset((void*)(p_datseg + 0x4272), ds_writews(0x422a, 0), 60);
	memset((void*)(p_datseg + 0xe4c9), 0, 15);

	ds_writefp(0x426a, (RealPt)RealMake(datseg, 0xa66d));
	while (host_readb(Real2Host(ds_readd(0x426a))) != route_nr && host_readbs(Real2Host(ds_readd(0x426a))) != -1)
	{
		add_ds_fp(0x426a, 3);
	}

	ds_writew(TRV_RETURN, 0);
	ds_writefp(0x425e, ds_readfp(0x425a));
	ds_writew(0x423c, (ds_readws(0x424c) + host_readbs(Real2Host(ds_readd(0x426e)) + 3) * ds_readws(0x424c) / 10) * 18);

	/* random section starts */
	if (ds_readws(QUESTED_MONTHS) > 3)
	{
		if ((ds_writew(0x4244, (random_schick(100) <= 2 ? 1 : 0))) != 0)
		{
			ds_writew(0x4246, random_schick(ds_readws(0x423c)));
		}
	} else {
		ds_writew(0x4244, 0);
	}

	if ((ds_writew(0x4240, (random_schick(100) <= host_readb(Real2Host(ds_readd(0x426e)) + 4) ? 1: 0))) != 0)
	{
		ds_writew(0x4242, random_schick(ds_readws(0x423c)));
	}

	if ((ds_writew(0x4248, (random_schick(100) <= host_readb(Real2Host(ds_readd(0x426e)) + 7) / 3 ? 1: 0))) != 0)
	{
		ds_writew(0x424a, random_schick(ds_readws(0x423c)));
	}

	ds_writew(0x423c, 0);
	/* random section ends */

	while (host_readbs(Real2Host(ds_readfp(0x426a))) != -1 && host_readb(Real2Host(ds_readfp(0x426a))) == route_nr)
	{
		ptr = p_datseg + 0x4272 + 4 * ds_readws(0x422a);
		host_writew(ptr, host_readb(Real2Host(ds_readfp(0x426a)) + 1));
		host_writew(ptr + 2, host_readb(Real2Host(ds_readfp(0x426a)) + 2));

		if (backwards)
		{
			host_writew(ptr, host_readb(Real2Host(ds_readd(0x426e)) + 2) - host_readws(ptr));
		}

		mul_ptr_ws(ptr, 100);

		add_ds_fp(0x426a, 3);
		inc_ds_ws(0x422a);
	}

	ds_writew(0x422a, ds_writew(0x4238, ds_writeb(0x4333, 0)));

	while (host_readws(Real2Host(ds_readfp(0x425a)) + 2 * ds_writew(0x4236, 0)) != -1 &&
		!ds_readb(0x4333) &&
		ds_readw(0xc3c1) == 0)
	{
		if (is_mouse_in_rect(host_readws(Real2Host(ds_readfp(0x425a))) - 16,
					host_readws(Real2Host(ds_readfp(0x425a)) + 2) - 16,
					host_readws(Real2Host(ds_readfp(0x425a))) + 16,
					host_readws(Real2Host(ds_readfp(0x425a)) + 2) + 16))
		{
			update_mouse_cursor();
			ds_writew(0x4236, 1);
		}

		if (ds_readws(TRV_RETURN) == 2)
		{
			dec_ds_ws(0x422a);

			mem_writeb(Real2Phys(fb_start) + host_readws(Real2Host(ds_readfp(0x425a)) + 2) * 320 + host_readws(Real2Host(ds_readfp(0x425a))),
				host_readb(Real2Host(ds_readd(0xd299)) + ds_readws(0x422a)));

			mem_writeb(Real2Phys(ds_readd(0xd299)) + ds_readws(0x422a), 0xaa);
		} else {

			host_writeb(Real2Host(ds_readd(0xd299)) + ds_readws(0x422a),
				mem_readb(Real2Phys(fb_start) + host_readws(Real2Host(ds_readfp(0x425a)) + 2) * 320 + host_readws(Real2Host(ds_readfp(0x425a)))));

			inc_ds_ws(0x422a);

			mem_writeb(Real2Phys(fb_start) + host_readws(Real2Host(ds_readfp(0x425a)) + 2) * 320 + host_readws(Real2Host(ds_readfp(0x425a))), 0x1c);
		}

		if (ds_readw(0x4236) != 0)
		{
			refresh_screen_size();
		}

		ds_writew(0x4228, 0);
		while (ds_readws(0x4234) / 2 > ds_readws(0x4228))
		{
			handle_input();

			if (ds_readws(0x4228) % 2)
			{
				wait_for_vsync();
			}

			timewarp(MINUTES(2));

			inc_ds_ws(0x4228);
		}

		add_ds_ws(0x4238, (ds_readws(TRV_RETURN) == 2 ? -ds_readws(0x423a) : ds_readws(0x423a)));
		add_ds_ws(0x423c, ds_readws(0x423a));

		if (ds_readws(0xc3d3) != 0 || ds_readws(ACTION) == 73)
		{
			if (!ds_readb(0x4332))
			{
				answer = GUI_radio(get_ltx(0xcbc), 3,
							get_dtp(0x128),
							get_ltx(0xcc0),
							get_ltx(0x994));
			} else {
				answer = GUI_radio(get_ltx(0xcbc), 2,
							get_ltx(0xcc0),
							get_ltx(0x994));
			}

			if (answer == 1 && !ds_readb(0x4332))
			{
				ds_writew(0x422c, random_schick(10));
				ds_writew(0x424c, ds_readws(0x422a) + 197);
				ds_writeb(0x4332, 2);
				ds_writew(0x4232, ds_readws(0x4230) / (ds_readws(0x424c) + (host_readbs(Real2Host(ds_readd(0x426e)) + 3) * ds_readws(0x424c)) / 10) * 60);
				ds_writew(0x4234, ds_readws(0x4232) / ds_readws(0x422e));
				shr_ds_ws(0x422c, 1);

				hero = get_hero(0);
				for (ds_writew(0x4228, 0); ds_readws(0x4228) <= 6; inc_ds_ws(0x4228), hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != 0 &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero))
					{
						sub_hero_le(hero, ds_readws(0x422c));
					}
				}

			} else if ((answer == 1 && ds_readb(0x4332) != 0) ||
					(answer == 2 && !ds_readb(0x4332)))
			{
				ds_writew(WILDCAMP_SLEEP_QUALITY, -3);
				ds_writeb(0xe4c8, 99);
				ds_writeb(LOCATION, 6);
				do_location();
				ds_writeb(0xe4c8, ds_writeb(LOCATION, (unsigned char)ds_writew(WILDCAMP_SLEEP_QUALITY, 0)));
				ds_writew(WALLCLOCK_UPDATE, 0);
				ds_writew(REQUEST_REFRESH, 2);
			}
		}

		if (ds_readw(0x4240) != 0 && ds_readws(0x423c) >= ds_readws(0x4242) && ds_readws(0xc3c1) == 0)
		{
			random_encounter(route_nr);
			ds_writew(0x4240, 0);

		} else if (ds_readw(0x4248) != 0 && ds_readws(0x423c) >= ds_readws(0x424a) && ds_readws(0xc3c1) == 0)
		{
			do_wild8_fight();

		} else if (ds_readw(0x4244) != 0 && ds_readws(0x423c) >= ds_readws(0x4246) && ds_readws(0xc3c1) == 0)
		{
			ds_writew(TYPEINDEX, random_schick(100) <= 50 ? 10 : 12);
			bak1 = ds_readws(0x2ca2);
			bak2 = ds_readws(0x2ca4);
			ds_writew(0x2ca2, ds_writew(0x2ca4, 0));
			do_informer();
			ds_writew(0x2ca2, bak1);
			ds_writew(0x2ca4, bak2);

		}

		if (ds_readws(0xc3c1) == 0)
		{
			for (ds_writew(0x4228, 0); ds_readws(0x4228) < 15; inc_ds_ws(0x4228))
			{
				if ((ds_readws(0x4272 + 4 * ds_readws(0x4228)) <= ds_readws(0x4238) &&
					ds_readws(TRV_RETURN) == 0 &&
					!ds_readb(0xe4c9 + ds_readws(0x4228))) ||
					(ds_readws(0x4272 + 4 * ds_readws(0x4228)) >= ds_readws(0x4238) &&
					ds_readws(TRV_RETURN) == 2 &&
					ds_readbs(0xe4c9 + ds_readws(0x4228)) == 2))
				{
					if (ds_readws((0x4272 + 2) + 4 * ds_readws(0x4228)) != 0)
					{
						TRV_event(ds_readws((0x4272 + 2) + 4 * (l5 = ds_readws(0x4228))));

						if (!ds_readbs(SEA_TRAVEL + ds_readws((0x4272 + 2) + 4 * ds_readws(0x4228))))
						{
							ds_writeb(0xe4c9 + ds_readws(0x4228), 1);
						} else if (ds_readws(TRV_RETURN) == 0)
						{
							ds_writeb(0xe4c9 + ds_readws(0x4228), 2);
						} else {
							ds_writeb(0xe4c9 + ds_readws(0x4228), 0);
						}

						if (ds_readws(REQUEST_REFRESH) != 0 && !ds_readb(0x4333))
						{
							ds_writew(WALLCLOCK_UPDATE, 0);
						}
					}
				}
			}
		}

		if (ds_readds(DAY_TIMER) >= HOURS(20) &&
			!ds_readb(0x4333) &&
			ds_readws(0xc3c1) == 0 &&
			2 * ds_readws(0x423a) < ds_readws(0x4238) &&
			ds_readws(0x4230) - 2 * ds_readws(0x423a) > ds_readws(0x4238))
		{
			ds_writew(WALLCLOCK_UPDATE, 0);

			if (ds_readws(REQUEST_REFRESH) != 0)
			{
				load_map();
			}

			GUI_input(get_dtp(0x118), 0);

			ds_writeb(LOCATION, 6);
			do_location();
			ds_writeb(LOCATION, 0);

			ds_writew(REQUEST_REFRESH, 2);
			ds_writew(WALLCLOCK_UPDATE, 0);

			if (ds_readws(0xc3c1) == 0)
			{
				ds_writew(0x423c, (ds_readws(0x424c) + (host_readbs(Real2Host(ds_readd(0x426e)) + 3) * ds_readws(0x424c) / 10)) * 18);

				if ((ds_writew(0x4240, random_schick(100) <= host_readb(Real2Host(ds_readd(0x426e)) + 4) ? 1 : 0)) != 0)
				{
					ds_writew(0x4242, random_schick(ds_readws(0x423c)));
				}

				if ((ds_writew(0x4248, random_schick(100) <= host_readb(Real2Host(ds_readd(0x426e)) + 7) / 3 ? 1 : 0)) != 0)
				{
					ds_writew(0x424a, random_schick(ds_readws(0x423c)));
				}

				if (ds_readws(QUESTED_MONTHS) > 3)
				{
					if ((ds_writew(0x4244, random_schick(100) <= 2 ? 1 : 0)) != 0)
					{
						ds_writew(0x4246, random_schick(ds_readws(0x423c)));
					}
				} else {
					ds_writew(0x4244, 0);
				}

				ds_writew(0x423c, 0);
			}
		}

		if (ds_readw(CHECK_DISEASE) != 0 && !ds_readb(0x4333) && ds_readw(0xc3c1) == 0)
		{
			disease_effect();
		}

		if (ds_readw(REQUEST_REFRESH) != 0 && !ds_readb(0x4333) && ds_readw(0xc3c1) == 0)
		{
			update_mouse_cursor();
			load_map();
			bc_memmove((RealPt)ds_readd(0xd2ff), (RealPt)ds_readd(0x432e), 64000);

			wait_for_vsync();
			set_palette(Real2Host(ds_readd(0x432e)) + 64000 + 2, 0, 0x20);

			ds_writeb(PP20_INDEX, 5);
			ds_writew(0x4228, 0);
			ds_writefp(0x4262, ds_readfp(0x425e));

			if (route_nr == 59)
			{
				TM_func8(0);
			}

			while (host_readb(Real2Host(ds_readd(0xd299)) + inc_ds_ws_post(0x4228)) != 0xaa)
			{
				mem_writeb(Real2Phys(fb_start) + 320 * host_readws(Real2Host(ds_readd(0x4262)) + 2) + host_readws(Real2Host(ds_readd(0x4262))), 0x1c);
				add_ds_fp(0x4262, 2 * (!backwards ? 2 : -2));
			}

			refresh_screen_size();

			if (ds_readws(REQUEST_REFRESH) == 2 && route_nr != 59)
			{
				if (GUI_radio(get_dtp(0x11c), 2, get_dtp(0x120), get_dtp(0x124)) == 2)
				{
					ds_writew(TRV_RETURN, ds_readws(TRV_RETURN) == 0 ? 1 : -1);

					if (l5 != -1)
					{
						ds_writeb(0xe4c9 + l5, ds_readws(TRV_RETURN) == 1 ? 0 : 2);
					}
				}
			}
			ds_writew(WALLCLOCK_X, ds_readws(0x2ca2) + 120);
			ds_writew(WALLCLOCK_Y, ds_readws(0x2ca4) + 87);
			ds_writew(WALLCLOCK_UPDATE, 1);
			ds_writew(REQUEST_REFRESH, 0);
		}

		if (ds_readws(TRV_RETURN) == 1 || ds_readws(TRV_RETURN) == -1)
		{
			ds_writew(TRV_RETURN, ds_readws(TRV_RETURN) == 1 ? 2: 0);

			add_ds_fp(0x425a, 2 * ((!backwards && ds_readws(TRV_RETURN) == 0) || (backwards && ds_readws(TRV_RETURN) != 0) ? -2 : 2));
		}

		add_ds_fp(0x425a, 2 * ((!backwards && ds_readws(TRV_RETURN) == 0) || (backwards && ds_readws(TRV_RETURN) != 0) ? 2 : -2));
	}

	if (ds_readw(0xc3c1) == 0 && !ds_readb(0x4333) && ds_readw(TRV_RETURN) != 2)
	{
		update_mouse_cursor();

		do {
			add_ds_fp(0x425a, 2 * (!backwards ? -2 : 2));
			dec_ds_ws(0x422a);
			mem_writeb(Real2Phys(fb_start) + host_readws(Real2Host(ds_readfp(0x425a)) + 2) * 320 + host_readws(Real2Host(ds_readfp(0x425a))),
				host_readb(Real2Host(ds_readd(0xd299)) + ds_readws(0x422a)));

		} while (host_readws(Real2Host(ds_readfp(0x425a))) != -1);

		if (route_nr == 59)
		{
			TM_func8(1);
		}

		refresh_screen_size();
	}

	ds_writeb(SEA_TRAVEL, 0);
}

signed short TM_unused1(RealPt ptr, signed short off)
{
	signed short l_di;
	signed short l4;
	signed short answer;
	signed short l5;
	signed short l6;
	signed short l7;
	signed short l8;
	Bit8u *array[7];

	l8 = host_readb(Real2Host(host_readd(Real2Host(ptr) + 2)) + off) - 1;
	ds_writeb(CURRENT_TOWN, (signed char)(l7 = ds_readws(0x434a)));
	ptr = (RealPt)RealMake(datseg, 0xa0b4);

	do {
		if (host_readb(Real2Host(ptr)) == l7)
		{
			l_di = 0;
			while (host_readbs(Real2Host(host_readd(Real2Host(ptr) + 2)) + l_di) != -1)
			{
				if (host_readb(Real2Host(host_readd(Real2Host(ptr) + 2)) + l_di) - 1 == l8 &&
					(l_di || host_readb(Real2Host(host_readd(Real2Host(ptr) + 2)) + (l_di + 1)) != 255))
				{
					l5 = l6 = 0;
					while ((l4 = host_readb(Real2Host(host_readd(Real2Host(ptr) + 2)) + l6)) != 255)
					{
						if (l6 != l_di)
						{
							array[l5++] = get_ltx(4 * (0xeb + ds_writebs(0x4344 + l5, ((answer = ds_readb(0x9dbd + 9 * l4)) != ds_readbs(CURRENT_TOWN) ? (unsigned char)answer : ds_readb(0x9dbe + 9 * l4)))));
						}
						l6++;
					}

					ds_writeb(0x4344 + l5, (signed char)l7);
					array[l5] = get_ltx(0x88c);
					l5++;
					ds_writefp(0x4340, ptr);

					set_textbox_positions(l7);
					answer = GUI_radio(get_ltx(0x888), (signed char)l5,
								array[0],
								array[1],
								array[2],
								array[3],
								array[4],
								array[5]);

					if (answer == -1)
					{
						answer = l5;
					}

					ds_writew(0x434a, ds_readbs((0x4344 - 1) + answer));
					return answer;
				}

				l_di++;
			}
		}

		ptr += 6;

	} while (host_readbs(Real2Host(ptr)) != -1);

	return -1;
}

signed short TM_get_track_length(Bit8u *track)
{
	signed short length;

	length = 0;

	while (host_readws(track) != -1)
	{
		track += 4;
		length++;
	}
	return length;
}

signed short TM_enter_target_town(void)
{
	signed short l_si;
	signed short l_di;
	signed short l3;
	Bit8u *ptr1;
	Bit8u *ptr2;

	l_di = 0;
	ds_writew(0x4338, ds_readw(0x434a));
	l_di = 1;

	if (l_di)
	{
		ptr1 = p_datseg + 0xa0b4;
		l_di = 0;
		do {
			if (host_readb(ptr1) == ds_readw(0x4338))
			{
				l_si = 0;

				do {
					l3 = host_readb(Real2Host(host_readd(ptr1 + 2)) + l_si) - 1;

					if (ds_readbs(0x9dc6 + 9 * l3) == ds_readbs(CURRENT_TOWN) || ds_readbs(0x9dc7 + 9 * l3) == ds_readbs(CURRENT_TOWN))
					{
						l_di = host_readb(ptr1 + 1);
						break;
					}

					l_si++;

				} while (host_readb(Real2Host(host_readd(ptr1 + 2)) + l_si) != 255);
			}

			ptr1 += 6;

		} while (!l_di && host_readb(ptr1) != 255);

		if (l_di)
		{
			/* set the target town as current town */
			l3 = ds_readbs(CURRENT_TOWN);
			ds_writeb(CURRENT_TOWN, (signed char)ds_readws(0x4338));

			/* load the map */
			call_load_area(1);

			ptr2 = p_datseg + 0xc025;
			while (host_readb(ptr2 + 2) != 12 || host_readb(ptr2 + 3) != l_di)
			{
				ptr2 += 6;
			}

			l_si = host_readws(ptr2 + 4);
			ds_writew(0x433a, (l_si >> 8) & 0xff);
			ds_writew(0x433c, l_si & 0xf);
			ds_writew(0x433e, TM_get_looking_direction(host_readws(ptr2)));

			ds_writeb(CURRENT_TOWN, (signed char)l3);

			/* load the map */
			call_load_area(1);
		}
	}

	return 0;
}

signed short TM_get_looking_direction(signed short coordinates)
{
	signed short x;
	signed short y;
	signed short retval;

	x = (coordinates >> 8) & 0xff;
	y = coordinates & 0xf;

	retval = (ds_readws(0x433a) < x ? EAST :
			(ds_readws(0x433a) > x ? WEST :
			(ds_readws(0x433c) < y ? SOUTH : NORTH)));

	return retval;
}

void TM_draw_track(signed short a1, signed short a2, signed short a3, signed short a4)
{
	signed short i;
	Bit8u *ptr;
	RealPt fb_start;

	fb_start = (RealPt)ds_readd(0xd2ff);
	ptr = Real2Host(F_PADD(F_PADD(ds_readd(BUFFER9_PTR), host_readws(Real2Host(ds_readd(BUFFER9_PTR)) + 4 * (a1 - 1))), 0xec));
	ptr += 4;

	if (a3)
	{
		/* move ptr to the last valid value */
		while (host_readws(ptr) != -1)
		{
			ptr += 4;
		}

		ptr -= 4;
	}

	for (i = 0; i < a2; i++)
	{
		if (a4 == 0)
		{
			/* save the old pixel from the map */
			ds_writeb(0xe4b4 + i,
				mem_readb(Real2Phys(fb_start) + 320 * host_readws(ptr + 2) + host_readws(ptr)));

			/* write a new one */
			mem_writeb(Real2Phys(fb_start) + 320 * host_readws(ptr + 2) + host_readws(ptr), 0x1c);

			/* move the pointer */
			ptr += 2 * ((!a3 ? 2 : -2));
		} else {
			/* move the pointer */
			ptr += 2 * ((!a3 ? 2 : -2));

			/* restore the pixel from the map */
			mem_writeb(Real2Phys(fb_start) + 320 * host_readws(ptr + 2) + host_readws(ptr),
				ds_readb(0xe4b4 + i));
		}
	}
}

void TM_unused2(void)
{

}

void TM_func8(signed short a1)
{
	if (!(ds_readb(0xe4a2) & 1))
	{
		if (ds_readbs(CURRENT_TOWN) == 4)
		{
			TM_draw_track(11, 9, 0, a1);
		} else {
			TM_draw_track(11, 17, 1, a1);
		}
	} else {
		if (ds_readbs(CURRENT_TOWN) == 7)
		{
			TM_draw_track(14, 8, 0, a1);
		} else {
			TM_draw_track(14, 17, 1, a1);
		}
	}
}

void TM_func9(void)
{
	TM_func1(59, ds_readb(0xe4a2) & 1);

	TRV_event(145);
}

#if !defined(__BORLANDC__)
}
#endif
