#include "mem.h"

#include "../schick.h"

#include "seg008.h"

void set_var_to_zero() {
	real_writew(datseg, 0x29ae, 0);
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

void schick_set_video() {
	set_video_mode(0x13);
	set_color(MemBase + PhysMake(datseg, 0x4b03), 0xff);
}

void schick_reset_video() {
	set_video_mode(real_readw(datseg, 0xd30d));
	set_video_page(real_readw(datseg, 0xd30b));
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

	x1 = real_readw(datseg, 0xc011);
	y1 = real_readw(datseg, 0xc013);

	x2 = real_readw(datseg, 0xc015);
	y2 = real_readw(datseg, 0xc017);

	src = real_readd(datseg, 0xc019);
	dst = real_readd(datseg, 0xc00d);

	v10 = y1 * 320 + x1;
	width = x2 - x1 + 1;
	height = y2 - y1 + 1;

	save_rect(PhysMake(RealSeg(dst), RealOff(dst) + v10), Real2Phys(src), width, height);
}
