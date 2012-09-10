#include <string.h>

#include "schick.h"
#include "v302de.h"
#include "common.h"

#include "seg000.h"
#include "seg002.h"
#include "seg026.h"

namespace M302de {

void load_buffer_1(signed short index)
{
	signed int len;
	unsigned short fd;

	if (index == -1)
		return;

	fd = load_archive_file(index);

	len = read_archive_file(fd, Real2Host(ds_readd(0xd2b5)), 64000);

	bc_close(fd);

	split_textbuffer(Real2Host(ds_readd(DIALOG_TEXT)), ds_readd(0xd2b5), len);

	ds_writew(0x26bf, index);

}

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

	split_textbuffer(Real2Host(ds_readd(CITY_LTX)), ds_readd(0xc3a9), len);
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

void prepare_chr_name(char *dst, char *src)
{
	char tmp_str[40];
	unsigned short i;

	strcpy(tmp_str, src);

	for (i = 0; i < 8; i++) {

		if (tmp_str[i] == '\0')
			break;

		/* maybe !isalnum(tmp_str[i]) */
		if ((ds_readb(0xb4e9 + tmp_str[i]) & 0x0e) == 0)
			tmp_str[i] = 0x5f;
	}

	strncpy(dst, tmp_str, 8);
	dst[8] = '\0';
	strcat(dst, ".CHR");
}

void prepare_sg_name(char *dst, char *src)
{
	char tmp_str[40];
	Bit16u i;

	strcpy(tmp_str, src);

	i = 0;
	while (i < 8) {
		if (tmp_str[i] == '\0') {
			while (i < 8) {
				/* fill up with underscores */
				tmp_str[i] = 0x5f;
				i++;
			}
			break;
		} else {
			if ((ds_readb(0xb4e9 + tmp_str[i]) & 0x0e) == 0)
				tmp_str[i] = 0x5f;
		}

		i++;
	}

	strncpy(dst, tmp_str, 8);
	dst[8] = '\0';
}

void write_chr_temp(unsigned short hero)
{
	char fname[20];
	unsigned short fd;

	prepare_chr_name(fname, (char*)get_hero(hero));

	sprintf((char*)Real2Host(ds_readd(0xd2eb)),
		(char*)Real2Host(ds_readd(0x4c88)),		/* "TEMP\\%s" */
		fname);

	fd = bc__creat(ds_readd(0xd2eb), 0);
	bc__write(fd, ds_readd(HEROS) + hero * 0x6da, 0x6da);
	bc_close(fd);
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

void load_tempicon(unsigned short nr)
{
	struct nvf_desc nvf;
	unsigned short fd;

	if (nr == 14)
		nr = 7;

	/* load TEMPICON */
	fd = load_archive_file(0xb4);
	read_archive_file(fd, Real2Host(ds_readd(0xc3a9)), 7000);
	bc_close(fd);

	nvf.dst = Real2Host(ds_readd(0xc3a9)) + 7000;
	nvf.src = Real2Host(ds_readd(0xc3a9));
	nvf.nr = nr;
	nvf.type = 0;
	nvf.width = (Bit8u*)&fd;
	nvf.height = (Bit8u*)&fd;

	process_nvf(&nvf);
}

}
