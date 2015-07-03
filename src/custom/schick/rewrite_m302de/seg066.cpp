/**
 *	Rewrite of DSA1 v3.02_de functions of seg066 (city)
 *	Functions rewritten: 3/18
 */

#include <stdlib.h>

#include "v302de.h"

#include "seg002.h"
#include "seg008.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void TLK_eremit(signed short)
{
	DUMMY_WARNING();
}

void do_town(void)
{
	DUMMY_WARNING();
}

/* 0xb73 */
signed short get_border_index(unsigned char val)
{
	signed short i;

	i = 0;
	while (ds_readb(0x7246 + i) < val) {
		i++;
	}

	ds_writew(0xe412, (((val & 3) + 4) - ds_readbs(DIRECTION)) & 3);

	if (i == 0)
		i = 1;

	i--;

	return i;

}

/* 0xf62 */
void load_city_texture(signed short v1, signed short v2, signed short nvf_nr,
		signed short v4)
{

	struct nvf_desc nvf;
	Bit8u *dst;
	Bit8u *src;
	signed short direction;
	signed short copy_height;
	signed short copy_width;
	signed short height;
	signed short width;

	direction = (nvf_nr & 0x8000 ? 1: 0);
	nvf_nr &= 0x3fff;

	v4 -= 184;

	nvf.dst = src = Real2Host(ds_readd(0xd303)) + 0x7530;
	nvf.src = Real2Host(ds_readd(0xe414 + v4 * 4));

	if (v4 == 48 || v4 == 49) {
		nvf.src = Real2Host(ds_readd(0xd2b5));
	}

	nvf.nr = nvf_nr;
	nvf.type = (direction == 0 ? 3: 5);
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	height = host_readws((Bit8u*)&height);
	width = host_readws((Bit8u*)&width);
#endif

	copy_width = width;
	copy_height = height;

	if (v1 < 0) {
		if (v1 + copy_width > 0) {
			copy_width += v1;
			src += abs(v1);
			v1 = 0;
		}
	}

	if (v2 < 0) {
		if (v2 + copy_height > 0) {
			copy_height -= v2;
			src += abs(v2) * width;
			v2 = 0;
		}
	}


	if (v1 < 208 && v2 < 135 && v1 >= 0 && v2 >= 0) {

		if (copy_width + v1 > 208) {
			copy_width = 208 - v1;
		}

		if (copy_height + v2 > 135) {
			copy_height = 135 - v2;
		}

		dst = Real2Host(ds_readd(0xd303)) + v2 * 208 + v1;

		copy_solid(dst, src, copy_width, copy_height, 208, width,
			v4 == 0 ? 0 : 128);
	}
}

/* 0x172b */
void seg066_172b(void)
{
	signed short l_si;
	signed short l_di;
	Bit8u *ptr = p_datseg + 0xbd95;

	ds_writeb(0xe400, ds_writeb(0xe401, ds_writeb(0xe402, ds_writeb(0xe403, 0))));

	for (l_di = 0; ds_readb(0xbd94) * 16 > l_di; l_di++) {

		l_si = get_border_index(host_readbs(ptr + l_di));

		if (l_si == 2) {
			inc_ds_bs_post(0xe400);
		} else if (l_si == 3) {
			inc_ds_bs_post(0xe401);
		} else if ((l_si == 4) || (l_si == 1)) {
			inc_ds_bs_post(0xe402);
		} else if (l_si == 5) {
			inc_ds_bs_post(0xe403);
		}
	}

	l_di = 5;
	l_si = 2000;

	if (ds_readb(0xe400) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 0));
	}

	if (ds_readb(0xe401) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 1));
	}

	if (ds_readb(0xe402) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 2));
	}

	if (ds_readb(0xe403) < l_si) {
		l_si = ds_readb(0xe400 + (l_di = 3));
	}

	ds_writeb(0xe400 + l_di, 0);
}

#if !defined(__BORLANDC__)
}
#endif

