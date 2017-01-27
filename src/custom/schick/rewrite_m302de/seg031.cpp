/*
 *	Rewrite of DSA1 v3.02_de functions of seg031 (tavern helpers)
 *	Functions rewritten: 10/10 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg031.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg007.h"
#include "seg026.h"
#include "seg028.h"
#include "seg030.h"
#include "seg031.h"
#include "seg067.h"
#include "seg073.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct tlk_option {
	signed short txt, goto_state;
};

void do_random_talk(signed short talk_id, signed short informer_id)
{
	signed short optioncount;
	signed short answer = 0;
	signed short txt_id;
	signed short txt_offset;
	signed short opt0_txt;
	signed short opt1_txt;
	signed short opt2_txt;
	signed short txt_id_raw;
	signed short txt_id_rand;
	signed short opt0_rand;
	signed short opt1_rand;
	signed short opt2_rand;
	Bit8u *state_ptr;
	Bit8u *states_tab;
	Bit8u *partners_tab;
	Bit8u *dialog_title;
	char *dst;
	char *fmt;
	signed short shufflepair_1;
	signed short shufflepair_2;
	signed short shufflecount;
	signed short i;
	signed short tmp1;
	signed short tmp2;
	struct tlk_option options[3];

	ds_writew(DIALOG_INFORMER, informer_id);
	ds_writew(TLK_ID, talk_id);

	load_tlk(talk_id + ARCHIVE_FILE_DIALOGS_TLK);
	ds_writew(DIALOG_STATE, ds_writew(DIALOG_DONE, 0));
	partners_tab = p_datseg + DIALOG_PARTNERS;
	states_tab = Real2Host(host_readds(partners_tab + 38 * informer_id));
	txt_offset = host_readws(partners_tab + 4 + 38 * informer_id);
	dialog_title = 38 * informer_id + partners_tab + 6;
	load_in_head(host_readws(partners_tab + 38 * informer_id + 36));
	dst = (char*)Real2Host(ds_readd(DTP2)) + 0x400;

	do {
		answer = optioncount = 0;
		state_ptr = 8 * ds_readws(DIALOG_STATE) + states_tab;

		if (ds_readws(TLK_ID) == 13 && ds_readws(DIALOG_STATE) >= 20) {
			txt_id_rand = opt0_rand = opt1_rand = opt2_rand = 0;
		} else {
			txt_id_rand = random_schick(4) - 1;
			opt0_rand = random_schick(4) - 1;
			opt1_rand = random_schick(4) - 1;
			opt2_rand = random_schick(4) - 1;
		}

		if ((txt_id_raw = host_readws(state_ptr)) != -1) {
			txt_id_raw = 4 * txt_id_raw + txt_id_rand;
		}
		if (host_readb(state_ptr + 2) != 0) {
			opt0_txt = 4 * host_readb(state_ptr + 2) + opt0_rand;
			optioncount++;
		}
		if (host_readb(state_ptr + 3) != 0) {
			opt1_txt = 4 * host_readb(state_ptr + 3) + opt1_rand;
			optioncount++;
		}
		if (host_readb(state_ptr + 4) != 0) {
			opt2_txt = 4 * host_readb(state_ptr + 4) + opt2_rand;
			optioncount++;
		}

		if (txt_id_raw != -1) {

			txt_id = (4 * host_readw(state_ptr) + txt_id_rand) & 0x7fff;
			fmt = (char*)get_tx(txt_id + txt_offset);

			if (ds_readws(TLK_ID) == 15) {

				if (ds_readws(DIALOG_STATE) == 13) {
					sprintf(dst, fmt, (char*)Real2Host(waffinfo_herbs()));
				} else {
					strcpy(dst, fmt);
				}

			} else if (ds_readws(TLK_ID) == 14) {

				if (ds_readws(DIALOG_STATE) == 11) {
					sprintf(dst, fmt, (char*)Real2Host(waffinfo_general()));
				} else {
					strcpy(dst, fmt);
				}

			} else if (ds_readws(TLK_ID) == 16) {

				if (ds_readws(DIALOG_STATE) == 19 || ds_readws(DIALOG_STATE) == 23) {
					sprintf(dst, fmt, (char*)Real2Host(waffinfo_weapons()));
				} else {
					strcpy(dst, fmt);
				}

			} else if (ds_readws(TLK_ID) == 1) {

				if (ds_readws(DIALOG_STATE) == 16) {
					sprintf(dst, fmt, (char*)Real2Host(load_current_town_gossip()));
				} else {
					strcpy(dst, fmt);
				}

			} else {
				strcpy(dst, fmt);
			}

			options[0].txt = opt0_txt + txt_offset;
			options[0].goto_state = host_readb(state_ptr + 5);
			options[1].txt = opt1_txt + txt_offset;
			options[1].goto_state = host_readb(state_ptr + 6);
			options[2].txt = opt2_txt + txt_offset;
			options[2].goto_state = host_readb(state_ptr + 7);

			if (optioncount) {

				shufflecount = random_schick(5);
				for (i = 0; i < shufflecount; i++) {

					shufflepair_1 = random_schick(3) - 1;
					shufflepair_2 = random_schick(3) - 1;

					tmp1 = options[shufflepair_1].txt;
					tmp2 = options[shufflepair_2].txt;

					if (tmp1 != txt_offset && tmp2 != txt_offset) {
						options[shufflepair_1].txt = tmp2;
						options[shufflepair_2].txt = tmp1;
						tmp1 = options[shufflepair_1].goto_state;
						options[shufflepair_1].goto_state = options[shufflepair_2].goto_state;
						options[shufflepair_2].goto_state = tmp1;
					}
				}
			}

			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), dialog_title, (Bit8u*)dst, optioncount,
					get_tx(options[0].txt),
					get_tx(options[1].txt),
					get_tx(options[2].txt));

		} else {
			options[0].goto_state = host_readb(state_ptr + 5);
		}

		ds_writews(DIALOG_NEXT_STATE, -1);
		if ((host_readw(state_ptr) & 0x8000) || host_readws(state_ptr) == -1) {
			talk_switch();
		}

		ds_writew(DIALOG_STATE, ds_readws(DIALOG_NEXT_STATE) == -1 ? options[0].goto_state : ds_readws(DIALOG_NEXT_STATE));

		if (ds_readws(DIALOG_DONE) == 0) {

			if (optioncount) {

				if (answer == -1) {
					ds_writew(DIALOG_DONE, 1);
				} else if (answer == 1) {
					ds_writew(DIALOG_STATE, options[0].goto_state);
				} else if (answer == 2) {
					ds_writew(DIALOG_STATE, options[1].goto_state);
				} else if (answer == 3) {
					ds_writew(DIALOG_STATE, options[2].goto_state);
				}
			}

			if (ds_readws(DIALOG_STATE) == 255) {
				ds_writew(DIALOG_DONE, 1);
			}
		}

	} while (ds_readws(DIALOG_DONE) == 0);

	ds_writews(TEXT_FILE_INDEX, ds_writews(CURRENT_ANI, -1));
	load_tx(ds_readws(TX_FILE_INDEX));
}

/* This function is dead code */
RealPt get_informer_forename(void)
{
	signed short i;
	char tmp;
	Bit8u *p_info;
	Bit8u *informer_name;

	p_info = p_datseg + INFORMER_TAB;

	for (i = 0; i < 15; i++, p_info += 4) {

		if (host_readbs(p_info + 2) == ds_readbs(CURRENT_TOWN)) {

			i = 0;
			informer_name = get_ttx(host_readws(p_info));

			do {
				tmp = host_readbs(informer_name);
				informer_name++;
				i++;
			} while (tmp != ' ');

			strncpy((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), (char*)get_ttx(host_readws(p_info)), i);
#ifdef M302de_ORIGINAL_BUGFIX
			break;
#endif
		}
	}
	return (RealPt)ds_readd(TEXT_OUTPUT_BUF);
}

/**
 * \brief   ?
 *
 * \return              a value between 0 and 15
 */
signed short get_town_lookup_entry(void)
{
	Bit8u *ptr;
	signed short i;

	ptr = p_datseg + INFORMER_TAB;
	for (i = 0; i < 15; i++, ptr += 4) {
		if (host_readb(ptr + 2) == ds_readb(CURRENT_TOWN)) {
			return i;
		}
	}

	return 15;
}

/**
 * \brief   gives a hint where a informer lives
 *
 *          Game Info: You can ask in some towns where informers live.
 *          This function returns a pointer to the answer or to an empty string.
 *
 * \return              a pointer to the string.
 */
RealPt get_informer_hint(void)
{
	signed short i;
	Bit8u *ptr;

	ptr = p_datseg + INFORMER_TAB;
	for (i = 0; i < 15; i++, ptr += 4) {
		if (host_readb(ptr + 2) == ds_readb(CURRENT_TOWN)) {
			return (RealPt)host_readd(Real2Host(ds_readd(TEXT_LTX_INDEX)) + 4 * (i + 0x2cb));
		}
	}

	return (RealPt)host_readd(Real2Host(ds_readd(TEXT_LTX_INDEX)) + 0xb54);
}

/**
 * \brief   get the name on the current informer
 *
 * \return              a pointer to the name of the informer
 */
RealPt get_informer_name(void)
{
	return (RealPt)host_readd(Real2Host(ds_readd(TEXT_LTX_INDEX)) + ds_readw(INFORMER_TAB - 4 + ds_readb(CURRENT_INFORMER) * 4) * 4);
}

/**
 * \brief   get the name of the informer in this town
 *
 * \return              a pointer to the name of the informer
 */
RealPt get_informer_name2(void)
{
	return (RealPt)host_readd(Real2Host(ds_readd(TEXT_LTX_INDEX)) +
			ds_readw(INFORMER_TAB + get_town_lookup_entry() * 4) * 4);
}

/**
 * \brief   loads a gossip message from the current town
 *
 * \return              a pointer to the message.
 */
RealPt load_current_town_gossip(void)
{
	signed short gossip_id;
	Bit8u *ptr;

	/* load TOWN.LTX */
	load_ltx(ds_readbs(CURRENT_TOWN) + ARCHIVE_FILE_CITY_LTX);

	/* mark some buffers invalid */
	ds_writews(AREA_PREPARED, ds_writews(CURRENT_ANI, -1));

	/* get the pointer to the ltx buffer */
	ptr = Real2Host(ds_readd(BUFFER9_PTR3));

	/* get some gossip */
	gossip_id = get_tavern_gossip();

	/* return the pointer to the gossip (pointers are stored in the first 1000 bytes) */
	return (RealPt)host_readd(ptr + 4 * gossip_id);
}

RealPt get_random_tavern_message(void)
{
	signed short randval;
	RealPt ptr;

	randval = random_schick(20) - 1;

	ptr = (RealPt)host_readd(Real2Host(ds_readd(TX_INDEX)) + 4 * (randval + 147));

	if (!randval || randval == 19) {

		sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
			(char*)Real2Host(ptr),
			(char*)Real2Host(load_current_town_gossip()));

		return (RealPt)ds_readd(TEXT_OUTPUT_BUF);

	} else if (randval == 3) {

		sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
			(char*)Real2Host(ptr),
			(char*)get_ttx(ds_readbs(CURRENT_TOWN) + 235));

		return (RealPt)ds_readd(TEXT_OUTPUT_BUF);
	} else {
		return ptr;
	}
}

/* 0x70b */
/**
 * \brief   drink while sitting in a tavern
 *
 * \param   amount      how much amount fluid you get
 *
 * \todo    This function is only called while sitting in a tavern.
 *          When you enter a tavern and order food another function is called.
 *          Also this function is called in taverns with amount = 10,
 *          and in the thorwalian dungeon with 100.
 */
void drink_while_drinking(signed short amount)
{
	Bit8u *hero;
	signed short i;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero)) {

			/* sub fluid amount */
			host_writeb(hero + HERO_THIRST, host_readb(hero + HERO_THIRST) - amount);

			/* adjust food counter */
			if (host_readbs(hero + HERO_THIRST) < 0) {
				host_writeb(hero + HERO_THIRST, 0);
			}
		}
	}
}

/**
 * \brief   eat food while sitting in a tavern
 *
 * \param   amount      how much food you get
 *
 * \todo    This function is only called while sitting in a tavern.
 *          When you enter a tavern and order food another function is called.
 *          Also this function is called only at one play with amount = 100,
 *          so there is space for tuning.
 */
void eat_while_drinking(signed short amount)
{
	Bit8u *hero;
	signed short i;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero)) {

			/* sub food amount */
			host_writeb(hero + HERO_HUNGER, host_readb(hero + HERO_HUNGER) - amount);

			/* adjust food counter */
			if (host_readbs(hero + HERO_HUNGER) < 0) {
				host_writeb(hero + HERO_HUNGER, 0);
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
