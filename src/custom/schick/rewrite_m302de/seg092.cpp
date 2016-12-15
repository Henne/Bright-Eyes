/**
 *	Rewrite of DSA1 v3.02_de functions of seg092 (treasures)
 *	Functions rewritten: 22/22 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg092.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg047.h"
#include "seg076.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
#include "t_map.h"
#endif

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void chest_protected_normal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x818));

	/* the first hero gets wounded with 1W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), random_schick(6));
}

void chest_closed(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x81c));
}

void chest_protected_heavy(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x820));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(2, 6, 0));
}

void chest_poisoned1(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x820));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host((RealPt)(ds_writed(0x3e20, (Bit32u)get_first_hero_available_in_group()))), dice_roll(2, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(Real2Host(ds_readd(0x3e20)), 1);
}

void chest_poisoned2(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x820));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host((RealPt)(ds_writed(0x3e20, (Bit32u)get_first_hero_available_in_group()))), dice_roll(2, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(Real2Host(ds_readd(0x3e20)), 2);
}

void chest_poisoned3(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x820));

	/* the first hero gets wounded with 1W6 */
	sub_hero_le(Real2Host((RealPt)(ds_writed(0x3e20, (Bit32u)get_first_hero_available_in_group()))), dice_roll(1, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(Real2Host(ds_readd(0x3e20)), 8);
}

void chest_protected_brutal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x820));

	/* the first hero gets wounded with 4W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(4, 6, 0));
}

void chest_stoned(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0xc20));

	/* save pointer of the first hero */
	ds_writed(0x3e20, (Bit32u)get_first_hero_available_in_group());

	/* and make him stoned */
	or_ptr_bs(Real2Host(ds_readd(0x3e20)) + 0xaa, 0x04);
}

void chest_ignifax_normal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0xc24));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(2, 6, 0));
}

void chest_ignifax_brutal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0xc24));

	/* the first hero gets wounded with 4W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(4, 6, 0));
}

void chest_ignifax_heavy(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0xc24));

	/* the first hero gets wounded with 3W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(3, 6, 0));
}

void chest_crossbow_bolts(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0xc50));

	/* the first hero gets wounded with 2W6+6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(2, 6, 6));
}

void chest_cursed(void)
{
	signed short i;
	Bit8u *hero;

	/* get the group leader */
	hero = Real2Host(get_first_hero_available_in_group());

	if (!hero_transformed(hero)) {

		/* set transformed flag */
		or_ptr_bs(hero + HERO_STATUS2, 0x40);

		/* decrement each good attribute */
		for (i = 0; i <= 6; i++) {
			dec_ptr_bs(hero + HERO_MU + 3 * i);
		}

		/* print a message */
		print_msg_with_first_hero(get_ltx(0x8cc));
	}
}

void chest_fulminictus(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x8d0));

	/* the first hero gets wounded with 4W6+5 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(4, 6, 5));
}


/**
 * \brief loot a chest, with standard text messages
 * \param chest			pointer to the chest
 */
void loot_simple_chest(Bit8u *chest)
{
	signed short item_nr;
	signed short item_id;
	signed short tw_bak;
	char names[20][30];

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writews(TEXTBOX_WIDTH, 7);

	host_writeb(chest + 0x2, 0);

	do {

		item_nr = 0;

		/* write the names of the items in the chest into names[] */
		while((item_id = host_readb(Real2Host(host_readd(chest + 0x0b)) + item_nr)) != (signed short)0x00ff) {


			strcpy(names[item_nr++],
				(char*)Real2Host(GUI_name_plural(0, (Bit8u*)get_itemname(item_id))));
		}

		if (item_nr == 0) {
			/* this chest is empty */
			GUI_output(get_ltx(0x828));
			break;
		} else {

			/* show radio menu with item names */
			item_nr = GUI_radio(get_ltx(0x824), (signed char)item_nr,
						names[0], names[1], names[2], names[3],
						names[4], names[5], names[6], names[7],
						names[8], names[9], names[10], names[11],
						names[12], names[13], names[14], names[15],
						names[16], names[17], names[18], names[19]) - 1;

			if (item_nr != -2) {
				/* if not pressed ESC */
				if (get_item(host_readb(Real2Host(host_readd(chest + 0xb)) + item_nr), 1, 1))
				{
					/* got the item in inventory => remove from chest */
					delete_chest_item(chest, item_nr);
				} else {
					/* group has not taken the item */
					item_nr = -2;
				}
			}
		}

	} while (item_nr != -2);

	ds_writews(TEXTBOX_WIDTH, tw_bak);
}

/**
 * \brief deletes an item from a chest
 * \param chest		pointer to the chest
 * \param item_nr	the number of the item to be deleted
 */
void delete_chest_item(Bit8u *chest, signed short item_nr)
{
	signed char tmp;

	do {
#if defined(__BORLANDC__)
		Real2Host(host_readd(chest + 0xb))[item_nr] = tmp = Real2Host(host_readd(chest + 0xb))[item_nr + 1];
#else
		host_writeb(Real2Host(host_readd(chest + 0xb)) + item_nr,
			tmp = host_readbs(Real2Host(host_readd(chest + 0xb)) + item_nr + 1));
#endif
		item_nr++;

	} while (tmp != -1);

}

/**
 * \brief loot a chest, with different text messages
 * \param chest			pointer to the chest
 * \param text_non_empty	shown text if chest is not empty
 * \param text_empty		shown text if chest is empty
 */
void loot_chest(Bit8u *chest, Bit8u *text_non_empty, Bit8u *text_empty)
{
	signed short item_nr;
	signed short item_id;
	signed short tw_bak;
	char names[20][20];

	host_writeb(chest + 0x2, 0);

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writews(TEXTBOX_WIDTH, 7);

	do {

		item_nr = 0;

		/* write the names of the items in the chest into names[] */
		while((item_id = host_readb(Real2Host(host_readd(chest + 0x0b)) + item_nr)) != (signed short)0x00ff) {


			strcpy(names[item_nr++],
				(char*)Real2Host(GUI_name_plural(0, (Bit8u*)get_itemname(item_id))));
		}

		if (item_nr == 0) {
			/* this chest is empty */
			GUI_output(text_empty);
			break;
		} else {

			/* show radio menu with item names */
			item_nr = GUI_radio(text_non_empty, (signed char)item_nr,
						names[0], names[1], names[2], names[3],
						names[4], names[5], names[6], names[7],
						names[8], names[9], names[10], names[11],
						names[12], names[13], names[14], names[15],
						names[16], names[17], names[18], names[19]) - 1;

			if (item_nr != -2) {
				/* if not pressed ESC */
				if (get_item(host_readb(Real2Host(host_readd(chest + 0xb)) + item_nr), 1, 1))
				{
					/* got the item in inventory => remove from chest */
					delete_chest_item(chest, item_nr);
				} else {
					/* group has not taken the item */
					item_nr = -2;
				}
			}
		}

	} while (item_nr != -2);

	ds_writews(TEXTBOX_WIDTH, tw_bak);
}

/**
 * \brief	check if a hero has lockpicks
 * \param hero	pointer to the hero
 * \return	-1 = no lockpicks, -2 = all lockpicks are broken, else position of the lockpicks
 */
signed short hero_has_lockpicks(Bit8u *hero)
{
	signed short retval = -1;
	signed short i;

	/* in each knapsack slot... */
	for (i = 0; i < 23; i++) {

		/* ... check for lockpicks ... */
		if (host_readws(hero + HERO_ITEM_HEAD + 14 * i) == 35) {

			/* ... which are not broken */
			if (!ks_broken(hero + HERO_ITEM_HEAD + 14 * i)) {
				return i;
			} else {
				retval = -2;
			}
		}
	}

	return retval;
}

void (*func)(RealPt);

struct chest {
	signed short id;
	signed char mod;
	void (*func1)(RealPt);
	void (*func2)(void);
	void (*func3)(RealPt);
	unsigned short ap;
	unsigned short money;
	signed short food;
};

void seg092_06b4(signed short a1)
{
	signed short x;
	signed short y;
	signed short chest_id;
	signed short l4;
	RealPt chest_ptr;
	Bit8u *ptr;

#if !defined(__BORLANDC__)
	const int dng = ds_readbs(DUNGEON_INDEX);
	if (dng == 2 || dng == 3 || dng == 8)
	{
		DUMMY_WARNING();
		return;
	}
#endif
	chest_ptr = (RealPt)ds_readd(0x9d84 + 4 * ds_readbs(DUNGEON_INDEX));
	ptr = p_datseg + 0xbd95;
	ds_writew(GET_EXTRA_LOOT, 0);
	x = ds_readws(X_TARGET);
	y = ds_readws(Y_TARGET);

	if (a1 != 0) {
		switch(ds_readbs(DIRECTION)) {
			case 0: y--; break;
			case 1: x++; break;
			case 2: y++; break;
			case 3: x--; break;
		}
	}

	l4 = host_readb(ptr + 16 * y + x) & 2;
	chest_id = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * x + y;

	play_voc(ARCHIVE_FILE_FX13_VOC);

	do {

		if (host_readws(Real2Host(chest_ptr)) == chest_id) {

			if (l4 != 0 && host_readd(Real2Host(chest_ptr) + 11)) {
#if defined(__BORLANDC__)
				((void (*)(RealPt))((RealPt)host_readd(Real2Host(chest_ptr) + 11)))(chest_ptr);
#else
				(t_map(chest_ptr, 11)(chest_ptr));
#endif
			} else if (host_readbs(Real2Host(chest_ptr) + 2) != 0) {
#if defined(__BORLANDC__)
				((void (*)(RealPt))((RealPt)host_readd(Real2Host(chest_ptr) + 3)))(chest_ptr);
#else
				(t_map(chest_ptr, 3)(chest_ptr));
#endif
			} else if ((RealPt)host_readd(Real2Host(chest_ptr) + 3)) {
#if defined(__BORLANDC__)
				((void (*)(RealPt))((RealPt)host_readd(Real2Host(chest_ptr) + 3)))(chest_ptr);
#else
				(t_map(chest_ptr, 3)(chest_ptr));
#endif
			} else if ((RealPt)host_readd(Real2Host(chest_ptr) + 11)) {
#if defined(__BORLANDC__)
				((void (*)(RealPt))((RealPt)host_readd(Real2Host(chest_ptr) + 11)))(chest_ptr);
#else
				(t_map(chest_ptr, 11)(chest_ptr));
#endif
				ds_writew(GET_EXTRA_LOOT, 1);
			} else if (host_readws(Real2Host(chest_ptr) + 17) != 0) {
				ds_writew(GET_EXTRA_LOOT, 1);
			}

			break;
		}

#if !defined(__BORLANDC__)
		chest_ptr += 21;
	} while (host_readws(Real2Host(chest_ptr)) != -1);
#else
	} while (host_readws(Real2Host(((struct chest*)chest_ptr)++)) != -1);
#endif

	if (l4 == 0 && ds_readws(GET_EXTRA_LOOT) != 0) {

		if (host_readws(Real2Host(chest_ptr) + 15) != 0) {
			/* There are AP in the chest */
			add_hero_ap_all(host_readws(Real2Host(chest_ptr) + 15));
		}

		if (host_readws(Real2Host(chest_ptr) + 17) != 0) {

			/* There is money in the chest */
			make_valuta_str((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)), host_readw(Real2Host(chest_ptr) + 17));
			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_ltx(0xc64),
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
			GUI_output(Real2Host(ds_readd(DTP2)));

			set_party_money(get_party_money() + host_readw(Real2Host(chest_ptr) + 17));
		}

		if (host_readws(Real2Host(chest_ptr) + 19) != 0) {
			/* There are FOOD PACKAGES in the chest */
			get_item(45, 1, host_readws(Real2Host(chest_ptr) + 19));
		}
	}
}

void use_lockpicks_on_chest(RealPt chest_ptr)
{
	signed short l_si;
	signed short l_di;
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());

	if ((l_si = hero_has_lockpicks(hero)) != -1) {

		if (l_si != -2) {

			l_di = test_skill(hero, 48, host_readbs(Real2Host(chest_ptr) + 2));

			if (l_di == -99) {
				/* unlucky, your lockpicks break... */

				print_msg_with_first_hero(get_ltx(0x854));
				or_ptr_bs(hero + (HERO_ITEM_HEAD+4) + 14 * l_si, 1);

				/* ... and you trigger the trap */
#if !defined(__BORLANDC__)
				if (t_map(chest_ptr, 7) != NULL) {
					((treasure_trap)(t_map(chest_ptr, 7)))();
				}
#else
				if ((RealPt)host_readd(Real2Host(chest_ptr) + 7)) {
					((void (*)(void))((RealPt)host_readd(Real2Host(chest_ptr) + 7)))();
				}
#endif

			} else if (l_di <= 0) {
				/* trigger the trap */
#if !defined(__BORLANDC__)
				if (t_map(chest_ptr, 7) != NULL) {
					((treasure_trap)(t_map(chest_ptr, 7)))();
				}
#else
				if ((RealPt)host_readd(Real2Host(chest_ptr) + 7)) {
					((void (*)(void))((RealPt)host_readd(Real2Host(chest_ptr) + 7)))();
				}
#endif

			} else {
				/* success */

				add_hero_ap(hero, 1);


#if !defined(__BORLANDC__)
				if (t_map(chest_ptr, 11) != NULL)
				{
					(t_map(chest_ptr, 11))(chest_ptr);

					if (((treasure_trap)(t_map(chest_ptr, 7))) == chest_protected_heavy)
					{
						add_hero_ap(hero, 5);
					}
				}
#else
				if ((RealPt)host_readd(Real2Host(chest_ptr) + 11))
				{
					((void (*)(RealPt))((RealPt)host_readd(Real2Host(chest_ptr) + 11)))(chest_ptr);

					if ((RealPt)host_readd(Real2Host(chest_ptr) + 7) == (RealPt)&chest_protected_heavy)
					{
						add_hero_ap(hero, 5);
					}
				}
#endif

				ds_writew(GET_EXTRA_LOOT, 1);
			}

		} else {
			print_msg_with_first_hero(get_ltx(0x84c));
		}
	} else {
		print_msg_with_first_hero(get_ltx(0x848));
	}
}

void use_key_on_chest(RealPt chest_ptr)
{
	signed short key_pos;
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());

	/* the leader of the group must have the key */
	if ((key_pos = get_item_pos(hero, host_readb(Real2Host(chest_ptr) + 2))) != -1)
	{

		if (!ks_broken(hero + HERO_ITEM_HEAD + 14 * key_pos))
		{

#if defined(__BORLANDC__)
			((void (*)(RealPt))((RealPt)host_readd(Real2Host(chest_ptr) + 11)))(chest_ptr);
#else
			t_map(chest_ptr, 11)(chest_ptr);
#endif

			ds_writew(GET_EXTRA_LOOT, 1);
		}
	} else {
#if defined(__BORLANDC__)
		((void (*)(void))((RealPt)host_readd(Real2Host(chest_ptr) + 7)))();
#else
		((treasure_trap)(t_map(chest_ptr, 7)))();
#endif
	}
}

void loot_multi_chest(Bit8u *chest, Bit8u *msg)
{
	unsigned short l_si;
	signed short item_nr;
	signed short item_cnt;
	signed short tw_bak;
	char temp_str[10];
	signed short len;
	char names[20][25];

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	do {

		item_nr = 0;
		while ((item_cnt = host_readb((item_nr + item_nr) + chest)) != 255) {

			names[item_nr][0] = '\0';

			if ((l_si = chest[item_nr + item_nr + 1]) > 1) {
#if defined(__BORLANDC__)
				itoa(l_si, names[item_nr], 10);
#else
				sprintf(names[item_nr], "%d", l_si);
#endif
				strcat(names[item_nr], (char*)p_datseg + 0x9dc4);
			}

			strcat(names[item_nr++], (char*)Real2Host(GUI_name_plural( ((signed short)(l_si > 1 ? (unsigned short)1 : (unsigned short)0)) ? 4 : 0, (Bit8u*)get_itemname(item_cnt))));
		}

		if (item_nr != 0) {

			item_nr = GUI_radio(msg, (signed char)item_nr,
				names[0], names[1], names[2], names[3],
				names[4], names[5], names[6], names[7],
				names[8], names[9], names[10], names[11],
				names[12], names[13], names[14], names[15],
				names[16], names[17], names[18], names[19]) - 1;

			if (item_nr != -2) {
				item_nr += item_nr;
#if defined(__BORLANDC__)
				itoa(chest[item_nr + 1], temp_str, 10);
#else
				sprintf(temp_str, "%d", host_readb(chest + item_nr + 1));
#endif
				len = strlen(temp_str);

				do {
					item_cnt = (l_si = chest[item_nr + 1]) > 1 ? GUI_input(get_ltx(0x944), len) : l_si;

				} while (item_cnt < 0);

				if (item_cnt > l_si) {
					item_cnt = l_si;
				}

				if (item_cnt != 0) {

					if (chest[item_nr] == 250) {
						add_party_money(item_cnt * 100L);
					} else {
						item_cnt = get_item(chest[item_nr], 1, item_cnt);
					}

					if (item_cnt == l_si) {

						do {
							chest[item_nr] = (unsigned char)(item_cnt = chest[item_nr + 2]);
							chest[item_nr + 1] = chest[item_nr + 3];
							item_nr += 2;
						} while (item_cnt != 255);

					} else if (item_cnt != 0) {
						chest[item_nr + 1] -= item_cnt;
					} else {
						item_nr = -2;
					}
				}
			}


		} else {
			item_nr = -2;
		}

	} while (item_nr != -2);

	ds_writew(TEXTBOX_WIDTH, tw_bak);
}

#if !defined(__BORLANDC__)
}
#endif
