/**
 *	Rewrite of DSA1 v3.02_de functions of seg076 (dungeon: common)
 *	Functions rewritten: 2/11
 */

#include <stdio.h>

#include "v302de.h"

/* we need an own prototype of div16 */
#define DONT_DEF_DIV16

#include "seg002.h"
#include "seg047.h"
#include "seg075.h"
#include "seg076.h"
#include "seg092.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"

#if defined(__BORLANDC__)
signed short div16(signed char);
#endif

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct dummy5 {
	unsigned char a[5];
};

/**
 * \brief	opening doors logic
 * \param	action	how to try to open the door (smash / lockpicks / magic)
 */
/* Borlandified and identical */
void DNG_door(signed short action)
{
	signed short x;
	signed short y;
	signed short pos;
	signed short l4;
	signed short spell_result;
	struct dummy5 *ptr;
	Bit8u *hero;
	signed short hero_pos;
	signed short lockpick_pos;
	signed short lockpick_result;

	ptr = (struct dummy5*)Real2Host(ds_readd(0xe49c));
	x = ds_readws(X_TARGET);
	y = ds_readws(Y_TARGET);

	switch (ds_readbs(DIRECTION))
	{
		case 0: y--; break;
		case 1:	x++; break;
		case 2: y++; break;
		case 3: x--; break;
	}

	pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * x + y;

	do {

		if (host_readws((Bit8u*)ptr + 0) == pos)
		{

			if (action == 135)
			{
				/* smash door */
				if (ds_readws(0xd011) != 5)
				{

					if (ds_readb(DUNGEON_INDEX) == 15 && pos == 0x1903 && ds_readb(0x41c7) != 0)
					{
						GUI_output(get_dtp(0x48));

					} else if (!(ds_readb(DUNGEON_INDEX) == 11 && pos == 0xc08 && ds_readb(0x4168) != 4) &&
							!(ds_readb(DUNGEON_INDEX) == 6 && pos == 0xb01 && ds_readb(0x3ca7)) &&
							!(ds_readb(DUNGEON_INDEX) == 12 && ds_readb(0x3fa1)))
					{
						/* for this door works an IRON KEY */
						/* BEWARE: there are two of them */
						if (ds_readb(DUNGEON_INDEX) == 11 && pos == 0xc08 && get_first_hero_with_item(209) != -1)
						{
							or_ptr_bs(Real2Host(ds_readd(0xe488)) + (y << 4) + x, 0x02);
						}

						if (div16(host_readb(Real2Host(ds_readd(0xe488)) + (y << 4) + x)) == 1)
						{
							/* the door is closed -> open it */
							l4 = host_readb(Real2Host(ds_readd(0xe488)) + (y << 4) + x) & 0x02;

							if (l4 != 0 || !host_readbs((Bit8u*)ptr + 2))
							{
								and_ptr_bs(Real2Host(ds_readd(0xe488)) + (y << 4) + x, 0x0f);
								or_ptr_bs(Real2Host(ds_readd(0xe488)) + (y << 4) + x, 0x20);
								ds_writeb(0xbd4d, host_readb(Real2Host(ds_readd(0xe488)) + (y << 4) + x));
								DNG_open_door();

								ds_writebs((0xbd38 + 6), ds_writebs((0xbd38 + 7), ds_writebs((0xbd38 + 8), -1)));
								ds_writew(0xd013, 1);
								ds_writew(0xd011, 3);
							} else {
								ds_writebs((0xbd38 + 6), 0x22);
								ds_writebs((0xbd38 + 7), 0x23);
								ds_writebs((0xbd38 + 8), 0x0b);
								ds_writew(0xd013, 1);
								ds_writew(0xd011, 5);
							}

						} else if (div16(host_readbs(Real2Host(ds_readd(0xe488)) + (y << 4) + x)) == 2)
						{

							/* the door is open -> close it */
							DNG_close_door();

							and_ptr_bs(Real2Host(ds_readd(0xe488)) + (y << 4) + x, 0x0f);
							or_ptr_bs(Real2Host(ds_readd(0xe488)) + (y << 4) + x, 0x12);

							ds_writeb(0xbd4d, host_readb(Real2Host(ds_readd(0xe488)) + (y << 4) + x));
							ds_writew(0xd011, 1);
						}
					}
				} else {
					play_voc(ARCHIVE_FILE_FX14_VOC);

					if (check_heros_KK(host_readbs((Bit8u*)ptr + 2)))
					{
						and_ptr_bs(Real2Host(ds_readd(0xe488)) + (y << 4) + x, 0x0f);
						or_ptr_bs(Real2Host(ds_readd(0xe488)) + (y << 4) + x, 0x90);

						ds_writeb(0xbd4d, host_readb(Real2Host(ds_readd(0xe488)) + (y << 4) + x));
						ds_writew(0xe482, -1);
					}

					sub_hero_le(Real2Host(get_first_hero_available_in_group()), 1);

					if (get_second_hero_available_in_group())
					{
						sub_hero_le(Real2Host(get_second_hero_available_in_group()), 1);
					}
				}
			}

			if (action == 136)
			{
				/* use lockpicks */

				hero = Real2Host(get_first_hero_available_in_group());

				if ((lockpick_pos = hero_has_lockpicks(hero)) != -1)
				{
					if (lockpick_pos != -2)
					{
						lockpick_result = test_skill(hero, 48, host_readbs((Bit8u*)ptr + 3));

						play_voc(ARCHIVE_FILE_FX11_VOC);

						inc_ds_ws(LOCKPICK_TRY_COUNTER);

						if (lockpick_result == -99 || ds_readws(LOCKPICK_TRY_COUNTER) > 3)
						{
							/* LOCKPICK breaks if the test was unlucky,
								or when tried three times without moving */
							print_msg_with_first_hero(get_ltx(0x854));

							or_ptr_bs(hero + 14 * lockpick_pos + (HERO_ITEM_HEAD + 4), 1);

							ds_writew(LOCKPICK_TRY_COUNTER, 0);

						} else if (lockpick_result <= 0) {

							/* just failed */
							print_msg_with_first_hero(get_ltx(0x850));

						} else {
							/* success => the door opens */
							and_ptr_bs(Real2Host(ds_readd(0xe488)) + (y << 4) + x, 0x0f);
							or_ptr_bs(Real2Host(ds_readd(0xe488)) + (y << 4) + x, 0x20);
							ds_writeb(0xbd4d, host_readb(Real2Host(ds_readd(0xe488)) + (y << 4) + x));
							DNG_open_door();

							add_hero_ap(hero, 1L);

							ds_writebs((0xbd38 + 6), ds_writebs((0xbd38 + 7), ds_writebs((0xbd38 + 8), -1)));
							ds_writew(0xd013, 1);
						}

					} else {
						/* all LOCKPICKS are broken */
						print_msg_with_first_hero(get_ltx(0x84c));
					}
				} else {
					/* the leader has no LOCKPICKS */
					print_msg_with_first_hero(get_ltx(0x848));
				}
			}

			if (action == 137)
			{
				/* use magic */
				hero_pos = select_hero_ok(get_ltx(0x4f4));

				if (hero_pos != -1)
				{
					hero = get_hero(hero_pos);

					if (host_readbs(hero + HERO_TYPE) < 7)
					{
						GUI_output(get_ltx(0x528));
					} else {
						spell_result = test_spell(hero, 28, host_readbs((Bit8u*)ptr + 4));

						if (spell_result == -99)
						{
							/* unlucky => just print a message */
							sprintf((char*)Real2Host(ds_readd(DTP2)),
								(char*)get_ltx(0x97c),
								(char*)hero + HERO_NAME2);

							GUI_output(Real2Host(ds_readd(DTP2)));
						} else if (spell_result <= 0)
						{
							/* failed => half AE costs */
							sub_ae_splash(hero, get_spell_cost(28, 1));

							play_voc(ARCHIVE_FILE_FX17_VOC);
						} else {
							play_voc(ARCHIVE_FILE_FX17_VOC);

							sub_ae_splash(hero, get_spell_cost(28, 0));

							/* success => the door opens */
							and_ptr_bs(Real2Host(ds_readd(0xe488)) + (y << 4) + x, 0x0f);
							or_ptr_bs(Real2Host(ds_readd(0xe488)) + (y << 4) + x, 0x20);
							ds_writeb(0xbd4d, host_readb(Real2Host(ds_readd(0xe488)) + (y << 4) + x));
							DNG_open_door();

							add_hero_ap(hero, 1L);

							ds_writebs((0xbd38 + 6), ds_writebs((0xbd38 + 7), ds_writebs((0xbd38 + 8), -1)));
							ds_writew(0xd013, 1);
						}
					}
				}
			}
		}

	} while (host_readws((Bit8u*)(ptr++)) != -1);
}

void print_msg_with_first_hero(Bit8u *msg)
{
	Bit8u *hero;

	hero = Real2Host(get_first_hero_available_in_group());

	sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
		(char*)msg, (char*)hero + HERO_NAME2);

	GUI_input(Real2Host(ds_readd(TEXT_OUTPUT_BUF)), 0);
	return;
}

void do_dungeon(void)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif
