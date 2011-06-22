#include "mem.h"
#include "paging.h"
#include "callback.h"

#include "schick.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"

void set_var_to_zero() {
	ds_writew(0x29ae, 0);
}

void clear_ani() {
	unsigned short i,j;

	ds_writew(0xc3e7, 0);
	ds_writeb(0xc3ed, 0);
	ds_writeb(0xc3ee, 0);
	ds_writed(0xce35, 0);
	ds_writew(0xce3d, 0);
	ds_writew(0xce3c, 0);

	for (i = 0; i < 10; i++) {
		ds_writew(0xc3f4 + i * 0x107, 0);
		ds_writeb(0xc3f6 + i * 0x107, 0);
		ds_writew(0xc3f8 + i * 0x107, 0);
		ds_writeb(0xc3f7 + i * 0x107, 0);
		ds_writeb(0xc3fa + i * 0x107, 0);
		ds_writeb(0xc3fb + i * 0x107, 0);
		ds_writew(0xc44c + i * 0x107, 0);
		ds_writeb(0xc3f3 + i * 0x107, 0);

		for (j = 0; j < 20; j++)
			ds_writed(0xc3fc + i * 0x107 + (j << 2), 0);

		for (j = 0; j < 42; j++) {
			ds_writew(0xc44e + i * 0x107 + (j << 2), 0);
			ds_writew(0xc450 + i * 0x107 + (j << 2), 0);
		}
	 }
}
/**
	draw_bar - draws a bar to visualize LE or AE
	@type:		0 = LE / 1 = AE
	@hero:		number of hero when mode is zero
	@pts_cur:	current points
	@pts_max:	maximum points
	@mode:		0 on game mask, 1 in fight

	It should be used, either hero or mode is zero,
	since in fight mode only the active hero is shown.
*/
void draw_bar(unsigned short type, unsigned short hero, unsigned short pts_cur, unsigned short pts_max, unsigned short mode) {
	PhysPt dst;
	unsigned short x, lost;
	unsigned short i, y_min;

	if (mode == 0)
		update_mouse_cursor();

	if (mode == 0) {
		x = ds_readw(0x2d01 + hero * 2) + type * 4 + 34;
		y_min = 188;
		dst = Real2Phys(ds_readd(0xd2ff));
	} else {
		x = type * 4 + 36;
		y_min = 42;
		dst = Real2Phys(ds_readd(0xd303));
	}

	if (pts_cur == 0) {
		/* draw 4 black vertical lines */
		for (i = 0; i < 3; i++)
			do_v_line(dst, x + i, y_min - 30, y_min, 0);
	} else
		if (pts_cur == pts_max) {
			/* draw 4 full lines in the color of the type */
			for (i = 0; i < 3; i++)
				do_v_line(dst, x + i, y_min - 30, y_min,
					ds_readb(0x4a94 + type * 2));
		} else {
			lost = 30 * pts_cur / pts_max;

			if (lost == 0)
				lost++;
			/* draw visible part */
			for (i = 0; i < 3; i++)
				do_v_line(dst, x + i, y_min - lost, y_min,
					ds_readb(0x4a94 + type * 2));
			/* draw black part */
			for (i = 0; i < 3; i++)
				do_v_line(dst, x + i, y_min - 30,
					y_min - lost - 1, 0);
		}

	if (mode == 0)
		refresh_screen_size();
}

void restore_rect(PhysPt dst, Bit8u *src, unsigned short x, unsigned short y, char n, char m) {

	unsigned short i, j;
	char c;

	dst += y * 320 + x;

	for (i = 0; i < m; dst += 320, i++)
		for (j = 0; j < n; j++)
			if (c = *src++)
				mem_writeb_inline(dst + j, c);
}

void restore_rect_rle(PhysPt dst, Bit8u *src, unsigned short x, unsigned short y, char width, char height, unsigned short v1) {
	unsigned short i, si, di;
	unsigned char c, cnt, tmp;

	dst += y * 320 + x;
	update_mouse_cursor();

	for (i = 0; i < height; dst += 320, i++) {
		si = 0;
		while (si < width) {
			if ((c = *src++) == 0x7f) {
				cnt = *src++;
				tmp = *src++;
				if (tmp || v1 != 2)
					for (di = 0; di < cnt; di++)
						mem_writeb(dst + si + di, tmp);
				si += cnt;
				continue;
			}
			if (c || v1 != 2)
				mem_writeb(dst + si, c);
			si++;
		}
	}
	refresh_screen_size();
}

void draw_mouse_cursor() {
	short mask, x, y, width, height;
	PhysPt dst;
	short *mouse_cursor;
	char i,j;

	dst = Real2Phys(ds_readd(0xd2ff));
	mouse_cursor = (short*)(MemBase + Real2Phys(ds_readd(0xcecb)) + 32);

	x = ds_readw(0x299c) - ds_readw(0x29a6);
	y = ds_readw(0x299e) - ds_readw(0x29a8);

	width = height = 16;

	if (x > 304)
		width = 320 - x;

	if (y > 184)
		height = 200 - y;

	dst += y * 320 + x;

	for (i = 0; i < height; dst += 320, i++)
		for (mask = *mouse_cursor++, j = 0; j < width; j++)
			if ((0x8000 >> j) & mask)
				mem_writeb_inline(dst + j, 0xff);
}

void save_mouse_bg() {
	PhysPt src;
	short di,v6,v8,va;
	short si, j;

	src = Real2Phys(ds_readd(0xd2ff));

	di = ds_readw(0x299c) - ds_readw(0x29a6);
	v6 = ds_readw(0x299e) - ds_readw(0x29a8);

	v8 = va = 16;

	if (di > 304)
		v8 = 320 - di;

	if (v6 > 184)
		va = 200 - v6;

	src += v6 * 320 + di;

	for (si = 0; si < va; src += 320, si++)
		for (j = 0; j < v8; j++)
			ds_writeb(0xcf0f + si * 16 + j , mem_readb_inline(src + j));
}

void restore_mouse_bg() {
	PhysPt dst;
	short v2, v3, v4;
	short si, di, j;

	/* gfx memory */
	dst = Real2Phys(ds_readd(0xd2ff));
	di = ds_readw(0x29a0) - ds_readw(0x29aa);
	v2 = ds_readw(0x29a2) - ds_readw(0x29ac);
	v4 = v3 = 16;

	if (di > 304)
		v3 = 320 - di;

	if (v2 > 184)
		v4 = 200 - v2;

	dst += (v2 * 320) + di;

	for (si = 0; si < v4; dst += 320, si++)
		for (j = 0; j < v3; j++)
			mem_writeb_inline(dst + j, ds_readb(0xcf0f + si*16 + j));

}
/**
	array_add - adds op to each element of an array
	@dst:	pointer to array
	@len:	length of array
	@op:	operator
	@flag:	if 2, op will not be added if array element is 0
*/
void array_add(PhysPt dst, unsigned short len, unsigned char op, unsigned short flag) {
	unsigned short i;
	char tmp;

	for (i = 0; i < len; i++) {
		tmp = mem_readb(dst+i);

		if (flag == 2 && !tmp)
			continue;

		mem_writeb(dst + i, tmp + op);
	}
}

void schick_set_video() {
	set_video_mode(0x13);
	set_color(MemBase + PhysMake(datseg, 0x4b03), 0xff);
}

void schick_reset_video() {
	set_video_mode(ds_readw(0xd30d));
	set_video_page(ds_readw(0xd30b));
}

void do_h_line(PhysPt ptr, unsigned short x1, unsigned short x2, unsigned short y, char color) {

	unsigned short count, tmp;
	PhysPt dst;

	if (x1 == x2)
		return;

	if (x1 > x2) {
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	count = x2 - x1 +1;
	dst = y *320 + x1 + ptr;

	draw_h_line(dst, count, color);
}

void do_v_line(PhysPt ptr, unsigned short x, unsigned short y1, unsigned short y2, char color) {

	unsigned short count, tmp;
	PhysPt dst;

	if (y1 == y2)
		return;

	if (y1 > y2) {
		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	count = y2 - y1 + 1;
	dst = y1 * 320 + x + ptr;

	draw_h_spaced_dots(dst, count, color, 320);
}

void do_border(PhysPt dst, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, char color) {
	update_mouse_cursor();
	do_h_line(dst, x1, x2, y1, color);
	do_h_line(dst, x1, x2, y2, color);
	do_v_line(dst, x1, y1, y2, color);
	do_v_line(dst, x2, y1, y2, color);
	refresh_screen_size();
}

void do_pic_copy(unsigned short mode) {
	short x2, y2;
	short v1, v2, v3, v4;
	short width, height;
	RealPt src, dst;
	short x1, y1;

	x1 = ds_readw(0xc011);
	y1 = ds_readw(0xc013);
	x2 = ds_readw(0xc015);
	y2 = ds_readw(0xc017);

	v1 = ds_readw(0xc01d);
	v2 = ds_readw(0xc01f);
	v3 = ds_readw(0xc021);
	v4 = ds_readw(0xc023);

	width = x2 - x1 + 1;
	height = y2 - y1 + 1;

	src = ds_readd(0xc019);
	dst = ds_readd(0xc00d);

	pic_copy(Real2Phys(dst), x1, y1, x2, y2, v1, v2, v3, v4, width, height, MemBase + Real2Phys(src), mode);
}

void do_save_rect() {
	unsigned short width,height,x1,y1,x2,y2;
	RealPt dst,src;
	unsigned short v10;

	x1 = ds_readw(0xc011);
	y1 = ds_readw(0xc013);

	x2 = ds_readw(0xc015);
	y2 = ds_readw(0xc017);

	src = ds_readd(0xc019);
	dst = ds_readd(0xc00d);

	v10 = y1 * 320 + x1;
	width = x2 - x1 + 1;
	height = y2 - y1 + 1;

	save_rect(PhysMake(RealSeg(dst), RealOff(dst) + v10), Real2Phys(src), width, height);
}

void do_fill_rect(RealPt dst, unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned short color) {
	unsigned short width, height;

	width = w - x + 1;
	height = h - y + 1;

	dst += y * 320 + x;

	update_mouse_cursor();
	fill_rect(Real2Phys(dst), color, width, height);
	refresh_screen_size();
}

void wait_for_vsync()
{
	CALLBACK_RunRealFar(reloc_game + 0xb2a, 0x150d);
}
