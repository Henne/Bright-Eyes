/*
 *	Rewrite of DSA1 v3.02_de functions of seg032 (fight)
 *	Functions rewritten 8/12
*/

#include "schick.h"

#include "v302de.h"

#include "seg007.h"

/**
 *	FIG_set_cb_field - sets an object to a chessboard position
 *	@y:		the Y-Coordinate on the chessboard
 *	@x:		the X-Coordinate on the chessboard
 *	@object:	number of the object
 *			0=free, 1-6=heroes, >10=enemies,
 *			>50walls, trees, etc
 *			-3 = flee
 *
 *	This function is called multiple times when an entrant in a fight
 *	is moving. The initial placements of static objects are also done
 *	with this function.
 */
void FIG_set_cb_field(signed short y, signed short x, signed char object) {

	/* check that the object is in the borders */
	if (y < 0 || y > 24 || x < 0 || x > 24)
		return;

	set_cb_val(x, y, object);
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
		hero = get_hero(retval);

	/* search for a hero who has a class, is in the current group and
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
 *	Orig_BUG: I had this loop running infinitely.
 */
unsigned short FIG_choose_next_enemy() {

	Bit8u *enemy;
	unsigned short retval;

	do {
		retval = random_schick(ds_readw(0xd872)) - 1;
		enemy = MemBase + PhysMake(datseg, 0xd34b) + retval * 62;

	} while (host_readb(enemy) == 0 || host_readb(enemy + 0x28) == 0);

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

	hero_i = get_hero(0);

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

	hero_i = get_hero(0);
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

//static
unsigned short FIG_fight_continues() {

	if (seg032_02db()) {
		ds_writew(0x5f14, 1);
		return 0;
	}

	if (FIG_get_first_active_hero() == -1) {
		ds_writew(0xc3c1, 1);
		return 0;
	}

	if (FIG_count_active_enemies() == 0)
		return 0;

	return 1;
}
