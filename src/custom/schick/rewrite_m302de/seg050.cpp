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

	if ((host_readbs(hero + HERO_TYPE) == 7) &&
		(ds_readbs((0x099d + 0) + 10 * spell) == 2))
	{
		/* spell is a warlock spell */
		l_di = 2;
	}

	if ((host_readbs(hero + HERO_TYPE) >= 10) &&
		((ds_readbs((0x099d + 0) + 10 * spell) == 3) ||
			(ds_readbs((0x099d + 0) + 10 * spell) == 5) ||
			(ds_readbs((0x099d + 0) + 10 * spell) == 4)))
	{
		/* elven spell */
		l_di = 2;
	}

	if ((host_readbs(hero + HERO_TYPE) == 8) &&
		(ds_readbs((0x099d + 0) + 10 * spell) == 0))
	{
		/* spell is a druid spell */
		l_di = 2;
	}

	if (host_readbs(hero + HERO_TYPE) == 9) {

		/* mages */
		if (ds_readbs((0x099d + 0) + 10 * spell) == 1) {
			/* spell is a mage spell */
			l_di = 2;
		}

		if ((a.a[host_readbs(hero + HERO_MAGIC_SCHOOL)].first <= spell) &&
			(a.a[host_readbs(hero + HERO_MAGIC_SCHOOL)].last >= spell)) {
			l_di = 2;
		}

		if (is_in_word_array(spell, (signed short*)Real2Host(ds_readd(0x0d97 + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))))
		{
			l_di = 3;
		}
	}

	if (host_readbs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell + 1) >= l_di) {

		/* no increase is possible */

		GUI_output(get_city(0xac));

	} else if (host_readbs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell) == 3) {

		/* used up legal increase */

		GUI_output(get_city(0xb0));
	} else {
		/* try to increase */

		dec_ptr_bs(hero + HERO_SP_RISE);

		if (host_readbs(hero + HERO_SPELLS + spell) >= 11) {
			randval = random_interval(3, 18);
		} else {
			randval = random_interval(2, 12);
		}

		if (host_readbs(hero + HERO_SPELLS + spell) < randval) {

			/* success */

			GUI_output(get_city(0x94));

			/* increment spell value */
			inc_ptr_bs(hero + spell + 0x13d);

			/* set the try counter to 0 */
			host_writebs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell, 0);
			/* increment the increase counter */
			inc_ptr_bs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell + 1);
		} else {
			/* fail */
			GUI_output(get_ltx(0x548));

			/* increment the try counter */
			inc_ptr_bs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell);
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

	max_incs = ds_readbs(SKILLS_EXTRA + 4 * skill + 3);


	if (host_readbs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill + 1) >= max_incs) {

		/* no increase is possible */

		GUI_output(get_city(0xac));

	} else if (host_readbs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill) == 3) {

		/* used up legal increase */

		GUI_output(get_city(0xb0));
	} else {
		/* try to increase */

		dec_ptr_bs(hero + HERO_TA_RISE);

		if (host_readbs(hero + HERO_TA_FIGHT + skill) >= 11) {
			randval = random_interval(3, 18);
		} else {
			randval = random_interval(2, 12);
		}

		if (host_readbs(hero + HERO_TA_FIGHT + skill) < randval) {

			/* success */

			GUI_output(get_city(0x94));

			/* increment spell value */
			inc_ptr_bs(hero + skill + 0x108);

			/* set the try counter to 0 */
			host_writebs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill, 0);
			/* increment the increase counter */
			inc_ptr_bs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill + 1);

			if (skill <= 6) {
				/* increment a melee weapon skill */

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ltx(0x6a8), get_ltx(4 * (48 + skill)));

				randval = -1;

				/* AT - value */
				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					(char*)get_ltx(0x6ac), host_readbs(hero + skill + 0x68));

				/* PA - value */
				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 50,
					(char*)get_ltx(0x6b0), host_readbs(hero + skill + 0x6f));

				do {
					randval = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
								Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
								Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 50);
				} while (randval == -1);

				if (randval == 1) {
					inc_ptr_bs(hero + HERO_AT + skill);
				} else {
					inc_ptr_bs(hero + HERO_PA + skill);
				}
			}

		} else {
			/* fail */
			GUI_output(get_ltx(0x548));

			/* increment the try counter */
			inc_ptr_bs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill);
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
		if ((host_readbs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + skill * 2) == 3) ||
			(host_readbs(hero + HERO_TA_FIGHT + skill) == 18)) {
			done = 1;
#if !defined(__BORLANDC__)
			D1_INFO("%s hat alle Versuche aufgebraucht\n", hero + HERO_NAME2);
#endif
		} else {

			/* dec available skill incs */
			dec_ptr_bs(hero + HERO_TA_RISE);

			/* check if available skill incs are 0 */
			if (!host_readbs(hero + HERO_TA_RISE))
				done = 1;

			/* on a  skill value < 11 use 2W6 else 3W6 */
			if (host_readbs(hero + HERO_TA_FIGHT + skill) >= 11) {
				randval = random_interval(3, 18);
			} else {
				randval = random_interval(2, 12);
			}

			/* check if increase success */
			if (host_readbs(hero + HERO_TA_FIGHT + skill) < randval) {

				/* inc skill value */
				inc_ptr_bs(hero + HERO_TA_FIGHT + skill);

				/* reset failed counter */
				host_writeb(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill, 0);

				done = 1;

				/* adjust AT PA values */
				if (skill <= 6) {
					if (host_readbs(hero + HERO_AT + skill) > host_readbs(hero + HERO_PA + skill)) {
						inc_ptr_bs(hero + HERO_PA + skill);
					} else {
						inc_ptr_bs(hero + HERO_AT + skill);
					}
				}

			} else {
				/* inc failed counter */
				inc_ptr_bs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill);
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
		if ((host_readb(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell) == 3) ||
			(host_readbs(hero + HERO_SPELLS + spell) == 18)) {
			done = 1;
#if !defined(__BORLANDC__)
			D1_INFO("%s hat alle Versuche aufgebraucht\n", hero + HERO_NAME2);
#endif
		} else {

			/* dec available spell incs */
			dec_ptr_bs(hero + HERO_SP_RISE);

			/* check if available spell incs are 0 */
			if (!host_readbs(hero + HERO_SP_RISE))
				done = 1;

			/* on a  spell value < 11 use 2W6 else 3W6 */
			if (host_readbs(hero + HERO_SPELLS + spell) >= 11)
				randval = random_interval(3, 18);
			else
				randval = random_interval(2, 12);

			/* check if increase success */
			if (host_readbs(hero + HERO_SPELLS + spell) < randval) {

				/* inc spell value */
				inc_ptr_bs(hero + HERO_SPELLS + spell);

				/* reset failed counter */
				host_writeb(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell, 0);

				done = 1;

			} else {
				/* inc failed oounter */
				inc_ptr_bs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell);
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
	signed short l_si;
	signed short l_di;
	signed char mr;
	signed char v2;
	signed char v3;
	signed short i;
	Bit8u *hero;
	signed short city_bak;

	hero = get_hero(hero_pos);

	if (ds_readb(0x4475) != 0) {
		refresh_colors();
	}

	ds_writew(TIMERS_DISABLED, 1);

	city_bak = ds_readws(TEXT_FILE_INDEX);

	load_city_ltx(ARCHIVE_FILE_CHARTEXT_LTX);

	ds_writed(INC_SKILLS_COUNTER, (Bit32u)((RealPt)ds_readd(BUFFER8_PTR) + 4500));
	ds_writed(INC_SPELLS_COUNTER, (Bit32u)((RealPt)ds_readd(INC_SKILLS_COUNTER) + 208));
	ds_writed(SKILLS_BUFFER, ds_readd(BUFFER9_PTR));

	l_si = load_archive_file(ARCHIVE_FILE_BSKILLS_DAT);

	read_archive_file(l_si, Real2Host(ds_readd(SKILLS_BUFFER)), 1300);

	bc_close(l_si);


	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_ltx(0x66c),
		(char*)hero + HERO_NAME2);

	GUI_output(Real2Host(ds_readd(DTP2)));

	ds_writew(ACTION, 0);
	ds_writew(STATUS_PAGE_MODE, 1);

	for (i = 0; i < 86; i++) {
		host_writeb(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * i,
			host_writebs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * i + 1, 0));
	}

	for (i = 0; i < 52; i++) {
		host_writeb(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * i,
			host_writebs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * i + 1, 0));
	}

	load_ggsts_nvf();

	/* increment level */
	inc_ptr_bs(hero + HERO_LEVEL);

	ds_writew(STATUS_PAGE_HERO, hero_pos);

	status_show(hero_pos);

	ds_writebs(STATUS_PAGE_HUNGER, ds_writebs(STATUS_PAGE_THIRST, -1));

	update_status_bars();

	l_si = -1;

	for (i = 0; i <= 6; i++) {
		if (host_readbs(hero + HERO_MU_ORIG + 3 * i) < 20) {
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

			if (host_readbs(hero + HERO_MU_ORIG + 3 * l_si) >= 20) {
				l_si = -2;
				GUI_output(get_city(0xac));
			}

		} while (l_si == -2);

		inc_ptr_bs(hero + HERO_MU_ORIG + 0 + 3 * l_si);
		inc_ptr_bs(hero + HERO_MU_ORIG + 1 + 3 * l_si);

		status_show(hero_pos);

		ds_writebs(STATUS_PAGE_HUNGER, ds_writebs(STATUS_PAGE_THIRST, -1));

		update_status_bars();
	}

	l_si = -1;

	for (i = 7; i <= 13; i++) {
		if (host_readbs(hero + HERO_MU_ORIG + 3 * i) > 2) {
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

			if (host_readbs(hero + HERO_MU_ORIG + 3 * (l_si + 7)) <= 2) {
				l_si = -2;
				GUI_output(get_city(0xc0));
			}

		} while (l_si == -2);

		v3 = 0;

		for (i = 0; i < 3; i++) {

			if (random_schick(20) <= host_readbs(hero + HERO_MU_ORIG + 3 * (l_si + 7))) {
				v3 = 1;
			}
		}

		if (v3 != 0) {

			dec_ptr_bs(hero + HERO_MU_ORIG + 0 + 3 * (l_si + 7));
			dec_ptr_bs(hero + HERO_MU_ORIG + 1 + 3 * (l_si + 7));

			GUI_output(get_city(0x94));

			status_show(hero_pos);

			ds_writebs(STATUS_PAGE_HUNGER, ds_writebs(STATUS_PAGE_THIRST, -1));

			update_status_bars();
		} else {

			GUI_output(get_ltx(0x548));
		}
	}

	/* check changes in MR */

	mr = (host_readbs(hero + HERO_KL_ORIG) + host_readbs(hero + HERO_MU_ORIG) + host_readbs(hero + HERO_LEVEL)) / 3 - 2 * host_readbs(hero + HERO_AG_ORIG);
	mr += ds_readbs(MR_MODIFICATORS + host_readbs(hero + HERO_TYPE));

	if (host_readbs(hero + HERO_MR) != mr) {
		/* set new basic MR */
		host_writebs(hero + HERO_MR, mr);

		/* check for STIRNREIF / CORONET equipped => MR+2 */
		if (host_readws(hero + HERO_ITEM_HEAD) == 217) {
			add_ptr_bs(hero + HERO_MR, 2);
		}
		/* check for RING / RING equipped => MR+2 */
		if (host_readws(hero + HERO_ITEM_HEAD + 4 * SIZEOF_KS_ITEM) == 165) {
			add_ptr_bs(hero + HERO_MR, 2);
		}
		/* check for AMULETT / in inventory => MR+5 */
		if (get_item_pos(hero, 163) != -1) {
			add_ptr_bs(hero + HERO_MR, 5);
		}

		/* show the user the new MR value */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0xa4),
			host_readbs(hero + HERO_MR));

		GUI_output(Real2Host(ds_readd(DTP2)));

		/* update status background */
		status_show(hero_pos);

		/* update_the bars of the hero */
		ds_writebs(STATUS_PAGE_HUNGER, ds_writebs(STATUS_PAGE_THIRST, -1));

		update_status_bars();
	}


	/* add skill increasements */
	add_ptr_bs(hero + HERO_TA_RISE, ds_readbs(0x2d0d + 1 + host_readbs(hero + HERO_TYPE)));

	/* roll how many LE points the hero may get */
	i = random_schick(6);

	if (host_readbs(hero + HERO_TYPE) >= 7) {
		/* a magic user */

		/* add spell increasements */
		add_ptr_bs(hero + HERO_SP_RISE, ds_readbs(0x2d14 + host_readbs(hero + HERO_TYPE)));

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
		add_ptr_ws(hero + HERO_LE_ORIG, l_si);
		add_hero_le(hero, l_si);
		/* add AE and fill them up */
		add_ptr_ws(hero + HERO_AE_ORIG, i - l_si);
		add_hero_ae(hero, i - l_si);

		/* change skill increasements into AE */
		if (host_readbs(hero + HERO_TYPE) == 9 && ds_readws(GAME_MODE) == 2) {

			if (GUI_bool(get_city(0xa0))) {
				/* trade 10 skill increasements into 1W6+2 AE */
				add_ptr_bs(hero + HERO_SP_RISE, -10);
				i = random_interval(3, 8);
				add_ptr_ws(hero + HERO_AE_ORIG, i);
				add_ptr_ws(hero + HERO_AE, i);
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
		add_ptr_ws(hero + HERO_LE_ORIG, i);
		add_hero_le(hero, i);
	}

	status_show(hero_pos);

	/* update_the bars of the hero */
	ds_writebs(STATUS_PAGE_HUNGER, ds_writebs(STATUS_PAGE_THIRST, -1));

	update_status_bars();

	update_atpa(hero);

	if (ds_readws(GAME_MODE) == 1) {

		i = v2 = 0;

		while (host_readbs(hero + HERO_TA_RISE) > 0) {

			l_si = host_readws(Real2Host(ds_readd(SKILLS_BUFFER)) + 100 * host_readbs(hero + HERO_TYPE) + 4 * i);

			if (host_readbs(hero + HERO_TA_FIGHT + l_si) < host_readws(Real2Host(ds_readd(SKILLS_BUFFER)) + 100 * host_readbs(hero + HERO_TYPE) + 4 * i + 2))
			{
				inc_skill_novice(hero, l_si);
			}

			i++;

			if (i > 24) {

				i = 0;

				v2++;

				if (v2 > 5) {
					host_writeb(hero + HERO_TA_RISE, 0);
				}

			}
		}

		if (host_readbs(hero + HERO_TYPE) >= 7) {

			i = 1;
			v2 = 0;

			while (host_readbs(hero + HERO_SP_RISE) != 0 && v2 < 3) {

				v2++;

				switch (host_readbs(hero + HERO_TYPE)) {

					case 7: {

						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 2 && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 && ds_readws(0x6544 + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(0x6544 + 2 * i));
							i++;
						}

						break;
					}
					case 8: {

						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 0 && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 && ds_readws(0x654a + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(0x654a + 2 * i));
							i++;
						}

						break;
					}
					case 9: {

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 &&
							(host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) != -1) {

							if (host_readbs(hero + HERO_SPELLS +
									host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) < 11)
							{
								inc_spell_novice(hero,
									host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i));
							}
							i++;
						}

						i = 0;
						while (host_readbs(hero + HERO_SP_RISE) != 0 &&
							(host_readws(Real2Host((ds_readd(0x662a + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) != -1) {

							if (host_readbs(hero + HERO_SPELLS +
									host_readws(Real2Host((ds_readd(0x662a + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) < 11)
							{
								inc_spell_novice(hero,
									host_readws(Real2Host((ds_readd(0x662a + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i));
							}
							i++;
						}

						i = 0;
						while (host_readbs(hero + HERO_SP_RISE) != 0 &&
							(host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) != -1) {

							if (host_readbs(hero + HERO_SPELLS +
									host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) < 11)
							{
								inc_spell_novice(hero,
									host_readws(Real2Host((ds_readd(0x0d97 + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i));
							}
							i++;
						}

						break;
					}
					case 10: {

						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(0x099d + 0 + 10 * i) == 3 && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 && ds_readws(0x664e + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(0x664e + 2 * i));
							i++;
						}

						i = 1;
						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(0x099d + 0 + 10 * i) == 3 && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						break;
					}
					case 11: {

						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 4 && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 && ds_readws(0x666a + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(0x666a + 2 * i));
							i++;
						}

						i = 1;
						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 4 && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}
						break;
					}
					case 12: {

						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 5 && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 && ds_readws(0x665c + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(0x665c + 2 * i));
							i++;
						}

						i = 1;
						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(0x99d + 0 + 10 * i) == 5 && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}
						break;
					}
				}
			}

			while (host_readbs(hero + HERO_SP_RISE) != 0) {
				inc_spell_novice(hero, random_schick(86));
			}
		}

	} else {

		if ((host_readbs(hero + HERO_TYPE) >= 7) &&
			(l_di = ds_readbs(0x2d1a + host_readbs(hero + HERO_TYPE))) &&
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

				sub_ptr_bs(hero + HERO_SP_RISE, i);
				add_ptr_bs(hero + HERO_TA_RISE, i);

			} else {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0xbc), l_di);

				i = GUI_input(Real2Host(ds_readd(DTP2)), 1);

				if (i > 0) {

					if (i > l_di) {
						i = l_di;
					}

					add_ptr_bs(hero + HERO_SP_RISE, i);
					sub_ptr_bs(hero + HERO_TA_RISE, i);
				}
			}
		}

		while (host_readbs(hero + HERO_TA_RISE) != 0) {

			l_si = LVL_select_talent(hero, 1);

			if (l_si >= 0) {

				inc_skill_advanced(hero, l_si);

			} else if (l_si == -2) {

				if (GUI_bool(get_city(0xa8))) {
					break;
				}
			}
		}

		while (host_readbs(hero + HERO_SP_RISE) != 0) {

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

	/* restore text file except for CHARTEXT.LTX, TAVERN.TLK and except for dialogs */
	if (city_bak != -1 && city_bak != ARCHIVE_FILE_CHARTEXT_LTX
	    && city_bak != ARCHIVE_FILE_TAVERN_TLK
	    && (city_bak < 156 || city_bak > 176)) {
		load_city_ltx(city_bak);
	}
}

#if !defined(__BORLANDC__)
}
#endif
