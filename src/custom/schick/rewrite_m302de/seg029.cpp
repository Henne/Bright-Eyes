#include "schick.h"

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg096.h"

namespace M302de {

/**
 *	draw_playmask() - loads and draws the playmask to the screen
 */
//static
void draw_playmask()
{
	ds_writew(0xc3cb, 0);

	/* load the desired playmask */
	if (ds_readb(0xbc62) == 0)
		load_pp20(0);
	else
		load_pp20(0xd6);

	ds_writeb(0x2845, 0);

	wait_for_vsync();

	set_palette(MemBase + PhysMake(datseg, 0x26c3), 0xe0, 0x20);

	update_mouse_cursor();

	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 199);
	ds_writed(0xc019, ds_readd(0xd303));

	do_pic_copy(0);

	wait_for_vsync();

	set_color(MemBase + PhysMake(datseg, 0xb22d), 0);

	set_palette(MemBase + PhysMake(datseg, 0x27e3), 0xe0, 0x20);

	ds_writew(0xce41, 16);
	ds_writew(0xce3f, 2);

	GUI_set_smth(0x1f, 0x1b);

	ds_writew(0xd2d9, 196);
	ds_writew(0xd2d7, 12);
	ds_writew(0xd2d3, 103);
	ds_writew(0xd2d5, 113);

	ds_writew(0xd313, 205);

	ds_writed(0x29e0, RealMake(datseg, 0x29e8));
	ds_writed(0x29e4, 0);

	ds_writeb(0xc3cb, 1);

	refresh_screen_size();

}

/**
 * copy_forename - copys the forename from a name
 * @dst:	wheres the forename should be stored
 * @name:	the full name
 *
 * A forename has a maximum length of 7 characters.
 */
void copy_forename(Bit8u *dst, Bit8u *name) {

	unsigned short i;

	for (i = 0; i < 7; i++) {
		if (host_readb(name + i) == 0x20) {
			host_writeb(dst + i, 0);
			break;
		}
		host_writeb(dst + i, host_readb(name + i));
	}

	host_writeb(dst + 7, 0);
}

/**
 *	draw_status_line() - draws the status line (pictures and names)
 */
void draw_status_line()
{
	Bit8u *src, *dst;
	unsigned short i, j;
	Bit16s fg, bg;
	Bit16s head_bak;

	ds_writew(0xc3cb, 0);

	GUI_get_smth(&fg, &bg);

	for (i = 0; i < 7; i++) {
		/* Clear name field */
		do_fill_rect(Real2Phys(ds_readd(0xd2ff)),
			ds_readw(0x2d01 + i * 2), 190,
			ds_readw(0x2d01 + i * 2) + 41, 197, 0);

		if (host_readb(get_hero(i) + 0x21) != 0) {

			copy_forename(Real2Host(ds_readd(0xd2f3)),
				get_hero(i) + 0x10);

			GUI_set_smth(0xff, 0);

			/* Gray the names of heros in another group */
			if (host_readb(get_hero(i) + 0x87) != ds_readb(0x2d35))
				GUI_set_smth(0x6f, 0);

			/* print the name */
			GUI_print_string(Real2Host(ds_readd(0xd2f3)),
				GUI_get_first_pos_centered(Real2Host(ds_readd(0xd2f3)),	ds_readw(0x2d01 + i * 2), 43, 0), 190);
		}

		wait_for_vsync();
		update_mouse_cursor();

		if (host_readb(get_hero(i) + 0x21) == 0) {
			clear_hero_icon(i);
		} else {
			if (host_readb(get_hero(i) + 0x87) == ds_readb(0x2d35)) {
				ds_writew(0xc011, ds_readw(0x2d01 + 2 * i));
				ds_writew(0xc013, 157);
				ds_writew(0xc015, ds_readw(0x2d01 + 2 * i) + 31);
				ds_writew(0xc017, 188);

				head_bak = -1;

				/* load skull if hero is dead */
				if (host_readb(get_hero(i) + 0xaa) & 1) {
					head_bak = ds_readw(0x515c);
					load_in_head(0x29);
				}

				/* set the src pointer of the head */
				if (host_readb(get_hero(i) + 0xaa) & 1)
					ds_writed(0xc019, ds_readd(0xd2f3));
				else
					ds_writed(0xc019, ds_readd(0xbd34) + i * 0x6da + 0x2da);

				do_pic_copy(0);

				if (head_bak != -1)
					load_in_head(head_bak);
			} else {

				dst = Real2Host(ds_readd(0xd303));
				head_bak = -1;

				/* load skull if hero is dead */
				if (host_readb(get_hero(i) + 0xaa) & 1) {
					head_bak = ds_readw(0x515c);
					load_in_head(0x29);
				}

				/* set the src pointer of the head */
				if (host_readb(get_hero(i) + 0xaa) & 1)
					src = Real2Host(ds_readd(0xd2f3));
				else
					src = Real2Host(ds_readd(0xbd34)) + i * 0x6da + 0x2da;

				/* Gray out picture */
				for (j = 0; j < 1024; src++, dst++, j++)
					*dst = *src + 0x40;

				ds_writew(0xc011, ds_readw(0x2d01 + 2 * i));
				ds_writew(0xc013, 157);
				ds_writew(0xc015, ds_readw(0x2d01 + 2 * i) + 31);
				ds_writew(0xc017, 188);
				ds_writed(0xc019, ds_readd(0xd303));

				do_pic_copy(0);

				if (head_bak == -1)
					load_in_head(head_bak);
			}

			for (j = 0; j < 6; j++)
				ds_writew(0x2c18 + i * 8 + j * 2, 0xffff);
		}

		refresh_screen_size();
	}

	GUI_set_smth(fg, bg);

	ds_writew(0xc3cb, 1);
}

/**
 * clear_hero_icon - fills hero icon and bars with black color
 * @pos:	position of the hero
 */
void clear_hero_icon(unsigned short pos) {

	/* fill icon area black */
	do_fill_rect(ds_readd(0xd2ff), ds_readw(0x2d01 + pos * 2), 157,
		ds_readw(0x2d01 + pos * 2) + 31, 188, 0);

	/* return if the hero has a class */
	if (host_readb(get_hero(pos) + 0x21))
		return;

	/* fill bars area black */
	do_fill_rect(ds_readd(0xd2ff), ds_readw(0x2d01 + pos * 2) + 33, 157,
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

	seg002_0c72(fd, icon * 576);

	read_archive_file(fd, Real2Host(ds_readd(0xd2e7)) + pos * 576, 576);

	bc_close(fd);

	if (fileindex == 0x0f)
		/* Real Icon */
		ds_writeb(0x5ecc + pos, icon);
	else
		/* Blank Icon */
		ds_writeb(0x5ecc + pos, 0xff);
}

/**
 *	draw_icons() - draws all icons
 */
void draw_icons()
{
	Bit16u i;

	if (ds_readb(0x2845) != 0)
		return;

	update_mouse_cursor();

	for (i = 0; i < 9; i++) {

		ds_writew(0xc011, ds_readw(0x2cdd + i * 4));
		ds_writew(0xc013, ds_readw(0x2cdd + 2 + i * 4));
		ds_writew(0xc015, ds_readw(0x2cdd + i * 4) + 23);
		ds_writew(0xc017, ds_readw(0x2cdd + 2 + i * 4) + 23);
		ds_writed(0xc019, ds_readd(0xd2e7) + i * 576);

		if (ds_readb(0xbd38 + i) != 0xff) {
			if (ds_readb(0x5ecc + i) != ds_readb(0xbd38 + i))
				load_icon(0x0f, ds_readb(0xbd38 + i), i);
		} else {
			if (ds_readb(0x5ecc + i) != 0xff)
				load_icon(0x06, i, i);
		}

		do_pic_copy(0);
	}

	refresh_screen_size();
}

/**
 *	draw_main_screen() - draws the main screen
 */
void draw_main_screen()
{
	ds_writew(0xe111, 0xf1);
	ds_writew(0xe10f, 0x1f);
	ds_writew(0xe113, 0);

	set_var_to_zero();

	if (ds_readb(0x2845))
		draw_playmask();

	clear_loc_line();

	draw_status_line();

	draw_icons();

	draw_compass();

	ds_writew(0xe10d, 1);
	ds_writew(0xe113, 1);

	GUI_set_smth(0x1f, 0x1b);
}

void clear_loc_line() {
	update_mouse_cursor();
	do_fill_rect(ds_readd(0xd2ff), 3, 140, 316, 153, 0);
	refresh_screen_size();
}

/**
 * select_hero_icon - marks a heros icon in the playmask
 * @pos:	position of the hero
 *
 * This function is only used from sell- and repair screens.
 */
void select_hero_icon(unsigned short pos) {

	short fg, bg;

	/* paint a blue border for the pic and bars */
	do_border(Real2Phys(ds_readd(0xd2ff)),
		ds_readw(0x2d01 + pos * 2) - 1, 156,
		ds_readw(0x2d01 + pos * 2) + 42, 189, 0xfc);

	/* paint a blue border for the name */
	do_border(Real2Phys(ds_readd(0xd2ff)),
		ds_readw(0x2d01 + pos * 2) - 1, 189,
		ds_readw(0x2d01 + pos * 2) + 42, 198, 0xfc);

	/* save the textcolors */
	GUI_get_smth(&fg, &bg);

	/* copy the heros forename */
	copy_forename(MemBase + Real2Phys(ds_readd(0xd2f3)),
		get_hero(pos) + 0x10);

	/* set the textcolors */
	GUI_set_smth(0xfc, 0);

	/* print forename */
	GUI_print_string(MemBase + Real2Phys(ds_readd(0xd2f3)),
		GUI_get_first_pos_centered(MemBase + Real2Phys(ds_readd(0xd2f3)),
			ds_readw(0x2d01 + pos * 2), 43, 0), 190);


	/* restore textcolors */
	GUI_set_smth(fg, bg);
}

/**
 * deselect_hero_icon - deselect a heros icon in the playmask
 * @pos:	position of the hero
 *
 * This function is only used from sell- and repair screens.
 */
/* TODO: this function an select_hero_icon() can be merged into one. */
void deselect_hero_icon(unsigned short pos) {

	short fg, bg;

	/* paint a gray border for the pic and bars */
	do_border(Real2Phys(ds_readd(0xd2ff)),
		ds_readw(0x2d01 + pos * 2) - 1, 156,
		ds_readw(0x2d01 + pos * 2) + 42, 189, 0xe6);

	/* paint a gray border for the name */
	do_border(Real2Phys(ds_readd(0xd2ff)),
		ds_readw(0x2d01 + pos * 2) - 1, 189,
		ds_readw(0x2d01 + pos * 2) + 42, 198, 0xe6);

	/* save the textcolors */
	GUI_get_smth(&fg, &bg);

	/* copy the heros forename */
	copy_forename(MemBase + Real2Phys(ds_readd(0xd2f3)),
		get_hero(pos) + 0x10);

	/* set the textcolors */
	GUI_set_smth(0xff, 0);

	/* print forename */
	GUI_print_string(MemBase + Real2Phys(ds_readd(0xd2f3)),
		GUI_get_first_pos_centered(MemBase + Real2Phys(ds_readd(0xd2f3)),
			ds_readw(0x2d01 + pos * 2), 43, 0), 190);


	/* restore textcolors */
	GUI_set_smth(fg, bg);
}

}
