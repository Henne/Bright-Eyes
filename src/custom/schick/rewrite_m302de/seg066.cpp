/**
 *	Rewrite of DSA1 v3.02_de functions of seg066 (city)
 *	Functions rewritten: 18/18 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg066.cpp
 */

#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg003.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg024.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg029.h"
#include "seg030.h"
#include "seg032.h"
#include "seg049.h"
#include "seg066.h"
#include "seg067.h"
#include "seg068.h"
#include "seg069.h"
#include "seg070.h"
#include "seg071.h"
#include "seg074.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
signed short get_border_index(unsigned char);
#endif

signed short enter_location(signed short town_id)
{
	signed short map_pos;
	signed short b_index;
	Bit8u *ptr;

	if (town_id == 40) {
		return enter_location_daspota();
	}

	map_pos = 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);
	ptr = p_datseg + 0xc025;
	ds_writeb(0xe10c, 0);

	do {
		if (host_readws(ptr) == map_pos) {

			/* found the location */
			ds_writeb(0x2d9f, 0);
			ds_writebs(LOCATION, host_readbs(ptr + 2));
			ds_writew(TYPEINDEX, host_readb(ptr + 3));
			ds_writew(CITYINDEX, host_readw(ptr + 4));

			if (ds_readbs(LOCATION) == 9) {
				ds_writebs(LOCATION, 0);
				ds_writeb(0xe10c, 1);
			}

			return 1;
		}

		ptr += 6;

	} while (host_readws(ptr) != -1);

	move();

	if ((b_index = get_border_index(cast_u16(ds_readbs((0xbd6e + 1))))) >= 2 && b_index <= 5) {

		ds_writeb(0x2d9f, 0);
		ds_writew(CITYINDEX, ds_readb(0x71c9 + town_id));

		if (!((ds_readbs(DIRECTION) + ds_readws(X_TARGET) + ds_readws(Y_TARGET)) & 1)) {
			ds_writebs(LOCATION, 10);
		} else {
			ds_writebs(LOCATION, 16);
			inc_ds_ws(CITYINDEX);
		}

		return 1;
	}

	return 0;
}

signed short enter_location_daspota(void)
{
	signed short map_pos;
	signed short b_index;
	Bit8u *ptr;

	if (ds_readws(0xc3c1) == 7) {
		return 1;
	}

	map_pos = 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);
	ptr = p_datseg + 0xc025;
	ds_writeb(0xe10c, 0);

	do {

		if (host_readws(ptr) == map_pos) {

			ds_writew(TYPEINDEX, host_readb(ptr + 3));

			if (host_readb(ptr + 2) != 12) {

				GUI_print_loc_line(get_dtp(4 * host_readw(ptr + 4)));

				if (!ds_readb(0x331f + host_readw(ptr + 4))) {

					do_talk(host_readbs(ptr + 2), host_readb(ptr + 3) - 1);

					if (!ds_readb(0x331f + host_readw(ptr + 4))) {
						turnaround();
						return 1;
					}
				}

				draw_main_screen();
				set_var_to_zero();

				load_ani(10);
				GUI_print_loc_line(get_dtp(4 * host_readw(ptr + 4)));
				init_ani(0);

				if (ds_readd(0x71fa + 4 * host_readw(ptr + 4))) {

					loot_multi_chest(Real2Host((RealPt)ds_readd(0x71fa + 4 * host_readw(ptr + 4))), get_dtp(0x54));

				} else {

					do {
						handle_gui_input();
					} while (ds_readws(ACTION) == 0 && ds_readws(MOUSE1_EVENT2) == 0);

					ds_writew(MOUSE1_EVENT2, 0);
				}

				set_var_to_zero();

				if (host_readw(ptr + 4) == 6) {
					do_fight(FIGHTS_DASP6B);
				} else if (host_readw(ptr + 4 ) == 12) {
					do_fight(FIGHTS_DASP12B);
				}

				turnaround();

			} else {
				ds_writeb(0x2d9f, 0);
				ds_writebs(LOCATION, host_readbs(ptr + 2));
				ds_writew(CITYINDEX, host_readw(ptr + 4));
			}

			return 1;
		}

		ptr += 6;

	} while (host_readws(ptr) != -1);

	move();

	if ((b_index = get_border_index(cast_u16(ds_readb((0xbd6e + 1))))) >= 2 && b_index <= 5) {

		ds_writeb(0x2d9f, 0);
		ds_writebs(LOCATION, 10);
		ds_writew(CITYINDEX, 19);
		return 1;
	}

	return 0;
}

void do_special_buildings(void)
{
	signed short type;
	signed short tw_bak;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	type = ds_readws(TYPEINDEX);

	if (ds_readb(CURRENT_TOWN) == 1) {
		/* THORWAL */

		load_city_ltx(type < 41 ? ARCHIVE_FILE_THORWAL1_LTX : ARCHIVE_FILE_THORWAL2_LTX);
		ds_writew(TEXTBOX_WIDTH, 9);

		if (type == 28) {
			THO_hetmann();
		} else if (type == 29) {
			THO_windriders();
		} else if (type == 30) {
			THO_stormchildren();
		} else if (type == 31) {
			THO_garaldsson();
		} else if (type == 32) {
			THO_eisenhof();
		} else if (type == 33) {
			THO_imman();
		} else if (type == 34) {
			THO_botschaft();
		} else if (type == 35) {
			GUI_output(get_city(0x118));
		} else if (type == 36) {
			GUI_output(get_city(0x11c));
		} else if (type == 37) {
			THO_bank();
		} else if (type == 38) {
			GUI_output(get_city(0x148));
		} else if (type == 39) {
			GUI_output(get_city(0x14c));
		} else if (type == 40) {
			GUI_output(get_city(0x150));
		} else if (type == 41) {
			THO_arsenal();
		} else if (type == 42) {
			THO_magistracy();
		} else if (type == 43) {
			THO_mueller();
		} else if (type == 44) {
			THO_black_finger();
		} else if (type == 45) {
			THO_ugdalf();
		} else if (type == 46) {
			THO_academy();
		} else if (type == 11 || type == 14 || type == 17 || type == 21) {
			THO_tav_inn_combi();
		}

	} else if (ds_readb(CURRENT_TOWN) == 18) {
		/* PHEXCAER */

		load_city_ltx(type <= 3 ? ARCHIVE_FILE_PHEX2_LTX : ARCHIVE_FILE_PHEX1_LTX);
		ds_writew(TEXTBOX_WIDTH, 9);

		if (type == 1) {
			PHX_phextempel();
		} else if (type == 3) {
			PHX_drogentempel();
		} else if (type == 18) {
			PHX_apotheke();
		} else if (type == 20) {
			PHX_healer();
		} else if (type == 23) {
			PHX_bordell();
		} else if (type == 24) {
			PHX_villa_gremob();
		} else if (type == 25) {
			PHX_spielhaus();
		} else if (type == 26) {
			PHX_fuhrhaus();
		} else if (type == 27) {
			PHX_stadthaus();
		}


	} else if (ds_readb(CURRENT_TOWN) == 21) {
		/*  HERMITS LAKE / EINSIEDLERSEE */

		if (type == 1) {
			load_ani(8);
			init_ani(1);
			do_talk(18, 0);
			set_var_to_zero();
		}
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	turnaround();
}

void TLK_eremit(signed short state)
{
	signed short i;
	Bit8u *hero;

	if (!state) {
		ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3615) != 0 ? 1 : 2);
	} else if (state == 6) {

		hero = get_hero(0);
		for (i = 0 ; i <= 6; i++, hero += SIZEOF_HERO) {

			/* remove hunger and thirst */
			host_writeb(hero + HERO_HUNGER, host_writebs(hero + HERO_THIRST, 0));

			/* heal all wounds */
			add_hero_le(hero, host_readws(hero + HERO_LE_ORIG));
		}

	} else if (state == 10) {
		/* group learns about two places to rest */
		ds_writeb(0x3e09, ds_writeb(0x3e08, 1));
	} else if (state == 13) {
		ds_writeb(0x3615, 1);
	} else if (state == 14) {
		timewarp(MINUTES(30));
	}
}

void do_town(void)
{
	if (ds_readbs(CITY_AREA_LOADED) != ds_readbs(CURRENT_TOWN) ||
		ds_readws(AREA_PREPARED) != 1)
	{
		seg028_0555(1);

		set_audio_track(ARCHIVE_FILE_THORWAL_XMI);

		ds_writew(REQUEST_REFRESH, 1);

		diary_new_entry();
	}

	ds_writews(CURRENT_ANI, -1);

	ds_writebs(0x2da6, ds_readbs(CURRENT_TOWN));

	city_step();
}

/* static */
void refresh_floor_and_sky(void)
{
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	nvf.dst = Real2Host(ds_readd(BUFFER1_PTR));
	nvf.src = Real2Host(ds_readd(TEX_SKY));
	nvf.nr = 0;
	nvf.type = 3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;

	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	nvf.dst = Real2Host(ds_readd(BUFFER1_PTR)) + 208 * height;
	nvf.src = Real2Host(ds_readd(TEX_FLOOR));
	nvf.nr = 0;
	nvf.type = 3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;

	process_nvf(&nvf);
}

void seg066_0692(void)
{
	refresh_floor_and_sky();
	move();
	seg066_06c1();
	seg066_0bad();

	/* TODO: these are write only variables */
	ds_writew(ALWAYS_ZERO2, ds_writew(ALWAYS_ZERO1, 0));

	city_water_and_grass();
	city_building_textures();
	city_fade_and_colors();
}

void seg066_06c1(void)
{
	signed short bi;

	if ((bi = get_border_index(ds_readb((0xbd6e + 3)))) >= 2 && bi <= 5) {
		ds_writeb((0xbd6e + 3), 0);
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 7)))) >= 2 && bi <= 5) {
		ds_writeb((0xbd6e + 7), 0);
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 8)))) >= 2 && bi <= 5) {
		ds_writeb((0xbd6e + 8), 0);
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 14)))) >= 2 && bi <= 5) {
		ds_writeb((0xbd6e + 14), 0);
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 15)))) >= 2 && bi <= 5) {
		ds_writeb((0xbd6e + 15), 0);
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 21)))) >= 2 && bi <= 5) {
		ds_writeb((0xbd6e + 21), 0);
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 0)))) >= 2 && bi <= 5) {

		ds_writeb((0xbd6e + 9), ds_writeb((0xbd6e + 15), ds_writeb((0xbd6e + 22), 0)));

		if ((bi = get_border_index(ds_readb((0xbd6e + 5)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 4), ds_writeb((0xbd6e + 10), ds_writeb((0xbd6e + 16), ds_writeb((0xbd6e + 23), 0))));
		}
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 2)))) >= 2 && bi <= 5) {

		ds_writeb((0xbd6e + 13), ds_writeb((0xbd6e + 21), ds_writeb((0xbd6e + 28),0)));

		if ((bi = get_border_index(ds_readb((0xbd6e + 5)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 6), ds_writeb((0xbd6e + 12), ds_writeb((0xbd6e + 20), ds_writeb((0xbd6e + 27), 0))));
		}
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 4)))) >= 2 && bi <= 5) {

		ds_writeb((0xbd6e + 9), ds_writeb((0xbd6e + 16), ds_writeb((0xbd6e + 22), ds_writeb((0xbd6e + 23), 0))));

		if ((bi = get_border_index(ds_readb((0xbd6e + 5)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 10), 0);
		}

		if ((bi = get_border_index(ds_readb((0xbd6e + 11)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 10), ds_writeb((0xbd6e + 17), ds_writeb((0xbd6e + 18), 0)));
			ds_writeb((0xbd6e + 24), ds_writeb((0xbd6e + 25), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 5)))) >= 2 && bi <= 5) {

		ds_writeb((0xbd6e + 11), ds_writeb((0xbd6e + 17), ds_writeb((0xbd6e + 18), 0)));

		ds_writeb((0xbd6e + 19), ds_writeb((0xbd6e + 24), ds_writeb((0xbd6e + 25), 0)));

		ds_writeb((0xbd6e + 26), 0);
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 6)))) >= 2 && bi <= 5) {

		ds_writeb((0xbd6e + 13), ds_writeb((0xbd6e + 20), ds_writeb((0xbd6e + 27), ds_writeb((0xbd6e + 28), 0))));

		if ((bi = get_border_index(ds_readb((0xbd6e + 5)))) >= 2 && bi <= 5) {

			ds_writeb((0xbd6e + 11), ds_writeb((0xbd6e + 12), ds_writeb((0xbd6e + 17), 0)));

			ds_writeb((0xbd6e + 18), ds_writeb((0xbd6e + 19), ds_writeb((0xbd6e + 24), 0)));

			ds_writeb((0xbd6e + 25), ds_writeb((0xbd6e + 26), 0));
		}

		if ((bi = get_border_index(ds_readb((0xbd6e + 11)))) >= 2 && bi <= 5) {

			ds_writeb((0xbd6e + 12), ds_writeb((0xbd6e + 18), ds_writeb((0xbd6e + 19), 0)));

			ds_writeb((0xbd6e + 25), ds_writeb((0xbd6e + 26), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 9)))) >= 2 && bi <= 5) {

		ds_writeb((0xbd6e + 22), 0);

		if ((bi = get_border_index(ds_readb((0xbd6e + 17)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 16), ds_writeb((0xbd6e + 23), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 13)))) >= 2 && bi <= 5) {

		ds_writeb((0xbd6e + 28), 0);

		if ((bi = get_border_index(ds_readb((0xbd6e + 19)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 20), ds_writeb((0xbd6e + 27), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 10)))) >= 2 && bi <= 5) {

		ds_writeb((0xbd6e + 9), ds_writeb((0xbd6e + 16), ds_writeb((0xbd6e + 22), ds_writeb((0xbd6e + 23), 0))));

		if ((bi = get_border_index(ds_readb((0xbd6e + 11)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 18), 0);
		}

		if (((bi = get_border_index(ds_readb((0xbd6e + 11)))) >= 2 && bi <= 5) ||
			((bi = get_border_index(ds_readb((0xbd6e + 18)))) >= 2 && bi <= 5)) {

			ds_writeb((0xbd6e + 17), ds_writeb((0xbd6e + 24), ds_writeb((0xbd6e + 25), 0)));
		}
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 12)))) >= 2 && bi <= 5) {

		ds_writeb((0xbd6e + 13), ds_writeb((0xbd6e + 20), ds_writeb((0xbd6e + 27), ds_writeb((0xbd6e + 28), 0))));

		if ((bi = get_border_index(ds_readb((0xbd6e + 11)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 18), 0);
		}

		if (((bi = get_border_index(ds_readb((0xbd6e + 11)))) >= 2 && bi <= 5) ||
			((bi = get_border_index(ds_readb((0xbd6e + 18)))) >= 2 && bi <= 5)) {

			ds_writeb((0xbd6e + 19), ds_writeb((0xbd6e + 25), ds_writeb((0xbd6e + 26), 0)));
		}
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 11)))) >= 2 && bi <= 5) {
		ds_writeb((0xbd6e + 18), ds_writeb((0xbd6e + 25), 0));
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 16)))) >= 2 && bi <= 5) {

		ds_writeb((0xbd6e + 22), 0);

		if ((bi = get_border_index(ds_readb((0xbd6e + 24)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 23), 0);
		}
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 20)))) >= 2 && bi <= 5) {

		ds_writeb((0xbd6e + 28), 0);

		if ((bi = get_border_index(ds_readb((0xbd6e + 26)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 27), 0);
		}
	}

	if ((bi = get_border_index(ds_readb((0xbd6e + 18)))) >= 2 && bi <= 5) {
		ds_writeb((0xbd6e + 25), 0);
	}

	if (((bi = get_border_index(ds_readb((0xbd6e + 18)))) >= 2 && bi <= 5) ||
		((bi = get_border_index(ds_readb((0xbd6e + 25)))) >= 2 && bi <= 5)) {

		if ((bi = get_border_index(ds_readb((0xbd6e + 17)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 24), 0);
		}

		if ((bi = get_border_index(ds_readb((0xbd6e + 19)))) >= 2 && bi <= 5) {
			ds_writeb((0xbd6e + 26), 0);
		}
	}
}

#if !defined(__BORLANDC__)
signed short get_border_index(unsigned short val)
#else
signed short get_border_index(unsigned char val)
#endif
{
	signed short i;

	i = 0;
	while (ds_readb(0x7246 + i) < val) {
		i++;
	}

	ds_writew(0xe412, (((val & 3) + 4) - ds_readbs(DIRECTION)) & 3);

	if (i == 0)
		i = 1;

	i--;

	return i;
}

void seg066_0bad(void)
{
	signed short i;
	signed short bi;

	for (i = 28; i >= 0; i--) {

		bi = get_border_index(ds_readb((0xbd6e + i)));

		if (bi == -1) {
			ds_writebs(0xbd50 + i, -1);
		} else {
			ds_writebs(0xbd50 + i,	bi == 2 ? ds_readbs(0x7252 + i) : (
						bi == 3 ? ds_readbs(0x726f + i) : (
						bi == 4 ? ds_readbs(0x728c + i) : (
						bi == 5 ? ds_readbs(0x72a9 + i) : (
						bi == 6 ? ds_readbs(0x72c6 + i) : (
						bi == 7 ? ds_readbs(0x72c6 + i) : (
						bi == 8 ? ds_readbs(0x72e3 + i) : (
						bi == 9 ? ds_readbs(0x7300 + i) : (
						bi == 10 ? ds_readbs(0x731d + i) : (
						bi == 1 ? ds_readbs(0x728c + i) : -1))))))))));
		}
	}
}

/**
 *  \brief	draws water and grass textures
 */
void city_water_and_grass(void)
{
	signed short i;
	signed short nvf_nr;
	signed short x;
	signed short y;
	signed char c1;
	signed char bi;
	unsigned char c2;
	Bit8u *ptr;

	for (i = 0; i < 29; i++) {

		c1 = ds_readbs(0x7c24 + i);
		c2 = ds_readb(0xbd6e + c1);

		if (c2 != 0) {

			bi = (signed char)get_border_index(c2);

			if (bi == 6 || bi == 7) {
				/* water or grass */

				ptr = 4 * c1 + p_datseg + 0x7496;

				x = host_readws(ptr);
				y = host_readws(ptr + 2);

				c1 = ds_readbs(0xbd50 + c1);

				if (c1 != -1) {

					ptr = c1 * 18 + p_datseg + 0x74f8;

					if ((nvf_nr = host_readws(ptr + 4)) != -1) {

						if (bi == 7) {
							nvf_nr += 15;
						}

						load_city_texture(x + host_readws(ptr), y + host_readws(ptr + 2), nvf_nr, 184);
					}
				}
			}
		}
	}
}

/**
 *  \brief	draws building textures
 */
void city_building_textures(void)
{
	signed short nvf_nr;
	signed short i;
	signed short x;
	signed short y;
	signed short l4;
	signed char c1;
	signed char bi;
	unsigned char c2;
	Bit8u *ptr;

	for (i = 0; i < 29; i++) {

		c1 = ds_readbs(0x7c24 + i);
		c2 = ds_readb(0xbd6e + c1);

		if (c2 != 0) {

			bi = (signed char)get_border_index(c2);

			if (bi != 7 && bi != 6) {

				ptr = p_datseg + 0x733a + 4 * c1;

				if (bi == 8) {
					ptr = p_datseg + 0x73ae + 4 * c1;
				} else if (bi == 9 || bi == 10) {
					ptr = p_datseg + 0x7422 + 4 * c1;
				}

				x = host_readws(ptr);
				y = host_readws(ptr + 2);

				c1 = ds_readbs(0xbd50 + c1);

				if (c1 != -1) {

					ptr = 18 * c1 + p_datseg + 0x74f8;

					l4 =	bi == 2 ? 186 : (
						bi == 3 ? 187 : (
						bi == 4 ? 188 : (
						bi == 5 ? 189 : (
						bi == 1 ? 188 : (
						bi == 9 ? 232 : (
						bi == 10 ? 233 : 185))))));

					if ((nvf_nr = host_readws(ptr + 4)) != -1) {

						if (ds_readws(0xe412) == 2 && bi >= 1 && bi <= 5) {

							if (bi == 1) {
								nvf_nr -= 5;
								l4 = 185;
							} else {
								nvf_nr -= 10;
							}
						}

						if (bi == 9 || bi == 10) {
							load_special_textures(bi);
						}

						load_city_texture(x + host_readws(ptr), y + host_readws(ptr + 2), nvf_nr, l4);

						if (bi == 9 || bi == 10) {
							call_load_buffer();
						}
					}

					if ((nvf_nr = host_readws(ptr + 10)) != -1) {

						if (bi == 1) {
							l4 = 188;
						}

						if (ds_readws(0xe412) == 1 &&
							!(nvf_nr & 0x8000) &&
							bi >= 1 && bi <= 5)
						{
							nvf_nr -= 10;

						} else if (ds_readws(0xe412) == 3 && (nvf_nr & 0x8000))
						{
							nvf_nr = ((unsigned short)(nvf_nr & 0x7fff) - 10) | 0x8000;
						}

						load_city_texture(x + host_readws(ptr + 6), y + host_readws(ptr + 8), nvf_nr, l4);
					}

					if ((nvf_nr = host_readws(ptr + 0x10)) != -1) {
						load_city_texture(x + host_readws(ptr + 12), y + host_readws(ptr + 14), nvf_nr, l4);
					}
				}
			}
		}
	}
}

void load_city_texture(signed short v1, signed short v2, signed short nvf_nr,
		signed short v4)
{
	signed short width;
	signed short height;
	signed short copy_width;
	signed short copy_height;
	signed short direction;
	Bit8u *src;
	Bit8u *dst;
	struct nvf_desc nvf;

	direction = (nvf_nr & 0x8000 ? 1: 0);
	nvf_nr &= 0x3fff;

	v4 -= 184;

	nvf.dst = src = Real2Host(ds_readd(BUFFER1_PTR)) + 30000;
	nvf.src = Real2Host(ds_readd(TEX_FLOOR + v4 * 4));

	if (v4 == 48 || v4 == 49) {
		nvf.src = Real2Host(ds_readd(BUFFER7_PTR));
	}

	nvf.nr = nvf_nr;
	nvf.type = (direction == 0 ? 3: 5);
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	height = host_readws((Bit8u*)&height);
	width = host_readws((Bit8u*)&width);
#endif

	copy_width = width;
	copy_height = height;

	if (v1 < 0) {
		if (v1 + copy_width > 0) {
			copy_width += v1;
			src += __abs__(v1);
			v1 = 0;
		}
	}

	if (v2 < 0) {
		if (v2 + copy_height > 0) {
			copy_height -= v2;
			src += __abs__(v2) * width;
			v2 = 0;
		}
	}


	if (v1 < 208 && v2 < 135 && v1 >= 0 && v2 >= 0) {

		if (v1 + copy_width > 208) {
			copy_width = 208 - v1;
		}

		if (v2 + copy_height > 135) {
			copy_height = 135 - v2;
		}

		dst = Real2Host(ds_readd(BUFFER1_PTR)) + v2 * 208 + v1;

		copy_solid(dst, src, copy_width, copy_height, 208, width,
			v4 == 0 ? 0 : 128);
	}
}

void seg066_10c8(void)
{
	set_var_to_zero();
	seg066_0692();
	ds_writews(CITY_REFRESH_X_TARGET, ds_readws(X_TARGET));
	ds_writews(CITY_REFRESH_Y_TARGET, ds_readws(Y_TARGET));
	ds_writews(CITY_REFRESH_DIRECTION, ds_readbs(DIRECTION));
}

signed short city_step(void)
{
	signed short i;
	signed short bi;
	signed short options;
	signed short l4;

	ds_writebs((0xbd38 + 0), 12);
	l4 = ds_readbs((0xbd38 + 1));
	ds_writebs((0xbd38 + 1), ds_readbs(CAN_MERGE_GROUP) == -1 ? 45 : 15);

	if (ds_readbs((0xbd38 + 1)) != l4) {
		ds_writew(0xd013, 1);
	}

	ds_writebs((0xbd38 + 2), 29);
	ds_writebs((0xbd38 + 3), 37);
	ds_writebs((0xbd38 + 4), 39);
	ds_writebs((0xbd38 + 5), 11);
	ds_writebs((0xbd38 + 6), 54);

	if (ds_readws(REQUEST_REFRESH) != 0) {

		draw_main_screen();
		GUI_print_loc_line(get_dtp(0x0000));

		ds_writew(REQUEST_REFRESH, ds_writews(0xd013, 0));
		ds_writews(CITY_REFRESH_X_TARGET, -1);
	}

	if (ds_readw(0xd013) != 0 && ds_readbs(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK) {
		draw_icons();
		ds_writews(0xd013, 0);
	}

	/* check if position or direction has changed */
	if (ds_readbs(DIRECTION) != ds_readws(CITY_REFRESH_DIRECTION) ||
		ds_readws(X_TARGET) != ds_readws(CITY_REFRESH_X_TARGET) ||
		ds_readws(Y_TARGET) != ds_readws(CITY_REFRESH_Y_TARGET))
	{
		seg066_10c8();
	}

	if (ds_readws(X_TARGET) != ds_readws(0x2d83) ||
		ds_readws(Y_TARGET) != ds_readws(0x2d85))
	{
		ds_writebs(CAN_MERGE_GROUP, (signed char)can_merge_group());
		set_automap_tiles(ds_readws(X_TARGET), ds_readws(Y_TARGET));
	}

	ds_writew(0x2d83, ds_readws(X_TARGET));
	ds_writew(0x2d85, ds_readws(Y_TARGET));

	handle_gui_input();

	if (ds_readw(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == 73) {

		for (i = options = 0; i < 9; i++) {
			if (ds_readbs(0xbd38 + i) != -1) {
				options++;
			}
		}

		i = GUI_radio(get_ltx(0x8e8), (signed char)options,
				get_ltx(0x85c), get_ltx(0x860), get_ltx(0x864),
				get_ltx(0x868), get_ltx(0x86c), get_ltx(0x354),
				get_ltx(0x4c8), get_ltx(0x8e4)) - 1;

		if (i != -2) {
			ds_writew(ACTION, i + 129);
		}
	}

	i = 0;

	if (ds_readws(ACTION) == 129) {

		GRP_split();
		ds_writebs(CAN_MERGE_GROUP, (signed char)can_merge_group());

	} else if (ds_readws(ACTION) == 130) {

		GRP_merge();
		ds_writebs(CAN_MERGE_GROUP, -1);

	} else if (ds_readws(ACTION) == 131) {

		GRP_switch_to_next(0);
		i = 1;

	} else if (ds_readws(ACTION) == 132) {

		game_options();

	} else if (ds_readws(ACTION) == 133) {

		show_automap();

	} else if (ds_readws(ACTION) == 134) {

		select_magic_user();

	} else if (ds_readws(ACTION) == 135) {

		ds_writebs(LOCATION, 18);
		ds_writeb(0xbd27, 1);
		i = 1;

	} else if (ds_readws(ACTION) == 136 && ds_readbs((0xbd38 + 7)) != -1) {

		ds_writebs(LOCATION, 9);
		i = 1;

	} else if (ds_readws(ACTION) == 75) {

		update_direction(3);

	} else if (ds_readws(ACTION) == 77) {

		update_direction(1);

	} else if (ds_readws(ACTION) == 72) {

		bi = get_border_index(ds_readb(0xbd4d));

		if (!bi || bi == 7 || bi == 8) {
			seg066_14dd(1);
		} else if (bi >= 1 && bi <= 5 && ds_readw(0xe412) == 2) {
			seg066_14dd(1);
		} else {
			no_way();
		}

	} else if (ds_readws(ACTION) == 80) {

		bi = get_border_index(ds_readb(0xbd4e));

		if (!bi || bi == 7 || bi == 8) {
			seg066_14dd(-1);
		} else {
			no_way();
		}
	}

	if (ds_readb(CURRENT_TOWN) != 0 && ds_readbs(CITY_AREA_LOADED) != -1) {

		if (!i) {
			options = enter_location(ds_readbs(CURRENT_TOWN));
		}

		/* check move and a big city */
		if ((ds_readws(Y_TARGET) != ds_readws(0x2d85) ||
			(ds_readws(X_TARGET) != ds_readws(0x2d83))) &&

			(ds_readb(CURRENT_TOWN) == 1 || ds_readb(CURRENT_TOWN) == 39 ||
			ds_readb(CURRENT_TOWN) == 18 || ds_readb(CURRENT_TOWN) == 17))
		{

			/* roll a dice */
			if (random_schick(100) <= 1 &&
				ds_readds(DAY_TIMER) > HOURS(8) &&
				ds_readds(DAY_TIMER) < HOURS(20))
			{
				city_event_switch();
			}
		}

		if (ds_readb(0xe10c) != 0 && ds_readb((0xbd38 + 7)) != 43) {

			if (((i = ds_readws((0x70a8 + 4) + 8 * ds_readws(TYPEINDEX))) == -1 ||
				ds_readbs(DAY_OF_WEEK) == i) &&
				ds_readds(DAY_TIMER) >= HOURS(6) &&
				ds_readds(DAY_TIMER) <= HOURS(16))
			{
				ds_writebs((0xbd38 + 7), 43);
				draw_icons();
			}

		} else if (!ds_readbs(0xe10c) && ds_readbs((0xbd38 + 7)) == 43) {

			ds_writebs((0xbd38 + 7), -1);
			draw_icons();
		}
	}

	return 0;
}

void seg066_14dd(signed short forward)
{
	signed short dir;

	timewarp(MINUTES(2));

	dir = ds_readbs(DIRECTION);

	if (forward == 1) {

		if (!dir) {
			dec_ds_ws(Y_TARGET);
		} else if (dir == 1) {
			inc_ds_ws(X_TARGET);
		} else if (dir == 2) {
			inc_ds_ws(Y_TARGET);
		} else {
			dec_ds_ws(X_TARGET);
		}

	} else {

		if (!dir) {
			inc_ds_ws(Y_TARGET);
		} else if (dir == 1) {
			dec_ds_ws(X_TARGET);
		} else if (dir == 2) {
			dec_ds_ws(Y_TARGET);
		} else {
			inc_ds_ws(X_TARGET);
		}
	}

	if (ds_readws(X_TARGET) < 0) {

		ds_writews(X_TARGET, 0);
		no_way();

	} else if (ds_readb(DNG_MAP_SIZE) - 1 < ds_readws(X_TARGET)) {

		ds_writews(X_TARGET, ds_readb(DNG_MAP_SIZE) - 1);
		no_way();

	}

	if (ds_readws(Y_TARGET) < 0) {

		ds_writews(Y_TARGET, 0);
		no_way();

	} else if (ds_readws(Y_TARGET) > 15) {

		ds_writews(Y_TARGET, 15);
		no_way();
	}
}

void city_fade_and_colors(void)
{
	signed short i;
	Bit8u *dst;
	Bit8u *pal_ptr;

	if (ds_readb(0x4475) == 2) {

		fade_into();
		ds_writeb(0x4475, 1);

	}

	if (ds_readb(0x4475) == 3) {

		set_palette(p_datseg + 0x26c3, 0x00, 0x20);
		set_palette(p_datseg + 0x26c3, 0x80, 0x20);
		set_palette(p_datseg + 0x26c3, 0xa0, 0x20);

		ds_writeb(0x4475, 1);
	}

	draw_compass();

	ds_writew(PIC_COPY_X1, ds_readws(0xce41));
	ds_writew(PIC_COPY_Y1, ds_readws(0xce3f));
	ds_writew(PIC_COPY_X2, ds_readws(0xce41) + 207);
	ds_writew(PIC_COPY_Y2, ds_readws(0xce3f) + 134);
	ds_writed(PIC_COPY_SRC, ds_readd(BUFFER1_PTR));

	ds_writeb(0x45b8, 0);

	update_mouse_cursor();
	wait_for_vsync();

	do_pic_copy(1);

	refresh_screen_size();

	if (ds_readb(0x4475) != 0) {

		dst = Real2Host(ds_readd(BUFFER1_PTR)) + 500;
		pal_ptr = Real2Host(ds_readd(BUFFER1_PTR));

		memset(Real2Host(ds_readd(BUFFER1_PTR)), 0, 0x120);
		memcpy(dst, p_datseg + 0x3e53, 0x120);

		for (i = 0; i < 64; i += 2) {

			pal_fade_in(pal_ptr, dst, i, 0x60);
			pal_fade_in(pal_ptr, dst, i + 1, 0x60);

			wait_for_vsync();

			set_palette(pal_ptr, 0x00, 0x20);
			set_palette(pal_ptr + 0x60, 0x80, 0x40);
		}

		ds_writeb(0x4475, 0);

	} else {

		wait_for_vsync();

		set_palette(p_datseg + 0x3e53, 0x00, 0x20);
		set_palette(p_datseg + 0x3eb3, 0x80, 0x40);
	}
}

void seg066_172b(void)
{
	signed short l_si;
	signed short l_di;
	Bit8u *ptr = p_datseg + DNG_MAP;

	ds_writeb(0xe400, ds_writeb((0xe400+1), ds_writeb((0xe400+2), ds_writeb((0xe400+3), 0))));

	for (l_di = 0; ds_readb(DNG_MAP_SIZE) * 16 > l_di; l_di++) {

		l_si = get_border_index(host_readb(ptr + l_di));

		if (l_si == 2) {
			inc_ds_bs_post(0xe400);
		} else if (l_si == 3) {
			inc_ds_bs_post((0xe400+1));
		} else if ((l_si == 4) || (l_si == 1)) {
			inc_ds_bs_post((0xe400+2));
		} else if (l_si == 5) {
			inc_ds_bs_post((0xe400+3));
		}
	}

	l_di = 5;
	l_si = 2000;

	if (ds_readb(0xe400) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 0));
	}

	if (ds_readb((0xe400+1)) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 1));
	}

	if (ds_readb((0xe400+2)) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 2));
	}

	if (ds_readb((0xe400+3)) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 3));
	}

	ds_writeb(0xe400 + l_di, 0);
}

#if !defined(__BORLANDC__)
}
#endif

