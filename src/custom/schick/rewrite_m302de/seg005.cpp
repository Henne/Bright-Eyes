/*
 *	Rewrite of DSA1 v3.02_de functions of seg005 (fight)
 *	Functions rewritten: 8/9
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(__BORLANDC__)
#include "callback.h"
#endif

#include "v302de.h"

#include "seg001.h"
#include "seg004.h"
#include "seg096.h"


#if !defined(__BORLANDC__)
namespace M302de {
#endif


/**
 *
 *	@p:	pointer to an object 35 byte
 *	@x:	x coordinate on the screen
 *	@y:	y coordinate on the screen
 *
*/
/* Borlandified and identical */
unsigned short FIG_obj_needs_refresh(Bit8u *p, signed short x, signed short y)
{
	Bit8u *i;
	signed short ox;
	signed short oy;

	if (host_readb(p + 0x12) != 0) {

		if ((host_readbs(p + 0xe) != -1) || (host_readbs(p + 0x12) == 3))
			goto damn_label;

		/* i = i->next; */
		for (i = Real2Host(ds_readd(0xe108)); i != p; i = Real2Host(host_readd(i + 0x1b)))
		{

			if (host_readbs(i + 0x12) >= 2) {

				ox = 10 - host_readbs(i + 8) / 2
					+ (host_readbs(i + 3) + host_readbs(i + 4)) * 10;

				oy = 118 - host_readbs(i + 7)
					+ (host_readbs(i + 3) - host_readbs(i + 4)) * 5;

				ox += host_readbs(i + 5);
				oy += host_readbs(i + 6);

				if (!((x + host_readbs(p + 8) < ox)
					|| (x - host_readbs(i + 8) > ox)
					|| (y + host_readbs(p + 7) < oy)
					|| (y - host_readbs(i + 7) > oy)))
				{
damn_label:
					if (host_readb(p + 0x12) == 1)
						host_writeb(p + 0x12, 2);
					return 1;
				}
			}
		}
	}

	return 0;
}

/**
 *	FIG_set_star_color() - set the color of the star in fights
 *	@ptr:	pointer to the star template
 *	@count: number of bytes the star has
 *	@color: 1=red/2=green/3=blue/4=yellow/11=darkbrown/12=lightbrown/13=pink
 *
 *	Sets the color of the star which shows fight activities,
 *	like damage, in fights.
 */
/* Borlandified and identical */
void FIG_set_star_color(PhysPt ptr, unsigned short count, unsigned char color)
{
	PhysPt p;

	color += 0x80;

	for (p = ptr; count--; p++) {
		if (mem_readb(p))
			mem_writeb(p, color);
	}
}

/*
 *	FIG_name_3rd_case - returns name in the 3rd german case
 *	@type:	1 = enemy / 2 = hero
 *	@pos:	position
 *
 *	Would return "einem Magier" if the enemy is a "Magier".
*/
/* Borlandified and identical */
//static
RealPt FIG_name_3rd_case(unsigned short type, volatile unsigned short pos)
{
	if (type == 2) {
		return (RealPt)ds_readd(HEROS) + pos * 0x6da + 0x10;
	} else {
		return GUI_names_grammar(3, pos, 1);
	}
}

/*
 *	FIG_name_4th_case - returns name in the 4th german case
 *	@type:	1 = enemy / 2 = hero
 *	@pos:	position
 *
 *	Would return "einen Magier" if the enemy is a "Magier".
*/
/* Borlandified and identical */
//static
RealPt FIG_name_4th_case(unsigned short type, volatile unsigned short pos)
{

	if (type == 2)
		return (RealPt)ds_readd(HEROS) + pos * 0x6da + 0x10;
	else
		return GUI_names_grammar(2, pos, 1);
}

/*
 *	FIG_name_1st_case - returns name in the 1st german case
 *	@type:	1 = enemy / 2 = hero
 *	@pos:	position
 *
 *	Would return "ein Magier" if the enemy is a "Magier".
*/
/* Borlandified and identical */
//static
RealPt FIG_name_1st_case(unsigned short type, volatile unsigned short pos)
 {

	if (type == 2)
		return (RealPt)ds_readd(HEROS) + pos * 0x6da + 0x10;
	else
		return GUI_names_grammar(0, pos, 1);
}

#define idx (ds_readw(0x4b7a + ds_readw(0xd333 + ds_readbs(0x4b78) * 4) * 2))

/* Borlandified and identical */
unsigned short fight_printer(void)
{
	unsigned short fg_bak;
	unsigned short bg_bak;
	RealPt gfx_pos_bak;
	Bit16u x;
	char str[6];
	RealPt gfx_dst_bak;

	Bit16s f_action;

	if (ds_readw(0xd333) == 0)
		ds_writew(0x26b1, 0);

	if (ds_readw(0x4b79) == 0 && ds_readb(0x4b94) != 0) {
		inc_ds_bs(0x4b78);
		ds_writeb(0x4b94, 0);

		ds_writew(0x4b79, ds_readw(AUTOFIGHT) ? 10: ds_readws(DELAY_FACTOR) * 6);

		if (ds_readw(0xd333 + ds_readbs(0x4b78) * 4) == 0)
			ds_writew(0x26b1, 0);
	}

	if (ds_readw(0x26b1) != 0) {
		if (ds_readb(0x4b78) != ds_readb(0x4b7b)) {

		ds_writeb(0x4b94, 1);

		f_action = ds_readw(0xd333 + ds_readbs(0x4b78) * 4);
		if (f_action != 0) {

			gfx_pos_bak = (RealPt)ds_readd(0xd2fb);

			ds_writed(0xd2fb, ds_readd(0xd303));
			get_textcolor(&fg_bak, &bg_bak);

			FIG_set_star_color(Real2Phys(ds_readd(0xd29d)),
				3724, ds_readb(0x4b6b + f_action));

			ds_writew(0xc011, 0);
			ds_writew(0xc013, 150);
			ds_writew(0xc015, 75);
			ds_writew(0xc017, 198);
			ds_writed(0xc019, ds_readd(0xd29d));
			ds_writed(0xc00d, ds_readd(0xd303));
			gfx_dst_bak = (RealPt)ds_readd(0xc00d);
			do_pic_copy(2);

			ds_writed(0xc00d, (Bit32u)gfx_dst_bak);

			/* print number into the star */
			if (ds_readw(0xd335 + ds_readbs(0x4b78) * 4) != 0) {
				set_textcolor(0xff, ds_readbs(0x4b6b + f_action) + 0x80);
#if !defined(__BORLANDC__)
				sprintf(str, "%d", ds_readws(0xd335 + ds_readbs(0x4b78) * 4));
#else
				itoa(ds_readws(0xd335 + ds_readbs(0x4b78) * 4), str, 10);
#endif
				x = GUI_get_first_pos_centered((Bit8u*)str, 30, 20, 0);
				GUI_print_string((Bit8u*)str, x, 170);
			}

			/* Generate textmessage */
			if (ds_readw(0x4b7a + f_action * 2) != 0) {
				ds_writew(0xc011, ds_writew(0xc01d, 0));
				ds_writew(0xc013, ds_writew(0xc01f, 194));
				ds_writew(0xc015, 318);
				ds_writew(0xc017, 199);
				ds_writed(0xc019, ds_readd(0xc3a9));
				do_pic_copy(3);

				set_textcolor(0xff, 0);

				if (f_action == 1 || f_action == 3) {
//					case 1:	/* heros attack fails */
//					case 3: /* enemy attack fails */

					sprintf(getString(ds_readd(0xd2eb)),
						(char*)get_dtp(idx * 4),
					getString(FIG_name_3rd_case(ds_readw(0xe2b8), ds_readw(0xe2ba))));
				} else if (f_action == 2 || f_action == 4 || f_action == 7) {
//					case 2: /* hero parade fails */
//					case 4: /* enemy parade fails */
//					case 7:	/* hero get unconscious */

					sprintf(getString(ds_readd(0xd2eb)),
						(char*)get_dtp(idx * 4),
					getString(FIG_name_3rd_case(ds_readw(0xe2bc), ds_readw(0xe2be))));


				} else if (f_action == 8 || f_action == 11) {
//					case 8:		/* enemy hits hero */
//					case 11:	/* hero hits enemy */

					sprintf(getString(ds_readd(0xd2eb)),
						(char*)get_dtp(idx * 4),
					getString(FIG_name_1st_case(ds_readw(0xe2b8), ds_readw(0xe2ba))),
					getString(FIG_name_4th_case(ds_readw(0xe2bc), ds_readw(0xe2be))));
				} else {
					/* case 5: hero successful parade */
					/* case 6: weapon broke */
					strcpy(getString(ds_readd(0xd2eb)), (char*)get_dtp(idx * 4));
				}

				GUI_print_string(Real2Host(ds_readd(0xd2eb)),
					1, 194);
			}
			ds_writed(0xd2fb, (Bit32u)gfx_pos_bak);
			set_textcolor(fg_bak, bg_bak);
		}
		ds_writeb(0x4b7b, ds_readbs(0x4b78));
		return 1;
		} else {
			return 0;
		}
	} else {
		ds_writeb(0x4b78, 0);

		ds_writew(0x4b79, ds_readw(AUTOFIGHT) ? 10 : ds_readw(DELAY_FACTOR) * 6);

		ds_writeb(0x4b7b, 0xff);

		return 0;
	}

}
#undef idx

#if !defined(__BORLANDC__)
/* TODO This callback does not work */
void seg005_0598(Bit16u val)
{
	CPU_Push16(val);
	CALLBACK_RunRealFar(reloc_game + 0xc85, 0x598);
	CPU_Pop16();
}
#endif

//static
void set_delay_timer() {
	/* set delay_timer to delay_factor */
	ds_writew(DELAY_TIMER, ds_readw(DELAY_FACTOR));
}
//static
void fight_delay()
{
	seg001_02c4();

	while (ds_readw(DELAY_TIMER) > 0) {
		ds_writew(DELAY_TIMER, ds_readw(DELAY_TIMER) - 1);
		wait_for_vsync();
	}

}

#if !defined(__BORLANDC__)
}
#endif
