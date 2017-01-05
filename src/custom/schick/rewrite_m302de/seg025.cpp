/**
 *	Rewrite of DSA1 v3.02_de functions of seg025 (locations)
 *	Functions rewritten: 15/15 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg025.cpp
 */

#include <string.h>
#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg024.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg030.h"
#include "seg049.h"
#include "seg061.h"
#include "seg075.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

#include "seg051.h"
#include "seg052.h"
#include "seg053.h"
#include "seg054.h"
#include "seg055.h"
#include "seg058.h"
#include "seg059.h"
#include "seg063.h"
#include "seg065.h"
#include "seg066.h"
#include "seg122.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if !defined(__BORLANDC__)


static void (*locationhandler[])(void) = {
	NULL,
	do_location1,
	do_temple,
	do_tavern,
	do_healer,
	do_merchant,
	do_wildcamp,
	do_inn,
	do_smith,
	do_market,
	show_citizen,
	do_harbour,
	enter_map,
	do_informer,
	show_entrance,
	NULL,
	do_house,
	do_special_buildings,
	do_citycamp,
};
#endif

void show_entrance(void)
{
	draw_main_screen();
	set_var_to_zero();
	load_ani(34);
	init_ani(1);

	if (GUI_bool(get_ltx(0xbe0))) {

		init_ani_busy_loop(2);
		DNG_enter_dungeon(ds_readws(TYPEINDEX));
	} else {

		turnaround();
	}
}

/**
 * \brief	the screen when entering a house in the city
 */
void show_citizen(void)
{
	ds_writew(REQUEST_REFRESH, 1);

	do {
		handle_gui_input();

		if (ds_readw(REQUEST_REFRESH) != 0) {

			draw_main_screen();
			set_var_to_zero();
			load_ani(20);
			init_ani(ds_writew(REQUEST_REFRESH, 0));

			strcpy((char*)Real2Host((RealPt)ds_readd(TEXT_OUTPUT_BUF)),
				(char*)get_dtp(ds_readw(CITYINDEX) * 4));

			if (ds_readbs(YEAR) == 15 && ds_readbs(MONTH) == 1 && random_schick(100) <= 20) {

				if (!show_storytext()) {
					GUI_print_loc_line(Real2Host((RealPt)ds_readd(TEXT_OUTPUT_BUF)));
				} else {
					ds_writew(ACTION, 1);
				}
			} else {
				GUI_print_loc_line(Real2Host((RealPt)ds_readd(TEXT_OUTPUT_BUF)));
#ifdef M302de_SPEEDFIX
				delay_or_keypress(200);
#endif
			}
		}

	} while ((ds_readw(ACTION) == 0) && (ds_readw(MOUSE1_EVENT2) == 0));

	ds_writew(MOUSE1_EVENT2, 0);
	set_var_to_zero();
	copy_palette();
	turnaround();
}

/**
 * \brief	break into a house
 */
void do_house(void)
{
	signed short i;
	signed short l_di;
	Bit8u *hero;

	/* prepare the question */
	strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(4 * ds_readws(CITYINDEX)));

	strcat((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x9bc));

	ds_writew(MENU_DEFAULT_SELECT, 1);

	if (GUI_bool(Real2Host(ds_readd(DTP2)))) {

		/* break into the house */

		set_var_to_zero();
		load_ani(10);
		init_ani(0);

		/* print a randomized text */
		GUI_output(get_ltx(4 * (random_schick(8) + 623)));

		hero = get_hero(0);

		for (i = 0; i < 6; i++, hero += SIZEOF_HERO) {

			if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
				(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
				!hero_dead(hero) &&
				(test_skill(hero, TA_VERSTECKEN, -2) <= 0))
			{
				/* every hero must pass a sneak -2 test */

				i = ds_readbs(CURRENT_TOWN);

				if ((i == 1) || (i == 39) || (i == 18) || (i == 17)) {

					/* sneak test failed in a town with guards */

					GUI_output(get_ltx(0x9e0));

					l_di = 0;

					for (i = 0; i < 6; i++) {

						hero = get_hero(i);

						if (check_hero(hero) && !host_readbs(hero + HERO_JAIL)) {
							l_di = 1;
						}
					}

					if ((ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) < ds_readbs(TOTAL_HERO_COUNTER)) && l_di)
					{
						i = 0;

						while (host_readbs(get_hero(i) + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
						{
							/* imprison hero */
							host_writeb(get_hero(i) + HERO_JAIL, 1);
							i++;
						}

						GRP_switch_to_next(1);

					} else {

						if (ds_readds(DAY_TIMER) < HOURS(6)) {
							/* before 6:00 turn clock to 0:00 */
							timewarp_until(0);
						}

						timewarp_until(HOURS(6));

						GUI_output(get_ltx(0x9e4));
					}
				}
				break;
			}
		}

		set_var_to_zero();

		turnaround();

	} else {
		ds_writeb(LOCATION, ds_readb(LOCATION_BAK));
		ds_writew(X_TARGET, ds_readw(X_TARGET_BAK));
		ds_writew(Y_TARGET, ds_readw(Y_TARGET_BAK));
	}

}

void do_informer(void)
{
	signed short nr = ds_readws(TYPEINDEX) - 1;

	if (!nr) do_talk(6, 0);
	else if (nr == 1) do_talk(6, 1);
	else if (nr == 2) do_talk(7, 0);
	else if (nr == 3) do_talk(7, 1);
	else if (nr == 4) do_talk(7, 2);
	else if (nr == 5) do_talk(8, 0);
	else if (nr == 6) do_talk(8, 1);
	else if (nr == 7) do_talk(10, 1);
	else if (nr == 8) do_talk(9, 0);
	else if (nr == 9) do_talk(10, 0);
	else if (nr == 10) do_talk(12, 0);
	else if (nr == 11) do_talk(11, 0);
	else if (nr == 12) do_talk(11, 2);
	else if (nr == 13) do_talk(8, 2);
	else if (nr == 14) do_talk(9, 1);

	turnaround();
}

void enter_map(void)
{
	ds_writew(0x4334, ds_readw(TYPEINDEX));

	ds_writew(TYPEINDEX, ds_readbs(CURRENT_TOWN));

	ds_writeb(LOCATION, ds_writeb(CURRENT_TOWN, 0));
	ds_writeb(SHOW_TRAVEL_MAP, 1);
}

void show_treasure_map(void)
{
	signed short l_si;
	signed short tw_bak;
	signed short count;	/* #collected treasure map parts */
	signed short width;
	signed short height;
	signed short pp20_index_bak;
	Bit32s length;
	struct nvf_desc nvf;

	/* count the collected treasure map parts */
	for (l_si = count = 0; l_si < 9; l_si++) {
		if (ds_readbs(TREASURE_MAPS + l_si) != 0) {
			count++;
		}
	}

	if (count == 0) {
		/* no treasure map parts found */
		GUI_output(get_ltx(0x984));
	} else {
		ds_writeb(0x45b8, 1);
		pp20_index_bak = ds_readbs(PP20_INDEX);
		ds_writeb(PP20_INDEX, (ARCHIVE_FILE_DNGS + 13));
		set_var_to_zero();

		/* load SKARTE.NVF */
		l_si = load_archive_file(ARCHIVE_FILE_SKARTE_NVF);

		read_archive_file(l_si, Real2Host(ds_readd(BUFFER9_PTR)), 30000);

		length = get_readlength2(l_si);

		bc_close(l_si);

		/* clear the screen */
		wait_for_vsync();

		set_palette(p_datseg + 0x26c3, 0, 0x20);

		do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 0, 0, 319, 199, 0);

		update_mouse_cursor();

		for (l_si = 0; l_si < 10; l_si++) {

			if (ds_readbs(TREASURE_MAPS + l_si) != 0 &&
				(l_si != 9 || (l_si == 9 && !ds_readbs(TREASURE_MAPS + 6))))
			{
				/* decompress picture */
				nvf.dst = Real2Host(F_PADD((RealPt)ds_readd(BUFFER9_PTR), 30000));
				nvf.src = Real2Host(ds_readd(BUFFER9_PTR));
				nvf.nr = l_si;
				nvf.type = 0;
				nvf.width = (Bit8u*)&width;
				nvf.height = (Bit8u*)&height;

				process_nvf(&nvf);

				#if !defined(__BORLANDC__)
				/* BE-fix */
				width = host_readws((Bit8u*)&width);
				height = host_readws((Bit8u*)&height);
				#endif

				/* copy to screen */
				ds_writew(PIC_COPY_X1, ds_readws(TMAP_X + 2 * l_si));
				ds_writew(PIC_COPY_Y1, ds_readws(TMAP_Y + 2 * l_si));
				ds_writew(PIC_COPY_X2, ds_readws(TMAP_X + 2 * l_si) + width - 1);
				ds_writew(PIC_COPY_Y2, ds_readws(TMAP_Y + 2 * l_si) + height - 1);
				ds_writed(PIC_COPY_SRC, (Bit32u)F_PADD((RealPt)ds_readd(BUFFER9_PTR), 30000));
				ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));
				do_pic_copy(0);
			}
		}

		wait_for_vsync();

		set_palette(Real2Host(F_PADD(F_PADD((RealPt)ds_readd(BUFFER9_PTR), length), -0x60)), 0, 0x20);

		refresh_screen_size();

		if (ds_readb(TMAP_DOUBLE1) != 0) {
			/* unicorn brought a piece you already have */
			tw_bak = ds_readws(TEXTBOX_WIDTH);
			ds_writew(TEXTBOX_WIDTH, 3);

			GUI_output(get_ltx(0xca0));

			ds_writew(TEXTBOX_WIDTH, tw_bak);
			ds_writeb(TMAP_DOUBLE1, 0);
		}

		if (ds_readb(TMAP_DOUBLE2) != 0) {
			/* you got a piece you already have from the unicorn */
			tw_bak = ds_readws(TEXTBOX_WIDTH);
			ds_writew(TEXTBOX_WIDTH, 3);

			GUI_output(get_ltx(0xca4));

			ds_writew(TEXTBOX_WIDTH, tw_bak);
			ds_writeb(TMAP_DOUBLE2, 0);
		}

		if (count >= 7 && !ds_readb(FIND_HYGGELIK)) {
			/* the way can now be found */

			tw_bak = ds_readws(TEXTBOX_WIDTH);
			ds_writew(TEXTBOX_WIDTH, 3);

			/* */
			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)get_ltx(0xb5c),
				(char*)get_hero(get_random_hero()) + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			ds_writew(TEXTBOX_WIDTH, tw_bak);
			ds_writeb(FIND_HYGGELIK, 1);
		}

		delay_or_keypress(1000);

		if (ds_readb(0x4c3a) != 0) {
			/* copy to screen */
			ds_writew(PIC_COPY_X1, 0);
			ds_writew(PIC_COPY_Y1, 0);
			ds_writew(PIC_COPY_X2, 319);
			ds_writew(PIC_COPY_Y2, 199);
			ds_writed(PIC_COPY_SRC, ds_readd(BUFFER1_PTR));
			ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

			update_mouse_cursor();
			wait_for_vsync();

			set_palette(Real2Host(ds_readd(BUFFER1_PTR)) + 64000 + 2, 0, 0x20);

			do_pic_copy(0);

			refresh_screen_size();

			ds_writeb(0x4c3a, 0);
			ds_writeb(0x45b8, 0);
			ds_writeb(PP20_INDEX, pp20_index_bak);
		} else {
			ds_writew(CURRENT_ANI, -1);
			ds_writew(REQUEST_REFRESH, 1);
			ds_writew(AREA_PREPARED, -1);
			ds_writeb(0x45b8, 0);
			draw_main_screen();
		}
	}
}

signed short game_options(void)
{
	signed short done;
	signed short answer;
	signed short fg_bak;
	signed short bg_bak;
	signed short bak1;
	signed short bak2;
	signed short tw_bak;
	signed short game_state;
	signed short new_delay;

	done = 0;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);
	ds_writeb(0x45b8, 1);
	ds_writew(WALLCLOCK_UPDATE, 0);
	ds_writew(AREA_PREPARED, -1);
	ds_writed(CURRENT_CURSOR, (Bit32u)RealMake(datseg, DEFAULT_MOUSE_CURSOR));

	load_pp20(ARCHIVE_FILE_BUCH_DAT);
	ds_writeb(PP20_INDEX, ARCHIVE_FILE_BUCH_DAT);

	get_textcolor(&fg_bak, &bg_bak);

	ds_writed(TMP_FRAMEBUF_PTR, ds_readd(BUFFER9_PTR));

	bak1 = ds_readws(0xd2d5);
	bak2 = ds_readws(0xd2d9);
	ds_writew(0xd2d5, 200);
	ds_writew(0xd2d9, 70);

	set_textcolor(4, 0);

	memset(Real2Host(ds_readd(BUFFER9_PTR)), 0, 20000);

	prepare_date_str();

	GUI_print_header(Real2Host(ds_readd(DTP2)));

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 319);
	ds_writew(PIC_COPY_Y2, 61);
	ds_writed(PIC_COPY_SRC, ds_readd(BUFFER9_PTR));
	ds_writed(PIC_COPY_DST, (Bit32u)((RealPt)ds_readd(BUFFER1_PTR) + 9600));
	do_pic_copy(2);

	memset(Real2Host(ds_readd(BUFFER9_PTR)), 0, 28000);

	if (ds_readbs(CURRENT_TOWN) != 0) {
		/* if the party is in a town */
		load_tx(ARCHIVE_FILE_MAPTEXT_LTX);

		GUI_print_header(get_dtp(4 * (ds_readbs(CURRENT_TOWN) - 1)));

		load_tx(ds_readbs(CURRENT_TOWN) + (ARCHIVE_FILE_CITY_DAT-1));

		ds_writew(PIC_COPY_X1, 0);
		ds_writew(PIC_COPY_Y1, 0);
		ds_writew(PIC_COPY_X2, 319);
		ds_writew(PIC_COPY_Y2, 86);
		ds_writed(PIC_COPY_SRC, ds_readd(BUFFER9_PTR));
		ds_writed(PIC_COPY_DST, (Bit32u)((RealPt)ds_readd(BUFFER1_PTR) + 22400));
		do_pic_copy(2);
	}

	/* draw the icons */
	draw_icon(3, 5, 30);
	draw_icon(4, 5, 60);
	draw_icon(2, 5, 90);

	draw_icon(44, 70, 170);
	draw_icon(52, 110, 170);
	draw_icon(51, 150, 170);
	draw_icon(53, 190, 170);
	draw_icon(5, 236, 170);

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 319);
	ds_writew(PIC_COPY_Y2, 199);
	ds_writed(PIC_COPY_SRC, ds_readd(BUFFER1_PTR));
	ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

	update_mouse_cursor();
	wait_for_vsync();

	set_palette(Real2Host(ds_readd(BUFFER1_PTR)) + 64002, 0, 32);

	do_pic_copy(0);
	refresh_screen_size();

	set_textcolor(fg_bak, bg_bak);

	ds_writed(PIC_COPY_DST, ds_writed(TMP_FRAMEBUF_PTR, ds_readd(FRAMEBUF_PTR)));

	ds_writew(0xd2d9, bak2);
	ds_writew(0xd2d5, bak1);
	ds_writed(0xbff9, ds_readd(BUFFER9_PTR));

	do {
		ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)RealMake(datseg, ACTION_TABLE_OPTIONS));
		handle_input();
		ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)0);

		if (ds_readw(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == 73) {

			/* use the radio menu */
			answer = GUI_radio(get_ltx(0x938), 9,
						get_ltx(0x390),
						get_ltx(0x394),
						get_ltx(0x494),
						get_ltx(0x980),
						get_ltx(0xcf8),
						get_ltx(0xcf0),
						get_ltx(0xcfc),
						get_ltx(0x930),
						get_ltx(0x934)) - 1;

			if (answer != -2) {
				ds_writew(ACTION, answer + 129);
			}
		}

		if (ds_readws(ACTION) == 129) {

			do {
				game_state = load_game_state();
			} while (game_state == -1);

			if (game_state != 0) {
				done = 1;
			}

		} else if (ds_readws(ACTION) == 130) {

			if (save_game_state()) {
				done = 1;
			}

		} else if (ds_readws(ACTION) == 131) {

			ds_writeb(0x4c3a, 1);
			char_erase();
			ds_writeb(0x4c3a, 0);

		} else if (ds_readws(ACTION) == 132) {

			ds_writeb(0x4c3a, 1);
			show_treasure_map();
			ds_writeb(0x45b8, 1);

		} else if (ds_readws(ACTION) == 133) {

			diary_show();
			done = 1;
		} else if (ds_readws(ACTION) == 134) {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0xcec),
				ds_readws(DELAY_FACTOR));

			new_delay = GUI_input(Real2Host(ds_readd(DTP2)), 2);

			if (new_delay != -1) {
				ds_writew(DELAY_FACTOR, new_delay);
			}

		} else if (ds_readws(ACTION) == 135) {

			sound_menu();

		} else if (ds_readws(ACTION) == 136) {

			if (GUI_bool(get_ltx(0x4ac))) {

				done = -1;
				ds_writew(GAME_STATE, GAME_STATE_QUIT);
			}

		} else if (ds_readws(ACTION) == 137) {
			done = 1;
		}

	} while (!done);

	ds_writed(0xbff9, ds_readd(BUFFER1_PTR));

	ds_writews(FIG_FIGURE1, ds_writews(FIG_FIGURE2, ds_writews(CURRENT_ANI, ds_writebs(PP20_INDEX, (ARCHIVE_FILE_DNGS + 13)))));
	ds_writew(REQUEST_REFRESH, 1);
	ds_writeb(0x45b8, 0);

	if (ds_readbs(CURRENT_TOWN) != 0) {
		ds_writeb(0x4475, 3);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);

	return done == -1 ? 1 : 0;
}

void draw_icon(signed short id, signed short x, signed short y)
{
	signed short handle;
	RealPt ptr_bak;

	handle = load_archive_file(ARCHIVE_FILE_ICONS);

	seek_archive_file(handle, id * 576L, 0);

	read_archive_file(handle, Real2Host(ds_readd(ICON)), 576);

	bc_close(handle);

	ptr_bak = (RealPt)ds_readd(PIC_COPY_DST);

	ds_writed(PIC_COPY_SRC, ds_readd(ICON));
	ds_writew(PIC_COPY_X1, x);
	ds_writew(PIC_COPY_Y1, y);
	ds_writew(PIC_COPY_X2, x + 23);
	ds_writew(PIC_COPY_Y2, y + 23);
	ds_writed(PIC_COPY_DST, ds_readd(BUFFER1_PTR));
	do_pic_copy(2);

	ds_writed(PIC_COPY_DST, (Bit32u)ptr_bak);
}

/* 0xd54 */
/**
 * \brief	show storytexts
 *
 * \returns	1 if dialog was shown / 0 if had already been shown
 * These were introduced in V3.00 (de and en) to find better into the story.
 */
/* static */
signed short show_storytext(void)
{
	Bit8u *ptr;
	signed short person;
	signed short icon;

	load_tx(ARCHIVE_FILE_STORY_LTX);

	person = random_schick(17) - 1;

	ptr = get_dtp(4 * person);

	switch (person) {

		case 0:	icon = 0x3f; break;
		case 1: icon = 0x44; break;
		case 2: icon = 0x44; break;
		case 3: icon = 0x45; break;
		case 4: icon = 0x43; break;
		case 5: icon = 0x46; break;
		case 6: icon = 0x41; break;
		case 7: icon = 0x42; break;
		case 8: icon = 0x40; break;
		case 9: icon = 0x46; break;
		case 10: icon = 0x40; break;
		case 11: icon = 0x45; break;
		case 12: icon = 0x43; break;
		case 13: icon = 0x40; break;
		case 14: icon = 0x43; break;
		case 15: icon = 0x41; break;
		case 16: icon = 0x44; break;

	}

	if (!ds_readbs(KNOWN_PERSONS + person)) {
		GUI_dialog_na(icon, ptr);
		ds_writeb(KNOWN_PERSONS + person, 1);
		return 1;
	} else {
		return 0;
	}
}

void do_location(void)
{
	signed short bak1;
	signed short bak2;
	signed short tw_bak;
	signed short tm_bak;
	void (*func)(void);

	tm_bak = ds_readb(SHOW_TRAVEL_MAP);
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	bak1 = ds_readws(BASEPOS_X);
	bak2 = ds_readws(BASEPOS_Y);

	ds_writew(BASEPOS_X, 0);
	ds_writew(BASEPOS_Y, 0);
	ds_writeb(SHOW_TRAVEL_MAP, 0);
	ds_writew(TEXTBOX_WIDTH, 3);

#if !defined(__BORLANDC__)
	func = locationhandler[ds_readbs(LOCATION)];
#else
	func = (void (*)(void))ds_readd(LOCATION_HANDLERS + 4 * ds_readbs(LOCATION));
#endif

	ds_writed(CURRENT_CURSOR, (Bit32u)RealMake(datseg, DEFAULT_MOUSE_CURSOR));

	if (func) {
		func();
	}

	ds_writew(BASEPOS_X, bak1);
	ds_writew(BASEPOS_Y, bak2);
	ds_writew(TEXTBOX_WIDTH, tw_bak);

	if (!ds_readb(SHOW_TRAVEL_MAP)) {
		ds_writeb(SHOW_TRAVEL_MAP, tm_bak);
	}

	ds_writebs(0x2ca8, -1);
}

/**
 * \brief	turn around in a pseudo 3d-view
 */
void turnaround(void)
{
	set_var_to_zero();

	/* reset location */
	ds_writeb(LOCATION, ds_readb(LOCATION_BAK));

	/* set target  coordinates*/
	ds_writew(X_TARGET, ds_readw(X_TARGET_BAK));
	ds_writew(Y_TARGET, ds_readw(Y_TARGET_BAK));

	/* recalc direction */
	ds_writeb(DIRECTION, (ds_readbs(DIRECTION) + 2) % 4);

	set_to_ff();

	ds_writew(REQUEST_REFRESH, ds_writebs(0x45b8, 1));
}

void leave_dungeon(void)
{
	signed short i;
	Bit8u *ptr;

	DNG_lights();
	ptr = Real2Host(ds_readd(TEXT_OUTPUT_BUF));

	memset(Real2Host(ds_readd(BUFFER1_PTR)), 0, 0xc0);

	for (i = 0; i < 64; i++) {

		pal_fade(ptr, Real2Host(ds_readd(BUFFER1_PTR)));
		pal_fade(ptr + 0x60, Real2Host(ds_readd(BUFFER1_PTR)) + 0x60);
		wait_for_vsync();
		set_palette(ptr, 0x80, 0x40);
	}

	ds_writeb(LOCATION, ds_writeb(LOCATION_BAK, 0));
	ds_writeb(CURRENT_TOWN, ds_readb(CURRENT_TOWN_BAK));
	ds_writeb(DUNGEON_INDEX_BAK, ds_readb(DUNGEON_INDEX));
	ds_writeb(DUNGEON_INDEX, ds_writeb(DUNGEON_LEVEL, ds_writeb(DUNGEON_LIGHT, 0)));
	ds_writebs(CITY_AREA_LOADED, -1);
	ds_writeb(0x4475, ds_writew(REQUEST_REFRESH, 1));

	do_fill_rect((RealPt)ds_readd(BUFFER1_PTR), 0, 0, 319, 199, 0);

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 240);
	ds_writew(PIC_COPY_Y2, 136);
	ds_writed(PIC_COPY_SRC, ds_readd(BUFFER1_PTR));

	update_mouse_cursor();

	do_pic_copy(1);
	refresh_screen_size();
	wait_for_vsync();
	set_palette(Real2Host(ds_readd(BUFFER1_PTR)), 0 , 0x20);

	/* disable the deathtrap */
	ds_writew(DEATHTRAP, 0);
}

/**
 * \brief	party gets 1W6 LE damage
 */
void tumult(void)
{
	signed short textbox_width_bak;

	textbox_width_bak = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	/* print message */
	GUI_output(get_ltx(0xbf0));

	/* each hero in the group looses 1W6 LE */
	sub_group_le(random_schick(6));


	/* the guards or a mob */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_ltx(0xbf4),
		((ds_readb(CURRENT_TOWN) == 39 ||	/* PREM */
			ds_readb(CURRENT_TOWN) == 18 ||	/* PHEXCAER */
			ds_readb(CURRENT_TOWN) == 1 ||	/* THORWAL */
			ds_readb(CURRENT_TOWN) == 17)	/* OBERORKEN */
				? (char*)get_ltx(0xbf8) : (char*)get_ltx(0xbfc)));

	GUI_output(Real2Host(ds_readd(DTP2)));

	ds_writew(TEXTBOX_WIDTH, textbox_width_bak);
}

/**
 * \brief	fade when leaving a location
 */
void fade_into(void)
{
	Bit8u *ptr;
	signed short i;

	ptr = Real2Host(ds_readd(BUFFER1_PTR)) + 0xfa00;

	memset(Real2Host(ds_readd(BUFFER1_PTR)), 0, 0xc0);

	wait_for_vsync();

	set_palette(Real2Host(ds_readd(BUFFER1_PTR)), 0x80, 0x40);

	for (i = 0; i < 0x20; i++) {

		pal_fade(ptr, Real2Host(ds_readd(BUFFER1_PTR)));

		pal_fade(ptr, Real2Host(ds_readd(BUFFER1_PTR)));

		wait_for_vsync();

		set_palette(ptr, 0, 0x20);
	}


}

void copy_palette(void)
{
	memcpy(Real2Host(ds_readd(BUFFER1_PTR)) + 0xfa00, Real2Host(ds_readd(0xce3b)), 0x60);
	ds_writeb(0x4475, 2);
}

#if !defined(__BORLANDC__)
}
#endif
