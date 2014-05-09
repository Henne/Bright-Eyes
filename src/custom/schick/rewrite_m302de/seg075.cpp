/**
 *	Rewrite of DSA1 v3.02_de functions of seg075 (dungeons generic)
 *	Functions rewritten: 2/20
 */

#include "v302de.h"

#include "seg002.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
/* static */
void DNG_floor_ceil(void)
{
	struct nvf_desc nvf;
	signed short width, height;

	/* Load ceiling */
	nvf.dst = Real2Host(ds_readd(0xd303));
	nvf.src = Real2Host(ds_readd(0xd019));
	nvf.nr = 0;
	nvf.type = (!ds_readbs(0xe48c)) ? 3 : 5;
	nvf.width = (unsigned char*)&width;
	nvf.height = (unsigned char*)&height;
	process_nvf(&nvf);


	/* Load ceiling */
	nvf.dst = Real2Host(ds_readd(0xd303)) + 0x4030;
	nvf.src = Real2Host(ds_readd(0xd019));
	nvf.nr = (!(ds_readbs(0x2d3d)&1)) ? 1 : 2;
	nvf.type = (!ds_readbs(0xe48c)) ? 3 : 5;
	nvf.width = (unsigned char*)&width;
	nvf.height = (unsigned char*)&height;
	process_nvf(&nvf);

}

/* 0xa46 */

/**
 * is_staff_lvl2_in_group() - check for stafflevel >= in current group
 *
 * returns 0 = false, 1 = true
*/
signed short is_staff_lvl2_in_group(void)
{
	Bit8u *hero_i;
	signed short i;

	hero_i = get_hero(0);
	for (i = 0; i <= 6; i++, hero_i += 0x6da) {

		if (host_readbs(hero_i + 0x21) &&
			(host_readbs(hero_i + 0x87) == ds_readbs(CURRENT_GROUP)) &&
			check_hero(hero_i) &&
			(host_readbs(hero_i + 0x195) >= 2))
		{
			return 1;
		}
	}

	return 0;
}


#if !defined(__BORLANDC__)
}
#endif

