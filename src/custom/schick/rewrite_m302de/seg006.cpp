/*
 *      Rewrite of DSA1 v3.02_de functions of seg006 (Fight)
 *      Functions rewritten 16/16 (complete)
 *
*/

#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"

#include "seg002.h"
#include "seg004.h"
#include "seg006.h"
#include "seg027.h"
#include "seg096.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
RealPt FIG_get_ptr(signed char v1) {
	RealPt ptr;
	ptr = (RealPt)ds_readd(0xe108);

	while (mem_readb(Real2Phys(ptr + 0x10)) != v1) {
		if (mem_readd(Real2Phys(ptr + 0x1b)) == 0)
			return (RealPt)ds_readd(0xe108);
		ptr = (RealPt)mem_readd(Real2Phys(ptr + 0x1b));
	}
	return ptr;
}

/* Borlandified and identical */
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

/* BC-TODO: calculations are done in another way */
void FIG_draw_figures(void)
{
	signed short l1, l2;
	Bit8u *list_i;
	struct screen_rect screen_mode;
	RealPt gfx_dst_bak;
	signed short l_si, l_di;

	l1 = 10;
	l2 = 118;

	gfx_dst_bak = (RealPt)ds_readd(0xc00d);
	ds_writed(0xc00d, ds_readd(0xd303));

	/* backup a structure */
	screen_mode = *((struct screen_rect*)(p_datseg + 0x2990));

	list_i = Real2Host(ds_readd(0xe108));

	do {

		if (host_readb(list_i + 0x12) == 1) {
			l_si = (host_readbs(list_i + 3)
				+ host_readbs(list_i + 4)) * 10
				+ l1
				- abs(host_readbs(list_i + 8)) / 2;

			l_di = (host_readbs(list_i + 3)
				- host_readbs(list_i + 4)) * 5
				+ l2
				- host_readbs(list_i + 7);

			l_si += host_readbs(list_i + 5);
			l_di += host_readbs(list_i + 6);

			ds_writew(0xc011, l_si);
			ds_writew(0xc013, l_di);
			ds_writew(0xc015, l_si + host_readbs(list_i + 8) - 1);
			ds_writew(0xc017, l_di + host_readbs(list_i + 7) - 1);
			/* set gfx_src */
			ds_writed(0xc019, host_readd(list_i + 0x17));

			ds_writew(0x2990,
				l_di + host_readbs(list_i + 0xa));
			if (ds_readws(0x2990) < 0)
				ds_writew(0x2990, 0);

			ds_writew(0x2992,
				l_si + host_readbs(list_i + 9));
			if (ds_readws(0x2992) < 0)
				ds_writew(0x2992, 0);

			ds_writew(0x2994,
				l_di + host_readbs(list_i + 0xc));
			if (ds_readws(0x2994) > 199)
				ds_writew(0x2994, 199);

			ds_writew(0x2996,
				l_si + host_readbs(list_i + 0xb));
			if (ds_readws(0x2996) > 319)
				ds_writew(0x2996, 319);

			do_pic_copy(2);
		}

		list_i = Real2Host(host_readd(list_i + 0x1b));

	} while (NOT_NULL(list_i));

	/* restore a structure */
	//struct_copy(p_datseg + 0x2990, screen_mode, 8);
	*((struct screen_rect*)(p_datseg + 0x2990)) = screen_mode;
	ds_writed(0xc00d, (Bit32u)gfx_dst_bak);
}

/* Borlandified and identical */
void FIG_set_gfx() {
	RealPt ptr_bak;

	ptr_bak = (RealPt)ds_readd(0xc00d);
	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 319);
	ds_writew(0xc017, 199);
	ds_writed(0xc019, ds_readd(0xd303));
	ds_writed(0xc00d, ds_readd(0xd2ff));
	update_mouse_cursor();
	do_pic_copy(0);
	refresh_screen_size();
	ds_writed(0xc00d, (Bit32u)ptr_bak);
}

/* BC-TODO: functions in this segment are called with far call, not near */
void FIG_call_draw_pic(void)
{
	FIG_draw_pic();
}

/* BC-TODO: functions in this segment are called with far call, not near */
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

/* Borlandified and identical */
RealPt FIG_get_hero_ptr(unsigned short v1) {

	signed short i;

	for (i = 0; i <= 6; i++)
		if (host_readbs(Real2Host((RealPt)ds_readd(HEROS)) + i * 0x6da + 0x81) == v1)
			return (RealPt)ds_readd(HEROS) + i * 0x6da;
	return (RealPt)ds_readd(HEROS);
}

RealPt seg006_033c(short v)
{

	signed short i;

	for (i = 0; i < 20; i++) {
		if (v == ds_readbs(ENEMY_SHEETS + 38 + i * 62))
			return RealMake(datseg, ENEMY_SHEETS + i * 62);
	}

	return 0;
}

/* Borlandified and identical */
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
/* Borlandified and identical */
void FIG_reset_12_13(signed char id) {
	Bit8u *ptr1, *ptr2;

	ptr1 = Real2Host(ds_readd(0xe108));

	while (host_readb(ptr1 + 0x10) != id) {
		if (host_readd(ptr1 + 0x1b) == 0)
			return;
		ptr1 = Real2Host(host_readd(ptr1 + 0x1b));
	}
	host_writeb(ptr1 + 0x12, 0);

	if (host_readbs(ptr1 + 0x13) != -1) {

		ptr2 = Real2Host(ds_readd(0xe108));

		while (ds_readb(0xe35a + host_readbs(ptr1 + 0x13)) != host_readb(ptr2 + 0x10)) {
			ptr2 = Real2Host(host_readd(ptr2 + 0x1b));
		}
		host_writeb(ptr2 + 0x12, 0);
	}
}

/* Borlandified and identical */
void FIG_set_12_13(signed char id) {
	Bit8u *ptr1, *ptr2;

	ptr1 = Real2Host(ds_readd(0xe108));

	while (host_readb(ptr1 + 0x10) != id) {
		if (host_readd(ptr1 + 0x1b) == 0)
			return;
		ptr1 = Real2Host(host_readd(ptr1 + 0x1b));
	}
	host_writeb(ptr1 + 0x12, 1);

	if (host_readbs(ptr1 + 0x13) != -1) {

		ptr2 = Real2Host(ds_readd(0xe108));

		while (ds_readb(0xe35a + host_readbs(ptr1 + 0x13)) != host_readb(ptr2 + 0x10)) {
			ptr2 = Real2Host(host_readd(ptr2 + 0x1b));
		}

		host_writeb(ptr2 + 0x12, 1);
	}
}

/* Borlandified and identical */
void FIG_set_0f(signed char id, signed char val) {
	Bit8u *ptr = Real2Host(ds_readd(0xe108));

	while (host_readb(ptr + 0x10) != id) {
		if (host_readd(ptr + 0x1b) == 0)
			return;
		ptr = Real2Host(host_readd(ptr + 0x1b));
	}
	host_writeb(ptr + 0x0f, val);
}

/* Borlandified and nearly identical, see BC-TODO */
void FIG_remove_from_list(signed char id, signed char v2)
{
	Bit8u* p = Real2Host(ds_readd(0xe108));

	/* NULL check */
	if (!NOT_NULL(p))
		return;

	while (host_readb(p + 0x10) != id) {
		/* if (ptr->next == NULL); */
		if (host_readd(p + 0x1b) == 0)
			return;
		/* ptr = ptr->next; */
		p = Real2Host(host_readd(p + 0x1b));
	}

	if (!v2)
		ds_writeb(0xe089 + id, 0);
	else
		/* BC-TODO */
		struct_copy(p_datseg + 0xe066, p, 35);

	/* check if p == HEAD */
	if (p == Real2Host(ds_readd(0xe108))) {
		/* Set HEAD: head = p->next;*/
		ds_writed(0xe108, host_readd(p + 0x1b));
		if (ds_readd(0xe108) != 0)
			/* head->prev = NULL */
			host_writed(Real2Host(ds_readd(0xe108)) + 0x1f, 0);
	} else {
		/* check if p == tail */
		if (host_readd(p + 0x1b) == 0) {
			/* p->prev->next == NULL */
			host_writed(Real2Host(host_readd(p + 0x1f)) + 0x1b , 0);
		} else {
			/* remove ptr from list
			p->prev->next = p->next;
			p->next->prev = p->prev; */
			host_writed(Real2Host(host_readd(p + 0x1f)) + 0x1b, host_readd(p + 0x1b));
			host_writed(Real2Host(host_readd(p + 0x1b)) + 0x1f, host_readd(p + 0x1f));
		}
	}
	memset(p, 0, 35);
	host_writeb(p + 0x10, 0xff);
}

/* Borlandified and nearly identical, see BC-TODO */
signed char FIG_add_to_list(signed char v) {
	RealPt p1;
	RealPt p2;
	signed short x, y;

	p1 = (RealPt)ds_readd(0xe37c);
	x = (signed char)ds_readb(0xe069);
	y = (signed char)ds_readb(0xe06a);

	/* FIG_list_start == NULL */
	if (ds_readd(0xe108) == 0) {

		ds_writed(0xe108, ds_readd(0xe37c));

		struct_copy(Real2Host(ds_readd(0xe108)), p_datseg + 0xe066, 35);

		if (v == -1)
			host_writeb(Real2Host(ds_readd(0xe108)) + 0x10,
				FIG_set_array());

		host_writed(Real2Host(ds_readd(0xe108)) + 0x1f, 0);
		host_writed(Real2Host(ds_readd(0xe108)) + 0x1b, 0);

#if !defined(__BORLANDC__)
		D1_LOG("\tlist created x = %d, y = %d\n", x, y);
#endif

		return host_readb(Real2Host(ds_readd(0xe108)) + 0x10);
	}

	while ((signed char)host_readb(Real2Host(p1) + 0x10) != -1) {
		p1 += 35;
	}

	/* BC-TODO: */
	struct_copy(Real2Host(p1), p_datseg + 0xe066, 35);

	if (v == -1)
		host_writeb(Real2Host(p1) + 0x10, FIG_set_array());
	else
		host_writeb(Real2Host(p1) + 0x10, v);

	p2 = (RealPt)ds_readd(0xe108);

	if ((signed char)ds_readb(0xe077) != -1)
		for (; (signed char)mem_readb(Real2Phys(p2) + 3) <= x &&
		((signed char)mem_readb(Real2Phys(p2) + 3) != x ||
		(signed char)mem_readb(Real2Phys(p2) + 4) >= y) &&
		((signed char)mem_readb(Real2Phys(p2) + 3) != x ||
		(signed char)mem_readb(Real2Phys(p2) + 4) != y ||
		(signed char)mem_readb(Real2Phys(p2) + 0x11) <= (signed char)ds_readb(0xe077))
			/* p2 = p2->next */
			 ; p2 = (RealPt)host_readd(Real2Host(p2) + 0x1b)) {

			/* p2->next != NULL */
			if (host_readd(Real2Host(p2) + 0x1b) == 0) {

				/* append to end of the list */

				/* p2->next = p1 */
				host_writed(Real2Host(p2) + 0x1b, (Bit32u)p1);
				/* p1->prev = p2 */
				host_writed(Real2Host(p1) + 0x1f, (Bit32u)p2);
				/* p1->next = NULL */
				host_writed(Real2Host(p1) + 0x1b, 0);
#if !defined(__BORLANDC__)
				D1_LOG("\tlist appended x = %d, y = %d\n", x, y);
#endif
				return host_readb(Real2Host(p1) + 0x10);
			}
		}

	/* p1->prev = p2->prev; */
	host_writed(Real2Host(p1) + 0x1f, host_readd(Real2Host(p2) + 0x1f));

	/* if (p2->prev) */
	if (host_readd(Real2Host(p2) + 0x1f) != 0)
		/* p2->prev->next = p1 */
		host_writed(Real2Host(host_readd(Real2Host(p2) + 0x1f)) + 0x1b, (Bit32u)p1);
	else
		/* FIG_list_start = p1 */
		ds_writed(0xe108, (Bit32u)p1);

	/* p2->prev = p1 */
	host_writed(Real2Host(p2) + 0x1f, (Bit32u)p1);
	/* p1->next = p2 */
	host_writed(Real2Host(p1) + 0x1b, (Bit32u)p2);
#if !defined(__BORLANDC__)
	D1_LOG("\tlist insert x = %d, y = %d\n", x, y);
#endif
	return host_readb(Real2Host(p1) + 0x10);
}

/**
	FIG_draw_char_pic - draws the heroes picture to the fight screen
	@pos:		0 upper left / 1 lower left
	@hero_nr:	number of the hero
*/
/* Borlandified and identical */
void FIG_draw_char_pic(unsigned short pos, unsigned short hero_nr) {
	RealPt hero;
	unsigned short fg_bak, bg_bak;

	hero = (RealPt)ds_readd(HEROS) + (hero_nr - 1)  * 0x6da;
	ds_writed(0xc019, (Bit32u)(hero + 0x2da));

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
/* Borlandified and nearly identical, see BC-TODO */
void FIG_draw_enemy_pic(unsigned short loc, unsigned short id)
{
	signed short height_width;
	Bit8u *p_enemy;
	unsigned short fg_bak;
	unsigned short bg_bak;
	RealPt p1;
	struct nvf_desc nvf;

	/* BC-TODO */
	p1 = F_PADD((RealPt)(ds_readd(0xc3a9)), -1288);

	p_enemy = p_datseg + 0xd0df + id * 62;

	if (ds_readbs(0x12c0 + host_readbs(p_enemy + 1) * 5) != ds_readws(0x4b9e)) {

		nvf.src = Real2Host(load_fight_figs(ds_readbs(0x12c0 + host_readbs(p_enemy + 1) * 5)));
		nvf.dst = Real2Host(p1);
		nvf.nr = 1;
		nvf.type = 0;
		nvf.width = (Bit8u*)&height_width;
		nvf.height = (Bit8u*)&height_width;

		process_nvf(&nvf);

		ds_writew(0x4b9e,
			ds_readbs(0x12c0 + host_readbs(p_enemy + 1) * 5));
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
		ds_writed(0xc019, (Bit32u)p1);
		do_pic_copy(0);
		GUI_print_string(Real2Host(GUI_name_singular(get_monname(host_readbs(p_enemy)))), 1, 1);
	} else {
		do_border(Real2Phys(ds_readd(0xd303)), 1, 149, 34, 190, 0x1d);
		ds_writew(0xc011, 2);
		ds_writew(0xc013, 150);
		ds_writew(0xc015, 33);
		ds_writew(0xc017, 189);
		ds_writed(0xc019, (Bit32u)p1);
		do_pic_copy(0);
		GUI_print_string(Real2Host(GUI_name_singular(get_monname(host_readbs(p_enemy)))), 1, 193);
	}

	ds_writed(0xc00d, ds_readd(0xd2ff));
	ds_writed(0xd2fb, ds_readd(0xd2ff));

	set_textcolor(fg_bak, bg_bak);
}

#if !defined(__BORLANDC__)
}
#endif
