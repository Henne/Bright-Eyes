/*
 *	Rewrite of DSA1 v3.02_de functions of seg028 (file loader: maps, npcs, special textures, dialogs, fight backgrounds)
 *	Functions rewritten: 19/19 (complete)
 *
 *	Functions called rewritten: 16/16
 *	Functions uncalled rewritten: 3/3
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg028.cpp
*/

#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg009.h"
#include "seg010.h"
#include "seg026.h"
#include "seg028.h"
#include "seg066.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void prepare_dungeon_area(void)
{
	signed short index;
	Bit32u v1;
	Bit32u v2;
	HugePt buf;

	signed short l_si;
	signed short handle;

	index = ds_readbs(DUNGEON_INDEX) + 0x10f;

	if (ds_readbs(0x2ca6) != ds_readbs(DUNGEON_INDEX)) {

		load_area_description(1);
		ds_writeb(0x2ca7, -1);
		load_dungeon_ddt();
	}

	load_buffer_1(index);

	if ((ds_readws(0x2ccb) == -1) || (ds_readws(0x2ccb) == 1)) {

		set_var_to_zero();
		ds_writew(CURRENT_ANI, -1);

		l_si = (ds_readbs(DUNGEON_INDEX) == 1) ? 0xb5 :
			(((ds_readbs(DUNGEON_INDEX) == 2) ||
				(ds_readbs(DUNGEON_INDEX) == 7) ||
				(ds_readbs(DUNGEON_INDEX) == 9) ||
				(ds_readbs(DUNGEON_INDEX) == 11) ||
				(ds_readbs(DUNGEON_INDEX) == 12) ||
				(ds_readbs(DUNGEON_INDEX) == 13) ||
				(ds_readbs(DUNGEON_INDEX) == 14) ||
				(ds_readbs(DUNGEON_INDEX) == 15)) ? 0xb7 : 0xb6);

		ds_writeb(0x3616, (l_si == 0xb5) ? 0 : ((l_si == 0xb7) ? 1 : 2));

		handle = load_archive_file(l_si);
		v1 = v2 = 0;

		/* clear palette */
		buf = (HugePt)(ds_readd(0xd019));
		memset(Real2Host(buf), 0, 0xc0);
		wait_for_vsync();
		set_palette(Real2Host(buf), 0x80, 0x40);

		do {
			v1 = read_archive_file(handle, Real2Host(buf), 65000);
#if !defined(__BORLANDC__)
			buf = F_PADD(buf, v1);
#else
			buf += v1;
#endif
			v2 += v1;
		} while (v1);

		bc_close(handle);

		ds_writed(0xe404, (Bit32u)F_PADD(F_PADD((HugePt)ds_readd(0xd019), v2), -0xc0));

		ds_writew(0x2ccb, !ds_readbs(DUNGEON_INDEX));
	}

	ds_writeb(0x2ca6, ds_readbs(DUNGEON_INDEX));
	ds_writeb(0x2ca7, -1);
	set_automap_tiles(ds_readws(X_TARGET), ds_readws(Y_TARGET));
}

void load_dungeon_ddt(void)
{
	signed short index;
	signed short low;
	signed short high;
	signed short handle;

	index = ds_readbs(DUNGEON_INDEX) + 0x100;
	handle = load_archive_file(index);
	read_archive_file(handle, (Bit8u*)&low, 2);
	read_archive_file(handle, (Bit8u*)&high, 2);

#if !defined(__BORLANDC__)
	/* BE-Fix: */
	low = host_readws((Bit8u*)&low);
	high = host_readws((Bit8u*)&high);
#endif

	read_archive_file(handle, Real2Host(ds_readd(0xe494)), low);
	read_archive_file(handle, Real2Host(ds_readd(0xe49c)), high - low);
	read_archive_file(handle, Real2Host(ds_readd(0xe498)), 0x7d0);

	bc_close(handle);

}

void seg028_0224(void)
{
	signed short l_si;
	signed short l1;
	RealPt arr[4];

	l1 = ds_readbs(CURRENT_TOWN) + 77;

	if (ds_readbs(0x2ca7) != ds_readbs(CURRENT_TOWN)) {
		load_area_description(1);
		ds_writeb(0x2ca6, -1);
	}

	load_buffer_1(l1);

	if ((ds_readws(0x2ccb) == -1) || (ds_readws(0x2ccb) == 0)) {

		set_var_to_zero();

		ds_writew(CURRENT_ANI, -1);

		seg066_172b();

		ds_writed(0xe3fc, ds_readd(0xd019));

		for (l_si = 0; l_si < 4; l_si++) {

			if (ds_readb(0xe400 + l_si) != 0) {

				arr[l_si] = seg028_0444(!l_si ? 186 : (l_si == 1 ? 187 : (l_si == 2 ? 188 : 189)), 0, 0, 0);


			}
		}

		for (l_si = 0; l_si < 4; l_si++) {
			if (!ds_readb(0xe400 + l_si)) {

				arr[l_si] = (!l_si ? arr[l_si + 1] : arr[l_si - 1]);
			}
		}

		ds_writed(0xe41c, (Bit32u)arr[0]);
		ds_writed(0xe420, (Bit32u)arr[1]);
		ds_writed(0xe424, (Bit32u)arr[2]);
		ds_writed(0xe428, (Bit32u)arr[3]);

		if ((ds_readds(DAY_TIMER) >= HOURS(7)) && (ds_readds(DAY_TIMER) <= HOURS(20)))
		{
			ds_writed(TEX_SKY, (Bit32u) seg028_0444(185, 0x80, 0x40, 0));

			memcpy(p_datseg + 0x3eb3, Real2Host(ds_readd(0xe404)), 0xc0);
		} else {
			ds_writed(TEX_SKY, (Bit32u) seg028_0444(185, 0x80, 0x40, 0));
		}

		ds_writed(TEX_FLOOR, (Bit32u) seg028_0444(!ds_readbs(LARGE_BUF)? 184: 287, 0, 0x20, 0));

		if ((ds_readds(DAY_TIMER) >= HOURS(7)) && (ds_readds(DAY_TIMER) <= HOURS(20)))
		{
			memcpy(p_datseg + 0x3e53, Real2Host(ds_readd(0xe404)), 0x60);
		}

		ds_writew(0x2ccb, 1);
	}

	ds_writeb(0x2ca7, ds_readbs(CURRENT_TOWN));
	ds_writeb(0x2ca6, -1);

	set_automap_tiles(ds_readw(X_TARGET), ds_readw(Y_TARGET));
}

RealPt seg028_0444(signed short index, signed short firstcol, signed short colors, signed short ref)
{
	signed short fd;
	Bit32s v1;
	Bit32s v2;
	RealPt ptr;

	ptr = (RealPt)ds_readd(0xe3fc);

	fd = load_archive_file(index);

	v1 = v2 = 0L;

	do {
		v1 = read_archive_file(fd, Real2Host(ds_readd(0xe3fc)), 65000);

#if !defined(__BORLANDC__)
		F_PADA(RealMake(datseg, 0xe3fc), v1);
#else
		*(HugePt*)(p_datseg + 0xe3fc) += v1;
#endif

		v2 += v1;

	} while (v1);

	bc_close(fd);

	if (colors) {

		ds_writed(0xe404, (Bit32u)(ptr + v2 - 3 * colors));

		if ((ref != 0) && (!ds_readb(0x4475))) {

			wait_for_vsync();

			set_palette(Real2Host(ds_readd(0xe404)), firstcol, colors);
		}
	}

	return ptr;

}

void load_special_textures(signed short arg)
{
	signed short fd;

	/* load 0xe8 LTURM.NVF else FINGER.NVF */
	fd = load_archive_file(arg == 9 ? 0xe8 : 0xe9);
	read_archive_file(fd, Real2Host(ds_readd(0xd2b5)), 64000);
	bc_close(fd);

}

void call_load_buffer(void)
{
	load_buffer_1(ds_readws(0x26bf));
}

void seg028_0555(signed short town)
{
	if (town == 1) {
		seg028_0224();
	} else {
		prepare_dungeon_area();
	}
}

/**
 *	load_area_description() - writes the old area and reads a new one
 *	@type:		either 0,1,2
 *
 *	0 = only write old area
 *	1 = do both
 *	2 = only read new area (loading a savegame)
*/
void load_area_description(signed short type)
{
	signed short f_index;
	signed short fd;

	if (ds_readw(0x5ebc) != 0) {
		if (type != 2) {
			fd = load_archive_file(ds_readw(0x5ebc) + 0x8000);

			if ((ds_readw(0x5ebe) == 0) && (ds_readb(0xbd94) == 0x20)) {
				bc__write(fd, RealMake(datseg, 0xbd95), 0x200);
			} else {
				bc_lseek(fd, ds_readws(0x5eba) * 0x140, 0);
				bc__write(fd, RealMake(datseg, 0xbd95), 0x100);
			}
			/* write automap tiles */
			bc__write(fd, RealMake(datseg, 0xe442), 64);
			/* write something unknown */
			bc__write(fd, RealMake(datseg, 0xc025),
				ds_readw(0x5eb8));

			bc_close(fd);

			ds_writew(0x5ebc, ds_writew(0x5eba, ds_writew(0x5eb8, ds_writew(0x5ebe, 0))));
		}
	}

	if (type != 0) {

		/* calc archive file index */
		if (ds_readbs(DUNGEON_INDEX) != 0) {
			/* dungeon */
			ds_writew(0x5ebc, f_index = ds_readbs(DUNGEON_INDEX) + 0xf1);
		} else {
			/* city */
			ds_writew(0x5ebc, f_index = ds_readbs(CURRENT_TOWN) + 0x19);
		}

		/* save dungeon level */
		ds_writew(0x5eba, ds_readbs(DUNGEON_LEVEL));

		/* save if we are in a dungeon */
		ds_writew(0x5ebe, ds_readbs(DUNGEON_INDEX) != 0 ? 1 : 0);

		/* load DAT or DNG file */
		fd = load_archive_file(f_index + 0x8000);

		if (!ds_readbs(DUNGEON_INDEX) &&
			(ds_readb(CURRENT_TOWN) == 1
				|| ds_readb(CURRENT_TOWN) == 39
				|| ds_readb(CURRENT_TOWN) == 18))
		{
			/* path taken in THORWAL PREM and PHEXCAER */
			bc__read(fd, p_datseg + 0xbd95, 0x200);
			/* read automap tiles */
			bc__read(fd, p_datseg + 0xe442, 0x40);

			/* TODO: is that neccessary ? */
			memset(p_datseg + 0xc025, -1, 900);

			ds_writew(0x5eb8,
				bc__read(fd, p_datseg + 0xc025, 1000));

			ds_writeb(0xbd94, 0x20);
		} else {
			/* Seek to Dungeon Level * 320 */
			bc_lseek(fd, ds_readbs(DUNGEON_LEVEL) * 320, 0);
			bc__read(fd, p_datseg + 0xbd95, 0x100);

			/* read automap tiles */
			bc__read(fd, p_datseg + 0xe442, 0x40);
			ds_writew(0x5eb8, 0);

			if (!ds_readbs(DUNGEON_INDEX)) {
				/* TODO: is that neccessary ? */
				memset(p_datseg + 0xc025, -1, 900);
				ds_writew(0x5eb8,
					bc__read(fd, p_datseg + 0xc025, 1000));
			}

			ds_writeb(0xbd94, 0x10);
		}
		bc_close(fd);
	}
}

void call_load_area(signed short type)
{
	load_area_description(type);
}

void unused_store(signed short nr)
{
	signed short width;
	signed short height;
	Bit8u *ptr;
	struct nvf_desc nvf;
	signed short size;

	nvf.dst = Real2Host(ds_readd(0xd303)) + 30000;
	nvf.src = Real2Host(ds_readd(0xd019));
	nvf.nr = nr;
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	EMS_map_memory(ds_readws(0xbd92), ds_readws(0x5ec0), 0);
	EMS_map_memory(ds_readws(0xbd92), ds_readws(0x5ec0) + 1, 1);
	EMS_map_memory(ds_readws(0xbd92), ds_readws(0x5ec0) + 2, 2);
	EMS_map_memory(ds_readws(0xbd92), 0, 3);

	size = width * height;
	memmove(Real2Host((RealPt)ds_readd(0x4baa) + ds_readws(0x5ec2)),
			Real2Host((RealPt)ds_readd(0xd303) + 0x7530),
			size);

	ptr = nr * 5 + Real2Host(ds_readd(0xbd8c));

	host_writeb(ptr, (signed char)ds_readws(0x5ec0));
	host_writeb(ptr + 1, ds_readws(0x5ec2) >> 8);
	host_writew(ptr + 2, width);
	host_writeb(ptr + 4, (signed char)height);

	ds_writew(0x5ec0, (ds_readw(0x5ec0) + ((ds_readw(0x5ec2) + size) >> 14)));
	ds_writew(0x5ec2, ((((ds_readw(0x5ec2) + size) & 0x3fff) + 0x100) & 0xff00));
}

RealPt unused_load(signed short nr)
{
	signed short l_si;

	EMS_map_memory(ds_readws(0xbd92), 0, 3);

	l_si = host_readb(Real2Host(ds_readd(0xbd8c)) + 5 * nr);

	EMS_map_memory(ds_readws(0xbd92), l_si, 0);
	EMS_map_memory(ds_readws(0xbd92), l_si + 1, 1);
	EMS_map_memory(ds_readws(0xbd92), l_si + 2, 2);

	return (RealPt)ds_readd(0x4baa) + 256 * host_readb(Real2Host(ds_readd(0xbd8c)) + 5 * nr + 1);
}

void load_map(void)
{
	signed short fd;
	signed short bak;
	struct nvf_desc nvf;

	bak = ds_readw(0xe113);
	ds_writew(0xe113, 0);

	ds_writew(0x2ccb, 0xffff);
	/* set current_ani to -1 */
	ds_writew(CURRENT_ANI, 0xffff);

	/* open OBJECTS.NVF */
	fd = load_archive_file(7);
	read_archive_file(fd, Real2Host(ds_readd(0xd303)), 2000);
	bc_close(fd);

	nvf.src = Real2Host(ds_readd(0xd303));
	nvf.type = 0;
	nvf.width = (Bit8u*)&fd;
	nvf.height = (Bit8u*)&fd;
	nvf.dst = Real2Host(F_PADD((RealPt)ds_readd(0xc3db), 18000));
	nvf.nr = 16;

	process_nvf(&nvf);

	array_add(Real2Host(F_PADD((RealPt)ds_readd(0xc3db), 18000)), 3003, 0xe0, 2);

	ds_writeb(0x2845, 5);

	/* if the ems_map_hanlder exists */
	if (ds_readw(0xbd90) != 0) {
		/* get data from EMS */
		EMS_map_memory(ds_readw(0xbd90), 0, 0);
		EMS_map_memory(ds_readw(0xbd90), 1, 1);
		EMS_map_memory(ds_readw(0xbd90), 2, 2);
		EMS_map_memory(ds_readw(0xbd90), 3, 3);
		/* set map pointer to EMS */
		ds_writed(0x432e, ds_readd(0x4baa));
	} else {
		/* or read KARTE.DAT from file */
		fd = load_archive_file(5);

		read_archive_file(fd, Real2Host(ds_writed(0x432e, ds_readd(0xd303))), 64098);
		bc_close(fd);

		if (ds_readb(EMS_ENABLED) != 0) {

			if ((ds_writew(0xbd90, alloc_EMS(64100)))) {
				/* map the map into EMS */
				EMS_map_memory(ds_readw(0xbd90), 0, 0);
				EMS_map_memory(ds_readw(0xbd90), 1, 1);
				EMS_map_memory(ds_readw(0xbd90), 2, 2);
				EMS_map_memory(ds_readw(0xbd90), 3, 3);
				bc_memmove((RealPt)ds_readd(0x4baa),
					(RealPt)ds_readd(0xd303), 64098);
			}
		}
	}

	/* load LROUT.DAT */
	fd = load_archive_file(8);
	read_archive_file(fd, Real2Host(ds_readd(0xc3db)), 7600);
	bc_close(fd);

	/* load HSROUT.DAT */
	fd = load_archive_file(0xa);
	read_archive_file(fd, Real2Host(F_PADD(ds_readd(0xc3db), 7600)), 3800);
	bc_close(fd);

	/* load SROUT.DAT */
	fd = load_archive_file(9);
	read_archive_file(fd, Real2Host(F_PADD(ds_readd(0xc3db), 11400)), 5900);
	bc_close(fd);

	load_buffer_1(0x13);

	ds_writew(0xe113, bak);
}

void load_npc(signed short index)
{
	Bit8u *npc_dst;
	signed short fd;

	npc_dst = get_hero(6);

	/* load from temp directory */
	fd = load_archive_file(index | 0x8000);
	bc__read(fd, npc_dst, SIZEOF_HERO);
	bc_close(fd);

	if (host_readb(npc_dst + 0x22) == 1) {
		/* female */
		/* set an unknown variable to typus + 11 */
		host_writeb(npc_dst + 0x9b, host_readb(npc_dst + 0x21) + 11);
		if (host_readbs(npc_dst + 0x9b) > 21)
			host_writeb(npc_dst + 0x9b, 21);
	} else {
		/* male */
		host_writeb(npc_dst + 0x9b, host_readb(npc_dst + 0x21));
		if (host_readbs(npc_dst + 0x9b) > 10)
			host_writeb(npc_dst + 0x9b, 10);
	}
}

void save_npc(signed short index)
{
	signed short fd;

	fd = load_archive_file(index | 0x8000);

	bc__write(fd, (RealPt)ds_readd(HEROS) + 6 * SIZEOF_HERO, SIZEOF_HERO);

	bc_close(fd);
}

void load_splashes(void)
{
	signed short fd;
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	/* read SPLASHES.DAT */
	fd = load_archive_file(0xec);
	read_archive_file(fd, Real2Host(ds_readd(0xd303)), 3000);
	bc_close(fd);

	/* nvf.dst = splash_le = ds_readd() */
	nvf.dst = Real2Host(ds_writed(SPLASH_LE, ds_readd(0xd2bd)));
	nvf.src = Real2Host(ds_readd(0xd303));
	nvf.nr = 0;
	nvf.type = 1;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	fd = (signed short)process_nvf(&nvf);

	/* nvf.dst = splash_ae = ds_readd() */
	nvf.dst = Real2Host(ds_writed(SPLASH_AE, (Bit32u)((RealPt)ds_readd(0xd2bd) + fd)));
	nvf.src = Real2Host(ds_readd(0xd303));
	nvf.nr = 1;
	nvf.type = 1;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);
}

void load_informer_tlk(signed short index)
{
	signed short i;
	signed short fd;

	Bit32s text_len;
	Bit32s off;
	signed short partners;
	Bit8u *ptr;


	ds_writew(0x26bd, index);

	fd = load_archive_file(index);

	/* read the header */
	read_archive_file(fd, (Bit8u*)&off, 4);
	read_archive_file(fd, (Bit8u*)&partners, 2);

#if !defined(__BORLANDC__)
	/* BE-Fix */
	off = host_readd((Bit8u*)&off);
	partners = host_readw((Bit8u*)&partners);
#endif

	/* read the partner structures */
	read_archive_file(fd, ptr = (p_datseg + 0x3618), partners * 0x26);

	/* read the dialog layouts */
	read_archive_file(fd, p_datseg + 0x3794, (Bit16u)(off - partners * 0x26));

	/* read the text */
	text_len = (signed short)read_archive_file(fd, Real2Host(ds_readd(0xc3a9)), 10000);

	bc_close(fd);

	split_textbuffer(Real2Host(ds_readd(CITY_LTX)),
		(RealPt)ds_readd(0xc3a9), text_len);

	/* adjust the pointers to the layouts */
	for (i = 0; i < partners; i++, ptr += 0x26) {
		host_writed(ptr, (Bit32u)RealMake(datseg, host_readw(ptr) + 0x3794));
	}
}

void load_tlk(signed short index)
{
	signed short i;
	signed short fd;
	Bit32s text_len;
	Bit32s off;
	signed short partners;
	Bit8u *ptr;

	ds_writew(0x26bd, index);

	fd = load_archive_file(index);

	/* read the header */
	read_archive_file(fd, (Bit8u*)&off, 4);
	read_archive_file(fd, (Bit8u*)&partners, 2);

#if !defined(__BORLANDC__)
	/* BE-Fix */
	off = host_readd((Bit8u*)&off);
	partners = host_readw((Bit8u*)&partners);
#endif

	/* read the partner structures */
	read_archive_file(fd, ptr = p_datseg + 0x3618, partners * 0x26);

	/* read the dialog layouts */
	read_archive_file(fd,
		Real2Host(RealMake(datseg, 0x3794)), off - partners * 0x26);

	/* read the text */
	text_len = (signed short)read_archive_file(fd, Real2Host(ds_readd(0xd2b5)), 64000);

	bc_close(fd);

	split_textbuffer(Real2Host(ds_readd(DIALOG_TEXT)),
		(RealPt)ds_readd(0xd2b5), text_len);

	/* adjust the pointers to the layouts */
	for (i = 0; i < partners; i++, ptr += 0x26) {
		host_writed(ptr, (Bit32u)RealMake(datseg, host_readw(ptr) + 0x3794));
	}
}

void unused_load_archive_file(signed short index, signed short a2, Bit32u seg)
{
	signed short fd;

	fd = load_archive_file(index);
	read_archive_file(fd, Real2Host(RealMake(seg, a2)), 64000);
	bc_close(fd);
}


void load_fightbg(signed short index)
{
	signed short fd;

	fd = load_archive_file(index);
	read_archive_file(fd, Real2Host(ds_readd(0xd303)), 30000);
	decomp_pp20(Real2Host(ds_readd(0xd303)),
			Real2Host(ds_readd(0xc3a9)),
#if !defined(__BORLANDC__)
			Real2Host(ds_readd(0xd303)) + 4,
#else
			FP_OFF(ds_readd(0xd303)) + 4,
			FP_SEG(ds_readd(0xd303)),
#endif
			get_readlength2(fd));
	bc_close(fd);
}

#if !defined(__BORLANDC__)
}
#endif
