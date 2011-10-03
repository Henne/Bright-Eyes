/*
	Rewrite of DSA1 v3.02_de functions of seg027 (file loader)
	Functions rewritten: 1/8
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

}
