/*
 *	Rewrite of DSA1 v3.02_de functions of seg005 (fight)
 *	Functions rewritten: 8/9
 */
#include <stdlib.h>
#include <string.h>

#include "callback.h"

#include "schick.h"

#include "v302de.h"

#include "seg001.h"
#include "seg004.h"
#include "seg096.h"

namespace M302de {
/**
 *
 *	@p:	pointer to an object 35 byte
 *	@x:	x coordinate on the screen
 *	@y:	y coordinate on the screen
 *
*/
unsigned short FIG_obj_needs_refresh(Bit8u *p, signed short x, signed short y) {

	Bit8u *i;

	if (host_readb(p + 0x12) == 0)
		return 0;

	if (host_readb(p + 0xe) != 0xff || host_readb(p + 0x12) == 3) {

		if (host_readb(p + 0x12) == 1)
			host_writeb(p + 0x12, 2);

		return 1;
	}

	i = MemBase + Real2Phys(ds_readd(0xe108));

	/* i = i->next; */
	for (; i != p; i = MemBase + Real2Phys(host_readd(i + 0x1b))) {

		if (host_readb(i + 0x12) < 2)
			continue;

		signed short i3, i4, i5, i6, i7, i8;
		signed short osx, osy;

		i3 = (signed char)host_readb(i + 3);
		i4 = (signed char)host_readb(i + 4);
		i5 = (signed char)host_readb(i + 5);
		i6 = (signed char)host_readb(i + 6);
		i7 = (signed char)host_readb(i + 7);
		i8 = (signed char)host_readb(i + 8);

		osx = 10 - i8 / 2 + (i3 + i4) * 10 + i5;
		osy = 118 - i7 + (i3 - i4) * 5 + i6;

		D1_LOG("3=%d 4=%d 5=%d 6=%d 7=%d 8=%d osx=%d osy=%d\n",
			i3, i4, i5, i6, i7, i8, osx, osy);

		if (x + i8 < osx || x - i8 > osx)
			continue;

		if (y + i7 < osy || y - i7 > osy)
			continue;

		if (host_readb(p + 0x12) == 1)
			host_writeb(p + 0x12, 2);

		return 1;
	}

	return 0;
}

/**
 *	FIG_set_star_color - set the color of the star in fights
 *	@ptr:	pointer to the star template
 *	@count: number of bytes the star has
 *	@color: 1=red/2=green/3=blue/4=yellow/11=darkbrown/12=lightbrown/13=pink
 *
 *	Sets the color of the star which shows fight activities,
 *	like damage, in fights.
 */
void FIG_set_star_color(PhysPt ptr, unsigned short count, unsigned char color) {

	color += 0x80;
	for (; --count; ptr++) {
		if (mem_readb(ptr))
			mem_writeb(ptr, color);
	}
}

/*
 *	FIG_name_3rd_case - returns name in the 3rd german case
 *	@type:	1 = enemy / 2 = hero
 *	@pos:	position
 *
 *	Would return "einem Magier" if the enemy is a "Magier".
*/
//static
RealPt FIG_name_3rd_case(unsigned short type, unsigned short pos) {

	if (type == 2)
		return ds_readd(HEROS) + pos * 0x6da + 0x10;
	else
		return GUI_names_grammar(3, pos, 1);
}

/*
 *	FIG_name_4th_case - returns name in the 4th german case
 *	@type:	1 = enemy / 2 = hero
 *	@pos:	position
 *
 *	Would return "einen Magier" if the enemy is a "Magier".
*/
//static
RealPt FIG_name_4th_case(unsigned short type, unsigned short pos) {

	if (type == 2)
		return ds_readd(HEROS) + pos * 0x6da + 0x10;
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
//static
RealPt FIG_name_1st_case(unsigned short type, unsigned short pos) {

	if (type == 2)
		return ds_readd(HEROS) + pos * 0x6da + 0x10;
	else
		return GUI_names_grammar(0, pos, 1);
}
unsigned short fight_printer()
{
	char str[6];
	RealPt gfx_pos_bak;
	RealPt gfx_dst_bak;
	Bit16s fg_bak, bg_bak;
	Bit16s f_action;
	Bit16u x;

	if (ds_readw(0xd333) == 0)
		ds_writew(0x26b1, 1);

	if (ds_readw(0x4b79) == 0 && ds_readb(0x4b94) != 0) {
		ds_writeb(0x4b78, ds_readb(0x4b78) + 1);
		ds_writeb(0x4b94, 0);

		if (ds_readw(0xe318))
			ds_writew(0x4b79, 10);
		else
			ds_writew(0x4b79, ds_readw(0x4b66) * 6);

		if (ds_readw(0xd333 + ds_readb(0x4b78) * 4) == 0)
			ds_writew(0x26b1, 0);
	}

	if (ds_readw(0x26b1) != 0) {
		if (ds_readb(0x4b78) == ds_readb(0x4b7b))
			return 0;

		ds_writeb(0x4b94, 1);

		f_action = ds_readw(0xd333 + ds_readb(0x4b78) * 4);
		if (f_action != 0) {

			gfx_pos_bak = ds_readd(0xd2fb);

			ds_writed(0xd2fb, ds_readd(0xd303));
			GUI_get_smth(&fg_bak, &bg_bak);

			FIG_set_star_color(Real2Phys(ds_readd(0xd29d)),
				3724, ds_readb(0x4b6b + f_action));

			ds_writew(0xc011, 0);
			ds_writew(0xc013, 150);
			ds_writew(0xc015, 75);
			ds_writew(0xc017, 198);
			ds_writed(0xc019, ds_readd(0xd29d));
			ds_writed(0xc00d, ds_readd(0xd303));
			gfx_dst_bak = ds_readd(0xc00d);
			do_pic_copy(2);

			ds_writed(0xc00d, gfx_dst_bak);

			/* print number into the star */
			if (ds_readw(0xd333 + 2 + ds_readb(0x4b78) * 4)) {
				GUI_set_smth(0xff, ds_readb(0x4b6b + f_action) + 0x80);
				sprintf(str, "%d", (signed short)
					ds_readw(0xd333 + 2 + ds_readb(0x4b78) * 4));
				x = GUI_get_first_pos_centered((Bit8u*)str, 30, 20, 0);
				GUI_print_string((Bit8u*)str, x, 170);
			}

			/* Generate textmessage */
			if (ds_readw(0x4b7a + f_action * 2) != 0) {
				ds_writew(0xc01d, 0);
				ds_writew(0xc011, 0);
				ds_writew(0xc01f, 194);
				ds_writew(0xc013, 194);
				ds_writew(0xc015, 318);
				ds_writew(0xc017, 199);
				ds_writed(0xc019, ds_readd(0xc3a9));
				do_pic_copy(3);

				GUI_set_smth(0xff, 0);

				switch (f_action) {
				case 1:	/* heros attack fails */
				case 3: /* enemy attack fails */
				{
					Bit16u idx;

					idx = ds_readw(0x4b7a + ds_readw(0xd333 + ds_readb(0x4b78) * 4) * 2);

					sprintf(getString(ds_readd(0xd2eb)),
						(char*)get_dtp(idx * 4),
					getString(FIG_name_3rd_case(ds_readw(0xe2b8), ds_readw(0xe2ba))));
					break;
				}
				case 2: /* hero parade fails */
				case 4: /* enemy parade fails */
				case 7:	/* hero get unconscious */
				{
					Bit16u idx;

					idx = ds_readw(0x4b7a + ds_readw(0xd333 + ds_readb(0x4b78) * 4) * 2);

					sprintf(getString(ds_readd(0xd2eb)),
						(char*)get_dtp(idx * 4),
					getString(FIG_name_3rd_case(ds_readw(0xe2bc), ds_readw(0xe2be))));


					break;
				}
				case 8:		/* enemy hits hero */
				case 11:	/* hero hits enemy */
				{
					Bit16u idx;

					idx = ds_readw(0x4b7a + ds_readw(0xd333 + ds_readb(0x4b78) * 4) * 2);

					sprintf(getString(ds_readd(0xd2eb)),
						(char*)get_dtp(idx * 4),
					getString(FIG_name_1st_case(ds_readw(0xe2b8), ds_readw(0xe2ba))),
					getString(FIG_name_4th_case(ds_readw(0xe2bc), ds_readw(0xe2be))));
					break;
				}
				default: {
					/* case 5: hero successful parade */
					/* case 6: weapon broke */
					Bit16u idx;

					idx = ds_readw(0x4b7a + ds_readw(0xd333 + ds_readb(0x4b78) * 4) * 2);
					strcpy(getString(ds_readd(0xd2eb)), (char*)get_dtp(idx * 4));

					if (f_action != 5 && f_action != 6) {
					D1_INFO("f_action = %d\n", f_action);
					D1_INFO("%s\n", getString(ds_readd(0xd2eb)));
					}
				}
				}
				GUI_print_string(Real2Host(ds_readd(0xd2eb)),
					1, 194);
			}
			ds_writed(0xd2fb, gfx_pos_bak);
			GUI_set_smth(fg_bak, bg_bak);
		}
		ds_writeb(0x4b7b, ds_readb(0x4b78));
		return 1;
	} else {
		ds_writeb(0x4b78, 0);

		if (ds_readw(0xe318))
			ds_writew(0x4b79, 10);
		else
			ds_writew(0x4b79, ds_readw(0x4b66) * 6);

		ds_writeb(0x4b7b, 0xff);

		return 0;
	}

}

/* TODO This callback does not work */
void seg005_0598(Bit16u val)
{
	CPU_Push16(val);
	CALLBACK_RunRealFar(reloc_game + 0xc85, 0x598);
	CPU_Pop16();
}

//static
void set_delay_timer() {
	/* set delay_timer to delay_factor */
	ds_writew(0xe2d0, ds_readw(0x4b66));
}
//static
void fight_delay()
{
	seg001_02c4();

	while (ds_readw(0xe2d0) > 0) {
		ds_writew(0xe2d0, ds_readw(0xe2d0) - 1);
		wait_for_vsync();
	}

}

}
