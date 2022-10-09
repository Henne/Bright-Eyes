/*
 *	Rewrite of DSA1 v3.02_de functions of seg098 (Magic)
 *	Functions rewritten: 12/12 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg098.cpp
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg029.h"
#include "seg041.h"
#include "seg047.h"
#include "seg097.h"
#include "seg098.h"
#include "seg099.h"
#include "seg100.h"
#include "seg101.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if !defined(__BORLANDC__)
static void (*spellhandler[])(void) = {
	NULL,
	/* Dispel / Antimagie */
	spell_beherrschung,
	spell_destructibo,
	spell_gardanium,
	spell_illusionen,
	spell_verwandlung,
	/* Domionation / Beherrschung */
	spell_band,
	spell_bannbaladin,
	spell_boeser_blick,
	spell_grosse_gier,
	spell_grosse_ver,
	spell_herrdertiere,
	spell_horriphobus,
	spell_magischerraub,
	spell_respondami,
	spell_sanftmut,
	spell_somnigravis,
	spell_zwingtanz,
	/* Demonology / Demonologie */
	spell_furor_blut,
	spell_geister_bannen,
	spell_geister_rufen,
	spell_heptagon,
	spell_kraehenruf,
	spell_skelettarius,
	/* Elements / Elemente */
	spell_elementare,
	spell_nihilatio,
	spell_solidirid,
	/* Movement / Bewegung */
	spell_axxeleratus,
	spell_foramen,
	spell_motoricus,
	spell_spurlos,
	spell_transversalis,
	spell_ueber_eis,
	/* Healing / Heilung */
	spell_balsam,
	spell_hexenspeichel,
	spell_klarum_purum,
	spell_ruhe_koerper,
	spell_tiere_heilen,
	/* Clairvoyance / Hellsicht */
	spell_adleraug,
	(void(*)(void))spell_analues,
	spell_eigenschaften,
	spell_exposami,
	spell_odem_arcanum,
	spell_penetrizzel,
	spell_sensibar,
	/* Illusion */
	spell_chamaelioni,
	spell_duplicatus,
	spell_harmlos,
	spell_hexenknoten,
	/* Combat / Kampf */
	spell_blitz,
	spell_ecliptifactus,
	spell_eisenrost,
	spell_fulminictus,
	spell_ignifaxius,
	spell_plumbumbarum,
	spell_radau,
	spell_saft_kraft,
	spell_scharfes_auge,
	/* Communication / Verstaendigung */
	spell_hexenblick,
	spell_necropathia,
	/* Transformation / Verwandlung */
	spell_adler,
	spell_arcano,
	spell_armatrutz,
	spell_inc_ch,
	spell_feuerbann,
	spell_inc_ff,
	spell_inc_ge,
	spell_inc_in,
	spell_inc_kk,
	spell_inc_kl,
	spell_inc_mu,
	spell_mutabili,
	spell_paralue,
	spell_salander,
	spell_see,
	spell_visibili,
	/* Transmutation / Veraenderung */
	spell_abvenenum,
	spell_aeolitus,
	spell_brenne,
	spell_claudibus,
	spell_dunkelheit,
	spell_erstarre,
	spell_flimflam,
	spell_schmelze,
	spell_silentium,
	spell_sturmgebr
};
#endif

struct dummy1 {
	signed short a[5];
};

/**
 * \brief   show some stars on the picture of a healed hero
 *
 * \param   hero        the hero who heals
 */
void magic_heal_ani(Bit8u *hero)
{
	signed short target_no;
	struct dummy1 a = *(struct dummy1*)(p_datseg + ANI_HEAL_PICSTARS);
	RealPt target;
	signed short fd;
	signed short i;

#if !defined(__BORLANDC__)
	a.a[0] = 0;
	a.a[1] = 1;
	a.a[2] = 2;
	a.a[3] = 1;
	a.a[4] = 0;
#endif

	/* load SPSTAR.NVF */
	fd = load_archive_file(ARCHIVE_FILE_SPSTAR_NVF);
	read_archive_file(fd, Real2Host(ds_readd(BUFFER8_PTR)), 0x400);
	read_archive_file(fd, Real2Host(ds_readd(BUFFER8_PTR)) + 0x400, 0x400);
	read_archive_file(fd, Real2Host(ds_readd(BUFFER8_PTR)) + 0x800, 0x400);
	bc_close(fd);

	target_no = host_readbs(hero + HERO_ENEMY_ID) - 1;
	target = (RealPt)ds_readd(HEROES) + SIZEOF_HERO * target_no;

	ds_writew(PIC_COPY_V1, 0);
	ds_writew(PIC_COPY_V2, 0);
	ds_writew(PIC_COPY_V3, 31);
	ds_writew(PIC_COPY_V4, 31);

	for (i = 0; i < 5; i++) {

		/* copy hero picture into buffer */
		ds_writew(PIC_COPY_X1, 0);
		ds_writew(PIC_COPY_Y1, 0);
		ds_writew(PIC_COPY_X2, 31);
		ds_writew(PIC_COPY_Y2, 31);
		ds_writed(PIC_COPY_DST, ds_readd(RENDERBUF_PTR));
		ds_writed(PIC_COPY_SRC, (Bit32u)(target + HERO_PORTRAIT));
		do_pic_copy(0);

		/* copy stars over it */
		ds_writed(PIC_COPY_SRC, (Bit32u)((RealPt)ds_readd(BUFFER8_PTR) + (a.a[i] * 1024)));
		do_pic_copy(2);

		/* copy buffer content to screen */
		ds_writew(PIC_COPY_X1, ds_readw(HERO_PIC_POSX + 2 * target_no));
		ds_writew(PIC_COPY_Y1, 157);
		ds_writew(PIC_COPY_X2, ds_readw(HERO_PIC_POSX + 2 * target_no) + 31);
		ds_writew(PIC_COPY_Y2, 188);
		ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));
		ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));
		do_pic_copy(3);

		delay_or_keypress(10);
	}

	draw_status_line();
}

/**
 * \brief   account physical spell damage in fight
 *
 * \param   le          LE someone looses
 */
void FIG_do_spell_damage(signed short le)
{

	if (le <= 0)
		return;

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {
		/* attack hero */

		/* set pointer */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROES) + (host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) * SIZEOF_HERO));


		/* ensure the spelluser does not attack himself */
		if (get_spelltarget() != get_spelluser()) {

			/* do the damage */
			sub_hero_le(get_spelltarget(), le);

			/* add a message (ired star with le) */
			FIG_add_msg(0x08, le);

			/* set a variable if the hoer died */
			if (hero_dead(get_spelltarget())) {
				ds_writew(DEFENDER_DEAD, 1);
			}
		}

	} else {
		/* attack enemy */

		/* set a pointer to the enemy */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* do the damage */
		FIG_damage_enemy(get_spelltarget_e(), le, 0);
		/* add a message (green star with le) */
		FIG_add_msg(0x0b, le);

		/* set a variable if the enemy died */
		if (enemy_dead(get_spelltarget_e())) {
			ds_writew(DEFENDER_DEAD, 1);
		}
	}
}

/**
 * \brief   calculates the PA value of one who is attacked
 *
 *          This function is only used by the spell Kraehenruf.
 */
signed short get_attackee_parade(void)
{

	/* check if enemy or hero is attacked */
	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {

		/* attacked a hero */

		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROES) + (host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) * SIZEOF_HERO));

		/* calculate PA  */

		/* PA = PA-Current-Weapon - AT-Modificator - 1/2 * RS-BE */

		return host_readbs(get_spelltarget() + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE) + HERO_PA)
			- host_readbs(get_spelltarget() + HERO_ATTACK_TYPE)
			- host_readbs(get_spelltarget() + HERO_RS_BE) / 2;
	} else {

		/* attacked an enemy */

		/* set a global pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		return host_readbs(get_spelltarget_e() + ENEMY_SHEET_PA);
	}
}

/**
 * \brief   calculates the RS value of one who is attacked
 *
 *          This function is unused.
 */
signed short get_attackee_rs(void)
{
	/* check if enemy or hero is attacked */
	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {

		/* attacked a hero */

		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROES) + (host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) * SIZEOF_HERO));

		return host_readbs(get_spelltarget() + HERO_RS_BONUS1); /* why not also HERO_RS_BONUS2? Anyway, function is unused... */

	} else {

		/* attacked an enemy */

		/* set a global pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		return host_readbs(get_spelltarget_e() + ENEMY_SHEET_RS);
	}
}

/**
 * \brief   get the amount of AE-Points needed for a spell
 *
 * \param   spell       numberof the spell
 * \param   half        cost    	the caster needs only half AE
 */
signed short get_spell_cost(signed short spell, signed short half_cost)
{
	signed char ret;

	ret = ds_readbs(SPELL_DESCRIPTIONS + SPELL_DESCRIPTIONS_AE_COST + spell * SIZEOF_SPELL_DESCRIPTIONS);

	if (half_cost != 0) {
		if (ret == -1) {
			ret = (signed char)random_interval(5, 10);
		} else {
			ret /= 2;
		}

		if (!ret) {
			/* spell cost is at least 1 */
			ret = 1;
		}
	}

	return ret;
}

/**
 * \brief   use magic menu, meditation and staffspell logic
 *
 * \param   hero        pointer to the hero
 * \return              {0, 1, 2}
 */
signed short use_magic(RealPt hero)
{
	signed short ae;
	signed short retval;
	signed short answer;
	signed short thonny_pos;

	retval = 0;

	answer = GUI_radio(get_ttx(329), 3, get_ttx(311), get_ttx(312), get_ttx(213));

	if (answer != -1) {

		switch(answer) {
		case 1: {
			/* Meditation */

			if (host_readbs(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_MAGE) {
				/* not a mage, need thonnys */


				if ((thonny_pos = get_item_pos(Real2Host(hero), ITEM_THONNYS)) == -1) {
					GUI_output(get_ttx(790));
					return 0;
				}
			} else {
				/* a mage */
				thonny_pos = -1;
			}

			/* Asks how many AE should be generated */
			ae = GUI_input(get_ttx(333), 2);

			if (ae != -1) {
				retval = 2;

				if (thonny_pos != -1) {
					/* drop a thonny */
					drop_item(Real2Host(hero), thonny_pos, 1);
				}

				/* cap the converted AE such that the hero has at most HERO_AE_ORIG in the end. */
				if (host_readws(Real2Host(hero) + HERO_AE_ORIG) - host_readws(Real2Host(hero) + HERO_AE)  < ae) {
					ae = host_readws(Real2Host(hero) + HERO_AE_ORIG) - host_readws(Real2Host(hero) + HERO_AE);
				}

				/* spend one AE point */
				sub_ae_splash(Real2Host(hero), 1);

#if !defined(__BORLANDC__)
				D1_INFO("%s Meditationsprobe +0 ",(char*)(hero + HERO_NAME2));
#endif
				if (test_attrib3(Real2Host(hero), ATTRIB_MU, ATTRIB_CH, ATTRIB_KK, 0) > 0) {
					/* Success */

					/* cap the converted AE such that at least 5 LE remain .*/
					if (host_readws(Real2Host(hero) + HERO_LE) <= ae + 8) {
						ae = host_readws(Real2Host(hero) + HERO_LE) - 8;
					}

					sub_hero_le(Real2Host(hero), ae + 3);
					add_hero_ae(Real2Host(hero), ae);
				} else {
					/* Failed, print only a message */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(795),
						(char*)Real2Host(hero) + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			}
			break;
		}
		case 2: {
			/* Staffspell */

			if (host_readbs(Real2Host(hero) + HERO_TYPE) != HERO_TYPE_MAGE) {
				/* only for mages */
				GUI_output(get_ttx(403));
				return 0;
			}

			if (host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL) == 7) {
				/* Original-Bug: This never happens.
				 * The highest possible staff spell is the 4th one, since the 5th staff spell has handicap 99.
				 * The check should ask for '== 4' instead. */
				GUI_output(get_ttx(335));
			} else {

				if (ds_readbs((STAFFSPELL_DESCRIPTIONS + STAFFSPELL_DESCRIPTIONS_AE_COST) + SIZEOF_STAFFSPELL_DESCRIPTIONS * host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL)) <= host_readws(Real2Host(hero) + HERO_AE)) {
					/* check AE */

					retval = 1;

#if !defined(__BORLANDC__)
					D1_INFO("%s Probe fuer Stabzauber Nr. %d (%+d)",(char*)(Real2Host(hero) + HERO_NAME2), host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL + 1), ds_readbs((STAFFSPELL_DESCRIPTIONS + 3) + 6 * host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL)));
#endif
					if (test_attrib3(Real2Host(hero),

						ds_readbs((STAFFSPELL_DESCRIPTIONS + STAFFSPELL_DESCRIPTIONS_ATTRIB1) + SIZEOF_STAFFSPELL_DESCRIPTIONS * host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL)),
						ds_readbs((STAFFSPELL_DESCRIPTIONS + STAFFSPELL_DESCRIPTIONS_ATTRIB2) + SIZEOF_STAFFSPELL_DESCRIPTIONS * host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL)),
#ifndef M302de_ORIGINAL_BUGFIX
						/* Original-Bug 17: the first attribute is tested twice, the second one is left out */
						ds_readbs((STAFFSPELL_DESCRIPTIONS + STAFFSPELL_DESCRIPTIONS_ATTRIB2) + SIZEOF_STAFFSPELL_DESCRIPTIONS * host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL)),
#else
						ds_readbs((STAFFSPELL_DESCRIPTIONS + STAFFSPELL_DESCRIPTIONS_ATTRIB3) + SIZEOF_STAFFSPELL_DESCRIPTIONS * host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL)),
#endif
						ds_readbs((STAFFSPELL_DESCRIPTIONS + STAFFSPELL_DESCRIPTIONS_HANDICAP) + SIZEOF_STAFFSPELL_DESCRIPTIONS * host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL))) > 0)
					{
						/* Success */

						/* print a message */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ttx(339),
							host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL) + 1);

						GUI_output(Real2Host(ds_readd(DTP2)));

						sub_ae_splash(Real2Host(hero), ds_readbs((STAFFSPELL_DESCRIPTIONS + STAFFSPELL_DESCRIPTIONS_AE_COST) + SIZEOF_STAFFSPELL_DESCRIPTIONS * host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL)));

						sub_ptr_ws(Real2Host(hero) + HERO_AE_ORIG, ds_readbs((STAFFSPELL_DESCRIPTIONS + STAFFSPELL_DESCRIPTIONS_AE_MOD) + SIZEOF_STAFFSPELL_DESCRIPTIONS * host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL)));

						/* Staffspell level +1 */
						inc_ptr_bs(Real2Host(hero) + HERO_STAFFSPELL_LVL);

						/* set the timer */
						host_writed(Real2Host(hero) + HERO_STAFFSPELL_TIMER, HOURS(12));

						timewarp(HOURS(5));
					} else {
						/* Failed */
						GUI_output(get_ttx(338));

						/* only half of the AE costs */
						sub_ae_splash(Real2Host(hero), ds_readbs((STAFFSPELL_DESCRIPTIONS + STAFFSPELL_DESCRIPTIONS_AE_COST) + SIZEOF_STAFFSPELL_DESCRIPTIONS * host_readbs(Real2Host(hero) + HERO_STAFFSPELL_LVL)) / 2);

						timewarp(HOURS(2));
					}
				} else {
					/* not enough AE */
					GUI_output(get_ttx(337));
				}
			}

			break;
		}
		case 3: {
			/* Cast Spell */
			use_spell(hero, 1, 0);
			break;
		}
		}
	}

	return retval;
}

/**
 * \brief   check if a spellclass can be used
 *
 * \param   hero        pointer to the hero
 * \param   spellclass_no the number of the spellclass
 * \return              0 = can't be used, 1 = can be used
 */
signed short can_use_spellclass(Bit8u *hero, signed short spellclass_no)
{
	signed short i;
	signed short first_spell;


	first_spell = ds_readbs(SPELLS_INDEX + 2 * spellclass_no);
	for (i = 0; ds_readbs((SPELLS_INDEX + 1) + 2 * spellclass_no) > i; i++) {

		if ((host_readbs(hero + HERO_SPELLS + first_spell + i) >= -5) &&
			(((ds_readw(IN_FIGHT) != 0) && (ds_readbs((SPELL_DESCRIPTIONS + 5) + 10 * (first_spell + i)) == 1)) ||
			((ds_readw(IN_FIGHT) == 0) && (ds_readbs((SPELL_DESCRIPTIONS + 5) + 10 * (first_spell + i)) != 1))))
		{
			return 1;
		}

	}

	return 0;
}

struct dummy3	{ char a[3]; };
struct dummy5	{ char a[5]; };
struct dummy6	{ char a[6]; };
struct dummy8	{ char a[8]; };
struct dummy12	{ char a[12]; };

signed short select_spell(Bit8u *hero, signed short show_vals)
{
	signed short l_di;
	signed short answer1;
	signed short first_spell;
	signed short retval = -1;
	struct dummy6 str_val = *(struct dummy6*)(p_datseg + SPELL_SELECT_STR_KEYVAL);
	struct dummy5 col_str = *(struct dummy5*)(p_datseg + SPELL_SELECT_STR_KEY);
	struct dummy3 str = *(struct dummy3*)(p_datseg + SPELL_SELECT_STR_KEY_COLOR);
	struct dummy8 col_str_val = *(struct dummy8*)(p_datseg + SPELL_SELECT_STR_KEYVAL_COLOR);
	struct dummy12 ones = *(struct dummy12*)(p_datseg + SPELL_SELECT_ONES);

	if ((show_vals == 0) && (ds_readws(GAME_MODE) == GAME_MODE_ADVANCED)) {
		show_vals = 2;
	}

	/* only for magic users */
	if (host_readbs(hero + HERO_TYPE) < HERO_TYPE_WITCH) {
		GUI_output(get_ttx(330));
		return -2;
	}

	if (show_vals == 1) {

		strcpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_ttx(205));

		if (host_readbs(hero + HERO_SP_RISE) > 1) {
			strcat((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_ttx(393));
		}

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ttx(204),
			(host_readbs(hero + HERO_SP_RISE) > 1) ? get_ttx(305) : get_ttx(304),
			host_readbs(hero + HERO_SP_RISE),
			Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

		answer1 = GUI_radio(Real2Host(ds_readd(DTP2)), 12,
					get_ttx(192), get_ttx(193),
					get_ttx(194), get_ttx(195),
					get_ttx(196), get_ttx(197),
					get_ttx(198), get_ttx(199),
					get_ttx(100), get_ttx(201),
					get_ttx(202), get_ttx(203)) - 1;
	} else {

		strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(216));

		for (l_di = 0; l_di < 12; l_di++) {

			ds_writed(RADIO_NAME_LIST + 4 * l_di,
				(Bit32u)((RealPt)ds_readd(DTP2) + 50 * (l_di + 1)));

			ones.a[l_di] = (signed char)can_use_spellclass(hero, l_di);

			sprintf((char*)Real2Host(ds_readd(RADIO_NAME_LIST + 4 * l_di)),
				(ones.a[l_di] != 0) ? str.a : col_str.a,
				get_ttx(l_di + 192));
		}

		answer1 = GUI_radio(Real2Host(ds_readd(DTP2)), 12,
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x00))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x04))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x08))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x0c))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x10))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x14))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x18))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x1c))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x20))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x24))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x28))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x2c)))) - 1;
	}

	if (answer1 != -2) {

		if (!ones.a[answer1]) {
			/* this cant use any spells of this class */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ttx(559),
				(char*)hero + HERO_NAME2);

			GUI_output(Real2Host(ds_readd(DTP2)));

			retval = -2;
		} else {

			first_spell = ds_readbs(SPELLS_INDEX + 2 * answer1);

			for (l_di = 0; l_di < ds_readbs((SPELLS_INDEX + 1) + 2 * answer1); l_di++) {

				ds_writed(RADIO_NAME_LIST + 4 * l_di,
					(Bit32u)((RealPt)ds_readd(DTP2) + 50 * (l_di)));


				if (show_vals == 1) {

					sprintf((char*)Real2Host(ds_readd(RADIO_NAME_LIST + 4 * l_di)),
						(char*)str_val.a,
						(char*)get_ttx(first_spell + l_di + 106),
						host_readbs(hero + HERO_SPELLS + first_spell + l_di));
				} else if (
					(((ds_readw(IN_FIGHT) != 0) && (ds_readbs((SPELL_DESCRIPTIONS + 5) + 10 * (first_spell + l_di)) == 1)) ||
					((ds_readw(IN_FIGHT) == 0) && (ds_readbs((SPELL_DESCRIPTIONS + 5) + 10 * (first_spell + l_di)) != 1))) &&
					(host_readbs(hero + HERO_SPELLS + first_spell + l_di) >= -5))
				{

					if (show_vals == 2) {
						sprintf((char*)Real2Host(ds_readd(RADIO_NAME_LIST + 4 * l_di)),
							(char*)str_val.a,
							(char*)get_ttx(first_spell + l_di + 106),
							host_readbs(hero + HERO_SPELLS + first_spell + l_di));
					} else {
						sprintf((char*)Real2Host(ds_readd(RADIO_NAME_LIST + 4 * l_di)),
							(char*)str.a,
							(char*)get_ttx(first_spell + l_di + 106));
					}
				} else if (show_vals == 2) {
					sprintf((char*)Real2Host(ds_readd(RADIO_NAME_LIST + 4 * l_di)),
						(char*)col_str_val.a,
						(char*)get_ttx(first_spell + l_di + 106),
						host_readbs(hero + HERO_SPELLS + first_spell + l_di));
				} else {
					sprintf((char*)Real2Host(ds_readd(RADIO_NAME_LIST + 4 * l_di)),
						(char*)col_str.a,
						(char*)get_ttx(first_spell + l_di + 106));
				}
			}

			retval = GUI_radio(get_ttx(217), ds_readbs((SPELLS_INDEX + 1) + 2 * answer1),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x00))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x04))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x08))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x0c))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x10))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x14))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x18))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x1c))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x20))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x24))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x28))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x2c))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x30))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x34))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x38))),
					Real2Host(ds_readd((RADIO_NAME_LIST + 0x3c))));

			if (retval != -1) {
				if ((host_readbs((retval + first_spell) + hero + HERO_TA_RISE) < -5) &&
					(show_vals == 0))
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ttx(560),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));
					retval = -1;
				} else {
					retval += first_spell -1;
				}
			}
		}

		if (retval > 0) {
			if ((ds_readw(IN_FIGHT) == 0) &&
				(ds_readbs((SPELL_DESCRIPTIONS + 5) + 10 * retval) == 1) &&
				(show_vals == 0))
			{
				GUI_output(get_ttx(591));
				retval = -2;
			} else {
				if ((ds_readw(IN_FIGHT) != 0) &&
					(ds_readbs((SPELL_DESCRIPTIONS + 5) + 10 * retval) == -1))
				{
					GUI_output(get_ttx(592));
					retval = -2;
				}
			}
		}
	} else {
		retval = -2;
	}

	return retval;
}

/**
 * \brief   makes a spell test. no AE deduction in this function.
 */
signed short test_spell(Bit8u *hero, signed short spell_no, signed char handicap)
{
	signed short retval;
	Bit8u *spell_desc;

	/* check if class is magic user */
	if ((host_readbs(hero + HERO_TYPE) < HERO_TYPE_WITCH) || (check_hero(hero) == 0)) {
		return 0;
	}
	/* check if spell skill >= -5 */
	if (host_readbs(hero + spell_no + HERO_SPELLS) < -5)
		return 0;
	/* check if hero has enough AE */
	if (get_spell_cost(spell_no, 0) > host_readws(hero + HERO_AE))
		return -99;

	spell_desc = p_datseg + spell_no * 10 + SPELL_DESCRIPTIONS;

	if (host_readb(spell_desc + 0x9) != 0) {

		if (host_readbs(hero + HERO_ENEMY_ID) >= 10) {

			handicap += ds_readbs(host_readbs(hero + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + ((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + 25));

			if (test_bit6(p_datseg + host_readbs(hero + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + ((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + 49))) {
				return 0;
			}
		} else {
			handicap += host_readbs(get_hero(host_readbs(hero + HERO_ENEMY_ID) - 1) + 0x66);
		}
	}

	if ((spell_no >= 1) && (spell_no <= 85)) {

#if !defined(__BORLANDC__)
		D1_INFO("%s Zauberprobe %s %+d (TaW %d)",(char*)(hero + HERO_NAME2), names_spell[spell_no], handicap, host_readbs(hero + spell_no + HERO_SPELLS));
#endif

		handicap -= host_readbs(hero + spell_no + HERO_SPELLS);

		retval = test_attrib3(hero, host_readbs(spell_desc+1),
			host_readbs(spell_desc+2), host_readbs(spell_desc+3), handicap);

		if (retval == -99) {
			retval = -1;
		}

		return retval;
	}

	return 0;
}

/**
 * \brief   makes a spell test for all magic users in the current group
 */
signed short test_spell_group(signed short spell, signed char handicap)
{

	Bit8u *hero_i = get_hero(0);
	signed short i;

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

		/* Check class is magicuser */
		if ((host_readbs(hero_i + HERO_TYPE) >= HERO_TYPE_WITCH) &&
			/* Check class  BOGUS */
			(host_readbs(hero_i + HERO_TYPE) != HERO_TYPE_NONE) &&
			/* Check in group */
			(host_readbs(hero_i + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
			/* Check if dead */
			!hero_dead(hero_i))
			/* Original-Bug: what if petrified, sleeping, unconcious etc. */
		{

			if (test_spell(hero_i, spell, handicap) > 0) {
				return 1;
			}
		}
	}

	return 0;
}

signed short select_magic_user(void)
{
	signed short answer;

	/* select the hero who shoulds cast a spell */
	answer = select_hero_ok(get_ttx(317));

	if (answer != -1) {
		/* valid answer => cast spell */
/*		return use_spell(get_hero(answer), 1, 0); */
		return use_spell((RealPt)ds_readd(HEROES) + SIZEOF_HERO * answer, 1, 0);
	}

	/* abort with error message */
	return -2;
}

/**
 * \brief   casts a spell
 *
 * \param   hero        	the hero who casts the spell
 * \param   selection_menu	1: select spell from menu / 0: spell is preselected in HERO_SPELL_ID
 * \param   handicap		modifier for the spell cast
 */
signed short use_spell(RealPt hero, signed short selection_menu, signed char handicap)
{
	signed short retval = 1;
	signed short spell_id;
	signed short ae_cost;
	signed short textbox_width_bak;
	Bit8u *ptr;
	void (*func)(void);
	signed short l4;
#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 29: see below */
	signed short x;
	signed short y;
	signed short pos;
	Bit8u *ptr_doors;
#endif

	if (!check_hero(Real2Host(hero)) && !hero_renegade(Real2Host(hero))) {

		return 0;
	}

	textbox_width_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);

	if (selection_menu == 1) {
		spell_id = select_spell(Real2Host(hero), 0);

		if (spell_id > SP_NONE) {
			/* pointer to the spell description */
			ptr = p_datseg + SPELL_DESCRIPTIONS + SIZEOF_SPELL_DESCRIPTIONS * spell_id;
			/* reset the spelltarget of the hero */
			host_writeb(Real2Host(hero) + HERO_ENEMY_ID, 0);

			if ((host_readbs(ptr + SPELL_DESCRIPTIONS_TARGET_TYPE) != 0) && (host_readbs(ptr + SPELL_DESCRIPTIONS_TARGET_TYPE) != 4)) {

				if (host_readbs(ptr + SPELL_DESCRIPTIONS_TARGET_TYPE) == 1) {

					GUI_output(get_ttx(234));

				} else {

					host_writeb(Real2Host(hero) + HERO_ENEMY_ID, select_hero_from_group(get_ttx(47)) + 1);

					if (host_readbs(Real2Host(hero) + HERO_ENEMY_ID) <= 0) {
						spell_id = -1;
					}
				}
			}
		}

	} else {
		spell_id = host_readbs(Real2Host(hero) + HERO_SPELL_ID);
	}

	if (spell_id > 0) {

		/* pointer to the spell description */
		ptr = p_datseg + SPELL_DESCRIPTIONS + SIZEOF_SPELL_DESCRIPTIONS * spell_id;

		if ((ds_readws(IN_FIGHT) == 0) && (host_readbs(ptr + SPELL_DESCRIPTIONS_WHERE_TO_USE) == 1)) {
			GUI_output(get_ttx(591));
			retval = 0;

		} else if ((ds_readws(IN_FIGHT) != 0) && (host_readbs(ptr + SPELL_DESCRIPTIONS_WHERE_TO_USE) == -1)) {
			GUI_output(get_ttx(592));
			retval = 0;
		}

		if (retval) {

#ifdef M302de_ORIGINAL_BUGFIX
			/* Original-Bug 29: door-specific spell handicap is not considered in a free Foramen spell (from the spellcast menu). */
			if (spell_id == SP_FORAMEN_FORAMINOR && ds_readbs(DUNGEON_INDEX) != DUNGEONS_NONE && (ds_readw(DNG_MENU_MODE) == DNG_MENU_MODE_OPEN_DOOR || ds_readw(DNG_MENU_MODE) == DNG_MENU_MODE_UNLOCK_DOOR)) {
				x = ds_readws(X_TARGET);
				y = ds_readws(Y_TARGET);
				ptr_doors = Real2Host(ds_readd(DUNGEON_DOORS_BUF));

				switch (ds_readbs(DIRECTION))
				{
					case NORTH: y--; break;
					case EAST:  x++; break;
					case SOUTH: y++; break;
					case WEST:  x--; break;
				}

				pos = DNG_POS(ds_readbs(DUNGEON_LEVEL), x, y);

				if ((host_readb(Real2Host(ds_readd(DNG_MAP_PTR)) + MAP_POS(x,y)) & 0x02) == 0) {
					/* flag 1 'unlocked' is not set -> door is locked  */
					while (host_readws(ptr_doors + 0) != pos) {
						/* ASSERT */
						/*
						if (host_readws(ptr_doors + 0) == -1) {
						D1_INFO("In free call of Foramen spell: door not found. This should not happen.\n");
						 */

						ptr_doors += 5;
					}
					handicap += host_readbs(ptr_doors + 4);
				}
			}
#endif

			ds_writew(SPELLTEST_RESULT, test_spell(Real2Host(hero), spell_id, handicap));

			if (ds_readws(SPELLTEST_RESULT) == -99) {

				/* prepare output */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ttx(607),
					(char*)Real2Host(hero) + HERO_NAME2);

				if (ds_readws(IN_FIGHT) == 0) {
					GUI_output(Real2Host(ds_readd(DTP2)));
				}

				retval = -1;

			} else if ((ds_readws(SPELLTEST_RESULT) <= 0) || (ds_readds(INGAME_TIMERS + 4 * INGAME_TIMER_RONDRA_NO_SPELLS) > 0)) {

				strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(606));

				sub_ae_splash(Real2Host(hero), get_spell_cost(spell_id, 1));

				if (ds_readws(IN_FIGHT) == 0) {
					GUI_output(Real2Host(ds_readd(DTP2)));
				}

				retval = 0;
			} else {
				/* set global spelluser variable */
				ds_writed(SPELLUSER, (Bit32u)hero);

				ae_cost = get_spell_cost(spell_id, 0);

				ds_writew(SPELL_SPECIAL_AECOST, -1);

				host_writeb(Real2Host(ds_readd(DTP2)), 0);

				l4 = ds_readws(TX_FILE_INDEX);

				load_tx(ARCHIVE_FILE_SPELLTXT_LTX);
#if !defined(__BORLANDC__)
				func = spellhandler[spell_id];
#else
				func = (void (*)(void))ds_readd(SPELL_HANDLERS + 4 * spell_id);
#endif
				func();

				if ((l4 != -1) && (l4 != 222)) {
					load_tx(l4);
				}

				retval = 1;

				if (ds_readws(SPELL_SPECIAL_AECOST) == 0) {
					retval = -1;

					if (!host_readbs(Real2Host(ds_readd(DTP2)))) {
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(606));
					}
				} else if (ds_readws(SPELL_SPECIAL_AECOST) == -2) {

					strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ttx(606));
					sub_ae_splash(Real2Host(hero), get_spell_cost(spell_id, 1));
					retval = 0;
				} else if (ds_readws(SPELL_SPECIAL_AECOST) != -1) {
					sub_ae_splash(Real2Host(hero), ds_readws(SPELL_SPECIAL_AECOST));
				} else {
					sub_ae_splash(Real2Host(hero), ae_cost);
				}

				if (ds_readws(IN_FIGHT) == 0) {

					GUI_output(Real2Host(ds_readd(DTP2)));

					if (retval > 0) {
						play_voc(ARCHIVE_FILE_FX17_VOC);

						if ((host_readbs(Real2Host(hero) + HERO_ENEMY_ID) < 10) &&
							(host_readbs(Real2Host(hero) + HERO_ENEMY_ID) > 0) &&
							(ds_readbs(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK))
						{
							magic_heal_ani(Real2Host(hero));
						}
					}
				}
			}
		} else {
			retval = 0;
		}
	} else {
		retval = -1;
	}

	ds_writew(TEXTBOX_WIDTH, textbox_width_bak);

	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
