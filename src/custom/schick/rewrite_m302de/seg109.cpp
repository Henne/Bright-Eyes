/*
 *	Rewrite of DSA1 v3.02_de functions of seg109 (travel events 1 / 10)
 *	Functions rewritten: 10/30
*/

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg032.h"
#include "seg047.h"
#include "seg051.h"
#include "seg097.h"
#include "seg109.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void TRV_load_textfile(signed short travel_event)
{
	load_buffer_1(190);

	if (travel_event == -1) {
		travel_event = ds_readws(0xb133);
	}

	load_city_ltx( (travel_event == 37 || travel_event == 47 || travel_event == 100) ? 237 :
			( travel_event == 16 ? 198 :
			( travel_event < 30 ? 191 :
			( travel_event < 54 ? 192 :
			( travel_event < 70 ? 193 :
			( travel_event < 90 ? 194 :
			( travel_event < 111 ? 195 :
			( travel_event < 126 ? 196 :
			( travel_event < 143 ? 197 : 198)))))))));

	ds_writews(0xb133, travel_event);
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void TRV_event(signed short travel_event)
{
	signed short tw_bak;
	signed short bak1;
	signed short bak2;
	signed short traveling_bak;
	void (*event_handler)(void);

	TRV_load_textfile(travel_event);

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	bak1 = ds_readws(0x2ca2);
	bak2 = ds_readws(0x2ca4);
	traveling_bak = ds_readb(TRAVELING);
	ds_writews(0x2ca2, 0);
	ds_writews(0x2ca4, 0);
	ds_writeb(TRAVELING, 0);
	ds_writews(TEXTBOX_WIDTH, 9);
	ds_writeb(0x2c98, 1);
	ds_writeb(0xb132, 1);

	event_handler = (void (*)(void))ds_readd(0xaeea + 4 * travel_event);
	if (event_handler) event_handler();

	ds_writeb(0xb132, 0);
	ds_writeb(TRAVELING, traveling_bak);
	ds_writews(0x2ca2, bak1);
	ds_writews(0x2ca4, bak2);
	ds_writews(TEXTBOX_WIDTH, tw_bak);
	ds_writeb(0x2c98, 0);
	load_buffer_1(19);
	ds_writew(0xe113, 1);
}
#endif

/**
 * \brief	executes a fight and load a textfile
 * \param fight_nr	ID of the fight
 * \param travel_event	ID of the travel event
 * \return return value of the fight
 */
/* Borlandified and identical */
signed short TRV_fight_event(signed short fight_nr, signed short travel_event)
{
	signed short retval;

	retval = do_fight(fight_nr);
	TRV_load_textfile(travel_event);

	return retval;
}

/* Borlandified and identical */
void TRV_found_herb_place(signed short a0)
{
	signed short answer;
	signed short randval;
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());

	randval = random_schick(5) + 2;

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x0000),
		(char*)get_dtp(4 * randval),
		(char*)hero + 0x10,
		(char*)(a0 != 0 ? get_dtp(0x00a8) : p_datseg + 0xb13b));

	do {
		answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
				get_dtp(0x0004),
				get_dtp(0x0008));

	} while (answer == -1);

	if (answer == 1) {
		TRV_inside_herb_place();
	}
}

/* Borlandified and identical */
void TRV_inside_herb_place(void)
{
	signed short hero_pos;
	signed short hours;
	signed short bak;

	load_ani(1);
	draw_main_screen();
	init_ani(0);
	hero_pos = select_hero_ok(get_ltx(0x518));

	if (hero_pos != -1) {

		hours = GUI_input(get_ltx(0x51c), 1);

		if (hours > 0) {
			bak = ds_readbs(0x45b8);
			ds_writebs(0x45b8, 1);

			gather_herbs(get_hero(hero_pos), hours - 1, -3);

			ds_writebs(0x45b8, bak);
		}
	}

	set_var_to_zero();
	ds_writew(0x2846, 1);
}

/* Borlandified and identical */
#if defined(__BORLANDC__)
/* depends on: do_location() */
signed short TRV_found_camp_place(signed short a0)
{
	signed short answer;
	signed short randval;

	randval = random_schick(5) + 10;
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x020),
		(char*)get_dtp(4 * randval),
		(char*)(a0 == 1 ? get_dtp(0xa8) : (a0 == 2 ? get_dtp(0xb4) : p_datseg + 0xb13c)));
	do {
		answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
					get_dtp(0x24),
					get_dtp(0x28));
	} while (answer == -1);

	if (answer == 1) {

		ds_writew(0xd32d, ds_writews(0xd331, ds_writews(0xd32f, 0)));

		if (a0 == 1) {
			ds_writews(0xd331, -3);
		} else if (a0 == 2) {
			ds_writews(0xd32f, -3);
		}

		ds_writeb(0xe4c8, 1);
		ds_writeb(LOCATION, 6);

		do_location();

		ds_writeb(LOCATION, ds_writeb(0xe4c8, 0));

		TRV_load_textfile(-1);

		ds_writew(0xd32d, ds_writews(0xd331, ds_writews(0xd32f, 0)));
		ds_writew(0x2846, 2);

		return 1;
	}

	return 0;
}
#endif

/* Borlandified and identical */
void TRV_found_replenish_place(signed short a0)
{
	signed short hero_pos;
	signed short answer;

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_dtp(0x040),
		(char*)get_dtp(4 * (random_schick(5) + 11)),
		(char*)(a0 != 0 ? get_dtp(0xb4) : p_datseg + 0xb13d));
	do {
		answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
					get_dtp(0x54),
					get_dtp(0x58));
	} while (answer == -1);

	if (answer == 1) {

		load_ani(1);
		draw_main_screen();
		init_ani(0);

		hero_pos = get_hero_index(Real2Host(get_first_hero_available_in_group()));

		ds_writeb(0xe3c8 + hero_pos, ds_writeb(0xe3c1 + hero_pos, ds_writeb(0xe3cf + hero_pos, ds_writeb(0xe3d6 + hero_pos, 0))));

		replenish_stocks(-3, hero_pos);

		timewarp(HOURS(1));

		set_var_to_zero();

		ds_writew(0x2846, 1);
	}
}

#if defined(__BORLANDC__)
/* depends on: do_location() */
/* Borlandified and identical */
void TRV_found_inn(signed short city, signed short type)
{
	load_ani(12);
	draw_main_screen();
	init_ani(0);

	if (GUI_bool(get_dtp(0x5c))) {
		ds_writew(CITYINDEX, city);
		ds_writew(TYPEINDEX, type);
		ds_writeb(LOCATION, 7);

		do_location();

		ds_writeb(LOCATION, 0);

		TRV_load_textfile(-1);
	}

	set_var_to_zero();
	ds_writew(0x2846, 1);
}
#endif

/* 0x4f2 */
/**
 * enter_hut_question() - question if you want to rest in a hut
 *
 * Returns the answer 0 = no, 1 = yes
 */
signed short enter_hut_question(void)
{
	signed short answer;

	load_ani(8);

	draw_main_screen();

	init_ani(0);

	answer = GUI_bool(get_dtp(0x60));

	set_var_to_zero();

	ds_writew(0x2846, 1);

	return answer;
}

void seg109_067e(signed short mod, signed short time)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else
#endif
}

/* The hunter Varnheim <-> Daspota */
void tevent_002(void)
{
	signed short answer;

	/* load dialog head */
	load_in_head(0x0a);

	/* show dialogbox */
	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
				NULL,
				get_city(0x00),
				3,
				get_city(0x04),
				get_city(0x08),
				get_city(0x0c));

	} while (answer == -1);

	/* talk to the hunter for 15 min */
	if (answer == 1 || answer == 2) {

		if (answer == 1)
			GUI_dialog_na(0, get_city(0x10));
		else
			GUI_dialog_na(0, get_city(0x14));

		timewarp(0x546);
	}
}

#if !defined(__BORLANDC__)
}
#endif
