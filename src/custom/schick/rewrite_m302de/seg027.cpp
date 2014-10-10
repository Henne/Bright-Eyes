/*
 *	Rewrite of DSA1 v3.02_de functions of seg027 (file loader)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg027.cpp
 *
*/

#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg009.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void load_pp20(signed short index)
{
	volatile signed short fd;
	RealPt buffer_ptr;
	signed short bi;

	if (index <= 5 || index == 0xd6 || index == 0x14 || index == 0xd7) {
		/* These pictures are buffered for faster access */
		bi = index;

		/* PLAYM_US */
		if (index == 0xd6)
			bi = 6;
		/* ZUSTA_UK */
		if (index == 0x14)
			bi = 7;
		/* ZUSTA_US */
		if (index == 0xd7)
			bi = 8;

		if (ds_readd(0x5e6a + bi * 4)) {

			/* already buffered, just decomp */
			decomp_pp20(Real2Host(ds_readd(0x5e6a + bi * 4)),
				Real2Host(ds_readd(0xd303)),
#if !defined(__BORLANDC__)
				Real2Host(ds_readd(0x5e6a) + 4 + bi) + 4,
#else
				ds_readw(0x5e6a + 4 * bi) + 4,
				ds_readw(0x5e6c + 4 * bi),
#endif
				ds_readd(0x5e8e + bi * 4));

		} else {
			fd = load_archive_file(index);

			if ((buffer_ptr = schick_alloc_emu(get_readlength2(fd)))) {
				/* successful allocation */

				/* save pointer */
				ds_writed(0x5e6a + bi * 4, (Bit32u)buffer_ptr);
				/* save length */
				ds_writed(0x5e8e + bi * 4, get_readlength2(fd));

				/* read pic */
				read_archive_file(fd,
					Real2Host(ds_readd(0x5e6a + bi * 4)),
					ds_readw(0x5e8e + bi * 4));

				/* decompress */
				decomp_pp20(Real2Host(ds_readd(0x5e6a + bi * 4)),
					Real2Host(ds_readd(0xd303)),
#if !defined(__BORLANDC__)
					Real2Host(ds_readd(0x5e6a + 4 + bi)) + 4,
#else
					ds_readw((0x5e6a + 0) + bi * 4) + 4,
					ds_readw((0x5e6a + 2) + bi * 4),
#endif
					ds_readd(0x5e8e + bi * 4));

				bc_close(fd);
			} else {
				/* failed allocation */

				/* read it directly */
				read_archive_file(fd,
					Real2Host(ds_readd(0xd303)) -8,
					64000);

				/* decompress it */
				decomp_pp20(Real2Host(ds_readd(0xd303)) -8,
					Real2Host(ds_readd(0xd303)),
#if !defined(__BORLANDC__)
					Real2Host(ds_readd(0xd303)) -8 +4,
#else
					FP_OFF((RealPt)ds_readd(0xd303) -8) +4,
					FP_SEG((RealPt)ds_readd(0xd303) -8),
#endif
					get_readlength2(fd));

				bc_close(fd);
			}
		}
	} else {
		/* unbuffered picture */

		fd = load_archive_file(index);

		read_archive_file(fd, Real2Host(ds_readd(0xd303)) - 8, 64000);

		/* decompress it */
		decomp_pp20(Real2Host(ds_readd(0xd303)) -8,
			Real2Host(ds_readd(0xd303)),
#if !defined(__BORLANDC__)
			Real2Host(ds_readd(0xd303) - 8 + 4),
#else
			FP_OFF((RealPt)ds_readd(0xd303) - 8) + 4,
			FP_SEG((RealPt)ds_readd(0xd303) - 8),
#endif
			get_readlength2(fd));

		bc_close(fd);
	}
}

/**
 * load_fight_figs() - load fight sprites of actors
 * @fig_old:	the fig number
 *
 * Returns a pointer to the location where the data is.
 *
 */
/* Original-Bug: when using EMS for caching something strage happens. */
RealPt load_fight_figs(signed short fig)
{
	signed short i;

	RealPt dst;
	unsigned short ems_handle;
	Bit32u offset;
	Bit32u len;
	unsigned short fd;
	signed short max_entries;
	Bit8u *mem_slots;
	Bit8u *p_tab;
	signed short index;
	RealPt src;

	/* check if fig is at a known place */
	if (fig == ds_readws(0x2cd1)) {
		return (RealPt)ds_readd(0xd2df);
	} else if (fig == ds_readws(0x2cd3)) {
			return (RealPt)ds_readd(0xd2db);
	} else if (ds_readws(0x2cd3) != -1) {
		ds_writew(0x2cd1, ds_readw(0x2cd3));
		memcpy(Real2Host(ds_readd(0xd2df)),
			Real2Host(ds_readd(0xd2db)), 20000);
		src = (RealPt)ds_readd(0xd2db);
		ds_writew(0x2cd3, fig);
	} else if (ds_readws(0x2cd1) != -1) {
		src = (RealPt)ds_readd(0xd2db);
		ds_writew(0x2cd3, fig);
	} else {
		src = (RealPt)ds_readd(0xd2df);
		ds_writew(0x2cd1, fig);
	}

	/* prepare archive access... */

	if (fig >= 88) {
		/* ...for foes */
		max_entries = 36;
		mem_slots = Real2Host(ds_readd(MEM_SLOTS_MON));
		p_tab = p_datseg + 0xd01d;
		index = 16;
		fig -= 88;
	} else {
		/* ...for heroes */
		max_entries = 43;

		if (fig >= 44) {
			/* female */
			p_tab = p_datseg + 0xd0ad;
			index = 234;
			mem_slots = Real2Host(ds_readd(MEM_SLOTS_WFIG));
			fig -= 44;
		} else {
			/* male */
			p_tab = p_datseg + 0xd159;
			index = 24;
			mem_slots = Real2Host(ds_readd(MEM_SLOTS_MFIG));
		}
	}

	/* check if fig is already in memory */
	for (i = 0; i < max_entries; i++) {
		if (host_readw(mem_slots + i * 12) == fig)
			break;
	}

	if (i != max_entries) {
		/* Yes, it is */

		if (host_readw(mem_slots + i * 12 + 6) != 0) {
			/* is in EMS */
			ems_handle = host_readw(mem_slots + i * 12 + 6);
			from_EMS(src, ems_handle, host_readd(mem_slots + i * 12 + 8));
		} else {
			/* is in HEAP */
#if !defined(__BORLANDC__)
			D1_LOG("cached from HEAP %d\n", fig);
#endif
			memcpy(Real2Host(src),
				Real2Host(host_readd(mem_slots + i * 12 + 2)),
				host_readw(mem_slots + i * 12 + 8));
		}
	} else {
#if !defined(__BORLANDC__)
		D1_LOG("not cached\n");
#endif

		/* read fig from file */
		offset = host_readd(p_tab + (fig - 1) * 4);
		len = host_readd(p_tab + fig * 4) - offset;

		fd = load_archive_file(index);

		seg002_0c72(fd, offset, 0);

		read_archive_file(fd, Real2Host(src), (unsigned short)len);

		bc_close(fd);

		if ((dst = schick_alloc_emu(len))) {
#if !defined(__BORLANDC__)
			D1_LOG("use HEAP for fig %d\n", fig);
#endif
			/* use heap */

			for (i = 0; i < max_entries - 1; i++) {
				if (host_readw(mem_slots + i * 12) == 0)
					break;
			}

			host_writew(mem_slots + i * 12, fig);
			host_writed(mem_slots + i * 12 + 2, (Bit32u)dst);
			host_writew(mem_slots + i * 12 + 6, 0);
			host_writed(mem_slots + i * 12 + 8, len);

			memcpy(Real2Host(dst), Real2Host(src),
				(unsigned short)len);

		} else if (ds_readb(EMS_ENABLED) != 0) {
#if !defined(__BORLANDC__)
			D1_LOG("use EMS for fig %d\n", fig);
#endif

			if ((ems_handle = alloc_EMS(len))) {

				/* find a free slot */
				for (i = 0; i < max_entries - 1; i++) {
					if (host_readw(mem_slots + i * 12) == 0)
						break;
				}

				/* write slot */
				host_writew(mem_slots + i * 12, fig);
				host_writew(mem_slots + i * 12 + 6, ems_handle);
				host_writed(mem_slots + i * 12 + 2, 0);
				host_writed(mem_slots + i * 12 + 8, len);

				/* copy to EMS */
				to_EMS(ems_handle, src, len);
			}
		}
	}

	return src;
}

/**
 *	load_ani() -	loads an animation
 *	@nr:	number of the animation
 */
void load_ani(const signed short nr)
{
	signed short i_area;
	signed short area_pics;
	signed short area_changes;
	unsigned short fd;
	signed short i;
	Bit8u *p_area;
	unsigned short ems_handle;
	RealPt ani_buffer;
#if !defined(__BORLANDC__)
	Bit8u *p1;
#else
	Bit8u huge *p1;
#endif
	Bit8u *p2;
	Bit32s p3;
	Bit32s p4;
	Bit8u *p5;
	Bit32s ani_off;
	Bit32s ani_len;
#if !defined(__BORLANDC__)
	Bit8u *dst;
	Bit8u *p6;
#else
	Bit8u huge *dst;
	Bit8u huge *p6;
#endif
	Bit32s offset;
	Bit32s len_3;
	Bit32s len_4;
	Bit32s offset_2;
	Bit32s len;
	Bit32s area_size;

	signed short di;


	/* sanity check */
	if (nr == -1)
		return;

	/* no need to reload  the same ani*/
	if (nr == (signed short)ds_readw(CURRENT_ANI))
		return;

	ds_writew(0x29ae, 0);
	/* set the new ani nr*/
	ds_writew(CURRENT_ANI, nr);
	/* clear the old ani */
	clear_ani();

	/* count to the ordered ani in an array*/
	for (i = 0; i < 37; i++) {
		if (nr == host_readw(Real2Host(ds_readd(0xe121)) + i * 8))
			break;
	}

	if (i != 37) {
		/* already buffered in EMS, get from there */
		ems_handle = host_readw(Real2Host(ds_readd(0xe121)) + i * 8 + 2);
		from_EMS((RealPt)ds_readd(0xc3db), ems_handle,
			host_readd(Real2Host(ds_readd(0xe121)) + i * 8 + 4));
	} else {
		/* load it from file */
		ani_off = ds_readd(0xd205 - 4 + nr * 4);
		ani_len = ds_readd(0xd205 + nr * 4) - ani_off;

		/* load ANIS */
		fd = load_archive_file(0x16);
		/* seek to ordered ani */
		seg002_0c72(fd, ani_off, 0);
		read_archive_file(fd, Real2Host(ds_readd(0xc3db)),
			(unsigned short)ani_len);

		/* if EMS is enabled buffer it */
		if ((ds_readb(EMS_ENABLED) != 0) &&
			((ems_handle = alloc_EMS(ani_len))))
		{

			/* find an empty EMS slot */
			for (i = 0; i < 36; i++) {
				if (host_readw(Real2Host(ds_readd(0xe121)) + i * 8) == 0)
					break;
			}

			/* fill the entry */
			host_writew(Real2Host(ds_readd(0xe121)) + i * 8, nr);
			host_writew(Real2Host(ds_readd(0xe121)) + i * 8 + 2,
				ems_handle);
			host_writed(Real2Host(ds_readd(0xe121)) + i * 8 + 4,
				ani_len);

			/* copy data to EMS */
			to_EMS(ems_handle, (RealPt)ds_readd(0xc3db), ani_len);
		}

		bc_close(fd);
	}

	ani_buffer = (RealPt)ds_readd(0xc3db);

	/* set start of picture data */
	ds_writed(ANI_MAIN_PTR,
		(Bit32u)(F_PADD(ani_buffer, host_readd(Real2Host(ds_readd(0xc3db))))));
	/* set start of palette */
	ds_writed(0xce3b,
		(Bit32u)(F_PADD(F_PADD(ani_buffer, host_readd(Real2Host(F_PADD(ds_readd(0xc3db), 4L)))), 6L)));
	//	(Bit32u)(host_readd(Real2Host(ds_readd(0xc3db)) + 4) + ani_buffer + 6));

	/* read some bytes between data and palette */
	ds_writew(0xc3e9,
		host_readw(Real2Host(F_PADD(ds_readd(0xce3b), -6))));
	ds_writew(0xc3eb,
		host_readw(Real2Host(F_PADD(ds_readd(0xce3b), -4))));
	/* compression type */
	ds_writeb(0xce39,
		host_readb(Real2Host(F_PADD(ds_readd(0xce3b), -1))));
	ds_writeb(0xce3a,
		host_readb(Real2Host(F_PADD(ds_readd(0xce3b), -2))));

	p6 = Real2Host(F_PADD(ds_readd(0xce3b), 3 * ds_readb(0xce3a)));

	/* set picture size */
	ds_writew(0xc3e7, host_readw(Real2Host(F_PADD(ds_readd(0xc3db), 8))));
	ds_writeb(0xc3ed, host_readb(Real2Host(F_PADD(ds_readd(0xc3db), 10))));
	/* set number of areas */
	ds_writeb(0xc3ee, host_readb(Real2Host(F_PADD(ds_readd(0xc3db), 11))));

	/* Process Main Picture */
	if (ds_readb(0xce39) != 0) {
		len_4 = host_readd(Real2Host(ds_readd(ANI_MAIN_PTR)));
		p1 = Real2Host(ds_readd(ANI_MAIN_PTR));

		p1 += (len_4 - 4);

		len_3 = host_readd(p1);
		len_3 = swap_u32(len_3) >> 8;

		decomp_pp20(Real2Host(ds_readd(ANI_MAIN_PTR)),
			Real2Host(ds_readd(0xd303)),
#if !defined(__BORLANDC__)
			Real2Host(ds_readd(ANI_MAIN_PTR) + 4),
#else
			FP_OFF((RealPt)ds_readd(ANI_MAIN_PTR)) + 4,
			FP_SEG((RealPt)ds_readd(ANI_MAIN_PTR)),
#endif
			len_4);

		offset = len_3 - len_4;
		dst = Real2Host(ds_readd(ANI_MAIN_PTR));
		dst += len_4;
		len = p6 - dst;
		memcpy(p6 + offset, dst, len);

		memcpy(Real2Host(ds_readd(ANI_MAIN_PTR)),
			Real2Host(ds_readd(0xd303)), len_3);
		dst += offset;
		memcpy(dst, p6 + offset, len);

#if !defined(__BORLANDC__)
		ds_writed(0xce3b, ds_readd(0xce3b) + offset);
#else
		*(HugePt*)(p_datseg + 0xce3b) += offset;
#endif

		p6 += offset;
	}

	/* Process the Areas */
	for (i_area = 0; ds_readbs(0xc3ee) > i_area; i_area++) {
		p5 = Real2Host(RealMake(datseg, 0xc3ef + i_area * 0x107));
		p3 = host_readd(Real2Host(F_PADD(F_PADD(ds_readd(0xc3db), 4 * i_area), 0xc)));
		p_area = Real2Host(F_PADD(ds_readd(0xc3db), p3));
		strncpy((char*)p5, (char*)p_area, 4);

		host_writew(p5 + 5, host_readw(p_area + 4));
		host_writeb(p5 + 7, host_readb(p_area + 6));
		host_writeb(p5 + 8, host_readb(p_area + 7));
		host_writew(p5 + 9, host_readw(p_area + 8));
		host_writeb(p5 + 0x0b, host_readb(p_area + 0x0a));

		host_writeb(p5 + 0x0c, (signed char)(area_pics = host_readbs(p_area + 0x0b)));

		if (ds_readb(0xce39) != 0) {

			p4 = host_readd(p_area + 0xc);
			p4 += offset;
			p1 = Real2Host(F_PADD(ds_readd(0xc3db), p4));

			len_4 = host_readd(p1);
			p1 += (len_4 - 4);
			area_size = host_readd(p1);
			area_size = swap_u32(area_size) >> 8;

			decomp_pp20(Real2Host(F_PADD(ds_readd(0xc3db), p4)),
				Real2Host(ds_readd(0xd303)),
#if !defined(__BORLANDC__)
				Real2Host(ds_readd(0xc3db)) + p4 + 4,
#else
				FP_OFF(F_PADD(ds_readd(0xc3db), p4)) + 4,
				FP_SEG(F_PADD(ds_readd(0xc3db), p4)),
#endif
				len_4);

			offset_2 = area_size - len_4;
			offset += offset_2;

			dst = Real2Host(ds_readd(0xc3db));
			dst += p4;
			dst += len_4;
			len = p6 - dst;
			memcpy(p6 + offset_2, dst, (unsigned short)len);

			memcpy(Real2Host(F_PADD(ds_readd(0xc3db), p4)),
				Real2Host(ds_readd(0xd303)), (unsigned short)area_size);
			dst += offset_2;
			memcpy(dst, p6 + offset_2, (unsigned short)len);
#if !defined(__BORLANDC__)
		ds_writed(0xce3b, ds_readd(0xce3b) + offset_2);
#else
		*(HugePt*)(p_datseg + 0xce3b) += offset_2;
#endif
			p6 += offset_2;
			area_size = (unsigned char)host_readb(p5 + 8)
				* (signed short)host_readw(p5 + 9);

			for (di = 0; di < area_pics; di++) {
				host_writed(p5 + di * 4 + 0xd,
					(Bit32u)(F_PADD(F_PADD(ds_readd(0xc3db), p4), di * area_size)));
			}
		} else {
			for (di = 0; di < area_pics; di++) {
				p4 = host_readd(p_area + di * 4 + 0xc);
				host_writed(p5 + di * 4 + 0x0d,
					(Bit32u)(F_PADD(ds_readd(0xc3db), p4)));
			}
		}

		host_writew(p5 + 0x5d, area_changes = host_readw(p_area + area_pics * 4 + 0x0c));

		p2 = p_area + area_pics * 4 + 0x0e;
		for (di = 0; di < area_changes; di++) {
			host_writew(p5 + 0x5f + di * 4,
				host_readw(p2 + ((di << 1) << 1)));
			host_writew(p5 + 0x61 + di * 4,
				host_readw(p2 + ((di << 1) << 1)  + 2));
		}
	}

	ani_len = p6 - Real2Host(ds_readd(0xc3db));
	/* this is always true */
	if (ani_len > (Bit32s)ds_readd(0xce43)) {
		ds_writew(0x2ccb, 0xffff);
	}
}

void load_scenario(signed short nr)
{
	unsigned short fd;
	signed short buf;

	/* load SCENARIO.LST */
	fd = load_archive_file(0xc8);

	/* read the first two bytes == nr of scenarios */
	read_archive_file(fd, (Bit8u*)&buf, 2);

#if !defined(__BORLANDC__)
	/* BE-fix: */
	buf = host_readw((Bit8u*)&buf);
#endif

	/* check if scenario nr is valid */
	if ((nr > buf) || (nr < 1))
		nr = 1;

	/* seek to the scenario */
	seg002_0c72(fd, 621L * (nr - 1) + 2, 0);

	/* read scenario */
	read_archive_file(fd, Real2Host(ds_readd(SCENARIO_BUF)), 621);

	/* close archive */
	bc_close(fd);
}

signed short count_fight_enemies(signed short nr)
{
	signed short i;
	signed short retval;
	Bit8u *buf;
	unsigned short fd;
	signed short max;

	retval = 0;

	buf = Real2Host(ds_readd(DTP2));

	/* load FIGHT.LST from TEMP dir */
	fd = load_archive_file(0x8000 | 0xcd);

	/* read the first 2 bytes (max number of fights) */
	bc__read(fd, (Bit8u*)&max, 2);

#if !defined(__BORLANDC__)
	/* BE-fix: */
	max = host_readw((Bit8u*)&max);
#endif
	/* sanity check for parameter nr */
	if ((nr > (max - 1)) || (nr < 0))
		nr = 0;

	/* seek to file position */
	bc_lseek(fd, 216L * nr + 2, SEEK_SET);

	/* read the fight entry */
	bc__read(fd, buf, 216);

	/* close FIGHT.LST */
	bc_close(fd);

	/* check all enemies */
	for (i = 0; i < 20; i++) {
		/* no enemy and enemy does not appear in the first round */
		if ((host_readb(buf + 0x16 + i * 5) != 0)
			&& (!host_readbs(buf + 0x1a + i * 5)))
		{
			/* increment counter */
			retval++;
		}
	}

	return retval;
}

void read_fight_lst(signed short nr)
{
	unsigned short fd;
	signed short max;

	/* load FIGHT.LST from TEMP dir */
	fd = load_archive_file(0x8000 | 0xcd);

	/* read the first 2 bytes (max number of fights) */
	bc__read(fd, (Bit8u*)&max, 2);

#if !defined(__BORLANDC__)
	/* BE-fix: */
	max = host_readw((Bit8u*)&max);
#endif
	/* sanity check for parameter nr */
	if ((max - 1) < nr || nr < 0)
		nr = 0;

	/* write the fight number to a global var */
	ds_writew(0x5eb2, nr);

	/* seek to file position */
	bc_lseek(fd, 216L * nr + 2, SEEK_SET);

	/* read the fight entry */
	bc__read(fd, Real2Host(ds_readd(PTR_FIGHT_LST)), 216);

#if !defined(__BORLANDC__)
	char fight_name[21];
	/* Improvement */
	strncpy(fight_name, (char*)Real2Host(ds_readd(PTR_FIGHT_LST)), 20);
	fight_name[20] = '\0';
	D1_INFO("Lade Kampf Nr %3d\t Name \"%s\"\n", nr, fight_name);
	/* Improvement end */
#endif

	/* close FIGHT.LST */
	bc_close(fd);
}

void write_fight_lst(void)
{
	signed short nr;
	unsigned short fd;

	nr = ds_readw(0x5eb2);

	/* load FIGHT.LST from TEMP dir */
	fd = load_archive_file(0x8000 | 0xcd);

	/* seek to the entry */
	bc_lseek(fd, 216L * nr + 2, SEEK_SET);

	/* write it */
	bc__write(fd, (RealPt)ds_readd(PTR_FIGHT_LST), 216);

	/* close the file */
	bc_close(fd);
}

void init_common_buffers(void)
{
	unsigned short fd;
	signed short bytes;

	/* load POPUP.DAT */
	fd = load_archive_file(0x99);
	bytes = read_archive_file(fd, Real2Host((RealPt)ds_readd(POPUP) - 8), 500);
	bc_close(fd);

	/* decompress POPUP.DAT */
	decomp_pp20(Real2Host((RealPt)ds_readd(POPUP) - 8),
		Real2Host(ds_readd(POPUP)),
#if !defined(__BORLANDC__)
		Real2Host(ds_readd(POPUP)) - 8 + 4,
#else
		FP_OFF((RealPt)ds_readd(POPUP) - 8) + 4,
		FP_SEG((RealPt)ds_readd(POPUP) - 8),
#endif
		bytes);

	/* load COMPASS */
	fd = load_archive_file(0x12);
	bytes = read_archive_file(fd, Real2Host(ds_readd(0xd2b1)), 5000);
	bc_close(fd);

	/* load ITEMS.DAT */
	fd = load_archive_file(0xdc);
	bytes = read_archive_file(fd, Real2Host(ds_readd(ITEMSDAT)), 3060);
	bc_close(fd);

	/* load ANIS.TAB */
	fd = load_archive_file(0x17);
	read_archive_file(fd, Real2Host(RealMake(datseg, 0xd205)), 148);
	bc_close(fd);

	/* load MFIGS.TAB */
	fd = load_archive_file(0x19);
	read_archive_file(fd, Real2Host(RealMake(datseg, 0xd159)), 172);
	bc_close(fd);

	/* load WFIGS.TAB */
	fd = load_archive_file(0xeb);
	read_archive_file(fd, Real2Host(RealMake(datseg, 0xd0ad)), 172);
	bc_close(fd);

	/* load MONSTER.TAB */
	fd = load_archive_file(0x11);
	read_archive_file(fd, Real2Host(RealMake(datseg, 0xd01d)), 144);
	bc_close(fd);

	/* load GAMES.NAM */
	fd = load_regular_file(0xcf);
	bc__read(fd, Real2Host(RealMake(datseg, 0xe2da)), 45);
	bc_close(fd);

	/* load TOWNPAL.DAT */
	fd = load_archive_file(0x8d);
	read_archive_file(fd, Real2Host(ds_readd(TOWNPAL_BUF)), 288);
	bc_close(fd);

}

#if !defined(__BORLANDC__)
}
#endif
