/**
 *	Rewrite of DSA1 v3.02_de functions of seg040 (scenarios)
 *	Functions rewritten: 2/3
 */

#include <string.h>

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg006.h"
#include "seg032.h"
#include "seg039.h"

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

void FIG_draw_scenario(void)
{
	struct nvf_desc nvf;
	RealPt ptr;
	signed short height;
	signed short width;
	signed short obj_id;
	signed short cb_y;
	signed short cb_x;

	for (cb_x = 0; cb_x < 24; cb_x++) {
		for (cb_y = 0; cb_y < 24; cb_y++) {

			obj_id = host_readbs(Real2Host(ds_readd(SCENARIO_BUF)) + cb_y * 25 + cb_x + 0x15);

			if (obj_id >= 0x32) {
				if (obj_id < 0x6c || obj_id > 0x6f) {

					obj_id -= 50;

					/* NULL check */
					if (host_readd(Real2Host(ds_readd(0xe388)) + obj_id * 4)) {
						ptr = host_readd(Real2Host(ds_readd(0xe388)) + obj_id * 4);
					} else {
						ptr = ds_readd(0xd86e);

						nvf.dst = Real2Host(ptr);
						nvf.src = Real2Host(ds_readd(0xbd30));
						nvf.nr = obj_id;
						nvf.type = 0;
						nvf.width = (Bit8u*)&width;
						nvf.height = (Bit8u*)&height;
						process_nvf(&nvf);

						/* save sprite info */
						host_writed(Real2Host(ds_readd(0xe388)) + obj_id * 4, ds_readd(0xd86e));
						host_writew(Real2Host(ds_readd(0xe384)) + obj_id * 2, width);
						host_writew(Real2Host(ds_readd(0xe380)) + obj_id * 2, height);

						/* adjust pointer */
						ds_writed(0xd86e, ds_readd(0xd86e) + (height * width + 8));
						/* var -= height * width + 8; */
						ds_writed(0xe370, ds_readd(0xe370) - (height * width + 8));
					}

					ds_writew(0xe066, 0);
					ds_writeb(0xe068, obj_id);
					ds_writeb(0xe069, cb_x);
					ds_writeb(0xe06a, cb_y);
					ds_writeb(0xe06b, ds_readb(0x6060 + obj_id * 2));
					ds_writeb(0xe06c, ds_readb(0x60de + obj_id * 2));
					ds_writeb(0xe06d, host_readb(Real2Host(ds_readd(0xe380)) + obj_id * 2));
					ds_writeb(0xe06e, host_readb(Real2Host(ds_readd(0xe384)) + obj_id * 2));
					ds_writeb(0xe06f, 0);
					ds_writeb(0xe070, 0);
					ds_writeb(0xe071, host_readb(Real2Host(ds_readd(0xe384)) + obj_id * 2) - 1);
					ds_writeb(0xe072, host_readb(Real2Host(ds_readd(0xe380)) + obj_id * 2) - 1);
					ds_writeb(0xe07b, 0);
					ds_writeb(0xe073, 0);
					ds_writeb(0xe075, -1);
					ds_writeb(0xe074, -1);
					ds_writed(0xe07d, ptr);
					ds_writeb(0xe077, obj_id >= 58 && obj_id <= 61 ? -1 : 50);
					ds_writeb(0xe078, 1);
					ds_writeb(0xe079, -1);
					ds_writeb(0xd874 + ds_readw(0x605e), FIG_add_to_list(-1));
					/* inc */
					ds_writew(0x605e, ds_readw(0x605e) + 1);
					place_obj_on_cb(cb_x, cb_y, obj_id + 50, obj_id, 0);
					/* 0x5ba - 0x6aa */
				}
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif

