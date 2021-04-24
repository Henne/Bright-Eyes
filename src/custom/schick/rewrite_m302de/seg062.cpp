/**
 *	Rewrite of DSA1 v3.02_de functions of seg062 (temple: miracle)
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

	load_tx2(ARCHIVE_FILE_WONDER_LTX);

	strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(0));

	/* check gods estimation */
	if (ds_readds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD)) >= 100) {

		bonus = (signed short)((ga1.a[ds_readws(TEMPLE_GOD)] * (ds_readds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD)) / 100) / 10) - l3);

		if (ds_readbs(CURRENT_TOWN) == TOWNS_CLANEGH) {
			/* CLANEGH */
			bonus += 2;
		}

		sub_ds_ds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD), 10);

		if (random_schick(100) <= god_dice.a[ds_readws(TEMPLE_GOD)] + bonus) {

			l_si = random_schick(god_dice.a[ds_readws(TEMPLE_GOD)]);

			if (god_dice.a[ds_readws(TEMPLE_GOD)] == l_si) {
				miracle_resurrect(get_tx2(35));
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
								miracle_modify(get_hero(0) + (HERO_ATTRIB + 3 * ATTRIB_MU) - get_hero(0), HOURS(24), 1);
								strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(1));
								ds_writed(INGAME_TIMERS + 0x28, HOURS(24));
							}
						} else if (l_si <= 7) {
							/* MR+99 for 3 days */
							if (!ds_readd(INGAME_TIMERS + 0x2c)) {

								i = get_random_hero();

								if (i != -1 && !hero_gods_pissed(get_hero(i))) {

									slot = get_free_mod_slot();
									set_mod_slot(slot, DAYS(3), get_hero(i) + HERO_MR,
										99, (signed char)i);

									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_tx2(2),
										(char*)get_hero(i) + HERO_NAME2);
									ds_writed(INGAME_TIMERS + 0x2c, 3 * HOURS(24));
								}
							}
						} else if (l_si <= 8) {
							/* remove a transformation or a curse of one hero */
							for (i = 0; i <= 6; i++) {
								hero = get_hero(i);

								if (hero_transformed(hero)) {

									and_ptr_bs(hero + HERO_STATUS2, 0xbf); /* unset 'transformed' status bit */

									for (i = 0; i <= 6; i++) {
										inc_ptr_bs(hero + HERO_ATTRIB + 3 * i);
									}

									sprintf((char*)Real2Host(ds_readd(DTP2)),
										(char*)get_ttx(565),
										(char*)hero + HERO_NAME2);
									break;
								} else {
									if (hero_renegade(hero) &&
										host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
										!hero_gods_pissed(hero))
									{
										and_ptr_bs(hero + HERO_STATUS1, 0xdf); /* unset 'renegade' status bit */

										sprintf((char*)Real2Host(ds_readd(DTP2)),
											(char*)get_tx2(3),
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
							miracle_modify(get_hero(0) + (HERO_TALENTS + TA_SCHWERTER) - get_hero(0), 3 * HOURS(24), 1); /* for 3 days: skill 'Schwerter' + 1 */
							miracle_modify(get_hero(0) + (HERO_AT + 3) - get_hero(0), 3 * HOURS(24), 1); /* for 3 days: AT + 1 */
							ds_writed(INGAME_TIMERS + 0x30, 3 * HOURS(24));
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(4));
						}
					} else if (l_si <= 7) {
						ds_writed(INGAME_TIMERS, HOURS(6));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(5));
					} else if (l_si <= 8) {
						if (!ds_readd(INGAME_TIMERS + 0x34)) {
							miracle_weapon(get_tx2(6), 0);
							ds_writed(INGAME_TIMERS + 0x34, HOURS(24));
						}
					}
					break;
				}
				case 3: {
					/* EFFERD */
					if (l_si <= 5) {
						ds_writed(INGAME_TIMERS + 4, 3 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(7));
					} else if (l_si <= 8) {
						ds_writed(INGAME_TIMERS + 8, 3 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(8));
					} else if (l_si <= 9) {
						if (!ds_readd(INGAME_TIMERS + 0x38)) {
							/* Schwimmen + 2 for 4 days */
							miracle_modify(get_hero(0) + (HERO_TALENTS + TA_SCHWIMMEN) - get_hero(0), 4 * HOURS(24), 2);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(9));
							ds_writed(INGAME_TIMERS + 0x38, 4 * HOURS(24));
						}
					}
					break;
				}
				case 4: {
					/* TRAVIA */
					if (l_si <= 10) {
						/* food miracle */
						for (i = 0; i <= 6; i++) {
							hero = get_hero(i);

							if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
								host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_gods_pissed(hero))
							{
								host_writebs(hero + HERO_HUNGER, host_writebs(hero + HERO_THIRST, 0));
							}
						}

						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(10));
					} else if (l_si <= 15) {
						miracle_heal_hero(dice_roll(1, 6, 2), get_tx2(11));
					} else if (l_si <= 16) {
						ds_writed(INGAME_TIMERS + 0x10, 7 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(12));
					}
					break;
				}
				case 5: {
					/* BORON */
					if (l_si <= 3) {
						ds_writed(INGAME_TIMERS + 0x14, 3 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(13));
					} else if (l_si <= 4) {
						miracle_resurrect(get_tx2(14));
					} else if (l_si <= 5) {
						if (!ds_readd(INGAME_TIMERS + 0x3c)) {
							miracle_modify(get_hero(0) + (HERO_ATTRIB + 3 * ATTRIB_TA) - get_hero(0), 4 * HOURS(24), -1);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(15));
							ds_writed(INGAME_TIMERS + 0x3c, 4 * HOURS(24));
						}
					}
					break;
				}
				case 6: {
					/* HESINDE */
					if (l_si <= 3) {
						if (!ds_readd(INGAME_TIMERS + 0x40)) {
							miracle_modify(get_hero(0) + (HERO_SP_VISION + 1) - get_hero(0), 4 * HOURS(24), 1);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(16));
							ds_writed(INGAME_TIMERS + 0x40, 4 * HOURS(24));
						}
					} else if (l_si <= 6) {
						/* unset transformation or renegade state of the first feasible hero */
						for (i = 0; i <= 6; i++) {
							hero = get_hero(i);

							if (hero_transformed(hero)) {

								and_ptr_bs(hero + HERO_STATUS2, 0xbf); /* unset 'transformed' status bit */

								for (i = 0; i <= 6; i++) {
									inc_ptr_bs(hero + HERO_ATTRIB + 3 * i);
								}

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_ttx(565),
									(char*)hero + HERO_NAME2);
								break;
							} else if (hero_renegade(hero) &&
								host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_gods_pissed(hero))
							{
								and_ptr_bs(hero + HERO_STATUS1, 0xdf); /* unset 'renegade' status bit */

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_tx2(17),
									(char*)hero + HERO_NAME2);
								break;
							}
						}
					} else if (l_si <= 7) {
						if (!ds_readd(INGAME_TIMERS + 0x44)) {
							miracle_modify(get_hero(0) + HERO_MR - get_hero(0), 3 * HOURS(24), 5);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(18));
							ds_writed(INGAME_TIMERS + 0x44, 3 * HOURS(24));
						}
					}
					break;
				}
				case 7: case 14: {
					/* FIRUN & IFIRN */
					if (l_si <= 5) {
						ds_writed(INGAME_TIMERS + 0x0c, 3 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(19));
					} else if (l_si <= 8) {
						ds_writed(INGAME_TIMERS + 0x0c, 7 * HOURS(24));
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(19));
					} else if (l_si <= 9) {
						/* +1 for 7 days */
						if (!ds_readd(INGAME_TIMERS + 0x48)) {

							i = get_random_hero();

							if (i != -1 && !hero_gods_pissed(get_hero(i))) {

								slot = get_free_mod_slot();
								set_mod_slot(slot, DAYS(7), get_hero(i) + HERO_HUNGER_TIMER,
									1, (signed char)i);

								host_writebs(get_hero(i) + HERO_HUNGER, host_writebs(get_hero(i) + HERO_THIRST, 0));

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_tx2(20),
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
						miracle_heal_hero(dice_roll(2, 6, 0), get_tx2(21));
					} else if (l_si <= 15) {

						hero = get_hero(0);
						for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

							if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
								host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_dead(hero) &&
								!hero_gods_pissed(hero))
							{
								/* heal hero completely */
								add_hero_le(hero, host_readws(hero + HERO_LE_ORIG));
							}
						}

						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(22));

					} else if (l_si <= 18) {
						miracle_resurrect(get_tx2(23));
					}
					break;
				}
				case 9: {
					/* PHEX wants a bit more estimation */
					if (ds_readds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD)) > 500) {

						if (l_si <= 5) {
							if (!ds_readd(INGAME_TIMERS + 0x4c)) {
								/* Taschendiebstahl +1 for 3 days */
								miracle_modify(get_hero(0) + (HERO_TALENTS + TA_TASCHENDIEBSTAHL) - get_hero(0), 3 * HOURS(24), 1);
								/* Schloesser knacken +1 for 3 days */
								miracle_modify(get_hero(0) + (HERO_TALENTS + TA_SCHLOESSER) - get_hero(0), 3 * HOURS(24), 1);
								strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(24));
								ds_writed(INGAME_TIMERS + 0x4c, 3 * HOURS(24));
							}
						} else if (l_si <= 8) {
							if (!ds_readd(INGAME_TIMERS + 0x50)) {
								/* Feilschen +1 for 3 days */
								miracle_modify(get_hero(0) + (HERO_TALENTS + TA_FEILSCHEN) - get_hero(0), 3 * HOURS(24), 1);
								strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(25));
								ds_writed(INGAME_TIMERS + 0x50, 3 * HOURS(24));
							}
						} else if (l_si <= 9) {
							if (!ds_readd(INGAME_TIMERS + 0x54)) {
								/* FF +1 for 3 days */
								miracle_modify(get_hero(0) + (HERO_ATTRIB + 3 * ATTRIB_FF) - get_hero(0), 3 * HOURS(24), 1);
								strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(26));
								ds_writed(INGAME_TIMERS + 0x54, 3 * HOURS(24));
							}
						}
					}
					break;
				}
				case 10: {
					/* PERAINE */
					if (l_si <= 10) {
						miracle_heal_hero(dice_roll(1, 6, 0), get_tx2(27));
					} else if (l_si <= 16) {
						miracle_heal_hero(dice_roll(2, 6, 0), get_tx2(27));
					} else if (l_si <= 18) {

						for (i = 0; i <= 6; i++) {
							hero = get_hero(i);
							disease = hero_is_diseased(hero);

							if (disease != 0 &&
								host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_gods_pissed(hero))
							{
								host_writeb(hero + (HERO_ILLNESS-5) + 5 * disease, 1);
								host_writeb(hero + (HERO_ILLNESS-4) + 5 * disease, 0);

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_tx2(28),
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

							if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
								host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_dead(hero) &&
								!hero_gods_pissed(hero))
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

						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(29));

					} else if (l_si <= 6) {
						if (!ds_readd(INGAME_TIMERS + 0x58)) {
							miracle_weapon(get_tx2(30), 0);
							ds_writed(INGAME_TIMERS + 0x58, HOURS(24));
						}
					} else if (l_si <= 7) {
						miracle_weapon(get_tx2(31), 1);
					}
					break;
				}
				case 12: {
					/* RAHJA */
					if (l_si <= 8) {
						if (!ds_readd(INGAME_TIMERS + 0x5c)) {
							/* Betören +2 for 7 days */
							miracle_modify(get_hero(0) + (HERO_TALENTS + TA_BETOEREN) - get_hero(0), 7 * HOURS(24), 2);
							/* Tanzen +2 for 7 days */
							miracle_modify(get_hero(0) + (HERO_TALENTS + TA_TANZEN) - get_hero(0), 7 * HOURS(24), 2);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(32));
							ds_writed(INGAME_TIMERS + 0x5c, 7 * HOURS(24));
						}
					} else if (l_si <= 13) {
						if (!ds_readd(INGAME_TIMERS + 0x60)) {
							/* CH +1 for 3 days */
							miracle_modify(get_hero(0) + (HERO_ATTRIB + 3 * ATTRIB_CH) - get_hero(0), 3 * HOURS(24), 1);
							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(33));
							ds_writed(INGAME_TIMERS + 0x60, 3 * HOURS(24));
						}
					} else if (l_si <= 14) {

						if (!ds_readds(INGAME_TIMERS + 0x64)) {

							hero = get_hero(0);
							for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

								if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
									host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
									!hero_dead(hero) &&
									!hero_gods_pissed(hero))
								{
									/* permanent Betören +1 */
									inc_ptr_bs(hero + (HERO_TALENTS + TA_BETOEREN));
									/* permanent Tanzen +1 */
									inc_ptr_bs(hero + (HERO_TALENTS + TA_TANZEN));
								}
							}

							strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_tx2(34));
							ds_writed(INGAME_TIMERS + 0x64, 7 * HOURS(24));
						}

					}
					break;
				}
				/* NO miracles for SWAFNIR */
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
		load_tx2(fi_bak);
	}
}

#if !defined(__BORLANDC__)
}
#endif
