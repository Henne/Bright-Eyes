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
	signed short bonus;
	signed short l5;
	signed short i;
	signed short slot;
	signed short item_id;
#if !defined(__BORLANDC__)
	struct dummy15 ga1 = {{0, 2, 15, 10, 20, 5, 10, 1, 15, 3, 15, 5, 10, 0}};
	struct dummy15 god_dice = {{0, 9, 9, 10, 17, 6, 10, 10, 18, 10, 19, 8, 15, 0, 10}};
#else
	struct dummy15 ga1 = *(struct dummy15*)(p_datseg + TEMPLE_MIRACLE_BONUS);
	struct dummy15 god_dice = *(struct dummy15*)(p_datseg + TEMPLE_MIRACLE_DICE);
#endif

	l3 = 0;
	fi_bak = ds_readws(TEXT_FILE_INDEX);

	load_city_ltx(ARCHIVE_FILE_WONDER_LTX);

	strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(0));

	/* check gods estimation */
	if (ds_readds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD)) >= 100) {

		bonus = (signed short)((ga1.a[ds_readws(TEMPLE_GOD)] * (ds_readds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD)) / 100) / 10) - l3);

		if (ds_readbs(CURRENT_TOWN) == 23) {
			/* CLANEGH */
			bonus += 2;
		}

		sub_ds_ds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD), 10);

		if (random_schick(100) <= god_dice.a[ds_readws(TEMPLE_GOD)] + bonus) {

			l_si = random_schick(god_dice.a[ds_readws(TEMPLE_GOD)]);

			if (god_dice.a[ds_readws(TEMPLE_GOD)] == l_si) {
				miracle_resurrect(get_city(0x8c));
			} else {
				switch (ds_readws(TEMPLE_GOD)) {
				case 1: {
					/* PRAIOS */
					l5 = 1;

					for (i = 0; ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) > i; i++) {
						if (host_readbs(get_hero(i) + HERO_TYPE) >= 7) {
							l5 = 1;
						}
					}

					if (l5 != 0) {

						if (l_si <= 5) {
							/* MU+1 for 1 day */
							if (!ds_readd(INGAME_TIMERS + 0x28)) {
								miracle_modify(get_hero(0) + HERO_MU - get_hero(0), HOURS(24), 1);
								strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_city(4));
								ds_writed(INGAME_TIMERS + 0x28, HOURS(24));
							}
						} else if (l_si <= 7) {
							/* MR+99 for 3 days */
							if (!ds_readd(INGAME_TIMERS + 0x2c)) {

								i = get_random_hero();

								if (i != -1 && !hero_dummy4(get_hero(i))) {

									slot = get_free_mod_slot();
									set_mod_slot(slot, 3 * HOURS(24), get_hero(i) + HERO_MR,
										99, (signed char)i);

									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_city(8),
										(char*)get_hero(i) + HERO_NAME2);
									ds_writed(INGAME_TIMERS + 0x2c, 3 * HOURS(24));
								}
							}
						} else if (l_si <= 8) {
							/* remove a transformation or a curse of one hero */
							for (i = 0; i <= 6; i++) {
								hero = get_hero(i);

								if (hero_transformed(hero)) {

									/* unset transformation */
									and_ptr_bs(hero + HERO_STATUS2, 0xbf);

									for (i = 0; i <= 6; i++) {
										inc_ptr_bs(hero + HERO_MU + 3 * i);
									}

									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ltx(0x8d4),
										(char*)hero + HERO_NAME2);
									break;
								} else {
									if (hero_cursed(hero) &&
										host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
										!hero_dummy4(hero))
									{
										/* unset curse */
										and_ptr_bs(hero + HERO_STATUS1, 0xdf);

										sprintf((char*)Real2Host(ds_readd(DTP2)),
											(char*)get_city(0x0c),
											(char*)hero + HERO_NAME2);
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

							if (host_readbs(hero + HERO_TYPE) != 0 &&
								host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_dummy4(hero))
							{
								host_writebs(hero + HERO_HUNGER, host_writebs(hero + HERO_THIRST, 0));
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
								and_ptr_bs(hero + HERO_STATUS2, 0xbf);

								for (i = 0; i <= 6; i++) {
									inc_ptr_bs(hero + HERO_MU + 3 * i);
								}

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_ltx(0x8d4),
									(char*)hero + HERO_NAME2);
								break;
							} else {
								if (hero_cursed(hero) &&
									host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
									!hero_dummy4(hero))
								{
									/* unset curse */
									and_ptr_bs(hero + HERO_STATUS1, 0xdf);

									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_city(0x44),
										(char*)hero + HERO_NAME2);
									break;
								}
							}
						}
					} else if (l_si <= 7) {
						if (!ds_readd(INGAME_TIMERS + 0x44)) {
							miracle_modify(get_hero(0) + HERO_MR - get_hero(0), 3 * HOURS(24), 5);
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
								set_mod_slot(slot, 7 * HOURS(24), get_hero(i) + HERO_HUNGER_TIMER,
									1, (signed char)i);

								host_writebs(get_hero(i) + HERO_HUNGER, host_writebs(get_hero(i) + HERO_THIRST, 0));

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_city(0x50),
									(char*)get_hero(i) + HERO_NAME2);
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
						for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

							if (host_readbs(hero + HERO_TYPE) != 0 &&
								host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_dead(hero) &&
								!hero_dummy4(hero))
							{
								/* heal hero completely */
								add_hero_le(hero, host_readws(hero + HERO_LE_ORIG));
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
					if (ds_readds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD)) > 500) {

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
								host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_dummy4(hero))
							{
								host_writeb(hero + (HERO_ILLNESS-5) + 5 * disease, 1);
								host_writeb(hero + (HERO_ILLNESS-4) + 5 * disease, 0);

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_city(0x70),
									(char*)hero + HERO_NAME2);
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

							if (host_readbs(hero + HERO_TYPE) != 0 &&
								host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_dead(hero) &&
								!hero_dummy4(hero))
							{

								for (j = 0; j < 23; j++) {
									if ((item_id = host_readws(hero + HERO_ITEM_HEAD + 14 * j)) &&
										item_weapon(get_itemsdat(item_id)))
									{
										if (host_readbs(hero + (HERO_ITEM_HEAD+6) + 14 * j) > 0) {
											dec_ptr_bs(hero + (HERO_ITEM_HEAD+6) + 14 * j);
										}

										if (host_readbs(hero + (HERO_ITEM_HEAD+6) + 14 * j) > 0) {
											dec_ptr_bs(hero + (HERO_ITEM_HEAD+6) + 14 * j);
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
							for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

								if (host_readbs(hero + HERO_TYPE) != 0 &&
									host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
									!hero_dead(hero) &&
									!hero_dummy4(hero))
								{
									inc_ptr_bs(hero + (HERO_TA_SOCIAL+1));
									inc_ptr_bs(hero + (HERO_TA_BODY+7));
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

	/* restore text file except for WONDER.LTX, TAVERN.TLK and except for dialogs */
	if (fi_bak != -1 && fi_bak != ARCHIVE_FILE_WONDER_LTX
	    && fi_bak != ARCHIVE_FILE_TAVERN_TLK
	    && (fi_bak < 156 || fi_bak > 176)) {
		load_city_ltx(fi_bak);
	}
}

#if !defined(__BORLANDC__)
}
#endif
