/*
        Rewrite of DSA1 v3.02_de functions of seg041 (fight)
        Functions rewritten: 7/9
*/
#include <stdlib.h>
#include <string.h>

#include "schick.h"

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg097.h"
#include "seg105.h"

namespace M302de {

/* DS:0x26ad */
static unsigned short msg_counter;

void FIG_output(Bit8u *str)
{

	if (*str == 0)
		return;

	GUI_output(str);

}

/**
 *	FIG_clear_msgs() - clears the fight messages buffer
*/
void FIG_clear_msgs() {
	memset(MemBase + PhysMake(datseg, 0xd333), 0 , 20);
	msg_counter = 0;
}

void FIG_add_msg(unsigned short f_action, unsigned short damage) {
	ds_writew(0xd333 + 4 * msg_counter, f_action);
	ds_writew(0xd333 + 4 * msg_counter + 2 , damage);
	if (msg_counter < 4)
		msg_counter++;
}

/**
 * FIG_damage_enemy() -	damages an enemy
 * @enemy:	pointer to the enemy
 * @damage:	the damage
 * @flag:	unknown
 *
 * This function has some tweaks, dependent on the fight number.
 */
void FIG_damage_enemy(Bit8u *enemy, Bit16s damage, bool flag)
{
	unsigned short i;

	/* subtract the damage from the enemies LE */
	host_writew(enemy + 0x13, host_readw(enemy + 0x13) - damage);

	/* are the enemies LE lower than 0 */
	if ((signed short)host_readw(enemy + 0x13) <= 0) {
		/* set a flag, maybe dead */
		host_writeb(enemy + 0x31, host_readb(enemy + 0x31) | 1);
		/* set LE to 0 */
		host_writew(enemy + 0x13, 0);

		if (ds_readw(0xe316) == 0x5e && host_readb(enemy) == 0x38) {
			/* slaying a special cultist */
			/* set a flag in the status area */
			ds_writeb(0x40f9, 0);
		} else if (ds_readw(0xe316) == 0xc0 && host_readb(enemy) == 0x48) {
			/* slaying the orc champion */
			if (ds_readb(0x5f30) == 0)
				ds_writew(0x2cd5, 0);
		} else if (ds_readw(0xe316) == 0xb4 && host_readb(enemy) == 0x46) {
			/* slaying Gorah make everything flee than Heshtot*/
			for (i = 0; i < 20; i++)
				if (ds_readb(0xd34b + 1 + i * 62) != 0x1a)
					ds_writeb(0xd34b + 0x32 + i * 62,
						ds_readb(0xd34b + 0x32 + i * 62) | 4);
		}
	}

	if (flag)
		host_writeb(enemy + 0x32, host_readb(enemy + 0x32) & 0xfd);
}

/**
 *	FIG_get_enemy_attack_damage() - calculates attack damage from an enemy
 *	@attacker:	the enemy which attacks
 *	@attacked:	the enemy/hero which gets attacked
 *	@is_enemy:	1 = if attacked is an enemy / 0 = attacked is a hero
 *
 */

signed short FIG_get_enemy_attack_damage(Bit8u *attacker, Bit8u *attacked, bool is_enemy)
{
	Bit8u *hero;
	signed short pos, damage;
	unsigned short dice;

	dice = host_readw(attacker + 0x1e);

	if (host_readw(attacker + 0x20) != 0 && random_schick(100) < 50)
		dice = host_readw(attacker + 0x20);

	damage = dice_template(dice);

	if (is_enemy == 0) {
		/* the attacked is a hero */
		hero = attacked;

		/* subtract RS */
		damage -= host_readb(hero + 0x30);

		/* armour bonus against skelettons an zombies */
		if (host_readb(hero + 0x1b2) == 0xc5 &&
			host_readb(attacker + 1) == 0x22 ||
			host_readb(attacker + 1) == 0x1c)
				damage -= 3;

		/* Totenkopfguertel makes damage 0, but can be lost */
		pos = get_item_pos(hero, 0xb6);

		if (pos != -1 && (host_readb(attacker + 1) == 0x22 ||
			host_readb(attacker + 1) == 0x1c)) {

			damage = 0;
			if (random_schick(100) < 5) {
				drop_item(hero, pos, 1);
				GUI_output(get_dtp(0x2c));
			}
		}

		/* no damage if the hero is stoned */
		if (((host_readb(hero + 0xaa) >> 2) & 1) != 0)
			damage = 0;
	} else {
		/* the attacked is an enemy */

		/* subtract RS */
		damage -= host_readb(attacked + 0x2);

		/* check unknown flag, maybe stoned */
		if (((host_readb(attacked + 0x31) >> 2) & 1) != 0)
			damage = 0;

		/* check if the attacked is immune
		 * against non-magicial weapons */
		if (host_readb(attacked + 0x24) != 0)
			damage = 0;
	}

	/* damage bonus */
	damage += (signed char)host_readb(attacker + 0x2e);

	/* half damage */
	if (host_readb(attacker + 0x30) != 0)
		damage = abs(damage) / 2;

	return damage;
}

void seg041_8c8() {
	unsigned short i;

	for (i = 0; i < 8; i++)
		memset(MemBase + PhysMake(datseg, 0xd8ce) + i * 0xf3, 0xffff, 0xf3);

//		That would be better
//		memset(MemBase + PhysMake(datseg, 0xd8ce), 0xffff, 0xf3 * 8);
}

signed short weapon_check(Bit8u *hero) {

	Bit8u *item_p;
	unsigned short item;

	/* get the number of the equipped weapon */
	item = host_readw(hero + 0x1c0);

	item_p = get_itemsdat(item);

	/* check if its a weapon */
	if (((host_readw(item_p + 2) >> 1) & 1) == 0)
		return -1;

	if (host_readb(hero + 0x1c4) & 1)
		return -1;

	/* this test is always true */
	if ((host_readw(item_p + 2) >> 1) & 1) {
		unsigned char pos = host_readb(item_p + 3);

		if (pos == 7 || pos == 8)
			return -1;

		/* At the legs, and no staffs */
		if (pos == 5 && item != 133 && item != 69)
			return -1;
	}

	if (is_in_word_array(item, MemBase + PhysMake(datseg, 0x615c)))
		return 1;

	if (is_in_word_array(item, MemBase + PhysMake(datseg, 0x616e)))
		return 0;

	return 2;
}

}
