/**
 *	Rewrite of DSA1 v3.02_de functions of seg035 (fightsystem)
 *	Functions rewritten: 1/4
 */

#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg032.h"

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

#if !defined(__BORLANDC__)
}
#endif
