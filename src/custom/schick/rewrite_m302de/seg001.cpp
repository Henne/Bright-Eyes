#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "cpu.h"

#include "schick.h"

#include "seg000.h"
#include "seg001.h"

unsigned short CD_has_drives() {

	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	if (reg_bx == 0)
		return 0;

	return 1;
}

unsigned short CD_count_drives() {

	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	return reg_bx;
}

unsigned short CD_get_first_drive() {

	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	return reg_cx;
}

unsigned short CD_set_drive_nr() {

	if(CD_has_drives() == 0)
		return 0;

	if(CD_count_drives() == 0)
		return 0;

	ds_writew(0xbc52, CD_get_first_drive());
	return 1;
}

void CD_driver_request(RealPt req) {
	reg_ax = 0x1510;
	reg_cx = ds_readw(0xbc52);
	CPU_SetSegGeneral(es, RealSeg(req));
	reg_bx = RealOff(req);
	CALLBACK_RunRealInt(0x2f);
}

unsigned int CD_get_tod() {
	reg_ah = 0;
	CALLBACK_RunRealInt(0x1a);
	reg_ax = reg_dx;
	reg_dx = reg_cx;

	return (reg_dx << 16) | reg_ax;
}

void seg001_00c1(unsigned short track_nr) {
	unsigned int track_start, track_end;
	unsigned int track_len, tmp;

	if (ds_readw(0x95) == 0)
		return;

	real_writew(relocation + 0x1238, 0x8f, 0);

	tmp = real_readd(relocation + 0x1238, 0x10a + track_nr * 8) & 0x00ffffff;
	real_writed(relocation + 0x1238, 0x9a, tmp);

	/* calculate track_start */
	tmp = real_readb(relocation + 0x1238, 0x10c + track_nr * 8) * 60;
	tmp += real_readb(relocation + 0x1238, 0x10b + track_nr * 8);
	tmp *= 75;
	tmp += real_readb(relocation + 0x1238, 0x10a + track_nr * 8);
	track_start = tmp;

	/* calculate track_end */
	if (real_readb(relocation + 0x1238, 0x422) == track_nr) {
		tmp = real_readb(relocation + 0x1238, 0x425) * 60;
		tmp += real_readb(relocation + 0x1238, 0x424);
		tmp *= 75;
		tmp += real_readb(relocation + 0x1238, 0x423);
	} else {
		tmp = real_readb(relocation + 0x1238, 0x114 + track_nr * 8) * 60;
		tmp += real_readb(relocation + 0x1238, 0x113 + track_nr * 8);
		tmp *= 75;
		tmp += real_readb(relocation + 0x1238, 0x112 + track_nr * 8);
	}
	track_end = tmp;

	track_len = track_end - track_start;
	real_writed(relocation + 0x1238, 0x9e, track_len - 150);

	CD_driver_request(RealMake(relocation + 0x1238, 0x8c));
	ds_writed(0xbc4e, ((track_len - 150) * 0x1234e) / 0x4b000);
	ds_writed(0xbc4a, CD_get_tod());
}

void seg001_02c4() {

	signed int val;

	if (ds_readw(0x95) == 0)
		return;

	val = CD_get_tod();
	val -= ds_readd(0xbc4a);

	if (val < ds_readd(0xbc4e))
		return;

	if (ds_readw(0x9b) != 1)
		return;

	seg001_0322();
	seg001_0322();
	seg001_00c1(ds_readw(0xbc40));
	ds_writew(0x9b, 1);
}

signed short CD_bioskey(signed short cmd) {
	seg001_02c4();
	return bioskey(cmd);
}

void seg001_0322() {
	if (ds_readw(0x95) == 0)
		return;

	real_writew(relocation + 0x1238, 3, 0);
	CD_driver_request(RealMake(relocation + 0x1238, 0));
	ds_writew(0x9b, 0);
}

void seg001_034f() {
	if (ds_readw(0x95) == 0)
		return;

	seg001_0322();
	real_writew(relocation + 0x1238, 0x1f, 0);
	CD_driver_request(RealMake(relocation + 0x1238, 0x1c));
}

void CD_audio_pause() {
	/* Is CD initialized ? */
	if (ds_readw(0x95) == 0)
		return;

	/* Is CD already paused ? */
	if (ds_readw(0xa1) != 0)
		return;

	/* set CD pause */
	ds_writew(0xa1, 1);
	ds_writed(0xbc3c, CD_get_tod());
	/* save current position */
	ds_writed(0xbc38, ds_readd(0xbc4e));
	/* set current position to maximum singned int */
	ds_writed(0xbc4e, 0x7fffffff);

	real_writew(relocation + 0x1238, 0xab, 0);
	CD_driver_request(RealMake(relocation + 0x1238, 0xa8));
}

void CD_audio_play() {
	/* Is CD initialized ? */
	if (ds_readw(0x95) == 0)
		return;

	/* Is CD paused ? */
	if (ds_readw(0xa1) == 0)
		return;

	//CD_check();

	/* reset CD pause */
	ds_writew(0xa1, 0);
	ds_writed(0xbc4e, ds_readd(0xbc38));
	ds_writed(0xbc4a, CD_get_tod() - ds_readd(0xbc3c) + ds_readd(0xbc4a));

	real_writew(relocation + 0x1238, 0xc7, 0);
	CD_driver_request(RealMake(relocation + 0x1238, 0xc4));
}
