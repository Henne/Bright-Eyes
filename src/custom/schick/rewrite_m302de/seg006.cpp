/*
 *      Rewrite of DSA1 v3.02_de functions of seg006 (Fight)
 *      Functions rewritten 13/16
 *
*/

#include "string.h"

#include "mem.h"

#include "schick.h"

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg006.h"
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

void FIG_draw_pic() {
	mem_memcpy(Real2Phys(ds_readd(0xd303)), Real2Phys(ds_readd(0xc3a9)), 64000);
	ds_writeb(0x26af, 1);

	if (ds_readw(0x26b3))
		FIG_draw_char_pic(0, ds_readw(0x26b3));
	else if (ds_readw(0x26b5))
		FIG_draw_enemy_pic(0, ds_readw(0x26b5));
}

RealPt FIG_get_hero_ptr(unsigned short v1) {
	RealPt heros = ds_readd(HEROS);
	unsigned short i;

	for (i = 0; i <= 6; i++)
		if (mem_readb(Real2Phys(heros) + i * 0x6da + 0x81) == v1)
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
	Bit8u *ptr = MemBase + Real2Phys(ds_readd(0xe108));

	while (host_readb(ptr + 0x10) != id) {
		if (host_readd(ptr + 0x1b) == 0)
			return;
		ptr = MemBase + Real2Phys(host_readd(ptr + 0x1b));
	}
	host_writeb(ptr + 0x0e, val);
	host_writeb(ptr + 0x12, 1);
}

/* Used by range attack and spells with more than 1 field distance */
void FIG_reset_12_13(signed char id) {
	Bit8u *ptr1, *ptr2;

	ptr1 = MemBase + Real2Phys(ds_readd(0xe108));

	while (host_readb(ptr1 + 0x10) != id) {
		if (host_readd(ptr1 + 0x1b) == 0)
			return;
		ptr1 = MemBase + Real2Phys(host_readd(ptr1 + 0x1b));
	}
	host_writeb(ptr1 + 0x12, 0);

	if (host_readb(ptr1 + 0x13) == 0xff)
		return;

	ptr2 = MemBase + Real2Phys(ds_readd(0xe108));

	while (ds_readb(0xe35a + host_readb(ptr1 + 0x13)) != host_readb(ptr2 + 0x10)) {
		ptr2 = MemBase + Real2Phys(host_readd(ptr2 + 0x1b));
	}
	host_writeb(ptr2 + 0x12, 0);
}

void FIG_set_12_13(signed char id) {
	Bit8u *ptr1, *ptr2;

	ptr1 = MemBase + Real2Phys(ds_readd(0xe108));

	while (host_readb(ptr1 + 0x10) != id) {
		if (host_readd(ptr1 + 0x1b) == 0)
			return;
		ptr1 = MemBase + Real2Phys(host_readd(ptr1 + 0x1b));
	}
	host_writeb(ptr1 + 0x12, 1);

	if (host_readb(ptr1 + 0x13) == 0xff)
		return;

	ptr2 = MemBase + Real2Phys(ds_readd(0xe108));

	while (ds_readb(0xe35a + host_readb(ptr1 + 0x13)) != host_readb(ptr2 + 0x10)) {
		ptr2 = MemBase + Real2Phys(host_readd(ptr2 + 0x1b));
	}
	host_writeb(ptr2 + 0x12, 1);
}

void FIG_set_0f(signed char id, signed char val) {
	Bit8u *ptr = MemBase + Real2Phys(ds_readd(0xe108));

	while (host_readb(ptr + 0x10) != id) {
		if (host_readd(ptr + 0x1b) == 0)
			return;
		ptr = MemBase + Real2Phys(host_readd(ptr + 0x1b));
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

		fls = MemBase + Real2Phys(ds_readd(0xe108));

		memcpy(fls, MemBase + PhysMake(datseg, 0xe066), 35);

		if (v == -1)
			host_writeb(fls + 0x10,	FIG_set_array());

		host_writed(fls + 0x1f, 0);
		host_writed(fls + 0x1b, 0);

		D1_LOG("\tlist created x = %d, y = %d\n", x, y);

		return host_readb(fls + 0x10);
	}

	while ((signed char)mem_readb(Real2Phys(p1) + 0x10) != -1) {
		p1 += 35;
	}

	memcpy(MemBase + Real2Phys(p1), MemBase + PhysMake(datseg, 0xe066), 35);

	if (v == -1)
		host_writeb(MemBase + Real2Phys(p1) + 0x10, FIG_set_array());
	else
		host_writeb(MemBase + Real2Phys(p1) + 0x10, v);

	p2 = ds_readd(0xe108);

	if ((signed char)ds_readb(0xe077) != -1)
		for (; (signed char)mem_readb(Real2Phys(p2) + 3) <= x &&
		((signed char)mem_readb(Real2Phys(p2) + 3) != x ||
		(signed char)mem_readb(Real2Phys(p2) + 4) >= y) &&
		((signed char)mem_readb(Real2Phys(p2) + 3) != x ||
		(signed char)mem_readb(Real2Phys(p2) + 4) != y ||
		(signed char)mem_readb(Real2Phys(p2) + 0x11) <= (signed char)ds_readb(0xe077))
				/* p2 = p2->next */
			 ; p2 = mem_readd(Real2Phys(p2) + 0x1b)) {

			/* p2->next != NULL */
			if (mem_readd(Real2Phys(p2) + 0x1b) != 0)
				continue;

			/* append to end of the list */

			/* p2->next = p1 */
			mem_writed(Real2Phys(p2) + 0x1b, p1);
			/* p1->prev = p2 */
			mem_writed(Real2Phys(p1) + 0x1f, p2);
			/* p1->next = NULL */
			mem_writed(Real2Phys(p1) + 0x1b, 0);

			D1_LOG("\tlist appended x = %d, y = %d\n", x, y);

			return mem_readb(Real2Phys(p1) + 0x10);
		}

	/* p1->prev = p2->prev; */
	mem_writed(Real2Phys(p1) + 0x1f, mem_readd(Real2Phys(p2) + 0x1f));

	/* if (p2->prev) */
	if (mem_readd(Real2Phys(p2) + 0x1f) != 0)
		/* p2->prev->next = p1 */
		mem_writed(Real2Phys(mem_readd(Real2Phys(p2) + 0x1f)) + 0x1b, p1);
	else
		/* FIG_list_start = p1 */
		ds_writed(0xe108, p1);

	/* p2->prev = p1 */
	mem_writed(Real2Phys(p2) + 0x1f, p1);
	/* p1->next = p2 */
	mem_writed(Real2Phys(p1) + 0x1b, p2);

	D1_LOG("\tlist insert x = %d, y = %d\n", x, y);

	return mem_readb(Real2Phys(p1) + 0x10);
}

/**
	FIG_draw_char_pic - draws the heroes picture to the fight screen
	@pos:		0 upper left / 1 lower left
	@hero_nr:	number of the hero
*/
void FIG_draw_char_pic(unsigned short pos, unsigned short hero_nr) {
	RealPt hero;
	short bak1, bak2;

	hero = ds_readd(HEROS) + (hero_nr - 1)  * 0x6da;
	ds_writed(0xc019, hero + 0x2da);

	GUI_get_smth(&bak1, &bak2);
	GUI_set_smth(0xff, 0);

	ds_writed(0xc00d, ds_readd(0xd303));
	ds_writed(0xd2fb, ds_readd(0xd303));

	if (pos == 0) {

		do_border(Real2Phys(ds_readd(0xd303)), 1, 9, 34, 42, 29);
		ds_writew(0xc011, 2);
		ds_writew(0xc013, 10);
		ds_writew(0xc015, 33);
		ds_writew(0xc017, 41);
		GUI_print_string(MemBase + Real2Phys(hero + 0x10), 1, 1);
		draw_bar(0, 0, mem_readw(Real2Phys(hero+0x60)), mem_readw(Real2Phys(hero+0x5e)), 1);
		draw_bar(1, 0, mem_readw(Real2Phys(hero+0x62)), mem_readw(Real2Phys(hero+0x64)), 1);
	} else {
		do_border(Real2Phys(ds_readd(0xd303)), 1, 157, 34, 190, 29);
		ds_writew(0xc011, 2);
		ds_writew(0xc013, 158);
		ds_writew(0xc015, 33);
		ds_writew(0xc017, 189);
		GUI_print_string(MemBase + Real2Phys(hero + 0x10), 1, 193);
	}

	do_pic_copy(0);
	ds_writed(0xc00d, ds_readd(0xd2ff));
	ds_writed(0xd2fb, ds_readd(0xd2ff));
	GUI_set_smth(bak1, bak2);
}

void FIG_draw_enemy_pic(unsigned short v1, unsigned short v2) {
}

}
