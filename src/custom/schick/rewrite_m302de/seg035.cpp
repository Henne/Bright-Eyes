/**
 *	Rewrite of DSA1 v3.02_de functions of seg035 (fightsystem)
 *	Functions rewritten: 2/4
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg032.h"
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

#if !defined(__BORLANDC__)
}
#endif
