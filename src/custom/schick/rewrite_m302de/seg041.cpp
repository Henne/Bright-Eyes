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
/* FIG_MSG_COUNTER */
static unsigned short msg_counter;
#endif

/**
 * \brief	check if a range attack can be done
 *
 * \param hero	the hero who attacks
 * \param arg	0 = drop one unit, 1 = just check, 2 = check with output
 *
 * \return	0 = no ammo / 1 = have ammo
 */
signed short range_attack_check_ammo(Bit8u *hero, signed short arg)
{
	signed short right_hand;
	signed short left_hand;
	signed short retval;

	retval = 0;

	/* read the item ids from the hands */
	right_hand = host_readws(hero + HERO_ITEM_RIGHT);
	left_hand = host_readws(hero + HERO_ITEM_LEFT);

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

			if (ds_readws(FIG_DROPPED_COUNTER) < 30) {
				ds_writew(FIG_DROPPED_WEAPONS + ds_readw(FIG_DROPPED_COUNTER) * 2, right_hand);
				inc_ds_ws(FIG_DROPPED_COUNTER);
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
					(char*)hero + HERO_NAME2);

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
					(char*)hero + HERO_NAME2);

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
					(char*)hero + HERO_NAME2);

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
	memset(p_datseg + FIG_MSG_DATA, 0 , 20);
	ds_writew(FIG_MSG_COUNTER, 0);
}

void FIG_add_msg(unsigned short f_action, unsigned short damage)
{
	ds_writew(FIG_MSG_DATA + 4 * ds_readws(FIG_MSG_COUNTER), f_action);
	ds_writew(FIG_MSG_DATA + 2 + 4 * ds_readws(FIG_MSG_COUNTER) , damage);
	if (ds_readws(FIG_MSG_COUNTER) < 4)
		inc_ds_ws(FIG_MSG_COUNTER);
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
	sub_ptr_ws(enemy + ENEMY_SHEET_LE, damage);

	/* are the enemies LE lower than 0 */
	if (host_readws(enemy + ENEMY_SHEET_LE) <= 0) {
		/* set a flag, maybe dead */
		or_ptr_bs(enemy + ENEMY_SHEET_STATUS1, 1);
		/* set LE to 0 */
		host_writew(enemy + ENEMY_SHEET_LE, 0);

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
		and_ptr_bs(enemy + ENEMY_SHEET_STATUS2, 0xfd);
}

signed short FIG_get_hero_melee_attack_damage(Bit8u* hero, Bit8u* target, signed short attack_hero)
{
	signed short damage;
	signed short damage_mod;
	Bit8u* item_p_rh;
	Bit8u* p_weapontab;
	Bit8u* p_rangedtab;
	signed short target_size_bonus;
	signed short right_hand;
	signed short beeline;
	signed short distance_malus;
	signed short x_hero;
	signed short y_hero;
	signed short x_target;
	signed short y_target;
	signed short hero_idx;
	signed char enemy_gfx_id;
	Bit8u* enemy_p;
	signed short weapon_type;

	damage_mod = 0;

	if (attack_hero == 0) {
		enemy_p = target;
	}

	right_hand = host_readw(hero + HERO_ITEM_RIGHT);

	item_p_rh = get_itemsdat(right_hand);

	weapon_type = weapon_check(hero);

	if (weapon_type == -1) {
		weapon_type = FIG_get_range_weapon_type(hero);
	}

	if (weapon_type != -1) {

		p_weapontab = p_datseg + WEAPONS_TABLE + host_readbs(item_p_rh + 4) * 7;

		damage = dice_roll(host_readbs(p_weapontab), 6, host_readbs(p_weapontab + 1));

		if (host_readbs(p_weapontab + 4) != -1) {

			hero_idx = get_hero_index(hero);

			FIG_search_obj_on_cb(hero_idx + 1, &x_hero, &y_hero);
			FIG_search_obj_on_cb(host_readbs(hero + HERO_ENEMY_ID), &x_target, &y_target);

#if !defined(__BORLANDC__)
			/* BE-fix */
			x_hero = host_readws((Bit8u*)&x_hero);
			y_hero = host_readws((Bit8u*)&y_hero);
			x_target = host_readws((Bit8u*)&x_target);
			y_target = host_readws((Bit8u*)&y_target);
#endif
			beeline = calc_beeline(x_hero, y_hero, x_target, y_target);

			if (beeline <= 2) {
				distance_malus = 0;
			} else if (beeline <= 4) {
				distance_malus = 1;
			} else if (beeline <= 6) {
				distance_malus = 2;
			} else if (beeline <= 9) {
				distance_malus = 3;
			} else if (beeline <= 15) {
				distance_malus = 4;
			} else if (beeline <= 20) {
				distance_malus = 5;
			} else {
				distance_malus = 6;
			}

			p_rangedtab = p_datseg + RANGED_WEAPONS_TABLE + host_readbs(p_weapontab + 4) * 8;

			if (attack_hero != 0) {
				if (host_readbs(target + HERO_TYPE) == HERO_TYPE_DWARF) {
					/* ZWERG / DWARF */
					target_size_bonus = 2;
				} else {
					target_size_bonus = 3;
				}
			} else {
					/* size of the enemy */
				target_size_bonus = host_readbs(target + ENEMY_SHEET_SIZE);
			}

			damage_mod = (test_skill(hero,
					(host_readbs(item_p_rh + 3) == 8 ? 8 : 7),
					host_readbs(p_rangedtab + 7) + 2 * distance_malus - 2 * target_size_bonus) > 0)
				? ds_readbs(RANGED_WEAPONS_TABLE + 8 * host_readbs(p_weapontab + 4) + distance_malus)
				: -damage;

			if (damage_mod != 0) {
				damage += damage_mod;
			}


		} else {

			damage_mod = host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK)) - host_readbs(p_weapontab + 2);
			if (damage_mod > 0) {
				damage += damage_mod;
			}
		}
	} else {
		damage = random_schick(6);
	}

	if (attack_hero == 0) {

		enemy_gfx_id = host_readbs(enemy_p + ENEMY_SHEET_GFX_ID);

		/* magic SABRE gives Damage + 1 to SKELETONS and ZOMBIES */
		if ((right_hand == 172) && (enemy_gfx_id == 0x1c || enemy_gfx_id == 0x22)) {
			damage++;
		} else {
			if (right_hand == 193) {
				/* KUKRIS DAGGER / KUKRISDOLCH */

				/* Interesting */
				damage = 1000;

				/* drop the KUKRISDOLCH and equip a normal DOLCH / DAGGER */
				drop_item(hero, 3, 1);
				give_hero_new_item(hero, 14, 1 ,1);
				move_item(3, get_item_pos(hero, 14), hero);

			} else if (right_hand == 200) {
				/* KUKRISMENGBILLAR */

				/* Interesting */
				damage = 1000;

				/* drop the KUKRISMENGBILAR and equip a normal MENGBILAR  */
				drop_item(hero, 3, 1);
				give_hero_new_item(hero, 109, 1 ,1);
				move_item(3, get_item_pos(hero, 109), hero);

			} else if ((right_hand == 214) && (enemy_gfx_id == 0x1c)) {
				/* SILVER MACE/ SILBERSTREITKOLBEN gives Damage + 4 to SKELETONS */
				damage += 4;
			} else if ((right_hand == 181) && (enemy_gfx_id == 0x18)) {
				/* DAS SCHWERT GRIMRING gives Damage + 5 to ORCS */
				damage += 5;
			}
		}
	}

	damage += host_readbs(hero + HERO_SAFTKRAFT);

	if (damage > 0) {

		if (ks_poison1(hero + HERO_ITEM_RIGHT)) {
			damage += dice_roll(1, 6, 2);
			and_ptr_bs(hero + (HERO_ITEM_RIGHT+4), 0xdf);
		}

		if (ks_poison2(hero + HERO_ITEM_RIGHT)) {
			damage += dice_roll(1, 20, 5);
			and_ptr_bs(hero + (HERO_ITEM_RIGHT+4), 0xbf);
		}

		if (host_readbs(hero + HERO_ITEM_RIGHT + 9) != 0) {

			if (host_readbs(hero + HERO_ITEM_RIGHT + 9) == 3) {
				or_ptr_bs(enemy_p + ENEMY_SHEET_STATUS2, 0x04);
				and_ptr_bs(enemy_p + ENEMY_SHEET_STATUS2, 0xfd);
			} else {

				damage += 10 * ds_readws(POISON_PRICES + 2 * host_readbs(hero + HERO_ITEM_RIGHT + 9));
			}

			dec_ptr_bs(hero + HERO_ITEM_RIGHT + 10);

			if (!host_readbs(hero + HERO_ITEM_RIGHT + 10)) {
				host_writebs(hero + HERO_ITEM_RIGHT + 9, 0);
			}
		}
	}

	if (host_readbs(hero + HERO_ECLIPTIFACTUS) != 0) {
		damage *= 2;
	}

	if ((ds_readbs(0x3dda) != 0) &&
		(host_readbs(hero + HERO_TYPE) == HERO_TYPE_DWARF) &&
		(attack_hero == 0) &&
		(host_readbs(enemy_p + ENEMY_SHEET_GFX_ID) == 0x18))
	{
		damage++;
	}

	if (attack_hero == 0) {
		damage -= host_readbs(enemy_p + 2);

		if (enemy_stoned(enemy_p)) {
			damage = 0;
		}

		if ((host_readbs(enemy_p + ENEMY_SHEET_MAGIC) != 0) && !ks_magic_hidden(hero + HERO_ITEM_RIGHT)) {
			damage = 0;
		}

		if (host_readws(enemy_p + ENEMY_SHEET_LE) < damage) {
			damage = host_readws(enemy_p + ENEMY_SHEET_LE) + 1;
		}
	} else {
		damage -= host_readbs(target + HERO_RS_BONUS1);

		if (hero_stoned(target)) {
			damage = 0;
		}

		if (host_readws(target + HERO_LE) < damage) {
			damage = host_readws(target + HERO_LE) + 1;
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

	signed short damage;
	signed short dice;

	dice = host_readw(attacker + ENEMY_SHEET_DAM1);

	if (host_readw(attacker + ENEMY_SHEET_DAM2) != 0 && random_schick(100) < 50)
		dice = host_readw(attacker + ENEMY_SHEET_DAM2);

	damage = dice_template(dice);

	if (!is_enemy) {
		/* the attacked is a hero */
		hero = attacked;

		/* subtract RS */
		damage -= host_readbs(hero + HERO_RS_BONUS1);

		/* armour bonus against skelettons an zombies */
		if (host_readw(hero + HERO_ITEM_BODY) == 0xc5 && (
			host_readb(attacker + ENEMY_SHEET_GFX_ID) == 0x22 ||
			host_readb(attacker + ENEMY_SHEET_GFX_ID) == 0x1c)) {
				damage -= 3;
		}

		/* get position of Totenkopfguertel/Skullbelt */

		if ( (pos = get_item_pos(hero, 0xb6)) != -1 &&
			(host_readb(attacker + ENEMY_SHEET_GFX_ID) == 0x22 ||
			host_readb(attacker + ENEMY_SHEET_GFX_ID) == 0x1c)) {

			/* no damage for the hero who has it */
			damage = 0;

			/* 5% chance to loose this item */
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
		damage -= host_readbs(attacked + ENEMY_SHEET_RS);

		/* check unknown flag, maybe stoned */
		if (enemy_stoned(attacked))
			damage = 0;

		/* check if the attacked is immune
		 * against non-magicial weapons */
		if (host_readb(attacked + ENEMY_SHEET_MAGIC) != 0)
			damage = 0;
	}

	/* damage bonus */
	damage += host_readbs(attacker + ENEMY_SHEET_DUMMY5);

	/* half damage */
	if (host_readb(attacker + ENEMY_SHEET_BROKEN) != 0)
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
	item = host_readw(hero + HERO_ITEM_RIGHT);

	item_p = get_itemsdat(item);

	if (!item_weapon(item_p) ||	/* check if its a weapon */
		ks_broken(hero + HERO_ITEM_RIGHT) ||
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
