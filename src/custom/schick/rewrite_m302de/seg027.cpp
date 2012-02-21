/*
	Rewrite of DSA1 v3.02_de functions of seg027 (file loader)
	Functions rewritten: 7/8
*/

#include "dos_inc.h"

#include "schick.h"

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg009.h"

namespace M302de {

void load_pp20(Bit16u index)
{
	RealPt buffer_ptr;
	Bit16u fd;
	Bit16u bi;

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
				Real2Host(ds_readd(0x5e6a + bi * 4)) + 4,
				ds_readd(0x5e8e + bi * 4));
		} else {
			fd = load_archive_file(index);
			buffer_ptr = schick_alloc_emu(get_readlength2(fd));

			if (buffer_ptr) {
				/* successful allocation */

				/* save pointer */
				ds_writed(0x5e6a + bi * 4, buffer_ptr);
				/* save length */
				ds_writed(0x5e8e + bi * 4, get_readlength2(fd));

				/* read pic */
				read_archive_file(fd,
					Real2Host(ds_readd(0x5e6a + bi * 4)),
					ds_readw(0x5e8e + bi * 4));

				/* decompress */
				decomp_pp20(Real2Host(ds_readd(0x5e6a + bi * 4)),
					Real2Host(ds_readd(0xd303)),
					Real2Host(ds_readd(0x5e6a + bi * 4)) + 4,
					ds_readd(0x5e8e + bi * 4));
				bc_close(fd);
			} else {
				/* failed allocation */

				/* read it directly */
				read_archive_file(fd,
					Real2Host(ds_readd(0xd303)) - 8,
					64000);

				/* decompress it */
				decomp_pp20(Real2Host(ds_readd(0xd303)) - 8,
					Real2Host(ds_readd(0xd303)),
					Real2Host(ds_readd(0xd303)) - 8 + 4,
					get_readlength2(fd));
				bc_close(fd);
			}
		}
	} else {
		/* unbuffered picture */

		fd = load_archive_file(index);

		read_archive_file(fd, Real2Host(ds_readd(0xd303)) - 8, 64000);

		/* decompress it */
		decomp_pp20(Real2Host(ds_readd(0xd303)) - 8,
			Real2Host(ds_readd(0xd303)),
			Real2Host(ds_readd(0xd303)) - 8 + 4,
			get_readlength2(fd));

		bc_close(fd);
	}
}

/**
 *	load_ani() -	loads an animation
 *	@nr:	number of the animation
 */
void load_ani(const signed short nr)
{
	signed int area_size;
	int len;
	signed int len_3, len_4;
	unsigned int offset, offset_2;
	Bit8u *dst, *p6;
	int ani_off, ani_len;
	Bit8u *p5;
	signed int p4;
	signed int p3;
	Bit8u *p2;
	Bit8u *p1;
	RealPt ani_buffer;
	unsigned short ems_handle;
	Bit8u *p_area;
	unsigned short i;
	unsigned short fd;
	signed short area_changes;
	signed short area_pics;
	unsigned short i_area;
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
		from_EMS(ds_readd(0xc3db), ems_handle,
			host_readd(Real2Host(ds_readd(0xe121)) + i * 8 + 4));
	} else {
		/* load it from file */
		ani_off = ds_readd(0xd205 - 4 + nr * 4);
		ani_len = ds_readd(0xd205 + nr * 4) - ani_off;

		/* load ANIS */
		fd = load_archive_file(0x16);
		/* seek to ordered ani */
		seg002_0c72(fd, ani_off);
		read_archive_file(fd, Real2Host(ds_readd(0xc3db)),
			(unsigned short)ani_len);

		/* if EMS is enabled buffer it */
		if (ds_readb(0x26ab) != 0) {
			ems_handle = alloc_EMS(ani_len);

			/* find an empty EMS slot */
			if (ems_handle != 0) {
				for (i = 0; i < 36; i++)
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
			to_EMS(ems_handle, ds_readd(0xc3db), ani_len);
		}

		bc_close(fd);
	}

	ani_buffer = ds_readd(0xc3db);

	/* set start of picture data */
	ds_writed(ANI_MAIN_PTR,
		host_readd(Real2Host(ds_readd(0xc3db))) + ani_buffer);
	/* set start of palette */
	ds_writed(0xce3b,
		host_readd(Real2Host(ds_readd(0xc3db)) + 4) + ani_buffer + 6);

	/* read some bytes between data and palette */
	ds_writew(0xc3e9,
		host_readw(Real2Host(ds_readd(0xce3b)) - 6));
	ds_writew(0xc3eb,
		host_readw(Real2Host(ds_readd(0xce3b)) - 4));
	/* compression type */
	ds_writeb(0xce39,
		host_readb(Real2Host(ds_readd(0xce3b)) - 1));
	ds_writeb(0xce3a,
		host_readb(Real2Host(ds_readd(0xce3b)) - 2));

	p6 = Real2Host(ds_readd(0xce3b) + ds_readb(0xce3a) * 3);
	/* set picture size */
	ds_writew(0xc3e7, host_readw(Real2Host(ds_readd(0xc3db) + 8)));
	ds_writeb(0xc3ed, host_readb(Real2Host(ds_readd(0xc3db) + 10)));
	/* set number of areas */
	ds_writeb(0xc3ee, host_readb(Real2Host(ds_readd(0xc3db) + 11)));

	/* Process Main Picture */
	if (ds_readb(0xce39) != 0) {
		len_4 = host_readd(Real2Host(ds_readd(ANI_MAIN_PTR)));
		p1 = Real2Host(ds_readd(ANI_MAIN_PTR));

		p1 += (len_4 - 4);

		len_3 = host_readd(p1);
		len_3 = swap_u32(len_3) >> 8;

		decomp_pp20(Real2Host(ds_readd(ANI_MAIN_PTR)),
			Real2Host(ds_readd(0xd303)),
			Real2Host(ds_readd(ANI_MAIN_PTR)) + 4,
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
		ds_writed(0xce3b, ds_readd(0xce3b) + offset);
		p6 += offset;
	}

	/* Process the Areas */
	for (i_area = 0; (signed char)ds_readb(0xc3ee) > i_area; i_area++) {
		p5 = Real2Host(RealMake(datseg, 0xc3ef + i_area * 0x107));
		p3 = host_readd(Real2Host(ds_readd(0xc3db) + i_area * 4 + 0xc));
		p_area = Real2Host(ds_readd(0xc3db) + p3);
		strncpy((char*)p5, (char*)p_area, 4);

		host_writew(p5 + 5, host_readw(p_area + 4));
		host_writeb(p5 + 7, host_readb(p_area + 6));
		host_writeb(p5 + 8, host_readb(p_area + 7));
		host_writew(p5 + 9, host_readw(p_area + 8));
		host_writeb(p5 + 0x0b, host_readb(p_area + 0x0a));

		area_pics = (signed char)host_readb(p_area + 0x0b);
		host_writeb(p5 + 0x0c, (unsigned char)area_pics);
		if (ds_readb(0xce39) != 0) {

			p4 = host_readd(p_area + 0xc);
			p4 += offset;
			p1 = Real2Host(ds_readd(0xc3db) + p4);

			len_4 = host_readd(p1);
			p1 += (len_4 - 4);
			area_size = host_readd(p1);
			area_size = swap_u32(area_size) >> 8;

			decomp_pp20(Real2Host(ds_readd(0xc3db)) + p4,
				Real2Host(ds_readd(0xd303)),
				Real2Host(ds_readd(0xc3db)) + p4 + 4,
				len_4);

			offset_2 = area_size - len_4;
			offset += offset_2;

			dst = Real2Host(ds_readd(0xc3db));
			dst += p4;
			dst += len_4;
			len = p6 - dst;
			memcpy(p6 + offset_2, dst, (unsigned short)len);

			memcpy(Real2Host(ds_readd(0xc3db)) + p4,
				Real2Host(ds_readd(0xd303)), (unsigned short)area_size);
			dst += offset_2;
			memcpy(dst, p6 + offset_2, (unsigned short)len);
			ds_writed(0xce3b, ds_readd(0xce3b) + offset_2);
			p6 += offset_2;
			area_size = (unsigned char)host_readb(p5 + 8)
				* (signed short)host_readw(p5 + 9);

			for (di = 0; di < area_pics; di++) {
				host_writed(p5 + di * 4 + 0xd,
					ds_readd(0xc3db) + p4 + di * area_size);
			}
		} else {
			for (di = 0; di < area_pics; di++) {
				p4 = host_readd(p_area + di * 4 + 0xc);
				host_writed(p5 + di * 4 + 0x0d,
					ds_readd(0xc3db) + p4);
			}
		}

		area_changes = host_readw(p_area + area_pics * 4 + 0x0c);
		host_writew(p5 + 0x5d, area_changes);

		p2 = p_area + area_pics * 4 + 0x0e;
		for (di = 0; di < area_changes; di++) {
			host_writew(p5 + 0x5f + di * 4,
				host_readw(p2 + di * 4));
			host_writew(p5 + 0x61 + di * 4,
				host_readw(p2 + di * 4 + 2));
		}
	}

	ani_len = p6 - Real2Host(ds_readd(0xc3db));
	/* this is always true */
	if (ani_len > (signed int)ds_readd(0xce43)) {
		ds_writew(0x2ccb, 0xffff);
	}
}

void load_scenario(signed short nr)
{
	unsigned short fd;
	signed short n;
	char buf[2];

	n = nr;

	/* load SCENARIO.LST */
	fd = load_archive_file(0xc8);

	/* read the first two bytes == nr of scenarios */
	read_archive_file(fd, (Bit8u*)buf, 2);

	/* check if scenario nr is valid */
	if ((host_readw((Bit8u*)buf) < n) || (n < 1))
		nr = 1;

	/* seek to the scenario */
	seg002_0c72(fd, (n - 1) * 621 + 2);

	/* read scenario */
	read_archive_file(fd, Real2Host(ds_readd(0xbd2c)), 621);

	/* close archive */
	bc_close(fd);
}

unsigned short count_fight_enemies(signed short nr)
{
	Bit8u *buf;
	unsigned short fd;
	unsigned short max;
	unsigned short retval;
	unsigned short i;

	retval = 0;

	buf = Real2Host(ds_readd(0xd2f3));

	/* load FIGHT.LST from TEMP dir */
	fd = load_archive_file(0x8000 | 0xcd);

	/* read the first 2 bytes (max number of fights) */
	bc__read(fd, (Bit8u*)&max, 2);

	/* sanity check for parameter nr */
	if ((max - 1) < nr || nr <= 0)
		nr = 0;

	/* seek to file position */
	bc_lseek(fd, nr * 216 + 2, DOS_SEEK_SET);

	/* read the fight entry */
	bc__read(fd, buf, 216);

	/* close FIGHT.LST */
	bc_close(fd);

	/* check all enemies */
	for (i = 0; i < 20; i++) {
		/* no enemy */
		if (host_readb(buf + 0x16 + i * 5) == 0)
			continue;
		/* enemy does not appear in the first round */
		if (host_readb(buf + 0x1a + i * 5) != 0)
			continue;
		/* increment counter */
		retval++;
	}

	return retval;
}

void read_fight_lst(signed short nr)
{
	char fight_name[21];
	unsigned short fd;
	unsigned short max;

	/* load FIGHT.LST from TEMP dir */
	fd = load_archive_file(0x8000 | 0xcd);

	/* read the first 2 bytes (max number of fights) */
	bc__read(fd, (Bit8u*)&max, 2);

	/* sanity check for parameter nr */
	if ((max - 1) < nr || nr <= 0)
		nr = 0;

	/* write the fight number to a global var */
	ds_writew(0x5eb2, nr);

	/* seek to file position */
	bc_lseek(fd, nr * 216 + 2, DOS_SEEK_SET);

	/* read the fight entry */
	bc__read(fd, Real2Host(ds_readd(0xbd28)), 216);

	/* Improvement */
	strncpy(fight_name, (char*)Real2Host(ds_readd(0xbd28)), 20);
	fight_name[20] = '\0';
	D1_INFO("Lade Kampf Nr %3d\t Name \"%s\"\n", nr, fight_name);
	/* Improvement end */

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
	bc_lseek(fd, nr * 216 + 2, DOS_SEEK_SET);

	/* write it */
	bc__write(fd, ds_readd(0xbd28), 216);

	/* close the file */
	bc_close(fd);
}

void init_common_buffers()
{

	unsigned short fd;
	unsigned short bytes;

	/* load POPUP.DAT */
	fd = load_archive_file(0x99);
	bytes = read_archive_file(fd, Real2Host(ds_readd(0xd2ad) - 8), 500);
	bc_close(fd);

	/* decompress POPUP.DAT */
	decomp_pp20(Real2Host(ds_readd(0xd2ad) - 8),
		Real2Host(ds_readd(0xd2ad)),
		Real2Host(ds_readd(0xd2ad) - 8 + 4),
		bytes);

	/* load COMPASS */
	fd = load_archive_file(0x12);
	bytes = read_archive_file(fd, Real2Host(ds_readd(0xd2b1)), 5000);
	bc_close(fd);

	/* load ITEMS.DAT */
	fd = load_archive_file(0xdc);
	bytes = read_archive_file(fd, Real2Host(ds_readd(0xe22b)), 3060);
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
	read_archive_file(fd, Real2Host(ds_readd(0xd321)), 288);
	bc_close(fd);

}

}
