#include "mem.h"
#include "paging.h"

#include "../schick.h"

#include "seg002.h"
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

void schick_set_video() {
	set_video_mode(0x13);
	set_color(MemBase + PhysMake(datseg, 0x4b03), 0xff);
}

void schick_reset_video() {
	set_video_mode(ds_readw(0xd30d));
	set_video_page(ds_readw(0xd30b));
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
