/**
 *	Rewrite of DSA1 v3.02_de functions of seg120 (misc: rabies, game init, DOS-related)
 *	Functions rewritten: 10/11
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg001.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg010.h"
#include "seg026.h"
#include "seg028.h"
#include "seg029.h"
#include "seg047.h"
#include "seg049.h"
#include "seg063.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg104.h"
#include "seg120.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void rabies(RealPt hero, signed short hero_pos)
{
	signed short answer;
	signed short l_di;
	signed short done;
	signed short bak;
	Bit8u *hero2;
	signed short group_bak;
	signed short group_nr;
	signed char sex_bak;

	done = 0;

	group_bak = ds_readbs(CURRENT_GROUP);
	sex_bak = host_readbs(Real2Host(hero) + 0x22);
	group_nr = host_readbs(Real2Host(hero) + 0x87);

	/* TODO : Sex = 50, what means 50 ? */
	host_writeb(Real2Host(hero) + 0x22, 50);

	/* switch to the group of the hero */
	while (ds_readbs(CURRENT_GROUP) != group_nr) {
		GRP_switch_to_next(1);
	}

	hero_pos = 0;
	while (host_readbs(get_hero(hero_pos) + 0x22) != 50) {
		hero_pos++;
	}

	hero = (RealPt)ds_readd(HEROS) + 0x6da * hero_pos;
	host_writeb(Real2Host(hero) + 0x22, sex_bak);

	if (ds_readbs(0x2845) == 0) {
		draw_status_line();
	}

	while (done == 0) {

		if (count_heroes_available_in_group() > 1) {

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0xb94),
				(char*)Real2Host(hero) + 0x10,
				(char*)Real2Host(GUI_get_ptr(host_readbs(Real2Host(hero) + 0x22), 2)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(Real2Host(hero) + 0x22), 2)));

			sprintf((char*)Real2Host(ds_readd(DTP2)) + 500,
				(char*)get_ltx(0xb98),
				(char*)Real2Host(hero) + 0x10);

			sprintf((char*)Real2Host(ds_readd(DTP2)) + 600,
				(char*)get_ltx(0xb9c),
				(char*)Real2Host(hero) + 0x10);

			bak = ds_readws(TEXTBOX_WIDTH);
			ds_writew(TEXTBOX_WIDTH, 6);

			answer = GUI_dialogbox(hero + 0x2da,
						Real2Host(hero) + 0x10,
						Real2Host(ds_readd(DTP2)),
						3,
						Real2Host(ds_readd(DTP2)) + 500,
						Real2Host(ds_readd(DTP2)) + 600,
						get_ltx(0xba0));

			ds_writew(TEXTBOX_WIDTH, bak);

			if (answer == 1) {
				/* knock the infected hero out */

				sub_hero_le(Real2Host(hero), host_readws(Real2Host(hero) + 0x60) / 2);

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ltx(0xba4),
					(char*)Real2Host(hero) + 0x10);

				GUI_output(Real2Host(ds_readd(DTP2)));

				ds_writeb(0x64a2, (signed char)hero_pos);

				answer = select_hero_ok(get_ltx(0x62c));

				if (answer != -1) {

					talent_cure_disease(get_hero(answer), Real2Host(hero), 10, 1);
				}

				done = 1;

			} else if (answer == 2) {
				/* calm the hero */

				for (l_di = 0; l_di <= 6; l_di++) {

					/* one of the other heros must pass CH+0 */
					if ((l_di != hero_pos) &&
						(test_attrib(get_hero(l_di), 2, 0) != 0))
					{
						done = 1;
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0xba8),
							(char*)Real2Host(hero) + 0x10);

						GUI_output(Real2Host(ds_readd(DTP2)));

						ds_writeb(0x64a2, (signed char)hero_pos);

						answer = select_hero_ok(get_ltx(0x62c));

						if (answer != -1) {
							talent_cure_disease(get_hero(answer), Real2Host(hero), 10, 1);
						}
						break;
					}
				}
			} else if (answer == 3) {
				/* cast a spell */

				ds_writeb(0x64a2, (signed char)hero_pos);

				answer = select_hero_ok(get_ltx(0x354));

				if (answer != -1) {

					hero2 = get_hero(answer);

					/* check that hero2 is a magic user */
					if (host_readbs(hero2 + 0x21) >= 7) {

						/* need 15 AE */
						if (host_readws(hero2 + 0x64) >= 15) {

							/* spell must succeed */
							if (test_spell(hero2, 15, 0)) {

								done = 1;

								sub_ae_splash(hero2, 15);

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_ltx(0xba8),
									(char*)Real2Host(hero) + 0x10);

								GUI_output(Real2Host(ds_readd(DTP2)));

								ds_writeb(0x64a2, (signed char)hero_pos);

								answer = select_hero_ok(get_ltx(0x62c));

								if ((answer != -1) && (answer != hero_pos)) {
									talent_cure_disease(get_hero(answer), Real2Host(hero), 10, 1);
								}
							}
						} else {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ltx(0x97c),
								(char*)hero2 + 0x10);

							GUI_output(Real2Host(ds_readd(DTP2)));
						}
					}
				}
			}
		} else {

			/* Hero has rabies / Tollwut */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0xbac),
				(char*)Real2Host(hero) + 0x10);

			GUI_output(Real2Host(ds_readd(DTP2)));

			done = 1;
		}

		if (done == 0) {
			/* every other hero in the group looses 1W6+2 LE */
			hero2 = get_hero(0);
			for (l_di = 0; l_di <= 6; l_di++, hero2 += 0x6da) {

				if ((l_di != hero_pos) &&
					(host_readbs(hero2 + 0x21) != 0) &&
					(host_readbs(hero2 + 0x87) == ds_readbs(CURRENT_GROUP)) &&
					!hero_dead(hero2))
				{
					sub_hero_le(hero2, dice_roll(1, 6, 2));
				}
			}

			/* hero has berserker fury / Berserkerwut */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0xc5c),
				(char*)Real2Host(hero) + 0x10);

			GUI_output(Real2Host(ds_readd(DTP2)));

			done = 1;
		}
	}

	/* switch back to the group */
	while (ds_readbs(CURRENT_GROUP) != group_bak) {
		GRP_switch_to_next(1);
	}

	if (ds_readbs(0x2845) == 0) {
		draw_status_line();
	}
}

/* Borlandified and identical */
void init_global_buffer(void)
{

	ds_writed(0xe5e0, (Bit32u)schick_alloc_emu(ds_readd(BUFFERSIZE)));
	ds_writed(0xd303, (Bit32u)F_PADD(ds_readd(0xe5e0), 8));
	ds_writed(0xd2b9, (Bit32u)F_PADD(ds_readd(0xd303), 65000));

	ds_writed(TEXT_LTX, (Bit32u)F_PADD(ds_readd(0xd2b9), 30500));
	ds_writed(DIALOG_TEXT, (Bit32u)((RealPt)ds_readd(TEXT_LTX) + 3360));
	ds_writed(CITY_LTX, (Bit32u)((RealPt)ds_readd(TEXT_LTX) + 3960));

	ds_writed(0xd2e3, (Bit32u)(F_PADD(ds_readd(TEXT_LTX), 4760)));
	ds_writed(DTP2, (Bit32u)(F_PADD(ds_readd(0xd2e3), 3400)));
	ds_writed(0xd2ef, (Bit32u)((RealPt)ds_readd(DTP2) + 1500));
	ds_writed(0xd2eb, (Bit32u)(F_PADD(ds_readd(DTP2), 1524)));
	ds_writed(0xd2a5, (Bit32u)(F_PADD(ds_readd(0xd2eb), 300)));
	ds_writed(0xd2b1, (Bit32u)(F_PADD(ds_readd(0xd2a5), 3880)));
	ds_writed(0xd2b5, (Bit32u)(F_PADD(ds_readd(0xd2b1), 2200)));
	ds_writed(0xc3a9, (Bit32u)(F_PADD(ds_readd(0xd2b5), 10000)));
	ds_writed(0xc3db, (Bit32u)(F_PADD(ds_readd(0xd2b5), 22008)));

	ds_writed(0xd019, ds_writed(0xd015, ds_readd(0xc3db)));
	ds_writed(0xce43, 0);

}

/* Borlandified and identical */
signed short init_memory(void)
{
	signed short error = 0;
	Bit32u freemem;

	/* disable EMS memory */
	ds_writeb(EMS_ENABLED, 0);

	if (EMS_init()) {
		bc_OvrInitEms(0, 0, 0);
	} else {
		;
	}

	/* set the pointer for the framebuffer */
	ds_writed(0xd2fb, ds_writed(0xd2ff, (Bit32u)RealMake(0xa000, 0x0000)));

	/* allocate small chunks of memory */
	ds_writed(ITEMSNAME,		(Bit32u)schick_alloc_emu(1016));
	ds_writed(ITEMSDAT,		(Bit32u)schick_alloc_emu(3060));
	ds_writed(0xd2a1,		(Bit32u)schick_alloc_emu(950));
	ds_writed(MONNAMES,		(Bit32u)schick_alloc_emu(308));
	ds_writed(0xe121,		(Bit32u)schick_alloc_emu(296));
	ds_writed(MEM_SLOTS_MFIG,	(Bit32u)schick_alloc_emu(516));
	ds_writed(MEM_SLOTS_WFIG,	(Bit32u)schick_alloc_emu(516));
	ds_writed(MEM_SLOTS_MON,		(Bit32u)schick_alloc_emu(432));
	ds_writed(HEROS,		(Bit32u)schick_alloc_emu(7 * 0x6da));
	ds_writed(0xe494,		(Bit32u)schick_alloc_emu(630));
	ds_writed(0xe49c,		(Bit32u)schick_alloc_emu(225));
	ds_writed(0xe498,		(Bit32u)schick_alloc_emu(80));
	ds_writed(BUF_FONT6,		(Bit32u)schick_alloc_emu(592));
	ds_writed(0xd2bd,		(Bit32u)schick_alloc_emu(1000));
	ds_writed(0xd299,		(Bit32u)schick_alloc_emu(500));
	ds_writed(CHESSBOARD,		(Bit32u)schick_alloc_emu(625));
	ds_writed(POPUP,		(Bit32u)(schick_alloc_emu(1673) + 8));
	ds_writed(ICON,			(Bit32u)(schick_alloc_emu(1500) + 8));
	ds_writed(BUF_ICON,		(Bit32u)schick_alloc_emu(5184));
	ds_writed(TOWNPAL_BUF,		(Bit32u)schick_alloc_emu(288));

	freemem = bc_farcoreleft();

	if (freemem > 334000) {

		if (freemem >= 357000) {
			ds_writed(BUFFERSIZE, 357000);
			ds_writeb(LARGE_BUF, 1);
		} else {
			ds_writed(BUFFERSIZE, 334000);
			ds_writeb(LARGE_BUF, 0);
		}

		init_global_buffer();

	} else {

		printf((char*)p_datseg + 0xb317, 329000 - freemem);

		wait_for_keypress();
		error = 1;
	}

	if (!error) {

		init_text();

		ds_writed(0xd2df, (Bit32u)F_PADD(ds_readd(0xd019), 180000));
		if (ds_readb(LARGE_BUF) == 1) {
			add_ds_ws(0xd2df, 23000);
		}

		ds_writed(0xd2db, (Bit32u)F_PADD(ds_readd(0xd2df), -20000));
		ds_writed(0xd2a9, (Bit32u)F_PADD(ds_readd(0xd2db), -16771));
	}

	return error;
}

/* Borlandified and identical */
void refresh_colors(void)
{
	set_color(p_datseg + 0xb22d, 0);
	set_color(p_datseg + 0xb22a, 0xff);
	set_palette(p_datseg + 0xb230, 0xd8, 8);

	set_palette(p_datseg + 0xb248, 0xc8, 3);
	set_palette(p_datseg + 0xb251, 0x40, 0x20);
	set_palette(p_datseg + 0xb2b1, 0x20, 0x20);
	set_palette(p_datseg + 0x2723, 0x60, 0x20);

}


/* Borlandified and nearly identical */
Bit32s get_diskspace(void)
{
	unsigned short a[4];
	Bit32s space;

#if !defined(__BORLANDC__)
	bc_dos_getdiskfree(bc_getdisk() + 1, (Bit8u*)&a);
	space = (Bit32s)host_readws((Bit8u*)&a[0])
			* (Bit32s)host_readws((Bit8u*)&a[2])
			* (Bit32s)host_readws((Bit8u*)&a[3]);
#else
	/* BC-TODO: here the return value of get_disk() produces other code */
	bc_dos_getdiskfree(((signed char)bc_getdisk()) + 1, (diskfree_t*)&a);
	space = (Bit32s)a[0] * (Bit32s)a[2] * (Bit32s)a[3];
#endif


	return space - 204800;

}

/* Borlandified and identical */
void init_game_state(void)
{
	signed short i;

	refresh_colors();

	ds_writew(0xd2cb, 0xc8);
	ds_writew(0xd2cd, 0xc9);
	ds_writew(0xd2cf, 0xca);

	set_to_ff();

	for (i = 0; i < 0xfe; i++) {
		ds_writeb(0xe12d + i, 0);
	}

	ds_writeb(0x2d9f, 0);
	/* Travia Temple in Thorwal */
	ds_writeb(LOCATION, 2);
	ds_writew(TYPEINDEX, 1);
	ds_writew(0x2d83, 9);
	ds_writew(0x2d85, 9);
	ds_writew(X_TARGET, 9);
	ds_writew(Y_TARGET, 8);
	ds_writeb(0x2d7c, 0);
	ds_writeb(DIRECTION, 0);
	ds_writeb(DUNGEON_INDEX, 0);

	ds_writeb(0x2da6, ds_writeb(CURRENT_TOWN, 1));

	ds_writew(TEXTBOX_WIDTH, 3);

	/* timer */
	/* ds_writed(DAY_TIMER, 24 * 0x1518 - 1);*/
	ds_writed(DAY_TIMER, 0x1fa3f);
	timewarp_until(1);
	/* ds_writed(DAY_TIMER,  8 * 0x1518); */
	ds_writed(DAY_TIMER,  0xa8c0);
	ds_writeb(DAY_OF_WEEK, 4);
	ds_writeb(DAY_OF_MONTH, 17);
	ds_writeb(MONTH, 1);
	ds_writeb(YEAR, 15);

	ds_writed(0xc00d, ds_readd(0xd2ff));

	load_objects_nvf();
	passages_init();

	ds_writew(CURRENT_ANI, -1);
	ds_writew(0xe113, 1);

	ds_writed(0xbff9, ds_readd(0xd303));
	load_splashes();
}

/* Borlandified and identical */
int err_handler(void)
{
	bc_hardresume(3);
	return 1;
}

/* Borlandified and identical */
void prepare_dirs(void)
{
	signed short l_si;
	signed short l_di;
	signed short drive;
	signed short drive_bak;
	signed short errorval;
	struct ffblk blk;
	char gamepath[40];

#if !defined(__BORLANDC__)
	bc_harderr(RealMake(reloc_game + 0x3c0, 0x48));
#else
	/* TODO: only the adress differs, should be the stub adress */
	bc_harderr((int(*)(int, int, int, int))err_handler);
#endif

	drive_bak = drive = bc_getdisk();

	errorval = 0;
	while (errorval != 2) {

		bc_setdisk(drive);

		/* copy "X:\\" */
		strcpy(gamepath, (char*)p_datseg + 0xb393);

		/* set drive name in path */
		gamepath[0] = drive + 'A';

		if (drive_bak == drive) {
			bc_getcurdir(0, &gamepath[3]);
		} else {
			gamepath[2] = '\0';
		}

		strcpy((char*)Real2Host(ds_readd(0xd2eb)), gamepath);
		/* "\\TEMP" */
		strcat((char*)Real2Host(ds_readd(0xd2eb)), (char*)p_datseg + 0xb311);

		if (!bc_chdir((char*)Real2Host(ds_readd(0xd2eb)))) {
			/*	check if it's possible to change to TEMP-dir: OK
				change to gamepath */

			bc_chdir(gamepath);
			errorval = 2;

		} else {

			if (bc_mkdir((char*)Real2Host(ds_readd(0xd2eb)))) {
				errorval = 1;
			} else {
				errorval = 2;
			}
		}

		if (errorval == 1) {

			/* ERROR, cant write => exit */

			GUI_output(p_datseg + 0xb397);

			cleanup_game();

			bc_exit(0);
		}
	}

	/* delete *.* in TEMP-dir */
	sprintf((char*)Real2Host(ds_readd(0xd2eb)),
		(char*)Real2Host(ds_readd(0x4c88)),
		(char*)p_datseg + 0xb4af);

	l_si = bc_findfirst((RealPt)ds_readd(0xd2eb), &blk, 0);

	if (!l_si) {

		do {
			sprintf((char*)Real2Host(ds_readd(0xd2eb)),
				(char*)Real2Host(ds_readd(0x4c88)),
				((char*)(&blk)) + 30);			/* contains a filename */

			bc_unlink((RealPt)ds_readd(0xd2eb));

			l_si = bc_findnext(&blk);

		} while (!l_si);
	}

	/* search for "*.CHR" in the gamepath */
	l_si = bc_findfirst((RealPt)RealMake(datseg, 0xb4b3), &blk, 0);

	while (!l_si) {

		/* open CHR-file and copy it into TEMP-dir */
#if !defined(__BORLANDC__)
		/* need a RealPt => create one on the stack */
		char *str = (char*)Real2Host(RealMake(SegValue(ss), reg_esp - 100));

		strcpy(str, ((char*)(&blk)) + 30);

		l_di = bc__open(RealMake(SegValue(ss), reg_esp - 100), 0x8004);

#else
		l_di = bc__open(((char*)&blk) + 30, 0x8004);
#endif

		bc__read(l_di, Real2Host(ds_readd(0xd303)), 0x6da);

		bc_close(l_di);

		sprintf((char*)Real2Host(ds_readd(0xd2eb)),
			(char*)Real2Host(ds_readd(0x4c88)),
			((char*)(&blk)) + 30);			/* contains a filename */

		l_di = bc__creat((RealPt)ds_readd(0xd2eb), 0);

		bc__write(l_di, (RealPt)ds_readd(0xd303), 0x6da);

		bc_close(l_di);

		l_si = bc_findnext(&blk);
	}

	bc_setdisk(drive_bak);
}

/* Borlandified and identical */
void cleanup_game(void)
{
	struct ffblk blk;
	signed short l_si;
	signed short l_di;

	/* disable AIL */
	exit_AIL();

	/* disable CD-Audio */
	if (ds_readw(0x95) != 0) {
		CD_audio_stop();
	}

	/* free EMS memory */
	if (ds_readb(EMS_ENABLED) != 0) {

		for (l_si = 0; l_si < 37; l_si++) {
			if (host_readw(Real2Host(ds_readd(0xe121)) + l_si * 8) != 0) {
				EMS_free_pages(host_readw(Real2Host(ds_readd(0xe121)) + 2 + l_si * 8));
			}
		}

		/* free male and female figures */
		for (l_si = 0; l_si < 43; l_si++) {

			if ((host_readw(Real2Host(ds_readd(MEM_SLOTS_MFIG)) + l_si * 12) != 0) &&
				((host_readw(Real2Host(ds_readd(MEM_SLOTS_MFIG)) + l_si * 12 + 6) != 0)))
			{
				EMS_free_pages(host_readw(Real2Host(ds_readd(MEM_SLOTS_MFIG)) + 6 + l_si * 12));
			}

			if ((host_readw(Real2Host(ds_readd(MEM_SLOTS_WFIG)) + l_si * 12) != 0) &&
				((host_readw(Real2Host(ds_readd(MEM_SLOTS_WFIG)) + l_si * 12 + 6) != 0)))
			{
				EMS_free_pages(host_readw(Real2Host(ds_readd(MEM_SLOTS_WFIG)) + 6 + l_si * 12));
			}
		}

		/* free monster figures */
		for (l_si = 0; l_si < 36; l_si++) {

			if ((host_readw(Real2Host(ds_readd(MEM_SLOTS_MON)) + l_si * 12) != 0) &&
				((host_readw(Real2Host(ds_readd(MEM_SLOTS_MON)) + l_si * 12 + 6) != 0)))
			{
				EMS_free_pages(host_readw(Real2Host(ds_readd(MEM_SLOTS_MON)) + 6 + l_si * 12));
			}
		}

		if (ds_readw(0xbd92) != 0) {
			EMS_free_pages(ds_readw(0xbd92));
		}

		/* free map memory */
		if (ds_readw(0xbd90) != 0) {
			EMS_free_pages(ds_readw(0xbd90));
		}
	}

	/* delete all files in TEMP */

	sprintf((char*)Real2Host(ds_readd(0xd2eb)),
		(char*)Real2Host(ds_readd(0x4c88)),	/* contains "TEMP\\%s" */
		(char*)p_datseg + 0xb4b9);		/* contains "*.*" */

	l_di = bc_findfirst((RealPt)ds_readd(0xd2eb), &blk, 0);

	if (l_di == 0) {
		do {
			/* delete each found file */
			sprintf((char*)Real2Host(ds_readd(0xd2eb)),
				(char*)Real2Host(ds_readd(0x4c88)),	/* contains "TEMP\\%s" */
				((char*)(&blk)) + 30);			/* contains a filename */

			bc_unlink((RealPt)ds_readd(0xd2eb));

			l_di = bc_findnext(&blk);
		} while (!l_di);
	}

	/* misc cleanups */
	update_mouse_cursor();
	disable_mouse();
	reset_timer();
	schick_reset_video();
	bc_clrscr();
}

/**
 * \brief	shows the game over screen
 */
/* Borlandified and identical */
void game_over_screen(void)
{
	signed short handle;

	set_var_to_zero();

	set_audio_track(150);

	/* load SKULL.NVF */
	handle = load_archive_file(137);

	read_archive_file(handle, Real2Host(ds_readd(0xd303)), 64200);

	bc_close(handle);

	update_mouse_cursor();

	ds_writew(0xe113, 0);

	wait_for_vsync();

	set_palette(p_datseg + 0x26c3, 0x00, 0x20);
	set_palette(p_datseg + 0x26c3, 0x20, 0x20);

	bc_memcpy((RealPt)ds_readd(0xd2ff), (RealPt)ds_readd(0xd303), 64000);

	set_palette(Real2Host(ds_readd(0xd303)) + 64002, 0x00, 0x40);

	wait_for_keypress();

	bc_memset((RealPt)ds_readd(0xd2ff), 0, 64000);

	wait_for_vsync();

	set_palette(p_datseg + 0x26c3, 0x00, 0x20);
	set_palette(p_datseg + 0x26c3, 0x20, 0x20);

	refresh_screen_size();

	ds_writeb(0x2845, -1);
}

void call_gen(void)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif
