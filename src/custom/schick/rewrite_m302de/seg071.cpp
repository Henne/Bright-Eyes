/**
 *	Rewrite of DSA1 v3.02_de functions of seg071 (phexcaer: buildings 2/2)
 *	Functions rewritten: 1/3
 */

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg026.h"
#include "seg027.h"
#include "seg032.h"
#include "seg047.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
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

			do_fight(236);
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

#if !defined(__BORLANDC__)
}
#endif
