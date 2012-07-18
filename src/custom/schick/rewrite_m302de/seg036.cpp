/**
 *	Rewrite of DSA1 v3.02_de functions of seg036 (Fight Hero KI)
 *	Functions rewritten: 3/10
 */

#include "schick.h"

#include "v302de.h"

#include "seg002.h"

namespace M302de {

/* 0x39b */

/**
 * KI_can_attack_neighbour() - check if a neighbour can be attacked
 * @start_x:	X-Coordinate of the hero
 * @start_y:	Y-Coordinate of the hero
 * @offset_x:	X-Direction -1/0/+1
 * @offset_y:	Y-Direction -1/0/+1
 * @mode:	0 = Attack only Enemies / 1 = Attack heroes and enemies / 2 = Attack only Heros
 *
 * Returns 1 if an attack is possible in that mode, else 0.
 */
signed short KI_can_attack_neighbour(signed short start_x, signed short start_y,
			signed short offset_x, signed short offset_y,
			signed short mode)
{
	signed char target = get_cb_val(start_x + offset_x, start_y + offset_y);

	if (mode == 1) {
		/* target is hero or enemy */
		if ((target > 0) && (target < 10) &&
			!hero_dead(get_hero(target - 1)) &&
			!hero_unc(get_hero(target - 1))) {

			return 1;

		} else if ((target >= 10) && (target < 30) &&
			!(ds_readb(0xd0df + target * 0x3e + 0x31) & 1) &&
			/* TODO: this is mysterious */
			!((ds_readb(0xd0df + target * 0x3e + 0x32) >> 1) & 1)) {

			return 1;
		}

		return 0;

	} else if (!mode) {
		/* target is an enemy */
		if ((target >= 10) && (target < 30) &&
			!(ds_readb(0xd0df + target * 0x3e + 0x31) & 1)) {
			return 1;
		} else {
			return 0;
		}
	} else if (mode == 2) {
		/* target is a hero */
		if ((target > 0) && (target < 10) &&
			!hero_dead(get_hero(target - 1)) &&
			!hero_unc(get_hero(target - 1))) {

			return 1;
		} else {
			return 0;
		}
	} else
		return 0;
}

/* 0x863 */

/**
 * KI_get_spell() - ???
 * @spell:	spell index
 * @mode:	mode 0/1
 */
signed short KI_get_spell(signed short spell, signed short mode)
{
	Bit8u * p;
	signed short retval = -1;

	/* make a pointer to the spell description */
	p = p_datseg + spell * 10 + 0x99d;

	if (mode == 0) {
		if (host_readb(p + 7) == 2)
			retval = 1;
		else if (host_readb(p + 7) == 1 || host_readb(p + 7) == 3)
			retval = 0;
		else
			retval = 2;
	} else {
		if (host_readb(p + 7) == 3)
			retval = 1;
		else if (host_readb(p + 7) == 0)
			retval = 2;
	}

	return retval;
}

/* 0xc39 */

/**
 * KI_count_available_heros() - count heros in group
 * @index:	index of the calling hero
 *
 * Retval: Number of heros in the group - 1
 */
signed short KI_count_heros(signed short index)
{
	signed short cnt = 0;
	signed short i;

	/* for each hero in this group */
	for (i = 0; ds_readb(0x2d36 + ds_readb(CURRENT_GROUP)) > i; i++) {

		if (index == i)
			continue;

		if (!check_hero(get_hero(i)))
			continue;

		cnt++;
	}

	return cnt;
}

}
