/*
 *	Rewrite of DSA1 v3.02_de functions of seg005 (fight)
 *	Functions rewritten: 7/9
 */
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
