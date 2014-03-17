/*
 *	Rewrite of DSA1 v3.02_de functions of seg032 (fight)
 *	Functions rewritten 8/12
*/
#include <stdlib.h>

#include "v302de.h"

#include "seg007.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

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
void FIG_set_cb_field(signed short y, signed short x, signed short object)
{

	/* check that the object is in the borders */
	if (y < 0 || y > 24 || x < 0 || x > 24)
		return;

	set_cb_val(x, y, (signed char)object);
}

/**
 *	FIG_choose_next_hero	-	chooses the next hero on turn
 *
 *	This is simply done randomly.
 */
unsigned short FIG_choose_next_hero()
{
	Bit8u *hero;
	unsigned short retval;
	unsigned short i;
#if !defined(__BORLANDC__)
	unsigned short loop_cnt = 0;
#endif
	long tries[7] = {0, 0, 0, 0, 0, 0, 0};

	do {
		retval = random_schick(7) - 1;
		tries[retval]++;

#if !defined(__BORLANDC__)
		if (++loop_cnt > 200) {
			D1_ERR("Possible infinite loop in %s()\n", __func__);
			D1_ERR("I'll try to get a possible hero\n");

			retval = 0xffff;

			/*
			 * print random statistic
			 */
			for (i = 0; i < 7; i++)
				D1_ERR("tries[%d] = %ld\n", i, tries[i]);

			/*
			 * search by hand for a hero and dumpi the
			 * interesting bits
			 */
			hero = get_hero(0);
			for (i = 0; i < 7; i++, hero += 0x6da) {
				D1_ERR("Hero %d typus = %x group=%x current_group=%x actions=%x\n",
					i, host_readb(hero + 0x21),
					host_readb(hero + 0x87),
					ds_readb(CURRENT_GROUP),
					host_readb(hero + 0x83));

				if (host_readb(hero + 0x21) &&
					host_readb(hero + 0x87) == ds_readb(CURRENT_GROUP) &&
					host_readb(hero + 0x83))
						retval = i;
			}

			/* exit if we didnt found a hero */
			if (retval == 0xffff) {
				D1_ERR("Sorry, this is an infinite loop.\n");
				D1_ERR("I'll exit\n");
				exit(1);
			}

			return retval;
		}
#endif

		hero = get_hero(retval);

	/* search for a hero who has a class, is in the current group and
		something still unknown */
	} while (host_readb(hero + 0x21) == 0 ||
			host_readb(hero + 0x87) != ds_readb(CURRENT_GROUP) ||
			host_readb(hero + 0x83) == 0);

	return retval;
}
/**
 *	FIG_choose_next_enemy	-	chooses the next enemy on turn
 *
 *	This is simply done randomly.
 *	Orig_BUG: I had this loop running infinitely.
 */
unsigned short FIG_choose_next_enemy(void)
{
	Bit8u *enemy;
	unsigned short retval;
	unsigned short i;
#if !defined(__BORLANDC__)
	unsigned short loop_cnt = 0;
#endif
	long tries[20] = {	0, 0, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0};
	do {

		retval = random_schick(ds_readw(NR_OF_ENEMIES)) - 1;
		tries[retval]++;

#if !defined(__BORLANDC__)
		if (++loop_cnt > 200) {
			D1_ERR("Possible infinite loop in %s()\n", __func__);
			D1_ERR("I'll try to get a possible enemy\n");

			retval = 0xffff;

			/*
			 * print random statistic
			 */
			for (i = 0; i < 20; i++)
				D1_ERR("tries[%d] = %ld\n", i, tries[i]);

			/*
			 * search by hand for an enemy and dump	the
			 * interesting bits
			 */
			enemy = p_datseg + ENEMY_SHEETS;
			for (i = 0; i < ds_readw(NR_OF_ENEMIES); i++, enemy += 62) {
				D1_ERR("Enemy %02d %x %x\n",
					i, host_readb(enemy),
					host_readb(enemy + 0x28));

				if (host_readb(enemy) &&
					host_readb(enemy + 0x28))
						retval = i;
			}

			/* exit if we didnt found an enemy */
			if (retval == 0xffff) {
				D1_ERR("Sorry, this is an infinite loop.\n");
				D1_ERR("I'll exit\n");
				exit(1);
			}

			return retval;
		}
#endif
		enemy = p_datseg + ENEMY_SHEETS + retval * 62;

	} while (host_readb(enemy) == 0 || host_readb(enemy + 0x28) == 0);

	return retval;
}

/**
 *	FIG_count_active_enemies -	return the number of active enemies
 *
 */
unsigned short FIG_count_active_enemies(void)
{
	Bit8u *enemy;
	unsigned short i, retval = 0;

	for (i = 0; i < 20; i++) {
		enemy = p_datseg + ENEMY_SHEETS + i * 62;

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
unsigned short FIG_is_enemy_active(Bit8u *enemy)
{
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
		if (host_readbs(enemy + 0x35) > 0)
			return 0;

		return 1;
}

/**
 *	FIG_get_first_active_hero -	returns the first active hero
 *
 *	Returns the index of the firsta active hero.
 */
signed short FIG_get_first_active_hero(void)
{
	Bit8u *hero_i;
	unsigned short i;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* check class */
		if (host_readb(hero_i + 0x21) == 0)
			continue;
		/* check group */
		if (host_readb(hero_i + 0x87) != ds_readb(CURRENT_GROUP))
			continue;
		/* hero is dead */
		if (hero_dead(hero_i))
			continue;
		/* hero is stoned */
		if (hero_stoned(hero_i))
			continue;
		/* hero is cursed */
		if (hero_cursed(hero_i))
			continue;
		/* yet unknown */
		if (host_readb(hero_i + 0xab) & 1)
			continue;
		/* hero is unconscious */
		if (hero_unc(hero_i))
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
unsigned short seg032_02db(void)
{
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
		if (host_readb(hero_i + 0x87) != ds_readb(CURRENT_GROUP))
			continue;
		/* hero is dead */
		if (hero_dead(hero_i))
			continue;
		/* unknown */
		if (host_readb(hero_i + 0x84) != 0x10)
			continue;

		return 1;
	}

	return 0;
}

//static
unsigned short FIG_fight_continues(void)
{

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

#if !defined(__BORLANDC__)
}
#endif
