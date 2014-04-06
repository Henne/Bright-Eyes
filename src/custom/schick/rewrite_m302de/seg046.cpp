/*
 *	Rewrite of DSA1 v3.02_de functions of seg046 (Status)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg046.cpp
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg027.h"
#include "seg046.h"
#include "seg047.h"
#include "seg096.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 *	status_show_spell -	prints spellname and value
 *	@hero:	the hero the spell is from
 *	@spell:	spellnumber
 *	@fsig:	the first spell in the spellgroup
 *	@x1:	the leftmost x coordinate
 *	@x2:	the rightmost x coordinate
 *	@gy:	the upper y coordinate of this spellgroup
 */
/* Borlandified and identical */
void status_show_spell(Bit8u *hero, unsigned short spell, unsigned short fsig,
			unsigned short x1, unsigned short x2, unsigned short gy) {
	unsigned short group;
	char str[10];

	group = spell - fsig;

	/* print spellname */
	GUI_print_string(get_ltx((spell + 0x6a) * 4), x1, gy + group * 7);

	/* convert value to string */
#if !defined(__BORLANDC__)
	sprintf(str, "%d", host_readbs(hero + 0x13d + spell));
#else
	itoa(host_readbs(hero + 0x13d + spell) , str, 10);
#endif

	/* print value */
	GUI_print_string((Bit8u*)str, x2 - GUI_get_space_for_string((Bit8u*)str, 0), gy + group * 7);
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
/* Borlandified and identical */
void status_show_talent(Bit8u *hero, unsigned short talent, unsigned short ftig,
			unsigned short x1, unsigned short x2, unsigned short gy) {
	unsigned short group;
	char str[10];

	group = talent - ftig;

	/* print talentname */
	GUI_print_string(get_ltx((talent + 0x30) * 4), x1, gy + group * 7);

	/* convert value to string */
#if !defined(__BORLANDC__)
	sprintf(str, "%d", host_readbs(hero + 0x108 + talent));
#else
	itoa(host_readbs(hero + 0x108 + talent) , str, 10);
#endif

	/* print value */
	GUI_print_string((Bit8u*)str, x2 - GUI_get_space_for_string((Bit8u*)str, 0), gy + group * 7);
}

/**
 *	status_show_talents -	shows all talents and their values
 *	@hero:	the hero which talents should be shown
 */
/* Borlandified and identical */
void status_show_talents(Bit8u *hero) {

	signed short i, j;

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
		j = ds_readbs(0x10ce + i * 2);
		while (ds_readbs(0x10ce + i * 2) + ds_readbs(0x10cf + i * 2) > j) {
			status_show_talent(hero, j,
				ds_readbs(0x10ce + i * 2),
				ds_readw(0x6476 + i * 6),
				ds_readw(0x6478 + i * 6),
				ds_readw(0x647a + i * 6));
			j++;
		}
	}
}

#ifdef M302de_ORIGINAL_BUGFIX
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
#endif

/**
 * status_show() - shows the status screen of a hero
 * @index:	index of the hero
*/
void status_show(Bit16u index)
{
#ifdef M302de_ORIGINAL_BUGFIX
	char le_fix[10];
#endif
	RealPt hero;
	Bit16u bak1, bak2, bak3, bak4;
	Bit8s val;
	signed short width;
	signed short height;
	signed short at;
	signed short pa;
	signed short l1;
	signed short bp;
	signed short i;
	signed short j;

	struct nvf_desc nvf;

	bak1 = ds_readw(0xd313);
	bak2 = ds_readw(0xd315);
	bak3 = ds_readw(0xd317);
	bak4 = ds_readw(0xd319);

	hero = (RealPt)ds_readd(HEROS) + index * 0x6da;

	set_var_to_zero();
	update_mouse_cursor();

	if (ds_readb(0x2845) != 20) {
		ds_writew(0xc3cb, 0);
		ds_writeb(0x2845, 20);
		do_fill_rect((RealPt)ds_readd(0xd2ff), 0, 0, 319, 199, 0);
		wait_for_vsync();
		set_palette(p_datseg + 0x6372, 0, 0x20);
	}

	ds_writed(0x29e0, (Bit32u)RealMake(datseg, 0x2ad8));
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
	ds_writed(0xc019, (Bit32u)(hero + 0x2da));
	do_pic_copy(0);

	ds_writed(0xc00d, ds_readd(0xd2ff));

	/* print invetory and silouette values */
	if (ds_readws(0x2c9b) < 3) {

		nvf.src = Real2Host(ds_readd(0xd2a9));
		nvf.type = 0;

		nvf.width = (Bit8u*)&width;
		nvf.height = (Bit8u*)&height;

		for (i = 0; i < 23; i++) {

			if (host_readw(Real2Host(hero) + i * 14 + 0x196) == 0)
				continue;

			nvf.dst = Real2Host(ds_readd(0xd2f7));
			/* set nr */
			nvf.nr = host_readw(get_itemsdat(host_readw(Real2Host(hero) + i * 14 + 0x196)));

			process_nvf(&nvf);

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
			/* TODO: bit flags operation */
			if (item_stackable(get_itemsdat(host_readw(Real2Host(hero) + i * 14 + 0x196)))) {

				set_textcolor(0xff, 0);
				/* originally itoa() */
#if !defined(__BORLANDC__)
				sprintf((char*)Real2Host(ds_readd(DTP2)), "%d",
					host_readw(Real2Host(hero) + i * 14 + 0x196 + 2));
#else
				itoa(host_readw(Real2Host(hero) + i * 14 + 0x196 + 2),
					(char*)Real2Host(ds_readd(DTP2)), 10);
#endif

				GUI_print_string(Real2Host(ds_readd(DTP2)),
					ds_readw(0x63d2 + i * 4) + 16 - GUI_get_space_for_string(Real2Host(ds_readd(DTP2)), 0),
					ds_readw(0x63d2 + i * 4 + 2) + 9);

				set_textcolor(0, 2);
			}
		}

		/* print height */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0x84),
			host_readb(Real2Host(hero) + 0x23));
		GUI_print_string(Real2Host(ds_readd(DTP2)), 158, 116);

		/* print weight */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0x88),
			host_readw(Real2Host(hero) + 0x24));
		GUI_print_string(Real2Host(ds_readd(DTP2)), 59, 179);
	} else {
		do_fill_rect((RealPt)ds_readd(0xd303), 0, 50, 319, 191, 2);
	}

	ds_writew(0xd313, 0x5f);

	/* print name */
	set_textcolor(0xff, 2);
	GUI_print_string(Real2Host(hero) + 0x10, 59, 9);

	/* print typus */
	set_textcolor(0, 2);

	GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + (
	((host_readb(Real2Host(hero) + 0x22)) ? 0x251 : 0x9) + host_readbs(Real2Host(hero) + 0x21)) * 4)), 59, 16);


	/* show AP */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x9ac)),
		host_readd(Real2Host(hero) + 0x28));
	GUI_print_string(Real2Host(ds_readd(DTP2)), 59, 26);

	/* print level */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_city(0x1c),
		host_readbs(Real2Host(hero) + 0x27));
	GUI_print_string(Real2Host(ds_readd(DTP2)), 59, 33);

	/* print money */
	make_valuta_str((char*)Real2Host(ds_readd(0xd2eb)),
		host_readd(Real2Host(hero) + 0x2c));

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0x4b0)),
		(char*)Real2Host(ds_readd(0xd2eb)));
	GUI_print_string(Real2Host(ds_readd(DTP2)), 59, 43);

	/* dead, unconscious or drunk */
	if (hero_dead(Real2Host(hero)))
		/* print if dead */
		GUI_print_string(get_city(0x00), 155, 9);
	else if (hero_unc(Real2Host(hero)))
		/* print if uncounscious */
		GUI_print_string(get_city(0x18), 155, 9);
	else if (host_readb(Real2Host(hero) + 0xa1))
		/* print if drunk */
		GUI_print_string(get_city(0xd8), 155, 9);

	/* print sleeps */
	if (hero_sleeps(Real2Host(hero)))
		GUI_print_string(get_city(0x04), 155, 16);

	/* print stoned */
	if (hero_stoned(Real2Host(hero)))
		GUI_print_string(get_city(0x08), 155, 23);

	/* print diseased */
	if (hero_is_diseased(Real2Host(hero)))
		GUI_print_string(get_city(0x10), 155, 30);

	/* print poison */
	if (hero_is_poisoned(Real2Host(hero)))
		GUI_print_string(get_city(0x0c), 155, 37);

	/* print hunger */
	GUI_print_string(get_city(0x20), 220, 36);
	/* print thirst */
	GUI_print_string(get_city(0x24), 220, 43);


	/* print page content */
	switch (ds_readw(0x2c9b)) {
		/* base values */
		case 1: {

			/* print god */
			ds_writew(0xd313, 265);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x28),
					(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + (host_readbs(Real2Host(hero) + 0x26) + 0x15) * 4)));
			GUI_print_string(Real2Host(ds_readd(DTP2)), 200, 55);

			/* show attributes */
			ds_writew(0xd313, 220);
			ds_writew(0xd315, 265);
			ds_writew(0xd317, 285);

			for (i = 0; i <= 13; i++) {

				val = host_readbs(Real2Host(hero) + i * 3 + 0x35)
					+ host_readbs(Real2Host(hero) + i * 3 + 0x36);

				sprintf((char*)Real2Host(ds_readd(0xd2eb)) + i * 10,
					(char*)get_city(0xcc),
					host_readbs(Real2Host(hero) + i * 3 + 0x34) != val ?
						(char*)get_city(0xc4) :
						(char*)p_datseg + 0x64a0,
					val,
					host_readbs(Real2Host(hero) + i * 3 + 0x34) != val ?
						(char*)get_city(0xc8) :
						(char*)p_datseg + 0x64a1,
					host_readbs(Real2Host(hero) + i * 3 + 0x34));

			}
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x30),
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

			GUI_print_string(Real2Host(ds_readd(DTP2)), 200, 70);

			/* calculate BP */
			bp = 8;
			if (host_readbs(Real2Host(hero) + 0x47) * 50 <= host_readws(Real2Host(hero) + 0x2d8))
				bp--;
			if (host_readbs(Real2Host(hero) + 0x47) * 75 <= host_readws(Real2Host(hero) + 0x2d8))
				bp -= 2;
			if (host_readbs(Real2Host(hero) + 0x47) * 100 <= host_readws(Real2Host(hero) + 0x2d8))
				bp -= 2;

			if (ds_readw(0xc003) == 2) {
				/* advanded mode */

#ifdef M302de_ORIGINAL_BUGFIX
				/* Original-Bugfix: show permanent damage red */
				char le_fix[10];
				set_status_string((char*)get_city(0x34));

				if (host_readb(Real2Host(hero) + 0x7a)) {
					/* print max LE in red if hero has permanent damage */
					sprintf(le_fix, "%c%d%c", 0xf1, host_readw(Real2Host(hero) + 0x5e), 0xf0);
				} else {
					/* print max LE in black if hero has no permanent damage */
					sprintf(le_fix, "%d", host_readw(Real2Host(hero) + 0x5e));
				}

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x34),
					host_readw(Real2Host(hero) + 0x60), le_fix,			/* LE */
					host_readw(Real2Host(hero) + 0x64), host_readw(Real2Host(hero) + 0x62),	/* AE */
					host_readbs(Real2Host(hero) + 0x66),			/* MR */
					host_readbs(Real2Host(hero) + 0x30) + host_readbs(Real2Host(hero) + 0x31), /* RS */
					host_readbs(Real2Host(hero) + 0x47) + host_readw(Real2Host(hero) + 0x60) +
						host_readbs(Real2Host(hero) + 0x48),		/* Ausdauer*/
					host_readw(Real2Host(hero) + 0x2d8),				/* Last */
					bp);							/* BP */
				reset_status_string((char*)get_city(0x34));
				/* Original-Bugfix end */
#else

				/* Original Behaviour: print max LE in black */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x34),
					host_readw(Real2Host(hero) + 0x60), host_readw(Real2Host(hero) + 0x5e),	/* LE */
					host_readw(Real2Host(hero) + 0x64), host_readw(Real2Host(hero) + 0x62),	/* AE */
					host_readbs(Real2Host(hero) + 0x66),			/* MR */
					host_readbs(Real2Host(hero) + 0x30) + host_readbs(Real2Host(hero) + 0x31), /* RS */
					host_readbs(Real2Host(hero) + 0x48) + (host_readws(Real2Host(hero) + 0x60) +
						host_readbs(Real2Host(hero) + 0x47)),		/* Ausdauer*/
					host_readw(Real2Host(hero) + 0x2d8),				/* Last */
					bp);							/* BP */
#endif


				GUI_print_string(Real2Host(ds_readd(DTP2)), 200, 130);
			} else {
				/* novice mode */

				/* calculate AT base value */
				if (host_readb(Real2Host(hero) + 0x32) & 1) {
					l1 = -1;
				} else {
					l1 = 0;
				}

				at = (host_readbs(Real2Host(hero) + 0x78) < 7) ?
					/* melee weapons */
					(host_readbs(Real2Host(hero) + 0x68 + host_readbs(Real2Host(hero) + 0x78)) +
						host_readbs(Real2Host(hero) + 0x76) -
						(host_readbs(Real2Host(hero) + 0x32) / 2)) :
					/* range weapons */
					(host_readbs(Real2Host(hero) + 0x68) -
						(host_readbs(Real2Host(hero) + 0x32) / 2));
				at += l1;

				/* calculate PA base value */
				pa = (host_readbs(Real2Host(hero) + 0x78) < 7) ?
					/* melee weapons */
					(host_readbs(Real2Host(hero) + 0x6f + host_readbs(Real2Host(hero) + 0x78)) +
						host_readbs(Real2Host(hero) + 0x77) -
						(host_readbs(Real2Host(hero) + 0x32) / 2)) :
					/* range weapons */
					 (host_readbs(Real2Host(hero) + 0x6f) -
						(host_readbs(Real2Host(hero) + 0x32) / 2));

				if (at < 0)
					at = 0;
				if (pa < 0)
					pa = 0;
#ifdef M302de_ORIGINAL_BUGFIX
				/* Original-Bugfix: show permanent damage in red */
				set_status_string((char*)get_city(0xd0));

				if (host_readb(Real2Host(hero) + 0x7a)) {
					/* print max LE in red if hero has permanent damage */
					sprintf(le_fix, "%c%d%c", 0xf1, host_readw(Real2Host(hero) + 0x5e), 0xf0);
				} else {
					/* print max LE in black if hero has no permanent damage */
					sprintf(le_fix, "%d", host_readw(Real2Host(hero) + 0x5e));
				}


				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0xd0),
					host_readw(Real2Host(hero) + 0x60), le_fix,			/* LE */
					host_readw(Real2Host(hero) + 0x64), host_readw(Real2Host(hero) + 0x62),	/* AE */
					at, pa,							/* AT PA */
					host_readbs(Real2Host(hero) + 0x66),			/* MR */
					host_readbs(Real2Host(hero) + 0x30) + host_readbs(Real2Host(hero) + 0x31),	/* RS */
					host_readbs(Real2Host(hero) + 0x47) + host_readw(Real2Host(hero) + 0x60) +
						host_readbs(Real2Host(hero) + 0x48),		/* Ausdauer */
					host_readw(Real2Host(hero) + 0x2d8),				/* Last */
					bp);							/* BP */

				reset_status_string((char*)get_city(0xd0));
				/* Original-Bugfix end */
#else
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0xd0),
					host_readw(Real2Host(hero) + 0x60), host_readw(Real2Host(hero) + 0x5e),	/* LE */
					host_readw(Real2Host(hero) + 0x64), host_readw(Real2Host(hero) + 0x62),	/* AE */
					at, pa,							/* AT PA */
					host_readbs(Real2Host(hero) + 0x66),			/* MR */
					host_readbs(Real2Host(hero) + 0x30) + host_readbs(Real2Host(hero) + 0x31),	/* RS */
					host_readws(Real2Host(hero) + 0x60) + host_readbs(Real2Host(hero) + 0x47) +
						host_readbs(Real2Host(hero) + 0x48),		/* Ausdauer */
					host_readw(Real2Host(hero) + 0x2d8),				/* Last */
					bp);							/* BP */
#endif

				GUI_print_string(Real2Host(ds_readd(DTP2)), 200, 124);
			}

			break;
		}
		/* AT PA values */
		case 2: {
			ds_writew(0xd313, 275);
			ds_writew(0xd315, 295);

			j = (host_readbs(Real2Host(hero) + 0x38) +
				host_readbs(Real2Host(hero) + 0x39) +
				host_readbs(Real2Host(hero) + 0x41) +
				host_readbs(Real2Host(hero) + 0x42) +
				host_readbs(Real2Host(hero) + 0x47) +
				host_readbs(Real2Host(hero) + 0x48)) / 4;

			if (host_readb(Real2Host(hero) + 0x32) & 1)
				l1 = -1;
			else
				l1 = 0;

			at = (host_readbs(Real2Host(hero) + 0x78) < 7) ?
				(host_readbs(Real2Host(hero) + 0x68 + host_readbs(Real2Host(hero) + 0x78)) +
					host_readbs(Real2Host(hero) + 0x76) -
					host_readbs(Real2Host(hero) + 0x32) / 2) :
				host_readbs(Real2Host(hero) + 0x68) - host_readbs(Real2Host(hero) + 0x32) / 2;

			at += l1;

			pa =  (host_readbs(Real2Host(hero) + 0x78) < 7) ?
				host_readbs(Real2Host(hero) + 0x6f + host_readbs(Real2Host(hero) + 0x78)) +
					host_readbs(Real2Host(hero) + 0x77) -
					host_readbs(Real2Host(hero) + 0x32) / 2 :
				host_readbs(Real2Host(hero) + 0x6f) - host_readbs(Real2Host(hero) + 0x32) / 2;

			if (at < 0)
				at = 0;

			if (pa < 0)
				pa = 0;

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x14),
				host_readbs(Real2Host(hero) + 0x67),
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xc0)),

				host_readbs(Real2Host(hero) + 0x68) - host_readbs(Real2Host(hero) + 0x32) / 2,
				host_readbs(Real2Host(hero) + 0x6f) - host_readbs(Real2Host(hero) + 0x32) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xc4)),

				host_readbs(Real2Host(hero) + 0x69) - host_readbs(Real2Host(hero) + 0x32) / 2,
				host_readbs(Real2Host(hero) + 0x70) - host_readbs(Real2Host(hero) + 0x32) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xc8)),

				host_readbs(Real2Host(hero) + 0x6a) - host_readbs(Real2Host(hero) + 0x32) / 2,
				host_readbs(Real2Host(hero) + 0x71) - host_readbs(Real2Host(hero) + 0x32) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xcc)),

				host_readbs(Real2Host(hero) + 0x6b) - host_readbs(Real2Host(hero) + 0x32) / 2,
				host_readbs(Real2Host(hero) + 0x72) - host_readbs(Real2Host(hero) + 0x32) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xd0)),

				host_readbs(Real2Host(hero) + 0x6c) - host_readbs(Real2Host(hero) + 0x32) / 2,
				host_readbs(Real2Host(hero) + 0x73) - host_readbs(Real2Host(hero) + 0x32) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xd4)),

				host_readbs(Real2Host(hero) + 0x6d) - host_readbs(Real2Host(hero) + 0x32) / 2,
				host_readbs(Real2Host(hero) + 0x74) - host_readbs(Real2Host(hero) + 0x32) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xd8)),

				host_readbs(Real2Host(hero) + 0x6e) - host_readbs(Real2Host(hero) + 0x32) / 2,
				host_readbs(Real2Host(hero) + 0x75) - host_readbs(Real2Host(hero) + 0x32) / 2,
				at,
				pa,

				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xdc)),
				host_readbs(Real2Host(hero) + 0x10f) + j,

				(char*)Real2Host(host_readd(Real2Host(ds_readd(0xc3b5)) + 0xe0)),
				host_readbs(Real2Host(hero) + 0x110) + j);

			GUI_print_string(Real2Host(ds_readd(DTP2)), 200, 60);
			break;
		}
		/* skills */
		case 3: {
			status_show_talents(Real2Host(hero));
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

			for (j = 0; j < 8; j++) {

				i = ds_readbs(0xd03 + j * 2);

				while (ds_readbs(0xd03 + j * 2) + ds_readbs(0xd03 + 1 + j * 2) > i) {

					status_show_spell(Real2Host(hero),
						i,
						ds_readbs(0xd03 + j * 2),
						ds_readws(0x642e + 0 + j * 6),
						ds_readws(0x642e + 2 + j * 6),
						ds_readws(0x642e + 4 + j * 6));
					i++;
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

			for (j = 0; j < 4; j++) {

				i = ds_readbs(0xd13 + j * 2);

				while (ds_readbs(0xd13 + j * 2) + ds_readbs(0xd13 + 1 + j * 2) > i) {

					status_show_spell(Real2Host(hero),
						i,
						ds_readbs(0xd13 + j * 2),
						ds_readws(0x645e + 0 + j * 6),
						ds_readws(0x645e + 2 + j * 6),
						ds_readws(0x645e + 4 + j * 6));
					i++;
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

	if (ds_readws(0x2c9b) >= 3) {
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

#if !defined(__BORLANDC__)
}
#endif
