#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "cpu.h"

#include "schick.h"

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
