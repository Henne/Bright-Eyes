/*
	Rewrite of DSA1 v3.02_de functions of seg050 (level up)
	Functions rewritten: 1/5
*/

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#include "v302de.h"

#include "seg007.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * inc_skill_novice() - tries to increace a skill in novice mode
 * @hero:	pointer to the hero
 * @skill:	number of the skill
 *
 * Remarks:	This function is quiet and does not check how many times
 *		a skill can be increased. So the correct rules come
 *		from the array which contain the skills.
 */
void inc_skill_novice(Bit8u *hero, unsigned short skill)
{
	unsigned short done;
	unsigned short randval;

	done = 0;

	while (!done) {
		/* leave the loop if 3 incs failes or the skill value is 18 */
		if (host_readb(Real2Host(ds_readd(0xe3b6)) + skill * 2) == 3 ||
			host_readb(hero + 0x108 + skill) == 18) {
			done = 1;
#if !defined(__BORLANDC__)
			D1_INFO("%s hatt alle Versuche aufgebraucht\n", hero + 0x10);
#endif
			continue;
		}

		/* dec available skill incs */
		host_writeb(hero + 0x13c, host_readb(hero + 0x13c) - 1);

		/* check if available skill incs are 0 */
		if (!(signed char)host_readb(hero + 0x13c))
			done = 1;

		/* on a  skill value < 11 use 2W6 else 3W6 */
		if ((signed char)host_readb(hero + 0x108 + skill) >= 11)
			randval = random_interval(3, 18);
		else
			randval = random_interval(2, 12);

		/* check if increase success */
		if ((signed char)host_readb(hero + 0x108 + skill) < randval) {
			/* inc skill value */
			host_writeb(hero + 0x108 + skill,
				host_readb(hero + 0x108 + skill) + 1);

			/* reset failed counter */
			host_writeb(Real2Host(ds_readd(0xe3b6) + skill * 2), 0);

			done = 1;

			/* adjust AT PA values */
			if (skill <= 6) {
				if (host_readb(hero + 0x68 + skill) > host_readb(hero + 0x6f + skill)) {
					host_writeb(hero + 0x6f + skill,
						host_readb(hero + 0x6f + skill) + 1);
				} else {
					host_writeb(hero + 0x68 + skill,
						host_readb(hero + 0x68 + skill) + 1);
				}
			}

		} else {
			/* inc failed oounter */
			host_writeb(Real2Host(ds_readd(0xe3b6)) + skill * 2,
				host_readb(Real2Host(ds_readd(0xe3b6)) + skill * 2) + 1);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
