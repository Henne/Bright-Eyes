/*
        Rewrite of DSA1 v3.02_de functions of seg098 (Magic)
        Functions rewritten: 7/11
*/
#include <stdlib.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg029.h"
#include "seg041.h"

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
short get_spell_cost(unsigned short spell, unsigned short half_cost)
{
	signed char ret;

	ret = ds_readb(0x99d + 4 + spell * 10);

	if (half_cost != 0) {
		if (ret == -1)
			ret = (signed char)random_interval(5, 10);
		else
			ret = abs(ret) / 2;

		if (ret == 0)
			ret = 1;
	}
	return ret;
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

#if !defined(__BORLANDC__)
}
#endif
