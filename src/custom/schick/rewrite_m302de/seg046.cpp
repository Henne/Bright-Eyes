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
	my_itoa(host_readbs(hero + HERO_SPELLS + spell) , str, 10);

	/* print value */
	GUI_print_string((Bit8u*)str, x2 - GUI_get_space_for_string((Bit8u*)str, 0), gy + group * 7);
}

/**
 *	status_show_skill -	prints skillname and value
 *	@hero:	the hero the skill is from
 *	@talen:	skillnumber
 *	@ftig:	the first skill in the skillgroup
 *	@x1:	the leftmost x coordinate
 *	@x2:	the rightmost x coordinate
 *	@gy:	the upper y coordinate of this skillgroup
 */
/* Borlandified and identical */
void status_show_skill(Bit8u *hero, unsigned short skill, unsigned short ftig,
			unsigned short x1, unsigned short x2, unsigned short gy) {
	unsigned short group;
	char str[10];

	group = skill - ftig;

	/* print skillname */
	GUI_print_string(get_ltx((skill + 0x30) * 4), x1, gy + group * 7);

	/* convert value to string */
	my_itoa(host_readbs(hero + HERO_TA_FIGHT + skill) , str, 10);

	/* print value */
	GUI_print_string((Bit8u*)str, x2 - GUI_get_space_for_string((Bit8u*)str, 0), gy + group * 7);
}

/**
 *	status_show_skills -	shows all skills and their values
 *	@hero:	the hero whose skills should be shown
 */
/* Borlandified and identical */
void status_show_skills(Bit8u *hero) {

	signed short skill_category, skill_nr;

	set_textcolor(0xff, 2);

	/* print skill category names */
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

	for (skill_category = 0; skill_category < 7; skill_category++) {
		skill_nr = ds_readbs(SKILLS_INDEX + skill_category * 2);
		while (ds_readbs(SKILLS_INDEX + skill_category * 2) + ds_readbs((SKILLS_INDEX + 1) + skill_category * 2) > skill_nr) {
			status_show_skill(hero, skill_nr,
				ds_readbs(SKILLS_INDEX + skill_category * 2),
				ds_readw(0x6476 + skill_category * 6),
				ds_readw((0x6476 + 2) + skill_category * 6),
				ds_readw((0x6476 + 4) + skill_category * 6));
			skill_nr++;
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
	Bit16u txt_tabpos1_bak, txt_tabpos2_bak, txt_tabpos3_bak, txt_tabpos4_bak;
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

	txt_tabpos1_bak = ds_readw(TXT_TABPOS1);
	txt_tabpos2_bak = ds_readw(TXT_TABPOS2);
	txt_tabpos3_bak = ds_readw(TXT_TABPOS3);
	txt_tabpos4_bak = ds_readw(TXT_TABPOS4);

	hero = (RealPt)ds_readd(HEROS) + index * SIZEOF_HERO;

	set_var_to_zero();
	update_mouse_cursor();

	if (ds_readb(PP20_INDEX) != ARCHIVE_FILE_ZUSTA_UK) {
		ds_writew(UPDATE_STATUSLINE, 0);
		ds_writeb(PP20_INDEX, ARCHIVE_FILE_ZUSTA_UK);
		do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 0, 0, 319, 199, 0);
		wait_for_vsync();
		set_palette(p_datseg + 0x6372, 0, 0x20);
	}

	ds_writed(ACTION_TABLE_PRIMARY, (Bit32u)RealMake(datseg, ACTION_TABLE_STATUS));
	ds_writed(ACTION_TABLE_SECONDARY, 0);
	ds_writed(TMP_FRAMEBUF_PTR, ds_readd(BUFFER1_PTR));
	set_textcolor(0, 2);

	/* load and draw the background */
	load_pp20(ARCHIVE_FILE_ZUSTA_UK);

	/* draw the picture of the hero */
	ds_writew(PIC_COPY_X1, 16);
	ds_writew(PIC_COPY_Y1, 9);
	ds_writew(PIC_COPY_X2, 47);
	ds_writew(PIC_COPY_Y2, 40);
	ds_writed(PIC_COPY_DST, ds_readd(BUFFER1_PTR));
	/* the source must be passed here as RealPt */
	ds_writed(PIC_COPY_SRC, (Bit32u)(hero + HERO_PORTRAIT));
	do_pic_copy(0);

	ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

	/* print invetory and silouette values */
	if (ds_readws(STATUS_PAGE_MODE) < 3) {

		nvf.src = Real2Host(ds_readd(BUFFER10_PTR));
		nvf.type = 0;

		nvf.width = (Bit8u*)&width;
		nvf.height = (Bit8u*)&height;

		for (i = 0; i < 23; i++) {

			if (host_readw(Real2Host(hero) + i * 14 + HERO_ITEM_HEAD) == 0)
				continue;

			nvf.dst = Real2Host(ds_readd(ICON));
			/* set nr */
			nvf.nr = host_readw(get_itemsdat(host_readw(Real2Host(hero) + i * 14 + HERO_ITEM_HEAD)));

			process_nvf(&nvf);

			/* draw the item icon */
			ds_writew(PIC_COPY_X1, ds_readw(0x63d2 + i * 4));
			ds_writew(PIC_COPY_Y1, ds_readw(0x63d2 + i * 4 + 2));
			ds_writew(PIC_COPY_X2, ds_readw(0x63d2 + i * 4) + 15);
			ds_writew(PIC_COPY_Y2, ds_readw(0x63d2 + i * 4 + 2) + 15);
			ds_writed(PIC_COPY_DST, ds_readd(BUFFER1_PTR));
			ds_writed(PIC_COPY_SRC, ds_readd(ICON));
			do_pic_copy(0);

			ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

			/* check if stackable */
			if (item_stackable(get_itemsdat(host_readw(Real2Host(hero) + i * 14 + HERO_ITEM_HEAD)))) {

				set_textcolor(0xff, 0);
				my_itoa(host_readw(Real2Host(hero) + i * 14 + HERO_ITEM_HEAD + 2),
					(char*)Real2Host(ds_readd(DTP2)), 10);

				GUI_print_string(Real2Host(ds_readd(DTP2)),
					ds_readw(0x63d2 + i * 4) + 16 - GUI_get_space_for_string(Real2Host(ds_readd(DTP2)), 0),
					ds_readw(0x63d2 + i * 4 + 2) + 9);

				set_textcolor(0, 2);
			}
		}

		/* print height */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0x84),
			host_readb(Real2Host(hero) + HERO_HEIGHT));
		GUI_print_string(Real2Host(ds_readd(DTP2)), 158, 116);

		/* print weight */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0x88),
			host_readw(Real2Host(hero) + HERO_WEIGHT));
		GUI_print_string(Real2Host(ds_readd(DTP2)), 59, 179);
	} else {
		do_fill_rect((RealPt)ds_readd(BUFFER1_PTR), 0, 50, 319, 191, 2);
	}

	ds_writew(TXT_TABPOS1, 0x5f);

	/* print name */
	set_textcolor(0xff, 2);
	GUI_print_string(Real2Host(hero) + HERO_NAME2, 59, 9);

	/* print typus */
	set_textcolor(0, 2);

	GUI_print_string(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + (
	((host_readb(Real2Host(hero) + HERO_SEX)) ? 0x251 : 0x9) + host_readbs(Real2Host(hero) + HERO_TYPE)) * 4)), 59, 16);


	/* show AP */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x9ac)),
		host_readd(Real2Host(hero) + HERO_AP));
	GUI_print_string(Real2Host(ds_readd(DTP2)), 59, 26);

	/* print level */
	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_city(0x1c),
		host_readbs(Real2Host(hero) + HERO_LEVEL));
	GUI_print_string(Real2Host(ds_readd(DTP2)), 59, 33);

	/* print money */
	make_valuta_str((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
		host_readd(Real2Host(hero) + HERO_MONEY));

	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x4b0)),
		(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
	GUI_print_string(Real2Host(ds_readd(DTP2)), 59, 43);

	/* dead, unconscious or drunk */
	if (hero_dead(Real2Host(hero)))
		/* print if dead */
		GUI_print_string(get_city(0x00), 155, 9);
	else if (hero_unc(Real2Host(hero)))
		/* print if uncounscious */
		GUI_print_string(get_city(0x18), 155, 9);
	else if (host_readb(Real2Host(hero) + HERO_DRUNK))
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
	switch (ds_readw(STATUS_PAGE_MODE)) {
		/* base values */
		case 1: {

			/* print god */
			ds_writew(TXT_TABPOS1, 265);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x28),
					(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + (host_readbs(Real2Host(hero) + HERO_GOD) + 0x15) * 4)));
			GUI_print_string(Real2Host(ds_readd(DTP2)), 200, 55);

			/* show attributes */
			ds_writew(TXT_TABPOS1, 220);
			ds_writew(TXT_TABPOS2, 265);
			ds_writew(TXT_TABPOS3, 285);

			for (i = 0; i <= 13; i++) {

				val = host_readbs(Real2Host(hero) + i * 3 + HERO_MU)
					+ host_readbs(Real2Host(hero) + i * 3 + 0x36);

				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + i * 10,
					(char*)get_city(0xcc),
					host_readbs(Real2Host(hero) + i * 3 + 0x34) != val ?
						(char*)get_city(0xc4) :
						(char*)p_datseg + EMPTY_STRING6,
					val,
					host_readbs(Real2Host(hero) + i * 3 + 0x34) != val ?
						(char*)get_city(0xc8) :
						(char*)p_datseg + EMPTY_STRING7,
					host_readbs(Real2Host(hero) + i * 3 + 0x34));

			}
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x30),
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 70,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 10,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 80,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 20,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 90,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 30,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 100,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 40,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 110,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 50,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 120,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 60,
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 130);

			GUI_print_string(Real2Host(ds_readd(DTP2)), 200, 70);

			/* calculate BP */
			bp = 8;
			if (host_readbs(Real2Host(hero) + HERO_KK) * 50 <= host_readws(Real2Host(hero) + HERO_LOAD))
				bp--;
			if (host_readbs(Real2Host(hero) + HERO_KK) * 75 <= host_readws(Real2Host(hero) + HERO_LOAD))
				bp -= 2;
			if (host_readbs(Real2Host(hero) + HERO_KK) * 100 <= host_readws(Real2Host(hero) + HERO_LOAD))
				bp -= 2;

			if (ds_readw(GAME_MODE) == 2) {
				/* advanded mode */

#ifdef M302de_ORIGINAL_BUGFIX
				/* Original-Bugfix: show permanent damage red */
				char le_fix[10];
				set_status_string((char*)get_city(0x34));

				if (host_readb(Real2Host(hero) + HERO_LE_MOD)) {
					/* print max LE in red if hero has permanent damage */
					sprintf(le_fix, "%c%d%c", 0xf1, host_readw(Real2Host(hero) + HERO_LE_ORIG), 0xf0);
				} else {
					/* print max LE in black if hero has no permanent damage */
					sprintf(le_fix, "%d", host_readw(Real2Host(hero) + HERO_LE_ORIG));
				}

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x34),
					host_readw(Real2Host(hero) + HERO_LE), le_fix,			/* LE */
					host_readw(Real2Host(hero) + HERO_AE), host_readw(Real2Host(hero) + HERO_AE_ORIG),	/* AE */
					host_readbs(Real2Host(hero) + HERO_MR),			/* MR */
					host_readbs(Real2Host(hero) + HERO_RS_BONUS1) + host_readbs(Real2Host(hero) + HERO_RS_BONUS2), /* RS */
					host_readbs(Real2Host(hero) + HERO_KK) + host_readw(Real2Host(hero) + HERO_LE) +
						host_readbs(Real2Host(hero) + HERO_KK_MOD),		/* Ausdauer*/
					host_readw(Real2Host(hero) + HERO_LOAD),				/* Last */
					bp);							/* BP */
				reset_status_string((char*)get_city(0x34));
				/* Original-Bugfix end */
#else

				/* Original Behaviour: print max LE in black */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x34),
					host_readw(Real2Host(hero) + HERO_LE), host_readw(Real2Host(hero) + HERO_LE_ORIG),	/* LE */
					host_readw(Real2Host(hero) + HERO_AE), host_readw(Real2Host(hero) + HERO_AE_ORIG),	/* AE */
					host_readbs(Real2Host(hero) + HERO_MR),			/* MR */
					host_readbs(Real2Host(hero) + HERO_RS_BONUS1) + host_readbs(Real2Host(hero) + HERO_RS_BONUS2), /* RS */
					host_readbs(Real2Host(hero) + HERO_KK_MOD) + (host_readws(Real2Host(hero) + HERO_LE) +
						host_readbs(Real2Host(hero) + HERO_KK)),		/* Ausdauer*/
					host_readw(Real2Host(hero) + HERO_LOAD),				/* Last */
					bp);							/* BP */
#endif


				GUI_print_string(Real2Host(ds_readd(DTP2)), 200, 130);
			} else {
				/* novice mode */

				/* calculate AT base value */
				if (host_readb(Real2Host(hero) + HERO_RS_BE) & 1) {
					l1 = -1;
				} else {
					l1 = 0;
				}

				at = (host_readbs(Real2Host(hero) + HERO_WP_CLASS) < 7) ?
					/* melee weapons */
					(host_readbs(Real2Host(hero) + HERO_AT + host_readbs(Real2Host(hero) + HERO_WP_CLASS)) +
						host_readbs(Real2Host(hero) + HERO_AT_MOD) -
						(host_readbs(Real2Host(hero) + HERO_RS_BE) / 2)) :
					/* range weapons */
					(host_readbs(Real2Host(hero) + HERO_AT) -
						(host_readbs(Real2Host(hero) + HERO_RS_BE) / 2));
				at += l1;

				/* calculate PA base value */
				pa = (host_readbs(Real2Host(hero) + HERO_WP_CLASS) < 7) ?
					/* melee weapons */
					(host_readbs(Real2Host(hero) + HERO_PA + host_readbs(Real2Host(hero) + HERO_WP_CLASS)) +
						host_readbs(Real2Host(hero) + HERO_PA_MOD) -
						(host_readbs(Real2Host(hero) + HERO_RS_BE) / 2)) :
					/* range weapons */
					 (host_readbs(Real2Host(hero) + HERO_PA) -
						(host_readbs(Real2Host(hero) + HERO_RS_BE) / 2));

				if (at < 0)
					at = 0;
				if (pa < 0)
					pa = 0;
#ifdef M302de_ORIGINAL_BUGFIX
				/* Original-Bugfix: show permanent damage in red */
				set_status_string((char*)get_city(0xd0));

				if (host_readb(Real2Host(hero) + HERO_LE_MOD)) {
					/* print max LE in red if hero has permanent damage */
					sprintf(le_fix, "%c%d%c", 0xf1, host_readw(Real2Host(hero) + HERO_LE_ORIG), 0xf0);
				} else {
					/* print max LE in black if hero has no permanent damage */
					sprintf(le_fix, "%d", host_readw(Real2Host(hero) + HERO_LE_ORIG));
				}


				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0xd0),
					host_readw(Real2Host(hero) + HERO_LE), le_fix,			/* LE */
					host_readw(Real2Host(hero) + HERO_AE), host_readw(Real2Host(hero) + HERO_AE_ORIG),	/* AE */
					at, pa,							/* AT PA */
					host_readbs(Real2Host(hero) + HERO_MR),			/* MR */
					host_readbs(Real2Host(hero) + HERO_RS_BONUS1) + host_readbs(Real2Host(hero) + HERO_RS_BONUS2),	/* RS */
					host_readbs(Real2Host(hero) + HERO_KK) + host_readw(Real2Host(hero) + HERO_LE) +
						host_readbs(Real2Host(hero) + HERO_KK_MOD),		/* Ausdauer */
					host_readw(Real2Host(hero) + HERO_LOAD),				/* Last */
					bp);							/* BP */

				reset_status_string((char*)get_city(0xd0));
				/* Original-Bugfix end */
#else
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0xd0),
					host_readw(Real2Host(hero) + HERO_LE), host_readw(Real2Host(hero) + HERO_LE_ORIG),	/* LE */
					host_readw(Real2Host(hero) + HERO_AE), host_readw(Real2Host(hero) + HERO_AE_ORIG),	/* AE */
					at, pa,							/* AT PA */
					host_readbs(Real2Host(hero) + HERO_MR),			/* MR */
					host_readbs(Real2Host(hero) + HERO_RS_BONUS1) + host_readbs(Real2Host(hero) + HERO_RS_BONUS2),	/* RS */
					host_readws(Real2Host(hero) + HERO_LE) + host_readbs(Real2Host(hero) + HERO_KK) +
						host_readbs(Real2Host(hero) + HERO_KK_MOD),		/* Ausdauer */
					host_readw(Real2Host(hero) + HERO_LOAD),				/* Last */
					bp);							/* BP */
#endif

				GUI_print_string(Real2Host(ds_readd(DTP2)), 200, 124);
			}

			break;
		}
		/* AT PA values */
		case 2: {
			ds_writew(TXT_TABPOS1, 275);
			ds_writew(TXT_TABPOS2, 295);

			j = (host_readbs(Real2Host(hero) + HERO_KL) +
				host_readbs(Real2Host(hero) + HERO_KL_MOD) +
				host_readbs(Real2Host(hero) + HERO_GE) +
				host_readbs(Real2Host(hero) + HERO_GE_MOD) +
				host_readbs(Real2Host(hero) + HERO_KK) +
				host_readbs(Real2Host(hero) + HERO_KK_MOD)) / 4;

			if (host_readb(Real2Host(hero) + HERO_RS_BE) & 1)
				l1 = -1;
			else
				l1 = 0;

			at = (host_readbs(Real2Host(hero) + HERO_WP_CLASS) < 7) ?
				(host_readbs(Real2Host(hero) + HERO_AT + host_readbs(Real2Host(hero) + HERO_WP_CLASS)) +
					host_readbs(Real2Host(hero) + HERO_AT_MOD) -
					host_readbs(Real2Host(hero) + HERO_RS_BE) / 2) :
				host_readbs(Real2Host(hero) + HERO_AT) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2;

			at += l1;

			pa =  (host_readbs(Real2Host(hero) + HERO_WP_CLASS) < 7) ?
				host_readbs(Real2Host(hero) + HERO_PA + host_readbs(Real2Host(hero) + HERO_WP_CLASS)) +
					host_readbs(Real2Host(hero) + HERO_PA_MOD) -
					host_readbs(Real2Host(hero) + HERO_RS_BE) / 2 :
				host_readbs(Real2Host(hero) + HERO_PA) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2;

			if (at < 0)
				at = 0;

			if (pa < 0)
				pa = 0;

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x14),
				host_readbs(Real2Host(hero) + HERO_ATPA_BASIS),
				(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0xc0)),

				host_readbs(Real2Host(hero) + HERO_AT) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				host_readbs(Real2Host(hero) + HERO_PA) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0xc4)),

				host_readbs(Real2Host(hero) + (HERO_AT + 1)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				host_readbs(Real2Host(hero) + (HERO_PA + 1)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0xc8)),

				host_readbs(Real2Host(hero) + (HERO_AT + 2)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				host_readbs(Real2Host(hero) + (HERO_PA + 2)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0xcc)),

				host_readbs(Real2Host(hero) + (HERO_AT + 3)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				host_readbs(Real2Host(hero) + (HERO_PA + 3)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0xd0)),

				host_readbs(Real2Host(hero) + (HERO_AT + 4)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				host_readbs(Real2Host(hero) + (HERO_PA + 4)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0xd4)),

				host_readbs(Real2Host(hero) + (HERO_AT + 5)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				host_readbs(Real2Host(hero) + (HERO_PA + 5)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0xd8)),

				host_readbs(Real2Host(hero) + (HERO_AT + 6)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				host_readbs(Real2Host(hero) + (HERO_PA + 6)) - host_readbs(Real2Host(hero) + HERO_RS_BE) / 2,
				at,
				pa,

				(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0xdc)),
				host_readbs(Real2Host(hero) + (HERO_TA_FIGHT + 7)) + j,

				(char*)Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0xe0)),
				host_readbs(Real2Host(hero) + (HERO_TA_FIGHT + 8)) + j);

			GUI_print_string(Real2Host(ds_readd(DTP2)), 200, 60);
			break;
		}
		/* skills */
		case 3: {
			status_show_skills(Real2Host(hero));
			break;
		}
		/* spells */
		case 4: {
			/* print headers */
			set_textcolor(0xff, 2);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x300)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x300)), 5, 100, 0),
				55);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x308)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x308)), 110, 100, 0),
				55);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x314)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x314)), 215, 100, 0),
				55);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x304)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x304)), 5, 100, 0),
				97);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x30c)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x30c)), 110, 100, 0),
				104);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x318)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x318)), 215, 100, 0),
				97);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x310)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x310)), 110, 100, 0),
				132);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x31c)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x31c)), 215, 100, 0),
				153);

			/* print values */
			set_textcolor(0, 2);

			for (j = 0; j < 8; j++) {

				i = ds_readbs(SPELLS_INDEX + j * 2);

				while (ds_readbs(SPELLS_INDEX + j * 2) + ds_readbs(SPELLS_INDEX + 1 + j * 2) > i) {

					status_show_spell(Real2Host(hero),
						i,
						ds_readbs(SPELLS_INDEX + j * 2),
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
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x190)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x190)), 5, 100, 0),
				55);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x324)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x324)), 5, 100, 0),
				129);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x328)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x328)), 110, 100, 0),
				55);

			GUI_print_string(
				Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x32c)),
				GUI_get_first_pos_centered(Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0x32c)), 215, 100, 0),
				55);


			/* show values */
			set_textcolor(0, 2);

			for (j = 0; j < 4; j++) {

				i = ds_readbs(SPELLS_INDEX2 + j * 2);

				while (ds_readbs(SPELLS_INDEX2 + j * 2) + ds_readbs(SPELLS_INDEX2 + 1 + j * 2) > i) {

					status_show_spell(Real2Host(hero),
						i,
						ds_readbs(SPELLS_INDEX2 + j * 2),
						ds_readws(0x645e + 0 + j * 6),
						ds_readws(0x645e + 2 + j * 6),
						ds_readws(0x645e + 4 + j * 6));
					i++;
				}
			}
			break;
		}
	}

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 319);
	ds_writew(PIC_COPY_Y2, 199);
	ds_writed(PIC_COPY_SRC, ds_readd(BUFFER1_PTR));
	do_pic_copy(0);

	ds_writew(UPDATE_STATUSLINE, 1);

	if (ds_readws(STATUS_PAGE_MODE) >= 3) {
		do_v_line((RealPt)ds_readd(FRAMEBUF_PTR), 107, 54, 195, 0);
		do_v_line((RealPt)ds_readd(FRAMEBUF_PTR), 212, 54, 195, 0);
	}

	ds_writew(TXT_TABPOS1, txt_tabpos1_bak);
	ds_writew(TXT_TABPOS2, txt_tabpos2_bak);
	ds_writew(TXT_TABPOS3, txt_tabpos3_bak);
	ds_writew(TXT_TABPOS4, txt_tabpos4_bak);

	ds_writed(TMP_FRAMEBUF_PTR, ds_readd(FRAMEBUF_PTR));

	refresh_screen_size();
}

#if !defined(__BORLANDC__)
}
#endif
