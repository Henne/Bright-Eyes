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
 * \brief   execute the fight action of a monster
 *
 * \param   monster     pointer to a monster datasheet
 * \param   monster_pos position of the monster (fighter_id = monster_pos + 10)
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
	signed short randval2;
	signed short attacker_at;
	signed short defender_at;
	signed short defender_pa;
	signed char attack_hero;
	signed short l11;
	signed short l12;
	signed short l13;
	signed short l14;
	signed short l15;
	Bit8u *l16;
	signed short l17 = 0;
	signed short fighter_id;
	signed short hero_x;
	signed short hero_y;
	signed short target_x;
	signed short target_y;
	signed short dir;
	struct dummy dst = *(struct dummy*)(p_datseg + VIEWDIR_INVOFFSETS3);
	struct msg tmp;

	update_mouse_cursor();

	if (ds_readws(IN_FIGHT) != 0) {

		FIG_clear_msgs();

		l4 = ds_writews(ATTACKER_ATTACKS_AGAIN, ds_writews(DEFENDER_ATTACKS, ds_writews(ATTACKER_DEAD, ds_writews(DEFENDER_DEAD, 0))));

		ds_writew(FIG_ACTOR_GRAMMAR_TYPE, 1);
		ds_writew(FIG_ACTOR_GRAMMAR_ID, host_readbs(Real2Host(monster)));

		if (host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID) < 10) {

			/* monster attacks hero */
			hero = get_hero(host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID) - 1);

			ds_writew(FIG_TARGET_GRAMMAR_TYPE, 2);
			ds_writew(FIG_TARGET_GRAMMAR_ID, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID) - 1);

			if (hero_dead(hero) || !host_readbs(hero + HERO_TYPE)) {
				return;
			}

			attack_hero = 1;
		} else {

			mon = p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + SIZEOF_ENEMY_SHEET * host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID);

			ds_writew(FIG_TARGET_GRAMMAR_TYPE, 1);
			ds_writew(FIG_TARGET_GRAMMAR_ID, host_readbs(mon));

			if (enemy_dead(mon) || !host_readbs(mon)) {
				return;
			}

			attack_hero = 0;

			if ((is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) &&
				(l17 == 0))
			{
				FIG_search_obj_on_cb(host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), &target_x, &target_y);
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

				if (host_readbs(mon + ENEMY_SHEET_VIEWDIR) != dir) {

					fighter_id = get_cb_val(hero_x + dst.a[dir].x, hero_y + dst.a[dir].y);

					if (fighter_id != 0) {

						if ((fighter_id >= 50) ||
							((fighter_id < 10) && !hero_dead(get_hero(fighter_id - 1))) ||
							((fighter_id >= 10) && (fighter_id < 30) && !enemy_dead((p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)) + (SIZEOF_ENEMY_SHEET * fighter_id))) ||
							((fighter_id >= 30) && (fighter_id < 50) && !enemy_dead((p_datseg + (ENEMY_SHEETS - 30*SIZEOF_ENEMY_SHEET)) + (SIZEOF_ENEMY_SHEET * fighter_id))))
						{
							l17 = 1;
						}
					}
				}

			}
		}

		if (host_readbs(Real2Host(monster) + ENEMY_SHEET_ACTION_ID) == FIG_ACTION_ATTACK) {

			if (host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID) < 10) {

				/* attack a hero */

				p_weapon = hero + HERO_ITEM_RIGHT;

				weapon_type = weapon_check(hero);

				if (weapon_type == -1) {
					/* no valid weapon == bare hands */
					defender_at = host_readbs(hero + HERO_AT) + host_readbs(hero + HERO_ATTACK_TYPE);
					defender_pa = host_readbs(hero + HERO_PA) - host_readbs(hero + HERO_ATTACK_TYPE);
				} else {
					defender_at = host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WP_CLASS)) + host_readbs(hero + HERO_ATTACK_TYPE) + host_readbs(hero + HERO_AT_MOD);
					defender_pa = host_readbs(hero + HERO_PA + host_readbs(hero + HERO_WP_CLASS)) - host_readbs(hero + HERO_ATTACK_TYPE) + host_readbs(hero + HERO_PA_MOD);
				}

				/* guarding heros get a PA-bonus of 3 */
				if (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_GUARD) {
					defender_pa += 3;
				}

				/* after destroying the orc statuette between Oberorken and Felsteyn, dwarfs get a PA-bonus against orcs */
				if ((ds_readbs(TEVENT071_ORCSTATUE) != 0) &&
					(host_readbs(hero + HERO_TYPE) == HERO_TYPE_DWARF) &&
					(host_readbs(Real2Host(monster) + ENEMY_SHEET_GFX_ID) == 24))
				{
					defender_pa++;
				}

				/* subtract RS handycap */
				defender_at -= host_readbs(hero + HERO_RS_BE) / 2;
				defender_pa -= host_readbs(hero + HERO_RS_BE) / 2;

			} else {
				/* attack a monster */
				defender_at = host_readbs(mon + ENEMY_SHEET_AT);
				defender_pa = host_readbs(mon + ENEMY_SHEET_PA);
			}

			/* spell_dunkelheit() is active => AT-4, PA-4*/
			if (ds_readds(INGAME_TIMERS + 0x24)) {
				defender_at -= 4;
				defender_pa -= 4;
			}

			attacker_at = host_readbs(Real2Host(monster) + ENEMY_SHEET_AT);
			if (ds_readds(INGAME_TIMERS + 0x24)) {
				attacker_at -= 4;
			}

			if (attack_hero != 0) {

				/* TODO */
				if (ds_readbs((HERO_IS_TARGET-1) + host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID)) == 1) {
					attacker_at += 2;
				}

				/* spell_chamaelioni() is active on that hero => AT-5 */
				if (hero_cham(hero) == 1) {
					attacker_at -= 5;
				}

				/* TODO */
				if (hero_dummy1(hero) == 1) {
					attacker_at /= 2;
				}
			} else {
				/* TODO */
				if (ds_readbs(0xd82d + host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID)) == 1) {
					attacker_at += 2;
				}
			}

			randval = random_schick(20);

			if ((randval == 20) && (random_schick(20) > attacker_at - 9)) {

				if (!attack_hero || check_hero(hero)) {

					FIG_add_msg(3, 0);

					two_w_6 = random_interval(2, 12);

					if ((two_w_6 >= 3) && (two_w_6 <= 8)) {

						ds_writew(DEFENDER_ATTACKS, 1);

						if (random_schick(20) <= defender_at) {

							if (attack_hero != 0) {
								damage = FIG_get_hero_melee_attack_damage(hero, Real2Host(monster), 0);
							} else {
								damage = FIG_get_enemy_attack_damage(mon, Real2Host(monster), 1);
							}

							if (damage > 0) {

								FIG_damage_enemy(Real2Host(monster), damage, 1);

								FIG_add_msg(11, damage);

								/* swap msg struct */
								tmp = *(struct msg*)(p_datseg + FIG_TARGET_GRAMMAR_TYPE);
								*(struct msg*)(p_datseg + FIG_TARGET_GRAMMAR_TYPE) = *(struct msg*)(p_datseg + FIG_ACTOR_GRAMMAR_TYPE);
								*(struct msg*)(p_datseg + FIG_ACTOR_GRAMMAR_TYPE) = tmp;

								if (enemy_dead(Real2Host(monster))) {
									ds_writew(ATTACKER_DEAD, 1);
								}
							}
						}
					} else if ((two_w_6 >= 9) && (two_w_6 <= 11)) {

						damage = random_schick(6);

						FIG_damage_enemy(Real2Host(monster), damage, 1);

						FIG_add_msg(11, damage);

						*(struct msg*)(p_datseg + FIG_TARGET_GRAMMAR_TYPE) = *(struct msg*)(p_datseg + FIG_ACTOR_GRAMMAR_TYPE);

						if (enemy_dead(Real2Host(monster))) {
							ds_writew(ATTACKER_DEAD, 1);
						}
					} else if (two_w_6 == 12) {
					}
				}
			} else {

				if (randval <= attacker_at) {

					if (((attack_hero != 0) && !ds_readbs((HERO_IS_TARGET-1) + host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID)) && check_hero(hero)) ||
						(!attack_hero && (!ds_readbs(0xd82d + host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID)))))
					{

						randval2 = random_schick(20);

						if ((randval2 == 20) && (random_schick(20) > defender_pa - 7)) {

							FIG_add_msg(2, 0);

							two_w_6 = random_interval(2, 12);

							if ((two_w_6 >= 3) && (two_w_6 <= 8)) {

								ds_writew(ATTACKER_ATTACKS_AGAIN, 1);

								if (random_schick(20) <= attacker_at) {

									if (attack_hero != 0) {
										damage = FIG_get_enemy_attack_damage(Real2Host(monster), hero, 0);

										if (damage > 0) {

											/* HESHTOT */
											if (host_readbs(Real2Host(monster)) != 77) {
												sub_hero_le(hero, damage);
											}

											FIG_add_msg(8, damage);

											if (hero_dead(hero)) {
												ds_writew(DEFENDER_DEAD, 1);
											}
										}
									} else {
										damage = FIG_get_enemy_attack_damage(Real2Host(monster), mon, 1);

										if (damage > 0) {

											FIG_damage_enemy(mon, damage, 1);

											FIG_add_msg(11, damage);

											if (enemy_dead(mon)) {
												ds_writew(DEFENDER_DEAD, 1);
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
										ds_writew(DEFENDER_DEAD, 1);
									}

								} else {

									FIG_damage_enemy(mon, damage, 1);

									FIG_add_msg(11, damage);

									if (enemy_dead(mon)) {
										ds_writew(DEFENDER_DEAD, 1);
									}
								}

							}
						} else {
							if (randval2 > defender_pa) {
								l4 = 1;
							} else {

								FIG_add_msg(5, 0);

								/* TODO */
								if ((randval2 == randval) && (attack_hero != 0) && (host_readbs(p_weapon + 6) != (signed char)0x9d)) {

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
									ds_writew(DEFENDER_DEAD, 1);
								}
							}
						} else {

							damage = FIG_get_enemy_attack_damage(Real2Host(monster), mon, 1);

							if (damage > 0) {

								FIG_damage_enemy(mon, damage, 1);

								FIG_add_msg(11, damage);

								if (enemy_dead(mon)) {
									ds_writew(DEFENDER_DEAD, 1);
								}
							}
						}
					}
				}
			}

			seg041_8c8();

			if (attack_hero != 0) {

				if (check_hero(hero) || (ds_readws(DEFENDER_DEAD) != 0)) {

					FIG_prepare_hero_fight_ani(0, hero, weapon_type, 100, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
				}

			} else if (l17 == 0) {
					FIG_prepare_enemy_fight_ani(0, mon, 100, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
			} else if (ds_readws(DEFENDER_DEAD) != 0) {
					FIG_prepare_enemy_fight_ani(0, mon, 0, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
			}

			FIG_prepare_enemy_fight_ani(1, Real2Host(monster), 2, monster_pos + 10, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), 0);
			ds_writew(FIG_CONTINUE_PRINT, 1);
			draw_fight_screen_pal(0);
			seg041_8c8();

		} else {

			if (host_readbs(Real2Host(monster) + ENEMY_SHEET_ACTION_ID) == FIG_ACTION_RANGE_ATTACK) {

				if (host_readbs(Real2Host(monster) + ENEMY_SHEET_SHOTS) > 0) {
					l15 = 3;
					dec_ptr_bs(Real2Host(monster) + ENEMY_SHEET_SHOTS);
				} else {
					l15 = 4;
					dec_ptr_bs(Real2Host(monster) + ENEMY_SHEET_THROWS);
				}

				if (attack_hero != 0) {

					/* attack_hero */

					damage = dice_template(l15 == 3 ? host_readws(Real2Host(monster) + ENEMY_SHEET_SHOT_DAM) : host_readws(Real2Host(monster) + ENEMY_SHEET_THROW_DAM));

					damage = (damage * 8) / 10;

					/* RS */
					damage -= host_readbs(hero + HERO_RS_BONUS1);

					if (damage > 0) {

						sub_hero_le(hero, damage);

						FIG_add_msg(8, damage);

						if (hero_dead(hero)) {
							ds_writew(DEFENDER_DEAD, 1);
						}
					}

				} else {

					/* attack monster */

					damage = dice_template(l15 == 3 ? host_readws(Real2Host(monster) + ENEMY_SHEET_SHOT_DAM) : host_readws(Real2Host(monster) + ENEMY_SHEET_THROW_DAM))
							- host_readbs(mon + ENEMY_SHEET_RS);

					if (damage > 0) {

						FIG_damage_enemy(mon, damage, 1);

						FIG_add_msg(11, damage);

						if (enemy_dead(mon)) {
							ds_writew(DEFENDER_DEAD, 1);
						}
					}
				}

				seg041_8c8();

				l11 = l15;
				l12 = 0;

				FIG_call_draw_pic();

				FIG_prepare_enemy_fight_ani(0, Real2Host(monster), 15, monster_pos + 10, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), 0);

				l12 = seg045_01a0(7, l11, monster_pos + 10, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), host_readbs(Real2Host(monster) + ENEMY_SHEET_VIEWDIR));

				FIG_set_0e(host_readbs(Real2Host(monster) + ENEMY_SHEET_FIGHTER_ID), 0);

				draw_fight_screen_pal(0);

				if (l12 != 0) {

					FIG_set_0e(ds_readbs(FIG_SHOT_BOLT_ID), 7);

					draw_fight_screen((l12 == 0) && (ds_readws(DEFENDER_DEAD) == 0) ? 0 : 1);

					FIG_reset_12_13(ds_readbs(FIG_SHOT_BOLT_ID));
				}

				ds_writew(FIG_CONTINUE_PRINT, 1);

				if (ds_readws(DEFENDER_DEAD) != 0) {

					if (attack_hero != 0) {

						FIG_prepare_hero_fight_ani(1, hero, -1, 0, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
					} else {

						FIG_prepare_enemy_fight_ani(1, mon, 0, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
					}
				}

				FIG_remove_smth();
				draw_fight_screen(0);
				seg041_8c8();

			} else if (host_readbs(Real2Host(monster) + ENEMY_SHEET_ACTION_ID) == FIG_ACTION_SPELL) {

				/* spellcast */

				l14 = ds_readbs((MON_SPELL_DESCRIPTIONS + 0x7) + 8 * host_readbs(Real2Host(monster) + ENEMY_SHEET_CUR_SPELL));

				host_writebs(Real2Host(ds_readd(DTP2)), 0);

				l13 = MON_cast_spell(monster, 0);

				seg041_8c8();

				if (host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID) != 0) {

					l11 = l12 = 0;

					if (random_schick(100) > 50) {
						l11 = 1;
					}

					if (host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID) < 10) {
						l11 = 2;
					}

					FIG_call_draw_pic();

					if (l13 != -1) {

						seg044_002f(0, Real2Host(monster), 4, monster_pos + 10, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), 0);
					}

					if (l13 > 0) {

						if (l14 > 0) {

							seg045_041b(6, Real2Host(monster), l14);

						} else {

							if (host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID) > 0) {

								if (!attack_hero) {

									seg044_002f(1, mon, 99, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
								} else {

									if (check_hero(hero) || (ds_readws(DEFENDER_DEAD) != 0)) {

										seg044_002a(1, hero, 99, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), 0, -1, 1);
									}
								}
							}
						}

						if ((host_readbs(Real2Host(monster) + ENEMY_SHEET_GFX_ID) != 0x12) &&
							(host_readbs(Real2Host(monster) + ENEMY_SHEET_GFX_ID) != 7) &&
							(host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID) > 0))
						{
							l12 = seg045_01a0(7, l11, monster_pos + 10, host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID), host_readbs(Real2Host(monster) + ENEMY_SHEET_VIEWDIR));
						}

					}
					if (l13 != -1) {

						FIG_set_0e(host_readbs(Real2Host(monster) + ENEMY_SHEET_FIGHTER_ID), 0);

						draw_fight_screen_pal(1);
					}

					if (l13 > 0) {

						if (l12 != 0) {

							FIG_set_0e(ds_readbs(FIG_SHOT_BOLT_ID), 7);

							draw_fight_screen(1);

							FIG_reset_12_13(ds_readbs(FIG_SHOT_BOLT_ID));
						}

						if (l14 > 0) {
							FIG_set_0e(ds_readbs(FIG_SPELLGFX_ID), 6);
						}

						if (!attack_hero) {

							FIG_set_0e(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID), 1);


							if (is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {

								l16 = Real2Host(FIG_get_ptr(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID)));

								FIG_set_0e(ds_readbs(FIG_TWOFIELDED_TABLE + host_readbs(l16 + 0x13)), 3);
							}
						} else {

							if (host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID) > 0) {

								FIG_set_0e(host_readbs(hero + HERO_FIGHTER_ID), 1);
							}
						}

						ds_writew(FIG_CONTINUE_PRINT, 1);

						draw_fight_screen(1);

						if (l14 > 0) {
							FIG_reset_12_13(ds_readbs(FIG_SHOT_BOLT_ID));
						}

						if (ds_readw(MSPELL_AWAKE_FLAG) != 0) {

							ds_writew(MSPELL_AWAKE_FLAG, 0);

							FIG_remove_from_list(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID), 1);

							ds_writew((FIG_LIST_ELEM + 0x00), ds_readbs(GFXTAB_FIGURES_MAIN + 5 * host_readbs(mon + ENEMY_SHEET_GFX_ID)));
							ds_writeb((FIG_LIST_ELEM + 0x02), host_readbs(mon + ENEMY_SHEET_VIEWDIR));
							ds_writeb((FIG_LIST_ELEM + 0x05), ds_readbs(GFXTAB_OFFSETS_MAIN + 10 * host_readbs(mon + ENEMY_SHEET_GFX_ID) + 2 * host_readbs(mon + ENEMY_SHEET_VIEWDIR)));
							ds_writeb((FIG_LIST_ELEM + 0x06), ds_readbs((GFXTAB_OFFSETS_MAIN + 1) + 10 * host_readbs(mon + ENEMY_SHEET_GFX_ID) + 2 * host_readbs(mon + ENEMY_SHEET_VIEWDIR)));

							if (is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {
								ds_writeb((FIG_LIST_ELEM + 0x09), ds_readbs(GFXTAB_TWOFIELDED_X1 + host_readbs(mon + ENEMY_SHEET_VIEWDIR)));
								ds_writeb((FIG_LIST_ELEM + 0x0b), ds_readbs(GFXTAB_TWOFIELDED_X2 + host_readbs(mon + ENEMY_SHEET_VIEWDIR)));
							} else {
								ds_writeb((FIG_LIST_ELEM + 0x09), 0);
								ds_writeb((FIG_LIST_ELEM + 0x0b), 31);
								ds_writeb((FIG_LIST_ELEM + 0x13), -1);
							}

							ds_writeb((FIG_LIST_ELEM + 0x0a), 0);
							ds_writeb((FIG_LIST_ELEM + 0x0c), 39);
							ds_writeb((FIG_LIST_ELEM + 0x07), 40);
							ds_writeb((FIG_LIST_ELEM + 0x08), 32);
							ds_writeb((FIG_LIST_ELEM + 0x15), 1);
							ds_writeb((FIG_LIST_ELEM + 0x0d), -1);
							ds_writeb((FIG_LIST_ELEM + 0x0f), -1);
							ds_writeb((FIG_LIST_ELEM + 0x0e), -1);

							FIG_add_to_list(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID));
						}

						if (ds_readws(SPELL_ILLUSIONEN) != 0) {

							if (host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID) >= 10) {

								FIG_reset_12_13(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID));

								if (is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {

									l16 = Real2Host(FIG_get_ptr(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID)));

									FIG_reset_12_13(ds_readbs(FIG_TWOFIELDED_TABLE + host_readbs(l16 + 0x13)));
								}
							} else {

								if (host_readbs(Real2Host(monster) + ENEMY_SHEET_ENEMY_ID) > 0) {

									FIG_reset_12_13(host_readbs(hero + HERO_FIGHTER_ID));
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
 * \brief   *
 *
 * \param   hero        pointer to the hero who uses the item
 * \param   target_monster pointer to the monster
 * \param   target_hero pointer to the hero
 * \param   flag        bool value, used when a hero attacks a hero
 * \param   hero_pos    position of the hero
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
	} else if (!item_useable(p_item) || (host_readws(hero + (HERO_ITEM_LEFT+2)) == 0)) {
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

			strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx(0x94));

			FIG_damage_enemy(target_monster, damage, 0);

			FIG_add_msg(11, damage);

			if (enemy_dead(target_monster)) {
				ds_writew(DEFENDER_DEAD, 1);
			}
		} else {

			if (flag != 0) {

				strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx(0x94));

				sub_hero_le(target_hero, damage);

				FIG_add_msg(8, damage);

				if (hero_dead(target_hero)) {
					ds_writew(DEFENDER_DEAD, 1);
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
				ds_writew(DEFENDER_DEAD, 1);
			}
		} else {
			/* .. used on another hero */
			if (flag != 0) {

				sub_hero_le(target_hero, 20);

				FIG_add_msg(8, 20);

				if (hero_dead(target_hero)) {
					ds_writew(DEFENDER_DEAD, 1);
				}
			}
		}

		/* drop the item in the left hand */
		drop_item(hero, 4, 1);

		hylailic = 1;
	} else {
		/* use item in the regular way */

		use_item(4, hero_pos);

		host_writeb(Real2Host(ds_readd(DTP2)), 0);
	}

	if (usecase > 0) {

		seg041_8c8();

		FIG_prepare_hero_fight_ani(0, hero, -1, usecase == 1 ? 102 : 103, hero_pos + 1, host_readbs(hero + HERO_ENEMY_ID), 0);

		l3 = 0;

		if (hylailic != 0) {
			seg045_0394(6, hero, 2);
		} else {
			ds_writew(FIG_CONTINUE_PRINT, 1);
		}

		draw_fight_screen_pal(0);

		if (hylailic != 0) {

			FIG_set_0e(ds_readbs(FIG_SPELLGFX_ID), 6);

			l3 = 1;

			ds_writew(FIG_CONTINUE_PRINT, 1);

			draw_fight_screen(1);
		}

		if (l3 != 0) {
			FIG_remove_smth2();
		}

		if (ds_readws(DEFENDER_DEAD) != 0) {

			if (flag != 0) {
				FIG_prepare_hero_fight_ani(1, target_hero, -1, 0, host_readbs(hero + HERO_ENEMY_ID), hero_pos + 1, 1);
			} else {
				FIG_prepare_enemy_fight_ani(1, target_monster, 0, host_readbs(hero + HERO_ENEMY_ID), hero_pos + 1, 1);
			}

		}

		if ((l3 != 0) || (ds_readws(DEFENDER_DEAD) != 0)) {
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
