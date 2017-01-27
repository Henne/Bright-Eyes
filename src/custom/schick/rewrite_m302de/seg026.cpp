/**
 *	Rewrite of DSA1 v3.02_de functions of seg026 (texts savegames)
 *	Functions rewritten: 15/15 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg026.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg026.h"
#include "seg028.h"
#include "seg097.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void init_text(void)
{
	Bit32s len;
	signed short handle;

	handle = load_archive_file(ARCHIVE_FILE_FONT6);
	read_archive_file(handle, Real2Host(ds_readd(BUF_FONT6)), 1000);
	bc_close(handle);

	handle = load_archive_file(ARCHIVE_FILE_TEXT_LTX);
	len = (signed short)read_archive_file(handle, Real2Host(ds_readd(TEXT_LTX_BUFFER)), 64000);
	bc_close(handle);

	split_textbuffer(Real2Host(ds_readd(TEXT_LTX_INDEX)), (RealPt)ds_readd(TEXT_LTX_BUFFER), len);

	handle = load_archive_file(ARCHIVE_FILE_ITEMNAME);
	len = (signed short)read_archive_file(handle, Real2Host(ds_readd(BUFFER5_PTR)), 5000);
	bc_close(handle);

	split_textbuffer(Real2Host(ds_readd(ITEMSNAME)), (RealPt)ds_readd(BUFFER5_PTR), len);

	handle = load_archive_file(ARCHIVE_FILE_MONNAMES);
	len = (signed short)read_archive_file(handle, Real2Host(ds_readd(MONNAMES_BUFFER)), 5000);
	bc_close(handle);

	split_textbuffer(Real2Host(ds_readd(MONNAMES_INDEX)), (RealPt)ds_readd(MONNAMES_BUFFER), len);

}

void load_tx(signed short index)
{
	Bit32s archive_file_len;
	signed short archive_file_handle;

	if (index == -1)
		return;

	archive_file_handle = load_archive_file(index);

	archive_file_len = (signed short)read_archive_file(archive_file_handle, Real2Host(ds_readd(BUFFER7_PTR)), 64000);

	bc_close(archive_file_handle);

	split_textbuffer(Real2Host(ds_readd(TX_INDEX)), (RealPt)ds_readd(BUFFER7_PTR), archive_file_len);

	ds_writew(TX_FILE_INDEX, index);
}

void load_tx2(signed short index)
{
	Bit32s len;
	register signed short fd;

	if (index == -1)
		return;

	ds_writew(TEXT_FILE_INDEX, index);
	fd = load_archive_file(index);
	len = (signed short)read_archive_file(fd, Real2Host(ds_readd(BUFFER8_PTR)), 12000);
	bc_close(fd);

	split_textbuffer(Real2Host(ds_readd(TX2_INDEX)), (RealPt)ds_readd(BUFFER8_PTR), len);
}

void load_ltx(unsigned short index)
{
	Bit32s len;
	signed short fd;

	fd = load_archive_file(index);
	ds_writew(AREA_PREPARED, 0xffff);
	len = (signed short)read_archive_file(fd, Real2Host(ds_readd(BUFFER9_PTR3)) + 1000, 64000);
	bc_close(fd);

	split_textbuffer(Real2Host(ds_readd(BUFFER9_PTR3)),
		F_PADD((RealPt)ds_readd(BUFFER9_PTR3), 1000L), len);
}

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

void load_ggsts_nvf(void)
{
	Bit16u fd;

	/* seek to GGSTS.NVF */
	fd = load_archive_file(ARCHIVE_FILE_GGSTS_NVF);
	/* read it */
	read_archive_file(fd, Real2Host(ds_readd(BUFFER10_PTR)), 16771);
	/* close it */
	bc_close(fd);

	ds_writew(AREA_PREPARED, 0xffff);
}

void prepare_chr_name(char *dst, char *src)
{
	char tmp_str[40];
	signed short i;

	strcpy(tmp_str, src);

	for (i = 0; i < 8; i++) {

		if (!tmp_str[i])
			break;

		/* maybe !isalnum(tmp_str[i]) */
		if (!(ds_readbs(CHAR_TYPE_TABLE + tmp_str[i]) & 0x0e)) {
			tmp_str[i] = 0x5f;
		}
	}

	strncpy(dst, tmp_str, 8);
	dst[8] = '\0';
	strcat(dst, (char*)p_datseg + CHR_FILE_SUFFIX);
}

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
		if (!(ds_readbs(CHAR_TYPE_TABLE + tmp_str[i]) & 0x0e)) {
			tmp_str[i] = 0x5f;
		}
		i++;
	}

	strncpy(dst, tmp_str, 8);
	dst[8] = '\0';
}

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

	/* select a game state */
	answer = GUI_radio(get_ttx(0), 6,
			p_datseg + (SAVEGAME_NAMES + 0),
			p_datseg + (SAVEGAME_NAMES + 9),
			p_datseg + (SAVEGAME_NAMES + 18),
			p_datseg + (SAVEGAME_NAMES + 27),
			p_datseg + (SAVEGAME_NAMES + 36),
			get_ttx(737)) -1;

	/* sanity check if answer is in range */
	if (answer != -2 && answer != 5) {

		prepare_sg_name((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)p_datseg + SAVEGAME_NAMES + 9 * answer);
		/* concat with ".gam" */
		strcat((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)p_datseg + SAVEGAME_SUFFIX);

		/* open the game state file */
		if ((handle_gs = bc_open(ds_readfp(TEXT_OUTPUT_BUF), 0x8001)) == -1)
		{
			GUI_output(get_ttx(635));
			retval = -1;
			return retval;
		}

		update_mouse_cursor();

		/* something ani related */
		l1 = ds_readws(UPDATE_STATUSLINE);
		ds_writew(UPDATE_STATUSLINE, 0);

		l4 = ds_readws(ANI_ENABLED);
		ds_writew(ANI_ENABLED, 0);

		/* delete every file in TEMP */
		sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
			/* "TEMP\\%s" */
			(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),
			/* "*.*" */
			(char*)p_datseg + ALL_FILES_WILDCARD);

		l2 = bc_findfirst((RealPt)ds_readd(TEXT_OUTPUT_BUF), &blk, 0);

		if (l2 == 0) {

			do {
				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),
					((char*)(&blk))+ 30);

				bc_unlink((RealPt)ds_readd(TEXT_OUTPUT_BUF));

				l2 = bc_findnext(&blk);

			} while (l2 == 0);
		}

		/* init */
		ds_writed(SAVED_FILES_BUF, ds_readd(DTP2));
		memset(Real2Host(ds_readd(SAVED_FILES_BUF)), 0, 286 * 4);

		/* read version info */
		bc__read(handle_gs, Real2Host(ds_readd(TEXT_OUTPUT_BUF)), 12);
		bc__read(handle_gs, (Bit8u*)&version[3], 1);
		bc__read(handle_gs, (Bit8u*)&version[2], 1);
		bc__read(handle_gs, (Bit8u*)&version[0], 1);
		bc__read(handle_gs, (Bit8u*)&version[1], 1);

		bc__read(handle_gs, p_datseg + DATSEG_STATUS_START, 4);

		/* read game status */
		p_status_start = (HugePt)RealMake(datseg, DATSEG_STATUS_START);
		p_status_end = (HugePt)RealMake(datseg, DATSEG_STATUS_END);
#if !defined(__BORLANDC__)
		status_length = (signed short)F_PSUB(p_status_end, p_status_start);
#else
		status_length = (signed short)(p_status_end - p_status_start);
#endif

		bc__read(handle_gs, p_datseg + DATSEG_STATUS_START, status_length);

		ds_writeb(SPECIAL_SCREEN, 1);

		/* read file table */
		bc__read(handle_gs, Real2Host(ds_readd(SAVED_FILES_BUF)), 286 * 4);

		/* create for each saved file in gam a file in TEMP */
		for (i = 0; i < 286; i++) {

			if (host_readd(Real2Host(ds_readd(SAVED_FILES_BUF)) + 4 * i)) {

				/* write file content to TEMP */
				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),
					(char*)Real2Host(ds_readd(FNAMES + 4 * i)));

				handle = bc__creat((RealPt)ds_readd(TEXT_OUTPUT_BUF), 0);

				bc__read(handle_gs, Real2Host(ds_readd(RENDERBUF_PTR)), (unsigned short)host_readd(Real2Host(ds_readd(SAVED_FILES_BUF)) + 4 * i));
				bc__write(handle, (RealPt)ds_readd(RENDERBUF_PTR), (unsigned short)host_readd(Real2Host(ds_readd(SAVED_FILES_BUF)) + 4 * i));
				bc_close(handle);
			}
		}

		/* clear the heros */
		hero_i = (RealPt)ds_readd(HEROS);
		for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
			memset(Real2Host(hero_i), 0, SIZEOF_HERO);
		}

		hero_i = (RealPt)ds_readd(RENDERBUF_PTR);

		do {
			l3 = bc__read(handle_gs, Real2Host(hero_i), SIZEOF_HERO);

			if (l3 != 0) {

				prepare_chr_name(name, (char*)Real2Host(hero_i));

				/* write file content to TEMP */
				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),
					name);

				handle = bc__creat((RealPt)ds_readd(TEXT_OUTPUT_BUF), 0);

				bc__write(handle, hero_i, SIZEOF_HERO);
				bc_close(handle);

				if (host_readbs(Real2Host(hero_i) + HERO_GROUP_POS) != 0) {

					prepare_chr_name(name, (char*)Real2Host(hero_i));
#if !defined(__BORLANDC__)
					{
						/* create a char[20] on the stack */
						Bit16u sp_bak = reg_sp;
						reg_sp -= 20;

						RealPt r_name = RealMake(SegValue(ss), reg_sp);
						strncpy((char*)Real2Host(r_name), name, 20);
						host_writeb(Real2Host(r_name) + 20, 0);
						read_chr_temp(r_name, host_readbs(Real2Host(hero_i) + HERO_GROUP_POS) - 1, host_readbs(Real2Host(hero_i) + HERO_GROUP_NO));
						reg_sp = sp_bak;
					}
#else
					read_chr_temp(name, host_readbs(Real2Host(hero_i) + HERO_GROUP_POS) - 1, host_readbs(Real2Host(hero_i) + HERO_GROUP_NO));
#endif
				}
			}
		} while (l3 != 0);

		bc_close(handle_gs);

		/* search for "*.CHR" */
		l2 = bc_findfirst((RealPt)RealMake(datseg, ALL_CHR_WILDCARD), &blk, 0);

		while (l2 == 0) {

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),
				((char*)(&blk)) + 30);

			if ((handle_gs = bc_open((RealPt)ds_readd(TEXT_OUTPUT_BUF), 0x8004)) == -1) {
#if !defined(__BORLANDC__)
				{
					Bit16u sp_bak = reg_sp;
					reg_sp -= 128;
					RealPt fname = RealMake(SegValue(ss), reg_sp);
					strncpy((char*)Real2Host(fname), (char*)(&blk) + 30, 128);
					host_writeb(Real2Host(fname) + 128, 0);
					handle = bc_open(fname, 0x8004);
					reg_sp = sp_bak;
				}
#else
				handle = bc_open((char*)(&blk) + 30, 0x8004);
#endif
				bc__read(handle, Real2Host(ds_readd(RENDERBUF_PTR)), SIZEOF_HERO);
				bc_close(handle);

				handle_gs = bc__creat((RealPt)ds_readd(TEXT_OUTPUT_BUF), 0);
				bc__write(handle_gs, (RealPt)ds_readd(RENDERBUF_PTR), SIZEOF_HERO);
			} else {
				/* Yes, indeed! */
			}

			bc_close(handle_gs);

			l2 = bc_findnext(&blk);
		}

		for (i = ARCHIVE_FILE_NPCS; i <= (ARCHIVE_FILE_NPCS+5); i++) {
			load_npc(i);

			if (host_readbs(get_hero(6) + HERO_GROUP_POS) != 7) {
				memset(get_hero(6), 0, SIZEOF_HERO);
			} else {
				break;
			}
		}

		ds_writew(AREA_PREPARED, -1);
		ds_writew(REQUEST_REFRESH, retval = 1);
		ds_writew(CHECK_DISEASE, 0);
		ds_writew(CHECK_POISON, 0);
		ds_writeb(FADING_STATE, 3);

		if (ds_readbs(LOCATION) != LOCATION_TEMPLE) {
			ds_writebs((NEW_MENU_ICONS + 6), ds_writebs((NEW_MENU_ICONS + 7), ds_writebs((0xbd38 + 8), -1)));
		}

		load_area_description(2);

		ds_writews(UPDATE_STATUSLINE, l1);
		ds_writews(ANI_ENABLED, l4);

		refresh_screen_size();
	}

	return retval;
}

/**
 * \brief   writes a game state file
 *
 * \return              1 = OK, 0 = error
 */
signed short save_game_state(void)
{
	signed short l_di;
	HugePt p_status_start;
	HugePt p_status_end;
	unsigned short status_len;
	signed short handle;
	signed short tw_bak;
	signed short l1;
	signed short slot;
	Bit32u filepos;
	Bit32u filepos2;
	signed short flag;
	Bit32u len;
	struct ffblk blk;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 5);

	/* prepare the header for the radio box */
	if (ds_readws(GAME_STATE) == GAME_STATE_VICTORY) {

		/* game done */
		strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_ttx(810));

	} else {

		if (ds_readbs(LOCATION) != LOCATION_TEMPLE && ds_readws(GAME_STATE) != GAME_STATE_VICTORY) {

			/* save outside the temple */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(813),
				1,
				get_ttx(392),
				p_datseg + EMPTY_STRING1);

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)get_ttx(1),
				(char*)Real2Host(ds_readd(DTP2)));
		} else {

			/* save inside a temple */
			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)get_ttx(1),
				(char*)p_datseg + EMPTY_STRING2);
		}
	}

	/* get the slot number */
	slot = GUI_radio(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), 6,
			p_datseg + (SAVEGAME_NAMES + 9 * 0),
			p_datseg + (SAVEGAME_NAMES + 9 * 1),
			p_datseg + (SAVEGAME_NAMES + 9 * 2),
			p_datseg + (SAVEGAME_NAMES + 9 * 3),
			p_datseg + (SAVEGAME_NAMES + 9 * 4),
			get_ttx(737)) - 1;

	ds_writew(TEXTBOX_WIDTH, tw_bak);

	ds_writed(SAVED_FILES_BUF, ds_readd(DTP2));
	memset(Real2Host(ds_readd(SAVED_FILES_BUF)), 0, 4 * 286);

	if (slot != -2 && slot != 5) {

		do {
			/* ask for filename */
			ds_writew(GUI_ENTERING_SAVEGAME, 1);
			strcpy((char*)Real2Host(ds_readd(TEXT_INPUT_BUF)), (char*)p_datseg + SAVEGAME_NAMES + 9 * slot);
			GUI_input(get_ttx(634), 8);
			ds_writew(GUI_ENTERING_SAVEGAME, 0);

			if (host_readbs(Real2Host(ds_readd(TEXT_INPUT_BUF))) == 0) {
				return 0;
			}

			flag = 0;

			prepare_sg_name((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)Real2Host(ds_readd(TEXT_INPUT_BUF)));

			for (tw_bak = 0; tw_bak < 5; tw_bak++) {

				prepare_sg_name((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 50, (char*)p_datseg + SAVEGAME_NAMES + 9 * tw_bak);

				if (slot != tw_bak && !strcmp((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 50)) {

					GUI_output(get_ttx(806));
					flag = 1;
				}
			}
		} while (flag != 0);

		/* delete the previous file of that slot */
		prepare_sg_name((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)p_datseg + SAVEGAME_NAMES + 9 * slot);
		strcat((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)p_datseg + SAVEGAME_SUFFIX2);
		bc_unlink((RealPt)ds_readd(TEXT_OUTPUT_BUF));
		strcpy((char*)p_datseg + SAVEGAME_NAMES + 9 * slot, (char*)Real2Host(ds_readd(TEXT_INPUT_BUF)));

		/* create a CHR-file for each hero in TEMP */
		for (tw_bak = 0; tw_bak < 6; tw_bak++) {

			if (host_readbs(get_hero(tw_bak) + HERO_TYPE) != HERO_TYPE_NONE) {

				/* save position on the playmask */
				host_writebs(get_hero(tw_bak) + HERO_GROUP_POS, tw_bak + 1);

				if (ds_readws(GAME_STATE) != GAME_STATE_VICTORY &&
					ds_readbs(LOCATION) != LOCATION_TEMPLE &&
					host_readds(get_hero(tw_bak) + HERO_AP) > 0)
				{
					add_hero_ap(get_hero(tw_bak), -1L);
				}

				write_chr_temp(tw_bak);
			}
		}

		/* save the current NPC in TEMP */
		if (host_readbs(get_hero(6) + HERO_TYPE) != HERO_TYPE_NONE) {

			host_writeb(get_hero(6) + HERO_GROUP_POS, 7);
			save_npc(host_readbs(get_hero(6) + HERO_NPC_ID) + 225);
		}

		load_area_description(1);

		p_status_start = (HugePt)RealMake(datseg, DATSEG_STATUS_START);
		p_status_end = (HugePt)RealMake(datseg, DATSEG_STATUS_END);
#if !defined(__BORLANDC__)
		status_len = (signed short)F_PSUB(p_status_end, p_status_start);
#else
		status_len = (signed short)(p_status_end - p_status_start);
#endif

		prepare_sg_name((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)p_datseg + SAVEGAME_NAMES + 9 * slot);
		strcat((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)p_datseg + SAVEGAME_SUFFIX3);

		while ((l_di = bc__creat((RealPt)ds_readd(TEXT_OUTPUT_BUF), 0)) == -1) {
			GUI_output(get_ttx(348));
			return 0;
		}

#if !defined(__BORLANDC__)
		bc_time_dosbox((RealPt)RealMake(datseg, LAST_SAVE_TIME));
#else
		bc_time((Bit32s*)RealMake(datseg, LAST_SAVE_TIME));
#endif

		filepos = 0;

		/* write version identifier 16 bytes */
		filepos += bc__write(l_di, (RealPt)RealMake(datseg, DSA_VERSION_STRING), 12);
		filepos += bc__write(l_di, (RealPt)RealMake(datseg, VERSION_TOKEN4), 1);
		filepos += bc__write(l_di, (RealPt)RealMake(datseg, VERSION_TOKEN3), 1);
		filepos += bc__write(l_di, (RealPt)RealMake(datseg, VERSION_TOKEN1), 1);
		filepos += bc__write(l_di, (RealPt)RealMake(datseg, VERSION_TOKEN2), 1);

		/* write fileposition 4 bytes */
		/* this will be updated later to find the data of the CHR files */
#if !defined(__BORLANDC__)
		/*	The value of filepos needs to be written to the file in
		 *	LE format. bc__write() needs an adress in RealMode-Space,
		 *	so some space on the stack is allocated.
		 */
		{
			Bit16u sp_bak = reg_sp;
			reg_sp -= 64;
			RealPt fp_le = RealMake(SegValue(ss), reg_sp);

			host_writed(Real2Host(fp_le), filepos);
			filepos += bc__write(l_di, fp_le, 4);

			reg_sp = sp_bak;
		}
#else
		filepos += bc__write(l_di, &filepos, 4);
#endif

		/* save the satus section 5952 bytes */
		filepos += bc__write(l_di, p_status_start, status_len);

		/* check if enough bytes were written */
		if (status_len + 16 + 4L != filepos) {
			GUI_output(get_ttx(348));
			bc_close(l_di);
			return 0;
		}

		filepos2 = filepos;
		len = (Bit16u)bc__write(l_di, (RealPt)ds_readd(SAVED_FILES_BUF), 4 * 286);
		filepos += len;

		if (len != 4 * 286) {
			GUI_output(get_ttx(348));
			bc_close(l_di);
			return 0;
		}

		/* save all changed files from SCHICK.DAT */
		for (tw_bak = 0; tw_bak < 286; tw_bak++) {

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),
				(char*)Real2Host(ds_readd(FNAMES + 4 * tw_bak)));

			l1 = bc_findfirst((RealPt)ds_readd(TEXT_OUTPUT_BUF), &blk, 0);


			if (l1 == 0) {

				handle = load_archive_file(tw_bak + 0x8000);
				host_writed(Real2Host(ds_readd(SAVED_FILES_BUF)) + 4 * tw_bak, get_readlength2(handle));
				bc__read(handle, Real2Host(ds_readd(RENDERBUF_PTR)), (unsigned short)host_readd(Real2Host(ds_readd(SAVED_FILES_BUF)) + 4 * tw_bak));
				bc_close(handle);

				len = (Bit16u)bc__write(l_di, (RealPt)ds_readd(RENDERBUF_PTR), (unsigned short)host_readd(Real2Host(ds_readd(SAVED_FILES_BUF)) + 4 * tw_bak));
				filepos += len;

				if ((Bit16u)host_readd(Real2Host(ds_readd(SAVED_FILES_BUF)) + 4 * tw_bak) != len) {
					GUI_output(get_ttx(348));
					bc_close(l_di);
					return 0;
				}
			}
		}

		/* skip back to the start of the offset of the CHR data */
		bc_lseek(l_di, 16, 0);
#if !defined(__BORLANDC__)
		/*	The value of filepos needs to be written to the file in
		 *	LE format. bc__write() needs an adress in RealMode-Space,
		 *	so some space on the stack is allocated.
		 */
		{
			Bit16u sp_bak = reg_sp;
			reg_sp -= 64;
			RealPt fp_le = RealMake(SegValue(ss), reg_sp);

			host_writed(Real2Host(fp_le), filepos);
			bc__write(l_di, fp_le, 4);

			reg_sp = sp_bak;
		}
#else
		bc__write(l_di, &filepos, 4);
#endif

		/* write the file table */
		bc_lseek(l_di, filepos2, 0);
		bc__write(l_di, (RealPt)ds_readd(SAVED_FILES_BUF), 4 * 286);

		/* append all CHR files */
		bc_lseek(l_di, filepos, 0);
		sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
			(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),
			(char*)p_datseg + ALL_CHR_WILDCARD2);

		l1 = bc_findfirst((RealPt)ds_readd(TEXT_OUTPUT_BUF), &blk, 0);
		do {
			/* create the CHR filename */
			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),
				((char*)(&blk)) + 30);

			/* read the CHR file from temp */
			handle = bc_open(ds_readfp(TEXT_OUTPUT_BUF), 0x8004);
			bc__read(handle, Real2Host(ds_readd(RENDERBUF_PTR)), SIZEOF_HERO);
			bc_close(handle);

			/* append it */
			len = bc__write(l_di, (RealPt)ds_readd(RENDERBUF_PTR), SIZEOF_HERO);

			if (len != SIZEOF_HERO) {
				GUI_output(get_ttx(348));
				bc_close(l_di);
				return 0;
			}

			l1 = bc_findnext(&blk);

		} while (l1 == 0);

		bc_close(l_di);

		/* rewrite GAMES.NAM */
		l_di = bc__creat((RealPt)ds_readd(FNAMES + 0x33c), 0);
		bc__write(l_di, RealMake(datseg, SAVEGAME_NAMES), 45);
		bc_close(l_di);

		return 1;
	}

	return 0;
}

/**
 * \brief   reads a CHR file from TEMP-dir
 *
 * \param   fname       filename
 * \param   hero_pos    position of the hero
 * \param   a2          ???
 * \return              1 = OK, 0 = Error
 */
signed short read_chr_temp(RealPt fname, signed short hero_pos, signed short a2)
{
	signed short handle;
	signed short hero_size = SIZEOF_HERO;
	Bit8u *hero;

	sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
		(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),
		(char*)Real2Host(fname));

	if ((handle = bc_open((RealPt)ds_readd(TEXT_OUTPUT_BUF), 0x8004)) == -1) {
		copy_file_to_temp(fname, (RealPt)ds_readd(TEXT_OUTPUT_BUF));
		handle = bc_open((RealPt)ds_readd(TEXT_OUTPUT_BUF), 0x8004);
	}

	if (handle != -1) {

		hero = get_hero(hero_pos);
		bc__read(handle, hero, hero_size);
		bc_close(handle);

		host_writeb(hero + HERO_GROUP_NO, (signed char)a2);

		if (host_readbs(hero + HERO_SEX) == 1) {
			host_writeb(hero + HERO_SPRITE_NO, host_readbs(hero + HERO_TYPE) + 11);

			if (host_readbs(hero + HERO_SPRITE_NO) > 21) {
				host_writeb(hero + HERO_SPRITE_NO, 21);
			}
		} else {
			host_writeb(hero + HERO_SPRITE_NO, host_readbs(hero + HERO_TYPE));

			if (host_readbs(hero + HERO_SPRITE_NO) > 10) {
				host_writeb(hero + HERO_SPRITE_NO, 10);
			}
		}

		if (!host_readbs(hero + HERO_START_GEAR)) {

			startup_equipment(hero);
			host_writeb(get_hero(hero_pos) + HERO_START_GEAR, 1);

			write_chr_temp(hero_pos);
		}

	} else {
		GUI_output(get_ttx(4));
		return 0;
	}

	return 1;

}

/**
 * \brief   writes a CHR file to temp
 *
 * \param   hero_pos    position of the hero
 */
void write_chr_temp(unsigned short hero_pos)
{
	char fname[20];
	unsigned short fd;

	prepare_chr_name(fname, (char*)get_hero(hero_pos));

	sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
		(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),		/* "TEMP\\%s" */
		fname);

	fd = bc__creat((RealPt)ds_readd(TEXT_OUTPUT_BUF), 0);
	bc__write(fd, (RealPt)ds_readd(HEROS) + SIZEOF_HERO * hero_pos, SIZEOF_HERO);
	bc_close(fd);
}

/**
 * \brief   copy the names from CHR file
 *
 * \param   ptr         pointer
 * \param   temple_id   > 0 the id of the temple, -1 on delete mode
 * \return              # of CHR-files in TEMP-dir
 */
signed short copy_chr_names(Bit8u *ptr, signed short temple_id)
{
	signed short count = 0;
	signed short l_di;
	signed short handle;
	Bit8u *buf;
	struct ffblk blk;

	buf = Real2Host(ds_readd(RENDERBUF_PTR)) + 60000;
	sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
		(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),
		(char*)p_datseg + ALL_CHR_WILDCARD3);

	l_di = bc_findfirst((RealPt)ds_readd(TEXT_OUTPUT_BUF), &blk, 0);

	if (!l_di) {

		do {
			/* create the CHR filename */
			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)Real2Host(ds_readd(STR_TEMP_XX_PTR2)),
				((char*)(&blk)) + 30);

			/* read the CHR file from temp */
			handle = bc_open((RealPt)ds_readd(TEXT_OUTPUT_BUF), 0x8004);
			bc__read(handle, buf, SIZEOF_HERO);
			bc_close(handle);

			if ((host_readbs(buf + 0x88) == temple_id && !host_readbs(buf + 0x8a)) ||
				(!host_readbs(buf + 0x8a) && temple_id == -1))
			{
				strcpy((char*)ptr + 32 * count, (char*)buf);
				strcpy((char*)ptr + 32 * count + 16, (char*)buf + 16);
				count++;
			}

			l_di = bc_findnext(&blk);

		} while (!l_di);

		return count;
	} else {
		return 0;
	}
}

/**
 * \brief   loads a head icon from IN_HEADS.NVF
 *
 * \param   head        index of the desired head
 */
void load_in_head(signed short head)
{
	signed short handle;

	if (head >= 0) {

		handle = load_archive_file(ARCHIVE_FILE_IN_HEADS_NVF);

		seek_archive_file(handle, 1024L * head, 0);

		read_archive_file(handle, Real2Host(ds_readd(DTP2)), 1024);

		bc_close(handle);

		ds_writew(LOADED_HEAD_ID, head);
	}
}

/**
 * \brief   load a temple icon
 *
 * \param   no          the number of the icon
 */
void load_tempicon(signed short no)
{
	struct nvf_desc nvf;
	signed short handle;

	if (no == 14) {
		no = 7;
	}

	/* load TEMPICON */
	handle = load_archive_file(ARCHIVE_FILE_TEMPICON);
	read_archive_file(handle, Real2Host(ds_readd(BUFFER8_PTR)), 7000);
	bc_close(handle);

	nvf.dst = Real2Host(ds_readd(BUFFER8_PTR)) + 7000;
	nvf.src = Real2Host(ds_readd(BUFFER8_PTR));
	nvf.no = no;
	nvf.type = 0;
	nvf.width = (Bit8u*)&handle;
	nvf.height = (Bit8u*)&handle;

	process_nvf(&nvf);
}

#if !defined(__BORLANDC__)
}
#endif
