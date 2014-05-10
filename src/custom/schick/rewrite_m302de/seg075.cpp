/**
 *	Rewrite of DSA1 v3.02_de functions of seg075 (dungeons generic)
 *	Functions rewritten: 4/20
 */

#include "v302de.h"

#include "seg002.h"
#include "seg003.h"
#include "seg075.h"

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

/* Borlandified and identical */
/* static */
void DNG_turn(void)
{
	signed short tmp;

	if ( ((tmp = div16(ds_readb(0xbd71))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd71, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd75))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd75, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd76))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd76, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd7c))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd7c, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd7d))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd7d, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd83))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd83, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd6e))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd77,
			ds_writeb(0xbd7d,
			ds_writeb(0xbd84, 0)));

		if ( ((tmp = div16(ds_readb(0xbd73))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd72,
				ds_writeb(0xbd78,
				ds_writeb(0xbd7e,
				ds_writeb(0xbd85, 0))));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd70))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd7b,
			ds_writeb(0xbd83,
			ds_writeb(0xbd8a, 0)));

		if ( ((tmp = div16(ds_readb(0xbd73))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd74,
				ds_writeb(0xbd7a,
				ds_writeb(0xbd82,
				ds_writeb(0xbd89, 0))));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd72))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd77,
			ds_writeb(0xbd7e,
			ds_writeb(0xbd84,
			ds_writeb(0xbd85, 0))));

		if ( ((tmp = div16(ds_readb(0xbd73))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd78, 0);
		}

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1)) {
				ds_writeb(0xbd78,
					ds_writeb(0xbd7f,
					ds_writeb(0xbd80, 0)));

				ds_writeb(0xbd86,
					ds_writeb(0xbd87, 0));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd73))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd79,
			ds_writeb(0xbd7f,
			ds_writeb(0xbd80, 0)));

		ds_writeb(0xbd81,
			ds_writeb(0xbd86,
			ds_writeb(0xbd87, 0)));

		ds_writeb(0xbd88, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd74))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd7b,
			ds_writeb(0xbd82,
			ds_writeb(0xbd89,
			ds_writeb(0xbd8a, 0))));

		if ( ((tmp = div16(ds_readb(0xbd73))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd79,
				ds_writeb(0xbd7a,
				ds_writeb(0xbd7f, 0)));

			ds_writeb(0xbd80,
				ds_writeb(0xbd81,
				ds_writeb(0xbd86, 0)));

			ds_writeb(0xbd87,
				ds_writeb(0xbd88, 0));

		}

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1)) {
				ds_writeb(0xbd7a,
					ds_writeb(0xbd80,
					ds_writeb(0xbd81, 0)));

				ds_writeb(0xbd87,
					ds_writeb(0xbd88, 0));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd77))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd84, 0);

		if ( ((tmp = div16(ds_readb(0xbd7f))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd7e,
				ds_writeb(0xbd85, 0));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd7b))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd8a, 0);

		if ( ((tmp = div16(ds_readb(0xbd81))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd82,
				ds_writeb(0xbd89, 0));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd78))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd77,
			ds_writeb(0xbd7e,
			ds_writeb(0xbd84,
			ds_writeb(0xbd85, 0))));

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd80, 0);
		}

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1) ||
			((tmp = div16(ds_readb(0xbd80))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd7f,
				ds_writeb(0xbd86,
				ds_writeb(0xbd87, 0)));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd7a))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd7b,
			ds_writeb(0xbd82,
			ds_writeb(0xbd89,
			ds_writeb(0xbd8a, 0))));

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd80, 0);
		}

		if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1) ||
			((tmp = div16(ds_readb(0xbd80))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd81,
				ds_writeb(0xbd87,
				ds_writeb(0xbd88, 0)));
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd79))) == 15) || (tmp == 1)) {
		ds_writeb(0xbd80,
			ds_writeb(0xbd87, 0));
	}

	if ( ((tmp = div16(ds_readb(0xbd7e))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd84, 0);

		if ( ((tmp = div16(ds_readb(0xbd86))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd85, 0);
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd82))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd8a, 0);

		if ( ((tmp = div16(ds_readb(0xbd88))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd89, 0);
		}
	}

	if ( ((tmp = div16(ds_readb(0xbd80))) == 15) || (tmp == 1)) {

		ds_writeb(0xbd87, 0);
	}

	if ( ((tmp = div16(ds_readb(0xbd80))) == 15) || (tmp == 1) ||
		((tmp = div16(ds_readb(0xbd87))) == 15) || (tmp == 1)) {

		if ( ((tmp = div16(ds_readb(0xbd7f))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd86, 0);
		}
		if ( ((tmp = div16(ds_readb(0xbd81))) == 15) || (tmp == 1)) {

			ds_writeb(0xbd88, 0);
		}

	}
}

/* 0x56b */
/* Borlandified and identical */
void DNG_stub1(void)
{
	DNG_floor_ceil();
	move();
	DNG_turn();
	DNG_stub3();
	DNG_stub4();
	DNG_stub2();
	DNG_stub5();
}

/* 0x591 */
void DNG_stub2(void)
{

}

/* 0x5e5 */
void DNG_stub3(void)
{

}

/* 0x693 */
void DNG_draw_walls(signed short a1, signed short a2, signed short a3)
{

}

/* 0x82e */
void DNG_stub4(void)
{

}

/* 0x9ef*/
void DNG_stub5(void)
{

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

/* 0xaaa*/
void DNG_lights(void)
{

}


#if !defined(__BORLANDC__)
}
#endif

