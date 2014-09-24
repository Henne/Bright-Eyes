#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg026.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void init_text(void)
{
	Bit32s len;
	signed short handle;

	/* FONT6 */
	handle = load_archive_file(0x0b);
	read_archive_file(handle, Real2Host(ds_readd(BUF_FONT6)), 1000);
	bc_close(handle);

	/* TEXT.LTX */
	handle = load_archive_file(0x0e);
	len = (signed short)read_archive_file(handle, Real2Host(ds_readd(0xd2b9)), 64000);
	bc_close(handle);

	split_textbuffer(Real2Host(ds_readd(0xc3b5)), (RealPt)ds_readd(0xd2b9), len);

	/* ITEMNAME */
	handle = load_archive_file(0xd0);
	len = (signed short)read_archive_file(handle, Real2Host(ds_readd(0xd2a5)), 5000);
	bc_close(handle);

	split_textbuffer(Real2Host(ds_readd(0xe22f)), (RealPt)ds_readd(0xd2a5), len);

	/* MONNAMES */
	handle = load_archive_file(0xd1);
	len = (signed short)read_archive_file(handle, Real2Host(ds_readd(0xd2a1)), 5000);
	bc_close(handle);

	split_textbuffer(Real2Host(ds_readd(0xe129)), (RealPt)ds_readd(0xd2a1), len);

}

/* Borlandified and identical */
void load_buffer_1(signed short index)
{
	Bit32s len;
	signed short fd;

	if (index == -1)
		return;

	fd = load_archive_file(index);

	len = (signed short)read_archive_file(fd, Real2Host(ds_readd(0xd2b5)), 64000);

	bc_close(fd);

	split_textbuffer(Real2Host(ds_readd(DIALOG_TEXT)), (RealPt)ds_readd(0xd2b5), len);

	ds_writew(0x26bf, index);
}

/* Borlandified and identical */
void load_city_ltx(signed short index)
{
	Bit32s len;
	register signed short fd;

	if (index == -1)
		return;

	ds_writew(0x26bd, index);
	fd = load_archive_file(index);
	len = (signed short)read_archive_file(fd, Real2Host(ds_readd(0xc3a9)), 12000);
	bc_close(fd);

	split_textbuffer(Real2Host(ds_readd(CITY_LTX)), (RealPt)ds_readd(0xc3a9), len);
}

/* Borlandified and identical */
void load_ltx(unsigned short index)
{
	Bit32s len;
	signed short fd;

	fd = load_archive_file(index);
	ds_writew(0x2ccb, 0xffff);
	len = (signed short)read_archive_file(fd, Real2Host(ds_readd(0xd019)) + 1000, 64000);
	bc_close(fd);

	split_textbuffer(Real2Host(ds_readd(0xd019)),
		F_PADD((RealPt)ds_readd(0xd019), 1000L), len);
}

/* Borlandified and identical */
void split_textbuffer(Bit8u *dst, RealPt src, Bit32u len)
{
	Bit32u i = 0;

	host_writed(dst, (Bit32u)src);
	dst += 4;

	for (; i != len; src++, i++) {
		/* continue if not the end of the string */
		if (!host_readbs(Real2Host(src))) {

			/* write the adress of the next string */
			host_writed(dst, (Bit32u)(src + 1));
			dst += 4;
		}
	}
}

/* Borlandified and identical */
void load_ggsts_nvf(void)
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

/* Borlandified and identical */
void prepare_chr_name(char *dst, char *src)
{
	char tmp_str[40];
	signed short i;

	strcpy(tmp_str, src);

	for (i = 0; i < 8; i++) {

		if (!tmp_str[i])
			break;

#if !defined(__BORLANDC__)
		/* maybe !isalnum(tmp_str[i]) */
		if ((ds_readb(0xb4e9 + tmp_str[i]) & 0x0e) == 0)
#else
		if (!isalnum(tmp_str[i]))
#endif
			tmp_str[i] = 0x5f;


	}

	strncpy(dst, tmp_str, 8);
	dst[8] = '\0';
	strcat(dst, (char*)p_datseg + 0x5e3e);
}

/* Borlandified and identical */
void prepare_sg_name(char *dst, char *src)
{
	char tmp_str[40];
	signed short i;

	strcpy(tmp_str, src);

	i = 0;
	while (i < 8) {
		if (!tmp_str[i]) {
			while (i < 8) {
				/* fill up with underscores */
				tmp_str[i] = 0x5f;
				i++;
			}
			break;
		}
#if !defined(__BORLANDC__)
		if ((ds_readb(0xb4e9 + tmp_str[i]) & 0x0e) == 0)
#else
		if (!isalnum(tmp_str[i]))
#endif
			tmp_str[i] = 0x5f;
		i++;
	}

	strncpy(dst, tmp_str, 8);
	dst[8] = '\0';
}

signed short load_save_game(void)
{
	DUMMY_WARNING();
	return -1;
}

void write_chr_temp(unsigned short hero)
{
	char fname[20];
	unsigned short fd;

	prepare_chr_name(fname, (char*)get_hero(hero));

	sprintf((char*)Real2Host(ds_readd(0xd2eb)),
		(char*)Real2Host(ds_readd(0x4c88)),		/* "TEMP\\%s" */
		fname);

	fd = bc__creat((RealPt)ds_readd(0xd2eb), 0);
	bc__write(fd, (RealPt)ds_readd(HEROS) + hero * 0x6da, 0x6da);
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

	seg002_0c72(fd, head * 1024, 0);

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

#if !defined(__BORLANDC__)
}
#endif
