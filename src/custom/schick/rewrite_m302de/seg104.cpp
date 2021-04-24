/**
 *	Rewrite of DSA1 v3.02_de functions of seg104 (alchemy, cure disease, heroes)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg104.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg026.h"
#include "seg047.h"
#include "seg049.h"
#include "seg053.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short hero_has_ingrendients(Bit8u *hero, signed short recipe_index)
{
	signed short i = 0;
	signed short retval = 1;
	Bit8u* r_ptr = p_datseg + ALCHEMY_RECIPES + recipe_index * RECIPE_SIZE;
	signed short item_pos;

	/* loop over ingrendients */
	while ((host_readws(r_ptr + i * 2 + RECIPE_INGREDIENTS) != -1) && (retval)) {

		item_pos = get_item_pos(hero, host_readws(r_ptr + i * 2 + RECIPE_INGREDIENTS));

		if (item_pos == -1) {
			/* needed item missing */
			retval = 0;
			ds_writew(ALCHEMY_MISSING_ITEM, host_readws(r_ptr + i * 2 + RECIPE_INGREDIENTS));
		} else {
			/* drop all needed items */
			drop_item(hero, item_pos, 1);
		}

		i++;
	}

	i--;

	if (!retval) {
		i--;
	}

	while (i >= 0) {
		/* give all needed items back */
		give_hero_new_item(hero, host_readws(r_ptr + i * 2 + RECIPE_INGREDIENTS), 1, 1);
		i--;
	}

	return retval;
}

void hero_use_ingrendients(Bit8u *hero, signed short recipe_index)
{
	signed short i = 0;
	Bit8u* r_ptr = p_datseg + ALCHEMY_RECIPES + recipe_index * RECIPE_SIZE;
	signed short item_pos;

	/* loop over ingrendients */
	while (host_readws(r_ptr + i * 2 + RECIPE_INGREDIENTS) != -1) {

		item_pos = get_item_pos(hero, host_readws(r_ptr + i * 2 + RECIPE_INGREDIENTS));

		/* drop all needed items */
		drop_item(hero, item_pos, 1);

		/* exchange wine- or brandybottles into glass flask */
		if ((host_readws(r_ptr + i * 2 + RECIPE_INGREDIENTS) == ITEM_WINE) ||
			(host_readws(r_ptr + i * 2 + RECIPE_INGREDIENTS) == ITEM_BRANDY))
		{
			give_hero_new_item(hero, ITEM_FLASK_GLASS, 1, 1);
		}

		/* exchange oil into bronze flask */
		if (host_readws(r_ptr + i * 2 + RECIPE_INGREDIENTS) == ITEM_OIL)
		{
			give_hero_new_item(hero, ITEM_FLASK_BRONZE, 1, 1);
		}

		i++;
	}
}

signed short do_alchemy(Bit8u* hero, signed short recipe_index, signed short flag_abort)
	/* flag_abort = 0: finalize running brewing process
	 * flag_abort = 1: abort running brewing process */
{
	Bit8u* r_ptr = p_datseg + ALCHEMY_RECIPES + recipe_index * RECIPE_SIZE;

	hero_use_ingrendients(hero, recipe_index);

	sub_ae_splash(hero, host_readws(r_ptr + RECIPE_AE));

	and_ptr_bs(hero + HERO_STATUS1, 0xf7); /* unset 'brewing' status bit */
	host_writeb(hero + HERO_RECIPE_TIMER, 0);
	/* set heroes receipe to 0 */
	host_writeb(hero + HERO_RECIPE_ID, 0);
	host_writeb(hero + HERO_HOSTEL_ID, 0);

	if ((test_skill(hero, TA_ALCHIMIE, host_readbs(r_ptr + RECIPE_DIFFICULTY)) > 0) && (flag_abort == 0))
	{
		/* success */

		give_hero_new_item(hero, host_readws(r_ptr + RECIPE_OUTCOME), 1, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(731),
			hero + HERO_NAME2,
			Real2Host(GUI_names_grammar(1, host_readws(r_ptr + RECIPE_OUTCOME), 0)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		return 1;
	} else {
		/* failure */
		/* give first ingredient back, which is always the bottle (glass or bronze). */
		give_hero_new_item(hero, host_readws(r_ptr + RECIPE_INGREDIENTS), 1, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(732),
			hero + HERO_NAME2,
			Real2Host(GUI_names_grammar(2, host_readws(r_ptr + RECIPE_OUTCOME), 0)));

		GUI_output(Real2Host(ds_readd(DTP2)));
		return 0;
	}

}

signed short plan_alchemy(Bit8u *hero)
{
	signed short retval;
	signed short item_pos;
	signed short recipes;
	signed short answer;
	signed short decision;
	signed short l5;
	signed short i;
	signed char recipe_index;
	Bit8u *hero_p;
	signed short l7;
	signed char array[13];


	retval = 1;
	recipes = 0;
	item_pos = get_item_pos(hero, ITEM_ALCHEMY_KIT);
	if (item_pos == -1) {
		/* no alchemy kit */
		GUI_output(get_tx(42));
		retval = 0;
	} else {

		/* count all recipes and prepare the menu */
		for (i = 0; i <= 12; i++) {
			if (get_item_pos(hero, ds_readws(ALCHEMY_RECIPES + i * RECIPE_SIZE)) != -1) {

				strcpy((char*)Real2Host(ds_readd(DTP2)) + recipes * 50,
					(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(ds_readws((ALCHEMY_RECIPES + RECIPE_OUTCOME) + i * RECIPE_SIZE)))));

				ds_writed(RADIO_NAME_LIST + recipes * 4, (Bit32u)((RealPt)ds_readd(DTP2) + recipes * 50));
				array[recipes] = (signed char)i;
				recipes++;
			}
		}

		if (recipes != 0) {
			/* ask which recipe should be used */
			l7 = ds_readws(TEXTBOX_WIDTH);
			ds_writew(TEXTBOX_WIDTH, 7);

			answer = GUI_radio(get_tx(43), (signed char)recipes,
						Real2Host(ds_readd(RADIO_NAME_LIST)),
						Real2Host(ds_readd((RADIO_NAME_LIST + 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 2 * 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 3 * 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 4 * 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 5 * 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 6 * 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 7 * 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 8 * 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 9 * 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 10 * 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 11 * 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 12 * 4))),
						Real2Host(ds_readd((RADIO_NAME_LIST + 13 * 4))));

			ds_writew(TEXTBOX_WIDTH, l7);

			if (answer != -1) {

				recipe_index = array[answer -1];

				if (hero_has_ingrendients(hero, recipe_index)) {

					if (ds_readws((ALCHEMY_RECIPES + RECIPE_AE) + recipe_index * RECIPE_SIZE) > host_readws(hero + HERO_AE)) {
						/* AE not sufficient => brewing not possible */

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(607),
							(char*)hero + HERO_NAME2);

							GUI_output(Real2Host(ds_readd(DTP2)));

							retval = 0;
					} else {
						/* AE sufficient */

						if ((ds_readbs(LOCATION) == LOCATION_INN) && (ds_readbs(SLEEP_QUALITY) == -1)) {
							/* no room booked => brewing not possible */

							GUI_output(get_ttx(346));

							return 0;
						}

						if ((ds_readbs((ALCHEMY_RECIPES + RECIPE_DURATION) + recipe_index * RECIPE_SIZE) > 8) && (ds_readbs(LOCATION) != LOCATION_INN)) {
							/* recipes with durations > 8 hours have to be done in a inn. */
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_tx(44),
								ds_readbs((ALCHEMY_RECIPES+RECIPE_DURATION) + recipe_index * RECIPE_SIZE));

							GUI_output(Real2Host(ds_readd(DTP2)));

							retval = 0;
						} else {
							if (
#ifndef M302de_ORIGINAL_BUGFIX
								/* Original-Bug 12: If the total number of heroes is >= 2, and the current group consists only of a single available hero (+possibly the non-brewing NPC),
								 * the option to separate that hero from the group is shown, resulting in an active group which is empty or contains only the NPC and dead/petrified/unconscious/renegade heroes
								 * See https://www.crystals-dsa-foren.de/showthread.php?tid=98&pid=166399#pid166399 and the following posts. */
								(ds_readbs(TOTAL_HERO_COUNTER) > 1) &&
#else
								((hero == get_hero(6)) || (count_heroes_available_in_group_ignore_npc() > 1)) && /* still allow to single out the NPC if he is the brewing hero */
#endif
								(ds_readbs(LOCATION) != LOCATION_WILDCAMP) &&
								(ds_readbs((ALCHEMY_RECIPES + RECIPE_DURATION) + recipe_index * RECIPE_SIZE) > 8)
							) {

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_tx(45),
									ds_readbs((ALCHEMY_RECIPES + RECIPE_DURATION) + recipe_index * RECIPE_SIZE));

								sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
									(char*)get_tx(47),
									(char*)hero + HERO_NAME2);

								ds_writew(TEXTBOX_WIDTH, 7);

								do {
									decision = GUI_radio(Real2Host(ds_readd(DTP2)), 3,
											get_tx(46), /* Die Zeit einfach verstreichen lassen */
											Real2Host(ds_readd(TEXT_OUTPUT_BUF)), /* <Held> von der Gruppe trennen */
											get_tx(48)); /* Lieber doch nicht brauen */
								} while (decision == -1);

								ds_writew(TEXTBOX_WIDTH, 3);

							} else {
								decision = 1;
							}

							if (decision == 1) {
								/* rest of group waits */
								timewarp(ds_readbs((ALCHEMY_RECIPES + RECIPE_DURATION) + recipe_index * RECIPE_SIZE) * 0x1518L);

								if (ds_readbs(LOCATION) != LOCATION_WILDCAMP) {
									hero_p = get_hero(0);
									for (i = 0; i <= 6; i++, hero_p += SIZEOF_HERO) {
										if ((host_readbs(hero_p + HERO_TYPE) != HERO_TYPE_NONE) &&
											(host_readbs(hero_p + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)))
										{
											GRP_hero_sleep(hero_p, ds_readbs(SLEEP_QUALITY));
										}
									}
								} else {
									host_writed(hero + HERO_STAFFSPELL_TIMER, 0x1fa40); /* 1 day. Why STAFFSPELL ?? */
								}

								retval = do_alchemy(hero, recipe_index, 0);

								ds_writeb(SLEEP_QUALITY, -1);

							} else if (decision == 2) {
								/* split group */

								/* find an empty group */
								/* Original-Bug: only 6 groups, but 7 heroes might cause an 'out of boundary' here */
								for (l5 = 0; ds_readbs(GROUP_MEMBER_COUNTS + l5) != 0; l5++);

								host_writebs(hero + HERO_GROUP_NO, (signed char)l5);
								inc_ds_bs_post(GROUP_MEMBER_COUNTS + l5);
								dec_ds_bs_post(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));

								host_writeb(hero + HERO_RECIPE_TIMER,
									ds_readbs((ALCHEMY_RECIPES + RECIPE_DURATION) + recipe_index * RECIPE_SIZE) / 24);
									/* time in days, rounded down */

								host_writeb(hero + HERO_RECIPE_ID, recipe_index);
								host_writeb(hero + HERO_HOSTEL_ID, ds_readbs(TYPEINDEX));
								or_ptr_bs(hero + HERO_STATUS1, 8); /* set 'brewing' status bit */

								GRP_save_pos(l5);
							} else {
								/* decision == 3, abort brewing process */
								retval = 0;
							}
						}
					}
				} else {
					/* not all ingrendients */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx(49),
						(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(ds_readws(ALCHEMY_MISSING_ITEM)))));

					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			}
		} else {
			/* no recipes */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(50),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));
		}
	}

	return retval;
}

signed short has_herb_for_disease(Bit8u *hero, signed short disease)
{
	signed short retval = 0;

	switch (disease) {
	case 1:
	case 3: {
		retval = 99;
		break;
	}
	case 2: {
		/* a subset of herbs */
		if (get_item_pos(hero, ITEM_BELMART)          != -1)
      retval = ITEM_BELMART;
		else if (get_item_pos(hero, ITEM_WHIRLWEED)   != -1)
      retval = ITEM_WHIRLWEED;
		else if (get_item_pos(hero, ITEM_EINBEERE)    != -1)
      retval = ITEM_EINBEERE;
		else if (get_item_pos(hero, ITEM_TARNELE)     != -1)
      retval = ITEM_TARNELE;
		else if (get_item_pos(hero, ITEM_DONF_SPRING) != -1)
      retval = ITEM_DONF_SPRING;
		else if (get_item_pos(hero, ITEM_FINAGE_TREE) != -1)
      retval = ITEM_FINAGE_TREE;
		else if (get_item_pos(hero, ITEM_MENCHAL)     != -1)
      retval = ITEM_MENCHAL;
		else if (get_item_pos(hero, ITEM_OLGIN_ROOT)  != -1)
      retval = ITEM_OLGIN_ROOT;
		else if (get_item_pos(hero, ITEM_JORUGA_ROOT) != -1)
      retval = ITEM_JORUGA_ROOT;
		break;
	}
	case 4: {
		/* DONF SPRING */
		if (get_item_pos(hero, ITEM_DONF_SPRING) != -1)
      retval = ITEM_DONF_SPRING;
		break;
	}
	case 5: {
		/* JORUGA ROOT & GULMOND LEAF */
    if ( (get_item_pos(hero, ITEM_JORUGA_ROOT) != -1)
         && (get_item_pos(hero, ITEM_GULMOND_LEAF) != -1))
      retval = 999;
		break;
	}
	case 6: {
		/* WHIRLWEED */
		if (get_item_pos(hero, ITEM_WHIRLWEED) != -1)
      retval = ITEM_WHIRLWEED;
		break;
	}
	case 7: {
		/* JORUGA ROOT */
		if (get_item_pos(hero, ITEM_JORUGA_ROOT) != -1)
      retval = ITEM_JORUGA_ROOT;
		break;
	}
	}

	return retval;
}

signed short skill_cure_disease(Bit8u *healer, Bit8u *patient, signed short handycap, signed short flag)
{
	signed short disease;
	signed short retval;

	signed short damage;
	signed short tx_file_bak;
	signed short herb;

	retval = 0;

	if (flag) {
		tx_file_bak = ds_readws(TX_FILE_INDEX);
		load_tx(ARCHIVE_FILE_SPELLTXT_LTX);
	}

	if (is_hero_healable(patient)) {

		disease = hero_is_diseased(patient);

		if (!disease) {
			/* not diseased */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(462),
				(char*)patient + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

		} else if (host_readds(patient + HERO_HEAL_TIMER) > 0) {

			/* recently tried to cure with skill */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(697),
				(char*)patient + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

		} else if (!(herb = has_herb_for_disease(healer, disease))) {
			/* no herb for this disease */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx(118),
				(char*)healer + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));
		} else {
			timewarp(0x1c2);

			/* set timer */
			host_writed(patient + HERO_HEAL_TIMER, 0x5460);

			if ((flag != 0) || (test_skill(healer, TA_HEILEN_KRANKHEITEN, (signed char)handycap) > 0)) {

				if (((retval = test_skill(healer, TA_HEILEN_KRANKHEITEN, ds_readbs(DISEASE_PRICES + 2 * disease) + handycap)) > 0) &&
					(disease != 1) && (disease != 3))
				{

					add_hero_le(patient, retval);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(695),
						(char*)healer + HERO_NAME2,
						(char*)patient + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(patient + 0x22), 3)),
						retval);

					GUI_output(Real2Host(ds_readd(DTP2)));

					/* cure the disease */
					host_writeb(patient + 0xae + disease * 5, 1);
					host_writeb(patient + 0xaf + disease * 5, 0);

					if (herb == 999) {
						/* drop JORUGA & GULMOND LEAF */
						drop_item(healer, get_item_pos(healer, ITEM_JORUGA_ROOT), 1);
						drop_item(healer, get_item_pos(healer, ITEM_GULMOND_LEAF), 1);
					} else {
						/* drop the herb */
						drop_item(healer, get_item_pos(healer, herb), 1);
					}

					retval = 1;
				} else {
					damage = 3;

					if (host_readws(patient + 0x60) <= damage) {
						damage = host_readws(patient + 0x60) - 1;
					}

					sub_hero_le(patient, damage);

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(694),
						(char*)patient + HERO_NAME2, damage);

					GUI_output(Real2Host(ds_readd(DTP2)));
				}

			} else {
				/* failed to heal */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(696),
					(char*)healer + HERO_NAME2,
					(char*)patient + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}


		if ((flag != 0) && (tx_file_bak != -1) && (tx_file_bak != ARCHIVE_FILE_SPELLTXT_LTX)) {
			load_tx(tx_file_bak);
		}
	}

	return retval;
}

RealPt get_heaviest_hero(void)
{
	RealPt hero;
	RealPt retval;

	signed short weight;
	signed short w_max;
	signed short i;

	w_max = 0;
	hero = (RealPt)ds_readd(HEROES);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if ((host_readbs(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readbs(Real2Host(hero) + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)))
		{
			weight = host_readws(Real2Host(hero) + HERO_WEIGHT) + host_readws(Real2Host(hero) + HERO_LOAD);

			if (weight > w_max) {
				w_max = weight;
				retval = hero;
			}
		}
	}

	return retval;
}

signed short get_hero_weight(Bit8u *hero)
{
	return host_readws(hero + HERO_WEIGHT) + host_readws(hero + HERO_LOAD);
}

signed short get_skilled_hero_pos(signed short skill)
{
	signed short i;
	signed short cur;

	signed short max;
	signed short pos;
	Bit8u *hero;

	max = -100;

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)))
		{

			cur =	host_readbs(hero + HERO_ATTRIB + 3 * (ds_readbs(SKILL_DESCRIPTIONS + 4 * skill))) +
				host_readbs(hero + HERO_ATTRIB_MOD + 3 * (ds_readbs(SKILL_DESCRIPTIONS + 4 * skill))) +
				host_readbs(hero + HERO_ATTRIB + 3 * (ds_readbs((SKILL_DESCRIPTIONS + 1) + 4 * skill))) +
				host_readbs(hero + HERO_ATTRIB_MOD + 3 * (ds_readbs((SKILL_DESCRIPTIONS + 1) + 4 * skill))) +
				host_readbs(hero + HERO_ATTRIB + 3 * (ds_readbs((SKILL_DESCRIPTIONS + 2) + 4 * skill))) +
				host_readbs(hero + HERO_ATTRIB_MOD + 3 * (ds_readbs((SKILL_DESCRIPTIONS + 2) + 4 * skill))) +
				host_readbs(hero + HERO_TALENTS + skill);

			if (cur > max) {
				max = cur;
				pos = i;
			}
		}
	}

	return pos;
}

#if !defined(__BORLANDC__)
}
#endif
