/*
 *	Rewrite of DSA1 v3.02_de functions of seg029 (playmask)
 *	Functions rewritten: 10/10 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg029.cpp
*/

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg096.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if 0
struct coord {
	unsigned short x,y;
};

/* DS:0x2cdd - DS:0x2d01 */
/* Positions of the icons */
static const struct coord icon_pos[9] = {
	{241, 57}, {268, 57}, {295, 57},
	{241, 84}, {268, 84}, {295, 84},
	{241, 111}, {268, 111}, {295, 111}
};

/* DS:0x5ecc - DS:0x5ed4 */
static signed char icon_array[9] = {
	-2, -2, -2,
	-2, -2, -2,
	-2, -2, -2
};
#endif

/**
 *	draw_playmask() - loads and draws the playmask to the screen
 */
//static
void draw_playmask(void)
{
	ds_writew(0xc3cb, 0);

	/* load the desired playmask */
	if (ds_readb(0xbc62) != 0)
		load_pp20(0xd6);
	else
		load_pp20(0);

	ds_writeb(0x2845, 0);

	wait_for_vsync();

	set_palette(p_datseg + 0x26c3, 0xe0, 0x20);

	update_mouse_cursor();

	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 199);
	ds_writed(0xc019, ds_readd(0xd303));

	do_pic_copy(0);

	wait_for_vsync();

	set_color(p_datseg + 0xb22d, 0);

	set_palette(p_datseg + 0x27e3, 0xe0, 0x20);

	ds_writew(0xce41, 16);
	ds_writew(0xce3f, 2);

	set_textcolor(0x1f, 0x1b);

	ds_writew(0xd2d9, 196);
	ds_writew(0xd2d7, 12);
	ds_writew(0xd2d3, 103);
	ds_writew(0xd2d5, 113);

	ds_writew(0xd313, 205);
#if !defined(__BORLANDC__)
	ds_writed(0x29e0, (long)RealMake(datseg, 0x29e8));
#else
	ds_writed(0x29e0, (long)MK_FP(datseg, 0x29e8));
#endif
	ds_writed(0x29e4, 0);

	ds_writew(0xc3cb, 1);

	refresh_screen_size();

}

/**
 * copy_forename - copys the forename from a name
 * @dst:	wheres the forename should be stored
 * @name:	the full name
 *
 * A forename has a maximum length of 7 characters.
 */
void copy_forename(Bit8u *dst, Bit8u *name)
{

	int i;

	for (i = 0; i < 7; i++) {
		if (host_readb(name + i) == 0x20) {
			host_writeb(dst + i, 0);
			break;
		} else {
			host_writeb(dst + i, host_readb(name + i));
		}
	}

	host_writeb(dst + 7, 0);
}

/**
 *	draw_status_line() - draws the status line (pictures and names)
 */
void draw_status_line(void)
{
	signed short fg_bak, bg_bak;
	Bit8u *src, *dst;
	Bit16s head_bak;
	signed short i, j;

	ds_writew(0xc3cb, 0);

	get_textcolor(&fg_bak, &bg_bak);

	for (i = 0; i < 7; i++) {
		/* Clear name field */
		do_fill_rect((RealPt)ds_readd(0xd2ff),
			ds_readw(0x2d01 + i * 2), 190,
			ds_readw(0x2d01 + i * 2) + 41, 197, 0);

		if (host_readb(get_hero(i) + 0x21) != 0) {

			copy_forename(Real2Host(ds_readd(DTP2)),
				get_hero(i) + 0x10);

			set_textcolor(0xff, 0);

			/* Gray the names of heros in another group */
			if (host_readb(get_hero(i) + 0x87) != ds_readb(CURRENT_GROUP))
				set_textcolor(0x6f, 0);

			/* print the name */
			GUI_print_string(Real2Host(ds_readd(DTP2)),
				GUI_get_first_pos_centered(Real2Host(ds_readd(DTP2)),	ds_readw(0x2d01 + i * 2), 43, 0), 190);
		}

		wait_for_vsync();
		update_mouse_cursor();

		if (!host_readbs(get_hero(i) + 0x21)) {
			clear_hero_icon(i);
		} else {
			if (host_readb(get_hero(i) + 0x87) == ds_readb(CURRENT_GROUP)) {
				ds_writew(0xc011, ds_readw(0x2d01 + 2 * i));
				ds_writew(0xc013, 157);
				ds_writew(0xc015, ds_readw(0x2d01 + 2 * i) + 31);
				ds_writew(0xc017, 188);

				head_bak = -1;

				/* load skull if hero is dead */
				if (hero_dead(get_hero(i))) {
					head_bak = ds_readw(0x515c);
					load_in_head(0x29);
				}

				/* set the src pointer of the head */
				ds_writed(0xc019, (hero_dead(get_hero(i)) ? ds_readd(DTP2) :
					(Bit32u)((RealPt)ds_readd(HEROS) + i * 0x6da + 0x2da)));

				do_pic_copy(0);

				if (head_bak != -1)
					load_in_head(head_bak);
			} else {

				dst = Real2Host(ds_readd(0xd303));
				head_bak = -1;

				/* load skull if hero is dead */
				if (hero_dead(get_hero(i))) {
					head_bak = ds_readw(0x515c);
					load_in_head(0x29);
				}

				/* set the src pointer of the head */
				/* TODO: expression to complicated ? */
				src = (hero_dead(get_hero(i))) ? Real2Host(ds_readd(DTP2)) : (get_hero(i) + 0x2da);

				/* Gray out picture */
				for (j = 0; j < 1024; src++, dst++, j++)
					*dst = *src + 0x40;

				ds_writew(0xc011, ds_readw(0x2d01 + 2 * i));
				ds_writew(0xc013, 157);
				ds_writew(0xc015, ds_readw(0x2d01 + 2 * i) + 31);
				ds_writew(0xc017, 188);
				ds_writed(0xc019, ds_readd(0xd303));

				do_pic_copy(0);

				if (head_bak != -1)
					load_in_head(head_bak);
			}

			for (j = 0; j < 6; j++)
				ds_writew(0x2c18 + i * 8 + j * 2, 0xffff);
		}

		refresh_screen_size();
	}

	set_textcolor(fg_bak, bg_bak);

	ds_writew(0xc3cb, 1);
}

/**
 * clear_hero_icon - fills hero icon and bars with black color
 * @pos:	position of the hero
 */
void clear_hero_icon(unsigned short pos)
{

	/* fill icon area black */
	do_fill_rect((RealPt)ds_readd(0xd2ff), ds_readw(0x2d01 + pos * 2), 157,
		ds_readw(0x2d01 + pos * 2) + 31, 188, 0);

	/* return if the hero has a class */
	if (!host_readbs(get_hero(pos) + 0x21))
		/* fill bars area black */
		do_fill_rect((RealPt)ds_readd(0xd2ff), ds_readw(0x2d01 + pos * 2) + 33, 157,
			ds_readw(0x2d01 + pos * 2) + 39, 188, 0);
}

/**
 * load_icon - loads an icon to the icoc_buffer
 * @fileindex:	index of the icon file (ICONS or BICONS)
 * @icon:	index if the icon in the file
 * @pos:	position of the icon (0-8)
 */
//static
void load_icon(Bit16u fileindex, Bit16s icon, Bit16s pos)
{
	Bit16u fd;

	fd = load_archive_file(fileindex);

	seg002_0c72(fd, icon * 576L, 0);

	read_archive_file(fd, Real2Host(ds_readd(BUF_ICON)) + pos * 576, 576);

	bc_close(fd);

	/* set a real or blank icon */
	ds_writeb(0x5ecc + pos, (fileindex == 0x0f) ? icon : -1);
}

/**
 *	draw_icons() - draws all icons
 */
void draw_icons(void)
{
	signed short i;

	if (ds_readb(0x2845) != 0)
		return;

	update_mouse_cursor();

	for (i = 0; i < 9; i++) {

		ds_writew(0xc011, ds_readw(0x2cdd + i * 4));
		ds_writew(0xc013, ds_readw(0x2cdd + i * 4 + 2));
		ds_writew(0xc015, ds_readw(0x2cdd + i * 4) + 23);
		ds_writew(0xc017, ds_readw(0x2cdd + i * 4 + 2) + 23);
		ds_writed(0xc019, (Bit32u)((RealPt)ds_readd(BUF_ICON) + i * 576));

		if (ds_readbs(0xbd38 + i) != -1) {
			if (ds_readbs(0x5ecc + i) != ds_readbs(0xbd38 + i))
				load_icon(0x0f, ds_readbs(0xbd38 + i), i);
		} else {
			if (ds_readbs(0x5ecc + i) != -1)
				load_icon(0x06, i, i);
		}

		do_pic_copy(0);
	}

	refresh_screen_size();
}

/**
 *	draw_main_screen() - draws the main screen
 */
void draw_main_screen(void)
{
	ds_writew(0xe111, 0xf1);
	ds_writew(WALLCLOCK_Y, 0x1f);
	ds_writew(0xe113, 0);

	set_var_to_zero();

	if (ds_readb(0x2845))
		draw_playmask();

	clear_loc_line();

	draw_status_line();

	draw_icons();

	draw_compass();

	ds_writew(0xe113, ds_writew(0xe10d, 1));

	set_textcolor(0x1f, 0x1b);
}

void clear_loc_line(void)
{
	update_mouse_cursor();
	do_fill_rect((RealPt)ds_readd(0xd2ff), 3, 140, 316, 153, 0);
	refresh_screen_size();
}

/**
 * select_hero_icon - marks a heros icon in the playmask
 * @pos:	position of the hero
 *
 * This function is only used from sell- and repair screens.
 */
void select_hero_icon(unsigned short pos) {

	signed short fg_bak, bg_bak;

	/* paint a blue border for the pic and bars */
	do_border((RealPt)ds_readd(0xd2ff),
		ds_readw(0x2d01 + pos * 2) - 1, 156,
		ds_readw(0x2d01 + pos * 2) + 42, 189, (signed char)0xfc);

	/* paint a blue border for the name */
	do_border((RealPt)ds_readd(0xd2ff),
		ds_readw(0x2d01 + pos * 2) - 1, 189,
		ds_readw(0x2d01 + pos * 2) + 42, 198, (signed char)0xfc);

	/* save the textcolors */
	get_textcolor(&fg_bak, &bg_bak);

	/* copy the heros forename */
	copy_forename(Real2Host(ds_readd(DTP2)), get_hero(pos) + 0x10);

	/* set the textcolors */
	set_textcolor(0xfc, 0);

	/* print forename */
	GUI_print_string(Real2Host(ds_readd(DTP2)),
		GUI_get_first_pos_centered(Real2Host(ds_readd(DTP2)),
			ds_readw(0x2d01 + pos * 2), 43, 0), 190);


	/* restore textcolors */
	set_textcolor(fg_bak, bg_bak);
}

/**
 * deselect_hero_icon - deselect a heros icon in the playmask
 * @pos:	position of the hero
 *
 * This function is only used from sell- and repair screens.
 */
/* TODO: this function an select_hero_icon() can be merged into one. */
void deselect_hero_icon(unsigned short pos) {

	signed short fg_bak, bg_bak;

	/* paint a gray border for the pic and bars */
	do_border((RealPt)ds_readd(0xd2ff),
		ds_readw(0x2d01 + pos * 2) - 1, 156,
		ds_readw(0x2d01 + pos * 2) + 42, 189, (signed char)0xe6);

	/* paint a gray border for the name */
	do_border((RealPt)ds_readd(0xd2ff),
		ds_readw(0x2d01 + pos * 2) - 1, 189,
		ds_readw(0x2d01 + pos * 2) + 42, 198, (signed char)0xe6);

	/* save the textcolors */
	get_textcolor(&fg_bak, &bg_bak);

	/* copy the heros forename */
	copy_forename(Real2Host(ds_readd(DTP2)), get_hero(pos) + 0x10);

	/* set the textcolors */
	set_textcolor(0xff, 0);

	/* print forename */
	GUI_print_string(Real2Host(ds_readd(DTP2)),
		GUI_get_first_pos_centered(Real2Host(ds_readd(DTP2)),
			ds_readw(0x2d01 + pos * 2), 43, 0), 190);


	/* restore textcolors */
	set_textcolor(fg_bak, bg_bak);
}

#if !defined(__BORLANDC__)
}
#endif
