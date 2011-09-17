#include "schick.h"

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg027.h"
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
