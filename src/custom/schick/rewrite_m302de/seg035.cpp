/**
 *	Rewrite of DSA1 v3.02_de functions of seg035 (fightsystem)
 *	Functions rewritten: 4/4 (complete)
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg032.h"
#include "seg034.h"
#include "seg038.h"
#include "seg039.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct dummy5 {
	char a[5];
};

struct dummy62 {
	char a[62];
};

/**
 * \brief compress monsters
 */
/* Borlandified and identical */
void FIG_tidy_monsters(void)
{
	signed short i;
	signed short j;
	signed short monsters = FIG_count_active_enemies();

	i = 0;
	while (i < 20) {

		/* if the monster is not able to fight anymore ... */
		if ((ds_readbs(ENEMY_SHEETS + 62 * i) != 0) &&
			(enemy_dead(Real2Host(RealMake(datseg, ENEMY_SHEETS + 62 * i))) ||
			enemy_uncon(Real2Host(RealMake(datseg, ENEMY_SHEETS + 62 * i))) ||
			enemy_stoned(Real2Host(RealMake(datseg, ENEMY_SHEETS + 62 * i))) ||
			((host_readbs(Real2Host(ds_readd(0xbd28)) + 5 * i + 0x1a) != 0) && (monsters == 0))))
		{

			if (i == 19) {
				/* just clear the last one */
				memset(Real2Host(ds_readd(0xbd28)) + 5 * i + 0x16, 0, 5);
				break;
			} else {
				/* move the next monsters one position to the front */
				for (j = i; j < 19; j++) {

					*(struct dummy5*)(Real2Host(ds_readd(0xbd28)) + 5 * j + 0x16) =
						*(struct dummy5*)(Real2Host(ds_readd(0xbd28)) + 5 * (j + 1) + 0x16);

					memset(Real2Host(ds_readd(0xbd28)) + 5 * (j + 1) + 0x16, 0, 5);

					*(struct dummy62*)(p_datseg + ENEMY_SHEETS + 62 * j) =
						*(struct dummy62*)(p_datseg + ENEMY_SHEETS + 62 * (j + 1));

					memset(p_datseg + ENEMY_SHEETS + 62 * (j + 1), 0, 62);

					or_ds_bs(0xd3ba + 62 * j, 1);
				}
			}
		} else {
			i++;
		}
	}
}

/* Borlandified and identical */
void FIG_loot_monsters(void)
{
	signed short l_si;
	signed short l_di;
	signed short l1;
	signed short l3;
	signed short l4;
	signed short l5;
	signed short l6;
	Bit32s money;
	signed short bak1;
	signed short bak2;

	char *a[31];

	l_si = 0;

	bak1 = ds_readws(0xe318);
	ds_writew(0xe318, 0);

	for (l_di = 0; l_di < 30; l_di++) {

			a[l_di] = (char*)Real2Host(F_PADD(ds_readd(0xc3db), 40 * l_di));
	}

	do {

		l_di = l3 = 0;

		while (((l1 = host_readws(Real2Host(ds_readd(0xbd28)) + 2 * l_di + 0x96)) != 0) &&
			/* ITEM 164 is "BONE WITH RUNES" */
			(l_di < 30) && (l1 != 164))
		{
			strcpy(a[l_di++], (char*)Real2Host(GUI_name_plural(0, (Bit8u*)get_itemname(l1))));
			l3++;
		}

		if (l3 != 0) {

			if (l3 > 15) {

				if (l_si == 0) {
					l5 = 15;
					l6 = 16;
				} else {
					l5 = l3;
					l6 = l3 - 14;
				}

				strcpy(a[l5], (char*)get_ltx(0xbbc));
			} else {
				l_si = 0;
				l5 = 0;
				l6 = l3;
			}

			bak2 = ds_readws(0xbffd);
			ds_writew(0xbffd, 6);

			l4 = GUI_radio(get_dtp(0x38), l6,
						a[0 + l_si], a[1 + l_si], a[2 + l_si], a[3 + l_si],
						a[4 + l_si], a[5 + l_si], a[6 + l_si], a[7 + l_si],
						a[8 + l_si], a[9 + l_si], a[10 + l_si], a[11 + l_si],
						a[12 + l_si], a[13 + l_si], a[14 + l_si], a[15 + l_si]) - 1;

			ds_writew(0xbffd, bak2);


			if ((l5 != 0) && (l6 - 1 == l4)) {
				if (!l_si) {
					l_si = 15;
				} else {
					l_si = 0;
				}
			}

			if ((l4 != -2) && ((l5 == 0) || ((l5 != 0) && (l6 - 1 != l4)))) {

				if (!get_item(host_readws(Real2Host(ds_readd(0xbd28)) + 2 * (l4 + l_si) + 0x96), 1, 1))
				{
					l4 = -2;
				} else {
					host_writew(Real2Host(ds_readd(0xbd28)) + 2 * (l4 + l_si) + 0x96, 0);

					for (l_di = l4 + l_si; l_di < 29; l_di++) {

						host_writew(Real2Host(ds_readd(0xbd28)) + 2 * (l_di) + 0x96,
							host_readws(Real2Host(ds_readd(0xbd28)) + 2 * (l_di + 1) + 0x96));

						host_writew(Real2Host(ds_readd(0xbd28)) + 2 * (l_di + 1) + 0x96, 0);
					}
				}
			}

		} else {
			l4 = -2;
		}
	} while (l4 != -2);

	money = host_readws(Real2Host(ds_readd(0xbd28)) + 0xd2) * 100;
	money += host_readws(Real2Host(ds_readd(0xbd28)) + 0xd4) * 10;
	money += host_readws(Real2Host(ds_readd(0xbd28)) + 0xd6);

	if (money > 0) {

		make_valuta_str((char*)Real2Host(ds_readd(0xd2eb)), money);

		sprintf((char*)(Real2Host(ds_readd(DTP2))),
			(char*)get_dtp(0x3c),
			Real2Host(ds_readd(0xd2eb)));
		GUI_output(Real2Host(ds_readd(DTP2)));

		set_party_money(get_party_money() + money);
	}

	ds_writew(0xe318, bak1);
}

/**
 * \brief	give the group the AP after a fight
 */
/* Borlandified and identical */
void FIG_split_ap(void)
{
	signed short l_si;
	signed short ap;
	signed short known_ap;
	signed short bak;

	ap = 0;
	bak = ds_readws(0xe318);
	ds_writew(0xe318, 0);

	/* calculate ap from all monsters in that fight */
	for (l_si = 0; l_si < 20; l_si++) {

		if (ds_readbs(ENEMY_SHEETS + 62 * l_si) != 0) {

			if (ds_readbs(0x4351 + ds_readbs(ENEMY_SHEETS + 62 * l_si)) != 0) {

				/* monster is already known */
				known_ap = ds_readbs((ENEMY_SHEETS + 26) + 62 * l_si) / 10;
				ap += (known_ap == 0 ? 1 : known_ap);
			} else {
				/* first time bonus */
				ap += ds_readbs((ENEMY_SHEETS + 26) + 62 * l_si);
			}
		}
	}

	/* mark each monster type from that fight */
	for (l_si = 0; l_si < 20; l_si++) {

		if (ds_readbs(ENEMY_SHEETS + 62 * l_si) != 0) {

			ds_writeb(0x4351 + ds_readbs(ENEMY_SHEETS + 62 * l_si), 1);
		}
	}

	if (count_heroes_available_in_group() > ap) {
		/* every hero gets at least 1 AP */

		ap = count_heroes_available_in_group();

	} else {
		/* every hero should get the same AP */
		while ((ap % count_heroes_available_in_group())) {
			ap++;
		}
	}

	/* prepare output */
	sprintf((char*)Real2Host(ds_readd(DTP2)), (char*)get_dtp(0x84), ap);
	GUI_output(Real2Host(ds_readd(DTP2)));

	/* give AP to the group */
	add_group_ap(ap);

	ds_writew(0xe318, bak);
}

/**
 * \brief		test if a range attack is possible
 *
 * \param hero		pointer to the hero
 * \param hero_pos	position of the hero
 *
 * \return		1 = range attack is possible, 0 = ... not possible
 */
/* Borlandified and identical */
signed short check_hero_range_attack(Bit8u *hero, signed short hero_pos)
{
	signed short i;
	signed short retval;	/* retval */
	signed short target_x;
	signed short target_y;
	signed short hero_x;
	signed short hero_y;

	retval = 1;

	/* get position of the target */
	FIG_search_obj_on_cb(host_readbs(hero + 0x86), &target_x, &target_y);

	/* get position of the hero */
	FIG_search_obj_on_cb(hero_pos + 1, &hero_x, &hero_y);

	/* check that the range attack is in the same line */
	if ((hero_x != target_x) && (hero_y != target_y)) {
		GUI_output(get_dtp(0xb4));
		retval = 0;
	}

	if (calc_beeline(hero_x, hero_y, target_x, target_y) != 1) {

		/* exchange coordinates if needed */
		if (hero_x > target_x) {
			i = hero_x;
			hero_x = target_x;
			target_x = i;
		}

		/* exchange coordinates if needed */
		if (hero_y > target_y) {
			i = hero_y;
			hero_y = target_y;
			target_y = i;
		}

		if (hero_x != target_x) {

			for (i = hero_x; i < target_x; i++) {

				if (!seg034_000(hero_x, hero_y, i, hero_y, 1, 0, 99)) {
					GUI_output(get_dtp(0xb4));
					retval = 0;
					break;
				}
			}
		} else {
			for (i = hero_y; i < target_y; i++) {

				if (!seg034_000(hero_x, hero_y, hero_x, i, 0, 1, 99)) {
					GUI_output(get_dtp(0xb4));
					retval = 0;
					break;
				}
			}
		}
	} else {

		if (host_readbs(hero + 0x84) == 15) {
			GUI_output(get_ltx(0x7f0));
			retval = 0;
		}
	}

	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
