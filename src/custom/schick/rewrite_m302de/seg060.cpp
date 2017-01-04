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
	signed short txt_id;
	signed short food_quality;
	signed short answer;
	signed short options;
	Bit8u *state_ptr;
	Bit8u *states_tab;
	char *text_buffer;
	char *answer1_buffer;
	char *answer2_buffer;
	char *answer3_buffer;
	char *format;
	Bit8u *hero;
	Bit8u *gossip_ptr1;
	Bit8u *gossip_ptr2;

	answer = 0;

	text_buffer =(char*) Real2Host(ds_readd(DTP2));
	answer1_buffer = text_buffer + 1000;
	answer2_buffer = answer1_buffer + 60;
	answer3_buffer = answer2_buffer + 60;
	ds_writews(TEXTBOX_WIDTH, 9);
	ds_writeb(TLK_TAV_FULLNESS, ds_writeb(TLK_TAV_TESTDRUNK, ds_writeb(CURRENT_INFORMER, ds_writeb(TLK_TAV_FOLLOWINFORMER, 0))));
	ds_writeb(TLK_TAV_DRINKCOUNT, 0);

	set_var_to_zero();
	load_ani(13);
	init_ani(0);
	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
	ds_writews(DIALOG_STATE, ds_writews(DIALOG_DONE, 0));
	states_tab = p_datseg + DIALOG_STATES;

	do {
		answer = options = 0;

		state_ptr = states_tab + 8 * ds_readws(DIALOG_STATE);

		if (host_readbs(state_ptr + 2) != 0) {
			options++;
		}
		if (host_readbs(state_ptr + 3) != 0) {
			options++;
		}
		if (host_readbs(state_ptr + 4) != 0) {
			options++;
		}

		if (host_readws(state_ptr) != -1) {

			txt_id = host_readws(state_ptr) & 0x7fff;
			format = (char*)get_dtp(4 * txt_id);
			hero = Real2Host(get_first_hero_available_in_group());

			if (txt_id == 52 || txt_id == 72 || txt_id == 78 || txt_id == 83 || txt_id == 89) {

				sprintf(text_buffer, format, count_heroes_in_group());

			} else if (txt_id == 16 || txt_id == 70) {

				sprintf(text_buffer, format, (char*)hero + HERO_NAME2);

			} else if (txt_id == 95) {

				hero = get_hero(ds_readb(TLK_TAV_TESTDRUNK) - 1);

				sprintf(text_buffer, format,
					(char*)hero + HERO_NAME2,
					Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)),
					Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 1)));
;
			} else if (txt_id == 18 || txt_id == 31) {

				sprintf(text_buffer, format, 20 * ds_readb(TLK_TAV_FULLNESS));

			} else if (txt_id == 40 || txt_id == 46) {

				gossip_ptr1 = Real2Host(load_current_town_gossip());

				do {
					gossip_ptr2 = Real2Host(load_current_town_gossip());
				} while (gossip_ptr1 == gossip_ptr2);

				sprintf(text_buffer, format, (char*)gossip_ptr1, (char*)gossip_ptr2);

			} else if (txt_id == 66) {

				sprintf(text_buffer, format, get_informer_hint());

			} else if (txt_id == 105) {

				sprintf(text_buffer, format,
					random_interval(3, 6),
					random_schick(2) == 1 ? get_dtp(0x244) : get_dtp(0x248));

			} else if (txt_id == 114) {

				food_quality = ds_readws(0x6c84 + 4 * ds_readws(TYPEINDEX) + 0);

				/* print quality [-1, 2..20]  2 = best, 20 = worse */
				sprintf(text_buffer, format,
					food_quality >= 1 && food_quality <= 3 ? get_dtp(0x220) : (
					food_quality >= 4 && food_quality <= 6 ? get_dtp(0x224) : (
					food_quality >= 7 && food_quality <= 9 ? get_dtp(0x228) : (
					food_quality >= 10 && food_quality <= 12 ? get_dtp(0x22c) : (
					food_quality >= 13 && food_quality <= 15 ? get_dtp(0x230) : (
					food_quality >= 15 && food_quality <= 18 ? get_dtp(0x234) : get_dtp(0x238)))))));

			} else if (txt_id == 115) {

				ds_writeb(TLK_TAV_INFORMERSEX, random_schick(2));

				sprintf(text_buffer, format,
					ds_readb(TLK_TAV_INFORMERSEX) == 1 ? get_dtp(0x23c) : get_dtp(0x240));

			} else if (txt_id == 119) {

				sprintf(text_buffer, format, Real2Host(get_random_tavern_message()));

			} else if (txt_id == 122 || txt_id == 126 || txt_id == 127 || txt_id == 132) {

				sprintf(text_buffer, format,
					Real2Host(GUI_get_ptr(ds_readb(TLK_TAV_INFORMERSEX) - 1, 0)),
					Real2Host(GUI_get_ptr(ds_readb(TLK_TAV_INFORMERSEX) - 1, 0)));

			} else if (txt_id == 128) {

				sprintf(text_buffer, format,
					!ds_readb(CURRENT_INFORMER) ? Real2Host(get_drinkmate()) : Real2Host(get_informer_name()));
			} else if (txt_id == 131) {

				sprintf(text_buffer, format,
					Real2Host(GUI_get_ptr(ds_readb(TLK_TAV_INFORMERSEX) - 1, 0)),
					Real2Host(GUI_get_ptr(ds_readb(TLK_TAV_INFORMERSEX) - 1, 0)),
					Real2Host(load_current_town_gossip()));

			} else {

				strcpy(text_buffer, format);
			}

			txt_id = host_readb(state_ptr + 2);
			format = (char*)get_dtp(4 * txt_id);


			if (txt_id == 1 || txt_id == 19) {
				sprintf(answer1_buffer, format, count_heroes_in_group());
			} else {
				strcpy(answer1_buffer, format);
			}

			txt_id = host_readb(state_ptr + 3);
			format = (char*)get_dtp(4 * txt_id);

			if (txt_id == 13) {

				sprintf(answer2_buffer, format, (char*)hero + HERO_NAME2);

			} else if (txt_id == 61) {

				sprintf(answer2_buffer, format, count_heroes_in_group());

			} else if (txt_id == 39) {

				sprintf(answer2_buffer, format,
					!ds_readb(INFORMER_FLAGS + get_town_lookup_entry()) ?
						Real2Host(get_drinkmate()) :
						Real2Host(get_informer_name2()));

			} else {
				strcpy(answer2_buffer, format);
			}

			txt_id = host_readb(state_ptr + 4);
			strcpy(answer3_buffer, (char*)get_dtp(4 * txt_id));

			do {
				answer = GUI_radio((Bit8u*)text_buffer, (signed char)options, answer1_buffer, answer2_buffer, answer3_buffer);
			} while (answer == -1);
		}

		ds_writews(DIALOG_NEXT_STATE, -1);

		if (host_readws(state_ptr) & 0x8000 || host_readws(state_ptr) == -1) {
			TLK_tavern(answer);
		}

		ds_writews(DIALOG_STATE, ds_readws(DIALOG_NEXT_STATE) == -1 ? host_readb(state_ptr + 5) : ds_readws(DIALOG_NEXT_STATE));

		if (ds_readws(DIALOG_DONE) == 0) {

			if (options != 0) {

				if (answer == -1) {
					ds_writew(DIALOG_DONE, 1);
				} else if (answer == 1) {
					ds_writew(DIALOG_STATE, host_readb(state_ptr + 5));
				} else if (answer == 2) {
					ds_writew(DIALOG_STATE, host_readb(state_ptr + 6));
				} else if (answer == 3) {
					ds_writew(DIALOG_STATE, host_readb(state_ptr + 7));
				}
			}

			if (ds_readws(DIALOG_STATE) == 255) {
				ds_writew(DIALOG_DONE, 1);
			}
		}

	} while (ds_readws(DIALOG_DONE) == 0);

	ds_writews(NPC_MEET_TAVERN, 0);
	ds_writews(TEXTBOX_WIDTH, 3);
	ds_writews(TEXT_FILE_INDEX, -1);
	load_tx(ds_readws(TX_FILE_INDEX));
	set_var_to_zero();
}

void TLK_tavern(signed short answer)
{
	signed short old_state;
	signed short tmp;
	Bit32s p_money;
	Bit8u *hero;
	signed short hero_pos;

	old_state = ds_readws(DIALOG_STATE);
	hero = Real2Host(get_first_hero_available_in_group());

	if (!old_state) {

		if (ds_readb(TAV_KICKED_FLAGS + ds_readws(TYPEINDEX)) != 0) {

			hero_pos = get_hero_CH_best();

			ds_writew(DIALOG_NEXT_STATE, test_attrib(get_hero(hero_pos), ATTRIB_CH, 0) <= 0 ? 112 : 113);

			ds_writeb(TAV_KICKED_FLAGS + ds_readws(TYPEINDEX), 0);

		} else {
			ds_writew(DIALOG_NEXT_STATE, 113);
		}

	} else if (old_state == 2) {

		if ((ds_readb(TLK_TAV_FULLNESS) == 1 || ds_readb(TLK_TAV_FULLNESS) == 2) && answer == 1) {
			ds_writew(DIALOG_NEXT_STATE, 3);
		} else if ((ds_readb(TLK_TAV_FULLNESS) == 1 || ds_readb(TLK_TAV_FULLNESS) == 2) && answer == 2) {
			ds_writew(DIALOG_NEXT_STATE, 5);
		} else {
			ds_writew(DIALOG_NEXT_STATE, 4);
		}

	} else if (old_state == 5) {

		ds_writew(DIALOG_NEXT_STATE, test_attrib(hero, ATTRIB_CH, 3) > 0 ? 81 : 3);

	} else if (old_state == 9) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + get_town_lookup_entry()) != 0 ? 10 : 11);

	} else if (old_state == 12 || old_state == 19 || old_state == 21) {

		ds_writeb(TLK_TAV_FIRSTINFO, 1);

	} else if (old_state == 14) {

		if (test_attrib(hero, ATTRIB_CH, 0) > 0) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FULLNESS) == 1 ? 16 : 17);
		} else {
			ds_writew(DIALOG_NEXT_STATE, 15);
		}

	} else if (old_state == 17 || old_state == 39 || old_state == 57) {

		ds_writeb(TLK_TAV_INFOPROMISE, 1);

	} else if (old_state == 18) {

		ds_writew(DIALOG_NEXT_STATE, test_attrib(hero, ATTRIB_CH, 2) > 0 ? 19 : 20);

	} else if (old_state == 24) {

		drink_while_drinking(10);

		tmp = 20 * ds_readb(TLK_TAV_FULLNESS);

		p_money = get_party_money();

		ds_writew(DIALOG_NEXT_STATE, tmp <= p_money ? 25 : 26);

	} else if (old_state == 27) {

		ds_writeb(TLK_TAV_ROUND, 1);

		drink_while_drinking(10);

		tmp = 20 * ds_readb(TLK_TAV_FULLNESS);

		p_money = get_party_money();

		p_money -= tmp;

		set_party_money(p_money);

	} else if (old_state == 28) {

		ds_writeb(TLK_TAV_ROUND, 1);

		drink_while_drinking(10);

		tmp = 20 * ds_readb(TLK_TAV_FULLNESS);

		p_money = get_party_money();

		p_money -= tmp + 10;

		set_party_money(p_money);

	} else if (old_state == 29) {

		drink_while_drinking(10);

		tmp = 20 * ds_readb(TLK_TAV_FULLNESS);

		p_money = get_party_money();

		ds_writew(DIALOG_NEXT_STATE, tmp <= p_money ? 30 : 31);

	} else if (old_state == 32) {

		sub_group_le(ds_readb(TLK_TAV_FULLNESS));

		ds_writeb(TAV_KICKED_FLAGS + ds_readws(TYPEINDEX), 1);

		if (ds_readb(TLK_TAV_FULLNESS) == 3) {
			ds_writeb(TOWN_OUTLAWED_FLAGS + ds_readws(TYPEINDEX), 1);
		}

	} else if (old_state == 33) {

		sub_group_le(2 * ds_readb(TLK_TAV_FULLNESS));

		ds_writeb(TAV_KICKED_FLAGS + ds_readws(TYPEINDEX), 1);

		if (ds_readb(TLK_TAV_FULLNESS) == 3) {
			ds_writeb(TOWN_OUTLAWED_FLAGS + ds_readws(TYPEINDEX), 1);
		}

	} else if (old_state == 34) {

		drink_while_drinking(10);

		tmp = 20 * ds_readb(TLK_TAV_FULLNESS);

		p_money = get_party_money();

		p_money -= tmp;

		set_party_money(p_money);

	} else if (old_state == 40) {

		drink_while_drinking(10);

		tmp = 20 * ds_readb(TLK_TAV_FULLNESS);

		p_money = get_party_money();

		ds_writew(DIALOG_NEXT_STATE, tmp <= p_money ? 41 : 42);

	} else if (old_state == 43) {

		if (test_attrib(hero, ATTRIB_CH, 4) > 0 && (ds_readb(TLK_TAV_FULLNESS) == 1 || ds_readb(TLK_TAV_FULLNESS) == 2)) {
			ds_writew(DIALOG_NEXT_STATE, 56);
		} else {
			ds_writew(DIALOG_NEXT_STATE, 44);
		}

	} else if (old_state == 45) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FULLNESS) == 3 ? 57 : 46);

	} else if (old_state == 46) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + get_town_lookup_entry()) != 0 ? 47 : 48);

	} else if (old_state == 49) {

		ds_writew(DIALOG_NEXT_STATE, test_attrib(hero, ATTRIB_CH, 4) > 0 ? 19 : 20);

	} else if (old_state == 54) {

		ds_writeb(TAV_KICKED_FLAGS + ds_readws(TYPEINDEX), 1);

	} else if (old_state == 55) {

		sub_group_le(1);

		ds_writeb(TAV_KICKED_FLAGS + ds_readws(TYPEINDEX), 1);

	} else if (old_state == 56) {

		ds_writeb(TLK_TAV_FREEBEER, 1);

	} else if (old_state == 58) {

		add_party_money(10);

		ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + get_town_lookup_entry()) != 0 ? 59 : 60);

	} else if (old_state == 61) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FULLNESS) == 3 ? 63 : 77);

	} else if (old_state == 63) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_INFOPROMISE) != 0 ? 64 : 65);

	} else if (old_state == 68) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_INFOPROMISE) != 0 ? 69 : 36);

	} else if (old_state == 71) {

		ds_writeb(TLK_TAV_INFOPROMISE, 0);
		ds_writeb(TLK_TAV_FIRSTINFO, 1);

	} else if (old_state == 73) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FIRSTINFO) != 0 ? 74 : 4);

	} else if (old_state == 77) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_INFOPROMISE) != 0 ? 78 : 82);

	} else if (old_state == 79) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + get_town_lookup_entry()) != 0 ? 153 : 154);

	} else if (old_state == 80) {

		ds_writew(DIALOG_NEXT_STATE, test_attrib(hero, ATTRIB_CH, 0) <= 0 && ds_readb(TLK_TAV_ROUND) != 0 ? 20 : 19);

	} else if (old_state == 85) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FIRSTINFO) != 0 ? 88 : 7);

	} else if (old_state == 86) {

		tmp = test_attrib(hero, ATTRIB_CH, 0);

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FIRSTINFO) != 0 || (!ds_readb(TLK_TAV_FIRSTINFO) && !ds_readb(TLK_TAV_ROUND) && tmp <= 0) ? 84 : 81);

	} else if (old_state == 87) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FIRSTINFO) != 0 ? 88 : 89);

	} else if (old_state == 88) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_ROUND) != 0 ? 92 : 97);

	} else if (old_state == 89) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_ROUND) != 0 ? 90 : 91);

	} else if (old_state == 93) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + get_town_lookup_entry()) != 0 ? 94 : 95);

	} else if (old_state == 98) {

		ds_writew(DIALOG_NEXT_STATE, test_attrib(hero, ATTRIB_CH, 0) > 0 ? 99 : 102);

	} else if (old_state == 99) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + get_town_lookup_entry()) != 0 ? 100 : 101);

	} else if (old_state == 103) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FULLNESS) == 3 ? 131 : 132);

	} else if (old_state == 104) {

		ds_writew(DIALOG_NEXT_STATE, ds_readds(DAY_TIMER) >= HOURS(22) && ds_readb(TLK_TAV_FULLNESS) != 3 ? 117 : 119);

	} else if (old_state == 106) {

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_ROUND) != 0 ? 107 : 108);

	} else if (old_state == 108) {

		if (ds_readb(TLK_TAV_TESTDRUNK) < count_heroes_in_group()) {
			ds_writew(DIALOG_NEXT_STATE, 109);
		} else {
			ds_writew(DIALOG_NEXT_STATE, 111);
			ds_writeb(TLK_TAV_TESTDRUNK, 0);
		}

	} else if (old_state == 109) {

		ds_writew(DIALOG_NEXT_STATE, 108);

		if ((host_readbs(get_hero(ds_readb(TLK_TAV_TESTDRUNK)) + 0x21) != 0) &&
			!hero_dead(get_hero(ds_readb(TLK_TAV_TESTDRUNK))) &&
			ds_readb(TLK_TAV_DRINKCOUNT) != 0)
		{
			ds_writew(DIALOG_NEXT_STATE, test_skill(get_hero(ds_readb(TLK_TAV_TESTDRUNK)), TA_ZECHEN, ds_readbs(TLK_TAV_DRINKCOUNT) - 8) > 0 ? 108 : 110);
		}

		inc_ds_bs_post(TLK_TAV_TESTDRUNK);	/* TODO: this variable is unsigned */

	} else if (old_state == 110) {

		hero_get_drunken(get_hero(ds_readb(TLK_TAV_TESTDRUNK) - 1));

	} else if (old_state == 112) {

		sub_group_le(1);

	} else if (old_state == 113) {

		ds_writeb(TLK_TAV_FULLNESS, random_schick(3));

		ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FULLNESS) == 1 ? 114 : (ds_readb(TLK_TAV_FULLNESS) == 2 ? 115 : 116));

	} else if (old_state == 119) {

		tmp = random_schick(3);

		ds_writew(DIALOG_NEXT_STATE, tmp == 1 ? 120 : (tmp == 2 ? 121 : 122));

	} else if (old_state == 120) {

		ds_writew(DIALOG_NEXT_STATE, random_schick(2) == 1 ? 123 : 124);

	} else if (old_state == 121) {

		tmp = random_schick(5);

		ds_writew(DIALOG_NEXT_STATE, tmp == 1 || tmp == 2 ? 125 : (tmp == 3 ? 126 : 127));

	} else if (old_state == 122) {

		ds_writew(DIALOG_NEXT_STATE, random_schick(3) == 1 ? 128 : 129);

	} else if (old_state == 124 || old_state == 128) {

		ds_writeb(TLK_TAV_FULLNESS, 2);

	} else if (old_state == 125) {

		ds_writeb(TLK_TAV_FULLNESS, 1);

	} else if (old_state == 127) {

		ds_writeb(TLK_TAV_FULLNESS, 3);

	} else if (old_state == 130) {

		tmp = count_heroes_in_group() * (6 - ds_readws(0x6c84 + 4 * ds_readws(TYPEINDEX)) / 4);
		tmp += (tmp * ds_readws(0x6c84 + 2 + 4 * ds_readws(TYPEINDEX))) / 100;
		p_money = get_party_money();
		p_money -= tmp;
		set_party_money(p_money);

		drink_while_drinking(10);
		eat_while_drinking(100);

		timewarp(HOURS(1));

	} else if (old_state == 131) {

		timewarp(MINUTES(45));
		inc_ds_bs_post(TLK_TAV_DRINKCOUNT);

		p_money = get_party_money();
		p_money -= 10;
		set_party_money(p_money);

		drink_while_drinking(10);

		ds_writew(DIALOG_NEXT_STATE, p_money >= 10 ? 104 : 105);
	}

	if (old_state == 132) {

		ds_writew(DIALOG_NEXT_STATE, npc_meetings(ds_readws(TYPEINDEX)) ? 144 : (!tavern_quest_infos() ? 133 : 144));

	} else if (old_state == 133) {

		ds_writew(DIALOG_NEXT_STATE, random_schick(5) == 5 ? 152 : 131);

	} else if (old_state == 134) {

		ds_writeb(TLK_TAV_INFORMERSEX, 1);

	} else if (old_state == 138) {

		timewarp(MINUTES(5));

	} else if (old_state == 139) {

		ds_writew(DIALOG_NEXT_STATE, random_schick(4) == 1 ? 140 : 138);

	} else if (old_state == 140) {

		ds_writew(DIALOG_NEXT_STATE, random_schick(3) == 2 ? 142 : 141);

	} else if (old_state == 146) {

		if (!ds_readb(CURRENT_INFORMER)) {
			get_drinkmate();
		} else {
			get_informer_name();
		}

		ds_writew(DIALOG_NEXT_STATE, !ds_readb(CURRENT_INFORMER) ? 148 : 147);

	} else if (old_state == 151) {

		ds_writeb(TLK_TAV_FOLLOWINFORMER, 1);

	} else if (old_state == 152) {

		ds_writew(DIALOG_NEXT_STATE, random_schick(3) == 2 ? 134 : 135);

	}
}

#if !defined(__BORLANDC__)
}
#endif
