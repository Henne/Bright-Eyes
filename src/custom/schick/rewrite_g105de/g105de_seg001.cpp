#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "cpu.h"

#include "schick.h"

#include "g105de_seg001.h"

static unsigned short CD_has_drives() {

	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	if (reg_bx == 0)
		return 0;

	return 1;
}

static unsigned short CD_count_drives() {

	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	return reg_bx;
}

static unsigned short CD_get_first_drive() {

	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	return reg_cx;
}

unsigned short G105de::CD_set_drive_nr() {

	if(CD_has_drives() == 0)
		return 0;

	if(CD_count_drives() == 0)
		return 0;

	ds_writew(0x246c, CD_get_first_drive());
	return 1;
}

void G105de::CD_driver_request(RealPt req) {
	reg_ax = 0x1510;
	reg_cx = ds_readw(0x246c);
	CPU_SetSegGeneral(es, RealSeg(req));
	reg_bx = RealOff(req);
	CALLBACK_RunRealInt(0x2f);
}
