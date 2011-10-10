/*
        Rewrite of DSA1 v3.02_de functions of seg041 (fight)
        Functions rewritten: 4/9
*/
#include <string.h>

#include "schick.h"

#include "v302de.h"

#include "seg007.h"

namespace M302de {

/* DS:0x26ad */
static unsigned short msg_counter;

/**
 *	FIG_clear_msgs() - clears the fight messages buffer
*/
void FIG_clear_msgs() {
	memset(MemBase + PhysMake(datseg, 0xd333), 0 , 20);
	msg_counter = 0;
}

void FIG_add_msg(unsigned short f_action, unsigned short damage) {
	ds_writew(0xd333 + 4 * msg_counter, f_action);
	ds_writew(0xd333 + 4 * msg_counter + 2 , damage);
	if (msg_counter < 4)
		msg_counter++;
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
