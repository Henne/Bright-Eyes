/*
	Rewrite of DSA1 v3.02_de functions of seg031 (???)
	Functions rewritten: 7/10
*/

#include "schick.h"

#include "v302de.h"

#include "seg026.h"
#include "seg073.h"

namespace M302de {

/**
 * get_town_lookup_entry() - ?
 *
 * return a value between 0 and 15
 */
signed short get_town_lookup_entry(void)
{
	Bit8u *ptr;
	register signed short i;

	ptr = p_datseg + 0x5ed6;
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
	register signed short i;	/* cx */

	ptr = p_datseg + 0x5ed6;
	for (i = 0; i < 15; i++, ptr += 4) {
		if (host_readb(ptr + 2) == ds_readb(CURRENT_TOWN)) {
			return host_readd(Real2Host(ds_readd(TEXT_LTX) + (i + 0x2cb) * 4));
		}
	}

	return host_readd(Real2Host(ds_readd(TEXT_LTX)) + 0xb54);
}

/* 0x5f2 */
/**
 * get_informer_name() - get the name on the current informer
 *
 */
RealPt get_informer_name(void)
{
	return host_readd(Real2Host(ds_readd(TEXT_LTX)) + ds_readw(0x5ed6 - 4 + ds_readb(CURRENT_INFORMER) * 4) * 4);
}

/* 0x617 */
/**
 * get_informer_name2() - get the name of the informer in this town
 *
 */
RealPt get_informer_name2(void)
{
	return host_readd(Real2Host(ds_readd(TEXT_LTX)) +
			ds_readw(0x5ed6 + get_town_lookup_entry() * 4) * 4);
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
	ptr = ds_readd(0xd019);

	/* get some gossip */
	gossip_id = get_tavern_gossip();

	/* return the pointer to the gossip (pointers are stored in the first 1000 bytes) */
	return host_readd(Real2Host(ptr) + gossip_id * 4);

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
	register unsigned short i;

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
		if ((signed char)host_readb(hero + 0x80) < 0) {
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
	register unsigned short i;

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
		if ((signed char)host_readb(hero + 0x7f) < 0) {
			host_writeb(hero + 0x7f, 0);
		}
	}
}

}
