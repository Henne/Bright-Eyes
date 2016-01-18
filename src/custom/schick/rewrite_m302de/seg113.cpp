/*
 *	Rewrite of DSA1 v3.02_de functions of seg113 (travel events 5 / 10)
 *	Functions rewritten: 9/22
*/

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg026.h"
#include "seg028.h"
#include "seg029.h"
#include "seg032.h"
#include "seg097.h"
#include "seg103.h"
#include "seg109.h"

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

/* 0x900 */

void hero_disappear(Bit8u *hero, unsigned short pos, signed char type)
{

	/* decrement the number of heroes */
	ds_writeb(0x2d3c, ds_readb(0x2d3c) - 1);

	/* load a new savegame if no hero is present */
	if (ds_readb(0x2d3c) == 0)
		ds_writew(0xc3c1, 1);

	/* decrement group counter */
	ds_writeb(0x2d36 + ds_readb(CURRENT_GROUP),
		ds_readb(0x2d36 + ds_readb(CURRENT_GROUP)) - 1);

	/* write type to character sheet */
	host_writeb(hero + 0x88, type);

	/* reset position in group */
	host_writeb(hero + 0x8a, 0);

	if (pos == 6) {
		/* NPC */
		save_npc(0xe2 + host_readb(hero + 0x89));

		/* reset NPC timer */
		ds_writeb(0x3602 + host_readb(hero + 0x89), 0xff);
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

#if !defined(__BORLANDC__)
}
#endif
