/**
 *	Rewrite of DSA1 v3.02_de functions of seg110 (travel events 2 / 10)
 *	Functions rewritten: 1/35
 */

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg097.h"
#include "seg103.h"
#include "seg110.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
/* Borlandified and identical */
void tevent_011(void)
{
	signed short answer;
	Bit8u *hero;

	if ((test_skill((hero = Real2Host(get_first_hero_available_in_group())), 51, 8) > 0 && !ds_readb(0x3da6)) ||
		ds_readb(0x3da6) != 0)
	{
		ds_writeb(0x3da6, 1);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0x60),
			(char*)hero + HERO_NAME2,
			(char*)hero + HERO_NAME2);

		do {
			answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
						get_city(0x64),
						get_city(0x68));
		} while (answer == -1);

		if (answer == 2)
		{
			do {
				answer = GUI_radio(get_city(0x6c), 3,
							get_city(0x70),
							get_city(0x74),
							get_city(0x78));
			} while (answer == -1);

			if (answer == 1)
			{
				func1(0, 5);
				func2();
			} else if (answer == 2) {
				timewarp(HOURS(1));
				func1(0, 0);
				func2();
			}
		}
	}
}

/* should be static */
void func1(signed char mod, signed short percent)
{

}

/* should be static */
void func2(void)
{

}
#endif

#if !defined(__BORLANDC__)
}
#endif
