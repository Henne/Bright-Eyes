/*
	Rewrite of DSA1 v3.02_de functions of seg028 (map / file loader)
	Functions rewritten: 8/19
*/

#include <string.h>

#include "schick.h"

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg009.h"
#include "seg026.h"

namespace M302de {

void load_special_textures(signed short arg)
{
	Bit16u fd;

	/* load 0xe8 LTURM.NVF else FINGER.NVF */
	fd = load_archive_file(arg == 9 ? 0xe8 : 0xe9);
	read_archive_file(fd, Real2Host(ds_readd(0xd2b5)), 64000);
	bc_close(fd);

}

void call_load_buffer(void)
{
	load_buffer_1((signed short)ds_readw(0x26bf));
}

/**
 *	load_area_description() - writes the old area and reads a new one
 *	@type:		either 0,1,2
 *
 *	0 = only write old area
 *	1 = do both
 *	2 = only read new area (loading a savegame)
*/
void load_area_description(unsigned short type)
{
	unsigned short f_index;
	unsigned short fd;

	if (ds_readw(0x5ebc) != 0) {
		if (type != 2) {
			fd = load_archive_file(ds_readw(0x5ebc) + 0x8000);

			if (ds_readw(0x5ebe) == 0 && ds_readb(0xbd94) == 0x20) {
				bc__write(fd, RealMake(datseg, 0xbd95), 0x200);
			} else {
				bc_lseek(fd, ds_readw(0x5eba) * 0x140, 0);
				bc__write(fd, RealMake(datseg, 0xbd95), 0x100);
			}
			/* write automap tiles */
			bc__write(fd, RealMake(datseg, 0xe442), 64);
			/* write something unknown */
			bc__write(fd, RealMake(datseg, 0xc025),
				ds_readw(0x5eb8));

			bc_close(fd);

			ds_writew(0x5ebe, 0);
			ds_writew(0x5eb8, 0);
			ds_writew(0x5eba, 0);
			ds_writew(0x5ebc, 0);
		}
	}

	if (type != 0) {

		/* calc archive file index */
		if (ds_readb(0x2d6e) != 0)
			/* dungeon */
			f_index = ds_readb(0x2d6e) + 0xf1;
		else
			/* city */
			f_index = ds_readb(0x2d67) + 0x19;

		/* save archive index */
		ds_writew(0x5ebc, f_index);
		/* save dungeon level */
		ds_writew(0x5eba, ds_readb(0x2d75));

		if (ds_readb(0x2d6e) != 0)
			ds_writew(0x5ebe, 1);
		else
			ds_writew(0x5ebe, 0);

		/* load DAT or DNG file */
		fd = load_archive_file(f_index + 0x8000);

		if (ds_readb(0x2d6e) == 0 &&
			(ds_readb(0x2d67) == 1 || ds_readb(0x2d67) == 0x27 ||
				ds_readb(0x2d67) == 0x11)) {
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
			bc_lseek(fd, ds_readb(0x2d75) * 320, 0);
			bc__read(fd, p_datseg + 0xbd95, 0x100);
			/* read automap tiles */
			bc__read(fd, p_datseg + 0xe442, 0x40);
			ds_writew(0x5eb8, 0);

			if (ds_readb(0x2d6e) == 0) {
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

void call_load_area(unsigned short type)
{
	load_area_description(type);
}

void load_npc(signed short index)
{
	Bit8u *npc_dst;
	unsigned short fd;

	npc_dst = get_hero(0) + 0x291c;
	/* load from temp directory */
	fd = load_archive_file(index | 0x8000);
	bc__read(fd, npc_dst, 0x6da);
	bc_close(fd);

	if (host_readb(npc_dst + 0x22) == 1) {
		/* female */
		/* set an unknown variable to typus + 11 */
		host_writeb(npc_dst + 0x9b, host_readb(npc_dst + 0x21) + 11);
		if (host_readb(npc_dst + 0x9b) > 21)
			host_writeb(npc_dst + 0x9b, 21);
	} else {
		/* male */
		host_writeb(npc_dst + 0x9b, host_readb(npc_dst + 0x21));
		if (host_readb(npc_dst + 0x9b) > 10)
			host_writeb(npc_dst + 0x9b, 10);
	}


}

void save_npc(signed short index)
{
	Bit16u fd;

	fd = load_archive_file(index | 0x8000);

	bc__write(fd, ds_readd(HEROS) + 0x291c, 0x6da);

	bc_close(fd);

}

void load_tlk(signed short index)
{
	Bit8u *ptr;
	unsigned int off, text_len;
	unsigned short partners;
	unsigned short fd;
	unsigned short i;

	ds_writew(0x26bd, index);

	fd = load_archive_file(index);

	/* read the header */
	read_archive_file(fd, (Bit8u*)&off, 4);
	read_archive_file(fd, (Bit8u*)&partners, 2);

	/* read the partner structures */
	read_archive_file(fd,
		ptr = Real2Host(RealMake(datseg, 0x3618)), partners * 0x26);

	/* read the dialog layouts */
	read_archive_file(fd,
		Real2Host(RealMake(datseg, 0x3794)), off - partners * 0x26);

	/* read the text */
	text_len = read_archive_file(fd, Real2Host(ds_readd(0xd2b5)), 64000);

	bc_close(fd);

	split_textbuffer(Real2Host(ds_readd(DIALOG_TEXT)),
		ds_readd(0xd2b5), text_len);

	/* adjust the pointers to the layouts */
	for (i = 0; i < partners; i++, ptr += 0x26) {
		host_writed(ptr, RealMake(datseg, host_readw(ptr) + 0x3794));
	}
}

void load_fightbg(signed short index)
{
	unsigned short fd;

	fd = load_archive_file(index);
	read_archive_file(fd, Real2Host(ds_readd(0xd303)), 30000);
	decomp_pp20(Real2Host(ds_readd(0xd303)), Real2Host(ds_readd(0xc3a9)),
		Real2Host(ds_readd(0xd303)) + 4, get_readlength2(fd));
	bc_close(fd);

}

}
