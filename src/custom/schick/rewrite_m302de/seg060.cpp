/**
 *	Rewrite of DSA1 v3.02_de functions of seg060 (tavern)
 *	Functions rewritten: 1/2
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg031.h"
#include "seg047.h"
#include "seg060.h"
#include "seg073.h"
#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
/* Borlandified and identical */
void talk_tavern(void)
{
	signed short l_si;
	signed short l_di;
	signed short answer;
	signed short options;
	Bit8u *ptr1;
	Bit8u *ptr2;
	char *ptr3;
	char *ptr4;
	Bit8u *ptr5;
	Bit8u *ptr6;
	char *format;
	Bit8u *hero;
	Bit8u *ptr9;
	Bit8u *ptr10;

	answer = 0;

	ptr3 = Real2Host(ds_readd(DTP2));
	ptr4 = (char*)ptr3 + 1000;
	ptr5 = ptr4 + 60;
	ptr6 = ptr5 + 60;
	ds_writews(TEXTBOX_WIDTH, 9);
	ds_writeb(0x3610, ds_writeb(0x3611, ds_writeb(CURRENT_INFORMER, ds_writeb(0x360f, 0))));
	ds_writeb(0x360c, 0);

	set_var_to_zero();
	load_ani(13);
	init_ani(0);
	/* load TAVERN.TLK */
	load_tlk(130);
	ds_writews(DIALOG_STATE, ds_writews(DIALOG_DONE, 0));
	ptr2 = p_datseg + 0x3794;

	do {
		answer = options = 0;

		ptr1 = ptr2 + 8 * ds_readws(DIALOG_STATE);

		if (host_readbs(ptr1 + 2) != 0) {
			options++;
		}
		if (host_readbs(ptr1 + 3) != 0) {
			options++;
		}
		if (host_readbs(ptr1 + 4) != 0) {
			options++;
		}

		if (host_readws(ptr1) != -1) {

			l_si = host_readws(ptr1) & 0x7fff;
			format = get_dtp(4 * l_si);
			hero = Real2Host(get_first_hero_available_in_group());

			if (l_si == 52 || l_si == 72 || l_si == 78 || l_si == 83 || l_si == 89) {

				sprintf(ptr3, format, count_heroes_in_group());

			} else if (l_si == 16 || l_si == 70) {

				sprintf(ptr3, format, (char*)hero + 0x10);

			} else if (l_si == 95) {

				hero = get_hero(ds_readb(0x3611) - 1);

				sprintf(ptr3, format,
					(char*)hero + 0x10,
					Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 3)),
					Real2Host(GUI_get_ptr(host_readbs(hero + 0x22), 1)));
;
			} else if (l_si == 18 || l_si == 31) {

				sprintf(ptr3, format, 20 * ds_readb(0x3610));

			} else if (l_si == 40 || l_si == 46) {

				ptr9 = Real2Host(load_current_town_gossip());

				do {
					ptr10 = Real2Host(load_current_town_gossip());
				} while (ptr9 == ptr10);

				sprintf(ptr3, format, (char*)ptr9, (char*)ptr10);

			} else if (l_si == 66) {

				sprintf(ptr3, format, get_informer_hint());

			} else if (l_si == 105) {

				sprintf(ptr3, format,
					random_interval(3, 6),
					random_schick(2) == 1 ? get_dtp(0x244) : get_dtp(0x248));

			} else if (l_si == 114) {

				l_di = ds_readws(0x6c84 + 4 * ds_readws(TYPEINDEX) + 0);

				/* print quality [-1, 2..20]  2 = best, 20 = worse */
				sprintf(ptr3, format,
					l_di >= 1 && l_di <= 3 ? get_dtp(0x220) : (
					l_di >= 4 && l_di <= 6 ? get_dtp(0x224) : (
					l_di >= 7 && l_di <= 9 ? get_dtp(0x228) : (
					l_di >= 10 && l_di <= 12 ? get_dtp(0x22c) : (
					l_di >= 13 && l_di <= 15 ? get_dtp(0x230) : (
					l_di >= 15 && l_di <= 18 ? get_dtp(0x234) : get_dtp(0x238)))))));

			} else if (l_si == 115) {

				ds_writeb(0x360d, random_schick(2));

				sprintf(ptr3, format,
					ds_readb(0x360d) == 1 ? get_dtp(0x23c) : get_dtp(0x240));

			} else if (l_si == 119) {

				sprintf(ptr3, format, Real2Host(get_random_tavern_message()));

			} else if (l_si == 122 || l_si == 126 || l_si == 127 || l_si == 132) {

				sprintf(ptr3, format,
					Real2Host(GUI_get_ptr(ds_readb(0x360d) - 1, 0)),
					Real2Host(GUI_get_ptr(ds_readb(0x360d) - 1, 0)));

			} else if (l_si == 128) {

				sprintf(ptr3, format,
					!ds_readb(CURRENT_INFORMER) ? Real2Host(get_drinkmate()) : Real2Host(get_informer_name()));
			} else if (l_si == 131) {

				sprintf(ptr3, format,
					Real2Host(GUI_get_ptr(ds_readb(0x360d) - 1, 0)),
					Real2Host(GUI_get_ptr(ds_readb(0x360d) - 1, 0)),
					Real2Host(load_current_town_gossip()));

			} else {

				strcpy(ptr3, format);
			}

			l_si = host_readb(ptr1 + 2);
			format = (char*)get_dtp(4 * l_si);


			if (l_si == 1 || l_si == 19) {
				sprintf(ptr4, format, count_heroes_in_group());
			} else {
				strcpy(ptr4, format);
			}

			l_si = host_readb(ptr1 + 3);
			format = (char*)get_dtp(4 * l_si);

			if (l_si == 13) {

				sprintf(ptr5, format, (char*)hero + 0x10);

			} else if (l_si == 61) {

				sprintf(ptr5, format, count_heroes_in_group());

			} else if (l_si == 39) {

				sprintf(ptr5, format,
					!ds_readb(INFORMER_JURGE + get_town_lookup_entry()) ?
						Real2Host(get_drinkmate()) :
						Real2Host(get_informer_name2()));

			} else {
				strcpy(ptr5, format);
			}

			l_si = host_readb(ptr1 + 4);
			strcpy(ptr6, (char*)get_dtp(4 * l_si));

			do {
				answer = GUI_radio(ptr3, options, ptr4, ptr5, ptr6);
			} while (answer == -1);
		}

		ds_writews(0xe30e, -1);

		if (host_readws(ptr1) & 0x8000 || host_readws(ptr1) == -1) {
			TLK_tavern(answer);
		}

		ds_writews(DIALOG_STATE, ds_readws(0xe30e) == -1 ? host_readb(ptr1 + 5) : ds_readws(0xe30e));

		if (ds_readws(DIALOG_DONE) == 0) {

			if (options != 0) {

				if (answer == -1) {
					ds_writew(DIALOG_DONE, 1);
				} else if (answer == 1) {
					ds_writew(DIALOG_STATE, host_readb(ptr1 + 5));
				} else if (answer == 2) {
					ds_writew(DIALOG_STATE, host_readb(ptr1 + 6));
				} else if (answer == 3) {
					ds_writew(DIALOG_STATE, host_readb(ptr1 + 7));
				}
			}

			if (ds_readws(DIALOG_STATE) == 255) {
				ds_writew(DIALOG_DONE, 1);
			}
		}

	} while (ds_readws(DIALOG_DONE) == 0);

	ds_writews(0x346e, 0);
	ds_writews(TEXTBOX_WIDTH, 3);
	ds_writews(0x26bd, -1);
	load_buffer_1(ds_readws(0x26bf));
	set_var_to_zero();
}

void TLK_tavern(signed short a0)
{

}
#endif

#if !defined(__BORLANDC__)
}
#endif
