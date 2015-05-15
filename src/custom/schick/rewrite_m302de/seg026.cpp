/**
 *	Rewrite of DSA1 v3.02_de functions of seg026 (texts savegames)
 *	Functions rewritten: 12/15
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg026.h"
#include "seg028.h"
#include "seg097.h"

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

	split_textbuffer(Real2Host(ds_readd(TEXT_LTX)), (RealPt)ds_readd(0xd2b9), len);

	/* ITEMNAME */
	handle = load_archive_file(0xd0);
	len = (signed short)read_archive_file(handle, Real2Host(ds_readd(0xd2a5)), 5000);
	bc_close(handle);

	split_textbuffer(Real2Host(ds_readd(ITEMSNAME)), (RealPt)ds_readd(0xd2a5), len);

	/* MONNAMES */
	handle = load_archive_file(0xd1);
	len = (signed short)read_archive_file(handle, Real2Host(ds_readd(0xd2a1)), 5000);
	bc_close(handle);

	split_textbuffer(Real2Host(ds_readd(MONNAMES)), (RealPt)ds_readd(0xd2a1), len);

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

		/* maybe !isalnum(tmp_str[i]) */
		if (!(ds_readbs(0xb4e9 + tmp_str[i]) & 0x0e)) {
			tmp_str[i] = 0x5f;
		}
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

		/* maybe !isalnum(tmp_str[i]) */
		if (!(ds_readbs(0xb4e9 + tmp_str[i]) & 0x0e)) {
			tmp_str[i] = 0x5f;
		}
		i++;
	}

	strncpy(dst, tmp_str, 8);
	dst[8] = '\0';
}

/* Borlandified and identical */
signed short load_game_state(void)
{
	register signed short handle_gs;
	signed short i;
	signed short handle;
	signed short answer;
	signed short l1;
	HugePt p_status_start;
	HugePt p_status_end;
	signed short status_length;
	signed short l2;
	signed short l3;
	signed short retval;
	signed short l4;
	RealPt hero_i;
	signed char version[4];
	struct ffblk blk;
	char name[20];

	retval = 0;
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else

	/* select a game state */
	answer = GUI_radio(get_ltx(0), 6,
			p_datseg + (0xe2da + 0),
			p_datseg + (0xe2da + 9),
			p_datseg + (0xe2da + 18),
			p_datseg + (0xe2da + 27),
			p_datseg + (0xe2da + 36),
			get_ltx(0xb84)) -1;

	/* sanity check if answer is in range */
	if (answer != -2 && answer != 5) {

		prepare_sg_name((char*)Real2Host(ds_readd(0xd2eb)), (char*)p_datseg + 0xe2da + 9 * answer);
		/* concat with ".gam" */
		strcat((char*)Real2Host(ds_readd(0xd2eb)), (char*)p_datseg + 0x5e43);

		/* open the game state file */
		if ((handle_gs = bc__open((RealPt)ds_readd(0xd2eb), 0x8001)) == -1) {
			GUI_output(get_ltx(0x9ec));
			retval = -1;
			return retval;
		}

		update_mouse_cursor();

		/* something ani related */
		l1 = ds_readws(0xc3cb);
		ds_writew(0xc3cb, 0);

		l4 = ds_readws(0x29ae);
		ds_writew(0x29ae, 0);

		/* delete every file in TEMP */
		sprintf((char*)Real2Host(ds_readd(0xd2eb)),
			/* "TEMP\\%s" */
			(char*)Real2Host(ds_readd(0x4c88)),
			/* "*.*" */
			(char*)p_datseg + 0x5e48);

		l2 = bc_findfirst((RealPt)ds_readd(0xd2eb), &blk, 0);

		if (l2 == 0) {

			do {
				sprintf((char*)Real2Host(ds_readd(0xd2eb)),
					(char*)Real2Host(ds_readd(0x4c88)),
					((char*)(&blk))+ 30);

				bc_unlink((RealPt)ds_readd(0xd2eb));

				l2 = bc_findnext(&blk);

			} while (l2 == 0);
		}

		/* init */
		ds_writed(0xe2d2, ds_readd(DTP2));
		memset(Real2Host(ds_readd(0xe2d2)), 0, 286 * 4);

		/* read version info */
		bc__read(handle_gs, Real2Host(ds_readd(0xd2eb)), 12);
		bc__read(handle_gs, (Bit8u*)&version[3], 1);
		bc__read(handle_gs, (Bit8u*)&version[2], 1);
		bc__read(handle_gs, (Bit8u*)&version[0], 1);
		bc__read(handle_gs, (Bit8u*)&version[1], 1);

		bc__read(handle_gs, p_datseg + 0x2d34, 4);

		/* read game status */
		p_status_start = (HugePt)RealMake(datseg, 0x2d34);
		p_status_end = (HugePt)RealMake(datseg, 0x4474);
#if !defined(__BORLANDC__)
		status_length = F_PSUB(p_status_end, p_status_start);
#else
		status_length = p_status_end - p_status_start;
#endif

		bc__read(handle_gs, p_datseg + 0x2d34, status_length);

		ds_writeb(0x45b8, 1);

		/* read file table */
		bc__read(handle_gs, Real2Host(ds_readd(0xe2d2)), 286 * 4);

		/* create for each saved file in gam a file in TEMP */
		for (i = 0; i < 286; i++) {

			if (host_readd(Real2Host(ds_readd(0xe2d2)) + 4 * i)) {

				/* write file content to TEMP */
				sprintf((char*)Real2Host(ds_readd(0xd2eb)),
					(char*)Real2Host(ds_readd(0x4c88)),
					(char*)Real2Host(ds_readd(0x4c8c + 4 * i)));

				handle = bc__creat((RealPt)ds_readd(0xd2eb), 0);

				bc__read(handle_gs, Real2Host(ds_readd(0xd303)), host_readd(Real2Host(ds_readd(0xe2d2)) + 4 * i));
				bc__write(handle, (RealPt)ds_readd(0xd303), host_readd(Real2Host(ds_readd(0xe2d2)) + 4 * i));
				bc_close(handle);
			}
		}

		/* clear the heros */
		hero_i = (RealPt)ds_readd(HEROS);
		for (i = 0; i <= 6; i++, hero_i += 0x6da) {
			memset(Real2Host(hero_i), 0, 0x6da);
		}

		hero_i = (RealPt)ds_readd(0xd303);

		do {
			l3 = bc__read(handle_gs, Real2Host(hero_i), 0x6da);

			if (l3 != 0) {

				prepare_chr_name(name, Real2Host(hero_i));

				/* write file content to TEMP */
				sprintf((char*)Real2Host(ds_readd(0xd2eb)),
					(char*)Real2Host(ds_readd(0x4c88)),
					name);

				handle = bc__creat((RealPt)ds_readd(0xd2eb), 0);

				bc__write(handle, hero_i, 0x6da);
				bc_close(handle);

				if (host_readbs(Real2Host(hero_i) + 0x8a) != 0) {

					prepare_chr_name(name, Real2Host(hero_i));

					read_chr_temp(name, host_readbs(Real2Host(hero_i) + 0x8a) - 1, host_readbs(Real2Host(hero_i) + 0x87));
				}
			}
		} while (l3 != 0);

		bc_close(handle_gs);

		/* search for "*.CHR" */
		l2 = bc_findfirst(p_datseg + 0x5e4c, &blk, 0);

		while (l2 == 0) {

			sprintf((char*)Real2Host(ds_readd(0xd2eb)),
				(char*)Real2Host(ds_readd(0x4c88)),
				((char*)(&blk)) + 30);

			if ((handle_gs = bc__open((char*)Real2Host(ds_readd(0xd2eb)), 0x8004)) == -1) {

				handle = bc__open((char*)(&blk) + 30, 0x8004);
				bc__read(handle, Real2Host(ds_readd(0xd303)), 0x6da);
				bc_close(handle);

				handle_gs = bc__creat(Real2Host(ds_readd(0xd2eb)), 0);
				bc__write(handle_gs, (RealPt)ds_readd(0xd303), 0x6da);
			} else {
				/* Yes, indeed! */
			}

			bc_close(handle_gs);

			l2 = bc_findnext(&blk);
		}

		for (i = 226; i <= 231; i++) {
			load_npc(i);

			if (host_readbs(get_hero(6) + 0x8a) != 7) {
				memset(get_hero(6), 0, 0x6da);
			} else {
				break;
			}
		}

		ds_writew(0x2ccb, -1);
		ds_writew(0x2846, retval = 1);
		ds_writew(CHECK_DISEASE, 0);
		ds_writew(CHECK_POISON, 0);
		ds_writeb(0x4475, 3);

		if (ds_readbs(LOCATION) != 2) {
			ds_writebs((0xbd38 + 6), ds_writebs((0xbd38 + 7), ds_writebs((0xbd38 + 8), -1)));
		}

		load_area_description(2);

		ds_writews(0xc3cb, l1);
		ds_writews(0x29ae, l4);

		refresh_screen_size();
	}

#endif
	return retval;
}

signed short read_chr_temp(Bit8u* str, signed short a1, signed short a2)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#endif
	return 0;
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

	read_archive_file(fd, Real2Host(ds_readd(DTP2)), 1024);

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
