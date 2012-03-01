/*
	Rewrite of DSA1 v3.02_de functions of seg046 (Status)
	Functions rewritten: 4/4
*/

#include <stdlib.h>
#include <string.h>

#include "schick.h"
#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg027.h"
#include "seg046.h"
#include "seg047.h"
#include "seg096.h"

namespace M302de {

/**
 *	status_show_spell -	prints spellname and value
 *	@hero:	the hero the spell is from
 *	@spell:	spellnumber
 *	@fsig:	the first spell in the spellgroup
 *	@x1:	the leftmost x coordinate
 *	@x2:	the rightmost x coordinate
 *	@gy:	the upper y coordinate of this spellgroup
 */
void status_show_spell(Bit8u *hero, unsigned short spell, unsigned short fsig,
			unsigned short x1, unsigned short x2, unsigned short gy) {
	char str[10];
	unsigned short group;
	unsigned short y;

	group = spell - fsig;
	y = group * 7 + gy;

	/* print spellname */
	GUI_print_string(get_ltx((spell + 0x6a) * 4), x1, y);

	/* convert value to string */
	sprintf(str, "%d", (signed char)host_readb(hero + 0x13d + spell));

	/* print value */
	GUI_print_string((Bit8u*)str, x2 - GUI_get_space_for_string((Bit8u*)str, 0), y);
}

/**
 *	status_show_talent -	prints talentname and value
 *	@hero:	the hero the talent is from
 *	@talen:	talentnumber
 *	@ftig:	the first talent in the talentgroup
 *	@x1:	the leftmost x coordinate
 *	@x2:	the rightmost x coordinate
 *	@gy:	the upper y coordinate of this talentgroup
 */
void status_show_talent(Bit8u *hero, unsigned short talent, unsigned short ftig,
			unsigned short x1, unsigned short x2, unsigned short gy) {
	char str[10];
	unsigned short group;
	unsigned short y;

	group = talent - ftig;
	y = group * 7 + gy;

	/* print talentname */
	GUI_print_string(get_ltx((talent + 0x30) * 4), x1, y);

	/* convert value to string */
	sprintf(str, "%d", (signed char)host_readb(hero + 0x108 + talent));

	/* print value */
	GUI_print_string((Bit8u*)str, x2 - GUI_get_space_for_string((Bit8u*)str, 0), y);
}

/**
 *	status_show_talents -	shows all talents and their values
 *	@hero:	the hero which talents should be shown
 */
void status_show_talents(Bit8u *hero) {

	unsigned short i, j;

	set_textcolor(0xff, 2);

	/* print talent category names */
	GUI_print_string(get_ltx(0x190),
		GUI_get_first_pos_centered(get_ltx(0x190), 5, 100, 0), 55);

	GUI_print_string(get_ltx(0x1a0),
		GUI_get_first_pos_centered(get_ltx(0x1a0), 110, 100, 0), 55);

	GUI_print_string(get_ltx(0x194),
		GUI_get_first_pos_centered(get_ltx(0x194), 215, 100, 0), 55);

	GUI_print_string(get_ltx(0x1a4),
		GUI_get_first_pos_centered(get_ltx(0x1a4), 5, 100, 0), 125);

	GUI_print_string(get_ltx(0x19c),
		GUI_get_first_pos_centered(get_ltx(0x19c), 110, 100, 0), 125);

	GUI_print_string(get_ltx(0x198),
		GUI_get_first_pos_centered(get_ltx(0x198), 215, 100, 0), 132);

	GUI_print_string(get_ltx(0x1a8),
		GUI_get_first_pos_centered(get_ltx(0x1a8), 5, 100, 0), 174);

	set_textcolor(0, 2);

	for (i = 0; i < 7; i++) {
		j = ds_readb(0x10ce + i * 2);
		while (ds_readb(0x10ce + i * 2) + ds_readb(0x10cf + i * 2) > j) {
			status_show_talent(hero, j,
				ds_readb(0x10ce + i * 2),
				ds_readw(0x6476 + i * 6),
				ds_readw(0x6478 + i * 6),
				ds_readw(0x647a + i * 6));
			j++;
		}
	}
}

/**
 * set_status_string() - a helper for an Original Bugfix
 * @fmt:	format string
 *
 * This makes changes to the max LE visible by changing the format string.
 */
static void set_status_string(char *fmt)
{
	char *fp;

	fp = strstr(fmt, "%d");

	fp[1] = 's';
}

/**
 * reset_status_string() - a helper for an Original Bugfix
 * @fmt:	format string
 *
 * This makes changes to the max LE visible by changing the format string.
 */
static void reset_status_string(char *fmt)
{
	char *fp;

	fp = strstr(fmt, "%s");

	fp[1] = 'd';
}

/**
 * status_show() - shows the status screen of a hero
 * @index:	index of the hero
*/
void status_show(Bit16u index)
{
	char le_fix[10];
	Bit8u *hero;
	char nvf[19];
	Bit8u *n = (Bit8u*)nvf;
	Bit16u bak1, bak2, bak3, bak4;
	Bit16u i, j;
	Bit16u bp;
	Bit16s l1, at, pa, fb;
	Bit8s val;

	bak1 = ds_readw(0xd313);
	bak2 = ds_readw(0xd315);
	bak3 = ds_readw(0xd317);
	bak4 = ds_readw(0xd319);

	hero = get_hero(index);

	set_var_to_zero();
	update_mouse_cursor();

	if (ds_readb(0x2845) != 20) {
		ds_writew(0xc3cb, 0);
		ds_writeb(0x2845, 20);
		do_fill_rect(ds_readd(0xd2ff), 0, 0, 319, 199, 0);
		wait_for_vsync();
		set_palette(MemBase + PhysMake(datseg, 0x6372), 0, 0x20);
	}

	ds_writed(0x29e0, RealMake(datseg, 0x2ad8));
	ds_writed(0x29e4, 0);
	ds_writed(0xd2fb, ds_readd(0xd303));
	set_textcolor(0, 2);

	/* load and draw the background */
	load_pp20(20);

	/* draw the picture of the hero */
	ds_writew(0xc011, 16);
	ds_writew(0xc013, 9);
	ds_writew(0xc015, 47);
	ds_writew(0xc017, 40);
	ds_writed(0xc00d, ds_readd(0xd303));
	/* the source must be passed here as RealPt */
	ds_writed(0xc019, ds_readd(0xbd34) + 0x6da * index + 0x2da);
	do_pic_copy(0);

	ds_writed(0xc00d, ds_readd(0xd2ff));

	/* print invetory and silouette values */
	if (ds_readw(0x2c9b) < 3) {
		/* set dst */
		host_writed(n + 4, ds_readd(0xd2a9));
		/* set type */
		host_writew(n + 10, 0);
		/* place somewhere on unused stack */
		host_writed(n + 11, RealMake(SegValue(ss), reg_sp - 8));
		host_writed(n + 15, RealMake(SegValue(ss), reg_sp - 10));

		for (i = 0; i < 23; i++) {

			if (host_readw(hero + i * 14 + 0x196) == 0)
				continue;

			/* set src */
			host_writed(n + 0, ds_readd(0xd2f7));
			/* set nr */
			host_writew(n + 8,
				host_readw(get_itemsdat(host_readw(hero + i * 14 + 0x196))));

			process_nvf(n);

			/* draw the item icon */
			ds_writew(0xc011, ds_readw(0x63d2 + i * 4));
			ds_writew(0xc013, ds_readw(0x63d2 + i * 4 + 2));
			ds_writew(0xc015, ds_readw(0x63d2 + i * 4) + 15);
			ds_writew(0xc017, ds_readw(0x63d2 + i * 4 + 2) + 15);
			ds_writed(0xc00d, ds_readd(0xd303));
			ds_writed(0xc019, ds_readd(0xd2f7));
			do_pic_copy(0);

			ds_writed(0xc00d, ds_readd(0xd2ff));

			/* check if stackable */
			if (((host_readb(get_itemsdat(host_readw(hero + i * 14 + 0x196)) + 2) >> 4) & 1) == 0)
				continue;

			set_textcolor(0xff, 0);
			/* originally itoa() */
			sprintf((char*)Real2Host(ds_readd(0xd2f3)), "%d",
				host_readw(hero + i * 14 + 0x196 + 2));

			GUI_print_string(Real2Host(ds_readd(0xd2f3)),
				ds_readw(0x63d2 + i * 4) + 16 - GUI_get_space_for_string(Real2Host(ds_readd(0xd2f3)), 0),
				ds_readw(0x63d2 + i * 4 + 2) + 9);

			set_textcolor(0, 2);
		}

		/* print height */
		sprintf((char*)Real2Host(ds_readd(0xd2d3)),
			(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x84)),
			host_readb(hero + 0x23));
		GUI_print_string(Real2Host(ds_readd(0xd2d3)), 158, 116);

		/* print weight */
		sprintf((char*)Real2Host(ds_readd(0xd2d3)),
			(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x88)),
			host_readw(hero + 0x24));
		GUI_print_string(Real2Host(ds_readd(0xd2d3)), 59, 179);
	} else {
		do_fill_rect(ds_readd(0xd303), 0, 50, 319, 191, 2);
	}

	ds_writew(0xd313, 0x5f);

	/* print name */
	set_textcolor(0xff, 2);
	GUI_print_string(hero + 0x10, 59, 9);

	/* print typus */
	set_textcolor(0, 2);

	if (host_readb(hero + 0x22))
		GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + (0x251 + host_readb(hero + 0x21)) * 4)), 59, 16);
	else
		GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + (0x9 + host_readb(hero + 0x21)) * 4)), 59, 16);

	/* show AP */
	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x9ac)),
		host_readd(hero + 0x28));
	GUI_print_string(Real2Host(ds_readd(0xd2f3)), 59, 26);

	/* print level */
	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x1c)),
		host_readb(hero + 0x27));
	GUI_print_string(Real2Host(ds_readd(0xd2f3)), 59, 33);

	/* print money */
	make_valuta_str((char*)Real2Host(ds_readd(0xd2eb)),
		host_readd(hero + 0x2c));

	sprintf((char*)Real2Host(ds_readd(0xd2f3)),
		(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x4b0)),
		(char*)Real2Host(ds_readd(0xd2eb)));
	GUI_print_string(Real2Host(ds_readd(0xd2f3)), 59, 43);

	/* dead, unconscious or drunk */
	if ((host_readb(hero + 0xaa) & 1))
		/* print if dead */
		GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x00)), 155, 9);
	else if (((host_readb(hero + 0xaa) >> 6) & 1))
		/* print if uncounscious */
		GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x18)), 155, 9);
	else if (host_readb(hero + 0xa1))
		/* print if drunk */
		GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0xd8)), 155, 9);

	/* print sleeps */
	if (((host_readb(hero + 0xaa) >> 1) & 1))
		GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x04)), 155, 16);

	/* print stoned */
	if (((host_readb(hero + 0xaa) >> 2) & 1))
		GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x08)), 155, 23);

	/* print diseased */
	if (hero_is_diseased(hero))
		GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x10)), 155, 30);

	/* print poison */
	if (hero_is_poisoned(hero))
		GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x0c)), 155, 37);

	/* print hunger */
	GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x20)), 220, 36);
	/* print thirst */
	GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x24)), 220, 43);


	/* print page content */
	switch (ds_readw(0x2c9b)) {
		/* base values */
		case 1: {

			/* print god */
			ds_writew(0xd313, 265);

			sprintf((char*)Real2Host(ds_readd(0xd2f3)),
					(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x28)),
					(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + (host_readb(hero + 0x26) + 0x15) * 4)));
			GUI_print_string(Real2Host(ds_readd(0xd2f3)), 200, 55);

			/* show attributes */
			ds_writew(0xd313, 220);
			ds_writew(0xd315, 265);
			ds_writew(0xd317, 285);

			for (i = 0; i <= 13; i++) {

				val = (signed char)host_readb(hero + i * 3 + 0x35);
				val += (signed char)host_readb(hero + i * 3 + 0x36);

				sprintf((char*)Real2Host(ds_readd(0xd2eb)) + i * 10,
					(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0xcc)),
					host_readb(hero + i * 3 + 0x34) != val ?
						(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0xc4)) :
						(char*)MemBase + PhysMake(datseg, 0x64a0),
					val,
					host_readb(hero + i * 3 + 0x34) != val ?
						(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0xc8)) :
						(char*)MemBase + PhysMake(datseg, 0x64a1),
					host_readb(hero + i * 3 + 0x34));

			}
			sprintf((char*)Real2Host(ds_readd(0xd2f3)),
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x30)),
				(char*)Real2Host(ds_readd(0xd2eb)),
				(char*)Real2Host(ds_readd(0xd2eb)) + 70,
				(char*)Real2Host(ds_readd(0xd2eb)) + 10,
				(char*)Real2Host(ds_readd(0xd2eb)) + 80,
				(char*)Real2Host(ds_readd(0xd2eb)) + 20,
				(char*)Real2Host(ds_readd(0xd2eb)) + 90,
				(char*)Real2Host(ds_readd(0xd2eb)) + 30,
				(char*)Real2Host(ds_readd(0xd2eb)) + 100,
				(char*)Real2Host(ds_readd(0xd2eb)) + 40,
				(char*)Real2Host(ds_readd(0xd2eb)) + 110,
				(char*)Real2Host(ds_readd(0xd2eb)) + 50,
				(char*)Real2Host(ds_readd(0xd2eb)) + 120,
				(char*)Real2Host(ds_readd(0xd2eb)) + 60,
				(char*)Real2Host(ds_readd(0xd2eb)) + 130);

			GUI_print_string(Real2Host(ds_readd(0xd2f3)), 200, 70);

			/* calculate BP */
			bp = 8;
			if ((signed char)host_readb(hero + 0x47) * 50 <= host_readw(hero + 0x2d8))
				bp--;
			if ((signed char)host_readb(hero + 0x47) * 75 <= host_readw(hero + 0x2d8))
				bp -= 2;
			if ((signed char)host_readb(hero + 0x47) * 100 <= host_readw(hero + 0x2d8))
				bp -= 2;

			if (ds_readw(0xc003) == 2) {
				/* advanded mode */

				/* Original-Bugfix: show permanent damage red */
				set_status_string((char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x34)));

				if (host_readb(hero + 0x7a)) {
					/* print max LE in red if hero has permanent damage */
					sprintf(le_fix, "%c%d%c", 0xf1, host_readw(hero + 0x5e), 0xf0);
				} else {
					/* print max LE in black if hero has no permanent damage */
					sprintf(le_fix, "%d", host_readw(hero + 0x5e));
				}

				sprintf((char*)Real2Host(ds_readd(0xd2f3)),
					(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x34)),
					host_readw(hero + 0x60), le_fix,			/* LE */
					host_readw(hero + 0x64), host_readw(hero + 0x62),	/* AE */
					(signed char)host_readb(hero + 0x66),			/* MR */
					(signed char)host_readb(hero + 0x30) + (signed char)host_readb(hero + 0x31), /* RS */
					(signed char)host_readb(hero + 0x47) + host_readw(hero + 0x60) +
						(signed char)host_readb(hero + 0x48),		/* Ausdauer*/
					host_readw(hero + 0x2d8),				/* Last */
					bp);							/* BP */
				reset_status_string((char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x34)));
				/* Original-Bugfix end */

				GUI_print_string(Real2Host(ds_readd(0xd2f3)), 200, 130);
			} else {
				/* novice mode */

				/* calculate AT base value */
				l1 = (host_readb(hero + 0x32) & 1) ? -1: 0;

				if (host_readb(hero + 0x78) < 7) {
					/* melee weapons */
					at = (signed char)host_readb(hero + 0x68 + (signed char)host_readb(hero + 0x78)) +
						(signed char)host_readb(hero + 0x76) -
						abs((signed char)host_readb(hero + 0x32)) / 2;
				} else {
					/* range weapons */
					at = (signed char)host_readb(hero + 0x68) -
						abs((signed char)host_readb(hero + 0x32)) / 2;
				}
				at += l1;

				/* calculate PA base value */
				if (host_readb(hero + 0x78) < 7) {
					/* melee weapons */
					pa = (signed char)host_readb(hero + 0x6f + (signed char)host_readb(hero + 0x78)) +
						(signed char)host_readb(hero + 0x77) -
						abs((signed char)host_readb(hero + 0x32)) / 2;
				} else {
					/* range weapons */
					pa = (signed char)host_readb(hero + 0x6f) -
						abs((signed char)host_readb(hero + 0x32)) / 2;
				}

				if (at < 0)
					at = 0;
				if (pa < 0)
					pa = 0;

				/* Original-Bugfix: show permanent damage in red */
				set_status_string((char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0xd0)));

				if (host_readb(hero + 0x7a)) {
					/* print max LE in red if hero has permanent damage */
					sprintf(le_fix, "%c%d%c", 0xf1, host_readw(hero + 0x5e), 0xf0);
				} else {
					/* print max LE in black if hero has no permanent damage */
					sprintf(le_fix, "%d", host_readw(hero + 0x5e));
				}


				sprintf((char*)Real2Host(ds_readd(0xd2f3)),
					(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0xd0)),
					host_readw(hero + 0x60), le_fix,			/* LE */
					host_readw(hero + 0x64), host_readw(hero + 0x62),	/* AE */
					at, pa,							/* AT PA */
					(signed char)host_readb(hero + 0x66),			/* MR */
					(signed char)host_readb(hero + 0x30) + (signed char)host_readb(hero + 0x31),	/* RS */
					(signed char)host_readb(hero + 0x47) + host_readw(hero + 0x60) +
						(signed char)host_readb(hero + 0x48),		/* Ausdauer */
					host_readw(hero + 0x2d8),				/* Last */
					bp);							/* BP */

				reset_status_string((char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0xd0)));
				/* Original-Bugfix end */

				GUI_print_string(Real2Host(ds_readd(0xd2f3)), 200, 124);
			}

			break;
		}
		/* AT PA values */
		case 2: {
			ds_writew(0xd313, 275);
			ds_writew(0xd315, 295);

			fb = ((signed char)host_readb(hero + 0x38) +
				(signed char)host_readb(hero + 0x39) +
				(signed char)host_readb(hero + 0x41) +
				(signed char)host_readb(hero + 0x42) +
				(signed char)host_readb(hero + 0x47) +
				(signed char)host_readb(hero + 0x48)) / 4;

			if (host_readb(hero + 0x32) & 1)
				l1 = -1;
			else
				l1 = 0;

			if ((signed char)host_readb(hero + 0x78) < 7) {
				at = (signed char)host_readb(hero + 0x68 + (signed char)host_readb(hero + 0x78)) +
					(signed char)host_readb(hero + 0x76) -
					abs((signed char)host_readb(hero + 0x32)) / 2;
			} else {
				at = (signed char)host_readb(hero + 0x68) - abs((signed char)host_readb(hero + 0x32)) / 2;
			}

			at += l1;

			if ((signed char)host_readb(hero + 0x78) < 7) {
				pa = (signed char)host_readb(hero + 0x6f + (signed char)host_readb(hero + 0x78)) +
					(signed char)host_readb(hero + 0x77) -
					abs((signed char)host_readb(hero + 0x32)) / 2;
			} else {
				pa = (signed char)host_readb(hero + 0x6f) - abs((signed char)host_readb(hero + 0x32)) / 2;
			}

			if (at < 0)
				at = 0;

			if (pa < 0)
				pa = 0;

			sprintf((char*)Real2Host(ds_readd(0xd2f3)),
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3ad)) + 0x14)),

				(signed char)host_readb(hero + 0x67),
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xc0)),

				(signed char)host_readb(hero + 0x68) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(signed char)host_readb(hero + 0x6f) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xc4)),

				(signed char)host_readb(hero + 0x69) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(signed char)host_readb(hero + 0x70) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xc8)),

				(signed char)host_readb(hero + 0x6a) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(signed char)host_readb(hero + 0x71) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xcc)),

				(signed char)host_readb(hero + 0x6b) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(signed char)host_readb(hero + 0x72) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xd0)),

				(signed char)host_readb(hero + 0x6c) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(signed char)host_readb(hero + 0x73) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xd4)),

				(signed char)host_readb(hero + 0x6d) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(signed char)host_readb(hero + 0x74) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xd8)),

				(signed char)host_readb(hero + 0x6e) - abs((signed char)host_readb(hero + 0x32)) / 2,
				(signed char)host_readb(hero + 0x75) - abs((signed char)host_readb(hero + 0x32)) / 2,
				at,
				pa,

				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xdc)),
				(signed char)host_readb(hero + 0x10f) + fb,

				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xe0)),
				(signed char)host_readb(hero + 0x110) + fb);

			GUI_print_string(Real2Host(ds_readd(0xd2f3)), 200, 60);
			break;
		}
		/* skills */
		case 3: {
			status_show_talents(hero);
			break;
		}
		/* spells */
		case 4: {
			/* print headers */
			set_textcolor(0xff, 2);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x300)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x300)), 5, 100, 0),
				55);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x308)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x308)), 110, 100, 0),
				55);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x314)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x314)), 215, 100, 0),
				55);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x304)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x304)), 5, 100, 0),
				97);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x30c)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x30c)), 110, 100, 0),
				104);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x318)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x318)), 215, 100, 0),
				97);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x310)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x310)), 110, 100, 0),
				132);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x31c)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x31c)), 215, 100, 0),
				153);

			/* print values */
			set_textcolor(0, 2);

			for (i = 0; i < 8; i++) {

				j = ds_readb(0xd03 + i * 2);

				while (ds_readb(0xd03 + i * 2) + ds_readb(0xd03 + 1 + i * 2) > j) {

					status_show_spell(hero,
						j,
						ds_readb(0xd03 + i * 2),
						ds_readw(0x642e + 0 + i * 6),
						ds_readw(0x642e + 2 + i * 6),
						ds_readw(0x642e + 4 + i * 6));
					j++;
				}
			}
			break;
		}
		/* more spells */
		case 5: {
			/* print headers */
			set_textcolor(0xff, 2);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x190)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x190)), 5, 100, 0),
				55);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x324)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x324)), 5, 100, 0),
				129);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x328)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x328)), 110, 100, 0),
				55);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x32c)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x32c)), 215, 100, 0),
				55);


			/* show values */
			set_textcolor(0, 2);

			for (i = 0; i < 4; i++) {

				j = ds_readb(0xd13 + i * 2);

				while (ds_readb(0xd13 + i * 2) + ds_readb(0xd13 + 1 + i * 2) > j) {

					status_show_spell(hero,
						j,
						ds_readb(0xd13 + i * 2),
						ds_readw(0x645e + 0 + i * 6),
						ds_readw(0x645e + 2 + i * 6),
						ds_readw(0x645e + 4 + i * 6));
					j++;
				}
			}
			break;
		}
	}

	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 199);
	ds_writed(0xc019, ds_readd(0xd303));
	do_pic_copy(0);

	ds_writew(0xc3cb, 1);

	if (ds_readw(0x2c9b) >= 3) {
		do_v_line(Real2Phys(ds_readd(0xd2ff)), 107, 54, 195, 0);
		do_v_line(Real2Phys(ds_readd(0xd2ff)), 212, 54, 195, 0);
	}

	ds_writew(0xd313, bak1);
	ds_writew(0xd315, bak2);
	ds_writew(0xd317, bak3);
	ds_writew(0xd319, bak4);

	ds_writed(0xd2fb, ds_readd(0xd2ff));

	refresh_screen_size();
}

}
