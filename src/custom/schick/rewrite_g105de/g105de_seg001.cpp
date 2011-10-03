#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "cpu.h"

#include "schick.h"

#include "g105de_seg000.h"
#include "g105de_seg001.h"

#define CDSEG (0xc83)

namespace G105de {

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

unsigned short CD_set_drive_nr() {

	if(CD_has_drives() == 0)
		return 0;

	if(CD_count_drives() == 0)
		return 0;

	ds_writew(0x246c, CD_get_first_drive());
	return 1;
}

void CD_driver_request(RealPt req) {
	reg_ax = 0x1510;
	reg_cx = ds_readw(0x246c);
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

void seg001_00bb(unsigned short track_nr)
{
	unsigned int track_start, track_end;
	unsigned int track_len, tmp;

	if (ds_readw(0x95) == 0)
		return;

	real_writew(relocation + CDSEG, 0x8f, 0);

	tmp = real_readd(relocation + CDSEG, 0x10a + track_nr * 8) & 0x00ffffff;
	real_writed(relocation + CDSEG, 0x9a, tmp);

	/* calculate track_start */
	tmp = real_readb(relocation + CDSEG, 0x10c + track_nr * 8) * 60;
	tmp += real_readb(relocation + CDSEG, 0x10b + track_nr * 8);
	tmp *= 75;
	tmp += real_readb(relocation + CDSEG, 0x10a + track_nr * 8);
	track_start = tmp;

	/* calculate track_end */
	if (real_readb(relocation + CDSEG, 0x422) == track_nr) {
		tmp = real_readb(relocation + CDSEG, 0x425) * 60;
		tmp += real_readb(relocation + CDSEG, 0x424);
		tmp *= 75;
		tmp += real_readb(relocation + CDSEG, 0x423);
	} else {
		tmp = real_readb(relocation + CDSEG, 0x114 + track_nr * 8) * 60;
		tmp += real_readb(relocation + CDSEG, 0x113 + track_nr * 8);
		tmp *= 75;
		tmp += real_readb(relocation + CDSEG, 0x112 + track_nr * 8);
	}
	track_end = tmp;

	track_len = track_end - track_start;
	real_writed(relocation + CDSEG, 0x9e, track_len - 150);

	CD_driver_request(RealMake(relocation + CDSEG, 0x8c));
	ds_writed(0x2468, ((track_len - 150) * 0x1234e) / 0x4b000);
	ds_writed(0x2464, CD_get_tod());
}

static void seg001_02ba()
{
	signed int val;

	if (ds_readw(0x95) == 0)
		return;

	val = CD_get_tod();
	val -= ds_readd(0x2464);

	if (val < ds_readd(0x2468))
		return;

	if (ds_readw(0x9b) != 1)
		return;

	seg001_0312();
	seg001_0312();
	seg001_00bb(ds_readw(0x245a));
	ds_writew(0x9b, 1);
}

signed short CD_bioskey(signed short cmd)
{
	seg001_02ba();
	return bioskey(cmd);
}

void seg001_0312()
{
	if (ds_readw(0x95) == 0)
		return;

	real_writew(relocation + CDSEG, 3, 0);
	CD_driver_request(RealMake(relocation + CDSEG, 0));
	ds_writew(0x9b, 0);
}

void seg001_033b()
{
	if (ds_readw(0x95) == 0)
		return;

	seg001_0312();
	real_writew(relocation + CDSEG, 0x1f, 0);
	CD_driver_request(RealMake(relocation + CDSEG, 0x1c));
}
void seg001_03a8()
{
	Bit16u v;

	if (ds_readw(0x95) == 0)
		return;

	real_writew(relocation + CDSEG, 0x3b, 0);
	real_writew(relocation + CDSEG, 0x48, relocation + CDSEG);
	real_writew(relocation + CDSEG, 0x46, 0x420);
	real_writeb(relocation + CDSEG, 0x420, 10);
	CD_driver_request(RealMake(relocation + CDSEG, 0x38));

	v = real_readb(relocation + CDSEG, 0x421);
	for (; real_readb(relocation + CDSEG, 0x422) >= v; v++) {
		real_writew(relocation + CDSEG, 0x3b, 0);
		real_writew(relocation + CDSEG, 0x48, relocation + CDSEG);
		real_writew(relocation + CDSEG, 0x46, 0x108 + v * 8);
		real_writeb(relocation + CDSEG, v * 8 + 0x108, 11);
		real_writeb(relocation + CDSEG, v * 8 + 0x109, (unsigned char)v);

		CD_driver_request(RealMake(relocation + CDSEG, 0x38));
	}

}

void seg001_0465()
{
	seg001_0312();
	seg001_0312();
	ds_writew(0x245a, 4);
	seg001_00bb(ds_readw(0x245a));
	ds_writew(0x9b, 1);
}

bool seg001_0600()
{
	if (CD_set_drive_nr() == 0)
		return false;

	ds_writew(0x95, 1);
	/* CHECK_CD() would have been called here */
	seg001_033b();
	seg001_03a8();

	return true;
}

}
