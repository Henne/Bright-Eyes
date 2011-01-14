/*
 *	Rewrite of DSA1 v3.02_de functions of seg039 (fight)
 *	Functions rewritten 3/7
*/

#include <stdlib.h>
#include <string.h>

#include "schick.h"

#include "v302de.h"

#include "seg007.h"

/* is used at selecting a target */
signed short seg039_0000(unsigned short v1, unsigned short v2, unsigned short v3, unsigned short v4) {
	return abs(v1 - v3) + abs(v2 - v4);
}

signed short seg039_0023(Bit8u *hero) {
	Bit8u *ptr;
	signed short retval = -1;
	unsigned short weapon;

	/* get equipped weapon of the hero */
	weapon = host_readw(hero + 0x1c0);
	/* make a pointer to the entry of ITEMS.DAT */
	ptr = get_itemsdat(weapon);

	D1_LOG("weapon 0x%x +2 0x%x +3 0x%x\n", weapon,
		host_readb(ptr + 2), host_readb(ptr + 3));

	if (((host_readb(ptr + 2) >> 1) & 1) == 0)
		return retval;

	/* weapons are not MagicStaffs or Fightstaffs */
	if (host_readb(ptr + 3) == 5 && weapon != 0x85 && weapon != 0x45)
		return 5;

	if (host_readb(ptr + 3) == 7)
		return 3;

	if (host_readb(ptr + 3) == 8)
		return 4;

	return -1;
}

/**
 * fill_enemy_sheet - fills an enemies sheet from a template
 * @sheet_nr:	the number of the sheet
 * @enemy_id:	the ID of the enemy (MONSTER.DAT)
 * @round:	the fight round the enemy appears
 */
void fill_enemy_sheet(unsigned short sheet_nr, signed char enemy_id, unsigned char round) {

	Bit8u *sheet;
	Bit8u *temp;
	unsigned short i;

	/* calculate the pointers */
	temp = MemBase + Real2Phys(ds_readd(0xe125)) + enemy_id * 44;
	sheet = MemBase + PhysMake(datseg, 0xd34b) + sheet_nr * 62;

	/* erease the sheet */
	memset(sheet, 0, 62);

	/* copy enemy id, gfx_id and RS to the sheet */
	host_writeb(sheet + 0, host_readb(temp + 0));
	host_writeb(sheet + 1, host_readb(temp + 1));
	host_writeb(sheet + 2, host_readb(temp + 2));

	/* roll attributes  and save them to the sheet */
	for (i = 0; i < 7; i++) {
		signed char tmp;

		tmp = dice_template(host_readw(temp + i * 2 + 3));
		host_writeb(sheet + i * 2 + 4, tmp);
		host_writeb(sheet + i * 2 + 3, tmp);
	}

	/* roll out LE and save it to the sheet */
	host_writew(sheet + 0x11, dice_template(host_readw(temp + 0x11)));
	/* recalculate LE = LE / 6 * 5; */
	host_writew(sheet + 0x11, host_readw(sheet + 0x11) / 6 * 5);
	/* copy LE*/
	host_writew(sheet + 0x13, host_readw(sheet + 0x11));

	/* roll out AE and save it to the sheet */
	host_writew(sheet + 0x17, dice_template(host_readw(temp + 0x13)));
	host_writew(sheet + 0x15, host_readw(sheet + 0x17));

	/* roll out MR  and save it */
	host_writeb(sheet + 0x19, dice_template(host_readw(temp + 0x15)));

	/* Terrible hack:
		if the current fight is 0xbc, set MR to 5 (Travel-Event 84),
		if the current fight is 0xc0, and the enemy
		is no "Orkcachampion" then set a flag */
	if (ds_readw(0xe316) == 0xbc)
		host_writeb(sheet + 0x19, 5);
	else if (ds_readw(0xe316) == 0xc0 && host_readb(sheet) == 0x48)
			host_writeb(sheet + 0x31,
				host_readb(sheet + 0x31) | 0x20);

	/* copy the first encounter AP */
	host_writeb(sheet + 0x1a, host_readb(temp + 0x17));
	/* copy the number of attacks */
	host_writeb(sheet + 0x1b, host_readb(temp + 0x18));
	/* copy AT value */
	host_writeb(sheet + 0x1c, host_readb(temp + 0x19));
	/* copy PA value */
	host_writeb(sheet + 0x1d, host_readb(temp + 0x1a));

	/* Damage first attack ? */
	host_writew(sheet + 0x1e, host_readw(temp + 0x1b));
	/* Damage second attack ? */
	host_writew(sheet + 0x20, host_readw(temp + 0x1d));

	/* copy BP */
	host_writeb(sheet + 0x22, host_readb(temp + 0x1f));

	/* set BP to 10 if greater */
	if (host_readb(sheet + 0x22) > 10)
		host_writeb(sheet + 0x22, 10);

	/* copy imunnity against non-magicial weapons ? */
	host_writeb(sheet + 0x24, host_readb(temp + 0x20));
	/* copy ID Magicuser class ? */
	host_writeb(sheet + 0x25, host_readb(temp + 0x21));

	/* bogus this value is 0x00 or 0x20 */
	host_writeb(sheet + 0x31, host_readb(sheet + 0x31) & 0xfe);

	host_writeb(sheet + 0x26, 0xff);

	/* copy Level */
	host_writeb(sheet + 0x29, host_readb(temp + 0x22));
	/* copy size class */
	host_writeb(sheet + 0x34, host_readb(temp + 0x23));
	/* copy flags */
	host_writeb(sheet + 0x36, host_readb(temp + 0x24));

	/* set the fight round the enemy appears */
	host_writeb(sheet + 0x35, round);

	/* set to ? */
	host_writeb(sheet + 0x27,
		host_readb(MemBase + Real2Phys(ds_readd(0xbd28)) + sheet_nr * 5 + 0x19));

	/* copy number of ammo */
	host_writeb(sheet + 0x37, host_readb(temp + 0x25));
	/* copy damage of ammo */
	host_writew(sheet + 0x38, host_readw(temp + 0x26));

	/* copy number of thorowing weapons */
	host_writeb(sheet + 0x3a, host_readb(temp + 0x28));
	/* copy damage of thorowing weapons */
	host_writew(sheet + 0x3b, host_readw(temp + 0x29));

	/* copy LE fleeing value */
	host_writeb(sheet + 0x3d, host_readb(temp + 0x2b));

	/* Another hack:
		If the current fight == 0x5e and the enemy is "Kultist",
		set a flag */
	if (ds_readw(0xe316) == 0x5e && host_readb(sheet) == 0x38)
		host_writeb(sheet + 0x32, host_readb(sheet + 0x32) | 4);
}
