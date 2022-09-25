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
 * \brief   tries to increase a spell in advanced mode
 *
 * \param   hero        pointer to the hero
 * \param   spell       number of the spell
 */
void inc_spell_advanced(Bit8u *hero, signed short spell)
{
	signed short max_incs = 1;
	signed short randval;
	struct dummy a = *(struct dummy*)(p_datseg + MAGIC_SCHOOL_SPELLRANGES);

	if ((host_readbs(hero + HERO_TYPE) == HERO_TYPE_WITCH) &&
		(ds_readbs((SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE) + SIZEOF_SPELL_DESCRIPTIONS * spell) == SPELL_DESC_HEROTYPE_WITCH))
	{
		/* witch spell */
		max_incs = 2;
	}

	if ((host_readbs(hero + HERO_TYPE) >= HERO_TYPE_GREEN_ELF) && /* hero is one of the three elven types */
		((ds_readbs((SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE) + SIZEOF_SPELL_DESCRIPTIONS * spell) == SPELL_DESC_HEROTYPE_GELF) ||
			(ds_readbs((SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE) + SIZEOF_SPELL_DESCRIPTIONS * spell) == SPELL_DESC_HEROTYPE_SELF) ||
			(ds_readbs((SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE) + SIZEOF_SPELL_DESCRIPTIONS * spell) == SPELL_DESC_HEROTYPE_IELF)))
	{
		/* elven spell */
		max_incs = 2;
	}

	if ((host_readbs(hero + HERO_TYPE) == HERO_TYPE_DRUID) &&
		(ds_readbs((SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE) + SIZEOF_SPELL_DESCRIPTIONS * spell) == SPELL_DESC_HEROTYPE_DRUID))
	{
		/* druid spell */
		max_incs = 2;
	}

	if (host_readbs(hero + HERO_TYPE) == HERO_TYPE_MAGE) {

		/* mages */
		if (ds_readbs((SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE) + SIZEOF_SPELL_DESCRIPTIONS * spell) == SPELL_DESC_HEROTYPE_MAGE) {
			/* spell is a mage spell */
			max_incs = 2;
		}

		/* check if spell is of the specialized group of the mage (according to his HERO_MAGIC_SCHOOL) */
		if ((a.a[host_readbs(hero + HERO_MAGIC_SCHOOL)].first <= spell) &&
			(a.a[host_readbs(hero + HERO_MAGIC_SCHOOL)].last >= spell)) {
			max_incs = 2;
		}

		/* depending on the HERO_MAGIC_SCHOOL, the mage will get 3 possible increases on certain spells */
		if (is_in_word_array(spell, (signed short*)Real2Host(ds_readd(MAGIC_SCHOOLS_INDEX + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))))
		{
			max_incs = 3;
		}
	}

	if (host_readbs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell + 1) >= max_incs) {

		/* no increase is possible */

		GUI_output(get_tx2(43));

	} else if (host_readbs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell) == 3) {

		/* 3 increase failures -> no further increase try allowed */

		GUI_output(get_tx2(44));
	} else {
		/* try to increase */

		dec_ptr_bs(hero + HERO_SP_RISE);

#ifndef M302de_FEATURE_MOD
		if (host_readbs(hero + HERO_SPELLS + spell) >= 11) {
			randval = random_interval(3, 18);
		} else {
			randval = random_interval(2, 12);
		}
#else
		/* Feature mod 2:
		 * use the exact skill/spell increase mechanism as in DSA 2/3 */
		if (host_readbs(hero + HERO_SPELLS + spell) >= 10) {
			randval = dice_roll(3,6,0);
		} else {
			randval = dice_roll(2,6,0);
		}
#endif

		if (host_readbs(hero + HERO_SPELLS + spell) < randval) {

			/* success */

			GUI_output(get_tx2(37));

			/* increment spell value */
			inc_ptr_bs(hero + spell + HERO_SPELLS);

			/* set the try counter to 0 */
			host_writebs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell, 0);
			/* increment the increase counter */
			inc_ptr_bs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell + 1);
		} else {
			/* fail */
			GUI_output(get_ttx(338));

			/* increment the try counter */
			inc_ptr_bs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell);
		}
	}
}

/**
 * \brief   tries to increase a skill in advanced mode
 *
 * \param   hero        pointer to the hero
 * \param   skill       number of the skill
 */
void inc_skill_advanced(Bit8u *hero, signed short skill)
{
	signed short randval;
	signed short max_incs;

	max_incs = ds_readbs(SKILL_DESCRIPTIONS + 4 * skill + 3);


	if (host_readbs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill + 1) >= max_incs) {

		/* no increase is possible */

		GUI_output(get_tx2(43));

	} else if (host_readbs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill) == 3) {

		/* used up legal increase */

		GUI_output(get_tx2(44));
	} else {
		/* try to increase */

		dec_ptr_bs(hero + HERO_TA_RISE);

#ifndef M302de_FEATURE_MOD
		if (host_readbs(hero + HERO_TALENTS + skill) >= 11) {
			randval = random_interval(3, 18);
		} else {
			randval = random_interval(2, 12);
		}
#else
		/* Feature mod 2:
		 * use the exact spell/skill increase mechanism as in DSA 2/3 */
		if (host_readbs(hero + HERO_TALENTS + skill) >= 10) {
			randval = dice_roll(3,6,0);
		} else {
			randval = dice_roll(2,6,0);
		}
#endif

		if (host_readbs(hero + HERO_TALENTS + skill) < randval) {

			/* success */

			GUI_output(get_tx2(37));

			/* increment spell value */
			inc_ptr_bs(hero + skill + HERO_TALENTS);

			/* set the try counter to 0 */
			host_writebs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill, 0);
			/* increment the increase counter */
			inc_ptr_bs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill + 1);

			if (skill <= 6) {
				/* increment a melee weapon skill */

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(426), get_ttx(48 + skill));

				randval = -1;

				/* AT - value */
				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
					(char*)get_ttx(427), host_readbs(hero + skill + 0x68));

				/* PA - value */
				sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)) + 50,
					(char*)get_ttx(428), host_readbs(hero + skill + 0x6f));

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
			GUI_output(get_ttx(338));

			/* increment the try counter */
			inc_ptr_bs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + 2 * skill);
		}
	}
}

/**
 * \brief   tries to increase a skill in novice mode
 *
 *          This function is quiet and does not check how many times
 *          a skill can be increased. So the correct rules come
 *          from the array which contain the skills.
 *
 * \param   hero        pointer to the hero
 * \param   skill       number of the skill
 */
void inc_skill_novice(Bit8u *hero, signed short skill)
{
	signed short done;
	signed short randval;

	done = 0;

	while (!done) {

		/* leave the loop if 3 incs failes or the skill value is 18 */
		if ((host_readbs(Real2Host(ds_readd(INC_SKILLS_COUNTER)) + skill * 2) == 3) ||
			(host_readbs(hero + HERO_TALENTS + skill) == 18)) {
			done = 1;
#if !defined(__BORLANDC__)
			D1_INFO("%s kann Talent nicht weiter steigern (3 Fehlversuche oder Talentwert 18)\n", hero + HERO_NAME2);
#endif
		} else {

			/* dec available skill incs */
			dec_ptr_bs(hero + HERO_TA_RISE);

			/* check if available skill incs are 0 */
			if (!host_readbs(hero + HERO_TA_RISE)) {
				done = 1;
			}

#ifndef M302de_FEATURE_MOD
			/* on a  skill value < 11 use 2W6 else 3W6 */
			if (host_readbs(hero + HERO_TALENTS + skill) >= 11) {
				randval = random_interval(3, 18);
			} else {
				randval = random_interval(2, 12);
			}
#else
		/* Feature mod 2:
		 * use the exact skill/spell increase mechanism as in DSA 2/3 */
			if (host_readbs(hero + HERO_TALENTS + skill) >= 10) {
				randval = dice_roll(3,6,0);
			} else {
				randval = dice_roll(2,6,0);
			}
#endif

			/* check if increase success */
			if (host_readbs(hero + HERO_TALENTS + skill) < randval) {

				/* inc skill value */
				inc_ptr_bs(hero + HERO_TALENTS + skill);

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
 * \brief   tries to increase a spell in novice mode
 *
 *          This function is quiet and does not check how many times
 *          a spell can be increased. So the correct rules come
 *          from the array which contain the spells.
 *
 * \param   hero        pointer to the hero
 * \param   spell       number of the spell
 */
void inc_spell_novice(Bit8u *hero, signed short spell)
{
	signed short done;
	signed short randval;

	done = 0;

	while (!done) {
		/* leave the loop if 3 tries to increase have failed or if the spell is already at the max value 18 */
		if ((host_readb(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell) == 3) ||
			(host_readbs(hero + HERO_SPELLS + spell) == 18)) {
			done = 1;
#if !defined(__BORLANDC__)
			D1_INFO("%s kann Zauber nicht weiter steigern (3 Fehlversuche oder Talentwert 18)\n", hero + HERO_NAME2);
#endif
		} else {

			/* dec available spell incs */
			dec_ptr_bs(hero + HERO_SP_RISE);

			/* check if available spell incs are 0 */
			if (!host_readbs(hero + HERO_SP_RISE)) {
				done = 1;
			}

#ifndef M302de_FEATURE_MOD
			/* on a  spell value < 11 use 2W6 else 3W6 */
			if (host_readbs(hero + HERO_SPELLS + spell) >= 11) {
				randval = random_interval(3, 18);
			} else {
				randval = random_interval(2, 12);
			}
#else
		/* Feature mod 2:
		 * use the exact skill/spell increase mechanism as in DSA 2/3 */
			if (host_readbs(hero + HERO_SPELLS + spell) >= 10) {
				randval = dice_roll(3,6,0);
			} else {
				randval = dice_roll(2,6,0);
			}
#endif

			/* check if increase success */
			if (host_readbs(hero + HERO_SPELLS + spell) < randval) {

				/* inc spell value */
				inc_ptr_bs(hero + HERO_SPELLS + spell);

				/* reset failed counter */
				host_writeb(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell, 0);

				done = 1;

			} else {
				/* inc failed counter */
				inc_ptr_bs(Real2Host(ds_readd(INC_SPELLS_COUNTER)) + 2 * spell);
			}
		}
	}
}

/**
 * \brief   brings your hero to the next level
 *
 * \param   hero_pos    the position of the hero
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

	if (ds_readb(FADING_STATE) != 0) {
		refresh_colors();
	}

	ds_writew(TIMERS_DISABLED, 1);

	city_bak = ds_readws(TEXT_FILE_INDEX);

	load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);

	ds_writed(INC_SKILLS_COUNTER, (Bit32u)((RealPt)ds_readd(BUFFER8_PTR) + 4500));
	ds_writed(INC_SPELLS_COUNTER, (Bit32u)((RealPt)ds_readd(INC_SKILLS_COUNTER) + 208));
	ds_writed(SKILLS_BUFFER, ds_readd(BUFFER9_PTR));

	l_si = load_archive_file(ARCHIVE_FILE_BSKILLS_DAT);

	read_archive_file(l_si, Real2Host(ds_readd(SKILLS_BUFFER)), 1300);

	bc_close(l_si);


	sprintf((char*)Real2Host(ds_readd(DTP2)),
		(char*)get_ttx(411),
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
		if (host_readbs(hero + HERO_ATTRIB_ORIG + 3 * i) < 20) {
			l_si = 0;
		}
	}

	if (l_si != -1) {

		l_si = -2;


		do {

			ds_writew(BASEPOS_X, -30);

			l_si = GUI_radio(get_tx2(35), 7,
						get_ttx(412),
						get_ttx(413),
						get_ttx(414),
						get_ttx(415),
						get_ttx(416),
						get_ttx(417),
						get_ttx(418)) - 1;

			if (host_readbs(hero + HERO_ATTRIB_ORIG + 3 * l_si) >= 20) {
				l_si = -2;
				GUI_output(get_tx2(43));
			}

		} while (l_si == -2);

		inc_ptr_bs(hero + HERO_ATTRIB_ORIG + 3 * l_si);
		inc_ptr_bs(hero + HERO_ATTRIB + 3 * l_si);

		status_show(hero_pos);

		ds_writebs(STATUS_PAGE_HUNGER, ds_writebs(STATUS_PAGE_THIRST, -1));

		update_status_bars();
	}

	l_si = -1;

	for (i = 7; i <= 13; i++) {
		if (host_readbs(hero + HERO_ATTRIB_ORIG + 3 * i) > 2) {
			l_si = 0;
		}
	}

	if (l_si != -1) {

		l_si = -2;

		do {

			l_si = GUI_radio(get_tx2(36), 7,
						get_ttx(419),
						get_ttx(420),
						get_ttx(421),
						get_ttx(422),
						get_ttx(423),
						get_ttx(424),
						get_ttx(425)) - 1;

			if (host_readbs(hero + HERO_ATTRIB_ORIG + 3 * (l_si + 7)) <= 2) {
				l_si = -2;
				GUI_output(get_tx2(48));
			}

		} while (l_si == -2);

		v3 = 0;

		for (i = 0; i < 3; i++) {

			if (random_schick(20) <= host_readbs(hero + HERO_ATTRIB_ORIG + 3 * (l_si + 7))) {
				v3 = 1;
			}
		}

		if (v3 != 0) {

			dec_ptr_bs(hero + HERO_ATTRIB_ORIG + 3 * (l_si + 7));
			dec_ptr_bs(hero + HERO_ATTRIB + 3 * (l_si + 7));

			GUI_output(get_tx2(37));

			status_show(hero_pos);

			ds_writebs(STATUS_PAGE_HUNGER, ds_writebs(STATUS_PAGE_THIRST, -1));

			update_status_bars();
		} else {

			GUI_output(get_ttx(338));
		}
	}

	/* check changes in MR */

	mr = (host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_KL)) + host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_MU)) + host_readbs(hero + HERO_LEVEL)) / 3 - 2 * host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_AG));
	mr += ds_readbs(MR_MODIFICATORS + host_readbs(hero + HERO_TYPE));

	if (host_readbs(hero + HERO_MR) != mr) {
		/* set new basic MR */
		host_writebs(hero + HERO_MR, mr);

		/* check for STIRNREIF [blau] / CORONET [blue] equipped => MR + 2 */
		if (host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + HERO_INVENTORY_SLOT_HEAD * SIZEOF_INVENTORY) == ITEM_CORONET_BLUE) {
			add_ptr_bs(hero + HERO_MR, 2);
		}
		/* check for RING / RING equipped => MR + 2 */
		if (host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + HERO_INVENTORY_SLOT_LEFT_HAND * SIZEOF_INVENTORY) == ITEM_RING_RED) {
			add_ptr_bs(hero + HERO_MR, 2);
		}
		/* check for AMULETT / in inventory => MR + 5 */
		if (get_item_pos(hero, ITEM_AMULET_BLUE) != -1) {
			add_ptr_bs(hero + HERO_MR, 5);
		}

		/* show the user the new MR value */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx2(41),
			host_readbs(hero + HERO_MR));

		GUI_output(Real2Host(ds_readd(DTP2)));

		/* update status background */
		status_show(hero_pos);

		/* update_the bars of the hero */
		ds_writebs(STATUS_PAGE_HUNGER, ds_writebs(STATUS_PAGE_THIRST, -1));

		update_status_bars();
	}


	/* add skill increasements */
	add_ptr_bs(hero + HERO_TA_RISE, ds_readbs((LEVELUP_TA_RISE-1) + host_readbs(hero + HERO_TYPE)));

	/* roll how many LE points the hero may get */
	i = random_schick(6);

	if (host_readbs(hero + HERO_TYPE) >= 7) {
		/* a magic user */

		/* add spell increasements */
		add_ptr_bs(hero + HERO_SP_RISE, ds_readbs((LEVELUP_SP_RISE - 7) + host_readbs(hero + HERO_TYPE)));

		i += 2;

		/* show how many LE the hero may get */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_tx2(39),
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
		if (host_readbs(hero + HERO_TYPE) == HERO_TYPE_MAGE && ds_readws(GAME_MODE) == GAME_MODE_ADVANCED) {

			if (GUI_bool(get_tx2(40))) {
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
			(char*)get_tx2(38),
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

	if (ds_readws(GAME_MODE) == GAME_MODE_BEGINNER) {

		i = v2 = 0;

		while (host_readbs(hero + HERO_TA_RISE) > 0) {

			l_si = host_readws(Real2Host(ds_readd(SKILLS_BUFFER)) + 100 * host_readbs(hero + HERO_TYPE) + 4 * i);

			if (host_readbs(hero + HERO_TALENTS + l_si) < host_readws(Real2Host(ds_readd(SKILLS_BUFFER)) + 100 * host_readbs(hero + HERO_TYPE) + 4 * i + 2))
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

		if (host_readbs(hero + HERO_TYPE) >= HERO_TYPE_WITCH) {
			/* hero has magic type */

			i = 1;
			v2 = 0;

			while (host_readbs(hero + HERO_SP_RISE) != 0 && v2 < 3) {

				v2++;

				switch (host_readbs(hero + HERO_TYPE)) {

					case HERO_TYPE_WITCH: {

						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE + SIZEOF_SPELL_DESCRIPTIONS * i) == SPELL_DESC_HEROTYPE_WITCH && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 && ds_readws(AUTOINC_SPELLS_WITCH + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(AUTOINC_SPELLS_WITCH + 2 * i));
							i++;
						}

						break;
					}
					case HERO_TYPE_DRUID: {

						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE + SIZEOF_SPELL_DESCRIPTIONS * i) == SPELL_DESC_HEROTYPE_DRUID && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 && ds_readws(AUTOINC_SPELLS_DRUID + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(AUTOINC_SPELLS_DRUID + 2 * i));
							i++;
						}

						break;
					}
					case HERO_TYPE_MAGE: {

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 &&
							(host_readws(Real2Host((ds_readd(MAGIC_SCHOOLS_INDEX + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) != -1) {

							if (host_readbs(hero + HERO_SPELLS +
									host_readws(Real2Host((ds_readd(MAGIC_SCHOOLS_INDEX + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) < 11)
							{
								inc_spell_novice(hero,
									host_readws(Real2Host((ds_readd(MAGIC_SCHOOLS_INDEX + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i));
							}
							i++;
						}

						i = 0;
						while (host_readbs(hero + HERO_SP_RISE) != 0 &&
							(host_readws(Real2Host((ds_readd(AUTOINC_SPELLS_MAGE_INDEX + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) != -1) {

							if (host_readbs(hero + HERO_SPELLS +
									host_readws(Real2Host((ds_readd(AUTOINC_SPELLS_MAGE_INDEX + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) < 11)
							{
								inc_spell_novice(hero,
									host_readws(Real2Host((ds_readd(AUTOINC_SPELLS_MAGE_INDEX + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i));
							}
							i++;
						}

						i = 0;
						while (host_readbs(hero + HERO_SP_RISE) != 0 &&
							(host_readws(Real2Host((ds_readd(MAGIC_SCHOOLS_INDEX + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) != -1) {

							if (host_readbs(hero + HERO_SPELLS +
									host_readws(Real2Host((ds_readd(MAGIC_SCHOOLS_INDEX + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i)) < 11)
							{
								inc_spell_novice(hero,
									host_readws(Real2Host((ds_readd(MAGIC_SCHOOLS_INDEX + 4 * host_readbs(hero + HERO_MAGIC_SCHOOL)))) + 2 * i));
							}
							i++;
						}

						break;
					}
					case HERO_TYPE_GREEN_ELF: {

						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE + SIZEOF_SPELL_DESCRIPTIONS * i) == SPELL_DESC_HEROTYPE_GELF && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 && ds_readws(AUOTINC_SPELLS_GELF + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(AUOTINC_SPELLS_GELF + 2 * i));
							i++;
						}

						i = 1;
						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE + SIZEOF_SPELL_DESCRIPTIONS * i) == SPELL_DESC_HEROTYPE_GELF && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						break;
					}
					case HERO_TYPE_ICE_ELF: {

						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE + SIZEOF_SPELL_DESCRIPTIONS * i) == SPELL_DESC_HEROTYPE_IELF && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 && ds_readws(AUTOINC_SPELLS_IELF + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(AUTOINC_SPELLS_IELF + 2 * i));
							i++;
						}

						i = 1;
						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE + SIZEOF_SPELL_DESCRIPTIONS * i) == SPELL_DESC_HEROTYPE_IELF && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}
						break;
					}
					case HERO_TYPE_SYLVAN_ELF: {

						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE + SIZEOF_SPELL_DESCRIPTIONS * i) == SPELL_DESC_HEROTYPE_SELF && host_readbs(hero + HERO_SPELLS + i) < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (host_readbs(hero + HERO_SP_RISE) != 0 && ds_readws(AUTOINC_SPELLS_SELF + 2 * i) != -1) {
							inc_spell_novice(hero, ds_readws(AUTOINC_SPELLS_SELF + 2 * i));
							i++;
						}

						i = 1;
						while (host_readbs(hero + HERO_SP_RISE) != 0 && i < 86) {

							if (ds_readbs(SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_HEROTYPE + SIZEOF_SPELL_DESCRIPTIONS * i) == SPELL_DESC_HEROTYPE_SELF && host_readbs(hero + HERO_SPELLS + i) < 11) {
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
			(l_di = ds_readbs((LEVELUP_SPTA_CONV - 7) + host_readbs(hero + HERO_TYPE))) &&
			GUI_bool(get_tx2(45)))
		{
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_tx2(46), l_di);

			i = GUI_input(Real2Host(ds_readd(DTP2)), 1);

			if (i > 0) {

				if (i > l_di) {
					i = l_di;
				}
				l_di -= i;

				sub_ptr_bs(hero + HERO_SP_RISE, (unsigned char)i);
				add_ptr_bs(hero + HERO_TA_RISE, (unsigned char)i);

			} else {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx2(47), l_di);

				i = GUI_input(Real2Host(ds_readd(DTP2)), 1);

				if (i > 0) {

					if (i > l_di) {
						i = l_di;
					}

					add_ptr_bs(hero + HERO_SP_RISE, (unsigned char)i);
					sub_ptr_bs(hero + HERO_TA_RISE, (unsigned char)i);
				}
			}
		}

		while (host_readbs(hero + HERO_TA_RISE) != 0) {

			l_si = LVL_select_skill(hero, 1);

			if (l_si >= 0) {

				inc_skill_advanced(hero, l_si);

			} else if (l_si == -2) {

				if (GUI_bool(get_tx2(42))) {
					break;
				}
			}
		}

		while (host_readbs(hero + HERO_SP_RISE) != 0) {

			l_si = select_spell(hero, 1);

			if (l_si > 0) {

				inc_spell_advanced(hero, l_si);

			} else if (l_si == -2) {

				if (GUI_bool(get_tx2(42))) {
					break;
				}
			}
		}
	}

	ds_writew(BASEPOS_X, 0);
	ds_writew(REQUEST_REFRESH, 1);
	ds_writew(TIMERS_DISABLED, 0);

	ds_writews(CURRENT_ANI, -1);

	/* restore text file except for CHARTEXT.LTX, TAVERN.TLK and except for dialogs */
	if (city_bak != -1 && city_bak != ARCHIVE_FILE_CHARTEXT_LTX
	    && city_bak != ARCHIVE_FILE_TAVERN_TLK
	    && (city_bak < 156 || city_bak > 176)) {
		load_tx2(city_bak);
	}
}

#if !defined(__BORLANDC__)
}
#endif
