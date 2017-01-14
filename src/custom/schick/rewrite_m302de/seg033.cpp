/**
 *	Rewrite of DSA1 v3.02_de functions of seg033 (fightsystem: menu)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg033.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg006.h"
#include "seg007.h"
#include "seg032.h"
#include "seg034.h"
#include "seg035.h"
#include "seg036.h"
#include "seg038.h"
#include "seg039.h"
#include "seg041.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg105.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif
/**
 * \brief   combat menu
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position of the hero
 * \param   x           x-coordinate on the chessboard
 * \param   y           y-coordinate on the chessboard
 */
void FIG_menu(Bit8u *hero, signed short hero_pos, signed short x, signed short y)
{
	signed short selected;
	signed short l1;
	signed short txt_tabpos_bak;
	signed short done;
	signed short target_x;
	signed short target_y;
	signed short range_weapon;
	signed char target_id;
	signed short rwt1;
	signed short rwt2;
	signed char at;
	signed char pa;
	Bit8u *p_itemsdat;
	Bit8u *p_weapontab;
	Bit8u *spell;
	signed short damage_lo;
	signed short damage_hi;
	signed short weapon_id;
	signed short radio_i;
	signed short slot_nr;
	Bit8u *ptr;
	signed short textbox_width_bak;
	signed short slots[16];

	l1 = 1;
	done = 0;

	while ((done == 0) && (host_readbs(hero + HERO_BP_LEFT) > 0)) {

		if (ds_readbs(FIG_CB_MAKRER_ID) != -1) {
			FIG_remove_from_list(ds_readbs(FIG_CB_MAKRER_ID), 0);
			ds_writeb(FIG_CB_MAKRER_ID, -1);
		}

		FIG_init_list_elem(hero_pos + 1);
		draw_fight_screen_pal(0);

		if ((hero_unkn3(hero)) || (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_FLEE)) {

			and_ptr_bs(hero + HERO_STATUS1, 0x7f);
			and_ptr_bs(hero + HERO_STATUS1, 0xfb);

			if (seg038(hero, hero_pos, x, y, 5) != -1) {
				seg036_00ae(hero, hero_pos);
			}
			done = 1;

		} else if (hero_cursed(hero) || (host_readbs(hero + HERO_NPC_ID) > 0)|| (ds_readws(AUTOFIGHT) != 0)) {

			host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);

			if (((ds_readws(CURRENT_FIG_NR) != 192) || (ds_readbs(FINALFIGHT_TUMULT) != 0)) &&
				(host_readbs(hero + HERO_BP_LEFT) >= 3))
			{
				KI_hero(hero, hero_pos, x, y);
			}
			done = 1;
		} else {

			selected = -1;
			do {
				ds_writew(BASEPOS_X, (x + y < 15) ? 90 : -90);
				ds_writew(BASEPOS_Y, (x + y < 15) ? -30 : 30);

				/* prepare question with BP */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(0x48),
					host_readbs(hero + HERO_BP_LEFT));

				txt_tabpos_bak = ds_readws(TXT_TABPOS1);
				ds_writews(TXT_TABPOS1, ds_readws(BASEPOS_X) + 204);

				refresh_screen_size();

				weapon_id = host_readws(hero + HERO_ITEM_RIGHT);

				if (!item_weapon(get_itemsdat(weapon_id)) || (item_weapon(get_itemsdat(weapon_id)) && test_bit0(hero + (HERO_ITEM_RIGHT+4)))) {
					/* no weapon or weapon broken, use red color for "change weapon" */
					sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
						(char*)p_datseg + RED_STRING1,
						get_tx(0x60));
				} else {
					/* good weapon, no special color */
					strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_tx(0x60));
				}

				if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 110 <= host_readws(hero + HERO_LOAD)) {
					/* too much weight, use red color for "drop item" */
					sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 50,
						(char*)p_datseg + RED_STRING2,
						get_tx(0xb8));
				} else {
					/* weight ok, no special color */
					strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 50, (char*)get_tx(0xb8));
				}

				/* show radion menu */
				selected = GUI_radio(Real2Host(ds_readd(DTP2)), l1 != 0 ? 13 : 12,
							get_tx(0x4c),
							get_tx(0x50),
							get_tx(0x54),
							get_tx(0x58),
							get_tx(0x5c),
							Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 50,
							Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
							get_tx(0x40),
							get_tx(0x68),
							get_tx(0x64),
							get_tx(0x88),
							get_tx(0xcc),
							get_tx(0x6c));

				update_mouse_cursor();
				ds_writew(TXT_TABPOS1, txt_tabpos_bak);
				ds_writew(BASEPOS_X, 0);
				ds_writew(BASEPOS_Y, 0);

			} while (selected == -1);

			l1 = 0;

			if (selected == FIG_ACTION_MOVE) {
				/* MOVE / BEWEGEN */

				if (hero_unkn2(hero)) {
					/* MU + 2 */
					if (test_attrib(hero, ATTRIB_MU, 2) > 0) {

						/* unset this bit */
						and_ptr_bs(hero + HERO_STATUS1, 0x7f);

					} else if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_MU)) > 4) {
						slot_nr = get_free_mod_slot();
						set_mod_slot(slot_nr, HOURS(7), hero + (HERO_ATTRIB + 3 * ATTRIB_MU), -2, (signed char)hero_pos);
					}
				}

				if (!hero_unkn2(hero)) {

					host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);

					if (host_readbs(hero + HERO_BP_LEFT) > 0) {
						/* let the player select a move destination */

						refresh_screen_size();
#if !defined(__BORLANDC__)
						/* BE-fix */
						x = host_readws((Bit8u*)&x);
						y = host_readws((Bit8u*)&y);
#endif
						FIG_move_hero(hero, hero_pos, (Bit8u*)&x, (Bit8u*)&y);

#if !defined(__BORLANDC__)
						/* BE-fix */
						x = host_readws((Bit8u*)&x);
						y = host_readws((Bit8u*)&y);
#endif
						update_mouse_cursor();
						and_ptr_bs(hero + HERO_STATUS1, 0xef);

					} else {
						/* no BP left */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_tx(0x44),
							(char*)hero + HERO_NAME2);
						GUI_output(Real2Host(ds_readd(DTP2)));
					}

				} else {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx(0x1c),
						(char*)hero + HERO_NAME2);
					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			} else if (selected == FIG_ACTION_ATTACK) {
				/* ATTACK / ANGRIFF */

				if (host_readbs(hero + HERO_BP_LEFT) >= 3) {

					target_x = x;
					target_y = y;
					target_id = -5;
					if ((range_weapon = FIG_get_range_weapon_type(hero)) == -1) {
						/* not a range weapon */
						refresh_screen_size();
#if !defined(__BORLANDC__)
						/* BE-fix */
						target_x = host_readws((Bit8u*)&target_x);
						target_y = host_readws((Bit8u*)&target_y);
#endif
						target_id = FIG_cb_select_target((Bit8u*)&target_x, (Bit8u*)&target_y, 1);
#if !defined(__BORLANDC__)
						/* BE-fix */
						target_x = host_readws((Bit8u*)&target_x);
						target_y = host_readws((Bit8u*)&target_y);
#endif
						update_mouse_cursor();
					} else {
						if (range_attack_check_ammo(hero, 1)) {
							/* a range weapon */
							refresh_screen_size();
#if !defined(__BORLANDC__)
							/* BE-fix */
							target_x = host_readws((Bit8u*)&target_x);
							target_y = host_readws((Bit8u*)&target_y);
#endif
							target_id = FIG_cb_select_target((Bit8u*)&target_x, (Bit8u*)&target_y, 99);
#if !defined(__BORLANDC__)
							/* BE-fix */
							target_x = host_readws((Bit8u*)&target_x);
							target_y = host_readws((Bit8u*)&target_y);
#endif
							update_mouse_cursor();
						}
					}

					if (target_id != -5) {

						if ((target_id <= 0) || (target_id >= 50)) {
							GUI_output(get_tx(0x70));
						} else if (target_id == (hero_pos + 1)) {
							GUI_output(get_tx(0x0c));
						} else if (((target_id < 10) && hero_dead(get_hero(target_id - 1))) ||
								((target_id >= 10) && (target_id < 30) &&
										/* unconscious or dead */
										(test_bit0(p_datseg + ((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_STATUS1) + SIZEOF_ENEMY_SHEET * target_id) ||
										test_bit6(p_datseg + ((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_STATUS1) + SIZEOF_ENEMY_SHEET * target_id))) ||
								((target_id >= 30) &&
										/* unconscious or dead */
										(test_bit0(p_datseg + ((ENEMY_SHEETS - 30*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_STATUS1) + SIZEOF_ENEMY_SHEET * target_id) ||
										test_bit6(p_datseg + ((ENEMY_SHEETS - 30*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_STATUS1) + SIZEOF_ENEMY_SHEET * target_id))))
						{
							GUI_output(get_tx(0x74));

							host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);

							/* set target id to 0 */
							host_writeb(hero + HERO_ENEMY_ID, 0);

						} else if ((range_weapon != -1) && (calc_beeline(x, y, target_x, target_y) < 2)) {

							GUI_output(get_ttx(0x7f0));

							host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);

							/* set target id to 0 */
							host_writeb(hero + HERO_ENEMY_ID, 0);

						} else {
							host_writeb(hero + HERO_ATTACK_TYPE, 0);

							if (range_weapon == -1) {

								selected = -1;

								do {
									refresh_screen_size();

									selected = GUI_radio(get_ttx(0x920), 3,
											get_ttx(0x924),
											get_ttx(0x928),
											get_ttx(0x92c));
									update_mouse_cursor();
								} while (selected == -1);

								host_writeb(hero + HERO_ATTACK_TYPE, selected == 1 ? 2 : (selected == 2) ? 0 : -2);
							}

							/* set target id */
							host_writeb(hero + HERO_ENEMY_ID, target_id);
							/* set BP to 0 */
							host_writeb(hero + HERO_BP_LEFT, 0);
							host_writeb(hero + HERO_ACTION_ID, (range_weapon > 0 ? FIG_ACTION_RANGE_ATTACK : FIG_ACTION_ATTACK));
							done = 1;
						}
					}
				} else {
					/* no BP left */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx(0x44),
						(char*)hero + HERO_NAME2);
					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			} else if (selected == FIG_ACTION_GUARD) {
				/* GUARD / SICHERN */

				if (host_readbs(hero + HERO_BP_LEFT) >= 3) {

					/* set BP to 0 */
					host_writeb(hero + HERO_BP_LEFT, 0);

					host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_GUARD);

					done = 1;
				} else {
					/* no BP left */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx(0x44),
						(char*)hero + HERO_NAME2);
					GUI_output(Real2Host(ds_readd(DTP2)));
				}

			} else if (selected == FIG_ACTION_SPELL) {
				/* CAST SPELL / ZAUBERN */

				if (host_readbs(hero + HERO_TYPE) < 7) {
					/* not a magic user */
					GUI_output(get_ttx(0x35c));
				} else {

					if (host_readbs(hero + HERO_BP_LEFT) >= 5) {

						refresh_screen_size();
						host_writeb(hero + HERO_SPELL_ID, select_spell(hero, 0));
						update_mouse_cursor();

						if (host_readbs(hero + HERO_SPELL_ID) > 0) {

							host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);
							host_writeb(hero + HERO_ENEMY_ID, 0);

							spell = p_datseg + SPELL_DESCRIPTIONS + 10 * host_readbs(hero + HERO_SPELL_ID);

							if (host_readbs(spell + 5) == -1) {

								/* not a combat spell */
								GUI_output(get_ttx(0x940));

							} else {

								if (host_readbs(spell + 7) != 0) {

									target_x = x;
									target_y = y;
									weapon_id = 1;
									if (host_readbs(spell + 8) > 0) {
										weapon_id = 99;
									}
									refresh_screen_size();
#if !defined(__BORLANDC__)
									/* BE-fix */
									target_x = host_readws((Bit8u*)&target_x);
									target_y = host_readws((Bit8u*)&target_y);
#endif
									target_id = FIG_cb_select_target((Bit8u*)&target_x, (Bit8u*)&target_y, weapon_id);
#if !defined(__BORLANDC__)
									/* BE-fix */
									target_x = host_readws((Bit8u*)&target_x);
									target_y = host_readws((Bit8u*)&target_y);
#endif
									update_mouse_cursor();

									if (target_id != -99) {

										if (target_id < 0) {
											GUI_output(get_tx(0x70));

										} else if ((target_id == 0) &&
											(host_readbs(spell + 7) != 4))
										{
											GUI_output(get_tx(0x10));

										} else if ((target_id < 10) &&
											(host_readbs(spell + 7) != 2) &&
											(host_readbs(spell + 7) != 3))
										{
											GUI_output(get_tx(0x14));
										} else if ((target_id >= 10) &&
											(target_id < 50) &&
											(host_readbs(spell + 7) != 1) &&
											(host_readbs(spell + 7) != 3))
										{
											GUI_output(get_tx(0x18));

										} else {
											host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_SPELL);
											host_writeb(hero + HERO_ENEMY_ID, target_id);
											done = 1;
										}
									}
								} else {
									host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_SPELL);
									host_writeb(hero + HERO_ENEMY_ID, 0);
									done = 1;
								}
							}

						}

						if ((host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_SPELL) &&
							(get_spell_cost(host_readbs(hero + HERO_SPELL_ID), 1) > host_readws(hero + HERO_AE)))
						{
							/* not enough AE */
							GUI_output(get_ttx(0x544));
							host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);
							done = 0;
						}
					} else {
						/* no BP left */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_tx(0x44),
							(char*)hero + HERO_NAME2);
						GUI_output(Real2Host(ds_readd(DTP2)));
					}
				}
			} else if (selected == FIG_ACTION_USE_ITEM) {
				/* USE ITEM / GGST. BENUTZEN */

					if (host_readbs(hero + HERO_BP_LEFT) >= 3) {

						if (is_in_word_array(host_readws(hero + HERO_ITEM_LEFT), (signed short*)(p_datseg + ATTACK_ITEMS)))
						{
							target_x = x;
							target_y = y;
							refresh_screen_size();
#if !defined(__BORLANDC__)
							/* BE-fix */
							target_x = host_readws((Bit8u*)&target_x);
							target_y = host_readws((Bit8u*)&target_y);
#endif
							target_id = FIG_cb_select_target((Bit8u*)&target_x, (Bit8u*)&target_y, 99);
#if !defined(__BORLANDC__)
							/* BE-fix */
							target_x = host_readws((Bit8u*)&target_x);
							target_y = host_readws((Bit8u*)&target_y);
#endif
							update_mouse_cursor();
							host_writeb(hero + HERO_ENEMY_ID, target_id);
						} else {
							host_writeb(hero + HERO_ENEMY_ID, 0);
						}

						if ((host_readbs(hero + HERO_ENEMY_ID) < 0) || host_readbs(hero + HERO_ENEMY_ID) >= 50) {
							host_writeb(hero + HERO_ENEMY_ID, 0);
							GUI_output(get_tx(0x70));
						} else {
							/* set BP to 0 */
							host_writeb(hero + HERO_BP_LEFT, 0);

							host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_USE_ITEM);
							done = 1;
						}
					} else {
						/* no BP left */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_tx(0x44),
							(char*)hero + HERO_NAME2);
						GUI_output(Real2Host(ds_readd(DTP2)));
					}
			} else if (selected == FIG_ACTION_EXCHANGE_ITEM) {
				/* EXCHANGE ITEM / GGST. WECHSELN */
					if (host_readbs(hero + HERO_BP_LEFT) >= 2) {

						radio_i = 0;

						for (slot_nr = 7; slot_nr < 23; slot_nr++) {

							weapon_id = host_readws(hero + HERO_ITEM_HEAD + 14 * slot_nr);

							if (weapon_id != 0) {

								slots[radio_i] = slot_nr;

								ds_writed(RADIO_NAME_LIST + 4 * radio_i,
									(Bit32u)((RealPt)(ds_readd(DTP2)) + 30 * radio_i));

								strcpy((char*)Real2Host(ds_readd(RADIO_NAME_LIST + 4 * radio_i)),
									(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(weapon_id))));

								radio_i++;
							}
						}

						if (radio_i == 0) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ttx(0xbb8),
								(char*)hero + HERO_NAME2);
							GUI_output(Real2Host(ds_readd(DTP2)));
						} else {
							if (host_readws(hero + HERO_ITEM_LEFT) == 0) {
								sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
									(char*)get_tx(0xf0),
									(char*)hero + HERO_NAME2);
							} else {
								sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
									(char*)get_tx(0x7c),
									(char*)hero + HERO_NAME2,
									(char*)Real2Host(GUI_names_grammar((signed short)0x8002, host_readws(hero + HERO_ITEM_LEFT), 0)));
							}

							refresh_screen_size();
							textbox_width_bak = ds_readws(TEXTBOX_WIDTH);
							ds_writews(TEXTBOX_WIDTH, 6);

							selected = GUI_radio(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), radio_i,
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x00))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x04))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x08))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x0c))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x10))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x14))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x18))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x1c))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x20))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x24))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x28))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x2c))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x30))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x34))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x38))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x3c))));

							ds_writews(TEXTBOX_WIDTH, textbox_width_bak);

							update_mouse_cursor();

							if (selected != -1) {

								/* subtract 2 BP */
								sub_ptr_bs(hero + HERO_BP_LEFT, 2);
								move_item(4, slots[selected -1], hero);
							}
						}
					} else {
						/* no BP left */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_tx(0x44),
							(char*)hero + HERO_NAME2);
						GUI_output(Real2Host(ds_readd(DTP2)));
					}
			} else if (selected == FIG_ACTION_EXCHANGE_WEAPON) {
				/* CHANGE WEAPON / WAFFE WECHSELN */
					if (host_readbs(hero + HERO_BP_LEFT) >= 2) {

						radio_i = 0;

						for (slot_nr = 7; slot_nr < 23; slot_nr++) {

							weapon_id = host_readws(hero + HERO_ITEM_HEAD + 14 * slot_nr);

							if (item_weapon(get_itemsdat(weapon_id))) {

								slots[radio_i] = slot_nr;

								ds_writed(RADIO_NAME_LIST + 4 * radio_i,
									(Bit32u)((RealPt)(ds_readd(DTP2)) + 40 * radio_i));

								sprintf((char*)Real2Host(ds_readd(RADIO_NAME_LIST + 4 * radio_i)),
									(char*)p_datseg + SPACE_SEPARATED_STRINGS, /* "%s %s" */
									(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(weapon_id))),
									ks_broken(hero + HERO_ITEM_HEAD + 14 * slot_nr) ? get_ttx(0x778) : p_datseg + EMPTY_STRING3);

								radio_i++;
							}
						}

						if (radio_i == 0) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_tx(0x4),
								(char*)hero + HERO_NAME2);
							GUI_output(Real2Host(ds_readd(DTP2)));
						} else {
							sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
								(char*)get_tx(0x8),
								(char*)hero + HERO_NAME2,
								(char*)Real2Host(GUI_names_grammar((signed short)0x8002, host_readws(hero + HERO_ITEM_RIGHT), 0)));

							refresh_screen_size();
							textbox_width_bak = ds_readws(TEXTBOX_WIDTH);
							ds_writews(TEXTBOX_WIDTH, 6);

							selected = GUI_radio(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), radio_i,
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x00))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x04))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x08))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x0c))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x10))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x14))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x18))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x1c))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x20))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x24))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x28))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x2c))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x30))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x34))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x38))),
									Real2Host(ds_readd((RADIO_NAME_LIST + 0x3c))));

							ds_writews(TEXTBOX_WIDTH, textbox_width_bak);

							update_mouse_cursor();

							if (selected != -1) {

								/* subtract 2 BP */
								sub_ptr_bs(hero + HERO_BP_LEFT, 2);

								rwt1 = FIG_get_range_weapon_type(hero);

								move_item(3, slots[selected -1], hero);

								rwt2 = FIG_get_range_weapon_type(hero);

								if (rwt1 != rwt2) {

									ptr = Real2Host(FIG_get_ptr(host_readbs(hero + HERO_FIGHTER_ID)));

									if (rwt2 != -1) {
										host_writeb(ptr + 2,
										ds_readbs((NVFTAB_FIGURES_RANGEWEAPON - 12) + 12 * host_readbs(hero + HERO_SPRITE_NO) + 4 * rwt2 + host_readbs(hero + HERO_VIEWDIR)));
									} else {
										host_writeb(ptr + 2, host_readbs(hero + HERO_VIEWDIR));
									}

									host_writeb(ptr + 0xd, -1);

									draw_fight_screen_pal(0);
								}
							}
						}
					} else {
						/* no BP left */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_tx(0x44),
							(char*)hero + HERO_NAME2);
						GUI_output(Real2Host(ds_readd(DTP2)));
					}
			} else if (selected == FIG_ACTION_CHECK_VALUES) {
				/* CHECK VALUES / WERTE PRUEFEN */

				rwt1 = weapon_check(hero);

				if (rwt1 == -1) {

					rwt1 = FIG_get_range_weapon_type(hero);

					if (rwt1 == -1) {

						/* calculate AT and PA values for "BARE HANDS" */
						at = host_readbs(hero + HERO_AT) - host_readbs(hero + HERO_RS_BE) / 2;
						if (host_readbs(hero + HERO_RS_BE) & 1) {
							at--;
						}

						pa = host_readbs(hero + HERO_PA) - host_readbs(hero + HERO_RS_BE) / 2;

						/* set damage bounds: 1W6 */
						damage_lo = 1;
						damage_hi = 6;

					} else {

						/* calculate AT and PA values for range weapons */
						at = host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WP_CLASS))
							- host_readbs(hero + HERO_RS_BE) / 2
							+ host_readbs(hero + HERO_AT_MOD);

						if (host_readbs(hero + HERO_RS_BE) & 1) {
							at--;
						}
						pa = host_readbs(hero + HERO_PA + host_readbs(hero + HERO_WP_CLASS))
							- host_readbs(hero + HERO_RS_BE) / 2
							+ host_readbs(hero + HERO_PA_MOD);


						p_itemsdat = get_itemsdat(host_readws(hero + HERO_ITEM_RIGHT));
						p_weapontab = p_datseg + WEAPONS_TABLE + 7 * host_readbs(p_itemsdat + 4);

						calc_damage_range(host_readbs(p_weapontab), 6, host_readbs(p_weapontab + 1),
							(Bit8u*)&damage_lo, (Bit8u*)&damage_hi);

					}
				} else {
					/* calculate AT and PA values for melee weapons */
					at = host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WP_CLASS))
						- host_readbs(hero + HERO_RS_BE) / 2
						+ host_readbs(hero + HERO_AT_MOD);

					if (host_readbs(hero + HERO_RS_BE) & 1) {
						at--;
					}
					pa = host_readbs(hero + HERO_PA + host_readbs(hero + HERO_WP_CLASS))
						- host_readbs(hero + HERO_RS_BE) / 2
						+ host_readbs(hero + HERO_PA_MOD);


					p_itemsdat = get_itemsdat(host_readws(hero + HERO_ITEM_RIGHT));
					p_weapontab = p_datseg + WEAPONS_TABLE + 7 * host_readbs(p_itemsdat + 4);

					calc_damage_range(host_readbs(p_weapontab), 6, host_readbs(p_weapontab + 1),
						(Bit8u*)&damage_lo, (Bit8u*)&damage_hi);

					/* "THE SWORD GRIMRING" gets a damage bonus + 5 in the final fight */
					if ((host_readws(hero + HERO_ITEM_RIGHT) == 181) && (ds_readws(CURRENT_FIG_NR) == 192)) {
						damage_lo += 5;
						damage_hi += 5;
					}

					weapon_id = host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK))
							+ host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK))
							- host_readbs(p_weapontab + 2);

					if (weapon_id > 0) {
						damage_lo += weapon_id;
						damage_hi += weapon_id;
					}
				}

				if (at < 0) {
					at = 0;
				}

				if (pa < 0) {
					pa = 0;
				}

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx(0x0),
					/* name */
					(char*)hero + HERO_NAME2,
					/* typus */
					get_ttx(4 * ((host_readbs(hero + HERO_SEX) != 0 ? 0x251 : 9) + host_readbs(hero + HERO_TYPE))),
					/* level */
					host_readbs(hero + HERO_LEVEL),
					/* AT and PA value */
					at, pa,
					/* RS */
					host_readbs(hero + HERO_RS_BONUS1),
					/* weapon name */
					Real2Host(GUI_name_singular((Bit8u*)get_itemname(host_readws(hero + HERO_ITEM_RIGHT)))),
					/* damage bounds */
					damage_lo, damage_hi,
					/* LE */
					host_readws(hero + HERO_LE), host_readws(hero + HERO_LE_ORIG),
					/* AE */
					host_readws(hero + HERO_AE), host_readws(hero + HERO_AE_ORIG),
					/* poison */
					hero_is_poisoned(hero) ? get_tx(0x90) : p_datseg + EMPTY_STRING4,
					/* cursed */
					hero_cursed(hero) == 1 ? get_tx(0x98) : p_datseg + EMPTY_STRING5);

				GUI_output(Real2Host(ds_readd(DTP2)));

			} else if (selected == FIG_ACTION_WAIT) {
				/* WAIT / ABWARTEN */

				done = 1;
				host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);
				/* set BP to 0 */
				host_writeb(hero + HERO_BP_LEFT, 0);
				/* set target id to 0 */
				host_writeb(hero + HERO_ENEMY_ID, 0);

			} else if (selected == FIG_ACTION_COMPUTER_FIGHT) {
				/* COMPUTER FIGHT / COMPUTERKAMPF */

				if (ds_readws(CURRENT_FIG_NR) != 192) {

					refresh_screen_size();

					/* use magic in autofight */
					ds_writeb(AUTOFIGHT_MAGIC, GUI_bool(get_tx(0xd4)));

					update_mouse_cursor();

					ds_writew(AUTOFIGHT, 1);
				}

			} else if (selected == FIG_ACTION_DROP_ITEM) {
				/* DROP ITEM / GGST. WEGWERFEN */
				if (host_readbs(hero + HERO_BP_LEFT) >= 1) {

					radio_i = 0;

					for (slot_nr = 7; slot_nr < 23; slot_nr++) {

						weapon_id = host_readws(hero + HERO_ITEM_HEAD + 14 * slot_nr);

						if (weapon_id != 0) {

							slots[radio_i] = slot_nr;

							ds_writed(RADIO_NAME_LIST + 4 * radio_i,
								(Bit32u)((RealPt)(ds_readd(DTP2)) + 30 * radio_i));

							strcpy((char*)Real2Host(ds_readd(RADIO_NAME_LIST + 4 * radio_i)),
								(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(weapon_id))));

							radio_i++;
						}
					}

					if (radio_i == 0) {
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(0xbb8),
							(char*)hero + HERO_NAME2);
						GUI_output(Real2Host(ds_readd(DTP2)));
					} else {
						sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
							(char*)get_tx(0xbc),
							(char*)hero + HERO_NAME2);

						refresh_screen_size();
						textbox_width_bak = ds_readws(TEXTBOX_WIDTH);
						ds_writews(TEXTBOX_WIDTH, 6);

						selected = GUI_radio(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), radio_i,
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x00))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x04))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x08))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x0c))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x10))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x14))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x18))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x1c))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x20))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x24))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x28))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x2c))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x30))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x34))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x38))),
								Real2Host(ds_readd((RADIO_NAME_LIST + 0x3c))));

						ds_writews(TEXTBOX_WIDTH, textbox_width_bak);

						update_mouse_cursor();

						if (selected != -1) {

							drop_item(hero, slots[selected -1], -1);

							/* subtract 1 BP */
							dec_ptr_bs(hero + HERO_BP_LEFT);
						}
					}
				} else {
					/* no BP left */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx(0x44),
						(char*)hero + HERO_NAME2);
					GUI_output(Real2Host(ds_readd(DTP2)));
				}

			} else if (selected == FIG_ACTION_QUIT_AND_LOAD) {
				/* QUIT AND RELOAD / VERLASSEN / LADEN */

				refresh_screen_size();

				if (GUI_bool(get_tx(0xd0))) {
					done = 1;
					ds_writew(IN_FIGHT, 0);
					ds_writew(GAME_STATE, GAME_STATE_FIGQUIT);
				}

				update_mouse_cursor();

			} else if (selected == FIG_ACTION_REPEAT_OPTION) {
				/* REPEAT OPTIONS / ALTE OPTION */

				done = 1;

				/* check last action and target_id */
				if (((host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_SPELL) ||
					(host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_ATTACK) ||
					(host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_RANGE_ATTACK)) && (host_readbs(hero + HERO_ENEMY_ID) > 0))
				{

					/* TODO: check fighter_id upper bound */
					if (((host_readbs(hero + HERO_ENEMY_ID) >= 10)
						&& (test_bit0(p_datseg + ((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + 49) + SIZEOF_ENEMY_SHEET * host_readbs(hero + HERO_ENEMY_ID)))) ||
						((host_readbs(hero + HERO_ENEMY_ID) < 10)
						&& (hero_dead(get_hero(host_readbs(hero + HERO_ENEMY_ID) - 1)))))
					{

						GUI_output(get_tx(0x74));
						host_writebs(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);
						host_writebs(hero + HERO_ENEMY_ID, 0);
						done = 0;

					/* TODO: check fighter_id upper bound */
					} else if (((host_readbs(hero + HERO_ENEMY_ID) >= 10)
						&& (test_bit2(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + 0x32 + SIZEOF_ENEMY_SHEET * host_readbs(hero + HERO_ENEMY_ID)))) ||
						((host_readbs(hero + HERO_ENEMY_ID) < 10)
						&& (hero_unkn3(get_hero(host_readbs(hero + HERO_ENEMY_ID) - 1)))))
					{

						/* GUI_output(get_tx(0x74)); */
						host_writebs(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);
						host_writebs(hero + HERO_ENEMY_ID, 0);
						done = 0;
					} else if (((host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_SPELL) ||
							(host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_RANGE_ATTACK)) &&
							!check_hero_range_attack(hero, hero_pos))
					{
						/* GUI_output(get_tx(0x74)); */
						host_writebs(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);
						host_writebs(hero + HERO_ENEMY_ID, 0);
						done = 0;
					}
				}
			}
		}
	}

	if ((ds_readws(CURRENT_FIG_NR) == 192) &&
		(get_hero_index(Real2Host(ds_readd(MAIN_ACTING_HERO))) != hero_pos) &&
		((host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_ATTACK) || (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_RANGE_ATTACK) ||
		(host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_SPELL) || (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_USE_ITEM)))
	{
		for (slot_nr = 0; slot_nr < 20; slot_nr++) {
			and_ds_bs((ENEMY_SHEETS + ENEMY_SHEET_STATUS1) + SIZEOF_ENEMY_SHEET * slot_nr, (signed char)0xdf);
		}

		ds_writeb(FINALFIGHT_TUMULT, 1);
	}
}

#if !defined(__BORLANDC__)
}
#endif
