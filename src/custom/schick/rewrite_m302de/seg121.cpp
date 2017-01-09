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

	for (i = 0; i <= 6; i++, hero+=SIZEOF_HERO) {

		if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) && !hero_dead(hero)) {

			poison_ptr = hero + HERO_POISON + POISON_SHURINKNOLLENGIFT * 5;

			/* SHURINKNOLLENGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if (host_readbs(poison_ptr + 1) >= 6) {

					/* KK */
					if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) != 0) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(0x8f0),
							(char*)hero + HERO_NAME2);

						GUI_output(Real2Host(ds_readd(DTP2)));

						inc_ptr_bs(poison_ptr + 2);
						dec_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK));
					}

					/* GE */
					if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) != 0) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(0xb4),
							(char*)hero + HERO_NAME2);

						GUI_output(Real2Host(ds_readd(DTP2)));

						inc_ptr_bs(poison_ptr + 3);
						dec_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE));
					}

					if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) < 0 || host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) < 0) {
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
							(char*)get_ttx(0x8f4),
							(char*)hero + HERO_NAME2);

						GUI_output(Real2Host(ds_readd(DTP2)));

						dec_ptr_bs(poison_ptr + 2);
						inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK));
				}

				if ((host_readbs(poison_ptr + 3) != 0) && (!host_readbs(poison_ptr + 1) % 12 )) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(0x908),
							(char*)hero + HERO_NAME2);

						GUI_output(Real2Host(ds_readd(DTP2)));

						dec_ptr_bs(poison_ptr + 3);
						inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE));
				}
			}


			poison_ptr = hero + (HERO_POISON + POISON_ARAX * 5);

			/* ARAXGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if ((host_readbs(poison_ptr + 1) >= 2) && !(host_readbs(poison_ptr + 4))) {

					host_writeb(poison_ptr + 4, 1);

					for (j = 0; j < 7; j++) {
						/* all AT - 2 */
						sub_ptr_bs(hero + HERO_AT + j, 2);
						/* all PA - 2 */
						sub_ptr_bs(hero + HERO_PA + j, 2);
					}

					/* GE - 2 */
					host_writebs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) - 2);
					/* KK - 2 */
					host_writebs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) - 2);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(0x94),
						(char*)hero + HERO_NAME2);

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
					add_ptr_bs(hero + HERO_AT + j, 2);
					/* all PA + 2 */
					add_ptr_bs(hero + HERO_PA + j, 2);
				}

				/* GE + 2 */
				host_writebs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) + 2);
				/* KK + 2 */
				host_writebs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + 2);

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(0x98),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}


			poison_ptr = hero + (HERO_POISON + POISON_ANGSTGIFT * 5);

			/* ANGSTGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if(!host_readbs(poison_ptr + 4)) {

					host_writeb(poison_ptr + 4, 1);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_MU), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_MU)) - 2);

					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_AG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_AG)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_HA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_HA)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_RA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_RA)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_GG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GG)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_TA)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_NG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_NG)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_JZ), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_JZ)) + 2);
					or_ptr_bs(hero + HERO_STATUS2, 1);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(0x9c),
						(char*)hero + HERO_NAME2);

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

					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_MU), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_MU)) + 2);

					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_AG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_AG)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_HA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_HA)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_RA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_RA)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_GG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GG)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_TA)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_NG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_NG)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_JZ), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_JZ)) - 2);
					and_ptr_bs(hero + HERO_STATUS2, 0xfe);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(0xa0),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));
			}


			poison_ptr = hero + (HERO_POISON + POISON_SCHLAFGIFT * 5);

			/* SCHLAFGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if (!host_readbs(poison_ptr + 4)) {

					host_writeb(poison_ptr + 4, 1);
					or_ptr_bs(hero + HERO_STATUS1, 0x02);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(0xa4),
						(char*)hero + HERO_NAME2);

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
				and_ptr_bs(hero + HERO_STATUS1, 0xfd);

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(0xa8),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}


			poison_ptr = hero + (HERO_POISON + POISON_GOLDLEIM * 5);

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


			poison_ptr = hero + (HERO_POISON + POISON_KROETENSCHEMEL * 5);

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


			poison_ptr = hero + (HERO_POISON + POISON_LOTUSGIFT * 5);

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


			poison_ptr = hero + (HERO_POISON + POISON_KUKRIS * 5);

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


			poison_ptr = hero + (HERO_POISON + POISON_BANNSTAUB * 5);

			/* BANNSTAUB: hero gets poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if (host_readbs(poison_ptr + 1) >= dice_roll(3, 6, 0)) {
					host_writebs(poison_ptr, 1);
				}

				j = dice_roll(1, 6, 2);

				if (host_readws(hero + HERO_AE) >= j) {

					/* loose 1W6+2 AEmax */
					host_writeb(poison_ptr + 2, host_readbs(poison_ptr + 2) + j);

					sub_ptr_ws(hero + HERO_AE, j);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(0xac),
						(char*)hero + HERO_NAME2, j);

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
						inc_ptr_ws(hero + HERO_AE);

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(0xb0),
							(char*)hero + HERO_NAME2);

						GUI_output(Real2Host(ds_readd(DTP2)));
					}
				}
			}


			for (j = 1; j <= 9; j++) {
				if (host_readbs(hero + (HERO_POISON-5) + j * 5) != 0) {
					inc_ptr_bs(hero + (HERO_POISON-4) + j * 5);
				}
			}

			/* KK <= 0 */
			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) <= 0) {
				/* reset KK */
				host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_KK)));
				/* die */
				sub_hero_le(hero, 5000);
			}

			/* GE <= 0 */
			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) <= 0) {
				/* reset GE */
				host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_GE)));
				/* die */
				sub_hero_le(hero, 5000);
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
