/*
	Rewrite of DSA1 v3.02_de functions of seg031 (???)
	Functions rewritten: 1/10
*/

#include "schick.h"

#include "v302de.h"

#include "seg026.h"
#include "seg073.h"

namespace M302de {

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
