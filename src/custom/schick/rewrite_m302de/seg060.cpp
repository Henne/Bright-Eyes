/**
 *	Rewrite of DSA1 v3.02_de functions of seg060 (tavern talking)
 *	Functions rewritten: 2/2 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg060.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg031.h"
#include "seg047.h"
#include "seg060.h"
#include "seg073.h"
#include "seg095.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void talk_tavern(void)
{
	signed short l_si;
	signed short l_di;
	signed short answer;
	signed short options;
	Bit8u *ptr1;
	Bit8u *ptr2;
	char *ptr3;
	char *ptr4;
	char *ptr5;
	char *ptr6;
	char *format;
	Bit8u *hero;
	Bit8u *ptr9;
	Bit8u *ptr10;

	answer = 0;

	ptr3 =(char*) Real2Host(ds_readd(DTP2));
	ptr4 = ptr3 + 1000;
	ptr5 = ptr4 + 60;
	ptr6 = ptr5 + 60;
	ds_writews(TEXTBOX_WIDTH, 9);
	ds_writeb(0x3610, ds_writeb(0x3611, ds_writeb(CURRENT_INFORMER, ds_writeb(0x360f, 0))));
	ds_writeb(0x360c, 0);

	set_var_to_zero();
	load_ani(13);
	init_ani(0);
	/* load TAVERN.TLK */
	load_tlk(130);
	ds_writews(DIALOG_STATE, ds_writews(DIALOG_DONE, 0));
	ptr2 = p_datseg + 0x3794;

	do {
		answer = options = 0;

		ptr1 = ptr2 + 8 * ds_readws(DIALOG_STATE);

		if (host_readbs(ptr1 + 2) != 0) {
			options++;
		}
		if (host_readbs(ptr1 + 3) != 0) {
			options++;
		}
		if (host_readbs(ptr1 + 4) != 0) {
			options++;
		}

		if (host_readws(ptr1) != -1) {

			l_si = host_readws(ptr1) & 0x7fff;
			format = (char*)get_dtp(4 * l_si);
			hero = Real2Host(get_first_hero_available_in_group());

			if (l_si == 52 || l_si == 72 || l_si == 78 || l_si == 83 || l_si == 89) {

				sprintf(ptr3, format, count_heroes_in_group());

			} else if (l_si == 16 || l_si == 70) {

				sprintf(ptr3, format, (char*)hero + 0x10);

			} else if (l_si == 95) {

				hero = get_hero(ds_readb(0x3611) - 1);

				sprintf(ptr3, format,
					(char*)hero + 0x10,
					Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 3)),
					Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 1)));
;
			} else if (l_si == 18 || l_si == 31) {

				sprintf(ptr3, format, 20 * ds_readb(0x3610));

			} else if (l_si == 40 || l_si == 46) {

				ptr9 = Real2Host(load_current_town_gossip());

				do {
					ptr10 = Real2Host(load_current_town_gossip());
				} while (ptr9 == ptr10);

				sprintf(ptr3, format, (char*)ptr9, (char*)ptr10);

			} else if (l_si == 66) {

				sprintf(ptr3, format, get_informer_hint());

			} else if (l_si == 105) {

				sprintf(ptr3, format,
					random_interval(3, 6),
					random_schick(2) == 1 ? get_dtp(0x244) : get_dtp(0x248));

			} else if (l_si == 114) {

				l_di = ds_readws(0x6c84 + 4 * ds_readws(TYPEINDEX) + 0);

				/* print quality [-1, 2..20]  2 = best, 20 = worse */
				sprintf(ptr3, format,
					l_di >= 1 && l_di <= 3 ? get_dtp(0x220) : (
					l_di >= 4 && l_di <= 6 ? get_dtp(0x224) : (
					l_di >= 7 && l_di <= 9 ? get_dtp(0x228) : (
					l_di >= 10 && l_di <= 12 ? get_dtp(0x22c) : (
					l_di >= 13 && l_di <= 15 ? get_dtp(0x230) : (
					l_di >= 15 && l_di <= 18 ? get_dtp(0x234) : get_dtp(0x238)))))));

			} else if (l_si == 115) {

				ds_writeb(0x360d, random_schick(2));

				sprintf(ptr3, format,
					ds_readb(0x360d) == 1 ? get_dtp(0x23c) : get_dtp(0x240));

			} else if (l_si == 119) {

				sprintf(ptr3, format, Real2Host(get_random_tavern_message()));

			} else if (l_si == 122 || l_si == 126 || l_si == 127 || l_si == 132) {

				sprintf(ptr3, format,
					Real2Host(GUI_get_ptr(ds_readb(0x360d) - 1, 0)),
					Real2Host(GUI_get_ptr(ds_readb(0x360d) - 1, 0)));

			} else if (l_si == 128) {

				sprintf(ptr3, format,
					!ds_readb(CURRENT_INFORMER) ? Real2Host(get_drinkmate()) : Real2Host(get_informer_name()));
			} else if (l_si == 131) {

				sprintf(ptr3, format,
					Real2Host(GUI_get_ptr(ds_readb(0x360d) - 1, 0)),
					Real2Host(GUI_get_ptr(ds_readb(0x360d) - 1, 0)),
					Real2Host(load_current_town_gossip()));

			} else {

				strcpy(ptr3, format);
			}

			l_si = host_readb(ptr1 + 2);
			format = (char*)get_dtp(4 * l_si);


			if (l_si == 1 || l_si == 19) {
				sprintf(ptr4, format, count_heroes_in_group());
			} else {
				strcpy(ptr4, format);
			}

			l_si = host_readb(ptr1 + 3);
			format = (char*)get_dtp(4 * l_si);

			if (l_si == 13) {

				sprintf(ptr5, format, (char*)hero + 0x10);

			} else if (l_si == 61) {

				sprintf(ptr5, format, count_heroes_in_group());

			} else if (l_si == 39) {

				sprintf(ptr5, format,
					!ds_readb(INFORMER_JURGE + get_town_lookup_entry()) ?
						Real2Host(get_drinkmate()) :
						Real2Host(get_informer_name2()));

			} else {
				strcpy(ptr5, format);
			}

			l_si = host_readb(ptr1 + 4);
			strcpy(ptr6, (char*)get_dtp(4 * l_si));

			do {
				answer = GUI_radio((Bit8u*)ptr3, options, ptr4, ptr5, ptr6);
			} while (answer == -1);
		}

		ds_writews(0xe30e, -1);

		if (host_readws(ptr1) & 0x8000 || host_readws(ptr1) == -1) {
			TLK_tavern(answer);
		}

		ds_writews(DIALOG_STATE, ds_readws(0xe30e) == -1 ? host_readb(ptr1 + 5) : ds_readws(0xe30e));

		if (ds_readws(DIALOG_DONE) == 0) {

			if (options != 0) {

				if (answer == -1) {
					ds_writew(DIALOG_DONE, 1);
				} else if (answer == 1) {
					ds_writew(DIALOG_STATE, host_readb(ptr1 + 5));
				} else if (answer == 2) {
					ds_writew(DIALOG_STATE, host_readb(ptr1 + 6));
				} else if (answer == 3) {
					ds_writew(DIALOG_STATE, host_readb(ptr1 + 7));
				}
			}

			if (ds_readws(DIALOG_STATE) == 255) {
				ds_writew(DIALOG_DONE, 1);
			}
		}

	} while (ds_readws(DIALOG_DONE) == 0);

	ds_writews(0x346e, 0);
	ds_writews(TEXTBOX_WIDTH, 3);
	ds_writews(0x26bd, -1);
	load_buffer_1(ds_readws(0x26bf));
	set_var_to_zero();
}

void TLK_tavern(signed short answer)
{
	signed short l_si;
	signed short l_di;
	Bit32s p_money;
	Bit8u *hero;
	signed short hero_pos;

	l_si = ds_readws(DIALOG_STATE);
	hero = Real2Host(get_first_hero_available_in_group());

	if (!l_si) {

		if (ds_readb(0x3374 + ds_readws(TYPEINDEX)) != 0) {

			hero_pos = get_hero_CH_best();

			ds_writew(0xe30e, test_attrib(get_hero(hero_pos), 2, 0) <= 0 ? 112 : 113);

			ds_writeb(0x3374 + ds_readws(TYPEINDEX), 0);

		} else {
			ds_writew(0xe30e, 113);
		}

	} else if (l_si == 2) {

		if ((ds_readb(0x3610) == 1 || ds_readb(0x3610) == 2) && answer == 1) {
			ds_writew(0xe30e, 3);
		} else if ((ds_readb(0x3610) == 1 || ds_readb(0x3610) == 2) && answer == 2) {
			ds_writew(0xe30e, 5);
		} else {
			ds_writew(0xe30e, 4);
		}

	} else if (l_si == 5) {

		ds_writew(0xe30e, test_attrib(hero, 2, 3) > 0 ? 81 : 3);

	} else if (l_si == 9) {

		ds_writew(0xe30e, ds_readb(INFORMER_JURGE + get_town_lookup_entry()) != 0 ? 10 : 11);

	} else if (l_si == 12 || l_si == 19 || l_si == 21) {

		ds_writeb(0x3608, 1);

	} else if (l_si == 14) {

		if (test_attrib(hero, 2, 0) > 0) {
			ds_writew(0xe30e, ds_readb(0x3610) == 1 ? 16 : 17);
		} else {
			ds_writew(0xe30e, 15);
		}

	} else if (l_si == 17 || l_si == 39 || l_si == 57) {

		ds_writeb(0x3609, 1);

	} else if (l_si == 18) {

		ds_writew(0xe30e, test_attrib(hero, 2, 2) > 0 ? 19 : 20);

	} else if (l_si == 24) {

		drink_while_drinking(10);

		l_di = 20 * ds_readb(0x3610);

		p_money = get_party_money();

		ds_writew(0xe30e, l_di <= p_money ? 25 : 26);

	} else if (l_si == 27) {

		ds_writeb(0x360a, 1);

		drink_while_drinking(10);

		l_di = 20 * ds_readb(0x3610);

		p_money = get_party_money();

		p_money -= l_di;

		set_party_money(p_money);

	} else if (l_si == 28) {

		ds_writeb(0x360a, 1);

		drink_while_drinking(10);

		l_di = 20 * ds_readb(0x3610);

		p_money = get_party_money();

		p_money -= l_di + 10;

		set_party_money(p_money);

	} else if (l_si == 29) {

		drink_while_drinking(10);

		l_di = 20 * ds_readb(0x3610);

		p_money = get_party_money();

		ds_writew(0xe30e, l_di <= p_money ? 30 : 31);

	} else if (l_si == 32) {

		sub_group_le(ds_readb(0x3610));

		ds_writeb(0x3374 + ds_readws(TYPEINDEX), 1);

		if (ds_readb(0x3610) == 3) {
			ds_writeb(0x33cc + ds_readws(TYPEINDEX), 1);
		}

	} else if (l_si == 33) {

		sub_group_le(2 * ds_readb(0x3610));

		ds_writeb(0x3374 + ds_readws(TYPEINDEX), 1);

		if (ds_readb(0x3610) == 3) {
			ds_writeb(0x33cc + ds_readws(TYPEINDEX), 1);
		}

	} else if (l_si == 34) {

		drink_while_drinking(10);

		l_di = 20 * ds_readb(0x3610);

		p_money = get_party_money();

		p_money -= l_di;

		set_party_money(p_money);

	} else if (l_si == 40) {

		drink_while_drinking(10);

		l_di = 20 * ds_readb(0x3610);

		p_money = get_party_money();

		ds_writew(0xe30e, l_di <= p_money ? 41 : 42);

	} else if (l_si == 43) {

		if (test_attrib(hero, 2, 4) > 0 && (ds_readb(0x3610) == 1 || ds_readb(0x3610) == 2)) {
			ds_writew(0xe30e, 56);
		} else {
			ds_writew(0xe30e, 44);
		}

	} else if (l_si == 45) {

		ds_writew(0xe30e, ds_readb(0x3610) == 3 ? 57 : 46);

	} else if (l_si == 46) {

		ds_writew(0xe30e, ds_readb(INFORMER_JURGE + get_town_lookup_entry()) != 0 ? 47 : 48);

	} else if (l_si == 49) {

		ds_writew(0xe30e, test_attrib(hero, 2, 4) > 0 ? 19 : 20);

	} else if (l_si == 54) {

		ds_writeb(0x3374 + ds_readws(TYPEINDEX), 1);

	} else if (l_si == 55) {

		sub_group_le(1);

		ds_writeb(0x3374 + ds_readws(TYPEINDEX), 1);

	} else if (l_si == 56) {

		ds_writeb(0x360b, 1);

	} else if (l_si == 58) {

		add_party_money(10);

		ds_writew(0xe30e, ds_readb(INFORMER_JURGE + get_town_lookup_entry()) != 0 ? 59 : 60);

	} else if (l_si == 61) {

		ds_writew(0xe30e, ds_readb(0x3610) == 3 ? 63 : 77);

	} else if (l_si == 63) {

		ds_writew(0xe30e, ds_readb(0x3609) != 0 ? 64 : 65);

	} else if (l_si == 68) {

		ds_writew(0xe30e, ds_readb(0x3609) != 0 ? 69 : 36);

	} else if (l_si == 71) {

		ds_writeb(0x3609, 0);
		ds_writeb(0x3608, 1);

	} else if (l_si == 73) {

		ds_writew(0xe30e, ds_readb(0x3608) != 0 ? 74 : 4);

	} else if (l_si == 77) {

		ds_writew(0xe30e, ds_readb(0x3609) != 0 ? 78 : 82);

	} else if (l_si == 79) {

		ds_writew(0xe30e, ds_readb(INFORMER_JURGE + get_town_lookup_entry()) != 0 ? 153 : 154);

	} else if (l_si == 80) {

		ds_writew(0xe30e, test_attrib(hero, 2, 0) <= 0 && ds_readb(0x360a) != 0 ? 20 : 19);

	} else if (l_si == 85) {

		ds_writew(0xe30e, ds_readb(0x3608) != 0 ? 88 : 7);

	} else if (l_si == 86) {

		l_di = test_attrib(hero, 2, 0);

		ds_writew(0xe30e, ds_readb(0x3608) != 0 || (!ds_readb(0x3608) && !ds_readb(0x360a) && l_di <= 0) ? 84 : 81);

	} else if (l_si == 87) {

		ds_writew(0xe30e, ds_readb(0x3608) != 0 ? 88 : 89);

	} else if (l_si == 88) {

		ds_writew(0xe30e, ds_readb(0x360a) != 0 ? 92 : 97);

	} else if (l_si == 89) {

		ds_writew(0xe30e, ds_readb(0x360a) != 0 ? 90 : 91);

	} else if (l_si == 93) {

		ds_writew(0xe30e, ds_readb(INFORMER_JURGE + get_town_lookup_entry()) != 0 ? 94 : 95);

	} else if (l_si == 98) {

		ds_writew(0xe30e, test_attrib(hero, 2, 0) > 0 ? 99 : 102);

	} else if (l_si == 99) {

		ds_writew(0xe30e, ds_readb(INFORMER_JURGE + get_town_lookup_entry()) != 0 ? 100 : 101);

	} else if (l_si == 103) {

		ds_writew(0xe30e, ds_readb(0x3610) == 3 ? 131 : 132);

	} else if (l_si == 104) {

		ds_writew(0xe30e, ds_readds(DAY_TIMER) >= HOURS(22) && ds_readb(0x3610) != 3 ? 117 : 119);

	} else if (l_si == 106) {

		ds_writew(0xe30e, ds_readb(0x360a) != 0 ? 107 : 108);

	} else if (l_si == 108) {

		if (ds_readb(0x3611) < count_heroes_in_group()) {
			ds_writew(0xe30e, 109);
		} else {
			ds_writew(0xe30e, 111);
			ds_writeb(0x3611, 0);
		}

	} else if (l_si == 109) {

		ds_writew(0xe30e, 108);

		if ((host_readbs(get_hero(ds_readb(0x3611)) + 0x21) != 0) &&
			!hero_dead(get_hero(ds_readb(0x3611))) &&
			ds_readb(0x360c) != 0)
		{
			ds_writew(0xe30e, test_skill(get_hero(ds_readb(0x3611)), 18, ds_readbs(0x360c) - 8) > 0 ? 108 : 110);
		}

		inc_ds_bs_post(0x3611);	/* TODO: this variable is unsigned */

	} else if (l_si == 110) {

		hero_get_drunken(get_hero(ds_readb(0x3611) - 1));

	} else if (l_si == 112) {

		sub_group_le(1);

	} else if (l_si == 113) {

		ds_writeb(0x3610, random_schick(3));

		ds_writew(0xe30e, ds_readb(0x3610) == 1 ? 114 : (ds_readb(0x3610) == 2 ? 115 : 116));

	} else if (l_si == 119) {

		l_di = random_schick(3);

		ds_writew(0xe30e, l_di == 1 ? 120 : (l_di == 2 ? 121 : 122));

	} else if (l_si == 120) {

		ds_writew(0xe30e, random_schick(2) == 1 ? 123 : 124);

	} else if (l_si == 121) {

		l_di = random_schick(5);

		ds_writew(0xe30e, l_di == 1 || l_di == 2 ? 125 : (l_di == 3 ? 126 : 127));

	} else if (l_si == 122) {

		ds_writew(0xe30e, random_schick(3) == 1 ? 128 : 129);

	} else if (l_si == 124 || l_si == 128) {

		ds_writeb(0x3610, 2);

	} else if (l_si == 125) {

		ds_writeb(0x3610, 1);

	} else if (l_si == 127) {

		ds_writeb(0x3610, 3);

	} else if (l_si == 130) {

		l_di = count_heroes_in_group() * (6 - ds_readws(0x6c84 + 4 * ds_readws(TYPEINDEX)) / 4);
		l_di += (l_di * ds_readws(0x6c84 + 2 + 4 * ds_readws(TYPEINDEX))) / 100;
		p_money = get_party_money();
		p_money -= l_di;
		set_party_money(p_money);

		drink_while_drinking(10);
		eat_while_drinking(100);

		timewarp(HOURS(1));

	} else if (l_si == 131) {

		timewarp(MINUTES(45));
		inc_ds_bs_post(0x360c);

		p_money = get_party_money();
		p_money -= 10;
		set_party_money(p_money);

		drink_while_drinking(10);

		ds_writew(0xe30e, p_money >= 10 ? 104 : 105);
	}

	if (l_si == 132) {

		ds_writew(0xe30e, npc_meetings(ds_readws(TYPEINDEX)) ? 144 : (!tavern_quest_infos() ? 133 : 144));

	} else if (l_si == 133) {

		ds_writew(0xe30e, random_schick(5) == 5 ? 152 : 131);

	} else if (l_si == 134) {

		ds_writeb(0x360d, 1);

	} else if (l_si == 138) {

		timewarp(MINUTES(5));

	} else if (l_si == 139) {

		ds_writew(0xe30e, random_schick(4) == 1 ? 140 : 138);

	} else if (l_si == 140) {

		ds_writew(0xe30e, random_schick(3) == 2 ? 142 : 141);

	} else if (l_si == 146) {

		if (!ds_readb(CURRENT_INFORMER)) {
			get_drinkmate();
		} else {
			get_informer_name();
		}

		ds_writew(0xe30e, !ds_readb(CURRENT_INFORMER) ? 148 : 147);

	} else if (l_si == 151) {

		ds_writeb(0x360f, 1);

	} else if (l_si == 152) {

		ds_writew(0xe30e, random_schick(3) == 2 ? 134 : 135);

	}
}

#if !defined(__BORLANDC__)
}
#endif
