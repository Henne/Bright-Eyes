/*
	Rewrite of DSA1 v3.02_de functions of seg046 (Status)
	Functions rewritten: 3/4
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

/**
 *	status_show_talents -	shows all talents and their values
 *	@hero:	the hero which talents should be shown
 */
void status_show_talents(Bit8u *hero) {

	unsigned short i, j;

	GUI_set_smth(0xff, 2);

	/* print talent category names */
	GUI_print_string(get_ltx(0x190),
		GUI_get_first_pos_centered(get_ltx(0x190), 5, 100, 0), 55);

	GUI_print_string(get_ltx(0x1a0),
		GUI_get_first_pos_centered(get_ltx(0x1a0), 110, 100, 0), 55);

	GUI_print_string(get_ltx(0x194),
		GUI_get_first_pos_centered(get_ltx(0x194), 215, 100, 0), 55);

	GUI_print_string(get_ltx(0x1a4),
		GUI_get_first_pos_centered(get_ltx(0x1a4), 5, 100, 0), 125);

	GUI_print_string(get_ltx(0x19c),
		GUI_get_first_pos_centered(get_ltx(0x19c), 110, 100, 0), 125);

	GUI_print_string(get_ltx(0x198),
		GUI_get_first_pos_centered(get_ltx(0x198), 215, 100, 0), 132);

	GUI_print_string(get_ltx(0x1a8),
		GUI_get_first_pos_centered(get_ltx(0x1a8), 5, 100, 0), 174);

	GUI_set_smth(0, 2);

	for (i = 0; i < 7; i++) {
		j = ds_readb(0x10ce + i * 2);
		while (ds_readb(0x10ce + i * 2) + ds_readb(0x10cf + i * 2) > j) {
			status_show_talent(hero, j,
				ds_readb(0x10ce + i * 2),
				ds_readw(0x6476 + i * 6),
				ds_readw(0x6478 + i * 6),
				ds_readw(0x647a + i * 6));
			j++;
		}
	}
}
