/**
 *	Rewrite of DSA1 v3.02_de functions of seg025 (city)
 *	Functions rewritten: 2/18
 */

#include <stdlib.h>

#include "v302de.h"

#include "seg002.h"
#include "seg008.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0xb73 */
signed short get_border_index(signed short val)
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

#if !defined(__BORLANDC__)
}
#endif

