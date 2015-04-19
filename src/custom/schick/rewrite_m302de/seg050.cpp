/*
 *	Rewrite of DSA1 v3.02_de functions of seg050 (level up)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg050.cpp
*/
#include <stdio.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg046.h"
#include "seg047.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg120.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct dummy_in {
	signed char first, last;
};

struct dummy {
	struct dummy_in a[9];
};

/**
 * \brief		tries to increase a spell in advanced mode
 * \param	hero	pointer to the hero
 * \param	spell	number of the spell
 */
void inc_spell_advanced(Bit8u *hero, signed short spell)
{
	signed short l_di = 1;
	signed short randval;
	struct dummy a = *(struct dummy*)(p_datseg + 0x6682);

	if ((host_readbs(hero + 0x21) == 7) &&
		(ds_readbs((0x099d + 0) + 10 * spell) == 2))
	{
		/* spell is a warlock spell */
		l_di = 2;
	}

	if ((host_readbs(hero + 0x21) >= 10) &&
		((ds_readbs((0x099d + 0) + 10 * spell) == 3) ||
			(ds_readbs((0x099d + 0) + 10 * spell) == 5) ||
			(ds_readbs((0x099d + 0) + 10 * spell) == 4)))
	{
		/* elven spell */
		l_di = 2;
	}

	if ((host_readbs(hero + 0x21) == 8) &&
		(ds_readbs((0x099d + 0) + 10 * spell) == 0))
	{
		/* spell is a druid spell */
		l_di = 2;
	}

	if (host_readbs(hero + 0x21) == 9) {

		/* mages */
		if (ds_readbs((0x099d + 0) + 10 * spell) == 1) {
			/* spell is a mage spell */
			l_di = 2;
		}

		if ((a.a[host_readbs(hero + 0x194)].first <= spell) &&
			(a.a[host_readbs(hero + 0x194)].last >= spell)) {
			l_di = 2;
		}

		if (is_in_word_array(spell, (signed short*)Real2Host(ds_readd(0x0d97 + 4 * host_readbs(hero + 0x194)))))
		{
			l_di = 3;
		}
	}

	if (host_readbs(Real2Host(ds_readd(0xe3b2)) + 2 * spell + 1) >= l_di) {

		/* no increase is possible */

		GUI_output(get_city(0xac));

	} else if (host_readbs(Real2Host(ds_readd(0xe3b2)) + 2 * spell) == 3) {

		/* used up legal increase */

		GUI_output(get_city(0xb0));
	} else {
		/* try to increase */

		dec_ptr_bs(hero + 0x193);

		if (host_readbs(hero + 0x13d + spell) >= 11) {
			randval = random_interval(3, 18);
		} else {
			randval = random_interval(2, 12);
		}

		if (host_readbs(hero + 0x13d + spell) < randval) {

			/* success */

			GUI_output(get_city(0x94));

			/* increment spell value */
			inc_ptr_bs(hero + spell + 0x13d);

			/* set the try counter to 0 */
			host_writebs(Real2Host(ds_readd(0xe3b2)) + 2 * spell, 0);
			/* increment the increase counter */
			inc_ptr_bs(Real2Host(ds_readd(0xe3b2)) + 2 * spell + 1);
		} else {
			/* fail */
			GUI_output(get_ltx(0x548));

			/* increment the try counter */
			inc_ptr_bs(Real2Host(ds_readd(0xe3b2)) + 2 * spell);
		}
	}
}

/**
 * \brief		tries to increase a skill in advanced mode
 * \param	hero	pointer to the hero
 * \param	skill	number of the skill
 */
void inc_skill_advanced(Bit8u *hero, signed short skill)
{
	signed short randval;
	signed short max_incs;

	max_incs = ds_readbs(0x0ffe + 4 * skill + 3);


	if (host_readbs(Real2Host(ds_readd(0xe3b6)) + 2 * skill + 1) >= max_incs) {

		/* no increase is possible */

		GUI_output(get_city(0xac));

	} else if (host_readbs(Real2Host(ds_readd(0xe3b6)) + 2 * skill) == 3) {

		/* used up legal increase */

		GUI_output(get_city(0xb0));
	} else {
		/* try to increase */

		dec_ptr_bs(hero + 0x13c);

		if (host_readbs(hero + 0x108 + skill) >= 11) {
			randval = random_interval(3, 18);
		} else {
			randval = random_interval(2, 12);
		}

		if (host_readbs(hero + 0x108 + skill) < randval) {

			/* success */

			GUI_output(get_city(0x94));

			/* increment spell value */
			inc_ptr_bs(hero + skill + 0x108);

			/* set the try counter to 0 */
			host_writebs(Real2Host(ds_readd(0xe3b6)) + 2 * skill, 0);
			/* increment the increase counter */
			inc_ptr_bs(Real2Host(ds_readd(0xe3b6)) + 2 * skill + 1);

			if (skill <= 6) {
				/* increment a melee weapon skill */

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ltx(0x6a8), get_ltx(4 * (48 + skill)));

				randval = -1;

				/* AT - value */
				sprintf((char*)Real2Host(ds_readd(0xd2eb)),
					(char*)get_ltx(0x6ac), host_readbs(hero + skill + 0x68));

				/* PA - value */
				sprintf((char*)Real2Host(ds_readd(0xd2eb)) + 50,
					(char*)get_ltx(0x6b0), host_readbs(hero + skill + 0x6f));

				do {
					randval = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
								Real2Host(ds_readd(0xd2eb)),
								Real2Host(ds_readd(0xd2eb)) + 50);
				} while (randval == -1);

				if (randval == 1) {
					inc_ptr_bs(hero + 0x68 + skill);
				} else {
					inc_ptr_bs(hero + 0x6f + skill);
				}
			}

		} else {
			/* fail */
			GUI_output(get_ltx(0x548));

			/* increment the try counter */
			inc_ptr_bs(Real2Host(ds_readd(0xe3b6)) + 2 * skill);
		}
	}
}

/**
 * \brief		tries to increase a skill in novice mode
 * \param	hero	pointer to the hero
 * \param	skill	number of the skill
 *
 * This function is quiet and does not check how many times
 *	a skill can be increased. So the correct rules come
 *	from the array which contain the skills.
 */
void inc_skill_novice(Bit8u *hero, signed short skill)
{
	signed short done;
	signed short randval;

	done = 0;

	while (!done) {

		/* leave the loop if 3 incs failes or the skill value is 18 */
		if ((host_readbs(Real2Host(ds_readd(0xe3b6)) + skill * 2) == 3) ||
			(host_readbs(hero + 0x108 + skill) == 18)) {
			done = 1;
#if !defined(__BORLANDC__)
			D1_INFO("%s hat alle Versuche aufgebraucht\n", hero + 0x10);
#endif
		} else {

			/* dec available skill incs */
			dec_ptr_bs(hero + 0x13c);

			/* check if available skill incs are 0 */
			if (!host_readbs(hero + 0x13c))
				done = 1;

			/* on a  skill value < 11 use 2W6 else 3W6 */
			if (host_readbs(hero + 0x108 + skill) >= 11) {
				randval = random_interval(3, 18);
			} else {
				randval = random_interval(2, 12);
			}

			/* check if increase success */
			if (host_readbs(hero + 0x108 + skill) < randval) {

				/* inc skill value */
				inc_ptr_bs(hero + 0x108 + skill);

				/* reset failed counter */
				host_writeb(Real2Host(ds_readd(0xe3b6)) + 2 * skill, 0);

				done = 1;

				/* adjust AT PA values */
				if (skill <= 6) {
					if (host_readbs(hero + 0x68 + skill) > host_readbs(hero + 0x6f + skill)) {
						inc_ptr_bs(hero + 0x6f + skill);
					} else {
						inc_ptr_bs(hero + 0x68 + skill);
					}
				}

			} else {
				/* inc failed counter */
				inc_ptr_bs(Real2Host(ds_readd(0xe3b6)) + 2 * skill);
			}
		}
	}
}

/**
 * \brief		tries to increase a spell in novice mode
 * \param	hero	pointer to the hero
 * \param	spell	number of the spell
 *
 * This function is quiet and does not check how many times
 *	a spell can be increased. So the correct rules come
 *	from the array which contain the spells.
 */
void inc_spell_novice(Bit8u *hero, signed short spell)
{
	signed short done;
	signed short randval;

	done = 0;

	while (!done) {
		/* leave the loop if 3 incs failes or the spell value is 18 */
		if ((host_readb(Real2Host(ds_readd(0xe3b2)) + 2 * spell) == 3) ||
			(host_readbs(hero + 0x13d + spell) == 18)) {
			done = 1;
#if !defined(__BORLANDC__)
			D1_INFO("%s hat alle Versuche aufgebraucht\n", hero + 0x10);
#endif
		} else {

			/* dec available spell incs */
			dec_ptr_bs(hero + 0x193);

			/* check if available spell incs are 0 */
			if (!host_readbs(hero + 0x193))
				done = 1;

			/* on a  spell value < 11 use 2W6 else 3W6 */
			if (host_readbs(hero + 0x13d + spell) >= 11)
				randval = random_interval(3, 18);
			else
				randval = random_interval(2, 12);

			/* check if increase success */
			if (host_readbs(hero + 0x13d + spell) < randval) {

				/* inc spell value */
				inc_ptr_bs(hero + 0x13d + spell);

				/* reset failed counter */
				host_writeb(Real2Host(ds_readd(0xe3b2)) + 2 * spell, 0);

				done = 1;

			} else {
				/* inc failed oounter */
				inc_ptr_bs(Real2Host(ds_readd(0xe3b2)) + 2 * spell);
			}
		}
	}
}

/*
 * \brief		brings your hero to the next level
 * \param hero_pos	the position of the hero
 */
void level_up(signed short hero_pos)
{
	register signed short l_si;
	register signed short l_di;
	signed char mr;
	char v2;
	char v3;
	volatile signed short i;
	Bit8u *hero;
	volatile signed short city_bak;

	hero = get_hero(hero_pos);

	if (ds_readb(0x4475) != 0) {
		refresh_colors();
	}

	ds_writew(TIMERS_DISABLED, 1);

	city_bak = ds_readws(0x26bd);

	load_city_ltx(221);

	ds_writed(0xe3b6, (Bit32u)((RealPt)ds_readd(0xc3a9) + 4500));
	ds_writed(0xe3b2, (Bit32u)((RealPt)ds_readd(0xe3b6) + 208));
	ds_writed(0xe3ba, ds_readd(0xc3db));

	l_si = load_archive_file(219);

	read_archive_file(l_si, Real2Host(ds_readd(0xe3ba)), 1300);

	bc_close(l_si);


	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_ltx(0x66c),
		(char*)hero + 0x10);

	GUI_output(Real2Host(ds_readd(DTP2)));

	ds_writew(ACTION, 0);
	ds_writew(0x2c9b, 1);

	for (i = 0; i < 86; i++) {
		host_writeb(Real2Host(ds_readd(0xe3b2)) + 2 * i,
			host_writebs(Real2Host(ds_readd(0xe3b2)) + 2 * i + 1, 0));
	}

	for (i = 0; i < 52; i++) {
		host_writeb(Real2Host(ds_readd(0xe3b6)) + 2 * i,
			host_writebs(Real2Host(ds_readd(0xe3b6)) + 2 * i + 1, 0));
	}

	load_ggsts_nvf();

	/* increment level */
	inc_ptr_bs(hero + 0x27);

	ds_writew(0x2c9d, hero_pos);

	status_show(hero_pos);

	ds_writebs(0x2c9f, ds_writebs(0x2ca0, -1));

	update_status_bars();

	l_si = -1;

	for (i = 0; i <= 6; i++) {
		if (host_readbs(hero + 0x34 + 3 * i) < 20) {
			l_si = 0;
		}
	}

	if (l_si != -1) {

		l_si = -2;


		do {

			ds_writew(0x2ca2, -30);

			l_si = GUI_radio(get_city(0x8c), 7,
						get_ltx(0x670),
						get_ltx(0x674),
						get_ltx(0x678),
						get_ltx(0x67c),
						get_ltx(0x680),
						get_ltx(0x684),
						get_ltx(0x688)) - 1;

			if (host_readbs(hero + 0x34 + 3 * l_si) >= 20) {
				l_si = -2;
				GUI_output(get_city(0xac));
			}

		} while (l_si == -2);

		inc_ptr_bs(hero + 0x34 + 0 + 3 * l_si);
		inc_ptr_bs(hero + 0x34 + 1 + 3 * l_si);

		status_show(hero_pos);

		ds_writebs(0x2c9f, ds_writebs(0x2ca0, -1));

		update_status_bars();
	}

	l_si = -1;

	for (i = 7; i <= 13; i++) {
		if (host_readbs(hero + 0x34 + 3 * i) > 2) {
			l_si = 0;
		}
	}

	if (l_si != -1) {

		l_si = -2;

		do {

			l_si = GUI_radio(get_city(0x90), 7,
						get_ltx(0x68c),
						get_ltx(0x690),
						get_ltx(0x694),
						get_ltx(0x698),
						get_ltx(0x69c),
						get_ltx(0x6a0),
						get_ltx(0x6a4)) - 1;

			if (host_readbs(hero + 0x34 + 3 * (l_si + 7)) <= 2) {
				l_si = -2;
				GUI_output(get_city(0xc0));
			}

		} while (l_si == -2);

		v3 = 0;

		for (i = 0; i < 3; i++) {

			if (random_schick(20) <= host_readbs(hero + 0x34 + 3 * (l_si + 7))) {
				v3 = 1;
			}
		}

		if (v3 != 0) {

			dec_ptr_bs(hero + 0x34 + 0 + 3 * (l_si + 7));
			dec_ptr_bs(hero + 0x34 + 1 + 3 * (l_si + 7));

			GUI_output(get_city(0x94));

			status_show(hero_pos);

			ds_writebs(0x2c9f, ds_writebs(0x2ca0, -1));

			update_status_bars();
		} else {

			GUI_output(get_ltx(0x548));
		}
	}

	/* check changes in MR */

	mr = (host_readbs(hero + 0x37) + host_readbs(hero + 0x34) + host_readbs(hero + 0x27)) / 3 - 2 * host_readbs(hero + 0x49);
	mr += ds_readbs(MR_MODIFICATORS + host_readbs(hero + 0x21));

	if (host_readbs(hero + 0x66) != mr) {
		/* set new basic MR */
		host_writebs(hero + 0x66, mr);

		/* check for STIRNREIF / CORONET equipped => MR+2 */
		if (host_readws(hero + 0x196) == 217) {
			add_ptr_bs(hero + 0x66, 2);
		}
		/* check for RING / RING equipped => MR+2 */
		if (host_readws(hero + 0x196 + 4 * SIZEOF_KS_ITEM) == 165) {
			add_ptr_bs(hero + 0x66, 2);
		}
		/* check for AMULETT / in inventory => MR+5 */
		if (get_item_pos(hero, 163) != -1) {
			add_ptr_bs(hero + 0x66, 5);
		}

		/* show the user the new MR value */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0xa4),
			host_readbs(hero + 0x66));

		GUI_output(Real2Host(ds_readd(DTP2)));

		/* update status background */
		status_show(hero_pos);

		/* update_the bars of the hero */
		ds_writebs(0x2c9f, ds_writebs(0x2ca0, -1));

		update_status_bars();
	}


	/* add skill increasements */
	add_ptr_bs(hero + 0x13c, ds_readbs(0x2d0d + 1 + host_readbs(hero + 0x21)));

	/* roll how many LE points the hero may get */
	i = random_schick(6);

	if (host_readbs(hero + 0x21) >= 7) {
		/* a magic user */

		/* add spell increasements */
		add_ptr_bs(hero + 0x193, ds_readbs(0x2d14 + host_readbs(hero + 0x21)));

		i += 2;

		/* show how many LE the hero may get */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0x9c),
			i);

		do {
			l_si = GUI_input(Real2Host(ds_readd(DTP2)), 1);
		} while (l_si < 0);

		if (l_si > i) {
			l_si = i;
		}


		/* add LE and fill them up */
		add_ptr_ws(hero + 0x5e, l_si);
		add_hero_le(hero, l_si);
		/* add AE and fill them up */
		add_ptr_ws(hero + 0x62, i - l_si);
		add_hero_ae(hero, i - l_si);

		/* change skill increasements into AE */
		if (host_readbs(hero + 0x21) == 9 && ds_readws(GAME_MODE) == 2) {

			if (GUI_bool(get_city(0xa0))) {
				/* trade 10 skill increasements into 1W6+2 AE */
				add_ptr_bs(hero + 0x193, -10);
				i = random_interval(3, 8);
				add_ptr_ws(hero + 0x62, i);
				add_ptr_ws(hero + 0x64, i);
			}
		}

	} else {
		/* not a magic user */

		/* show how many LE the hero gets */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0x98),
			i);

		GUI_output(Real2Host(ds_readd(DTP2)));

		/* add LE and fill them up */
		add_ptr_ws(hero + 0x5e, i);
		add_hero_le(hero, i);
	}

	status_show(hero_pos);

	/* update_the bars of the hero */
	ds_writebs(0x2c9f, ds_writebs(0x2ca0, -1));

	update_status_bars();

	update_atpa(hero);

	if (ds_readws(GAME_MODE) == 1) {

		i = v2 = 0;

		while (host_readbs(hero + 0x13c) > 0) {

			l_si = host_readws(Real2Host(ds_readd(0xe3ba)) + 100 * host_readbs(hero + 0x21) + 4 * i);

			if (host_readbs(hero + 0x108 + l_si) < host_readws(Real2Host(ds_readd(0xe3ba)) + 100 * host_readbs(hero + 0x21) + 4 * i + 2))
			{
				inc_skill_novice(hero, l_si);
			}

			i++;

			if (i > 24) {

				i = 0;

				v2++;

				if (v2 > 5) {
					host_writeb(hero + 0x13c, 0);
				}

			}
		}

		if (host_readbs(hero + 0x21) >= 7) {

			i = 1;
			v2 = 0;

			while (host_readbs(hero + 0x193) != 0 && v2 < 3) {

				v2++;

				switch (host_readbs(hero + 0x21)) {

					case 7: {

						while (host_readbs(hero + 0x193) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 2 && host_readbs(hero + 0x13d + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + 0x193) != 0 && ds_readws(0x6544 + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(0x6544 + 2 * i));
							i++;
						}

						break;
					}
					case 8: {

						while (host_readbs(hero + 0x193) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 0 && host_readbs(hero + 0x13d + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + 0x193) != 0 && ds_readws(0x654a + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(0x654a + 2 * i));
							i++;
						}

						break;
					}
					case 9: {

						i = 0;

						while (host_readbs(hero + 0x193) != 0 &&
							(host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + 0x194)))) + 2 * i)) != -1) {

							if (host_readbs(hero + 0x13d +
									host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + 0x194)))) + 2 * i)) < 11)
							{
								inc_spell_novice(hero,
									host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + 0x194)))) + 2 * i));
							}
							i++;
						}

						i = 0;
						while (host_readbs(hero + 0x193) != 0 &&
							(host_readws(Real2Host((ds_readd(0x662a + 4 * host_readbs(hero + 0x194)))) + 2 * i)) != -1) {

							if (host_readbs(hero + 0x13d +
									host_readws(Real2Host((ds_readd(0x662a + 4 * host_readbs(hero + 0x194)))) + 2 * i)) < 11)
							{
								inc_spell_novice(hero,
									host_readws(Real2Host((ds_readd(0x662a + 4 * host_readbs(hero + 0x194)))) + 2 * i));
							}
							i++;
						}

						i = 0;
						while (host_readbs(hero + 0x193) != 0 &&
							(host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + 0x194)))) + 2 * i)) != -1) {

							if (host_readbs(hero + 0x13d +
									host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + 0x194)))) + 2 * i)) < 11)
							{
								inc_spell_novice(hero,
									host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + 0x194)))) + 2 * i));
							}
							i++;
						}

						break;
					}
					case 10: {

						while (host_readbs(hero + 0x193) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 3 && host_readbs(hero + 0x13d + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + 0x193) != 0 && ds_readws(0x664e + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(0x664e + 2 * i));
							i++;
						}

						i = 1;
						while (host_readbs(hero + 0x193) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 3 && host_readbs(hero + 0x13d + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						break;
					}
					case 11: {

						while (host_readbs(hero + 0x193) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 4 && host_readbs(hero + 0x13d + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + 0x193) != 0 && ds_readws(0x666a + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(0x666a + 2 * i));
							i++;
						}

						i = 1;
						while (host_readbs(hero + 0x193) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 4 && host_readbs(hero + 0x13d + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}
						break;
					}
					case 12: {

						while (host_readbs(hero + 0x193) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 5 && host_readbs(hero + 0x13d + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + 0x193) != 0 && ds_readws(0x665c + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(0x665c + 2 * i));
							i++;
						}

						i = 1;
						while (host_readbs(hero + 0x193) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 5 && host_readbs(hero + 0x13d + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}
						break;
					}
				}
			}

			while (host_readbs(hero + 0x193) != 0) {
				inc_spell_novice(hero, random_schick(86));
			}
		}

	} else {

		if ((host_readbs(hero + 0x21) >= 7) &&
			(l_di = ds_readbs(0x2d1a + host_readbs(hero + 0x21))) &&
			GUI_bool(get_city(0xb4)))
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0xb8), l_di);

			i = GUI_input(Real2Host(ds_readd(DTP2)), 1);

			if (i > 0) {

				if (i > l_di) {
					i = l_di;
				}
				l_di -= i;

				sub_ptr_bs(hero + 0x193, i);
				add_ptr_bs(hero + 0x13c, i);

			} else {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0xbc), l_di);

				i = GUI_input(Real2Host(ds_readd(DTP2)), 1);

				if (i > 0) {

					if (i > l_di) {
						i = l_di;
					}

					add_ptr_bs(hero + 0x193, i);
					sub_ptr_bs(hero + 0x13c, i);
				}
			}
		}

		while (host_readbs(hero + 0x13c) != 0) {

			l_si = LVL_select_talent(hero, 1);

			if (l_si >= 0) {

				inc_skill_advanced(hero, l_si);

			} else if (l_si == -2) {

				if (GUI_bool(get_city(0xa8))) {
					break;
				}
			}
		}

		while (host_readbs(hero + 0x193) != 0) {

			l_si = select_spell(hero, 1);

			if (l_si > 0) {

				inc_spell_advanced(hero, l_si);

			} else if (l_si == -2) {

				if (GUI_bool(get_city(0xa8))) {
					break;
				}
			}
		}
	}

	ds_writew(0x2ca2, 0);
	ds_writew(0x2846, 1);
	ds_writew(TIMERS_DISABLED, 0);

	ds_writews(CURRENT_ANI, -1);

	if (city_bak != -1 && city_bak != 221 && city_bak != 130 && (city_bak < 156 || city_bak > 176)) {
		load_city_ltx(city_bak);
	}
}

#if !defined(__BORLANDC__)
}
#endif
