/*
	Rewrite of DSA1 v3.02_de functions of seg030 (???)
	Functions rewritten: 5/6
*/

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg028.h"
#include "seg030.h"
#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Borlandified and identical */
RealPt seg030_0000(signed short arg0)
{
	signed short i;
	signed short counter;

	for (i = counter = 0; i < 15; i++) {
		if ((ds_readb(INFORMER_JURGE + i) != 0)
			&& (arg0 - 1 != i)
			&& (i != 9)
			&& (i != 11)
			&& (i != 12)) {

			counter++;
		}
	}

	if (counter == 0) {
		return seg030_008d(arg0);
	} else {

		do {
			i = random_schick(15);
		} while (i == 10 || i == 12 || i == 13 || !ds_readb(0x3363 + i)|| i == arg0);

		return (RealPt)host_readd(Real2Host((RealPt)ds_readd(TEXT_LTX) + (0x291 + i) * 4));
	}
}

/* Borlandified and identical */
RealPt seg030_008d(signed short arg0)
{
	signed short i;
	signed short v2;

	for (i = v2 = 0; i < 15; i++) {
		if (!(ds_readb(INFORMER_JURGE + i))
			&& (arg0 - 1 != i)
			&& (i != 9)
			&& (i != 11)
			&& (i != 12)) {

			v2++;
		}
	}

	if (v2 == 0) {
		return seg030_0000(arg0);
	} else {

		do {
			i = random_schick(15);
		} while (i == 10 || i == 12 || i == 13 || ds_readb(0x3363 + i)|| i == arg0);
		ds_writeb(0x3363 + i, 1);

		return (RealPt)host_readd(Real2Host((RealPt)ds_readd(TEXT_LTX) + (0x291 + i) * 4));
	}
}

/* 0x11e */
/* unused in the game */
/* Borlandified and identical */
void print_date(void)
{
	unsigned short bak;

	prepare_date_str();
	bak = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);

	GUI_input(Real2Host(ds_readd(DTP2)), 0);
	ds_writew(TEXTBOX_WIDTH, bak);
}

/* 0x14f */
/* Borlandified and identical */
void prepare_date_str(void)
{
	unsigned short unused;
	signed short hour;

	unused = 0;

	hour = (signed short)(ds_readd(DAY_TIMER) / 0x1518);

	if (ds_readbs(DAY_OF_MONTH) < 0) {
		/* Days of the nameless */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0x61c),
			get_ltx((0x15d + ds_readbs(DAY_OF_WEEK)) * 4),
			ds_readbs(YEAR), hour);
	} else {
		/* Normal day */
		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_ltx(0x590),
			get_ltx((0x15d + ds_readbs(DAY_OF_WEEK)) * 4),
			ds_readbs(DAY_OF_MONTH),
			get_ltx((0x15 + ds_readbs(MONTH)) * 4),
			get_ltx((0x227 + get_current_season()) * 4),
			ds_readbs(YEAR), hour);
	}

	if (ds_readbs(SPECIAL_DAY) != 0) {
		sprintf((char*)Real2Host(ds_readd(0xd2eb)),
			(char*)get_ltx(0x594),
			get_ltx((0x165 + ds_readbs(SPECIAL_DAY)) * 4));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)Real2Host(ds_readd(0xd2eb)));
	}
}

struct dummy3 {
	signed short a, b;
};

/* Borlandified and identical */
void do_talk(signed short talk_id, signed short tlk_informer)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else
	signed short l_si;
	signed short l_di;
	signed short answer;
	signed short options;
	Bit8u *ptr1;
	Bit8u *ptr2;
	RealPt ptr3;
	char *dst;
	char *fmt;
	Bit8u *hero;
	signed short rand3_1;
	signed short rand3_2;
	signed short rand5;
	signed short l5;
	signed short l6;
	signed short l7;

	struct dummy3 array[3];

	answer = 0;

	ds_writews(DIALOG_INFORMER, tlk_informer);
	ds_writews(TLK_ID, talk_id);

	load_informer_tlk(talk_id + 156);

	ds_writews(DIALOG_STATE, ds_writews(DIALOG_DONE, 0));

	ptr3 = (RealPt)RealMake(datseg, 0x3618);
	ptr2 = Real2Host(host_readd(Real2Host(ptr3) + 38 * tlk_informer));
	l_di = host_readws(Real2Host(ptr3) + 38 * tlk_informer + 4);
	ds_writed(0xe308, (Bit32u)(tlk_informer * 38 + ptr3 + 6));

	load_in_head(host_readws(Real2Host(ptr3) + 38 * tlk_informer + 0x24));
	dst = (char*)Real2Host(ds_readd(DTP2)) + 0x400;

	do {
		answer = options = 0;
		ptr1 = 8 * ds_readws(DIALOG_STATE) + ptr2;

		if (host_readbs(ptr1 + 2) != 0) options++;
		if (host_readbs(ptr1 + 3) != 0) options++;
		if (host_readbs(ptr1 + 4) != 0) options++;

		if (host_readws(ptr1) != -1) {

			l_si = host_readws(ptr1) & 0x7fff;

			fmt = (char*)get_city(4 * (l_si + l_di));

			if (ds_readws(TLK_ID) == 11) {
				if (ds_readws(DIALOG_INFORMER) == 2) {

					if (l_si == 12 || l_si == 16 || l_si == 17 ||
						l_si == 18 || l_si == 20 || l_si == 21 ||
						l_si == 22 || l_si == 25 || l_si == 26 ||
						l_si == 27 || l_si == 28)
					{
						sprintf(dst, fmt,
							Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10,
							Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 0)),
							Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 1)));

					} else if (l_si == 19) {

						sprintf(dst, fmt,
							Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10,
							Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10);

					} else if (l_si == 23) {

						sprintf(dst, fmt,
							Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10,
							Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 2)))
;
					} else if (l_si == 29) {

						sprintf(dst, fmt,
							Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10,
							Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 0)),
							Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 1)),
							Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 2)));

					} else if (l_si == 30) {

						sprintf(dst, fmt,
							Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10,
							Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 1)),
							Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 2)));

					} else if (l_si == 31) {

						sprintf(dst, fmt,
							Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 3)));

					} else if (l_si == 32) {

						sprintf(dst, fmt,
							Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10,
							Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 0)),
							Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10);

					} else {
						strcpy(dst, fmt);
					}
				} else {
					strcpy(dst, fmt);
				}
			} else {

				if (ds_readws(TLK_ID) == 9 && ds_readws(DIALOG_INFORMER) == 1) {

					if (l_si == 21) {

						sprintf(dst, fmt, (char*)get_ltx(0xa48));

					} else if (l_si == 22) {

						sprintf(dst, fmt, (char*)get_ltx(0xa58));

					} else if (l_si == 23) {

						sprintf(dst, fmt, (char*)get_ltx(0xa54));

					} else if (l_si == 29) {

						hero = get_hero(ds_readb(0x3468));

						sprintf(dst, fmt,
							(char*)hero + 0x10,
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)));

					} else {

						strcpy(dst, fmt);

					}
				} else if (ds_readws(TLK_ID) == 7 && ds_readws(DIALOG_INFORMER) == 2) {

					if (l_si == 19) {

						sprintf(dst, fmt, Real2Host(seg030_0000(5)));

					} else if (l_si == 20) {

						sprintf(dst, fmt, Real2Host(seg030_008d(5)));

					} else {

						strcpy(dst, fmt);

					}

				} else if (ds_readws(TLK_ID) == 6 && ds_readws(DIALOG_INFORMER) == 0) {

					if (l_si == 35 || l_si == 36) {

						sprintf(dst, fmt, Real2Host(seg030_008d(1)));

					} else if (l_si == 34) {

						sprintf(dst, fmt, Real2Host(seg030_0000(1)));

					} else {

						strcpy(dst, fmt);
					}

				} else if (ds_readws(TLK_ID) == 12) {

					if (ds_readws(DIALOG_INFORMER) == 0) {

						hero = Real2Host(get_first_hero_available_in_group());

						if (!l_si || l_si == 18) {

							sprintf(dst, fmt, (char*)hero + 0x10);

						} else if (l_si == 28) {

							sprintf(dst, fmt, (char*)get_ltx(0xa5c));

						} else if (l_si == 29) {

							sprintf(dst, fmt, (char*)get_ltx(0xa60));

						} else if (l_si == 30) {

							sprintf(dst, fmt, (char*)get_ltx(0xa80));

						} else if (l_si == 38) {

							sprintf(dst, fmt, (char*)get_ltx(4 * (235 + ds_readb(SWAFNILD_TP4))));

						} else if (l_si == 49) {

							sprintf(dst, fmt, (char*)get_ltx(4 * (235 + ds_readb(SWAFNILD_TP4))));

							ds_writebs(CURRENT_TOWN, ds_readbs(SWAFNILD_TP4));

							ds_writews(0x2d83, ds_readbs(SWAFNILD_TP4) == 35 ? 10 : (ds_readbs(SWAFNILD_TP4) == 32 ? 2 : 7));
							ds_writews(0x2d85, ds_readbs(SWAFNILD_TP4) == 35 ? 2 : (ds_readbs(SWAFNILD_TP4) == 32 ? 14 : 3));

						} else if (l_si == 52) {

							sprintf(dst, fmt,
								(char*)get_ltx(4 * (235 + ds_readb(SWAFNILD_TP1))),
								(char*)get_ltx(4 * (235 + ds_readb(SWAFNILD_TP2))),
								(char*)get_ltx(4 * (235 + ds_readb(SWAFNILD_TP3))));

						} else if (l_si == 59) {

							sprintf(dst, fmt, (char*)get_ltx(4 * (235 + (ds_readb(0x3461) == 1 ? ds_readb(SWAFNILD_TP1) : (ds_readb(0x3461) == 2 ? ds_readb(SWAFNILD_TP2) : ds_readb(SWAFNILD_TP3))))));

						} else {
							strcpy(dst, fmt);
						}

					} else if (ds_readws(DIALOG_INFORMER) == 1) {

						if (!l_si || l_si == 3 || l_si == 4) {

							sprintf(dst, fmt,
								Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10,
								Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 0)));

						} else if (l_si == 5) {

							sprintf(dst, fmt,
								Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 0)),
								Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10);

						} else if (l_si == 6) {

							sprintf(dst, fmt,
								Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10,
								Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x22), 1)),
								Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10);

						} else if (l_si == 8) {

							sprintf(dst, fmt,
								Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10,
								Real2Host(ds_readd(UNICORN_HERO_PTR)) + 0x10);

						} else {
							strcpy(dst, fmt);
						}

					}
				} else if (ds_readws(TLK_ID) == 10 && ds_readws(DIALOG_INFORMER) == 0) {

					if (l_si == 18) {

						sprintf(dst, fmt, Real2Host(seg030_008d(10)));

					} else if (l_si == 29) {

						sprintf(dst, fmt, Real2Host(seg030_0000(10)));

					} else {

						strcpy(dst, fmt);
					}

				} else if (ds_readws(TLK_ID) == 17) {


					hero = Real2Host(get_first_hero_available_in_group());

					if (!l_si || l_si == 2 || l_si == 25 || l_si == 31 ||
						l_si == 32 || l_si == 60 || l_si == 78 || l_si == 87)
					{
						sprintf(dst, fmt, (char*)hero + 0x10);

					} else if (l_si == 13 || l_si == 19 || l_si == 88 || l_si == 24) {

						sprintf(dst, fmt,
							(char*)hero + 0x10,
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)),
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)));

					} else if (l_si == 14 || l_si == 15 || l_si == 76) {

						sprintf(dst, fmt,
							(char*)Real2Host(ds_readd(0x3e1c)) + 0x10,
							(char*)Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(0x3e1c)) + 0x22), 0)));

					} else if (l_si == 26 || l_si == 65) {

						sprintf(dst, fmt,
							(char*)hero + 0x10,
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 0)),
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 2)),
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 1)));

					} else {

						strcpy(dst, fmt);

					}

				} else if (ds_readws(TLK_ID) == 0) {

					if (l_si == 40 || l_si == 41 || l_si == 43) {

						sprintf(dst, fmt,
							(char*)Real2Host(ds_readd(RANDOM_TLK_HERO)) + 0x10,
							(char*)Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(RANDOM_TLK_HERO)) + 0x22), 0)),
							(char*)Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(RANDOM_TLK_HERO)) + 0x22), 2)));
					} else {

						strcpy(dst, fmt);
					}
				} else {
					strcpy(dst, fmt);
				}
			}

			array[0].a = host_readb(ptr1 + 2) + l_di;
			array[0].b = host_readb(ptr1 + 5);
			array[1].a = host_readb(ptr1 + 3) + l_di;
			array[1].b = host_readb(ptr1 + 6);
			array[2].a = host_readb(ptr1 + 4) + l_di;
			array[2].b = host_readb(ptr1 + 7);
			rand5 = random_schick(5);

			for (l5 = 0; l5 < rand5; l5++) {

				rand3_1 = random_schick(3) - 1;
				rand3_2 = random_schick(3) - 1;

				l6 = array[rand3_1].a;
				l7 = array[rand3_2].a;

				if (l6 != l_di && l7 != l_di) {

					array[rand3_1].a = l7;
					array[rand3_2].a = l6;

					l6 = array[rand3_1].b;

					array[rand3_1].b = array[rand3_2].b;

					array[rand3_2].b = l6;
				}
			}

			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), Real2Host(ds_readd(0xe308)), dst, options,
					get_city(4 * (host_readb(ptr1 + 2) + l_di)),
					get_city(4 * (host_readb(ptr1 + 3) + l_di)),
					get_city(4 * (host_readb(ptr1 + 4) + l_di)));
		}


		ds_writews(0xe30e, -1);

		if (host_readws(ptr1) & 0x8000 || host_readws(ptr1) == -1) {
			seg030_0fd5();
		}

		ds_writew(DIALOG_STATE, ds_readws(0xe30e) == -1 ? host_readb(ptr1 + 5) : ds_readws(0xe30e));

		if (ds_readws(DIALOG_DONE) == 0) {

			/* set the new dialog state */

			if (options != 0 ) {
				if (answer == -1) {
					ds_writew(DIALOG_DONE, 1);
				} else if (answer == 1) {
					ds_writews(DIALOG_STATE, host_readb(ptr1 + 5));
				} else if (answer == 2) {
					ds_writews(DIALOG_STATE, host_readb(ptr1 + 6));
				} else if (answer == 3) {
					ds_writews(DIALOG_STATE, host_readb(ptr1 + 7));
				}
			}

			if (ds_readws(DIALOG_STATE) == 255) {
				ds_writew(DIALOG_DONE, 1);
			}
		}

	} while (ds_readws(DIALOG_DONE) == 0);

	ds_writews(0x26bd, -1);
	ds_writews(CURRENT_ANI, -1);
#endif
}

void seg030_0fd5(void)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif
