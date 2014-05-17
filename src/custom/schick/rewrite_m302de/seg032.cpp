/*
 *	Rewrite of DSA1 v3.02_de functions of seg032 (fight)
 *	Functions rewritten 9/12
*/
#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg005.h"
#include "seg006.h"
#include "seg007.h"
#include "seg008.h"

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
/* Borlandified and identical */
void FIG_set_cb_field(signed short y, signed short x, signed short object)
{

	/* check that the object is in the borders */
	if (y < 0 || y > 24 || x < 0 || x > 24)
		return;

	set_cb_val(x, y, (signed char)object);
}

/* Borlandified and identical */
void draw_fight_screen_pal(signed short mode)
{
	FIG_draw_pic();

	/* check for palette update */
	if (ds_readbs(0x2845) != -1) {

		update_mouse_cursor();

		/* clear framebuffer */
#if !defined(__BORLANDC__)
		PhysPt p = Real2Phys((RealPt)ds_readd(0xd2ff));

		for (int i = 0; i < 64000; i+=4) {
			mem_writed(p + i, 0);
		}
#else
		memset((RealPt)ds_readd(0xd2ff), 0, 64000);
#endif

		/* set palettes */
		set_palette(p_datseg + 0x2783, 0x00, 0x20);
		set_palette(p_datseg + 0x7d0e, 0x80, 0x14);
		set_palette(Real2Host((RealPt)ds_readd(0xc3a9) + 0xfa02), 0x60, 0x20);

		ds_writeb(0x2845, -1);

		refresh_screen_size();
	}

	draw_fight_screen(mode);
}

/**
 *	FIG_choose_next_hero	-	chooses the next hero on turn
 *
 *	This is simply done randomly.
 */
/* Borlandified and identical */
signed short FIG_choose_next_hero(void)
{
#if !defined(__BORLANDC__)
	signed short loop_cnt = 0;
	long tries[7] = {0, 0, 0, 0, 0, 0, 0};
#endif

	signed short retval;

	do {
		retval = random_schick(7) - 1;

#if !defined(__BORLANDC__)
		tries[retval]++;

		if (++loop_cnt > 200) {
			D1_ERR("Possible infinite loop in %s()\n", __func__);
			D1_ERR("I'll try to get a possible hero\n");

			retval = 0xffff;

			/*
			 * print random statistic
			 */
			for (int i = 0; i < 7; i++)
				D1_ERR("tries[%d] = %ld\n", i, tries[i]);

			/*
			 * search by hand for a hero and dump the
			 * interesting bits
			 */
			Bit8u *hero = get_hero(0);
			for (int i = 0; i < 7; i++, hero += 0x6da) {
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


	/* search for a hero who has a class, is in the current group and
		something still unknown */
	} while (host_readb(get_hero(retval) + 0x21) == 0 ||
			host_readb(get_hero(retval) + 0x87) != ds_readb(CURRENT_GROUP) ||
			host_readb(get_hero(retval) + 0x83) == 0);

	return retval;
}
/**
 *	FIG_choose_next_enemy	-	chooses the next enemy on turn
 *
 *	This is simply done randomly.
 *	Orig_BUG: I had this loop running infinitely.
 */
/* Borlandified and identical */
signed short FIG_choose_next_enemy(void)
{
	signed short retval;

#if !defined(__BORLANDC__)
	Bit8u *enemy;
	unsigned short i;
	unsigned short loop_cnt = 0;
	long tries[20] = {	0, 0, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0};
#endif

	do {

		retval = random_schick(ds_readw(NR_OF_ENEMIES)) - 1;

#if !defined(__BORLANDC__)
		tries[retval]++;

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
		enemy = p_datseg + ENEMY_SHEETS + retval * 62;
#endif

	} while (ds_readbs(ENEMY_SHEETS + retval * 62) == 0 || ds_readbs(ENEMY_SHEETS + 0x28 + retval * 62) == 0);

	return retval;
}

/**
 *	FIG_count_active_enemies -	return the number of active enemies
 *
 */
/* Borlandified and identical */
signed short FIG_count_active_enemies(void)
{
	Bit8u *enemy;
	signed short i, retval = 0;

	for (i = 0; i < 20; i++) {

		enemy = p_datseg + ENEMY_SHEETS + i * 62;

		/* if enemy has no monster class */
		if ((host_readb(enemy) != 0) &&
			!enemy_dead(enemy) &&
			!enemy_stoned(enemy) &&
			!enemy_cursed(enemy) &&
			!enemy_uncon(enemy) &&
			!enemy_busy(enemy) &&
			!host_readbs(enemy + 0x35))
		{
			retval++;
		}
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
/* Borlandified and identical */
signed short FIG_is_enemy_active(Bit8u *enemy)
{
	if (enemy_sleeps(enemy) ||
		enemy_dead(enemy) ||
		enemy_stoned(enemy) ||
		enemy_bit11(enemy) ||
		enemy_uncon(enemy) ||
		enemy_busy(enemy) ||
		enemy_bit8(enemy) ||
		(host_readbs(enemy + 0x35) > 0))
	{
		return 0;
	}

	return 1;
}

/**
 *	FIG_get_first_active_hero -	returns the first active hero
 *
 *	Returns the index of the firsta active hero.
 */
/* Borlandified and identical */
signed short FIG_get_first_active_hero(void)
{
	Bit8u *hero_i;
	signed short i;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* check class */
		if ((host_readb(hero_i + 0x21) != 0) &&
			(host_readb(hero_i + 0x87) == ds_readb(CURRENT_GROUP)) &&
			!hero_dead(hero_i) &&
			!hero_stoned(hero_i) &&
			!hero_cursed(hero_i) &&
			!hero_unkn3(hero_i) &&
			!hero_unc(hero_i))
		{
			return i;
		}
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
/* Borlandified and identical */
unsigned short seg032_02db(void)
{
	Bit8u *hero_i;
	signed short i;

	if (FIG_get_first_active_hero() == -1) {
		hero_i = get_hero(0);
		for (i = 0; i <= 6; i++, hero_i += 0x6da) {
			if ((host_readb(hero_i + 0x21) != 0) &&
				(host_readb(hero_i + 0x87) == ds_readb(CURRENT_GROUP)) &&
				!hero_dead(hero_i) &&
				(host_readb(hero_i + 0x84) == 0x10))
			{
				return 1;
			}
		}

		return 0;
	} else {
		return 0;
	}
}

//static
/* Borlandified and identical */
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
