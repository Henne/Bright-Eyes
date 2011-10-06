/*
 *	Rewrite of DSA1 v3.02_de functions of seg039 (fight)
 *	Functions rewritten 5/7
*/

#include <stdlib.h>
#include <string.h>

#include "schick.h"

#include "v302de.h"

#include "seg006.h"
#include "seg007.h"
#include "seg032.h"

namespace M302de {

/* is used at selecting a target */
signed short seg039_0000(unsigned short v1, unsigned short v2, unsigned short v3, unsigned short v4) {
	return abs(v1 - v3) + abs(v2 - v4);
}

signed short seg039_0023(Bit8u *hero) {
	Bit8u *ptr;
	unsigned short weapon;

	/* get equipped weapon of the hero */
	weapon = host_readw(hero + 0x1c0);
	/* make a pointer to the entry of ITEMS.DAT */
	ptr = get_itemsdat(weapon);

	D1_LOG("weapon 0x%x +2 0x%x +3 0x%x\n", weapon,
		host_readb(ptr + 2), host_readb(ptr + 3));

	/* not a weapon */
	if (((host_readb(ptr + 2) >> 1) & 1) == 0)
		return -1;

	/* weapons are not MagicStaffs or Fightstaffs */
	if (host_readb(ptr + 3) == 5 && weapon != 0x85 && weapon != 0x45)
		return 5;

	if (host_readb(ptr + 3) == 7)
		return 3;

	if (host_readb(ptr + 3) == 8)
		return 4;

	return -1;
}

/**
 * fill_enemy_sheet - fills an enemies sheet from a template
 * @sheet_nr:	the number of the sheet
 * @enemy_id:	the ID of the enemy (MONSTER.DAT)
 * @round:	the fight round the enemy appears
 */
void fill_enemy_sheet(unsigned short sheet_nr, signed char enemy_id, unsigned char round) {

	Bit8u *sheet;
	Bit8u *temp;
	unsigned short i;

	/* calculate the pointers */
	temp = Real2Host(ds_readd(0xe125)) + enemy_id * 44;
	sheet = MemBase + PhysMake(datseg, 0xd34b) + sheet_nr * 62;

	/* erease the sheet */
	memset(sheet, 0, 62);

	/* copy enemy id, gfx_id and RS to the sheet */
	host_writeb(sheet + 0, host_readb(temp + 0));
	host_writeb(sheet + 1, host_readb(temp + 1));
	host_writeb(sheet + 2, host_readb(temp + 2));

	/* roll attributes  and save them to the sheet */
	for (i = 0; i < 7; i++) {
		signed char tmp;

		tmp = dice_template(host_readw(temp + i * 2 + 3));
		host_writeb(sheet + i * 2 + 4, tmp);
		host_writeb(sheet + i * 2 + 3, tmp);
	}

	/* roll out LE and save it to the sheet */
	host_writew(sheet + 0x11, dice_template(host_readw(temp + 0x11)));
	/* recalculate LE = LE / 6 * 5; */
	host_writew(sheet + 0x11, host_readw(sheet + 0x11) / 6 * 5);
	/* copy LE*/
	host_writew(sheet + 0x13, host_readw(sheet + 0x11));

	/* roll out AE and save it to the sheet */
	host_writew(sheet + 0x17, dice_template(host_readw(temp + 0x13)));
	host_writew(sheet + 0x15, host_readw(sheet + 0x17));

	/* roll out MR  and save it */
	host_writeb(sheet + 0x19, dice_template(host_readw(temp + 0x15)));

	/* Terrible hack:
		if the current fight is 0xbc, set MR to 5 (Travel-Event 84),
		if the current fight is 0xc0, and the enemy
		is no "Orkcachampion" then set a flag */
	if (ds_readw(0xe316) == 0xbc)
		host_writeb(sheet + 0x19, 5);
	else if (ds_readw(0xe316) == 0xc0 && host_readb(sheet) == 0x48)
			host_writeb(sheet + 0x31,
				host_readb(sheet + 0x31) | 0x20);

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
	if (host_readb(sheet + 0x22) > 10)
		host_writeb(sheet + 0x22, 10);

	/* copy imunnity against non-magicial weapons ? */
	host_writeb(sheet + 0x24, host_readb(temp + 0x20));
	/* copy ID Magicuser class ? */
	host_writeb(sheet + 0x25, host_readb(temp + 0x21));

	/* bogus this value is 0x00 or 0x20 */
	host_writeb(sheet + 0x31, host_readb(sheet + 0x31) & 0xfe);

	host_writeb(sheet + 0x26, 0xff);

	/* copy Level */
	host_writeb(sheet + 0x29, host_readb(temp + 0x22));
	/* copy size class */
	host_writeb(sheet + 0x34, host_readb(temp + 0x23));
	/* copy flags */
	host_writeb(sheet + 0x36, host_readb(temp + 0x24));

	/* set the fight round the enemy appears */
	host_writeb(sheet + 0x35, round);

	/* set to ? */
	host_writeb(sheet + 0x27,
		host_readb(Real2Host(ds_readd(0xbd28)) + sheet_nr * 5 + 0x19));

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
		If the current fight == 0x5e and the enemy is "Kultist",
		set a flag */
	if (ds_readw(0xe316) == 0x5e && host_readb(sheet) == 0x38)
		host_writeb(sheet + 0x32, host_readb(sheet + 0x32) | 4);
}

/**
 *	place_obj_on_cb -	places an object on the chessboard
 *	@x:		X-Coordinate
 *	@y:		Y-Coordinate
 *	@object:	object number
 *	@type:		typus for heros, monster_id for enemies
 *	@dir:		looking direction
 *
 *	Returns 1 if the placement was successful or 0 if not.
 */
unsigned short place_obj_on_cb(unsigned short x, unsigned short y, signed short object, signed char type, signed char dir) {

	unsigned short i;

	/* check if an object is already on that field */
	if (get_cb_val(x, y) > 0)
		return 0;

	/* check if the object nr is valid */
	if (object < 0)
		return 0;

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
		/* if object is an enemy an needs 2 fields */
		if (object >= 10 &&
			is_in_byte_array(type, MemBase + PhysMake(datseg, 0x25f9)))
		{
			signed short x_diff, y_diff;

			x_diff = ds_readw(0x6018 + dir * 4);
			y_diff = ds_readw(0x601a + dir * 4);

			/* check if field is empty */
			if (get_cb_val(x + x_diff, y + y_diff) != 0)
				return 0;

			x_diff += x;
			y_diff += y;

			/* check if y is in the borders */
			if (y_diff < 0 || y_diff > 23)
				return 0;
			/* check if x is in the borders */
			if (x_diff < 0 || x_diff > 23)
				return 0;

			FIG_set_cb_field(y_diff, x_diff, object + 20);
		}
	}

	/* set the object to the chessboard */
	FIG_set_cb_field(y, x, object);

	return 1;
}

void FIG_init_heroes()
{
	Bit8u *hero;
	Bit16s cb_x, cb_y;
	Bit16s l_si, l_di;

	for (l_si = 0; l_si <= 6; l_si++) {

		if (host_readb(get_hero(l_si) + 0x81) == 0xff)
			continue;

		FIG_remove_from_list(host_readb(get_hero(l_si) + 0x81), 0);
		host_writeb(get_hero(l_si) + 0x81, 0xff);
	}

	for (l_si = 0; l_si <= 6; l_si++) {
		hero = get_hero(l_si);

		/* check typus */
		if (host_readb(hero + 0x21) == 0)
			continue;
		/* check group */
		if (host_readb(hero + 0x87) != ds_readb(0x2d35))
			continue;

		/* these two are unknown */
		host_writeb(hero + 0x84, 10);
		host_writeb(hero + 0x86, 0);

		/* check special fight */
		if (ds_readw(0xe316) == 0xc0) {
			if (hero == Real2Host(ds_readd(0x3e20))) {
				cb_x = host_readb(Real2Host(ds_readd(0xbd28)) + 0x7a);
				cb_y = host_readb(Real2Host(ds_readd(0xbd28)) + 0x7b);
				host_writeb(hero + 0x82,
					host_readb(Real2Host(ds_readd(0xbd28)) + 0x7c));
			} else {
				do {
					l_di = random_schick(6);

					cb_x = host_readb(Real2Host(ds_readd(0xbd28)) + l_di * 4 + 0x7a);
					cb_y = host_readb(Real2Host(ds_readd(0xbd28)) + l_di * 4 + 0x7b);
					host_writeb(hero + 0x82,
						host_readb(Real2Host(ds_readd(0xbd28)) + l_di * 4 + 0x7c));
				} while (get_cb_val(cb_x, cb_y) != 0);
			}
		} else {
			cb_x = host_readb(Real2Host(ds_readd(0xbd28)) + 0x7a + 4 * l_si);
			cb_y = host_readb(Real2Host(ds_readd(0xbd28)) + 0x7b + 4 * l_si);
			/* Direction */
			host_writeb(hero + 0x82, host_readb(Real2Host(ds_readd(0xbd28)) + 0x7c + 4 * l_si));
		}

		/* heros sleep until they appear */
		if (host_readb(Real2Host(ds_readd(0xbd28)) + l_si * 4 + 0x7d) != 0) {
			if ((host_readb(hero + 0xaa) & 1) == 0)
				host_writeb(hero + 0xaa, host_readb(hero + 0xaa) | 2);
		}

		place_obj_on_cb(cb_x, cb_y, l_si + 1,
			host_readb(hero + 0x21), host_readb(hero + 0x82));

		l_di = seg039_0023(hero);

		if (l_di != -1) {
			ds_writeb(0xe068,
				ds_readb(0x10d0 + host_readb(hero + 0x82) + host_readb(hero + 0x9b) * 12 + l_di * 4));
		} else {
			ds_writeb(0xe068, host_readb(hero + 0x82));
		}

		ds_writew(0xe066, ds_readb(0x12c0 + host_readb(hero + 0x9b) * 5));
		ds_writeb(0xe069, cb_x);
		ds_writeb(0xe06a, cb_y);
		ds_writeb(0xe06b, 0);
		ds_writeb(0xe06c, 0);

		if (host_readb(hero + 0xaa) & 1) {
			/* if hero is dead */
			ds_writeb(0xe068,
				ds_readb(0x1a13 + host_readb(hero + 0x9b) * 2));
			ds_writeb(0xe06b,
				ds_readb(0x1539 + host_readb(hero + 0x9b) * 10));
			ds_writeb(0xe06c,
				ds_readb(0x1539 + 1 + host_readb(hero + 0x9b) * 10));
		} else if ((host_readb(hero + 0xaa) >> 1) || host_readb(hero + 0xaa) >> 6) {
			/* sleeps or is unconscious */
			ds_writeb(0xe068,
				host_readb(hero + 0x82) + ds_readb(0x11e4 + host_readb(hero + 0x9b) * 2));

			ds_writew(0xe06b,
				ds_readb(0x1210 + host_readb(hero + 0x9b) * 8 + host_readb(hero + 0x82) * 2));
			ds_writew(0xe06c,
				ds_readb(0x1210 + 1 + host_readb(hero + 0x9b) * 8 + host_readb(hero + 0x82) * 2));
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
		ds_writew(0xd86e, ds_readw(0xd86e) + 0x508);
		ds_writed(0xe370, ds_readd(0xe370) - 0x508);
		ds_writeb(0xe077, 0x63);
		ds_writeb(0xe078, 1);
		ds_writeb(0xe079, 0xff);


		host_writeb(get_hero(l_si) + 0x81, (signed char)FIG_add_to_list(0xffff));
	}

}

}
