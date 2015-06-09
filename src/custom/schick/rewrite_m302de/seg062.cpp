/**
 *	Rewrite of DSA1 v3.02_de functions of seg062 (temple: wonder)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg062.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg047.h"
#include "seg061.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct dummy15 {
	signed char a[15];
};

void ask_miracle(void)
{
	signed short l_si;
	signed short j;
	Bit8u *hero;
	signed short disease;
	signed short fi_bak;
	signed short l3;
	signed short l4;
	signed short l5;
	signed short i;
	signed short slot;
	signed short item_id;
#if !defined(__BORLANDC__)
	struct dummy15 ga1 = *(struct dummy15*)(p_datseg + 0x6ea4);
	struct dummy15 ga2 = *(struct dummy15*)(p_datseg + 0x6eb3);
#else
	struct dummy15 ga1 = *(struct dummy15*)(p_datseg + 0x6ea4);
	struct dummy15 ga2 = *(struct dummy15*)(p_datseg + 0x6eb3);
#endif

	l3 = 0;
	fi_bak = ds_readws(0x26bd);

	load_city_ltx(223);

	strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0));

	/* check gods estimation */
	if (ds_readds(GODS_ESTIMATION + 4 * ds_readws(0xe3f8)) >= 100) {

		l4 = (ga1.a[ds_readws(0xe3f8)] * (ds_readds(GODS_ESTIMATION + 4 * ds_readws(0xe3f8)) / 100) / 10) - l3;

		if (ds_readbs(CURRENT_TOWN) == 23) {
			l4 += 2;
		}

		sub_ds_ds(GODS_ESTIMATION + 4 * ds_readws(0xe3f8), 10);

		if (random_schick(100) <= ga2.a[ds_readws(0xe3f8)] + l4) {

			l_si = random_schick(ga2.a[ds_readws(0xe3f8)]);

			if (ga2.a[ds_readws(0xe3f8)] == l_si) {
				miracle_resurrect(get_city(0x8c));
			} else {
				switch (ds_readws(0xe3f8)) {
				case 1: {
					/* PRAIOS */
					l5 = 1;

					for (i = 0; ds_readbs(0x2d36 + ds_readbs(CURRENT_GROUP)) > i; i++) {
						if (host_readbs(get_hero(i) + 0x21) >= 7) {
							l5 = 1;
						}
					}

					if (l5 != 0) {

						if (l_si <= 5) {
							/* MU+1 for 1 day */
							if (!ds_readd(INGAME_TIMERS + 0x28)) {
								miracle_modify(get_hero(0) + 0x35 - get_hero(0), HOURS(24), 1);
								strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(4));
								ds_writed(INGAME_TIMERS + 0x28, HOURS(24));
							}
						} else if (l_si <= 7) {
							/* MR+99 for 3 days */
							if (!ds_readd(INGAME_TIMERS + 0x2c)) {

								i = get_random_hero();

								if (i != -1 && !hero_dummy4(get_hero(i))) {

									slot = get_free_mod_slot();
									set_mod_slot(slot, 3 * HOURS(24), get_hero(i) + 0x66,
										99, i);

									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_city(8),
										(char*)get_hero(i) + 0x10);
									ds_writed(INGAME_TIMERS + 0x2c, 3 * HOURS(24));
								}
							}
						} else if (l_si <= 8) {
							/* remove a transformation or a curse of one hero */
							for (i = 0; i <= 6; i++) {
								hero = get_hero(i);

								if (hero_transformed(hero)) {

									/* unset transformation */
									and_ptr_bs(hero + 0xab, 0xbf);

									for (i = 0; i <= 6; i++) {
										inc_ptr_bs(hero + 0x35 + 3 * i);
									}

									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ltx(0x8d4),
										(char*)hero + 0x10);
									break;
								} else {
									if (hero_cursed(hero) &&
										host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
										!hero_dummy4(hero))
									{
										/* unset curse */
										and_ptr_bs(hero + 0xaa, 0xdf);

										sprintf((char*)Real2Host(ds_readd(DTP2)),
											(char*)get_city(0x0c),
											(char*)hero + 0x10);
										break;
									}
								}
							}
						}
					}
					break;
				}
				case 2: {
					/* RONDRA */
					if (l_si <= 5) {
						if (!ds_readd(INGAME_TIMERS + 0x30)) {
							miracle_modify(get_hero(0) + 0x10b - get_hero(0), 3 * HOURS(24), 1);
							miracle_modify(get_hero(0) + 0x6b - get_hero(0), 3 * HOURS(24), 1);
							ds_writed(INGAME_TIMERS + 0x30, 3 * HOURS(24));
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x10));
						}
					} else if (l_si <= 7) {
						ds_writed(INGAME_TIMERS, HOURS(6));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x14));
					} else if (l_si <= 8) {
						if (!ds_readd(INGAME_TIMERS + 0x34)) {
							miracle_weapon(get_city(0x18), 0);
							ds_writed(INGAME_TIMERS + 0x34, HOURS(24));
						}
					}
					break;
				}
				case 3: {
					/* EFFERD */
					if (l_si <= 5) {
						ds_writed(INGAME_TIMERS + 4, 3 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x1c));
					} else if (l_si <= 8) {
						ds_writed(INGAME_TIMERS + 8, 3 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x20));
					} else if (l_si <= 9) {
						if (!ds_readd(INGAME_TIMERS + 0x38)) {
							miracle_modify(get_hero(0) + 0x116 - get_hero(0), 4 * HOURS(24), 2);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x24));
							ds_writed(INGAME_TIMERS + 0x38, 4 * HOURS(24));
						}
					}
					break;
				}
				case 4: {
					/* TRAVIA */
					if (l_si <= 10) {
						/* food wonder */
						for (i = 0; i <= 6; i++) {
							hero = get_hero(i);

							if (host_readbs(hero + 0x21) != 0 &&
								host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
								!hero_dummy4(hero))
							{
								host_writebs(hero + 0x7f, host_writebs(hero + 0x80, 0));
							}
						}

						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x28));
					} else if (l_si <= 15) {
						miracle_heal_hero(dice_roll(1, 6, 2), get_city(0x2c));
					} else if (l_si <= 16) {
						ds_writed(INGAME_TIMERS + 0x10, 7 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x30));
					}
					break;
				}
				case 5: {
					/* BORON */
					if (l_si <= 3) {
						ds_writed(INGAME_TIMERS + 0x14, 3 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x34));
					} else if (l_si <= 4) {
						miracle_resurrect(get_city(0x38));
					} else if (l_si <= 5) {
						if (!ds_readd(INGAME_TIMERS + 0x3c)) {
							miracle_modify(get_hero(0) + 0x56 - get_hero(0), 4 * HOURS(24), -1);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x3c));
							ds_writed(INGAME_TIMERS + 0x3c, 4 * HOURS(24));
						}
					}
					break;
				}
				case 6: {
					/* HESINDE */
					if (l_si <= 3) {
						if (!ds_readd(INGAME_TIMERS + 0x40)) {
							miracle_modify(get_hero(0) + 0x164 - get_hero(0), 4 * HOURS(24), 1);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x40));
							ds_writed(INGAME_TIMERS + 0x40, 4 * HOURS(24));
						}
					} else if (l_si <= 6) {
						/* remove a transformation or a curse of one hero */
						for (i = 0; i <= 6; i++) {
							hero = get_hero(i);

							if (hero_transformed(hero)) {

								/* unset transformation */
								and_ptr_bs(hero + 0xab, 0xbf);

								for (i = 0; i <= 6; i++) {
									inc_ptr_bs(hero + 0x35 + 3 * i);
								}

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_ltx(0x8d4),
									(char*)hero + 0x10);
								break;
							} else {
								if (hero_cursed(hero) &&
									host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
									!hero_dummy4(hero))
								{
									/* unset curse */
									and_ptr_bs(hero + 0xaa, 0xdf);

									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_city(0x44),
										(char*)hero + 0x10);
									break;
								}
							}
						}
					} else if (l_si <= 7) {
						if (!ds_readd(INGAME_TIMERS + 0x44)) {
							miracle_modify(get_hero(0) + 0x66 - get_hero(0), 3 * HOURS(24), 5);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x48));
							ds_writed(INGAME_TIMERS + 0x44, 3 * HOURS(24));
						}
					}
					break;
				}
				case 7: case 14: {
					/* FIRUN & IFIRN */
					if (l_si <= 5) {
						ds_writed(INGAME_TIMERS + 0x0c, 3 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x4c));
					} else if (l_si <= 8) {
						ds_writed(INGAME_TIMERS + 0x0c, 7 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x4c));
					} else if (l_si <= 9) {
						/* +1 for 7 days */
						if (!ds_readd(INGAME_TIMERS + 0x48)) {

							i = get_random_hero();

							if (i != -1 && !hero_dummy4(get_hero(i))) {

								slot = get_free_mod_slot();
								set_mod_slot(slot, 7 * HOURS(24), get_hero(i) + 0x7e,
									1, i);

								host_writebs(get_hero(i) + 0x7f, host_writebs(get_hero(i) + 0x80, 0));

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_city(0x50),
									(char*)get_hero(i) + 0x10);
								ds_writed(INGAME_TIMERS + 0x48, 7 * HOURS(24));
							}
						}
					}
					break;
				}
				case 8: {
					/* TSA */
					if (l_si <= 10) {
						miracle_heal_hero(dice_roll(2, 6, 0), get_city(0x54));
					} else if (l_si <= 15) {

						hero = get_hero(0);
						for (i = 0; i <= 6; i++, hero += 0x6da) {

							if (host_readbs(hero + 0x21) != 0 &&
								host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
								!hero_dead(hero) &&
								!hero_dummy4(hero))
							{
								/* heal hero completely */
								add_hero_le(hero, host_readws(hero + 0x5e));
							}
						}

						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x58));

					} else if (l_si <= 18) {
						miracle_resurrect(get_city(0x5c));
					}
					break;
				}
				case 9: {
					/* PHEX wants a bit more estimation */
					if (ds_readds(GODS_ESTIMATION + 4 * ds_readws(0xe3f8)) > 500) {

						if (l_si <= 5) {
							if (!ds_readd(INGAME_TIMERS + 0x4c)) {
								miracle_modify(get_hero(0) + 0x139 - get_hero(0), 3 * HOURS(24), 1);
								miracle_modify(get_hero(0) + 0x138 - get_hero(0), 3 * HOURS(24), 1);
								strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x60));
								ds_writed(INGAME_TIMERS + 0x4c, 3 * HOURS(24));
							}
						} else if (l_si <= 8) {
							if (!ds_readd(INGAME_TIMERS + 0x50)) {
								miracle_modify(get_hero(0) + 0x11d - get_hero(0), 3 * HOURS(24), 1);
								strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x64));
								ds_writed(INGAME_TIMERS + 0x50, 3 * HOURS(24));
							}
						} else if (l_si <= 9) {
							if (!ds_readd(INGAME_TIMERS + 0x54)) {
								miracle_modify(get_hero(0) + 0x3e - get_hero(0), 3 * HOURS(24), 1);
								strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x68));
								ds_writed(INGAME_TIMERS + 0x54, 3 * HOURS(24));
							}
						}
					}
					break;
				}
				case 10: {
					/* PERAINE */
					if (l_si <= 10) {
						miracle_heal_hero(dice_roll(1, 6, 0), get_city(0x6c));
					} else if (l_si <= 16) {
						miracle_heal_hero(dice_roll(2, 6, 0), get_city(0x6c));
					} else if (l_si <= 18) {

						for (i = 0; i <= 6; i++) {
							hero = get_hero(i);
							disease = hero_is_diseased(hero);

							if (disease != 0 &&
								host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
								!hero_dummy4(hero))
							{
								host_writeb(hero + 0xae + 5 * disease, 1);
								host_writeb(hero + 0xaf + 5 * disease, 0);

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_city(0x70),
									(char*)hero + 0x10);
								break;
							}
						}
					}
					break;
				}
				case 11: {
					/* INGERIMM */
					if (l_si <= 5) {

						for (i = 0; i <= 6; i++) {
							hero = get_hero(i);

							if (host_readbs(hero + 0x21) != 0 &&
								host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
								!hero_dead(hero) &&
								!hero_dummy4(hero))
							{

								for (j = 0; j < 23; j++) {
									if ((item_id = host_readws(hero + 0x196 + 14 * j)) &&
										item_weapon(get_itemsdat(item_id)))
									{
										if (host_readbs(hero + 0x19c + 14 * j) > 0) {
											dec_ptr_bs(hero + 0x19c + 14 * j);
										}

										if (host_readbs(hero + 0x19c + 14 * j) > 0) {
											dec_ptr_bs(hero + 0x19c + 14 * j);
										}
									}
								}
							}
						}

						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x74));

					} else if (l_si <= 6) {
						if (!ds_readd(INGAME_TIMERS + 0x58)) {
							miracle_weapon(get_city(0x78), 0);
							ds_writed(INGAME_TIMERS + 0x58, HOURS(24));
						}
					} else if (l_si <= 7) {
						miracle_weapon(get_city(0x7c), 1);
					}
					break;
				}
				case 12: {
					/* RAHJA */
					if (l_si <= 8) {
						if (!ds_readd(INGAME_TIMERS + 0x5c)) {
							miracle_modify(get_hero(0) + 0x11c - get_hero(0), 7 * HOURS(24), 2);
							miracle_modify(get_hero(0) + 0x118 - get_hero(0), 7 * HOURS(24), 2);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x80));
							ds_writed(INGAME_TIMERS + 0x5c, 7 * HOURS(24));
						}
					} else if (l_si <= 13) {
						if (!ds_readd(INGAME_TIMERS + 0x60)) {
							miracle_modify(get_hero(0) + 0x3b - get_hero(0), 3 * HOURS(24), 1);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x84));
							ds_writed(INGAME_TIMERS + 0x60, 3 * HOURS(24));
						}
					} else if (l_si <= 14) {

						if (!ds_readds(INGAME_TIMERS + 0x64)) {

							hero = get_hero(0);
							for (i = 0; i <= 6; i++, hero += 0x6da) {

								if (host_readbs(hero + 0x21) != 0 &&
									host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
									!hero_dead(hero) &&
									!hero_dummy4(hero))
								{
									inc_ptr_bs(hero + 0x11c);
									inc_ptr_bs(hero + 0x118);
								}
							}

							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0x88));
							ds_writed(INGAME_TIMERS + 0x64, 7 * HOURS(24));
						}

					}
					break;
				}
				/* NO wonders for SWAFNIR */
				}
			}
		}
	}

	/* print the output */
	GUI_output(Real2Host(ds_readd(DTP2)));

	/* restore text file */
	if (fi_bak != -1 && fi_bak != 223 && fi_bak != 130 && (fi_bak < 156 || fi_bak > 176)) {
		load_city_ltx(fi_bak);
	}
}

#if !defined(__BORLANDC__)
}
#endif
