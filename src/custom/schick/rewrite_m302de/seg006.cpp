/*
 *      Rewrite of DSA1 v3.02_de functions of seg006 (Fight)
 *      Functions rewritten: 16/16 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg006.cpp
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

/**
 * \brief	search in a DL-list for an element
 * \param	fight_id	fight_id
 * \return	a pointer to the list elemtent or to the list head in error case
 */
RealPt FIG_get_ptr(signed char fight_id)
{
	RealPt ptr = (RealPt)ds_readd(FIG_LIST_HEAD);

	while (host_readbs(Real2Host(ptr) + 0x10) != fight_id) {

		/* check if its the last element */
		if (host_readd(Real2Host(ptr) + 0x1b) == 0) {
			/* return list head */
			return (RealPt)ds_readd(FIG_LIST_HEAD);
		}

		/* set ptr to the next element */
		ptr = (RealPt)host_readd(Real2Host(ptr) + 0x1b);
	}

	return ptr;
}

/* static */
signed char FIG_set_array(void)
{
	signed char i = 0;

	/* find first element that is zero */
	while (ds_readb(0xe089 + i) != 0) {

		i++;
	}

	/* make it 1 */
	ds_writeb(0xe089 + i, 1);

	/* return the number of the index */
	return i;
}

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

	list_i = Real2Host(ds_readd(FIG_LIST_HEAD));

	do {

		if (host_readb(list_i + 0x12) == 1) {
			l_si =	(l1 - host_readbs(list_i + 8) / 2) +
				(host_readbs(list_i + 3) + host_readbs(list_i + 4)) * 10;

			l_di =	(l2 - host_readbs(list_i + 7)) +
				(host_readbs(list_i + 3) - host_readbs(list_i + 4)) * 5;

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

	} while (NOT_NULL(list_i = (Bit8u*)Real2Host((RealPt)host_readd(list_i + 0x1b))));

	/* restore a structure */
	//struct_copy(p_datseg + 0x2990, screen_mode, 8);
	*((struct screen_rect*)(p_datseg + 0x2990)) = screen_mode;
	ds_writed(0xc00d, (Bit32u)gfx_dst_bak);
}

void FIG_set_gfx(void)
{
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

void FIG_call_draw_pic(void)
{
	FIG_draw_pic();
}

void FIG_draw_pic(void)
{
	mem_memcpy(Real2Phys(ds_readd(0xd303)),
		Real2Phys(ds_readd(0xc3a9)), 64000);

	ds_writew(0x26af, 1);

	if (ds_readw(0x26b3)) {
		FIG_draw_char_pic(0, ds_readw(0x26b3));
	} else if (ds_readw(0x26b5)) {
		FIG_draw_enemy_pic(0, ds_readw(0x26b5));
	}
}

RealPt FIG_get_hero_ptr(signed short v1)
{
	signed short i;

	for (i = 0; i <= 6; i++) {
		if (host_readbs(Real2Host(ds_readd(HEROS)) + i * 0x6da + 0x81) == v1)
			return (RealPt)ds_readd(HEROS) + i * 0x6da;
	}

	return (RealPt)ds_readd(HEROS);
}

RealPt seg006_033c(signed short v)
{
	signed short i;

	for (i = 0; i < 20; i++) {
		if (v == ds_readbs(ENEMY_SHEETS + 38 + (i * 62)))
#if !defined(__BORLANDC__)
			return (RealPt)RealMake(datseg, ENEMY_SHEETS + i * 62);
#else
			return (RealPt)&(((struct enemy_sheets*)(p_datseg + ENEMY_SHEETS))[i]);
#endif
	}

	return 0;
}

void FIG_set_0e(signed char fight_id, signed char val)
{
	Bit8u *ptr = Real2Host(ds_readd(FIG_LIST_HEAD));

	while (host_readbs(ptr + 0x10) != fight_id) {

		/* check for end of list */
		if (host_readd(ptr + 0x1b) == 0) {
			return;
		}

		/* set ptr to ptr->next */
		ptr = Real2Host(host_readd(ptr + 0x1b));
	}

	host_writeb(ptr + 0x0e, val);

	/* set presence flag */
	host_writeb(ptr + 0x12, 1);
}

/* Used by range attack and spells with more than 1 field distance */
void FIG_reset_12_13(signed char fight_id)
{
	Bit8u *ptr1, *ptr2;

	ptr1 = Real2Host(ds_readd(FIG_LIST_HEAD));

	while (host_readb(ptr1 + 0x10) != fight_id) {

		if (host_readd(ptr1 + 0x1b) == 0) {
			return;
		}

		ptr1 = Real2Host(host_readd(ptr1 + 0x1b));
	}

	host_writeb(ptr1 + 0x12, 0);

	if (host_readbs(ptr1 + 0x13) != -1) {

		ptr2 = Real2Host(ds_readd(FIG_LIST_HEAD));

		while (ds_readb(0xe35a + host_readbs(ptr1 + 0x13)) != host_readb(ptr2 + 0x10)) {
			ptr2 = Real2Host(host_readd(ptr2 + 0x1b));
		}
		host_writeb(ptr2 + 0x12, 0);
	}
}

void FIG_set_12_13(signed short fight_id)
{
	Bit8u *ptr1, *ptr2;

	ptr1 = Real2Host(ds_readd(FIG_LIST_HEAD));

	while (host_readb(ptr1 + 0x10) != (signed char)fight_id) {

		if (host_readd(ptr1 + 0x1b) == 0){
			return;
		}

		ptr1 = Real2Host(host_readd(ptr1 + 0x1b));
	}

	host_writeb(ptr1 + 0x12, 1);

	if (host_readbs(ptr1 + 0x13) != -1) {

		ptr2 = Real2Host(ds_readd(FIG_LIST_HEAD));

		while (ds_readb(0xe35a + host_readbs(ptr1 + 0x13)) != host_readb(ptr2 + 0x10)) {

			ptr2 = Real2Host(host_readd(ptr2 + 0x1b));

		}

		host_writeb(ptr2 + 0x12, 1);
	}
}

void FIG_set_0f(signed char fight_id, signed char val)
{
	Bit8u *ptr = Real2Host(ds_readd(FIG_LIST_HEAD));

	while (host_readb(ptr + 0x10) != fight_id) {

		if (host_readd(ptr + 0x1b) == 0) {
			return;
		}

		ptr = Real2Host(host_readd(ptr + 0x1b));
	}

	host_writeb(ptr + 0x0f, val);
}

struct dummy {
	char a[35];
};

void FIG_remove_from_list(signed char fight_id, signed char v2)
{
	Bit8u* p = Real2Host(ds_readd(FIG_LIST_HEAD));

	/* NULL check */
	if (!NOT_NULL(p))
		return;

	while (host_readb(p + 0x10) != fight_id) {

		/* if (ptr->next == NULL); */
		if (host_readd(p + 0x1b) == 0) {
			return;
		}

		/* ptr = ptr->next; */
		p = Real2Host(host_readd(p + 0x1b));
	}

	if (!v2) {
		ds_writeb(0xe089 + fight_id, 0);
	} else {
//		struct_copy(p_datseg + 0xe066, p, 35);
		*((struct dummy*)(p_datseg + 0xe066)) = *((struct dummy*)(p));
	}

	/* check if p == HEAD */
	if (p == Real2Host(ds_readd(FIG_LIST_HEAD))) {
		/* Set HEAD: head = p->next;*/
		ds_writed(FIG_LIST_HEAD, host_readd(p + 0x1b));
		if (ds_readd(FIG_LIST_HEAD) != 0)
			/* head->prev = NULL */
			host_writed(Real2Host(ds_readd(FIG_LIST_HEAD)) + 0x1f, 0);
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

	host_writeb(p + 0x10, -1);
}

signed char FIG_add_to_list(signed char v)
{
	RealPt p1;
	RealPt p2;
	signed short x, y;

	p1 = (RealPt)ds_readd(0xe37c);
	x = ds_readbs(0xe069);
	y = ds_readbs(0xe06a);

	/* FIG_list_start == NULL */
	if (ds_readd(FIG_LIST_HEAD) == 0) {

		ds_writed(FIG_LIST_HEAD, ds_readd(0xe37c));

//		struct_copy(Real2Host(ds_readd(FIG_LIST_HEAD)), p_datseg + 0xe066, 35);
		*((struct dummy*)(Real2Host(ds_readd(FIG_LIST_HEAD)))) = *((struct dummy*)(p_datseg + 0xe066));

		if (v == -1) {
			host_writeb(Real2Host(ds_readd(FIG_LIST_HEAD)) + 0x10,
				FIG_set_array());
		}

		host_writed(Real2Host(ds_readd(FIG_LIST_HEAD)) + 0x1f, 0);
		host_writed(Real2Host(ds_readd(FIG_LIST_HEAD)) + 0x1b, 0);

#if !defined(__BORLANDC__)
		D1_LOG("\tlist created x = %d, y = %d\n", x, y);
#endif

		return host_readbs(Real2Host(ds_readd(FIG_LIST_HEAD)) + 0x10);
	}

	while (host_readbs(Real2Host(p1) + 0x10) != -1) {
		p1 += 35;
	}

//	struct_copy(Real2Host(p1), p_datseg + 0xe066, 35);
	*((struct dummy*)(Real2Host(p1))) =	*((struct dummy*)(p_datseg + 0xe066));

	if (v == -1) {
		host_writeb(Real2Host(p1) + 0x10, FIG_set_array());
	} else {
		host_writeb(Real2Host(p1) + 0x10, v);
	}

	p2 = (RealPt)ds_readd(FIG_LIST_HEAD);

	if (ds_readbs(0xe077) != -1) {
		while ((host_readbs(Real2Host(p2) + 3) <= x) &&
		(host_readbs(Real2Host(p2) + 3) != x ||
		host_readbs(Real2Host(p2) + 4) >= y) &&
		(host_readbs(Real2Host(p2) + 3) != x ||
		host_readbs(Real2Host(p2) + 4) != y ||
		host_readbs(Real2Host(p2) + 0x11) <= ds_readbs(0xe077)))
		{

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
				return host_readbs(Real2Host(p1) + 0x10);
			}

			/* p2 = p2->next */
			p2 = (RealPt)host_readd(Real2Host(p2) + 0x1b);
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
		ds_writed(FIG_LIST_HEAD, (Bit32u)p1);

	/* p2->prev = p1 */
	host_writed(Real2Host(p2) + 0x1f, (Bit32u)p1);
	/* p1->next = p2 */
	host_writed(Real2Host(p1) + 0x1b, (Bit32u)p2);
#if !defined(__BORLANDC__)
	D1_LOG("\tlist insert x = %d, y = %d\n", x, y);
#endif
	return host_readbs(Real2Host(p1) + 0x10);
}

/**
 * \brief		draws the heroes picture to the fight screen
 * \param loc		0 = upper left, 1 = lower left
 * \param hero_pos	position of the hero
*/
void FIG_draw_char_pic(signed short loc, signed short hero_pos)
{
	RealPt hero;
	signed short fg_bak, bg_bak;

	hero = (RealPt)ds_readd(HEROS) + (hero_pos - 1)  * 0x6da;
	ds_writed(0xc019, (Bit32u)(hero + 0x2da));

	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(0xff, 0);

	ds_writed(0xc00d, ds_readd(0xd303));
	ds_writed(0xd2fb, ds_readd(0xd303));

	if (loc == 0) {

		do_border((RealPt)ds_readd(0xd303), 1, 9, 34, 42, 29);
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
		do_border((RealPt)ds_readd(0xd303), 1, 157, 34, 190, 29);
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
 * \brief	draws a picture of the monster, when on turn
 * \param loc	0 = left side, 1 = right side
 * \param id	ID of the enemy
 */
void FIG_draw_enemy_pic(signed short loc, signed short id)
{
	signed short height_width;
	Bit8u *p_enemy;
	signed short fg_bak;
	signed short bg_bak;
	RealPt p1;
	struct nvf_desc nvf;

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
		do_border((RealPt)ds_readd(0xd303), 1, 9, 34, 50, 0x1d);
		ds_writew(0xc011, 2);
		ds_writew(0xc013, 10);
		ds_writew(0xc015, 33);
		ds_writew(0xc017, 49);
		ds_writed(0xc019, (Bit32u)p1);
		do_pic_copy(0);
		GUI_print_string(Real2Host(GUI_name_singular(get_monname(host_readbs(p_enemy)))), 1, 1);
	} else {
		do_border((RealPt)ds_readd(0xd303), 1, 149, 34, 190, 0x1d);
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
