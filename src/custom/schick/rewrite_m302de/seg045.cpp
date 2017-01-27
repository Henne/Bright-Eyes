/*
 *	Rewrite of DSA1 v3.02_de functions of seg045 (fight helper)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg045.cpp
 */

#include "v302de.h"

#include "seg002.h"
#include "seg006.h"
#include "seg038.h"
#include "seg039.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void seg045_0000(signed short fighter_id, signed short type, signed short a3)
{
	signed short obj_x;
	signed short obj_y;
	struct nvf_desc nvf;

	FIG_search_obj_on_cb(fighter_id, &obj_x, &obj_y);

#if !defined(__BORLANDC__)
	/* BE-fix */
	obj_x = host_readws((Bit8u*)&obj_x);
	obj_y = host_readws((Bit8u*)&obj_y);
#endif

	ds_writew(FIG_LIST_ELEM, 0);

	ds_writeb((FIG_LIST_ELEM+2), ds_readb(type * 2 + GFXTAB_SHOTBOLT_NVFNO));

	if (type != 2) {
		ds_writeb((FIG_LIST_ELEM+2), ds_readbs((FIG_LIST_ELEM+2)) + a3);
	}

	if (a3 == 0) {
		obj_x += 2;
	}

	ds_writebs((FIG_LIST_ELEM+3), (signed char)obj_x);
	ds_writebs((FIG_LIST_ELEM+4), (signed char)obj_y);
	ds_writeb((FIG_LIST_ELEM+5), ds_readb(GFXTAB_SHOTBOLT_OX + type * 8 + a3 * 2));
	ds_writeb((FIG_LIST_ELEM+6), ds_readb(GFXTAB_SHOTBOLT_OY + type * 8 + a3 * 2));
	ds_writeb((FIG_LIST_ELEM+7), ds_readb(GFXTAB_SHOTBOLT_HEIGHT + type * 2));
	ds_writeb((FIG_LIST_ELEM+8), ds_readb(GFXTAB_SHOTBOLT_WIDTH + type * 2));
	ds_writeb((FIG_LIST_ELEM+9), 0);
	ds_writeb((FIG_LIST_ELEM+10), 0);

	ds_writebs((FIG_LIST_ELEM+11), ds_readbs(GFXTAB_SHOTBOLT_WIDTH + type * 2) - 1);
	ds_writebs((FIG_LIST_ELEM+12), ds_readbs(GFXTAB_SHOTBOLT_HEIGHT + type * 2) - 1);
	ds_writeb((FIG_LIST_ELEM+21), 0);
	ds_writeb((FIG_LIST_ELEM+13), 0);
	ds_writeb((FIG_LIST_ELEM+15), -1);
	ds_writeb((FIG_LIST_ELEM+14), -1);
	ds_writed((FIG_LIST_ELEM+23), ds_readd(FIG_SHOT_BOLT_BUF));
	ds_writeb((FIG_LIST_ELEM+17), 100);
	ds_writeb((FIG_LIST_ELEM+18), 0);
	ds_writeb((FIG_LIST_ELEM+19), -1);

	nvf.dst = Real2Host(ds_readd(FIG_SHOT_BOLT_BUF));
	nvf.src = Real2Host(ds_readd(SPELLOBJ_NVF_BUF));
	nvf.no = ds_readbs((FIG_LIST_ELEM+2));
	nvf.type = 0;
	nvf.width = (Bit8u*)&obj_x;
	nvf.height = (Bit8u*)&obj_x;

	process_nvf(&nvf);

	ds_writeb(FIG_SHOT_BOLT_ID, FIG_add_to_list(-1));
}

void FIG_remove_smth(void)
{
	FIG_remove_from_list(ds_readb(FIG_SHOT_BOLT_ID), 0);
	ds_writeb(FIG_SHOT_BOLT_ID, 0xff);
}

/**
 * \brief   copies chunks of 3 bytes
 *
 * \param   dst         pointer to destination
 * \param   src         pointer to sources
 * \param   term        termination sign
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

signed short seg045_01a0(signed short a1, signed short a2, signed short fighter_id1, signed short fighter_id2, signed short a5)
{
	signed short i;
	Bit8u *ptr;
	signed short id1_x;
	signed short id1_y;
	signed short id2_x;
	signed short id2_y;
	signed short beeline;

	FIG_search_obj_on_cb(fighter_id2, &id2_x, &id2_y);
	FIG_search_obj_on_cb(fighter_id1, &id1_x, &id1_y);

#if !defined(__BORLANDC__)
	/* BE-fix */
	id1_x = host_readws((Bit8u*)&id1_x);
	id1_y = host_readws((Bit8u*)&id1_y);
	id2_x = host_readws((Bit8u*)&id2_x);
	id2_y = host_readws((Bit8u*)&id2_y);
#endif

	beeline = calc_beeline(id1_x, id1_y, id2_x, id2_y);

	if (beeline <= 1) {
		return 0;
	}

	ptr = p_datseg + (0xd8ce + 1) + a1 * 0xf3;
	ds_writeb(0xd8ce + a1 * 0xf3, 0);
	ds_writeb((0xd8ce + 242) + a1 * 0xf3, 0);

	for (i = 0; beeline - 1 > i; i++) {
		ptr += FIG_copy_it(ptr, Real2Host(host_readd(Real2Host(ds_readd(ANITAB_SHOTBOLT_INDEX + a2 * 4)) + a5 * 4)), -1);
	}
	host_writeb(ptr, -1);

	seg045_0000(fighter_id1, a2, a5);

	return 1;
}

struct dummy2 {
	signed char a[2];
};

struct dummy4 {
	signed short a[2];
};

void seg045_0273(signed short x, signed short y, signed short spell_ani_id)
{
	unsigned short height;
	unsigned short width;

	/* TODO: some kind of initialized structure */
	struct dummy2 a = *(struct dummy2*)(p_datseg + ANITAB_SPELL_NVFNO);
	struct dummy4 b = *(struct dummy4*)(p_datseg + ANITAB_SPELL_OX);
	struct dummy4 c = *(struct dummy4*)(p_datseg + ANITAB_SPELL_OY);

	struct nvf_desc nvf;

	nvf.dst = Real2Host(ds_readd(FIG_SPELLGFX_BUF));
	nvf.src = Real2Host(ds_readd(SPELLOBJ_NVF_BUF));

	nvf.no = a.a[spell_ani_id - 1];
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	height = host_readws((Bit8u*)&height);
	width = host_readws((Bit8u*)&width);
#endif

	ds_writew(FIG_LIST_ELEM, 0);
	ds_writeb((FIG_LIST_ELEM+2), a.a[spell_ani_id - 1]);
	ds_writeb((FIG_LIST_ELEM+3), (signed char)x);
	ds_writeb((FIG_LIST_ELEM+4), (signed char)y);

	ds_writeb((FIG_LIST_ELEM+5), (unsigned char)b.a[spell_ani_id - 1]);
	ds_writeb((FIG_LIST_ELEM+6), (unsigned char)c.a[spell_ani_id - 1]);

	ds_writeb((FIG_LIST_ELEM+7), (unsigned char)height);
	ds_writeb((FIG_LIST_ELEM+8), (unsigned char)width);
	ds_writeb((FIG_LIST_ELEM+9), 0);
	ds_writeb((FIG_LIST_ELEM+10), 0);
	ds_writeb((FIG_LIST_ELEM+11), (unsigned char)(width - 1));
	ds_writeb((FIG_LIST_ELEM+12), (unsigned char)(height - 1));
	ds_writeb((FIG_LIST_ELEM+21), 0);
	ds_writeb((FIG_LIST_ELEM+13), 0);
	ds_writeb((FIG_LIST_ELEM+15), -1);
	ds_writeb((FIG_LIST_ELEM+14), -1);
	ds_writed((FIG_LIST_ELEM+23), ds_readd(FIG_SPELLGFX_BUF));
	ds_writeb((FIG_LIST_ELEM+17), 99);
	ds_writeb((FIG_LIST_ELEM+18), 0);
	ds_writeb((FIG_LIST_ELEM+19), -1);
	ds_writeb(FIG_SPELLGFX_ID, FIG_add_to_list(-1));
}

void FIG_remove_smth2(void)
{
	FIG_remove_from_list(ds_readb(FIG_SPELLGFX_ID), 0);
	ds_writeb(FIG_SPELLGFX_ID, 0xff);
}

void seg045_0394(signed short a1, Bit8u *hero, signed short spell_ani_id)
{
	Bit8u *ptr;
	signed short x;
	signed short y;

	/* search the target on the chessboard */
	FIG_search_obj_on_cb(host_readbs(hero + HERO_ENEMY_ID), &x, &y);

#if !defined(__BORLANDC__)
	/* BE-fix */
	x = host_readws((Bit8u*)&x);
	y = host_readws((Bit8u*)&y);
#endif

	ptr = p_datseg + a1 * 0xf3 + (0xd8ce + 1);

	ds_writeb(0xd8ce + a1 * 0xf3, 0);
	ds_writeb((0xd8ce + 242) + a1 * 0xf3, -1);

	/* copy the ani sequence and terminate it */
	ptr += FIG_copy_it(ptr, Real2Host(ds_readd((ANITAB_SPELL_INDEX - 4) + spell_ani_id * 4)), -1);
	host_writeb(ptr, -1);

	seg045_0273(x, y, spell_ani_id);
}

void seg045_041b(signed short a1, Bit8u *enemy, signed short spell_ani_id)
{
	Bit8u *ptr;
	signed short x;
	signed short y;

#if !defined(__BORLANDC__)
	/* BE-fix */
	x = host_readws((Bit8u*)&x);
	y = host_readws((Bit8u*)&y);
#endif

	/* search the target on the chessboard */
	FIG_search_obj_on_cb(host_readbs(enemy + ENEMY_SHEET_ENEMY_ID), &x, &y);

	ptr = p_datseg + a1 * 0xf3 + (0xd8ce + 1);

	ds_writeb(0xd8ce + a1 * 0xf3, 0);
	ds_writeb((0xd8ce + 242) + a1 * 0xf3, -1);

	/* copy the ani sequence and terminate it */
	ptr += FIG_copy_it(ptr, Real2Host(ds_readd((ANITAB_SPELL_INDEX - 4) + spell_ani_id * 4)), -1);
	host_writeb(ptr, -1);

	seg045_0273(x, y, spell_ani_id);
}

#if !defined(__BORLANDC__)
}
#endif
