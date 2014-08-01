/*
 *	Rewrite of DSA1 v3.02_de functions of seg045 (fight helper)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y SEG045.CPP
 */

#include "v302de.h"

#include "seg002.h"
#include "seg006.h"
#include "seg038.h"
#include "seg039.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void seg045_0000(signed short fight_id, signed short type, signed short a3)
{
	signed short obj_x;
	signed short obj_y;
	struct nvf_desc nvf;

	FIG_search_obj_on_cb(fight_id, &obj_x, &obj_y);

	ds_writew(0xe066, 0);

	ds_writeb(0xe068, ds_readb(type * 2 + 0x61f8));

	if (type != 2) {
		ds_writeb(0xe068, ds_readbs(0xe068) + a3);
	}

	if (a3 == 0) {
		obj_x += 2;
	}

	ds_writeb(0xe069, obj_x);
	ds_writeb(0xe06a, obj_y);
	ds_writeb(0xe06b, ds_readb(0x6198 + type * 8 + a3 * 2));
	ds_writeb(0xe06c, ds_readb(0x61c8 + type * 8 + a3 * 2));
	ds_writeb(0xe06d, ds_readb(0x6204 + type * 2));
	ds_writeb(0xe06e, ds_readb(0x6210 + type * 2));
	ds_writeb(0xe06f, 0);
	ds_writeb(0xe070, 0);

	ds_writeb(0xe071, (signed char)(ds_readb(0x6210 + type * 2) - 1));
	ds_writeb(0xe072, (signed char)(ds_readb(0x6204 + type * 2) - 1));
	ds_writeb(0xe07b, 0);
	ds_writeb(0xe073, 0);
	ds_writeb(0xe075, -1);
	ds_writeb(0xe074, -1);
	ds_writed(0xe07d, ds_readd(0xd85a));
	ds_writeb(0xe077, 100);
	ds_writeb(0xe078, 0);
	ds_writeb(0xe079, -1);

	nvf.dst = Real2Host(ds_readd(0xd85a));
	nvf.src = Real2Host(ds_readd(0xd866));
	nvf.nr = ds_readbs(0xe068);
	nvf.type = 0;
	nvf.width = (Bit8u*)&obj_x;
	nvf.height = (Bit8u*)&obj_x;

	process_nvf(&nvf);

	ds_writeb(0xe38d, FIG_add_to_list(-1));
}

void FIG_remove_smth(void)
{
	FIG_remove_from_list(ds_readb(0xe38d), 0);
	ds_writeb(0xe38d, 0xff);
}

/**
 * FIG_copy_it() - copies chunks of 3 bytes
 * @dst:	pointer to destination
 * @src:	pointer to sources
 * @term:	termination sign
 */
signed short FIG_copy_it(Bit8u *dst, Bit8u *src, signed char term)
{
	signed short i;

	for (i = 0; host_readbs(src) != term; i += 3) {

		host_writeb(dst, host_readb(src));
		src++;
		dst++;

		host_writeb(dst, host_readb(src));
		src++;
		dst++;

		host_writeb(dst, host_readb(src));
		src++;
		dst++;
	}

	return i;
}

signed short seg045_01a0(signed short a1, signed short a2, signed short fight_id1, signed short fight_id2, signed short a5)
{
	signed short i;
	Bit8u *ptr;
	signed short id1_x;
	signed short id1_y;
	signed short id2_x;
	signed short id2_y;
	signed short distance;

	FIG_search_obj_on_cb(fight_id2, &id2_x, &id2_y);
	FIG_search_obj_on_cb(fight_id1, &id1_x, &id1_y);

	distance = seg039_0000(id1_x, id1_y, id2_x, id2_y);

	if (distance <= 1) {
		return 0;
	}

	ptr = p_datseg + 0xd8cf + a1 * 0xf3;
	ds_writeb(0xd8ce + a1 * 0xf3, 0);
	ds_writeb(0xd9c0 + a1 * 0xf3, 0);

	for (i = 0; distance - 1 > i; i++) {
		ptr += FIG_copy_it(ptr, Real2Host(host_readd(Real2Host(ds_readd(0x6324 + a2 * 4)) + a5 * 4)), -1);
	}

	host_writeb(ptr, -1);
	seg045_0000(fight_id1, a2, a5);

	return 1;
}

void seg045_0273(signed short x, signed short y, signed short a3)
{
	unsigned short height;
	unsigned short width;

	/* TODO: some kind of initialized structure */
	signed short l3;
	Bit8u *l4;
	Bit32s l5;

	struct nvf_desc nvf;

	l3 = ds_readws(0x633c);
	l4 = Real2Host(ds_readd(0x633e));
	l5 = ds_readd(0x6342);

	nvf.dst = Real2Host(ds_readd(0xd856));
	nvf.src = Real2Host(ds_readd(0xd866));
	/* TODO: ugly */
	nvf.nr = host_readbs((Bit8u*)&l3 + a3 - 1);
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	height = host_readws((Bit8u*)&height);
	width = host_readws((Bit8u*)&width);
#endif

	ds_writew(0xe066, 0);
	/* TODO: ugly */
	ds_writeb(0xe068, host_readbs((Bit8u*)&l3 + a3 - 1));
	ds_writeb(0xe069, x);
	ds_writeb(0xe06a, y);

	/* TODO: ugly */
	ds_writeb(0xe06b, host_readbs(((Bit8u*)&(l3)) - 6 + a3 * 2));
	/* TODO: ugly */
	ds_writeb(0xe06c, host_readbs(((Bit8u*)&(l3)) - 10 + a3 * 2));

	ds_writeb(0xe06d, height);
	ds_writeb(0xe06e, width);
	ds_writeb(0xe06f, 0);
	ds_writeb(0xe070, 0);
	ds_writeb(0xe071, width - 1);
	ds_writeb(0xe072, height - 1);
	ds_writeb(0xe07b, 0);
	ds_writeb(0xe073, 0);
	ds_writeb(0xe075, -1);
	ds_writeb(0xe074, -1);
	ds_writed(0xe07d, ds_readd(0xd856));
	ds_writeb(0xe077, 99);
	ds_writeb(0xe078, 0);
	ds_writeb(0xe079, -1);
	ds_writeb(0xe38c, FIG_add_to_list(-1));
}

void FIG_remove_smth2(void)
{
	FIG_remove_from_list(ds_readb(0xe38c), 0);
	ds_writeb(0xe38c, 0xff);
}

void seg045_0394(signed short a1, Bit8u *hero, signed short a3)
{
	Bit8u *ptr;
	signed short x;
	signed short y;

	FIG_search_obj_on_cb(host_readbs(hero + 0x86), &x, &y);

	ptr = p_datseg + a1 * 0xf3 + 0xd8cf;

	ds_writeb(0xd8ce + a1 * 0xf3, 0);
	ds_writeb(0xd9c0 + a1 * 0xf3, -1);

	ptr += FIG_copy_it(ptr, Real2Host(ds_readd(0x6362 + a3 * 4)), -1);
	host_writeb(ptr, -1);
	seg045_0273(x, y, a3);
}

void seg045_041b(signed short a1, Bit8u *enemy, signed short a3)
{
	Bit8u *ptr;
	signed short x;
	signed short y;

	FIG_search_obj_on_cb(host_readbs(enemy + 0x2d), &x, &y);

	ptr = p_datseg + a1 * 0xf3 + 0xd8cf;

	ds_writeb(0xd8ce + a1 * 0xf3, 0);
	ds_writeb(0xd9c0 + a1 * 0xf3, -1);

	ptr += FIG_copy_it(ptr, Real2Host(ds_readd(0x6362 + a3 * 4)), -1);
	host_writeb(ptr, -1);
	seg045_0273(x, y, a3);
}

#if !defined(__BORLANDC__)
}
#endif
