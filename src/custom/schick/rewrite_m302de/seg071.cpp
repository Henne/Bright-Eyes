/**
 *	Rewrite of DSA1 v3.02_de functions of seg071 (phexcaer: buildings 2/2)
 *	Functions rewritten: 3/3 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg071.cpp
 */

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg032.h"
#include "seg047.h"
#include "seg061.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void PHX_drogentempel(void)
{
	signed short answer;
	signed short nr_heros;
	Bit32s money;

	load_ani(3);

	init_ani(0);

	money = get_party_money();

	if ((ds_readds(DAY_TIMER) >= HOURS(15)) && (ds_readds(DAY_TIMER) <= HOURS(19))) {

		/* load the picture of the TEMPLE SERVANT */

		load_in_head(6);

		nr_heros = count_heroes_in_group();

		if (ds_readb(DRUG_TIMER) && (nr_heros * 2000 <= money)) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x3c), get_city(0x38), 0);

			money -= nr_heros * 2000;
			set_party_money(money);

		} else {

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x3c), get_city(0x00),
						3,
						get_city(0x04),
						get_city(0x08),
						get_city(0x0c));
			} while (answer == -1);

			if (answer == 1) {

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_city(0x3c), get_city(0x10),
							3,
							get_city(0x14),
							get_city(0x18),
							get_city(0x1c));
				} while (answer == -1);

				if (answer == 3) {
					GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_city(0x3c), get_city(0x20), 0);

					ds_writeb(DRUG_TIMER, 10);
				}

			} else if (answer == 2) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x3c), get_city(0x20), 0);

				ds_writeb(DRUG_TIMER, 10);
			} else {

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_city(0x3c), get_city(0x24),
							3,
							get_city(0x28),
							get_city(0x2c),
							get_city(0x30));
				} while (answer == -1);

				if (answer == 3) {

					GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x3c), get_city(0x34), 0);

				} else {

					GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x3c), get_city(0x20), 0);

					ds_writeb(DRUG_TIMER, 10);
				}
			}
		}
	} else {

		/* load the picture of EKTOR GREMOB */
		load_in_head(8);

		nr_heros = count_heroes_in_group();

		if (ds_readb(GREMOB_INVITED) != 0) {

			GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_city(0x40), get_city(0x74), 0);

			do_fight(FIGHTS_PHEX3);
		} else {

			if (ds_readb(DRUG_TIMER) && (nr_heros * 2500 <= money)) {

				GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x40), get_city(0x70), 0);

				money -= nr_heros * 2500;
				set_party_money(money);
			} else {

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_city(0x40), get_city(0x44),
							2,
							get_city(0x4c),
							get_city(0x78));
				} while (answer == -1);

				if (answer == 1) {

					do {
						answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_city(0x40), get_city(0x48),
							3,
							get_city(0x4c),
							get_city(0x50),
							get_city(0x54));
					} while (answer == -1);

					GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x40), get_city(0x68), 0);

					if (nr_heros * 2000 <= money) {

						if (GUI_bool(get_city(0x7c))) {
							money -= nr_heros * 2000;
							set_party_money(money);
						}
					}
				} else {
					do {
						answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_city(0x40), get_city(0x58),
							3,
							get_city(0x5c),
							get_city(0x60),
							get_city(0x64));
					} while (answer == -1);

					if (answer == 1) {
						GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x40), get_city(0x6c), 0);
						ds_writeb(GREMOB_INVITED, 1);
					} else {
						GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x40), get_city(0x20), 0);
						ds_writeb(DRUG_TIMER, 10);
					}
				}
			}
		}
	}

	set_var_to_zero();
}

/**
 * \brief the Phextemple of Phexcaer
 *
 * This function shows how to enter a temple manually.
 * Also there is some code, which will never be executed,
 * because PTEMPLE_CLOSED is initially 0 and never changes.
*/
void PHX_phextempel(void)
{
	signed short answer;		/* the selected answer from the dialogs */
	signed short old_tb_width;	/* saving the width of the textboxes */
	signed short old_loc;		/* saving the nr of the old location */

	old_tb_width = ds_readws(TEXTBOX_WIDTH);

	/* this variable is never != 0 */
	if (ds_readb(PTEMPLE_CLOSED) != 0) {

		/* the cleric is gone */
		GUI_output(get_city(0xf0));

	} else {

		/* load the picture of THE FEMALE CLERIC */

		load_in_head(12);

		/* show introduction text */
		GUI_output(get_city(0x84));

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_city(0x80), get_city(0x88),
					3,
					get_city(0x8c),
					get_city(0x90),
					get_city(0x94));
		} while (answer == -1);

		if (answer == 1) {

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x80), get_city(0x98),
						3,
						get_city(0x9c),
						get_city(0xa0),
						get_city(0xa4));
			} while (answer == -1);

			if (answer == 1) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x80), get_city(0xc8), 0);
			} else {

				/* enter the temple */

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x80), get_city(0xcc), 0);

				ds_writew(TYPEINDEX, 59);

				ds_writew(TEXTBOX_WIDTH, 3);

				old_loc = ds_readbs(LOCATION);

				ds_writebs(LOCATION, 2);

				do_temple();

				ds_writebs(LOCATION, (signed char)old_loc);

				ds_writew(TEXTBOX_WIDTH, old_tb_width);
			}

		} else if (answer == 2) {

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x80), get_city(0xa8),
						3,
						get_city(0xac),
						get_city(0xb0),
						get_city(0xb4));
			} while (answer == -1);

			if (answer == 1) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x80), get_city(0xc8), 0);

			} else if (answer == 2) {

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_city(0x80), get_city(0xd0),
							3,
							get_city(0xd4),
							get_city(0xd8),
							get_city(0xdc));
				} while (answer == -1);

				if (answer == 2) {

					GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x80), get_city(0xe0), 0);
				}

			} else {

				/* enter the temple */

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x80), get_city(0xcc), 0);

				ds_writew(TYPEINDEX, 59);

				ds_writew(TEXTBOX_WIDTH, 3);

				old_loc = ds_readbs(LOCATION);

				ds_writebs(LOCATION, 2);

				do_temple();

				ds_writebs(LOCATION, (signed char)old_loc);

				ds_writew(TEXTBOX_WIDTH, old_tb_width);
			}

		} else {

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x80), get_city(0xb8),
						3,
						get_city(0xbc),
						get_city(0xc0),
						get_city(0xc4));
			} while (answer == -1);

			if (answer == 1) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x80), get_city(0xe4), 0);
			} else if (answer == 2) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x80), get_city(0xe8), 0);
			} else if (answer == 3) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0x80), get_city(0xec), 0);
			}

			/* enter the temple */

			ds_writew(TYPEINDEX, 59);

			ds_writew(TEXTBOX_WIDTH, 3);

			old_loc = ds_readbs(LOCATION);

			ds_writebs(LOCATION, 2);

			do_temple();

			ds_writebs(LOCATION, (signed char)old_loc);

			ds_writew(TEXTBOX_WIDTH, old_tb_width);
		}
	}
}

/**
 * \brief the dialog with ALRIK DERONDAN
 *
*/
void PHX_alrik_derondan(void)
{
	signed short answer;
	signed short l_di = 0;
	Bit32s money;

	load_city_ltx(ARCHIVE_FILE_PHEX2_LTX);

	/* load the picture of ALRIK DERONDAN */
	load_in_head(3);

	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_city(0xf4), get_city(0xf8),
				3,
				get_city(0xfc),
				get_city(0x100),
				get_city(0x104));
	} while (answer == -1);

	if (answer == 1) {

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_city(0xf4), get_city(0x108),
					3,
					get_city(0x10c),
					get_city(0x110),
					get_city(0x114));
		} while (answer == -1);

		if (answer == 3) {

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_city(0xf4), get_city(0x128),
						3,
						get_city(0x12c),
						get_city(0x130),
						get_city(0x134));
			} while (answer == -1);

			if (answer == 1) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_city(0xf4), get_city(0x144), 0);

				ds_writeb(0x3f79, 1);

			} else if (answer == 2) {
				l_di = 1;
			} else {
				l_di = 3;
			}
		} else {
			GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_city(0xf4), get_city(0x138), 0);
		}

	} else if (answer == 2) {

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_city(0xf4), get_city(0x118),
					3,
					get_city(0x11c),
					get_city(0x120),
					get_city(0x124));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_city(0xf4), get_city(0x138), 0);

		} else if (answer == 2) {
			l_di = 2;
		} else {
			l_di = 1;
		}
	} else {
		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_city(0xf4), get_city(0x128),
					3,
					get_city(0x12c),
					get_city(0x130),
					get_city(0x134));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_city(0xf4), get_city(0x144), 0);

			ds_writeb(0x3f79, 1);
		} else if (answer == 2) {
			l_di = 1;
		} else {
			l_di = 3;
		}
	}

	if ((l_di == 2) || (l_di == 3)) {

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_city(0xf4),
					l_di == 3 ? get_city(0x148) : get_city(0x13c),
					3,
					get_city(0x150),
					get_city(0x154),
					get_city(0x158));
		} while (answer == -1);

		if (answer == 1) {
			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_city(0xf4), get_city(0x15c), 0);
		} else if (answer == 2) {
			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_city(0xf4), get_city(0x160), 0);
		} else {

			l_di = 1;
		}
	}

	if (l_di == 1) {

		money = get_party_money();

		answer = money >= 500 ? 3 : 2;

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_city(0xf4),
					get_city(0x140),
					answer,
					get_city(0x164),
					get_city(0x16c),
					get_city(0x168));
		} while (answer == -1);

		if (answer == 3) {

			money -= 500;
			set_party_money(money);

			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_city(0xf4), get_city(0x170), 0);

		} else if (answer == 2) {
			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_city(0xf4), get_city(0x138), 0);
		} else {

			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_city(0xf4),
				(test_attrib(Real2Host(get_first_hero_available_in_group()), 2, 0) > 0) ? get_city(0x170) : get_city(0x138),
				0);
		}
	}

	/* reset the flag */
	ds_writeb(ALRIK_DERONDAN, 0);

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

#if !defined(__BORLANDC__)
}
#endif
