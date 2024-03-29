/*
 *	Rewrite of DSA1 v3.02_de functions of seg001 (cdrom)
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
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg001.h"
#include "seg097.h"
#include "seg120.h"

#define CDA_DATASEG (0x1238)

#if defined(__BORLANDC__)
#include "seg013.h"
#else
#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "cpu.h"
#endif


#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   check if cdrom-drives are installed
 *
 * \return              0 - no drive, 1 - at least one drive
 */
/* Borlandified and identical */
static unsigned short CD_has_drives(void)
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

/* Borlandified and identical */
static unsigned short CD_count_drives(void)
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

/* Borlandified and identical */
static unsigned short CD_get_first_drive(void)
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

/* Borlandified and identical */
unsigned short CD_set_drive_no(void)
{

	if (CD_has_drives() == 0)
		return 0;

	if (CD_count_drives() == 0)
		return 0;

	ds_writew(CD_DRIVE_NO, CD_get_first_drive());
	return 1;
}

/* Borlandified and identical */
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

/* Borlandified and nearly identical */
void seg001_00c1(signed short track_no)
{
	Bit32s track_start;
	Bit32s track_end;

#if defined(__BORLANDC__)
	asm {
		mov si, track_no
	}

	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	host_writew(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x8f)), 0);

	/* TODO: write this code in C */
	asm {

		mov bx, si
		shl bx, 3
		mov ax, CDA_DATASEG
		mov es, ax
		mov al, [es:bx+0x10b]
		mov ah, 0
		shl ax, 8

		mov bx, si
		shl bx, 3
		mov dx, CDA_DATASEG
		mov es, dx
		mov dl, [es:bx+0x10a]
		mov dh, 0

		add ax,dx

		mov bx, si
		shl bx, 3
		mov dx, CDA_DATASEG
		mov es, dx
		mov dl, [es:bx+0x10c]
		mov dh, 0

		mov bx, CDA_DATASEG
		mov es, bx
		mov [es:0x9c], dx
		mov [es:0x9a], ax

	}
	/* calculate track_start */
	track_start = (
				((60UL * (Bit16u)host_readb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x10c + _SI * 8)))) +
				((Bit16s)host_readb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x10b + _SI * 8))))) * 75UL +
			((Bit16s)host_readb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x10a + _SI * 8))))
			);

	/* calculate track_end */
	if (real_readb(reloc_game + CDA_DATASEG, 0x422) == _SI)
	{
		track_end = (((60UL * (Bit16u)real_readb(reloc_game + CDA_DATASEG, 0x425) +
			(Bit16s)real_readb(reloc_game + CDA_DATASEG, 0x424)) * 75UL) +
			(Bit16s)real_readb(reloc_game + CDA_DATASEG, 0x423));
	} else {
		track_end = (((60UL * (Bit16u)real_readb(reloc_game + CDA_DATASEG, 0x114 + _SI * 8) +
			(Bit16s)real_readb(reloc_game + CDA_DATASEG, 0x113 + _SI * 8)) * 75UL) +
			(Bit16s)real_readb(reloc_game + CDA_DATASEG, 0x112 + _SI * 8));
	}

#else

	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	host_writew(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x8f)), 0);

	host_writed(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x9a)),
		((((Bit16u)host_readb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x10b + track_no * 8))) << 8) +
		(Bit16u)host_readb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x10a + track_no * 8)))) +
		((Bit16u)host_readb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x10c + track_no * 8))) << 16)));

	/* calculate track_start */
	track_start = ((host_readb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x10c + track_no * 8))) * 60) +
		host_readb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x10b + track_no * 8)))) * 75 +
		host_readb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x10a + track_no * 8)));

	/* calculate track_end */
	if (real_readb(reloc_game + CDA_DATASEG, 0x422) == track_no)
	{
		track_end = (((60UL * (Bit16u)real_readb(reloc_game + CDA_DATASEG, 0x425) +
			(Bit16s)real_readb(reloc_game + CDA_DATASEG, 0x424)) * 75UL) +
			(Bit16s)real_readb(reloc_game + CDA_DATASEG, 0x423));
	} else {
		track_end = (((60UL * (Bit16u)real_readb(reloc_game + CDA_DATASEG, 0x114 + track_no * 8) +
			(Bit16s)real_readb(reloc_game + CDA_DATASEG, 0x113 + track_no * 8)) * 75UL) +
			(Bit16s)real_readb(reloc_game + CDA_DATASEG, 0x112 + track_no * 8));
	}

#endif

	track_start = track_end - track_start;
	host_writew(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x9e)), ((Bit16u)track_start) - 150);
	host_writew(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0xa0)), (Bit32s)(track_start >> 16));

#if defined(__BORLANDC__)
	CD_driver_request((driver_request*)RealMake(reloc_game + CDA_DATASEG, 0x8c));
#else
	CD_driver_request(RealMake(reloc_game + CDA_DATASEG, 0x8c));
#endif

	ds_writed(CD_AUDIO_POS, (((Bit32u)track_start - 150) * 0x1234e) / 0x4b000);
	ds_writed(CD_AUDIO_TOD, CD_get_tod());
}

/* Borlandified and identical */
void seg001_02c4(void)
{
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	if (CD_get_tod() - (Bit32s)ds_readd(CD_AUDIO_TOD) < (Bit32s)ds_readd(CD_AUDIO_POS))
		return;

	if (ds_readw(CD_AUDIO_REPEAT) == 1)
	{

		CD_audio_stop_hsg();
		CD_audio_stop_hsg();
		seg001_00c1(ds_readw(CD_AUDIO_TRACK));
		ds_writew(CD_AUDIO_REPEAT, 1);
	}
}

/* Borlandified and identical */
signed short CD_bioskey(signed short cmd)
{
	seg001_02c4();
	return bc_bioskey(cmd);
}

/* CD_audio_stop_hsg() - stop audio playback in HSG format */
/* static */
/* Borlandified and identical */
void CD_audio_stop_hsg(void)
{

	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

#if defined(__BORLANDC__)
	req[0].status = 0;
	CD_driver_request(&req[0]);
#else
	real_writew(reloc_game + CDA_DATASEG, 3, 0);
	CD_driver_request(RealMake(reloc_game + CDA_DATASEG, 0));
#endif

	ds_writew(CD_AUDIO_REPEAT, 0);
}

/**
 *\brief	stop audio playback in HSG and redbook format
 */
/* Borlandified and identical */
void CD_audio_stop(void)
{
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	CD_audio_stop_hsg();
#if defined(__BORLANDC__)
	req[1].status = 0;
	CD_driver_request(&req[1]);
#else
	real_writew(reloc_game + CDA_DATASEG, 0x1f, 0);
	CD_driver_request(RealMake(reloc_game + CDA_DATASEG, 0x1c));
#endif
}

/* Borlandified and identical */
void CD_audio_pause(void)
{
	/* Is CD initialized ? */
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	/* Is CD already paused ? */
	if (ds_readw(CD_AUDIO_PAUSED) != 0)
		return;

	/* set CD pause */
	ds_writew(CD_AUDIO_PAUSED, 1);
	ds_writed(CD_AUDIO_PAUSE_TOD, CD_get_tod());
	/* save current position */
	ds_writed(CD_AUDIO_PAUSE_POS, ds_readd(CD_AUDIO_POS));
	/* set current position to maximum singned int */
	ds_writed(CD_AUDIO_POS, 0x7fffffff);

	host_writew(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0xab)), 0);
#if defined(__BORLANDC__)
	CD_driver_request((driver_request*)RealMake(reloc_game + CDA_DATASEG, 0xa8));
#else
	CD_driver_request(RealMake(reloc_game + CDA_DATASEG, 0xa8));
#endif
}

/* Borlandified and identical */
void CD_audio_play(void)
{
	/* Is CD initialized ? */
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	/* Is CD paused ? */
	if (ds_readw(CD_AUDIO_PAUSED) == 0)
		return;

	CD_check();

	/* reset CD pause */
	ds_writew(CD_AUDIO_PAUSED, 0);
	ds_writed(CD_AUDIO_POS, ds_readd(CD_AUDIO_PAUSE_POS));
	add_ds_ds(CD_AUDIO_TOD, (CD_get_tod() - ds_readds(CD_AUDIO_PAUSE_TOD)));
	host_writew(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0xc7)), 0);
#if defined(__BORLANDC__)
	CD_driver_request((driver_request*)RealMake(reloc_game + CDA_DATASEG, 0xc4));
#else
	CD_driver_request(RealMake(reloc_game + CDA_DATASEG, 0xc4));
#endif
}

/* Borlandified and nearly identical */
void CD_0432(void)
{
	signed short track_no;

	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	host_writew(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x3b)), 0);
	host_writed(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x46)), (Bit32u)(RealMake(reloc_game + CDA_DATASEG, 0x420)));
	host_writeb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x420)), 0x0a);
#if defined(__BORLANDC__)
	/* BC-TODO: this constant ist pushed as a byte instead of a word */
	CD_driver_request((driver_request*)RealMake(reloc_game + CDA_DATASEG, 0x38));
	asm {nop}
#else
	CD_driver_request(RealMake(reloc_game + CDA_DATASEG, 0x38));
#endif

	track_no = host_readb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x421)));

	while (host_readb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x422))) >= track_no)
	{
		host_writew(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x3b)), 0);
		host_writed(Real2Host(RealMake(reloc_game + CDA_DATASEG, 0x46)), (Bit32u)(RealMake(reloc_game + CDA_DATASEG, 8 * track_no + 0x108)));
		host_writeb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 8 * track_no + 0x108)), 0x0b);
		host_writeb(Real2Host(RealMake(reloc_game + CDA_DATASEG, 8 * track_no + 0x109)), track_no);

#if defined(__BORLANDC__)
		/* BC-TODO: this constant ist pushed as a byte instead of a word */
		CD_driver_request((driver_request*)RealMake(reloc_game + CDA_DATASEG, 0x38));
		asm{nop}
#else
		CD_driver_request(RealMake(reloc_game + CDA_DATASEG, 0x38));
#endif
		track_no++;
	}
}

struct dummy15 {
	signed short a[15];
};

/* Borlandified and identical */
void CD_set_track(signed short index)
{
	signed short i;
#if defined(__BORLANDC__)
	struct dummy15 tracks = *(struct dummy15*)(p_datseg + CD_AUDIO_TRACKLIST);
#else
	struct dummy15 tracks;
	tracks.a[0] = 0x7fff;
	tracks.a[1] = 0x7fff;
	tracks.a[2] = 0x7fff;
	tracks.a[3] = 0x7fff;
	tracks.a[4] = ARCHIVE_FILE_SUMMARY_XMI;
	tracks.a[5] = ARCHIVE_FILE_THORWAL_XMI;
	tracks.a[6] = ARCHIVE_FILE_TEMPLE_XMI;
	tracks.a[7] = ARCHIVE_FILE_TERMS_XMI;
	tracks.a[8] = ARCHIVE_FILE_SMITH_XMI;
	tracks.a[9] = ARCHIVE_FILE_INN_XMI;
	tracks.a[10] = ARCHIVE_FILE_HEALER_XMI;
	tracks.a[11] = ARCHIVE_FILE_CAMP_XMI;
	tracks.a[12] = ARCHIVE_FILE_VICTORY_XMI;
	tracks.a[13] = ARCHIVE_FILE_COMBAT_XMI;
	tracks.a[14] = ARCHIVE_FILE_DUNGEON_XMI;
#endif

	for (i = 0; i < 15; i++)
	{
		if (tracks.a[i] == index) break;
	}

	ds_writew(CD_AUDIO_TRACK, i + 1);

	if (ds_readw(CD_AUDIO_TRACK_BAK) == ds_readw(CD_AUDIO_TRACK))
	{
	} else {
		ds_writew(CD_AUDIO_TRACK_BAK, ds_readw(CD_AUDIO_TRACK));
		CD_audio_stop_hsg();
		CD_audio_stop_hsg();

		seg001_00c1(ds_readw(CD_AUDIO_TRACK));

		ds_writew(CD_AUDIO_REPEAT, 1);

		if (ds_readw(CD_AUDIO_PAUSED) != 0)
		{
			ds_writew(CD_AUDIO_PAUSED, 0);
			CD_audio_pause();
		}
	}
}

/* Borlandified and identical */
signed short CD_read_exe(char *path)
{
	signed short fd;
	signed short buffer;
	unsigned short nread;

	/* skip read check */
	if (ds_readd(CD_CHECK_SKIPMAGIC) == 0x682772e4) return 1;

#if defined(__BORLANDC__)
	if (bc__dos_open(path, 1, (int*)&fd)) return -1;

	if (bc__dos_read(fd, &buffer, 1, (unsigned int*)&nread) != 0) return -1;

	bc_lseek(fd, 2000L, 0);

	if (bc__dos_read(fd, &buffer, 1, (unsigned int*)&nread) != 0) return -1;
#else
	if (bc__dos_open(path, 1, &fd)) return -1;
	/* BE-fix */
	fd = host_readws((Bit8u*)&fd);

	if (bc__dos_read(fd, &buffer, 1, &nread) != 0) return -1;

	bc_lseek(fd, 2000L, 0);

	if (bc__dos_read(fd, &buffer, 1, &nread) != 0) return -1;
	/* BE-fix */
	nread = host_readw((Bit8u*)&nread);
#endif

	bc__dos_close(fd);
	return nread;
}

/* Borlandified and identical */
void CD_insert_msg(void)
{
	signed short answer;
	char str[160];

	sprintf(str, (char*)p_datseg + STR_INSERT_CD, ds_readws(CD_DRIVE_NO) + 'A');

	answer = -2;
	while (answer == -2)
	{
		answer = GUI_radio((Bit8u*)str, 2,
					(char*)p_datseg + STR_REPEAT,
					(char*)p_datseg + STR_QUIT);
	}

	if (answer == 2)
	{
		cleanup_game();
#if !defined(__BORLANDC__)
		D1_INFO("\nCHANGED BEHAVIOUR: For technical reasons Bright-Eyes must be started anew\n\n");
		fflush(stdout);
#endif
		exit(0);
	}

}

/* Borlandified and identical */
signed short CD_harderr_handler(void)
{
	if (ds_readw(CD_CHECK_ERR_COUNTDOWN) == 0)
	{
		CD_insert_msg();
		ds_writew(CD_CHECK_ERR_COUNTDOWN, 5);
	}

	dec_ds_ws_post(CD_CHECK_ERR_COUNTDOWN);

	bc_hardresume(1);

	return 1;
}

/* Borlandified and identical */
void CD_check(void)
{
	char text[80];

#if defined(__BORLANDC__)
	bc_harderr((int(*)(int, int, int, int))CD_harderr_handler);
#else
	bc_harderr(RealMake(reloc_game + 0x4ac, 0x65a));
#endif

	strcpy(text, (char*)p_datseg + STR_CD_EXEPATH);

	text[0] = ds_readw(CD_DRIVE_NO) + 'A';

	while (CD_read_exe(text) <= 0)
	{
		CD_insert_msg();
	}
}

/* Borlandified and identical */
signed short CD_init(void)
{
	char str[80];

	if (!CD_set_drive_no())
	{
		GUI_output(p_datseg + STR_CD_MISSING);

		return 0;
	}

	sprintf(str, (char*)p_datseg + STR_CD_INIT, ds_readws(CD_DRIVE_NO) + 'A');

	GUI_output((Bit8u*)str);

	ds_writew(CD_INIT_SUCCESSFUL, 1);

	CD_audio_stop();

	CD_check();

	CD_0432();

	return 1;
}

#if !defined(__BORLANDC__)
}
#endif
