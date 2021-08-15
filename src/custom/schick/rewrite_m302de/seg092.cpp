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

#include "seg000.h"
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
	print_msg_with_first_hero(get_ttx(518));

	/* the first hero gets wounded with 1W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), random_schick(6));
}

void chest_closed(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(519));
}

void chest_protected_heavy(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(2, 6, 0));
}

void chest_poisoned1(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host((RealPt)(ds_writed(MAIN_ACTING_HERO, (Bit32u)get_first_hero_available_in_group()))), dice_roll(2, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(Real2Host(ds_readd(MAIN_ACTING_HERO)), 1);
}

void chest_poisoned2(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host((RealPt)(ds_writed(MAIN_ACTING_HERO, (Bit32u)get_first_hero_available_in_group()))), dice_roll(2, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(Real2Host(ds_readd(MAIN_ACTING_HERO)), 2);
}

void chest_poisoned3(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 1W6 */
	sub_hero_le(Real2Host((RealPt)(ds_writed(MAIN_ACTING_HERO, (Bit32u)get_first_hero_available_in_group()))), dice_roll(1, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(Real2Host(ds_readd(MAIN_ACTING_HERO)), 8);
}

void chest_protected_brutal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 4W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(4, 6, 0));
}

void chest_petrified(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(776));

	/* save pointer of the first hero */
	ds_writed(MAIN_ACTING_HERO, (Bit32u)get_first_hero_available_in_group());

	/* and make him petrified */
	or_ptr_bs(Real2Host(ds_readd(MAIN_ACTING_HERO)) + 0xaa, 0x04);
}

void chest_ignifax_normal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(777));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(2, 6, 0));
}

void chest_ignifax_brutal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(777));

	/* the first hero gets wounded with 4W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(4, 6, 0));
}

void chest_ignifax_heavy(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(777));

	/* the first hero gets wounded with 3W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(3, 6, 0));
}

void chest_crossbow_bolts(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(788));

	/* the first hero gets wounded with 2W6+6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(2, 6, 6));
}

void chest_cursed(void)
{
	/* cursed chest on the Totenschiff. 50D, but each good attribute of the group leader is decreased by 1.
	 * can be cured by 'Verwandlung beenden' spell or a Praios/Hesinde miracle */
	signed short i;
	Bit8u *hero;

	/* get the group leader */
	hero = Real2Host(get_first_hero_available_in_group());

	if (!hero_transformed(hero)) {

		or_ptr_bs(hero + HERO_FLAGS2, 0x40); /* set 'transformed' flag */

		/* decrement each good attribute */
		for (i = 0; i <= 6; i++) {
			dec_ptr_bs(hero + HERO_ATTRIB + 3 * i);
		}

		/* print a message */
		print_msg_with_first_hero(get_ttx(563));
	}
}

void chest_fulminictus(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(564));

	/* the first hero gets wounded with 4W6+5 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(4, 6, 5));
}


/**
 * \brief   loot a chest, with standard text messages
 *
 * \param   chest       pointer to the chest
 */
void loot_simple_chest(Bit8u *chest)
{
	signed short item_no;
	signed short item_id;
	signed short tw_bak;
	char names[20][30];

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writews(TEXTBOX_WIDTH, 7);

	host_writeb(chest + 0x2, 0);

	do {

		item_no = 0;

		/* write the names of the items in the chest into names[] */
		while((item_id = host_readb(Real2Host(host_readd(chest + 0x0b)) + item_no)) != (signed short)0x00ff) {


			strcpy(names[item_no++],
				(char*)Real2Host(GUI_name_plural(0, (Bit8u*)get_itemname(item_id))));
		}

		if (item_no == 0) {
			/* this chest is empty */
			GUI_output(get_ttx(522));
			break;
		} else {

			/* show radio menu with item names */
			item_no = GUI_radio(get_ttx(521), (signed char)item_no,
						names[0], names[1], names[2], names[3],
						names[4], names[5], names[6], names[7],
						names[8], names[9], names[10], names[11],
						names[12], names[13], names[14], names[15],
						names[16], names[17], names[18], names[19]) - 1;

			if (item_no != -2) {
				/* if not pressed ESC */
				if (get_item(host_readb(Real2Host(host_readd(chest + 0xb)) + item_no), 1, 1))
				{
					/* got the item in inventory => remove from chest */
					delete_chest_item(chest, item_no);
				} else {
					/* group has not taken the item */
					item_no = -2;
				}
			}
		}

	} while (item_no != -2);

	ds_writews(TEXTBOX_WIDTH, tw_bak);
}

/**
 * \brief   deletes an item from a chest
 *
 * \param   chest       pointer to the chest
 * \param   item_no     the number of the item to be deleted
 */
void delete_chest_item(Bit8u *chest, signed short item_no)
{
	signed char tmp;

	do {
#if defined(__BORLANDC__)
		Real2Host(host_readd(chest + 0xb))[item_no] = tmp = Real2Host(host_readd(chest + 0xb))[item_no + 1];
#else
		host_writeb(Real2Host(host_readd(chest + 0xb)) + item_no,
			tmp = host_readbs(Real2Host(host_readd(chest + 0xb)) + item_no + 1));
#endif
		item_no++;

	} while (tmp != -1);

}

/**
 * \brief   loot a chest, with different text messages
 *
 * \param   chest       pointer to the chest
 * \param   text_non_empty shown text if chest is not empty
 * \param   text_empty  shown text if chest is empty
 */
void loot_chest(Bit8u *chest, Bit8u *text_non_empty, Bit8u *text_empty)
{
	signed short item_no;
	signed short item_id;
	signed short tw_bak;
	char names[20][20];

	host_writeb(chest + 0x2, 0);

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writews(TEXTBOX_WIDTH, 7);

	do {

		item_no = 0;

		/* write the names of the items in the chest into names[] */
		while((item_id = host_readb(Real2Host(host_readd(chest + 0x0b)) + item_no)) != (signed short)0x00ff) {


			strcpy(names[item_no++],
				(char*)Real2Host(GUI_name_plural(0, (Bit8u*)get_itemname(item_id))));
		}

		if (item_no == 0) {
			/* this chest is empty */
			GUI_output(text_empty);
			break;
		} else {

			/* show radio menu with item names */
			item_no = GUI_radio(text_non_empty, (signed char)item_no,
						names[0], names[1], names[2], names[3],
						names[4], names[5], names[6], names[7],
						names[8], names[9], names[10], names[11],
						names[12], names[13], names[14], names[15],
						names[16], names[17], names[18], names[19]) - 1;

			if (item_no != -2) {
				/* if not pressed ESC */
				if (get_item(host_readb(Real2Host(host_readd(chest + 0xb)) + item_no), 1, 1))
				{
					/* got the item in inventory => remove from chest */
					delete_chest_item(chest, item_no);
				} else {
					/* group has not taken the item */
					item_no = -2;
				}
			}
		}

	} while (item_no != -2);

	ds_writews(TEXTBOX_WIDTH, tw_bak);
}

/**
 * \brief   check if a hero has lockpicks
 *
 * \param   hero        pointer to the hero
 * \return              -1 = no lockpicks, -2 = all lockpicks are broken, else position of the lockpicks
 */
signed short hero_has_lockpicks(Bit8u *hero)
{
	signed short retval = -1;
	signed short i;

	/* in each inventory slot... */
	for (i = 0; i < NR_HERO_INVENTORY_SLOTS; i++) {

		/* ... check for lockpicks ... */
		if (host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * i) == ITEM_PICKLOCKS) {

			/* ... which are not broken */
			if (!inventory_broken(hero + HERO_INVENTORY + SIZEOF_INVENTORY * i)) {
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

/* handle special chest */
void seg092_06b4(signed short a1)
{
	signed short x;
	signed short y;
	signed short chest_id;
	signed short l4;
	RealPt chest_ptr;
	Bit8u *ptr;

	chest_ptr = (RealPt)ds_readd(DNG_SPECIALCHEST_INDEX + 4 * ds_readbs(DUNGEON_INDEX));
	ptr = p_datseg + DNG_MAP;
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
				(char*)get_ttx(793),
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));
			GUI_output(Real2Host(ds_readd(DTP2)));

			set_party_money(get_party_money() + host_readw(Real2Host(chest_ptr) + 17));
		}

		if (host_readws(Real2Host(chest_ptr) + 19) != 0) {
			/* There are FOOD PACKAGES in the chest */
			get_item(ITEM_FOOD_PACKAGE, 1, host_readws(Real2Host(chest_ptr) + 19));
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

			l_di = test_skill(hero, TA_SCHLOESSER, host_readbs(Real2Host(chest_ptr) + 2));

			if (l_di == -99) {
				/* unlucky, your lockpicks break... */

				print_msg_with_first_hero(get_ttx(533));
				or_ptr_bs(hero + (HERO_INVENTORY + INVENTORY_FLAGS) + SIZEOF_INVENTORY * l_si, 1); /* set 'broken' flag */

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
			print_msg_with_first_hero(get_ttx(531));
		}
	} else {
		print_msg_with_first_hero(get_ttx(530));
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

		if (!inventory_broken(hero + HERO_INVENTORY + SIZEOF_INVENTORY * key_pos))
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
	unsigned short item_cnt;
	signed short item_no;
	signed short i;
	signed short tw_bak;
	char temp_str[10];
	signed short len;
	char names[20][25];

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	do {

		item_no = 0;
		while ((i = host_readb((item_no + item_no) + chest)) != 255) {

			names[item_no][0] = '\0';

			if ((item_cnt = chest[item_no + item_no + 1]) > 1)
			{
				my_itoa(item_cnt, names[item_no], 10);

				strcat(names[item_no], (char*)p_datseg + STR_SINGLE_SPACE);
			}

			strcat(names[item_no++], (char*)Real2Host(GUI_name_plural( ((signed short)(item_cnt > 1 ? (unsigned short)1 : (unsigned short)0)) ? 4 : 0, (Bit8u*)get_itemname(i))));
		}

		if (item_no != 0) {

			item_no = GUI_radio(msg, (signed char)item_no,
				names[0], names[1], names[2], names[3],
				names[4], names[5], names[6], names[7],
				names[8], names[9], names[10], names[11],
				names[12], names[13], names[14], names[15],
				names[16], names[17], names[18], names[19]) - 1;

			if (item_no != -2) {
				item_no += item_no;

				my_itoa(chest[item_no + 1], temp_str, 10);

				len = strlen(temp_str);

				do {
					i = (item_cnt = chest[item_no + 1]) > 1 ? GUI_input(get_ttx(593), len) : item_cnt;

				} while (i < 0);

				if (i > item_cnt) {
					i = item_cnt;
				}

				if (i != 0) {

					if (chest[item_no] == 250) {
						add_party_money(i * 100L);
					} else {
						i = get_item(chest[item_no], 1, i);
					}

					if (i == item_cnt) {

						do {
							chest[item_no] = (unsigned char)(i = chest[item_no + 2]);
							chest[item_no + 1] = chest[item_no + 3];
							item_no += 2;
						} while (i != 255);

					} else if (i != 0) {
						chest[item_no + 1] -= i;
					} else {
						item_no = -2;
					}
				}
			}


		} else {
			item_no = -2;
		}

	} while (item_no != -2);

	ds_writew(TEXTBOX_WIDTH, tw_bak);
}

#if !defined(__BORLANDC__)
}
#endif
