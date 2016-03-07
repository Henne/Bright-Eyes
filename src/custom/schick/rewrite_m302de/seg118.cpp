/**
 *	Rewrite of DSA1 v3.02_de functions of seg118 (travel events 10 / 10)
 *	Functions rewritten: 1/11
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg027.h"
#include "seg029.h"
#include "seg096.h"
#include "seg097.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* The griffon */
/* Borlandified and identical */
void tevent_037(void)
{
	signed short answer;
	signed short done;
	signed short hours;
	Bit8u *hero;

	done = 0;

	if (!ds_readb(0x3dba))
	{
		ds_writeb(0xe5d2, 1);

		load_ani(28);
		draw_main_screen();
		init_ani(0);

		do {
			answer = GUI_radio(get_city(0x68), 2,
						get_city(0x6c),
						get_city(0x70));
		} while (answer == -1);

		if (answer == 1) {
			/* wait */

			timewarp(MINUTES(15));

			do {
				answer = GUI_radio(get_city(0x74), 2,
							get_city(0x78),
							get_city(0x70));
			} while (answer == -1);

			if (answer == 1)
			{

				timewarp(MINUTES(15));

				/* loop until you decicide to go on */
				do {
					answer = GUI_radio(get_city(0x7c), 2,
								get_city(0x78),
								get_city(0x70));

				} while (answer == -1 || answer == 1);
			}
		}

		if (answer == 2) {
			/* go on */

			do {
				answer = GUI_radio(get_city(0x80), 2,
							get_city(0x84),
							get_city(0x88));

			} while (answer == -1);
		}

		do {

			if (answer == 1)
			{
				/* turn around */
				hours = GUI_input(get_city(0x8c), 2);

				if (hours >= 4)
				{
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0x90),
						hours);

					GUI_output(Real2Host(ds_readd(DTP2)));

					done = 1;

				} else {

					/* entered time was to short */
					do {
						answer = GUI_radio(get_city(0x94), 2,
									get_city(0x84),
									get_city(0x88));

					} while (answer == -1);

					if (answer == 1)
					{
						timewarp(HOURS(4));

						GUI_output(get_city(0x98));

						done = 1;
					}
				}
			} else {
				do {
					answer = GUI_radio(get_city(0x9c), 3,
								get_city(0xa0),
								get_city(0xa4),
								get_city(0xa8));

				} while (answer == -1);

				if (answer == 1)
				{
					/* offer a deposit */
					ds_writeb(0x3dba, 1);

					/* select a hero randomly */
					hero = get_hero(answer = get_random_hero());

					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_city(0xb4),
						(char*)hero + HERO_NAME2,
						(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

					GUI_output(Real2Host(ds_readd(DTP2)));

					add_hero_ap(hero, 20);

					add_hero_ap_all(20);

					GUI_output(get_city(0xb8));

					GUI_input(get_city(0xbc), 10);

					/* check if the answer is "RAD" 1st */
					if (strcmp((char*)Real2Host(ds_readd(0xd2ef)), (char*)p_datseg + 0xb21e))
					{
						GUI_input(get_city(0xc0), 10);

						/* check if the answer is "RAD" 2nd */
						if (strcmp((char*)Real2Host(ds_readd(0xd2ef)), (char*)p_datseg + 0xb222))
						{
							GUI_input(get_city(0xc4), 10);

							/* check if the answer is "RAD" 3rd */
							if (strcmp((char*)Real2Host(ds_readd(0xd2ef)), (char*)p_datseg + 0xb226))
							{
								sprintf((char*)Real2Host(ds_readd(DTP2)),
									(char*)get_city(0xc8),
									(char*)hero + HERO_NAME2,
									(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
									(char*)hero + HERO_NAME2,
									(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

								GUI_output(Real2Host(ds_readd(DTP2)));

								/* the hero is now lost completely */
								hero_disappear(hero, answer, -1);

								done = 1;
							} else {

								GUI_output(get_city(0xcc));

								add_hero_ap_all(10);

								done = 1;
							}
						} else {

							GUI_output(get_city(0xcc));

							add_hero_ap_all(10);

							done = 1;
						}
					} else {

						GUI_output(get_city(0xcc));

						add_hero_ap_all(10);

						done = 1;
					}

				} else {
					/* talk of hetmann or hyggelik */

					do {
						answer = GUI_radio(get_city(0xac), 2,
									get_city(0x84),
									get_city(0x88));

					} while (answer == -1);

					if (answer == 2)
					{
						/* select a hero randomly */
						hero = get_hero(get_random_hero());

						timewarp(HOURS(1));

						sprintf((char*)Real2Host(ds_readd(DTP2)),
							(char*)get_city(0xb0),
							(char*)hero + HERO_NAME2);

						GUI_output(Real2Host(ds_readd(DTP2)));
					}
				}
			}

		} while (!done);

		set_var_to_zero();

		ds_writeb(0xe5d2, 0);
		ds_writew(0x2846, 1);
	}
}

#if !defined(__BORLANDC__)
}
#endif
