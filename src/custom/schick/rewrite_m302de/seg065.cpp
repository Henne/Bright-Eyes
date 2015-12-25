/**
 *	Rewrite of DSA1 v3.02_de functions of seg065 (special animations)
 *	Functions rewritten: 7/9
 */

#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg025.h"
#include "seg027.h"
#include "seg029.h"
#include "seg055.h"
#include "seg097.h"
#include "seg120.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
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
			ds_writeb(0x6870 + 90 * 9 + 0, ds_readb(0x70aa + 8 * ds_readws(TYPEINDEX)));
			ds_writeb(0x6870 + 90 * 9 + 2, ds_readb(0x70ae + 8 * ds_readws(TYPEINDEX)));
			ds_writeb(0x6870 + 90 * 9 + 1, answer);
			type_bak = ds_readws(TYPEINDEX);
			ds_writew(TYPEINDEX, 90);

			/* visit this merchant */
			do_merchant();

			/* change back to market */
			ds_writeb(LOCATION, 9);

			/* clean up */
			ds_writew(TYPEINDEX, type_bak);
			ds_writeb(0x2d7c, bak1);
			ds_writeb(DIRECTION, dir_bak);
			ds_writeb(0x6870 + 90 * 9 + 0, 0);
			ds_writeb(0x6870 + 90 * 9 + 2, 0);
			ds_writeb(0x6870 + 90 * 9 + 1, 0);
		}

	} while (!done);

	ds_writeb(LOCATION, 0);
	copy_palette();
}

/* Borlandified and identical */
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

	ds_writew(0xe113, 0);

	/* load FACE.NVF */
	handle = load_archive_file(134);
	len = read_archive_file(handle, Real2Host(ds_readd(0xc3db)), 64000);
	bc_close(handle);

	ptr1 = Real2Host(F_PADD(F_PADD(ds_readd(0xc3db), len), -(96 * 3)));

	do_fill_rect((RealPt)ds_readd(0xd2ff), 0, 0, 319, 199, 0);

	wait_for_vsync();

	set_palette(ptr1, 0, 0x60);

	ptr2 = (RealPt)F_PADD(ds_readd(0xc3db), 80000);

	nvf.dst = Real2Host(ds_readd(0xd303));
	nvf.src = Real2Host(ds_readd(0xc3db));
	nvf.nr = 0;
	nvf.type = 3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

	map_effect(Real2Host(ds_readd(0xd303)));

	nvf.dst = Real2Host(ptr2);
	nvf.src = Real2Host(ds_readd(0xc3db));
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
	ds_writed(0xc00d, ds_readd(0xd303));

	do_pic_copy(2);

	delay_or_keypress(100);

	map_effect(Real2Host(ds_readd(0xd303)));

	ds_writed(0xc00d, ds_readd(0xd2ff));

	delay_or_keypress(250);

	memset(Real2Host(ds_readd(0xd303)), 0, 96 * 3);

	for (i = 0; i < 0x40; i++) {

		pal_fade(ptr1, Real2Host(ds_readd(0xd303)));
		pal_fade(ptr1 + 0x60, Real2Host(ds_readd(0xd303)) + 0x60);
		pal_fade(ptr1 + 0xc0, Real2Host(ds_readd(0xd303)) + 0xc0);

		wait_for_vsync();

		set_palette(ptr1, 0, 0x60);
	}

	bc_memset((RealPt)ds_readd(0xd2ff), 0, 64000);

	refresh_colors();
	refresh_screen_size();
}

/* Borlandified and identical */
RealPt hyg_ani_1(signed short nvf_nr, Bit8u *ptr)
{
	HugePt retval;
	struct nvf_desc nvf;

	nvf.dst = Real2Host(host_readd(ptr));
	nvf.src = Real2Host(ds_readd(0xd303));
	nvf.nr = nvf_nr;
	nvf.type = 3;
	nvf.width = ptr + 4;
	nvf.height = ptr + 6;

	process_nvf(&nvf);

	retval = F_PADD((RealPt)host_readd(ptr),
			host_readws(ptr + 4) * host_readws(ptr + 6));

	return (RealPt)retval;
}

/* Borlandified and identical */
void hyg_ani_2(Bit8u *ptr, signed short x, signed short y)
{
	ds_writew(0xc011, x);
	ds_writew(0xc013, y);
	ds_writew(0xc015, x + host_readws(ptr + 4) - 1);
	ds_writew(0xc017, y + host_readws(ptr + 6) - 1);

	ds_writed(0xc019, host_readd(ptr));
	ds_writed(0xc00d, ds_readd(0xd303));

	do_pic_copy(2);
}

/* Borlandified and identical */
void hyg_ani_3(void)
{
	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 199);
	ds_writed(0xc019, (Bit32u)F_PADD((HugePt)ds_readd(0xc3db), 0x1fbd0));
	ds_writed(0xc00d, ds_readd(0xd303));

	do_pic_copy(0);
}

/* Borlandified and identical */
void hyg_ani_4(void)
{
	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 199);
	ds_writed(0xc019, ds_readd(0xd303));
	ds_writed(0xc00d, ds_readd(0xd2ff));

	do_pic_copy(0);
}

/* Borlandified and identical */
void show_hyggelik_ani(void)
{
	signed short i;
	signed short handle;
	Bit32s filelen;
	Bit8u *src;
	RealPt ptr1;
	RealPt ptr2;
	Bit8u array[30*8];

	ds_writew(0xe113, 0);
	ptr1 = (RealPt)ds_readd(0xc3db);
	ptr2 = (RealPt)F_PADD((HugePt)ds_readd(0xc3db), 0x1fbd0);

	handle = load_archive_file(135);
	filelen = read_archive_file(handle, Real2Host(ds_readd(0xd303)), 64000);
	bc_close(handle);
	src = &(Real2Host(ds_readd(0xd303))[filelen - 0xc0]);

	do_fill_rect((RealPt)ds_readd(0xd2ff), 0, 0, 319, 199, 0);
	memcpy((void*)Real2Host(ds_readd(DTP2)), src, 192);
	src = Real2Host(ds_readd(DTP2));

	wait_for_vsync();
	set_palette(src, 0 , 0x40);
	host_writed(array + 0, (Bit32u)ptr2);

	hyg_ani_1(0, array);

	handle = load_archive_file(136);
	filelen = read_archive_file(handle, Real2Host(ds_readd(0xd303)), 64000);
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

	map_effect(Real2Host(ds_readd(0xd303)));

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
	do_fill_rect((RealPt)ds_readd(0xd303), 0, 0, 319, 199, 0);

	hyg_ani_2(array + 25 * 8, 100, 0);
	ds_writed(0xc00d, ds_readd(0xd2ff));
	map_effect(Real2Host(ds_readd(0xd303)));
	delay_or_keypress(500);

	memset((void*)Real2Host(ds_readd(0xd303)), 0, 0xc0);

	for (i = 0; i < 64; i++) {
		pal_fade(src, Real2Host(ds_readd(0xd303)));
		pal_fade(src + 0x60, Real2Host(ds_readd(0xd303)) + 0x60);
		wait_for_vsync();
		set_palette(src, 0, 0x40);
	}

	refresh_screen_size();
	bc_memset((RealPt)ds_readd(0xd2ff), 0, 64000);
	refresh_colors();
}

#if !defined(__BORLANDC__)
}
#endif
