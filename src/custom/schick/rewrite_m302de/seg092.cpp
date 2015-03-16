/**
 *	Rewrite of DSA1 v3.02_de functions of seg092 (treasures)
 *	Functions rewritten: 15/22
 */

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
#include "seg047.h"
#include "seg076.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
void chest_protected_normal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x818));

	/* the first hero gets wounded with 1W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), random_schick(6));
}

/* Borlandified and identical */
void chest_closed(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x81c));
}

/* Borlandified and identical */
void chest_protected_heavy(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x820));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(2, 6, 0));
}

/* Borlandified and identical */
void chest_poisoned1(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x820));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host((RealPt)(ds_writed(0x3e20, (Bit32u)get_first_hero_available_in_group()))), dice_roll(2, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(Real2Host(ds_readd(0x3e20)), 1);
}

/* Borlandified and identical */
void chest_poisoned2(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x820));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host((RealPt)(ds_writed(0x3e20, (Bit32u)get_first_hero_available_in_group()))), dice_roll(2, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(Real2Host(ds_readd(0x3e20)), 2);
}

/* Borlandified and identical */
void chest_poisoned3(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x820));

	/* the first hero gets wounded with 1W6 */
	sub_hero_le(Real2Host((RealPt)(ds_writed(0x3e20, (Bit32u)get_first_hero_available_in_group()))), dice_roll(1, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(Real2Host(ds_readd(0x3e20)), 8);
}

/* Borlandified and identical */
void chest_protected_brutal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x820));

	/* the first hero gets wounded with 4W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(4, 6, 0));
}

/* Borlandified and identical */
void chest_stoned(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0xc20));

	/* save pointer of the first hero */
	ds_writed(0x3e20, (Bit32u)get_first_hero_available_in_group());

	/* and make him stoned */
	or_ptr_bs(Real2Host(ds_readd(0x3e20)) + 0xaa, 0x04);
}

/* Borlandified and identical */
void chest_ignifax_normal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0xc24));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(2, 6, 0));
}

/* Borlandified and identical */
void chest_ignifax_brutal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0xc24));

	/* the first hero gets wounded with 4W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(4, 6, 0));
}

/* Borlandified and identical */
void chest_ignifax_heavy(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0xc24));

	/* the first hero gets wounded with 3W6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(3, 6, 0));
}

/* Borlandified and identical */
void chest_crossbow_bolts(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0xc50));

	/* the first hero gets wounded with 2W6+6 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(2, 6, 6));
}

/* Borlandified and identical */
void chest_cursed(void)
{
	signed short i;
	Bit8u *hero;

	/* get the group leader */
	hero = Real2Host(get_first_hero_available_in_group());

	if (!hero_transformed(hero)) {

		/* set transformed flag */
		or_ptr_bs(hero + 0xab, 0x40);

		/* decrement each good attribute */
		for (i = 0; i <= 6; i++) {
			dec_ptr_bs(hero + 0x35 + 3 * i);
		}

		/* print a message */
		print_msg_with_first_hero(get_ltx(0x8cc));
	}
}

/* Borlandified and identical */
void chest_fulminictus(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ltx(0x8d0));

	/* the first hero gets wounded with 4W6+5 */
	sub_hero_le(Real2Host(get_first_hero_available_in_group()), dice_roll(4, 6, 5));
}

/**
 * \brief deletes an item from a chest
 * \param chest		pointer to the chest
 * \param item_nr	the number of the item to be deleted
 */
/* Borlandified and identical */
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

#if !defined(__BORLANDC__)
}
#endif
