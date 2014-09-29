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
 * \brief	calculates the beeline
 *
 *		This function calculates the between two points on the chessboard.
 *		It is also used as a heuristic.
 * \param x1	x-coordinate of the first point
 * \param y1	y-coordinate of the first point
 * \param x2	x-coordinate of the second point
 * \param y2	y-coordinate of the seconn point
 *
 * \return		beeline between the two points
 */
signed short calc_beeline(signed short x1, signed short y1, signed short x2, signed short y2)
{
	return __abs__(x1 - x2) + __abs__(y1 - y2);
}


/**
 * \brief get the type of the range weapon of a hero
 * \param hero	pointer to hero
 *
 * \return range weapon type {-1, 3, 4, 5}: -1 = none, 3 = shooting, 4 = throwing, 5 = spear weapon
*/
signed short FIG_get_range_weapon_type(Bit8u *hero)
{
	Bit8u *ptr;
	signed short retval = -1;
	signed short weapon;

	/* get equipped weapon of the hero and make a pointer to the entry of ITEMS.DAT */
	ptr = get_itemsdat((weapon = host_readw(hero + 0x1c0)));


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
 * \brief fills an enemies sheet from a template
 * \param sheet_nr	the number of the sheet
 * \param enemy_id	the ID of the enemy (MONSTER.DAT)
 * \param round		the fight round the enemy appears
 */
void fill_enemy_sheet(unsigned short sheet_nr, signed char enemy_id, unsigned char round)
{

	Bit8u *temp;
	Bit8u *sheet;
	signed short i;

	/* calculate the pointers */
	temp = Real2Host(ds_readd(0xe125)) + enemy_id * 44;
	sheet = p_datseg + ENEMY_SHEETS + sheet_nr * 62;

	/* erease the sheet */
	memset(sheet, 0, 62);

	/* copy enemy id, gfx_id and RS to the sheet */
	host_writeb(sheet + 0, host_readb(temp + 0));
	host_writeb(sheet + 1, host_readb(temp + 1));
	host_writeb(sheet + 2, host_readb(temp + 2));

	/* roll attributes  and save them to the sheet */
	for (i = 0; i < 7; i++) {

		/* UGLY: a = b = dice_template() */
		host_writebs(sheet + i * 2 + 3,
			host_writebs(sheet + i * 2 + 4,
				dice_template(host_readw(temp + i * 2 + 3))));
	}

	/* roll out LE and save it to the sheet */
	host_writew(sheet + 0x11, dice_template(host_readw(temp + 0x11)));
	/* recalculate LE = LE / 6 * 5; */
	host_writew(sheet + 0x11, host_readws(sheet + 0x11) / 6 * 5);
	/* copy LE*/
	host_writew(sheet + 0x13, host_readw(sheet + 0x11));

	/* roll out AE and save it to the sheet */
	host_writews(sheet + 0x15, host_writews(sheet + 0x17, dice_template(host_readw(temp + 0x13))));

	/* roll out MR  and save it */
	host_writeb(sheet + 0x19,
		(signed char)dice_template(host_readw(temp + 0x15)));

	/* Terrible hack:
		if the current fight is 188, set MR to 5 (Travel-Event 84),
		if the current fight is 192, and the enemy
		is no "Orkchampion" then set a flag */
	if (ds_readw(CURRENT_FIG_NR) == 188) {

		host_writeb(sheet + 0x19, 5);

	} else if ((ds_readw(CURRENT_FIG_NR) == 192) && (host_readb(sheet) != 0x48)) {

			or_ptr_bs(sheet + 0x31, 0x20);

	}

	/* copy the first encounter AP */
	host_writeb(sheet + 0x1a, host_readb(temp + 0x17));
	/* copy the number of attacks */
	host_writeb(sheet + 0x1b, host_readb(temp + 0x18));
	/* copy AT value */
	host_writeb(sheet + 0x1c, host_readb(temp + 0x19));
	/* copy PA value */
	host_writeb(sheet + 0x1d, host_readb(temp + 0x1a));

	/* Damage first attack ? */
	host_writew(sheet + 0x1e, host_readw(temp + 0x1b));
	/* Damage second attack ? */
	host_writew(sheet + 0x20, host_readw(temp + 0x1d));

	/* copy BP */
	host_writeb(sheet + 0x22, host_readb(temp + 0x1f));

	/* set BP to 10 if greater */
	if (host_readbs(sheet + 0x22) > 10)
		host_writeb(sheet + 0x22, 10);

	/* copy imunnity against non-magicial weapons ? */
	host_writeb(sheet + 0x24, host_readb(temp + 0x20));
	/* copy ID Magicuser class ? */
	host_writeb(sheet + 0x25, host_readb(temp + 0x21));

	/* bogus this value is 0x00 or 0x20 */
	and_ptr_bs(sheet + 0x31, 0xfe);

	host_writeb(sheet + 0x26, 0xff);

	/* copy Level */
	host_writeb(sheet + 0x29, host_readb(temp + 0x22));
	/* copy size class */
	host_writeb(sheet + 0x34, host_readb(temp + 0x23));
	/* copy flags */
	host_writeb(sheet + 0x36, host_readb(temp + 0x24));

	/* set the fight round the enemy appears */
	host_writeb(sheet + 0x35, round);

	/* copy the looking direction */
	host_writeb(sheet + 0x27,
		host_readb(Real2Host(ds_readd(PTR_FIGHT_LST)) + sheet_nr * 5 + 0x19));

	/* copy number of ammo */
	host_writeb(sheet + 0x37, host_readb(temp + 0x25));
	/* copy damage of ammo */
	host_writew(sheet + 0x38, host_readw(temp + 0x26));

	/* copy number of thorowing weapons */
	host_writeb(sheet + 0x3a, host_readb(temp + 0x28));
	/* copy damage of thorowing weapons */
	host_writew(sheet + 0x3b, host_readw(temp + 0x29));

	/* copy LE fleeing value */
	host_writeb(sheet + 0x3d, host_readb(temp + 0x2b));

	/* Another hack:
		If the current fight == 94 and the enemy is "Kultist",
		set a flag */
	if ((ds_readw(CURRENT_FIG_NR) == 94) && (host_readb(sheet) == 0x38)) {

		or_ptr_bs(sheet + 0x32, 0x4);

	}
}

/**
 *	\brief		places an object on the chessboard
 *	\param	x	X-Coordinate
 *	\param	y	Y-Coordinate
 *	\param	object	object ID
 *	\param	type	typus for heros, monster_id for enemies
 *	\param	dir	looking direction
 *
 *	\return 1 if the placement was successful or 0 if not.
 */
unsigned short place_obj_on_cb(signed short x, signed short y, signed short object, signed char type, signed char dir)
{

	signed short i;

	/* check if an object is already on that field
		check if the object nr is valid */
	if ((get_cb_val(x, y) > 0) || (object < 0)) {
		return 0;
	}

	/* check if the object is decoration */
	if (object >= 50) {
		if (((signed char)type == 57) || ((signed char)type == 56) || ((signed char)type == 62)) {
			FIG_set_cb_field(y + 1, x, object);
			FIG_set_cb_field(y + 1, x - 1, object);
			FIG_set_cb_field(y, x - 1, object);
		} else if ((signed char)type == 9) {
				FIG_set_cb_field(y, x + 1, object);
				FIG_set_cb_field(y - 1, x, object);
		} else if ((signed char)type == 43 || (signed char)type == 44 || (signed char)type == 48 ||
				(signed char)type == 49 || (signed char)type == 50 || (signed char)type == 51 ||
				(signed char)type == 52 || (signed char)type == 53 || (signed char)type == 54 ||
				(signed char)type == 55) {

				FIG_set_cb_field(y + 1, x, object);
		} else if ((signed char)type == 60) {
			for (i = 0; i < 7; i++)
				FIG_set_cb_field(y + i, x, object);
		} else if ((signed char)type == 61) {
			for (i = 0; i < 7; i++)
				FIG_set_cb_field(y, x + i, object);
		}
	} else {
		/* if object is an enemy an needs 2 fields */
		if (object >= 10 &&
			is_in_byte_array((signed char)type, p_datseg + TWO_FIELDED_SPRITE_ID))
		{

			/* check if field is empty */
			if ((get_cb_val(x + ds_readws(0x6018 + dir * 4), y + ds_readws(0x601a + dir * 4)) != 0) ||
				(y + ds_readws(0x601a + dir * 4) < 0) ||
				(y + ds_readws(0x601a + dir * 4) > 23) ||
				(x + ds_readws(0x6018 + dir * 4) < 0 ||
				(x + ds_readws(0x6018 + dir * 4) > 23)))
			{
				return 0;
			}

			FIG_set_cb_field(y + ds_readws(0x601a + dir * 4),
				x + ds_readws(0x6018 + dir * 4),
					object + 20);
		}
	}

	/* set the object to the chessboard */
	FIG_set_cb_field(y, x, object);

	return 1;
}

/**
 * \brief load the sprites from monsters
 *
 * \param ptr	pointer to a monster datasheet
 * \param x	x-coordinate on the chessboard
 * \param y	y-coordinate on the chessboard
 */
void FIG_load_enemy_sprites(Bit8u *ptr, signed short x, signed short y)
{
	struct nvf_desc nvf;
	signed short l1;

	ds_writew(0xe066, ds_readbs(0x12c0 + host_readbs(ptr + 1) * 5));
	ds_writeb(0xe068, host_readbs(ptr + 0x27));
	ds_writeb(0xe069, (signed char)x);
	ds_writeb(0xe06a, (signed char)y);

	ds_writeb(0xe06b,
		ds_readb(0x1531 + host_readbs(ptr + 1) * 10 + host_readbs(ptr + 0x27) * 2));

	ds_writeb(0xe06c,
		ds_readb(0x1532 + host_readbs(ptr + 1) * 10 + host_readbs(ptr + 0x27) * 2));

	if (is_in_byte_array(host_readbs(ptr + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {
		/* sprite uses two fields */

		ds_writeb(0xe06f, ds_readbs(0x6030 + host_readbs(ptr + 0x27)));
		ds_writeb(0xe071, ds_readbs(0x6034 + host_readbs(ptr + 0x27)));

		/* TODO: b = ++a; */
		ds_writeb(0xe079, ds_writeb(0xe36f, ds_readb(0xe36f) + 1));
	} else {
		/* sprite uses one field */
		ds_writeb(0xe06f, 0);
		ds_writeb(0xe071, 0x1f);
		ds_writeb(0xe079, -1);
	}

	ds_writeb(0xe070, 0);
	ds_writeb(0xe072, 0x27);
	ds_writeb(0xe06d, 0x28);
	ds_writeb(0xe06e, 0x20);
	ds_writeb(0xe07b, 1);
	ds_writeb(0xe07c, host_readbs(ptr + 1)); /* gfx_set_id */
	ds_writeb(0xe073, -1);
	ds_writeb(0xe075, -1);
	ds_writeb(0xe074, -1);
	ds_writed(0xe07d, ds_readd(0xd86e)); /* ->prev */
	ds_writeb(0xe07a, 0); /* ->next */

	add_ds_ws(0xd86e, 0x508);
	sub_ds_ds(0xe370, 0x508);
	ds_writeb(0xe077, 0x63);

	/* check presence in the first round */
	ds_writeb(0xe078, host_readb(ptr + 0x35) == 0 ? 1 : 0);

	if (is_in_byte_array(host_readb(ptr + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {

		nvf.src = Real2Host(load_fight_figs(ds_readw(0xe066)));
		nvf.dst = Real2Host(ds_readd(0xe07d));
		nvf.nr = ds_readbs(0xe068);
		nvf.type = 0;
		nvf.width = (Bit8u*)&l1;
		nvf.height = (Bit8u*)&l1;
		process_nvf(&nvf);
		ds_writeb(0xe073, 0);
	}

	host_writeb(ptr + 0x26, FIG_add_to_list(-1));

	if (is_in_byte_array(host_readb(ptr + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {

		ds_writeb(0xe069, x + ds_readbs(0x6018 + host_readbs(ptr + 0x27) * 4));
		ds_writeb(0xe06a, y + ds_readbs(0x601a + host_readbs(ptr + 0x27) * 4));

		add_ds_bs(0xe06b, ds_readbs(0x6028 + host_readbs(ptr + 0x27)));
		add_ds_bs(0xe06c, ds_readbs(0x602c + host_readbs(ptr + 0x27)));
		ds_writeb(0xe06f, ds_readb(0x6038 + host_readbs(ptr + 0x27)));
		ds_writeb(0xe071, ds_readb(0x603c + host_readbs(ptr + 0x27)));
		ds_writeb(0xe070, 0);
		ds_writeb(0xe072, 0x27);
		ds_writeb(0xe07b, 1);
		ds_writeb(0xe077, 10);
		ds_writeb(0xe079, ds_readb(0xe36f) + 20);
		ds_writeb(0xe35a + ds_readbs(0xe36f), FIG_add_to_list(-1));
	}
}

void FIG_init_enemies(void)
{
	signed short i;
	signed short x;
	signed short y;

	/* Cleanup the old enemy tables */
	for (i = 0; i < 20; i++) {

		if (ds_readbs(ENEMY_SHEETS + 38 + i * 62) != -1) {

			FIG_remove_from_list(ds_readbs(ENEMY_SHEETS + 38 + i * 62), 0);

			ds_writeb(ENEMY_SHEETS + 38 + i * 62, -1);
		}

		or_ds_bs(0xd37c + i * 62, 1);
	}

	ds_writew(NR_OF_ENEMIES, 0);


	/* Fill the tables with new values */
	for (i = 0; i < 20; i++) {

		if (host_readb(Real2Host(ds_readd(PTR_FIGHT_LST)) + i * 5 + 0x16) != 0) {

			fill_enemy_sheet(i,
				host_readb(Real2Host(ds_readd(PTR_FIGHT_LST)) + i * 5 + 0x16),
				host_readb(Real2Host(ds_readd(PTR_FIGHT_LST)) + i * 5 + 0x1a));

			inc_ds_ws(NR_OF_ENEMIES);
		}
	}

	/* place the enemies on the chessboard */
	for (i = 0; i < ds_readws(NR_OF_ENEMIES); i++) {

		x = host_readbs(Real2Host(ds_readd(PTR_FIGHT_LST)) + i * 5 + 0x17);
		y = host_readbs(Real2Host(ds_readd(PTR_FIGHT_LST)) + i * 5 + 0x18);


		/* place only the enemies from round 0 */
		if (!host_readbs(Real2Host(ds_readd(PTR_FIGHT_LST)) + i * 5 + 0x1a)) {

			place_obj_on_cb(x, y, i + 10,
				ds_readbs(i * 62 + (ENEMY_SHEETS + 1)),
				host_readb(Real2Host(ds_readd(PTR_FIGHT_LST)) + i * 5 + 0x19));
		}

		/* load the sprites */
#if !defined(__BORLANDC__)
		FIG_load_enemy_sprites(Real2Host(RealMake(datseg, ENEMY_SHEETS + i * 62)), x, y);
#else
		FIG_load_enemy_sprites((Bit8u*)&((enemy_sheets*)(p_datseg +  ENEMY_SHEETS))[i], x, y);
#endif
	}
}

void FIG_init_heroes(void)
{
	Bit8u *hero;
	signed short cb_x;
	signed short cb_y;
	signed short l_si;
	signed short l_di;

	for (l_si = 0; l_si <= 6; l_si++) {

		if (host_readbs(get_hero(l_si) + 0x81) != -1) {

			FIG_remove_from_list(host_readb(get_hero(l_si) + 0x81), 0);
			host_writeb(get_hero(l_si) + 0x81, 0xff);
		}
	}

	for (l_si = 0; l_si <= 6; l_si++) {
		hero = get_hero(l_si);

		/* check typus */
		if (host_readb(hero + 0x21) == 0)
			continue;
		/* check group */
		if (host_readb(hero + 0x87) != ds_readb(CURRENT_GROUP))
			continue;

		/* these two are unknown */
		host_writeb(hero + 0x84, 10);
		host_writeb(hero + 0x86, 0);

		/* FINAL FIGHT */
		if (ds_readw(CURRENT_FIG_NR) == 192) {
			if (hero == Real2Host(ds_readd(0x3e20))) {
				cb_x = host_readbs(Real2Host(ds_readd(PTR_FIGHT_LST)) + 0x7a);
				cb_y = host_readbs(Real2Host(ds_readd(PTR_FIGHT_LST)) + 0x7b);
				host_writeb(hero + 0x82,
					host_readb(Real2Host(ds_readd(PTR_FIGHT_LST)) + 0x7c));
			} else {
				do {
					l_di = random_schick(6);

					cb_x = host_readbs(Real2Host(ds_readd(PTR_FIGHT_LST)) + l_di * 4 + 0x7a);
					cb_y = host_readbs(Real2Host(ds_readd(PTR_FIGHT_LST)) + l_di * 4 + 0x7b);
					host_writeb(hero + 0x82,
						host_readb(Real2Host(ds_readd(PTR_FIGHT_LST)) + l_di * 4 + 0x7c));
				} while (get_cb_val(cb_x, cb_y) != 0);
			}
		} else {
			cb_x = host_readbs(Real2Host(ds_readd(PTR_FIGHT_LST)) + 0x7a + 4 * l_si);
			cb_y = host_readbs(Real2Host(ds_readd(PTR_FIGHT_LST)) + 0x7b + 4 * l_si);
			/* Direction */
			host_writeb(hero + 0x82, host_readb(Real2Host(ds_readd(PTR_FIGHT_LST)) + 0x7c + 4 * l_si));
		}

		/* heros sleep until they appear */
		if (host_readb(Real2Host(ds_readd(PTR_FIGHT_LST)) + l_si * 4 + 0x7d) != 0) {
			if (!hero_dead(hero))
				or_ptr_bs(hero + 0xaa, 2);
		}

		place_obj_on_cb(cb_x, cb_y, l_si + 1,
			host_readb(hero + 0x21), host_readb(hero + 0x82));

		l_di = FIG_get_range_weapon_type(hero);

		if (l_di != -1) {
			ds_writeb(0xe068,
				ds_readb(0x10d0 + host_readbs(hero + 0x9b) * 12 + l_di * 4 + host_readbs(hero + 0x82)));
		} else {
			ds_writeb(0xe068, host_readb(hero + 0x82));
		}

		ds_writew(0xe066, ds_readbs(0x12c0 + host_readbs(hero + 0x9b) * 5));
		ds_writeb(0xe069, (signed char)cb_x);
		ds_writeb(0xe06a, (signed char)cb_y);
		ds_writeb(0xe06b, 0);
		ds_writeb(0xe06c, 0);

		if (hero_dead(hero)) {
			/* if hero is dead */
			ds_writeb(0xe068,
				ds_readb(0x1a13 + host_readbs(hero + 0x9b) * 2));
			ds_writeb(0xe06b,
				ds_readb(0x1539 + host_readbs(hero + 0x9b) * 10));
			ds_writeb(0xe06c,
				ds_readb(0x1539 + 1 + host_readbs(hero + 0x9b) * 10));
		} else if (hero_sleeps(hero) || hero_unc(hero)) {
			/* sleeps or is unconscious */
			ds_writeb(0xe068,
				ds_readb(0x11e4 + host_readbs(hero + 0x9b) * 2) + host_readbs(hero + 0x82));

			ds_writeb(0xe06b,
				ds_readbs(0x1210 + host_readbs(hero + 0x9b) * 8 + host_readbs(hero + 0x82) * 2));
			ds_writeb(0xe06c,
				ds_readbs(0x1210 + 1 + host_readbs(hero + 0x9b) * 8 + host_readbs(hero + 0x82) * 2));
		}


		ds_writeb(0xe06d, 40);
		ds_writeb(0xe06e, 32);
		ds_writeb(0xe06f, 0);
		ds_writeb(0xe070, 0);
		ds_writeb(0xe071, 31);
		ds_writeb(0xe072, 39);
		ds_writeb(0xe07b, 2);
		ds_writeb(0xe07c, host_readb(hero + 0x9b));
		ds_writeb(0xe073, 0xff);
		ds_writeb(0xe075, 0xff);
		ds_writeb(0xe074, 0xff);
		ds_writed(0xe07d, ds_readd(0xd86e));
		ds_writeb(0xe07a, 0);
		add_ds_ws(0xd86e, 0x508);
		sub_ds_ds(0xe370, 0x508);
		ds_writeb(0xe077, 0x63);
		ds_writeb(0xe078, 1);
		ds_writeb(0xe079, 0xff);


		host_writeb(get_hero(l_si) + 0x81, FIG_add_to_list(-1));
	}
}

#if !defined(__BORLANDC__)
}
#endif
