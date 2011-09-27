#include <string.h>

#include "schick.h"

#include "v302de.h"

#include "seg007.h"

namespace M302de {

/**
 *	FIG_clear_msgs() - clears the fight messages buffer
*/
void FIG_clear_msgs() {
	memset(MemBase + PhysMake(datseg, 0xd333), 0 , 20);
	ds_writew(0x26ad, 0);
}

void FIG_add_msg(unsigned short f_action, unsigned short damage) {
	ds_writew(0xd333 + 4 * ds_readw(0x26ad), f_action);
	ds_writew(0xd333 + 4 * ds_readw(0x26ad) + 2 , damage);
	if (ds_readw(0x26ad) < 4)
		ds_writew(0x26ad, ds_readw(0x26ad) + 1);
}

void seg041_8c8() {
	unsigned short i;

	for (i = 0; i < 8; i++)
		memset(MemBase + PhysMake(datseg, 0xd8ce) + i * 0xf3, 0xffff, 0xf3);

//		That would be better
//		memset(MemBase + PhysMake(datseg, 0xd8ce), 0xffff, 0xf3 * 8);
}

signed short weapon_check(Bit8u *hero) {

	Bit8u *item_p;
	unsigned short item;

	/* get the number of the equipped weapon */
	item = host_readw(hero + 0x1c0);

	item_p = get_itemsdat(item);

	/* check if its a weapon */
	if ((host_readw(item_p + 2) >> 1) & 1 == 0)
		return -1;

	if (host_readb(hero + 0x1c4) & 1)
		return -1;

	/* this test is always true */
	if ((host_readw(item_p + 2) >> 1) & 1) {
		unsigned char pos = host_readb(item_p + 3);

		if (pos == 7 || pos == 8)
			return -1;

		/* At the legs, and no staffs */
		if (pos == 5 && item != 133 && item != 69)
			return -1;
	}

	if (is_in_word_array(item, MemBase + PhysMake(datseg, 0x615c)))
		return 1;

	if (is_in_word_array(item, MemBase + PhysMake(datseg, 0x616e)))
		return 0;

	return 2;
}

}
