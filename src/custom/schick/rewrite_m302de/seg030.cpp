/*
 *	Rewrite of DSA1 v3.02_de functions of seg030 (tlk_logic, date_printer)
 *	Functions rewritten: 6/6 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg030.cpp
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
#include "seg032.h"
#include "seg054.h"
#include "seg055.h"
#include "seg058.h"
#include "seg066.h"
#include "seg072.h"
#include "seg096.h"
#include "seg097.h"
#include "seg116.h"
#include "seg117.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

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

/* unused in the game */
void print_date(void)
{
	unsigned short bak;

	prepare_date_str();
	bak = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);

	GUI_input(Real2Host(ds_readd(DTP2)), 0);
	ds_writew(TEXTBOX_WIDTH, bak);
}

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
		sprintf((char*)Real2Host(ds_readd(BUFFER4_PTR)),
			(char*)get_ltx(0x594),
			get_ltx((0x165 + ds_readbs(SPECIAL_DAY)) * 4));

		strcat((char*)Real2Host(ds_readd(DTP2)),
			(char*)Real2Host(ds_readd(BUFFER4_PTR)));
	}
}

struct dummy3 {
	signed short a, b;
};

void do_talk(signed short talk_id, signed short tlk_informer)
{
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
							(char*)hero + HERO_NAME2,
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

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

							sprintf(dst, fmt, (char*)hero + HERO_NAME2);

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
						sprintf(dst, fmt, (char*)hero + HERO_NAME2);

					} else if (l_si == 13 || l_si == 19 || l_si == 88 || l_si == 24) {

						sprintf(dst, fmt,
							(char*)hero + HERO_NAME2,
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

					} else if (l_si == 14 || l_si == 15 || l_si == 76) {

						sprintf(dst, fmt,
							(char*)Real2Host(ds_readd(RUIN_HERO)) + 0x10,
							(char*)Real2Host(GUI_get_ptr(host_readbs(Real2Host(ds_readd(RUIN_HERO)) + 0x22), 0)));

					} else if (l_si == 26 || l_si == 65) {

						sprintf(dst, fmt,
							(char*)hero + HERO_NAME2,
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)),
							(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 1)));

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

			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), Real2Host(ds_readd(0xe308)), (Bit8u*)dst, options,
					get_city(4 * (host_readb(ptr1 + 2) + l_di)),
					get_city(4 * (host_readb(ptr1 + 3) + l_di)),
					get_city(4 * (host_readb(ptr1 + 4) + l_di)));
		}


		ds_writews(0xe30e, -1);

		if (host_readws(ptr1) & 0x8000 || host_readws(ptr1) == -1) {
			talk_switch();
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

	ds_writews(TEXT_FILE_INDEX, -1);
	ds_writews(CURRENT_ANI, -1);
}

/* Borlandified and identical */
void talk_switch(void)
{
	signed short state;

	state = ds_readws(DIALOG_STATE);

	if (ds_readws(TLK_ID) == 3) {
		/* DASPOTA1.TLK */

		if (ds_readws(DIALOG_INFORMER) == 0) {
			if ((state == 1 || state == 2 || state == 3) && !ds_readb(0x3320)) {

				ds_writew(FIG_DISCARD, 1);

				if (!do_fight(FIGHTS_DASP1A)) {
					if (GUI_bool(get_dtp(0x58))) {

						ds_writew(FIG_DISCARD, 0);

						if (!do_fight(FIGHTS_DASP1B)) {
							ds_writeb(0x3320, 1);
						}
					}
				}

				ds_writew(FIG_DISCARD, 0);
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 1) {

			if (state == 1 && !ds_readb(0x3321)) {

				if (!do_fight(FIGHTS_DASP2)) {
					ds_writeb(0x3321, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 2) {

			if ((state == 4 || state == 6) && !ds_readb(0x3322)) {

				if (!do_fight(FIGHTS_DASP3)) {
					ds_writeb(0x3322, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 3) {

			if (state == 4 && !ds_readb(0x3323)) {

				if (!do_fight(FIGHTS_DASP4)) {
					ds_writeb(0x3323, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 4) {

			if ((state == 1 || state == 4 || state == 7) && !ds_readb(0x3324)) {

				if (!do_fight(FIGHTS_DASP5)) {
					ds_writeb(0x3324, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 5) {

			if ((state == 4 || state == 5) && !ds_readb(0x3325)) {

				if (!do_fight(FIGHTS_DASP6A)) {
					ds_writeb(0x3325, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 6) {

			if ((state == 1 || state == 4 || state == 7) && !ds_readb(0x3326)) {

				if (!do_fight(FIGHTS_DASP7)) {
					ds_writeb(0x3326, 1);
				}
			}
		}

	} else if (ds_readws(TLK_ID) == 4) {
		/* DASPOTA2.TLK */

		if (ds_readws(DIALOG_INFORMER) == 0) {

			if (state == 4 && !ds_readb(0x3327)) {

				if (!do_fight(FIGHTS_DASP8)) {
					ds_writeb(0x3327, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 1) {

			if (state == 1 && !ds_readb(0x3328)) {

				if (!do_fight(FIGHTS_DASP9)) {
					ds_writeb(0x3328, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 2) {

			if (state == 1 && !ds_readb(0x3329)) {

				if (!do_fight(FIGHTS_DASP10)) {
					ds_writeb(0x3329, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 3) {

			if (state == 4 && !ds_readb(0x332a)) {

				if (!do_fight(FIGHTS_DASP11)) {
					ds_writeb(0x332a, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 4) {

			if (state == 1 && !ds_readb(0x332b)) {

				if (!do_fight(FIGHTS_DASP12A)) {
					ds_writeb(0x332b, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 5) {

			if (state == 5 && !ds_readb(0x332c)) {

				if (!do_fight(FIGHTS_DASP13)) {
					ds_writeb(0x332c, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 6) {

			if (state == 1 && !ds_readb(0x332d)) {

				if (!do_fight(FIGHTS_DASP14)) {
					ds_writeb(0x332d, 1);
				}
			}
		}

	} else if (ds_readws(TLK_ID) == 5) {
		/* DASPOTA3.TLK */

		if (ds_readws(DIALOG_INFORMER) == 0) {

			if ((state == 4 || state == 6) && !ds_readb(0x332f)) {

				if (!do_fight(FIGHTS_DASP16)) {
					ds_writeb(0x332f, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 1) {

			if ((state == 4 || state == 5 || state == 6) && !ds_readb(0x3330)) {

				if (!do_fight(FIGHTS_DASP17)) {
					ds_writeb(0x3330, 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 2) {

			if ((state == 6 || state == 7) && !ds_readb(0x3331)) {

				if (!do_fight(FIGHTS_DASP18)) {
					ds_writeb(0x3331, 1);
				}
			}
		}

	} else if (ds_readws(TLK_ID) == 6) {
		/* INFO1.TLK */
		INF_jurge_hjore(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 7) {
		/* INFO2.TLK */
		INF_yasma_umbrik_isleif(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 8) {
		/* INFO3.TLK */
		INF_ragna_beorn_algrid(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 9) {
		/* INFO4.TLK */
		INF_eliane_tiomar(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 10) {
		/* INFO5.TLK */
		INF_olvir_asgrimm(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 11) {
		/* INFO6.TLK */
		INF_treborn_unicorn(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 12) {
		/* INFO7.TLK */
		INF_swafnild_unicorn(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 13) {
		/* SCHMIED.TLK */
		TLK_schmied(state);

	} else if (ds_readws(TLK_ID) == 14) {
		/* GHANDEL.TLK */
		TLK_ghandel(state);

	} else if (ds_readws(TLK_ID) == 15) {
		/* KHANDEL.TLK */
		TLK_khandel(state);

	} else if (ds_readws(TLK_ID) == 16) {
		/* WHANDEL.TLK */
		TLK_whandel(state);

	} else if (ds_readws(TLK_ID) == 1) {
		/* HERBERG.TLK */
		TLK_herberg(state);

	} else if (ds_readws(TLK_ID) == 0) {
		/* F092.TLK */
		TLK_old_woman(state);

	} else if (ds_readws(TLK_ID) == 17) {
		/* F115.TLK */
		TLK_way_to_ruin(state);

	} else if (ds_readws(TLK_ID) == 18) {
		/* EREMIT.TLK */
		TLK_eremit(state);
	}
}

#if !defined(__BORLANDC__)
}
#endif
