/**
 *	Rewrite of DSA1 v3.02_de functions of seg116 (travel events 8 / 10)
 *	Functions rewritten: 1/17
 */

#include "v302de.h"

#include "seg002.h"
#include "seg025.h"
#include "seg026.h"
#include "seg097.h"
#include "seg109.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
/* Borlandified and identical */
void tevent_130(void)
{
	signed short answer;

	if (TRV_enter_hut_question()) {

		ds_writews(0x434f, 0);

		ds_writeb(LOCATION, 6);
		do_location();
		ds_writeb(LOCATION, 0);

		TRV_load_textfile(-1);
		ds_writews(0x434f, -1);

		load_in_head(10);

		do {
			answer = GUI_dialogbox(Real2Host(ds_readd(DTP2)), (RealPt)NULL,
						get_city(0x4c), 2,
						get_city(0x50),
						get_city(0x54));
		} while (answer == -1);

		if (answer == 1) {

			do {
				answer = GUI_dialogbox(Real2Host(ds_readd(DTP2)), (RealPt)NULL,
							get_city(0x58), 3,
							get_city(0x5c),
							get_city(0x60),
							get_city(0x64));
			} while (answer == -1);

			if (answer == 1 || answer == 2) {
				GUI_dialog_na(0, answer == 1 ? get_city(0x68) : get_city(0x6c));
			} else {
					GUI_dialog_na(0, test_attrib(Real2Host(get_first_hero_available_in_group()), 2, 0) > 0 ? get_city(0x6c) : get_city(0x68));
			}
		}

		ds_writeb(LOCATION, 6);
		do_location();
		ds_writeb(LOCATION, 0);
	}
}
#endif

void TLK_old_woman(signed short state)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif
