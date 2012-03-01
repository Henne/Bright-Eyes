/*
 *      Rewrite of DSA1 v3.02_de functions of seg006 (Fight)
 *      Functions rewritten 16/16 (complete)
 *
*/

#include <stdlib.h>
#include <string.h>

#include "mem.h"

#include "schick.h"

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg006.h"
#include "seg027.h"
#include "seg096.h"

namespace M302de {

RealPt FIG_get_ptr(signed char v1) {
	RealPt ptr = ds_readd(0xe108);

	while (mem_readb(Real2Phys(ptr + 0x10)) != v1) {
		if (mem_readd(Real2Phys(ptr + 0x1b)) == 0)
			return ds_readd(0xe108);
		ptr = mem_readd(Real2Phys(ptr + 0x1b));
	}
	return ptr;
}

void FIG_draw_figures(void)
{
	RealPt gfx_dst_bak;
	unsigned char screen_mode[8];
	Bit8u *list_i;
	signed short l1, l2;
	signed short l_si, l_di;

	l1 = 10;
	l2 = 118;

	gfx_dst_bak = ds_readd(0xc00d);
	ds_writed(0xc00d, ds_readd(0xd303));

	/* backup a structure */
	memcpy(screen_mode, p_datseg + 0x2990, 8);

	list_i = Real2Host(ds_readd(0xe108));

	do {

		if (host_readb(list_i + 0x12) == 1) {
			l_si = ((signed char)host_readb(list_i + 3)
				+ (signed char)host_readb(list_i + 4)) * 10
				+ l1
				- abs((signed char)host_readb(list_i + 8)) / 2;

			l_di = ((signed char)host_readb(list_i + 3)
				- (signed char)host_readb(list_i + 4)) * 5
				+ l2
				- (signed char)host_readb(list_i + 7);

			l_si += (signed char)host_readb(list_i + 5);
			l_di += (signed char)host_readb(list_i + 6);

			ds_writew(0xc011, l_si);
			ds_writew(0xc013, l_di);
			ds_writew(0xc015, l_si + (signed char)host_readb(list_i + 8) - 1);
			ds_writew(0xc017, l_di + (signed char)host_readb(list_i + 7) - 1);
			/* set gfx_src */
			ds_writed(0xc019, host_readd(list_i + 0x17));

			ds_writew(0x2990,
				l_di + (signed char)host_readb(list_i + 0xa));
			if ((signed short)ds_readw(0x2990) < 0)
				ds_writew(0x2990, 0);

			ds_writew(0x2992,
				l_si + (signed char)host_readb(list_i + 9));
			if ((signed short)ds_readw(0x2992) < 0)
				ds_writew(0x2992, 0);

			ds_writew(0x2994,
				l_di + (signed char)host_readb(list_i + 0xc));
			if ((signed short)ds_readw(0x2994) > 199)
				ds_writew(0x2994, 199);

			ds_writew(0x2996,
				l_si + (signed char)host_readb(list_i + 0xb));
			if ((signed short)ds_readw(0x2996) > 319)
				ds_writew(0x2996, 319);

			do_pic_copy(2);
		}

		list_i = Real2Host(host_readd(list_i + 0x1b));

	} while (list_i != NULL && list_i != MemBase);

	/* restore a structure */
	memcpy(p_datseg + 0x2990, screen_mode, 8);
	ds_writed(0xc00d, gfx_dst_bak);
}

//static
signed char FIG_set_array() {
	char i;

	/* find first element that is zero */
	for (i = 0; ds_readb(0xe089 + i) != 0; i++);
	/* make it 1 */
	ds_writeb(0xe089 + i, 1);
	/* return the number of the index */
	return i;
}

void FIG_set_gfx() {
	RealPt ptr_bak;

	ptr_bak = ds_readd(0xc00d);
	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 199);
	ds_writed(0xc019, ds_readd(0xd303));
	ds_writed(0xc00d, ds_readd(0xd2ff));
	update_mouse_cursor();
	do_pic_copy(0);
	refresh_screen_size();
	ds_writed(0xc00d, ptr_bak);
}

void FIG_call_draw_pic(void)
{
	FIG_draw_pic();
}

void FIG_draw_pic(void)
{
	mem_memcpy(Real2Phys(ds_readd(0xd303)),
		Real2Phys(ds_readd(0xc3a9)), 64000);
	ds_writew(0x26af, 1);

	if (ds_readw(0x26b3))
		FIG_draw_char_pic(0, ds_readw(0x26b3));
	else if (ds_readw(0x26b5))
		FIG_draw_enemy_pic(0, ds_readw(0x26b5));
}

RealPt FIG_get_hero_ptr(unsigned short v1) {
	RealPt heros = ds_readd(HEROS);
	unsigned short i;

	for (i = 0; i <= 6; i++)
		if (host_readb(Real2Host(heros) + i * 0x6da + 0x81) == v1)
			return heros + i * 0x6da;
	return heros;
}

RealPt seg006_033c(short v) {
	unsigned short i;

	for (i = 0; i < 20; i++) {
		if (v == (signed char)ds_readb(0xd371 + i * 62))
			return RealMake(datseg, 0xd34b + i * 62);
	}

	return 0;
}

void FIG_set_0e(signed char id, signed char val) {
	Bit8u *ptr = Real2Host(ds_readd(0xe108));

	while (host_readb(ptr + 0x10) != id) {
		if (host_readd(ptr + 0x1b) == 0)
			return;
		ptr = Real2Host(host_readd(ptr + 0x1b));
	}
	host_writeb(ptr + 0x0e, val);
	host_writeb(ptr + 0x12, 1);
}

/* Used by range attack and spells with more than 1 field distance */
void FIG_reset_12_13(signed char id) {
	Bit8u *ptr1, *ptr2;

	ptr1 = Real2Host(ds_readd(0xe108));

	while (host_readb(ptr1 + 0x10) != id) {
		if (host_readd(ptr1 + 0x1b) == 0)
			return;
		ptr1 = Real2Host(host_readd(ptr1 + 0x1b));
	}
	host_writeb(ptr1 + 0x12, 0);

	if (host_readb(ptr1 + 0x13) == 0xff)
		return;

	ptr2 = Real2Host(ds_readd(0xe108));

	while (ds_readb(0xe35a + host_readb(ptr1 + 0x13)) != host_readb(ptr2 + 0x10)) {
		ptr2 = Real2Host(host_readd(ptr2 + 0x1b));
	}
	host_writeb(ptr2 + 0x12, 0);
}

void FIG_set_12_13(signed char id) {
	Bit8u *ptr1, *ptr2;

	ptr1 = Real2Host(ds_readd(0xe108));

	while (host_readb(ptr1 + 0x10) != id) {
		if (host_readd(ptr1 + 0x1b) == 0)
			return;
		ptr1 = Real2Host(host_readd(ptr1 + 0x1b));
	}
	host_writeb(ptr1 + 0x12, 1);

	if (host_readb(ptr1 + 0x13) == 0xff)
		return;

	ptr2 = Real2Host(ds_readd(0xe108));

	while (ds_readb(0xe35a + host_readb(ptr1 + 0x13)) != host_readb(ptr2 + 0x10)) {
		ptr2 = Real2Host(host_readd(ptr2 + 0x1b));
	}
	host_writeb(ptr2 + 0x12, 1);
}

void FIG_set_0f(signed char id, signed char val) {
	Bit8u *ptr = Real2Host(ds_readd(0xe108));

	while (host_readb(ptr + 0x10) != id) {
		if (host_readd(ptr + 0x1b) == 0)
			return;
		ptr = Real2Host(host_readd(ptr + 0x1b));
	}
	host_writeb(ptr + 0x0f, val);
}

void FIG_remove_from_list(signed char id, signed char v2) {
	PhysPt p = Real2Phys(ds_readd(0xe108));

	/* NULL check */
	if (p == 0)
		return;

	while (mem_readb(p + 0x10) != id) {
		/* if (ptr->next == NULL); */
		if (mem_readd(p + 0x1b) == 0)
			return;
		/* ptr = ptr->next; */
		p = Real2Phys(mem_readd(p + 0x1b));
	}

	if (v2 == 0)
		ds_writeb(0xe089 + id, 0);
	else
		mem_memcpy(PhysMake(datseg, 0xe066), p, 35);

	/* check if p == HEAD */
	if (p == Real2Phys(ds_readd(0xe108))) {
		/* Set HEAD: head = p->next;*/
		ds_writed(0xe108, mem_readd(p + 0x1b));
		if (ds_readd(0xe108) != 0)
			/* head->prev = NULL */
			mem_writed(Real2Phys(ds_readd(0xe108)) + 0x1f, 0);
	} else {
		/* check if p == tail */
		if (mem_readd(p + 0x1b) == 0) {
			/* p->prev->next == NULL */
			mem_writed(Real2Phys(mem_readd(p + 0x1f)) + 0x1b , 0);
		} else {
			/* remove ptr from list
			p->prev->next = p->next;
			p->next->prev = p->prev; */
			mem_writed(Real2Phys(mem_readd(p + 0x1f)) + 0x1b, mem_readd(p + 0x1b));
			mem_writed(Real2Phys(mem_readd(p + 0x1b)) + 0x1f, mem_readd(p + 0x1f));
		}
	}
	memset(MemBase + p, 0, 35);
	mem_writeb(p + 0x10, 0xff);
}

signed char FIG_add_to_list(signed char v) {
	Bit8u *fls;
	RealPt p1;
	RealPt p2;
	signed short x, y;

	p1 = ds_readd(0xe37c);
	x = (signed char)ds_readb(0xe069);
	y = (signed char)ds_readb(0xe06a);

	/* FIG_list_start == NULL */
	if (ds_readd(0xe108) == 0) {

		ds_writed(0xe108, ds_readd(0xe37c));

		fls = Real2Host(ds_readd(0xe108));

		memcpy(fls, MemBase + PhysMake(datseg, 0xe066), 35);

		if (v == -1)
			host_writeb(fls + 0x10,	FIG_set_array());

		host_writed(fls + 0x1f, 0);
		host_writed(fls + 0x1b, 0);

		D1_LOG("\tlist created x = %d, y = %d\n", x, y);

		return host_readb(fls + 0x10);
	}

	while ((signed char)host_readb(Real2Host(p1) + 0x10) != -1) {
		p1 += 35;
	}

	memcpy(Real2Host(p1), MemBase + PhysMake(datseg, 0xe066), 35);

	if (v == -1)
		host_writeb(Real2Host(p1) + 0x10, FIG_set_array());
	else
		host_writeb(Real2Host(p1) + 0x10, v);

	p2 = ds_readd(0xe108);

	if ((signed char)ds_readb(0xe077) != -1)
		for (; (signed char)mem_readb(Real2Phys(p2) + 3) <= x &&
		((signed char)mem_readb(Real2Phys(p2) + 3) != x ||
		(signed char)mem_readb(Real2Phys(p2) + 4) >= y) &&
		((signed char)mem_readb(Real2Phys(p2) + 3) != x ||
		(signed char)mem_readb(Real2Phys(p2) + 4) != y ||
		(signed char)mem_readb(Real2Phys(p2) + 0x11) <= (signed char)ds_readb(0xe077))
				/* p2 = p2->next */
			 ; p2 = host_readd(Real2Host(p2) + 0x1b)) {

			/* p2->next != NULL */
			if (host_readd(Real2Host(p2) + 0x1b) != 0)
				continue;

			/* append to end of the list */

			/* p2->next = p1 */
			host_writed(Real2Host(p2) + 0x1b, p1);
			/* p1->prev = p2 */
			host_writed(Real2Host(p1) + 0x1f, p2);
			/* p1->next = NULL */
			host_writed(Real2Host(p1) + 0x1b, 0);

			D1_LOG("\tlist appended x = %d, y = %d\n", x, y);

			return host_readb(Real2Host(p1) + 0x10);
		}

	/* p1->prev = p2->prev; */
	host_writed(Real2Host(p1) + 0x1f, host_readd(Real2Host(p2) + 0x1f));

	/* if (p2->prev) */
	if (host_readd(Real2Host(p2) + 0x1f) != 0)
		/* p2->prev->next = p1 */
		host_writed(Real2Host(host_readd(Real2Host(p2) + 0x1f)) + 0x1b, p1);
	else
		/* FIG_list_start = p1 */
		ds_writed(0xe108, p1);

	/* p2->prev = p1 */
	host_writed(Real2Host(p2) + 0x1f, p1);
	/* p1->next = p2 */
	host_writed(Real2Host(p1) + 0x1b, p2);

	D1_LOG("\tlist insert x = %d, y = %d\n", x, y);

	return host_readb(Real2Host(p1) + 0x10);
}

/**
	FIG_draw_char_pic - draws the heroes picture to the fight screen
	@pos:		0 upper left / 1 lower left
	@hero_nr:	number of the hero
*/
void FIG_draw_char_pic(unsigned short pos, unsigned short hero_nr) {
	RealPt hero;
	unsigned short fg_bak, bg_bak;

	hero = ds_readd(HEROS) + (hero_nr - 1)  * 0x6da;
	ds_writed(0xc019, hero + 0x2da);

	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(0xff, 0);

	ds_writed(0xc00d, ds_readd(0xd303));
	ds_writed(0xd2fb, ds_readd(0xd303));

	if (pos == 0) {

		do_border(Real2Phys(ds_readd(0xd303)), 1, 9, 34, 42, 29);
		ds_writew(0xc011, 2);
		ds_writew(0xc013, 10);
		ds_writew(0xc015, 33);
		ds_writew(0xc017, 41);
		GUI_print_string(Real2Host(hero) + 0x10, 1, 1);

		draw_bar(0, 0, host_readw(Real2Host(hero) + 0x60),
			host_readw(Real2Host(hero) + 0x5e), 1);

		draw_bar(1, 0, host_readw(Real2Host(hero) + 0x64),
			host_readw(Real2Host(hero) + 0x62), 1);
	} else {
		do_border(Real2Phys(ds_readd(0xd303)), 1, 157, 34, 190, 29);
		ds_writew(0xc011, 2);
		ds_writew(0xc013, 158);
		ds_writew(0xc015, 33);
		ds_writew(0xc017, 189);
		GUI_print_string(Real2Host(hero) + 0x10, 1, 193);
	}

	do_pic_copy(0);
	ds_writed(0xc00d, ds_readd(0xd2ff));
	ds_writed(0xd2fb, ds_readd(0xd2ff));
	set_textcolor(fg_bak, bg_bak);
}

/**
 * FIG_draw_enemy_pic() - draws a picture of the enemy when on turn
 * @loc:	0 - left side, 1 = right side
 * @id:		ID of the enemy
 */
void FIG_draw_enemy_pic(unsigned short loc, unsigned short id)
{
	char nvf[19];
	Bit8u *n = (Bit8u*)nvf;
	RealPt p1;
	unsigned short fg_bak, bg_bak;
	Bit8u *p_enemy;

	p1 = ds_readd(0xc3a9) - 1288;

	p_enemy = p_datseg + 0xd0df + id * 62;

	if ((signed char)ds_readb(0x12c0 + host_readb(p_enemy + 1) * 5) != (signed short)ds_readw(0x4b9e)) {

		/* set src */
		host_writed(n + 4,
			load_fight_figs(ds_readb(0x12c0 + host_readb(p_enemy + 1) * 5)));
		/* set dst */
		host_writed(n + 0, p1);
		/* set nr */
		host_writew(n + 8, 1);
		/* set type */
		host_writeb(n + 10, 0);

		host_writed(n + 11, RealMake(SegValue(ss), reg_sp - 8));
		host_writed(n + 15, RealMake(SegValue(ss), reg_sp - 10));

		process_nvf(n);

		ds_writew(0x4b9e,
			ds_readb(0x12c0 + host_readb(p_enemy + 1) * 5));
	}

	/* save and set text colors */
	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(0xff, 0);

	/* set gfx address */
	ds_writed(0xc00d, ds_readd(0xd303));
	ds_writed(0xd2fb, ds_readd(0xd303));

	if (loc == 0) {
		do_border(Real2Phys(ds_readd(0xd303)), 1, 9, 34, 50, 0x1d);
		ds_writew(0xc011, 2);
		ds_writew(0xc013, 10);
		ds_writew(0xc015, 33);
		ds_writew(0xc017, 49);
		ds_writed(0xc019, p1);
		do_pic_copy(0);
		GUI_print_string(Real2Host(GUI_name_singular(get_monname(host_readb(p_enemy)))), 1, 1);
	} else {
		do_border(Real2Phys(ds_readd(0xd303)), 1, 149, 34, 190, 0x1d);
		ds_writew(0xc011, 2);
		ds_writew(0xc013, 150);
		ds_writew(0xc015, 33);
		ds_writew(0xc017, 189);
		ds_writed(0xc019, p1);
		do_pic_copy(0);
		GUI_print_string(Real2Host(GUI_name_singular(get_monname(host_readb(p_enemy)))), 1, 193);
	}

	ds_writed(0xc00d, ds_readd(0xd2ff));
	ds_writed(0xd2fb, ds_readd(0xd2ff));

	set_textcolor(fg_bak, bg_bak);
}

}
