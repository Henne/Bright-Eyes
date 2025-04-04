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

#include <stdio.h>

#if !defined(__BORLANDC__)
#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "cpu.h"

#include "schick.h"
#endif

#include "symbols.h"

#include "g105de_seg000.h"
#include "g105de_seg001.h"
#include "g105de_seg002.h"

#define CDSEG (0xc83)

#if !defined(__BORLANDC__)
namespace G105de {
#endif

/* Borlandified and identical */
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

//	return _AX;
#else
	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	if (reg_bx == 0)
		return 0;

	return 1;
#endif
}

/* Borlandified and identical */
static unsigned short CD_count_drives()
{
#if defined(__BORLANDC__)
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}
		_AX = _BX;

//	return _BX;
#else

	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	return reg_bx;
#endif
}

/* Borlandified and identical */
static unsigned short CD_get_first_drive()
{
#if defined(__BORLANDC__)
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}

	_AX = _CX;
//	return _CX;
#else

	reg_ax = 0x1500;
	reg_bx = 0x0000;
	CALLBACK_RunRealInt(0x2f);

	return reg_cx;
#endif
}

/* Borlandified and identical */
unsigned short CD_set_drive_no()
{
	if (CD_has_drives() == 0) return 0;
	if (CD_count_drives() == 0) return 0;

	ds_writew(CD_DRIVE_NO, CD_get_first_drive());

#if !defined(__BORLANDC__)
	return 1;
#else
	_AX = 1;
#endif
}

/* Borlandified and identical */
#if defined(__BORLANDC__)
static void CD_driver_request(driver_request far* req)
#else
static void CD_driver_request(RealPt req)
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

/* Borlandified and far from identical, but unused (8 diffs)*/
/* TODO: check adresses of seg013 */
static void CD_unused1(void)
{
	if (ds_readw(CD_INIT_SUCCESSFUL) != 0) {

#if defined(__BORLANDC__)
	req[3].status = 0;
	req[3].ptr = cd_buf1;
	//cd_buf1[252] = 0x0c;
	asm { db 0x0f, 0x1f, 0x40, 0x00; } 	// BCC Sync-Point
	asm { db 0x0f, 0x1f, 0x40, 0x00; } 	// BCC Sync-Point
	CD_driver_request(&req[3]);
#else
	//DUMMY
#endif
	}
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
		//jmp near leave_tod
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

/* Seem Unborlandifiable to me */
void seg001_00bb(Bit16s track_no)
{
	Bit32s track_start;
	Bit32s track_end;
	unsigned int track_len, tmp;

	if (ds_readw(CD_INIT_SUCCESSFUL) != 0) {

		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x8f)), 0);

		host_writed(Real2Host(RealMake(reloc_gen + CDSEG, 0x9a)),
			(host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10b + track_no * 8))) << 8) +
			(host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10a + track_no * 8)))) +
			(host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10c + track_no * 8)))) << 16);

		/* calculate track_start */
		track_start = (60L * host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10b + track_no * 8)))
			+ host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10a + track_no * 8)))) * 75L
			+ host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10c + track_no * 8)));

		/* calculate track_end */
		if (host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x422))) == track_no) {

			track_end = (60L * host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x425))) +
					   host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x424)))) * 75L +
					   host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x423)));
		} else {
			track_end = (60L * host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x114 + track_no * 8))) +
					   host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x113 + track_no * 8)))) * 75L +
					   host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x112 + track_no * 8)));

		}

		track_start -= track_end;
		// track_start is now track length
		host_writed(Real2Host(RealMake(reloc_gen + CDSEG, 0x9e)), track_start - 150);

#if !defined(__BORLANDC__)
		CD_driver_request(RealMake(reloc_gen + CDSEG, 0x8c));
		ds_writed(CD_AUDIO_POS, ((track_start - 150) * 0x1234e) / 0x4b000);
#else
		CD_driver_request((driver_request*)RealMake(reloc_gen + CDSEG, 0x8c));

		asm { db 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x44, 0x00, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point

		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
#endif
		ds_writed(CD_AUDIO_TOD, CD_get_tod());
	}
}

/* Borlandified and nearly identical, but works */
static void seg001_02ba()
{
	if (ds_readw(CD_INIT_SUCCESSFUL) != 0) {

		if ((CD_get_tod() - (Bit32s)ds_readd(CD_AUDIO_TOD)) >= (Bit32s)ds_readd(CD_AUDIO_POS)) {

			if (ds_readw(CD_AUDIO_REPEAT) == 1) {
				seg001_0312();
				seg001_0312();
				seg001_00bb(ds_readw(CD_AUDIO_TRACK));
				ds_writew(CD_AUDIO_REPEAT, 1);
			}
		}
	}
}

/* Borlandified and identical */
signed short CD_bioskey(signed short cmd)
{
	seg001_02ba();

#if !defined(__BORLANDC__)
	return bc_bioskey(cmd);
#else
	// return
	bc_bioskey(cmd); // implicit return
#endif
}

/* Borlandified and nearly identical */
void seg001_0312()
{
	if (ds_readw(CD_INIT_SUCCESSFUL) != 0) {

		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 3)), 0);
#if !defined(__BORLANDC__)
		CD_driver_request(RealMake(reloc_gen + CDSEG, 0));
#else
		asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
		asm { db 0x0f, 0x1f, 0x00; }
		asm { db 0x0f, 0x1f, 0x00; }
		asm { nop; }
		asm { nop; }
#endif
		ds_writew(0x9b, 0);
	}
}

/* Borlandified and nearly identical */
void seg001_033b()
{
	if (ds_readw(CD_INIT_SUCCESSFUL) != 0) {

		seg001_0312();
		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x1f)), 0);
#if !defined(__BORLANDC__)
		CD_driver_request(RealMake(reloc_gen + CDSEG, 0x1c));
#else
		asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
		asm { db 0x0f, 0x1f, 0x00; }
		asm { db 0x0f, 0x1f, 0x00; }
		asm { nop; }
		asm { nop; }
#endif
	}
}

#if defined (__BORLANDC__)
/* Borlandified and nearly identical */
void CD_unused2()
{
	if (ds_readw(CD_INIT_SUCCESSFUL) != 0) {

		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0xab)), 0);
		//CD_driver_request(RealMake(reloc_gen + CDSEG, 0xa8));

		asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
		asm { db 0x0f, 0x1f, 0x00; }
		asm { db 0x0f, 0x1f, 0x00; }
		asm { nop; }
		asm { nop; }
	}
}
/* Borlandified and nearly identical */
void CD_unused3()
{
	if (ds_readw(CD_INIT_SUCCESSFUL) != 0) {

		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0xc7)), 0);
		//CD_driver_request(RealMake(reloc_gen + CDSEG, 0xc4));

		asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
		asm { db 0x0f, 0x1f, 0x00; }
		asm { db 0x0f, 0x1f, 0x00; }
		asm { nop; }
		asm { nop; }
	}
}
#endif

/* Borlandified and nearly identical */
void seg001_03a8()
{
	Bit16u v;

	if (ds_readw(CD_INIT_SUCCESSFUL) == 0) {

		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x3b)), 0);
		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x48)), reloc_gen + CDSEG);
		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x46)), 0x420);
		host_writeb(Real2Host(RealMake(reloc_gen + CDSEG, 0x420)), 10);
#if !defined(__BORLANDC__)
		CD_driver_request(RealMake(reloc_gen + CDSEG, 0x38));
#else
		CD_driver_request((driver_request*)RealMake(reloc_gen + CDSEG, 0x38));
#endif

		v = host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x421)));
		for (; host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x422))) >= v; v++) {
			host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x3b)), 0);
			host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x48)), reloc_gen + CDSEG);
#if !defined(__BORLANDC__)
			host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x46)), 0x108 + v * 8);
#else
			asm { db 0x66, 0x90; } // BCC Sync-Point
			asm { db 0x66, 0x90; }
			asm { nop; }
#endif
			host_writeb(Real2Host(RealMake(reloc_gen + CDSEG, v * 8 + 0x108)), 11);
			host_writeb(Real2Host(RealMake(reloc_gen + CDSEG, v * 8 + 0x109)), (unsigned char)v);

#if !defined(__BORLANDC__)
			CD_driver_request(RealMake(reloc_gen + CDSEG, 0x38));
#else
			CD_driver_request((driver_request*)RealMake(reloc_gen + CDSEG, 0x38));
#endif
		}
	}
}

/* Borlandified and identical */
void seg001_0465(unsigned short track)
{
	seg001_0312();
	seg001_0312();
	ds_writew(CD_AUDIO_TRACK, 4);
	seg001_00bb(ds_readw(CD_AUDIO_TRACK));
	ds_writew(0x9b, 1);
}

/* Borlandified and nearly identical, but works correctly */
//static
Bit16s CD_check_file(char *pathP)
{
	int handle;
	Bit16s buf;
	unsigned int nread;
	
#if !defined(__BORLANDC__)
	if (bc__dos_open((char*)pathP, 1, (Bit8u*)&handle)) return -1;

	if (bc__dos_read(handle, (Bit8u*)&buf, 1, (Bit16u*)&nread)) return -1;

	bc_lseek(handle, 2000L, 0);

	if (bc__dos_read(handle, (Bit8u*)&buf, 1, (Bit16u*)&nread)) return -1;

#else
	if (bc__dos_open((char*)pathP, 1, &handle)) return -1;

	if (bc__dos_read(handle, (Bit8u*)&buf, 1, &nread)) return -1;

	bc_lseek(handle, 2000L, 0);

	if (bc__dos_read(handle, (Bit8u*)&buf, 1, &nread)) return -1;
#endif

	bc__dos_close(handle);

#if !defined(__BORLANDC__)	
	return nread;
#else
	asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
#endif
}

/* Borlandified and identical */
void CD_radio_insert_cd()
{
	char text_buffer[160];

	Bit16s si;
	
	sprintf(text_buffer, (char*)Real2Host(RealMake(datseg, STR_INSERT_CD)), ds_readw(CD_DRIVE_NO) + 'A');

	si = -2;
	
	while (si == -2) {

		si = gui_radio((Bit8u*)text_buffer,
				2,
				(char*)Real2Host(RealMake(datseg, STR_REPEAT)),	
				(char*)Real2Host(RealMake(datseg, STR_QUIT)));	
	}

	if (si == 2) {
		stop_music();
		update_mouse_cursor();
		mouse_disable();
		restore_timer_isr();
		
		if (ds_readw(CALLED_WITH_ARGS) != 0) {
			call_fill_rect_gen((RealPt)ds_readd(VGA_MEMSTART), 0, 0, 319, 199, 0);
		} else {
			exit_video();
			bc_clrscr();
		}
		bc_exit(0);
	}
}

/* Borlandified and identical */
Bit16s CD_insert_loop()
{
	if (ds_readw(CD_INSERT_COUNTER) == 0) {
		CD_radio_insert_cd();
		ds_writew(CD_INSERT_COUNTER, 5);
	}
	ds_dec_ws(CD_INSERT_COUNTER);
#if defined(__BORLANDC__)
	bc__exit(0);
#endif

#if !defined(__BORLANDC__)
	return 1;
#else
	asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
#endif
}

/* Borlandified and nearly identical */
void CD_check_cd()
{
	char fname[80];

#if !defined(__BORLANDC__)
	bc_harderr(RealMake(reloc_gen + 0x364, 0x598));
#else
	bc_harderr((int(*)(int, int, int, int))CD_insert_loop);
#endif

	strcpy(fname, (char*)Real2Host(RealMake(datseg, STR_CD_EXEPATH)));
	fname[0] = ds_readbs(CD_DRIVE_NO) + 'A';

	while (CD_check_file(fname) <= 0) {
		CD_radio_insert_cd();
	}
}

/* Borlandified and identical */
signed short seg001_0600()
{
	if (CD_set_drive_no() == 0)
		return 0;

	ds_writew(CD_INIT_SUCCESSFUL, 1);
	CD_check_cd();
	seg001_033b();
	seg001_03a8();

#if !defined(__BORLANDC__)
	return 1;
#else
	asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
#endif
}

#if !defined(__BORLANDC__)
}
#endif
