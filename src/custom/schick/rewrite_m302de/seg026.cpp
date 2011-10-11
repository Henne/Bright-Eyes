#include "schick.h"
#include "schick_common.h"
#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg026.h"

namespace M302de {

void load_city_ltx(signed short index)
{
	unsigned int len;
	unsigned short fd;

	if (index == -1)
		return;

	ds_writew(0x26bd, index);
	fd = load_archive_file(index);
	len = read_archive_file(fd, Real2Host(ds_readd(0xc3a9)), 12000);
	bc_close(fd);

	split_textbuffer(Real2Host(ds_readd(0xc3ad)), ds_readd(0xc3a9), len);
}

void load_ltx(unsigned short index)
{
	unsigned int len;
	unsigned short fd;

	fd = load_archive_file(index);
	ds_writew(0x2ccb, 0xffff);
	len = read_archive_file(fd, Real2Host(ds_readd(0xd019)) + 1000, 64000);
	bc_close(fd);

	split_textbuffer(Real2Host(ds_readd(0xd019)),
		ds_readd(0xd019) + 1000, len);
}

void split_textbuffer(Bit8u *dst, RealPt src, Bit32u len)
{
	Bit32u i = 0;

	host_writed(dst, src);
	dst += 4;

	for (; i != len; src++, i++) {
		/* continue if not the end of the string */
		if (host_readb(Real2Host(src)) != 0)
			continue;
		/* write the adress of the next string */
		host_writed(dst, src + 1);
		dst += 4;
	}
}

void load_ggsts_nvf()
{
	Bit16u fd;

	/* seek to GGSTS.NVF */
	fd = load_archive_file(0x0d);
	/* read it */
	read_archive_file(fd, Real2Host(ds_readd(0xd2a9)), 16771);
	/* close it */
	bc_close(fd);

	ds_writew(0x2ccb, 0xffff);
}

/**
 *	load_in_head() - loads a head icon from IN_HEADS.NVF
 *	@head:	index of the desired head
 */
void load_in_head(Bit16s head)
{
	Bit16u fd;

	if (head < 0)
		return;

	fd = load_archive_file(0xb3);

	seg002_0c72(fd, head * 1024);

	read_archive_file(fd, Real2Host(ds_readd(0xd2f3)), 1024);

	bc_close(fd);

	ds_writew(0x515c, head);

}

}
