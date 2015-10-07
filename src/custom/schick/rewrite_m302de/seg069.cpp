/**
 *	Rewrite of DSA1 v3.02_de functions of seg069 (special buildings: Thorwal 2/2)
 *	Functions rewritten: 2/5
 */

#include "v302de.h"

#include "seg026.h"
#include "seg032.h"
#include "seg097.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void THO_hetmann(void)
{
	signed short answer;
	signed short i;
	signed short map_parts;

	load_in_head(66);

	if (!ds_readb(GOT_LETTER_HET) &&
		ds_readw(GOT_MAIN_QUEST) != 0 &&
		(ds_readb(0x344c) != 0 || ds_readb(NEED_LETTER) != 0))
	{

		/* count already collected parts of the map */
		for (i = map_parts = 0; i < 9; i++) {
			if (ds_readb(TREASURE_MAPS + i) != 0) {
				map_parts++;
			}
		}

		load_in_head(61);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				get_dtp(0x148), 0);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				get_dtp(0x14c), 0);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				ds_readws(MIN_MAP_PARTS) <= map_parts ? get_dtp(0x150) : get_dtp(0x154), 0);

		answer = ds_readws(MIN_MAP_PARTS) <= map_parts ? 10 : 50;

		for (i = 0; i < 14; i++) {
			add_ds_ds(GODS_ESTIMATION + 4 * i, answer);
		}

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				get_dtp(0x158), 0);

		ds_writeb(GOT_LETTER_HET, 1);
		get_item(235, 1, 1);

	} else if (ds_readw(HEARD_ANNOUNCE) == 0 || ds_readw(GOT_MAIN_QUEST) != 0) {
		GUI_output(get_city(0x00));

	} else {
		ds_writew(SUBVENTION, 0);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x04), 3,
					get_city(0x08),
					get_city(0x0c),
					get_city(0x10));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x14), 0);

		} else if (answer == 2) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x18), 0);

			add_ds_ws(SUBVENTION, 2);

		} else {

			GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x1c), 0);

			inc_ds_ws(SUBVENTION);
		}

		load_in_head(61);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				get_city(0x20), 0);

		ds_writew(HEARD_ANNOUNCE, 3);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				get_city(0x24), 0);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
					get_city(0x28), 3,
					get_city(0x2c),
					get_city(0x30),
					get_city(0x34));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
					get_city(0x38), 0);

			ds_writeb(INFORMER_ISLEIF, 1);
			inc_ds_ws(SUBVENTION);
			ds_writew(GOT_MAIN_QUEST, 1);
			ds_writew(QUESTED_MONTHS, 0);

		} else if (answer == 2) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
					get_city(0x3c), 0);

			ds_writeb(INFORMER_ISLEIF, 1);
			ds_writew(GOT_MAIN_QUEST, 1);
			ds_writew(QUESTED_MONTHS, 0);
			ds_writeb(GOT_LETTER_HET, 1);
			get_item(235, 1, 1);

		} else if (answer == 3) {

			add_ds_ws(SUBVENTION, 2);

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
						get_city(0x40), 2,
						get_city(0x44),
						get_city(0x48));
			} while (answer == -1);

			if (answer == 1) {
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
						get_city(0x4c), 0);

				ds_writew(GOT_MAIN_QUEST, 1);
				ds_writew(QUESTED_MONTHS, 0);
			}
		}
	}
}

/* Borlandified and identical */
void THO_windriders(void)
{
	signed short answer;

	answer = GUI_radio(get_city(0x50), 3,
				get_city(0x54),
				get_city(0x58),
				get_city(0x5c));
	if (answer != -1) {

		if (answer == 1) {

			GUI_output(get_city(0x60));

		} else if (answer == 2 || answer == 3) {

			if (ds_readw(OTTA_WINDRIDERS) == 0) {

				GUI_output(answer == 2 ? get_city(0x64): get_city(0x68));

			} else if (ds_readw(OTTA_WINDRIDERS) == 1) {

				GUI_output(get_city(0x6c));

			} else {

				GUI_output(get_city(0x70));

				ds_writew(0x26c1, 1);
				do_fight(213);
			}

			inc_ds_ws(OTTA_WINDRIDERS);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
