/*
 *	Rewrite of DSA1 v3.02_de functions of seg098 (Magic)
 *	Functions rewritten: 9/12
 */

#include <stdio.h>
#include <stdlib.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg029.h"
#include "seg041.h"
#include "seg097.h"
#include "seg098.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif


struct dummy1 {
	signed short a[5];
};

/**
 * magic_heal_ani() - show some stars on the picture of a healed hero
 * @hero:	the hero who heals
 *
*/
/* Borlandified and identical */
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
/* Borlandified and identical */
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
/* Borlandified and identical */
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
/* Borlandified and identical */
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
/* Borlandified and identical */
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

#if defined(__BORLANDC__)
/**
 * \brief use magic menu, meditation and staffspell logic
 *
 * \param hero	pointer to the hero
 *
 * \return {0, 1, 2}
 */
/* Borlandified and identical */
signed short use_magic(Bit8u *hero)
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

			if (host_readbs(hero + 0x21) != 9) {
				/* not a mage, need thonnys */


				if ((thonny_pos = get_item_pos(hero, 131)) == -1) {
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
					drop_item(hero, thonny_pos, 1);
				}

				/* adjust LE */
				if (host_readws(hero + 0x62) - host_readws(hero + 0x64)  < le) {
					le = host_readws(hero + 0x62) - host_readws(hero + 0x64);
				}

				/* spend one AE point */
				sub_ae_splash(hero, 1);

				if (test_attrib3(hero, 0, 2, 6, 0) > 0) {
					/* Success */

					if (host_readws(hero + 0x60) <= le + 8) {
						le = host_readws(hero + 0x60) - 8;
					}

					sub_hero_le(hero, le + 3);
					add_hero_ae(hero, le);
				} else {
					/* Failed, print only a message */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_ltx(0xc6c),
						(char*)hero + 0x10);

					GUI_output(Real2Host(ds_readd(DTP2)));
				}
			}
			break;
		}
		case 2: {
			/* Staffspell */

			if (host_readbs(hero + 0x21) != 9) {
				/* only for mages */
				GUI_output(get_ltx(0x64c));
				return 0;
			}

			if (host_readbs(hero + 0x195) == 7) {
				GUI_output(get_ltx(0x53c));
			} else {

				if (ds_readbs((0x972 + 5) + 6 * host_readbs(hero + 0x195)) <= host_readws(hero + 0x64)) {
					/* check AE */

					retval = 1;

					/* Original-Bug: the second attribute is used twice here */
					if (test_attrib3(hero,
						ds_readbs((0x972 + 1) + 6 * host_readbs(hero + 0x195)),
						ds_readbs((0x972 + 2) + 6 * host_readbs(hero + 0x195)),
						ds_readbs((0x972 + 2) + 6 * host_readbs(hero + 0x195)),
						ds_readbs((0x972 + 4) + 6 * host_readbs(hero + 0x195))) > 0)
					{
						/* Success */

						/* print a message */
						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_ltx(0x54c),
							host_readbs(hero + 0x195) + 1);

						GUI_output(Real2Host(ds_readd(DTP2)));

						sub_ae_splash(hero, ds_readbs((0x972 + 5) + 6 * host_readbs(hero + 0x195)));

						sub_ptr_ws(hero + 0x62,	ds_readbs((0x972 + 6) + 6 * host_readbs(hero + 0x195)));

						/* Staffspell level +1 */
						inc_ptr_bs(hero + 0x195);

						/* set the timer */
						host_writed(hero + 0x8f, 0xfd20);

						/* let some time pass */
						timewarp(0x6978);
					} else {
						/* Failed */
						GUI_output(get_ltx(0x548));

						/* only half of the AE costs */
						sub_ae_splash(hero, ds_readbs((0x972 + 5) + 6 * host_readbs(hero + 0x195)) / 2);

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
			select_spell(hero, 1, 0);
			break;
		}
		}
	}

	return retval;
}
#endif

/**
 * \brief check if a spellclass can be used
 *
 * \param hero		pointer to the hero
 * \param spellclass_nr	the number of the spellclass
 *
 * \return 0 = can't be used, 1 = can be used
*/
/* Borlandified and identical */
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

/**
	test_spell - makes a spell test
*/
short test_spell(Bit8u *hero, unsigned short spell, signed char bonus)
{
	Bit8u *spell_desc;
	signed short retval;

	/* check if class is magic user */
	if (host_readb(hero+0x21) < 7)
		return 0;
	/* check hero */
	if (check_hero(hero) == 0)
		return 0;
	/* check if spell skill >= -5 */
	if (host_readbs(hero + spell + 0x13d) < -5)
		return 0;
	/* check if hero has enough AE */
	if (get_spell_cost(spell, 0) > host_readw(hero+0x64))
		return -99;

	spell_desc = p_datseg + spell * 10 + 0x99d;

	if (host_readb(spell_desc+0x9) != 0) {

		unsigned short addr;

		if ((char)host_readb(hero+0x86) >= 10) {

			addr = (char)host_readb(hero+0x86) * 62 + 0xd0f8;
			bonus += ds_readb(addr);

			addr = (char)host_readb(hero+0x86) * 62 + 0xd110;
			if ((ds_readb(addr) >> 6) & 1)
				return 0;
		} else {
			addr = ((char)host_readb(hero+0x86) - 1) * 0x6da;
			bonus += host_readb(p_datseg + HEROS + addr + 0x66);

		}
	}

	if (spell < 1 || spell > 85)
		return 0;

#if !defined(__BORLANDC__)
	D1_INFO("Zauberprobe : %s %+d ", names_spell[spell], bonus);
#endif

	bonus -= host_readbs(hero + spell + 0x13d);

	retval = test_attrib3(hero, host_readb(spell_desc+1),
		host_readb(spell_desc+2), host_readb(spell_desc+3), bonus);

	if (retval == -99)
		return -1;

	return retval;
}

/**
	test_spell_group - makes a spell test for all magic users in the current group
*/
unsigned short test_spell_group(unsigned short spell, signed char bonus)
{

	Bit8u *hero_i = get_hero(0);
	short i;

	for (i = 0; i <= 6; i++, hero_i += 0x6da) {
		/* Check class is magicuser */
		if (host_readb(hero_i + 0x21) < 7)
			continue;
		/* Check class  BOGUS */
		if (host_readb(hero_i + 0x21) == 0)
			continue;
		/* Check in group */
		if (host_readb(hero_i + 0x87) != ds_readb(CURRENT_GROUP))
			continue;
		/* Check if dead */
		if (hero_dead(hero_i))
			continue;

		if (test_spell(hero_i, spell, bonus) > 0)
			return 1;
	}
	return 0;
}

#if defined(__BORLANDC__)
signed short select_spell(Bit8u *hero, signed short spell_nr, signed char bonus)
{
}
#endif

#if !defined(__BORLANDC__)
}
#endif
