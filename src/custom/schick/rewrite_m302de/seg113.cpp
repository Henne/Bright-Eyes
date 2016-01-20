/*
 *	Rewrite of DSA1 v3.02_de functions of seg113 (travel events 5 / 10)
 *	Functions rewritten: 17/22
*/

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg028.h"
#include "seg029.h"
#include "seg032.h"
#include "seg047.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
/* The Tatzelwurm */
void tevent_080(void)
{
	signed short answer;
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());

	if ((!ds_readb(0x3de4) && (test_skill(hero, 31, 4) > 0)) ||
			ds_readb(0x3de4) != 0)
	{
		ds_writeb(0x3de4, 1);
		ds_writed(0x3e20, 0);

		TRV_found_replenish_place(0);

		hero = Real2Host(ds_readd(0x3e20));

		if ((hero && !ds_readb(TATZELWURM) && test_skill(hero, 26, 5) > 0) ||
			ds_readb(TATZELWURM) == 1)
		{
			ds_writeb(TATZELWURM, 1);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_city(0x15c),
				(char*)hero + 0x10);

			do {
				answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
							get_city(0x160),
							get_city(0x164));
			} while (answer == -1);

			if (answer == 1) {

				timewarp(HOURS(1));

				do {
					answer = GUI_radio(get_city(0x168), 2,
								get_city(0x16c),
								get_city(0x170));
				} while (answer == -1);

				if (answer == 2) {

					timewarp(HOURS(1));

					do {
						answer = GUI_radio(get_city(0x174), 2,
									get_city(0x178),
									get_city(0x17c));
					} while (answer == -1);

					if (answer == 1) {

						GUI_output(get_city(0x180));

						ds_writeb(TATZELWURM, 2);

						add_hero_ap_all(20);
					}
				}
			}
		}
	}
}

/* Borlandified and identical */
void tevent_081(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 2) > 0 && !ds_readb(0x3de6)) ||
		 ds_readb(0x3de6) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(0x3de6, 1);
	}
}

/* Borlandified and identical */
void tevent_082(void)
{
	TRV_cross_a_ford(get_dtp(0xcc), 30, 1);
}

/* Borlandified and identical */
void tevent_083(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 29, 6) > 0 && !ds_readb(0x3de7)) ||
		 ds_readb(0x3de7) != 0)
	{
		ds_writeb(0x66d0, 61);
		TRV_found_herb_place(0);
		ds_writeb(0x66d0, -1);
		ds_writeb(0x3de7, 1);
	}
}

/* Borlandified and identical */
void tevent_084(void)
{
	signed short answer;

	if (!ds_readb(0x3de8)) {

		load_in_head(44);

		do {

			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						get_city(0x184), 2,
						get_city(0x188),
						get_city(0x18c));
		} while (answer == -1);

		if (answer == 1) {

			if (!do_fight(188)) {
				ds_writeb(0x3de8, 1);
			}
		} else {

			if (test_skill(Real2Host(get_first_hero_available_in_group()), 37, 5) <= 0)
			{
				ds_writeb(0x26ac, 1);

				if (!do_fight(188)) {
					ds_writeb(0x3de8, 1);
				}
			}
		}
	}
}

/* Borlandified and identical */
void tevent_085(void)
{
	Bit8u *hero = Real2Host(get_first_hero_available_in_group());

	if ((test_skill(hero, 31, 4) > 0 && !ds_readb(0x3de9)) ||
		 ds_readb(0x3de9) != 0)
	{
		ds_writeb(0x3de9, 1);

		if ((test_skill(hero, 29, 6) > 0 && !ds_readb(0x3dea)) ||
			 ds_readb(0x3dea) != 0)
		{
			ds_writeb(0x3dea, 1);
			ds_writebs(0x66d0, 124);
			TRV_found_camp_place(2);
			ds_writebs(0x66d0, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

/* Borlandified and identical */
void tevent_086(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	load_in_head(58);

	do {
		answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
					get_city(0x190), 2,
					get_city(0x194), get_city(0x198));
	} while (answer == -1);

	if (answer == 1) {

		hero = get_hero(0);

		for (i = 0; i <= 6; i++, hero += 0x6da) {

			if (host_readbs(hero + 0x21) != 0 &&
				host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP))
			{
				sub_hero_le(hero, 2);
			}
		}

		GUI_dialog_na(0, get_city(0x19c));

		if (!ds_readb(0x3deb)) {

			add_hero_ap_all(ds_writeb(0x3deb, 15));
		}

	} else {

		GUI_dialog_na(0, get_city(0x1a0));

		if (!ds_readb(0x3deb)) {

			add_hero_ap_all(ds_writeb(0x3deb, 5));
		}
	}
}

/* Borlandified and identical */
void tevent_088(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), 31, 3) > 0 && !ds_readb(0x3dec)) ||
		 ds_readb(0x3dec) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(0x3dec, 1);
	}
}

/* Borlandified and identical */
void tevent_089(void)
{
	TRV_cross_a_ford(get_dtp(0xcc), 40, 2);
}

/* Borlandified and identical */
void tevent_097(void)
{
	if (TRV_enter_hut_question()) {

		ds_writeb(LOCATION, 6);
		do_location();
		ds_writeb(LOCATION, 0);
	}
}

/**
 * \brief travelevent 98: a gap
 *
 * disappeared heros can be found in the efferd temple in liskor
 */
/* Borlandified and identical */
void tevent_098(void)
{
	signed short answer;
	signed short repeat;
	signed short i;
	signed short hero_pos;
	Bit8u *hero;

	i = get_first_hero_with_item(121) != -1 || get_first_hero_with_item(32) != -1 ? 3 : 2;

	do {
		answer = GUI_radio(get_city(0x6c), (signed char)i,
				get_city(0x70),
				get_city(0x74),
				get_city(0x78));
	} while (answer == -1);

	do {
		repeat = 0;

		if (answer == 1) {

			hero = get_hero(0);

			for (i = 0; i <= 6; i++, hero += 0x6da) {

				if (host_readbs(hero + 0x21) != 0 &&
					host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero))
				{

					if (test_skill(hero, 9, 0) > 0) {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_city(0x7c),
							(char*)hero + 0x10);

						GUI_output(Real2Host(ds_readd(DTP2)));

					} else {

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_city(0x80),
							(char*)hero + 0x10);

						GUI_output(Real2Host(ds_readd(DTP2)));

						hero_disappear(hero, i, 33);
					}
				}
			}

		} else if (answer == 2) {

			GUI_output(get_city(0x84));

			/* Original-Bug: hero not initialized */
#ifdef M302de_ORIGINAL_BUGFIX
			/* Original-Bugfix: take the leader of the group */
			hero = Real2Host(get_first_hero_available_in_group());
#endif
			hero_disease_test(hero, 2, 20 - (host_readbs(hero + 0x47) + host_readbs(hero + 0x48)));

			loose_random_item(hero, 1, get_ltx(0x7e8));

			ds_writeb(LOCATION, 6);
			do_location();
			ds_writeb(LOCATION, 0);

			TRV_load_textfile(-1);
		} else {

			hero = get_hero(hero_pos = select_hero_ok_forced(get_city(0x88)));

			if (test_skill(hero, 9, 0) > 0) {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x8c),
					(char*)hero + 0x10);

				GUI_output(Real2Host(ds_readd(DTP2)));

				hero = get_hero(0);
				for (i = 0; i <= 6; i++, hero += 0x6da) {

					if (i != hero_pos &&
						host_readbs(hero + 0x21) != 0 &&
						host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero))
					{
						if (test_attrib(hero, 4, -4) > 0) {

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_city(0x90),
								(char*)hero + 0x10);

							GUI_output(Real2Host(ds_readd(DTP2)));

						} else {

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_city(0x80),
								(char*)hero + 0x10);

							GUI_output(Real2Host(ds_readd(DTP2)));

							hero_disappear(hero, i, 33);
						}
					}
				}
			} else {

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_city(0x80),
					(char*)hero + 0x10);

				GUI_output(Real2Host(ds_readd(DTP2)));

				hero_disappear(hero, i, 33);

				do {
					answer = GUI_radio(get_city(0x94), 3,
								get_city(0x70),
								get_city(0x74),
								get_city(0x78));
				} while (answer == -1);

				repeat = 1;
			}
		}

	} while (repeat);
}

/* Borlandified and identical */
void hero_disappear(Bit8u *hero, unsigned short pos, signed short type)
{

	/* decrement the number of heroes */
	dec_ds_bs_post(0x2d3c);

	/* load a new savegame if no hero is present */
	if (!ds_readbs(0x2d3c)) {
		ds_writew(0xc3c1, 1);
	}

	/* decrement group counter */
	dec_ds_bs_post(0x2d36 + ds_readbs(CURRENT_GROUP));

	/* write type to character sheet */
	host_writeb(hero + 0x88, (signed char)type);

	/* reset position in group */
	host_writeb(hero + 0x8a, 0);

	if (pos == 6) {
		/* NPC */
		save_npc(0xe2 + host_readbs(get_hero(6) + 0x89));

		/* reset NPC timer */
		ds_writebs(0x3602 + host_readbs(get_hero(6) + 0x89), -1);
	} else {
		/* Regular Hero */
		write_chr_temp(pos);
	}

	/* set typus to 0 */
	host_writeb(hero + 0x21, 0);

	if (type != -2) {
		draw_main_screen();
		init_ani(2);
		ds_writew(0x2846, 1);
	}

	/* set flag to check all heros */
	ds_writeb(CHECK_PARTY, 1);
}

/* Borlandified and identical */
/* fight with 4 harpyes */
void tevent_099(void)
{
	signed short answer;

	if (!ds_readb(0x3df2)) {

		do {
			answer = GUI_radio(get_city(0x98), 2,
						get_city(0x9c),
						get_city(0xa0));
		} while (answer == -1);

		/* Original-Bug: The 2nd option "try to flee (or avoid the fight)" is not a try.
		 *		 To trigger the code you have to flee from the fight mode,
		 *		 then the skill test is done, and if this one fails you reenter this fight.
		*/


		if (answer == 1) {

			if (!do_fight(189)) {
				ds_writeb(0x3df2, 1);
#ifdef M302de_ORIGINAL_BUGFIX
			}
#endif
		} else {

			if (test_skill(Real2Host(get_first_hero_available_in_group()), 37, 4) <= 0) {

				ds_writeb(0x26ac, 1);

				if (!do_fight(189)) {
					ds_writeb(0x3df2, 1);
				}
			}
#ifndef M302de_ORIGINAL_BUGFIX
			}
#endif
		}
	}
}

/* Borlandified and identical */
/* fight with 2-6 harpyes */
void tevent_101(void)
{
	signed short answer;
	signed short mod;

	if (!ds_readb(0x3df4)) {

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_city(0xa4),
			(mod = random_schick(4) + 2));


		do {
			answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
						get_city(0xa8),
						get_city(0xac));
		} while (answer == -1);

		ds_writew(MAX_ENEMIES, mod);

		/* Original-Bugfix: see description in tevent_099() */
		if (answer == 1) {

			if (!do_fight(190)) {
				ds_writeb(0x3df4, 1);
#ifdef M302de_ORIGINAL_BUGFIX
			}
#endif
		} else {

			if (test_skill(Real2Host(get_first_hero_available_in_group()), 37, mod) <= 0) {

				ds_writeb(0x26ac, 1);

				if (!do_fight(190)) {
					ds_writeb(0x3df4, 1);
				}
			}
#ifndef M302de_ORIGINAL_BUGFIX
			}
#endif
		}
	}
}

/* Borlandified and identical */
void tevent_102(void)
{
	if (TRV_enter_hut_question())
	{
		ds_writebs(LOCATION, 6);
		do_location();
		ds_writebs(LOCATION, 0);
	}
}

/* Borlandified and identical */
void tevent_103(void)
{
	signed short answer;
	Bit8u *hero;

	/* Original-Bug: save the address instead the return value of a function as a hero */
#ifdef M302de_ORIGINAL_BUGFIX
	hero = Real2Host(get_first_hero_available_in_group());
#else
	hero = Real2Host(get_first_hero_available_in_group);
#endif

	do {
		answer = GUI_radio(get_city(0xb0), 2,
					get_city(0xb4),
					get_city(0xb8));
	} while (answer == -1);

	if (answer == 1) {

		if (test_skill(hero, 28, 2) > 0) {

			timewarp(HOURS(4));

			GUI_output(get_city(0xbc));
		} else {

			timewarp(HOURS(6));

			GUI_output(get_city(0xc4));

			answer = -1;
		}
	} else {

		if (test_skill(hero, 28, 3) > 0) {

			timewarp(HOURS(6));

			GUI_output(get_city(0xc0));
		} else {

			timewarp(HOURS(8));

			GUI_output(get_city(0xc8));

			answer = -1;
		}
	}

	if (answer == -1) {

		if (test_skill(hero, 28, 4) > 0) {

			timewarp(HOURS(4));

			GUI_output(get_city(0xcc));
		} else {

			timewarp(HOURS(6));

			GUI_output(get_city(0xd0));

			ds_writebs(LOCATION, 6);
			do_location();
			ds_writebs(LOCATION, 0);

			TRV_load_textfile(-1);

			GUI_output(get_city(0xd4));
		}
	}
}

/* Borlandified and identical */
void tevent_104(void)
{
	signed short l_si;
	signed short done;
	signed short i;
	signed short nr_heros;
	Bit8u *hero;
	signed short spell_result;

	done = 0;

	load_in_head(57);

	do {

		GUI_dialog_na(0, get_city(0xd8));

		hero = get_hero(0);

		for (i = l_si = nr_heros = 0; i <= 6; i++, hero += 0x6da)
		{
			if (host_readbs(hero + 0x21) != 0 &&
				host_readbs(hero + 0x87) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero))
			{
				nr_heros++;

				/* test for HA+0 */

				if (test_attrib(hero, 8, 0) > 0)
				{

					timewarp(MINUTES(30));

					sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
						(char*)get_city(0xdc),
						(char*)hero + 0x10);

					GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

					l_si++;
				}
			}
		}

		if (!l_si) {

			/* everything is fine */

			GUI_dialog_na(0, get_city(0xfc));
			done = 1;

		} else if (l_si == nr_heros) {

			/* all heros have failed the test */

			do {
				l_si = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
							get_city(0xe0), 2,
							get_city(0xe4), get_city(0xe8));
			} while (l_si == -1);

			if (l_si == 2) {
				/* make a camp */

				ds_writebs(LOCATION, 6);
				do_location();
				ds_writebs(LOCATION, 0);

				TRV_load_textfile(-1);
#ifdef M302de_ORIGINAL_BUGFIX
				/* Original-Bug: The dialog picture is lost when a campfight occurs. */
				load_in_head(57);
#endif

			} else {
				/* turn around */

				ds_writew(0x4336, done = 1);
			}

		} else {

			/* some heros, but not all, have failed the test */

			nr_heros = 0;

			do {

				do {
					l_si = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
								(nr_heros == 0 ? get_city(0xec) : get_city(0x15c)), 3,
								get_city(0xf0), get_city(0xf4), get_city(0xf8));
				} while (l_si == -1);

				if (l_si == 1) {

					timewarp(HOURS(2));

					GUI_dialog_na(0, get_city(0xfc));

					done = 1;

				} else if (l_si == 2) {

					hero = get_hero(select_hero_ok_forced(get_ltx(0x4f4)));

					if (host_readbs(hero + 0x21) < 7) {
						/* hero is not a spell user */
						GUI_output(get_ltx(0x528));
					} else {

						spell_result = test_spell(hero, 7, 0);

						if (spell_result > 0) {

							sub_ae_splash(hero, get_spell_cost(7, 0));

							GUI_output(get_city(0x40));

							done = 1;

						} else if (spell_result != -99) {

							sub_ae_splash(hero, get_spell_cost(7, 1));

							nr_heros = 1;

						} else {

							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ltx(0x97c),
								(char*)hero + 0x10);

							GUI_output(Real2Host(ds_readd(DTP2)));
						}

						timewarp(MINUTES(30));
					}
				} else {

					timewarp(HOURS(4));

					GUI_dialog_na(0, get_city(0xfc));

					done = 1;
				}

			} while (!done);
		}

	} while (!done);
}

#if !defined(__BORLANDC__)
}
#endif
