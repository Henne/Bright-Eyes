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
#define ATTACK_ITEMS	(0x091f)	/* signed short[3] = { ITEM_MIASTHMATIKUM (0xee), ITEM_HYLAILIC_FIRE (0xef), -1 } */
#define SKILLS_EXTRA	(0x0ffe)	/* (struct { signed char attrib1, attrib2, attrib3, max_inc; })[52] */
#define SKILLS_INDEX	(0x10ce)	/* (struct { signed char first, length; })[7] = { {0,9}, {9,10}, {19,7}, {26,6}, {32,9}, {41,9}, {50,2}, } */
#define TWO_FIELDED_SPRITE_ID	(0x25f9)	/* char[5] array */
#define FOOD_MESSAGE_SHOWN	(0x26a4)	/* signed char[6] */
#define EMS_ENABLED	(0x26ab)
#define FIG_INITIATIVE	(0x26ac)	/* signed char, 0 = random, 1 = enemies, 2 = heroes (attack first) */
#define FIG_MSG_COUNTER	(0x26ad)	/* signed short */
#define FIG_CHAR_PIC	(0x26b3)	/* unsigned short */
#define FIG_ENEMY_PIC	(0x26b5)	/* unsigned short */
#define GUI_ENTERING_SAVEGAME	(0x26b7)	/* ?16 {0, 1} */
#define CHECK_DISEASE	(0x26b9)	/* ?16 {0, 1} */
#define CHECK_POISON	(0x26bb)	/* ?16 {0, 1} */
#define TEXT_FILE_INDEX	(0x26bd)	/* unsigned short */
#define BUF1_FILE_INDEX	(0x26bf)	/* signed short, index of file currently stored in buffer1 */
#define FIG_DISCARD	(0x26c1)	/* ?16 {0, 1}, whether to discard the fight data after the fight */
#define DEFAULT_MOUSE_CURSOR	(0x2848)	/* unsigned char[64] */
#define ACTION_TABLE_MENU	(0x29cc)	/* (struct { signed short x1, x2, y1, y2; unsigned short action; })[2] */
#define ACTION_TABLE_PRIMARY	(0x29e0) /* RealPt */
#define ACTION_TABLE_SECONDARY	(0x29e4) /* RealPt */
#define ACTION_TABLE_PLAYMASK	(0x29e8)	/* (struct { signed short x1, x2, y1, y2; unsigned short action; })[24] */
#define ACTION_TABLE_STATUS	(0x2ad8)	/* (struct { signed short x1, x2, y1, y2; unsigned short action; })[30] */
#define ACTION_TABLE_MERCHANT	(0x2c04)	/* (struct { signed short x1, x2, y1, y2; unsigned short action; })[2] */
#define CHAR_STATUS_BARS	(0x2c18)	/* (struct { signed short LE_ORIG, LE, AE_ORIG, AE; })[7] */
#define DISEASE_PRICES	(0x2c50)	/* signed short[8] */
#define DISEASE_DELAYS	(0x2c60)	/* signed short[8] */
#define POISON_PRICES	(0x2c70)	/* signed short[8] */
#define POISON_DELAYS	(0x2c80)	/* signed short[8] */
#define TIMERS_DISABLED	(0x2c99)
#define STATUS_PAGE_MODE	(0x2c9b)	/* signed short, one of 1,2,3,4,5 */
#define STATUS_PAGE_HERO	(0x2c9d)	/* signed short */
#define STATUS_PAGE_HUNGER	(0x2c9f)	/* signed char */
#define STATUS_PAGE_THIRST	(0x2ca0)	/* signed char */
#define CURRENT_ANI	(0x2ccf)
#define FIG_FIGURE1	(0x2cd1)	/* signed short */
#define FIG_FIGURE2	(0x2cd3)	/* signed short */
#define IN_FIGHT	(0x2cd5)
#define FIGHT_ROUND	(0x2cd7)
#define SKILLED_HERO_POS	(0x2cdb)	/* s16 {-1, 0..6} */
#define MR_MODIFICATORS	(0x2d27)	/* signed char[13] */
#define CURRENT_GROUP	(0x2d35)
#define GROUP_MEMBER_COUNTS	(0x2d36)	/* signed char[6], members per group */
#define TOTAL_HERO_COUNTER	(0x2d3c)	/* signed char */
#define DIRECTION	(0x2d3d)
#define GROUPS_DIRECTION	(0x2d3e)	/* unsigned char[6] */
#define X_TARGET	(0x2d44)
#define Y_TARGET	(0x2d46)
#define GROUPS_X_TARGET	(0x2d48)	/* unsigned short[6] */
#define GROUPS_Y_TARGET	(0x2d54)	/* unsigned short[6] */
#define LOCATION	(0x2d60)	/* signed char */
#define GROUPS_LOCATION	(0x2d61)	/* signed char[6] */
#define CURRENT_TOWN	(0x2d67)	/* signed char */
#define GROUPS_TOWN	(0x2d68)	/* signed char[6] */
#define DUNGEON_INDEX	(0x2d6e)	/* signed char */
#define GROUPS_DNG_INDEX	(0x2d6f)	/* signed char[6] */
#define DUNGEON_LEVEL	(0x2d75)
#define GROUPS_DNG_LEVEL	(0x2d76)	/* signed char[6] */
#define DAY_TIMER	(0x2dbb)
#define DAY_OF_WEEK	(0x2dbf)
#define DAY_OF_MONTH	(0x2dc0)
#define MONTH	(0x2dc1)
#define YEAR	(0x2dc2)
#define SPECIAL_DAY	(0x2dc3)
#define INGAME_TIMERS	(0x2dc4)	/* signed long[26] */
#define MODIFICATION_TIMERS	(0x2e2c)	/* (struct { long, short, char, char })[?] */
#define GODS_ESTIMATION	(0x314e)	/* signed long[15] */
#define SMITH_REPAIRITEMS	(0x31e2)	/* (struct { short, long })[50] */
#define DEATHTRAP_STEPS	(0x3310)	/* signed short */
#define DEATHTRAP	(0x3312)	/* signed short */
#define WEATHER1	(0x331b)	/* signed short, random(6) */
#define WEATHER2	(0x331d)	/* signed short, random(7) */
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
#define UNICORN_HERO_POS	(0x3464)	/* unsigned char {0-6} */
#define UNICORN_GET_MAP	(0x3465)
#define UNICORN_TIMER	(0x3466)
#define UNICORN_HERO_PTR	(0x346a)	/* RealPt */
#define NPC_MONTHS	(0x3470)	/* signed short */
#define QUEST_DEADSHIP	(0x35f0)	/* unsigned char {0,1} */
#define QUEST_DEADSHIP_DONE	(0x35f2)	/* unsigned char {0,1} */
#define QUEST_NAMELESS_DONE	(0x35fa)	/* unsigned char {0, 1} */
#define CURRENT_INFORMER	(0x3612)	/* unsigned char {0,1,6,7,8,14} */
#define DUNGEON_LIGHT	(0x3613)	/* unsigned char (0,1,2) */
#define TRAVELING	(0x3614)
#define INFORMER_ARRAY	(0x3618)	/* (struct of size 38)[10] */
#define DIALOG_STATES	(0x3794)	/* (struct of size 8)[160?] */
#define SPHERE_TIMER	(0x3c97)
#define SPHERE_ACTIVE	(0x3c98)
#define MAGE_POISON	(0x3cb6)
#define FIND_HYGGELIK	(0x3ddc)	/* unsigned char {0, 1} you are now able to find Hyggelik */
#define TATZELWURM	(0x3de5)	/* unsigned char {0, 1, 2} */
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

#define UNCONSCIOUS_MESSAGE	(0x4212)	/* unsigned char[7] */
#define FOOD_MESSAGE	(0x4219)	/* unsigned char[7] */
#define CITYINDEX	(0x4222)
#define TYPEINDEX	(0x4224)
#define TRV_RETURN	(0x4336)	/* signed short {-1, 0, 1, 2} + ? */
#define KNOWN_MONSTERS	(0x4351)	/* signed short[82] */
#define ARSENAL_MONEY	(0x43a3)	/* signed short {-1, 0 - 60 } */
#define ANNOUNCE_DAY	(0x43a5)	/* signed char, UNUSED */
#define KNOWN_PERSONS	(0x43a6)	/* signed short[14] */
#define DIARY_ENTRIES	(0x43b4)	/* (struct { short day, month, year, town; })[23] */
#define DIARY_ENTRY_COUNTER	(0x43ba)	/* signed short */
#define MUSIC_ENABLED	(0x4476)	/* unsigned char {0,1} */
#define SND_EFFECTS_ENABLED	(0x4477)	/* unsigned char {0,1} */
#define MUSIC_CURRENT_TRACK	(0x447a)	/* signed short */
#define PAUSE_STRING	(0x448a)	/* char[10] = "P A U S E" */
#define CHECK_PARTY	(0x4495)
#define FOOD_MOD	(0x4496)
#define SPECIAL_DAYS	(0x45b9)	/* (struct { char month, day, id; })[32] */
#define MONTHS_SPRING	(0x463a)	/* unsigned short[4] */
#define MONTHS_WINTER	(0x463e)	/* unsigned short[4] */
#define MONTHS_SUMMER	(0x4642)	/* unsigned short[4] */
#define BANK_HELLER	(0x4646)	/* signed short */
#define CONSUME_QUIET	(0x4648)
#define LEVEL_AP_TAB	(0x464e)	/* Bit32s[21] */
#define DSA_VERSION_STRING	(0x46e0)	/* char[12] = "DSA VERSION" */
#define GAME_VERSION	(0x46ec)	/* char[6] = "V3.02" */
#define VERSION_TOKEN1	(0x46f8)	/* unsigned char = 3 */
#define VERSION_TOKEN2	(0x46f9)	/* unsigned char = 1 */
#define VERSION_TOKEN3	(0x46fa)	/* unsigned char = 1 */
#define VERSION_TOKEN4	(0x46fb)	/* unsigned char = 1 */
#define QUESTIONS_HANDBOOK	(0x46fc)	/* (struct of size 19)[10] */
#define QUESTIONS_MAP	(0x47ba)	/* (struct of size 3)[10] */
#define SND_MENU_QUESTION	(0x47d8)	/* char[17] = "BITTE AUSW\x8eHLEN:" */
#define SND_MENU_RADIO1	(0x47e9)	/* char[12] = "KEINE MUSIK" */
#define SND_MENU_RADIO2	(0x47f5)	/* char[10] = "NUR MUSIK" */
#define SND_MENU_RADIO3	(0x47ff)	/* char[12] = "NUR EFFEKTE" */
#define SND_MENU_RADIO4	(0x480b)	/* char[18] = "MUSIK UND EFFEKTE" */
#define FNAME_SOUND_CFG	(0x481d)	/* char[10] = "SOUND.CFG" */
#define FNAME_SOUND_ADV2	(0x4827)	/* char[10] = "SOUND.ADV" */
#define FNAME_DIGI_ADV	(0x4831)	/* char[9] = "DIGI.ADV" */
#define SND_TXT_DISABLED_MEM	(0x483a)	/* char[51] = "SOUND EFFEKTE ABGESCHALTET - NICHT GENUG SPEICHER!" */
#define SND_TXT_HW_NOT_FOUND	(0x486d)	/* char[31] = "SOUND HARDWARE NICHT GEFUNDEN!" */
#define FNAME_SOUND_ADV	(0x488c)	/* char[10] = "SOUND.ADV" */
#define SND_TXT_HW_NOT_FOUND2	(0x4896)	/* char[31] == SND_TXT_HW_NOT_FOUND */
#define FNAME_SCHICK_DAT	(0x48ca)	/* char[11] = "SCHICK.DAT" */
#define SND_TXT_DISABLED_MEM2	(0x48d5)	/* char[43] = "MUSIK ABGESCHALTET - NICHT GENUG SPEICHER!" */
#define DIARY_STRING1	(0x4900)	/* char[14] = "%2d-~%-8s~%s." */
#define DIARY_STRING2	(0x490e)	/* char[17] = "%2d.~%-8s~IN %s." */
#define DIARY_STRING3	(0x491f)	/* char[27] = "%2d.~%-8s~BESUCHEN VON %s." */
#define DIARY_STRING4	(0x493a)	/* char[6] = "~~%s." */
#define DIARY_STRING5	(0x4940)	/* char[9] = "~~IN %s." */
#define DIARY_STRING6	(0x4949)	/* char[19] = "~~ABENTEUER IN %s." */
#define DIARY_STRING7	(0x495c)	/* char[22] = "~~RUMH\x8eNGEN IN %s." */
#define NOWAY_STRING	(0x4a68)	/* char[41] = "IN DIESE RICHTUNG GEHT ES NICHT WEITER." */
#define ANI_BUSY	(0x4a90)	/* unsigned short {0,1} */
#define STATUS_BAR_COLORS	(0x4a94)	/* 4 chars: le_color, 0, ae_color, 0 */
#define STATUS_PAGE_HUNGER_MAX_COUNTER	(0x4a9a)	/* signed char */
#define STATUS_PAGE_HUNGER_MAX_COLOR	(0x4a9b)	/* signed char */
#define STATUS_PAGE_THIRST_MAX_COUNTER	(0x4a9c)	/* signed char */
#define STATUS_PAGE_THIRST_MAX_COLOR	(0x4a9d)	/* signed char */
#define WALLCLOCK_POS	(0x4a9e)	/* signed short, position of sun/moon */
#define WALLCLOCK_POS_Y	(0x4aa0)	/* signed char[81] */
#define WALLCLOCK_PALETTE_DAY	(0x4af1)	/* (struct { unsigned char r,g,b; })[3] */
#define WALLCLOCK_PALETTE_NIGHT	(0x4afa)	/* (struct { unsigned char r,g,b; })[3] */
#define COLOR_PAL_BLACK	(0x4b03)	/* char[3] = {0x3f,0x3f,0x3f} */
#define DELAY_FACTOR	(0x4b66)
#define STR_TEMP_XX_PTR	(0x4b68)	/* unsigned long == RealMake(datseg, 0x4b95) */
#define FIG_STAR_COLORS	(0x4b6b)	/* signed char[13] */
#define FIG_STAR_COUNTER	(0x4b78)	/* signed char */
#define FIG_STAR_TIMER	(0x4b79)	/* signed short */
#define FIG_MSG_DTPS	(0x4b7a)	/* signed short[13] */
#define FIG_STAR_LAST_COUNT	(0x4b7b)	/* unsigned char */
#define FIG_STAR_PRINTED	(0x4b94)	/* signed char {0,1} */
#define STR_TEMP_XX	(0x4b95)	/* char[8] = "TEMP\XX" */
#define FIGHT_FIGS_INDEX	(0x4b9e)	/* signed short */
#define RANDOM_SCHICK_SEED	(0x4ba0)	/* unsigned short */
#define EMM_SIG	(0x4ba2)	/* char[8] */
#define ACTION_TABLE_OPTIONS	(0x4bae)	/* (struct { signed short x1, x2, y1, y2; unsigned short action; })[10] */
#define TMAP_X	(0x4c12)	/* signed short[10] */
#define TMAP_Y	(0x4c26)	/* signed short[10] */
#define LOCATION_HANDLERS	(0x4c3b)	/* (void (*)(void))[19] */
#define FNAMES	(0x4c8c)
#define CHR_FILE_SUFFIX	(0x5e3e)	/* char[5] = ".CHR" */
#define SAVEGAME_SUFFIX	(0x5e43)	/* char[5] = ".GAM" */
#define ALL_FILES_WILDCARD	(0x5e48)	/* char[4] = "*.*" */
#define ALL_CHR_WILDCARD	(0x5e4c)	/* char[6] = "*.CHR" */
#define EMPTY_STRING1	(0x5e52)	/* char[1] = "" */
#define EMPTY_STRING2	(0x5e53)	/* char[1] = "" */
#define SAVEGAME_SUFFIX2	(0x5e54)	/* char[5] = ".gam" */
#define SAVEGAME_SUFFIX3	(0x5e59)	/* char[5] = ".gam" */
#define ALL_CHR_WILDCARD2	(0x5e5e)	/* char[6] = "*.CHR" */
#define ALL_CHR_WILDCARD3	(0x5e64)	/* char[6] = "*.CHR" */
#define PP20_BUFFERS	(0x5e6a)	/* RealPt[9] */
#define PP20_BUFFER_LENGTHS	(0x5e8e)	/* unsigned long[9] */
#define CURRENT_FIGHT_NR	(0x5eb2)	/* unsigned short */
#define INFORMER_TAB	(0x5ed6)	/* struct informer[15] */
#define FIG_DROPPED_COUNTER	(0x5f12)	/* signed short */
#define MAX_ENEMIES	(0x5f16)	/* signed short: an upper bound for the number of enemies */
#define STR_ERROR_ON_OBJ_MALLOC	(0x5f18)	/* char[23] = "ERROR ON OBJECT MALLOC" */
#define AUTOFIGHT_MAGIC	(0x5f31)	/* ?8 */
#define RED_STRING1	(0x5f32)	/* char[5] = "\xf1%s\xf0" */
#define RED_STRING2	(0x5f37)	/* char[5] = "\xf1%s\xf0" */
#define SPACE_SEPARATED_STRINGS	(0x5f3c)	/* char[6] = "%s %s" */
#define EMPTY_STRING3	(0x5f42)	/* char[1] = "" */
#define EMPTY_STRING4	(0x5f43)	/* char[1] = "" */
#define EMPTY_STRING5	(0x5f44)	/* char[1] = "" */
#define STRING_14SPACES	(0x5f9c)	/* char[15] = "              " */
#define AF_SPELL_LIST	(0x5fac)	/* char[11] */
#define FIG_LIST_LENGTH	(0x605e)	/* unsigned short */
#define FORCE_WEAPONS	(0x615c)	/* signed short[9] terminated with -1 */
#define KNIVE_WEAPONS	(0x616e)	/* signed short[5] terminated with -1 */
#define STRING_CASTS_SPELL	(0x6180)	/* char[14] = "%s ZAUBERT %s" */
#define EMPTY_STRING6	(0x64a0)	/* char[1] = "" */
#define EMPTY_STRING7	(0x64a1)	/* char[1] = "" */
#define SEG047_INIT1	(0x64a3)	/* signed short[7] filled with zeroes */
#define SEG047_INIT2	(0x64b1)	/* signed short[7] filled with zeroes */
#define SEG047_INIT3	(0x64bf)	/* signed short[7] filled with zeroes */
#define EXTRASPACE_SEPARATED_STRINGS	(0x6534)	/* char[7] = "%s %s " */
#define EMPTY_STRING8	(0x653b)	/* char[1] = "" */
#define EXTRASPACE_SEPARATED_STRINGS2	(0x653c)	/* char[7] = "%s %s " */
#define EMPTY_STRING9	(0x6543)	/* char[1] = "" */
#define CAMPFIGHTS	(0x6694)	/* signed short[4] */
#define GATHER_HERBS_STR_FOUND	(0x66d1)	/* char[6] = "%d %s" */
#define GATHER_HERBS_STR_COMMA	(0x66d7)	/* char[3] = ", " */
#define GATHER_HERBS_STR_AND	(0x66da)	/* char[6] = " UND " */
#define GATHER_HERBS_STR_DOT	(0x66e0)	/* char[2] = "." */
#define DCAMPFIGHTS	(0x66e2)	/* signed short[4] */
#define BUY_SCREEN_STR_MONEY_H	(0x6bc8)	/* char[5] = "H %d" */
#define BUY_SCREEN_STR_MONEY_S	(0x6bcd)	/* char[5] = "S %d" */
#define BUY_SCREEN_STR_MONEY_D	(0x6bd2)	/* char[5] = "D %d" */
#define BUY_SCREEN_STR_COMMA_SPACE	(0x6bed)	/* char[3] = ", " */
#define SELL_SCREEN_STR_MONEY_H	(0x6bf0)	/* char[5] = "H %d" */
#define SELL_SCREEN_STR_MONEY_S	(0x6bf5)	/* char[5] = "S %d" */
#define SELL_SCREEN_STR_MONEY_D	(0x6bfa)	/* char[5] = "D %d" */
#define SMITH_STR_MONEY_H	(0x6c64)	/* char[5] = "H %d" */
#define SMITH_STR_MONEY_S	(0x6c69)	/* char[5] = "S %d" */
#define SMITH_STR_MONEY_D	(0x6c6e)	/* char[5] = "D %d" */
#define STR_TEMP_FILE_WILDCARD	(0x6e72)	/* char[8] = "TEMP\%s" */
#define STR_NO_SAVE_IN_TEMPLE	(0x6e7a)	/* char[42] = "IN DIESEM TEMPEL KEIN SPEICHERN M\x99GLICH!" */
#define TEMPLE_MIRACLE_BONUS	(0x6ea4)	/* signed char[15] = {0, 2, 15, 10, 20, 5, 10, 1, 15, 3, 15, 5, 10, 0} */
#define TEMPLE_MIRACLE_DICE	(0x6eb3)	/* signed char[15] = {0, 9, 9, 10, 17, 6, 10, 10, 18, 10, 19, 8, 15, 0, 10} */
#define TRAVEL_BY_SHIP	(0x7070)	/* ?8 0 = on land, 1 = at the ship */
#define SEA_TRAVEL_STR_T	(0x708d)	/* char[2] = "T" */
#define SEA_TRAVEL_STR_EN	(0x708f)	/* char[3] = "EN" */
#define SEA_TRAVEL_STR_COMMA	(0x7092)	/* char[3] = ", " */
#define SEA_TRAVEL_STR_HELLER	(0x7096)	/* char[10] = "%d^HELLER" */
#define SEA_TRAVEL_STR_NOTHING	(0x70a0)	/* char[7] = "NICHTS" */
#define CAN_MERGE_GROUP	(0x7c41)	/* signed char {-1,1} */
#define C_EVENT_ACTIVE	(0x7c42)	/* ?8 0 = default, 1 = city event active */
#define STR_BANK_DEPOSIT_TO_BIG	(0x7c44)	/* char[57] = "TUT MIR LEID, ICH AKZEPTIERE MAXIMAL 30000 SILBERM\x9aNZEN!" */
#define STR_OBVIOUSLY_CLOSED	(0x7c7d)	/* char[28] = "OFFENSICHTLICH GESCHLOSSEN!" */
#define TMAP_DOUBLE1	(0x7c9a)	/* ?8 1 = unicorn brought a piece you already have */
#define TMAP_DOUBLE2	(0x7c9b)	/* ?8 1 = you got a piece you already have from the unicorn */
#define MAX_GOSSIP	(0x7c9d)	/* s8[52], maximum of different gossips per town */
#define GOSSIP_STR_TRIPLE_WILDCARD	(0x7d06)	/* char[8] = "%s %s%s" */
#define DNG_STUB6_STR_COMMA	(0x92cc)	/* char[3] = ", " */
#define DNG_STUB6_STR_AND	(0x92cf)	/* char[6] = " UND " */
#define TOWN_POSITIONS	(0xa43b)	/* signed short[53][2] */
#define SEA_TRAVEL	(0xa842)	/* unsigned char {0,1} */
#define SPELL_SELECT_STR_KEYVAL	(0xac1a)	/* char[6] = "%s~%d" */
#define SPELL_SELECT_STR_KEY	(0xac20)	/* char[5] = "\xf2%s\xf0" */
#define SPELL_SELECT_STR_KEY_COLOR	(0xac25)	/* char[3] = "%s" */
#define SPELL_SELECT_STR_KEYVAL_COLOR	(0xac28)	/* char[8] = "\xf2%s~%d\xf0 */
#define SPELL_SELECT_ONES	(0xac30)	/* signed char[12] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } */
#define ANALUES_ITEMS	(0xac3c)	/* (struct { signed short item_id, barrier; signed char dtp; })[28] */
#define MONSTER_SPELL_COST	(0xaccc)	/* signed short */
#define SELECT_SKILL_LVLUP	(0xacce)	/* char[6] = "%s~%d" */
#define SELECT_SKILL_DEFAULTS	(0xacd4)	/* signed char[6] = {44, 45, 46, -1, -1, -1} */
#define ALCHEMY_RECIPES	(0xacda)	/* (struct of size 28)[12?] */
#define HERO_STARTUP_ITEMS	(0xae40)	/* (struct of size 8)[13] */
#define PREVENT_DROP_EQUIPPED_ITEMS	(0xae46)	/* unsigned char {0, 1} */
#define HERO_STARTUP_ITEMS_ALL	(0xaea8)	/* signed short[4] */
#define USE_SPECIAL_ITEM_HANDLERS	(0xaeb0)	/* signed short[4] */
#define LIGHT_TYPE	(0xaee8)	/* ?16 0 = none, 1 = torch, 2 = lantern */
#define TRAVEL_EVENT_HANDLERS	(0xaeea)	/* function pointers, long[146] */
#define TRAVEL_EVENT_ACTIVE	(0xb132)	/* signed char {0,1} */
#define EMPTY_STRING10	(0xb13b)	/* char[1] = "" */
#define EMPTY_STRING11	(0xb13c)	/* char[1] = "" */
#define EMPTY_STRING12	(0xb13d)	/* char[1] = "" */
#define STR_BACKSLASH_TEMP	(0xb311)	/* char[6] = "\\TEMP" */
#define STR_NOT_ENOUGH_MEM	(0xb317)	/* char[124] = "Not enough memory!\x0a\"Realms of Arkania - Blade of Destiny\" needs %ld Byte more memory!\x0aPlease uninstall resident programs!" */
#define STR_DRIVE_X	(0xb393)	/* char[4] = "X:\" */
#define STR_TEMP_DIR_FAIL	(0xb397)	/* char[280] = "\xf2!!!!! ACHTUNG !!!!!\xf0\x40\x40DAS SCHWARZE AUGE\x40KANN KEIN TEMPOR\x8eRES VERZEICHNIS ANLEGEN UND IST DAHER NICHT LAUFF\x8eHIG. WAHRSCHEINLICH VERSUCHEN SIE, DAS SPIEL AUF EINEM\xf2 SCHREIBGESCH\x9aTZTEN LAUFWERK\xf0\x40ZU STARTEN. BITTE INSTALLIEREN SIE DAS SPIEL AUF EINE FESTPLATTE.\x40\x40-TASTE ZUM BEENDEN-" */
#define ALL_FILES_WILDCARD2	(0xb4af)	/* char[4] = "*.*" */
#define ALL_CHR_WILDCARD4	(0xb4b3)	/* char[6] = "*.CHR" */
#define ALL_FILES_WILDCARD3	(0xb4b9)	/* char[4] = "*.*" */
#define STR_GEN_EXE	(0xb4bd)	/* char[8] = "gen.exe" */
#define STR_GEN_EXE2	(0xb4c5)	/* char[8] = "gen.exe" */
#define STR_GEN_B	(0xb4cd)	/* char[2] = "b" */
#define STR_GEN_A	(0xb4cf)	/* char[2] = "a" */
#define STR_GEN_N	(0xb4d1)	/* char[2] = "n" */
#define STR_GEN_1	(0xb4d3)	/* char[2] = "1" */
#define STR_GEN_GENERATION	(0xb4d5)	/* char[11] = "Generation" */
#define CD_AUDIO_POS_BAK	(0xbc38)	/* unsigned long */
#define CD_AUDIO_TOD_BAK	(0xbc3c)	/* unsigned long */
#define CD_AUDIO_TOD	(0xbc4a)	/* unsigned long */
#define CD_AUDIO_POS	(0xbc4e)	/* unsigned long */
#define CD_DRIVE_NR	(0xbc52)	/* ?16 */
#define SAMPLE_AD_LENGTH	(0xbc5a)	/* unsigned short */
#define SAMPLE_AD_IDX_ENTRY	(0xbc5c)	/* struct { signed char; signed char; unsigned long; } */
#define PLAYMASK_US	(0xbc62)	/* unsigned char, 1 = PLAYM_US, 0 = PLAYM_UK */
#define SPLASH_AE	(0xbcc7)
#define SPLASH_LE	(0xbccb)
#define ARCHIVE_FILE_OFFSET	(0xbcdf)	/* unsigned long, start offset in SCHICK.DAT */
#define ARCHIVE_FILE_REMAINING	(0xbce3)	/* unsigned long, flen - off */
#define ARCHIVE_FILE_LENGTH	(0xbce7)	/* unsigned long */
#define AIL_DIGI_DRIVER_BUF2	(0xbceb)	/* RealPt to buffer of size 5016 */
#define AIL_VOC_BUFFER	(0xbcef)	/* RealPt */
#define AIL_DIGI_DRIVER_BUF	(0xbcf3)	/* RealPt */
#define AIL_DIGI_DRIVER_DESCR	(0xbcf7)	/* RealPt */
#define AIL_DIGI_DRIVER_ID	(0xbcfb)	/* signed short */
#define SAMPLE_AD_HANDLE	(0xbd01)	/* signed short */
#define AIL_TIMBRE_CACHE_SIZE	(0xbd03)	/* unsigned short */
#define AIL_STATE_TABLE_SIZE	(0xbd05)	/* unsigned long */
#define AIL_MUSIC_DRIVER_BUF2	(0xbd09)	/* RealPt */
#define AIL_MIDI_BUFFER	(0xbd0d)	/* RealPt */
#define AIL_TIMBRE_CACHE	(0xbd11)	/* RealPt */
#define AIL_STATE_TABLE	(0xbd15)	/* RealPt */
#define AIL_MUSIC_DRIVER_BUF	(0xbd19)	/* RealPt */
#define AIL_MUSIC_DRIVER_DESCR	(0xbd1d)	/* RealPt */
#define AIL_SEQUENCE	(0xbd21)	/* signed short */
#define AIL_MUSIC_DRIVER_ID	(0xbd23)	/* signed short */
#define CURRENT_FIGHT	(0xbd28)	/* RealPt */
#define SCENARIO_BUF	(0xbd2c)
#define FIGHTOBJ_BUF	(0xbd30)	/* RealPt */
#define HEROS	(0xbd34)
#define RADIO_NAME_LIST	(0xbf95)	/* RealPt[15], used for items, heroes, spells, skills, recipes */
#define TEXTBOX_WIDTH	(0xbffd)	/* signed short */
#define TEXTBOX_POS_X	(0xbfff)	/* signed short, coordinate of upper left corner */
#define TEXTBOX_POS_Y	(0xc001)	/* signed short, coordinate of upper left corner */
#define GAME_MODE	(0xc003)	/* signed short {-1 = Input error, 1 = Beginner, 2 = Advanced } */
#define SELLITEMS	(0xc005)	/* RealPt, also used for repair items */
#define BUYITEMS	(0xc009)	/* RealPt, merchant's assortment */
#define BUFFER8_PTR	(0xc3a9)	/* RealPt to buffer of size 12008 */
#define CITY_LTX	(0xc3ad)
#define DIALOG_TEXT	(0xc3b1)
#define TEXT_LTX	(0xc3b5)
#define ACTION	(0xc3d9)	/* ? short */
#define BUFFER9_PTR	(0xc3db)	/* RealPt to buffer of size 180000 (or 203000 if LARGE_BUF), used for NVF */
#define ANI_MAIN_PTR	(0xce35)	/* RealPt */
#define GUI_TEXT_BUFFER	(0xce87)	/* unsigned char[64] */
#define BUFFER9_PTR2	(0xd015)	/* RealPt, copy of BUFFER9_PTR */
#define BUFFER9_PTR3	(0xd019)	/* RealPt, copy of BUFFER9_PTR */
#define BUFFER_MONSTER_TAB	(0xd01d)	/* unsigned long[36] */
#define BUFFER_WFIGS_TAB	(0xd0ad)	/* unsigned long[43] */
#define BUFFER_MFIGS_TAB	(0xd159)	/* unsigned long[43] */
#define BUFFER_ANIS_TAB	(0xd205)	/* unsigned long[37] */
#define BUFFER5_PTR	(0xd2a5)	/* RealPt to buffer of size 3880 */
#define BUFFER10_PTR	(0xd2a9)	/* RealPt to buffer of size 16771, used for NVF and text */
#define POPUP	(0xd2ad)	/* RealPt */
#define BUFFER6_PTR	(0xd2b1)	/* RealPt to buffer of size 2200, used for NVF */
#define BUFFER7_PTR	(0xd2b5)	/* RealPt to buffer of size 10000, used for NVF and text */
#define BUFFER2_PTR	(0xd2b9)	/* RealPt to buffer of size 30500 */
#define SPLASH_BUFFER	(0xd2bd)	/* RealPt to buffer of size 1000, used for Splashes */
#define BUF_FONT6	(0xd2c1)	/* RealPt */
#define TEXTCOLOR	(0xd2c5)
#define TEXTCOLOR_BG	(0xd2c7)
#define TEXTCOLOR_FG	(0xd2c9)
#define FIG_FIGURE2_BUF	(0xd2db)	/* RealPt to buffer of size 20000 */
#define FIG_FIGURE1_BUF	(0xd2df)	/* RealPt */
#define OBJECTS_NVF_BUF	(0xd2e3)	/* RealPt to buffer of size 3400 */
#define BUF_ICON	(0xd2e7)	/* RealPt */
#define TEXT_OUTPUT_BUF (0xd2eb)	/* RealPt to buffer of size 300 */
#define TEXT_INPUT_BUF	(0xd2ef)	/* RealPt to buffer of size 24 */
#define DTP2	(0xd2f3)
#define ICON	(0xd2f7)	/* RealPt */
#define BUFFER1_PTR	(0xd303)	/* RealPt to buffer of size 65000 */
#define VIDEO_PAGE_BAK	(0xd30b)	/* signed short */
#define VIDEO_MODE_BAK	(0xd30d)	/* signed short */
#define TXT_TABPOS1	(0xd313)	/* signed short */
#define TXT_TABPOS2	(0xd315)	/* signed short */
#define TXT_TABPOS3	(0xd317)	/* signed short */
#define TXT_TABPOS4	(0xd319)	/* signed short */
#define TXT_TABPOS5	(0xd31b)	/* signed short */
#define TXT_TABPOS6	(0xd31d)	/* signed short */
#define TXT_TABPOS7	(0xd31f)	/* signed short */
#define TOWNPAL_BUF	(0xd321)	/* RealPt */
#define HERO_SLEEP_MOD	(0xd32d)	/* signed short */
#define GATHER_HERBS_MOD	(0xd32f)	/* signed short */
#define REPLENISH_STOCKS_MOD	(0xd331)	/* signed short */
#define FIG_MSG_DATA	(0xd333)	/* (struct { signed short type, damage; })[5] */
#define ENEMY_SHEETS	(0xd34b)	/* struct enemy[20] */
#define HERO_IS_TARGET	(0xd84b)	/* signed char[7] */
#define CHESSBOARD	(0xd852)	/* RealPt */
#define SPELLOBJ_NVF_BUF	(0xd866)	/* RealPt to buffer of size 0xf5f */
#define WEAPONS_NVF_BUF	(0xd86a)	/* RealPt to buffer of size 0x1953 */
#define NR_OF_ENEMIES	(0xd872)	/* ? short */
#define FIG_LIST_ELEM	(0xe066)	/* struct of size 35 */
#define FIG_LIST_ARRAY	(0xe089)	/* unsigned char[127] */
#define FIG_LIST_HEAD	(0xe108)	/* RealPtr to a list */
#define WALLCLOCK_Y	(0xe10f)
#define WALLCLOCK_X	(0xe111)
#define WALLCLOCK_UPDATE	(0xe113)	/* unsigned short, 0 = don't update the wallclock */
#define MEM_SLOTS_MON	(0xe115)
#define MEM_SLOTS_WFIG	(0xe119)
#define MEM_SLOTS_MFIG	(0xe11d)
#define MEM_SLOTS_ANIS	(0xe121)	/* RealPt to (struct { unsigned short nr, ems_handle; long ani_len; })[36] */
#define MONSTER_DAT_BUF	(0xe125)	/* RealPt */
#define MONNAMES	(0xe129)
#define ITEMSDAT	(0xe22b)
#define ITEMSNAME	(0xe22f)
#define FIG_ACTOR_GRAMMAR_TYPE	(0xe2b8)	/* unsigned short, 2 = hero, 1 = monster */
#define FIG_ACTOR_GRAMMAR_ID	(0xe2ba)	/* unsigned short */
#define FIG_TARGET_GRAMMAR_TYPE	(0xe2bc)	/* unsigned short, 2 = hero, 1 = monster */
#define FIG_TARGET_GRAMMAR_ID	(0xe2be)	/* unsigned short */
#define DELAY_TIMER	(0xe2d0)
#define SAVEGAME_NAMES	(0xe2da)	/* (char[9])[5] */
#define DIALOG_INFORMER	(0xe30c)	/* ?16 */
#define DIALOG_DONE	(0xe310)	/* ?16 {0, 1} */
#define DIALOG_STATE	(0xe312)	/* ?16 */
#define TLK_ID	(0xe314)	/* ?16 */
#define CURRENT_FIG_NR	(0xe316)
#define AUTOFIGHT	(0xe318)	/* ?16 */
#define FIG_DROPPED_WEAPONS	(0xe31a)	/* signed short[30] */
#define BUFFER_WEAPANIDAT	(0xe374)	/* pointer to WEAPANI.DAT */
#define BUFFER_ANIDAT	(0xe378)	/* pointer to ANI.DAT buffer */
#define FIG_LIST_BUFFER	(0xe37c)	/* RealPt to buffer of size 4445, initial value of FIG_LIST_HEAD */
#define INC_SPELLS_COUNTER	(0xe3b2)	/* RealPt */
#define INC_SKILLS_COUNTER	(0xe3b6)	/* RealPt */
#define SKILLS_BUFFER	(0xe3ba)	/* RealPt */
#define WILDCAMP_GUARDS	(0xe3be)	/* signed char[3] */
#define WILDCAMP_HERBSTATUS	(0xe3c1)	/* signed char[7] */
#define WILDCAMP_REPLSTATUS	(0xe3c8)	/* signed char[7] */
#define WILDCAMP_MAGICSTATUS	(0xe3cf)	/* signed char[7] */
#define WILDCAMP_GUARDSTATUS	(0xe3d6)	/* signed char[7] */
#define CITYCAMP_GUARDS	(0xe3de)	/* signed char[3] */
#define CITYCAMP_MAGICSTATUS	(0xe3e1)	/* signed char[7] */
#define CITYCAMP_GUARDSTATUS	(0xe3e8)	/* signed char[7] */
#define BOOKED_INN_DAYS	(0xe3f0)	/* signed char */
#define SLEEP_QUALITY	(0xe3f1)	/* signed char */
#define BUY_SHOPPING_CART	(0xe3f2)	/* RealPt to buffer of size 250, each item has 4 bytes */
#define PRICE_MODIFICATOR	(0xe3f6)	/* signed short, price modificator for smith and sell */
#define TEMPLE_GOD	(0xe3f8)	/* signed short, id of current temple's god */
#define TEX_FLOOR	(0xe414)	/* Bit8u* */
#define TEX_SKY	(0xe418)	/* Bit8u* */
#define COMBO_MODE	(0xe42c)	/* signed short {0,1,2} */
#define DIALOGBOX_INDENT_HEIGHT	(0xe4d9)	/* unsigned short */
#define DIALOGBOX_INDENT_WIDTH	(0xe4db)	/* signed short */
#define GRAMMAR_STRING	(0xe4e3)	/* char grammar_string[201] */
#define SPELLTARGET_E	(0xe5b4)	/* Pointer to enemy */
#define SPELLTARGET	(0xe5b8)
#define SPELLUSER	(0xe5bc)
#define SPELLUSER_E	(0xe5c0)
#define ALCHEMY_MISSING_ITEM	(0xe5c4)	/* unsigned char */
#define USED_ITEM_DESC	(0xe5c6)	/* pointer to the item description */
#define USED_ITEM_ID	(0xe5ca)	/* s16 used_item ID */
#define USED_ITEM_POS	(0xe5cc)	/* s16 used_item position */
#define ITEMUSER	(0xe5ce)	/* pointer to hero */
#define WALLCLOCK_UPDATE_BAK	(0xe5d3)	/* unsigned short {0,1} */
#define BUFFERSIZE	(0xe5dc)	/* ?32 size of the global buffer */
#define GLOBAL_BUFFER_PTR	(0xe5e0)	/* RealPt, points to the start of the global buffer */
#define LARGE_BUF	(0xe5e4)	/* s8 {0,1} */

#endif
