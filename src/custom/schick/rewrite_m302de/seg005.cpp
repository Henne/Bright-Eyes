/*
 *	Rewrite of DSA1 v3.02_de functions of seg005 (fight)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg005.cpp
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(__BORLANDC__)
#include "callback.h"
#endif

#include "v302de.h"
#include "common.h"

#include "seg000.h"
#include "seg001.h"
#include "seg002.h"
#include "seg004.h"
#include "seg005.h"
#include "seg006.h"
#include "seg027.h"
#include "seg032.h"
#include "seg096.h"


#if !defined(__BORLANDC__)
namespace M302de {
#endif


/**
 *
 * \param   p           pointer to a FIGHTER object
 * \param   x           x coordinate on the screen
 * \param   y           y coordinate on the screen
 *
 */
unsigned short FIG_obj_needs_refresh(Bit8u *p, signed short x, signed short y)
{
	Bit8u *i;
	signed short ox;
	signed short oy;

	if (host_readb(p + FIGHTER_VISIBLE) != 0) {

	/* animated objects always need a refresh */
		if ((host_readbs(p + FIGHTER_SHEET) != -1) || (host_readbs(p + FIGHTER_VISIBLE) == 3))
			goto damn_label;

		/* i = i->next; */
		/* check if given object overlaps with any of the objects behind it */
		for (i = Real2Host(ds_readd(FIG_LIST_HEAD)); i != p; i = Real2Host(host_readd(i + FIGHTER_NEXT)))
		{
			/* Ignore invisible objects or objects, that are not refreshed */
			if (host_readbs(i + FIGHTER_VISIBLE) >= 2) {

				ox = 10 - host_readbs(i + FIGHTER_WIDTH) / 2
					+ (host_readbs(i + FIGHTER_CBX) + host_readbs(i + FIGHTER_CBY)) * 10;

				oy = 118 - host_readbs(i + FIGHTER_HEIGHT)
					+ (host_readbs(i + FIGHTER_CBX) - host_readbs(i + FIGHTER_CBY)) * 5;

				ox += host_readbs(i + FIGHTER_OFFSETX);
				oy += host_readbs(i + FIGHTER_OFFSETY);

				if (!((x + host_readbs(p + FIGHTER_WIDTH) < ox)
					|| (x - host_readbs(i + FIGHTER_WIDTH) > ox)
					|| (y + host_readbs(p + FIGHTER_HEIGHT) < oy)
					|| (y - host_readbs(i + FIGHTER_HEIGHT) > oy)))
				{
damn_label:
					if (host_readb(p + FIGHTER_VISIBLE) == 1)
						host_writeb(p + FIGHTER_VISIBLE, 2);
					return 1;
				}
			}
		}
	}

	return 0;
}

/**
 * \brief   set the color of the star in fights
 *
 *          Sets the color of the star which shows fight activities,
 *          like damage, in fights.
 *
 * \param   ptr         pointer to the star template
 * \param   count       number of bytes the star has
 * \param   color       1=red/2=green/3=blue/4=yellow/11=darkbrown/12=lightbrown/13=pink
 *
 */
void FIG_set_star_color(Bit8u *ptr, unsigned short count, unsigned char color)
{
	Bit8u *p;

	color += 0x80;

	for (p = ptr; count--; p++) {
		if (host_readb(p)) {
			host_writeb(p, color);
		}
	}
}

/**
 * \brief   returns name in the 3rd german case
 *
 * \param   type        1 = enemy / 2 = hero
 * \param   pos         position
 * \return              "einem Magier" if the enemy is a "Magier".
 */
//static
RealPt FIG_name_3rd_case(unsigned short type, volatile unsigned short pos)
{
	if (type == 2) {
		return (RealPt)ds_readd(HEROES) + pos * SIZEOF_HERO + HERO_NAME2;
	} else {
		return GUI_names_grammar(3, pos, 1);
	}
}

/**
 * \brief   returns name in the 4th german case
 *
 * \param   type        1 = enemy / 2 = hero
 * \param   pos         position
 * \return              "einen Magier" if the enemy is a "Magier".
 */
//static
RealPt FIG_name_4th_case(unsigned short type, volatile unsigned short pos)
{

	if (type == 2)
		return (RealPt)ds_readd(HEROES) + pos * SIZEOF_HERO + HERO_NAME2;
	else
		return GUI_names_grammar(2, pos, 1);
}

/**
 * \brief   returns name in the 1st german case
 *
 * \param   type        1 = enemy / 2 = hero
 * \param   pos         position
 * \return              "ein Magier" if the enemy is a "Magier".
 */
//static
RealPt FIG_name_1st_case(unsigned short type, volatile unsigned short pos)
{

	if (type == 2)
		return (RealPt)ds_readd(HEROES) + pos * SIZEOF_HERO + HERO_NAME2;
	else
		return GUI_names_grammar(0, pos, 1);
}

#define idx (ds_readw(((FIG_MSG_DTPS-2)) + ds_readw(FIG_MSG_DATA + ds_readbs(FIG_STAR_COUNTER) * 4) * 2))

unsigned short fight_printer(void)
{
	signed short fg_bak;
	signed short bg_bak;
	RealPt gfx_pos_bak;
	Bit16u x;
	char str[6];
	RealPt gfx_dst_bak;

	Bit16s f_action;

	if (ds_readw(FIG_MSG_DATA) == 0)
		ds_writew(FIG_CONTINUE_PRINT, 0);

	if (ds_readw(FIG_STAR_TIMER) == 0 && ds_readb(FIG_STAR_PRINTED) != 0) {
		inc_ds_bs_post(FIG_STAR_COUNTER);
		ds_writeb(FIG_STAR_PRINTED, 0);

		ds_writew(FIG_STAR_TIMER, ds_readw(AUTOFIGHT) ? 10: ds_readws(DELAY_FACTOR) * 6);

		if (ds_readw(FIG_MSG_DATA + ds_readbs(FIG_STAR_COUNTER) * 4) == 0)
			ds_writew(FIG_CONTINUE_PRINT, 0);
	}

	if (ds_readw(FIG_CONTINUE_PRINT) != 0) {
		if (ds_readb(FIG_STAR_COUNTER) != ds_readb(FIG_STAR_LAST_COUNT)) {

		ds_writeb(FIG_STAR_PRINTED, 1);

		f_action = ds_readw(FIG_MSG_DATA + ds_readbs(FIG_STAR_COUNTER) * 4);
		if (f_action != 0) {

			gfx_pos_bak = (RealPt)ds_readd(PRINT_STRING_BUFFER);

			ds_writed(PRINT_STRING_BUFFER, ds_readd(RENDERBUF_PTR));
			get_textcolor(&fg_bak, &bg_bak);

			FIG_set_star_color(Real2Host(ds_readd(FIG_STAR_GFX)),
				3724, ds_readb((FIG_STAR_COLORS-1) + f_action));

			ds_writew(PIC_COPY_X1, 0);
			ds_writew(PIC_COPY_Y1, 150);
			ds_writew(PIC_COPY_X2, 75);
			ds_writew(PIC_COPY_Y2, 198);
			ds_writed(PIC_COPY_SRC, ds_readd(FIG_STAR_GFX));
			ds_writed(PIC_COPY_DST, ds_readd(RENDERBUF_PTR));
			gfx_dst_bak = (RealPt)ds_readd(PIC_COPY_DST);
			do_pic_copy(2);

			ds_writed(PIC_COPY_DST, (Bit32u)gfx_dst_bak);

			/* print number into the star */
			if (ds_readw((FIG_MSG_DATA + 2) + ds_readbs(FIG_STAR_COUNTER) * 4) != 0) {
				set_textcolor(0xff, ds_readbs((FIG_STAR_COLORS-1) + f_action) + 0x80);

				my_itoa(ds_readws((FIG_MSG_DATA + 2) + ds_readbs(FIG_STAR_COUNTER) * 4), str, 10);

				x = GUI_get_first_pos_centered((Bit8u*)str, 30, 20, 0);
				GUI_print_string((Bit8u*)str, x, 170);
			}

			/* Generate textmessage */
			if (ds_readw(((FIG_MSG_DTPS-2)) + f_action * 2) != 0) {
				ds_writew(PIC_COPY_X1, ds_writew(PIC_COPY_V1, 0));
				ds_writew(PIC_COPY_Y1, ds_writew(PIC_COPY_V2, 194));
				ds_writew(PIC_COPY_X2, 318);
				ds_writew(PIC_COPY_Y2, 199);
				ds_writed(PIC_COPY_SRC, ds_readd(BUFFER8_PTR));
				do_pic_copy(3);

				set_textcolor(0xff, 0);

				if (f_action == 1 || f_action == 3) {
//					case 1:	/* heroes attack fails */
//					case 3: /* enemy attack fails */

					sprintf(getString(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_tx(idx),
					getString(FIG_name_3rd_case(ds_readw(FIG_ACTOR_GRAMMAR_TYPE), ds_readw(FIG_ACTOR_GRAMMAR_ID))));
				} else if (f_action == 2 || f_action == 4 || f_action == 7) {
//					case 2: /* hero parade fails */
//					case 4: /* enemy parade fails */
//					case 7:	/* hero get unconscious */

					sprintf(getString(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_tx(idx),
					getString(FIG_name_3rd_case(ds_readw(FIG_TARGET_GRAMMAR_TYPE), ds_readw(FIG_TARGET_GRAMMAR_ID))));


				} else if (f_action == 8 || f_action == 11) {
//					case 8:		/* enemy hits hero */
//					case 11:	/* hero hits enemy */

					sprintf(getString(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)get_tx(idx),
					getString(FIG_name_1st_case(ds_readw(FIG_ACTOR_GRAMMAR_TYPE), ds_readw(FIG_ACTOR_GRAMMAR_ID))),
					getString(FIG_name_4th_case(ds_readw(FIG_TARGET_GRAMMAR_TYPE), ds_readw(FIG_TARGET_GRAMMAR_ID))));
				} else {
					/* case 5: hero successful parade */
					/* case 6: weapon broke */
					strcpy(getString(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_tx(idx));
				}

				GUI_print_string(Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					1, 194);
			}
			ds_writed(PRINT_STRING_BUFFER, (Bit32u)gfx_pos_bak);
			set_textcolor(fg_bak, bg_bak);
		}
		ds_writeb(FIG_STAR_LAST_COUNT, ds_readbs(FIG_STAR_COUNTER));
		return 1;
		} else {
			return 0;
		}
	} else {
		ds_writeb(FIG_STAR_COUNTER, 0);

		ds_writew(FIG_STAR_TIMER, ds_readw(AUTOFIGHT) ? 10 : ds_readw(DELAY_FACTOR) * 6);

		ds_writeb(FIG_STAR_LAST_COUNT, 0xff);

		return 0;
	}

}
#undef idx
struct dummy_w4 {
	signed short a[4];
};

void draw_fight_screen(Bit16u val)
{
	signed short i;
	signed short obj_id;
	signed short width;
	signed short flag;
	signed short current_x1;
	signed short current_y1;
	signed short obj_x;
	signed short obj_y;

	RealPt p_figure_gfx;
	RealPt p_weapon_gfx;
	Bit8u *list_i;

	struct dummy_w4 coord_bak;
	Bit8u *hero;
	Bit8u *p_enemy_sheet;

	signed short viewdir_before;
	signed short viewdir_after;
	signed short target_id;
	signed char twofielded_move_tail_first;
	RealPt fighter_head_ptr;
	signed short viewdir_unconsc;
	Bit8u *sheet;
	Bit8u *p_weapon_anisheet;
	signed short handle;
	struct nvf_desc nvf;
	signed short figlist_remove[8];

	update_mouse_cursor();

	list_i = Real2Host(ds_readd(FIG_LIST_HEAD));

	do {
		/* Check for each list entry if a sprite is needed */

		if (host_readbs(list_i + FIGHTER_RELOAD) == -1) {

			nvf.src = Real2Host(load_fight_figs(host_readw(list_i + FIGHTER_FIGURE)));
			nvf.dst = Real2Host(host_readd(list_i + FIGHTER_GFXBUF));
			nvf.no = host_readbs(list_i + FIGHTER_NVF_NO);
			nvf.type = 0;
			nvf.width = (Bit8u*)&width;
			nvf.height = (Bit8u*)&obj_id;

			process_nvf(&nvf);

			host_writeb(list_i + FIGHTER_RELOAD, 0);
		}

	} while (NOT_NULL(list_i = Real2Host(host_readd(list_i + FIGHTER_NEXT))));

	/* set elements array[0] of array[9] */
	ds_writed(FIG_GFXBUFFERS, ds_readd(FIGHTOBJ_BUF_SEEK_PTR));
	ds_writew(FIG_ANI_STATE, -1);

	for (i = 1; i < 8; i++) {
		/* copy a pointer to the next position */
		ds_writed(FIG_GFXBUFFERS + i * 4, (Bit32u)F_PADD((RealPt)ds_readd((FIG_GFXBUFFERS - 4) + i * 4), 0x508));
		ds_writew(FIG_ANI_STATE + i * 2, -1);
	}

	list_i = Real2Host(ds_readd(FIG_LIST_HEAD));
	flag = 0;

	do {
#if !defined(__BORLANDC__)
		D1_LOG(" loop Figure = %3d Sheet_ID : %d 0xf : %d 0x12: %d object: %d\n",
				host_readw(list_i + FIGHTER_FIGURE),
				host_readbs(list_i + FIGHTER_SHEET), host_readbs(list_i + FIGHTER_WSHEET),
				host_readbs(list_i + FIGHTER_VISIBLE), host_readbs(list_i + FIGHTER_OBJ_ID));
#endif

		if (host_readbs(list_i + FIGHTER_SHEET) != -1) {
			/* Has a sheet id */

			if (host_readb(list_i + FIGHTER_VISIBLE) != 0) {
				host_writeb(list_i + FIGHTER_VISIBLE, 3);
			}

			flag = 1;

			ds_writew(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2, 0);

			memcpy(Real2Host(ds_readd(FIG_GFXBUFFERS + host_readbs(list_i + FIGHTER_SHEET) * 4)),
				Real2Host(host_readd(list_i + FIGHTER_GFXBUF)),
				host_readbs(list_i + FIGHTER_WIDTH) * host_readbs(list_i + FIGHTER_HEIGHT));
		}

		if (host_readbs(list_i + FIGHTER_WSHEET) != -1) {
			memset(Real2Host(ds_readd(FIG_GFXBUFFERS + host_readbs(list_i + FIGHTER_WSHEET) * 4)), 0, 0x508);
		}


	} while (NOT_NULL(list_i = Real2Host(host_readd(list_i + FIGHTER_NEXT))));

	/* write TEMP/XX */
	handle = bc__creat((RealPt)ds_readd(STR_TEMP_XX_PTR), 0);
	bc__write(handle, (RealPt)ds_readd(BUFFER8_PTR), 64000);
	bc_close(handle);

	if (flag != 0) {
		ds_writew(FIGOBJ_UNKN_X2_BAK, ds_writew(FIGOBJ_UNKN_X1_BAK, -1));

		FIG_draw_pic();

		FIG_draw_figures();

		memcpy(Real2Host(ds_readd(BUFFER8_PTR)), Real2Host(ds_readd(RENDERBUF_PTR)), 64000);
	}

	while (flag != 0) {

		set_delay_timer();

		ds_writed(PIC_COPY_DST, ds_writed(PRINT_STRING_BUFFER, ds_readd(RENDERBUF_PTR)));


		for (list_i = Real2Host(ds_readd(FIG_LIST_HEAD)); NOT_NULL(list_i); list_i = Real2Host(host_readd(list_i + FIGHTER_NEXT))) {
			if (host_readb(list_i + FIGHTER_VISIBLE) == 2)
				host_writeb(list_i + FIGHTER_VISIBLE, 1);
		}

		for (i = 0; i < 8; i++) {
			ds_writew(FIG_FIGLIST_READD + i * 2, -1);
			figlist_remove[i] = -1;
		}

		coord_bak = *(struct dummy_w4*)(p_datseg + PIC_COPY_DS_RECT);
		ds_writew(PIC_COPY_X1, ds_writew(PIC_COPY_V1, 0));
		ds_writew(PIC_COPY_Y1, ds_writew(PIC_COPY_V2, 0));
		ds_writew(PIC_COPY_X2, 318);
		ds_writew(PIC_COPY_Y2, 149);
		ds_writed(PIC_COPY_SRC, ds_readd(BUFFER8_PTR));
		do_pic_copy(3);

		ds_writew(PIC_COPY_X1, ds_writew(PIC_COPY_V1, 80));
		ds_writew(PIC_COPY_Y1, ds_writew(PIC_COPY_V2, 150));
		ds_writew(PIC_COPY_X2, 318);
		ds_writew(PIC_COPY_Y2, 193);
		ds_writed(PIC_COPY_SRC, ds_readd(BUFFER8_PTR));
		do_pic_copy(3);

		list_i = Real2Host(ds_readd(FIG_LIST_HEAD));

		do {
			p_weapon_gfx = 0;	/* NULL */

			obj_x = 10 - (host_readbs(list_i + FIGHTER_WIDTH) / 2) +
				(10 * (host_readbs(list_i + FIGHTER_CBX) + host_readbs(list_i + FIGHTER_CBY)));

			obj_y = 118 - host_readbs(list_i + FIGHTER_HEIGHT) + (host_readbs(list_i + FIGHTER_CBX) - host_readbs(list_i + FIGHTER_CBY)) * 5;

			obj_x += host_readbs(list_i + FIGHTER_OFFSETX);
			obj_y += host_readbs(list_i + FIGHTER_OFFSETY);

			p_figure_gfx = (RealPt)host_readd(list_i + FIGHTER_GFXBUF);

			if ((host_readbs(list_i + FIGHTER_SHEET) != -1) &&
				(ds_readws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) != -1)) {

				sheet = p_datseg + FIG_ANISHEETS + host_readbs(list_i + FIGHTER_SHEET) * 0xf3;

				if (host_readbs(sheet + 1 + ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3) == -1) {

					p_figure_gfx = (RealPt)ds_readd(FIG_GFXBUFFERS + host_readbs(list_i + FIGHTER_SHEET) * 4);
					ds_writew(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2, -1);
					host_writeb(list_i + FIGHTER_SHEET, host_writebs(list_i + FIGHTER_WSHEET, -1));

				} else {

					if (host_readbs(sheet + (ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3) + 1) == -7)
					{
						host_writeb(list_i + FIGHTER_Z,
							host_readbs(sheet + 2 + ds_readws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3));

						inc_ds_ws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2);

						ds_writew(FIG_FIGLIST_READD + host_readbs(list_i + FIGHTER_SHEET) * 2,
							host_readbs(list_i + FIGHTER_ID));
					}

					if (host_readbs(sheet + (ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3) + 1) == -9)
					{
						play_voc(0xc8 + host_readbs(sheet + 2 + ds_readws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3));
						inc_ds_ws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2);
					}


					if (host_readbs(sheet + (ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3) + 1) == -5) {

						if (host_readbs(list_i + FIGHTER_WSHEET) != - 1) {

							p_weapon_anisheet = p_datseg + FIG_ANISHEETS + host_readbs(list_i + FIGHTER_WSHEET) * 0xf3;

							if (host_readbs(p_weapon_anisheet + 1 + ds_readw(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3) == -9)
							{
								play_voc(0xc8 + host_readbs(p_weapon_anisheet + 2 + ds_readw(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET)  * 2) * 3));
							}
						}

						inc_ds_ws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2);

					} else if (host_readbs(sheet + (ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3) + 1) == -4) {

						host_writeb(sheet,
							host_readb(sheet + 2 + ds_readw(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3));

						inc_ds_ws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2);

					} else if ((host_readbs(sheet + (ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3) + 1) == -3) ||
							(host_readbs(sheet + (ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3) + 1) == -6)) {

						/* get nvf no */
						viewdir_before = host_readbs(list_i + FIGHTER_NVF_NO);

						if (host_readbs(sheet + 1 + (ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3)) == -3) {

							add_ptr_bs(list_i + FIGHTER_NVF_NO,
								host_readbs(sheet + (ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3) + 3));
						} else {

							host_writews(list_i,

								ds_readbs(GFXTAB_FIGURES_MAIN +  host_readbs(list_i + FIGHTER_SPRITE_NO) * 5 + host_readbs(sheet + 2 + (ds_readws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3))));
							host_writebs(list_i + FIGHTER_NVF_NO,
								host_readbs(sheet + 3 + ds_readws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3));
						}

						if (host_readws(list_i + FIGHTER_FIGURE) >= 88) {
							/* fighter uses figure from MONSTER file */

							if (host_readbs(list_i + FIGHTER_NVF_NO) > 3) {
								/* not standing still */
								host_writeb(list_i + FIGHTER_OFFSETX,
										ds_readbs((GFXTAB_OFFSETS_MAIN + 8) + host_readbs(list_i + FIGHTER_SPRITE_NO) * 10));
								host_writeb(list_i + FIGHTER_OFFSETY,
										ds_readbs((GFXTAB_OFFSETS_MAIN + 9) + host_readbs(list_i + FIGHTER_SPRITE_NO) * 10));

								if (host_readbs(list_i + FIGHTER_TWOFIELDED) != -1) {
									host_writeb(list_i + FIGHTER_X1, ds_readb((GFXTAB_TWOFIELDED_X1 + 1)));
									host_writeb(list_i + FIGHTER_X2, ds_readb((GFXTAB_TWOFIELDED_X2 + 1)));
								}

							} else {
								host_writeb(list_i + FIGHTER_OFFSETX,
										ds_readbs(GFXTAB_OFFSETS_MAIN + host_readbs(list_i + FIGHTER_SPRITE_NO) * 10 + host_readbs(list_i + FIGHTER_NVF_NO) * 2));
								host_writeb(list_i + FIGHTER_OFFSETY,
										ds_readbs((GFXTAB_OFFSETS_MAIN + 1) + host_readbs(list_i + FIGHTER_SPRITE_NO) * 10 + host_readbs(list_i + FIGHTER_NVF_NO) * 2));

								if (host_readbs(list_i + FIGHTER_TWOFIELDED) != -1) {
									host_writeb(list_i + FIGHTER_X1, ds_readb(GFXTAB_TWOFIELDED_X1 + host_readbs(list_i + FIGHTER_NVF_NO)));
									host_writeb(list_i + FIGHTER_X2, ds_readb(GFXTAB_TWOFIELDED_X2 + host_readbs(list_i + FIGHTER_NVF_NO)));
								}
							}

						} else {
							if (host_readbs(list_i + FIGHTER_NVF_NO) == ds_readws(NVFTAB_FIGURES_DEAD + host_readbs(list_i + FIGHTER_SPRITE_NO) * 2)) {
								host_writeb(list_i + FIGHTER_OFFSETX,
										ds_readbs((GFXTAB_OFFSETS_MAIN + 8) + host_readbs(list_i + FIGHTER_SPRITE_NO) * 10));
								host_writeb(list_i + FIGHTER_OFFSETY,
										ds_readbs((GFXTAB_OFFSETS_MAIN + 9) + host_readbs(list_i + FIGHTER_SPRITE_NO) * 10));

							} else {
								viewdir_unconsc = host_readbs(list_i + FIGHTER_NVF_NO) - ds_readws(NVFTAB_FIGURES_UNCONSCIOUS + host_readbs(list_i + FIGHTER_SPRITE_NO) * 2);

								if (viewdir_unconsc >= 0) {
									host_writeb(list_i + FIGHTER_OFFSETX,
										ds_readbs(GFXTAB_OFFSETS_UNCONSCIOUS + host_readbs(list_i + FIGHTER_SPRITE_NO) * 8 + viewdir_unconsc * 2));
									host_writeb(list_i + FIGHTER_OFFSETY,
										ds_readbs((GFXTAB_OFFSETS_UNCONSCIOUS + 1) + host_readbs(list_i + FIGHTER_SPRITE_NO) * 8 + viewdir_unconsc * 2));
								}
							}
						}

						obj_x = 10 - (host_readbs(list_i + FIGHTER_WIDTH) / 2) +
							(10 * (host_readbs(list_i + FIGHTER_CBX) + host_readbs(list_i + FIGHTER_CBY)));

						obj_y = 118 - host_readbs(list_i + FIGHTER_HEIGHT) + ((host_readbs(list_i + FIGHTER_CBX) - host_readbs(list_i + FIGHTER_CBY)) * 5);

						obj_x += host_readbs(list_i + FIGHTER_OFFSETX);
						obj_y += host_readbs(list_i + FIGHTER_OFFSETY);

						if ((host_readbs(list_i + FIGHTER_SHEET) < 6) && (host_readbs(sheet + 0xf2) >= 0)) {
							nvf.src = Real2Host(load_fight_figs(host_readw(list_i + FIGHTER_FIGURE)));
						} else {
							nvf.src =  Real2Host(ds_readd(SPELLOBJ_NVF_BUF));
						}

						nvf.dst = Real2Host(host_readd(list_i + FIGHTER_GFXBUF));
						nvf.no = host_readbs(list_i + FIGHTER_NVF_NO);
						nvf.type = 0;
						nvf.width = (Bit8u*)&width;
						nvf.height = (Bit8u*)&obj_id;

						process_nvf(&nvf);

						inc_ds_ws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2);

						p_figure_gfx = (RealPt)host_readd(list_i + FIGHTER_GFXBUF);

						if (host_readbs(list_i + FIGHTER_TWOFIELDED) > 20) {
							/* list_i is the fighter entry of the tail of a twofielded enemy */

							viewdir_after = (host_readbs(list_i + FIGHTER_NVF_NO) > 3) ? 1 : host_readbs(list_i + FIGHTER_NVF_NO);

							add_ptr_bs(list_i + FIGHTER_OFFSETX, ds_readbs(GFXTAB_TWOFIELDED_EXTRA_OX + viewdir_after));
							add_ptr_bs(list_i + FIGHTER_OFFSETY, ds_readbs(GFXTAB_TWOFIELDED_EXTRA_OY + viewdir_after));
							host_writeb(list_i + FIGHTER_X1, ds_readbs(GFXTAB_TWOFIELDED_EXTRA_X1 + viewdir_after));
							host_writeb(list_i + FIGHTER_X2, ds_readbs(GFXTAB_TWOFIELDED_EXTRA_X2 + viewdir_after));

							obj_id = get_cb_val(host_readbs(list_i + FIGHTER_CBX), host_readbs(list_i + FIGHTER_CBY)); /* enemy_id + 30 of the enemy the tail belongs to */

							FIG_set_cb_field(host_readbs(list_i + FIGHTER_CBY), host_readbs(list_i + FIGHTER_CBX), host_readbs(list_i + FIGHTER_OBJ_ID));

							/* update CBX depending on the view direction */
							if ( ((viewdir_after == 2) && ((viewdir_before == 1) || (viewdir_before == 3))) ||
								(((viewdir_after == 3) || (viewdir_after == 1)) && (viewdir_before == 0)))
							{
								inc_ptr_bs(list_i + FIGHTER_CBX);
							} else if ( ((viewdir_after == 0) && ((viewdir_before == 1) || (viewdir_before == 3))) ||
									(((viewdir_after == 3) || (viewdir_after == 1)) && (viewdir_before == 2)))
							{
									dec_ptr_bs(list_i + FIGHTER_CBX);
							}

							/* update CBY depending on the view direction */
							if ( ((viewdir_after == 3) && ((viewdir_before == 0) || (viewdir_before == 2))) ||
								(((viewdir_after == 0) || (viewdir_after == 2)) && (viewdir_before == 1)))
							{
								dec_ptr_bs(list_i + FIGHTER_CBY);

							} else	if ( ((viewdir_after == 1) && ((viewdir_before == 0) || (viewdir_before == 2))) ||
									(((viewdir_after == 0) || (viewdir_after == 2)) && (viewdir_before == 3)))
							{
								inc_ptr_bs(list_i + FIGHTER_CBY);
							} else if ((viewdir_after == 1) && (viewdir_before == 3))
							{
								host_writeb(list_i + FIGHTER_CBY, host_readbs(list_i + FIGHTER_CBY) + 2);
							}

							target_id = get_cb_val(host_readbs(list_i + FIGHTER_CBX), host_readbs(list_i + FIGHTER_CBY)); /* object id of the square the tail moves to */
							host_writeb(list_i + FIGHTER_OBJ_ID, (signed char)target_id); /* move it to FIGHTER_OBJ_ID */
							FIG_set_cb_field(host_readbs(list_i + FIGHTER_CBY), host_readbs(list_i + FIGHTER_CBX), obj_id); /* set object id of the target square to enemy_id + 30 */

							obj_x = 10 - (host_readbs(list_i + FIGHTER_WIDTH) / 2) +
								(10 * (host_readbs(list_i + FIGHTER_CBX) + host_readbs(list_i + FIGHTER_CBY)));

							obj_y = 118 - host_readbs(list_i + FIGHTER_HEIGHT) + ((host_readbs(list_i + FIGHTER_CBX) - host_readbs(list_i + FIGHTER_CBY)) * 5);

							obj_x += host_readbs(list_i + FIGHTER_OFFSETX);

							obj_y += host_readbs(list_i + FIGHTER_OFFSETY);

						}
					} else {

						/* move a hero/enemy */
						if (host_readbs(sheet + (ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3) + 1) == -2) {

							if (host_readbs(list_i + FIGHTER_SHEET) < 6) {

								obj_id = get_cb_val(host_readbs(list_i + FIGHTER_CBX), host_readbs(list_i + FIGHTER_CBY));

								/* copy FIGHTER_OBJ_ID back to the chessboard */
								FIG_set_cb_field(host_readbs(list_i + FIGHTER_CBY),
									host_readbs(list_i + FIGHTER_CBX),
									host_readbs(list_i + FIGHTER_OBJ_ID));

								host_writeb(list_i + FIGHTER_CBX,	host_readbs(list_i + FIGHTER_CBX) +
									host_readbs(sheet + 2 + ds_readw(FIG_ANI_STATE + (host_readbs(list_i + FIGHTER_SHEET) * 2)) * 3));

								host_writeb(list_i + FIGHTER_CBY,	host_readbs(list_i + FIGHTER_CBY) +
									host_readbs(sheet + 3 + ds_readw(FIG_ANI_STATE + (host_readbs(list_i + FIGHTER_SHEET) * 2)) * 3));

								twofielded_move_tail_first = 0;

								/* get the value from the cb where the actor wants to move to */
								target_id = get_cb_val(host_readbs(list_i + FIGHTER_CBX), host_readbs(list_i + FIGHTER_CBY));

								if ((host_readbs(list_i + FIGHTER_TWOFIELDED) > 20) && (obj_id - 20 == target_id)) {
									/* for a two-fielded enemy, either the head part or the tail part is moved first.
									 * This is the case that the tail part is moved first (the target square is the head part). */

#ifndef M302de_ORIGINAL_BUGFIX
									/* Original-Bug 5: */
									/* the removal of the following line is not strictly necessary, but it is not needed as a replacement is added further below. */
									host_writeb(list_i + FIGHTER_OBJ_ID, 0);
#endif
									twofielded_move_tail_first = 1;

									/* create pointer to the head part of the enemy */
									fighter_head_ptr = FIG_get_ptr(ds_readbs(((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_FIGHTER_ID) + target_id * SIZEOF_ENEMY_SHEET));

#ifdef M302de_ORIGINAL_BUGFIX
									/* Original-Bug 5: */
									/* The FIGHTER_OBJ_ID entry of the head part will be overwritten by the next line in the original code.
									 * In this way, sometimes dead bodies are lost from the chessboard after a two-fielded enemy walks over it.
									 * The right thing is to copy it to the FIGHTER_OBJ_ID of tail part. */
									host_writeb(list_i + FIGHTER_OBJ_ID, (signed char)host_readbs(Real2Host(fighter_head_ptr) + FIGHTER_OBJ_ID));
#endif
									host_writeb(Real2Host(fighter_head_ptr) + FIGHTER_OBJ_ID,  (signed char)obj_id);
									/* write cb_id of the tail part at FIGHTER_OBJ_ID of the head part.
									 * when the head part moves lated, it will be written to the cb.
									 * possible bug: the overwritten FIGHTER_OBJ_ID is lost! */
								} else {
									host_writeb(list_i + FIGHTER_OBJ_ID, (signed char)target_id);
								}

								/* check chessboard bounds */
								if ( (host_readbs(list_i + FIGHTER_CBX) >= 24) || (host_readbs(list_i + FIGHTER_CBY) >= 24)
									|| (host_readbs(list_i + FIGHTER_CBX) < 0) || (host_readbs(list_i + FIGHTER_CBY) < 0)
									|| (host_readbs(list_i + FIGHTER_OBJ_ID) < 0))
								{
									/* hero/enemy escapes */

										if (host_readbs(list_i + FIGHTER_IS_ENEMY) == 1) {
											/* enemy escapes */
											p_enemy_sheet = Real2Host(FIG_get_enemy_sheet(host_readbs(list_i + FIGHTER_ID)));
											if (NOT_NULL(p_enemy_sheet)) {
												or_ptr_bs(p_enemy_sheet + ENEMY_SHEET_STATUS1, 1); /* set 'dead' status bit */
												host_writeb(p_enemy_sheet + ENEMY_SHEET_BP, 0);
												figlist_remove[host_readbs(list_i + FIGHTER_SHEET)] = host_readbs(p_enemy_sheet + ENEMY_SHEET_FIGHTER_ID);

												if (host_readbs(list_i + FIGHTER_TWOFIELDED) != -1) {
#ifdef M302de_ORIGINAL_BUGFIX
													/* Original-Bug 4:
													 * remove tail of the escaped two-fielded enemy from the chessboard
													 * For more on this bug, see Original-Bug 3 at seg032.cpp */
													Bit8u *fighter_tmp = Real2Host(FIG_get_ptr(ds_readbs(FIG_TWOFIELDED_TABLE + host_readbs(list_i + FIGHTER_TWOFIELDED))));
													FIG_set_cb_field(host_readbs(fighter_tmp + FIGHTER_CBY), host_readbs(fighter_tmp + FIGHTER_CBX), host_readbs(fighter_tmp + FIGHTER_OBJ_ID));
#endif
													figlist_remove[2 + host_readbs(list_i + FIGHTER_SHEET)] = ds_readbs(FIG_TWOFIELDED_TABLE + host_readbs(list_i + FIGHTER_TWOFIELDED));
												}
											}
										} else {
											/* hero escapes */
											hero = Real2Host(FIG_get_hero_ptr(host_readbs(list_i + FIGHTER_ID)));
											if (NOT_NULL(hero)) {
												host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_FLEE);
												or_ptr_bs(hero + HERO_STATUS2, 1); /* set 'scared' status bit */

												/* set the dungeon position the hero escapes to.
												 * This depends on the direction the escape square on the battlefield has been entered.
												 * Note: Apparently, this is done in any fight, including seafights and wilderness fights where it doesn't make sense.
												 * The distinction is done only later. */
												host_writew(hero + HERO_ESCAPE_POSITION,
													ds_readws(FIG_FLEE_POSITION + 2 * ((host_readbs(hero + HERO_VIEWDIR) == 3) ? 0 : (host_readbs(hero + HERO_VIEWDIR) + 1))));
												figlist_remove[host_readbs(list_i + FIGHTER_SHEET)] = host_readbs(hero + HERO_FIGHTER_ID);

											}
										}

										host_writebs(list_i + FIGHTER_OBJ_ID, 0);
										host_writebs(sheet + 1 + 3 * (ds_readws(FIG_ANI_STATE + 2 * host_readbs(list_i + FIGHTER_SHEET)) + 1), -1);

										if (host_readbs(list_i + FIGHTER_TWOFIELDED) != -1) {
											ds_writeb((FIG_ANISHEETS + 4 + 2*0xf3) + (host_readbs(list_i + FIGHTER_SHEET) * 0xf3 + ds_readws((FIG_ANI_STATE + 4) + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3), -1);
											ds_writew((FIG_ANI_STATE + 4) + host_readbs(list_i + FIGHTER_SHEET) * 2, -1);
										}

								} else {
									if (!twofielded_move_tail_first) {
										FIG_set_cb_field(host_readbs(list_i + FIGHTER_CBY), host_readbs(list_i + FIGHTER_CBX), obj_id);
									}
								}
							} else {
								host_writeb(list_i + FIGHTER_CBX, host_readbs(list_i + FIGHTER_CBX) +
									host_readbs(sheet + ds_readw(FIG_ANI_STATE + (host_readbs(list_i + FIGHTER_SHEET) * 2)) * 3 + 2));

								host_writeb(list_i + FIGHTER_CBY, host_readbs(list_i + FIGHTER_CBY) +
									host_readbs(sheet + ds_readw(FIG_ANI_STATE + (host_readbs(list_i + FIGHTER_SHEET) * 2)) * 3 + 3));
							}

							ds_writew(FIG_FIGLIST_READD + host_readbs(list_i + FIGHTER_SHEET) * 2,
								host_readbs(list_i + FIGHTER_ID));

							inc_ds_ws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2);
						}

						if (host_readbs(sheet + 1 + (ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3)) == -1) {

							p_figure_gfx = (RealPt)ds_readd(FIG_GFXBUFFERS + host_readbs(list_i + FIGHTER_SHEET) * 4);
							ds_writew(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2, -1);
							host_writeb(list_i + FIGHTER_SHEET, host_writebs(list_i + FIGHTER_WSHEET, -1));

						} else {
							obj_x = 10 - (host_readbs(list_i + FIGHTER_WIDTH) / 2) +
								(10 * (host_readbs(list_i + FIGHTER_CBX) + host_readbs(list_i + FIGHTER_CBY)));

							obj_y = 118 - host_readbs(list_i + FIGHTER_HEIGHT) + ((host_readbs(list_i + FIGHTER_CBX) - host_readbs(list_i + FIGHTER_CBY)) * 5);

							obj_x += host_readbs(list_i + FIGHTER_OFFSETX);

							obj_y += host_readbs(list_i + FIGHTER_OFFSETY);

							obj_x += host_readbs(sheet + 2 + ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3);

							obj_y -= host_readbs(sheet + 3 + ds_readw(FIG_ANI_STATE +  host_readbs(list_i + FIGHTER_SHEET) * 2) * 3);

							i = ds_readbs(GFXTAB_FIGURES_MAIN + (host_readbs(list_i + FIGHTER_SPRITE_NO) * 5) + host_readbs(sheet));

							if ((host_readbs(list_i + FIGHTER_SHEET) < 6) && (host_readbs(sheet + 0xf2) >= 0)) {
								nvf.src = Real2Host(load_fight_figs(i));
							} else {
								nvf.src = Real2Host(ds_readd(SPELLOBJ_NVF_BUF));
							}

							nvf.dst = Real2Host(ds_readd(FIG_GFXBUFFERS + host_readbs(list_i + FIGHTER_SHEET) * 4));
							nvf.no = host_readbs(sheet + 1 + ds_readw(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3);
							nvf.type = 0;
							nvf.width = (Bit8u*)&width;
							nvf.height = (Bit8u*)&obj_id;

							process_nvf(&nvf);

							if (host_readbs(list_i + FIGHTER_WSHEET) != -1) {

								p_weapon_anisheet = p_datseg + FIG_ANISHEETS + host_readbs(list_i + FIGHTER_WSHEET) * 0xf3;

								if (host_readbs(p_weapon_anisheet + 1 + ds_readws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3) == -1)
								{
									host_writeb(list_i + FIGHTER_WSHEET, -1);
								} else {
									current_x1 = obj_x;
									current_y1 = obj_y;

									p_weapon_gfx = (RealPt)ds_readd(FIG_GFXBUFFERS + host_readbs(list_i + FIGHTER_WSHEET) * 4);

									if (host_readbs(p_weapon_anisheet + 1 + 3 * (ds_readws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2))) != -5) {
										nvf.dst = Real2Host(ds_readd(FIG_GFXBUFFERS + host_readbs(list_i + FIGHTER_WSHEET) * 4));
										nvf.src = Real2Host(ds_readd(WEAPONS_NVF_BUF));
										nvf.no = host_readb(p_weapon_anisheet + 1 + ds_readw(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3);
										nvf.type = 0;
										nvf.width = (Bit8u*)&width;
										nvf.height = (Bit8u*)&obj_id;

										process_nvf(&nvf);

										current_x1 += host_readbs(list_i + FIGHTER_WIDTH) - 14;
										current_x1 += host_readbs(p_weapon_anisheet + 2 + ds_readws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3);
										current_y1 -= host_readbs(p_weapon_anisheet + 3 + ds_readws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2) * 3);
									}
								}
							}

							inc_ds_ws(FIG_ANI_STATE + host_readbs(list_i + FIGHTER_SHEET) * 2);
						}

						if (host_readbs(list_i + FIGHTER_SHEET) != -1) {
							p_figure_gfx = (RealPt)ds_readd(FIG_GFXBUFFERS + host_readbs(list_i + FIGHTER_SHEET) * 4);
						}
					}
				}
			}
/* 0x17e5 */

			if (FIG_obj_needs_refresh(list_i, obj_x, obj_y)) {

				if ((host_readbs(list_i + FIGHTER_SHEET) == -1) || (figlist_remove[host_readbs(list_i + FIGHTER_SHEET)] == -1)) {

					if (host_readbs(list_i + FIGHTER_SHEET) != -1) {

						ds_writew(FIGOBJ_UNKN_X1, ds_readw(FIGOBJ_UNKN_X2));
						ds_writew(FIGOBJ_UNKN_Y1, ds_readw(FIGOBJ_UNKN_Y2));

						ds_writew(FIGOBJ_UNKN_X2, obj_x - 8);
						ds_writew(FIGOBJ_UNKN_Y2, obj_y - 8);
					}

					/* set Y1 */
					ds_writew(PIC_COPY_DS_RECT, obj_y + host_readbs(list_i + FIGHTER_Y1));
					if (ds_readws(PIC_COPY_DS_RECT) < 0)
						ds_writew(PIC_COPY_DS_RECT, 0);

					/* set X1 */
					ds_writew((PIC_COPY_DS_RECT + 2), obj_x + host_readbs(list_i + FIGHTER_X1));
					if (ds_readws((PIC_COPY_DS_RECT + 2)) < 0)
						ds_writew((PIC_COPY_DS_RECT + 2), 0);

					/* set Y2 */
					ds_writew((PIC_COPY_DS_RECT + 4), obj_y + host_readbs(list_i + FIGHTER_Y2));
					if (ds_readws((PIC_COPY_DS_RECT + 4)) > 199)
						ds_writew((PIC_COPY_DS_RECT + 4), 199);

					/* set X2 */
					ds_writew((PIC_COPY_DS_RECT + 6), obj_x + host_readbs(list_i + FIGHTER_X2));
					if (ds_readws((PIC_COPY_DS_RECT + 6)) > 318)
						ds_writew((PIC_COPY_DS_RECT + 6), 318);

					ds_writew(PIC_COPY_X1, obj_x);
					ds_writew(PIC_COPY_Y1, obj_y);
					ds_writew(PIC_COPY_X2, obj_x + host_readbs(list_i + FIGHTER_WIDTH) - 1);
					ds_writew(PIC_COPY_Y2, obj_y + host_readbs(list_i + FIGHTER_HEIGHT) - 1);
					ds_writed(PIC_COPY_SRC, (Bit32u)p_figure_gfx);

					do_pic_copy(2);	/* Critical */
				}


				/* NULL check on RealPt */
				if (p_weapon_gfx != 0)  {

					ds_writew(PIC_COPY_X1, current_x1);
					ds_writew(PIC_COPY_Y1, current_y1);
					ds_writew(PIC_COPY_X2, current_x1 + 13);
					ds_writew(PIC_COPY_Y2, current_y1 + 13);

					ds_writed(PIC_COPY_SRC, (Bit32u)p_weapon_gfx);

					do_pic_copy(2);
				}
			}

		} while (NOT_NULL(list_i = Real2Host(host_readd(list_i + FIGHTER_NEXT))));

		*(struct dummy_w4*)(p_datseg + PIC_COPY_DS_RECT) = coord_bak;
		ds_writew(FIGOBJ_UNKN_X2_BAK , ds_readw(FIGOBJ_UNKN_X2));
		ds_writew(FIGOBJ_UNKN_X2, -1);
		ds_writew(FIGOBJ_UNKN_X1_BAK , ds_readw(FIGOBJ_UNKN_X1));
		ds_writew(FIGOBJ_UNKN_X1, -1);
		ds_writew(FIGOBJ_UNKN_Y2_BAK , ds_readw(FIGOBJ_UNKN_Y2));
		ds_writew(FIGOBJ_UNKN_Y2, -1);
		ds_writew(FIGOBJ_UNKN_Y1_BAK , ds_readw(FIGOBJ_UNKN_Y1));
		ds_writew(FIGOBJ_UNKN_Y1, -1);

		for (i = 0; i < 8; i++) {
			if (ds_readws(FIG_FIGLIST_READD + i * 2) != -1) {
				FIG_remove_from_list(ds_readbs(FIG_FIGLIST_READD + i * 2), 1);
				FIG_add_to_list(ds_readbs(FIG_FIGLIST_READD + i * 2));
			}

			if (figlist_remove[i] != -1) {
				FIG_remove_from_list((signed char)figlist_remove[i], 0);
			}
		}

		flag = 0;

		for (i = 0; i < 8; i++) {
			if (ds_readws(FIG_ANI_STATE + i * 2) != -1) {
				flag = 1;
				break;
			}
		}

		ds_writeb(FIG_STAR_LAST_COUNT, -1);

		fight_printer();

		ds_writew(PIC_COPY_X1, 0);
		ds_writew(PIC_COPY_Y1, 0);
		ds_writew(PIC_COPY_X2, 319);
		ds_writew(PIC_COPY_Y2, 199);

		ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));
		ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

		fight_delay();

		do_pic_copy(0);

		ds_writed(PIC_COPY_DST, ds_readd(RENDERBUF_PTR));
	}

	for (list_i = Real2Host(ds_readd(FIG_LIST_HEAD)); NOT_NULL(list_i); list_i = Real2Host(host_readd(list_i + FIGHTER_NEXT))) {
		if (host_readb(list_i + FIGHTER_VISIBLE) != 0)
			host_writeb(list_i + FIGHTER_VISIBLE, 1);
	}

	refresh_screen_size();

	if (val == 0) {

		FIG_draw_pic();
		FIG_draw_figures();

		ds_writew(PIC_COPY_X1, 0);
		ds_writew(PIC_COPY_Y1, 0);
		ds_writew(PIC_COPY_X2, 319);
		ds_writew(PIC_COPY_Y2, 199);

		ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));
		ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

		do_pic_copy(0);

		while (ds_readw(FIG_CONTINUE_PRINT) == 1) {

/* We get in an endless loop hero,
when the Timer IRQ cannot set ds:FIG_CONTINUE_PRINT to 0.
So this call to wait_for_vsync() passes control
to the DOSBox-CPU and may run the timer.
 */
#ifdef M302de_SPEEDFIX
			wait_for_vsync();
#endif
			if (fight_printer()) {
				ds_writew(PIC_COPY_X1, 0);
				ds_writew(PIC_COPY_Y1, 0);
				ds_writew(PIC_COPY_X2, 319);
				ds_writew(PIC_COPY_Y2, 199);

				ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));
				ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

				do_pic_copy(0);
			}
		}

	} else {
		ds_writew(FIG_CONTINUE_PRINT, 0);
	}

	/* read TEMP/XX */
	handle = bc_open((RealPt)ds_readd(STR_TEMP_XX_PTR), 0);
	bc__read(handle, Real2Host(ds_readd(BUFFER8_PTR)), 64000);
	bc_close(handle);

	ds_writed(PIC_COPY_DST, ds_writed(PRINT_STRING_BUFFER, ds_readd(FRAMEBUF_PTR)));
}

//static
void set_delay_timer(void)
{
	/* set delay_timer to delay_factor */
	ds_writew(DELAY_TIMER, ds_readw(DELAY_FACTOR));
}

//static
void fight_delay(void)
{
	seg001_02c4();

	while (ds_readws(DELAY_TIMER) > 0) {
		dec_ds_ws(DELAY_TIMER);
		wait_for_vsync();
	}

}

#if !defined(__BORLANDC__)
}
#endif
