/**
 *	Rewrite of DSA1 v3.02_de functions of seg040 (scenarios)
 *	Functions rewritten: 3/3 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg040.cpp
 */

#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg006.h"
#include "seg032.h"
#include "seg039.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void FIG_chessboard_init(void)
{
	signed short i;
	signed short cb_x;
	signed short cb_y;
	signed short obj_id;

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
							FIG_set_cb_field(cb_y, cb_x + i, -3);
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

void FIG_preload_gfx(void)
{
	Bit8u *p1;
	signed short i;
	struct nvf_desc nvf;
	signed short handle;

	ds_writed(FIG_LIST_HEAD, 0);

	ds_writed(0xe37c, (Bit32u)F_PADD(ds_readd(0xd2db), -0x115d));

	memset(Real2Host(ds_readd(0xe37c)), 0, 0x115d);
	p1 = Real2Host(ds_readd(0xe37c));

	for (i = 0; i < 127; i++) {
		host_writeb(p1 + 0x10, -1);
		p1+=0x23;
		ds_writeb(0xe089 + i, 0);
	}

	ds_writed(0xd86a, (Bit32u)F_PADD(ds_readd(PTR_FIGHT_LST), 0xd8));

	ds_writed(0xd866, (Bit32u)((RealPt)ds_readd(0xd86a) + 0x1953));

	ds_writed(0xe388, (Bit32u)F_PADD(ds_readd(0xd866), 0xf5f));

	ds_writed(0xe384, (Bit32u)F_PADD(ds_readd(0xe388), 0xfc));

	ds_writed(0xe380, (Bit32u)F_PADD(ds_readd(0xe384), 0x7e));

	ds_writed(0xd86e, (Bit32u)F_PADD(ds_readd(0xe380), 0x7e));

	ds_writed(0xbd30, (Bit32u)F_PADD(ds_readd(0xe37c), -0x4217));

	/* set something in the hero charactersheet to -1 */
	for (i = 0; i <= 6; i++) {
		host_writeb(get_hero(i) + 0x81, -1);
	}

	/* set something in the enemy charactersheet to -1 */
	for (i = 0; i < 20; i++) {
		ds_writeb(i * 0x3e + ENEMY_SHEETS, 0);
		ds_writeb(i * 0x3e + 0xd371, -1);
	}

	for (i = 0; i < 90; i++) {
		ds_writeb(0xd874 + i, -1);
	}

	for (i = 0; i <= 62; i++) {
		host_writed(Real2Host(ds_readd(0xe388)) + i * 4, 0);
		host_writews(Real2Host(ds_readd(0xe384)) + i * 2,
			host_writews(Real2Host(ds_readd(0xe380)) + i * 2, 0));
	}

	ds_writeb(0xe38e, -1);

	for (i = 0; i < 20; i++) {
		ds_writeb(0xe38f + i, -1);
	}

	ds_writeb(0xe38d, -1);
	ds_writeb(0xe38c, -1);

	/* load ANI.DAT */
	ds_writed(0xe378, ds_readd(0xd86e));
	add_ds_ws(0xd86e, 9851);
	handle = load_archive_file(0xd8);
	read_archive_file(handle, Real2Host(ds_readd(0xe378)), 9851);
	bc_close(handle);

	/* load WEAPANI.DAT */
	ds_writed(0xe374, ds_readd(0xd86e));
	add_ds_ws(0xd86e, 1370);
	handle = load_archive_file(0xd9);
	read_archive_file(handle, Real2Host(ds_readd(0xe374)), 1370);
	bc_close(handle);

	/* process NVFs */

	ds_writed(0xd862, ds_readd(0xd86e));
	add_ds_ws(0xd86e, 300);

	nvf.dst = Real2Host(ds_readd(0xd862));
	nvf.src = Real2Host(ds_readd(0xd2e3));
	nvf.nr = 10;
	nvf.type = 0;
	nvf.width = (Bit8u*)&i;
	nvf.height = (Bit8u*)&i;
	process_nvf(&nvf);

	ds_writed(0xd85e, ds_readd(0xd86e));
	add_ds_ws(0xd86e, 300);

	nvf.dst = Real2Host(ds_readd(0xd85e));
	nvf.src = Real2Host(ds_readd(0xd2e3));
	nvf.nr = 11;
	nvf.type = 0;
	process_nvf(&nvf);

	ds_writed(0xd29d, ds_readd(0xd86e));
	add_ds_ws(0xd86e, 0xe8c);

	nvf.dst = Real2Host(ds_readd(0xd29d));
	nvf.src = Real2Host(ds_readd(0xd2e3));
	nvf.nr = 17;
	nvf.type = 0;
	process_nvf(&nvf);

	ds_writed(0xd85a, ds_readd(0xd86e));
	add_ds_ws(0xd86e, 400);
	ds_writed(0xd856, ds_readd(0xd86e));
	add_ds_ws(0xd86e, 1300);

	ds_writed(0xe370, F_PSUB(ds_readd(0xbd30), ds_readd(0xd86e)));

	ds_writew(0x605e, 0);
	ds_writeb(0xe36f, 0);
}

void FIG_draw_scenario(void)
{
	signed short cb_x;
	signed short cb_y;
	signed short obj_id;
	signed short width;
	signed short height;
	RealPt ptr;
	struct nvf_desc nvf;


	for (cb_x = 0; cb_x < 24; cb_x++) {
		for (cb_y = 0; cb_y < 24; cb_y++) {

			obj_id = host_readbs(Real2Host(ds_readd(SCENARIO_BUF)) + cb_y * 25 + cb_x + 0x15);

			if (obj_id >= 0x32) {
				if (obj_id < 0x6c || obj_id > 0x6f) {

					obj_id -= 50;

					/* NULL check */
					if (host_readd(Real2Host(ds_readd(0xe388)) + obj_id * 4)) {
						ptr = (RealPt)host_readd(Real2Host(ds_readd(0xe388)) + obj_id * 4);
					} else {
						ptr = (RealPt)ds_readd(0xd86e);

						nvf.dst = Real2Host(ptr);
						nvf.src = Real2Host(ds_readd(0xbd30));
						nvf.nr = obj_id;
						nvf.type = 0;
						nvf.width = (Bit8u*)&width;
						nvf.height = (Bit8u*)&height;
						process_nvf(&nvf);
#if !defined(__BORLANDC__)
						/* BE-fix */
						width = host_readws((Bit8u*)&width);
						height = host_readws((Bit8u*)&height);
#endif

						/* save sprite info */
						host_writed(Real2Host(ds_readd(0xe388)) + obj_id * 4, ds_readd(0xd86e));
						host_writew(Real2Host(ds_readd(0xe384)) + obj_id * 2, width);
						host_writew(Real2Host(ds_readd(0xe380)) + obj_id * 2, height);

						/* adjust pointer */
						add_ds_ws(0xd86e, (width * height + 8));
						/* var -= height * width + 8; */
						sub_ds_ds(0xe370, (width * height + 8L));
					}

					ds_writew(0xe066, 0);
					ds_writeb(0xe068, (signed char)obj_id);
					ds_writeb(0xe069, (signed char)cb_x);
					ds_writeb(0xe06a, (signed char)cb_y);
					ds_writeb(0xe06b, ds_readb(0x6060 + obj_id * 2));
					ds_writeb(0xe06c, ds_readb(0x60de + obj_id * 2));
					ds_writeb(0xe06d, host_readb(Real2Host(ds_readd(0xe380)) + obj_id * 2));
					ds_writeb(0xe06e, host_readb(Real2Host(ds_readd(0xe384)) + obj_id * 2));
					ds_writeb(0xe06f, 0);
					ds_writeb(0xe070, 0);
					ds_writeb(0xe071,
						(signed char)(host_readbs(Real2Host(ds_readd(0xe384)) + obj_id * 2)-1));
					ds_writeb(0xe072,
						(signed char)(host_readbs(Real2Host(ds_readd(0xe380)) + obj_id * 2)-1));
					ds_writeb(0xe07b, 0);
					ds_writeb(0xe073, 0);
					ds_writeb(0xe075, -1);
					ds_writeb(0xe074, -1);
					ds_writed(0xe07d, (Bit32u)ptr);
					ds_writeb(0xe077, obj_id >= 58 && obj_id <= 61 ? -1 : 50);
					ds_writeb(0xe078, 1);
					ds_writeb(0xe079, -1);
					ds_writeb(0xd874 + ds_readw(0x605e), FIG_add_to_list(-1));
					inc_ds_ws(0x605e);
					place_obj_on_cb(cb_x, cb_y, obj_id + 50, obj_id, 0);
				}
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif

