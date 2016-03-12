/**
 *	Rewrite of DSA1 v3.02_de functions of seg065 (special animations)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg065.cpp
 */

#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg055.h"
#include "seg097.h"
#include "seg120.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
void sub_light_timers(signed short);
#endif

void do_market(void)
{
	signed short done;
	signed short answer;
	signed short type_bak;
	signed short dir_bak;
	signed short bak1;

	done = 0;
	ds_writew(0x2846, 1);
	bak1 = ds_readbs(0x2d7c);
	dir_bak = ds_readbs(DIRECTION);

	do {

		if (ds_readw(0x2846) != 0) {
			draw_main_screen();
			set_var_to_zero();
			load_ani(16);
			init_ani(0);
			ds_writew(0x2846, 0);
		}

		answer = GUI_radio(get_ltx(0xaa0), 4,
					get_ltx(0xa90), get_ltx(0xa94),
					get_ltx(0xa98), get_ltx(0x994));

		if (answer == 4 || answer == -1) {
			done = 1;
		} else {

			/* set up parameters for this merchant */
			ds_writeb(0x6870 + 90 * 9 + 0, ds_readb((0x70a8 + 2) + 8 * ds_readws(TYPEINDEX)));
			ds_writeb(0x6870 + 90 * 9 + 2, ds_readb((0x70a8 + 6) + 8 * ds_readws(TYPEINDEX)));
			ds_writeb(0x6870 + 90 * 9 + 1, (signed char)answer);
			type_bak = ds_readws(TYPEINDEX);
			ds_writew(TYPEINDEX, 90);

			/* visit this merchant */
			do_merchant();

			/* change back to market */
			ds_writeb(LOCATION, 9);

			/* clean up */
			ds_writew(TYPEINDEX, type_bak);
			ds_writeb(0x2d7c, (signed char)bak1);
			ds_writeb(DIRECTION, (signed char)dir_bak);
			ds_writeb(0x6870 + 90 * 9 + 0, 0);
			ds_writeb(0x6870 + 90 * 9 + 2, 0);
			ds_writeb(0x6870 + 90 * 9 + 1, 0);
		}

	} while (!done);

	ds_writeb(LOCATION, 0);
	copy_palette();
}

void final_intro(void)
{
	signed short i;
	signed short handle;
	signed short width;
	signed short height;
	Bit32u len;
	Bit8u *ptr1;
	RealPt ptr2;
	struct nvf_desc nvf;

	ds_writebs(0x2845, -2);

	update_mouse_cursor();

	ds_writew(WALLCLOCK_UPDATE, 0);

	/* load FACE.NVF */
	handle = load_archive_file(ARCHIVE_FILE_FACE_NVF);
	len = read_archive_file(handle, Real2Host(ds_readd(BUFFER9_PTR)), 64000);
	bc_close(handle);

	ptr1 = Real2Host(F_PADD(F_PADD(ds_readd(BUFFER9_PTR), len), -(96 * 3)));

	do_fill_rect((RealPt)ds_readd(0xd2ff), 0, 0, 319, 199, 0);

	wait_for_vsync();

	set_palette(ptr1, 0, 0x60);

	ptr2 = (RealPt)F_PADD(ds_readd(BUFFER9_PTR), 80000);

	nvf.dst = Real2Host(ds_readd(BUFFER1_PTR));
	nvf.src = Real2Host(ds_readd(BUFFER9_PTR));
	nvf.nr = 0;
	nvf.type = 3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

	map_effect(Real2Host(ds_readd(BUFFER1_PTR)));

	nvf.dst = Real2Host(ptr2);
	nvf.src = Real2Host(ds_readd(BUFFER9_PTR));
	nvf.nr = 1;
	nvf.type = 3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

	ds_writew(0xc011, 0);
	ds_writew(0xc013, 20);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 39);
	ds_writed(0xc019, (Bit32u)ptr2);
	ds_writed(0xc00d, ds_readd(BUFFER1_PTR));

	do_pic_copy(2);

	delay_or_keypress(100);

	map_effect(Real2Host(ds_readd(BUFFER1_PTR)));

	ds_writed(0xc00d, ds_readd(0xd2ff));

	delay_or_keypress(250);

	memset(Real2Host(ds_readd(BUFFER1_PTR)), 0, 96 * 3);

	for (i = 0; i < 0x40; i++) {

		pal_fade(ptr1, Real2Host(ds_readd(BUFFER1_PTR)));
		pal_fade(ptr1 + 0x60, Real2Host(ds_readd(BUFFER1_PTR)) + 0x60);
		pal_fade(ptr1 + 0xc0, Real2Host(ds_readd(BUFFER1_PTR)) + 0xc0);

		wait_for_vsync();

		set_palette(ptr1, 0, 0x60);
	}

	bc_memset((RealPt)ds_readd(0xd2ff), 0, 64000);

	refresh_colors();
	refresh_screen_size();
}

RealPt hyg_ani_1(signed short nvf_nr, Bit8u *ptr)
{
	HugePt retval;
	struct nvf_desc nvf;

	nvf.dst = Real2Host(host_readd(ptr));
	nvf.src = Real2Host(ds_readd(BUFFER1_PTR));
	nvf.nr = nvf_nr;
	nvf.type = 3;
	nvf.width = ptr + 4;
	nvf.height = ptr + 6;

	process_nvf(&nvf);

	retval = F_PADD((RealPt)host_readd(ptr),
			host_readws(ptr + 4) * host_readws(ptr + 6));

	return (RealPt)retval;
}

void hyg_ani_2(Bit8u *ptr, signed short x, signed short y)
{
	ds_writew(0xc011, x);
	ds_writew(0xc013, y);
	ds_writew(0xc015, x + host_readws(ptr + 4) - 1);
	ds_writew(0xc017, y + host_readws(ptr + 6) - 1);

	ds_writed(0xc019, host_readd(ptr));
	ds_writed(0xc00d, ds_readd(BUFFER1_PTR));

	do_pic_copy(2);
}

void hyg_ani_3(void)
{
	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 199);
	ds_writed(0xc019, (Bit32u)F_PADD((HugePt)ds_readd(BUFFER9_PTR), 0x1fbd0));
	ds_writed(0xc00d, ds_readd(BUFFER1_PTR));

	do_pic_copy(0);
}

void hyg_ani_4(void)
{
	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 199);
	ds_writed(0xc019, ds_readd(BUFFER1_PTR));
	ds_writed(0xc00d, ds_readd(0xd2ff));

	do_pic_copy(0);
}

void show_hyggelik_ani(void)
{
	signed short i;
	signed short handle;
	Bit32s filelen;
	Bit8u *src;
	RealPt ptr1;
	RealPt ptr2;
	Bit8u array[30*8];

	ds_writew(WALLCLOCK_UPDATE, 0);
	ptr1 = (RealPt)ds_readd(BUFFER9_PTR);
	ptr2 = (RealPt)F_PADD((HugePt)ds_readd(BUFFER9_PTR), 0x1fbd0);

	handle = load_archive_file(ARCHIVE_FILE_HYGBACK_NVF);
	filelen = read_archive_file(handle, Real2Host(ds_readd(BUFFER1_PTR)), 64000);
	bc_close(handle);
	src = &(Real2Host(ds_readd(BUFFER1_PTR))[filelen - 0xc0]);

	do_fill_rect((RealPt)ds_readd(0xd2ff), 0, 0, 319, 199, 0);
	memcpy((void*)Real2Host(ds_readd(DTP2)), src, 192);
	src = Real2Host(ds_readd(DTP2));

	wait_for_vsync();
	set_palette(src, 0 , 0x40);
	host_writed(array + 0, (Bit32u)ptr2);

	hyg_ani_1(0, array);

	handle = load_archive_file(ARCHIVE_FILE_HYGGELIK_NVF);
	filelen = read_archive_file(handle, Real2Host(ds_readd(BUFFER1_PTR)), 64000);
	bc_close(handle);
	host_writed(array + 0, (Bit32u)ptr1);

	for (i = 0; i < 26; i++) {
		host_writed(array + 8 * i + 8, (Bit32u)hyg_ani_1(i, array + 8 * i));
	}

	update_mouse_cursor();

	hyg_ani_3();
	hyg_ani_2(array + 0 * 8, 145, 39);
	hyg_ani_2(array + 7 * 8, 142, 86);
	hyg_ani_2(array + 10 * 8, 82, 67);
	hyg_ani_2(array + 20 * 8, 186, 67);

	map_effect(Real2Host(ds_readd(BUFFER1_PTR)));

	for (i = 0; i < 7; i++) {
		hyg_ani_3();
		hyg_ani_2(array + 7 * 8, 142, 86);
		hyg_ani_2((array + 0 * 8) + 8 * i, 145, 39);
		hyg_ani_2(array + 10 * 8, 82, 67);
		hyg_ani_2(array + 20 * 8, 186, 67);
		hyg_ani_4();
		delay_or_keypress(3);
	}

	delay_or_keypress(6);

	for (i = 0; i < 5; i++) {
		hyg_ani_3();
		hyg_ani_2(array + 6 * 8, 145, 39);
		hyg_ani_2(array + 7 * 8, 142, 86);
		hyg_ani_2(array + 20 * 8, 186, 67);
		hyg_ani_2(array + 10 * 8 + 8 * i, ds_readb(0x71b8 + i), 67);
		hyg_ani_4();
		delay_or_keypress(3);
	}

	hyg_ani_3();
	hyg_ani_2(array + 5 * 8, 145, 39);
	hyg_ani_2(array + 7 * 8, 142, 86);
	hyg_ani_2(array + 15 * 8, ds_readb(0x71bd), 67);
	hyg_ani_2(array + 20 * 8, ds_readb(0x71c2), 67);
	hyg_ani_4();
	delay_or_keypress(3);

	hyg_ani_3();
	hyg_ani_2(array + 5 * 8, 145, 39);
	hyg_ani_2(array + 7 * 8, 142, 86);
	hyg_ani_2(array + 16 * 8, ds_readb(0x71be), 67);
	hyg_ani_2(array + 21 * 8, ds_readb(0x71c3), 67);
	hyg_ani_4();
	delay_or_keypress(3);

	for (i = 0; i < 3; i++) {
		hyg_ani_3();
		hyg_ani_2(array + 6 * 8, 145, 39);
		hyg_ani_2(array + 8 * 8, 144, ds_readb(0x71c7 + i));
		hyg_ani_2(array + 17 * 8 + 8 * i, ds_readb(0x71bf + i), 67);
		hyg_ani_2(array + 22 * 8 + 8 * i, ds_readb(0x71c4 + i), 67);
		hyg_ani_4();
		delay_or_keypress(3);
	}

	hyg_ani_3();
	hyg_ani_2(array + 6 * 8, 145, 39);
	hyg_ani_2(array + 9 * 8, 125, 104);
	hyg_ani_2(array + 19 * 8, ds_readb(0x71c1), 67);
	hyg_ani_2(array + 24 * 8, ds_readb(0x71c6), 67);
	hyg_ani_4();
	delay_or_keypress(100);

	/* clear the screen */
	do_fill_rect((RealPt)ds_readd(BUFFER1_PTR), 0, 0, 319, 199, 0);

	hyg_ani_2(array + 25 * 8, 100, 0);
	ds_writed(0xc00d, ds_readd(0xd2ff));
	map_effect(Real2Host(ds_readd(BUFFER1_PTR)));
	delay_or_keypress(500);

	memset((void*)Real2Host(ds_readd(BUFFER1_PTR)), 0, 0xc0);

	for (i = 0; i < 64; i++) {
		pal_fade(src, Real2Host(ds_readd(BUFFER1_PTR)));
		pal_fade(src + 0x60, Real2Host(ds_readd(BUFFER1_PTR)) + 0x60);
		wait_for_vsync();
		set_palette(src, 0, 0x40);
	}

	refresh_screen_size();
	bc_memset((RealPt)ds_readd(0xd2ff), 0, 64000);
	refresh_colors();
}

void show_times_up(void)
{
	signed short fi_bak;
	signed short tw_bak;
	signed short bak1;
	signed short bak2;

	fi_bak = ds_readws(TEXT_FILE_INDEX);
	load_city_ltx(ARCHIVE_FILE_CHARTEXT_LTX);
	set_audio_track(ARCHIVE_FILE_VICTORY_XMI);
	set_var_to_zero();
	draw_main_screen();
	load_ani(35);
	init_ani(0);

	delay_or_keypress(200);

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	bak1 = ds_readws(0x2ca2);
	bak2 = ds_readws(0x2ca4);

	ds_writew(TEXTBOX_WIDTH, 7);
	ds_writew(0x2ca2, 0);
	ds_writew(0x2ca4, 55);

	GUI_output(get_city(0xdc));
	GUI_output(get_city(0xe0));
	GUI_output(get_city(0xe4));

	ds_writew(0x2ca2, bak1);
	ds_writew(0x2ca4, bak2);
	ds_writew(TEXTBOX_WIDTH, tw_bak);

	/* restore text file except for CHARTEXT.LTX, TAVERN.TLK and except for dialogs */
	if (fi_bak != -1 && fi_bak != ARCHIVE_FILE_CHARTEXT_LTX
	    && fi_bak != ARCHIVE_FILE_TAVERN_TLK
	    && !(fi_bak >= 156 && fi_bak <= 176)) {
		load_city_ltx(fi_bak);
	}
}

void show_outro(void)
{
	signed short j;
	signed short handle;
	signed short width;
	signed short height;
	unsigned short len;
	Bit8u *pal_ptr;
	Bit8u *hero;
	signed short i;
	struct nvf_desc nvf;

	ds_writew(TEXTBOX_WIDTH, 7);
	ds_writew(0x2ca2, 0);
	ds_writew(0x2ca4, 60);

	load_city_ltx(ARCHIVE_FILE_CHARTEXT_LTX);
	set_audio_track(ARCHIVE_FILE_VICTORY_XMI);
	set_var_to_zero();

	/* load OUTRO1.NVF */
	handle = load_archive_file(ARCHIVE_FILE_OUTRO1_NVF);
	len = read_archive_file(handle, Real2Host(ds_readd(BUFFER9_PTR)), 64000);
	bc_close(handle);

	pal_ptr = Real2Host(F_PADD(F_PADD((HugePt)ds_readd(BUFFER9_PTR), len), - 0xc0));
	do_fill_rect((RealPt)ds_readd(0xd2ff), 0, 0, 319, 199, 0);
	wait_for_vsync();
	set_palette(pal_ptr, 0, 0x40);

	nvf.dst = Real2Host(ds_readd(BUFFER1_PTR));
	nvf.src = Real2Host(ds_readd(BUFFER9_PTR));
	nvf.nr = 0;
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);
#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	ds_writew(0xc011, (320 - width) / 2);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, (320 - width) / 2 + width - 1);
	ds_writew(0xc017, height - 1);
	ds_writed(0xc019, ds_readd(BUFFER1_PTR));
	do_pic_copy(0);

	delay_or_keypress(200);

	GUI_output(get_city(0xe8));

	/* load OUTRO2.NVF */
	handle = load_archive_file(ARCHIVE_FILE_OUTRO2_NVF);
	len = read_archive_file(handle, Real2Host(ds_readd(BUFFER9_PTR)), 64000);
	bc_close(handle);

	pal_ptr = Real2Host(F_PADD(F_PADD((HugePt)ds_readd(BUFFER9_PTR), len), - 0xc0));
	do_fill_rect((RealPt)ds_readd(0xd2ff), 0, 0, 319, 199, 0);
	wait_for_vsync();
	set_palette(pal_ptr, 0, 0x40);

	nvf.dst = Real2Host(ds_readd(BUFFER1_PTR));
	nvf.src = Real2Host(ds_readd(BUFFER9_PTR));
	nvf.nr = 0;
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);
#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	ds_writew(0xc011, (320 - width) / 2);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, (320 - width) / 2 + width - 1);
	ds_writew(0xc017, height - 1);
	ds_writed(0xc019, ds_readd(BUFFER1_PTR));
	do_pic_copy(0);

	delay_or_keypress(200);

	GUI_output(get_city(0xec));

	/* load OUTRO3.NVF */
	handle = load_archive_file(ARCHIVE_FILE_OUTRO3_NVF);
	len = read_archive_file(handle, Real2Host(ds_readd(BUFFER9_PTR)), 64000);
	bc_close(handle);

	pal_ptr = Real2Host(F_PADD(F_PADD((HugePt)ds_readd(BUFFER9_PTR), len), - 0xc0));
	do_fill_rect((RealPt)ds_readd(0xd2ff), 0, 0, 319, 199, 0);
	wait_for_vsync();
	set_palette(pal_ptr, 0, 0x40);

	nvf.dst = Real2Host(ds_readd(BUFFER1_PTR));
	nvf.src = Real2Host(ds_readd(BUFFER9_PTR));
	nvf.nr = 0;
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);
#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	ds_writew(0xc011, (320 - width) / 2);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, (320 - width) / 2 + width - 1);
	ds_writew(0xc017, height - 1);
	ds_writed(0xc019, ds_readd(BUFFER1_PTR));
	do_pic_copy(0);

	delay_or_keypress(200);

	GUI_output(get_city(0xf0));
	GUI_output(get_city(0xf4));
	GUI_output(get_city(0xf8));
	GUI_output(get_city(0xfc));
	GUI_output(get_city(0x100));
	GUI_output(get_city(0x104));

	/* reset the timers */
	sub_ingame_timers(DAYS(30));
	sub_mod_timers(DAYS(30));
	sub_light_timers(100);

	/* give the heros the reward and restore them */
	hero = get_hero(0);
	for (i = 0; i < 6; i++, hero += SIZEOF_HERO) {

		if (host_readbs(hero + HERO_TYPE)) {

			/* get 50D */
			add_party_money(5000);

			and_ptr_bs(hero + HERO_STATUS1, 0xfd);
			and_ptr_bs(hero + HERO_STATUS1, 0xfb);
			and_ptr_bs(hero + HERO_STATUS1, 0xf7);
			and_ptr_bs(hero + HERO_STATUS1, 0xdf);

			/* reset every disease */
			for (j = 0; j < 8; j++) {

				host_writeb(hero + (0xae + 0) + 5 * j, 0);
				host_writeb(hero + (0xae + 1) + 5 * j, 0);
				host_writeb(hero + (0xae + 2) + 5 * j, 0);
				host_writeb(hero + (0xae + 3) + 5 * j, 0);
				host_writeb(hero + (0xae + 4) + 5 * j, 0);
			}

			/* reset every poison */
			for (j = 0; j < 10; j++) {

				host_writeb(hero + (0xd6 + 0) + 5 * j, 0);
				host_writeb(hero + (0xd6 + 1) + 5 * j, 0);
				host_writeb(hero + (0xd6 + 2) + 5 * j, 0);
				host_writeb(hero + (0xd6 + 3) + 5 * j, 0);
				host_writeb(hero + (0xd6 + 4) + 5 * j, 0);
			}

			/* set LE to the max */
			add_hero_le(hero, host_readws(hero + HERO_LE_ORIG));
			/* set AE to the max */
			add_hero_ae(hero, host_readws(hero + HERO_AE_ORIG));

			/* ??? */
			host_writeb(hero + HERO_LE_MOD, 0);
			host_writeb(hero + HERO_HUNGER, 0);
			host_writeb(hero + HERO_THIRST, 0);
			host_writeb(hero + HERO_TEMPLE_ID, 0);
			host_writeb(hero + HERO_RECIPE_ID, 0);
			host_writeb(hero + HERO_RECIPE_TIMER, 0);
			host_writeb(hero + HERO_HOSTEL_ID, 0);
			host_writeb(hero + HERO_JAIL, 0);
			host_writeb(hero + HERO_DRUNK, 0);
			host_writeb(hero + HERO_RUHE_KOERPER, 0);

			for (j = 0; j <= 13; j++) {

				host_writeb(hero + HERO_MU + 3 * j, host_readbs(hero + HERO_MU_ORIG + 3 * j));
				host_writeb(hero + HERO_MU_MOD + 3 * j, 0);
			}

			host_writed(hero + HERO_HEAL_TIMER, 0);
			host_writed(hero + HERO_STAFFSPELL_TIMER, 0);

			host_writeb(hero + HERO_GROUP_POS, i + 1);
		}
	}

	/* share all money */
	set_party_money(get_party_money());

	/* mark the game as done */
	ds_writeb(0x2d34, 99);

	ds_writew(0x2ca2, 0);
	ds_writew(0x2ca4, 0);

	/* save the game */
	save_game_state();
}

#if !defined(__BORLANDC__)
}
#endif
