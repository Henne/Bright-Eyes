/*
 *	Rewrite of DSA1 GEN v1.05_de functions of seg001 (cdrom)
 *	Functions rewritten: 21/21 (complete)
 *
 *	Remarks:
 *		The first part of this file is for inclusion in DOSBox.
 *		Aim here is, that they work identical.
 *
 *		The second is a rewrite using the inline assembler of
 *		Borland C++ 3.1. The aim is, to produce exactly the same
 *		OPcodes like in the original. Seems to work. :)
 *
 *	MSCDEX:
 *		In this segment are mostly calls to the MSCDEX-API.
 *		It seems, that MSCDEX 2.1 is needed,
 *		since function 0x10 (driver request) is called.
 *		Further this code is written only on the first installed
 *		CD-ROM drive.
 *
 *
 */

#if !defined(__BORLANDC__)
#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "cpu.h"

#include "schick.h"
#else
#include "cda.h"
#endif



#include "symbols.h"

#include "g105de_seg000.h"
#include "g105de_seg001.h"

#define CDSEG (0xc83)

#if !defined(__BORLANDC__)
namespace G105de {
#endif

static unsigned short CD_has_drives()
{
#if defined(__BORLANDC__)
	/* al ==  0: return number of drive letters */
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
		xor ax, ax
		or  bx, bx
		jz has_cd
		inc ax
	}
has_cd:

	return _AX;
#else
	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	if (reg_bx == 0)
		return 0;

	return 1;
#endif
}

static unsigned short CD_count_drives()
{
#if defined(__BORLANDC__)
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}

	return _BX;
#else

	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	return reg_bx;
#endif
}

static unsigned short CD_get_first_drive()
{
#if defined(__BORLANDC__)
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}

	return _CX;
#else

	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	return reg_cx;
#endif
}

unsigned short CD_set_drive_no()
{

	if (CD_has_drives() == 0)
		return 0;

	if (CD_count_drives() == 0)
		return 0;

	ds_writew(CD_DRIVE_NO, CD_get_first_drive());
	return 1;
}

#if defined(__BORLANDC__)
void CD_driver_request(driver_request *req)
#else
void CD_driver_request(RealPt req)
#endif
{
#if defined(__BORLANDC__)
	asm {
		mov ax, 0x1510
		mov cx, [CD_DRIVE_NO]
		les bx, req
		int 0x2f
	}
#else
	reg_ax = 0x1510;
	reg_cx = ds_readw(CD_DRIVE_NO);
	CPU_SetSegGeneral(es, RealSeg(req));
	reg_bx = RealOff(req);
	CALLBACK_RunRealInt(0x2f);
#endif
}

/* Borlandified and identical */
/* TODO: check adresses of seg013 */
static void CD_unused1(void)
{
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;
#if defined(__BORLANDC__)
	req[3].status = 0;
	req[3].ptr = cd_buf1;
	cd_buf1[252] = 0x0c;
	CD_driver_request(&req[3]);
#else
	//DUMMY
#endif
}


/**
 * \brief   get time of day
 *
 * \return              clock ticks since midnight, the system time.
 *
 * \todo    produces a compiler warning and is a bit hacky
 */
/* Borlandified and identical */
Bit32s CD_get_tod(void)
{
#if defined(__BORLANDC__)
	asm {
		mov ah, 0x0
		int 0x1a
		mov ax, dx
		mov dx, cx
		jmp near leave_tod
	}
leave_tod:
#else
	reg_ah = 0;
	CALLBACK_RunRealInt(0x1a);
	reg_ax = reg_dx;
	reg_dx = reg_cx;

	return (reg_dx << 16) | reg_ax;
#endif
}

void seg001_00bb(unsigned short track_no)
{
	unsigned int track_start, track_end;
	unsigned int track_len, tmp;

	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	real_writew(reloc_gen + CDSEG, 0x8f, 0);

	tmp = real_readd(reloc_gen + CDSEG, 0x10a + track_no * 8) & 0x00ffffff;
	real_writed(reloc_gen + CDSEG, 0x9a, tmp);

	/* calculate track_start */
	tmp = real_readb(reloc_gen + CDSEG, 0x10c + track_no * 8) * 60;
	tmp += real_readb(reloc_gen + CDSEG, 0x10b + track_no * 8);
	tmp *= 75;
	tmp += real_readb(reloc_gen + CDSEG, 0x10a + track_no * 8);
	track_start = tmp;

	/* calculate track_end */
	if (real_readb(reloc_gen + CDSEG, 0x422) == track_no) {
		tmp = real_readb(reloc_gen + CDSEG, 0x425) * 60;
		tmp += real_readb(reloc_gen + CDSEG, 0x424);
		tmp *= 75;
		tmp += real_readb(reloc_gen + CDSEG, 0x423);
	} else {
		tmp = real_readb(reloc_gen + CDSEG, 0x114 + track_no * 8) * 60;
		tmp += real_readb(reloc_gen + CDSEG, 0x113 + track_no * 8);
		tmp *= 75;
		tmp += real_readb(reloc_gen + CDSEG, 0x112 + track_no * 8);
	}
	track_end = tmp;

	track_len = track_end - track_start;
	real_writed(reloc_gen + CDSEG, 0x9e, track_len - 150);

	CD_driver_request(RealMake(reloc_gen + CDSEG, 0x8c));
	ds_writed(CD_AUDIO_POS, ((track_len - 150) * 0x1234e) / 0x4b000);
	ds_writed(CD_AUDIO_TOD, CD_get_tod());
}

static void seg001_02ba()
{
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	if (CD_get_tod() - (Bit32s)ds_readd(CD_AUDIO_TOD) < (Bit32s)ds_readd(CD_AUDIO_POS))
		return;

	if (ds_readw(CD_AUDIO_REPEAT) == 1) {
		seg001_0312();
		seg001_0312();
		seg001_00bb(ds_readw(CD_AUDIO_TRACK));
		ds_writew(CD_AUDIO_REPEAT, 1);
	}
}

signed short CD_bioskey(signed short cmd)
{
	seg001_02ba();
	return bc_bioskey(cmd);
}

void seg001_0312()
{
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	real_writew(reloc_gen + CDSEG, 3, 0);
	CD_driver_request(RealMake(reloc_gen + CDSEG, 0));
	ds_writew(0x9b, 0);
}

void seg001_033b()
{
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	seg001_0312();
	real_writew(reloc_gen + CDSEG, 0x1f, 0);
	CD_driver_request(RealMake(reloc_gen + CDSEG, 0x1c));
}
void seg001_03a8()
{
	Bit16u v;

	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	real_writew(reloc_gen + CDSEG, 0x3b, 0);
	real_writew(reloc_gen + CDSEG, 0x48, reloc_gen + CDSEG);
	real_writew(reloc_gen + CDSEG, 0x46, 0x420);
	real_writeb(reloc_gen + CDSEG, 0x420, 10);
	CD_driver_request(RealMake(reloc_gen + CDSEG, 0x38));

	v = real_readb(reloc_gen + CDSEG, 0x421);
	for (; real_readb(reloc_gen + CDSEG, 0x422) >= v; v++) {
		real_writew(reloc_gen + CDSEG, 0x3b, 0);
		real_writew(reloc_gen + CDSEG, 0x48, reloc_gen + CDSEG);
		real_writew(reloc_gen + CDSEG, 0x46, 0x108 + v * 8);
		real_writeb(reloc_gen + CDSEG, v * 8 + 0x108, 11);
		real_writeb(reloc_gen + CDSEG, v * 8 + 0x109, (unsigned char)v);

		CD_driver_request(RealMake(reloc_gen + CDSEG, 0x38));
	}

}

void seg001_0465(unsigned short track)
{
	seg001_0312();
	seg001_0312();
	ds_writew(0x245a, 4);
	seg001_00bb(ds_readw(0x245a));
	ds_writew(0x9b, 1);
}

signed short seg001_0600()
{
	if (CD_set_drive_no() == 0)
		return 0;

	ds_writew(CD_INIT_SUCCESSFUL, 1);
	/* CHECK_CD() would have been called here */
	seg001_033b();
	seg001_03a8();

	return 1;
}

#if !defined(__BORLANDC__)
}
#endif
