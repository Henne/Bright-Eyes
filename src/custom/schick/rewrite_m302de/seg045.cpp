/*
	Rewrite of DSA1 v3.02_de functions of seg045 (fight helper)
	Functions rewritten: 4/8
*/

#include "v302de.h"

#include "seg002.h"
#include "seg006.h"
#include "seg038.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x000 */

/* Borlandified and identical */
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


/* 0x137 */
/* Borlandified and identical */
void FIG_remove_smth(void)
{
	FIG_remove_from_list(ds_readb(0xe38d), 0);
	ds_writeb(0xe38d, 0xff);
}

/* 0x14f */
/**
 * FIG_copy_it() - copies chunks of 3 bytes
 * @dst:	pointer to destination
 * @src:	pointer to sources
 * @term:	termination sign
 */
/* Borlandified and identical */
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

/* 0x37c */
void FIG_remove_smth2(void)
{
	FIG_remove_from_list(ds_readb(0xe38c), 0);
	ds_writeb(0xe38c, 0xff);
}

#if !defined(__BORLANDC__)
}
#endif
