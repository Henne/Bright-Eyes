/*
 *	Rewrite of DSA1 v3.02_de functions of seg002 (misc)
 *	Functions rewritten: 145/148
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#if !defined(__BORLANDC__)
#include "callback.h"
#include "regs.h"
#include "dos_inc.h"
#endif

#include "v302de.h"

#include "seg000.h"
#include "seg001.h"
#include "seg002.h"
#include "seg004.h"
#include "seg006.h"
#include "seg007.h"
#include "seg008.h"
#include "seg009.h"
#include "seg010.h"
#include "seg011.h"
#include "seg025.h"
#include "seg026.h"
#include "seg029.h"
#include "seg030.h"
#include "seg039.h"
#include "seg041.h"
#include "seg047.h"
#include "seg048.h"
#include "seg049.h"
#include "seg050.h"
#include "seg066.h"
#include "seg076.h"
#include "seg095.h"
#include "seg096.h"
#include "seg097.h"
#include "seg106.h"
#include "seg108.h"
#include "seg113.h"
#include "seg119.h"
#include "seg120.h"
#include "seg121.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
/* static */
void play_music_file(signed short index)
{
	if (ds_readbs(0x4476) != 0) {
		do_play_music_file(index);
	}
}

/* Borlandified and identical */
void set_audio_track(Bit16u index)
{
	CD_check();

	/* only do something when index is not the current track */
	if (ds_readw(0x447a) != index) {

		ds_writew(0x447a, index);

		if (ds_readw(0xbcfd) != 0) {
			/* we use CD */
			CD_set_track(index);
		} else {
			/* we use AIL */
			play_music_file(index);
		}
	}
}

/* Borlandified and identical */
void sound_menu(void)
{
	signed short answer;

	answer = GUI_radio(p_datseg + 0x47d8, 4,
				p_datseg + 0x47e9,
				p_datseg + 0x47f5,
				p_datseg + 0x47ff,
				p_datseg + 0x480b);

	switch (answer - 1) {
		case 0: {
			ds_writeb(0x4476, 0);
			ds_writeb(0x4477, 0);
			break;
		}
		case 1: {
			ds_writeb(0x4476, 1);
			ds_writeb(0x4477, 0);
			break;
		}
		case 2: {
			ds_writeb(0x4476, 0);
			ds_writeb(0x4477, 1);
			break;
		}
		case 3: {
			ds_writeb(0x4476, 1);
			ds_writeb(0x4477, 1);
			break;
		}
	}

	if (ds_readb(0x4476) == 0) {
		/* music disabled */
		if (ds_readw(0xbcfd) != 0) {
			CD_audio_pause();
		} else {
			stop_midi_playback();
		}
	} else {
		if (ds_readws(0x447a) != -1) {
			/* music enabled */
			if (ds_readw(0xbcfd) != 0) {
				CD_audio_play();
			} else {
				play_music_file(ds_readws(0x447a));
			}
		}
	}
}

/* Borlandified and identical */
void read_sound_cfg(void)
{
	signed short port;
	signed short l2;
	signed short l3;
	signed short l4;
	signed short handle;

	/* try to open SOUND.CFG */
	if ( (handle = bc__open((RealPt)RealMake(datseg, 0x481d), 0x8001)) != -1) {

		bc__read(handle, (Bit8u*)&port, 2);
		bc__read(handle, (Bit8u*)&l2, 2);
		bc__read(handle, (Bit8u*)&l3, 2);
		bc__read(handle, (Bit8u*)&l4, 2);
		bc_close(handle);

#if !defined(__BORLANDC__)
		/* be byte-ordering independent */
		port = host_readws((Bit8u*)&port);
		l2 = host_readws((Bit8u*)&l2);
		l3 = host_readws((Bit8u*)&l3);
		l4 = host_readws((Bit8u*)&l4);
#endif

		/* enable useage of audio-CD */
		ds_writew(0xbcfd, ds_writew(0xbcff, 1));

		/* disable loading of the music driver */
		if (0) {

			if (port != 0) {
				load_music_driver((RealPt)RealMake(datseg, 0x4827), 3, port);
			} else {

				/* music was disabled in SOUND.CFG */
				if (NOT_NULL(Real2Host(ds_readd(0xbd0d)))) {
					bc_farfree((RealPt)ds_readd(0xbd0d));
				}

				ds_writed(0xbd0d, 0);
			}
		}

		if (l3 != 0) {

			if (ds_readw(0x447c) != 0) {

				if (!load_digi_driver((RealPt)RealMake(datseg, 0x4831), 2, l3, l4)) {
					ds_writew(0x447c, 0);
				}
			} else {
				/* print that sound effects are disabled */
				GUI_output(p_datseg + 0x483a);
				ds_writew(0x447c, 0);
			}
		} else {
			ds_writew(0x447c, 0);
		}

	}
}

/* Borlandified and identical */
void init_AIL(Bit32u size)
{
	if (NOT_NULL(Real2Host((RealPt)ds_writed(0xbd0d, (Bit32u)schick_alloc_emu(size))))) {
		AIL_startup();
		ds_writew(0xbcff, 1);
	}
}

/* Borlandified and identical */
void exit_AIL(void)
{
	AIL_shutdown(NULL);

	if (ds_readd(0xbd11) != 0) {
		bc_farfree((RealPt)ds_readd(0xbd11));
	}

	if (ds_readd(0xbd15) != 0) {
		bc_farfree((RealPt)ds_readd(0xbd15));
	}

	if (ds_readd(0xbd0d) != 0) {
		bc_farfree((RealPt)ds_readd(0xbd0d));
	}

	if (ds_readd(0xbd09) != 0) {
		bc_farfree((RealPt)ds_readd(0xbd09));
	}

	/* set all pointers to NULL */
	ds_writed(0xbd11, ds_writed(0xbd15, ds_writed(0xbd0d, ds_writed(0xbd09, 0))));

	if (ds_readw(0x447c) != 0) {
		free_voc_buffer();
	}

}

/* Borlandified and identical */
RealPt read_music_driver(RealPt fname)
{
	Bit32u len;
	RealPt buf;
	Bit32u ptr;

	signed short handle;

	if ( (handle = bc__open(fname, 0x8001)) != -1) {

		len = 16500L;

		ds_writed(0xbd09, (Bit32u)schick_alloc_emu(len + 16L));
		/* insane pointer casting */
		ptr = (ds_readd(0xbd09) + 15L);
		ptr &= 0xfffffff0;
		buf = EMS_norm_ptr((RealPt)ptr);
		/* and_ptr_ds((Bit8u*)&ptr, 0xfffffff0); */
		bc__read(handle, Real2Host(buf), len);
		bc_close(handle);
		return buf;
	}
	return (RealPt)0;
}

/* Borlandified and identical */
/* static */
signed short prepare_midi_playback(signed short sequence)
{
	volatile signed short patch;
	RealPt ptr;
	unsigned short l_si;
	unsigned short l_di;

	if ((ds_writews(0xbd01, load_archive_file(0x97))) != -1) {

		if ((ds_writews(0xbd21, AIL_register_sequence(ds_readw(0xbd23), (RealPt)ds_readd(0xbd0d), sequence, (RealPt)ds_readd(0xbd15), 0))) != -1) {

			while ( (l_si = AIL_timbre_request(ds_readw(0xbd23), ds_readw(0xbd21))) != -1)
			{
				l_di = l_si >> 8;
				patch = l_si & 0xff;

				if ( (ptr = prepare_timbre(l_di, patch))) {
					AIL_install_timbre(ds_readw(0xbd23), l_di, patch, ptr);
					bc_farfree(ptr);
				}
			}

			bc_close(ds_readw(0xbd01));
			return 1;
		}

		bc_close(ds_readw(0xbd01));
	}
	return 0;
}

/* Borlandified and identical */
/* static */
signed short start_midi_playback(signed short seq)
{
	if (prepare_midi_playback(seq)) {
		AIL_start_sequence(ds_readw(0xbd23), seq);
		return 1;
	}

	return 0;
}


/* Borlandified and identical */
/* static */
RealPt prepare_timbre(signed short a1, signed short patch)
{
	RealPt buf;

	seg002_0c72(ds_readws(0xbd01), 0, 0);

	do {
		read_archive_file(ds_readws(0xbd01), p_datseg + 0xbc5c, 6);

		if (ds_readbs(0xbc5d) == -1) {
			return (RealPt)0;
		}
	} while ((ds_readbs(0xbc5d) != a1) || (ds_readbs(0xbc5c) != patch));

	seg002_0c72(ds_readws(0xbd01), ds_readd(0xbc5e), 0);

	read_archive_file(ds_readws(0xbd01), p_datseg + 0xbc5a, 2);

	buf = schick_alloc_emu(ds_readw(0xbc5a));

	host_writew(Real2Host(buf), ds_readw(0xbc5a));

	read_archive_file(ds_readws(0xbd01), Real2Host(buf) + 2, ds_readw(0xbc5a) - 2);

	return buf;
}

/* Borlandified and identical */
/* static */
signed short load_midi_file(signed short index)
{
	return do_load_midi_file(index);
}

/* Borlandified and identical */
/* static */
signed short do_load_midi_file(signed short index)
{
	signed short handle;

	if ((handle = load_archive_file(index)) != -1) {
		read_archive_file(handle, Real2Host(ds_readd(0xbd0d)), 0x7fff);
		bc_close(handle);
		return 1;
	}
	return 0;
}

/* static */
signed short load_music_driver(RealPt fname, signed short type, signed short port)
{

	if (port &&
		NOT_NULL(Real2Host((RealPt)ds_writed(0xbd19, (Bit32u)read_music_driver(fname)))) &&
		((ds_writew(0xbd23, AIL_register_driver((RealPt)ds_readd(0xbd19)))) != 0xffff))
	{

		ds_writed(0xbd1d, (Bit32u)AIL_describe_driver(ds_readw(0xbd23)));

		if (host_readws(Real2Host((RealPt)ds_readd(0xbd1d)) + 2) == type)
		{
			if (port == -1) {
				port = host_readws(Real2Host(ds_readd(0xbd1d)) + 0xc);
			}

			if (AIL_detect_device(ds_readw(0xbd23), port,
				host_readws(Real2Host(ds_readd(0xbd1d)) + 0x0e),
				host_readws(Real2Host(ds_readd(0xbd1d)) + 0x10),
				host_readws(Real2Host(ds_readd(0xbd1d)) + 0x12)))
			{
				AIL_init_driver(ds_readw(0xbd23), port,
					host_readws(Real2Host(ds_readd(0xbd1d)) + 0x0e),
					host_readws(Real2Host(ds_readd(0xbd1d)) + 0x10),
					host_readws(Real2Host(ds_readd(0xbd1d)) + 0x12));

				if (type == 3) {
					ds_writed(0xbd05, AIL_state_table_size(ds_readw(0xbd23)));
					ds_writed(0xbd15, (Bit32u)schick_alloc_emu(ds_readd(0xbd05)));
					ds_writew(0xbd03, AIL_default_timbre_cache_size(ds_readw(0xbd23)));

					if (ds_readw(0xbd03) != 0) {
						ds_writed(0xbd11, (Bit32u)schick_alloc_emu(ds_readw(0xbd03)));
						AIL_define_timbre_cache(ds_readw(0xbd23),
								(RealPt)ds_readd(0xbd11),
								ds_readw(0xbd03));
					}
				}

				ds_writew(0xbcff, 0);
				return 1;
			} else {

				/* no sound hardware found */
				GUI_output(Real2Host(RealMake(datseg, 0x486d)));
				exit_AIL();
			}
		}
	}

	ds_writew(0xbcff, 1);
	return 0;
}

/* Borlandified and identical */
/* static */
void do_play_music_file(signed short index)
{
	if ((ds_readw(0xbcff) == 0) && (host_readw(Real2Host(ds_readd(0xbd1d)) + 2) == 3)) {

		stop_midi_playback();
		load_midi_file(index);
		start_midi_playback(0);
	}
}

/* Borlandified and identical */
/* static */
void stop_midi_playback(void)
{
	if ((ds_readw(0xbcff) == 0) && (host_readw(Real2Host(ds_readd(0xbd1d)) + 2) == 3))
	{
		AIL_stop_sequence(ds_readws(0xbd23), ds_readws(0xbd21));
		AIL_release_sequence_handle(ds_readws(0xbd23), ds_readws(0xbd21));
	}
}

/* Borlandified and identical */
void start_midi_playback_IRQ(void)
{
	if ((ds_readw(0xbcff) == 0) &&
		(ds_readb(0x4476) != 0) &&
		(host_readw(Real2Host(ds_readd(0xbd1d)) + 2) == 3))
	{
		if (AIL_sequence_status(ds_readws(0xbd23), ds_readws(0xbd21)) == 2) {
			AIL_start_sequence(ds_readws(0xbd23), ds_readws(0xbd21));
		}
	}
}

/* These function is never called */
/* Borlandified and identical */
void cruft_1(void)
{
	if ((ds_readw(0xbcff) == 0) &&
		(host_readw(Real2Host(ds_readd(0xbd1d)) + 2) == 3))
	{
		AIL_start_sequence(ds_readws(0xbd23), ds_readws(0xbd21));
	}
}

/* These function is never called */
/* Borlandified and identical */
void cruft_2(signed short volume)
{
	if (ds_readw(0xbcff) == 0) {

		if (host_readw(Real2Host(ds_readd(0xbd1d)) + 2) == 3) {
			AIL_set_relative_volume(ds_readws(0xbd23), ds_readws(0xbd21), volume, 0);
		}

		if (!volume) {
			stop_midi_playback();
		}
	}
}


/* Borlandified and identical */
signed short have_mem_for_sound(void)
{
	Bit32s size;
	signed short retval;
	struct ffblk blk;

	if (!bc_findfirst((RealPt)RealMake(datseg, 0x488c), &blk, 0)) {
		/* SOUND.ADV was found */
		size = host_readd((Bit8u*)(&blk) + 26);
		size += 4000L;

		if (size < bc_farcoreleft()) {
			retval = 1;

			if (size + 25000L < bc_farcoreleft()) {

				ds_writew(0x447c, 1);
			}
		} else {
			retval = 0;
		}
	} else {
		/* SOUND.ADV was not found */
		retval = 1;

		if (25000L < bc_farcoreleft()) {
			ds_writew(0x447c, 1);
		}
	}

	return retval;
}

/* Borlandified and identical */
void play_voc(signed short index)
{
	if (ds_readw(0x447c) && ds_readb(0x4477)) {
		SND_set_volume(90);
		SND_play_voc(index);
	}
}

/* Borlandified and identical */
void play_voc_delay(signed short index)
{
	if (ds_readw(0x447c) && ds_readb(0x4477)) {
		SND_set_volume(90);
		SND_play_voc(index);

		while (AIL_VOC_playback_status(ds_readw(0xbcfb)) == 2) {
			wait_for_vsync();
		}
	}
}

/* Borlandified and identical */
void alloc_voc_buffer(Bit32u size)
{
	if (ds_readw(0x447c)) {
		if (NOT_NULL(Real2Host(ds_writed(0xbcef, (Bit32u)schick_alloc_emu(size))))) ;
	}
}

/* Borlandified and identical */
/* static */
void free_voc_buffer(void)
{
	if (ds_readw(0x447c) != 0) {

		if (ds_readd(0xbcef) != 0) {
			bc_farfree((RealPt)ds_readd(0xbcef));
		}

		if (ds_readd(0xbceb) != 0) {
			bc_farfree((RealPt)ds_readd(0xbceb));
		}

		ds_writed(0xbcef, ds_writed(0xbceb, 0));

	}
}

/* Borlandified and identical */
signed short read_new_voc_file(signed short index)
{
	if (AIL_VOC_playback_status(ds_readw(0xbcfb)) == 2) {
		SND_stop_digi();
	}

	if (read_voc_file(index)) {

		AIL_format_VOC_file(ds_readw(0xbcfb), (RealPt)ds_readd(0xbcef), -1);
		return 1;
	}

	return 0;
}

/* Borlandified and identical */
signed short read_voc_file(signed short index)
{
	signed short handle;

	if ( (handle = load_archive_file(index)) != -1) {
		read_archive_file(handle, Real2Host(ds_readd(0xbcef)), 0x7fff);
		bc_close(handle);
		return 1;
	}

	return 0;
}

/* Borlandified and identical */
void SND_play_voc(signed short index)
{
	if (ds_readw(0x447c)) {

		AIL_stop_digital_playback(ds_readw(0xbcfb));
		read_new_voc_file(index);
		AIL_play_VOC_file(ds_readw(0xbcfb), (RealPt)ds_readd(0xbcef), -1);
		AIL_start_digital_playback(ds_readw(0xbcfb));
	}
}

/* Borlandified and identical */
void SND_stop_digi(void)
{
	if (ds_readw(0x447c)) {
		AIL_stop_digital_playback(ds_readw(0xbcfb));
	}
}

/* Borlandified and identical */
void SND_set_volume(unsigned short volume)
{
	if (ds_readw(0x447c)) {

		AIL_set_digital_playback_volume(ds_readw(0xbcfb), volume);

		if (!volume) {
			SND_stop_digi();
		}
	}
}

/* static */
/* Borlandified and identical */
signed short load_digi_driver(RealPt fname, signed short type, signed short io, signed short irq)
{

	if (io &&
		NOT_NULL(Real2Host((RealPt)ds_writed(0xbcf3, (Bit32u)read_digi_driver(fname)))) &&
		((ds_writew(0xbcfb, AIL_register_driver((RealPt)ds_readd(0xbcf3)))) != 0xffff))
	{

		ds_writed(0xbcf7, (Bit32u)AIL_describe_driver(ds_readw(0xbcfb)));

		if (host_readws(Real2Host((RealPt)ds_readd(0xbcf7)) + 2) == type) {

			if (io == -1) {
				io = host_readws(Real2Host(ds_readd(0xbcf7)) + 0xc);
				irq = host_readws(Real2Host(ds_readd(0xbcf7)) + 0xe);
			}

			if (AIL_detect_device(ds_readw(0xbcfb), io, irq,
				host_readws(Real2Host(ds_readd(0xbcf7)) + 0x10),
				host_readws(Real2Host(ds_readd(0xbcf7)) + 0x12)))
			{
				AIL_init_driver(ds_readw(0xbcfb), io, irq,
					host_readws(Real2Host(ds_readd(0xbcf7)) + 0x10),
					host_readws(Real2Host(ds_readd(0xbcf7)) + 0x12));

				ds_writeb(0x4477, 1);
				return 1;
			} else {
				/* no sound hardware found */
				GUI_output(Real2Host(RealMake(datseg, 0x4896)));
				free_voc_buffer();
			}
		}
	}

	return 0;
}

/* Borlandified and identical */
RealPt read_digi_driver(RealPt fname)
{
	Bit32u len;
	RealPt buf;
	Bit32u ptr;

	signed short handle;

	if ( (handle = bc__open(fname, 0x8001)) != -1) {

		len = 5000L;

		ds_writed(0xbceb, (Bit32u)schick_alloc_emu(len + 16L));
		ptr = ds_readd(0xbceb) + 15L;
		ptr &= 0xfffffff0;
		buf = EMS_norm_ptr((RealPt)ptr);
		bc__read(handle, Real2Host(buf), len);
		bc_close(handle);
		return buf;
	}
	return (RealPt)0;
}

/**
 * open_and_seek_dat - opens SCHICK.DAT and seeks to desired position
 * @fileindex: the index of the file in SCHICK.DAT
 *
 * Returns the filehandle or 0xffff.
 */
/* static */
/* Borlandified and identical */
signed short open_and_seek_dat(unsigned short fileindex)
{
	Bit32u start, end;
	signed short fd;

	/* open SCHICK.DAT */
	if ( (fd =  bc__open((RealPt)RealMake(datseg, 0x48ca), 0x8001)) != -1) {

		/* seek to the fileindex position in the offset table */
		bc_lseek(fd, fileindex * 4, DOS_SEEK_SET);

		/* read the start offset of the desired file */
		bc__read(fd, (Bit8u*)&start, 4);

		/* read the start offset of the next file */
		bc__read(fd, (Bit8u*)&end, 4);
#if !defined(__BORLANDC__)
		/* BE-Fix */
		start = host_readd((Bit8u*)&start);
		end = host_readd((Bit8u*)&end);
#endif

		/* seek to the desired file */
		bc_lseek(fd, start, DOS_SEEK_SET);

		/* save the offset of the desired file */
		ds_writed(0xbcdf, start);

		/* save the length of the desired file in 2 variables */
		ds_writed(0xbce7, ds_writed(0xbce3, end - start));
	}

	return fd;
}

/* Borlandified and identical */
Bit32u get_readlength2(signed short index)
{
	return index != -1 ? ds_readd(0xbce7) : 0;
}

/* Borlandified and identical */
unsigned short read_archive_file(Bit16u handle, Bit8u *buffer, Bit16u len)
{

	/* no need to read */
	if (ds_readd(0xbce3) != 0) {

		/* adjust number of bytes to read */
		if (len > ds_readds(0xbce3))
			len = ds_readw(0xbce3);

		sub_ds_ds(0xbce3, len);

		return bc__read(handle, buffer, len);
	} else {
		return 0;
	}
}

/* Borlandified and identical */
void seg002_0c72(Bit16u handle, Bit32s off, Bit16u dummy) {

	Bit32u file_off;

	ds_writed(0xbce3, ds_readd(0xbce7) - off);

	file_off = ds_readd(0xbcdf) + off;

	bc_lseek(handle, file_off, DOS_SEEK_SET);

	return;
}

/* Borlandified and identical */
signed short load_regular_file(Bit16u index)
{

	signed short handle;

	if ( (handle = bc__open((RealPt)ds_readd(0x4c8c + index * 4), 0x8004)) == -1) {

		/* prepare string which file is missing */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)Real2Host(ds_readd(0x4480)),
			(char*)Real2Host(ds_readd(0x4c8c + index * 4)));
		ds_writeb(0x2ca1, 1);
		GUI_output(Real2Host(ds_readd(DTP2)));
		ds_writeb(0x2ca1, 0);
	}

	return handle;
}

/* Borlandified and identical */
signed short load_archive_file(Bit16u index)
{
	bc_flushall();

	return (index & 0x8000) ? open_temp_file(index & 0x7fff) : open_and_seek_dat(index);
}

/* Borlandified and identical */
signed short open_temp_file(unsigned short index)
{
/* HACK: need a Real Pointer to 40 bytes on the DOSBox Stack */
#if !defined(__BORLANDC__)
	reg_esp -= 40;
	RealPt tmppath = RealMake(SegValue(ss), reg_sp);
#else
	unsigned char tmppath[40];
#endif
	signed short handle;

	sprintf((char*)Real2Host(tmppath),
		(char*)Real2Host(ds_readd(0x4c88)),
		(char*)Real2Host(ds_readd(0x4c8c + index * 4)));

	while ( (handle = bc__open(tmppath, 0x8004)) == -1) {

		copy_from_archive_to_temp(index, tmppath);
	}

	/* get the length of the file */
	ds_writed(0xbce7, ds_writed(0xbce3, bc_lseek(handle, 0, 2)));
	/* seek to start */
	bc_lseek(handle, 0, 0);

	ds_writed(0xbcdf, 0);

#if !defined(__BORLANDC__)
	reg_esp += 40;
#endif

	return handle;
}

/* Borlandified and identical */
void copy_from_archive_to_temp(unsigned short index, RealPt fname)
{
	signed short handle1;
	signed short handle2;
	signed short len;

	if ( (handle1 = load_archive_file(index)) != -1) {

		/* create new file in TEMP */
		handle2 = bc__creat(fname, 0);

		/* copy it */
		while ( (len = read_archive_file(handle1, Real2Host(ds_readd(0xd303)), 60000)) && (len != -1))
		{
			bc__write(handle2, (RealPt)ds_readd(0xd303), len);
		}

		bc_close(handle1);
		bc_close(handle2);
	}
}

/* Borlandified and identical */
void copy_file_to_temp(RealPt src_file, RealPt fname)
{
	signed short handle1;
	signed short handle2;
	signed short len;

	if ( (handle1 = bc__open(src_file, 0x8001)) != -1) {

		/* create new file in TEMP */
		handle2 = bc__creat(fname, 0);

		/* copy it */
		while ( (len = bc__read(handle1, Real2Host(ds_readd(0xd303)), 60000)) && (len != -1))
		{
			bc__write(handle2, (RealPt)ds_readd(0xd303), len);
		}

		bc_close(handle1);
		bc_close(handle2);
	}
}

/* Borlandified and identical */
Bit32s process_nvf(struct nvf_desc *nvf)
{
	signed short i;
	Bit32u offs;
	signed short pics;
	signed short width;
	signed short height;
	signed short va;
	Bit32u p_size;
	Bit32u retval;
	signed char nvf_type;
#if !defined(__BORLANDC__)
	Bit8u *nvf_nr;
	Bit8u *src;
#else
	Bit8u huge *nvf_nr;
	Bit8u huge *src;
#endif
	Bit8u *dst;

	nvf_type = host_readb(nvf->src);
	va = nvf_type & 0x80;
	nvf_type &= 0x7f;

#if !defined(__BORLANDC__)
	pics = host_readws(H_PADD(nvf->src, 1L));
#else
	pics = host_readws(Real2Host(H_PADD(nvf->src, 1L)));
#endif

	if (nvf->nr < 0)
		nvf->nr = 0;

	if (nvf->nr > pics - 1)
		nvf->nr = pics - 1;

	switch (nvf_type) {

	case 0x00:
		width = host_readws(H_PADD(nvf->src, 3));
		height = host_readws(H_PADD(nvf->src, 5));
		p_size = width * height;
		src =  H_PADD(nvf->src, nvf->nr * p_size + 7);
		break;

	case 0x01:
		offs = pics * 4 + 3L;
		for (i = 0; i < nvf->nr; i++) {
#if !defined(__BORLANDC__)
			width = host_readw(nvf->src + i * 4 + 3);
			height = host_readw(nvf->src + i * 4 + 5);
#endif
			offs += width * height;
		}

		width = host_readw(H_PADD(nvf->src, nvf->nr * 4 + 3));
		height = host_readw(H_PADD(nvf->src, nvf->nr * 4 + 5));
		p_size = width * height;
		src = H_PADD(nvf->src, offs);
		break;

	case 0x02: case 0x04:
		width = host_readw(H_PADD(nvf->src, 3L));
		height = host_readw(H_PADD(nvf->src, 5));
		offs = pics * 4 + 7L;
		for (i = 0; i < nvf->nr; i++) {
			offs += host_readd(H_PADD(nvf->src, (i * 4) + 7));
		}

		p_size = host_readd(H_PADD(nvf->src, nvf->nr * 4 + 7));
		src = H_PADD(nvf->src, offs);
		break;

	case 0x03: case 0x05:
		offs = pics * 8 + 3L;
		for (i = 0; i < nvf->nr; i++)
			offs += host_readd(H_PADD(nvf->src, (i * 8) + 7));

		width = host_readw(H_PADD(nvf->src, nvf->nr * 8 + 3));
		height = host_readw(H_PADD(nvf->src, nvf->nr * 8 + 5));
		p_size = host_readd(H_PADD(nvf->src, i * 8 + 7));
		src = H_PADD(nvf->src, offs);
		break;
	}

	if (!nvf->type) {

		/* PP20 decompression */

		if (va != 0) {

			/* get size from unpacked picture */
			retval = host_readd(src);
			nvf_nr = src;
			nvf_nr += (retval + (-4L));
			retval = host_readd(nvf_nr);
			retval = swap_u32(retval) >> 8;

		} else {
			retval = width * height;
		}

#if !defined(__BORLANDC__)
		decomp_pp20(src, nvf->dst, src + (signed char)4, p_size);
#else
		decomp_pp20(src,
			nvf->dst,
			FP_OFF(src) + 4,
			FP_SEG(src),
			p_size);
#endif

	} else if (nvf->type >= 2 && nvf->type <= 5) {

		dst = nvf->dst;

		/* RLE decompression */
		decomp_rle(width, height, dst, src,
			Real2Host(ds_readd(0xd2eb)), nvf->type);
#ifdef M302de_ORIGINAL_BUGFIX
		/* retval was originally neither set nor used here.
			VC++2008 complains about an uninitialized variable
			on a Debug build, so we fix this for debuggings sake */
		/* Orig-Fix */
		retval = p_size;
#endif
	} else {
		/* No decompression, just copy */
		memmove(nvf->dst, src, (short)p_size);
		retval = p_size;
	}

	host_writew(nvf->width, width);
	host_writew(nvf->height, height);

	return retval;
}

/**
 * mouse_action -	does mouse programming
 * @p1:		function AX
 * @p2:		depends on AX
 * @p3:		depends on AX
 * @p4:		depends on AX
 * @p5:		depends on AX
 *
 * This function differs a bit. Borlands C-Library has a special function
 * to call interrupts. We use the one of DOSBox, which means, that we
 * put the values in the emulated registers, instead in a structure.
 */
/* Borlandified and identical */
void mouse_action(Bit8u *p1, Bit8u *p2, Bit8u *p3, Bit8u *p4, Bit8u *p5)
{
#if !defined(__BORLANDC__)

	if ((signed short)host_readw(p1) < 0)
		return;

	unsigned short ba, bb, bc, bd, be, bsi, bdi;

	/* save register content */
	ba = reg_ax;
	bb = reg_bx;
	bc = reg_cx;
	bd = reg_dx;
	be = SegValue(es);
	bsi = reg_si;
	bdi = reg_di;

	/* write paramters to registers */
	reg_ax = host_readw(p1);
	reg_bx = host_readw(p2);
	reg_cx = host_readw(p3);

	/* respect special functions */
	switch (reg_ax) {
		case 0x9:	/* define Cursor in graphic mode */
		case 0xc:	/* install event handler */
		case 0x14:	/* swap event handler */
		case 0x16:	/* save mouse state */
		case 0x17:	/* load mouse state */
			reg_dx = host_readw(p4);
			SegSet16(es, host_readw(p5));
			break;
		case 0x10:	/* define screen region for update */
			reg_cx = host_readw(p2);
			reg_dx = host_readw(p3);
			reg_si = host_readw(p4);
			reg_di = host_readw(p5);
			break;
		default:
			reg_dx = host_readw(p4);

			D1_LOG("%x %x %x %x %x\n", host_readw(p1),
				host_readw(p2), host_readw(p3),
				host_readw(p4),	host_readw(p5));

	}

	/* Call the interrupt */
	CALLBACK_RunRealInt(0x33);

	/* write the return values */
	if (reg_ax == 0x14)
		host_writew(p2, SegValue(es));
	else
		host_writew(p2, reg_bx);

	host_writew(p1, reg_ax);
	host_writew(p3, reg_cx);
	host_writew(p4, reg_dx);

	/* restore register values */
	reg_ax = ba;
	reg_bx = bb;
	reg_cx = bc;
	reg_dx = bd;
	SegSet16(es, be);
	reg_si = bsi;
	reg_di = bdi;

	return;
#else
	struct SREGS sregs;
	union REGS wregs;

	if (host_readws(p1) >= 0) {

		wregs.x.ax = host_readw(p1);
		wregs.x.bx = host_readw(p2);
		wregs.x.cx = host_readw(p3);

		switch (host_readw(p1)) {
		case 0x9:	/* define Cursor in graphic mode */
		case 0xc:	/* install event handler */
		case 0x14:	/* swap event handler */
		case 0x16:	/* save mouse state */
		case 0x17:	/* load mouse state */
		{
			wregs.x.dx = host_readw(p4);
			sregs.es = host_readw(p5);
			break;
		}
		case 0x10: {
			wregs.x.cx = host_readw(p2);
			wregs.x.dx = host_readw(p3);
			wregs.x.si = host_readw(p4);
			wregs.x.di = host_readw(p5);
			break;
		}
		default : {
			wregs.x.dx = host_readw(p4);
		}
		}

		int86x(0x33, &wregs, &wregs, &sregs);

		if (host_readw(p1) == 0x14) {
			host_writew(p2, sregs.es);
		} else {
			host_writew(p2, wregs.x.bx);
		}

		host_writew(p1, wregs.x.ax);
		host_writew(p3, wregs.x.cx);
		host_writew(p4, wregs.x.dx);
	}
#endif
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void interrupt mouse_isr(void)
{
	signed short l_si = _AX;
	signed short l1;
	signed short l3;
	signed short l4;
	signed short l5;
	signed short l6;

	if (ds_readws(0x2998) == 0) {

		if (l_si & 0x2) {
			ds_writew(0xc3d5, 1);
			ds_writew(0xc3d1, 1);
		}

		if (l_si & 0x8) {
			ds_writew(0xc3d3, 1);
		}

		if (((ds_readb(0x2d6e) != 0) || (ds_readb(0x2d67) != 0)) &&
				!ds_readbs(0x2d60) &&
				!ds_readbs(0x2c98) &&
				(ds_readbs(0x2845) == 0))
		{

			ds_writed(0xcecb, (Bit32u) (is_mouse_in_rect(68, 4, 171, 51) ? RealMake(datseg, 0x2888):
							(is_mouse_in_rect(68, 89, 171, 136) ? RealMake(datseg, 0x28c8) :
							(is_mouse_in_rect(16, 36, 67, 96) ? RealMake(datseg, 0x2908) :
							(is_mouse_in_rect(172, 36, 223, 96) ? RealMake(datseg, 0x2948) :
							(!is_mouse_in_rect(16, 4, 223, 138) ? RealMake(datseg, 0x2848) :
								(void*)ds_readd(0xcecb)))))));
		} else {
			if (ds_readbs(0x2c98) != 0) {
				ds_writed(0xcecb, (Bit32u) RealMake(datseg, 0x2848));
			}
		}

		if (l_si & 1) {
			l1 = 3;
			l4 = ds_readws(0x299c);
			l5 = ds_readws(0x299e);

			mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);

			ds_writew(0x299c, l4);
			ds_writew(0x299e, l5);

			if (ds_readws(0x299c) > ds_readws(0x298e)) {
				ds_writew(0x299c, ds_readws(0x298e));
			}
			if (ds_readws(0x299c) < ds_readws(0x298a)) {
				ds_writew(0x299c, ds_readws(0x298a));
			}
			if (ds_readws(0x299e) < ds_readws(0x2988)) {
				ds_writew(0x299e, ds_readws(0x2988));
			}
			if (ds_readws(0x299e) > ds_readws(0x298c)) {
				ds_writew(0x299e, ds_readws(0x298c));
			}

			l1 = 4;
			l4 = ds_readws(0x299c);
			l5 = ds_readws(0x299e);

			mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);

			ds_writew(0x29a4, 1);
		}
	}
}
#endif

/**
 * \brief	checks if the mouse cursor is in a rectangle
 *
 * \param x1	upper left x coordinate
 * \param y1	upper left y coordinate
 * \param x2	bottom right x coordinate
 * \param y2	bottom right y coordinate
 *
 * \return 1 if the pointer is in this rectangle, otherwise 0
 */
/* Borlandified and identical */
signed short is_mouse_in_rect(signed short x1, signed short y1,
				signed short x2, signed short y2)
{
	signed short m_x;
	signed short m_y;

	m_x = ds_readws(0x299c);
	m_y = ds_readws(0x299e);

	return ((m_x >= x1) && (m_x <= x2) && (m_y >= y1) && (m_y <= y2)) ? 1 : 0;
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void mouse_init(void)
{
	signed short l1;
	signed short l3;
	signed short l4;
	signed short l5;
	signed short l6;

	if (ds_readw(0xc3c7) == 2) {

		l1 = 0;

		mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);

		if (l1 == 0) {
			ds_writew(0xc3c7, 0);
		}

		ds_writed(0xcecb, (Bit32u)RealMake(datseg, 0x2848));
		ds_writed(0xcec7, (Bit32u)RealMake(datseg, 0x2848));

		if (ds_readw(0xc3c7) == 2) {

			l1 = 4;
			l4 = ds_readws(0x299c);
			l5 = ds_readws(0x299e);

			mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);

			mouse_irq_init(0x1f, mouse_isr);
		}
	}
}
#endif

/* Borlandified and identical */
void disable_mouse(void)
{
	if (ds_readw(0xc3c7) == 2) {
		mouse_reset_ehandler();
	}
}

/* Borlandified and identical */
void seg002_170e(Bit8u *a1, Bit8u *a2, Bit8u *a3, Bit8u *a4)
{
	signed short tmp;

	host_writew(a1, 5);

	mouse_action(a1, a2, a3, a4, (Bit8u*)&tmp);
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void call_mouse_isr(void)
{
	mouse_isr();
}

/* Borlandified and identical */
void mouse_irq_init(signed short irq_nr, void interrupt *(isr))
{
	signed short l1;
	signed short l3;
	signed short l4;
	signed short l5;
	signed short l6;

	l1 = 12;
	l4 = irq_nr;
	l5 = FP_OFF(call_mouse_isr);
	l6 = FP_SEG(call_mouse_isr);

	ds_writed(0xbcdb, (Bit32u)bc__dos_getvect(0x78));
	bc__dos_setvect(0x78, (INTCAST)isr);

	mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);

	ds_writew(0x4484, 1);
}
#endif


/* Borlandified and identical */
void mouse_reset_ehandler(void)
{
	signed short l1;
	signed short l2;
	signed short l3;
	signed short l4;
	signed short l5;

	bc__dos_setvect(0x78, (INTCAST) ds_readd(0xbcdb));

	l1 = 12;
	l3 = 0;
	l4 = 0;
	l5 = 0;

	mouse_action((Bit8u*)&l1, (Bit8u*)&l2, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5);

	ds_writew(0x4484, 0);
}

/**
 * \brief	move the mouse cursor to a position on the screen
 *
 * \param x	X - coordinate
 * \param y	Y - coordinate
 */
/* Borlandified and identical */
void mouse_move_cursor(signed short x, signed short y)
{
	signed short l1 = 4;
	signed short l3;
	signed short l4 = x;
	signed short l5 = y;
	signed short l6;

	mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);
}

/* Borlandified and identical */
/* unused */
void seg002_1838(signed short a1, signed short a2, signed short a3, signed short a4)
{
	signed short l1 = 9;
	signed short l3 = a1;
	signed short l4 = a2;
	signed short l5 = a3;
	signed short l6 = a4;

	mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);
}

/* Borlandified and identical */
/* unused */
void seg002_1880(signed short a1)
{
	signed short l1 = 29;
	signed short l3 = a1;
	signed short l4;
	signed short l5;
	signed short l6;

	mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);
}


/**
	make_ggst_cursor - makes a mouse cursor from a selected item
	@p:	pointer to the icon of the item
*/
/* Borlandified and identical */
void make_ggst_cursor(Bit8u *icon)
{
	signed short y;
	signed short x;

	/* clear the bitmask */
	for (y = 0; y < 16; y++) {
		ds_writew(0xceef + y * 2, 0);
	}

	/* make a bitmask from the icon */
	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			/* if pixelcolor of the icon is not black */
			if (*icon++ != 0x40) {
				or_ds_ws(0xceef + y * 2, (0x8000 >> x));
			}
		}
	}

	/* copy and negate the bitmask */
	for (y = 0; y < 16; y++) {
		ds_writew(0xcecf + y * 2, ~ds_readw(0xceef + y * 2));
	}
}

/* Borlandified and identical */
void update_mouse_cursor(void)
{
	update_mouse_cursor1();
}

/* Borlandified and identical */
void refresh_screen_size(void)
{
	refresh_screen_size1();
}

/* Borlandified and identical */
void update_mouse_cursor1(void)
{
	if (ds_readw(0x2998) == 0) {

		if  (ds_readw(0x299a) == 0) {
			ds_writew(0x2998, 1);
			restore_mouse_bg();
			ds_writew(0x2998, 0);
		}

		dec_ds_ws(0x299a);
	}
}

/* Borlandified and identical */
void refresh_screen_size1(void)
{
	/* check lock */
	if (ds_readw(0x2998) == 0) {

		inc_ds_ws(0x299a);

		if (ds_readw(0x299a) == 0) {

			/* get lock */
			ds_writew(0x2998, 1);

			if (ds_readws(0x299c) < ds_readws(0x29a6))
				ds_writew(0x299c, ds_readw(0x29a6));

			if (ds_readws(0x299c) > 315)
				ds_writew(0x299c, 315);

			if (ds_readws(0x299e) < ds_readws(0x29a8))
				ds_writew(0x299e, ds_readw(0x29a8));

			if (ds_readws(0x299e) > 195)
				ds_writew(0x299e, 195);

			save_mouse_bg();
			ds_writew(0x29a0, ds_readw(0x299c));
			ds_writew(0x29a2, ds_readw(0x299e));
			ds_writew(0x29aa, ds_readw(0x29a6));
			ds_writew(0x29ac, ds_readw(0x29a8));
			draw_mouse_cursor();

			/* put lock */
			ds_writew(0x2998, 0);
		}
	}
}

/* Borlandified and identical */
void mouse_19dc(void)
{
	/* return if mouse was not moved and the cursor remains */
	if ((ds_readw(0x29a4) != 0) || (ds_readd(0xcec7) != ds_readd(0xcecb))) {

		/* set new cursor */
		ds_writed(0xcec7, ds_readd(0xcecb));

		/* check if the new cursor is the default cursor */
		if (Real2Host(ds_readd(0xcecb)) == p_datseg + 0x2848) {
			/* set cursor size 0x0 */
			ds_writew(0x29a6, ds_writew(0x29a8, 0));
		} else {
			/* set cursor size 8x8 */
			ds_writew(0x29a6, ds_writew(0x29a8, 8));
		}

		/* reset mouse was moved */
		ds_writew(0x29a4, 0);
		update_mouse_cursor1();
		refresh_screen_size1();
	}
}

/* Borlandified and identical */
void handle_gui_input(void)
{
#if !defined(__BORLANDC__)
	CALLBACK_RunRealFar(reloc_game + 0x51e, 0x1a34);
#else
	signed short l_si;
	signed short l_di;
	signed short l1;

	ds_writew(0xc3d7, ds_writew(ACTION, l_si = 0));

	herokeeping();

	if (CD_bioskey(1)) {

		l_si = (ds_writews(0xc3d7, bc_bioskey(0))) >> 8;
		and_ds_ws(0xc3d7, 0xff);

		if (l_si == 0x24) {
			l_si = 0x2c;
		}

		if ((ds_readw(0xc3d7) == 0x11) && (ds_readw(0xbd25) == 0)) {
			cleanup_game();
			bc_exit(0);
		}
	}

	if (ds_readw(0xc3d5) == 0) {

		ds_writew(0x29ba, 0);

		if (ds_readw(0xc3c7) == 0) {
		}

		if (ds_readw(0xc3d7) == 5) {
			seg048_1498();
			l_si = 0;
		}

		if (ds_readw(0xc3d7) == 15) {
			GRP_swap_heros();
			l_si = 0;
		}

		if ((ds_readw(0xc3d7) == 0x13) && !ds_readbs(0x2c98)) {
			sound_menu();
		}

		if ((ds_readw(0xc3d7) == 0x10) &&
			(ds_readws(0xc3c5) == 0) &&
			!ds_readbs(0x2c98) &&
			(ds_readws(0xbd25) == 0))
		{
			ds_writew(0xc3c5, 1);
			inc_ds_ws(0x2c99);
			ds_writew(0xd2d1, 1);
			l_di = ds_readws(0xbffd);
			ds_writew(0xbffd, 2);
			GUI_output(p_datseg + 0x448a);		/* P A U S E */
			ds_writew(0xbffd, l_di);
			ds_writew(0xd2d1, 0);
			ds_writew(0xc3c5, l_si = ds_writew(0xc3d7, 0));
			dec_ds_ws(0x2c99);
		}
	} else {
		play_voc(0x121);
		ds_writew(0xc3d5, 0);
		l_si = 0;

		if (NOT_NULL(Real2Host(ds_readd(0x29e4)))) {
			l_si = get_mouse_action(ds_readw(0x299c),
					ds_readw(0x299e),
					Real2Host(ds_readd(0x29e4)));
		}

		if (!l_si && NOT_NULL(Real2Host(ds_readd(0x29e0)))) {
			l_si = get_mouse_action(ds_readw(0x299c),
					ds_readw(0x299e),
					Real2Host(ds_readd(0x29e0)));
		}

		if (ds_readw(0xc3c7) == 2) {

			for (l1 = 0; l1 < 15; l1++) {
				wait_for_vsync();
			}

			if (ds_readw(0xc3d5) != 0) {
				ds_writew(0xc3cf, 1);
				ds_writew(0xc3d5, 0);
			}
		}

		if ((l_si >= 0xf1) && (l_si <= 0xf8)) {

			if (ds_readws(0xc3cf) != 0) {

				if ((host_readbs(get_hero(l_si - 241) + 0x21) != 0) &&
						host_readbs(get_hero(l_si - 241) + 0x87) == ds_readbs(CURRENT_GROUP))
				{
					seg048_00d0(l_si - 241);
					l_si = 0;
					ds_writew(0xc3cf, 0);
					ds_writew(0xc3d5, 0);
				}
			} else {
				if ((ds_readws(0x29b4) != 0) &&
					(host_readbs(get_hero(l_si - 241) + 0x21) != 0) &&
						host_readbs(get_hero(l_si - 241) + 0x87) == ds_readbs(CURRENT_GROUP))
				{
					GRP_move_hero(l_si - 241);
					l_si = 0;
					ds_writew(0xc3cf, 0);
					ds_writew(0xc3d5, 0);
				}
			}
		} else if (l_si == 0xfd) {
			/* Credits */

			l_si = 0;
			l_di = ds_readws(0xbffd);
			ds_writew(0xbffd, 5);
			ds_writew(0xd2d1, 1);
			GUI_output(get_ltx(0x628));
			ds_writew(0xd2d1, 0);
			ds_writew(0xbffd, l_di);

		} else if (l_si == 0xfc) {
			/* Clock */
			l_si = 0;
			l_di = ds_readws(0xbffd);
			ds_writew(0xbffd, 5);
			ds_writew(0xd2d1, 1);
			prepare_date_str();
			GUI_output(Real2Host(ds_readd(DTP2)));
			ds_writew(0xd2d1, 0);
			ds_writew(0xbffd, l_di);

		}
	}

	mouse_19dc();
	ds_writew(ACTION, l_si);

#endif
}

/* Borlandified and identical */
signed short get_mouse_action(signed short x, signed short y, Bit8u *p)
{
	signed short i;

	for (i = 0; host_readws(p + i * 10) != -1; i++) {

		if ((host_readws(p + i * 10) <= x) &&
			(host_readws(p + i * 10 + 4) >= x) &&
			(host_readws(p + i * 10 + 2) <= y) &&
			(host_readws(p + i * 10 + 6) >= y))
		{
			return host_readw(p + i * 10 + 8);
		}

	}

	return 0;
}

/* Borlandified and identical */
void handle_input(void)
{
	signed short l_si;
	signed short l_di;

	ds_writew(0xc3d7, ds_writew(ACTION, l_si = 0));

	herokeeping();

	if (CD_bioskey(1)) {

		l_si = (ds_writews(0xc3d7, bc_bioskey(0))) >> 8;
		and_ds_ws(0xc3d7, 0xff);

		if (l_si == 0x24) {
			l_si = 0x2c;
		}

		if ((ds_readw(0xc3d7) == 0x11) && (ds_readw(0xbd25) == 0)) {
			cleanup_game();
			bc_exit(0);
		}
	}

	if (ds_readw(0xc3d5) == 0) {

		if (ds_readw(0xc3c7) == 0) {
		}

		if ((ds_readw(0xc3d7) == 0x13) && !ds_readbs(0x2c98)) {
			sound_menu();
		}

		if ((ds_readw(0xc3d7) == 0x10) &&
			(ds_readws(0xc3c5) == 0) &&
			!ds_readbs(0x2c98) &&
			(ds_readws(0xbd25) == 0))
		{
			inc_ds_ws(0x2c99);
			ds_writew(0xc3c5, 1);
			ds_writew(0xd2d1, 1);
			ds_writew(0xbffd, 2);
			GUI_output(p_datseg + 0x448a);		/* P A U S E */
			ds_writew(0xbffd, 3);
			ds_writew(0xd2d1, 0);
			dec_ds_ws(0x2c99);

			ds_writew(0xc3c5, l_si = ds_writew(0xc3d7, 0));
		}
	} else {
		play_voc(0x121);
		ds_writew(0xc3d5, 0);
		l_si = 0;

		if (NOT_NULL(Real2Host(ds_readd(0x29e4)))) {
			l_si = get_mouse_action(ds_readw(0x299c),
					ds_readw(0x299e),
					Real2Host(ds_readd(0x29e4)));
		}

		if (!l_si && NOT_NULL(Real2Host(ds_readd(0x29e0)))) {
			l_si = get_mouse_action(ds_readw(0x299c),
					ds_readw(0x299e),
					Real2Host(ds_readd(0x29e0)));
		}

		if (ds_readw(0xc3c7) == 2) {

			for (l_di = 0; l_di < 25; l_di++) {

				wait_for_vsync();

				if (ds_readw(0xc3d5) != 0) {
					ds_writew(0xc3cf, 1);
					ds_writew(0xc3d5, 0);
					break;
				}
			}
		}
	}

	mouse_19dc();
	ds_writew(ACTION, l_si);
}

/* Borlandified and identical */
void wait_for_keyboard1(void)
{
	while (CD_bioskey(1)) {

		bc_bioskey(0);
	}
}

/* Borlandified and identical */
void game_loop(void)
{
#if defined(__BORLANDC__)
	signed short answer;

	while (ds_readw(0xc3c1) == 0) {

		if (ds_readbs(LOCATION) != 0) {
			do_location();
		} else if (ds_readbs(CURRENT_TOWN) != 0) {
			do_town();
		} else if (ds_readbs(DUNGEON_INDEX) != 0) {
			do_dungeon();
		} else if (ds_readbs(0x3614) != 0) {
			/* do_travel_mode();	seg093 */
			/* TODO: replace with do_travel_mode() */
			do_dungeon();
		}

		if (ds_readb(0x2d34) == 99) {
			ds_writew(0xc3c1, 5);
		}

		if (ds_readw(CHECK_DISEASE) != 0) {
			disease_effect();
		}

		if (ds_readw(CHECK_POISON) != 0) {
			poison_effect();
		}

		if (ds_readbs(0x4495) != 0) {

			ds_writeb(0x4495, 0);

			if (!count_heros_available() ||
				((count_heros_available() == 1) && check_hero(get_hero(6))))
			{
				/* no heros or only the NPC can act => GAME OVER */
				ds_writew(0xc3c1, 1);

			} else if (!count_heroes_available_in_group() ||
				((count_heroes_available_in_group() == 1) && is_hero_available_in_group(get_hero(6))))
			{
				/* no heros or only the NPC in this group can act => switch to next */
				GRP_switch_to_next(2);
			}

		}

		if ((host_readbs(get_hero(6) + 0x21) != 0) &&
			((ds_readbs(CURRENT_TOWN) != 0) || (ds_readws(0xc3c1) == 99)) &&
			(ds_readws(NPC_MONTHS) >= 1) &&
			(ds_readbs(0x4494) != ds_readws(NPC_MONTHS)))
		{
			npc_farewell();
			ds_writeb(0x4494, (signed short)ds_readws(NPC_MONTHS));
		}

		if ((ds_readws(IN_FIGHT) == 0) &&
			((ds_readws(0xc3c1) == 0) || (ds_readws(0xc3c1) == 99)) &&
			!ds_readbs(LOCATION))
		{
			check_level_up();
		}

		if (ds_readbs(0x46df) != 0) {

			ds_writeb(0x46df, 0);

			if (ds_readbs(0x2845) == 0) {
				draw_status_line();
			}
		}

		if ((ds_readws(0xc3c1) != 0) && (ds_readbs(0x4475) != 0)) {
			refresh_colors();
		}

		if (ds_readws(0xc3c1) == 1) {
			/* TODO: replace with show_game_over() */
			do_dungeon();
		}

		if (ds_readws(0xc3c1) == 4) {
			/* TODO: replace show_times_up() */
			do_dungeon();
		}

		if ((ds_readws(0xc3c1) == 1) ||
			ds_readws(0xc3c1) == 2 ||
			ds_readws(0xc3c1) == 4 ||
			ds_readws(0xc3c1) == 5 ||
			ds_readws(0xc3c1) == 7)
		{
			ds_writeb(LOCATION, 0);

			do {
				answer = load_save_game();

			} while (answer == -1);

			if (answer) {
				ds_writew(0xc3c1, 0);
				refresh_colors();
			}
		}

		if (ds_readw(0xc3c1) == 99) {
			/* TODO: replace with play_outro() */
			do_dungeon();
			cleanup_game();
			bc_exit(0);
		}
	}
#endif
}

/* Borlandified and identical */
//static
void timers_daily(void)
{
	Bit8u *hero_i;
	signed short i = 0;

	/* Smith / items to repair */
	for (i = 0; i < 50; i++) {

		if (ds_readw(0x31e2 + i * 6) != 0) {
			/* set time to 6:00 am */
			ds_writed(0x31e4 + i * 6, 32400L);
		}
	}

	/* reset insulted merchants */
	for (i = 0; i < 94; i++) {
		ds_writeb(0x3592 + i, 0);
	}

	hero_i = get_hero(0);
	for (i = 0; i <=6; i++, hero_i += 0x6da) {

		if ((host_readb(get_hero(i) + 0x21) != 0) &&
			(host_readbs(hero_i + 0x94) > 0))
		{
			dec_ptr_bs(hero_i + 0x94);
		}
	}

	/* enable disease check */
	ds_writew(CHECK_DISEASE, 1);

#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-BUG: Reenable identifying item in the academy */
	ds_writew(0x335a, 0);
#endif

	/* Decrase monthly credit cens timer (bank) */
	if (ds_readws(0x335e) > 0) {

		dec_ds_ws(0x335e);

		if (ds_readws(0x335e) == 0) {
			ds_writew(0x3350, 0);
		}
	}

	/* Days until you run in trouble, if you have more
		than 1000S debt at the bank */
	if (ds_readws(0x3360) > 0) {

		dec_ds_ws(0x3360);

		if (ds_readws(0x3360) == 0) {
			ds_writew(0x3360, -1);
		}
	}
}

/* Borlandified and identical */
/* static */
void seg002_2177(void)
{
	signed short i;

	for (i = 0; ds_readws(0x70a8 + i * 8) != -1; i++) {

		ds_writew(0x70ae + i * 8,
			random_interval(ds_readws(0x70a8 + i * 8), 20));
	}
}

/* Borlandified and identical */
void pal_fade(Bit8u *dst, Bit8u *p2)
{
	signed short i;

	for (i = 0; i < 32; i++) {

		if ((host_readbs(p2 + 3 * i) < host_readbs(dst + 3 * i)) &&
			(host_readbs(dst + 3 * i) > 0))
		{

			dec_ptr_bs(dst + i * 3);

		} else {
			if ((host_readbs(p2 + 3 * i) > host_readbs(dst + 3 * i)) &&
				(host_readbs(dst + 3 * i) < 0x3f))
			{
				inc_ptr_bs(dst + i * 3);
			}
		}

		if ((host_readbs((p2 + 1) + 3 * i) < host_readbs((dst + 1) + 3 * i)) &&
			(host_readbs((dst + 1) + 3 * i) > 0))
		{

			dec_ptr_bs((dst + 1) + i * 3);

		} else {
			if ((host_readbs((p2 + 1) + 3 * i) > host_readbs((dst + 1) + 3 * i)) &&
				(host_readbs((dst + 1) + 3 * i) < 0x3f))
			{
				inc_ptr_bs((dst + 1) + i * 3);
			}
		}

		if ((host_readbs((p2 + 2) + 3 * i) < host_readbs((dst + 2) + 3 * i)) &&
			(host_readbs((dst + 2) + 3 * i) > 0))
		{

			dec_ptr_bs((dst + 2) + i * 3);

		} else {
			if ((host_readbs((p2 + 2) + 3 * i) > host_readbs((dst + 2) + 3 * i)) &&
				(host_readbs((dst + 2) + 3 * i) < 0x3f))
			{
				inc_ptr_bs((dst + 2) + i * 3);
			}
		}
	}
}

/* Borlandified and identical */
void pal_fade_in(Bit8u *dst, Bit8u *p2, signed short v3, signed short colors)
{
	signed short i, si;

	si = 0x40 - v3;

	for (i = 0; i < colors; i++) {

		if ((host_readbs((p2 + 0) + 3 * i) >= si) &&
			(host_readbs((p2 + 0) + 3 * i) > host_readbs((dst + 0) + 3 * i)))
		{
			inc_ptr_bs((dst + 0) + i * 3);
		}

		if ((host_readbs((p2 + 1) + 3 * i) >= si) &&
			(host_readbs((p2 + 1) + 3 * i) > host_readbs((dst + 1) + 3 * i)))
		{
			inc_ptr_bs((dst + 1) + i * 3);
		}

		if ((host_readbs((p2 + 2) + 3 * i) >= si) &&
			(host_readbs((p2 + 2) + 3 * i) > host_readbs((dst + 2) + 3 * i)))
		{
			inc_ptr_bs((dst + 2) + i * 3);
		}
	}
}

/**
 * \brief	adjusts palettes in the morning
 *
 */
/* Borlandified and identical */
void dawning(void)
{
	/* Between 6 and 7, in 64 steps */
	if ((ds_readds(DAY_TIMER) >= 0x7e90) &&
		(ds_readds(DAY_TIMER) <= 0x93a8) &&
		!((ds_readds(DAY_TIMER) - 0x7e90L) % 0x54))
	{

		/* floor */
		pal_fade(p_datseg + 0x3e53, Real2Host(ds_readd(0xd321)));
		/* buildings */
		pal_fade(p_datseg + 0x3eb3, Real2Host(ds_readd(0xd321)) + 0x60);
		/* sky */
		pal_fade(p_datseg + 0x3f13, Real2Host(ds_readd(0xd321)) + 0xc0);

		/* in a town */
		if (ds_readbs(CURRENT_TOWN) &&
			/* not in a dungeon */
			!ds_readbs(DUNGEON_INDEX) &&
			/* not in a location */
			!ds_readbs(LOCATION) &&
			/* not in a travel mode */
			!ds_readb(TRAVELING) &&
			/* unknown */
			!ds_readb(0xe5d2) &&
			/* unknown */
			!ds_readbs(0x45b8) &&
			/* unknown */
			(ds_readbs(0x2845) == 0))
		{
			wait_for_vsync();

			set_palette(p_datseg + 0x3e53, 0, 0x20);
			set_palette(p_datseg + 0x3eb3, 0x80, 0x40);
		}
	}
}

/**
 * \brief	adjusts palettes in the evening
 *
 */
/* Borlandified and identical */
void nightfall(void)
{
	/* Between 20 and 21 */
	if ((ds_readds(DAY_TIMER) >= 0x1a5e0L) &&
		(ds_readds(DAY_TIMER) <= 0x1baf8L) &&
		!((ds_readds(DAY_TIMER) - 0x1a5e0L) % 0x54))
	{

		/* floor */
		pal_fade(p_datseg + 0x3e53, p_datseg + 0x4498);
		/* buildings */
		pal_fade(p_datseg + 0x3eb3, p_datseg + 0x44f8);
		/* sky */
		pal_fade(p_datseg + 0x3f13, p_datseg + 0x4558);

		/* in a town */
		if (ds_readbs(CURRENT_TOWN) &&
			/* not in a dungeon */
			!ds_readbs(DUNGEON_INDEX) &&
			/* not in a location */
			!ds_readbs(LOCATION) &&
			/* not in a travel mode */
			!ds_readb(TRAVELING) &&
			/* unknown */
			!ds_readb(0xe5d2) &&
			/* unknown */
			!ds_readbs(0x45b8) &&
			/* unknown */
			(ds_readbs(0x2845) == 0))
		{
			wait_for_vsync();

			set_palette(p_datseg + 0x3e53, 0, 0x20);
			set_palette(p_datseg + 0x3eb3, 0x80, 0x40);
		}
	}
}

/**
 * \brief get season
 * \return number of the season {0 = WINTER, 1,2,3}
 */
/* Borlandified and identical */
signed short get_current_season(void)
{
	/* Check Winter */
	if (is_in_byte_array(ds_readb(MONTH), p_datseg + 0x463e)) {
		return 0;
	/* Check Summer */
	} else if (is_in_byte_array(ds_readb(MONTH), p_datseg + 0x4642)) {
		return 2;
	/* Check Spring */
	} else if (is_in_byte_array(ds_readb(MONTH), p_datseg + 0x463a)) {
		return 1;
	} else {
		return 3;
	}
}


/**
 * \brief	calc census for the bank depot
 *
 * If you put money on the bank, you get 5%.
 * If you borrowed money you pay 15%.
 */
/* Borlandified and identical */
/* static */
void do_census(void)
{

	signed short si = 0;
	Bit32s val;

	if (ds_readws(BANK_DEPOSIT) > 0) {
		si = 1;
	} else if (ds_readws(BANK_DEPOSIT) < 0) {
			si = -1;
	}

	/* bank transactions, no census */
	if (si == 0)
		return;

	/* convert to heller */
	val = ds_readws(BANK_DEPOSIT) * 10L;
	val += ds_readws(BANK_HELLER);

	if (val < 0)
		/* 15% Interest for borrowed money */
		val += val * 15 / 100L / 12L;
	else if (val > 0)
		/* 5% Interest for deposited money */
			val += val * 5 / 100L / 12L;

	/* remember the heller */
	ds_writew(BANK_HELLER, val % 10);

	if (val < 0) {
		ds_writew(BANK_HELLER, -__abs__(ds_readws(BANK_HELLER)));
	}

	/* save the new deposit */
	ds_writew(BANK_DEPOSIT, val / 10);

	/* fixup over- and underflows */
	if ((ds_readws(BANK_DEPOSIT) < 0) && (si == 1))
		ds_writew(BANK_DEPOSIT, 32760);
	else if ((ds_readws(BANK_DEPOSIT) > 0) && (si == -1))
			ds_writew(BANK_DEPOSIT, -32760);

}

/* Borlandified and identical */
void do_timers(void)
{
	Bit8u *hero_i;
	signed char afternoon;
	Bit8u *ptr;
	signed short i, di;

	afternoon = 0;

	if (ds_readw(TIMERS_DISABLED) != 0)
		return;

	dawning();

	nightfall();

	/* inc day timer */
	add_ds_ds(DAY_TIMER, 1);

	if (!ds_readbs(0xbcda)) {
		sub_ingame_timers(1);
		sub_mod_timers(1);
	}

	if (!ds_readbs(0xbcda)) {

		/* set day timer to pm */
		/* TODO: afternoon is useless */
		if (ds_readds(DAY_TIMER) >= 0xfd20) {
			sub_ds_ds(DAY_TIMER, 0xfd20);
			afternoon = 1;
		}

		/* every 5 minutes ingame */
		if (!(ds_readds(DAY_TIMER) % 0x1c2)) {
			seg002_2f7a(1);
		}

		/* every 15 minutes ingame */
		if (!(ds_readds(DAY_TIMER) % 0x546)) {
			sub_light_timers(1);
		}
		/* every hour ingame */
		if (!(ds_readds(DAY_TIMER) % 0x1518)) {

			magical_chainmail_damage();

			/* decrement unicorn timer */
			if (ds_readb(UNICORN_GET_MAP) != 0 &&
				ds_readb(UNICORN_TIMER) != 0)
			{
				dec_ds_bs(UNICORN_TIMER);
			}

			/* handle sphere timer */
			if (ds_readb(SPHERE_TIMER) != 0) {

				if (!add_ds_bu(SPHERE_TIMER, -1)) {
					ds_writeb(SPHERE_ACTIVE, 1);
				}
			}

			/* two timers of a dungeon */
			if (ds_readbs(0x3cc6) != 0) {
				dec_ds_bs(0x3cc6);
			}
			if (ds_readbs(0x3cc7) != 0) {
				dec_ds_bs(0x3cc7);
			}
		}

		/* reset the day timer to 24h time */
		if (afternoon) {
			add_ds_ds(DAY_TIMER, 0xfd20L);
		}
	}

	/* at 6 o'clock in the morninig */
	if (ds_readd(DAY_TIMER) == 0x7e90L) {

		hero_i = get_hero(0);

		for (i = 0; i <= 6; i++, hero_i += 0x6da) {

			/* check typus */
			if ((host_readb(hero_i + 0x21) != 0) &&
				/* unknown flag */
				(host_readb(hero_i + 0x9f) != 0))
			{

				/* reset flag */
				host_writeb(hero_i + 0x9f, 0);

				ds_writeb(0x2d61 + host_readbs(hero_i + 0x87),
					ds_readb(0x2da0 + host_readbs(hero_i + 0x87)));

				ds_writew(0x2d48 + host_readbs(hero_i + 0x87) * 2,
					ds_readw(0x2d87 + host_readbs(hero_i + 0x87) * 2));
				ds_writew(0x2d54 + host_readbs(hero_i + 0x87) * 2,
					ds_readw(0x2d93 + host_readbs(hero_i + 0x87) * 2));
			}
		}
	}

	/* at 10 o'clock */
	if (ds_readd(DAY_TIMER) == 0xd2f0) {

		hero_i = get_hero(0);

		for (i = 0; i <= 6; i++, hero_i += 0x6da) {
			/* check typus */
			if ((host_readb(hero_i + 0x21) != 0) &&
				/* check drunken */
				(host_readb(hero_i + 0xa1) != 0))
			{
				hero_get_sober(hero_i);
			}
		}
	}

	/* poison timer in the mage dungeon */
	if (ds_readd(MAGE_POISON) != 0) {

		/* decrement the timer */
		sub_ds_ds(MAGE_POISON, 1);

		/* every 15 minutes  do damage */
		if (ds_readd(MAGE_POISON) % 0x546 == 0) {

			ptr = get_hero(0);

			for (i = 0; i <= 6; i++, ptr += 0x6da) {

				/* check typus */
				if (host_readb(ptr + 0x21) != 0) {

					/* get group nr */
					di = host_readbs(ptr + 0x87);

					/* hero is in group and in mage dungeon */
					if ((ds_readbs(CURRENT_GROUP) == di) &&
						(ds_readb(DUNGEON_INDEX) == 7))
					{

						if (ds_readbs(DUNGEON_LEVEL) == 1) {
							/* 1W6-1 */
							sub_hero_le(ptr,
								dice_roll(1, 6,	-1));
						} else if (ds_readbs(DUNGEON_LEVEL) == 2) {
							/* 1W6+1 */
							sub_hero_le(ptr,
								dice_roll(1, 6, 1));
						}

					} else {
						if (ds_readb(0x2d6f + di) == 7) {

							if (ds_readbs(0x2d76 + di) == 1) {
								/* 1W6-1 */
								sub_hero_le(ptr,
									dice_roll(1, 6, -1));
							} else if (ds_readbs(0x2d76 + di) == 2) {
								/* 1W6+1 */
								sub_hero_le(ptr,
									dice_roll(1, 6, 1));
							}
						}
					}
				}
			}
		}
	}

	/* unknown timer */
	if (ds_readd(0x3fa2) != 0) {
#if !defined(__BORLANDC__)
		D1_LOG("Unknown Dungeon Timer\n");
#endif
		sub_ds_ds(0x3fa2, 1);
	}

	/* at 24 o'clock, daily stuff */
	if (ds_readds(DAY_TIMER) >= 0x1fa40L) {

		timers_daily();

		seg002_2177();

		/* reset day timer */
		ds_writed(DAY_TIMER, 0);

		/* inc DAY date */
		inc_ds_bs(DAY_OF_WEEK);
		inc_ds_bs(DAY_OF_MONTH);

		if (ds_readb(DAY_OF_WEEK) == 7) {
			ds_writeb(DAY_OF_WEEK, 0);
		}

		/* decrement NPC timers */
		for (i = 1; i < 7; i++) {

			if ((ds_readbs(0x3601 + i) != 0) &&
				(ds_readbs(0x3601 + i) != -1))
			{
				dec_ds_bs(0x3601 + i);
			}
		}

		/* drug timer (phexcaer) */
		if (ds_readb(0x3f76) != 0) {
			dec_ds_bs(0x3f76);
		}

		/* unknown timer */
		if (ds_readb(0x4332) != 0) {
			dec_ds_bs(0x4332);
		}

		/* calendar */
		if (ds_readb(DAY_OF_MONTH) == 31) {
			/* new month */
			ds_writeb(DAY_OF_MONTH, 1);
			inc_ds_bs(MONTH);

			/* increment quested months counter */
			if (ds_readw(GOT_MAIN_QUEST) != 0) {
				inc_ds_ws(QUESTED_MONTHS);
			}

			/* increment the months the NPC is in the group */
			if (host_readb(get_hero(6) + 0x21) != 0) {
				inc_ds_ws(NPC_MONTHS);
			}

			do_census();

			/* set days of the nameless (negative) */
			if (ds_readb(MONTH) == 13) {
				ds_writeb(DAY_OF_MONTH, -5);
			}
		} else {
			/* new year */
			if (ds_readb(DAY_OF_MONTH) == 0) {
				ds_writeb(MONTH, 1);
				inc_ds_bs(YEAR);
				ds_writeb(DAY_OF_MONTH, 1);
			}
		}

		/* check if we have a special day */
		ds_writeb(SPECIAL_DAY, 0);

		for (i = 0; ds_readbs(0x45b9 + 3 * i) != -1; i++) {

			if ((ds_readbs(0x45b9 + 3 * i) == ds_readbs(MONTH)) &&
				(ds_readbs((0x45b9 + 1) + 3 * i) == ds_readbs(DAY_OF_MONTH)))
			{
				ds_writeb(SPECIAL_DAY, ds_readb((0x45b9 + 2) + 3 * i));
				break;
			}
		}

		passages_recalc();

		/* roll out the weather, used for passages */
		ds_writew(0x331b, random_schick(6));
		ds_writew(0x331d, random_schick(7));

		/* check if times up */
		if ((ds_readbs(YEAR) == 17) &&
			(ds_readbs(MONTH) >= 10) &&
			(ds_readbs(DAY_OF_MONTH) >= 17))
		{
			ds_writew(0xc3c1, 4);
		}
	}

	/* at 9 o'clock */
	if (ds_readd(DAY_TIMER) == 0xbdd8) {
		passages_reset();
	}
}

/**
 * \brief	subtracts val from the ingame timers
 * \param val	vaule to subtract from the ingame timers
*/
/* Borlandified and identical */
void sub_ingame_timers(Bit32s val)
{
	signed short i = 0;

	if (ds_readw(TIMERS_DISABLED))
		return;

	for (i = 0; i < 26; i++) {

		/* only subtract if greater than zero */
		if (ds_readds(0x2dc4 + i * 4) > 0) {

			/* subtract val from timer*/
			sub_ds_ds(0x2dc4 + i * 4, val);

			/* if the timer is now lower than zero, set the timer to zero */
			if (ds_readds(0x2dc4 + i * 4) < 0) {
				ds_writed(0x2dc4 + i * 4, 0);
			}
		}
	}
}

/**
 * \brief	subtracts val from the modification timers
 * \param val	vaule to subtract from the modification timers
*/
/* Borlandified and identical */
void sub_mod_timers(Bit32s val)
{
	signed short i;
	signed short j;
	signed short h_index;
#if !defined(__BORLANDC__)
	Bit8u *mp;
#else
	Bit8u huge *mp;
#endif
	unsigned char target;
	unsigned char reset_target;
	Bit8u *sp;

	h_index = -1;

	if (ds_readw(TIMERS_DISABLED))
		return;

	for (i = 0; i < 100; i++) {

		/* if timer is 0 continue */
		if (ds_readd(0x2e2c + 8 * i) == 0)
			continue;

		/* subtract diff from timer */
		sub_ds_ds(0x2e2c + 8 * i, val);


		/* if timer > 0 continue */
		if (ds_readds(0x2e2c + 8 * i) <= 0) {


#if !defined(__BORLANDC__)
			D1_LOG("Mod Timer %d rueckgesetzt\n", i);
#endif

			/* set timer to 0 */
			ds_writed(0x2e2c + 8 * i, 0);

			/* make a pointer to the slot */
			sp = p_datseg + 0x2e2c + i * 8;

			if (host_readb(sp + 6) != 0) {
				/* target is a hero/npc */

				/* get the hero index from the target */
				target = host_readb(sp + 6);
				for (j = 0; j <= 6; j++) {
					if (host_readbs(get_hero(j) + 0x7b) == target) {
						h_index = j;
						break;
					}
				}

				if (h_index != -1) {
					/* if a hero/npc is determined */

					mp = get_hero(h_index);
					/* make a pointer to the heros attribute mod */
					mp += (Bit32u)host_readw(sp + 4);
					/* subtract the mod */
					sub_ptr_bs(mp, host_readbs(sp + 7));

					if (ds_readb(0x2845) == 20) {
						ds_writew(0x2846, 1);
					}

					/* reset target */
					host_writeb(sp + 6, 0);

					/* reset target if no other slots of target */
					reset_target = 1;
					for (j = 0; j < 100; j++) {
						if (ds_readb(0x2e32 + j * 8) == target) {
							reset_target = 0;
							break;
						}
					}

					if (reset_target) {
						host_writeb(get_hero(h_index) + 0x7b, 0);
					}
				} else {
#if !defined(__BORLANDC__)
					D1_ERR("Invalid Mod Timer Target %d\n", target);
#endif

					/* reset all slots of invalid target */
					for (j = 0; j < 100; j++) {
						if (ds_readb(0x2e32 + j * 8) == target) {
							host_writeb(sp + 6,
									host_writebs(sp + 7, 0));

							host_writew(sp + 4, 0);
						}
					}
				}

			} else {
				/* target affects the savegame */
				mp = p_datseg + 0x2d34;
				mp += host_readw(sp + 4);
				sub_ptr_bs(mp, host_readbs(sp + 7));
			}

			/* reset offset, target, and modificator */
			host_writeb(sp + 6,
				host_writebs(sp + 7, 0));

			host_writew(sp + 4, 0);
		}
	}
}

/**
 * \brief	get a free modification slot
 *
 * \return	number of the modification slot
 */
/* Borlandified and identical */
signed short get_free_mod_slot(void)
{
	signed short i;

	for (i = 0; i < 100; i++) {

		if (ds_readw(0x2e2c + i * 8 + 4) == 0) {
			break;
		}
	}

	if (i == 100) {

		/* set timer of slot 0 to 1 */
		host_writed(p_datseg + 0x2e2c, 1);
		/* subtract one */
		sub_mod_timers(1);

		return 0;
	}

	return i;
}

/* Borlandified and identical */
void set_mod_slot(signed short slot_nr, Bit32s timer_value, Bit8u *ptr,
	signed char mod, signed char who)
{
	signed short j;

#if !defined (__BORLANDC__)
	Bit8u *mod_ptr;
#else
	Bit8u huge *mod_ptr;
#endif
	signed char target;
	signed short i;
	signed short new_target;


	if (who == -1) {
		/* mod slot is on savegame */
		mod_ptr = p_datseg + 0x2d34;
	} else {
		/* mod slot is on a hero/npc */
		mod_ptr = get_hero(who);

		if (host_readb(get_hero(who) + 0x7b) != 0) {
			/* hero/npc has a target number */
			target = host_readbs(get_hero(who) + 0x7b);
		} else {
			/* hero/npc has no target number */

			for (i = 1; i < 8; i++) {

				new_target = 1;
				for (j = 0; j <= 6; j++) {
					if (host_readbs(get_hero(j) + 0x7b) == i) {
						new_target = 0;
						break;
					}
				}

				if (new_target != 0) {
					target = i;
					break;
				}
			}

			host_writeb(get_hero(who) + 0x7b, target);
		}

		ds_writeb(0x2e2c + slot_nr * 8 + 6, target);
	}

	ds_writeb(0x2e2c + slot_nr * 8 + 7, mod);
#if !defined (__BORLANDC__)
	ds_writew(0x2e2c + slot_nr * 8 + 4, ptr - mod_ptr);
#else
	ds_writew(0x2e2c + slot_nr * 8 + 4, (Bit8u huge*)ptr - mod_ptr);
#endif
	ds_writed(0x2e2c + slot_nr * 8, timer_value);
	add_ptr_bs(ptr, mod);
}

/**
 *
 *	@fmin:	five minutes
 */
/* Borlandified and identical */
void seg002_2f7a(Bit32s fmin)
{
	signed short i;
	Bit8u *hero_i;

	if (ds_readw(TIMERS_DISABLED) != 0)
		return;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* check class */
		if (host_readb(hero_i + 0x21) != 0) {

			/* Timer to the next healing attempt */
			if (host_readds(hero_i + 0x8b) > 0) {

				sub_ptr_ds(hero_i + 0x8b, fmin * 450);
#if !defined(__BORLANDC__)
				if (host_readds(hero_i + 0x8b) <= 0) {
					D1_INFO("%s kann wieder geheilt werden\n",
						(char*)hero_i + 0x10);
				}
#endif

				if (host_readds(hero_i + 0x8b) < 0) {
					host_writed(hero_i + 0x8b, 0);
				}
			}

			/* Timer set after Staffspell */
			if (host_readds(hero_i + 0x8f) > 0) {
				sub_ptr_ds(hero_i + 0x8f, fmin * 450);
#if !defined(__BORLANDC__)
				if (host_readds(hero_i + 0x8f) <= 0) {
					D1_INFO("%s kann wieder einen Stabzauber versuchen\n",
						(char*)(hero_i + 0x10));
				}

#endif
				if (host_readds(hero_i + 0x8f) < 0) {

					host_writed(hero_i + 0x8f, 0);
				}
			}

			ds_writew(CHECK_POISON, 1);
		}
	}
}

/**
 * \brief		decrements the light timers
 *
 * \param quarter	the time in quarters of an hour
 *
 *	This function decrements the timers of burning torches and lanterns.
 *	If the time of the lightsource is up the toch is removed from the
 *	inventory and the lantern is turned off.
*/
/* Borlandified and identical */
void sub_light_timers(Bit32s quarter)
{
	signed short j;
	signed short i;

	Bit8u *hero_i;
	signed char tmp;

	if (ds_readw(TIMERS_DISABLED))
		return;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* check class */
		if (host_readb(hero_i + 0x21) != 0) {

			if (quarter > 120) {
				tmp = 120;
			} else {
				tmp = quarter;
			}

			for (j = 0; j < 23; j++) {

				if (host_readw(hero_i + 0x196 + 14 * j) == 0x16) {

					/* Torch, burning */

					sub_ptr_bs(hero_i + 0x196 + 8 + 14 * j, tmp);

					if (host_readbs(hero_i + 0x196 + 8 + 14 * j) <= 0)
					{
						/* decrement item counter */
						dec_ptr_bs(hero_i + 0x20);

						/* subtract weight of a torch */
						sub_ptr_ws(hero_i + 0x2d8,
							host_readws(Real2Host(ds_readd(ITEMSDAT)) + 0x10d));

						/* Remove Torch from inventory */
						memset(hero_i + 0x196 + 14 * j, 0, 14);
					}

				} else if (host_readw(hero_i + 0x196 + 14 * j) == 0xf9) {

					/* Lantern, burning */
					sub_ptr_bs(hero_i + 0x196 + 8 + 14 * j, tmp);

					if (host_readbs(hero_i + 0x196 + 8 + 14 * j) <= 0) {
						/* Set timer to 0 */
						host_writeb(hero_i + 0x196 + 8 + 14 * j, 0);
						/* Set burning lantern to a not burning lantern */
						host_writew(hero_i + 0x196 + 14 * j, 0x19);
					}
				}
			}
		}
	}
}

/**
 * \brief	damage if a cursed chainmail is worn
 *
 */
/* Borlandified and identical */
void magical_chainmail_damage(void)
{
	signed short i;
	Bit8u *hero_i;

	if (ds_readw(TIMERS_DISABLED) != 0) {
		return;
	}

	ds_writeb(0x4649, (ds_readb(TRAVELING) != 0) ? 1 : 2);

	for (i = 0; i <= 6; i++) {

		/* check typus */
		if (host_readb(get_hero(i) + 0x21) != 0) {

			hero_i = get_hero(i);

			if (!hero_dead(hero_i) &&
				/* unknown */
				!host_readbs(hero_i + 0x9f) &&
				/* check magical chainmail is equipped */
				(host_readw(hero_i + 0x1b2) == 0xc5))
			{
				sub_hero_le(hero_i, 1);
			}
		}
	}
}

/**
 * \brief	consume food if needed and print warnings to the user
 *
 */
/* Borlandified and identical */
void herokeeping(void)
{
	register signed short i, pos;
	Bit8u *hero;
	char buffer[100];

	if (ds_readw(0xc3c1) != 0)
		return;

	/* for each hero ..*/
	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += 0x6da) {

		/* consume food and set messages */
		if (host_readb(hero + 0x21) != 0 &&		/* valid hero */
			ds_readb(0x4649) != 0 &&
			check_hero_no3(hero) &&			/* must be vital */
			!host_readbs(hero + 0x9f) &&
			!ds_readbs(0x4497))
		{
			/* Do the eating */

			/* check for magic bread bag */
			if (get_first_hero_with_item_in_group(0xb8, host_readbs(hero + 0x87)) == -1) {
				/* check if the hero has the food amulet */
				if (get_item_pos(hero, 0xaf) == -1) {

					/* eat if hunger > 90 % */
					if (host_readbs(hero + 0x7f) > 90) {

						/* search for Lunchpack */
						pos = get_item_pos(hero, 0x2d);

						if (pos != -1) {
							/* Lunchpack found, consume quiet */
							ds_writeb(CONSUME_QUIET, 1);
							consume(hero, hero, pos);
#if !defined(__BORLANDC__)
							D1_INFO("%s isst etwas\n", (char*)hero + 0x10);
#endif
							ds_writeb(CONSUME_QUIET, 0);

							/* search for another Lunchpack */
							/* print last ration message */
							if (get_item_pos(hero, 0x2d) == -1) {
								ds_writeb(FOOD_MESSAGE + i, 6);
							}
						} else {
							/* print ration warning */
							if (host_readbs(hero + 0x7f) < 100) {
								ds_writeb(FOOD_MESSAGE + i, 4);
							}
						}

					}

					if (host_readbs(hero + 0x7f) < 100) {
						/* increase food counter food_mod is always 0 or 1 */
						if (host_readbs(hero + 0x7e) <= 0) {
							/* increase more (2 or 1) */
							add_ptr_bs(hero + 0x7f, 2 / (ds_readbs(FOOD_MOD) * 2 + 1));
						} else {
							/* increase less (1 or 0) */
							add_ptr_bs(hero + 0x7f, 1 / (ds_readbs(FOOD_MOD) * 2 + 1));
						}

						/* adjust hunger */
						if (host_readbs(hero + 0x7f) > 100) {
							host_writeb(hero + 0x7f, 100);
						}
					} else {

						/* */
						if (host_readbs(hero + 0x7e) <= 0) {
							do_starve_damage(hero, i, 0);
						}
					}
				}
			} else {

				/* set hunger to 20 % */
				if (host_readbs(hero + 0x7f) > 20) {
					host_writeb(hero + 0x7f, 20);
				}
			}

			/* Do the drinking */

			/* check for magic waterskin in group */
			if ((get_first_hero_with_item_in_group(0xb9, host_readbs(hero + 0x87)) == -1) &&
				((host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
				(!ds_readbs(CURRENT_TOWN) || (ds_readbs(CURRENT_TOWN) != 0 && ds_readb(TRAVELING) != 0))) ||
				((host_readbs(hero + 0x87) != ds_readbs(CURRENT_GROUP) &&
				!ds_readbs(0x2d68 + host_readbs(hero + 0x87)))))) {

					/* check for food amulett */
					if (get_item_pos(hero, 0xaf) == -1) {

						/* hero should drink something */
						if (host_readbs(hero + 0x80) > 90) {

							ds_writeb(CONSUME_QUIET, 1);

							/* first check for beer :) */
							pos = get_item_pos(hero, 0x17);

							/* and then for water */
							if (pos == -1) {
								pos = get_full_waterskin_pos(hero);
							}

							if (pos != -1) {
								/* drink it */
								consume(hero, hero, pos);
#if !defined(__BORLANDC__)
								D1_INFO("%s trinkt etwas\n", (char*)hero + 0x10);
#endif
								/* nothing to drink message */
								if ((get_item_pos(hero, 0x17) == -1)
									&& (get_full_waterskin_pos(hero) == -1)) {
									ds_writeb(FOOD_MESSAGE + i, 5);
								}

							} else {
								/* hero has nothing to drink */
								if (host_readbs(hero + 0x80) < 100) {
									ds_writeb(FOOD_MESSAGE + i, 3);
								}
							}

							ds_writeb(CONSUME_QUIET, 0);
						}

						if (host_readbs(hero + 0x80) < 100) {
							/* increase thirst counter food_mod is always 0 or 1 */
							if (host_readbs(hero + 0x7e) <= 0) {

								add_ptr_bs(hero + 0x80, 4 / (ds_readbs(FOOD_MOD) * 2 + 1));
							} else {

								add_ptr_bs(hero + 0x80, 2 / (ds_readbs(FOOD_MOD) * 2 + 1));
							}

							/* adjust thirst */
							if (host_readbs(hero + 0x80) > 100) {
								host_writeb(hero + 0x80, 100);
							}

						} else {
							if (host_readbs(hero + 0x7e) <= 0) {
								do_starve_damage(hero, i, 1);
							}
						}

					}
			} else {

				/* set thirst to 20 % */
				if (host_readbs(hero + 0x80) > 20) {
					host_writeb(hero + 0x80, 20);
				}
			}
		}

		/* print hero message */
		if ((ds_readb(FOOD_MESSAGE + i) != 0) &&
			!ds_readbs(0x2c98) &&
			(ds_readw(IN_FIGHT) == 0) &&
			!ds_readbs(0xbcda))
		{

			if ((host_readb(hero + 0x21) != 0) &&
				(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)) &&
				!hero_dead(hero) &&
				(!ds_readb(TRAVELING) || ds_readb(0x26a4 + i) != ds_readb(FOOD_MESSAGE + i))) {

					sprintf(buffer,
						(ds_readb(FOOD_MESSAGE + i) == 1) ? (char*)get_ltx(0x380):
							((ds_readb(FOOD_MESSAGE + i) == 2) ? (char*)get_ltx(0x37c) :
							((ds_readb(FOOD_MESSAGE + i) == 3) ? (char*)get_ltx(0xc74) :
							((ds_readb(FOOD_MESSAGE + i) == 4) ? (char*)get_ltx(0xc78) :
							((ds_readb(FOOD_MESSAGE + i) == 5) ? (char*)get_ltx(0xc7c) :
							(char*)get_ltx(0xc80))))),

						(char*)hero + 0x10, (char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 1)));

					ds_writeb(0x26a4 + i, ds_readb(FOOD_MESSAGE + i));

					GUI_output((Bit8u*)buffer);

					if (ds_readb(0x2845) == 20) {
						ds_writew(0x2846, 1);
					}
			}

			ds_writeb(FOOD_MESSAGE + i, 0);
		}


		/* print unconscious message */
		if ((ds_readb(0x4212 + i) != 0) && !ds_readbs(0x2c98)) {

			if (host_readb(hero + 0x21) != 0 &&
				(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)) &&
				!hero_dead(hero)) {

					/* prepare output */
					sprintf(buffer, (char*)get_ltx(0xc54),
						(char*)hero + 0x10);

					/* print output */
					GUI_output((Bit8u*)buffer);

					if (ds_readb(0x2845) == 20) {
						ds_writew(0x2846, 1);
					}
			}

			/* reset condition */
			ds_writeb(0x4212 + i, 0);
		}
	}

	ds_writeb(0x4649, 0);
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void check_level_up(void)
{
	signed short i;
	signed short done;
	Bit8u *hero;

	if (ds_readw(TIMERS_DISABLED) != 0) {
		return;
	}

	do {
		done = 0;
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += 0x6da) {

			if ((host_readbs(hero + 0x21) != 0) &&
				!hero_dead(hero) &&
				(host_readbs(hero + 0x27) < 20) &&
				(ds_readds(LEVEL_AP_TAB + 4 * host_readbs(hero + 0x27)) < host_readds(hero + 0x28)))
			{
				level_up(i);
				done = 1;
			}
		}

	} while(done);
}
#endif

struct dummy {
	char a[24];
};

/* Borlandified and identical */
void seg002_37c4(void)
{
	signed short l_si = 0;
	RealPt p1;
	RealPt p2;
	RealPt p3;
	struct dummy a = *(struct dummy*)(p_datseg + 0xc00d);

	p1 = (RealPt)ds_readd(0xd2b1) + 2000;
	p2 = (RealPt)ds_readd(0xd2b1) + 2100;
	p3 = (RealPt)ds_readd(0xd2b1) + 1000;

	if ((ds_readws(0xe4a3) != 0) && (ds_readb(TRAVELING))) {

		ds_writew(0xe4ad,
				ds_readws(0xa43b + 4 * ds_readbs(0x4343 + ds_readws(0xe4a3))));
		ds_writew(0xe4ab,
				ds_readws(0xa43d + 4 * ds_readbs(0x4343 + ds_readws(0xe4a3))));

		ds_writew(0xc011, ds_readws(0xe4ad) - 4);
		ds_writew(0xc013, ds_readws(0xe4ab) - 4);
		ds_writew(0xc015, ds_readws(0xe4ad) + 4);
		ds_writew(0xc017, ds_readws(0xe4ab) + 4);
		ds_writed(0xc019, (Bit32u)p1);

		if (is_mouse_in_rect(ds_readws(0xc011) - 16, ds_readws(0xc013) - 16, ds_readws(0xc015) + 16, ds_readws(0xc017) + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		do_pic_copy(0);

		if (l_si) {
			refresh_screen_size();
		}

		ds_writew(0xe4a3, l_si = 0);
	}

	if (ds_readws(0xe4a5) != 0) {

		ds_writew(0xc011, ds_readws(0xe4a9) - 4);
		ds_writew(0xc013, ds_readws(0xe4a7) - 4);
		ds_writew(0xc015, ds_readws(0xe4a9) + 4);
		ds_writew(0xc017, ds_readws(0xe4a7) + 4);
		ds_writed(0xc019, (Bit32u)p2);

		if (is_mouse_in_rect(ds_readws(0xc011) - 16, ds_readws(0xc013) - 16, ds_readws(0xc015) + 16, ds_readws(0xc017) + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		if (ds_readws(0xe4a5) != 0) {
			do_pic_copy(0);
		}

		if (l_si) {
			refresh_screen_size();
		}

		l_si = ds_writew(0xe4a5, 0);
	}

	if (ds_readws(0xe4b1) != 0) {

		ds_writew(0xc011, ds_readws(0xe4b1) - 4);
		ds_writew(0xc013, ds_readws(0xe4af) - 4);
		ds_writew(0xc015, ds_readws(0xe4b1) + 4);
		ds_writew(0xc017, ds_readws(0xe4af) + 4);
		ds_writed(0xc019, (Bit32u)p2);

		if (is_mouse_in_rect(ds_readws(0xc011) - 16, ds_readws(0xc013) - 16, ds_readws(0xc015) + 16, ds_readws(0xc017) + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		do_save_rect();

		if (l_si) {
			refresh_screen_size();
		}

		ds_writew(0xe4a5, 1);
		ds_writew(0xe4a9, ds_readw(0xe4b1));
		ds_writew(0xe4a7, ds_readw(0xe4af));
		l_si = 0;

		if ((ds_readws(0xe5ae) != 0) && (ds_readb(TRAVELING))) {

			ds_writew(0xe4ad,
					ds_readws(0xa43b + 4 * ds_readbs(0x4343 + ds_readws(0xe5b0))));
			ds_writew(0xe4ab,
					ds_readws(0xa43d + 4 * ds_readbs(0x4343 + ds_readws(0xe5b0))));

			ds_writew(0xc011, ds_readws(0xe4ad) - 4);
			ds_writew(0xc013, ds_readws(0xe4ab) - 4);
			ds_writew(0xc015, ds_readws(0xe4ad) + 4);
			ds_writew(0xc017, ds_readws(0xe4ab) + 4);
			ds_writed(0xc019, (Bit32u)p1);

			if (is_mouse_in_rect(ds_readws(0xc011) - 16, ds_readws(0xc013) - 16, ds_readws(0xc015) + 16, ds_readws(0xc017) + 16))
			{
				update_mouse_cursor();
				l_si = 1;
			}

			do_save_rect();
			ds_writed(0xc019, (Bit32u)(p3 + 100 * ds_readws(0xbcd8)));
			do_pic_copy(2);

			if (l_si) {
				refresh_screen_size();
			}

			ds_writew(0xe4a3, ds_readws(0xe5b0));
			l_si = 0;
		}

		ds_writew(0xc011, ds_readws(0xe4b1) - 4);
		ds_writew(0xc013, ds_readws(0xe4af) - 4);
		ds_writew(0xc015, ds_readws(0xe4b1) + 4);
		ds_writew(0xc017, ds_readws(0xe4af) + 4);
		ds_writed(0xc019, (Bit32u)(p3 + 100 * (ds_readws(0xbcd8) + 5)));

		if (is_mouse_in_rect(ds_readws(0xc011) - 16, ds_readws(0xc013) - 16, ds_readws(0xc015) + 16, ds_readws(0xc017) + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		do_pic_copy(2);

		if (l_si) {
			refresh_screen_size();
		}

		ds_writew(0xe4a5, 1);
	}


	ds_writew(0xbcd6, 0);
	inc_ds_ws(0xbcd8);

	mod_ds_ws(0xbcd8, 5);

	*(struct dummy*)(p_datseg + 0xc00d) = a;
}

/* Borlandified and identical */
void set_and_spin_lock(void)
{
	ds_writew(0xbcd6, 1);

	while (ds_readw(0xbcd6)) {
#if !defined(__BORLANDC__)
		/* deadlock avoidance */
		static int cnt = 0;

		if (cnt % 256 == 0) {
			wait_for_vsync();
		}

		cnt++;
#else
	/* just spin */
#endif
	}
}

/* Borlandified and identical */
void passages_recalc(void)
{
	signed short i;
	signed short di;
	signed short locvar;
	Bit8u *p;

	p = p_datseg + 0x6f00;

	i = get_current_season();

	locvar = (i == 2) ? 2 : ((i == 0) ? 4 : 0);

	for (i = 0; i < 45; p += 8, i++) {

		if (dec_ptr_bs(p + 4) == -1) {

			host_writeb(p + 7, (unsigned char)random_interval(70, 130));
			host_writeb(p + 4, random_interval(0, host_readbs(p + 3) * 10 + host_readbs(p + 3) * locvar) / 10);

			di = random_schick(100);

			host_writeb(p + 6,
				(!host_readbs(p + 5)) ?
					((di <= 50) ? 0 : ((di <= 80) ? 1 : (di <= 95) ? 2 : 3)) :
					((di <= 10) ? 4 : ((di <= 40) ? 5 : (di <= 80) ? 6 : 7)));
		}
	}

	/* If a passage is hired decrement Passage days timer */
	if (ds_readb(0x42ae) == 0xaa) {
		dec_ds_bs(0x42af);
	}
}

/* Borlandified and identical */
void passages_reset(void)
{
	signed short i;
	Bit8u *p = p_datseg + 0x6f00;

#ifdef M302de_ORIGINAL_BUGFIX
	for (i = 0; i < 45; p += 8, i++)
#else
	/* Orig-BUG: the loop operates only on the first element
		sizeof(element) == 8 */
	for (i = 0; i < 45; i++)
#endif
	{
		if (!host_readbs(p + 4)) {
			host_writeb(p + 4, -1);
		}
	}

	/* If a passage is hired and the timer is zero, reset the passage */
	if ((ds_readb(0x42ae) == 0xaa) && !ds_readb(0x42af)) {
		ds_writeb(0x42ae, 0);
	}
}

/**
 * \brief	forwards the ingame time
 * \param time	ticks to forward
 */
/* Borlandified and identical */
void timewarp(Bit32s time)
{
	Bit32s i;
	signed short td_bak;
	signed short hour_diff;
	Bit32s timer_bak;
	register signed short hour_old, hour_new;

	timer_bak = ds_readd(DAY_TIMER);
	td_bak = ds_readw(TIMERS_DISABLED);
	ds_writew(TIMERS_DISABLED, 0);

	ds_writeb(0xbcda, 1);

	for (i = 0; i < time; i++) {
		do_timers();
#ifdef M302de_ORIGINAL_BUGFIX
		if (i % 768 == 0)
			wait_for_vsync();
#endif
	}

	sub_ingame_timers(time);

	sub_mod_timers(time);

	seg002_2f7a(time / 0x1c2);

	sub_light_timers(time / 0x546);

	/* calculate hours */
	hour_old = timer_bak / 0x1518;
	hour_new = (signed short)(ds_readd(DAY_TIMER) / 0x1518);

	if (hour_old != hour_new) {
		if (hour_new > hour_old) {
			hour_diff = hour_new - hour_old;
		} else {
			hour_diff = 23 - hour_old + hour_new;
		}

		for (i = 0; hour_diff > i; i++) {
			magical_chainmail_damage();
			herokeeping();
		}
	}

	/* restore variables */
	ds_writeb(0xbcda, 0);
	ds_writew(TIMERS_DISABLED, td_bak);
}

/**
 * timewarp_until() -	forwards the ingame time
 * @time:	ticks to forward to e.g 6 AM
 */
/* Borlandified and identical */
void timewarp_until(Bit32s time)
{
	Bit32s i;
	signed short td_bak;
	signed short j;
	signed short hour_diff;
	Bit32s timer_bak;
	register signed short hour_old, hour_new;

	i = 0;
	timer_bak = ds_readd(DAY_TIMER);
	td_bak = ds_readw(TIMERS_DISABLED);
	ds_writew(TIMERS_DISABLED, 0);

	ds_writeb(0xbcda, 1);

	do {
		do_timers();
		i++;
#ifdef M302de_ORIGINAL_BUGFIX
		if (i % 768 == 0)
			wait_for_vsync();
#endif
	} while (ds_readds(DAY_TIMER) != time);

	sub_ingame_timers(i);

	sub_mod_timers(i);

	seg002_2f7a(i / 0x1c2);

	sub_light_timers(i / 0x546);

	/* calculate hours */
	hour_old = timer_bak / 0x1518;
	hour_new = (signed short)(ds_readd(DAY_TIMER) / 0x1518);

	if (hour_old != hour_new) {
		if (hour_new > hour_old) {
			hour_diff = hour_new - hour_old;
		} else {
			hour_diff = 23 - hour_old + hour_new;
		}

		for (j = 0; j < hour_diff; j++) {
			magical_chainmail_damage();
			herokeeping();
		}
	}

	/* restore variables */
	ds_writeb(0xbcda, 0);
	ds_writew(TIMERS_DISABLED, td_bak);
}

/**
 * \brief	decrements splash timer and restores picture
*/
/* Borlandified and identical */
void dec_splash(void)
{
	signed short i;

	for (i = 0; i <= 6; i++) {

		/* I have no clue */
		if (!ds_readbs(0x2c98) &&
			/* Check if splash timer is 0 */
			(ds_readbs(0xbccf + i) != 0) &&
			!add_ds_bu(0xbccf + i, -1) &&
			/* Check splash timer again if 0 */
			/* I have no clue */
			/* Could be in fight */
			(ds_readb(0x2845) == 0) &&
			/* check if hero is dead */
			!hero_dead(get_hero(i)))
		{
			restore_rect((RealPt)ds_readd(0xd2ff), get_hero(i) + 0x2da, ds_readw(0x2d01 + i * 2), 157, 32, 32);
		}
	}
}

/**
 * \brief		draws a splash on a hero portrait
 *
 * \param hero_pos	on which slot the splash is drawn
 * \param type		kind of damage (0 = red,LE / !0 = yellow,AE)
*/
/* Borlandified and identical */
/* static */
void draw_splash(signed short hero_pos, signed short type)
{
	/* Could be in fight */
	if (ds_readb(0x2845) == 0) {

		Bit8u *splash = (type == 0) ? Real2Host(ds_readd(SPLASH_LE)) : Real2Host(ds_readd(SPLASH_AE));

		restore_rect_rle(Real2Phys(ds_readd(0xd2ff)), splash, ds_readw(0x2d01 + 2 * hero_pos), 157, 32, 32, 2);

		/* how long the splash should be displayed */
		ds_writeb(0xbccf + hero_pos, 10);
	}
}


/**
 * \brief	fast forward the ingame time to midnight
 */
/* Borlandified and identical */
void timewarp_until_midnight(void)
{
	Bit32s ticks_left;
	signed short td_bak;

	/* save the timers status */
	td_bak = ds_readw(TIMERS_DISABLED);

	/* enable timers */
	ds_writew(TIMERS_DISABLED, 0);

	/* calculate the ticks left on this day */
	ticks_left = (HOURS(24) -1) - ds_readd(DAY_TIMER);

	/* set the day timer to the last tick of this day */
	ds_writed(DAY_TIMER, (HOURS(24) - 1));

	do_timers();
	sub_ingame_timers(ticks_left);
	sub_mod_timers(ticks_left);
	seg002_2f7a(ticks_left / 450);
	sub_light_timers(100);

	/* restore the timer status */
	ds_writew(TIMERS_DISABLED, td_bak);
}

/* Borlandified and identical */
void wait_for_keyboard2(void)
{
	while (CD_bioskey(1)) {
#if !defined(__BORLANDC__)
		D1_LOG("loop in %s\n", __func__);
#endif
		bc_bioskey(0);
	}
}


/* Borlandified and identical */
/* unused */
void seg002_4031(Bit8u *ptr)
{
	delay_or_keypress(150 * GUI_print_header(ptr));
}

/* Borlandified and identical */
void wait_for_keypress(void)
{
	signed short si;

	bc_flushall();

	ds_writew(0xc3d5, 0);

	do {
		if (CD_bioskey(1)) {

			si = bc_bioskey(0);

			if (((si & 0xff) == 0x20) &&
				(ds_readw(0xc3c5) == 0))
			{

				seg002_47e2();

				while (!CD_bioskey(1)) {;}

				seg002_484f();
				break;
			}
		}

	} while (!CD_bioskey(1) && ds_readw(0xc3d5) == 0);

	if (CD_bioskey(1))
		si = bc_bioskey(0);

	ds_writew(0xc3d5, 0);
}

/**
 * \brief	wait for some time is elapsed or a key is pressed
 *
 * \param	duration	the maximal time to wait
 */
/* Borlandified and identical */
void delay_or_keypress(signed short duration)
{
	signed short done = 0;;
	signed short counter = 0;

	while (counter < duration) {

		ds_writeb(0x46a2, 1);
		handle_input();
		ds_writeb(0x46a2, 0);

		if (ds_readb(0x7c42) != 0) {

			if (ds_readw(ACTION) != 0) {

				if (ds_readw(ACTION) == 57) {

					seg002_47e2();
					while (!CD_bioskey(1)) { ; }
					seg002_484f();
					done = 1;

				} else {

					if ((ds_readw(ACTION) != 72) &&
						(ds_readw(ACTION) != 80) &&
						(ds_readw(ACTION) != 77) &&
						(ds_readw(ACTION) != 75))
					{
						done = 1;
					}
				}
			} else {

				if (ds_readw(0xc3d3) != 0) {
					done = 1;
				}
			}
		} else {

			if (ds_readw(ACTION) != 0) {

				if (ds_readw(ACTION) == 57) {

					seg002_47e2();
					while (!CD_bioskey(1)) { ; }
					seg002_484f();
				}

				done = 1;

			} else {

				if (ds_readw(0xc3d3) != 0) {
					done = 1;
				}
			}
		}


		if (done) {
			ds_writew(0xc3d3, 0);
			ds_writew(ACTION, 28);
			break;
		}

		wait_for_vsync();

		counter++;
	}
}

/* Borlandified and identical */
/* unused */
void unused_delay(signed short nr)
{
	signed short i = 0;

	while (i < nr) {
		wait_for_vsync();
		i++;
	}
}

/* Borlandified and identical */
/* unused */
void unused_spinlock(void)
{
	ds_writew(0xc3c9, 1);

	while (ds_readw(0xc3c9) != 0) {
	}
}

/**
 * \brief	calculates a 32bit BigEndian value into LittleEndian
 * \param v	32bit BE value
 * \return	32bit LE value
 */
/* Borlandified and identical */
Bit32u swap_u32(Bit32u v)
{
	register signed short tmp;
	signed short a[2];
	Bit32s *ptr = (Bit32s*)(&a[0]);

	*ptr = host_readd((Bit8u*)&v);

	tmp = a[0];
	a[0] = swap_u16(a[1]);
	a[1] = swap_u16(tmp);

	return host_readd((Bit8u*)ptr);
}

/* unused */
/* Borlandified and identical */
Bit32u swap_u32_unused(Bit32u v)
{
	signed short a[2];
	signed short tmp;
	Bit32s *ptr = (Bit32s*)(&a[0]);

	tmp = *ptr = host_readd((Bit8u*)&v);

	a[0] = a[1];
	a[1] = tmp;

	return host_readd((Bit8u*)ptr);
}

/**
 *	alloc_EMS -	allocates EMS memory
 *	@bytes:		bytes to allocate
 *
 *	Returns an EMS handle, to access the memory.
 */
/* Borlandified and identical */
signed short alloc_EMS(Bit32s bytes)
{
	signed short handle;

	/* calculate the number of needes EMS pages */
	handle = (signed short)((bytes / 0x4000) + 1);

	/* check if enought EMS is free */
	if (EMS_get_num_pages_unalloced() >= handle) {

		/* if allocation was successful return the handle */
		if ((handle = EMS_alloc_pages(handle)) != 0) {
			return handle;
		}
	}

	return 0;
}

/* Borlandified and identical */
void from_EMS(RealPt dst, signed short handle, Bit32s bytes)
{
	signed short si;
	signed short di;
	signed short v1, v2;
	RealPt ptr;

	di = (signed short)(bytes / 0x4000 + 1);
	v1 = si = 0;

	do {
		EMS_map_memory(handle, v1++, 0);
		ptr = F_PADD(dst, (((Bit32s)si) << 0x0e));
		si++;

		v2 = (bytes - 0x4000 > 0) ? 0x4000 : bytes;

		bytes -= 0x4000;

		bc_memmove(EMS_norm_ptr(ptr), (RealPt)ds_readd(0x4baa), v2);

	} while (--di != 0);

}

/* Borlandified and identical */
void to_EMS(signed short handle, RealPt src, Bit32s bytes)
{
	signed short si;
	signed short di;
	signed short v1, v2;
	RealPt ptr;

	di = (signed short)(bytes / 0x4000 + 1);
	v1 = si = 0;

	do {
		EMS_map_memory(handle, v1++, 0);
		ptr = F_PADD(src, ((((Bit32s)si) << 0x0e)));
		si++;

		v2 = (bytes - 0x4000 > 0) ? 0x4000 : bytes;

		bytes -= 0x4000;

		bc_memmove((RealPt)ds_readd(0x4baa), EMS_norm_ptr(ptr), v2);

	} while (--di != 0);

}

/* Borlandified and identical */
void set_to_ff(void)
{
	signed short i;

	for (i = 0; i < 9; i++) {
		ds_writeb(0xbd38 + i, -1);
	}
}

/**
 * \brief	draws the icons of locations
 * \param icons	number of icons
 * \param ...	icon ids
 */
/* Borlandified and identical */
void draw_loc_icons(signed short icons, ...)
{
	signed short icons_bak[9];
	va_list arguments;
	signed short i, changed;

	changed = 0;

	/* save icon ids in local variable */
	for (i = 0; i < 9; i++) {
		icons_bak[i] = ds_readbs(0xbd38 + i);
		ds_writeb(0xbd38 + i, -1);
	}

	va_start(arguments, icons);

	for (i = 0; i < icons; i++) {
		ds_writeb(0xbd38 + i, va_arg(arguments, int));

		if (ds_readbs(0xbd38 + i) != icons_bak[i]) {
			changed = 1;
		}
	}

	if (icons_bak[i] != -1)
		changed = 1;

	if (changed && ds_readb(0x2845) == 0) {
		draw_icons();
	}
}

/* Borlandified and identical */
signed short mod_day_timer(signed short val)
{
	return ((ds_readds(DAY_TIMER) % val) == 0) ? 1 : 0;
}

/* Borlandified and identical */
void draw_compass(void)
{
	signed short width;
	signed short height;
	struct nvf_desc n;

	/* No compass in a location */
	if (!ds_readbs(LOCATION) &&
		/* Has something to do with traveling */
		!ds_readbs(0xb132) &&
		/* Not in town or dungeon */
		((ds_readbs(DUNGEON_INDEX) != 0) || (ds_readbs(CURRENT_TOWN) != 0)) &&
		/* I have no clue */
		(ds_readb(0x4475) != 2))
	{

		/* set src */
		n.dst = Real2Host(ds_readd(0xd2f7));
		/* set dst */
		n.src = Real2Host(ds_readd(0xd2b1));
		/* set nr */
		n.nr = ds_readbs(DIRECTION);
		/* set type*/
		n.type = 0;

		n.width = (Bit8u*)&width;
		n.height = (Bit8u*)&height;

		/* process the nvf */
		process_nvf(&n);

		/* set x and y values */
		ds_writew(0xc011, 94);
		ds_writew(0xc013, 115);
		ds_writew(0xc015, 145);
		ds_writew(0xc017, 136);

		/* set source */
		ds_writed(0xc019, ds_readd(0xd2f7));

		update_mouse_cursor();
		do_pic_copy(2);
		refresh_screen_size();
	}
}

/* Borlandified and identical */
signed short can_merge_group(void)
{
	signed short i;
	signed short retval = -1;

	if (ds_readbs(ds_readbs(CURRENT_GROUP) + 0x2d36) == ds_readbs(0x2d3c)) {

		retval = -1;

	} else {

		for (i = 0; i < 6; i++)	{

			if ((i != ds_readbs(CURRENT_GROUP)) &&
				(0 != ds_readb(i + 0x2d36)) &&
				/* check XTarget */
				(ds_readw(i * 2 + 0x2d48) == ds_readw(X_TARGET)) &&
				/* check YTarget */
				(ds_readw(i * 2 + 0x2d54) == ds_readw(Y_TARGET)) &&
				/* check Location */
				(ds_readb(0x2d61 + i) == ds_readb(LOCATION)) &&
				/* check currentTown */
				(ds_readb(0x2d68 + i) == ds_readb(CURRENT_TOWN)) &&
				/* check DungeonIndex */
				(ds_readb(0x2d6f + i) == ds_readb(DUNGEON_INDEX)) &&
				/* check DungeonLevel */
				(ds_readb(0x2d76 + i) == ds_readb(DUNGEON_LEVEL)))
			{
				retval = i;
			}
		}
	}

	return retval;
}

/* Borlandified and identical */
signed short div16(signed short val)
{
	return ((unsigned char)val) >> 4;
}

/* This function is called in shops at sell/buy screens */
/* Borlandified and identical */
void select_with_mouse(Bit8u *p1, Bit8u *p2)
{
	signed short i;

	/* something mouse related */
	if (ds_readw(0xc3c7) != 2) {
		return;
	}

	for (i = 0; i < 15; i++) {
		if ((ds_readws(0x46a3 + i * 2) <= ds_readws(0x299c)) &&
			(ds_readws(0x46a3 + i * 2) + 50 >= ds_readws(0x299c)) &&
			(ds_readws(0x46c1 + i * 2) <= ds_readws(0x299e)) &&
			(ds_readws(0x46c1 + i * 2) + 17 >= ds_readws(0x299e)) &&
			(host_readws(p2 + i * 7) != 0))
		{
			host_writew(p1, i);
			return;
		}
	}
}

/* Borlandified and identical */
void select_with_keyboard(Bit8u *p1, Bit8u *p2)
{
	signed short pos = host_readws(p1);

	if (ds_readw(ACTION) == 72) {
		/* Key UP */
		if (pos) {
			pos--;
		} else {
			pos = 14;
			while (host_readw(p2 + pos * 7) == 0) {
				pos--;
			}
		}
	} else if (ds_readw(ACTION) == 80) {
		/* Key DOWN */
		if (pos < 14) {
			if (host_readw(p2 + (pos + 1) * 7) != 0) {
				pos++;
			} else {
				pos = 0;
			}
		} else {
			pos = 0;
		}
	} else if (ds_readw(ACTION) == 77) {
		/* Key RIGHT */
		if (pos < 10) {
			if (host_readw(p2 + (pos + 5) * 7) != 0) {
				pos += 5;
			}
		} else {
			pos -= 10;
		}
	} else if (ds_readw(ACTION) == 75) {
		/* Key LEFT */
		if (pos > 4) {
			pos -= 5;
		} else {
			if (host_readw(p2 + (pos + 10) * 7) != 0) {
				pos += 10;
			}
		}
	}

	host_writew(p1, pos);
}

/**
 * \brief	marks a tile in the automap as seen
 * \param x	X coordinate
 * \param y	Y coordinate
*/
/* Borlandified and identical */
void set_automap_tile(signed short x, signed short y)
{
	or_ds_bs(0xe442 + (4 * y + (x >> 3)), ds_readb(0x7d4a + (x & 0x7)));
}

/**
 * \brief	marks all sourrounding tiles in the automap as seen
 *
 * \param x	X xoordinate
 * \param y	Y xoordinate
*/
/* Borlandified and identical */
void set_automap_tiles(signed short x, signed short y)
{
	/* set upper line */
	if (y > 0) {

		if (x > 0) {
			set_automap_tile(x - 1, y - 1);
		}

		set_automap_tile(x, y - 1);

		if (ds_readb(0xbd94) - 1 > x) {
			set_automap_tile(x + 1, y - 1);
		}
	}

	/* set middle line */
	if (x > 0) {
		set_automap_tile(x - 1, y);
	}

	set_automap_tile(x, y);

	if (ds_readb(0xbd94) - 1 > x) {
		set_automap_tile(x + 1, y);
	}

	/* set lower line */
	if (y < 15) {
		if (x > 0) {
			set_automap_tile(x - 1, y + 1);
		}

		set_automap_tile(x, y + 1);

		if (ds_readb(0xbd94) - 1 > x) {
			set_automap_tile(x + 1, y + 1);
		}
	}
}

/**
 * \brief
*/
/* Borlandified and identical */
void seg002_47e2(void)
{
	/* save gfx settings to stack */
	struct dummy a = *(struct dummy*)(p_datseg + 0xc00d);

	/* set range 0,0 - 7,7 */
	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 7);
	ds_writew(0xc017, 7);

	/* set destination */
	ds_writed(0xc00d, ds_readd(0xd2ff));
	/* set source */
	ds_writed(0xc019, (Bit32u)RealMake(datseg, 0xbc63));

	do_save_rect();

	GUI_print_char('P', 0, 0);

	/* restore gfx settings from stack */
	*(struct dummy*)(p_datseg + 0xc00d) = a;
}

/**
*/
/* Borlandified and identical */
void seg002_484f(void)
{
	/* save gfx settings to stack */
	struct dummy a = *(struct dummy*)(p_datseg + 0xc00d);

	/* set range 0,0 - 7,7 */
	ds_writew(0xc011, 0);
	ds_writew(0xc013, 0);
	ds_writew(0xc015, 7);
	ds_writew(0xc017, 7);

	/* set destination */
	ds_writed(0xc00d, ds_readd(0xd2ff));
	/* set source */
	ds_writed(0xc019, (Bit32u)RealMake(datseg, 0xbc63));

	do_pic_copy(0);

	/* restore gfx settings from stack */
	*(struct dummy*)(p_datseg + 0xc00d) = a;
}

/**
 * \brief	returns true if heros not dead, stoned or unconscious
 *
 * \param hero	pointer to the hero
 *
 * \return {0, 1}
 */
/* should be static */
/* Borlandified and identical */
signed short check_hero(Bit8u *hero)
{
	if (!host_readbs(hero + 0x21) ||
		hero_sleeps(hero) ||
		hero_dead(hero) ||
		hero_stoned(hero) ||
		hero_unc(hero) ||
		hero_cursed(hero) ||
		/* Check if ??? */
		(host_readb(hero + 0x84) == 0x10))
	{
		return 0;
	}

	return 1;
}

/**
 * \brief	returns true if heros not dead, stoned or unconscious
*/
/* should be static */
/* Borlandified and identical */
signed short check_hero_no2(Bit8u *hero)
{

	if (!host_readbs(hero + 0x21) ||
		hero_dead(hero) ||
		hero_stoned(hero) ||
		hero_unc(hero) ||
		hero_cursed(hero))
	{
		return 0;
	}

	return 1;
}

/**
 * \brief check if hero is not dead, stoned or unconscious
 * \param hero	pointer to the hero
 * \return {0, 1}
 */
/* should be static */
/* Borlandified and identical */
signed short check_hero_no3(Bit8u *hero)
{
	if (!host_readbs(hero + 0x21) ||
		hero_dead(hero) ||
		hero_stoned(hero) ||
		hero_unc(hero))
	{
		return 0;
	}

	return 1;
}

/* Borlandified and identical */
signed short is_hero_available_in_group(Bit8u *hero)
{

	if (check_hero(hero) && (host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP))) {
		return 1;
	}

	return 0;
}

/**
 * \brief	subtract current ae with a splash
 * \param hero	the magicuser
 * \param ae	astralenergy to subtract
 */
/* Borlandified and identical */
void sub_ae_splash(Bit8u *hero, signed short ae)
{
	if (!hero_dead(hero) && (ae > 0)) {

		signed short tmp = ds_readw(0xc3cb);
		ds_writew(0xc3cb, 0);

		/* If Mage has 4th Staffspell */
		if ((host_readb(hero + 0x21) == 9) && (host_readbs(hero + 0x195) >= 4)) {
			ae -= 2;
			if (ae < 0)
				ae = 0;
		}

		/* Calc new AE */
		sub_ptr_ws(hero + 0x64, ae);

		/* Draw the splash */
		draw_splash(get_hero_index(hero), 1);

		/* set AE to 0 if they have gotten lower than 0 */
		if (host_readws(hero + 0x64) < 0) {
			host_writew(hero + 0x64, 0);
		}

		ds_writew(0xc3cb, tmp);

#ifdef M302de_ORIGINAL_BUGFIX
		/* AE Bar was not updated in Pseudo 3D mode */
		if (ds_readw(IN_FIGHT) == 0 && ds_readw(0xc3cf) != 0) {
			/* redraw AE Bar */
			draw_bar(1, get_hero_index(hero), host_readw(hero + 0x64),
				host_readw(hero + 0x62), 0);
		}
#endif
	}

}

/**
 * \brief	add AE points to heros current AE
*/
/* Borlandified and identical */
void add_hero_ae(Bit8u* hero, signed short ae)
{
	/* dont add AE if hero is dead or ae = 0 */
	if (!hero_dead(hero) && (ae > 0)) {

		signed short tmp = ds_readw(0xc3cb);
		ds_writew(0xc3cb, 0);

		/* add AE to heros current AE */
		add_ptr_ws(hero + 0x64, ae);

		/* if current AE is greater than AE maximum
			set current AE to AE maximum */
		if (host_readws(hero + 0x64) > host_readws(hero + 0x62))
			host_writew(hero + 0x64, host_readws(hero + 0x62));

		ds_writew(0xc3cb, tmp);
	}
}

/**
 * \brief	subtracts LE from a hero
 *
 * \param hero	pointer to the hero
 * \param le	LE the hero looses
 */
/* Borlandified and identical */
void sub_hero_le(Bit8u *hero, signed short le)
{
	signed short i;
	signed short bak;
	signed short old_le;
	Bit8u *ptr;
	Bit8u *hero_i;

	if (!hero_dead(hero) && (le > 0)) {

		bak = ds_readw(0xc3cb);
		ds_writew(0xc3cb, 0);

		/* do the damage */
		old_le = host_readw(hero + 0x60);
		sub_ptr_ws(hero + 0x60, le);

		if (hero_sleeps(hero)) {

			/* awake him/her */
			and_ptr_bs(hero + 0xaa, 0xfd);

			/* in fight mode */
			if (ds_readw(IN_FIGHT) != 0) {
				ptr = Real2Host(FIG_get_ptr(host_readb(hero + 0x81)));

				/* update looking dir and other  */
				host_writeb(ptr + 2, host_readb(hero + 0x82));
				host_writeb(ptr + 0xd, -1);
				host_writeb(ptr + 5, 0);
				host_writeb(ptr + 6, 0);
			}
		}

		draw_splash(get_hero_index(hero), 0);

		if (host_readws(hero + 0x60) <= 0) {

			/* set LE to 0 */
			host_writew(hero + 0x60, 0);

			/* mark hero as dead */
			or_ptr_bs(hero + 0xaa, 1);

			/* unknown */
			ds_writeb(0x4212 + get_hero_index(hero), 0);

			/* unknown */
			host_writeb(hero + 0x84, 100);

			if (ds_readb(0x2845) == 0) {
				ds_writeb(0x46df, 1);
			}

			/* reset sickness */
			for (i = 1; i <= 7; i++) {
				host_writeb(hero + 0xae + i * 5, 0);
				host_writeb(hero + 0xaf + i * 5, 0);
			}

			/* reset poison */
			for (i = 1; i <= 9; i++) {
				host_writeb(hero + 0xd6 + i * 5, 0);
				host_writeb(hero + 0xd7 + i * 5, 0);
			}

			/* FINAL FIGHT */
			if (ds_readw(CURRENT_FIG_NR) == 192) {
				if (hero == Real2Host(ds_readd(0x3e20))) {
					ds_writew(0xc3c1, 1);
					ds_writew(IN_FIGHT, 0);
				}
			}

			if ((ds_readb(0xa842) != 0)
				&& (ds_readw(IN_FIGHT) == 0) &&
				(!count_heroes_available_in_group() ||
				((count_heroes_available_in_group() == 1) && (is_hero_available_in_group(get_hero(6))))))
			{

				ds_writeb(0x4333, 99);

				hero_i = get_hero(0);
				for (i = 0; i <=6; i++, hero_i += 0x6da) {
					/* no typus */
					if ((host_readbs(hero_i + 0x21) != 0) &&
						(host_readbs(hero_i + 0x87) == ds_readbs(CURRENT_GROUP)))
					{
						hero_disappear(hero_i, i, -1);
					}
				}
			}

		} else {
			if ((old_le >= 5) && (host_readws(hero + 0x60) < 5)) {

				/* make hero unsonscious */
				or_ptr_bs(hero + 0xaa, 0x40);

				/* unknown yet */
				host_writeb(hero + 0x84, 10);

				/* unknown yet */
				ds_writeb(0x4212 + get_hero_index(hero), 1);

				/* in fight mode */
				if (ds_readw(IN_FIGHT) != 0) {

					ptr = Real2Host(FIG_get_ptr(host_readb(hero + 0x81)));

					host_writeb(ptr + 2,
						ds_readb(0x11e4 + host_readbs(hero + 0x9b) * 2) + host_readbs(hero + 0x82));

					host_writeb(ptr + 0x0d, -1);

					host_writeb(ptr + 5,
						ds_readb(0x1210 + host_readbs(hero + 0x9b) * 8 + host_readbs(hero + 0x82) * 2));

					host_writeb(ptr + 6,
						ds_readb(0x1211 + host_readbs(hero + 0x9b) * 8 + host_readbs(hero + 0x82) * 2));


					FIG_add_msg(7, 0);

					/* FINAL FIGHT */
					if (ds_readw(CURRENT_FIG_NR) == 192) {
						if (hero == Real2Host(ds_readd(0x3e20))) {
							ds_writew(0xc3c1, 1);
							ds_writew(IN_FIGHT, 0);
						}
					}
				}
			}
		}

		ds_writew(0xc3cb, bak);
	}

	if (ds_readw(IN_FIGHT) == 0) {
		ds_writeb(CHECK_PARTY, 1);
	}
}

/**
 * \brief	regenerates LE of a hero
 * \param hero	pointer to the hero
 * \param le	LE to be regenerated
 *
 * This functions does some magic in fights, when a hero awakes.
 */
/* Borlandified and identical */
void add_hero_le(Bit8u *hero, signed short le)
{
	signed short val_bak;
	Bit8u *ptr;
	signed short ret;

	/* dead heroes never get LE */
	if (!hero_dead(hero) && (le > 0)) {

		val_bak = ds_readw(0xc3cb);
		ds_writew(0xc3cb, 0);

		/* add LE */
		add_ptr_ws(hero + 0x60, le);

		/* set LE to maximum if greater than maximum */
		if (host_readws(hero + 0x60) > host_readws(hero + 0x5e))
			host_writew(hero + 0x60, host_readws(hero + 0x5e));

		/* if current LE is >= 5 and the hero is unconscissous */
		if ((host_readws(hero + 0x60) >= 5) && hero_unc(hero)) {

			/* awake */
			and_ptr_bs(hero + 0xaa, 0xbf);

			/* maybe if we are in a fight */
			if (ds_readw(IN_FIGHT)) {
				ptr = Real2Host(FIG_get_ptr(host_readb(hero + 0x81)));
				ret = FIG_get_range_weapon_type(hero);

				if (ret != -1) {
					host_writeb(ptr + 2, ds_readb(0x10d0 +
						host_readbs(hero + 0x9b) * 12 + 4 * ret + host_readbs(hero + 0x82)));
				} else {
					host_writeb(ptr + 2, host_readb(hero + 0x82));
				}

				host_writeb(ptr + 0x0d, -1);
				host_writeb(ptr + 5, 0);
				host_writeb(ptr + 6, 0);
			}
		}

		ds_writew(0xc3cb, val_bak);
	}
}

/**
 * \brief	regenerates LE of a group
 *
 * \param le	LE to be regenerated
 */
/* Borlandified and identical */
void add_group_le(signed short le)
{

	Bit8u *hero;
	signed short i;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += 0x6da) {

		/* check class and group */
		if ((host_readb(hero + 0x21) != 0) &&
			(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)))
		{
			add_hero_le(hero, le);
		}
	}
}

/**
 * \brief	damages starving heros
 *
 * \param hero	a pointer to the hero
 * \param index	the index number of the hero
 * \param type	the type of message which should be printed (0 = hunger / 1 = thirst)
 */
/* Borlandified and identical */
void do_starve_damage(Bit8u *hero, signed short index, signed short type)
{
	/* check if the hero is dead */
	if (!hero_dead(hero)) {

		/* save this value locally */
		signed short bak = ds_readw(0xc3cb);
		ds_writew(0xc3cb, 0);

		/* decrement the heros LE */
		dec_ptr_ws(hero + 0x60);

		/* set the critical message type for the hero */
		ds_writeb(FOOD_MESSAGE + index, type != 0 ? 1 : 2);

		if (host_readws(hero + 0x60) <= 0) {

			/* don't let the hero die */
			host_writew(hero + 0x60, 1);

			/* decrement the max LE and save them at 0x7a */
			if (host_readws(hero + 0x5e) >= 2) {
				dec_ptr_ws(hero + 0x5e);
				inc_ptr_bs(hero + 0x7a);
			}
		}

		/* restore the locally save value */
		ds_writew(0xc3cb, bak);
	}
}

/* unused */
/* Borlandified and identical */
signed short compare_name(Bit8u *name)
{
	signed short i;

	for (i = 0; i < 6; i++) {

		if (!strcmp((char*)(get_hero(i) + 0x10), (char*)name)) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief		make an attribute test
 *
 * \param hero		pointer to the hero
 * \param attrib	number of the attribute
 * \param bonus		handicap
 *
 * \return the result of the test, successful if greater than zero.
 */
/* Borlandified and identical */
signed short test_attrib(Bit8u* hero, signed short attrib, signed short bonus)
{
	signed short si = random_schick(20);
	signed short tmp;

#if !defined(__BORLANDC__)
	D1_INFO("Eigenschaftsprobe %s auf %s %+d: W20 = %d",
		(char*)(hero+0x10), names_attrib[attrib], bonus, si);
#endif

	if (si == 20) {
#if !defined(__BORLANDC__)
		D1_INFO("Ungluecklich\n");
#endif
		return -99;
	} else {

		si += bonus;
	}

	tmp = host_readbs(hero + 3 * attrib + 0x35) + host_readbs(hero + 3 * attrib + 0x36);

#if !defined(__BORLANDC__)
	D1_INFO(" -> %s mit %d\n",
		(tmp - si + 1) > 0 ? "bestanden" : "nicht bestanden", (tmp - si + 1));
#endif

	return tmp - si + 1;
}

/**
 * \brief		make three attribute tests
 *
 * \param hero		pointer to the hero
 * \param attrib1	attribute 1
 * \param attrib2	attribute 2
 * \param attrib3	attribute 3
 * \param bonus		handycap
 *
 * \return		a test is positive if the return value is greater than zero
 */
/* Borlandified and identical */
signed short test_attrib3(Bit8u* hero, signed short attrib1, signed short attrib2, signed short attrib3, signed char bonus)
{

	signed short i;
	signed short si;
	signed short tmp;
	signed short zw;

	zw = 0;
	si = 0;

#if !defined(__BORLANDC__)
	D1_INFO("%s -> (%s/%s/%s) %+d: ",
		(char*)(hero+0x10), names_attrib[attrib1],
		names_attrib[attrib2], names_attrib[attrib3], bonus);
#endif

	for (i = 0; i < 3; i++) {

		tmp = random_schick(20);

#if !defined(__BORLANDC__)
		D1_INFO("%d ", tmp);
#endif

		if (tmp == 20) {
			if (++zw == 2) {
#if !defined(__BORLANDC__)
				D1_INFO(" -> UNGLUECKLICH! nicht bestanden\n");
#endif
				return -99;
			}
		}

		si += tmp;
	}

	si += bonus;

	tmp = host_readbs(hero + 3 * attrib1 + 0x35) +
		host_readbs(hero + 3 * attrib1 + 0x36) +
		host_readbs(hero + 3 * attrib2 + 0x35) +
		host_readbs(hero + 3 * attrib2 + 0x36) +
		host_readbs(hero + 3 * attrib3 + 0x35) +
		host_readbs(hero + 3 * attrib3 + 0x36);

#if !defined(__BORLANDC__)
	D1_INFO(" -> %s mit %d\n",
		(tmp - si + 1) > 0 ? "bestanden" : "nicht bestanden", (tmp - si + 1));
#endif
	return tmp - si + 1;
}

/* Borlandified and identical */
signed short unused_cruft(void)
{

	signed short l_si;

	if (!ds_readbs(0x2d3c)) {
		return -1;
	}

	do {
		l_si = random_schick(6) - 1;

	} while (!(host_readbs(get_hero(l_si) + 0x21)) || (host_readbs(get_hero(l_si) + 0x87) != ds_readbs(CURRENT_GROUP)));

	return l_si;
}

/**
 * \brief	selects a hero randomly
 *
 * \return position of a randomly choosen hero
*/
/* Original-Bug: can loop forever if the position is greater than the
	number of heroes in the group */
/* Borlandified and identical */
signed short get_random_hero(void)
{
	signed short cur_hero;

	do {
		/* get number of current group */
		cur_hero = random_schick(ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP))) - 1;

#ifdef M302de_ORIGINAL_BUGFIX
		signed short pos = 0;

		Bit8u *hero = get_hero(0);
		for (int i = 0; i <= 6; i++, hero += 0x6da) {

			/* Check if hero has a class */
			if (host_readbs(hero + 0x21) == 0)
				continue;
			/* Check if in current group */
			if (host_readbs(hero + 0x87) != ds_readbs(CURRENT_GROUP))
				continue;

			if (pos == cur_hero) {
				pos = i;
				break;
			}

			pos++;
		}
		cur_hero = pos;
#endif

	} while (!host_readbs(get_hero(cur_hero) + 0x21) ||
			/* Check if in current group */
			(host_readbs(get_hero(cur_hero) + 0x87) != ds_readbs(CURRENT_GROUP)) ||
			/* Check if dead */
			hero_dead(get_hero(cur_hero)));

	return cur_hero;
}

/**
 * \brief	get the money of the current group
 *
 * \return	the sum of the money of all heros in the current group
*/
/* Borlandified and identical */
Bit32s get_party_money(void)
{
	signed short i;
	Bit32s sum = 0;
	Bit8u* hero;

	hero = get_hero(0);

	for (i=0; i < 6; i++, hero += 0x6da) {
		/* Check if hero has a class and is in the current group */
		if (host_readbs(hero + 0x21) &&
			(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)))
		{
			sum += host_readds(hero + 0x2c);
		}
	}

	return sum;
}

/**
 * \brief	shares money between current party members
 *
 * \param money	the money to share
 *
 *	If only a NPC is in that party, he gets all the money.
 *	If a hero is dead and in the current party, his money is set to 0.
*/
/* Borlandified and identical */
void set_party_money(Bit32s money)
{
	signed short heroes = 0;
	signed short i;
	Bit32s hero_money;
	Bit8u *hero;

	if (money < 0)
		money = 0;

	heroes = count_heroes_in_group();

	/* set hero to NPC */
	hero = get_hero(6);

	/* if we have an NPC in current group and alive */
	if (host_readbs(hero + 0x21) &&
		(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)) &&
		!hero_dead(hero)) {

		/* If only the NPC is in that group give him all the money */
		if (heroes > 1) {
			heroes--;
		} else {
			add_ptr_ds(hero + 0x2c, money);
			heroes = 0;
		}
	}

	if (heroes != 0) {

		hero_money = money / heroes;

		hero = get_hero(0);

		for (i = 0; i < 6; i++, hero += 0x6da) {

			if (host_readbs(hero + 0x21) &&
				(host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)) &&
				!hero_dead(hero))
			{
				/* account the money to hero */
				host_writed(hero + 0x2c, hero_money);
			} else {
				if (host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP)) {
					host_writed(hero + 0x2c, 0);
				}
			}
		}
	}
}

/**
 * \brief	adds money to the current group
 *
 * \param money	money to add
*/
/* Borlandified and identical */
void add_party_money(Bit32s money)
{
	set_party_money(get_party_money() + money);
}

/**
 * \brief	add AP to a hero
 *
 * \param hero	pointer to the hero
 * \param ap	AP the hero should get
*/
/* Borlandified and identical */
void add_hero_ap(Bit8u *hero, Bit32s ap)
{
	add_ptr_ds(hero + 0x28, ap);
}

/**
 * \brief	shares AP in the current group
 * \param ap	AP to share
 */
/* Borlandified and identical */
void add_group_ap(Bit32s ap)
{
	signed short i;
	Bit8u *hero_i;

	if (ap < 0) {
		return;
	}

	ap = ap / count_heroes_in_group();

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {

		/* Check class, group and deadness */
		if (host_readbs(hero_i + 0x21) &&
			(host_readbs(hero_i + 0x87) == ds_readbs(CURRENT_GROUP)) &&
			!hero_dead(hero_i))
		{
			add_hero_ap(hero_i, ap);
		}
	}
}

/**
 * \brief	add AP to every hero in the group
 *
 * \param ap	AP to add
*/
/* Borlandified and identical */
void add_hero_ap_all(signed short ap)
{
	Bit8u *hero_i;
	signed short i;

	if (ap < 0)
		return;

	hero_i = get_hero(0);
	for (i = 0; i <= 6; i++, hero_i += 0x6da) {

		/* Check class, group and deadness */
		if (host_readbs(hero_i + 0x21) &&
			(host_readbs(hero_i + 0x87) == ds_readbs(CURRENT_GROUP)) &&
			!hero_dead(hero_i))
		{
#if !defined(__BORLANDC__)
			D1_INFO("%s erhaelt %d AP\n",(char*)(hero_i+0x10), ap);
#endif

			add_hero_ap(hero_i, ap);
		}
	}
}

/**
 * \brief	subtracts AP from every hero in the group
 *
 * \param ap	AP to subtract
*/
/* Borlandified and identical */
void sub_hero_ap_all(signed short ap)
{
	signed short i;
	Bit8u *hero_i;

	if (ap < 0)
		return;

	hero_i = get_hero(0);
	for (i = 0; i <= 6; i++, hero_i += 0x6da) {

		/* Check class, group and deadness */
		if (host_readbs(hero_i + 0x21) &&
			(host_readbs(hero_i + 0x87) == ds_readbs(CURRENT_GROUP)) &&
			!hero_dead(hero_i))
		{
			if (ap <= host_readd(hero_i + 0x28)) {
#if !defined(__BORLANDC__)
				D1_INFO("%s erhaelt %+d AP\n",(char*)(hero_i+0x10), -ap);
#endif
				add_hero_ap(hero_i, -((Bit32s)ap));
			} else {
#if !defined(__BORLANDC__)
				D1_INFO("%s wird auf 0 AP gesetzt\n",(char*)(hero_i+0x10));
#endif
				host_writed(hero_i + 0x28, 0);
			}
		}
	}
}

/**
 * \brief	gets the position of a hero
 *
 * \param hero	pointer to the hero
 *
 * \return	position of the hero
*/
/* Borlandified and identical */
signed short get_hero_index(Bit8u *hero)
{
	signed short i = 0;
	Bit8u *p;

	p = get_hero(i);
	while (hero != p) {
		i++;
		p = get_hero(i);
	}

	return i;
}

/**
 * \brief	gets item position
 *
 * \param hero	pointer to the hero
 * \param item	item ID to look for
 *
 * \return position of the item or -1 if the item is not in the inventory.
*/
/* Borlandified and identical */
signed short get_item_pos(Bit8u *hero, signed short item)
{

	signed short i;

	for (i = 0; i < 23; i++) {
		if (item == host_readws(hero + i * 14 + 0x196)) {
			return i;
		}
	}

	return -1;
}

/**
 * \brief	gets the position of the first hero with an item
 *
 * \param item	item ID to look for
 *
 * \return position of the hero or -1 if nobody of the group has this item
*/
/* Borlandified and identical */
signed short get_first_hero_with_item(signed short item)
{
	signed short j;
	signed short i;
	Bit8u *hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {

		/* Check class and group */
		if (host_readbs(hero_i + 0x21) &&
			(host_readbs(hero_i + 0x87) == ds_readbs(CURRENT_GROUP)))
		{
			/* Search inventar */
			for (j = 0; j < 23; j++) {
				if (host_readw(hero_i + j * 14 + 0x196) == item) {
					return i;
				}
			}
		}
	}

	return -1;
}

/**
 * \brief	gets the position of the first hero with an item in a specified group
 *
 * \param item	item ID to look for
 * \param group	group number
 *
 * \return position of the hero or -1 if nobody in the specified group has this item
*/
/* Borlandified and identical */
signed short get_first_hero_with_item_in_group(signed short item, signed short group)
{
	signed short j;
	signed short i;
	Bit8u *hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {

		/* Check class and group */
		if (host_readbs(hero_i + 0x21) &&
			(host_readbs(hero_i + 0x87) == (signed char)group))
		{
			/* Search inventar */
			for (j = 0; j < 23; j++) {
				if (host_readws(hero_i + j * 14 + 0x196) == item) {
					return i;
				}
			}
		}
	}

	return -1;
}


/**
 * \brief	subtracts LE from every group member
 *
 * \param le	LE to subtract
 */
/* Borlandified and identical */
void sub_group_le(signed short le)
{
	signed short i;
	Bit8u *hero_i;

	for (i = 0; i <= 6; i++) {

		hero_i = get_hero(i);

		if (host_readbs(hero_i + 0x21) &&
			(host_readbs(hero_i + 0x87) == ds_readbs(CURRENT_GROUP)))
		{
			sub_hero_le(hero_i, le);
		}
	}
}

/**
 * \brief return a pointer to the first available hero
 *
 * \return a pointer to the first available hero. If none in available it returns a pointer to the first hero.
 */
/* Borlandified and identical */
RealPt get_first_hero_available_in_group(void)
{
	signed short i;
	RealPt hero_i = (RealPt)ds_readd(HEROS);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {

		/* Check class, group, deadness and check_hero() */
		if (host_readbs(Real2Host(hero_i) + 0x21) &&
			(host_readbs(Real2Host(hero_i) + 0x87) == ds_readbs(CURRENT_GROUP)) &&
			!hero_dead(Real2Host(hero_i)) &&
			check_hero(Real2Host(hero_i)))
		{
			return hero_i;
		}
	}

	return (RealPt) ds_readd(HEROS);
}

/**
 * \brief return a pointer to the second available hero in the group
 *
 * \return a pointer to the second available hero in the group or NULL.
 */
/* Borlandified and identical */
RealPt get_second_hero_available_in_group(void)
{
	signed short i;
	signed short tmp;
	RealPt hero_i;

	hero_i = (RealPt)ds_readd(HEROS);

	for (i = tmp = 0; i <= 6; i++, hero_i += 0x6da) {
		/* Check class, group and check_hero() */
		if (host_readbs(Real2Host(hero_i) + 0x21) &&
			(host_readbs(Real2Host(hero_i) + 0x87) == ds_readbs(CURRENT_GROUP)) &&
			check_hero(Real2Host(hero_i)))
		{
			if (tmp) {
				return hero_i;
			}

			tmp++;
		}
	}

	return (RealPt)0;
}

/**
 * \brief	count available heros
 *
 * \ return	number of available heros in all groups
 */
/* Borlandified and identical */
signed short count_heros_available(void)
{
	signed short i;
	signed short retval;
	Bit8u *hero_i;

	retval = 0;
	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* Check class */
		/* Check if hero is available */
		if (host_readbs(hero_i + 0x21) &&
			(check_hero(hero_i) || check_hero_no2(hero_i)))
		{
			retval++;
		}
	}

	return retval;
}

/**
 * \brief	TODO
*/
/* Borlandified and identical */
signed short count_heroes_available_in_group(void)
{
	signed short heroes = 0;
	signed short i;
	Bit8u *hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* Check class, group and check_hero_no2() */
		if (host_readbs(hero_i + 0x21) &&
			(host_readbs(hero_i + 0x87) == ds_readbs(CURRENT_GROUP)) &&
			check_hero_no2(hero_i))
		{
			heroes++;
		}
	}

	return heroes;
}

RealPt schick_alloc_emu(Bit32u size)
{
	return (RealPt)bc_farcalloc(size, 1);
}

#if !defined(__BORLANDC__)
}
#endif
