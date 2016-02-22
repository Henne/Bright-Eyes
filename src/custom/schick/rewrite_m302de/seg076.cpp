/**
 *	Rewrite of DSA1 v3.02_de functions of seg076 (dungeon: common)
 *	Functions rewritten: 1/11
 */

#include <stdio.h>

#include "v302de.h"

#include "seg002.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void print_msg_with_first_hero(Bit8u *msg)
{
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());

	sprintf((char*)Real2Host(ds_readd(0xd2eb)),
		(char*)msg, (char*)hero + HERO_NAME2);

	GUI_input(Real2Host(ds_readd(0xd2eb)), 0);
	return;
}

void do_dungeon(void)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif
