/*
	Rewrite of DSA1 v3.02_de functions of seg027 (file loader)
	Functions rewritten: 3/8
*/

#include "schick.h"

#include "seg000.h"
#include "seg002.h"
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
