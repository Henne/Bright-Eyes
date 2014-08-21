/*
 *      Rewrite of DSA1 v3.02_de functions of seg103 (talents)
 *      Functions rewritten 5/8
 *
*/

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct dummy {
	char a[6];
};

/* Borlandified and identical */
signed short LVL_select_talent(Bit8u *hero, signed short show_values)
{
	signed short i;

	signed short answer;
	signed short l1;
	signed short retval = -1;
	/* string on stack "%s~%d" */
	dummy format_str = *(struct dummy*)(p_datseg + 0xacce);

	if (show_values != 0) {

		strcpy((char*)Real2Host(ds_readd(0xd2eb)), (char*)get_ltx(0x334));

		if (host_readbs(hero + 0x13c) > 1) {
			strcat((char*)Real2Host(ds_readd(0xd2eb)), (char*)get_ltx(0x624));
		}

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0x330),
			/* sind / ist */
			(host_readbs(hero + 0x13c) > 1) ? get_ltx(0x4c4) : get_ltx(0x4c0),
			/* # of tries left */
			host_readbs(hero + 0x13c),
			Real2Host(ds_readd(0xd2eb)));
	} else {

		strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x360));
	}

	/* ask for the talent category */
	answer = GUI_radio(Real2Host(ds_readd(DTP2)), 7,
				get_ltx(0x190), get_ltx(0x194),
				get_ltx(0x198), get_ltx(0x1a4),
				get_ltx(0x19c), get_ltx(0x1a0),
				get_ltx(0x1a8)) - 1;

	if (answer != -2) {

		l1 = ds_readbs(0x10ce + 2 * answer);

		if (show_values != 0) {

			for (i = 0; ds_readbs(0x10cf + 2 * answer) > i; i++) {

				sprintf((char*)Real2Host(ds_readd(DTP2)) + 50 * i,
					format_str.a,
					get_ltx((l1 + i + 48) * 4),
					host_readbs(hero + l1 + i + 0x108));

				ds_writed(0xbf95 + 4 * i, (Bit32u)((RealPt)ds_readd(DTP2) + 50 * i));
			}
		} else {

			for (i = 0; ds_readbs(0x10cf + 2 * answer) > i; i++) {
				ds_writed(0xbf95 + 4 * i, (Bit32u)(host_readd(Real2Host(ds_readd(TEXT_LTX)) + (l1 + i + 48) * 4)));
			}
		}

		retval = GUI_radio(get_ltx(0x368), ds_readbs(0x10cf + 2 * answer),
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
				Real2Host(ds_readd(0xbfc9)),
				Real2Host(ds_readd(0xbfcd)),
				Real2Host(ds_readd(0xbfd1)));

		if (retval != -1) {
			retval += l1 - 1;
		}
	} else {
		retval = -2;
	}

	return retval;
}

/**
	get_proper_hero - returns hero which seems best for a skill
	@skill:		skill
*/
/* Borlandified and identical */
RealPt get_proper_hero(signed short skill)
{
	signed short i;
	signed short cur;

	signed short max = -1;
	RealPt hero_i;
	RealPt retval;

#if !defined(__BORLANDC__)
	retval = 0;
#endif

	hero_i = (RealPt)ds_readd(HEROS);

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* Check class */
		if ((host_readbs(Real2Host(hero_i) + 0x21) != 0) &&
			/* Check if in current group */
			(host_readb(Real2Host(hero_i) + 0x87) == ds_readb(CURRENT_GROUP)) &&
			/* Check hero is not dead */
			!hero_dead(Real2Host(hero_i))) {

			/* add current and maximum attibute values */
			cur =	host_readbs(Real2Host(hero_i) + 0x35 + 3 * ds_readbs(0xffe + 4 * skill)) +
				host_readbs(Real2Host(hero_i) + 0x36 + 3 * ds_readbs(0xffe + 4 * skill)) +
				host_readbs(Real2Host(hero_i) + 0x35 + 3 * ds_readbs(0xfff + 4 * skill)) +
				host_readbs(Real2Host(hero_i) + 0x36 + 3 * ds_readbs(0xfff + 4 * skill)) +
				host_readbs(Real2Host(hero_i) + 0x35 + 3 * ds_readbs(0x1000 + 4 * skill)) +
				host_readbs(Real2Host(hero_i) + 0x36 + 3 * ds_readbs(0x1000 + 4 * skill)) + +
				host_readbs(Real2Host(hero_i) + 0x108 + skill);

			if (cur > max) {

				/* if the sum is greater than the current best */
				max = cur;
				retval = hero_i;
			}
		}
	}

#if !defined(__BORLANDC__)
	/* sanity check for Original Bug hunting */
	if (retval == 0)
		D1_ERR("Original-Bug: %s undefinierter Rückgabewert\n",
			__func__);
#endif

	return retval;
}

/**
	test_skill - performs a skill test
	@hero:	hero which should be tested
	@skill:	the skill to test
	@bonus: the modification
*/
/* Borlandified and identical */
signed short test_skill(Bit8u *hero, signed short skill, signed char bonus)
{
	signed short randval;
	signed short e_skillval;

	/* dont test for weapon skills */
	if ((skill >= 7) && (skill <= 51)) {

#if !defined(__BORLANDC__)
		D1_INFO("Talentprobe %s %+d: ", names_skill[skill], bonus);
#endif

		/* special test if skill is a range weapon skill */
		if ((skill == 7) || (skill == 8)) {

			/* calculate range weapon base value */
			e_skillval = (host_readbs(hero + 0x38) + host_readbs(hero + 0x39) +
				host_readbs(hero + 0x41) + host_readbs(hero + 0x42) +
				host_readbs(hero + 0x47) + host_readbs(hero + 0x48)) / 4;

			/* add skill value */
			e_skillval += host_readbs(hero + 0x108 + skill);
			/* sub handycap */
			e_skillval -= bonus;

			randval = random_schick(20);

			/* Unlucky */
			if (randval == 20) {
#if !defined(__BORLANDC__)
				D1_INFO("Ungluecklich\n");
#endif
				return -1;
			}
			/* Lucky */
			if (randval == 1) {
#if !defined(__BORLANDC__)
				D1_INFO("Gluecklich\n");
#endif
				return 99;
			}
			if (randval <= e_skillval) {
				/* test successful */
#if !defined(__BORLANDC__)
				D1_INFO(" (%d) -> bestanden\n", randval);
#endif
				return e_skillval - randval + 1;
			}

			/* test unsuccessful */
#if !defined(__BORLANDC__)
			D1_INFO(" (%d) -> nicht bestanden\n", randval);
#endif
			return 0;
		}

		/* automatically get hero with best senses in beginner mode */
		if ((skill == 51) && (ds_readws(0xc003) == 1)) {
			hero = Real2Host(get_proper_hero(51));

#if defined(__BORLANDC__)
			/* seems to have been debug stuff with conditional compilation */
			if (0) return 0;
#endif
		}

		/* do the test */
		bonus -= host_readbs(hero + 0x108 + skill);

		return test_attrib3(hero, ds_readbs(0xffe + skill * 4), ds_readbs(0xfff + skill * 4), ds_readbs(0x1000 + skill * 4), bonus);

	}

	return 0;
}

struct dummy2 {
	signed char a[6];
};

/* Borlandified and identical */
signed short select_talent(void)
{
	signed short l_si = -1;
	signed short nr_talents = 3;
	/* available skills {44, 45, 46, -1, -1, -1} */
	dummy2 a = *(dummy2*)(p_datseg + 0xacd4);

	/* add skills for special location */
	/* 9 = ACROBATICS, 32 = ALCHEMY, 43 = CHEAT, 47 = INSTRUMENT, 49 = PICKPOCKET, */
	if (ds_readbs(LOCATION) == 3) {
		/* TAVERN */
		a.a[nr_talents] = 9;
		nr_talents++;

		if (ds_readws(0x6532) == 0) {
			a.a[nr_talents] = 43;
			nr_talents++;
		}

		a.a[nr_talents] = 47;
		nr_talents++;
	} else if ((ds_readbs(LOCATION) == 6) || (ds_readbs(LOCATION) == 7)) {
		/* CAMP (Wildernes) or INN */
		a.a[nr_talents] = 32;
		nr_talents++;
	} else if (ds_readbs(LOCATION) == 9) {
		/* MARKET */
		a.a[nr_talents] = 9;
		nr_talents++;
		a.a[nr_talents] = 49;
		nr_talents++;
	} else if (ds_readbs(LOCATION) == 5) {
		/* MERCHANT */
		a.a[nr_talents] = 49;
		nr_talents++;
	}

	l_si = GUI_radio(get_ltx(0x368), nr_talents,
				get_ltx((a.a[0] + 48) * 4),
				get_ltx((a.a[1] + 48) * 4),
				get_ltx((a.a[2] + 48) * 4),
				get_ltx((a.a[3] + 48) * 4),
				get_ltx((a.a[4] + 48) * 4),
				get_ltx((a.a[5] + 48) * 4),
				get_ltx((a.a[6] + 48) * 4));

	if (l_si != -1) {
		return a.a[l_si - 1];
	}

	return l_si;
}

/**
 *	bargain -	does a bargain throw
 *	@hero:		the hero who bargain
 *	@items:		the number of different goods
 *	@price:		the total price
 *	@percent:	how many percent the player wants to get
 *	@mod_init:	initial value for the modificator
 *
 *	Returns the result of the throw. A value greater than zero
 *	means success, below or zero means failed.
 */
signed short bargain(Bit8u *hero, unsigned short items, signed int price,
	signed short percent, signed char mod_init) {

	signed char mod;

	mod = mod_init;

	/* maybe a special NPC ? */
	if (host_readb(get_hero(6) + 0x89) == 2)
		mod -= 2;

	/* the more different items you buy, the easier the bargain */
	if (items == 1)
		mod += 2;
	else if (items == 2)
		mod += 1;
	else if (items < 5)
		mod += 0;
	else if (items < 9)
		mod -= 1;
	else mod -= 2;

	/* the higher the price, the easier the bargain */
	if (price < 100)
		mod += 2;
	if (price < 500)
		mod += 1;
	if (price < 1000)
		mod += 0;
	if (price < 2000)
		mod -= 1;
	else
		mod -= 2;

	/* the lower the percent, the easier the bargain */
	mod += percent / 5 + 1;

	return test_skill(hero, 0x15, mod);
}

#if !defined(__BORLANDC__)
}
#endif
