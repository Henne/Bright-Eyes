/*
	Rewrite of DSA1 v3.02_de functions of seg031 (???)
	Functions rewritten: 9/10
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

struct s2 {
	signed short a;
	signed short b;
};

/* Borlandified and identical */
void do_random_talk(signed short talk_id, signed short informer_id)
{
	signed short l_si;
	signed short l_di = 0;
	signed short l3;
	signed short l4;
	signed short l5;
	signed short l6;
	signed short l7;
	signed short l8;
	signed short l9;
	signed short l10;
	signed short l11;
	signed short l12;
	Bit8u *p1;
	Bit8u *p2;
	Bit8u *p3;
	Bit8u *p4;
	char *dst;
	char *fmt;
	signed short l13;
	signed short l14;
	signed short l15;
	signed short l16;
	signed short l17;
	signed short l18;
	struct s2 arr[3];

	ds_writew(DIALOG_INFORMER, informer_id);
	ds_writew(TLK_ID, talk_id);

	load_tlk(talk_id + 156);
	ds_writew(DIALOG_STATE, ds_writew(DIALOG_DONE, 0));
	p3 = p_datseg + 0x3618;
	p2 = Real2Host(host_readds(p3 + 38 * informer_id));
	l4 = host_readws(p3 + 4 + 38 * informer_id);
	p4 = 38 * informer_id + p3 + 6;
	load_in_head(host_readws(p3 + 38 * informer_id + 36));
	dst = (char*)Real2Host(ds_readd(DTP2)) + 0x400;

	do {
		l_di = l_si = 0;
		p1 = 8 * ds_readws(DIALOG_STATE) + p2;

		if (ds_readws(TLK_ID) == 13 && ds_readws(DIALOG_STATE) >= 20) {
			l9 = l10 = l11 = l12 = 0;
		} else {
			l9 = random_schick(4) - 1;
			l10 = random_schick(4) - 1;
			l11 = random_schick(4) - 1;
			l12 = random_schick(4) - 1;
		}

		if ((l8 = host_readws(p1)) != -1) {
			l8 = 4 * l8 + l9;
		}
		if (host_readb(p1 + 2) != 0) {
			l5 = 4 * host_readb(p1 + 2) + l10;
			l_si++;
		}
		if (host_readb(p1 + 3) != 0) {
			l6 = 4 * host_readb(p1 + 3) + l11;
			l_si++;
		}
		if (host_readb(p1 + 4) != 0) {
			l7 = 4 * host_readb(p1 + 4) + l12;
			l_si++;
		}

		if (l8 != -1) {

			l3 = (4 * host_readw(p1) + l9) & 0x7fff;
			fmt = (char*)get_dtp(4 * (l3 + l4));

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

			arr[0].a = l5 + l4;
			arr[0].b = host_readb(p1 + 5);
			arr[1].a = l6 + l4;
			arr[1].b = host_readb(p1 + 6);
			arr[2].a = l7 + l4;
			arr[2].b = host_readb(p1 + 7);

			if (l_si) {

				l15 = random_schick(5);
				for (l16 = 0; l16 < l15; l16++) {

					l13 = random_schick(3) - 1;
					l14 = random_schick(3) - 1;

					l17 = arr[l13].a;
					l18 = arr[l14].a;

					if (l17 != l4 && l18 != l4) {
						arr[l13].a = l18;
						arr[l14].a = l17;
						l17 = arr[l13].b;
						arr[l13].b = arr[l14].b;
						arr[l14].b = l17;
					}
				}
			}

			l_di = GUI_dialogbox((RealPt)ds_readd(DTP2), p4, (Bit8u*)dst, l_si,
					get_dtp(4 * arr[0].a),
					get_dtp(4 * arr[1].a),
					get_dtp(4 * arr[2].a));

		} else {
			arr[0].b = host_readb(p1 + 5);
		}

		ds_writews(0xe30e, -1);
		if ((host_readw(p1) & 0x8000) || host_readws(p1) == -1) {
			talk_switch();
		}

		ds_writew(DIALOG_STATE, ds_readws(0xe30e) == -1 ? arr[0].b : ds_readws(0xe30e));

		if (ds_readws(DIALOG_DONE) == 0) {

			if (l_si) {

				if (l_di == -1) {
					ds_writew(DIALOG_DONE, 1);
				} else if (l_di == 1) {
					ds_writew(DIALOG_STATE, arr[0].b);
				} else if (l_di == 2) {
					ds_writew(DIALOG_STATE, arr[1].b);
				} else if (l_di == 3) {
					ds_writew(DIALOG_STATE, arr[2].b);
				}
			}

			if (ds_readws(DIALOG_STATE) == 255) {
				ds_writew(DIALOG_DONE, 1);
			}
		}

	} while (ds_readws(DIALOG_DONE) == 0);

	ds_writews(0x26bd, ds_writews(0x2ccf, -1));
	load_buffer_1(ds_readws(0x26bf));
}

/* Borlandified and identical */
/* This function is dead code */
RealPt get_informer_forename(void)
{
	signed short i;
	char tmp;
	Bit8u *p_info;
	Bit8u *informer_name;

	p_info = p_datseg + 0x5ed6;

	for (i = 0; i < 15; i++, p_info += 4) {

		if (host_readbs(p_info + 2) == ds_readbs(CURRENT_TOWN)) {

			i = 0;
			informer_name = get_ltx(4 * host_readws(p_info));

			do {
				tmp = host_readbs(informer_name);
				informer_name++;
				i++;
			} while (tmp != ' ');

			strncpy((char*)Real2Host(ds_readd(0xd2eb)), (char*)get_ltx(4 * host_readws(p_info)), i);
#ifdef M302de_ORIGINAL_BUGFIX
			break;
#endif
		}
	}
	return (RealPt)ds_readd(0xd2eb);
}

/**
 * \brief ?
 *
 * return a value between 0 and 15
 */
/* Borlandified and identical */
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

/* 0x5a0 */

/**
 * get_informer_hint() - gives a hint where a informer lives
 *
 * Returns a pointer to the string.
 *
 * Game Info: You can ask in some towns where informers live.
 * This function returns a pointer to the answer or to an empty string.
 */
RealPt get_informer_hint(void)
{
	Bit8u *ptr;
	signed short i;	/* cx */

	ptr = p_datseg + INFORMER_TAB;
	for (i = 0; i < 15; i++, ptr += 4) {
		if (host_readb(ptr + 2) == ds_readb(CURRENT_TOWN)) {
			return (RealPt)host_readd(Real2Host(ds_readd(TEXT_LTX) + (i + 0x2cb) * 4));
		}
	}

	return (RealPt)host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0xb54);
}

/* 0x5f2 */
/**
 * get_informer_name() - get the name on the current informer
 *
 */
RealPt get_informer_name(void)
{
	return (RealPt)host_readd(Real2Host(ds_readd(TEXT_LTX)) + ds_readw(INFORMER_TAB - 4 + ds_readb(CURRENT_INFORMER) * 4) * 4);
}

/* 0x617 */
/**
 * get_informer_name2() - get the name of the informer in this town
 *
 */
RealPt get_informer_name2(void)
{
	return (RealPt)host_readd(Real2Host(ds_readd(TEXT_LTX)) +
			ds_readw(INFORMER_TAB + get_town_lookup_entry() * 4) * 4);
}

/* 0x63b */
/**
 * load_current_town_gossip() - loads a gossip message from the current town
 *
 * Returns a pointer to the message.
 */
RealPt load_current_town_gossip(void)
{
	RealPt ptr;
	unsigned short gossip_id;

	/* load TOWN.LTX */
	load_ltx(ds_readb(CURRENT_TOWN) + 0x4d);

	/* mark some buffers invalid */
	ds_writew(CURRENT_ANI, 0xffff);
	ds_writew(0x2ccb, 0xffff);

	/* get the pointer to the ltx buffer */
	ptr = (RealPt)ds_readd(0xd019);

	/* get some gossip */
	gossip_id = get_tavern_gossip();

	/* return the pointer to the gossip (pointers are stored in the first 1000 bytes) */
	return (RealPt)host_readd(Real2Host(ptr) + gossip_id * 4);
}

/* 0x70b */
/**
 * drink_while_drinking() - drink while sitting in a tavern
 * @amount:	how much amount fluid you get
 *
 * TODO:
 * This function is only called while sitting in a tavern.
 * When you enter a tavern and order food another function is called.
 * Also this function is called in taverns with amount = 10,
 * and in the thorwalian dungeon with 100.
 */
void drink_while_drinking(unsigned short amount)
{
	Bit8u *hero;
	unsigned short i;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += 0x6da) {

		/* is hero valid */
		if (host_readb(hero + 0x21) == 0)
			continue;

		/* is hero in group */
		if (host_readb(hero + 0x87) != ds_readb(CURRENT_GROUP))
			continue;

		/* hero is dead */
		if (hero_dead(hero))
			continue;

		/* sub fluid amount */
		host_writeb(hero + 0x80, host_readb(hero + 0x80) - amount);

		/* adjust food counter */
		if (host_readbs(hero + 0x80) < 0) {
			host_writeb(hero + 0x80, 0);
		}
	}
}

/* 0x77d */
/**
 * eat_while_drinking() - eat food while sitting in a tavern
 * @amount:	how much food you get
 *
 * TODO:
 * This function is only called while sitting in a tavern.
 * When you enter a tavern and order food another function is called.
 * Also this function is called only at one play with amount = 100,
 * so there is space for tuning.
 */
void eat_while_drinking(unsigned short amount)
{
	Bit8u *hero;
	unsigned short i;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += 0x6da) {

		/* is hero valid */
		if (host_readb(hero + 0x21) == 0)
			continue;

		/* is hero in group */
		if (host_readb(hero + 0x87) != ds_readb(CURRENT_GROUP))
			continue;

		/* hero is dead */
		if (hero_dead(hero))
			continue;

		/* sub food amount */
		host_writeb(hero + 0x7f, host_readb(hero + 0x7f) - amount);

		/* adjust food counter */
		if (host_readbs(hero + 0x7f) < 0) {
			host_writeb(hero + 0x7f, 0);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
