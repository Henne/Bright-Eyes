/*
 *	Rewrite of DSA1 v3.02_de functions of seg032 (fight)
 *	Functions rewritten 7/12
*/

#include "schick.h"

#include "seg007.h"

/**
 *	FIG_set_cb_field - sets an object to a chessboard position
 *	@row:		the row on the chessboard (iso_y)
 *	@col:		the column on the chessboard (iso_y)
 *	@object:	number of the object
 *			0=free, 1-6=heroes, >10=enemys,
 *			>50walls, trees, etc
 *			-3 = flee
 *
 *	This function is called multiple timeswhen an entrant in a fight
 *	is moving. The initial placements of static objects are also done
 *	with this function.
 */
void FIG_set_cb_field(signed short row, signed short col, signed char object) {
	if (row < 0 || row > 24 || col < 0 || col > 24)
		return;

	host_writeb(MemBase + Real2Phys(ds_readd(0xd852)) + row * 25 + col,
		object);
}

/**
 *	FIG_choose_next_hero	-	chooses the next hero on turn
 *
 *	This is simply done randomly.
 */
unsigned short FIG_choose_next_hero() {
	Bit8u *hero;
	unsigned short retval;

	do {
		retval = random_schick(7) - 1;
		hero = MemBase + Real2Phys(ds_readd(0xbd34)) + retval * 0x6da;

	/* search fo a hero who is not dead, in the current group and
		something still unknown */
	} while (host_readb(hero + 0x21) == 0 ||
			host_readb(hero + 0x87) != ds_readb(0x2d35) ||
			host_readb(hero + 0x83) == 0);

	return retval;
}
/**
 *	FIG_choose_next_enemy	-	chooses the next enemy on turn
 *
 *	This is simply done randomly.
 */
unsigned short FIG_choose_next_enemy() {

	Bit8u *enemy;
	unsigned short retval;

	do {
		retval = random_schick(ds_readw(0xd872)) - 1;
		enemy = MemBase + retval * 62;

	} while (host_readb(enemy + PhysMake(datseg, 0xd34b)) == 0 ||
			host_readb(enemy + PhysMake(datseg, 0xd373)) == 0);

	return retval;
}

/**
 *	FIG_count_active_enemies -	return the number of active enemies
 *
 */
unsigned short FIG_count_active_enemies() {

	Bit8u *enemy;
	unsigned short i, retval = 0;

	for (i = 0; i < 20; i++) {
		enemy = MemBase + PhysMake(datseg, 0xd34b) + i * 62;

		/* if enemy has no monster class */
		if (host_readb(enemy) == 0)
			continue;

		/* check flags */
		/* enemy is dead */
		if ((host_readb(enemy + 0x31)) & 1)
			continue;
		if ((host_readb(enemy + 0x31) >> 2) & 1)
			continue;
		if ((host_readb(enemy + 0x31) >> 5) & 1)
			continue;
		if ((host_readb(enemy + 0x31) >> 6) & 1)
			continue;
		if ((host_readb(enemy + 0x31) >> 3) & 1)
			continue;

		/* unknown */
		if (host_readb(enemy + 0x35))
			continue;

		retval++;
	}

	return retval;
}

/**
 *	FIG_is_enemy_active -	checks if an enemy is active
 *	@enemy:	pointer to the enemy sheet
 *
 *	Returns 1 if enemy can act or 0 if not.
 */
//static
unsigned short FIG_is_enemy_active(Bit8u *enemy) {

		if ((host_readb(enemy + 0x31) >> 1) & 1)
			return 0;
		/* check if enemy is dead */
		if ((host_readb(enemy + 0x31)) & 1)
			return 0;
		if ((host_readb(enemy + 0x31) >> 2) & 1)
			return 0;
		if ((host_readb(enemy + 0x32) >> 3) & 1)
			return 0;
		if ((host_readb(enemy + 0x31) >> 6) & 1)
			return 0;
		if ((host_readb(enemy + 0x31) >> 3) & 1)
			return 0;
		if ((host_readb(enemy + 0x32)) & 1)
			return 0;
		if (((signed char)host_readb(enemy + 0x35)) > 0)
			return 0;

		return 1;
}

/**
 *	FIG_get_first_active_hero -	returns the first active hero
 *
 *	Returns the index of the firsta active hero.
 */
signed short FIG_get_first_active_hero() {

	Bit8u *hero_i;
	unsigned short i;

	hero_i = MemBase + Real2Phys(ds_readd(0xbd34));

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* check class */
		if (host_readb(hero_i + 0x21) == 0)
			continue;
		/* check group */
		if (host_readb(hero_i + 0x87) != ds_readb(0x2d35))
			continue;
		/* hero is dead */
		if (host_readb(hero_i + 0xaa) & 1)
			continue;
		/* hero is stoned */
		if ((host_readb(hero_i + 0xaa) >> 2) & 1)
			continue;
		/* unknown */
		if ((host_readb(hero_i + 0xaa) >> 5) & 1)
			continue;
		/* unknown */
		if (host_readb(hero_i + 0xab) & 1)
			continue;
		/* hero is unconscious */
		if ((host_readb(hero_i + 0xaa) >> 6) & 1)
			continue;

		return i;
	}

	return -1;
}

/**
 *	seg032_02db -	currently unknown
 *
 *	Returns 1 if FIG_get_first_active_hero() returns -1
 *	and at least one hero in the group is not dead and has
 *	something at offset 0x84 set (maybe sleeping).
 *
 */
//static
unsigned short seg032_02db() {

	Bit8u *hero_i;
	unsigned short i;

	if (FIG_get_first_active_hero() != -1)
		return 0;

	hero_i = MemBase + Real2Phys(ds_readd(0xbd34));
	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* check class */
		if (host_readb(hero_i + 0x21) == 0)
			continue;
		/* check group */
		if (host_readb(hero_i + 0x87) != ds_readb(0x2d35))
			continue;
		/* hero is dead */
		if (host_readb(hero_i + 0xaa) & 1)
			continue;
		/* unknown */
		if (host_readb(hero_i + 0x84) != 0x10)
			continue;

		return 1;
	}

	return 0;
}
