/**
 *	Rewrite of DSA1 v3.02_de functions of seg072 (informers)
 *	Functions rewritten: 4/9
 */

#include "v302de.h"

#include "seg002.h"
#include "seg025.h"
#include "seg072.h"
#include "seg105.h"

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
 * \brief dialog logic for the informers yasma, umbrik and isleif
 * \param informer	0 = yasma, 1 = umbrik, 2 = isleif
 * \param state		state of the dialog
 */
/* Borlandified and identical */
void INF_yasma_umbrik_isleif(signed short informer, signed short state)
{
	Bit8u *hero;

	if (!informer) {
		/* YASMA THINMARSDOTTER */

		if (!state) {
			ds_writew(0xe30e, ds_readb(INFORMER_YASMA) == 2 ? 1 : 2);
		} else if (state == 15) {
			ds_writeb(0x3df3, 1);
		} else if (state == 22) {
			/* make HJORE AHRENSSON known */
			if (!ds_readb(INFORMER_HJORE)) ds_writeb(INFORMER_HJORE, 1);

			/* make UMBRIK SIEBENSTEIN known */
			if (!ds_readb(INFORMER_UMBRIK)) ds_writeb(INFORMER_UMBRIK, 1);

			/* mark YASMA THINMARSDOTTER as done */
			ds_writeb(INFORMER_YASMA, 2);
		}
	} else if (informer == 1) {
		/* UMBRIK SIEBENSTEIN */

		if (!state) {
			ds_writew(0xe30e, ds_readb(INFORMER_UMBRIK) == 2 ? 1 : 2);
		} else if (state == 2) {
			ds_writew(0xe30e, ds_readb(QUEST_GORAH) != 0 ? 15 : (!ds_readb(INFORMER_UMBRIK) ? 24 : 3));
		} else if (state == 11 || state == 13) {
			/* mark UMBRIK SIEBENSTEIN as done */
			ds_writeb(INFORMER_UMBRIK, 2);
		} else if (state == 14) {
			/* the heros now know of GORAH */
			ds_writeb(QUEST_GORAH, 1);
		} else if (state == 15) {
			/* check if the heros have the RUNENKNOCHEN / BONE WITH RUNE */
			ds_writew(0xe30e, get_first_hero_with_item(164) != -1 ? 16 : 17);
		} else if (state == 19) {
			/* give the RUNENKNOCHEN / BONE WITH RUNE to UMBRIK */
			hero = get_hero(get_first_hero_with_item(164));
			drop_item(hero, get_item_pos(hero, 164), 1);
		} else if (state == 23) {
			/* mark UMBRIK SIEBENSTEIN as done */
			ds_writeb(INFORMER_UMBRIK, 2);

			/* get EMPFEHLUNGSSCHREIBEN / LETTER OF INTRODUCTION */
			get_item(247, 1, 1);

			/* make BEORN HJALLASSON known */
			if (!ds_readb(INFORMER_BEORN)) ds_writeb(INFORMER_BEORN, 1);

			/* make TIOMAR SWAFNILDSSON known */
			if (!ds_readb(INFORMER_TIOMAR)) ds_writeb(INFORMER_TIOMAR, 1);
		}

	} else if (informer == 2) {
		/* ISLEIF OLGARDSSON */

		if (!state) {
			ds_writew(0xe30e, ds_readb(INFORMER_ISLEIF) == 2 ? 1 : 2);
		} else if (state == 2) {
			/* TODO: what does that mean ? */
			ds_writew(0xe30e, ds_readb(0x344f) != 0 ? 3 : 4);
		} else if (state == 8 || state == 23 || state == 25 || state == 27) {
			/* mark ISLEIF OLGARDSSON as done */
			ds_writeb(INFORMER_ISLEIF, 2);
		} else if (state == 9) {
			/* TODO: what does that mean ? */
			ds_writew(0xe30e, ds_readb(0x344f) != 0 ? 10 : 11);
		} else if (state == 15 || state == 19) {
			/* TODO: check what happens here */
		} else if (state == 16 || state == 20) {
			/* TODO: check what happens here */
		} else if (state == 17 || state == 26) {

			/* check if the heros already have the map */
			if (ds_readb(TREASURE_MAPS + 2) == 2) ds_writeb(TMAP_DOUBLE2, 1);

			/* the heros get the map */
			ds_writeb(TREASURE_MAPS + 2, 1);

			/* each of the heros gets 10 AP */
			add_hero_ap_all(10);

			show_treasure_map();

			/* TODO: what does that mean ? */
			if (state == 17) ds_writeb(0x3450, 1);

		} else if (state == 24) {
			/* TODO: what does that mean ? */
			ds_writew(0xe30e, ds_readb(0x3450) != 0 ? 25 : 26);
		}
	}
}

/**
 * \brief counts the collected parts of the treasure map
 * \return the number of map parts
 */
/* Borlandified and identical */
signed short count_map_parts(void)
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
 * \brief check if a hero in the group has the letter of introduction
 * \return 0 = no letter, 1 = have the letter
 *
 */
/* Borlandified and identical */
signed short has_intro_letter(void)
{
	/* check for the introduction letter / Empfehlungsschreiben */
	return (get_first_hero_with_item(0xeb) != -1) ? 1 : 0;
}

#if !defined(__BORLANDC__)
}
#endif

