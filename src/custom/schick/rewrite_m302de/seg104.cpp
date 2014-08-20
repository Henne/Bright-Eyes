/**
 *	Rewrite of DSA1 v3.02_de functions of seg104 (heros)
 *	Functions rewritten: 5/9
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg049.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
signed short hero_has_ingrendients(Bit8u *hero, signed short recipe_index)
{
	signed short i = 0;
	signed short retval = 1;
	Bit8u* r_ptr = p_datseg + 0xacda + recipe_index * 28;
	signed short item_pos;

	/* loop over ingrendients */
	while ((host_readws(r_ptr + i * 2 + 2) != -1) && (retval)) {

		item_pos = get_item_pos(hero, host_readws(r_ptr + i * 2 + 2));

		if (item_pos == -1) {
			/* needed item missing */
			retval = 0;
			ds_writew(0xe5c4, host_readws(r_ptr + i * 2 + 2));
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
		give_hero_new_item(hero, host_readws(r_ptr + i * 2 + 2), 1, 1);
		i--;
	}

	return retval;
}

/* Borlandified and identical */
void hero_use_ingrendients(Bit8u *hero, signed short recipe_index)
{
	signed short i = 0;
	Bit8u* r_ptr = p_datseg + 0xacda + recipe_index * 28;
	signed short item_pos;

	/* loop over ingrendients */
	while (host_readws(r_ptr + i * 2 + 2) != -1) {

		item_pos = get_item_pos(hero, host_readws(r_ptr + i * 2 + 2));

		/* drop all needed items */
		drop_item(hero, item_pos, 1);

		/* exchange wine- or brandybottles into glass flask */
		if ((host_readws(r_ptr + i * 2 + 2) == 0x5b) ||
			(host_readws(r_ptr + i * 2 + 2) == 0x5c))
		{
			give_hero_new_item(hero, 0x1f, 1, 1);
		}

		/* exchange oil into bronze flask */
		if (host_readws(r_ptr + i * 2 + 2) == 0x29)
		{
			give_hero_new_item(hero, 0x2a, 1, 1);
		}

		i++;
	}
}

signed short do_alchemy(Bit8u* hero, signed short recipe_index, signed short flag)
{
	Bit8u* r_ptr = p_datseg + 0xacda + recipe_index * 28;

	hero_use_ingrendients(hero, recipe_index);

	sub_ae_splash(hero, host_readws(r_ptr + 0x18));

	and_ptr_bs(hero + 0xaa, 0xf7);
	host_writeb(hero + 0x94, 0);
	/* set heros receipe to 0 */
	host_writeb(hero + 0x93, 0);
	host_writeb(hero + 0x9c, 0);

	if ((test_skill(hero, 0x20, host_readbs(r_ptr + 0x1a)) > 0) && (flag == 0))
	{
		/* success */

		give_hero_new_item(hero, host_readws(r_ptr + 0x16), 1, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0xb6c),
			hero + 0x10,
			Real2Host(GUI_names_grammar(1, host_readws(r_ptr + 0x16), 0)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		return 1;
	} else {
		/* fail */

		give_hero_new_item(hero, host_readws(r_ptr + 2), 1, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0xb70),
			hero + 0x10,
			Real2Host(GUI_names_grammar(2, host_readws(r_ptr + 0x16), 0)));

		GUI_output(Real2Host(ds_readd(DTP2)));
		return 0;
	}

}

/* Borlandified and identical */
signed short plan_alchemy(Bit8u *hero)
{
	signed short retval;
	signed short l_si;
	signed short item_pos;
	signed short recipes;
	signed short answer;
	signed short l4;
	signed short l5;
	signed short i;
	signed char recipe_index;
	Bit8u *hero_p;
	signed short l7;
	signed char array[13];


	retval = 1;
	recipes = 0;
	item_pos = get_item_pos(hero, 47);
	if (item_pos == -1) {
		/* no alchemyset */
		GUI_output(get_dtp(0xa8));
		retval = 0;
	} else {

		/* count all recipes an prepare the menu */
		for (i = 0; i <= 12; i++) {
			if (get_item_pos(hero, ds_readws(0xacda + i * 28)) != -1) {

				strcpy((char*)Real2Host(ds_readd(DTP2)) + recipes * 50,
					(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(ds_readws(0xacf0 + i * 28)))));

				ds_writed(0xbf95 + recipes * 4, (Bit32u)((RealPt)ds_readd(DTP2) + recipes * 50));
				array[recipes] = i;
				recipes++;
			}
		}

		if (recipes != 0) {
			/* ask which recipe should be used */
			l7 = ds_readws(0xbffd);
			ds_writew(0xbffd, 7);

			answer = GUI_radio(get_dtp(0xac), recipes,
						Real2Host(ds_readd(0xbf95)),
						Real2Host(ds_readd(0xbf99)),
						Real2Host(ds_readd(0xbf9d)),
						Real2Host(ds_readd(0xbfa1)),
						Real2Host(ds_readd(0xbfa5)),
						Real2Host(ds_readd(0xbfa9)),
						Real2Host(ds_readd(0xbfad)),
						Real2Host(ds_readd(0xbfb1)),
						Real2Host(ds_readd(0xbfb5)),
						Real2Host(ds_readd(0xbfb9)),
						Real2Host(ds_readd(0xbfbd)),
						Real2Host(ds_readd(0xbfc1)),
						Real2Host(ds_readd(0xbfc5)),
						Real2Host(ds_readd(0xbfc9)));

			ds_writew(0xbffd, l7);

			if (answer != -1) {

				recipe_index = array[answer -1];

				if (hero_has_ingrendients(hero, recipe_index)) {

					/* check AE costs */
					if (ds_readws(0xacf2 + recipe_index * 28) > host_readws(hero + 0x64)) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x97c),
							(char*)hero + 0x10);

							GUI_output(Real2Host(ds_readd(DTP2)));

							retval = 0;
					} else {

						if ((ds_readbs(LOCATION) == 7) && (ds_readbs(SLEEP_QUALITY) == -1)) {

							GUI_output(get_ltx(0x568));

							return 0;
						}

						/* check if the alchemic process takes more than 8h */
						if ((ds_readbs(0xacf5 + recipe_index * 28) > 8) && (ds_readbs(LOCATION) != 7)) {
								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_dtp(0xb0),
									ds_readbs(0xacf5 + recipe_index * 28));

									GUI_output(Real2Host(ds_readd(DTP2)));

									retval = 0;
						} else {
							if ((ds_readbs(0x2d3c) > 1) &&
								(ds_readbs(LOCATION) != 6) &&
								(ds_readbs(0xacf5 + recipe_index * 28) > 8))
							{

								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_dtp(0xb4),
									ds_readbs(0xacf5 + recipe_index * 28));

								sprintf((char*)Real2Host(ds_readd(0xd2eb)),
									(char*)get_dtp(0xbc),
									(char*)hero + 0x10);

								ds_writew(0xbffd, 7);

								do {
									l4 = GUI_radio(Real2Host(ds_readd(DTP2)), 3,
											get_dtp(0xb8),
											Real2Host(ds_readd(0xd2eb)),
											get_dtp(0xc0));
								} while (l4 == -1);

								ds_writew(0xbffd, 3);

							} else {
								l4 = 1;
							}

							if (l4 == 1) {
								timewarp(ds_readbs(0xacf5 + recipe_index *28) * 0x1518L);

								if (ds_readbs(LOCATION) != 6) {
									hero_p = get_hero(0);
									for (i = 0; i <= 6; i++, hero_p += 0x6da) {
										if ((host_readbs(hero_p + 0x21) != 0) &&
											(host_readbs(hero_p + 0x87) == ds_readbs(CURRENT_GROUP)))
										{
											GRP_hero_sleep(hero_p, ds_readbs(0xe3f1));
										}
									}
								} else {
									host_writed(hero + 0x8f, 0x1fa40);
								}

								retval = do_alchemy(hero, recipe_index, 0);

								ds_writeb(0xe3f1, -1);

							} else if (l4 == 2) {

								/* find a empty group */
								for (l5 = 0; ds_readbs(0x2d36 + l5) != 0; l5++);

								host_writeb(hero + 0x87, l5);
								inc_ds_bs(0x2d36 + l5);
								dec_ds_bs(0x2d36 + ds_readbs(CURRENT_GROUP));

								host_writeb(hero + 0x94,
									ds_readbs(0xacf5 + recipe_index * 28) / 24);

								host_writeb(hero + 0x93, recipe_index);
								host_writeb(hero + 0x9c, ds_readbs(TYPEINDEX));
								or_ptr_bs(hero + 0xaa, 8);

								GRP_save_pos(l5);
							} else {
								retval = 0;
							}
						}
					}
				} else {
					/* not all ingrendients */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0xc4),
						(char*)Real2Host(GUI_name_singular((Bit8u*)get_itemname(ds_readws(0xe5c4)))));

					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			}
		} else {
			/* no recipes */
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0xc8),
				(char*)hero + 0x10);

			GUI_output(Real2Host(ds_readd(DTP2)));
		}
	}

	return retval;
}

/* Borlandified and identical */
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
		if (get_item_pos(hero, 123) != -1) retval = 123;
		else if (get_item_pos(hero, 61) != -1) retval = 61;
		else if (get_item_pos(hero, 60) != -1) retval = 60;
		else if (get_item_pos(hero, 64) != -1) retval = 64;
		else if (get_item_pos(hero, 124) != -1) retval = 124;
		else if (get_item_pos(hero, 129) != -1) retval = 129;
		else if (get_item_pos(hero, 125) != -1) retval = 125;
		else if (get_item_pos(hero, 156) != -1) retval = 156;
		else if (get_item_pos(hero, 130) != -1) retval = 130;
		break;
	}
	case 4: {
		/* DONF SPRING */
		if (get_item_pos(hero, 124) != -1) retval = 124;
		break;
	}
	case 5: {
		/* JORUGA ROOT & GULMOND LEAF */
		if ((get_item_pos(hero, 130) != -1) && (get_item_pos(hero, 63) != -1)) retval = 999;
		break;
	}
	case 6: {
		/* WHIRLWEED */
		if (get_item_pos(hero, 61) != -1) retval = 61;
		break;
	}
	case 7: {
		/* JORUGA ROOT */
		if (get_item_pos(hero, 130) != -1) retval = 130;
		break;
	}
	}

	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
