/**
 *	Rewrite of DSA1 v3.02_de functions of seg072 (informers)
 *	Functions rewritten: 3/9
 */

#include "v302de.h"

#include "seg002.h"
#include "seg025.h"
#include "seg072.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief dialog logic for the informers hjore and jurge
 * \param informer	0 = jurge, 1 = hjore
 * \param state		state of the dialog
 */
/* Borlandified and identical */
void INF_jurge_hjore(signed short informer, signed short state)
{

	if (!informer) {
		/* JURGE TORFINSSON */

		if (!state) {
			ds_writew(0xe30e, (ds_readb(INFORMER_JURGE) == 1 || ds_readb(0x360f) != 0) ? 44 : 43);
		} else if (state == 11) {
			ds_writew(0xe30e, has_intro_letter() ? 36 : 37);
		} else if (state == 14) {
			ds_writew(0xe30e, has_intro_letter() ? 39 : 40);
		} else if (state == 19) {
			ds_writew(0xe30e, ds_readb(0x360f) != 0 ? 1 : (!ds_readb(0x360f) && ds_readb(INFORMER_JURGE) != 0 ? 2 : 3));
		} else if (state == 20) {
			ds_writew(0xe30e, has_intro_letter() ? 21 : 22);
		} else if (state == 27) {
			/* get the map */
			ds_writew(0xe30e, count_map_parts() ? 29 : 30);

			if (ds_readb(TREASURE_MAPS + 0) == 2) {
				ds_writeb(TMAP_DOUBLE2, 1);
			}

			ds_writeb(TREASURE_MAPS, 1);

			add_hero_ap_all(10);
		} else if (state == 29 || state == 30) {
			show_treasure_map();
		} else if (state == 35) {
			if (!ds_readb(INFORMER_YASMA)) ds_writeb(INFORMER_YASMA, 1);
			if (!ds_readb(INFORMER_RAGNA)) ds_writeb(INFORMER_RAGNA, 1);
			if (!ds_readb(INFORMER_SWAFNILD)) ds_writeb(INFORMER_SWAFNILD, 1);
			if (!ds_readb(INFORMER_ALGRID)) ds_writeb(INFORMER_ALGRID, 1);
		} else if (state == 40 || state == 41) {
			/* need introduction letter ? */
			ds_writeb(0x344c, 1);
		} else if (state == 42) {
			ds_writeb(INFORMER_JURGE, 2);
		} else if (state == 44) {
			ds_writew(0xe30e, ds_readb(0x344c) != 0 ? 20 : 19);
		} else if (state == 45) {
			ds_writew(0xe30e, !ds_readb(INFORMER_JURGE) ? 6 : 5);
		}
	} else if (informer == 1) {
		/* HJORE AHRENSSON */

		if (!state) {
			ds_writew(0xe30e, !ds_readb(INFORMER_HJORE) ? 16 : (ds_readb(INFORMER_HJORE) == 2 ? 1 : 5));
		} else if (state == 5) {
			ds_writew(0xe30e, get_first_hero_with_item(176) != -1 ? 6 : 7);
		} else if (state == 8 || state == 9 || state == 12) {
			ds_writeb(INFORMER_HJORE, 2);
		} else if (state == 10) {
			if (!ds_readb(INFORMER_BEORN)) ds_writeb(INFORMER_BEORN, 1);
		} else if (state == 13) {
			/* get the false map part ?*/
			if (ds_readb(0x333b) == 2) {
				ds_writeb(TMAP_DOUBLE2, 1);
			}

			ds_writeb(0x333b, 1);

			add_hero_ap_all(10);

			show_treasure_map();
		} else if (state == 14) {
			ds_writeb(INFORMER_HJORE, 2);

			if (!ds_readb(INFORMER_YASMA)) ds_writeb(INFORMER_YASMA, 1);
		}
	}
}

/**
 * count_map_parts - counts the parts of the treasure map
 */
/* Borlandified and identical */
short count_map_parts(void)
{
	int i;
	int parts;

	for (i = parts = 0; i < 9; i++) {
		if (ds_readb(TREASURE_MAPS + i) != 0)
			parts++;
	}

	return parts;
}

/**
 * has_intro_letter - check if a hero in the group has the introduction letter
 *
 */
/* Borlandified and identical */
short has_intro_letter(void)
{
	/* check for the introduction letter / Empfehlungsschreiben */
	return (get_first_hero_with_item(0xeb) != -1) ? 1 : 0;
}

#if !defined(__BORLANDC__)
}
#endif

