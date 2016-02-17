/*
 *	Rewrite of DSA1 v3.02_de functions of seg041 (fight)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg041.cpp
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg038.h"
#include "seg039.h"
#include "seg041.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if 0
/* DS:0x26ad */
static unsigned short msg_counter;
#endif

/**
 * range_attack_ammo()	- check if a range attack can be done
 * @hero:	the hero who attacks
 * @arg:	0 = drop one unit, 1 = just check, 2 = check with output
 *
 * Returns: 0 = no ammo / 1 = have ammo
*/
signed short seg041_0020(Bit8u *hero, signed short arg)
{
	signed short right_hand;
	signed short left_hand;
	signed short retval;

	retval = 0;

	/* read the item ids from the hands */
	right_hand = host_readws(hero + 0x1c0);
	left_hand = host_readws(hero + 0x1ce);

	switch (right_hand) {
	case 0x5:	/* SPEER	/ spear */
	case 0x10:	/* WURFBEIL	/ francesca */
	case 0x11:	/* WURFSTERN	/ throwing star */
	case 0x21:	/* WURFAXT	/ throwing axe */
	case 0x62:	/* WURFMESSER	/ throwing dagger */
	case 0x89:	/* SCHNEIDEZAHN	/ cutting tooth */
	case 0xda:
	{
		if (!arg) {

			if (ds_readws(0x5f12) < 30) {
				ds_writew(0xe31a + ds_readw(0x5f12) * 2, right_hand);
				inc_ds_ws(0x5f12);
			}

			drop_item(hero, 3, 1);

			if (left_hand == right_hand) {
				move_item(3, 4, hero);
			}

		}
		retval = 1;
		break;
	}
	case 0x9:	/* KURZBOGEN	/ SHORT BOW */
	case 0x13:	/* LANGBOGEN	/ LONG BOW */
	{
		/* PFEIL / ARROWS */
		if (left_hand != 10) {
			if (arg != 2) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x20),
					(char*)hero + 0x10);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}

		} else {
			if (!arg) {
				drop_item(hero, 4, 1);
			}
			retval = 1;
		}
		break;
	}
	case 0xc:	/* ARMBRUST	/ CROSSBOW */
	{
		/* BOLZEN / BOLT */
		if (left_hand != 13) {
			if (arg != 2) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x24),
					(char*)hero + 0x10);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		} else {
			if (!arg) {
				drop_item(hero, 4, 1);
			}
			retval = 1;
		}
		break;
	}
	case 0x78:	/* SCHLEUDER	/ SLING */
	{
		if (left_hand != 999) {
			if (arg != 2) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x28),
					(char*)hero + 0x10);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		} else {
			if (!arg) {
				drop_item(hero, 4, 1);
			}
			retval = 1;
		}
		break;
	}
	}
	return retval;
}

void FIG_output(Bit8u *str)
{
	if (*str != 0) {
		GUI_output(str);
	}
}

/**
 *	FIG_clear_msgs() - clears the fight messages buffer
*/
void FIG_clear_msgs(void)
{
	memset(p_datseg + 0xd333, 0 , 20);
	ds_writew(0x26ad, 0);
}

void FIG_add_msg(unsigned short f_action, unsigned short damage)
{
	ds_writew(0xd333 + 4 * ds_readws(0x26ad), f_action);
	ds_writew(0xd333 + 2 + 4 * ds_readws(0x26ad) , damage);
	if (ds_readws(0x26ad) < 4)
		inc_ds_ws(0x26ad);
}

/**
 * FIG_damage_enemy() -	damages an enemy
 * @enemy:	pointer to the enemy
 * @damage:	the damage
 * @flag:	unknown
 *
 * This function has some tweaks, dependent on the fight number.
 */
void FIG_damage_enemy(Bit8u *enemy, Bit16s damage, signed short flag)
{
	signed short i;

	/* subtract the damage from the enemies LE */
	sub_ptr_ws(enemy + 0x13, damage);

	/* are the enemies LE lower than 0 */
	if (host_readws(enemy + 0x13) <= 0) {
		/* set a flag, maybe dead */
		or_ptr_bs(enemy + 0x31, 1);
		/* set LE to 0 */
		host_writew(enemy + 0x13, 0);

		if ((ds_readw(CURRENT_FIG_NR) == 94) && (host_readb(enemy) == 0x38)) {
			/* slaying a special cultist */
			/* set a flag in the status area */
			ds_writeb(0x40f9, 0);

		} else if ((ds_readw(CURRENT_FIG_NR) == 192) &&
				(host_readb(enemy) == 0x48) &&
				!ds_readbs(0x5f30))
		{
			/* slaying the orc champion, ends the fight */
				ds_writew(IN_FIGHT, 0);

		} else if ((ds_readw(CURRENT_FIG_NR) == 180) && (host_readb(enemy) == 0x46)) {

			/* slaying Gorah make everything flee than Heshtot*/
			for (i = 0; i < 20; i++) {
#if !defined(__BORLANDC__)
				if (ds_readb(ENEMY_SHEETS + ENEMY_SHEET_GFX_ID + i * SIZEOF_ENEMY_SHEET) != 26)
					or_ds_bs((ENEMY_SHEETS + ENEMY_SHEET_STATUS2) + i * SIZEOF_ENEMY_SHEET, 4);
#else
				if ( ((struct enemy_sheets*)(Real2Host(RealMake(datseg, ENEMY_SHEETS))))[i].gfx_id != 0x1a)
					((struct enemy_sheets*)(Real2Host(RealMake(datseg, ENEMY_SHEETS))))[i].status2.bit10 = 1;
#endif
			}
		}
	}

	if (!flag)
		and_ptr_bs(enemy + 0x32, 0xfd);
}

signed short FIG_get_hero_melee_attack_damage(Bit8u* hero, Bit8u* target, signed short attack_hero)
{
	signed short damage;
	signed short l_di;
	Bit8u* item_p_rh;
	Bit8u* p2;
	Bit8u* p3;
	signed short target_size;
	signed short right_hand;
	signed short beeline;
	signed short v4;
	signed short x_hero;
	signed short y_hero;
	signed short x_target;
	signed short y_target;
	signed short v9;
	signed char enemy_gfx_id;
	Bit8u* enemy_p;
	signed short v11;

	l_di = 0;

	if (attack_hero == 0) {
		enemy_p = target;
	}

	right_hand = host_readw(hero + 0x1c0);

	item_p_rh = get_itemsdat(right_hand);

	v11 = weapon_check(hero);

	if (v11 == -1) {
		v11 = FIG_get_range_weapon_type(hero);
	}

	if (v11 != -1) {

		p2 = p_datseg + 0x6b0 + host_readbs(item_p_rh + 4) * 7;

		damage = dice_roll(host_readbs(p2), 6, host_readbs(p2 + 1));

		if (host_readbs(p2 + 4) != -1) {

			v9 = get_hero_index(hero);

			FIG_search_obj_on_cb(v9 + 1, &x_hero, &y_hero);
			FIG_search_obj_on_cb(host_readbs(hero + 0x86), &x_target, &y_target);

#if !defined(__BORLANDC__)
			/* BE-fix */
			x_hero = host_readws((Bit8u*)&x_hero);
			y_hero = host_readws((Bit8u*)&y_hero);
			x_target = host_readws((Bit8u*)&x_target);
			y_target = host_readws((Bit8u*)&y_target);
#endif
			beeline = calc_beeline(x_hero, y_hero, x_target, y_target);

			if (beeline <= 2) {
				v4 = 0;
			} else if (beeline <= 4) {
				v4 = 1;
			} else if (beeline <= 6) {
				v4 = 2;
			} else if (beeline <= 9) {
				v4 = 3;
			} else if (beeline <= 15) {
				v4 = 4;
			} else if (beeline <= 20) {
				v4 = 5;
			} else {
				v4 = 6;
			}

			p3 = p_datseg + 0x668 + host_readbs(p2 + 4) * 8;

			if (attack_hero != 0) {
				if (host_readbs(target + 0x21) == 6) {
					/* ZWERG / DWARF */
					target_size = 2;
				} else {
					target_size = 3;
				}
			} else {
					/* size of the enemy */
				target_size = host_readbs(target + 0x34);
			}

			l_di = (test_skill(hero,
					(host_readbs(item_p_rh + 3) == 8 ? 8 : 7),
					host_readbs(p3 + 7) + 2 * v4 - 2 * target_size) > 0)
				? ds_readbs(0x668 + 8 * host_readbs(p2 + 4) + v4)
				: -damage;

			if (l_di != 0) {
				damage += l_di;
			}


		} else {

			l_di = host_readbs(hero + 0x47) + host_readbs(hero + 0x48) - host_readbs(p2 + 2);
			if (l_di > 0) {
				damage += l_di;
			}
		}
	} else {
		damage = random_schick(6);
	}

	if (attack_hero == 0) {

		enemy_gfx_id = host_readbs(enemy_p + 1);

		if ((right_hand == 0xac) && (enemy_gfx_id == 0x1c || enemy_gfx_id == 0x22)) {
			damage++;
		} else {
			if (right_hand == 0xc1) {
				/* KUKRISDOLCH */

				/* Interesting */
				damage = 1000;

				/* drop the KUKRISDOLCH and equip a normal DOLCH / DAGGER */
				drop_item(hero, 3, 1);
				give_hero_new_item(hero, 14, 1 ,1);
				move_item(3, get_item_pos(hero, 14), hero);
			} else if (right_hand == 0xc8) {
				/* KUKRISMENGBILLAR */

				/* Interesting */
				damage = 1000;

				/* drop the KUKRISDOLCH and equip a normal DOLCH / DAGGER */
				drop_item(hero, 3, 1);
				give_hero_new_item(hero, 109, 1 ,1);
				move_item(3, get_item_pos(hero, 109), hero);
			} else if ((right_hand == 0xd6) && (enemy_gfx_id == 0x1c)) {
				/* SILBERSTREITKOLBEN */
				damage += 4;
			} else if ((right_hand == 0xb5) && (enemy_gfx_id == 0x18)) {
				/* DAS SCHWERT GRIMRING */
				damage += 5;
			}
		}
	}

	damage += host_readbs(hero + 0x98);

	if (damage > 0) {

		if (ks_poison1(hero + 0x1c0)) {
			damage += dice_roll(1, 6, 2);
			and_ptr_bs(hero + 0x1c4, 0xdf);
		}

		if (ks_poison2(hero + 0x1c0)) {
			damage += dice_roll(1, 20, 5);
			and_ptr_bs(hero + 0x1c4, 0xbf);
		}

		if (host_readbs(hero + 0x1c0 + 9) != 0) {

			if (host_readbs(hero + 0x1c0 + 9) == 3) {
				or_ptr_bs(enemy_p + 0x32, 0x04);
				and_ptr_bs(enemy_p + 0x32, 0xfd);
			} else {

				damage += 10 * ds_readws(0x2c70 + 2 * host_readbs(hero + 0x1c0 + 9));
			}

			dec_ptr_bs(hero + 0x1c0 + 10);

			if (!host_readbs(hero + 0x1c0 + 10)) {
				host_writebs(hero + 0x1c0 + 9, 0);
			}
		}
	}

	if (host_readbs(hero + 0x97) != 0) {
		damage *= 2;
	}

	if ( (ds_readbs(0x3dda) != 0) &&
		(host_readbs(hero + 0x21) == 6) &&
		(attack_hero == 0) &&
		(host_readbs(enemy_p + 1) == 0x18))
	{
		damage++;
	}

	if (attack_hero == 0) {
		damage -= host_readbs(enemy_p + 2);

		if (enemy_stoned(enemy_p)) {
			damage = 0;
		}

		if ((host_readbs(enemy_p + 0x24) != 0) && !ks_magic_hidden(hero + 0x1c0)) {
			damage = 0;
		}

		if (host_readws(enemy_p + 0x13) < damage) {
			damage = host_readws(enemy_p + 0x13) + 1;
		}
	} else {
		damage -= host_readbs(target + 0x30);

		if (hero_stoned(target)) {
			damage = 0;
		}

		if (host_readws(target + 0x60) < damage) {
			damage = host_readws(target + 0x60) + 1;
		}
	}

	return damage;
}

/**
 *	FIG_get_enemy_attack_damage() - calculates attack damage from an enemy
 *	@attacker:	the enemy which attacks
 *	@attacked:	the enemy/hero which gets attacked
 *	@is_enemy:	1 = if attacked is an enemy / 0 = attacked is a hero
 *
 */

signed short FIG_get_enemy_attack_damage(Bit8u *attacker, Bit8u *attacked, signed short is_enemy)
{
	signed short pos;
	Bit8u *hero;

	register signed short damage;
	register signed short dice;

	dice = host_readw(attacker + 0x1e);

	if (host_readw(attacker + 0x20) != 0 && random_schick(100) < 50)
		dice = host_readw(attacker + 0x20);

	damage = dice_template(dice);

	if (!is_enemy) {
		/* the attacked is a hero */
		hero = attacked;

		/* subtract RS */
		damage -= host_readbs(hero + 0x30);

		/* armour bonus against skelettons an zombies */
		if (host_readw(hero + 0x1b2) == 0xc5 && (
			host_readb(attacker + 1) == 0x22 ||
			host_readb(attacker + 1) == 0x1c)) {
				damage -= 3;
		}

		/* get position of Totenkopfguertel/Skullbelt */

		if ( (pos = get_item_pos(hero, 0xb6)) != -1 &&
			(host_readb(attacker + 1) == 0x22 ||
			host_readb(attacker + 1) == 0x1c)) {

			/* no damage for the hero who wears it */
			damage = 0;

			/* 55 chance to loose this item on use */
			if (random_schick(100) < 5) {
				drop_item(hero, pos, 1);
				GUI_output(get_dtp(0x2c));
			}
		}

		/* no damage if the hero is stoned */
		if (hero_stoned(hero))
			damage = 0;
	} else {
		/* the attacked is an enemy */

		/* subtract RS */
		damage -= host_readbs(attacked + 0x2);

		/* check unknown flag, maybe stoned */
		if (enemy_stoned(attacked))
			damage = 0;

		/* check if the attacked is immune
		 * against non-magicial weapons */
		if (host_readb(attacked + 0x24) != 0)
			damage = 0;
	}

	/* damage bonus */
	damage += host_readbs(attacker + 0x2e);

	/* half damage */
	if (host_readb(attacker + 0x30) != 0)
		damage /= 2;

	return damage;
}

void seg041_8c8(void)
{
	signed short i;

	for (i = 0; i < 8; i++)
		memset(p_datseg + 0xd8ce + i * 0xf3, -1, 0xf3);

	/* That would be better */
	/* memset(p_datseg + 0xd8ce, -1, 0xf3 * 8); */
}

/**
 *	weapon_check()	-	check if a hero is equipped with a vaild weapon
 *	@hero:	pointer to hero
 *
 *	Returns: -1 not a useful weapon, 0 = knive weapon, 1 = force weapon, 2 = everything else
*/
signed short weapon_check(Bit8u *hero)
{
	Bit8u *item_p;

	signed short item;
	signed short l_di;

	/* get the ID of the equipped weapon */
	item = host_readw(hero + 0x1c0);

	item_p = get_itemsdat(item);

	if (!item_weapon(item_p) ||	/* check if its a weapon */
		ks_broken(hero + 0x1c0) ||
		(item_weapon(item_p) &&
			((host_readbs(item_p + 3) == 7) ||
			(host_readbs(item_p + 3) == 8) ||
			(host_readbs(item_p + 3) == 5 && item != 133 && item != 69))))
	{
		l_di = -1;
	} else {
		if (is_in_word_array(item, (signed short*)(p_datseg + FORCE_WEAPONS))) {
			l_di = 1;
		} else if (is_in_word_array(item, (signed short*)(p_datseg + KNIVE_WEAPONS))) {
			l_di = 0;
		} else {
			l_di = 2;
		}
	}

	return l_di;
}

#if !defined(__BORLANDC__)
}
#endif
