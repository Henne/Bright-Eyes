/*
	Rewrite of DSA1 v3.02_de functions of seg031 (???)
	Functions rewritten: 2/10
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

}
