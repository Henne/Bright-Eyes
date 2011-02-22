#include <string.h>
#include <stdlib.h>

#include "regs.h"
#include "paging.h"

#include "../schick.h"

#include "g105de_seg002.h"
#include "g105de_seg003.h"

#include "../rewrite_m302de/seg008.h"

/* static */
void draw_mouse_ptr()
{
	if (ds_readw(0x1248))
		return;

	if (ds_readw(0x124a) == 0) {
		ds_writew(0x1248, 1);
		do_draw_mouse_ptr();
		ds_writew(0x1248, 0);
	}
	ds_writew(0x124a, ds_readw(0x124a) - 1);
}

/* static */
Bit16u get_mouse_action_gen(Bit16u x, Bit16u y, Bit8u *ptr) {

	Bit16u i;

	for (i = 0; host_readw(ptr + i * 10) != 0xffff; i++) {

		if (host_readw(ptr + i * 10 ) > x)
			continue;
		if (host_readw(ptr + i * 10 + 4) < x)
			continue;
		if (host_readw(ptr + i * 10 + 2) > y)
			continue;
		if (host_readw(ptr + i * 10 + 6) < y)
			continue;

		return host_readw(ptr + i * 10 + 8);
	}

	return 0;
}

/* static */
void do_draw_mouse_ptr()
{
	PhysPt ptr;
	Bit16u pos_x, pos_y;
	Bit16u d_x, d_y;
	Bit16u i, j;

	ptr = Real2Phys(ds_readd(0x47cb));

	pos_x = ds_readw(0x1250) - ds_readw(0x125a);
	pos_y = ds_readw(0x1252) - ds_readw(0x125c);
	d_x = d_y = 16;

	if (pos_x > 304)
		d_x = 320 - pos_x;
	if (pos_y > 184)
		d_y = 200 - pos_y;

	ptr += pos_y * 320 + pos_x;

	for (i = 0; i < d_y; ptr += 320, i++)
		for (j = 0; j < d_x; j++)
			mem_writeb_inline(ptr + j,
				ds_readb(0x4669 + i * 16 + j));
}

/* static */
Bit32s get_archive_offset(Bit8u *name, Bit8u *table) {

	Bit32s offset, length;
	Bit16u i;

	for (i = 0; i < 50; i++) {

		/* check the filename */
		if (strncmp((char*)name, (char*)table + i * 16, 12))
			continue;

		/* calculate offset and length */
		offset = host_readd(table + i * 16 + 0x0c);
		length = host_readd(table + (i + 1) * 16 + 0x0c) - offset;

		/* save length in 2 variables */
		ds_writed(0x3f2a, length);
		ds_writed(0x3f2e, length);

		return offset;
	}

	return -1;
}

Bit32s get_filelength() {

	return ds_readd(0x3f2a);
}

Bit16u ret_zero1() {
	return 0;
}

Bit32u swap32(Bit16u v1, Bit16u v2) {
	return (swap_u16(v2) << 16 | swap_u16(v1));
}

void init_video()
{
	/* set the video mode to 320x200 8bit */
	set_video_mode(0x13);
	set_color(MemBase + PhysMake(datseg, 0x1b79), 0xff);
}

void exit_video()
{
	/* restore old mode */
	set_video_mode(ds_readw(0x47dd));
	/* restore old page */
	set_video_page(ds_readw(0x47db));
}

void draw_v_line(Bit16u x, Bit16u y1, Bit16u y2, Bit16u color)
{
	Bit16u tmp, len, off;

	if (y1 > y2) {
		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	len = y2 - y1 + 1;
	off = y1 * 320 + x;

	draw_h_spaced_dots(PhysMake(0xa000, off), len, color, 320);
}

/* static */
void blit_smth3(PhysPt ptr, Bit16u v1, Bit16u v2) {

	Bit8u *src;
	Bit16u i, j;

	src = MemBase + PhysMake(datseg, 0x45e1);

	for (i = 0; i < v1; src += 8 - v2, ptr += 320, i++)
		for (j = 0; j < v2; src++, j++)
			mem_writeb_inline(ptr + j, host_readb(src));
}

Bit16u print_chr(unsigned char c, Bit16u x, Bit16u y) {

	Bit16u width, idx;

	idx = get_chr_info(c, (Bit8u*)&width);

	call_them_all(idx, width, x, y);

	return width;
}

/**
 * get_chr_info() - gets font information of a character
 * @c:		the character
 * @width:	pointer to save width
 *
 * Returns the font index.
 */
Bit8u get_chr_info(unsigned char c, Bit8u *ptr) {

	Bit16u i;

	for (i = 0; i != 222; i += 3) {
		/* search for the character */
		if (ds_readb(0x1b85 + i) != c)
			continue;

		host_writew(ptr, ds_readb(0x1b85 + 2 + i));
		return ds_readb(0x1b85 + 1 + i);
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		host_writew(ptr, 0);
		return 0;
	} else {
		host_writew(ptr, 6);
		return 0;
	}
}

/* static */
void call_them_all(Bit16u v1, Bit16u v2, Bit16u x, Bit16u y) {

	PhysPt gfx_ptr;
	Bit32u bogus;

	fill_smth();
	fill_smth2(MemBase + Real2Phys(ds_readd(0x4779) + v1 * 8));

	gfx_ptr = get_gfx_ptr(x, y);
	bogus = ret_zero();

	call_blit_smth3(Real2Phys(gfx_ptr), 7, 0, 0, v2);
}

/* static */
void fill_smth() {

	Bit8u *ptr;
	Bit16u i, j;

	if (ds_readb(0x109a))
		ptr = MemBase + PhysMake(datseg, 0x45a1);
	else
		ptr = MemBase + PhysMake(datseg, 0x45e1);

	for (i = 0; i < 8; i++, ptr += 8)
		for (j = 0; j < 8; j++)
			host_writeb(ptr + j, ds_readb(0x477f));
}

/* static */
void fill_smth2(Bit8u* ptr) {

	Bit8u *lp;
	Bit16u i, j;
	Bit8u lv;

	if (ds_readb(0x109a))
		lp = MemBase + PhysMake(datseg, 0x45a1);
	else
		lp = MemBase + PhysMake(datseg, 0x45e1);

	for (i = 0; i < 8; i++, lp += 8) {
		lv = host_readb(ptr);
		ptr++;
		for (j = 0; j < 8; j++) {
			if (!((0x80 >> j) & lv))
				continue;

			host_writeb(lp + j,
				ds_readb(0x4781 + ds_readw(0x477d) * 2));
		}
	}
}

/* static */
RealPt get_gfx_ptr(Bit16u x, Bit16u y) {

	RealPt start;

	start = ds_readd(0x47c7) + 320 * y + x;

	return start;
}

/* static */
Bit16u ret_zero() {
	return 0;
}

void call_fill_rect_gen(PhysPt ptr, Bit16u x1, Bit16u y1, Bit16u x2, Bit16u y2, Bit16u color)
{
	fill_rect(ptr + y1 * 320 + x1, color, x2 - x1 + 1, y2 - y1 + 1);
}

/* static */
void call_blit_smth3(PhysPt dst, Bit16u v1, Bit16u v2, Bit16u v3, Bit16u v4) {
	blit_smth3(dst, v1, v4);
}

/* static */
void set_vals(Bit16u v1, Bit16u v2) {
	ds_writew(0x4781, v1);
	ds_writew(0x477f, v2);
}

/* static */
void get_vals(Bit8u *p1, Bit8u *p2) {
	host_writew(p1, ds_readw(0x4781));
	host_writew(p2, ds_readw(0x477f));
}

Bit16u get_str_width(char *str) {

	Bit16u width, sum = 0;

	while (*str) {
		get_chr_info(*str++, (Bit8u*)&width);
		sum += width;
	}

	return sum;
}

/**
 * get_line_start_c() - calculates the start positon for a centered line
 * @str:	the string
 * @x:		start position of the string
 * @x_max:	end position of the string
 *
 * Returns the X coordinate where the strin must start.
 */
Bit16u get_line_start_c(char *str, Bit16u x, Bit16u x_max) {

	Bit16u width, sum = 0;

	while (*str && *str != 0x40 && *str != 0x0d) {
		get_chr_info(*str++, (Bit8u*)&width);
		sum += width;
	}
	return (x_max - sum) / 2 + x ;
}

/**
 * calc_at_pa() - calculate AT and PA values
 */
/* static */
void calc_at_pa() {
	Bit16u i;
	Bit16s tmp, base;

	/* base = (GE + IN + KK) / 5 */
	tmp = (signed char)ds_readb(0x136f);
	tmp += (signed char)ds_readb(0x1372);
	tmp += (signed char)ds_readb(0x136c);

	base = tmp / 5;

	/* round up if neccessary */
	if ((tmp % 5) >= 3)
		base++;

	/* save AT/PA base value */
	ds_writeb(0x1393, base);

	for (i = 0; i < 7; i++) {
		/* set the weapon values to base */
		ds_writeb(0x139b + i, base);
		ds_writeb(0x1394 + i, base);

		if ((signed char)ds_readb(0x1434 + i) < 0) {
			/* calculate ATPA for negative weapon skill */
			tmp = abs((signed char)ds_readb(0x1434 + i)) / 2;

			/* sub skill / 2 from AT */
			ds_writeb(0x1394 + i, ds_readb(0x1394 + i) - tmp);

			/* sub skill / 2 from PA */
			ds_writeb(0x139b + i, ds_readb(0x139b + i) - tmp);

			/* if skill % 2, then decrement PA */
			if (abs((signed char)ds_readb(0x1434 + i)) != tmp * 2)
				ds_writeb(0x139b + i, ds_readb(0x139b + i) - 1);
		} else {
			/* calculate ATPA for positive weapon skill */
			tmp = abs((signed char)ds_readb(0x1434 + i)) / 2;

			/* add skill / 2 to AT */
			ds_writeb(0x1394 + i, ds_readb(0x1394 + i) + tmp);

			/* add skill / 2 to PA */
			ds_writeb(0x139b + i, ds_readb(0x139b + i) + tmp);

			/* if skill % 2, then increment AT */
			if (ds_readb(0x1434 + i) != tmp * 2)
				ds_writeb(0x1394 + i, ds_readb(0x1394 + i) + 1);
		}

	}
}

/* static */
void clear_hero() {

	Bit16u i;

	ds_writew(0x2780, 0);
	ds_writew(0x2782, 0);

	ds_writeb(0x40b6, 0);
	ds_writeb(0x40b4, 0);
	ds_writeb(0x40b5, 0);
	ds_writeb(0x40b7, 0);

	for (i = 0; i < 14; i++)
		ds_writeb(0x4076 + i, 0);

	for (i = 0; i < 86; i++) {
		ds_writeb(0x3f63 + i * 2, 0);
		ds_writeb(0x3f62 + i * 2, 0);
	}
	for (i = 0; i < 52; i++) {
		ds_writeb(0x400f + i * 2, 0);
		ds_writeb(0x400e + i * 2, 0);
	}

	ds_writeb(0x1353, 1);
};

/**
 * skill_inc_novice() - tries to increment a skill in novice mode
 * @skill:	the skill which should be incremented
 *
 *
 */
void skill_inc_novice(Bit16u skill)
{
	Bit16u done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (ds_readw(0x400e + skill * 2) == 3) {
			/* set the flag to leave this loop */
			done++;
			continue;
		}

		/* decrement counter for skill increments */
		ds_writeb(0x1468, ds_readb(0x1468) - 1);

		/* check if the test is passed */
		if (random_interval_gen(2, 12) > (signed char)ds_readb(0x1434 + skill)) {
			/* increment skill */
			ds_writeb(0x1434 + skill, ds_readb(0x1434 + skill) + 1);

			/* set inc tries for this skill to zero */
			ds_writew(0x400e + skill * 2, 0);

			/* set the flag to leave this loop */
			done++;

			if (skill > 6)
				continue;

			/* set increment the lower AT/PA value */
			if (ds_readb(0x1394 + skill) > ds_readb(0x139b + skill))
				ds_writeb(0x139b + skill, ds_readb(0x139b + skill) + 1);
			else
				ds_writeb(0x1394 + skill, ds_readb(0x1394 + skill) + 1);
		} else
			/* inc tries for that skill */
			ds_writew(0x400e + skill * 2, ds_readw(0x400e + skill * 2) + 1);
	}
}

/**
 * spell_inc_novice() - tries to increment a spell in novice mode
 * @spell:	the spell which should be incremented
 *
 *
 */
void spell_inc_novice(Bit16u spell)
{
	Bit16u done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (ds_readw(0x3f62 + spell * 2) == 3) {
			/* set the flag to leave this loop */
			done++;
			continue;
		}

		/* decrement counter for spell increments */
		ds_writeb(0x14bf, ds_readb(0x14bf) - 1);

		/* check if the test is passed */
		if (random_interval_gen(2, 12) > (signed char)ds_readb(0x1469 + spell)) {
			/* increment spell */
			ds_writeb(0x1469 + spell, ds_readb(0x1469 + spell) + 1);

			/* set inc tries for this spell to zero */
			ds_writew(0x3f62 + spell * 2, 0);

			/* set the flag to leave this loop */
			done++;
		} else
			ds_writeb(0x3f62 + spell * 2, ds_readb(0x3f62 + spell * 2) + 1);
	}
}

void init_colors()
{
	set_palette(MemBase + PhysMake(datseg, 0x1d4c), 0x00, 1);
	set_palette(MemBase + PhysMake(datseg, 0x1d49), 0xff, 1);
	set_palette(MemBase + PhysMake(datseg, 0x1d4f), 0xd8, 8);
	set_palette(MemBase + PhysMake(datseg, 0x1d67), 0xc8, 3);
	set_palette(MemBase + PhysMake(datseg, 0x119b), 0x40, 0x20);
	set_palette(MemBase + PhysMake(datseg, 0x1d70), 0x20, 0x20);
	set_vals(0xff, 0x0);
}

void init_stuff()
{
	init_colors();

	/* these 3 variables are bogus */
	ds_writew(0x4783, 0xc8);
	ds_writew(0x4785, 0xc9);
	ds_writew(0x4787, 0xca);

	/* number of menu tiles width */
	ds_writew(0x40b9, 3);

	ds_writed(0x40c1, ds_readd(0x47cb));
}
