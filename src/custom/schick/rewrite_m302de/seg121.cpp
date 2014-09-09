/**
 *	Rewrite of DSA1 v3.02_de functions of seg121 (poison effect)
 *	Functions rewritten: 1/1
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg121.cpp
 */

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void poison_effect(void)
{
	signed short i;
	volatile signed short j;
	Bit8u *hero;
	Bit8u *poison_ptr;

	ds_writew(CHECK_POISON, 0);

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero+=0x6da) {

		if ((host_readbs(hero + 0x21) != 0) && !hero_dead(hero)) {

			/* SHURINKNOLLENGIFT (used) */
			poison_ptr = hero + 0xdb;

			/* SHURINKNOLLENGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if (host_readbs(poison_ptr + 1) >= 6) {

					/* KK */
					if (host_readbs(hero + 0x47) != 0) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x8f0),
							(char*)hero + 0x10);

						GUI_output(Real2Host(ds_readd(DTP2)));

						inc_ptr_bs(poison_ptr + 2);
						dec_ptr_bs(hero + 0x47);
					}

					/* GE */
					if (host_readbs(hero + 0x41) != 0) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0xb4),
							(char*)hero + 0x10);

						GUI_output(Real2Host(ds_readd(DTP2)));

						inc_ptr_bs(poison_ptr + 3);
						dec_ptr_bs(hero + 0x41);
					}

					if (host_readbs(hero + 0x47) < 0 || host_readbs(hero + 0x41) < 0) {
						sub_hero_le(hero, 1000);
					}
				}

				/* Loose 1W6 LE */
				sub_hero_le(hero, random_schick(6));
			}

			/* SHURINKNOLLENGIFT: hero regenerates */
			if (host_readbs(poison_ptr) == 1) {

				if (!host_readbs(poison_ptr + 2) && !host_readbs(poison_ptr + 3)) {

					host_writeb(poison_ptr + 1, 0);
					host_writeb(poison_ptr, 0);
				}

				if ((host_readbs(poison_ptr + 2) != 0) && (!host_readbs(poison_ptr + 1) % 12 )) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x8f4),
							(char*)hero + 0x10);

						GUI_output(Real2Host(ds_readd(DTP2)));

						dec_ptr_bs(poison_ptr + 2);
						inc_ptr_bs(hero + 0x47);
				}

				if ((host_readbs(poison_ptr + 3) != 0) && (!host_readbs(poison_ptr + 1) % 12 )) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x908),
							(char*)hero + 0x10);

						GUI_output(Real2Host(ds_readd(DTP2)));

						dec_ptr_bs(poison_ptr + 3);
						inc_ptr_bs(hero + 0x41);
				}
			}


			/* ARAXGIFT (used) */
			poison_ptr = hero + 0xe0;

			/* ARAXGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if ((host_readbs(poison_ptr + 1) >= 2) && !(host_readbs(poison_ptr + 4))) {

					host_writeb(poison_ptr + 4, 1);

					for (j = 0; j < 7; j++) {
						/* all AT - 2 */
						sub_ptr_bs(hero + 0x68 + j, 2);
						/* all PA - 2 */
						sub_ptr_bs(hero + 0x6f + j, 2);
					}

					/* GE - 2 */
					host_writebs(hero + 0x41, host_readbs(hero + 0x41) - 2);
					/* KK - 2 */
					host_writebs(hero + 0x47, host_readbs(hero + 0x47) - 2);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x94),
						(char*)hero + 0x10);

					GUI_output(Real2Host(ds_readd(DTP2)));

				}

				/* enable self-regeneration */
				if (host_readbs(poison_ptr + 1) >= 120) {
					host_writebs(poison_ptr, 1);
				}
			}

			/* ARAXGIFT: regeneration */
			if ((host_readbs(poison_ptr) == 1) && (host_readbs(poison_ptr + 4))) {

				host_writeb(poison_ptr, 0);
				host_writeb(poison_ptr + 1, 0);
				host_writeb(poison_ptr + 4, 0);

				for (j = 0; j < 7; j++) {
					/* all AT + 2 */
					add_ptr_bs(hero + 0x68 + j, 2);
					/* all PA + 2 */
					add_ptr_bs(hero + 0x6f + j, 2);
				}

				/* GE + 2 */
				host_writebs(hero + 0x41, host_readbs(hero + 0x41) + 2);
				/* KK + 2 */
				host_writebs(hero + 0x47, host_readbs(hero + 0x47) + 2);

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ltx(0x98),
					(char*)hero + 0x10);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}


			/* ANGSTGIFT */
			poison_ptr = hero + 0xe5;

			/* ANGSTGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if(!host_readbs(poison_ptr + 4)) {

					host_writeb(poison_ptr + 4, 1);
					host_writeb(hero + 0x35, host_readbs(hero + 0x35) - 2);

					host_writeb(hero + 0x4a, host_readbs(hero + 0x4a) + 2);
					host_writeb(hero + 0x4d, host_readbs(hero + 0x4d) + 2);
					host_writeb(hero + 0x50, host_readbs(hero + 0x50) + 2);
					host_writeb(hero + 0x53, host_readbs(hero + 0x53) + 2);
					host_writeb(hero + 0x56, host_readbs(hero + 0x56) + 2);
					host_writeb(hero + 0x59, host_readbs(hero + 0x59) + 2);
					host_writeb(hero + 0x5c, host_readbs(hero + 0x5c) + 2);
					or_ptr_bs(hero + 0xab, 1);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x9c),
						(char*)hero + 0x10);

					GUI_output(Real2Host(ds_readd(DTP2)));
				}

				/* self regeneration */
				if (host_readbs(poison_ptr + 1) >= 7) {
					host_writeb(poison_ptr, 1);
				}
			}

			/* ANGSTGIFT: regeneration */
			if ((host_readbs(poison_ptr) == 1) && host_readbs(poison_ptr + 4)) {

					host_writeb(poison_ptr, 0);
					host_writeb(poison_ptr + 1, 0);
					host_writeb(poison_ptr + 4, 0);

					host_writeb(hero + 0x35, host_readbs(hero + 0x35) + 2);

					host_writeb(hero + 0x4a, host_readbs(hero + 0x4a) - 2);
					host_writeb(hero + 0x4d, host_readbs(hero + 0x4d) - 2);
					host_writeb(hero + 0x50, host_readbs(hero + 0x50) - 2);
					host_writeb(hero + 0x53, host_readbs(hero + 0x53) - 2);
					host_writeb(hero + 0x56, host_readbs(hero + 0x56) - 2);
					host_writeb(hero + 0x59, host_readbs(hero + 0x59) - 2);
					host_writeb(hero + 0x5c, host_readbs(hero + 0x5c) - 2);
					and_ptr_bs(hero + 0xab, 0xfe);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0xa0),
						(char*)hero + 0x10);

					GUI_output(Real2Host(ds_readd(DTP2)));
			}


			/* SCHLAFGIFT */
			poison_ptr = hero + 0xea;

			/* SCHLAFGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if (!host_readbs(poison_ptr + 4)) {

					host_writeb(poison_ptr + 4, 1);
					or_ptr_bs(hero + 0xaa, 0x02);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0xa4),
						(char*)hero + 0x10);

					GUI_output(Real2Host(ds_readd(DTP2)));
				}

				/* self regeneration */
				if (host_readbs(poison_ptr + 1) >= 20) {
					host_writeb(poison_ptr, 1);
				}
			}

			/* SCHLAFGIFT: regeneration */
			if ((host_readbs(poison_ptr) == 1) && (host_readbs(poison_ptr + 4) != 0)) {

				host_writeb(poison_ptr, 0);
				host_writeb(poison_ptr + 1, 0);
				host_writeb(poison_ptr + 4, 0);
				and_ptr_bs(hero + 0xaa, 0xfd);

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ltx(0xa8),
					(char*)hero + 0x10);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}


			/* GOLDLEIM */
			poison_ptr = hero + 0xef;

			/* GOLDLEIM: hero gets poisoned */
			if (host_readbs(poison_ptr) == -1) {
				if (host_readbs(poison_ptr + 1) >= dice_roll(2, 6, 0) * 12) {
					host_writebs(poison_ptr, 0);
				}

				if (!(host_readbs(poison_ptr + 1) % 12)) {
					sub_hero_le(hero, dice_roll(2, 6, -3));
				}
			}

			/* GOLDLEIM: regeneration */
			if (host_readbs(poison_ptr) == 1) {
				host_writeb(poison_ptr, 0);
			}


			/* KROETENSCHEMEL */
			poison_ptr = hero + 0xf4;

			/* KROETENSCHEMEL: hero gets poisoned */
			if (host_readbs(poison_ptr) == -1) {
				if (host_readbs(poison_ptr + 1) >= 48) {
					host_writebs(poison_ptr, 0);
				}

				if (!(host_readbs(poison_ptr + 1) % 12)) {
					sub_hero_le(hero, dice_roll(1, 6, 2));
				}
			}

			/* KROETENSCHEMEL: regeneration */
			if (host_readbs(poison_ptr) == 1) {
				host_writeb(poison_ptr, 0);
			}


			/* LOTUSGIFT */
			poison_ptr = hero + 0xf9;

			/* LOTUSGIFT: hero gets poisoned */
			if (host_readbs(poison_ptr) == -1) {
				if (host_readbs(poison_ptr + 1) >= 24) {
					host_writebs(poison_ptr, 0);
				}

				sub_hero_le(hero, dice_roll(2, 6, 0));
			}

			/* LOTUSGIFT: regeneration */
			if (host_readbs(poison_ptr) == 1) {
				host_writeb(poison_ptr, 0);
			}


			/* KUKRIS (used) */
			poison_ptr = hero + 0xfe;

			/* KUKRIS: hero gets poisoned */
			if (host_readbs(poison_ptr) == -1) {
				if (host_readbs(poison_ptr + 1) >= 3) {
					host_writebs(poison_ptr, 0);
				}

				sub_hero_le(hero, dice_roll(50, 6, 0));
			}

			/* KUKRIS: regeneration */
			if (host_readbs(poison_ptr) == 1) {
				host_writeb(poison_ptr, 0);
			}


			/* BANNSTAUB */
			poison_ptr = hero + 0x103;

			/* BANNSTAUB: hero gets poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if (host_readbs(poison_ptr + 1) >= dice_roll(3, 6, 0)) {
					host_writebs(poison_ptr, 1);
				}

				j = dice_roll(1, 6, 2);

				if (host_readws(hero + 0x64) >= j) {

					/* loose 1W6+2 AEmax */
					host_writeb(poison_ptr + 2, host_readbs(poison_ptr + 2) + j);

					sub_ptr_ws(hero + 0x64, j);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0xac),
						(char*)hero + 0x10, j);

					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			}

			/* BANNSTAUB: regeneration */
			if ((host_readbs(poison_ptr) == 1)) {
				if (!host_readbs(poison_ptr + 2)) {
					host_writeb(poison_ptr, 0);
				} else {

					if (!host_readbs(poison_ptr +1) % 12) {
						/* regenerate one point at a time */
						dec_ptr_bs(poison_ptr + 2);
						inc_ptr_ws(hero + 0x64);

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0xb0),
							(char*)hero + 0x10);

						GUI_output(Real2Host(ds_readd(DTP2)));
					}
				}
			}


			for (j = 1; j <= 9; j++) {
				if (host_readbs(hero + 0xd6 + j * 5) != 0) {
					inc_ptr_bs(hero + 0xd7 + j * 5);
				}
			}

			/* KK <= 0 */
			if (host_readbs(hero + 0x47) <= 0) {
				/* reset KK */
				host_writeb(hero + 0x47, host_readbs(hero + 0x46));
				/* die */
				sub_hero_le(hero, 5000);
			}

			/* GE <= 0 */
			if (host_readbs(hero + 0x41) <= 0) {
				/* reset GE */
				host_writeb(hero + 0x41, host_readbs(hero + 0x40));
				/* die */
				sub_hero_le(hero, 5000);
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
