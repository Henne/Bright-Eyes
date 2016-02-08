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
 * \brief	combat menu
 *
 * \param hero		pointer to the hero
 * \param hero_pos	position of the hero
 * \param x		x-coordinate on the chessboard
 * \param y		y-coordinate on the chessboard
 */
void FIG_menu(Bit8u *hero, signed short hero_pos, signed short x, signed short y)
{
	signed short l_di;
	signed short l1;
	signed short l3;
	signed short done;
	signed short l5;
	signed short l6;
	signed short l7;
	signed char l8;
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
	signed short l100;
	signed short slot_nr;
	Bit8u *ptr;
	signed short bak;
	signed short slots[16];

	l1 = 1;
	done = 0;

	while ((done == 0) && (host_readbs(hero + 0x33) > 0)) {

		if (ds_readbs(0xe38e) != -1) {
			FIG_remove_from_list(ds_readbs(0xe38e), 0);
			ds_writeb(0xe38e, -1);
		}

		FIG_init_list_elem(hero_pos + 1);
		draw_fight_screen_pal(0);

		if ((hero_unkn3(hero)) || (host_readbs(hero + 0x84) == 16)) {

			and_ptr_bs(hero + 0xaa, 0x7f);
			and_ptr_bs(hero + 0xaa, 0xfb);

			if (seg038(hero, hero_pos, x, y, 5) != -1) {
				seg036_00ae(hero, hero_pos);
			}
			done = 1;

		} else if (hero_cursed(hero) || (host_readbs(hero + 0x89) > 0)|| (ds_readws(AUTOFIGHT) != 0)) {

			host_writeb(hero + 0x84, 10);

			if (((ds_readws(CURRENT_FIG_NR) != 192) || (ds_readbs(0x5f30) != 0)) &&
				(host_readbs(hero + 0x33) >= 3))
			{
				KI_hero(hero, hero_pos, x, y);
			}
			done = 1;
		} else {

			l_di = -1;
			do {
				ds_writew(0x2ca2, (x + y < 15) ? 90 : -90);
				ds_writew(0x2ca4, (x + y < 15) ? -30 : 30);

				/* prepare question with BP */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x48),
					host_readbs(hero + 0x33));

				l3 = ds_readws(0xd313);
				ds_writews(0xd313, ds_readws(0x2ca2) + 204);

				refresh_screen_size();

				weapon_id = host_readws(hero + 0x1c0);

				if (!item_weapon(get_itemsdat(weapon_id)) || (item_weapon(get_itemsdat(weapon_id)) && test_bit0(hero + 0x1c4))) {
					/* no weapon or weapon broken, use red color for "change weapon" */
					sprintf((char*)Real2Host(ds_readd(0xd2eb)),
						(char*)p_datseg + 0x5f32,
						get_dtp(0x60));
				} else {
					/* good weapon, no special color */
					strcpy((char*)Real2Host(ds_readd(0xd2eb)), (char*)get_dtp(0x60));
				}

				if (host_readbs(hero + 0x47) * 110 <= host_readws(hero + 0x2d8)) {
					/* too much weight, use red color for "drop item" */
					sprintf((char*)Real2Host(ds_readd(0xd2eb)) + 50,
						(char*)p_datseg + 0x5f37,
						get_dtp(0xb8));
				} else {
					/* weight ok, no special color */
					strcpy((char*)Real2Host(ds_readd(0xd2eb)) + 50, (char*)get_dtp(0xb8));
				}

				/* show radion menu */
				l_di = GUI_radio(Real2Host(ds_readd(DTP2)), l1 != 0 ? 13 : 12,
							get_dtp(0x4c),
							get_dtp(0x50),
							get_dtp(0x54),
							get_dtp(0x58),
							get_dtp(0x5c),
							Real2Host(ds_readd(0xd2eb)) + 50,
							Real2Host(ds_readd(0xd2eb)),
							get_dtp(0x40),
							get_dtp(0x68),
							get_dtp(0x64),
							get_dtp(0x88),
							get_dtp(0xcc),
							get_dtp(0x6c));

				update_mouse_cursor();
				ds_writew(0xd313, l3);
				ds_writew(0x2ca2, 0);
				ds_writew(0x2ca4, 0);

			} while (l_di == -1);

			l1 = 0;

			if (l_di == 1) {
				/* MOVE / BEWEGEN */

				if (hero_unkn2(hero)) {
					/* MU + 2 */
					if (test_attrib(hero, 0, 2) > 0) {

						/* unset this bit */
						and_ptr_bs(hero + 0xaa, 0x7f);

					} else if (host_readbs(hero + 0x35) > 4) {
						slot_nr = get_free_mod_slot();
						set_mod_slot(slot_nr, HOURS(7), hero + 0x35, -2, (signed char)hero_pos);
					}
				}

				if (!hero_unkn2(hero)) {

					host_writeb(hero + 0x84, 1);

					if (host_readbs(hero + 0x33) > 0) {
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
						and_ptr_bs(hero + 0xaa, 0xef);

					} else {
						/* no BP left */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_dtp(0x44),
							(char*)hero + 0x10);
						GUI_output(Real2Host(ds_readd(DTP2)));
					}

				} else {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x1c),
						(char*)hero + 0x10);
					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			} else if (l_di == 2) {
				/* ATTACK / ANGRIFF */

				if (host_readbs(hero + 0x33) >= 3) {

					l5 = x;
					l6 = y;
					l8 = -5;
					if ((l7 = FIG_get_range_weapon_type(hero)) == -1) {
						/* not a range weapon */
						refresh_screen_size();
#if !defined(__BORLANDC__)
						/* BE-fix */
						l5 = host_readws((Bit8u*)&l5);
						l6 = host_readws((Bit8u*)&l6);
#endif
						l8 = seg034_2e3((Bit8u*)&l5, (Bit8u*)&l6, 1);
#if !defined(__BORLANDC__)
						/* BE-fix */
						l5 = host_readws((Bit8u*)&l5);
						l6 = host_readws((Bit8u*)&l6);
#endif
						update_mouse_cursor();
					} else {
						if (seg041_0020(hero, 1)) {
							/* a range weapon */
							refresh_screen_size();
#if !defined(__BORLANDC__)
							/* BE-fix */
							l5 = host_readws((Bit8u*)&l5);
							l6 = host_readws((Bit8u*)&l6);
#endif
							l8 = seg034_2e3((Bit8u*)&l5, (Bit8u*)&l6, 99);
#if !defined(__BORLANDC__)
							/* BE-fix */
							l5 = host_readws((Bit8u*)&l5);
							l6 = host_readws((Bit8u*)&l6);
#endif
							update_mouse_cursor();
						}
					}

					if (l8 != -5) {

						if ((l8 <= 0) || (l8 >= 50)) {
							GUI_output(get_dtp(0x70));
						} else if (l8 == (hero_pos + 1)) {
							GUI_output(get_dtp(0x0c));
						} else if (((l8 < 10) && hero_dead(get_hero(l8 - 1))) ||
								((l8 >= 10) && (l8 < 30) &&
										(test_bit0(p_datseg + 0xd110 + 62 * l8) ||
										test_bit6(p_datseg + 0xd110 + 62 * l8))) ||
								((l8 >= 30) &&
										(test_bit0(p_datseg + 0xcc38 + 62 * l8) ||
										test_bit6(p_datseg + 0xcc38 + 62 * l8))))
						{
							GUI_output(get_dtp(0x74));

							host_writeb(hero + 0x84, 1);

							/* set target id to 0 */
							host_writeb(hero + 0x86, 0);

						} else if ((l7 != -1) && (calc_beeline(x, y, l5, l6) < 2)) {

							GUI_output(get_ltx(0x7f0));

							host_writeb(hero + 0x84, 1);

							/* set target id to 0 */
							host_writeb(hero + 0x86, 0);

						} else {
							host_writeb(hero + 0x79, 0);

							if (l7 == -1) {

								l_di = -1;

								do {
									refresh_screen_size();

									l_di = GUI_radio(get_ltx(0x920), 3,
											get_ltx(0x924),
											get_ltx(0x928),
											get_ltx(0x92c));
									update_mouse_cursor();
								} while (l_di == -1);

								host_writeb(hero + 0x79, l_di == 1 ? 2 : (l_di == 2) ? 0 : -2);
							}

							/* set target id */
							host_writeb(hero + 0x86, l8);
							/* set BP to 0 */
							host_writeb(hero + 0x33, 0);
							host_writeb(hero + 0x84, (l7 > 0 ? 15 : 2));
							done = 1;
						}
					}
				} else {
					/* no BP left */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x44),
						(char*)hero + 0x10);
					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			} else if (l_di == 3) {
				/* GUARD / SICHERN */

				if (host_readbs(hero + 0x33) >= 3) {

					/* set BP to 0 */
					host_writeb(hero + 0x33, 0);

					host_writeb(hero + 0x84, 3);

					done = 1;
				} else {
					/* no BP left */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x44),
						(char*)hero + 0x10);
					GUI_output(Real2Host(ds_readd(DTP2)));
				}

			} else if (l_di == 4) {
				/* CAST SPELL / ZAUBERN */

				if (host_readbs(hero + 0x21) < 7) {
					/* not a magic user */
					GUI_output(get_ltx(0x35c));
				} else {

					if (host_readbs(hero + 0x33) >= 5) {

						refresh_screen_size();
						host_writeb(hero + 0x85, select_spell(hero, 0));
						update_mouse_cursor();

						if (host_readbs(hero + 0x85) > 0) {

							host_writeb(hero + 0x84, 1);
							host_writeb(hero + 0x86, 0);

							spell = p_datseg + 0x099d + 10 * host_readbs(hero + 0x85);

							if (host_readbs(spell + 5) == -1) {

								/* not a combat spell */
								GUI_output(get_ltx(0x940));

							} else {

								if (host_readbs(spell + 7) != 0) {

									l5 = x;
									l6 = y;
									weapon_id = 1;
									if (host_readbs(spell + 8) > 0) {
										weapon_id = 99;
									}
									refresh_screen_size();
#if !defined(__BORLANDC__)
									/* BE-fix */
									l5 = host_readws((Bit8u*)&l5);
									l6 = host_readws((Bit8u*)&l6);
#endif
									l8 = seg034_2e3((Bit8u*)&l5, (Bit8u*)&l6, weapon_id);
#if !defined(__BORLANDC__)
									/* BE-fix */
									l5 = host_readws((Bit8u*)&l5);
									l6 = host_readws((Bit8u*)&l6);
#endif
									update_mouse_cursor();

									if (l8 != -99) {

										if (l8 < 0) {
											GUI_output(get_dtp(0x70));

										} else if ((l8 == 0) &&
											(host_readbs(spell + 7) != 4))
										{
											GUI_output(get_dtp(0x10));

										} else if ((l8 < 10) &&
											(host_readbs(spell + 7) != 2) &&
											(host_readbs(spell + 7) != 3))
										{
											GUI_output(get_dtp(0x14));
										} else if ((l8 >= 10) &&
											(l8 < 50) &&
											(host_readbs(spell + 7) != 1) &&
											(host_readbs(spell + 7) != 3))
										{
											GUI_output(get_dtp(0x18));

										} else {
											host_writeb(hero + 0x84, 4);
											host_writeb(hero + 0x86, l8);
											done = 1;
										}
									}
								} else {
									host_writeb(hero + 0x84, 4);
									host_writeb(hero + 0x86, 0);
									done = 1;
								}
							}

						}

						if ((host_readbs(hero + 0x84) == 4) &&
							(get_spell_cost(host_readbs(hero + 0x85), 1) > host_readws(hero + 0x64)))
						{
							/* not enough AE */
							GUI_output(get_ltx(0x544));
							host_writeb(hero + 0x84, 1);
							done = 0;
						}
					} else {
						/* no BP left */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_dtp(0x44),
							(char*)hero + 0x10);
						GUI_output(Real2Host(ds_readd(DTP2)));
					}
				}
			} else if (l_di == 5) {
				/* USE ITEM / GGST. BENUTZEN */

					if (host_readbs(hero + 0x33) >= 3) {

						if (is_in_word_array(host_readws(hero + 0x1ce), (signed short*)(p_datseg + 0x091f)))
						{
							l5 = x;
							l6 = y;
							refresh_screen_size();
#if !defined(__BORLANDC__)
							/* BE-fix */
							l5 = host_readws((Bit8u*)&l5);
							l6 = host_readws((Bit8u*)&l6);
#endif
							l8 = seg034_2e3((Bit8u*)&l5, (Bit8u*)&l6, 99);
#if !defined(__BORLANDC__)
							/* BE-fix */
							l5 = host_readws((Bit8u*)&l5);
							l6 = host_readws((Bit8u*)&l6);
#endif
							update_mouse_cursor();
							host_writeb(hero + 0x86, l8);
						} else {
							host_writeb(hero + 0x86, 0);
						}

						if ((host_readbs(hero + 0x86) < 0) || host_readbs(hero + 0x86) >= 50) {
							host_writeb(hero + 0x86, 0);
							GUI_output(get_dtp(0x70));
						} else {
							/* set BP to 0 */
							host_writeb(hero + 0x33, 0);

							host_writeb(hero + 0x84, 5);
							done = 1;
						}
					} else {
						/* no BP left */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_dtp(0x44),
							(char*)hero + 0x10);
						GUI_output(Real2Host(ds_readd(DTP2)));
					}
			} else if (l_di == 8) {
				/* EXCHANGE ITEM / GGST. WECHSELN */
					if (host_readbs(hero + 0x33) >= 2) {

						l100 = 0;

						for (slot_nr = 7; slot_nr < 23; slot_nr++) {

							weapon_id = host_readws(hero + 0x196 + 14 * slot_nr);

							if (weapon_id != 0) {

								slots[l100] = slot_nr;

								ds_writed(0xbf95 + 4 * l100,
									(Bit32u)((RealPt)(ds_readd(DTP2)) + 30 * l100));

								strcpy((char*)Real2Host(ds_readd(0xbf95 + 4 * l100)),
									(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(weapon_id))));

								l100++;
							}
						}

						if (l100 == 0) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ltx(0xbb8),
								(char*)hero + 0x10);
							GUI_output(Real2Host(ds_readd(DTP2)));
						} else {
							if (host_readws(hero + 0x1ce) == 0) {
								sprintf((char*)Real2Host(ds_readd(0xd2eb)),
									(char*)get_dtp(0xf0),
									(char*)hero + 0x10);
							} else {
								sprintf((char*)Real2Host(ds_readd(0xd2eb)),
									(char*)get_dtp(0x7c),
									(char*)hero + 0x10,
									(char*)Real2Host(GUI_names_grammar((signed short)0x8002, host_readws(hero + 0x1ce), 0)));
							}

							refresh_screen_size();
							bak = ds_readws(TEXTBOX_WIDTH);
							ds_writews(TEXTBOX_WIDTH, 6);

							l_di = GUI_radio(Real2Host(ds_readd(0xd2eb)), l100,
									Real2Host(ds_readd((0xbf95 + 0x00))),
									Real2Host(ds_readd((0xbf95 + 0x04))),
									Real2Host(ds_readd((0xbf95 + 0x08))),
									Real2Host(ds_readd((0xbf95 + 0x0c))),
									Real2Host(ds_readd((0xbf95 + 0x10))),
									Real2Host(ds_readd((0xbf95 + 0x14))),
									Real2Host(ds_readd((0xbf95 + 0x18))),
									Real2Host(ds_readd((0xbf95 + 0x1c))),
									Real2Host(ds_readd((0xbf95 + 0x20))),
									Real2Host(ds_readd((0xbf95 + 0x24))),
									Real2Host(ds_readd((0xbf95 + 0x28))),
									Real2Host(ds_readd((0xbf95 + 0x2c))),
									Real2Host(ds_readd((0xbf95 + 0x30))),
									Real2Host(ds_readd((0xbf95 + 0x34))),
									Real2Host(ds_readd((0xbf95 + 0x38))),
									Real2Host(ds_readd((0xbf95 + 0x3c))));

							ds_writews(TEXTBOX_WIDTH, bak);

							update_mouse_cursor();

							if (l_di != -1) {

								/* subtract 2 BP */
								sub_ptr_bs(hero + 0x33, 2);
								move_item(4, slots[l_di -1], hero);
							}
						}
					} else {
						/* no BP left */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_dtp(0x44),
							(char*)hero + 0x10);
						GUI_output(Real2Host(ds_readd(DTP2)));
					}
			} else if (l_di == 7) {
				/* CHANGE WEAPON / WAFFE WECHSELN */
					if (host_readbs(hero + 0x33) >= 2) {

						l100 = 0;

						for (slot_nr = 7; slot_nr < 23; slot_nr++) {

							weapon_id = host_readws(hero + 0x196 + 14 * slot_nr);

							if (item_weapon(get_itemsdat(weapon_id))) {

								slots[l100] = slot_nr;

								ds_writed(0xbf95 + 4 * l100,
									(Bit32u)((RealPt)(ds_readd(DTP2)) + 40 * l100));

								sprintf((char*)Real2Host(ds_readd(0xbf95 + 4 * l100)),
									(char*)p_datseg + 0x5f3c, /* "%s %s" */
									(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(weapon_id))),
									ks_broken(hero + 0x196 + 14 * slot_nr) ? get_ltx(0x778) : p_datseg + 0x5f42);

								l100++;
							}
						}

						if (l100 == 0) {
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_dtp(0x4),
								(char*)hero + 0x10);
							GUI_output(Real2Host(ds_readd(DTP2)));
						} else {
							sprintf((char*)Real2Host(ds_readd(0xd2eb)),
								(char*)get_dtp(0x8),
								(char*)hero + 0x10,
								(char*)Real2Host(GUI_names_grammar((signed short)0x8002, host_readws(hero + 0x1c0), 0)));

							refresh_screen_size();
							bak = ds_readws(TEXTBOX_WIDTH);
							ds_writews(TEXTBOX_WIDTH, 6);

							l_di = GUI_radio(Real2Host(ds_readd(0xd2eb)), l100,
									Real2Host(ds_readd((0xbf95 + 0x00))),
									Real2Host(ds_readd((0xbf95 + 0x04))),
									Real2Host(ds_readd((0xbf95 + 0x08))),
									Real2Host(ds_readd((0xbf95 + 0x0c))),
									Real2Host(ds_readd((0xbf95 + 0x10))),
									Real2Host(ds_readd((0xbf95 + 0x14))),
									Real2Host(ds_readd((0xbf95 + 0x18))),
									Real2Host(ds_readd((0xbf95 + 0x1c))),
									Real2Host(ds_readd((0xbf95 + 0x20))),
									Real2Host(ds_readd((0xbf95 + 0x24))),
									Real2Host(ds_readd((0xbf95 + 0x28))),
									Real2Host(ds_readd((0xbf95 + 0x2c))),
									Real2Host(ds_readd((0xbf95 + 0x30))),
									Real2Host(ds_readd((0xbf95 + 0x34))),
									Real2Host(ds_readd((0xbf95 + 0x38))),
									Real2Host(ds_readd((0xbf95 + 0x3c))));

							ds_writews(TEXTBOX_WIDTH, bak);

							update_mouse_cursor();

							if (l_di != -1) {

								/* subtract 2 BP */
								sub_ptr_bs(hero + 0x33, 2);

								rwt1 = FIG_get_range_weapon_type(hero);

								move_item(3, slots[l_di -1], hero);

								rwt2 = FIG_get_range_weapon_type(hero);

								if (rwt1 != rwt2) {

									ptr = Real2Host(FIG_get_ptr(host_readbs(hero + 0x81)));

									if (rwt2 != -1) {
										host_writeb(ptr + 2,
										ds_readbs(0x10d0 + 12 * host_readbs(hero + 0x9b) + 4 * rwt2 + host_readbs(hero + 0x82)));
									} else {
										host_writeb(ptr + 2, host_readbs(hero + 0x82));
									}

									host_writeb(ptr + 0xd, -1);

									draw_fight_screen_pal(0);
								}
							}
						}
					} else {
						/* no BP left */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_dtp(0x44),
							(char*)hero + 0x10);
						GUI_output(Real2Host(ds_readd(DTP2)));
					}
			} else if (l_di == 9) {
				/* CHECK VALUES / WERTE PRUEFEN */

				rwt1 = weapon_check(hero);

				if (rwt1 == -1) {

					rwt1 = FIG_get_range_weapon_type(hero);

					if (rwt1 == -1) {

						/* calculate AT and PA values for "BARE HANDS" */
						at = host_readbs(hero + 0x68) - host_readbs(hero + 0x32) / 2;
						if (host_readbs(hero + 0x32) & 1) {
							at--;
						}

						pa = host_readbs(hero + 0x6f) - host_readbs(hero + 0x32) / 2;

						/* set damage bounds: 1W6 */
						damage_lo = 1;
						damage_hi = 6;

					} else {

						/* calculate AT and PA values for range weapons */
						at = host_readbs(hero + 0x68 + host_readbs(hero + 0x78))
							- host_readbs(hero + 0x32) / 2
							+ host_readbs(hero + 0x76);

						if (host_readbs(hero + 0x32) & 1) {
							at--;
						}
						pa = host_readbs(hero + 0x6f + host_readbs(hero + 0x78))
							- host_readbs(hero + 0x32) / 2
							+ host_readbs(hero + 0x77);


						p_itemsdat = get_itemsdat(host_readws(hero + 0x1c0));
						p_weapontab = p_datseg + 0x06b0 + 7 * host_readbs(p_itemsdat + 4);

						calc_damage_range(host_readbs(p_weapontab), 6, host_readbs(p_weapontab + 1),
							(Bit8u*)&damage_lo, (Bit8u*)&damage_hi);

					}
				} else {
					/* calculate AT and PA values for melee weapons */
					at = host_readbs(hero + 0x68 + host_readbs(hero + 0x78))
						- host_readbs(hero + 0x32) / 2
						+ host_readbs(hero + 0x76);

					if (host_readbs(hero + 0x32) & 1) {
						at--;
					}
					pa = host_readbs(hero + 0x6f + host_readbs(hero + 0x78))
						- host_readbs(hero + 0x32) / 2
						+ host_readbs(hero + 0x77);


					p_itemsdat = get_itemsdat(host_readws(hero + 0x1c0));
					p_weapontab = p_datseg + 0x06b0 + 7 * host_readbs(p_itemsdat + 4);

					calc_damage_range(host_readbs(p_weapontab), 6, host_readbs(p_weapontab + 1),
						(Bit8u*)&damage_lo, (Bit8u*)&damage_hi);

					/* "THE SWORD GRIMRING" gets a damage bonus + 5 in the final fight */
					if ((host_readws(hero + 0x1c0) == 181) && (ds_readws(CURRENT_FIG_NR) == 192)) {
						damage_lo += 5;
						damage_hi += 5;
					}

					weapon_id = host_readbs(hero + 0x47)
							+ host_readbs(hero + 0x48)
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
					(char*)get_dtp(0x0),
					/* name */
					(char*)hero + 0x10,
					/* typus */
					get_ltx(4 * ((host_readbs(hero + 0x22) != 0 ? 0x251 : 9) + host_readbs(hero + 0x21))),
					/* level */
					host_readbs(hero + 0x27),
					/* AT and PA value */
					at, pa,
					/* RS */
					host_readbs(hero + 0x30),
					/* weapon name */
					Real2Host(GUI_name_singular((Bit8u*)get_itemname(host_readws(hero + 0x1c0)))),
					/* damage bounds */
					damage_lo, damage_hi,
					/* LE */
					host_readws(hero + 0x60), host_readws(hero + 0x5e),
					/* AE */
					host_readws(hero + 0x64), host_readws(hero + 0x62),
					/* poison */
					hero_is_poisoned(hero) ? get_dtp(0x90) : p_datseg + 0x5f43,
					/* cursed */
					hero_cursed(hero) == 1 ? get_dtp(0x98) : p_datseg + 0x5f44);

				GUI_output(Real2Host(ds_readd(DTP2)));

			} else if (l_di == 10) {
				/* WAIT / ABWARTEN */

				done = 1;
				host_writeb(hero + 0x84, 10);
				/* set BP to 0 */
				host_writeb(hero + 0x33, 0);
				/* set target id to 0 */
				host_writeb(hero + 0x86, 0);

			} else if (l_di == 11) {
				/* COMPUTER FIGHT / COMPUTERKAMPF */

				if (ds_readws(CURRENT_FIG_NR) != 192) {

					refresh_screen_size();

					/* use magic in autofight */
					ds_writeb(AUTOFIGHT_MAGIC, GUI_bool(get_dtp(0xd4)));

					update_mouse_cursor();

					ds_writew(AUTOFIGHT, 1);
				}

			} else if (l_di == 6) {
				/* DROP ITEM / GGST. WEGWERFEN */
				if (host_readbs(hero + 0x33) >= 1) {

					l100 = 0;

					for (slot_nr = 7; slot_nr < 23; slot_nr++) {

						weapon_id = host_readws(hero + 0x196 + 14 * slot_nr);

						if (weapon_id != 0) {

							slots[l100] = slot_nr;

							ds_writed(0xbf95 + 4 * l100,
								(Bit32u)((RealPt)(ds_readd(DTP2)) + 30 * l100));

							strcpy((char*)Real2Host(ds_readd(0xbf95 + 4 * l100)),
								(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(weapon_id))));

							l100++;
						}
					}

					if (l100 == 0) {
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0xbb8),
							(char*)hero + 0x10);
						GUI_output(Real2Host(ds_readd(DTP2)));
					} else {
						sprintf((char*)Real2Host(ds_readd(0xd2eb)),
							(char*)get_dtp(0xbc),
							(char*)hero + 0x10);

						refresh_screen_size();
						bak = ds_readws(TEXTBOX_WIDTH);
						ds_writews(TEXTBOX_WIDTH, 6);

						l_di = GUI_radio(Real2Host(ds_readd(0xd2eb)), l100,
								Real2Host(ds_readd((0xbf95 + 0x00))),
								Real2Host(ds_readd((0xbf95 + 0x04))),
								Real2Host(ds_readd((0xbf95 + 0x08))),
								Real2Host(ds_readd((0xbf95 + 0x0c))),
								Real2Host(ds_readd((0xbf95 + 0x10))),
								Real2Host(ds_readd((0xbf95 + 0x14))),
								Real2Host(ds_readd((0xbf95 + 0x18))),
								Real2Host(ds_readd((0xbf95 + 0x1c))),
								Real2Host(ds_readd((0xbf95 + 0x20))),
								Real2Host(ds_readd((0xbf95 + 0x24))),
								Real2Host(ds_readd((0xbf95 + 0x28))),
								Real2Host(ds_readd((0xbf95 + 0x2c))),
								Real2Host(ds_readd((0xbf95 + 0x30))),
								Real2Host(ds_readd((0xbf95 + 0x34))),
								Real2Host(ds_readd((0xbf95 + 0x38))),
								Real2Host(ds_readd((0xbf95 + 0x3c))));

						ds_writews(TEXTBOX_WIDTH, bak);

						update_mouse_cursor();

						if (l_di != -1) {

							drop_item(hero, slots[l_di -1], -1);

							/* subtract 1 BP */
							dec_ptr_bs(hero + 0x33);
						}
					}
				} else {
					/* no BP left */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x44),
						(char*)hero + 0x10);
					GUI_output(Real2Host(ds_readd(DTP2)));
				}

			} else if (l_di == 12) {
				/* QUIT AND RELOAD / VERLASSEN / LADEN */

				refresh_screen_size();

				if (GUI_bool(get_dtp(0xd0))) {
					done = 1;
					ds_writew(IN_FIGHT, 0);
					ds_writew(0xc3c1, 7);
				}

				update_mouse_cursor();

			} else if (l_di == 13) {
				/* REPEAT OPTIONS / ALTE OPTION */

				done = 1;

				/* check last action and target_id */
				if (((host_readbs(hero + 0x84) == 4) ||
					(host_readbs(hero + 0x84) == 2) ||
					(host_readbs(hero + 0x84) == 15)) && (host_readbs(hero + 0x86) > 0))
				{

					/* TODO: check fight_id upper bound */
					if (((host_readbs(hero + 0x86) >= 10)
						&& (test_bit0(p_datseg + 0xd110 + 62 * host_readbs(hero + 0x86)))) ||
						((host_readbs(hero + 0x86) < 10)
						&& (hero_dead(get_hero(host_readbs(hero + 0x86) - 1)))))
					{

						GUI_output(get_dtp(0x74));
						host_writebs(hero + 0x84, 10);
						host_writebs(hero + 0x86, 0);
						done = 0;

					/* TODO: check fight_id upper bound */
					} else if (((host_readbs(hero + 0x86) >= 10)
						&& (test_bit2(p_datseg + 0xd0df + 0x32 + 62 * host_readbs(hero + 0x86)))) ||
						((host_readbs(hero + 0x86) < 10)
						&& (hero_unkn3(get_hero(host_readbs(hero + 0x86) - 1)))))
					{

						/* GUI_output(get_dtp(0x74)); */
						host_writebs(hero + 0x84, 10);
						host_writebs(hero + 0x86, 0);
						done = 0;
					} else if (((host_readbs(hero + 0x84) == 4) ||
							(host_readbs(hero + 0x84) == 15)) &&
							!check_hero_range_attack(hero, hero_pos))
					{
						/* GUI_output(get_dtp(0x74)); */
						host_writebs(hero + 0x84, 10);
						host_writebs(hero + 0x86, 0);
						done = 0;
					}
				}
			}
		}
	}

	if ((ds_readws(CURRENT_FIG_NR) == 192) &&
		(get_hero_index(Real2Host(ds_readd(0x3e20))) != hero_pos) &&
		((host_readbs(hero + 0x84) == 2) || (host_readbs(hero + 0x84) == 15) ||
		(host_readbs(hero + 0x84) == 4) || (host_readbs(hero + 0x84) == 5)))
	{
		for (slot_nr = 0; slot_nr < 20; slot_nr++) {
			and_ds_bs((ENEMY_SHEETS + 0x31) + 62 * slot_nr, (signed char)0xdf);
		}

		ds_writeb(0x5f30, 1);
	}
}

#if !defined(__BORLANDC__)
}
#endif
