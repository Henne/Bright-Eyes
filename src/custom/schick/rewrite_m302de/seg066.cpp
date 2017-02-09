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
	ptr = p_datseg + LOCATIONS_TAB;
	ds_writeb(LOCATION_MARKET_FLAG, 0);

	do {
		if (host_readws(ptr) == map_pos) {

			/* found the location */
			ds_writeb(LOCATION_BAK, 0);
			ds_writebs(LOCATION, host_readbs(ptr + 2));
			ds_writew(TYPEINDEX, host_readb(ptr + 3));
			ds_writew(CITYINDEX, host_readw(ptr + 4));

			if (ds_readbs(LOCATION) == LOCATION_MARKET) {
				ds_writebs(LOCATION, 0);
				ds_writeb(LOCATION_MARKET_FLAG, 1);
			}

			return 1;
		}

		ptr += 6;

	} while (host_readws(ptr) != -1);

	move();

	if ((b_index = get_border_index(cast_u16(ds_readbs((VISUAL_FIELD_VALS + 1))))) >= 2 && b_index <= 5) {

		ds_writeb(LOCATION_BAK, 0);
		ds_writew(CITYINDEX, ds_readb((TOWNS_CITYINDEX_TABLE-1) + town_id));

		if (!((ds_readbs(DIRECTION) + ds_readws(X_TARGET) + ds_readws(Y_TARGET)) & 1)) {
			ds_writebs(LOCATION, LOCATION_CITIZEN);
		} else {
			ds_writebs(LOCATION, LOCATION_HOUSE);
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

	if (ds_readws(GAME_STATE) == GAME_STATE_FIGQUIT) {
		return 1;
	}

	map_pos = 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);
	ptr = p_datseg + LOCATIONS_TAB;
	ds_writeb(LOCATION_MARKET_FLAG, 0);

	do {

		if (host_readws(ptr) == map_pos) {

			ds_writew(TYPEINDEX, host_readb(ptr + 3));

			if (host_readb(ptr + 2) != 12) {

				GUI_print_loc_line(get_tx(host_readw(ptr + 4)));

				if (!ds_readb(DASPOTA_FIGHTFLAGS + host_readw(ptr + 4))) {

					do_talk(host_readbs(ptr + 2), host_readb(ptr + 3) - 1);

					if (!ds_readb(DASPOTA_FIGHTFLAGS + host_readw(ptr + 4))) {
						turnaround();
						return 1;
					}
				}

				draw_main_screen();
				set_var_to_zero();

				load_ani(10);
				GUI_print_loc_line(get_tx(host_readw(ptr + 4)));
				init_ani(0);

				if (ds_readd((DASPOTA_LOCLOOT_INDEX - 4) + 4 * host_readw(ptr + 4))) {

					loot_multi_chest(Real2Host((RealPt)ds_readd((DASPOTA_LOCLOOT_INDEX - 4) + 4 * host_readw(ptr + 4))), get_tx(21));

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
				ds_writeb(LOCATION_BAK, 0);
				ds_writebs(LOCATION, host_readbs(ptr + 2));
				ds_writew(CITYINDEX, host_readw(ptr + 4));
			}

			return 1;
		}

		ptr += 6;

	} while (host_readws(ptr) != -1);

	move();

	if ((b_index = get_border_index(cast_u16(ds_readb((VISUAL_FIELD_VALS + 1))))) >= 2 && b_index <= 5) {

		ds_writeb(LOCATION_BAK, 0);
		ds_writebs(LOCATION, LOCATION_CITIZEN);
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

	if (ds_readb(CURRENT_TOWN) == TOWNS_THORWAL) {

		load_tx2(type < 41 ? ARCHIVE_FILE_THORWAL1_LTX : ARCHIVE_FILE_THORWAL2_LTX);
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
			GUI_output(get_tx2(70));
		} else if (type == 36) {
			GUI_output(get_tx2(71));
		} else if (type == 37) {
			THO_bank();
		} else if (type == 38) {
			GUI_output(get_tx2(82));
		} else if (type == 39) {
			GUI_output(get_tx2(83));
		} else if (type == 40) {
			GUI_output(get_tx2(84));
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

	} else if (ds_readb(CURRENT_TOWN) == TOWNS_PHEXCAER) {

		load_tx2(type <= 3 ? ARCHIVE_FILE_PHEX2_LTX : ARCHIVE_FILE_PHEX1_LTX);
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


	} else if (ds_readb(CURRENT_TOWN) == TOWNS_EINSIEDL) {
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
		ds_writew(DIALOG_NEXT_STATE, ds_readb(HERMIT_VISITED) != 0 ? 1 : 2);
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
		ds_writeb(TEVENT137_FLAG, ds_writeb(TEVENT134_FLAG, 1));
	} else if (state == 13) {
		ds_writeb(HERMIT_VISITED, 1);
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

	ds_writebs(CURRENT_TOWN_BAK, ds_readbs(CURRENT_TOWN));

	city_step();
}

#if defined(__BORLANDC__)
static
#endif
void refresh_floor_and_sky(void)
{
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	nvf.dst = Real2Host(ds_readd(RENDERBUF_PTR));
	nvf.src = Real2Host(ds_readd(TEX_SKY));
	nvf.no = 0;
	nvf.type = 3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;

	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	nvf.dst = Real2Host(ds_readd(RENDERBUF_PTR)) + 208 * height;
	nvf.src = Real2Host(ds_readd(TEX_FLOOR));
	nvf.no = 0;
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

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 3)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 3), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 7)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 7), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 8)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 8), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 14)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 14), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 15)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 15), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 21)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 21), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 0)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 9), ds_writeb((VISUAL_FIELD_VALS + 15), ds_writeb((VISUAL_FIELD_VALS + 22), 0)));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 5)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 4), ds_writeb((VISUAL_FIELD_VALS + 10), ds_writeb((VISUAL_FIELD_VALS + 16), ds_writeb((VISUAL_FIELD_VALS + 23), 0))));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 2)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 13), ds_writeb((VISUAL_FIELD_VALS + 21), ds_writeb((VISUAL_FIELD_VALS + 28),0)));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 5)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 6), ds_writeb((VISUAL_FIELD_VALS + 12), ds_writeb((VISUAL_FIELD_VALS + 20), ds_writeb((VISUAL_FIELD_VALS + 27), 0))));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 4)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 9), ds_writeb((VISUAL_FIELD_VALS + 16), ds_writeb((VISUAL_FIELD_VALS + 22), ds_writeb((VISUAL_FIELD_VALS + 23), 0))));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 5)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 10), 0);
		}

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 10), ds_writeb((VISUAL_FIELD_VALS + 17), ds_writeb((VISUAL_FIELD_VALS + 18), 0)));
			ds_writeb((VISUAL_FIELD_VALS + 24), ds_writeb((VISUAL_FIELD_VALS + 25), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 5)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 11), ds_writeb((VISUAL_FIELD_VALS + 17), ds_writeb((VISUAL_FIELD_VALS + 18), 0)));

		ds_writeb((VISUAL_FIELD_VALS + 19), ds_writeb((VISUAL_FIELD_VALS + 24), ds_writeb((VISUAL_FIELD_VALS + 25), 0)));

		ds_writeb((VISUAL_FIELD_VALS + 26), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 6)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 13), ds_writeb((VISUAL_FIELD_VALS + 20), ds_writeb((VISUAL_FIELD_VALS + 27), ds_writeb((VISUAL_FIELD_VALS + 28), 0))));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 5)))) >= 2 && bi <= 5) {

			ds_writeb((VISUAL_FIELD_VALS + 11), ds_writeb((VISUAL_FIELD_VALS + 12), ds_writeb((VISUAL_FIELD_VALS + 17), 0)));

			ds_writeb((VISUAL_FIELD_VALS + 18), ds_writeb((VISUAL_FIELD_VALS + 19), ds_writeb((VISUAL_FIELD_VALS + 24), 0)));

			ds_writeb((VISUAL_FIELD_VALS + 25), ds_writeb((VISUAL_FIELD_VALS + 26), 0));
		}

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) {

			ds_writeb((VISUAL_FIELD_VALS + 12), ds_writeb((VISUAL_FIELD_VALS + 18), ds_writeb((VISUAL_FIELD_VALS + 19), 0)));

			ds_writeb((VISUAL_FIELD_VALS + 25), ds_writeb((VISUAL_FIELD_VALS + 26), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 9)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 22), 0);

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 17)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 16), ds_writeb((VISUAL_FIELD_VALS + 23), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 13)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 28), 0);

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 19)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 20), ds_writeb((VISUAL_FIELD_VALS + 27), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 10)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 9), ds_writeb((VISUAL_FIELD_VALS + 16), ds_writeb((VISUAL_FIELD_VALS + 22), ds_writeb((VISUAL_FIELD_VALS + 23), 0))));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 18), 0);
		}

		if (((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) ||
			((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 18)))) >= 2 && bi <= 5)) {

			ds_writeb((VISUAL_FIELD_VALS + 17), ds_writeb((VISUAL_FIELD_VALS + 24), ds_writeb((VISUAL_FIELD_VALS + 25), 0)));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 12)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 13), ds_writeb((VISUAL_FIELD_VALS + 20), ds_writeb((VISUAL_FIELD_VALS + 27), ds_writeb((VISUAL_FIELD_VALS + 28), 0))));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 18), 0);
		}

		if (((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) ||
			((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 18)))) >= 2 && bi <= 5)) {

			ds_writeb((VISUAL_FIELD_VALS + 19), ds_writeb((VISUAL_FIELD_VALS + 25), ds_writeb((VISUAL_FIELD_VALS + 26), 0)));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 18), ds_writeb((VISUAL_FIELD_VALS + 25), 0));
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 16)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 22), 0);

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 24)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 23), 0);
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 20)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 28), 0);

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 26)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 27), 0);
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 18)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 25), 0);
	}

	if (((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 18)))) >= 2 && bi <= 5) ||
		((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 25)))) >= 2 && bi <= 5)) {

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 17)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 24), 0);
		}

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 19)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 26), 0);
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
	while (ds_readb(MAPVAL_TO_LOCTYPE + i) < val) {
		i++;
	}

	ds_writew(ENTRANCE_ANGLE, (((val & 3) + 4) - ds_readbs(DIRECTION)) & 3);

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

		bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + i)));

		if (bi == -1) {
			ds_writebs(VISUAL_FIELDS_TEX + i, -1);
		} else {
			ds_writebs(VISUAL_FIELDS_TEX + i,	bi == 2 ? ds_readbs(SEG066_0BAD_UNKN0 + i) : (
						bi == 3 ? ds_readbs(SEG066_0BAD_UNKN1 + i) : (
						bi == 4 ? ds_readbs(SEG066_0BAD_UNKN2 + i) : (
						bi == 5 ? ds_readbs(SEG066_0BAD_UNKN3 + i) : (
						bi == 6 ? ds_readbs(SEG066_0BAD_UNKN4 + i) : (
						bi == 7 ? ds_readbs(SEG066_0BAD_UNKN4 + i) : (
						bi == 8 ? ds_readbs(SEG066_0BAD_UNKN5 + i) : (
						bi == 9 ? ds_readbs(SEG066_0BAD_UNKN6 + i) : (
						bi == 10 ? ds_readbs(SEG066_0BAD_UNKN7 + i) : (
						bi == 1 ? ds_readbs(SEG066_0BAD_UNKN2 + i) : -1))))))))));
		}
	}
}

/**
 * \brief   draws water and grass textures
 */
void city_water_and_grass(void)
{
	signed short i;
	signed short nvf_no;
	signed short x;
	signed short y;
	signed char c1;
	signed char bi;
	unsigned char c2;
	Bit8u *ptr;

	for (i = 0; i < 29; i++) {

		c1 = ds_readbs(VISUAL_FIELD_DRAW_ORDER + i);
		c2 = ds_readb(VISUAL_FIELD_VALS + c1);

		if (c2 != 0) {

			bi = (signed char)get_border_index(c2);

			if (bi == 6 || bi == 7) {
				/* water or grass */

				ptr = 4 * c1 + p_datseg + VISUAL_FIELD_OFFSETS_GRASS;

				x = host_readws(ptr);
				y = host_readws(ptr + 2);

				c1 = ds_readbs(VISUAL_FIELDS_TEX + c1);

				if (c1 != -1) {

					ptr = c1 * 18 + p_datseg + (TEX_DESCR_TABLE - 18);

					if ((nvf_no = host_readws(ptr + 4)) != -1) {

						if (bi == 7) {
							nvf_no += 15;
						}

						load_city_texture(x + host_readws(ptr), y + host_readws(ptr + 2), nvf_no, 184);
					}
				}
			}
		}
	}
}

/**
 * \brief   draws building textures
 */
void city_building_textures(void)
{
	signed short nvf_no;
	signed short i;
	signed short x;
	signed short y;
	signed short l4;
	signed char c1;
	signed char bi;
	unsigned char c2;
	Bit8u *ptr;

	for (i = 0; i < 29; i++) {

		c1 = ds_readbs(VISUAL_FIELD_DRAW_ORDER + i);
		c2 = ds_readb(VISUAL_FIELD_VALS + c1);

		if (c2 != 0) {

			bi = (signed char)get_border_index(c2);

			if (bi != 7 && bi != 6) {
			    /* if not grass or water */

				ptr = p_datseg + VISUAL_FIELD_OFFSETS_STD + 4 * c1;

				if (bi == 8) {
				    /* direction sign */
					ptr = p_datseg + VISUAL_FIELD_OFFSETS_SIGN + 4 * c1;
				} else if (bi == 9 || bi == 10) {
				    /* tavern/inn or shop */
					ptr = p_datseg + VISUAL_FIELD_OFFSETS_INN + 4 * c1;
				}

				x = host_readws(ptr);
				y = host_readws(ptr + 2);

				c1 = ds_readbs(VISUAL_FIELDS_TEX + c1);

				if (c1 != -1) {

					ptr = 18 * c1 + p_datseg + (TEX_DESCR_TABLE - 18);

					l4 =	bi == 2 ? 186 : (
						bi == 3 ? 187 : (
						bi == 4 ? 188 : (
						bi == 5 ? 189 : (
						bi == 1 ? 188 : (
						bi == 9 ? 232 : (
						bi == 10 ? 233 : 185))))));

					if ((nvf_no = host_readws(ptr + 4)) != -1) {

						if (ds_readws(ENTRANCE_ANGLE) == 2 && bi >= 1 && bi <= 5) {

							if (bi == 1) {
								nvf_no -= 5;
								l4 = 185;
							} else {
								nvf_no -= 10;
							}
						}

						if (bi == 9 || bi == 10) {
							load_special_textures(bi);
						}

						load_city_texture(x + host_readws(ptr), y + host_readws(ptr + 2), nvf_no, l4);

						if (bi == 9 || bi == 10) {
							call_load_buffer();
						}
					}

					if ((nvf_no = host_readws(ptr + 10)) != -1) {

						if (bi == 1) {
							l4 = 188;
						}

						if (ds_readws(ENTRANCE_ANGLE) == 1 &&
							!(nvf_no & 0x8000) &&
							bi >= 1 && bi <= 5)
						{
							nvf_no -= 10;

						} else if (ds_readws(ENTRANCE_ANGLE) == 3 && (nvf_no & 0x8000))
						{
							nvf_no = ((unsigned short)(nvf_no & 0x7fff) - 10) | 0x8000;
						}

						load_city_texture(x + host_readws(ptr + 6), y + host_readws(ptr + 8), nvf_no, l4);
					}

					if ((nvf_no = host_readws(ptr + 0x10)) != -1) {
						load_city_texture(x + host_readws(ptr + 12), y + host_readws(ptr + 14), nvf_no, l4);
					}
				}
			}
		}
	}
}

void load_city_texture(signed short v1, signed short v2, signed short nvf_no,
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

	direction = (nvf_no & 0x8000 ? 1: 0);
	nvf_no &= 0x3fff;

	v4 -= 184;

	nvf.dst = src = Real2Host(ds_readd(RENDERBUF_PTR)) + 30000;

	/*
	 * the following line accesses memory outside of the
	 * texture array if v4 is 48 or 49!?
	 */
	nvf.src = Real2Host(ds_readd(TEX_FLOOR + v4 * 4));

	if (v4 == 48 || v4 == 49) {
		nvf.src = Real2Host(ds_readd(BUFFER7_PTR));
	}

	nvf.no = nvf_no;
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

		dst = Real2Host(ds_readd(RENDERBUF_PTR)) + v2 * 208 + v1;

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

	ds_writebs((NEW_MENU_ICONS + 0), 12);
	l4 = ds_readbs((NEW_MENU_ICONS + 1));
	ds_writebs((NEW_MENU_ICONS + 1), ds_readbs(CAN_MERGE_GROUP) == -1 ? 45 : 15);

	if (ds_readbs((NEW_MENU_ICONS + 1)) != l4) {
		ds_writew(REDRAW_MENUICONS, 1);
	}

	ds_writebs((NEW_MENU_ICONS + 2), 29);
	ds_writebs((NEW_MENU_ICONS + 3), 37);
	ds_writebs((NEW_MENU_ICONS + 4), 39);
	ds_writebs((NEW_MENU_ICONS + 5), 11);
	ds_writebs((NEW_MENU_ICONS + 6), 54);

	if (ds_readws(REQUEST_REFRESH) != 0) {

		draw_main_screen();
		GUI_print_loc_line(get_tx(0));

		ds_writew(REQUEST_REFRESH, ds_writews(REDRAW_MENUICONS, 0));
		ds_writews(CITY_REFRESH_X_TARGET, -1);
	}

	if (ds_readw(REDRAW_MENUICONS) != 0 && ds_readbs(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK) {
		draw_icons();
		ds_writews(REDRAW_MENUICONS, 0);
	}

	/* check if position or direction has changed */
	if (ds_readbs(DIRECTION) != ds_readws(CITY_REFRESH_DIRECTION) ||
		ds_readws(X_TARGET) != ds_readws(CITY_REFRESH_X_TARGET) ||
		ds_readws(Y_TARGET) != ds_readws(CITY_REFRESH_Y_TARGET))
	{
		seg066_10c8();
	}

	if (ds_readws(X_TARGET) != ds_readws(X_TARGET_BAK) ||
		ds_readws(Y_TARGET) != ds_readws(Y_TARGET_BAK))
	{
		ds_writebs(CAN_MERGE_GROUP, (signed char)can_merge_group());
		set_automap_tiles(ds_readws(X_TARGET), ds_readws(Y_TARGET));
	}

	ds_writew(X_TARGET_BAK, ds_readws(X_TARGET));
	ds_writew(Y_TARGET_BAK, ds_readws(Y_TARGET));

	handle_gui_input();

	if (ds_readw(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == 73) {

		for (i = options = 0; i < 9; i++) {
			if (ds_readbs(NEW_MENU_ICONS + i) != -1) {
				options++;
			}
		}

		i = GUI_radio(get_ttx(570), (signed char)options,
				get_ttx(535), get_ttx(536), get_ttx(537),
				get_ttx(538), get_ttx(539), get_ttx(213),
				get_ttx(306), get_ttx(569)) - 1;

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

		ds_writebs(LOCATION, LOCATION_CITYCAMP);
		ds_writeb(CITYCAMP_CITY, 1);
		i = 1;

	} else if (ds_readws(ACTION) == 136 && ds_readbs((NEW_MENU_ICONS + 7)) != -1) {

		ds_writebs(LOCATION, LOCATION_MARKET);
		i = 1;

	} else if (ds_readws(ACTION) == 75) {

		update_direction(3);

	} else if (ds_readws(ACTION) == 77) {

		update_direction(1);

	} else if (ds_readws(ACTION) == 72) {

		bi = get_border_index(ds_readb(STEPTARGET_FRONT));

		if (!bi || bi == 7 || bi == 8) {
			seg066_14dd(1);
		} else if (bi >= 1 && bi <= 5 && ds_readw(ENTRANCE_ANGLE) == 2) {
			seg066_14dd(1);
		} else {
			no_way();
		}

	} else if (ds_readws(ACTION) == 80) {

		bi = get_border_index(ds_readb(STEPTARGET_BACK));

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
		if ((ds_readws(Y_TARGET) != ds_readws(Y_TARGET_BAK) ||
			(ds_readws(X_TARGET) != ds_readws(X_TARGET_BAK))) &&

			(ds_readb(CURRENT_TOWN) == TOWNS_THORWAL || ds_readb(CURRENT_TOWN) == TOWNS_PREM ||
			ds_readb(CURRENT_TOWN) == TOWNS_PHEXCAER || ds_readb(CURRENT_TOWN) == TOWNS_OBERORKEN))
		{

			/* roll a dice */
			if (random_schick(100) <= 1 &&
				ds_readds(DAY_TIMER) > HOURS(8) &&
				ds_readds(DAY_TIMER) < HOURS(20))
			{
				city_event_switch();
			}
		}

		if (ds_readb(LOCATION_MARKET_FLAG) != 0 && ds_readb((NEW_MENU_ICONS + 7)) != 43) {

			if (((i = ds_readws((MARKET_DESCR_TABLE + 4) + 8 * ds_readws(TYPEINDEX))) == -1 ||
				ds_readbs(DAY_OF_WEEK) == i) &&
				ds_readds(DAY_TIMER) >= HOURS(6) &&
				ds_readds(DAY_TIMER) <= HOURS(16))
			{
				ds_writebs((NEW_MENU_ICONS + 7), 43);
				draw_icons();
			}

		} else if (!ds_readbs(LOCATION_MARKET_FLAG) && ds_readbs((NEW_MENU_ICONS + 7)) == 43) {

			ds_writebs((NEW_MENU_ICONS + 7), -1);
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

	if (ds_readb(FADING_STATE) == 2) {

		fade_into();
		ds_writeb(FADING_STATE, 1);

	}

	if (ds_readb(FADING_STATE) == 3) {

		set_palette(p_datseg + PALETTE_ALLBLACK2, 0x00, 0x20);
		set_palette(p_datseg + PALETTE_ALLBLACK2, 0x80, 0x20);
		set_palette(p_datseg + PALETTE_ALLBLACK2, 0xa0, 0x20);

		ds_writeb(FADING_STATE, 1);
	}

	draw_compass();

	ds_writew(PIC_COPY_X1, ds_readws(ANI_POSX));
	ds_writew(PIC_COPY_Y1, ds_readws(ANI_POSY));
	ds_writew(PIC_COPY_X2, ds_readws(ANI_POSX) + 207);
	ds_writew(PIC_COPY_Y2, ds_readws(ANI_POSY) + 134);
	ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));

	ds_writeb(SPECIAL_SCREEN, 0);

	update_mouse_cursor();
	wait_for_vsync();

	do_pic_copy(1);

	refresh_screen_size();

	if (ds_readb(FADING_STATE) != 0) {

		dst = Real2Host(ds_readd(RENDERBUF_PTR)) + 500;
		pal_ptr = Real2Host(ds_readd(RENDERBUF_PTR));

		memset(Real2Host(ds_readd(RENDERBUF_PTR)), 0, 0x120);
		memcpy(dst, p_datseg + PALETTE_FLOOR, 0x120);

		for (i = 0; i < 64; i += 2) {

			pal_fade_in(pal_ptr, dst, i, 0x60);
			pal_fade_in(pal_ptr, dst, i + 1, 0x60);

			wait_for_vsync();

			set_palette(pal_ptr, 0x00, 0x20);
			set_palette(pal_ptr + 0x60, 0x80, 0x40);
		}

		ds_writeb(FADING_STATE, 0);

	} else {

		wait_for_vsync();

		set_palette(p_datseg + PALETTE_FLOOR, 0x00, 0x20);
		set_palette(p_datseg + PALETTE_BUILDINGS, 0x80, 0x40);
	}
}

void seg066_172b(void)
{
	signed short l_si;
	signed short l_di;
	Bit8u *ptr = p_datseg + DNG_MAP;

	ds_writeb(CITY_HOUSE_COUNT, ds_writeb((CITY_HOUSE_COUNT+1), ds_writeb((CITY_HOUSE_COUNT+2), ds_writeb((CITY_HOUSE_COUNT+3), 0))));

	for (l_di = 0; ds_readb(DNG_MAP_SIZE) * 16 > l_di; l_di++) {

		l_si = get_border_index(host_readb(ptr + l_di));

		/* count number of houses of certain kind */
		if (l_si == 2) {
			inc_ds_bs_post(CITY_HOUSE_COUNT);
		} else if (l_si == 3) {
			inc_ds_bs_post((CITY_HOUSE_COUNT+1));
		} else if ((l_si == 4) || (l_si == 1)) {
			inc_ds_bs_post((CITY_HOUSE_COUNT+2));
		} else if (l_si == 5) {
			inc_ds_bs_post((CITY_HOUSE_COUNT+3));
		}
	}

	l_di = 5;
	l_si = 2000;

	/* find house with lowest count on current map */
	if (ds_readb(CITY_HOUSE_COUNT) < l_si) {
		l_si = ds_readb(CITY_HOUSE_COUNT + (l_di = 0));
	}

	if (ds_readb((CITY_HOUSE_COUNT+1)) < l_si) {
		l_si = ds_readb(CITY_HOUSE_COUNT + (l_di = 1));
	}

	if (ds_readb((CITY_HOUSE_COUNT+2)) < l_si) {
		l_si = ds_readb(CITY_HOUSE_COUNT + (l_di = 2));
	}

	if (ds_readb((CITY_HOUSE_COUNT+3)) < l_si) {
		l_si = ds_readb(CITY_HOUSE_COUNT + (l_di = 3));
	}

	/* the kind of house with lowest count is deactivated, i.e. it's texture is
	 * not loaded and replaced by another texture in seg028_0224 */
	ds_writeb(CITY_HOUSE_COUNT + l_di, 0);
}

#if !defined(__BORLANDC__)
}
#endif

