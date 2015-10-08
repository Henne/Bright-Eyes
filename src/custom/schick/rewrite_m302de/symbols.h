/*
 *	symbol names for version v3.02_de
 *
 *	Here are the mappings from adresses in the datasegment
 *	to symbolic names of global variables.
 *	This will only work with the version v302de.
 */

#if !defined SYMBOLS_H
#define SYMBOLS_H

#define POISON_POTIONS	(0x08d3)	/* s16 array with item IDs of poisons */
#define TWO_FIELDED_SPRITE_ID (0x25f9)	/* char[5] array */
#define EMS_ENABLED	(0x26ab)
#define CHECK_DISEASE	(0x26b9)	/* ?16 {0, 1} */
#define CHECK_POISON	(0x26bb)	/* ?16 {0, 1} */
#define TIMERS_DISABLED	(0x2c99)
#define CURRENT_ANI	(0x2ccf)
#define IN_FIGHT	(0x2cd5)
#define FIGHT_ROUND	(0x2cd7)
#define SKILLED_HERO_POS (0x2cdb)	/* s16 {-1, 0..6} */
#define MR_MODIFICATORS	(0x2d27)	/* signed char[13] */
#define CURRENT_GROUP	(0x2d35)
#define DIRECTION	(0x2d3d)
#define X_TARGET	(0x2d44)
#define Y_TARGET	(0x2d46)
#define LOCATION	(0x2d60)
#define CURRENT_TOWN	(0x2d67)	/* signed char */
#define DUNGEON_INDEX	(0x2d6e)	/* signed char */
#define DUNGEON_LEVEL	(0x2d75)
#define DAY_TIMER	(0x2dbb)
#define DAY_OF_WEEK	(0x2dbf)
#define DAY_OF_MONTH	(0x2dc0)
#define MONTH		(0x2dc1)
#define YEAR		(0x2dc2)
#define SPECIAL_DAY	(0x2dc3)
#define INGAME_TIMERS	(0x2dc4)	/* signed long[26] */
#define GODS_ESTIMATION (0x314e)	/* signed long[15] */
#define DEATHTRAP_STEPS	(0x3310)	/* signed short */
#define DEATHTRAP	(0x3312)	/* signed short */
#define TREASURE_MAPS	(0x3332)	/* signed char[9] */
#define MIN_MAP_PARTS	(0x3340)	/* signed short {0} */
#define SUBVENTION	(0x3342)	/* unsigned char {0-4} */
#define OTTA_WINDRIDERS	(0x3344)	/* unsigned short */
#define OTTA_STORMCHILDREN	(0x3346)	/* unsigned short */
#define OTTA_GARALDSSON	(0x3348)	/* unsigned short */
#define HEARD_ANNOUNCE	(0x334a)	/* signed short: 0 = not heard, 1 = heard, 3 = spoke with hetman */
#define GOT_MAIN_QUEST	(0x334c)	/* signed short {0,1} */
#define QUESTED_MONTHS	(0x334e)
#define MONTHLY_CREDIT	(0x3350)	/* signed short */
#define GOT_LETTER_JAD	(0x3352)	/* signed short {0,1} */
#define VISITED_MILLER	(0x3354)	/* signed short {0,1} */
#define QUEST_UGDALF	(0x3356)	/* signed short {0, 1, 3, 4} */
#define ACADEMY_DAILY_CURSE	(0x3358)	/* signed short {0,1} */
#define ACADEMY_DAILY_IDENT	(0x335a)	/* signed short {0,1} */
#define BANK_DEPOSIT	(0x335c)	/* signed short */
#define DAYS_TO_CENS	(0x335e)	/* signed short */
#define DEBT_DAYS	(0x3360)	/* signed short */
#define IN_ACADEMY	(0x3362)	/* s16 */
#define INFORMER_JURGE	(0x3364)	/* unsigned char and unsigned char[15], {0,1,2} */
#define INFORMER_HJORE	(0x3365)	/* unsigned char {0,1,2} */
#define INFORMER_YASMA	(0x3366)	/* unsigned char {0,1,2} */
#define INFORMER_UMBRIK	(0x3367)	/* unsigned char {0,1,2} */
#define INFORMER_ISLEIF	(0x3368)	/* unsigned char {0,1,2} */
#define INFORMER_RAGNA	(0x3369)	/* unsigned char {0,1,2} */
#define INFORMER_BEORN	(0x336a)	/* unsigned char {0,1,2} */
#define INFORMER_ASGRIMM	(0x336b)	/* unsigned char {0,1,2} */
#define INFORMER_ELIANE	(0x336c)	/* unsigned char {0,1,2} */
#define INFORMER_OLVIR	(0x336d)	/* unsigned char {0,2} */
#define INFORMER_SWAFNILD	(0x336e)	/* unsigned char {0,1,2} */
#define INFORMER_TREBORN	(0x336f)	/* unsigned char {0,1} */
#define INFORMER_UNICORN	(0x3370)	/* unsigned char {0,2} */
#define INFORMER_ALGRID	(0x3371)	/* unsigned char {0,1,2} */
#define INFORMER_TIOMAR	(0x3372)	/* unsigned char {0,1,2} */
#define GOT_LETTER_HET	(0x344a)	/* unsigned char {0,1} */
#define QUEST_GORAH	(0x344d)	/* unsigned char {0,1 } */
#define QUEST_NAMELESS_GOT	(0x3455)	/* unsigned char {0, 1} */
#define SWAFNILD_TP1	(0x345d)	/* unsigned char */
#define SWAFNILD_TP2	(0x345e)	/* unsigned char */
#define SWAFNILD_TP3	(0x345f)	/* unsigned char */
#define SWAFNILD_TP4	(0x3460)	/* unsigned char */
#define TREBORN_DATE	(0x3463)	/* unsigned char {0,1} */
#define UNICORN_HERO_POS	(0x3464) /* unsigned char {0-6} */
#define UNICORN_GET_MAP	(0x3465)
#define UNICORN_TIMER	(0x3466)
#define UNICORN_HERO_PTR	(0x346a)	/* RealPt */
#define NPC_MONTHS	(0x3470)
#define QUEST_DEADSHIP	(0x35f0)	/* unsigned char {0,1} */
#define QUEST_NAMELESS_DONE	(0x35fa)	/* unsigned char {0, 1} */
#define CURRENT_INFORMER	(0x3612)	/* unsigned char {0,1,6,7,8,14} */
#define DUNGEON_LIGHT	(0x3613)	/* unsigned char (0,1,2) */
#define TRAVELING	(0x3614)
#define SPHERE_TIMER	(0x3c97)
#define SPHERE_ACTIVE	(0x3c98)
#define MAGE_POISON	(0x3cb6)
#define FIND_HYGGELIK	(0x3ddc)	/* unsigned char {0, 1} you are now able to find Hyggelik */
#define RANDOM_TLK_HERO	(0x3e18)	/* RealPt */
#define RUIN_HERO	(0x3e1c)	/* RealPt */
#define NEED_LETTER	(0x3f73)	/* unsigned char {0, 1} */
#define GREMOB_INVITED	(0x3f74)	/* unsigned char {0, 1} */
#define HARLOT_DATE	(0x3f75)	/* unsigned char {0, 1} */
#define DRUG_TIMER	(0x3f76)	/* unsigned char {0, 1, ..., 10} */
#define PTEMPLE_CLOSED	(0x3f77)	/* unsigned char {0, 1} */
#define ALRIK_DERONDAN	(0x3f78)	/* unsigned char {0, 1} */
#define INGERIMM_SACRIFICE	(0x3f9f)	/* unsigned char {0, 1} */
#define INGERIMM_HINT	(0x3fa0)	/* unsigned char {0, 1} */
#define FOOD_MESSAGE	(0x4219)
#define CITYINDEX	(0x4222)
#define TYPEINDEX	(0x4224)
#define ARSENAL_MONEY	(0x43a3)	/* signed short {-1, 0 - 60 } */
#define ANNOUNCE_DAY	(0x43a5)	/* signed char, UNUSED */
#define CHECK_PARTY	(0x4495)
#define FOOD_MOD	(0x4496)
#define BANK_HELLER	(0x4646)	/* signed short */
#define CONSUME_QUIET	(0x4648)
#define LEVEL_AP_TAB	(0x464e)	/* Bit32s[21] */
#define DELAY_FACTOR	(0x4b66)
#define EMM_SIG		(0x4ba2)	/* char[8] */
#define TMAP_X		(0x4c12)	/* signed short[10] */
#define TMAP_Y		(0x4c26)	/* signed short[10] */
#define FNAMES		(0x4c8c)
#define INFORMER_TAB	(0x5ed6)	/* struct informer[15] */
#define AUTOFIGHT_MAGIC (0x5f31)	/* ?8 */
#define AF_SPELL_LIST	(0x5fac)	/* char[11] */
#define FORCE_WEAPONS	(0x615c)	/* signed short[9] terminated with -1 */
#define KNIVE_WEAPONS	(0x616e)	/* signed short[5] terminated with -1 */
#define SEG047_INIT1	(0x64a3)	/* signed short[7] filled with zeroes */
#define SEG047_INIT2	(0x64b1)	/* signed short[7] filled with zeroes */
#define SEG047_INIT3	(0x64bf)	/* signed short[7] filled with zeroes */
#define TRAVEL_BY_SHIP	(0x7070)	/* ?8 0 = on land, 1 = at the ship */
#define C_EVENT_ACTIVE	(0x7c42)	/* ?8 0 = default, 1 = city event active */
#define TMAP_DOUBLE1	(0x7c9a)	/* ?8 1 = unicorn brought a piece you already have */
#define TMAP_DOUBLE2	(0x7c9b)	/* ?8 1 = you got a piece you already have from the unicorn */
#define MAX_GOSSIP	(0x7c9d)	/* s8[52], maximum of different gossips per town */
#define TOWN_POSITIONS	(0xa43b)	/* signed short[53][2] */
#define LIGHT_TYPE	(0xaee8)	/* ?16 0 = none, 1 = torch, 2 = lantern */
#define CD_DRIVE_NR	(0xbc52)	/* ?16 */
#define SPLASH_AE	(0xbcc7)
#define SPLASH_LE	(0xbccb)
#define SCENARIO_BUF	(0xbd2c)
#define PTR_FIGHT_LST	(0xbd28)	/* pointer to FIGHT.LST */
#define HEROS		(0xbd34)
#define TEXTBOX_WIDTH	(0xbffd)	/* signed short */
#define GAME_MODE	(0xc003)	/* signed short {-1 = Input error, 1 = Beginner, 2 = Advanced } */
#define DIALOG_TEXT	(0xc3b1)
#define CITY_LTX	(0xc3ad)
#define TEXT_LTX	(0xc3b5)
#define ACTION		(0xc3d9)	/* ? short */
#define ANI_MAIN_PTR	(0xce35)
#define POPUP		(0xd2ad)	/* RealPt */
#define BUF_FONT6	(0xd2c1)	/* RealPt */
#define TEXTCOLOR	(0xd2c5)
#define TEXTCOLOR_BG	(0xd2c7)
#define TEXTCOLOR_FG	(0xd2c9)
#define BUF_ICON	(0xd2e7)	/* RealPt */
#define DTP2		(0xd2f3)
#define ICON		(0xd2f7)	/* RealPt */
#define TOWNPAL_BUF	(0xd321)	/* RealPt */
#define ENEMY_SHEETS	(0xd34b)	/* struct enemy[20] */
#define CHESSBOARD	(0xd852)	/* RealPt */
#define NR_OF_ENEMIES	(0xd872)	/* ? short */
#define FIG_LIST_HEAD	(0xe108)	/* RealPtr to a list */
#define WALLCLOCK_Y	(0xe10f)
#define WALLCLOCK_X	(0xe111)
#define MEM_SLOTS_MON	(0xe115)
#define MEM_SLOTS_WFIG	(0xe119)
#define MEM_SLOTS_MFIG	(0xe11d)
#define MONNAMES	(0xe129)
#define ITEMSDAT	(0xe22b)
#define ITEMSNAME	(0xe22f)
#define DELAY_TIMER	(0xe2d0)
#define DIALOG_INFORMER	(0xe30c)	/* ?16 */
#define DIALOG_DONE	(0xe310)	/* ?16 {0, 1} */
#define DIALOG_STATE	(0xe312)	/* ?16 */
#define TLK_ID		(0xe314)	/* ?16 */
#define CURRENT_FIG_NR	(0xe316)
#define AUTOFIGHT	(0xe318)	/* ?16 */
#define BUFFER_WEAPANIDAT	(0xe374)	/* pointer to WEAPANI.DAT */
#define BUFFER_ANIDAT	(0xe378)	/* pointer to ANI.DAT buffer */
#define BOOKED_INN_DAYS	(0xe3f0)	/* signed char */
#define SLEEP_QUALITY	(0xe3f1)	/* signed char */
#define COMBO_MODE	(0xe42c)	/* signed short {0,1,2} */
#define GRAMMAR_STRING	(0xe4e3)	/* char grammar_string[201] */
#define SPELLTARGET_E	(0xe5b4)	/* Pointer to enemy */
#define SPELLTARGET	(0xe5b8)
#define SPELLUSER	(0xe5bc)
#define SPELLUSER_E	(0xe5c0)
#define USED_ITEM_DESC	(0xe5c6)	/* pointer to the item description */
#define USED_ITEM_ID	(0xe5ca)	/* s16 used_item ID */
#define USED_ITEM_POS	(0xe5cc)	/* s16 used_item position */
#define ITEMUSER	(0xe5ce)	/* pointer to hero */
#define BUFFERSIZE	(0xe5dc)	/* ?32 size of the global buffer */
#define LARGE_BUF	(0xe5e4)	/* s8 {0,1} */

#endif
