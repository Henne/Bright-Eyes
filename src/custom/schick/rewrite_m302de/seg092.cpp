/**
 *	Rewrite of DSA1 v3.02_de functions of seg092 (treasures)
 *	Functions rewritten: 3/22
 */

#include "v302de.h"

#include "seg002.h"
#include "seg007.h"
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

#if !defined(__BORLANDC__)
}
#endif
