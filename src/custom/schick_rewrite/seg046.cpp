/*
	Rewrite of DSA1 v3.02_de functions of seg046 (Status)
	Functions rewritten: 2/4
*/

#include "schick.h"
#include "v302de.h"

#include "seg096.h"

/**
 *	status_show_spell -	prints spellname and value
 *	@hero:	the hero the spell is from
 *	@spell:	spellnumber
 *	@fsig:	the first spell in the spellgroup
 *	@x1:	the leftmost x coordinate
 *	@x2:	the rightmost x coordinate
 *	@gy:	the upper y coordinate of this spellgroup
 */
void status_show_spell(Bit8u *hero, unsigned short spell, unsigned short fsig,
			unsigned short x1, unsigned short x2, unsigned short gy) {
	char str[10];
	unsigned short group;
	unsigned short y;

	group = spell - fsig;
	y = group * 7 + gy;

	/* print spellname */
	GUI_print_string(get_ltx((spell + 0x6a) * 4), x1, y);

	/* convert value to string */
	sprintf(str, "%d", (signed char)host_readb(hero + 0x13d + spell));

	/* print value */
	GUI_print_string((Bit8u*)str, x2 - GUI_get_space_for_string((Bit8u*)str, 0), y);
}

/**
 *	status_show_talent -	prints talentname and value
 *	@hero:	the hero the talent is from
 *	@talen:	talentnumber
 *	@ftig:	the first talent in the talentgroup
 *	@x1:	the leftmost x coordinate
 *	@x2:	the rightmost x coordinate
 *	@gy:	the upper y coordinate of this talentgroup
 */
void status_show_talent(Bit8u *hero, unsigned short talent, unsigned short ftig,
			unsigned short x1, unsigned short x2, unsigned short gy) {
	char str[10];
	unsigned short group;
	unsigned short y;

	group = talent - ftig;
	y = group * 7 + gy;

	/* print talentname */
	GUI_print_string(get_ltx((talent + 0x30) * 4), x1, y);

	/* convert value to string */
	sprintf(str, "%d", (signed char)host_readb(hero + 0x108 + talent));

	/* print value */
	GUI_print_string((Bit8u*)str, x2 - GUI_get_space_for_string((Bit8u*)str, 0), y);
}
