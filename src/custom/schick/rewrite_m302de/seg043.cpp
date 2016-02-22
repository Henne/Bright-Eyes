/**
 *	Rewrite of DSA1 v3.02_de functions of seg043 (fightsystem: monster action, use item)
 *	Functions rewritten: 2/2 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg043.cpp
 */
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg005.h"
#include "seg006.h"
#include "seg007.h"
#include "seg032.h"
#include "seg038.h"
#include "seg041.h"
#include "seg044.h"
#include "seg045.h"
#include "seg097.h"
#include "seg102.h"
#include "seg105.h"
#include "seg107.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct coords {
	signed char x, y;
};

struct dummy {
	struct coords a[4];
};

struct msg {
	signed short pos, type;
};

/**
 * \brief	execute the fight action of a monster
 *
 * \param	monster		pointer to a monster datasheet
 * \param	monster_pos	position of the monster (fight_id = monster_pos + 10)
 */
void FIG_do_monster_action(RealPt monster, signed short monster_pos)
{
	signed short damage;

	Bit8u *hero;
	Bit8u *mon;
	Bit8u *p_weapon;
	signed short two_w_6;
	signed short weapon_type;
	signed short l4;
	signed short randval;
	signed short l6;
	signed short l7;
	signed short at;
	signed short pa;
	signed char attack_hero;
	signed short l11;
	signed short l12;
	signed short l13;
	signed short l14;
	signed short l15;
	Bit8u *l16;
	signed short l17 = 0;
	signed short fight_id;
	signed short hero_x;
	signed short hero_y;
	signed short target_x;
	signed short target_y;
	signed short dir;
	struct dummy dst = *(struct dummy*)(p_datseg + 0x6190);
	struct msg tmp;

	update_mouse_cursor();

	if (ds_readws(IN_FIGHT) != 0) {

		FIG_clear_msgs();

		l4 = ds_writews(0xe3ac, ds_writews(0xe3aa, ds_writews(0xe3a8, ds_writews(0xe3a6, 0))));

		ds_writew(0xe2b8, 1);
		ds_writew(0xe2ba, host_readbs(Real2Host(monster)));

		if (host_readbs(Real2Host(monster) + 0x2d) < 10) {

			/* monster attacks hero */
			hero = get_hero(host_readbs(Real2Host(monster) + 0x2d) - 1);

			ds_writew(0xe2bc, 2);
			ds_writew(0xe2be, host_readbs(Real2Host(monster) + 0x2d) - 1);

			if (hero_dead(hero) || !host_readbs(hero + HERO_TYPE)) {
				return;
			}

			attack_hero = 1;
		} else {

			mon = p_datseg + 0xd0df + 0x3e * host_readbs(Real2Host(monster) + 0x2d);

			ds_writew(0xe2bc, 1);
			ds_writew(0xe2be, host_readbs(mon));

			if (enemy_dead(mon) || !host_readbs(mon)) {
				return;
			}

			attack_hero = 0;

			if ((is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) &&
				(l17 == 0))
			{
				FIG_search_obj_on_cb(host_readbs(Real2Host(monster) + 0x2d), &target_x, &target_y);
				FIG_search_obj_on_cb(monster_pos + 10, &hero_x, &hero_y);

#if !defined(__BORLANDC__)
				/* BE-fix */
				target_x = host_readws((Bit8u*)&target_x);
				target_y = host_readws((Bit8u*)&target_y);
				hero_x = host_readws((Bit8u*)&hero_x);
				hero_y = host_readws((Bit8u*)&hero_y);
#endif
				if (hero_x == target_x) {

					if (target_y < hero_y) {
						dir = 3;
					} else {
						dir = 1;
					}
				} else {
					if (target_x < hero_x) {
						dir = 0;
					} else {
						dir = 2;
					}
				}

				if (host_readbs(mon + 0x27) != dir) {

					fight_id = get_cb_val(hero_x + dst.a[dir].x, hero_y + dst.a[dir].y);

					if (fight_id != 0) {

						if ((fight_id >= 50) ||
							((fight_id < 10) && !hero_dead(get_hero(fight_id - 1))) ||
							((fight_id >= 10) && (fight_id < 30) && !enemy_dead((p_datseg + 0xd0df) + (0x3e * fight_id))) ||
							((fight_id >= 30) && (fight_id < 50) && !enemy_dead((p_datseg + 0xcc07) + (0x3e * fight_id))))
						{
							l17 = 1;
						}
					}
				}

			}
		}

		if (host_readbs(Real2Host(monster) + 0x2b) == 2) {

			if (host_readbs(Real2Host(monster) + 0x2d) < 10) {

				/* attack a hero */

				p_weapon = hero + HERO_ITEM_RIGHT;

				weapon_type = weapon_check(hero);

				if (weapon_type == -1) {
					/* no valid weapon == bare hands */
					at = host_readbs(hero + HERO_AT) + host_readbs(hero + HERO_ATTACK_TYPE);
					pa = host_readbs(hero + HERO_PA) - host_readbs(hero + HERO_ATTACK_TYPE);
				} else {
					at = host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WP_CLASS)) + host_readbs(hero + HERO_ATTACK_TYPE) + host_readbs(hero + HERO_AT_MOD);
					pa = host_readbs(hero + HERO_PA + host_readbs(hero + HERO_WP_CLASS)) - host_readbs(hero + HERO_ATTACK_TYPE) + host_readbs(hero + HERO_PA_MOD);
				}

				/* guarding heros get a PA-bonus of 3 */
				if (host_readbs(hero + HERO_UNKNOWN2) == 3) {
					pa += 3;
				}

				/* after destroying the orc statuette between Oberorken and Felsteyn, dwarfs get a PA-bonus against orcs */
				if ((ds_readbs(0x3dda) != 0) &&
					(host_readbs(hero + HERO_TYPE) == 6) &&
					(host_readbs(Real2Host(monster) + 1) == 24))
				{
					pa++;
				}

				/* subtract RS handycap */
				at -= host_readbs(hero + HERO_RS_BE) / 2;
				pa -= host_readbs(hero + HERO_RS_BE) / 2;

			} else {
				/* attack a monster */
				at = host_readbs(mon + 0x1c);
				pa = host_readbs(mon + 0x1d);
			}

			/* spell_dunkelheit() is active => AT-4, PA-4*/
			if (ds_readds(INGAME_TIMERS + 0x24)) {
				at -= 4;
				pa -= 4;
			}

			l7 = host_readbs(Real2Host(monster) + 0x1c);
			if (ds_readds(INGAME_TIMERS + 0x24)) {
				l7 -= 4;
			}

			if (attack_hero != 0) {

				/* TODO */
				if (ds_readbs(0xd84a + host_readbs(Real2Host(monster) + 0x2d)) == 1) {
					l7 += 2;
				}

				/* spell_chamaelioni() is active on that hero => AT-5 */
				if (hero_cham(hero) == 1) {
					l7 -= 5;
				}

				/* TODO */
				if (hero_dummy1(hero) == 1) {
					l7 /= 2;
				}
			} else {
				/* TODO */
				if (ds_readbs(0xd82d + host_readbs(Real2Host(monster) + 0x2d)) == 1) {
					l7 += 2;
				}
			}

			randval = random_schick(20);

			if ((randval == 20) && (random_schick(20) > l7 - 9)) {

				if (!attack_hero || check_hero(hero)) {

					FIG_add_msg(3, 0);

					two_w_6 = random_interval(2, 12);

					if ((two_w_6 >= 3) && (two_w_6 <= 8)) {

						ds_writew(0xe3aa, 1);

						if (random_schick(20) <= at) {

							if (attack_hero != 0) {
								damage = FIG_get_hero_melee_attack_damage(hero, Real2Host(monster), 0);
							} else {
								damage = FIG_get_enemy_attack_damage(mon, Real2Host(monster), 1);
							}

							if (damage > 0) {

								FIG_damage_enemy(Real2Host(monster), damage, 1);

								FIG_add_msg(11, damage);

								/* swap msg struct */
								tmp = *(struct msg*)(p_datseg + 0xe2bc);
								*(struct msg*)(p_datseg + 0xe2bc) = *(struct msg*)(p_datseg + 0xe2b8);
								*(struct msg*)(p_datseg + 0xe2b8) = tmp;

								if (enemy_dead(Real2Host(monster))) {
									ds_writew(0xe3a8, 1);
								}
							}
						}
					} else if ((two_w_6 >= 9) && (two_w_6 <= 11)) {

						damage = random_schick(6);

						FIG_damage_enemy(Real2Host(monster), damage, 1);

						FIG_add_msg(11, damage);

						*(struct msg*)(p_datseg + 0xe2bc) = *(struct msg*)(p_datseg + 0xe2b8);

						if (enemy_dead(Real2Host(monster))) {
							ds_writew(0xe3a8, 1);
						}
					} else if (two_w_6 == 12) {
					}
				}
			} else {

				if (randval <= l7) {

					if (((attack_hero != 0) && !ds_readbs(0xd84a + host_readbs(Real2Host(monster) + 0x2d)) && check_hero(hero)) ||
						(!attack_hero && (!ds_readbs(0xd82d + host_readbs(Real2Host(monster) + 0x2d)))))
					{

						l6 = random_schick(20);

						if ((l6 == 20) && (random_schick(20) > pa - 7)) {

							FIG_add_msg(2, 0);

							two_w_6 = random_interval(2, 12);

							if ((two_w_6 >= 3) && (two_w_6 <= 8)) {

								ds_writew(0xe3ac, 1);

								if (random_schick(20) <= l7) {

									if (attack_hero != 0) {
										damage = FIG_get_enemy_attack_damage(Real2Host(monster), hero, 0);

										if (damage > 0) {

											/* HESHTOT */
											if (host_readbs(Real2Host(monster)) != 77) {
												sub_hero_le(hero, damage);
											}

											FIG_add_msg(8, damage);

											if (hero_dead(hero)) {
												ds_writew(0xe3a6, 1);
											}
										}
									} else {
										damage = FIG_get_enemy_attack_damage(Real2Host(monster), mon, 1);

										if (damage > 0) {

											FIG_damage_enemy(mon, damage, 1);

											FIG_add_msg(11, damage);

											if (enemy_dead(mon)) {
												ds_writew(0xe3a6, 1);
											}
										}
									}
								}

							} else if ((two_w_6 >= 9) && (two_w_6 <= 11)) {

								damage = random_schick(6);

								if (attack_hero != 0) {

									sub_hero_le(hero, damage);

									FIG_add_msg(8, damage);

									if (hero_dead(hero)) {
										ds_writew(0xe3a6, 1);
									}

								} else {

									FIG_damage_enemy(mon, damage, 1);

									FIG_add_msg(11, damage);

									if (enemy_dead(mon)) {
										ds_writew(0xe3a6, 1);
									}
								}

							}
						} else {
							if (l6 > pa) {
								l4 = 1;
							} else {

								FIG_add_msg(5, 0);

								/* TODO */
								if ((l6 == randval) && (attack_hero != 0) && (host_readbs(p_weapon + 6) != (signed char)0x9d)) {

									if (host_readbs(p_weapon + 6) > 3) {

										if (random_schick(12) + host_readbs(p_weapon + 6) > 15) {

											or_ptr_bs(p_weapon + 4, 1);

											FIG_add_msg(6, 0);
										} else {
											inc_ptr_bs(p_weapon + 6);
										}
									} else {
										inc_ptr_bs(p_weapon + 6);
									}
								}
							}
						}
					} else {
						l4 = 1;
					}

					if (l4 != 0) {

						if (attack_hero != 0) {

							damage = FIG_get_enemy_attack_damage(Real2Host(monster), hero, 0);

							if (damage > 0) {

								if (host_readbs(Real2Host(monster)) != 77) {
									sub_hero_le(hero, damage);
								}

								FIG_add_msg(8, damage);

								if (hero_dead(hero)) {
									ds_writew(0xe3a6, 1);
								}
							}
						} else {

							damage = FIG_get_enemy_attack_damage(Real2Host(monster), mon, 1);

							if (damage > 0) {

								FIG_damage_enemy(mon, damage, 1);

								FIG_add_msg(11, damage);

								if (enemy_dead(mon)) {
									ds_writew(0xe3a6, 1);
								}
							}
						}
					}
				}
			}

			seg041_8c8();

			if (attack_hero != 0) {

				if (check_hero(hero) || (ds_readws(0xe3a6) != 0)) {

					FIG_prepare_hero_fight_ani(0, hero, weapon_type, 100, host_readbs(Real2Host(monster) + 0x2d), monster_pos + 10, 1);
				}

			} else if (l17 == 0) {
					FIG_prepare_enemy_fight_ani(0, mon, 100, host_readbs(Real2Host(monster) + 0x2d), monster_pos + 10, 1);
			} else if (ds_readws(0xe3a6) != 0) {
					FIG_prepare_enemy_fight_ani(0, mon, 0, host_readbs(Real2Host(monster) + 0x2d), monster_pos + 10, 1);
			}

			FIG_prepare_enemy_fight_ani(1, Real2Host(monster), 2, monster_pos + 10, host_readbs(Real2Host(monster) + 0x2d), 0);
			ds_writew(0x26b1, 1);
			draw_fight_screen_pal(0);
			seg041_8c8();

		} else {

			if (host_readbs(Real2Host(monster) + 0x2b) == 15) {

				if (host_readbs(Real2Host(monster) + 0x37) > 0) {
					l15 = 3;
					dec_ptr_bs(Real2Host(monster) + 0x37);
				} else {
					l15 = 4;
					dec_ptr_bs(Real2Host(monster) + 0x3a);
				}

				if (attack_hero != 0) {

					/* attack_hero */

					damage = dice_template(l15 == 3 ? host_readws(Real2Host(monster) + 0x38) : host_readws(Real2Host(monster) + 0x3b));

					damage = (damage * 8) / 10;

					/* RS */
					damage -= host_readbs(hero + HERO_RS_BONUS1);

					if (damage > 0) {

						sub_hero_le(hero, damage);

						FIG_add_msg(8, damage);

						if (hero_dead(hero)) {
							ds_writew(0xe3a6, 1);
						}
					}

				} else {

					/* attack monster */

					damage = dice_template(l15 == 3 ? host_readws(Real2Host(monster) + 0x38) : host_readws(Real2Host(monster) + 0x3b))
							- host_readbs(mon + 0x02);

					if (damage > 0) {

						FIG_damage_enemy(mon, damage, 1);

						FIG_add_msg(11, damage);

						if (enemy_dead(mon)) {
							ds_writew(0xe3a6, 1);
						}
					}
				}

				seg041_8c8();

				l11 = l15;
				l12 = 0;

				FIG_call_draw_pic();

				FIG_prepare_enemy_fight_ani(0, Real2Host(monster), 15, monster_pos + 10, host_readbs(Real2Host(monster) + 0x2d), 0);

				l12 = seg045_01a0(7, l11, monster_pos + 10, host_readbs(Real2Host(monster) + 0x2d), host_readbs(Real2Host(monster) + 0x27));

				FIG_set_0e(host_readbs(Real2Host(monster) + 0x26), 0);

				draw_fight_screen_pal(0);

				if (l12 != 0) {

					FIG_set_0e(ds_readbs(0xe38d), 7);

					draw_fight_screen((l12 == 0) && (ds_readws(0xe3a6) == 0) ? 0 : 1);

					FIG_reset_12_13(ds_readbs(0xe38d));
				}

				ds_writew(0x26b1, 1);

				if (ds_readws(0xe3a6) != 0) {

					if (attack_hero != 0) {

						FIG_prepare_hero_fight_ani(1, hero, -1, 0, host_readbs(Real2Host(monster) + 0x2d), monster_pos + 10, 1);
					} else {

						FIG_prepare_enemy_fight_ani(1, mon, 0, host_readbs(Real2Host(monster) + 0x2d), monster_pos + 10, 1);
					}
				}

				FIG_remove_smth();
				draw_fight_screen(0);
				seg041_8c8();

			} else if (host_readbs(Real2Host(monster) + 0x2b) == 4) {

				/* spellcast */

				l14 = ds_readbs((0x0f13 + 0x7) + 8 * host_readbs(Real2Host(monster) + 0x2c));

				host_writebs(Real2Host(ds_readd(DTP2)), 0);

				l13 = MON_cast_spell(monster, 0);

				seg041_8c8();

				if (host_readbs(Real2Host(monster) + 0x2d) != 0) {

					l11 = l12 = 0;

					if (random_schick(100) > 50) {
						l11 = 1;
					}

					if (host_readbs(Real2Host(monster) + 0x2d) < 10) {
						l11 = 2;
					}

					FIG_call_draw_pic();

					if (l13 != -1) {

						seg044_002f(0, Real2Host(monster), 4, monster_pos + 10, host_readbs(Real2Host(monster) + 0x2d), 0);
					}

					if (l13 > 0) {

						if (l14 > 0) {

							seg045_041b(6, Real2Host(monster), l14);

						} else {

							if (host_readbs(Real2Host(monster) + 0x2d) > 0) {

								if (!attack_hero) {

									seg044_002f(1, mon, 99, host_readbs(Real2Host(monster) + 0x2d), monster_pos + 10, 1);
								} else {

									if (check_hero(hero) || (ds_readws(0xe3a6) != 0)) {

										seg044_002a(1, hero, 99, host_readbs(Real2Host(monster) + 0x2d), 0, -1, 1);
									}
								}
							}
						}

						if ((host_readbs(Real2Host(monster) + 0x01) != 0x12) &&
							(host_readbs(Real2Host(monster) + 0x01) != 7) &&
							(host_readbs(Real2Host(monster) + 0x2d) > 0))
						{
							l12 = seg045_01a0(7, l11, monster_pos + 10, host_readbs(Real2Host(monster) + 0x2d), host_readbs(Real2Host(monster) + 0x27));
						}

					}
					if (l13 != -1) {

						FIG_set_0e(host_readbs(Real2Host(monster) + 0x26), 0);

						draw_fight_screen_pal(1);
					}

					if (l13 > 0) {

						if (l12 != 0) {

							FIG_set_0e(ds_readbs(0xe38d), 7);

							draw_fight_screen(1);

							FIG_reset_12_13(ds_readbs(0xe38d));
						}

						if (l14 > 0) {
							FIG_set_0e(ds_readbs(0xe38c), 6);
						}

						if (!attack_hero) {

							FIG_set_0e(host_readbs(mon + 0x26), 1);


							if (is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {

								l16 = Real2Host(FIG_get_ptr(host_readbs(mon + 0x26)));

								FIG_set_0e(ds_readbs(0xe35a + host_readbs(l16 + 0x13)), 3);
							}
						} else {

							if (host_readbs(Real2Host(monster) + 0x2d) > 0) {

								FIG_set_0e(host_readbs(hero + HERO_FIGHT_ID), 1);
							}
						}

						ds_writew(0x26b1, 1);

						draw_fight_screen(1);

						if (l14 > 0) {
							FIG_reset_12_13(ds_readbs(0xe38d));
						}

						if (ds_readw(0x618e) != 0) {

							ds_writew(0x618e, 0);

							FIG_remove_from_list(host_readbs(mon + 0x26), 1);

							ds_writew((0xe066 + 0x00), ds_readbs(0x12c0 + 5 * host_readbs(mon + 0x01)));
							ds_writeb((0xe066 + 0x02), host_readbs(mon + 0x27));
							ds_writeb((0xe066 + 0x05), ds_readbs(0x1531 + 10 * host_readbs(mon + 0x01) + 2 * host_readbs(mon + 0x27)));
							ds_writeb((0xe066 + 0x06), ds_readbs(0x1532 + 10 * host_readbs(mon + 0x01) + 2 * host_readbs(mon + 0x27)));

							if (is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {
								ds_writeb((0xe066 + 0x09), ds_readbs(0x6030 + host_readbs(mon + 0x27)));
								ds_writeb((0xe066 + 0x0b), ds_readbs(0x6034 + host_readbs(mon + 0x27)));
							} else {
								ds_writeb((0xe066 + 0x09), 0);
								ds_writeb((0xe066 + 0x0b), 31);
								ds_writeb((0xe066 + 0x13), -1);
							}

							ds_writeb((0xe066 + 0x0a), 0);
							ds_writeb((0xe066 + 0x0c), 39);
							ds_writeb((0xe066 + 0x07), 40);
							ds_writeb((0xe066 + 0x08), 32);
							ds_writeb((0xe066 + 0x15), 1);
							ds_writeb((0xe066 + 0x0d), -1);
							ds_writeb((0xe066 + 0x0f), -1);
							ds_writeb((0xe066 + 0x0e), -1);

							FIG_add_to_list(host_readbs(mon + 0x26));
						}

						if (ds_readws(0xe3a4) != 0) {

							if (host_readbs(Real2Host(monster) + 0x2d) >= 10) {

								FIG_reset_12_13(host_readbs(mon + 0x26));

								if (is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {

									l16 = Real2Host(FIG_get_ptr(host_readbs(mon + 0x26)));

									FIG_reset_12_13(ds_readbs(0xe35a + host_readbs(l16 + 0x13)));
								}
							} else {

								if (host_readbs(Real2Host(monster) + 0x2d) > 0) {

									FIG_reset_12_13(host_readbs(hero + HERO_FIGHT_ID));
								}
							}
						}

						if (l12 != 0) {
							FIG_remove_smth();
						}

						if (l14 > 0) {
							FIG_remove_smth2();
						}

						FIG_draw_figures();
					}


					FIG_output(Real2Host(ds_readd(DTP2)));

					seg041_8c8();
				} else {
					FIG_output(Real2Host(ds_readd(DTP2)));
				}
			}
		}

	}

	refresh_screen_size();
}

/**
 * \brief
 *
 * \param hero			pointer to the hero who uses the item
 * \param target_monster	pointer to the monster
 * \param target_hero		pointer to the hero
 * \param flag			bool value, used when a hero attacks a hero
 * \param hero_pos		position of the hero
 */
void FIG_use_item(Bit8u *hero, Bit8u *target_monster, Bit8u *target_hero, signed short flag, signed short hero_pos)
{
	signed short damage;

	signed short l3;
	signed short hylailic = 0;
	signed short usecase;
	signed short item_id = host_readws(hero + HERO_ITEM_LEFT);
	Bit8u *p_item = get_itemsdat(item_id);

	if (item_herb_potion(p_item)) {
		usecase = 1;
	} else if (!item_useable(p_item) || (host_readws(hero + 0x1d0) == 0)) {
		usecase = 0;
	} else {
		usecase = 2;
	}

	host_writeb(Real2Host(ds_readd(DTP2)), 0);

	if (host_readws(hero + HERO_ITEM_LEFT) == 238) {
		/* MIASTHMATIC */

		/* 1W6 + 4 */
		damage = dice_roll(1, 6, 4);

		if (host_readbs(hero + HERO_ENEMY_ID) >= 10) {

			strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(0x94));

			FIG_damage_enemy(target_monster, damage, 0);

			FIG_add_msg(11, damage);

			if (enemy_dead(target_monster)) {
				ds_writew(0xe3a6, 1);
			}
		} else {

			if (flag != 0) {

				strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(0x94));

				sub_hero_le(target_hero, damage);

				FIG_add_msg(8, damage);

				if (hero_dead(target_hero)) {
					ds_writew(0xe3a6, 1);
				}
			}
		}

		/* drop the item in the left hand */
		drop_item(hero, 4, 1);

	} else if (host_readws(hero + HERO_ITEM_LEFT) == 239) {

		/* HYLAILIC FIRE */

		if (host_readbs(hero + HERO_ENEMY_ID) >= 10) {

			/* .. used on a monster */

			FIG_damage_enemy(target_monster, 20, 0);

			FIG_add_msg(11, 20);

			if (enemy_dead(target_monster)) {
				ds_writew(0xe3a6, 1);
			}
		} else {
			/* .. used on another hero */
			if (flag != 0) {

				sub_hero_le(target_hero, 20);

				FIG_add_msg(8, 20);

				if (hero_dead(target_hero)) {
					ds_writew(0xe3a6, 1);
				}
			}
		}

		/* drop the item in the left hand */
		drop_item(hero, 4, 1);

		hylailic = 1;
	} else {
		/* use item in the regular way */

		use_item(4, hero_pos);

		/* TODO: needless */
		host_writeb(Real2Host(ds_readd(DTP2)), 0);
	}

	if (usecase > 0) {

		seg041_8c8();

		FIG_prepare_hero_fight_ani(0, hero, -1, usecase == 1 ? 102 : 103, hero_pos + 1, host_readbs(hero + HERO_ENEMY_ID), 0);

		l3 = 0;

		if (hylailic != 0) {
			seg045_0394(6, hero, 2);
		} else {
			ds_writew(0x26b1, 1);
		}

		draw_fight_screen_pal(0);

		if (hylailic != 0) {

			FIG_set_0e(ds_readbs(0xe38c), 6);

			l3 = 1;

			ds_writew(0x26b1, 1);

			draw_fight_screen(1);
		}

		if (l3 != 0) {
			FIG_remove_smth2();
		}

		if (ds_readws(0xe3a6) != 0) {

			if (flag != 0) {
				FIG_prepare_hero_fight_ani(1, target_hero, -1, 0, host_readbs(hero + HERO_ENEMY_ID), hero_pos + 1, 1);
			} else {
				FIG_prepare_enemy_fight_ani(1, target_monster, 0, host_readbs(hero + HERO_ENEMY_ID), hero_pos + 1, 1);
			}

		}

		if ((l3 != 0) || (ds_readws(0xe3a6) != 0)) {
			draw_fight_screen(0);
		}

		seg041_8c8();
	}

	if (host_readb(Real2Host(ds_readd(DTP2))) != 0) {
		/* show output string if needed */
		GUI_output(Real2Host(ds_readd(DTP2)));
	}
}

#if !defined(__BORLANDC__)
}
#endif
