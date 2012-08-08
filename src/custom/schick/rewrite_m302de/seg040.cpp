/**
 *	Rewrite of DSA1 v3.02_de functions of seg040 (scenarios)
 *	Functions rewritten: 1/3
 */

#include <string.h>

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"

#include "seg032.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void FIG_chessboard_init(void)
{
	signed short obj_id;
	signed short cb_y;

	signed short i, cb_x;

	/* initialize the chessboard */
	memset(Real2Host(ds_readd(CHESSBOARD)), 0, 600);

	for (cb_y = 0; cb_y < 24; cb_y++) {

		for (cb_x = 0; cb_x < 24; cb_x++) {

			obj_id = host_readbs(Real2Host(ds_readd(SCENARIO_BUF)) + cb_y * 25 + cb_x + 0x15);

			if (obj_id < 0) {

				FIG_set_cb_field(cb_y, cb_x, obj_id);

			} else {

				if (obj_id >= 0x6c && obj_id <= 0x6f) {

					obj_id -= 0x32;

					if (obj_id == 0x3c) {

						for (i = 1; i < 8; i++) {
							FIG_set_cb_field(cb_y + i, cb_x, -3);
						}

					} else if (obj_id == 0x3d) {

						for (i = 1; i < 8; i++) {
							FIG_set_cb_field(cb_y, i + cb_x, -3);
						}
					}

					FIG_set_cb_field(cb_y, cb_x, -3);
				}
			}
		}
	}

	i = 0;

	if (host_readbs(Real2Host(ds_readd(SCENARIO_BUF)) + 0x14) <= 3) {

		while (ds_readbs(0x6040 + i * 2) != -1) {

			FIG_set_cb_field(ds_readbs(0x6041 + i * 2), ds_readbs(0x6040 + i * 2), 50);
			i++;
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif

