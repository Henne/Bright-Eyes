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
	/* Clarivoyance / Hellsicht */
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
	spell_paral,
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
 * magic_heal_ani() - show some stars on the picture of a healed hero
 * @hero:	the hero who heals
 *
*/
void magic_heal_ani(Bit8u *hero)
{
	signed short target_nr;
	struct dummy1 a = *(struct dummy1*)(p_datseg + 0xac10);
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
	fd = load_archive_file(0x83);
	read_archive_file(fd, Real2Host(ds_readd(0xc3a9)), 0x400);
	read_archive_file(fd, Real2Host(ds_readd(0xc3a9)) + 0x400, 0x400);
	read_archive_file(fd, Real2Host(ds_readd(0xc3a9)) + 0x800, 0x400);
	bc_close(fd);

	target_nr = host_readbs(hero + 0x86) - 1;
	target = (RealPt)ds_readd(HEROS) + 0x6da * target_nr;

	ds_writew(0xc01d, 0);
	ds_writew(0xc01f, 0);
	ds_writew(0xc021, 31);
	ds_writew(0xc023, 31);

	for (i = 0; i < 5; i++) {

		/* copy hero picture into buffer */
		ds_writew(0xc011, 0);
		ds_writew(0xc013, 0);
		ds_writew(0xc015, 31);
		ds_writew(0xc017, 31);
		ds_writed(0xc00d, ds_readd(0xd303));
		ds_writed(0xc019, (Bit32u)(target + 0x2da));
		do_pic_copy(0);

		/* copy stars over it */
		ds_writed(0xc019, (Bit32u)((RealPt)ds_readd(0xc3a9) + (a.a[i] * 1024)));
		do_pic_copy(2);

		/* copy buffer content to screen */
		ds_writew(0xc011, ds_readw(0x2d01 + 2 * target_nr));
		ds_writew(0xc013, 157);
		ds_writew(0xc015, ds_readw(0x2d01 + 2 * target_nr) + 31);
		ds_writew(0xc017, 188);
		ds_writed(0xc019, ds_readd(0xd303));
		ds_writed(0xc00d, ds_readd(0xd2ff));
		do_pic_copy(3);

		delay_or_keypress(10);
	}

	draw_status_line();
}

/**
 * FIG_do_spell_damage() - account physical spell damage in fight
 * @le:	LE someone looses
*/
void FIG_do_spell_damage(signed short le)
{

	if (le <= 0)
		return;

	if (host_readbs(get_spelluser() + 0x86) < 10) {
		/* attack hero */

		/* set pointer */
		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROS) + (host_readbs(get_spelluser() + 0x86) - 1) * 0x6da));


		/* ensure the spelluser does not attack himself */
		if (Real2Host(ds_readd(SPELLTARGET)) != get_spelluser()) {

			/* do the damage */
			sub_hero_le(Real2Host(ds_readd(SPELLTARGET)), le);

			/* add a message (ired star with le) */
			FIG_add_msg(0x08, le);

			/* set a variable if the hoer died */
			if (hero_dead(Real2Host(ds_readd(SPELLTARGET)))) {
				ds_writew(0xe3a6, 1);
			}
		}

	} else {
		/* attack enemy */

		/* set a pointer to the enemy */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser() + 0x86) * 62));

		/* do the damage */
		FIG_damage_enemy(get_spelltarget_e(), le, 0);
		/* add a message (green star with le) */
		FIG_add_msg(0x0b, le);

		/* set a variable if the enemy died */
		if (enemy_dead(get_spelltarget_e())) {
			ds_writew(0xe3a6, 1);
		}
	}
}

/**
 * get_attackee_parade()  - calculates the PA value of one who is attacked
 *
 * This function is only used by the spell Kraehenruf.
*/
signed short get_attackee_parade(void)
{

	/* check if enemy or hero is attacked */
	if (host_readbs(get_spelluser() + 0x86) < 10) {

		/* attacked a hero */

		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROS) + (host_readbs(get_spelluser() + 0x86) - 1) * 0x6da));

		/* calculate PA  */

		/* PA = PA-Current-Weapon - AT-Modificator - 1/2 * RS-BE */

		return host_readbs(get_spelltarget() + host_readbs(get_spelltarget() + 0x78) + 0x6f)
			- host_readbs(get_spelltarget() + 0x79)
			- host_readbs(get_spelltarget() + 0x32) / 2;
	} else {

		/* attacked an enemy */

		/* set a global pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser() + 0x86) * 62));

		return host_readbs(get_spelltarget_e() + 0x1d);
	}
}

/**
 * get_attackee_rs()  - calculates the RS value of one who is attacked
 *
 * This function is unused.
*/
signed short get_attackee_rs(void)
{
	/* check if enemy or hero is attacked */
	if (host_readbs(get_spelluser() + 0x86) < 10) {

		/* attacked a hero */

		ds_writed(SPELLTARGET,
			(Bit32u)((RealPt)ds_readd(HEROS) + (host_readbs(get_spelluser() + 0x86) - 1) * 0x6da));

		return host_readbs(get_spelltarget() + 0x30);

	} else {

		/* attacked an enemy */

		/* set a global pointer to the target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)RealMake(datseg, 0xd0df + host_readbs(get_spelluser() + 0x86) * 62));

		return host_readbs(get_spelltarget_e() + 0x02);
	}
}

/**
 *	get_spell_cost() - get the amount of AE-Points needed for a spell
 *	@spell:		numberof the spell
 *	@half cost:	the caster needs only half AE
*/
signed short get_spell_cost(signed short spell, signed short half_cost)
{
	signed char ret;

	ret = ds_readbs(0x99d + 4 + spell * 10);

	if (half_cost != 0) {
		if (ret == -1) {
			ret = (signed char)random_interval(5, 10);
		} else {
			ret /= 2;
		}

		if (!ret) {
			ret = 1;
		}
	}

	return ret;
}

/**
 * \brief use magic menu, meditation and staffspell logic
 *
 * \param hero	pointer to the hero
 *
 * \return {0, 1, 2}
 */
signed short use_magic(RealPt hero)
{
	signed short le;
	signed short retval;
	signed short answer;
	signed short thonny_pos;

	retval = 0;

	answer = GUI_radio(get_ltx(0x524), 3, get_ltx(0x4dc), get_ltx(0x4e0), get_ltx(0x354));

	if (answer != -1) {

		switch(answer) {
		case 1: {
			/* Meditation */

			if (host_readbs(Real2Host(hero) + 0x21) != 9) {
				/* not a mage, need thonnys */


				if ((thonny_pos = get_item_pos(Real2Host(hero), 131)) == -1) {
					GUI_output(get_ltx(0xc58));
					return 0;
				}
			} else {
				/* a mage */
				thonny_pos = -1;
			}

			/* Aks how many LE should be used */
			le = GUI_input(get_ltx(0x534), 2);

			if (le != -1) {
				retval = 2;

				if (thonny_pos != -1) {
					/* drop a thonny */
					drop_item(Real2Host(hero), thonny_pos, 1);
				}

				/* adjust LE */
				if (host_readws(Real2Host(hero) + 0x62) - host_readws(Real2Host(hero) + 0x64)  < le) {
					le = host_readws(Real2Host(hero) + 0x62) - host_readws(Real2Host(hero) + 0x64);
				}

				/* spend one AE point */
				sub_ae_splash(Real2Host(hero), 1);

				if (test_attrib3(Real2Host(hero), 0, 2, 6, 0) > 0) {
					/* Success */

					if (host_readws(Real2Host(hero) + 0x60) <= le + 8) {
						le = host_readws(Real2Host(hero) + 0x60) - 8;
					}

					sub_hero_le(Real2Host(hero), le + 3);
					add_hero_ae(Real2Host(hero), le);
				} else {
					/* Failed, print only a message */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0xc6c),
						(char*)Real2Host(hero) + 0x10);

					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			}
			break;
		}
		case 2: {
			/* Staffspell */

			if (host_readbs(Real2Host(hero) + 0x21) != 9) {
				/* only for mages */
				GUI_output(get_ltx(0x64c));
				return 0;
			}

			if (host_readbs(Real2Host(hero) + 0x195) == 7) {
				GUI_output(get_ltx(0x53c));
			} else {

				if (ds_readbs((0x972 + 5) + 6 * host_readbs(Real2Host(hero) + 0x195)) <= host_readws(Real2Host(hero) + 0x64)) {
					/* check AE */

					retval = 1;

					/* Original-Bug: the second attribute is used twice here */
					if (test_attrib3(Real2Host(hero),
						ds_readbs((0x972 + 1) + 6 * host_readbs(Real2Host(hero) + 0x195)),
						ds_readbs((0x972 + 2) + 6 * host_readbs(Real2Host(hero) + 0x195)),
						ds_readbs((0x972 + 2) + 6 * host_readbs(Real2Host(hero) + 0x195)),
						ds_readbs((0x972 + 4) + 6 * host_readbs(Real2Host(hero) + 0x195))) > 0)
					{
						/* Success */

						/* print a message */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x54c),
							host_readbs(Real2Host(hero) + 0x195) + 1);

						GUI_output(Real2Host(ds_readd(DTP2)));

						sub_ae_splash(Real2Host(hero), ds_readbs((0x972 + 5) + 6 * host_readbs(Real2Host(hero) + 0x195)));

						sub_ptr_ws(Real2Host(hero) + 0x62,	ds_readbs((0x972 + 6) + 6 * host_readbs(Real2Host(hero) + 0x195)));

						/* Staffspell level +1 */
						inc_ptr_bs(Real2Host(hero) + 0x195);

						/* set the timer */
						host_writed(Real2Host(hero) + 0x8f, 0xfd20);

						/* let some time pass */
						timewarp(0x6978);
					} else {
						/* Failed */
						GUI_output(get_ltx(0x548));

						/* only half of the AE costs */
						sub_ae_splash(Real2Host(hero), ds_readbs((0x972 + 5) + 6 * host_readbs(Real2Host(hero) + 0x195)) / 2);

						/* let some time pass */
						timewarp(0x2a30);
					}
				} else {
					/* not enough AE */
					GUI_output(get_ltx(0x544));
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
 * \brief check if a spellclass can be used
 *
 * \param hero		pointer to the hero
 * \param spellclass_nr	the number of the spellclass
 *
 * \return 0 = can't be used, 1 = can be used
*/
signed short can_use_spellclass(Bit8u *hero, signed short spellclass_nr)
{
	signed short i;
	signed short first_spell;


	first_spell = ds_readbs(0xd03 + 2 * spellclass_nr);
	for (i = 0; ds_readbs(0xd04 + 2 * spellclass_nr) > i; i++) {

		if ((host_readbs(hero + 0x13d + first_spell + i) >= -5) &&
			(((ds_readw(IN_FIGHT) != 0) && (ds_readbs((0x99d + 5) + 10 * (first_spell + i)) == 1)) ||
			((ds_readw(IN_FIGHT) == 0) && (ds_readbs((0x99d + 5) + 10 * (first_spell + i)) != 1))))
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
	struct dummy6 str_val = *(struct dummy6*)(p_datseg + 0xac1a);
	struct dummy5 col_str = *(struct dummy5*)(p_datseg + 0xac20);
	struct dummy3 str = *(struct dummy3*)(p_datseg + 0xac25);
	struct dummy8 col_str_val = *(struct dummy8*)(p_datseg + 0xac28);
	struct dummy12 ones = *(struct dummy12*)(p_datseg + 0xac30);

	if ((show_vals == 0) && (ds_readws(GAME_MODE) == 2)) {
		show_vals = 2;
	}

	/* only for magic users */
	if (host_readbs(hero + 0x21) < 7) {
		GUI_output(get_ltx(0x528));
		return -2;
	}

	if (show_vals == 1) {

		strcpy((char*)Real2Host(ds_readd(0xd2eb)), (char*)get_ltx(0x334));

		if (host_readbs(hero + 0x193) > 1) {
			strcat((char*)Real2Host(ds_readd(0xd2eb)), (char*)get_ltx(0x624));
		}

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0x330),
			(host_readbs(hero + 0x193) > 1) ? get_ltx(0x4c4) : get_ltx(0x4c0),
			host_readbs(hero + 0x193),
			Real2Host(ds_readd(0xd2eb)));

		answer1 = GUI_radio(Real2Host(ds_readd(DTP2)), 12,
					get_ltx(0x300), get_ltx(0x304),
					get_ltx(0x308), get_ltx(0x30c),
					get_ltx(0x310), get_ltx(0x314),
					get_ltx(0x318), get_ltx(0x31c),
					get_ltx(0x190), get_ltx(0x324),
					get_ltx(0x328), get_ltx(0x32c)) - 1;
	} else {

		strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x360));

		for (l_di = 0; l_di < 12; l_di++) {

			ds_writed(0xbf95 + 4 * l_di,
				(Bit32u)((RealPt)ds_readd(DTP2) + 50 * (l_di + 1)));

			ones.a[l_di] = can_use_spellclass(hero, l_di);

			sprintf((char*)Real2Host(ds_readd(0xbf95 + 4 * l_di)),
				(ones.a[l_di] != 0) ? str.a : col_str.a,
				get_ltx(4 * (l_di + 192)));
		}

		answer1 = GUI_radio(Real2Host(ds_readd(DTP2)), 12,
					Real2Host(ds_readd((0xbf95 + 0x00))),
					Real2Host(ds_readd((0xbf95 + 0x04))),
					Real2Host(ds_readd((0xbf95 + 0x08))),
					Real2Host(ds_readd((0xbf95 + 0x0c))),
					Real2Host(ds_readd((0xbf95 + 0x10))),
					Real2Host(ds_readd((0xbf95 + 0x14))),
					Real2Host(ds_readd((0xbf95 + 0x18))),
					Real2Host(ds_readd((0xbf95 + 0x1c))),
					Real2Host(ds_readd((0xbf95 + 0x20))),
					Real2Host(ds_readd((0xbf95 + 0x24))),
					Real2Host(ds_readd((0xbf95 + 0x28))),
					Real2Host(ds_readd((0xbf95 + 0x2c)))) - 1;
	}

	if (answer1 != -2) {

		if (!ones.a[answer1]) {
			/* this cant use any spells of this class */

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0x8bc),
				(char*)hero + 0x10);

			GUI_output(Real2Host(ds_readd(DTP2)));

			retval = -2;
		} else {

			first_spell = ds_readbs(0xd03 + 2 * answer1);

			for (l_di = 0; l_di < ds_readbs(0xd04 + 2 * answer1); l_di++) {

				ds_writed(0xbf95 + 4 * l_di,
					(Bit32u)((RealPt)ds_readd(DTP2) + 50 * (l_di)));


				if (show_vals == 1) {

					sprintf((char*)Real2Host(ds_readd(0xbf95 + 4 * l_di)),
						(char*)str_val.a,
						(char*)get_ltx(4 * (first_spell + l_di + 106)),
						host_readbs(hero + 0x13d + first_spell + l_di));
				} else if (
					(((ds_readw(IN_FIGHT) != 0) && (ds_readbs((0x99d + 5) + 10 * (first_spell + l_di)) == 1)) ||
					((ds_readw(IN_FIGHT) == 0) && (ds_readbs((0x99d + 5) + 10 * (first_spell + l_di)) != 1))) &&
					(host_readbs(hero + 0x13d + first_spell + l_di) >= -5))
				{

					if (show_vals == 2) {
						sprintf((char*)Real2Host(ds_readd(0xbf95 + 4 * l_di)),
							(char*)str_val.a,
							(char*)get_ltx(4 * (first_spell + l_di + 106)),
							host_readbs(hero + 0x13d + first_spell + l_di));
					} else {
						sprintf((char*)Real2Host(ds_readd(0xbf95 + 4 * l_di)),
							(char*)str.a,
							(char*)get_ltx(4 * (first_spell + l_di + 106)));
					}
				} else if (show_vals == 2) {
					sprintf((char*)Real2Host(ds_readd(0xbf95 + 4 * l_di)),
						(char*)col_str_val.a,
						(char*)get_ltx(4 * (first_spell + l_di + 106)),
						host_readbs(hero + 0x13d + first_spell + l_di));
				} else {
					sprintf((char*)Real2Host(ds_readd(0xbf95 + 4 * l_di)),
						(char*)col_str.a,
						(char*)get_ltx(4 * (first_spell + l_di + 106)));
				}
			}

			retval = GUI_radio(get_ltx(0x364), ds_readbs(0xd04 + 2 * answer1),
					Real2Host(ds_readd((0xbf95 + 0x00))),
					Real2Host(ds_readd((0xbf95 + 0x04))),
					Real2Host(ds_readd((0xbf95 + 0x08))),
					Real2Host(ds_readd((0xbf95 + 0x0c))),
					Real2Host(ds_readd((0xbf95 + 0x10))),
					Real2Host(ds_readd((0xbf95 + 0x14))),
					Real2Host(ds_readd((0xbf95 + 0x18))),
					Real2Host(ds_readd((0xbf95 + 0x1c))),
					Real2Host(ds_readd((0xbf95 + 0x20))),
					Real2Host(ds_readd((0xbf95 + 0x24))),
					Real2Host(ds_readd((0xbf95 + 0x28))),
					Real2Host(ds_readd((0xbf95 + 0x2c))),
					Real2Host(ds_readd((0xbf95 + 0x30))),
					Real2Host(ds_readd((0xbf95 + 0x34))),
					Real2Host(ds_readd((0xbf95 + 0x38))),
					Real2Host(ds_readd((0xbf95 + 0x3c))));

			if (retval != -1) {
				if ((host_readbs((retval + first_spell) + hero + 0x13c) < -5) &&
					(show_vals == 0))
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0x8c0),
						(char*)hero + 0x10);

					GUI_output(Real2Host(ds_readd(DTP2)));
					retval = -1;
				} else {
					retval += first_spell -1;
				}
			}
		}

		if (retval > 0) {
			if ((ds_readw(IN_FIGHT) == 0) &&
				(ds_readbs((0x99d + 5) + 10 * retval) == 1) &&
				(show_vals == 0))
			{
				GUI_output(get_ltx(0x93c));
				retval = -2;
			} else {
				if ((ds_readw(IN_FIGHT) != 0) &&
					(ds_readbs((0x99d + 5) + 10 * retval) == -1))
				{
					GUI_output(get_ltx(0x940));
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
	test_spell - makes a spell test
*/
signed short test_spell(Bit8u *hero, signed short spell_nr, signed char bonus)
{
	signed short retval;
	Bit8u *spell_desc;

	/* check if class is magic user */
	if ((host_readbs(hero + 0x21) < 7) || (check_hero(hero) == 0)) {
		return 0;
	}
	/* check if spell skill >= -5 */
	if (host_readbs(hero + spell_nr + 0x13d) < -5)
		return 0;
	/* check if hero has enough AE */
	if (get_spell_cost(spell_nr, 0) > host_readws(hero + 0x64))
		return -99;

	spell_desc = p_datseg + spell_nr * 10 + 0x99d;

	if (host_readb(spell_desc + 0x9) != 0) {

		if (host_readbs(hero + 0x86) >= 10) {

			bonus += ds_readbs(host_readbs(hero + 0x86) * 62 + 0xd0f8);

			if (test_bit6(p_datseg + host_readbs(hero + 0x86) * 62 + 0xd110)) {
				return 0;
			}
		} else {
			bonus += host_readbs(get_hero(host_readbs(hero + 0x86) - 1) + 0x66);
		}
	}

	if ((spell_nr >= 1) && (spell_nr <= 85)) {

#if !defined(__BORLANDC__)
		D1_INFO("Zauberprobe : %s %+d ", names_spell[spell_nr], bonus);
#endif

		bonus -= host_readbs(hero + spell_nr + 0x13d);

		retval = test_attrib3(hero, host_readbs(spell_desc+1),
			host_readbs(spell_desc+2), host_readbs(spell_desc+3), bonus);

		if (retval == -99) {
			retval = -1;
		}

		return retval;
	}

	return 0;
}

/**
	test_spell_group - makes a spell test for all magic users in the current group
*/
signed short test_spell_group(signed short spell, signed char bonus)
{

	Bit8u *hero_i = get_hero(0);
	signed short i;

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {

		/* Check class is magicuser */
		if ((host_readbs(hero_i + 0x21) >= 7) &&
			/* Check class  BOGUS */
			(host_readbs(hero_i + 0x21) != 0) &&
			/* Check in group */
			(host_readbs(hero_i + 0x87) == ds_readbs(CURRENT_GROUP)) &&
			/* Check if dead */
			!hero_dead(hero_i))
		{

			if (test_spell(hero_i, spell, bonus) > 0) {
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
	answer = select_hero_ok(get_ltx(0x4f4));

	if (answer != -1) {
		/* valid answer => cast spell */
/*		return use_spell(get_hero(answer), 1, 0); */
		return use_spell((RealPt)ds_readd(HEROS) + 0x6da * answer, 1, 0);
	}

	/* abort with error message */
	return -2;
}

signed short use_spell(RealPt hero, signed short a2, signed char bonus)
{
	signed short retval = 1;
	signed short l_di;
	signed short ae_cost;
	signed short bak;
	Bit8u *ptr;
	void (*func)(void);
	signed short l4;

	if (!check_hero(Real2Host(hero)) && !hero_cursed(Real2Host(hero))) {

		return 0;
	}

	bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);

	if (a2 == 1) {
		l_di = select_spell(Real2Host(hero), 0);

		if (l_di > 0) {
			/* pointer to the spell description */
			ptr = p_datseg + 0x99d + 10 * l_di;
			/* reset the spelltarget of the hero */
			host_writeb(Real2Host(hero) + 0x86, 0);

			if ((host_readbs(ptr + 0x7) != 0) && (host_readbs(ptr + 7) != 4)) {

				if (host_readbs(ptr + 0x7) == 1) {

					GUI_output(get_ltx(0x3a8));

				} else {

					host_writeb(Real2Host(hero) + 0x86, select_hero_from_group(get_ltx(0xbc)) + 1);

					if (host_readbs(Real2Host(hero) + 0x86) <= 0) {
						l_di = -1;
					}
				}
			}
		}

	} else {
		l_di = host_readbs(Real2Host(hero) + 0x85);
	}

	if (l_di > 0) {

		/* pointer to the spell description */
		ptr = p_datseg + 0x99d + 10 * l_di;

		if ((ds_readws(IN_FIGHT) == 0) && (host_readbs(ptr + 5) == 1)) {
			GUI_output(get_ltx(0x93c));
			retval = 0;

		} else if ((ds_readws(IN_FIGHT) != 0) && (host_readbs(ptr + 5) == -1)) {
			GUI_output(get_ltx(0x940));
			retval = 0;
		}

		if (retval) {

			ds_writew(0xe5b2, test_spell(Real2Host(hero), l_di, bonus));

			if (ds_readws(0xe5b2) == -99) {

				/* prepare output */
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_ltx(0x97c),
					(char*)Real2Host(hero) + 0x10);

				if (ds_readws(IN_FIGHT) == 0) {
					GUI_output(Real2Host(ds_readd(DTP2)));
				}

				retval = -1;

			} else if ((ds_readws(0xe5b2) <= 0) || (ds_readds(INGAME_TIMERS) > 0)) {

				strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x978));

				sub_ae_splash(Real2Host(hero), get_spell_cost(l_di, 1));

				if (ds_readws(IN_FIGHT) == 0) {
					GUI_output(Real2Host(ds_readd(DTP2)));
				}

				retval = 0;
			} else {
				/* set global spelluser variable */
				ds_writed(SPELLUSER, (Bit32u)hero);

				ae_cost = get_spell_cost(l_di, 0);

				ds_writew(0xac0e, -1);

				host_writeb(Real2Host(ds_readd(DTP2)), 0);

				l4 = ds_readws(0x26bf);

				load_buffer_1(222);
#if !defined(__BORLANDC__)
				func = spellhandler[l_di];
#else
				func = (void (*)(void))ds_readd(0xdbb + 4 * l_di);
#endif
				func();

				if ((l4 != -1) && (l4 != 222)) {
					load_buffer_1(l4);
				}

				retval = 1;

				if (ds_readws(0xac0e) == 0) {
					retval = -1;

					if (!host_readbs(Real2Host(ds_readd(DTP2)))) {
						strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x978));
					}
				} else if (ds_readws(0xac0e) == -2) {

					strcpy((char*)Real2Host(ds_readd(DTP2)), (char*)get_ltx(0x978));
					sub_ae_splash(Real2Host(hero), get_spell_cost(l_di, 1));
					retval = 0;
				} else if (ds_readws(0xac0e) != -1) {
					sub_ae_splash(Real2Host(hero), ds_readws(0xac0e));
				} else {
					sub_ae_splash(Real2Host(hero), ae_cost);
				}

				if (ds_readws(IN_FIGHT) == 0) {

					GUI_output(Real2Host(ds_readd(DTP2)));

					if (retval > 0) {
						play_voc(305);

						if ((host_readbs(Real2Host(hero) + 0x86) < 10) &&
							(host_readbs(Real2Host(hero) + 0x86) > 0) &&
							(ds_readbs(0x2845) == 0))
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

	ds_writew(TEXTBOX_WIDTH, bak);

	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
