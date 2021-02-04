/*
 *	Rewrite of DSA1 v3.02_de functions of seg039 (fight)
 *	Functions rewritten: 7/7 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg039.cpp
 */

#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg006.h"
#include "seg007.h"
#include "seg027.h"
#include "seg032.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   calculates the beeline
 *
 *          This function calculates the between two points on the chessboard.
 *          It is also used as a heuristic.
 *
 * \param   x1          x-coordinate of the first point
 * \param   y1          y-coordinate of the first point
 * \param   x2          x-coordinate of the second point
 * \param   y2          y-coordinate of the seconn point
 * \return              beeline between the two points
 */
signed short calc_beeline(signed short x1, signed short y1, signed short x2, signed short y2)
{
	return __abs__(x1 - x2) + __abs__(y1 - y2);
}


/**
 * \brief   get the type of the range weapon of a hero
 *
 * \param   hero        pointer to hero
 * \return              range weapon type {-1, 3, 4, 5}: -1 = none, 3 = shooting, 4 = throwing, 5 = spear weapon
 */
signed short FIG_get_range_weapon_type(Bit8u *hero)
{
	Bit8u *ptr;
	signed short retval = -1;
	signed short weapon;

	/* get equipped weapon of the hero and make a pointer to the entry of ITEMS.DAT */
	ptr = get_itemsdat((weapon = host_readw(hero + HERO_ITEM_RIGHT)));


	/* not a weapon */
	if (item_weapon(ptr)) {

		/* MagicStaffs or Fightstaffs are spears, but no range weapons */
		if (host_readb(ptr + 3) == 5 && weapon != 0x85 && weapon != 0x45) {

			retval = 5;

		} else if (host_readb(ptr + 3) == 7) {

			retval = 3;

		} else if (host_readb(ptr + 3) == 8) {

			retval = 4;
		}
	}

	return retval;
}

/**
 * \brief   fills an enemies sheet from a template
 *
 * \param   sheet_no    the number of the sheet
 * \param   enemy_id    the ID of the enemy (MONSTER.DAT)
 * \param   round       the fight round the enemy appears
 */
void fill_enemy_sheet(unsigned short sheet_no, signed char enemy_id, unsigned char round)
{

	Bit8u *monster;
	Bit8u *sheet;
	signed short i;

	/* calculate the pointers */
	monster = Real2Host(ds_readd(MONSTER_DAT_BUF)) + enemy_id * SIZEOF_MONSTER;
	sheet = p_datseg + ENEMY_SHEETS + sheet_no * SIZEOF_ENEMY_SHEET;

	/* erease the sheet */
	memset(sheet, 0, SIZEOF_ENEMY_SHEET);

	/* copy enemy id, gfx_id and RS to the sheet */
	host_writeb(sheet + ENEMY_SHEET_MON_ID, host_readb(monster + MONSTER_MON_ID));
	host_writeb(sheet + ENEMY_SHEET_GFX_ID, host_readb(monster + MONSTER_GFX_ID));
	host_writeb(sheet + ENEMY_SHEET_RS, host_readb(monster + MONSTER_RS));

	/* roll attributes  and save them to the sheet */
	for (i = 0; i < 7; i++) {

		/* UGLY: a = b = dice_template() */
		host_writebs(sheet + i * 2 + ENEMY_SHEET_ATTRIB_ORIG,
			host_writebs(sheet + i * 2 + ENEMY_SHEET_ATTRIB,
				dice_template(host_readw(monster + i * 2 + MONSTER_ATTRIB))));
	}

	/* roll out LE and save it to the sheet */
	host_writew(sheet + ENEMY_SHEET_LE_ORIG, dice_template(host_readw(monster + MONSTER_LE)));
	/* recalculate LE = LE / 6 * 5; */
	host_writew(sheet + ENEMY_SHEET_LE_ORIG, host_readws(sheet + ENEMY_SHEET_LE_ORIG) / 6 * 5);
	/* copy LE*/
	host_writew(sheet + ENEMY_SHEET_LE, host_readw(sheet + ENEMY_SHEET_LE_ORIG));

	/* roll out AE and save it to the sheet */
	host_writews(sheet + ENEMY_SHEET_AE_ORIG, host_writews(sheet + ENEMY_SHEET_AE, dice_template(host_readw(monster + MONSTER_AE))));

	/* roll out MR  and save it */
	host_writeb(sheet + ENEMY_SHEET_MR,
		(signed char)dice_template(host_readw(monster + MONSTER_MR)));

	/* Terrible hack:
		if the current fight is 188, set MR to 5 (Travel-Event 84),
		if the current fight is 192, and the enemy is no "Orkchampion" then set the 'tied' status bit */
	if (ds_readw(CURRENT_FIG_NO) == 188) {

		host_writeb(sheet + ENEMY_SHEET_MR, 5);

	} else if ((ds_readw(CURRENT_FIG_NO) == 192) && (host_readb(sheet + ENEMY_SHEET_MON_ID) != 0x48)) {
		or_ptr_bs(sheet + ENEMY_SHEET_STATUS1, 0x20); /* set 'tied' status bit */

	}

	host_writeb(sheet + ENEMY_SHEET_FIRSTAP, host_readb(monster + MONSTER_FIRSTAP));
	host_writeb(sheet + ENEMY_SHEET_ATTACKS, host_readb(monster + MONSTER_ATTACKS));
	host_writeb(sheet + ENEMY_SHEET_AT, host_readb(monster + MONSTER_AT));
	host_writeb(sheet + ENEMY_SHEET_PA, host_readb(monster + MONSTER_PA));
	host_writew(sheet + ENEMY_SHEET_DAM1, host_readw(monster + MONSTER_DAM1));
	host_writew(sheet + ENEMY_SHEET_DAM2, host_readw(monster + MONSTER_DAM2));

	host_writeb(sheet + ENEMY_SHEET_BP_ORIG, host_readb(monster + MONSTER_BP));

	if (host_readbs(sheet + ENEMY_SHEET_BP_ORIG) > 10)
		host_writeb(sheet + ENEMY_SHEET_BP_ORIG, 10);

	host_writeb(sheet + ENEMY_SHEET_MAGIC, host_readb(monster + MONSTER_MAGIC));
	host_writeb(sheet + ENEMY_SHEET_MAG_ID, host_readb(monster + MONSTER_MAG_ID));

	/* bogus this value is 0x00 or 0x20 */
	and_ptr_bs(sheet + ENEMY_SHEET_STATUS1, 0xfe); /* unset 'dead' status bit */

	host_writeb(sheet + ENEMY_SHEET_FIGHTER_ID, 0xff);
	host_writeb(sheet + ENEMY_SHEET_LEVEL, host_readb(monster + MONSTER_LEVEL));
	host_writeb(sheet + ENEMY_SHEET_SIZE, host_readb(monster + MONSTER_SIZE));
	host_writeb(sheet + ENEMY_SHEET_IS_ANIMAL, host_readb(monster + MONSTER_IS_ANIMAL));
	host_writeb(sheet + ENEMY_SHEET_ROUND_APPEAR, round);

	host_writeb(sheet + ENEMY_SHEET_VIEWDIR,
		host_readb(Real2Host(ds_readd(CURRENT_FIGHT)) + sheet_no * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_VIEWDIR));

	host_writeb(sheet + ENEMY_SHEET_SHOTS, host_readb(monster + MONSTER_SHOTS));
	host_writew(sheet + ENEMY_SHEET_SHOT_DAM, host_readw(monster + MONSTER_SHOT_DAM));
	host_writeb(sheet + ENEMY_SHEET_THROWS, host_readb(monster + MONSTER_THROWS));
	host_writew(sheet + ENEMY_SHEET_THROW_DAM, host_readw(monster + MONSTER_THROW_DAM));
	host_writeb(sheet + ENEMY_SHEET_LE_FLEE, host_readb(monster + MONSTER_LE_FLEE));

	/* Another hack:
		If the current fight == 94 and the enemy is "Kultist", set the 'scared' status bit */
	if ((ds_readw(CURRENT_FIG_NO) == 94) && (host_readb(sheet + ENEMY_SHEET_MON_ID) == 0x38)) {
		/* Kultist will flee */
		or_ptr_bs(sheet + ENEMY_SHEET_STATUS2, 0x4); /* set 'scared' status bit */

	}
}

/**
 * \brief   places an object on the chessboard
 *
 * \param   x           X-Coordinate
 * \param   y           Y-Coordinate
 * \param   object      object ID
 * \param   type        typus for heros, monster_id for enemies
 * \param   dir         looking direction
 * \return              1 if the placement was successful or 0 if not.
 */
unsigned short place_obj_on_cb(signed short x, signed short y, signed short object, signed char type, signed char dir)
{

	signed short i;

	/* check if an object is already on that field
		check if the object no is valid */
	if ((get_cb_val(x, y) > 0) || (object < 0)) {
		return 0;
	}

	/* check if the object is decoration */
	if (object >= 50) {
		if (type == 57 || type == 56 || type == 62) {
			FIG_set_cb_field(y + 1, x, object);
			FIG_set_cb_field(y + 1, x - 1, object);
			FIG_set_cb_field(y, x - 1, object);
		} else if (type == 9) {
				FIG_set_cb_field(y, x + 1, object);
				FIG_set_cb_field(y - 1, x, object);
		} else if (type == 43 || type == 44 || type == 48 ||
				type == 49 || type == 50 || type == 51 ||
				type == 52 || type == 53 || type == 54 ||
				type == 55) {

				FIG_set_cb_field(y + 1, x, object);
		} else if (type == 60) {
			for (i = 0; i < 7; i++)
				FIG_set_cb_field(y + i, x, object);
		} else if (type == 61) {
			for (i = 0; i < 7; i++)
				FIG_set_cb_field(y, x + i, object);
		}
	} else {
		/* if object is an enemy and needs 2 fields */
		if (object >= 10 &&
			is_in_byte_array(type, p_datseg + TWO_FIELDED_SPRITE_ID))
		{

			/* check if field is empty */
			if ((get_cb_val(x + ds_readws(GFXTAB_TWOFIELDED_EXTRA_CB + dir * 4), y + ds_readws((GFXTAB_TWOFIELDED_EXTRA_CB + 2) + dir * 4)) != 0) ||
				(y + ds_readws((GFXTAB_TWOFIELDED_EXTRA_CB + 2) + dir * 4) < 0) ||
				(y + ds_readws((GFXTAB_TWOFIELDED_EXTRA_CB + 2) + dir * 4) > 23) ||
				(x + ds_readws(GFXTAB_TWOFIELDED_EXTRA_CB + dir * 4) < 0 ||
				(x + ds_readws(GFXTAB_TWOFIELDED_EXTRA_CB + dir * 4) > 23)))
			{
				return 0;
			}

			FIG_set_cb_field(y + ds_readws((GFXTAB_TWOFIELDED_EXTRA_CB + 2) + dir * 4),
				x + ds_readws(GFXTAB_TWOFIELDED_EXTRA_CB + dir * 4),
					object + 20);
		}
	}

	/* set the object to the chessboard */
	FIG_set_cb_field(y, x, object);

	return 1;
}

/**
 * \brief   load the sprites from monsters
 *
 * \param   ptr         pointer to a monster datasheet
 * \param   x           x-coordinate on the chessboard
 * \param   y           y-coordinate on the chessboard
 */
void FIG_load_enemy_sprites(Bit8u *ptr, signed short x, signed short y)
{
	struct nvf_desc nvf;
	signed short l1;

	ds_writew(FIG_LIST_ELEM, ds_readbs(GFXTAB_FIGURES_MAIN + host_readbs(ptr + ENEMY_SHEET_GFX_ID) * 5));
	ds_writeb((FIG_LIST_ELEM+FIGHTER_NVF_NO), host_readbs(ptr + ENEMY_SHEET_VIEWDIR));
	ds_writeb((FIG_LIST_ELEM+FIGHTER_CBX), (signed char)x);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_CBY), (signed char)y);

	ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETX),
		ds_readb(GFXTAB_OFFSETS_MAIN + host_readbs(ptr + ENEMY_SHEET_GFX_ID) * 10 + host_readbs(ptr + ENEMY_SHEET_VIEWDIR) * 2));

	ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETY),
		ds_readb((GFXTAB_OFFSETS_MAIN + 1) + host_readbs(ptr + ENEMY_SHEET_GFX_ID) * 10 + host_readbs(ptr + ENEMY_SHEET_VIEWDIR) * 2));

	if (is_in_byte_array(host_readbs(ptr + ENEMY_SHEET_GFX_ID), p_datseg + TWO_FIELDED_SPRITE_ID)) {
		/* sprite uses two fields */

		ds_writeb((FIG_LIST_ELEM+FIGHTER_X1), ds_readbs(GFXTAB_TWOFIELDED_X1 + host_readbs(ptr + ENEMY_SHEET_VIEWDIR)));
		ds_writeb((FIG_LIST_ELEM+FIGHTER_X2), ds_readbs(GFXTAB_TWOFIELDED_X2 + host_readbs(ptr + ENEMY_SHEET_VIEWDIR)));

		/* TODO: b = ++a; */
		ds_writeb((FIG_LIST_ELEM+FIGHTER_TWOFIELDED), ds_writeb(FIG_TWOFIELDED_COUNT, ds_readb(FIG_TWOFIELDED_COUNT) + 1));
	} else {
		/* sprite uses one field */
		ds_writeb((FIG_LIST_ELEM+FIGHTER_X1), 0);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_X2), 0x1f);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_TWOFIELDED), -1);
	}

	ds_writeb((FIG_LIST_ELEM+FIGHTER_Y1), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_Y2), 0x27);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_HEIGHT), 0x28);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_WIDTH), 0x20);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_MONSTER), 1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_SPRITE_NO), host_readbs(ptr + ENEMY_SHEET_GFX_ID)); /* gfx_set_id */
	ds_writeb((FIG_LIST_ELEM+FIGHTER_RELOAD), -1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_WSHEET), -1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_SHEET), -1);
	ds_writefp((FIG_LIST_ELEM+FIGHTER_GFXBUF), ds_readfp(FIGHTOBJ_BUF_SEEK_PTR)); /* ->prev */
	ds_writeb((FIG_LIST_ELEM+FIGHTER_OBJ_ID), 0); /* ->next */

	add_ds_fp(FIGHTOBJ_BUF_SEEK_PTR, 0x508);
	sub_ds_ds(FIGHTOBJ_BUF_FREESPACE, 0x508);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_Z), 0x63);

	/* check presence in the first round */
	ds_writeb((FIG_LIST_ELEM+FIGHTER_VISIBLE), host_readb(ptr + ENEMY_SHEET_ROUND_APPEAR) == 0 ? 1 : 0);

	if (is_in_byte_array(host_readb(ptr + ENEMY_SHEET_GFX_ID), p_datseg + TWO_FIELDED_SPRITE_ID)) {

		nvf.src = Real2Host(load_fight_figs(ds_readw(FIG_LIST_ELEM)));
		nvf.dst = Real2Host(ds_readd((FIG_LIST_ELEM+FIGHTER_GFXBUF)));
		nvf.no = ds_readbs((FIG_LIST_ELEM+FIGHTER_NVF_NO));
		nvf.type = 0;
		nvf.width = (Bit8u*)&l1;
		nvf.height = (Bit8u*)&l1;
		process_nvf(&nvf);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_RELOAD), 0);
	}

	host_writeb(ptr + ENEMY_SHEET_FIGHTER_ID, FIG_add_to_list(-1));

	if (is_in_byte_array(host_readb(ptr + ENEMY_SHEET_GFX_ID), p_datseg + TWO_FIELDED_SPRITE_ID)) {

		ds_writeb((FIG_LIST_ELEM+FIGHTER_CBX), x + ds_readbs(GFXTAB_TWOFIELDED_EXTRA_CB + host_readbs(ptr + ENEMY_SHEET_VIEWDIR) * 4));
		ds_writeb((FIG_LIST_ELEM+FIGHTER_CBY), y + ds_readbs((GFXTAB_TWOFIELDED_EXTRA_CB + 2) + host_readbs(ptr + ENEMY_SHEET_VIEWDIR) * 4));

		add_ds_bs((FIG_LIST_ELEM+FIGHTER_OFFSETX), ds_readbs(GFXTAB_TWOFIELDED_EXTRA_OX + host_readbs(ptr + ENEMY_SHEET_VIEWDIR)));
		add_ds_bs((FIG_LIST_ELEM+FIGHTER_OFFSETY), ds_readbs(GFXTAB_TWOFIELDED_EXTRA_OY + host_readbs(ptr + ENEMY_SHEET_VIEWDIR)));
		ds_writeb((FIG_LIST_ELEM+FIGHTER_X1), ds_readb(GFXTAB_TWOFIELDED_EXTRA_X1 + host_readbs(ptr + ENEMY_SHEET_VIEWDIR)));
		ds_writeb((FIG_LIST_ELEM+FIGHTER_X2), ds_readb(GFXTAB_TWOFIELDED_EXTRA_X2 + host_readbs(ptr + ENEMY_SHEET_VIEWDIR)));
		ds_writeb((FIG_LIST_ELEM+FIGHTER_Y1), 0);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_Y2), 0x27);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_MONSTER), 1);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_Z), 10);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_TWOFIELDED), ds_readb(FIG_TWOFIELDED_COUNT) + 20);
		ds_writeb(FIG_TWOFIELDED_TABLE + ds_readbs(FIG_TWOFIELDED_COUNT), FIG_add_to_list(-1));
	}
}

void FIG_init_enemies(void)
{
	signed short i;
	signed short x;
	signed short y;

	/* Cleanup the old enemy tables */
	for (i = 0; i < 20; i++) {

		if (ds_readbs(ENEMY_SHEETS + ENEMY_SHEET_FIGHTER_ID + i * SIZEOF_ENEMY_SHEET) != -1) {

			FIG_remove_from_list(ds_readbs(ENEMY_SHEETS + ENEMY_SHEET_FIGHTER_ID + i * SIZEOF_ENEMY_SHEET), 0);

			ds_writeb(ENEMY_SHEETS + ENEMY_SHEET_FIGHTER_ID + i * SIZEOF_ENEMY_SHEET, -1);
		}
		or_ds_bs((ENEMY_SHEETS + ENEMY_SHEET_STATUS1) + i * SIZEOF_ENEMY_SHEET, 1); /* set 'dead' status bit */
	}

	ds_writew(NR_OF_ENEMIES, 0);


	/* Fill the tables with new values */
	for (i = 0; i < 20; i++) {

		if (host_readb(Real2Host(ds_readd(CURRENT_FIGHT)) + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_ID) != 0) {

			fill_enemy_sheet(i,
				host_readb(Real2Host(ds_readd(CURRENT_FIGHT)) + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_ID),
				host_readb(Real2Host(ds_readd(CURRENT_FIGHT)) + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_ROUND_APPEAR));

			inc_ds_ws(NR_OF_ENEMIES);
		}
	}

	/* place the enemies on the chessboard */
	for (i = 0; i < ds_readws(NR_OF_ENEMIES); i++) {

		x = host_readbs(Real2Host(ds_readd(CURRENT_FIGHT)) + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_X);
		y = host_readbs(Real2Host(ds_readd(CURRENT_FIGHT)) + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_Y);


		/* place only the enemies from round 0 */
		if (!host_readbs(Real2Host(ds_readd(CURRENT_FIGHT)) + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_ROUND_APPEAR)) {

			place_obj_on_cb(x, y, i + 10,
				ds_readbs(i * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS + ENEMY_SHEET_GFX_ID)),
				host_readb(Real2Host(ds_readd(CURRENT_FIGHT)) + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_VIEWDIR));
		}

		/* load the sprites */
#if !defined(__BORLANDC__)
		FIG_load_enemy_sprites(Real2Host(RealMake(datseg, ENEMY_SHEETS + i * SIZEOF_ENEMY_SHEET)), x, y);
#else
		FIG_load_enemy_sprites((Bit8u*)&((struct enemy_sheets*)(p_datseg +  ENEMY_SHEETS))[i], x, y);
#endif
	}
}

void FIG_init_heroes(void)
{
	Bit8u *hero;
	signed short cb_x;
	signed short cb_y;
	signed short l_si;
	signed short l_di; /* player char no */

	for (l_si = 0; l_si <= 6; l_si++) {

		if (host_readbs(get_hero(l_si) + HERO_FIGHTER_ID) != -1) {

			FIG_remove_from_list(host_readb(get_hero(l_si) + HERO_FIGHTER_ID), 0);
			host_writeb(get_hero(l_si) + HERO_FIGHTER_ID, 0xff);
		}
	}

	for (l_si = 0; l_si <= 6; l_si++) {
		hero = get_hero(l_si);

		if (host_readb(hero + HERO_TYPE) == HERO_TYPE_NONE)
			continue;
		/* check group */
		if (host_readb(hero + HERO_GROUP_NO) != ds_readb(CURRENT_GROUP))
			continue;

		host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);
		host_writeb(hero + HERO_ENEMY_ID, 0);

		/* FINAL FIGHT */
		if (ds_readw(CURRENT_FIG_NO) == 192) {
			if (hero == Real2Host(ds_readd(MAIN_ACTING_HERO))) {
				cb_x = host_readbs(Real2Host(ds_readd(CURRENT_FIGHT)) + FIGHT_PLAYERS_X);
				cb_y = host_readbs(Real2Host(ds_readd(CURRENT_FIGHT)) + FIGHT_PLAYERS_Y);
				host_writeb(hero + HERO_VIEWDIR,
					host_readb(Real2Host(ds_readd(CURRENT_FIGHT)) + FIGHT_PLAYERS_VIEWDIR));
			} else {
				do {
					l_di = random_schick(6);

					cb_x = host_readbs(Real2Host(ds_readd(CURRENT_FIGHT)) + l_di * SIZEOF_FIGHT_PLAYER + FIGHT_PLAYERS_X);
					cb_y = host_readbs(Real2Host(ds_readd(CURRENT_FIGHT)) + l_di * SIZEOF_FIGHT_PLAYER + FIGHT_PLAYERS_Y);
					host_writeb(hero + HERO_VIEWDIR,
						host_readb(Real2Host(ds_readd(CURRENT_FIGHT)) + l_di * SIZEOF_FIGHT_PLAYER + FIGHT_PLAYERS_VIEWDIR));
				} while (get_cb_val(cb_x, cb_y) != 0);
			}
		} else {
			cb_x = host_readbs(Real2Host(ds_readd(CURRENT_FIGHT)) + FIGHT_PLAYERS_X + SIZEOF_FIGHT_PLAYER * l_si);
			cb_y = host_readbs(Real2Host(ds_readd(CURRENT_FIGHT)) + FIGHT_PLAYERS_Y + SIZEOF_FIGHT_PLAYER * l_si);
			/* Direction */
			host_writeb(hero + HERO_VIEWDIR, host_readb(Real2Host(ds_readd(CURRENT_FIGHT)) + FIGHT_PLAYERS_VIEWDIR + SIZEOF_FIGHT_PLAYER * l_si));
		}

		/* heros sleep until they appear */
		if (host_readb(Real2Host(ds_readd(CURRENT_FIGHT)) + l_si * SIZEOF_FIGHT_PLAYER + FIGHT_PLAYERS_ROUND_APPEAR) != 0) {
			if (!hero_dead(hero))
				or_ptr_bs(hero + HERO_STATUS1, 2); /* set 'sleep' status bit */
		}

		place_obj_on_cb(cb_x, cb_y, l_si + 1,
			host_readb(hero + HERO_TYPE), host_readb(hero + HERO_VIEWDIR));

		l_di = FIG_get_range_weapon_type(hero);

		if (l_di != -1) {
			ds_writeb((FIG_LIST_ELEM+FIGHTER_NVF_NO),
				ds_readb((NVFTAB_FIGURES_RANGEWEAPON - 12) + host_readbs(hero + HERO_SPRITE_NO) * 12 + l_di * 4 + host_readbs(hero + HERO_VIEWDIR)));
		} else {
			ds_writeb((FIG_LIST_ELEM+FIGHTER_NVF_NO), host_readb(hero + HERO_VIEWDIR));
		}

		ds_writew(FIG_LIST_ELEM, ds_readbs(GFXTAB_FIGURES_MAIN + host_readbs(hero + HERO_SPRITE_NO) * 5));
		ds_writeb((FIG_LIST_ELEM+FIGHTER_CBX), (signed char)cb_x);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_CBY), (signed char)cb_y);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETX), 0);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETY), 0);

		if (hero_dead(hero)) {
			/* hero is dead */
			ds_writeb((FIG_LIST_ELEM+FIGHTER_NVF_NO),
				ds_readb(NVFTAB_FIGURES_DEAD + host_readbs(hero + HERO_SPRITE_NO) * 2));
			ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETX),
				ds_readb((GFXTAB_OFFSETS_MAIN + 8) + host_readbs(hero + HERO_SPRITE_NO) * 10));
			ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETY),
				ds_readb((GFXTAB_OFFSETS_MAIN + 9) + host_readbs(hero + HERO_SPRITE_NO) * 10));
		} else if (hero_asleep(hero) || hero_unconscious(hero)) {
			/* hero is asleep or unconscious */
			ds_writeb((FIG_LIST_ELEM+FIGHTER_NVF_NO),
				ds_readb(NVFTAB_FIGURES_UNCONSCIOUS + host_readbs(hero + HERO_SPRITE_NO) * 2) + host_readbs(hero + HERO_VIEWDIR));

			ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETX),
				ds_readbs(GFXTAB_OFFSETS_UNCONSCIOUS + host_readbs(hero + HERO_SPRITE_NO) * 8 + host_readbs(hero + HERO_VIEWDIR) * 2));
			ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETY),
				ds_readbs(GFXTAB_OFFSETS_UNCONSCIOUS + 1 + host_readbs(hero + HERO_SPRITE_NO) * 8 + host_readbs(hero + HERO_VIEWDIR) * 2));
		}


		ds_writeb((FIG_LIST_ELEM+FIGHTER_HEIGHT), 40);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_WIDTH), 32);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_X1), 0);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_Y1), 0);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_X2), 31);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_Y2), 39);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_MONSTER), 2);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_SPRITE_NO), host_readb(hero + HERO_SPRITE_NO));
		ds_writeb((FIG_LIST_ELEM+FIGHTER_RELOAD), 0xff);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_WSHEET), 0xff);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_SHEET), 0xff);
		ds_writefp((FIG_LIST_ELEM+FIGHTER_GFXBUF), ds_readfp(FIGHTOBJ_BUF_SEEK_PTR));
		ds_writeb((FIG_LIST_ELEM+FIGHTER_OBJ_ID), 0);
		add_ds_fp(FIGHTOBJ_BUF_SEEK_PTR, 0x508);
		sub_ds_ds(FIGHTOBJ_BUF_FREESPACE, 0x508);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_Z), 0x63);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_VISIBLE), 1);
		ds_writeb((FIG_LIST_ELEM+FIGHTER_TWOFIELDED), 0xff);


		host_writeb(get_hero(l_si) + HERO_FIGHTER_ID, FIG_add_to_list(-1));
	}
}

#if !defined(__BORLANDC__)
}
#endif
