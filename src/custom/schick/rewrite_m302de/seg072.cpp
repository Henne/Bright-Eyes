/**
 *	Rewrite of DSA1 v3.02_de functions of seg072 (informer scripting)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg072.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg047.h"
#include "seg055.h"
#include "seg072.h"
#include "seg095.h"
#include "seg103.h"
#include "seg105.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief dialog logic for the informers hjore and jurge
 * \param informer	0 = jurge, 1 = hjore
 * \param state		state of the dialog
 */
void INF_jurge_hjore(signed short informer, signed short state)
{

	if (!informer) {
		/* JURGE TORFINSSON */

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, (ds_readb(INFORMER_FLAGS + INFORMER_JURGE) == 1 || ds_readb(TLK_TAV_FOLLOWINFORMER) != 0) ? 44 : 43);
		} else if (state == 11) {
			ds_writew(DIALOG_NEXT_STATE, has_intro_letter() ? 36 : 37);
		} else if (state == 14) {
			ds_writew(DIALOG_NEXT_STATE, has_intro_letter() ? 39 : 40);
		} else if (state == 19) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FOLLOWINFORMER) != 0 ? 1 : (!ds_readb(TLK_TAV_FOLLOWINFORMER) && ds_readb(INFORMER_FLAGS + INFORMER_JURGE) != 0 ? 2 : 3));
		} else if (state == 20) {
			ds_writew(DIALOG_NEXT_STATE, has_intro_letter() ? 21 : 22);
		} else if (state == 27) {
			/* get the map */
			ds_writew(DIALOG_NEXT_STATE, count_map_parts() ? 29 : 30);

			if (ds_readb(TREASURE_MAPS + 0) == 2) {
				ds_writeb(TMAP_DOUBLE2, 1);
			}

			ds_writeb(TREASURE_MAPS, 1);

			add_hero_ap_all(10);
		} else if (state == 29 || state == 30) {
			show_treasure_map();
		} else if (state == 35) {
			if (!ds_readb(INFORMER_FLAGS + INFORMER_YASMA)) ds_writeb(INFORMER_FLAGS + INFORMER_YASMA, 1);
			if (!ds_readb(INFORMER_FLAGS + INFORMER_RAGNA)) ds_writeb(INFORMER_FLAGS + INFORMER_RAGNA, 1);
			if (!ds_readb(INFORMER_FLAGS + INFORMER_SWAFNILD)) ds_writeb(INFORMER_FLAGS + INFORMER_SWAFNILD, 1);
			if (!ds_readb(INFORMER_FLAGS + INFORMER_ALGRID)) ds_writeb(INFORMER_FLAGS + INFORMER_ALGRID, 1);
		} else if (state == 40 || state == 41) {
			/* need introduction letter ? */
			ds_writeb(JURGE_AWAITS_LETTER, 1);
		} else if (state == 42) {
			ds_writeb(INFORMER_FLAGS + INFORMER_JURGE, 2);
		} else if (state == 44) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(JURGE_AWAITS_LETTER) != 0 ? 20 : 19);
		} else if (state == 45) {
			ds_writew(DIALOG_NEXT_STATE, !ds_readb(INFORMER_FLAGS + INFORMER_JURGE) ? 6 : 5);
		}
	} else if (informer == 1) {
		/* HJORE AHRENSSON */

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, !ds_readb(INFORMER_FLAGS + INFORMER_HJORE) ? 16 : (ds_readb(INFORMER_FLAGS + INFORMER_HJORE) == 2 ? 1 : 5));
		} else if (state == 5) {
			ds_writew(DIALOG_NEXT_STATE, get_first_hero_with_item(176) != -1 ? 6 : 7);
		} else if (state == 8 || state == 9 || state == 12) {
			ds_writeb(INFORMER_FLAGS + INFORMER_HJORE, 2);
		} else if (state == 10) {
			if (!ds_readb(INFORMER_FLAGS + INFORMER_BEORN)) ds_writeb(INFORMER_FLAGS + INFORMER_BEORN, 1);
		} else if (state == 13) {
			/* get the false map part ?*/
			if (ds_readb(TREASURE_MAPS + 9) == 2) {
				ds_writeb(TMAP_DOUBLE2, 1);
			}

			ds_writeb(TREASURE_MAPS + 9, 1);

			add_hero_ap_all(10);

			show_treasure_map();
		} else if (state == 14) {
			ds_writeb(INFORMER_FLAGS + INFORMER_HJORE, 2);

			if (!ds_readb(INFORMER_FLAGS + INFORMER_YASMA)) ds_writeb(INFORMER_FLAGS + INFORMER_YASMA, 1);
		}
	}
}

/**
 * \brief dialog logic for the informers yasma, umbrik and isleif
 * \param informer	0 = yasma, 1 = umbrik, 2 = isleif
 * \param state		state of the dialog
 */
void INF_yasma_umbrik_isleif(signed short informer, signed short state)
{
	Bit8u *hero;

	if (!informer) {
		/* YASMA THINMARSDOTTER */

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + INFORMER_YASMA) == 2 ? 1 : 2);
		} else if (state == 15) {
			ds_writeb(0x3df3, 1);
		} else if (state == 22) {
			/* make HJORE AHRENSSON known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_HJORE)) ds_writeb(INFORMER_FLAGS + INFORMER_HJORE, 1);

			/* make UMBRIK SIEBENSTEIN known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_UMBRIK)) ds_writeb(INFORMER_FLAGS + INFORMER_UMBRIK, 1);

			/* mark YASMA THINMARSDOTTER as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_YASMA, 2);
		}
	} else if (informer == 1) {
		/* UMBRIK SIEBENSTEIN */

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + INFORMER_UMBRIK) == 2 ? 1 : 2);
		} else if (state == 2) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(QUEST_GORAH) != 0 ? 15 : (!ds_readb(INFORMER_FLAGS + INFORMER_UMBRIK) ? 24 : 3));
		} else if (state == 11 || state == 13) {
			/* mark UMBRIK SIEBENSTEIN as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_UMBRIK, 2);
		} else if (state == 14) {
			/* the heros now know of GORAH */
			ds_writeb(QUEST_GORAH, 1);
		} else if (state == 15) {
			/* check if the heros have the RUNENKNOCHEN / BONE WITH RUNE */
			ds_writew(DIALOG_NEXT_STATE, get_first_hero_with_item(164) != -1 ? 16 : 17);
		} else if (state == 19) {
			/* give the RUNENKNOCHEN / BONE WITH RUNE to UMBRIK */
			hero = get_hero(get_first_hero_with_item(164));
			drop_item(hero, get_item_pos(hero, 164), 1);
		} else if (state == 23) {
			/* mark UMBRIK SIEBENSTEIN as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_UMBRIK, 2);

			/* get EMPFEHLUNGSSCHREIBEN / LETTER OF INTRODUCTION */
			get_item(247, 1, 1);

			/* make BEORN HJALLASSON known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_BEORN)) ds_writeb(INFORMER_FLAGS + INFORMER_BEORN, 1);

			/* make TIOMAR SWAFNILDSSON known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_TIOMAR)) ds_writeb(INFORMER_FLAGS + INFORMER_TIOMAR, 1);
		}

	} else if (informer == 2) {
		/* ISLEIF OLGARDSSON */

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + INFORMER_ISLEIF) == 2 ? 1 : 2);
		} else if (state == 2) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(ISLEIF_JANDAS_REGARDS) != 0 ? 3 : 4);
		} else if (state == 8 || state == 23 || state == 25 || state == 27) {
			/* mark ISLEIF OLGARDSSON as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_ISLEIF, 2);
		} else if (state == 9) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(ISLEIF_JANDAS_REGARDS) != 0 ? 10 : 11);
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
			if (state == 17) ds_writeb(ISLEIF_MAP_GIVEN, 1);

		} else if (state == 24) {
			/* TODO: what does that mean ? */
			ds_writew(DIALOG_NEXT_STATE, ds_readb(ISLEIF_MAP_GIVEN) != 0 ? 25 : 26);
		}
	}
}

/**
 * \brief dialog logic for the informers ragna, beorn and algrid
 * \param informer	0 = ragna, 1 = beorn, 2 = algrid
 * \param state		state of the dialog
 */
void INF_ragna_beorn_algrid(signed short informer, signed short state)
{
	unsigned char tmp;
	signed short beorn_ch_malus;

	if (informer == 0) {
		/* RAGNA FIRUNJASDOTTER */

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + INFORMER_RAGNA) == 2 ? 1 : 3);
		} else if (state == 4) {
			ds_writew(DIALOG_NEXT_STATE, count_map_parts() ? 9 : 10);
		} else if (state == 8 || state == 25) {
			/* mark RAGNA FIRUNJASDOTTER as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_RAGNA, 2);
		} else if (state == 12) {
			/* directly asking for map gets a brush-off ("don't know what you
			   are talking about") and is silently noted by ragna!
			   it's not possible to get to see the map after that (see state 17) */
			ds_writeb(RAGNA_ASKED_MAP, 1);
		} else if (state == 14) {
			/* mark ISLEIF OLGARDSSON as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_ISLEIF)) ds_writeb(INFORMER_FLAGS + INFORMER_ISLEIF, 1);
			/* mark SWAFNILD EGILSDOTTER as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_SWAFNILD)) ds_writeb(INFORMER_FLAGS + INFORMER_SWAFNILD, 1);
		} else if (state == 16) {
			/* mark JURGE TORFINSSON as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_JURGE))
			    ds_writeb(INFORMER_FLAGS + INFORMER_JURGE, 1);
		} else if (state == 17) {
			/* see state 12 */
			ds_writew(DIALOG_NEXT_STATE, ds_readb(RAGNA_ASKED_MAP) != 0 ? 18 : 19);
		} else if (state == 21) {
			/* check if the party already has this map piece */
			if (ds_readb(TREASURE_MAPS + 3) == 2) ds_writeb(TMAP_DOUBLE2, 1);
			/* get the map piece */
			ds_writeb(TREASURE_MAPS + 3, 1);
			/* each of the heros gets 10 AP */
			add_hero_ap_all(10);
		} else if (state == 22) {
			/* test the group leader on KL+5, to get the map */
			if (test_attrib(get_hero(0), ATTRIB_KL, 5) > 0) {
				/* check if the party already has this map piece */
				if (ds_readb(TREASURE_MAPS + 3) == 2) ds_writeb(TMAP_DOUBLE2, 1);
				/* get the map piece */
				ds_writeb(TREASURE_MAPS + 3, 1);
				/* each of the heros gets 10 AP */
				add_hero_ap_all(10);
			}
		} else if (state == 23) {

			/* she only shows you the map piece */

			tmp = ds_readb(TREASURE_MAPS + 3);

			/* check if the party already has this map piece */
			if (ds_readb(TREASURE_MAPS + 3) == 2) ds_writeb(TMAP_DOUBLE2, 1);

			/* get the map piece */
			ds_writeb(TREASURE_MAPS + 3, 1);

			show_treasure_map();
			/* remove the map piece */
			ds_writeb(TREASURE_MAPS + 3, tmp);
		}
	} else if (informer == 1) {
			/* BEORN HJALLASSON */

			if (!state) {
				ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + INFORMER_BEORN) == 2 ? 1 : 2);
			} else if (state == 2) {
				/* is ERWO in the group ? */
				ds_writew(DIALOG_NEXT_STATE,
					host_readbs(get_hero(6) + HERO_NPC_ID) == NPC_ERWO && is_hero_available_in_group(get_hero(6)) ? 3 : 15);
			} else if (state == 6) {

				/* copy the name */
				strcpy((char*)(p_datseg + STR_BEORN_HJALL), (char*)(get_hero(6) + HERO_NAME2));
				/* set a pointer */
				ds_writed(DIALOG_TITLE, (Bit32u)RealMake(datseg, STR_BEORN_HJALL));
				/* copy the picture of the NPC */
				memcpy(Real2Host(ds_readd(DTP2)), get_hero(6) + HERO_PORTRAIT, 0x400);
				/* remove the NPC from the group */
				remove_npc(24, 31, 231, get_ltx(0xbd8), (Bit8u*)0);

				ds_writew(REQUEST_REFRESH, 1);

			} else if (state == 7 || state == 8 || state == 9 || state == 10) {
				timewarp(MINUTES(30));
			} else if (state == 11 || state == 26) {
				/* check if the party already has this map piece */
				if (ds_readb(TREASURE_MAPS + 4) == 2) ds_writeb(TMAP_DOUBLE2, 1);
				/* get the map piece */
				ds_writeb(TREASURE_MAPS + 4, 1);
				/* each of the heros gets 10 AP */
				add_hero_ap_all(10);

				show_treasure_map();
			} else if (state == 12 || state == 29) {
				/* mark BEORN HJALLASSON as done */
				ds_writeb(INFORMER_FLAGS + INFORMER_BEORN, 2);
			} else if (state == 13 || state == 22) {
				/* make HJORE AHRENSSON known */
				if (!ds_readb(INFORMER_FLAGS + INFORMER_HJORE)) ds_writeb(INFORMER_FLAGS + INFORMER_HJORE, 1);
				/* mark RAGNA FIRUNJASDOTTER as known */
				if (!ds_readb(INFORMER_FLAGS + INFORMER_RAGNA)) ds_writeb(INFORMER_FLAGS + INFORMER_RAGNA, 1);
				/* mark SWAFNILD EGILSDOTTER as known */
				if (!ds_readb(INFORMER_FLAGS + INFORMER_SWAFNILD)) ds_writeb(INFORMER_FLAGS + INFORMER_SWAFNILD, 1);
			} else if (state == 18) {
				/* after "NUN, IHR SEID DOCH MIT HYGGELIK VERWANDT..." */
				ds_writeb(BEORN_LITT_BOLD, 1);
			} else if (state == 23) {
				/* after "NA, DAS WAR JA NICHT BESONDERS VIEL!" */
				ds_writeb(BEORN_VERY_BOLD, 1);
			} else if (state == 24) {
			    /* after 22 (new contacts), extremely bold heroes get the map now */
				ds_writew(DIALOG_NEXT_STATE, ds_readb(BEORN_LITT_BOLD) != 0 && ds_readb(BEORN_MEDI_BOLD) != 0 && ds_readb(BEORN_VERY_BOLD) != 0 ? 26 : 27);
			} else if (state == 25) {
				/* if asked "WISST IHR VIELLEICHT AUCH ETWAS?" */
				ds_writeb(BEORN_MEDI_BOLD, 1);
			} else if (state == 27) {
			    /* if not extremely bold, the hero is tested for CHARISMA with
			       malus as follows:
                       beorn_ch_malus = 10
                       beorn_ch_malus -= 4*l + 4*m + 6*v
                       if(l+m+v == 2) beorn_ch_malus -= 2
			       (l = BEORN_LITT_BOLD, m = BEORN_MEDI_BOLD, v = BEORN_VERY_BOLD)
			    */

				beorn_ch_malus = 10;

				if (ds_readb(BEORN_VERY_BOLD) && !ds_readb(BEORN_MEDI_BOLD) && !ds_readb(BEORN_LITT_BOLD)) {
					beorn_ch_malus = 4;
				} else if (ds_readb(BEORN_VERY_BOLD) && ds_readb(BEORN_MEDI_BOLD) && !ds_readb(BEORN_LITT_BOLD)) {
					beorn_ch_malus = -2;
				} else if (ds_readb(BEORN_VERY_BOLD) && !ds_readb(BEORN_MEDI_BOLD) && ds_readb(BEORN_LITT_BOLD)) {
					beorn_ch_malus = -2;
				} else if (!ds_readb(BEORN_VERY_BOLD) && ds_readb(BEORN_MEDI_BOLD) && !ds_readb(BEORN_LITT_BOLD)) {
					beorn_ch_malus = 6;
				} else if (!ds_readb(BEORN_VERY_BOLD) && !ds_readb(BEORN_MEDI_BOLD) && ds_readb(BEORN_LITT_BOLD)) {
					beorn_ch_malus = 6;
				} else if (ds_readb(BEORN_LITT_BOLD) && ds_readb(BEORN_MEDI_BOLD)) {
					beorn_ch_malus = 0;
				}

				ds_writew(DIALOG_NEXT_STATE, test_attrib(get_hero(0), ATTRIB_CH, beorn_ch_malus) > 0 ? 26 : 28);
			}
	} else if (informer == 2) {
		/* ALGRID TRONDESDOTTER */

		if (!state) {
			/* TODO: check what happens here */
			ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FOLLOWINFORMER) ? 23 : (ds_readb(INFORMER_FLAGS + INFORMER_ALGRID) == 2 ? 1 : 22));
		} else if (state == 2) {
			/* mark ALGRID TRONDESDOTTER as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_ALGRID, 2);

			ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + INFORMER_JURGE) == 2 ? 3 : 4);
		} else if (state == 3) {
			/* TODO: check what happens here */
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3467) ? 5 : 6);
		} else if (state == 4) {
			/* TODO: check what happens here */
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3467) ? 7 : 8);
		} else if (state == 14) {
			/* make TIOMAR SWAFNILDSSON known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_TIOMAR)) ds_writeb(INFORMER_FLAGS + INFORMER_TIOMAR, 1);

			/* make TREBORN KOLBERG known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_TREBORN)) ds_writeb(INFORMER_FLAGS + INFORMER_TREBORN, 1);
		}
	}
}

/**
 * \brief dialog logic for the informers eliane and tiomar
 * \param informer	0 = eliane, 1 = tiomar
 * \param state		state of the dialog
 */
void INF_eliane_tiomar(signed short informer, signed short state)
{
	if (!informer) {
		/* ELIANE WINDENBECK */

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(QUEST_NAMELESS_GOT) || ds_readw(GOT_MAIN_QUEST) == 0 ? 1 : 6);
		} else if (state == 1) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(QUEST_NAMELESS_DONE) && ds_readb(INFORMER_FLAGS + INFORMER_ELIANE) != 2 ? 2 : 3);
		} else if (state == 5 || state == 27) {
				/* check if the party already has this map piece */
				if (ds_readb(TREASURE_MAPS + 5) == 2) ds_writeb(TMAP_DOUBLE2, 1);
				/* get the map piece */
				ds_writeb(TREASURE_MAPS + 5, 1);
				/* each of the heros gets 10 AP */
				add_hero_ap_all(10);

				show_treasure_map();

				/* mark ELIANE WINDENBECK as done */
				ds_writeb(INFORMER_FLAGS + INFORMER_ELIANE, 2);
		} else if (state == 19) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(QUEST_NAMELESS_DONE) ? 20 : 30);
			ds_writeb(QUEST_NAMELESS_GOT, 1);
		} else if (state == 16) {
			/* mark YASMA THINMARSDOTTER as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_YASMA)) ds_writeb(INFORMER_FLAGS + INFORMER_YASMA, 1);
			/* mark SWAFNILD EGILSDOTTER as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_SWAFNILD)) ds_writeb(INFORMER_FLAGS + INFORMER_SWAFNILD, 1);
			/* mark ASGRIMM THURBOLDSSON as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_ASGRIMM)) ds_writeb(INFORMER_FLAGS + INFORMER_ASGRIMM, 1);
		} else if (state == 24) {
			/* the group has the SCHWARZE STATUETTE/BLACK FIGURINE */
			ds_writew(DIALOG_NEXT_STATE, get_first_hero_with_item(248) != -1 ? 27 : 28);
		}
	} else if (informer == 1) {
		/* TIOMAR SWAFNILDSSON */

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FOLLOWINFORMER) != 0 ?
						44 :
						(!ds_readb(INFORMER_FLAGS + INFORMER_TIOMAR) || ds_readb(INFORMER_FLAGS + INFORMER_TIOMAR) == 2 ?	2 : 1));
		} else if (state == 1) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3469) != 0 ? 36 : 3);
		} else if (state == 4) {
			ds_writew(DIALOG_NEXT_STATE, get_first_hero_with_item(247) != -1 ? 6 : 7);
		} else if (state == 12 || state == 42) {
				/* check if the party already has this map piece */
				if (ds_readb(TREASURE_MAPS + 8) == 2) ds_writeb(TMAP_DOUBLE2, 1);
				/* get the map piece */
				ds_writeb(TREASURE_MAPS + 8, 1);
				/* each of the heros gets 10 AP */
				add_hero_ap_all(10);

				show_treasure_map();

				/* mark TIOMAR SWAFNILDSSON as done */
				ds_writeb(INFORMER_FLAGS + INFORMER_TIOMAR, 2);
		} else if (state == 17) {
			/* mark JURGE TORFINSSON as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_JURGE)) ds_writeb(INFORMER_FLAGS + INFORMER_JURGE, 1);
			/* mark ISLEIF OLGARDSSON as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_ISLEIF)) ds_writeb(INFORMER_FLAGS + INFORMER_ISLEIF, 1);
		} else if (state == 20) {
			/* drink with TIOMAR */
			timewarp(HOURS(1));
			ds_writew(DIALOG_NEXT_STATE, test_skill(get_hero(ds_writeb(0x3468, (unsigned char)get_random_hero())), 18, 0) > 0 ? 21 : 22);
		} else if (state == 22) {
			/* TIOMARS drinkmate gets drunken */
			hero_get_drunken(get_hero(ds_readb(0x3468)));
		} else if (state == 31) {
			/* mark JURGE TORFINSSON as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_JURGE)) ds_writeb(INFORMER_FLAGS + INFORMER_JURGE, 1);
			/* mark ISLEIF OLGARDSSON as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_ISLEIF)) ds_writeb(INFORMER_FLAGS + INFORMER_ISLEIF, 1);
			/* mark UMBRIK SIEBENSTEIN as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_UMBRIK)) ds_writeb(INFORMER_FLAGS + INFORMER_UMBRIK, 1);
		} else if (state == 34) {
			ds_writeb(0x3469, 1);
		} else if (state == 36) {
			ds_writew(DIALOG_NEXT_STATE, get_first_hero_with_item(247) != -1 ? 37 : 2);
		} else if (state == 45) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + INFORMER_UMBRIK) == 2 ? 46 : 47);
		}

	}
}

/**
 * \brief dialog logic for the informers olvir and asgrimm
 * \param informer	0 = olvir, 1 = asgrimm
 * \param state		state of the dialog
 */
void INF_olvir_asgrimm(signed short informer, signed short state)
{
	Bit8u *hero;

	if (!informer) {
		/* OLVIR GUNDRIDSSON */

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + INFORMER_OLVIR) == 2 ? 1 : 2);
		} else if (state == 2) {
			/* mark OLVIR GUNDRIDSSON as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_OLVIR, 2);
		} else if (state == 8) {
			ds_writeb(0x3456, ds_writeb(0x345a, 1));
		} else if (state == 9) {
			ds_writeb(0x3457, ds_writeb(0x345b, 1));
		} else if (state == 10) {
			ds_writeb(0x3458, ds_writeb(0x345c, 1));
		} else if (state == 12 || state == 13 || state == 23 || state == 24 || state == 29 || state == 30) {
			timewarp(MINUTES(30));
		} else if (state == 14 || state == 15 || state == 21 || state == 22 || state == 28) {
			timewarp(HOURS(1));
		} else if (state == 16) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3459) != 0 ? 19 : 13);
			ds_writeb(0x3459, 1);
		} else if (state == 17) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3459) != 0 ? 19 : 14);
			ds_writeb(0x3459, 1);
		} else if (state == 18) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3459) != 0 ? 19 : 15);
			ds_writeb(0x3459, 1);
		} else if (state == 26) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3459) != 0 ? 19 : 22);
			ds_writeb(0x3459, 1);
		} else if (state == 27) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3459) != 0 ? 19 : 23);
			ds_writeb(0x3459, 1);
		} else if (state == 31) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3459) != 0 ? 19 : 29);
			ds_writeb(0x3459, 1);
		} else if (state == 32) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3459) != 0 ? 19 : 30);
			ds_writeb(0x3459, 1);
		} else if (state == 33) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x345a) != 0 ? 11 : (ds_readb(0x345b) != 0 ? 35 : 36));
		} else if (state == 34) {
			ds_writeb(0x345a, 0);
			ds_writeb(0x345b, 1);
		} else if (state == 35) {
			ds_writeb(0x345b, 0);
			ds_writeb(0x345c, 1);
		} else if (state == 37) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3457) != 0 ? 39 : 40);
		} else if (state == 39) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3456) != 0 ? 42 : 41);
		}
	} else if (informer == 1) {
		/* ASGRIMM THURBOLDSSON */

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(TLK_TAV_FOLLOWINFORMER) != 0 ? 22 : (ds_readw(GOT_MAIN_QUEST) == 0|| ds_readb(INFORMER_FLAGS + INFORMER_ASGRIMM) == 2 ? 1 : 2));
		} else if (state == 2) {
			/* mark ASGRIMM THURBOLDSSON as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_ASGRIMM, 2);
		} else if (state == 7) {
			signed short i;
			/* ASGRIMM takes a meal with the heros */
			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
					(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
					!hero_dead(hero))
				{
					/* set hunger and thirst to 0 */
					host_writebs(hero + HERO_HUNGER, host_writebs(hero + HERO_THIRST, 0));
				}
			}
		} else if (state == 16) {
			/* make HJORE AHRENSSON known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_HJORE)) ds_writeb(INFORMER_FLAGS + INFORMER_HJORE, 1);
			/* mark RAGNA FIRUNJASDOTTER as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_RAGNA)) ds_writeb(INFORMER_FLAGS + INFORMER_RAGNA, 1);
		}
	}
}

/**
 * \brief dialog logic for the informers treborn and unicorn (1st meeting)
 * \param informer	0 = treborn, 1 = treborn, 2 = unicorn
 * \param state		state of the dialog
 */
void INF_treborn_unicorn(signed short informer, signed short state)
{
	Bit32s money;
	signed short enough_money;

	money = get_party_money();

	if (!informer) {
		/* TREBORN KOLBERG (first meeting) */

		enough_money = money >= 6000 ? 1 : 0;

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + INFORMER_TREBORN) != 0 ? -1 : 1);
		} else if (state == 1) {
			/* mark TREBORN KOLBERG as met */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_TREBORN)) ds_writeb(INFORMER_FLAGS + INFORMER_TREBORN, 1);
		} else if (state == 5) {
			ds_writew(DIALOG_NEXT_STATE, enough_money ? 7 : 8);
		} else if (state == 9) {

			/* buy the map */
			money -= 6000;
			set_party_money(money);

			/* check if the party already has this map piece */
			if (ds_readb(TREASURE_MAPS + 7) == 2) ds_writeb(TMAP_DOUBLE2, 1);
			/* get the map piece */
			ds_writeb(TREASURE_MAPS + 7, 1);
			/* each of the heros gets 10 AP */
			add_hero_ap_all(10);

			/* mark ALGRID TRONDESDOTTER as known */
			/* Original-BUG: this would reactivate algrid */
			ds_writeb(INFORMER_FLAGS + INFORMER_ALGRID, ds_writeb(0x3467, 1));

			show_treasure_map();

		} else if (state == 11) {
			/* mark JURGE TORFINSSON as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_JURGE)) ds_writeb(INFORMER_FLAGS + INFORMER_JURGE, 1);
		} else if (state == 13) {
			ds_writew(DIALOG_NEXT_STATE, enough_money ? 14 : 15);
		} else if (state == 17) {
			ds_writew(DIALOG_NEXT_STATE, enough_money ? 19 : 20);
		} else if (state == 18) {
			ds_writew(TYPEINDEX, 91);
			do_merchant();
		} else if (state == 21) {
			ds_writeb(TREBORN_DATE, 1);
		} else if (state == 23) {
			ds_writew(DIALOG_NEXT_STATE, enough_money ? 25 : 24);
		}
	} else if (informer == 1) {
		/* TREBORN KOLBERG (second meeting) */

		enough_money = money >= 7500 ? 1 : 0;

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, !ds_readb(TREBORN_DATE) ? -1 : 1);
		} else if (state == 2) {
			ds_writew(DIALOG_NEXT_STATE, enough_money ? 3 : 4);
		} else if (state == 5) {
			ds_writew(DIALOG_NEXT_STATE, enough_money ? 6 : 7);
		} else if (state == 12) {
			/* buy the map */
			money -= 7500;
			set_party_money(money);

			/* check if the party already has this map piece */
			if (ds_readb(TREASURE_MAPS + 7) == 2) ds_writeb(TMAP_DOUBLE2, 1);
			/* get the map piece */
			ds_writeb(TREASURE_MAPS + 7, 1);
			/* each of the heros gets 10 AP */
			add_hero_ap_all(10);

			/* mark ALGRID TRONDESDOTTER as known */
			/* Original-BUG: this would reactivate algrid */
			ds_writeb(INFORMER_FLAGS + INFORMER_ALGRID, ds_writeb(0x3467, 1));

			show_treasure_map();
		} else if (state == 14) {
			/* mark JURGE TORFINSSON as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_JURGE)) ds_writeb(INFORMER_FLAGS + INFORMER_JURGE, 1);
		}

	} else if (informer == 2) {
		/* EINHORN / UNICORN (first meeting) */

		if (!state) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + INFORMER_UNICORN) == 2 ? 1 : 2);
		} else if (state == 2) {
			/* select the hero with the highest CH value */
			/* REMARK: what if the NPC is choosen ? */
			/* REMARK: what if the positions are changed ? */
			/* REMARK: what if the game is saved and the heros are at another mem location ? */
			ds_writed(UNICORN_HERO_PTR, (Bit32u)((RealPt)ds_readd(HEROS) + SIZEOF_HERO * ds_writeb(UNICORN_HERO_POS, (unsigned char)get_hero_CH_best())));
		} else if (state == 7) {
			timewarp(HOURS(1));
		} else if (state == 8) {
			timewarp(HOURS(1));
			/* mark UNICORN as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_UNICORN, 2);
		} else if (state == 9) {
			/* mark UNICORN as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_UNICORN, 2);
		} else if (state == 10) {
			/* test FF+2 */
			ds_writew(DIALOG_NEXT_STATE, test_attrib(Real2Host(ds_readd(UNICORN_HERO_PTR)), ATTRIB_GE, 2) > 0 ? 11 : 14);
		} else if (state == 11) {
			/* test FF+5 */
			ds_writew(DIALOG_NEXT_STATE, test_attrib(Real2Host(ds_readd(UNICORN_HERO_PTR)), ATTRIB_GE, 5) > 0 ? 12 : 13);
		} else if (state == 15) {
			ds_writew(DIALOG_NEXT_STATE, random_schick(100) <= 50 ? 16 : 17);
		} else if (state == 16) {
			/* the hero disappears */
			hero_disappear(Real2Host(ds_readd(UNICORN_HERO_PTR)), ds_readb(UNICORN_HERO_POS), -1);
		} else if (state == 17) {
			/* the hero gets heavily wounded, 1 LE left */
			sub_hero_le(Real2Host(ds_readd(UNICORN_HERO_PTR)), host_readws(Real2Host(ds_readd(UNICORN_HERO_PTR)) + HERO_LE) - 1);
			/* the party opens a camp */
			ds_writeb(LOCATION, 6);
			do_location();
		} else if (state == 18) {
			timewarp(HOURS(2));
		} else if (state == 25) {
			/* the UNICORN will get the map */
			ds_writeb(UNICORN_GET_MAP, 1);

			/* the hero gets 100 AP */
			add_hero_ap(Real2Host(ds_readd(UNICORN_HERO_PTR)), 100);

			/* set the unicorn timer (in days) */
			ds_writeb(UNICORN_TIMER, random_schick(24) + 36);
		}
	}
}

/**
 * \brief dialog logic for the informers swafnild and unicorn (2nd meeting)
 * \param informer	0 = treborn, 1 = unicorn
 * \param state		state of the dialog
 */
void INF_swafnild_unicorn(signed short informer, signed short state)
{
	if (!informer) {
		/* SWAFNILD EGILSDOTTER */

		if (state == 1) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(INFORMER_FLAGS + INFORMER_SWAFNILD) == 2 ? 3 : 7);
		} else if (state == 4) {
			ds_writew(DIALOG_NEXT_STATE, ds_readb(0x3462) != 0 ? 38 : 39);
		} else if (state == 7) {

			if ((ds_readbs(CURRENT_TOWN) >= 1 && ds_readbs(CURRENT_TOWN) <= 10) ||
				(ds_readbs(CURRENT_TOWN) >= 40 && ds_readbs(CURRENT_TOWN) <= 44))
			{
				ds_writeb(SWAFNILD_TP1, 39);
				ds_writeb(SWAFNILD_TP2, 38);
				ds_writeb(SWAFNILD_TP3, 37);
				ds_writeb(SWAFNILD_TP4, 35);
			} else if (ds_readbs(CURRENT_TOWN) >= 30 && ds_readbs(CURRENT_TOWN) <= 34) {
				ds_writeb(SWAFNILD_TP1, 37);
				ds_writeb(SWAFNILD_TP2, 38);
				ds_writeb(SWAFNILD_TP3, 39);
				ds_writeb(SWAFNILD_TP4, 42);
			} else {
				ds_writeb(SWAFNILD_TP1, 33);
				ds_writeb(SWAFNILD_TP2, 34);
				ds_writeb(SWAFNILD_TP3, 36);
				ds_writeb(SWAFNILD_TP4, 32);
			}
		} else if (state == 17) {
			/* mark RAGNA FIRUNJASDOTTER as known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_RAGNA)) ds_writeb(INFORMER_FLAGS + INFORMER_RAGNA, 1);
			/* make BEORN HJALLASSON known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_BEORN)) ds_writeb(INFORMER_FLAGS + INFORMER_BEORN, 1);
			/* make TIOMAR SWAFNILDSSON known */
			if (!ds_readb(INFORMER_FLAGS + INFORMER_TIOMAR)) ds_writeb(INFORMER_FLAGS + INFORMER_TIOMAR, 1);
		} else if (state == 18) {
			/* test CH+5 */
			ds_writew(DIALOG_NEXT_STATE, test_attrib(get_hero(0), ATTRIB_CH, 5) > 0 ? 19 : 20);
		} else if (state == 21) {
			/* mark SWAFNILD EGILSDOTTER as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_SWAFNILD, 2);
			ds_writeb(0x3462, 1);
		} else if (state == 22) {
			/* test CH+3 */
			ds_writew(DIALOG_NEXT_STATE, test_attrib(get_hero(0), ATTRIB_CH, 3) > 0 ? 24 : 23);
		} else if (state == 24 || state == 41) {
			/* mark SWAFNILD EGILSDOTTER as done */
			ds_writeb(INFORMER_FLAGS + INFORMER_SWAFNILD, 2);
		} else if (state == 32) {
				/* check if the party already has this map piece */
				if (ds_readb(TREASURE_MAPS + 6) == 2) ds_writeb(TMAP_DOUBLE2, 1);
				/* get the map piece */
				ds_writeb(TREASURE_MAPS + 6, 1);
				/* each of the heros gets 10 AP */
				add_hero_ap_all(10);

				show_treasure_map();
		} else if (state == 37) {

			ds_writeb(CURRENT_TOWN, ds_readb(0x3461) == 1 ? ds_readb(SWAFNILD_TP1) :
						ds_readb(0x3461) == 2 ? ds_readb(SWAFNILD_TP2) : ds_readb(SWAFNILD_TP3));

			switch (ds_readbs(CURRENT_TOWN)) {
				case 39: ds_writew(0x2d83, 22); ds_writew(0x2d85,  8); break;
				case 33: ds_writew(0x2d83,  4); ds_writew(0x2d85,  3); break;
				case 37: ds_writew(0x2d83,  9); ds_writew(0x2d85, 10); break;
				case 38: ds_writew(0x2d83, 11); ds_writew(0x2d85, 11); break;
				case 34: ds_writew(0x2d83,  4); ds_writew(0x2d85, 12); break;
				case 36: ds_writew(0x2d83,  6); ds_writew(0x2d85,  6); break;
			}

			ds_writeb(0x2d9f, 0);
			ds_writeb(0x7c9c, 1);

			timewarp_until_midnight();
		} else if (state == 42) {
			ds_writeb(0x3461, 1);
		} else if (state == 43) {
			ds_writeb(0x3461, 2);
		} else if (state == 44) {
			ds_writeb(0x3461, 3);
		}

	} else if (informer == 1) {
		/* EINHORN / UNICORN (second meeting) */

		if (state == 3) {
			signed short i;

			/* search a map part the party does not have */
			for (i = 0; i < 9; i++) {
				if (!ds_readbs(TREASURE_MAPS + i)) {
					ds_writeb(TREASURE_MAPS + i, 2);
					i = 99;
					break;
				}
			}

			/* check if the group had all map parts */
			if (i != 99) {
				ds_writeb(TMAP_DOUBLE1, 1);
			}

			show_treasure_map();
		}
	}
}

/**
 * \brief counts the collected parts of the treasure map
 * \return the number of map parts
 */
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
signed short has_intro_letter(void)
{
	/* check for the introduction letter / Empfehlungsschreiben */
	return (get_first_hero_with_item(0xeb) != -1) ? 1 : 0;
}

#if !defined(__BORLANDC__)
}
#endif

