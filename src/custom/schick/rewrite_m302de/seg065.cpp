/**
 *	Rewrite of DSA1 v3.02_de functions of seg065 (special animations)
 *	Functions rewritten: 6/9
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
#if !defined(__BORLANDC__)
}
#endif
