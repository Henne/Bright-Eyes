/**
 *	Rewrite of DSA1 v3.02_de functions of seg069 (special buildings: Thorwal 2/2)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg069.cpp
 */

#include "v302de.h"

#include "seg004.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg032.h"
#include "seg054.h"
#include "seg059.h"
#include "seg097.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void THO_hetmann(void)
{
	signed short answer;
	signed short i;
	signed short map_parts;

	load_in_head(66);

	if (!ds_readb(GOT_LETTER_HET) &&
		ds_readw(GOT_MAIN_QUEST) != 0 &&
		(ds_readb(JURGE_AWAITS_LETTER) != 0 || ds_readb(NEED_LETTER) != 0))
	{

		/* count already collected parts of the map */
		for (i = map_parts = 0; i < 9; i++) {
			if (ds_readb(TREASURE_MAPS + i) != 0) {
				map_parts++;
			}
		}

		load_in_head(61);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(85),
				get_tx(82), 0);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(85),
				get_tx(83), 0);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(85),
				ds_readws(MIN_MAP_PARTS) <= map_parts ? get_tx(84) : get_tx(85), 0);

		answer = ds_readws(MIN_MAP_PARTS) <= map_parts ? 10 : 50;

		for (i = 0; i < 14; i++) {
			add_ds_ds(GODS_ESTIMATION + 4 * i, answer);
		}

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(85),
				get_tx(86), 0);

		ds_writeb(GOT_LETTER_HET, 1);
		get_item(ITEM_WRITING_OF_HETMAN, 1, 1);

	} else if (ds_readw(HEARD_ANNOUNCE) == 0 || ds_readw(GOT_MAIN_QUEST) != 0) {
		GUI_output(get_tx2(0));

	} else {
		ds_writew(SUBVENTION, 0);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_tx2(1), 3,
					get_tx2(2),
					get_tx2(3),
					get_tx2(4));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_tx2(5), 0);

		} else if (answer == 2) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_tx2(6), 0);

			add_ds_ws(SUBVENTION, 2);

		} else {

			GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_tx2(7), 0);

			inc_ds_ws(SUBVENTION);
		}

		load_in_head(61);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(85),
				get_tx2(8), 0);

		ds_writew(HEARD_ANNOUNCE, 3);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(85),
				get_tx2(9), 0);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(85),
					get_tx2(10), 3,
					get_tx2(11),
					get_tx2(12),
					get_tx2(13));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(85),
					get_tx2(14), 0);

			ds_writeb(INFORMER_FLAGS + INFORMER_ISLEIF, 1);
			inc_ds_ws(SUBVENTION);
			ds_writew(GOT_MAIN_QUEST, 1);
			ds_writew(QUESTED_MONTHS, 0);

		} else if (answer == 2) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(85),
					get_tx2(15), 0);

			ds_writeb(INFORMER_FLAGS + INFORMER_ISLEIF, 1);
			ds_writew(GOT_MAIN_QUEST, 1);
			ds_writew(QUESTED_MONTHS, 0);
			ds_writeb(GOT_LETTER_HET, 1);
			get_item(ITEM_WRITING_OF_HETMAN, 1, 1);

		} else if (answer == 3) {

			add_ds_ws(SUBVENTION, 2);

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(85),
						get_tx2(16), 2,
						get_tx2(17),
						get_tx2(18));
			} while (answer == -1);

			if (answer == 1) {
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx2(85),
						get_tx2(19), 0);

				ds_writew(GOT_MAIN_QUEST, 1);
				ds_writew(QUESTED_MONTHS, 0);
			}
		}
	}
}

void THO_windriders(void)
{
	signed short answer;

	answer = GUI_radio(get_tx2(20), 3,
				get_tx2(21),
				get_tx2(22),
				get_tx2(23));
	if (answer != -1) {

		if (answer == 1) {

			GUI_output(get_tx2(24));

		} else if (answer == 2 || answer == 3) {

			if (ds_readw(OTTA_WINDRIDERS) == 0) {

				GUI_output(answer == 2 ? get_tx2(25): get_tx2(26));

			} else if (ds_readw(OTTA_WINDRIDERS) == 1) {

				GUI_output(get_tx2(27));

			} else {

				GUI_output(get_tx2(28));

				ds_writew(FIG_DISCARD, 1);
				do_fight(FIGHTS_THOR8);
			}

			inc_ds_ws(OTTA_WINDRIDERS);
		}
	}
}

void THO_tav_inn_combi(void)
{
	signed short answer;
	signed short type_bak;

	set_var_to_zero();
	load_ani(4);
	init_ani(0);

	answer = GUI_radio(get_ttx(673), 3,
				get_ttx(674),
				get_ttx(675),
				get_ttx(347));

	/* save the combo typeindex */
	type_bak = ds_readw(TYPEINDEX);

	do {

		/* restore the combo typeindex */
		ds_writew(TYPEINDEX, type_bak);

		if (answer == 1) {

			/* enter TAVERN */

			/* set combo_mode active */
			ds_writew(COMBO_MODE, 1);

			/* set the typeindex of the corresponding tavern */
			answer = ds_readw(TYPEINDEX);
			ds_writew(TYPEINDEX, answer == 11 ? 0 : (
					answer == 14 ? 1 : (
					answer == 17 ? 2 : 6)));

			ds_writew(TEXTBOX_WIDTH, 3);

			ds_writeb(LOCATION, LOCATION_TAVERN);
			do_tavern();

			/* leave the loop or enter the inn in the next iteration */
			answer = 0;
			if (ds_readw(COMBO_MODE) == 2) {
				answer = 2;
			}

		} else if (answer == 2) {

			/* enter INN */

			/* set combo_mode active */
			ds_writew(COMBO_MODE, 1);

			/* set the typeindex of the corresponding inn */
			answer = ds_readw(TYPEINDEX);
			ds_writew(TYPEINDEX, answer == 11 ? 70 : (
					answer == 14 ? 71 : (
					answer == 17 ? 72 : 73)));

			ds_writew(TEXTBOX_WIDTH, 3);

			ds_writeb(LOCATION, LOCATION_INN);
			do_inn();

			/* leave the loop or enter the tavern in the next iteration */
			answer = 0;
			if (ds_readw(COMBO_MODE) == 2) {
				answer = 1;
			}

		} else {
			set_var_to_zero();
			return;
		}

	} while (answer != 0);

	set_var_to_zero();
	leave_location();
}


void THO_stormchildren(void)
{
	signed short answer;

	answer = GUI_radio(get_tx2(29), 3,
				get_tx2(30),
				get_tx2(31),
				get_tx2(32));
	if (answer != -1) {

		if (answer == 1) {

			GUI_output(get_tx2(33));

		} else if (answer == 2 || answer == 3) {

			if (ds_readw(OTTA_STORMCHILDREN) == 0) {

				GUI_output(answer == 2 ? get_tx2(34): get_tx2(35));

			} else if (ds_readw(OTTA_STORMCHILDREN) == 1) {

				GUI_output(get_tx2(36));

			} else {

				GUI_output(get_tx2(37));

				ds_writew(FIG_DISCARD, 1);
				do_fight(FIGHTS_THOR8);
			}

			inc_ds_ws(OTTA_STORMCHILDREN);
		}
	}
}

void THO_garaldsson(void)
{
	signed short answer;

	answer = GUI_radio(get_tx2(38), 3,
				get_tx2(39),
				get_tx2(40),
				get_tx2(41));
	if (answer != -1) {

		if (answer == 1) {

			GUI_output(get_tx2(42));

		} else if (answer == 2 || answer == 3) {

			if (ds_readw(OTTA_GARALDSSON) == 0) {

				GUI_output(answer == 2 ? get_tx2(43): get_tx2(44));

			} else if (ds_readw(OTTA_GARALDSSON) == 1) {

				GUI_output(get_tx2(45));

			} else {

				GUI_output(get_tx2(46));

				ds_writew(FIG_DISCARD, 1);
				do_fight(FIGHTS_THOR8);
			}

			inc_ds_ws(OTTA_GARALDSSON);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
