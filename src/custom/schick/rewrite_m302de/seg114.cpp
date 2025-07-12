/**
 *	Rewrite of DSA1 v3.02_de functions of seg114 (travel events 6 / 10)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg114.cpp
 */
#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* wall of rock */
void tevent_110(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_tx2(79), 2,
					get_tx2(80),
					get_tx2(81));
	} while (answer == -1);

	if (answer == 1)
	{
		/* try to climb */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, TA_KLETTERN, 0) <= 0)
			{
				/* skill test failed */

				if (get_first_hero_with_item(121) != -1)
				{
					/* one hero in the group has a rope */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(85),
						(char*)hero + HERO_NAME2);

					sub_hero_le(hero, random_schick(10));

				} else {
					/* all heroes in the group have no ropes */
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(86),
						(char*)hero + HERO_NAME2);

					sub_hero_le(hero, random_schick(16) + 4);
				}

				GUI_output(Real2Host(ds_readd(DTP2)));
			}

		}

		GUI_output(get_tx2(84));

	} else {

		/* try to walk arround */

		if (test_skill(Real2Host(get_first_hero_available_in_group()), TA_ORIENTIERUNG, 0) > 0)
		{
			/* success */
			timewarp(HOURS(4));
			GUI_output(get_tx2(82));
		} else {
			/* fail */
			timewarp(DAYS(1) + HOURS(6));
			GUI_output(get_tx2(83));
		}
	}
}

/* hunt a rhino */
void tevent_111(void)
{
	signed short counter;
	signed short answer;
	signed short i;
	signed short ret_skill_test2;
	signed short ret_skill_test3;
	signed short unlucky_tests;
	Bit8u *hero;

	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_FAEHRTENSUCHEN, 1) > 0 && !ds_readb(TEVENT111_FLAG)) ||
		ds_readb(TEVENT111_FLAG) == 1)
	{
		ds_writeb(TEVENT111_FLAG, 1);
		ds_writeb(EVENT_ANI_BUSY, 1);

		load_ani(33);
		draw_main_screen();
		init_ani(0);

		GUI_output(get_tx2(0));

		hero = get_hero(0);
		for (i = counter = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, TA_SCHLEICHEN, -5) <= 0)
			{
				counter++;
			}
		}

		if (counter)
		{
			do {
				answer = GUI_radio(get_tx2(1), 2,
							get_tx2(7),
							get_tx2(8));
			} while (answer == -1);

			if (answer == 1)
			{
				GUI_output(get_tx2(9));

				add_hero_ap_all(7);

				timewarp(HOURS(2));
			} else {
				add_hero_ap_all(5);
			}

		} else {

			hero = get_hero(select_hero_ok_forced(get_tx2(2)));

			if (test_skill(hero, TA_SCHLEICHEN, 0) <= 0)
			{
				/* skill test failed */
				do {
					answer = GUI_radio(get_tx2(1), 2,
								get_tx2(7),
								get_tx2(8));
				} while (answer == -1);

				if (answer == 1)
				{
					GUI_output(get_tx2(9));

					add_hero_ap_all(7);

					timewarp(HOURS(2));
				} else {
					add_hero_ap_all(5);
				}

			} else {
				/* skill test succeeded */

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx2(3),
					(char*)hero + HERO_NAME2);

				GUI_input(Real2Host(ds_readd(DTP2)), counter = unlucky_tests = 0);

				if ((i = test_skill(hero, TA_SCHUSSWAFFEN, 12)) > 0) counter++;
				if ((ret_skill_test2 = test_skill(hero, TA_SCHUSSWAFFEN, 12)) > 0) counter++;
				if ((ret_skill_test3 = test_skill(hero, TA_SCHUSSWAFFEN, 12)) > 0) counter++;

				if (i == 99) unlucky_tests++;
				if (ret_skill_test2 == 99) unlucky_tests++;
				if (ret_skill_test3 == 99) unlucky_tests++;

				if (counter == 3 || unlucky_tests >= 2)
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(4),
						(char*)hero + HERO_NAME2);

					GUI_output(Real2Host(ds_readd(DTP2)));

					add_hero_ap_all(5);

					add_hero_ap(hero, 20);

					timewarp(HOURS(3));

					get_item(45, 1, 200);

					ds_writeb(TEVENT111_FLAG, 2);

				} else if (counter == 2 || unlucky_tests == 1 || counter == 1)
				{

					do {
						answer = GUI_radio(get_tx2(5), 2,
									get_tx2(10),
									get_tx2(11));
					} while (answer == -1);

					if (answer == 1)
					{
						GUI_output(get_tx2(12));

						add_hero_ap_all(7);

						add_hero_ap(hero, 5);
					} else {

						GUI_output(get_tx2(13));

						add_hero_ap_all(15);

						add_hero_ap(hero, 5);

						timewarp(HOURS(4));

						hero = get_hero(0);
						for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
						{
							if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
								host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
								!hero_dead(hero) &&
								test_attrib(hero, ATTRIB_GE, 2) <= 0)
							{
								/* attribute test failed */
								sub_hero_le(hero, random_schick(10));
							}
						}

						get_item(45, 1, 180);
						ds_writeb(TEVENT111_FLAG, 2);
					}

				} else {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_tx2(6),
						(char*)hero + HERO_NAME2);

					do {
						answer = GUI_radio(Real2Host(ds_readd(DTP2)), 2,
									get_tx2(7),
									get_tx2(8));
					} while (answer == -1);

					if (answer == 1)
					{
						GUI_output(get_tx2(9));

						add_hero_ap_all(7);

						timewarp(HOURS(2));
					} else {
						add_hero_ap_all(5);
					}
				}
			}
		}

		set_var_to_zero();
		ds_writew(REQUEST_REFRESH, 1);
	}

	ds_writeb(EVENT_ANI_BUSY, 0);
}

/* a camp place */
void tevent_112(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT112_FLAG)) ||
		ds_readb(TEVENT112_FLAG) != 0)
	{
		ds_writeb(TEVENT112_FLAG, 1);

		if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 2) > 0 && !ds_readb(TEVENT112_HERB_FLAG)) ||
			ds_readb(TEVENT112_HERB_FLAG) != 0)
		{
			ds_writeb(TEVENT112_HERB_FLAG, 1);

			ds_writeb(GATHER_HERBS_SPECIAL, 131);
			TRV_found_camp_place(2);
			ds_writeb(GATHER_HERBS_SPECIAL, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

/* FIRUN-temple in the wilderness */
void tevent_113(void)
{
	signed short answer;
	Bit32s est_old;
	Bit32s est_diff;

	do {
		answer = GUI_radio(get_tx2(14), 2,
					get_tx2(15),
					get_tx2(16));
	} while (answer == -1);

	if (answer == 1)
	{
		GUI_output(get_tx2(17));

		/* enter the temple */
		ds_writew(TYPEINDEX, 58);
		est_old = ds_readds(GODS_ESTIMATION + 4 * 7);
		ds_writeb(LOCATION, LOCATION_TEMPLE);
		do_location();
		TRV_load_textfile(-1);

		GUI_output(get_tx2(18));

		/* calculate the difference of the estimation */
		est_diff = ds_readds(GODS_ESTIMATION + 4 * 7) - est_old;

		ds_writew(TEXTBOX_WIDTH, 3);

		/* the more money you spend, the better the group will sleep */
		ds_writew(WILDCAMP_SLEEP_QUALITY, (est_diff <= 10 ? 1 :
					(est_diff <= 50 ? 2 :
					(est_diff <= 100 ? 3 : 4))));

		ds_writeb(LOCATION, LOCATION_WILDCAMP);
		do_location();
		ds_writeb(LOCATION, 0);

		ds_writew(TEXTBOX_WIDTH, 9);
		ds_writew(REQUEST_REFRESH, 2);
	}
}

/* a swamp */
void tevent_114(void)
{
	signed short i;
	signed short answer;
	signed short done;
	signed short j;
	Bit8u *hero;

	done = 0;

	do {

		do {
			answer = GUI_radio(get_tx2(19), 2,
						get_tx2(20),
						get_tx2(21));
		} while (answer == -1);

		if (answer == 1)
		{
			do {
				answer = GUI_radio(get_tx2(28), 2,
							get_tx2(29),
							get_tx2(30));
			} while (answer == -1);

			if (answer == 1)
			{
				/* turn around */
				ds_writew(TRV_RETURN, done = 1);
			}

			if (answer == 2)
			{
				/* go on careful */
				GUI_output(get_tx2(22));
			}

		} else {
			GUI_output(get_tx2(22));
		}

		if (done == 0)
		{
			/* walk through the swamp */
			for (j = 0; j < 2; j++)
			{
				for (i = 0, hero = get_hero(0); i <= 6; i++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero) &&
						test_attrib(hero, ATTRIB_GE, 4) <= 0)
					{
						/* attrib test failed */
						timewarp(MINUTES(30));

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_tx2(23),
							(char*)hero + HERO_NAME2,
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

						GUI_output(Real2Host(ds_readd(DTP2)));

						sub_hero_le(hero, random_schick(8));

						loose_random_item(hero, 50, get_ttx(506));
						loose_random_item(hero, 50, get_ttx(506));
						loose_random_item(hero, 50, get_ttx(506));
					}
				}
			}

			if (!ds_readb(TEVENT114_OLIMONE_FLAG))
			{
				/* meet OLIMONE */
				GUI_output(get_tx2(24));

				load_in_head(12);

				GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(52), get_tx2(25), 0);
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(52), get_tx2(26), 0);

				/* get RECIPE FOR POTENT HEALING POTION */
				get_item(240, 1, 1);

				/* ORIGINAL-BUG: this item could have been not taken => get_hero(-1) => SEGFAULT */
				hero = get_hero(get_first_hero_with_item(240));

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_tx2(27),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				timewarp(HOURS(8));

				for (i = 0, hero = get_hero(0); i <= 6; i++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero))
					{
						add_hero_le(hero, 7);
					}
				}

				ds_writeb(TEVENT114_OLIMONE_FLAG, (signed char)(done = 1));

			} else {
				GUI_output(get_tx2(64));

				timewarp(HOURS(8));

				for (i = 0, hero = get_hero(0); i <= 6; i++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero))
					{
						add_hero_le(hero, 4);
					}
				}

				done = 1;
			}
		}

	} while (done == 0);
}

/* a camp place */
void tevent_116(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 6) > 0 && !ds_readb(TEVENT116_FLAG)) ||
		ds_readb(TEVENT116_FLAG) != 0)
	{
		ds_writeb(TEVENT116_FLAG, 1);
		TRV_found_camp_place(1);
	}
}

/* an avalance */
void tevent_117(void)
{
	signed short i;
	Bit8u *hero;

	GUI_output(get_tx2(31));
	GUI_output(get_tx2(32));

	for (hero = get_hero(0), i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			test_attrib(hero, ATTRIB_GE, 0) <= 0)
		{
			/* attrib test failed */
			sub_hero_le(hero, random_schick(11) + 1);

			loose_random_item(hero, 15, get_ttx(506));
			loose_random_item(hero, 15, get_ttx(506));
			loose_random_item(hero, 15, get_ttx(506));
		}
	}

	GUI_output(get_tx2(33));
}

/* a herb place */
void tevent_118(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_PFLANZENKUNDE, 3) > 0 && !ds_readb(TEVENT118_FLAG)) ||
		ds_readb(TEVENT118_FLAG) != 0)
	{
		ds_writeb(GATHER_HERBS_SPECIAL, 60);
		TRV_found_herb_place(0);
		ds_writeb(GATHER_HERBS_SPECIAL, -1);
		ds_writeb(TEVENT118_FLAG, 1);
	}
}

/* a camp place */
void tevent_119(void)
{
	if ((test_skill(Real2Host(get_first_hero_available_in_group()), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT119_FLAG)) ||
		ds_readb(TEVENT119_FLAG) != 0)
	{
		ds_writeb(TEVENT119_FLAG, 1);
		TRV_found_camp_place(0);
	}
}

/* wolves attack */
void tevent_122(void)
{
	signed short answer;

	if (!ds_readb(TEVENT122_FLAG))
	{
		do {
			answer = GUI_radio(get_tx2(34), 2,
						get_tx2(35),
						get_tx2(36));
		} while (answer == -1);

		if (answer == 1)
		{
			if (!TRV_fight_event(FIGHTS_F122, 122))
			{
				ds_writeb(TEVENT122_FLAG, 1);
				add_hero_ap_all(10);

				load_in_head(4);

				GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(68), get_tx2(37), 0);
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_tx(68), get_tx2(63), 0);
			}
		} else {
			sub_hero_ap_all(20);
		}
	}
}

/* a bridge */
void tevent_123(void)
{
	signed short i;
	signed short counter;
	signed short answer;
	signed short done;
	signed short attrib_result;
	signed short skill_result;
	Bit8u *hero;

	load_in_head(54);

	do {
		done = 0;

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
						get_tx2(38), 3,
						get_tx2(39),
						get_tx2(40),
						get_tx2(41));
		} while (answer == -1);

		if (answer == 1)
		{
			/* go over the bridge */
			hero = get_hero(0);
			for (i = counter = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
					!hero_dead(hero) &&
					test_attrib(hero, ATTRIB_HA, 0) > 0)
				{
					/* attrib test failed */
					counter++;

					sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
						(char*)get_tx2(42),
						(char*)hero + HERO_NAME2);

					GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);
				}
			}

			if (!counter)
			{
				GUI_dialog_na(0, get_tx2(43));
				done = 1;
			}

		} else if (answer == 2)
		{
			/* turn arround */
			ds_writew(TRV_RETURN, done = 1);
		} else {
			/* fight acrophobia */
			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), NULL,
							get_tx2(44), 3,
							get_tx2(45),
							get_tx2(46),
							get_tx2(47));
			} while (answer == -1);

			if (answer == 1)
			{
				hero = get_hero(0);
				for (i = counter = 0; i <= 6; i++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero))
					{
						attrib_result = test_attrib(hero, ATTRIB_HA, 4);
						skill_result = test_skill(hero, TA_KLETTERN, 0);

						if (attrib_result == 99 && skill_result == -1)
						{
							sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_tx2(48),
								(char*)hero + HERO_NAME2);

							GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

							hero_disappear(hero, i, -1);

							counter++;

						} else if (attrib_result > 0 || skill_result <= 0)
						{
							sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_tx2(42),
								(char*)hero + HERO_NAME2);

							GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

							counter++;
						}
					}
				}

				if (!counter)
				{
					GUI_dialog_na(0, get_tx2(43));
					done = 1;
				}

			} else if (answer == 2)
			{
				/* cast a spell */
				hero = get_hero(select_hero_ok_forced(get_ttx(317)));

				if (test_spell(hero, 70, 0) > 0)
				{
					/* success */
					sub_ae_splash(hero, get_spell_cost(70, 0));

					GUI_dialog_na(0, get_tx2(43));

					done = 1;
				} else {
					/* failed */
					sub_ae_splash(hero, get_spell_cost(70, 1));

					hero = get_hero(0);
					for (i = counter = 0; i <= 6; i++, hero += SIZEOF_HERO)
					{
						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
							!hero_dead(hero))
						{
							sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_tx2(42),
								(char*)hero + HERO_NAME2);

							GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);
						}
					}
				}
			} else {
				hero = get_hero(0);
				for (i = counter = 0; i <= 6; i++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero))
					{
						attrib_result = test_attrib(hero, ATTRIB_HA, 2);

						if (attrib_result == 99)
						{
							/* unlucky */
							sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_tx2(48),
								(char*)hero + HERO_NAME2);

							GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

							hero_disappear(hero, i, -1);

							counter++;

						} else if (attrib_result > 0)
						{
							/* succeeded */
							sprintf((char*)Real2Host(ds_readd(DTP2)) + 0x400,
								(char*)get_tx2(42),
								(char*)hero + HERO_NAME2);

							GUI_dialog_na(0, Real2Host(ds_readd(DTP2)) + 0x400);

							counter++;
						}
					}
				}

				if (!counter)
				{
					GUI_dialog_na(0, get_tx2(43));
					done = 1;
				}
			}
		}


	} while (done == 0);
}

#if !defined(__BORLANDC__)
}
#endif
