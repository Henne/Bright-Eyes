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
	signed short nr_heroes;
	Bit32s money;

	load_ani(3);

	init_ani(0);

	money = get_party_money();

	if ((ds_readds(DAY_TIMER) >= HOURS(15)) && (ds_readds(DAY_TIMER) <= HOURS(19))) {

		/* load the picture of the TEMPLE SERVANT */

		load_in_head(6);

		nr_heroes = count_heroes_in_group();

		if (ds_readb(DRUG_TIMER) && (nr_heroes * 2000 <= money)) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(15), get_tx2(14), 0);

			money -= nr_heroes * 2000;
			set_party_money(money);

		} else {

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(15), get_tx2(0),
						3,
						get_tx2(1),
						get_tx2(2),
						get_tx2(3));
			} while (answer == -1);

			if (answer == 1) {

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_tx2(15), get_tx2(4),
							3,
							get_tx2(5),
							get_tx2(6),
							get_tx2(7));
				} while (answer == -1);

				if (answer == 3) {
					GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_tx2(15), get_tx2(8), 0);

					ds_writeb(DRUG_TIMER, 10);
				}

			} else if (answer == 2) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(15), get_tx2(8), 0);

				ds_writeb(DRUG_TIMER, 10);
			} else {

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_tx2(15), get_tx2(9),
							3,
							get_tx2(10),
							get_tx2(11),
							get_tx2(12));
				} while (answer == -1);

				if (answer == 3) {

					GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(15), get_tx2(13), 0);

				} else {

					GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(15), get_tx2(8), 0);

					ds_writeb(DRUG_TIMER, 10);
				}
			}
		}
	} else {

		/* load the picture of EKTOR GREMOB */
		load_in_head(8);

		nr_heroes = count_heroes_in_group();

		if (ds_readb(GREMOB_INVITED) != 0) {

			GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_tx2(16), get_tx2(29), 0);

			do_fight(FIGHTS_PHEX3);
		} else {

			if (ds_readb(DRUG_TIMER) && (nr_heroes * 2500 <= money)) {

				GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(16), get_tx2(28), 0);

				money -= nr_heroes * 2500;
				set_party_money(money);
			} else {

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_tx2(16), get_tx2(17),
							2,
							get_tx2(19),
							get_tx2(30));
				} while (answer == -1);

				if (answer == 1) {

					do {
						answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_tx2(16), get_tx2(18),
							3,
							get_tx2(19),
							get_tx2(20),
							get_tx2(21));
					} while (answer == -1);

					GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(16), get_tx2(26), 0);

					if (nr_heroes * 2000 <= money) {

						if (GUI_bool(get_tx2(31))) {
							money -= nr_heroes * 2000;
							set_party_money(money);
						}
					}
				} else {
					do {
						answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_tx2(16), get_tx2(22),
							3,
							get_tx2(23),
							get_tx2(24),
							get_tx2(25));
					} while (answer == -1);

					if (answer == 1) {
						GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(16), get_tx2(27), 0);
						ds_writeb(GREMOB_INVITED, 1);
					} else {
						GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(16), get_tx2(8), 0);
						ds_writeb(DRUG_TIMER, 10);
					}
				}
			}
		}
	}

	set_var_to_zero();
}

/**
 * \brief   the Phextemple of Phexcaer
 *
 *          This function shows how to enter a temple manually.
 *          Also there is some code, which will never be executed,
 *          because PTEMPLE_CLOSED is initially 0 and never changes.
 */
void PHX_phextempel(void)
{
	signed short answer;		/* the selected answer from the dialogs */
	signed short old_tb_width;	/* saving the width of the textboxes */
	signed short old_loc;		/* saving the no of the old location */

	old_tb_width = ds_readws(TEXTBOX_WIDTH);

	/* this variable is never != 0 */
	if (ds_readb(PTEMPLE_CLOSED) != 0) {

		/* the cleric is gone */
		GUI_output(get_tx2(60));

	} else {

		/* load the picture of THE FEMALE CLERIC */

		load_in_head(12);

		/* show introduction text */
		GUI_output(get_tx2(33));

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_tx2(32), get_tx2(34),
					3,
					get_tx2(35),
					get_tx2(36),
					get_tx2(37));
		} while (answer == -1);

		if (answer == 1) {

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(32), get_tx2(38),
						3,
						get_tx2(39),
						get_tx2(40),
						get_tx2(41));
			} while (answer == -1);

			if (answer == 1) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(32), get_tx2(50), 0);
			} else {

				/* enter the temple */

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(32), get_tx2(51), 0);

				ds_writew(TYPEINDEX, 59);

				ds_writew(TEXTBOX_WIDTH, 3);

				old_loc = ds_readbs(LOCATION);

				ds_writebs(LOCATION, LOCATION_TEMPLE);

				do_temple();

				ds_writebs(LOCATION, (signed char)old_loc);

				ds_writew(TEXTBOX_WIDTH, old_tb_width);
			}

		} else if (answer == 2) {

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(32), get_tx2(42),
						3,
						get_tx2(43),
						get_tx2(44),
						get_tx2(45));
			} while (answer == -1);

			if (answer == 1) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(32), get_tx2(50), 0);

			} else if (answer == 2) {

				do {
					answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
							get_tx2(32), get_tx2(52),
							3,
							get_tx2(53),
							get_tx2(54),
							get_tx2(55));
				} while (answer == -1);

				if (answer == 2) {

					GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(32), get_tx2(56), 0);
				}

			} else {

				/* enter the temple */

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(32), get_tx2(51), 0);

				ds_writew(TYPEINDEX, 59);

				ds_writew(TEXTBOX_WIDTH, 3);

				old_loc = ds_readbs(LOCATION);

				ds_writebs(LOCATION, LOCATION_TEMPLE);

				do_temple();

				ds_writebs(LOCATION, (signed char)old_loc);

				ds_writew(TEXTBOX_WIDTH, old_tb_width);
			}

		} else {

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(32), get_tx2(46),
						3,
						get_tx2(47),
						get_tx2(48),
						get_tx2(49));
			} while (answer == -1);

			if (answer == 1) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(32), get_tx2(57), 0);
			} else if (answer == 2) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(32), get_tx2(58), 0);
			} else if (answer == 3) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(32), get_tx2(59), 0);
			}

			/* enter the temple */

			ds_writew(TYPEINDEX, 59);

			ds_writew(TEXTBOX_WIDTH, 3);

			old_loc = ds_readbs(LOCATION);

			ds_writebs(LOCATION, LOCATION_TEMPLE);

			do_temple();

			ds_writebs(LOCATION, (signed char)old_loc);

			ds_writew(TEXTBOX_WIDTH, old_tb_width);
		}
	}
}

/**
 * \brief   the dialog with ALRIK DERONDAN
 */
void PHX_alrik_derondan(void)
{
	signed short answer;
	signed short l_di = 0;
	Bit32s money;

	load_tx2(ARCHIVE_FILE_PHEX2_LTX);

	/* load the picture of ALRIK DERONDAN */
	load_in_head(3);

	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_tx2(61), get_tx2(62),
				3,
				get_tx2(63),
				get_tx2(64),
				get_tx2(65));
	} while (answer == -1);

	if (answer == 1) {

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_tx2(61), get_tx2(66),
					3,
					get_tx2(67),
					get_tx2(68),
					get_tx2(69));
		} while (answer == -1);

		if (answer == 3) {

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
						get_tx2(61), get_tx2(74),
						3,
						get_tx2(75),
						get_tx2(76),
						get_tx2(77));
			} while (answer == -1);

			if (answer == 1) {

				GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_tx2(61), get_tx2(81), 0);

				ds_writeb(ALRIK_DERONDAN_INSULTED, 1);

			} else if (answer == 2) {
				l_di = 1;
			} else {
				l_di = 3;
			}
		} else {
			GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_tx2(61), get_tx2(78), 0);
		}

	} else if (answer == 2) {

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_tx2(61), get_tx2(70),
					3,
					get_tx2(71),
					get_tx2(72),
					get_tx2(73));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_tx2(61), get_tx2(78), 0);

		} else if (answer == 2) {
			l_di = 2;
		} else {
			l_di = 1;
		}
	} else {
		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_tx2(61), get_tx2(74),
					3,
					get_tx2(75),
					get_tx2(76),
					get_tx2(77));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_tx2(61), get_tx2(81), 0);

			ds_writeb(ALRIK_DERONDAN_INSULTED, 1);
		} else if (answer == 2) {
			l_di = 1;
		} else {
			l_di = 3;
		}
	}

	if ((l_di == 2) || (l_di == 3)) {

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_tx2(61),
					l_di == 3 ? get_tx2(82) : get_tx2(79),
					3,
					get_tx2(84),
					get_tx2(85),
					get_tx2(86));
		} while (answer == -1);

		if (answer == 1) {
			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_tx2(61), get_tx2(87), 0);
		} else if (answer == 2) {
			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_tx2(61), get_tx2(88), 0);
		} else {

			l_di = 1;
		}
	}

	if (l_di == 1) {

		money = get_party_money();

		answer = money >= 500 ? 3 : 2;

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2),
					get_tx2(61),
					get_tx2(80),
					answer,
					get_tx2(89),
					get_tx2(91),
					get_tx2(90));
		} while (answer == -1);

		if (answer == 3) {

			money -= 500;
			set_party_money(money);

			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_tx2(61), get_tx2(92), 0);

		} else if (answer == 2) {
			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_tx2(61), get_tx2(78), 0);
		} else {

			GUI_dialogbox((RealPt)ds_readd(DTP2),
				get_tx2(61),
				(test_attrib(Real2Host(get_first_hero_available_in_group()), ATTRIB_CH, 0) > 0) ? get_tx2(92) : get_tx2(78),
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
