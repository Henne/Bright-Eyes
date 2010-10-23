#include "mem.h"

#include "../schick.h"

#include "seg008.h"

void set_var_to_zero() {
	real_writew(datseg, 0x29ae, 0);
}

void schick_set_video() {
	set_video_mode(0x13);
	set_color(MemBase + PhysMake(datseg, 0x4b03), 0xff);
}

void schick_reset_video() {
	set_video_mode(real_readw(datseg, 0xd30d));
	set_video_page(real_readw(datseg, 0xd30b));
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
