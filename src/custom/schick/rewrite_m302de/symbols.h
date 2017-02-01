/*
 *	symbol names for version v3.02_de
 *
 *	Here are the mappings from adresses in the datasegment
 *	to symbolic names of global variables.
 *	This will only work with the version v302de.
 */

#if !defined SYMBOLS_H
#define SYMBOLS_H

// ?4
#define STR_BORLAND_CPP                 (0x0004)    /* char[43]; "Borland C++ - Copyright 1991 Borland Intl." */
#define STR_DIVIDE_ERROR                (0x002f)    /* char[45]; "Divide error\r\nAbnormal program termination\r\n" */
// ?57
#define CD_INIT_SUCCESSFUL              (0x0095)    /* unsigned short; {0,1} */
// ?4
#define CD_AUDIO_REPEAT                 (0x009b)    /* unsigned short; {0,1} */
// ?4
#define CD_AUDIO_PAUSED                 (0x00a1)    /* unsigned short; {0,1} */
#define CD_AUDIO_TRACK_BAK              (0x00a3)    /* unsigned short */
#define CD_AUDIO_TRACKLIST              (0x00a5)    /* signed short[15] */
#define CD_CHECK_SKIPMAGIC              (0x00c3)    /* unsigned long */
#define CD_CHECK_ERR_COUNTDOWN          (0x00c7)    /* unsigned short */
#define STR_INSERT_CD                   (0x00c9)    /* char[142]; "BITTE LEGEN SIE DIE \xf2SCHICKSALSKLINGE-CD\xf0 IN LAUFWERK %c: EIN. DIESE WIRD BEN\x99TIGT, DA DATEN W\x8eHREND DES SPIELS VON CD GELADEN WERDEN M\x9aSSEN." */
#define STR_REPEAT                      (0x0157)    /* char[12]; "WIEDERHOLEN" */
#define STR_QUIT                        (0x0163)    /* char[8]; "BEENDEN" */
#define STR_CD_EXEPATH                  (0x016b)    /* char[19]; "X:\DSA\SCHICKM.EXE" */
#define STR_CD_MISSING                  (0x017e)    /* char[204]; "DIESES PROGRAMM IST OHNE CD-ROM-LAUFWERK NICHT LAUFF\x8eHIG. BITTE INSTALLIEREN SIE DIE GER\x8eTETREIBER F\x9aR DAS CD-ROM-LAUFWERK GEM\x8eSS IHRER CD-ROM-DOKUMENTATION UND STARTEN SIE DIE 'SCHICKSALSKLINGE' ERNEUT." */
#define STR_CD_INIT                     (0x024a)    /* char[37]; "INITIALISIERE CD-ROM IN LAUFWERK %c:" */
// ?1
#define ITEMS_NOPLURAL                  (0x0270)    /* signed short[23]; { LAKRITZE (0x59), BONBONS (0x5a), SHURINKNOLLENGIFT (0x37), ARAXGIFT (0x38), ANGSTGIFT (0x39), SCHLAFGIFT (0x3a), GOLDLEIM (0x3b), LOTUSGIFT (0x8d), KUKRIS (0x8e), BANNSTAUB (0x8f), KROETENSCHEMELGIFT (0x90), ÖL (0x29), EXPURGICUM (0xa6), VOMICUM (0xa8), GEGENGIFT (0xb4), ERZKLUMPEN (0xb5), LOBPREISUNGEN (0xbd), PLATTENZEUG (0x52), LEDERZEUG (0x54), KETTENZEUG (0x53), MIASTHMATICUM (0xee), HYLAILIC_FIRE (0xef), -1 } */
#define ITEMS_PLURALWORDS               (0x029e)    /* signed short[7]; { DIETRICHE (0x23), SCHUHE (0x32), STIEFEL (0x33), SCHNURSCHUHE (0x34), BONBONS (0x5a), LOBPREISUNGEN (0xbd), -1 } */
#define ITEMS_GENDERS                   (0x02ac)    /* signed char[254] */
#define WEARABLE_ITEMS_TABLES           (0x03aa)    /* short[327]; 13 arrays, each terminated by -1 */
#define WEARABLE_ITEMS_INDEX            (0x0638)    /* RealPt[12]; items wearable depending on hero type */
#define RANGED_WEAPONS_TABLE            (0x0668)    /* struct(8)[9]; struct{char[8];} */
#define WEAPONS_TABLE                   (0x06b0)    /* struct(7)[65]; struct{char[7];} */
#define ARMORS_TABLE                    (0x0877)    /* struct(2)[25]; struct{char rs, be;} */
#define SPECIALITEMS_TABLE              (0x08a9)    /* struct(3)[14]; struct{char unkn1, unkn2, handler_no;} */
#define POISON_POTIONS                  (0x08d3)    /* signed short[10]; { SHURINKNOLLENGIFT (0x37), ARAXGIFT (0x38), ANGSTGIFT (0x39), SCHLAFGIFT (0x3a), GOLDLEIM (0x3b), LOTUSGIFT (0x8d), KUKRIS (0x8e), BANNSTAUB (0x8f), KROETENSCHEMELGIFT (0x90), 0xff } */
#define HERBS_TOXIC                     (0x08e7)    /* signed short[5]; { SHURINKNOLLE (0x7a), ALRAUNE (0x7e), LOTUSBLUTE (0x84), EITRIGER KROTENSCHEMEL (0x3e), 0xff } */
#define HERBS_UNEATABLE                 (0x08f1)    /* signed short[7]; { ILMENBLATT (0x80), FINAGEBÄUMCHEN (0x81), JORUGAWURZEL (0x82), KAIRANHALM (0x9d), OLGINWURZEL (0x9c), DONFSTENGEL (0x7c), 0xff } */
#define ELIXIR_POTIONS                  (0x08ff)    /* signed short[8]; { MU ELIXIER (0x93), KL ELIXIER (0x94), CH ELIXIER (0x95), FF ELIXIER (0x96), GE ELIXIER (0x97), IN ELIXIER (0x98), KK ELIXIER (0x99), 0xff } */
#define BAD_ELIXIRS                     (0x090f)    /* signed short[8]; { MU ELIXIER (0xe2), KL ELIXIER (0xe3), CH ELIXIER (0xe4), FF ELIXIER (0xe5), GE ELIXIER (0xe6), IN ELIXIER (0xe7), KK ELIXIER (0xe8), 0xff } */
#define ATTACK_ITEMS                    (0x091f)    /* signed short[3]; { ITEM_MIASTHMATICUM (0xee), ITEM_HYLAILIC_FIRE (0xef), -1 } */
#define MONNAME_GENDERS                 (0x0925)    /* signed char[78] */
#define STAFFSPELL_DESCRIPTIONS         (0x0973)    /* struct(6)[7]; struct{char attrib1, attrib2, attrib3, bonus, cost, ae_mod;} */
#define SPELL_DESCRIPTIONS              (0x099d)    /* struct(10)[87]; struct{char unkn0, attrib1, attrib2, attrib3, cost, combat, unkn6, target_type, range, fight;} */
#define SPELLS_INDEX                    (0x0d03)    /* struct(2)[8]; struct{signed char first, length;}; { {1,5}, {6,12}, {18,6}, {24,3}, {27,6}, {33,5}, {38,7}, {45,4} } */
#define SPELLS_INDEX2                   (0x0d13)    /* struct(2)[4]; struct{signed char first, length;}; { {49,9}, {58,2}, {60,16}, {76,10} } */
#define MAGIC_SCHOOLS_TABLE             (0x0d1b)    /* short[62]; 9 arrays, each terminated by -1 */
#define MAGIC_SCHOOLS_INDEX             (0x0d97)    /* RealPt[9] */
#define SPELL_HANDLERS                  (0x0dbb)    /* long[86]; function pointer[86] */
#define MON_SPELL_DESCRIPTIONS          (0x0f13)    /* struct(8)[15]; struct{char cost, mode, unkn1, attrib1, attrib2, attrib3, unkn2, ani_id;} */
#define MON_SPELL_REPERTOIRE            (0x0f8b)    /* struct(5)[11]; struct{char spells[5];} */
#define MON_SPELL_HANDLERS              (0x0fc2)    /* long[15]; function pointer[15] */
#define SKILL_DESCRIPTIONS              (0x0ffe)    /* struct(4)[52]; struct{signed char attrib1, attrib2, attrib3, max_inc;} */
#define SKILLS_INDEX                    (0x10ce)    /* struct(2)[7]; struct{signed char first, length;}; { {0,9}, {9,10}, {19,7}, {26,6}, {32,9}, {41,9}, {50,2} } */
#define NVFTAB_FIGURES_RANGEWEAPON      (0x10dc)    /* char[264]; by rwt/viewdir */
#define NVFTAB_FIGURES_UNCONSCIOUS      (0x11e4)    /* short[22] */
#define GFXTAB_OFFSETS_UNCONSCIOUS      (0x1210)    /* struct(2)[88]; struct{char x,y;}; by viewdir */
#define GFXTAB_FIGURES_MAIN             (0x12c0)    /* struct(5)[125]; struct{char figure_no, unkn2, unkn3, unkn4, unkn5;} */
#define GFXTAB_OFFSETS_MAIN             (0x1531)    /* struct(2)[625]; struct{char x,y;}; by viewdir+dead */
#define NVFTAB_FIGURES_DEAD             (0x1a13)    /* short[22] */
#define GFX_ANI_DESCRIPTIONS            (0x1a3f)    /* short[1419]; 41 arrays */
#define GFX_ANI_INDEX                   (0x2555)    /* RealPt[41] */
#define TWO_FIELDED_SPRITE_ID           (0x25f9)    /* char[5] */
#define WEAPONANI_TABLE                 (0x25fe)    /* unsigned short[72] */
#define WEAPONANI_TYPES                 (0x268e)    /* signed char[22]; { 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0 } */
#define FOOD_MESSAGE_SHOWN              (0x26a4)    /* signed char[7] */
#define EMS_ENABLED                     (0x26ab)    /* char */
#define FIG_INITIATIVE                  (0x26ac)    /* signed char; 0 = random, 1 = enemies, 2 = heroes (attack first) */
#define FIG_MSG_COUNTER                 (0x26ad)    /* signed short */
#define ALWAYS_ONE                      (0x26af)    /* unsigned short; writeonly (1) */
#define FIG_CONTINUE_PRINT              (0x26b1)    /* unsigned short; {0,1} */
#define FIG_CHAR_PIC                    (0x26b3)    /* unsigned short */
#define FIG_ENEMY_PIC                   (0x26b5)    /* unsigned short */
#define GUI_ENTERING_SAVEGAME           (0x26b7)    /* unsigned short; {0,1} */
#define CHECK_DISEASE                   (0x26b9)    /* unsigned short; {0,1} */
#define CHECK_POISON                    (0x26bb)    /* unsigned short; {0,1} */
#define TEXT_FILE_INDEX                 (0x26bd)    /* unsigned short */
#define TX_FILE_INDEX                   (0x26bf)    /* signed short; index of file stored with load_tx */
#define FIG_DISCARD                     (0x26c1)    /* unsigned short; {0,1}, whether to discard the fight data after the fight */
#define PALETTE_ALLBLACK2               (0x26c3)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define PALETTE_UNKNOWN1                (0x2723)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define PALETTE_FIGHT1                  (0x2783)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define PALETTE_SPECIAL                 (0x27e3)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
// ?2
#define PP20_INDEX                      (0x2845)    /* signed char; archive file index of current pp20 */
#define REQUEST_REFRESH                 (0x2846)    /* signed short; {0,1} */
#define DEFAULT_MOUSE_CURSOR            (0x2848)    /* struct(64); struct{char[32]; unsigned short mask[16];} */
#define CURSOR_ARROW_UP                 (0x2888)    /* struct(64); struct{char[32]; unsigned short mask[16];} */
#define CURSOR_ARROW_DOWN               (0x28c8)    /* struct(64); struct{char[32]; unsigned short mask[16];} */
#define CURSOR_ARROW_LEFT               (0x2908)    /* struct(64); struct{char[32]; unsigned short mask[16];} */
#define CURSOR_ARROW_RIGHT              (0x2948)    /* struct(64); struct{char[32]; unsigned short mask[16];} */
#define MOUSE_POSY_MIN                  (0x2988)    /* unsigned short */
#define MOUSE_POSX_MIN                  (0x298a)    /* unsigned short */
#define MOUSE_POSY_MAX                  (0x298c)    /* unsigned short */
#define MOUSE_POSX_MAX                  (0x298e)    /* unsigned short */
#define PIC_COPY_DS_RECT                (0x2990)    /* struct(8); struct screen_rect */
#define MOUSE_LOCKED                    (0x2998)    /* unsigned short */
#define MOUSE_REFRESH_FLAG              (0x299a)    /* signed short */
#define MOUSE_POSX                      (0x299c)    /* unsigned short */
#define MOUSE_POSY                      (0x299e)    /* unsigned short */
#define MOUSE_POSX_BAK                  (0x29a0)    /* unsigned short */
#define MOUSE_POSY_BAK                  (0x29a2)    /* unsigned short */
#define MOUSE_MOVED                     (0x29a4)    /* unsigned short; {0,1} */
#define MOUSE_POINTER_OFFSETX           (0x29a6)    /* unsigned short */
#define MOUSE_POINTER_OFFSETY           (0x29a8)    /* unsigned short */
#define MOUSE_POINTER_OFFSETX_BAK       (0x29aa)    /* unsigned short */
#define MOUSE_POINTER_OFFSETY_BAK       (0x29ac)    /* unsigned short */
#define ANI_ENABLED                     (0x29ae)    /* unsigned short; {0,1} */
// ?2
#define STATUSMENU_ALLOWED              (0x29b2)    /* unsigned short; {0,1}, readonly */
#define HEROSWAP_ALLOWED                (0x29b4)    /* unsigned short; {0,1} */
#define INTEMPLE                        (0x29b6)    /* unsigned short; writeonly (0,1), 0 while in temple */
#define INTEMPLE2                       (0x29b8)    /* unsigned short; {0,1}, copy of INTEMPLE */
#define ALWAYS_ZERO3                    (0x29ba)    /* unsigned short; writeonly (0) */
// ?16
#define ACTION_TABLE_MENU               (0x29cc)    /* struct(10)[2]; struct{signed short x1, x2, y1, y2; unsigned short action;} */
#define ACTION_TABLE_PRIMARY            (0x29e0)    /* RealPt; to ACTION_TABLE_MENU */
#define ACTION_TABLE_SECONDARY          (0x29e4)    /* RealPt */
#define ACTION_TABLE_PLAYMASK           (0x29e8)    /* struct(10)[24]; struct{signed short x1, x2, y1, y2; unsigned short action;} */
#define ACTION_TABLE_STATUS             (0x2ad8)    /* struct(10)[30]; struct{signed short x1, x2, y1, y2; unsigned short action;} */
#define ACTION_TABLE_MERCHANT           (0x2c04)    /* struct(10)[2]; struct{signed short x1, x2, y1, y2; unsigned short action;} */
#define CHAR_STATUS_BARS                (0x2c18)    /* struct(8)[7]; struct{signed short LE_ORIG, LE, AE_ORIG, AE;} */
#define DISEASE_PRICES                  (0x2c50)    /* signed short[8] */
#define DISEASE_DELAYS                  (0x2c60)    /* signed short[8] */
#define POISON_PRICES                   (0x2c70)    /* signed short[10] */
#define POISON_DELAYS                   (0x2c84)    /* signed short[10] */
#define DIALOGBOX_LOCK                  (0x2c98)    /* unsigned char; {0,1} */
#define TIMERS_DISABLED                 (0x2c99)    /* short */
#define STATUS_PAGE_MODE                (0x2c9b)    /* signed short; one of 1,2,3,4,5 */
#define STATUS_PAGE_HERO                (0x2c9d)    /* signed short */
#define STATUS_PAGE_HUNGER              (0x2c9f)    /* signed char */
#define STATUS_PAGE_THIRST              (0x2ca0)    /* signed char */
#define MISSING_FILE_GUILOCK            (0x2ca1)    /* unsigned char; writeonly (0,1) */
#define BASEPOS_X                       (0x2ca2)    /* signed short */
#define BASEPOS_Y                       (0x2ca4)    /* signed short */
#define DNG_AREA_LOADED                 (0x2ca6)    /* unsigned char */
#define CITY_AREA_LOADED                (0x2ca7)    /* unsigned char */
#define TRAVEL_MAP_LOADED               (0x2ca8)    /* signed char */
// ?33
#define ANI_UNKNOWN_FLAG                (0x2cca)    /* unsigned char; writeonly (0,1) */
#define AREA_PREPARED                   (0x2ccb)    /* signed short; -1 = unprepared, 0 = dungeon, 1 = town */
// ?1
#define ALWAYS_ZERO4                    (0x2cce)    /* char; writeonly (0) */
#define CURRENT_ANI                     (0x2ccf)    /* short */
#define FIG_FIGURE1                     (0x2cd1)    /* signed short */
#define FIG_FIGURE2                     (0x2cd3)    /* signed short */
#define IN_FIGHT                        (0x2cd5)    /* short */
#define FIGHT_ROUND                     (0x2cd7)    /* short */
// ?2
#define SKILLED_HERO_POS                (0x2cdb)    /* signed short; {-1, 0..6} */
#define GUI_BUTTONS_POS                 (0x2cdd)    /* struct(4)[9]; struct{unsigned short x,y;} */
#define HERO_PIC_POSX                   (0x2d01)    /* unsigned short[7]; { 3, 46, 89, 132, 175, 218, 275 } */
#define LEVELUP_TA_RISE                 (0x2d0f)    /* signed char[12] */
#define LEVELUP_SP_RISE                 (0x2d1b)    /* signed char[6] */
#define LEVELUP_SPTA_CONV               (0x2d21)    /* signed char[6]; { 5, 5, 5, 0, 0, 0 } */
#define MR_MODIFICATORS                 (0x2d27)    /* signed char[13]; { 0, 2, 0, 0, 2, -2, 2, 2, 2, 2, 3, 4, 3 } */
#define DATSEG_STATUS_START             (0x2d34)    /* unsigned char; 99 = game finished, area of the datseg that is stored one to one in savegame files */
#define CURRENT_GROUP                   (0x2d35)    /* signed char */
#define GROUP_MEMBER_COUNTS             (0x2d36)    /* signed char[6]; members per group */
#define TOTAL_HERO_COUNTER              (0x2d3c)    /* signed char */
#define DIRECTION                       (0x2d3d)    /* unsigned char */
#define GROUPS_DIRECTION                (0x2d3e)    /* unsigned char[6] */
#define X_TARGET                        (0x2d44)    /* unsigned short */
#define Y_TARGET                        (0x2d46)    /* unsigned short */
#define GROUPS_X_TARGET                 (0x2d48)    /* unsigned short[6] */
#define GROUPS_Y_TARGET                 (0x2d54)    /* unsigned short[6] */
#define LOCATION                        (0x2d60)    /* signed char */
#define GROUPS_LOCATION                 (0x2d61)    /* signed char[6] */
#define CURRENT_TOWN                    (0x2d67)    /* signed char */
#define GROUPS_TOWN                     (0x2d68)    /* signed char[6] */
#define DUNGEON_INDEX                   (0x2d6e)    /* signed char */
#define GROUPS_DNG_INDEX                (0x2d6f)    /* signed char[6] */
#define DUNGEON_LEVEL                   (0x2d75)    /* signed char */
#define GROUPS_DNG_LEVEL                (0x2d76)    /* signed char[6] */
#define DIRECTION_BAK                   (0x2d7c)    /* unsigned char */
#define GROUPS_DIRECTION_BAK            (0x2d7d)    /* unsigned char[6] */
#define X_TARGET_BAK                    (0x2d83)    /* signed short */
#define Y_TARGET_BAK                    (0x2d85)    /* signed short */
#define GROUPS_X_TARGET_BAK             (0x2d87)    /* signed short[6] */
#define GROUPS_Y_TARGET_BAK             (0x2d93)    /* signed short[6] */
#define LOCATION_BAK                    (0x2d9f)    /* signed char */
#define GROUPS_LOCATION_BAK             (0x2da0)    /* signed char[6] */
#define CURRENT_TOWN_BAK                (0x2da6)    /* signed char */
#define GROUPS_TOWN_BAK                 (0x2da7)    /* signed char[6] */
#define DUNGEON_INDEX_BAK               (0x2dad)    /* signed char */
#define GROUPS_DNG_INDEX_BAK            (0x2dae)    /* signed char[6] */
#define DUNGEON_LEVEL_BAK               (0x2db4)    /* signed char */
#define GROUPS_DNG_LEVEL_BAK            (0x2db5)    /* signed char[6] */
#define DAY_TIMER                       (0x2dbb)    /* long */
#define DAY_OF_WEEK                     (0x2dbf)    /* char */
#define DAY_OF_MONTH                    (0x2dc0)    /* char */
#define MONTH                           (0x2dc1)    /* char */
#define YEAR                            (0x2dc2)    /* char */
#define SPECIAL_DAY                     (0x2dc3)    /* char */
#define INGAME_TIMERS                   (0x2dc4)    /* signed long[26] */
#define MODIFICATION_TIMERS             (0x2e2c)    /* struct(8)[100]; struct{long; short; char; char} */
// ?2
#define GODS_ESTIMATION                 (0x314e)    /* signed long[15] */
#define TAV_CHEATED_FLAGS               (0x318a)    /* unsigned char[88] */
#define SMITH_REPAIRITEMS               (0x31e2)    /* struct(6)[50]; struct{short; long} */
#define DNG_HANDLED_POS                 (0x330e)    /* unsigned short */
#define DEATHTRAP_STEPS                 (0x3310)    /* signed short */
#define DEATHTRAP                       (0x3312)    /* signed short */
#define DNG01_AMULET_TAKEN              (0x3314)    /* signed char; {0,1} */
#define DNG01_SABRE_TAKEN               (0x3315)    /* signed char; {0,1} */
#define DNG01_CROSSBOW_TAKEN            (0x3316)    /* signed char; {0,1} */
#define DNG01_ARDORA_FREED              (0x3317)    /* signed char; {0,1} */
// ?1
#define DNG01_KEY_TAKEN                 (0x3319)    /* signed char; {0,1} */
#define DNG01_CORPSE_LOOTED             (0x331a)    /* signed char; {0,1} */
#define WEATHER1                        (0x331b)    /* signed short; random(6) */
#define WEATHER2                        (0x331d)    /* signed short; random(7) */
#define DASPOTA_FIGHTFLAGS              (0x331f)    /* signed char[19] */
#define TREASURE_MAPS                   (0x3332)    /* signed char[10] */
#define DEBTBOOK_READ_FLAG              (0x333c)    /* unsigned char; {0,1} */
#define ORCDOCUMENT_READ_FLAG           (0x333d)    /* unsigned char; {0,1} */
#define GARDANIUM_USED_AE               (0x333e)    /* unsigned short */
#define MIN_MAP_PARTS                   (0x3340)    /* signed short; {0} */
#define SUBVENTION                      (0x3342)    /* short; {0-4} */
#define OTTA_WINDRIDERS                 (0x3344)    /* unsigned short */
#define OTTA_STORMCHILDREN              (0x3346)    /* unsigned short */
#define OTTA_GARALDSSON                 (0x3348)    /* unsigned short */
#define HEARD_ANNOUNCE                  (0x334a)    /* signed short; {0 = not heard, 1 = heard, 3 = spoke with hetman} */
#define GOT_MAIN_QUEST                  (0x334c)    /* signed short; {0,1} */
#define QUESTED_MONTHS                  (0x334e)    /* short */
#define MONTHLY_CREDIT                  (0x3350)    /* signed short */
#define GOT_LETTER_JAD                  (0x3352)    /* signed short; {0,1} */
#define VISITED_MILLER                  (0x3354)    /* signed short; {0,1} */
#define QUEST_UGDALF                    (0x3356)    /* signed short; {0, 1, 3, 4} */
#define ACADEMY_DAILY_CURSE             (0x3358)    /* signed short; {0,1} */
#define ACADEMY_DAILY_IDENT             (0x335a)    /* signed short; {0,1} */
#define BANK_DEPOSIT                    (0x335c)    /* signed short */
#define DAYS_TO_CENS                    (0x335e)    /* signed short */
#define DEBT_DAYS                       (0x3360)    /* signed short */
#define IN_ACADEMY                      (0x3362)    /* unsigned short; {0,99} */
#define INFORMER_FLAGS                  (0x3364)    /* unsigned char[15]; {0,1,2} */
// ?1
#define TAV_KICKED_FLAGS                (0x3374)    /* signed char[88]; {0,1} */
#define TOWN_OUTLAWED_FLAGS             (0x33cc)    /* signed char[52]; {0,1} */
#define HERBERG_KICKED_FLAGS            (0x3400)    /* signed char[74]; {0,1} */
#define GOT_LETTER_HET                  (0x344a)    /* unsigned char; {0,1} */
// ?1
#define JURGE_AWAITS_LETTER             (0x344c)    /* unsigned char; {0,1} */
#define UMBRIK_QUEST_GORAH              (0x344d)    /* unsigned char; {0,1} */
// ?1
#define ISLEIF_JANDAS_REGARDS           (0x344f)    /* unsigned char; {0,1} */
#define ISLEIF_MAP_GIVEN                (0x3450)    /* unsigned char; {0,1} */
#define RAGNA_ASKED_MAP                 (0x3451)    /* unsigned char; {0,1} */
#define BEORN_LITT_BOLD                 (0x3452)    /* unsigned char; {0,1} */
#define BEORN_MEDI_BOLD                 (0x3453)    /* unsigned char; {0,1} */
#define BEORN_VERY_BOLD                 (0x3454)    /* unsigned char; {0,1} */
#define ELIANE_QUEST_NAMELESS           (0x3455)    /* unsigned char; {0,1} */
#define OLVIR_START_HETMANN             (0x3456)    /* unsigned char; {0,1} */
#define OLVIR_START_HYGGELIK            (0x3457)    /* unsigned char; {0,1} */
#define OLVIR_START_HASGAR              (0x3458)    /* unsigned char; {0,1} */
#define OLVIR_INTERRUPTED               (0x3459)    /* unsigned char; {0,1} */
#define OLVIR_SINGING_HETMANN           (0x345a)    /* unsigned char; {0,1} */
#define OLVIR_SINGING_HYGGELIK          (0x345b)    /* unsigned char; {0,1} */
#define OLVIR_SINGING_HASGAR            (0x345c)    /* unsigned char; {0,1} */
#define SWAFNILD_TP1                    (0x345d)    /* unsigned char */
#define SWAFNILD_TP2                    (0x345e)    /* unsigned char */
#define SWAFNILD_TP3                    (0x345f)    /* unsigned char */
#define SWAFNILD_TP4                    (0x3460)    /* unsigned char */
#define SWAFNILD_DESTINATION            (0x3461)    /* unsigned char; {1,2,3} */
#define SWAFNILD_NOMAP                  (0x3462)    /* unsigned char; {0,1} */
#define TREBORN_DATE                    (0x3463)    /* unsigned char; {0,1} */
#define UNICORN_HERO_POS                (0x3464)    /* unsigned char; {0-6} */
#define UNICORN_GET_MAP                 (0x3465)    /* char */
#define UNICORN_TIMER                   (0x3466)    /* char */
#define ALGRID_TREBORNS_HINT            (0x3467)    /* unsigned char; {0,1} */
#define TIOMAR_DRINKMATE                (0x3468)    /* unsigned char; {0-6} */
#define TIOMAR_AWAITS_LETTER            (0x3469)    /* unsigned char; {0,1} */
#define UNICORN_HERO_PTR                (0x346a)    /* RealPt */
#define NPC_MEET_TAVERN                 (0x346e)    /* signed short */
#define NPC_MONTHS                      (0x3470)    /* signed short */
#define SMITH_KICKED_FLAGS              (0x3472)    /* signed char[50]; {0,1} */
#define SMITH_FLOGGED_FLAGS             (0x34a4)    /* signed char[50]; {0,1} */
#define MERCHANT_KICKED_FLAGS           (0x34d6)    /* signed char[74]; {0,1} */
#define NPC_UNKNOWN_FLAGS               (0x3520)    /* unsigned char[20] */
#define MERCHANT_KICKED2_FLAGS          (0x3534)    /* unsigned char[94]; {0,1} */
#define MERCHANT_OFFENDED_FLAGS         (0x3592)    /* unsigned char[94]; {0,1} */
#define QUEST_DEADSHIP                  (0x35f0)    /* unsigned char; {0,1} */
// ?1
#define QUEST_DEADSHIP_DONE             (0x35f2)    /* unsigned char; {0,1} */
#define ALWAYS_ONE2                     (0x35f3)    /* unsigned char;  writeonly (1) */
#define DNG03_SPIDEREGGS_BURNED         (0x35f4)    /* unsigned char; {0,1} */
// ?2
#define DNG06_DASPTREAS_FOUND           (0x35f7)    /* unsigned char; {0,1} */
// ?2
#define QUEST_NAMELESS_DONE             (0x35fa)    /* unsigned char; {0,1} */
// ?4
#define DNG14_UGDALF_DONE               (0x35ff)    /* unsigned char; {0,1} */
#define GOT_GRIMRING                    (0x3600)    /* unsigned char; {0,1} */
#define NPC_TIMERS                      (0x3601)    /* unsigned char[7] */
#define TLK_TAV_FIRSTINFO               (0x3608)    /* unsigned char; {0,1} */
#define TLK_TAV_INFOPROMISE             (0x3609)    /* unsigned char; {0,1} */
#define TLK_TAV_ROUND                   (0x360a)    /* unsigned char; {0,1} */
#define TLK_TAV_FREEBEER                (0x360b)    /* unsigned char; {0,1} */
#define TLK_TAV_DRINKCOUNT              (0x360c)    /* unsigned char */
#define TLK_TAV_INFORMERSEX             (0x360d)    /* unsigned char; {1 = male,2 = female} */
// ?1
#define TLK_TAV_FOLLOWINFORMER          (0x360f)    /* unsigned char; {0,1} */
#define TLK_TAV_FULLNESS                (0x3610)    /* unsigned char */
#define TLK_TAV_TESTDRUNK               (0x3611)    /* unsigned char */
#define CURRENT_INFORMER                (0x3612)    /* unsigned char; {0,1,6,7,8,14} */
#define DUNGEON_LIGHT                   (0x3613)    /* unsigned char; 0 = light is on, 1, 2 = light is off */
#define SHOW_TRAVEL_MAP                 (0x3614)    /* unsigned char; {0,1} */
#define HERMIT_VISITED                  (0x3615)    /* unsigned char; {0,1} */
#define DUNGEON_TYPE                    (0x3616)    /* unsigned char; { 0 = SHIPS, 1 = MARBLES, 2 = STONES } */
#define ESTORIK_KNOWN                   (0x3617)    /* unsigned char; {0,1} */
#define DIALOG_PARTNERS                 (0x3618)    /* struct(38)[10] */
#define DIALOG_STATES                   (0x3794)    /* struct(8)[160] */
#define DNG02_SPHERE_KNOWN              (0x3c94)    /* unsigned char; {0,1} */
#define DNG02_CORPSE                    (0x3c95)    /* unsigned char; {0,1} */
#define DNG02_APPARATURE_DESTROYED      (0x3c96)    /* unsigned char; {0,1} */
#define DNG02_SPHERE_TIMER              (0x3c97)    /* unsigned char; {0,...,7} */
#define DNG02_SPHERE_ACTIVE             (0x3c98)    /* unsigned char; {0,1} */
#define DNG02_RAVEN_DESTROYED           (0x3c99)    /* unsigned char; {0,1} */
#define DNG02_SECRET_DOOR1              (0x3c9a)    /* unsigned char; {0,1,2} */
#define DNG02_SECRET_DOOR2              (0x3c9b)    /* unsigned char; {0,1,2} */
#define DNG02_SECRET_DOOR3              (0x3c9c)    /* unsigned char; {0,1,2} */
#define DNG03_LEVER_TRAP                (0x3c9d)    /* unsigned char; {0,1} */
#define DNG03_LEVER_CHEST11             (0x3c9e)    /* unsigned char; {0,1} */
#define DNG03_SANCTUM_SMASHED           (0x3c9f)    /* unsigned char; {0,1} */
#define DNG04_CORPSE0_FLAG              (0x3ca0)    /* unsigned char; {0,1} */
#define DNG04_CORPSE1_FLAG              (0x3ca1)    /* unsigned char; {0,1} */
#define DNG04_CORPSE2_FLAG              (0x3ca2)    /* unsigned char; {0,1} */
#define DNG04_GAP_FLAG                  (0x3ca3)    /* unsigned char; {0,1} */
#define DNG04_HELMET_FLAG               (0x3ca4)    /* unsigned char; {0,1} */
#define DNG04_LANTERN_FLAG              (0x3ca5)    /* unsigned char; {0,1} */
#define DNG04_DEEPGAP_FLAG              (0x3ca6)    /* unsigned char; {0,1} */
#define DNG06_PITDOOR_FLAG              (0x3ca7)    /* unsigned char; {0,1} */
#define DNG06_PROVIANT_FLAG             (0x3ca8)    /* unsigned char; {0,1} */
#define DNG06_MONEY_FLAG                (0x3ca9)    /* unsigned char; {0,1} */
#define DNG06_GOLDKEY_FLAG              (0x3caa)    /* unsigned char; {0,1,2} */
#define DNG06_COUNTDOWN_FLAG            (0x3cab)    /* unsigned char; {0,1,2,3,4,5} */
#define DNG06_LEVER_FLAG                (0x3cac)    /* unsigned char; {0,1} */
#define DNG06_BOOK_FLAG                 (0x3cad)    /* unsigned char; {0,1} */
#define DNG06_FIGHT19_FLAG              (0x3cae)    /* unsigned char; {0,1} */
#define DNG05_PROVIANT_FLAG             (0x3caf)    /* unsigned char; {0,1} */
#define DNG05_BATS_FLAG                 (0x3cb0)    /* unsigned char; {0,1} */
#define DNG05_GOD_FLAG                  (0x3cb1)    /* unsigned char; {0,1} */
#define DNG07_MUELIXIER_FLAG            (0x3cb2)    /* unsigned char; {0,1,2} */
#define DNG07_ANTIMUELIXIER_FLAG        (0x3cb3)    /* unsigned char; {0,1} */
#define DNG07_FLICKER_FLAG              (0x3cb4)    /* unsigned char; {0,1} */
#define DNG07_POISON_FLAG               (0x3cb5)    /* unsigned char; {0,1} */
#define DNG07_POISON_TIMER              (0x3cb6)    /* unsigned long */
#define DNG08_BED_00                    (0x3cba)    /* unsigned char; {0,1} */
#define DNG08_BED_01                    (0x3cbb)    /* unsigned char; {0,1} */
#define DNG08_BED_02                    (0x3cbc)    /* unsigned char; {0,1} */
#define DNG08_BED_03                    (0x3cbd)    /* unsigned char; {0,1} */
#define DNG08_BED_04                    (0x3cbe)    /* unsigned char; {0,1} */
#define DNG08_BED_05                    (0x3cbf)    /* unsigned char; {0,1} */
#define DNG08_BED_06                    (0x3cc0)    /* unsigned char; {0,1} */
#define DNG08_BED_07                    (0x3cc1)    /* unsigned char; {0,1} */
#define DNG08_BED_08                    (0x3cc2)    /* unsigned char; {0,1} */
#define DNG08_BED_09                    (0x3cc3)    /* unsigned char; {0,1} */
#define DNG08_BED_10                    (0x3cc4)    /* unsigned char; {0,1} */
#define DNG08_BED_11                    (0x3cc5)    /* unsigned char; {0,1} */
#define DNG08_TIMER1                    (0x3cc6)    /* unsigned char; {0, 1, ..., 24} */
#define DNG08_TIMER2                    (0x3cc7)    /* unsigned char; {0, 1, ..., 24} */
#define DNG08_BUNDLES_DONE              (0x3cc8)    /* unsigned char; {0,1} */
#define DNG08_BALLISTA_DONE             (0x3cc9)    /* unsigned char; {0,1} */
#define DNG08_CHEST2_LOOTED             (0x3cca)    /* unsigned char; {0,1} */
#define DNG08_SECRET_DOOR               (0x3ccb)    /* signed char; {0,1 = found,2 = opened} */
#define DNG08_CHEST0_LOOTED             (0x3ccc)    /* unsigned char; {0,1} */
#define DNG08_CHEST35_LOOTED            (0x3ccd)    /* unsigned char; {0,1,2,3} */
#define DASPOTA_LOCATION01_LOOT         (0x3cce)    /* char[29] */
#define DASPOTA_LOCATION02_LOOT         (0x3ceb)    /* char[15] */
#define DASPOTA_LOCATION03_LOOT         (0x3cfa)    /* char[15] */
#define DASPOTA_LOCATION04_LOOT         (0x3d09)    /* char[9] */
#define DASPOTA_LOCATION05_LOOT         (0x3d12)    /* char[31] */
#define DASPOTA_LOCATION06_LOOT         (0x3d31)    /* char[25] */
#define DASPOTA_LOCATION07_LOOT         (0x3d4a)    /* char[7] */
#define DASPOTA_LOCATION08_LOOT         (0x3d51)    /* char[15] */
#define DASPOTA_LOCATION10_LOOT         (0x3d60)    /* char[7] */
#define DASPOTA_LOCATION12_LOOT         (0x3d67)    /* char[21] */
#define DASPOTA_LOCATION13_LOOT         (0x3d7c)    /* char[9] */
#define DASPOTA_LOCATION14_LOOT         (0x3d85)    /* char[5] */
#define DASPOTA_LOCATION16_LOOT         (0x3d8a)    /* char[5] */
#define DASPOTA_LOCATION18_LOOT         (0x3d8f)    /* char[19] */
#define TEVENT004_FLAG                  (0x3da2)    /* unsigned char; {0,1} */
#define TEVENT005_FLAG                  (0x3da3)    /* unsigned char; {0,1} */
#define TEVENT008_FLAG                  (0x3da4)    /* unsigned char; {0,1} */
#define TEVENT009_FLAG                  (0x3da5)    /* unsigned char; {0,1} */
#define TEVENT011_FLAG                  (0x3da6)    /* unsigned char; {0,1} */
#define TEVENT013_FLAG                  (0x3da7)    /* unsigned char; {0,1} */
#define TEVENT014_FLAG                  (0x3da8)    /* unsigned char; {0,1} */
#define TEVENT016_FLAG                  (0x3da9)    /* unsigned char; {0,1} */
#define TEVENT017_FLAG                  (0x3daa)    /* unsigned char; {0,1} */
#define TEVENT020_FLAG                  (0x3dab)    /* unsigned char; {0,1} */
#define TEVENT021_FLAG                  (0x3dac)    /* unsigned char; {0,1} */
#define TEVENT022_FLAG                  (0x3dad)    /* unsigned char; {0,1} */
#define TEVENT022_TRACK_FLAG            (0x3dae)    /* unsigned char; {0,1} */
#define TEVENT024_FLAG                  (0x3daf)    /* unsigned char; {0,1} */
#define TEVENT025_FLAG                  (0x3db0)    /* unsigned char; {0,1} */
#define TEVENT028_FLAG                  (0x3db1)    /* unsigned char; {0,1} */
#define TEVENT030_FLAG                  (0x3db2)    /* unsigned char; {0,1} */
#define TEVENT031_FLAG                  (0x3db3)    /* unsigned char; {0,1} */
#define TEVENT032_FLAG                  (0x3db4)    /* unsigned char; {0,1} */
#define TEVENT032_HERB_FLAG             (0x3db5)    /* unsigned char; {0,1} */
#define TEVENT034_FLAG                  (0x3db6)    /* unsigned char; {0,1} */
#define TEVENT035_FLAG                  (0x3db7)    /* unsigned char; {0,1} */
#define TEVENT036_FLAG                  (0x3db8)    /* unsigned char; {0,1} */
#define TEVENT036_HERB_FLAG             (0x3db9)    /* unsigned char; {0,1} */
#define TEVENT037_FLAG                  (0x3dba)    /* unsigned char; {0,1} */
#define TEVENT039_FLAG                  (0x3dbb)    /* unsigned char; {0,1} */
#define TEVENT040_FLAG                  (0x3dbc)    /* unsigned char; {0,1} */
#define TEVENT041_FLAG                  (0x3dbd)    /* unsigned char; {0,1} */
#define TEVENT042_FLAG                  (0x3dbe)    /* unsigned char; {0,1} */
#define TEVENT043_FLAG                  (0x3dbf)    /* unsigned char; {0,1} */
#define TEVENT048_FLAG                  (0x3dc0)    /* unsigned char; {0,1} */
#define TEVENT048_TRACK_FLAG            (0x3dc1)    /* unsigned char; {0,1} */
#define TEVENT049_FLAG                  (0x3dc2)    /* unsigned char; {0,1} */
#define TEVENT050_FLAG                  (0x3dc3)    /* unsigned char; {0,1} */
#define TEVENT050_HERB_FLAG             (0x3dc4)    /* unsigned char; {0,1} */
#define TEVENT051_FLAG                  (0x3dc5)    /* unsigned char; {0,1} */
#define TEVENT053_FLAG                  (0x3dc6)    /* unsigned char; {0,1} */
#define TEVENT058_FLAG                  (0x3dc7)    /* unsigned char; {0,1} */
#define TEVENT061_FLAG                  (0x3dc8)    /* unsigned char; {0,1} */
#define TEVENT064_FLAG                  (0x3dc9)    /* unsigned char; {0,1} */
#define TEVENT066_FLAG                  (0x3dca)    /* unsigned char; {0,1} */
#define TEVENT066_TRACK_FLAG            (0x3dcb)    /* unsigned char; {0,1} */
#define TEVENT067_FLAG                  (0x3dcc)    /* unsigned char; {0,1} */
#define TEVENT069_FLAG                  (0x3dcd)    /* unsigned char; {0,1} */
#define TEVENT070_FLAG                  (0x3dce)    /* unsigned char; {0,1} */
#define TEVENT070_HERB_FLAG             (0x3dcf)    /* unsigned char; {0,1} */
#define TEVENT070_TRAIL_FLAG            (0x3dd0)    /* unsigned char; {0,1} */
#define TEVENT071_FLAG                  (0x3dd1)    /* unsigned char; {0,1} */
#define TEVENTU01_FLAG                  (0x3dd2)    /* unsigned char; {0,1} */
#define TEVENT064_SILENT_FLAG           (0x3dd3)    /* unsigned char; {0,1} */
// ?1
#define TEVENTU02_FLAG                  (0x3dd5)    /* unsigned char; {0,1} */
#define TEVENTU03_FLAG                  (0x3dd6)    /* unsigned char; {0,1} */
#define TEVENTU04_FLAG                  (0x3dd7)    /* unsigned char; {0,1} */
#define TEVENTU05_FLAG                  (0x3dd8)    /* unsigned char; {0,1} */
#define TEVENTU06_FLAG                  (0x3dd9)    /* unsigned char; {0,1} */
#define TEVENT071_ORCSTATUE             (0x3dda)    /* unsigned char; {0,1} */
#define MET_UNICORN_FLAG                (0x3ddb)    /* unsigned char; {0,1} */
#define FIND_HYGGELIK                   (0x3ddc)    /* unsigned char; {0,1 = you are able to find Hyggelik} */
#define TEVENT072_FLAG                  (0x3ddd)    /* unsigned char; {0,1} */
#define TEVENT073_FLAG                  (0x3dde)    /* unsigned char; {0,1} */
#define TEVENT074_FLAG                  (0x3ddf)    /* unsigned char; {0,1} */
#define TEVENT075_FLAG                  (0x3de0)    /* unsigned char; {0,1} */
#define TEVENT076_FLAG                  (0x3de1)    /* unsigned char; {0,1} */
#define TEVENT077_FLAG                  (0x3de2)    /* unsigned char; {0,1} */
#define TEVENT079_FLAG                  (0x3de3)    /* unsigned char; {0,1} */
#define TEVENT080_FLAG                  (0x3de4)    /* unsigned char; {0,1} */
#define TEVENT080_TATZELWURM            (0x3de5)    /* unsigned char; {0, 1, 2} */
#define TEVENT081_FLAG                  (0x3de6)    /* unsigned char; {0,1} */
#define TEVENT083_FLAG                  (0x3de7)    /* unsigned char; {0,1} */
#define TEVENT084_FLAG                  (0x3de8)    /* unsigned char; {0,1} */
#define TEVENT085_FLAG                  (0x3de9)    /* unsigned char; {0,1} */
#define TEVENT085_HERB_FLAG             (0x3dea)    /* unsigned char; {0,1} */
#define TEVENT086_FLAG                  (0x3deb)    /* unsigned char; {0,5,15} */
#define TEVENT088_FLAG                  (0x3dec)    /* unsigned char; {0,1} */
#define TEVENT091_FLAG                  (0x3ded)    /* unsigned char; {0,1} */
#define TEVENT093_FLAG                  (0x3dee)    /* unsigned char; {0,1} */
#define TEVENT094_FLAG                  (0x3def)    /* unsigned char; {0,1} */
#define TEVENTU07_FLAG                  (0x3df0)    /* unsigned char; {0,1} */
#define TEVENTU08_FLAG                  (0x3df1)    /* unsigned char; {0,1} */
#define TEVENT099_FLAG                  (0x3df2)    /* unsigned char; {0,1} */
#define TEVENT100_FLAG                  (0x3df3)    /* unsigned char; {0,1} */
#define TEVENT101_FLAG                  (0x3df4)    /* unsigned char; {0,1} */
#define TEVENT106_FLAG                  (0x3df5)    /* unsigned char; {0,1} */
#define TEVENT108_FLAG                  (0x3df6)    /* unsigned char; {0,1} */
#define TEVENT109_FLAG                  (0x3df7)    /* unsigned char; {0,1} */
#define TEVENT111_FLAG                  (0x3df8)    /* unsigned char; {0,1} */
#define TEVENT112_FLAG                  (0x3df9)    /* unsigned char; {0,1} */
#define TEVENT112_HERB_FLAG             (0x3dfa)    /* unsigned char; {0,1} */
#define TEVENT115_FLAG                  (0x3dfb)    /* unsigned char; {0,1} */
#define TEVENT116_FLAG                  (0x3dfc)    /* unsigned char; {0,1} */
#define TEVENT118_FLAG                  (0x3dfd)    /* unsigned char; {0,1} */
#define TEVENT119_FLAG                  (0x3dfe)    /* unsigned char; {0,1} */
#define TEVENT120_FLAG                  (0x3dff)    /* unsigned char; {0,1} */
#define TEVENT122_FLAG                  (0x3e00)    /* unsigned char; {0,1} */
// ?1
#define TEVENT128_FLAG                  (0x3e02)    /* unsigned char; {0,1} */
#define TEVENT128_REPLEN_FLAG           (0x3e03)    /* unsigned char; {0,1} */
#define TEVENT129_FLAG                  (0x3e04)    /* unsigned char; {0,1} */
#define TEVENT131_FLAG                  (0x3e05)    /* unsigned char; {0,1} */
#define TEVENT132_FLAG                  (0x3e06)    /* unsigned char; {0,1} */
#define TEVENT132_HERB_FLAG             (0x3e07)    /* unsigned char; {0,1} */
#define TEVENT134_FLAG                  (0x3e08)    /* unsigned char; {0,1} */
#define TEVENT137_FLAG                  (0x3e09)    /* unsigned char; {0,1} */
#define TEVENT138_FLAG                  (0x3e0a)    /* unsigned char; {0,1} */
#define TEVENTU09_FLAG                  (0x3e0b)    /* unsigned char; {0,1} */
#define TEVENT140_FLAG                  (0x3e0c)    /* unsigned char; {0,1} */
#define TEVENT140_HERB_FLAG             (0x3e0d)    /* unsigned char; {0,1} */
#define TEVENT141_FLAG                  (0x3e0e)    /* unsigned char; {0,1} */
#define TEVENT142_FLAG                  (0x3e0f)    /* unsigned char; {0,1} */
// ?1
#define TEVENTU10_FLAG                  (0x3e11)    /* unsigned char; {0,1} */
#define TEVENTU11_FLAG                  (0x3e12)    /* unsigned char; {0,1} */
#define TEVENTU12_FLAG                  (0x3e13)    /* unsigned char; {0,1} */
#define TEVENTU13_FLAG                  (0x3e14)    /* unsigned char; {0,1} */
#define TEVENT114_OLIMONE_FLAG          (0x3e15)    /* unsigned char; {0,1} */
#define TEVENT074_FIGHT_FLAG            (0x3e16)    /* unsigned char; {0,1} */
#define TEVENT077_FIGHT_FLAG            (0x3e17)    /* unsigned char; {0,1} */
#define RANDOM_TLK_HERO                 (0x3e18)    /* RealPt */
#define RUIN_HERO                       (0x3e1c)    /* RealPt */
#define MAIN_ACTING_HERO                (0x3e20)    /* RealPt */
#define TEVENT073_CORPSE                (0x3e24)    /* unsigned char[3] */
#define TEVENT014_CHEST                 (0x3e27)    /* unsigned char[4] */
#define TEVENT064_HUT_CONTENT           (0x3e2b)    /* unsigned char[19] */
#define TEVENT064_CHEST                 (0x3e3e)    /* unsigned char[21] */
#define PALETTE_FLOOR                   (0x3e53)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define PALETTE_BUILDINGS               (0x3eb3)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define PALETTE_SKY                     (0x3f13)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define NEED_LETTER                     (0x3f73)    /* unsigned char; {0,1} */
#define GREMOB_INVITED                  (0x3f74)    /* unsigned char; {0,1} */
#define HARLOT_DATE                     (0x3f75)    /* unsigned char; {0,1} */
#define DRUG_TIMER                      (0x3f76)    /* unsigned char; {0, 1, ..., 10} */
#define PTEMPLE_CLOSED                  (0x3f77)    /* unsigned char; {0,1} */
#define ALRIK_DERONDAN                  (0x3f78)    /* unsigned char; {0,1} */
#define ALRIK_DERONDAN_INSULTED         (0x3f79)    /* unsigned char; {0,1} */
#define DNG13_LANTERN_FLAG              (0x3f7a)    /* unsigned char; {0,1} */
#define DNG13_CORPSE0_FLAG              (0x3f7b)    /* unsigned char; {0,1} */
#define DNG13_CORPSE1_FLAG              (0x3f7c)    /* unsigned char; {0,1} */
#define DNG13_PASSAGE1_FLAG             (0x3f7d)    /* unsigned char; {0,1} */
#define DNG13_PASSAGE2_FLAG             (0x3f7e)    /* unsigned char; {0,1} */
#define DNG13_PASSAGE3_FLAG             (0x3f7f)    /* unsigned char; {0,1} */
#define DNG13_PASSAGE4_FLAG             (0x3f80)    /* unsigned char; {0,1} */
#define DNG13_PASSAGE5_FLAG             (0x3f81)    /* unsigned char; {0,1} */
#define DNG13_MONEY_FLAG                (0x3f82)    /* unsigned char; {0,1} */
#define DNG13_CHEST_EQUIPS              (0x3f83)    /* unsigned char[13] */
#define DNG13_CHEST0_CONTENT            (0x3f90)    /* unsigned char[6] */
#define DNG13_CHEST1_CONTENT            (0x3f96)    /* unsigned char[5] */
#define DNG12_TUNNEL1                   (0x3f9b)    /* signed char; {0,1} */
#define DNG12_TUNNEL2                   (0x3f9c)    /* signed char; {0,1} */
#define DNG12_TUNNEL3                   (0x3f9d)    /* signed char; {0,1} */
#define DNG12_TUNNEL4                   (0x3f9e)    /* signed char; {0,1} */
#define DNG12_INGERIMM_SACRIFICE        (0x3f9f)    /* unsigned char; {0,1} */
#define DNG12_INGERIMM_HINT             (0x3fa0)    /* unsigned char; {0,1} */
#define DNG12_WATERTRAP_WATER_RUNS      (0x3fa1)    /* signed char; {0,1} */
#define DNG12_WATERTRAP_TIMER           (0x3fa2)    /* signed long */
#define DNG12_WATERTRAP_ACTIVE          (0x3fa6)    /* signed char; {0,1} */
#define DNG12_OBSTACLE_HP               (0x3fa7)    /* signed char; {-25,...,40} */
#define DNG12_OBSTACLE_ACTIVE           (0x3fa8)    /* signed char; {0,1} */
#define DNG12_SPEARTRAP_ACTIVE          (0x3fa9)    /* signed char; {0,1} */
#define DNG12_CHEST1_CONTENT            (0x3faa)    /* signed char[12] */
#define DNG14_MONEY_FLAG                (0x3fb6)    /* unsigned char; {0,1} */
#define DNG14_ALARM_FLAG                (0x3fb7)    /* unsigned char; {0,1} */
#define DNG14_SECRETDOOR1               (0x3fb8)    /* unsigned char; {0, 1 = found, 2 = unlocked} */
#define DNG14_SECRETDOOR2               (0x3fb9)    /* unsigned char; {0, 1 = found, 2 = unlocked} */
#define DNG14_SECRETDOOR3               (0x3fba)    /* unsigned char; {0, 1 = found, 2 = unlocked} */
#define DNG14_SECRETDOOR4               (0x3fbb)    /* unsigned char; {0, 1 = found, 2 = unlocked} */
#define DNG14_SECRETDOOR5               (0x3fbc)    /* unsigned char; {0, 1 = found, 2 = unlocked} */
#define DNG14_POISONTRAP                (0x3fbd)    /* unsigned char; {0, 1, 2} */
#define DNG14_LVL2_FIGHT                (0x3fbe)    /* unsigned char; {0,1} */
#define DNG14_BOOTY_FLAG                (0x3fbf)    /* unsigned char; {0,1} */
#define DNG14_TORCHES_FLAG              (0x3fc0)    /* unsigned char; {0,1} */
#define DNG14_SPEAR_FLAG                (0x3fc1)    /* unsigned char; {0,1} */
#define DNG14_HATCHET_FLAG              (0x3fc2)    /* unsigned char; {0,1} */
#define DNG14_RING_FLAG                 (0x3fc3)    /* unsigned char; {0,1} */
#define DNG14_ORKNASE_FLAG              (0x3fc4)    /* unsigned char; {0,1} */
#define DNG14_SPOOKY_FLAG               (0x3fc5)    /* unsigned char; {0,1} */
#define DNG14_CELLAREXIT_FLAG           (0x3fc6)    /* unsigned char; {0,1} */
#define DNG14_CHEST_PANTRY              (0x3fc7)    /* unsigned char[9] */
#define DNG14_CHEST_GEAR                (0x3fd0)    /* unsigned char[13] */
#define DNG14_CHEST_BARREL              (0x3fdd)    /* unsigned char[7] */
#define DNG14_CHEST_X1                  (0x3fe4)    /* unsigned char[7] */
#define DNG14_CHEST_X2                  (0x3feb)    /* unsigned char[16] */
#define DNG14_CHEST_X3                  (0x3ffb)    /* unsigned char[11] */
#define DNG14_CHEST_X4                  (0x4006)    /* unsigned char[14] */
#define DNG14_CHEST_X5                  (0x4014)    /* unsigned char[10] */
#define DNG14_CHEST_X6                  (0x401e)    /* unsigned char[8] */
#define DNG14_CHEST_X7                  (0x4026)    /* unsigned char[7] */
#define DNG14_CHEST_X8                  (0x402d)    /* unsigned char[13] */
#define DNG14_FULLMAP_FLAG              (0x403a)    /* unsigned char; {0,1} */
#define DNG02_CHEST00_CONTENT           (0x403b)    /* unsigned char[2] */
#define DNG02_CHEST01_CONTENT           (0x403d)    /* unsigned char[4] */
#define DNG02_CHEST06_CONTENT           (0x4041)    /* unsigned char[4] */
#define DNG02_CHEST02_CONTENT           (0x4045)    /* unsigned char[8] */
#define DNG02_CHEST03_CONTENT           (0x404d)    /* unsigned char[8] */
#define DNG02_CHEST05_CONTENT           (0x4055)    /* unsigned char[8] */
#define DNG03_CHEST00_CONTENT           (0x405d)    /* signed char[7] */
#define DNG03_CHEST01_CONTENT           (0x4064)    /* signed char[8] */
#define DNG03_CHEST02_CONTENT           (0x406c)    /* signed char[13] */
#define DNG03_CHEST03_CONTENT           (0x4079)    /* signed char[3] */
#define DNG03_CHEST04_CONTENT           (0x407c)    /* signed char[9] */
#define DNG03_CHEST06_CONTENT           (0x4085)    /* signed char[8] */
#define DNG03_CHEST07_CONTENT           (0x408d)    /* signed char[2] */
#define DNG03_CHEST08_CONTENT           (0x408f)    /* signed char[4] */
#define DNG03_CHEST09_CONTENT           (0x4093)    /* signed char[11] */
#define DNG03_CHEST12_CONTENT           (0x409e)    /* signed char[7] */
#define DNG04_CHEST_CORPSE0             (0x40a5)    /* signed char[4] */
#define DNG04_CHEST_CORPSE1             (0x40a9)    /* signed char[12] */
#define DNG04_CHEST_CORPSE2             (0x40b5)    /* signed char[10] */
#define DNG06_CHEST1_CONTENT            (0x40bf)    /* signed char[16] */
#define DNG06_CHEST_DASPTREAS           (0x40cf)    /* signed char[17] */
#define DNG08_CHEST1_CONTENT            (0x40e0)    /* signed char[11] */
#define DNG08_CHEST2_CONTENT            (0x40eb)    /* signed char[2] */
#define DNG08_CHEST4_CONTENT            (0x40ed)    /* signed char[2] */
#define DNG08_CHEST5_CONTENT            (0x40ef)    /* signed char[6] */
#define DNG09_LEVER1_FLAG               (0x40f5)    /* unsigned char; {0,1} */
#define DNG09_BOLTTRAP1_FLAG            (0x40f6)    /* unsigned char; {0,1} */
#define DNG09_BOLTTRAP2_FLAG            (0x40f7)    /* unsigned char; {0,1} */
#define DNG09_BOLTTRAP3_FLAG            (0x40f8)    /* unsigned char; {0,1} */
#define DNG09_CULTIST_FLAG              (0x40f9)    /* unsigned char; {0,1} */
#define DNG09_LEVER2_FLAG               (0x40fa)    /* unsigned char; {0,1} */
#define DNG09_BOLTTRAP4_FLAG            (0x40fb)    /* unsigned char; {0,1} */
#define DNG09_CRYSTAL_FLAG              (0x40fc)    /* unsigned char; {0,1} */
#define DNG09_PIT_FLAG                  (0x40fd)    /* unsigned char; {0,1,2}, number of heroes in pit */
// ?1
#define DNG09_ALTAR_FLAG                (0x40ff)    /* unsigned char; {0,1} */
#define DNG09_SECRETDOOR1               (0x4100)    /* unsigned char; {0, 1 = found, 2 = unlocked} */
#define DNG09_SECRETDOOR2               (0x4101)    /* unsigned char; {0, 1 = found, 2 = unlocked} */
#define DNG09_CHEST0                    (0x4102)    /* unsigned char[12] */
#define DNG09_CHEST1                    (0x410e)    /* unsigned char[10] */
#define DNG09_CHEST2                    (0x4118)    /* unsigned char[8] */
#define DNG09_CHEST3                    (0x4120)    /* unsigned char[21] */
#define DNG09_CHEST5                    (0x4135)    /* unsigned char[4] */
#define DNG09_CHEST6                    (0x4139)    /* unsigned char[7] */
#define DNG10_CORPSE_LOOTED             (0x4140)    /* char; unsgiend {0,1} */
#define DNG10_LEVER_FOUND               (0x4141)    /* unsigned char; {0, 1, 3} ; default = 0 */
#define DNG10_HOLE_STATE                (0x4142)    /* unsigned char; {0, 1, 2} ; default = 0 */
#define DNG10_LEVER_STATE               (0x4143)    /* unsigned char; {0,1} ; default = 0 */
#define DNG10_HOLE_DAMAGE               (0x4144)    /* unsigned char; {0, 1, ..., 255} ; default = 0 */
#define DNG10_FLOORPLATE_FOUND          (0x4145)    /* unsigned char; {0,1} */
#define DNG10_FLOORPLATE_LOADS          (0x4146)    /* unsigned char; {0,1,2,3} ; default = 3 */
#define DNG10_MUMMY_LEVER               (0x4147)    /* char */
#define DNG10_HESHTOT                   (0x4148)    /* char */
#define DNG10_DRAGON_QUEST              (0x4149)    /* char */
#define DNG10_HOARD_PLUNDERED           (0x414a)    /* char */
#define DNG10_CHEST0_CONTENT            (0x414b)    /* char[5] */
#define DNG10_CHEST1_CONTENT            (0x4150)    /* char[4] */
#define DNG10_CHEST2_CONTENT            (0x4154)    /* char[4] */
#define DNG10_CHEST3_CONTENT            (0x4158)    /* char[3] */
#define DNG10_CHEST4_CONTENT            (0x415b)    /* char[2] */
#define DNG11_WATERBARREL1              (0x415d)    /* unsigned char; {0,1} */
#define DNG11_WATERBARREL2              (0x415e)    /* unsigned char; {0,1} */
#define DNG11_EFFERD_SACRIFICE          (0x415f)    /* unsigned char; {0,1} */
#define DNG11_EFFERD_HINT               (0x4160)    /* unsigned char; {0,1} */
#define DNG11_UNKNOWN1_FLAG             (0x4161)    /* unsigned char; {0,1} */
#define DNG11_FIRETRAP1_FLAG            (0x4162)    /* unsigned char; {0,1} */
#define DNG11_UNKNOWN2_FLAG             (0x4163)    /* unsigned char; {0,1} */
#define DNG11_FIRETRAP2_FLAG            (0x4164)    /* unsigned char; {0,1} */
#define DNG11_SECRETDOOR1_FLAG          (0x4165)    /* unsigned char; {0,1} */
#define DNG11_SECRETDOOR2_FLAG          (0x4166)    /* unsigned char; {0,1} */
#define DNG11_SECRETDOOR3_FLAG          (0x4167)    /* unsigned char; {0,1} */
#define DNG11_LEVER_FLAG                (0x4168)    /* unsigned char; {0-5} */
#define DNG11_PROVIANT1_FLAG            (0x4169)    /* unsigned char; {0,1} */
#define DNG11_PROVIANT2_FLAG            (0x416a)    /* unsigned char; {0,1} */
#define DNG11_CHEST3                    (0x416b)    /* unsigned char[15] */
#define DNG11_CHEST4                    (0x417a)    /* unsigned char[11] */
#define DNG11_CHEST5                    (0x4185)    /* unsigned char[5] */
#define DNG11_CHEST6                    (0x418a)    /* unsigned char[7] */
#define DNG11_CHEST7                    (0x4191)    /* unsigned char */
#define DNG11_CHEST8                    (0x4192)    /* unsigned char[6] */
#define DNG11_CHEST9                    (0x4198)    /* unsigned char[6] */
#define DNG11_CHEST10                   (0x419e)    /* unsigned char[11] */
#define DNG11_CHEST11                   (0x41a9)    /* unsigned char[4] */
#define DNG11_CHEST12                   (0x41ad)    /* unsigned char[13] */
#define DNG11_CHEST13                   (0x41ba)    /* unsigned char[6] */
#define DNG11_CHEST14                   (0x41c0)    /* unsigned char[6] */
#define DNG15_TOOK_CURSED_MONEY         (0x41c6)    /* unsigned char; {0,1} */
#define DNG15_UNKNOWN_FLAG              (0x41c7)    /* unsigned char; {0,1} */
#define DNG15_REACHED_HANDS             (0x41c8)    /* unsigned char; {0,1} */
#define DNG15_LEVER_SOUTH               (0x41c9)    /* unsigned char; {0,1} */
#define DNG15_LEVER_NORTH               (0x41ca)    /* unsigned char; {0,1} */
#define DNG15_TOOK_HOE                  (0x41cb)    /* unsigned char; {0,1} */
#define DNG15_CEILINGS_A                (0x41cc)    /* unsigned char[18]; {0, 1, 2, 3, 4} */
#define DNG15_CURSED_MONEY_A            (0x41de)    /* unsigned char[6]; {0,1} */
#define DNG15_UNDEAD_FIGHT              (0x41e4)    /* unsigned char; {0,1} */
#define DNG15_CHEST_EQUIPS              (0x41e5)    /* unsigned char[7] */
#define DNG01_CHEST1_CONTENT            (0x41ec)    /* unsigned char[8] */
#define DNG01_CHEST2_CONTENT            (0x41f4)    /* unsigned char[10] */
#define DNG01_CHEST3_CONTENT            (0x41fe)    /* unsigned char[2] */
#define DNG01_CHEST5_CONTENT            (0x4200)    /* unsigned char[13] */
#define DNG01_CHEST7_CONTENT            (0x420d)    /* unsigned char[5] */
#define UNCONSCIOUS_MESSAGE             (0x4212)    /* unsigned char[7] */
#define FOOD_MESSAGE                    (0x4219)    /* unsigned char[7] */
// ?2
#define CITYINDEX                       (0x4222)    /* short */
#define TYPEINDEX                       (0x4224)    /* short */
#define DNG03_HIGHPRIEST_KILLED         (0x4226)    /* unsigned char; {0, 14 = in fight 224, 16 = in fight 222} */
#define DNG03_CHEST12_LOADS             (0x4227)    /* unsigned char; {0,1,...,6} */
#define TRV_I                           (0x4228)    /* unsigned short */
#define ROUTE_STEPCOUNT                 (0x422a)    /* unsigned short */
#define FORCEDMARCH_LE_COST             (0x422c)    /* unsigned short */
#define ROUTE_TOTAL_STEPS               (0x422e)    /* unsigned short */
#define ROUTE_LENGTH                    (0x4230)    /* unsigned short */
#define ROUTE_DURATION                  (0x4232)    /* unsigned short */
#define ROUTE_TIMEDELTA                 (0x4234)    /* unsigned short */
#define ROUTE_MOUSEHOVER                (0x4236)    /* unsigned short; {0,1} */
#define ROUTE_PROGRESS                  (0x4238)    /* unsigned short */
#define ROUTE_STEPSIZE                  (0x423a)    /* unsigned short */
#define ROUTE_DAYPROGRESS               (0x423c)    /* unsigned short */
#define SEA_TRAVEL_PASSAGE_NO           (0x423e)    /* unsigned short */
#define ROUTE_ENCOUNTER_FLAG            (0x4240)    /* unsigned short; {0,1} */
#define ROUTE_ENCOUNTER_TIME            (0x4242)    /* unsigned short */
#define ROUTE_INFORMER_FLAG             (0x4244)    /* unsigned short; {0,1} */
#define ROUTE_INFORMER_TIME             (0x4246)    /* unsigned short */
#define ROUTE_FIGHT_FLAG                (0x4248)    /* unsigned short; {0,1} */
#define ROUTE_FIGHT_TIME                (0x424a)    /* unsigned short */
#define TRAVEL_SPEED                    (0x424c)    /* unsigned short */
#define PASSAGE_DEADSHIP_FLAG           (0x424e)    /* unsigned short; {0,1} */
#define PASSAGE_DEADSHIP_TIME           (0x4250)    /* unsigned short */
#define PASSAGE_OCTOPUS_FLAG            (0x4252)    /* unsigned short; {0,1} */
#define PASSAGE_OCTOPUS_TIME            (0x4254)    /* unsigned short */
#define PASSAGE_PIRATES_FLAG            (0x4256)    /* unsigned short; {0,1} */
#define PASSAGE_PIRATES_TIME            (0x4258)    /* unsigned short */
#define ROUTE_COURSE_PTR                (0x425a)    /* RealPt */
#define ROUTE_COURSE_START              (0x425e)    /* RealPt */
#define ROUTE_COURSE_PTR2               (0x4262)    /* RealPt */
#define SEA_TRAVEL_COURSES              (0x4266)    /* RealPt */
#define TEVENTS_TAB_PTR                 (0x426a)    /* unsigned long */
#define TRAVEL_ROUTE_PTR                (0x426e)    /* unsigned long */
#define ROUTE_TEVENTS                   (0x4272)    /* struct(4)[15]; struct{short place, event_id;} */
#define SEA_TRAVEL_PSGBOOKED_FLAG       (0x42ae)    /* signed char */
#define SEA_TRAVEL_PSGBOOKED_TIMER      (0x42af)    /* signed char */
#define SEA_TRAVEL_PASSAGE_UNKN1        (0x42b0)    /* signed char */
#define SEA_TRAVEL_PASSAGE_ID           (0x42b1)    /* signed char */
#define SEA_TRAVEL_MENU_PASSAGES        (0x42b2)    /* struct(12)[10] */
#define SEA_TRAVEL_PASSAGE_PRICE        (0x432a)    /* signed short */
#define SEA_TRAVEL_PASSAGE_UNKN2        (0x432c)    /* signed short */
#define TRAVEL_MAP_PTR                  (0x432e)    /* RealPt */
#define FORCEDMARCH_TIMER               (0x4332)    /* unsigned char */
#define TRAVEL_DETOUR                   (0x4333)    /* unsigned char */
#define CURRENT_DIRSIGN                 (0x4334)    /* unsigned short */
#define TRV_RETURN                      (0x4336)    /* signed short; {-1, 0, 1, 2} + ? */
#define TRV_DEST_REACHED                (0x4338)    /* unsigned short */
#define ARRIVAL_X_TARGET                (0x433a)    /* unsigned short */
#define ARRIVAL_Y_TARGET                (0x433c)    /* unsigned short */
#define ARRIVAL_DIRECTION               (0x433e)    /* signed short */
#define TM_UNUSED1_PTR                  (0x4340)    /* RealPt */
#define TRV_MENU_TOWNS                  (0x4344)    /* unsigned char[6] */
#define TRV_DESTINATION                 (0x434a)    /* unsigned short */
#define DNG08_WATERBARREL               (0x434c)    /* signed char; {0,1} */
#define DNG13_COLLAPSECOUNT             (0x434d)    /* unsigned char */
#define DNG13_HEROCOUNT                 (0x434e)    /* unsigned char */
#define CAMP_INCIDENT                   (0x434f)    /* signed short; -1 = not determined or will not happen, 0,1,2 = guard that will be affected */
#define KNOWN_MONSTERS                  (0x4351)    /* char[78] */
#define NAMELESS_DESTROYED              (0x439f)    /* unsigned char; {0,1} */
#define DEADSHIP_FINAL                  (0x43a0)    /* unsigned char; {0,1} */
#define DNG09_LEVER_FAST                (0x43a1)    /* unsigned char; {0,1} */
#define DNG11_SOUP_SUPPLY               (0x43a2)    /* unsigned char */
#define ARSENAL_MONEY                   (0x43a3)    /* signed short; {-1, 0 - 60 } */
#define ANNOUNCE_DAY                    (0x43a5)    /* signed char; UNUSED */
#define KNOWN_PERSONS                   (0x43a6)    /* char[17] */
// ?3
#define DIARY_ENTRY_COUNTER             (0x43ba)    /* signed short */
#define DIARY_ENTRIES                   (0x43bc)    /* struct(8)[23]; struct{short day, month, year, town;} */
#define DATSEG_STATUS_END               (0x4474)    /* char */
#define FADING_STATE                    (0x4475)    /* unsigned char; {0,1,2,3} */
#define MUSIC_ENABLED                   (0x4476)    /* unsigned char; {0,1} */
#define SND_EFFECTS_ENABLED             (0x4477)    /* unsigned char; {0,1} */
// ?2
#define MUSIC_CURRENT_TRACK             (0x447a)    /* signed short */
#define SND_VOC_ENABLED                 (0x447c)    /* unsigned short; {0,1} */
// ?2
#define STR_FILE_MISSING_PTR            (0x4480)    /* RealPt; to STR_FILE_MISSING */
#define MOUSE_IRQ_INIT                  (0x4484)    /* unsigned short */
// ?4
#define PAUSE_STRING                    (0x448a)    /* char[10]; "P A U S E" */
#define NPC_LAST_FAREWELLCHECK          (0x4494)    /* unsigned char */
#define CHECK_PARTY                     (0x4495)    /* char */
#define FOOD_MOD                        (0x4496)    /* char */
#define TRAVEL_HEROKEEPING              (0x4497)    /* unsigned char; {0,1} */
#define FLOOR_FADE_PALETTE              (0x4498)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define BUILDING_FADE_PALETTE           (0x44f8)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define SKY_FADE_PALETTE                (0x4558)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define SPECIAL_SCREEN                  (0x45b8)    /* unsigned char; {0,1}, e.g. diary, treasure map, game options, automap */
#define SPECIAL_DAYS                    (0x45b9)    /* struct(3)[43]; struct{char month, day, id;} */
#define MONTHS_SPRING                   (0x463a)    /* char[4] */
#define MONTHS_WINTER                   (0x463e)    /* char[4] */
#define MONTHS_SUMMER                   (0x4642)    /* char[4] */
#define BANK_HELLER                     (0x4646)    /* signed short */
#define CONSUME_QUIET                   (0x4648)    /* char */
#define HEROKEEPING_FLAG                (0x4649)    /* unsigned char; {0,1,2} */
// ?4
#define LEVEL_AP_TAB                    (0x464e)    /* signed long[21] */
#define DELAY_OR_KEYPRESS_FLAG          (0x46a2)    /* unsigned char; writeonly (0,1) */
#define MERCHANT_ITEMS_POSX             (0x46a3)    /* signed short[15] */
#define MERCHANT_ITEMS_POSY             (0x46c1)    /* signed short[15] */
#define REFRESH_STATUS_LINE             (0x46df)    /* signed char; {0,1} */
#define DSA_VERSION_STRING              (0x46e0)    /* char[12]; "DSA VERSION" */
#define GAME_VERSION                    (0x46ec)    /* char[6]; "V3.02" */
// ?6
#define VERSION_TOKEN1                  (0x46f8)    /* unsigned char; = 3 */
#define VERSION_TOKEN2                  (0x46f9)    /* unsigned char; = 1 */
#define VERSION_TOKEN3                  (0x46fa)    /* unsigned char; = 1 */
#define VERSION_TOKEN4                  (0x46fb)    /* unsigned char; = 1 */
#define QUESTIONS_HANDBOOK              (0x46fc)    /* struct(19)[10] */
#define QUESTIONS_MAP                   (0x47ba)    /* struct(3)[10] */
#define SND_MENU_QUESTION               (0x47d8)    /* char[17]; "BITTE AUSW\x8eHLEN:" */
#define SND_MENU_RADIO1                 (0x47e9)    /* char[12]; "KEINE MUSIK" */
#define SND_MENU_RADIO2                 (0x47f5)    /* char[10]; "NUR MUSIK" */
#define SND_MENU_RADIO3                 (0x47ff)    /* char[12]; "NUR EFFEKTE" */
#define SND_MENU_RADIO4                 (0x480b)    /* char[18]; "MUSIK UND EFFEKTE" */
#define FNAME_SOUND_CFG                 (0x481d)    /* char[10]; "SOUND.CFG" */
#define FNAME_SOUND_ADV2                (0x4827)    /* char[10]; "SOUND.ADV" */
#define FNAME_DIGI_ADV                  (0x4831)    /* char[9]; "DIGI.ADV" */
#define SND_TXT_DISABLED_MEM            (0x483a)    /* char[51]; "SOUND EFFEKTE ABGESCHALTET - NICHT GENUG SPEICHER!" */
#define SND_TXT_HW_NOT_FOUND            (0x486d)    /* char[31]; "SOUND HARDWARE NICHT GEFUNDEN!" */
#define FNAME_SOUND_ADV                 (0x488c)    /* char[10]; "SOUND.ADV" */
#define SND_TXT_HW_NOT_FOUND2           (0x4896)    /* char[31]; == SND_TXT_HW_NOT_FOUND */
#define STR_FILE_MISSING                (0x48b5)    /* char[20]; "FILE %s IS MISSING!" */
// ?1
#define FNAME_SCHICK_DAT                (0x48ca)    /* char[11]; "SCHICK.DAT" */
#define SND_TXT_DISABLED_MEM2           (0x48d5)    /* char[43]; "MUSIK ABGESCHALTET - NICHT GENUG SPEICHER!" */
#define DIARY_STRING1                   (0x4900)    /* char[14]; "%2d-~%-8s~%s." */
#define DIARY_STRING2                   (0x490e)    /* char[17]; "%2d.~%-8s~IN %s." */
#define DIARY_STRING3                   (0x491f)    /* char[27]; "%2d.~%-8s~BESUCHEN VON %s." */
#define DIARY_STRING4                   (0x493a)    /* char[6]; "~~%s." */
#define DIARY_STRING5                   (0x4940)    /* char[9]; "~~IN %s." */
#define DIARY_STRING6                   (0x4949)    /* char[19]; "~~ABENTEUER IN %s." */
#define DIARY_STRING7                   (0x495c)    /* char[19]; "~~RUMH\x8eNGEN IN %s." */
// ?1
//define DIRECTION0                     (0x4970)    /* struct(2)[31]; struct{signed char x,y;} */
//define DIRECTION1                     (0x49ae)    /* struct(2)[31]; struct{signed char x,y;} */
//define DIRECTION2                     (0x49ec)    /* struct(2)[31]; struct{signed char x,y;} */
//define DIRECTION3                     (0x4a2a)    /* struct(2)[31]; struct{signed char x,y;} */
#define NOWAY_STRING                    (0x4a68)    /* char[40]; "IN DIESE RICHTUNG GEHT ES NICHT WEITER." */
#define ANI_BUSY                        (0x4a90)    /* unsigned short; {0,1} */
#define PIC_COPY_FLAG                   (0x4a92)    /* unsigned short; {0,1} */
#define STATUS_BAR_COLORS               (0x4a94)    /* char[4]; {le_color, 0, ae_color, 0} */
// ?2
#define STATUS_PAGE_HUNGER_MAX_COUNTER  (0x4a9a)    /* signed char */
#define STATUS_PAGE_HUNGER_MAX_COLOR    (0x4a9b)    /* signed char */
#define STATUS_PAGE_THIRST_MAX_COUNTER  (0x4a9c)    /* signed char */
#define STATUS_PAGE_THIRST_MAX_COLOR    (0x4a9d)    /* signed char */
#define WALLCLOCK_POS                   (0x4a9e)    /* signed short; position of sun/moon */
#define WALLCLOCK_POS_Y                 (0x4aa0)    /* signed char[81] */
#define WALLCLOCK_PALETTE_DAY           (0x4af1)    /* struct(3)[3]; struct{unsigned char r,g,b;} */
#define WALLCLOCK_PALETTE_NIGHT         (0x4afa)    /* struct(3)[3]; struct{unsigned char r,g,b;} */
#define COLOR_PAL_BLACK                 (0x4b03)    /* char[3]; {0x3f,0x3f,0x3f} */
#define PALETTE_ALLBLACK                (0x4b06)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define DELAY_FACTOR                    (0x4b66)    /* short */
#define STR_TEMP_XX_PTR                 (0x4b68)    /* RealPt; to STR_TEMP_XX */
#define FIG_STAR_COLORS                 (0x4b6c)    /* signed char[12] */
#define FIG_STAR_COUNTER                (0x4b78)    /* signed char */
#define FIG_STAR_TIMER                  (0x4b79)    /* signed short */
#define FIG_STAR_LAST_COUNT             (0x4b7b)    /* unsigned char */
#define FIG_MSG_DTPS                    (0x4b7c)    /* signed short[12] */
#define FIG_STAR_PRINTED                (0x4b94)    /* signed char; {0,1} */
#define STR_TEMP_XX                     (0x4b95)    /* char[8]; "TEMP\XX" */
// ?1
#define FIGHT_FIGS_INDEX                (0x4b9e)    /* signed short */
#define RANDOM_SCHICK_SEED              (0x4ba0)    /* unsigned short */
#define EMM_SIG                         (0x4ba2)    /* char[8] */
#define EMS_FRAME_PTR                   (0x4baa)    /* RealPt */
#define ACTION_TABLE_OPTIONS            (0x4bae)    /* struct(10)[10]; struct{signed short x1, x2, y1, y2; unsigned short action;} */
#define TMAP_X                          (0x4c12)    /* signed short[10] */
#define TMAP_Y                          (0x4c26)    /* signed short[10] */
#define RENDERBUF_IN_USE_FLAG           (0x4c3a)    /* unsigned char; {0,1} */
#define LOCATION_HANDLERS               (0x4c3b)    /* RealPt[19]; (void (*)(void))[19] */
// ?1
#define STR_TEMP_XX_PTR2                (0x4c88)    /* RealPt; to STR_TEMP_XX2 */
#define FNAMES                          (0x4c8c)    /* long[308] */
#define LOADED_HEAD_ID                  (0x515c)    /* unsigned short */
#define STR_TEMP_XX2                    (0x515e)    /* char[8]; "TEMP\XX" */
#define SCHICK_DAT_FNAMES               (0x5166)    /* char[3288]; names of files in SCHICK.DAT */
#define CHR_FILE_SUFFIX                 (0x5e3e)    /* char[5]; ".CHR" */
#define SAVEGAME_SUFFIX                 (0x5e43)    /* char[5]; ".GAM" */
#define ALL_FILES_WILDCARD              (0x5e48)    /* char[4]; "*.*" */
#define ALL_CHR_WILDCARD                (0x5e4c)    /* char[6]; "*.CHR" */
#define EMPTY_STRING1                   (0x5e52)    /* char; "" */
#define EMPTY_STRING2                   (0x5e53)    /* char; "" */
#define SAVEGAME_SUFFIX2                (0x5e54)    /* char[5]; ".gam" */
#define SAVEGAME_SUFFIX3                (0x5e59)    /* char[5]; ".gam" */
#define ALL_CHR_WILDCARD2               (0x5e5e)    /* char[6]; "*.CHR" */
#define ALL_CHR_WILDCARD3               (0x5e64)    /* char[6]; "*.CHR" */
#define PP20_BUFFERS                    (0x5e6a)    /* RealPt[9] */
#define PP20_BUFFER_LENGTHS             (0x5e8e)    /* unsigned long[9] */
#define CURRENT_FIGHT_ID                (0x5eb2)    /* unsigned short */
// ?4
#define LOCATIONS_TAB_SIZE              (0x5eb8)    /* unsigned short */
#define AREADESCR_DNG_LEVEL             (0x5eba)    /* unsigned short */
#define AREADESCR_FILEID                (0x5ebc)    /* unsigned short */
#define AREADESCR_DNG_FLAG              (0x5ebe)    /* unsigned short; {0,1} */
//define EMS?                           (0x5ec0)    /* signed short */
//define EMS?                           (0x5ec2)    /* signed short */
// ?8
#define LOADED_MENU_ICONS               (0x5ecc)    /* signed char[9]; { -2, -2, -2, -2, -2, -2, -2, -2, -2 } */
// ?1
#define INFORMER_TAB                    (0x5ed6)    /* struct(4)[15]; struct{short name_id; char town, unkn;} */
#define FIG_DROPPED_COUNTER             (0x5f12)    /* signed short */
#define FIG_ALL_HEROES_WITHDRAWN        (0x5f14)    /* unsigned short; {0,1} */
#define MAX_ENEMIES                     (0x5f16)    /* signed short; an upper bound for the number of enemies */
#define STR_ERROR_ON_OBJ_MALLOC         (0x5f18)    /* char[23]; "ERROR ON OBJECT MALLOC" */
// ?1
#define FINALFIGHT_TUMULT               (0x5f30)    /* unsigned char; {0,1} */
#define AUTOFIGHT_MAGIC                 (0x5f31)    /* unsigned char */
#define RED_STRING1                     (0x5f32)    /* char[5]; "\xf1%s\xf0" */
#define RED_STRING2                     (0x5f37)    /* char[5]; "\xf1%s\xf0" */
#define SPACE_SEPARATED_STRINGS         (0x5f3c)    /* char[6]; "%s %s" */
#define EMPTY_STRING3                   (0x5f42)    /* char; "" */
#define EMPTY_STRING4                   (0x5f43)    /* char; "" */
#define EMPTY_STRING5                   (0x5f44)    /* char; "" */
// ?1
#define CB_OBJ_NONOBSTACLE              (0x5f46)    /* signed short[27]; { 23,24,25,26,27,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,-1 } */
#define VIEWDIR_OFFSETS1                (0x5f7c)    /* struct(4)[4]; struct{signed short x, y;}; { {1,0}, {0,-1}, {-1,0}, {0,1} } */
#define VIEWDIR_OFFSETS2                (0x5f8c)    /* struct(4)[4]; struct{signed short x, y;}; { {1,0}, {0,-1}, {-1,0}, {0,1} } */
#define STRING_14SPACES                 (0x5f9c)    /* char[15]; "              " */
// ?1
#define AF_SPELL_LIST                   (0x5fac)    /* char[11] */
#define VIEWDIR_OFFSETS3                (0x5fb7)    /* struct(4)[4]; struct{signed short x,y;}; { {1,0}, {0,-1}, {-1,0}, {0,1} } */
#define VIEWDIR_OFFSETS4                (0x5fc7)    /* struct(4)[4]; struct{signed short x,y;}; { {1,0}, {0,-1}, {-1,0}, {0,1} } */
// ?1
#define VIEWDIR_OFFSETS5                (0x5fd8)    /* struct(4)[4]; struct{signed short x,y;}; { {1,0}, {0,-1}, {-1,0}, {0,1} } */
#define VIEWDIR_OFFSETS6                (0x5fe8)    /* struct(4)[4]; struct{signed short x,y;}; { {1,0}, {0,-1}, {-1,0}, {0,1} } */
#define VIEWDIR_INVOFFSETS1             (0x5ff8)    /* struct(4)[4]; struct{signed short x,y;}; { {-1,0}, {0,1}, {1,0}, {0,-1} } */
#define VIEWDIR_OFFSETS7                (0x6008)    /* struct(4)[4]; struct{signed short x,y;}; { {1,0}, {0,-1}, {-1,0}, {0,1} } */
#define GFXTAB_TWOFIELDED_EXTRA_CB      (0x6018)    /* struct(4)[4]; struct{signed short x,y;}; { {-1,0}, {0,1}, {1,0}, {0,-1} } */
#define GFXTAB_TWOFIELDED_EXTRA_OX      (0x6028)    /* signed char[4]; { 10,-10,-10,10 } */
#define GFXTAB_TWOFIELDED_EXTRA_OY      (0x602c)    /* signed char[4]; { 10,-10,-5,-5 } */
#define GFXTAB_TWOFIELDED_X1            (0x6030)    /* signed char[4] */
#define GFXTAB_TWOFIELDED_X2            (0x6034)    /* signed char[4] */
#define GFXTAB_TWOFIELDED_EXTRA_X1      (0x6038)    /* signed char[4] */
#define GFXTAB_TWOFIELDED_EXTRA_X2      (0x603c)    /* signed char[4] */
#define CB_REAR_BORDER                  (0x6040)    /* struct(2)[15]; struct{signed char x,y;}; { {0,16}, {1,16}, {2,16}, {3,16}, {3,17}, {3,18}, {3,19}, {4,20}, {5,20}, {6,20}, {7,20}, {7,21}, {7,22}, {7,23}, {-1,0} } */
#define FIGHTOBJ_COUNT                  (0x605e)    /* unsigned short */
#define GFXTAB_OBJ_OFFSET_X             (0x6060)    /* signed short[63] */
#define GFXTAB_OBJ_OFFSET_Y             (0x60de)    /* signed short[63] */
#define FORCE_WEAPONS                   (0x615c)    /* signed short[9]; terminated with -1 */
#define KNIVE_WEAPONS                   (0x616e)    /* signed short[5]; terminated with -1 */
#define VIEWDIR_INVOFFSETS2             (0x6178)    /* struct(2)[4]; struct{signed char x,y;}; { {-1,0}, {0,1}, {1,0}, {0,-1} } */
#define STRING_CASTS_SPELL              (0x6180)    /* char[14]; "%s ZAUBERT %s" */
#define MSPELL_AWAKE_FLAG               (0x618e)    /* unsigned short; {0,1} */
#define VIEWDIR_INVOFFSETS3             (0x6190)    /* struct(2)[4]; struct{signed char x,y;}; { {-1,0}, {0,1}, {1,0}, {0,-1} } */
#define GFXTAB_SHOTBOLT_OX              (0x6198)    /* unsigned short[24] */
#define GFXTAB_SHOTBOLT_OY              (0x61c8)    /* unsigned short[24] */
#define GFXTAB_SHOTBOLT_NVFNO           (0x61f8)    /* signed short[6]; { 0, 4, 8, 12, 16, 20 } */
#define GFXTAB_SHOTBOLT_HEIGHT          (0x6204)    /* signed short[6]; { 15, 15, 15, 5, 5, 8 } */
#define GFXTAB_SHOTBOLT_WIDTH           (0x6210)    /* signed short[6]; { 25, 25, 25, 9, 9, 16 } */
#define ANITAB_SHOTBOLT_DATA            (0x621c)    /* char[168]; arrays, each terminated by -1 */
#define ANITAB_SHOTBOLT_INDEX2          (0x62c4)    /* RealPt[24]; 6 RealPt arrays of length 4 (viewdir) */
#define ANITAB_SHOTBOLT_INDEX           (0x6324)    /* RealPt[6] */
#define ANITAB_SPELL_NVFNO              (0x633c)    /* signed char[2]; { 9, 27 } */
#define ANITAB_SPELL_OX                 (0x633e)    /* signed short[2]; { 0, 0 } */
#define ANITAB_SPELL_OY                 (0x6342)    /* signed short[2]; { 0, 0 } */
#define ANITAB_SPELL_DATA               (0x6346)    /* char[32]; 2 arrays, each terminated by -1 */
#define ANITAB_SPELL_INDEX              (0x6366)    /* RealPt[2] */
#define STATUSPAGE_SELITEM1_NO          (0x636e)    /* unsigned char */
#define STATUSPAGE_SELITEM2_NO          (0x636f)    /* unsigned char */
#define STATUSPAGE_SELITEM3_NO          (0x6370)    /* unsigned char */
#define STATUSPAGE_SELITEM4_NO          (0x6371)    /* unsigned char */
#define STATUSPAGE_PALETTE              (0x6372)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define INVSLOT_ICONXY_TABLE            (0x63d2)    /* struct(4)[23]; struct{unsigned short x,y;} */
#define STATUSPAGE_SPELLS_XY            (0x642e)    /* struct(6)[8]; struct{unsigned short x_name,x_val,y;} */
#define STATUSPAGE_SPELLS2_XY           (0x645e)    /* struct(6)[4]; struct{unsigned short x_name,x_val,y;} */
#define STATUSPAGE_SKILLS_XY            (0x6476)    /* struct(6)[7]; struct{unsigned short x_name,x_val,y;} */
#define EMPTY_STRING6                   (0x64a0)    /* char; "" */
#define EMPTY_STRING7                   (0x64a1)    /* char; "" */
#define HERO_SEL_EXCLUDE                (0x64a2)    /* signed char */
#define SEG047_INIT1                    (0x64a3)    /* signed short[7]; filled with zeroes */
#define SEG047_INIT2                    (0x64b1)    /* signed short[7]; filled with zeroes */
#define SEG047_INIT3                    (0x64bf)    /* signed short[7]; filled with zeroes */
// ?1
#define INVSLOT_BORDERXY_TABLE          (0x64ce)    /* struct(4)[25]; struct{signed short x,y;} */
#define CHEATSKILL_USABLE               (0x6532)    /* signed short; {0,1} */
#define EXTRASPACE_SEPARATED_STRINGS    (0x6534)    /* char[7]; "%s %s " */
#define EMPTY_STRING8                   (0x653b)    /* char; "" */
#define EXTRASPACE_SEPARATED_STRINGS2   (0x653c)    /* char[7]; "%s %s " */
#define EMPTY_STRING9                   (0x6543)    /* char; "" */
#define AUTOINC_SPELLS_WITCH            (0x6544)    /* signed short[3] */
#define AUTOINC_SPELLS_DRUID            (0x654a)    /* signed short[4] */
#define AUTOINC_SPELLS_MAGE_TABLE       (0x6552)    /* short[108]; 9 arrays, each terminated by -1 */
#define AUTOINC_SPELLS_MAGE_INDEX       (0x662a)    /* RealPt[9]; by magic school */
#define AUOTINC_SPELLS_GELF             (0x664e)    /* signed short[7] */
#define AUTOINC_SPELLS_SELF             (0x665c)    /* signed short[7] */
#define AUTOINC_SPELLS_IELF             (0x666a)    /* signed short[12] */
#define MAGIC_SCHOOL_SPELLRANGES        (0x6682)    /* struct(2)[9]; struct{signed char first, last;} */
#define CAMPFIGHTS                      (0x6694)    /* signed short[4] */
#define GATHER_HERBS_TABLE              (0x669c)    /* struct(4)[13]; struct{unsigned char herb_id,chance,ta_mod,max_count;} */
#define GATHER_HERBS_SPECIAL            (0x66d0)    /* unsigned char */
#define GATHER_HERBS_STR_FOUND          (0x66d1)    /* char[6]; "%d %s" */
#define GATHER_HERBS_STR_COMMA          (0x66d7)    /* char[3]; ", " */
#define GATHER_HERBS_STR_AND            (0x66da)    /* char[6]; " UND " */
#define GATHER_HERBS_STR_DOT            (0x66e0)    /* char[2]; "." */
#define DCAMPFIGHTS                     (0x66e2)    /* signed short[4] */
#define HEALER_DESCR_TABLE              (0x66ea)    /* struct(2)[41]; struct{signed char price,quality;} */
#define INN_DESCR_TABLE                 (0x673c)    /* struct(4)[77]; struct{signed short quality,unkn;} */
#define SHOP_DESCR_TABLE                (0x6870)    /* struct(9)[95]; struct{char price_mod,type,size; short extra_items[3];} */
// ?1
#define BUY_SCREEN_STR_MONEY_H          (0x6bc8)    /* char[5]; "H %d" */
#define BUY_SCREEN_STR_MONEY_S          (0x6bcd)    /* char[5]; "S %d" */
#define BUY_SCREEN_STR_MONEY_D          (0x6bd2)    /* char[5]; "D %d" */
#define BUY_SCREEN_ITEMS_POSX           (0x6bd7)    /* signed short[3]; { 30, 95, 160 } */
#define BUY_SCREEN_ITEMS_POSY           (0x6bdd)    /* signed short[5]; { 35, 55, 75, 95, 115 } */
#define BUY_SCREEN_STR_D_S              (0x6be7)    /* char[6]; "%d %s" */
#define BUY_SCREEN_STR_COMMA_SPACE      (0x6bed)    /* char[3]; ", " */
#define SELL_SCREEN_STR_MONEY_H         (0x6bf0)    /* char[5]; "H %d" */
#define SELL_SCREEN_STR_MONEY_S         (0x6bf5)    /* char[5]; "S %d" */
#define SELL_SCREEN_STR_MONEY_D         (0x6bfa)    /* char[5]; "D %d" */
#define SELL_SCREEN_ITEMS_POSX          (0x6bff)    /* signed short[3]; { 30, 95, 160 } */
#define SELL_SCREEN_ITEMS_POSY          (0x6c05)    /* signed short[5]; { 35, 55, 75, 95, 115 } */
// ?1
#define SMITH_DESCR_TABLE               (0x6c10)    /* signed short[42] */
#define SMITH_STR_MONEY_H               (0x6c64)    /* char[5]; "H %d" */
#define SMITH_STR_MONEY_S               (0x6c69)    /* char[5]; "S %d" */
#define SMITH_STR_MONEY_D               (0x6c6e)    /* char[5]; "D %d" */
#define SMITH_ITEMS_POSX                (0x6c73)    /* signed short[3]; { 30, 95, 160 } */
#define SMITH_ITEMS_POSY                (0x6c79)    /* signed short[5]; { 35, 55, 75, 95, 115 } */
// ?1
#define TAVERN_DESCR_TABLE              (0x6c84)    /* struct(4)[89]; struct{short a,b;} */
#define GOD_TEMPLES_TABLES              (0x6de8)    /* char[78]; 14 arrays, each terminated by -1 */
#define GOD_TEMPLES_INDEX               (0x6e36)    /* RealPt[15] */
#define STR_TEMP_FILE_WILDCARD          (0x6e72)    /* char[8]; "TEMP\%s" */
#define STR_NO_SAVE_IN_TEMPLE           (0x6e7a)    /* char[41]; "IN DIESEM TEMPEL KEIN SPEICHERN M\x99GLICH!" */
// ?1
#define TEMPLE_MIRACLE_BONUS            (0x6ea4)    /* signed char[15]; {0, 2, 15, 10, 20, 5, 10, 1, 15, 3, 15, 5, 10, 0} */
#define TEMPLE_MIRACLE_DICE             (0x6eb3)    /* signed char[15]; {0, 9, 9, 10, 17, 6, 10, 10, 18, 10, 19, 8, 15, 0, 10} */
#define SEA_TRAVEL_TX_CLASS             (0x6ec2)    /* signed short[7] */
#define SEA_TRAVEL_QUALITY_TABLE        (0x6ed0)    /* struct(4)[8]; struct{unsigned char class,unkn,price,speed;} */
#define SEA_TRAVEL_TX_SHIP              (0x6ef0)    /* signed short[8] */
#define SEA_TRAVEL_PASSAGES             (0x6f00)    /* struct(8)[46] */
#define TRAVEL_BY_SHIP                  (0x7070)    /* unsigned char; 0 = on land, 1 = at the ship */
#define SEA_TRAVEL_SLEEPBONUS_TABLE1    (0x7071)    /* signed short[7]; { -2, 0, 5, 4, 3, 1, 0 } */
#define SEA_TRAVEL_SLEEPBONUS_TABLE2    (0x707f)    /* signed short[7]; { -2, 0, 5, 4, 3, 1, 0 } */
#define SEA_TRAVEL_STR_T                (0x708d)    /* char[2]; "T" */
#define SEA_TRAVEL_STR_EN               (0x708f)    /* char[3]; "EN" */
#define SEA_TRAVEL_STR_COMMA            (0x7092)    /* char[3]; ", " */
// ?1
#define SEA_TRAVEL_STR_HELLER           (0x7096)    /* char[10]; "%d^HELLER" */
#define SEA_TRAVEL_STR_NOTHING          (0x70a0)    /* char[7]; "NICHTS" */
// ?1
#define MARKET_DESCR_TABLE              (0x70a8)    /* struct(8)[34]; struct{short min_size,price_mod,market_day,size;} */
#define HYG_ANI_X0                      (0x71b8)    /* unsigned char[5] */
#define HYG_ANI_X1                      (0x71bd)    /* unsigned char */
#define HYG_ANI_X2                      (0x71be)    /* unsigned char */
#define HYG_ANI_X3                      (0x71bf)    /* unsigned char[3] */
#define HYG_ANI_X5                      (0x71c2)    /* unsigned char */
#define HYG_ANI_X6                      (0x71c3)    /* unsigned char */
#define HYG_ANI_X7                      (0x71c4)    /* unsigned char[3] */
#define HYG_ANI_X9                      (0x71c7)    /* unsigned char[3] */
#define TOWNS_CITYINDEX_TABLE           (0x71ca)    /* unsigned char[52] */
#define DASPOTA_LOCLOOT_INDEX           (0x71fe)    /* RealPt[18] */
#define MAPVAL_TO_LOCTYPE               (0x7246)    /* unsigned char[12]; { 0, 15, 31, 63, 95, 127, 159, 175, 191, 207, 223, 239 } */
//define UNKNOWN?                       (0x7252)    /* signed char[29] */
//define UNKNOWN?                       (0x726f)    /* signed char[29] */
//define UNKNOWN?                       (0x728c)    /* signed char[29] */
//define UNKNOWN?                       (0x72a9)    /* signed char[29] */
//define UNKNOWN?                       (0x72c6)    /* signed char[29] */
//define UNKNOWN?                       (0x72e3)    /* signed char[29] */
//define UNKNOWN?                       (0x7300)    /* signed char[29] */
//define UNKNOWN?                       (0x731d)    /* signed char[29] */
//define TEXTURES_BUILDING?             (0x733a)    /* struct(4)[29]; struct{signed short x,y;} */
//define TEXTURES_BUILDING?             (0x73ae)    /* struct(4)[29]; struct{signed short x,y;} */
//define TEXTURES_BUILDING?             (0x7422)    /* struct(4)[29]; struct{signed short x,y;} */
//define TEXTURES_WATER_GRASS?          (0x7496)    /* struct(4)[29]; struct{signed short x,y;} */
//define TEXTURES?                      (0x750a)    /* struct(18)[101] */
//define TEXTURES?                      (0x7c24)    /* signed char[29] */
#define CAN_MERGE_GROUP                 (0x7c41)    /* signed char; {-1,1} */
#define C_EVENT_ACTIVE                  (0x7c42)    /* unsigned char; 0 = default, 1 = city event active */
// ?1
#define STR_BANK_DEPOSIT_TO_BIG         (0x7c44)    /* char[57]; "TUT MIR LEID, ICH AKZEPTIERE MAXIMAL 30000 SILBERM\x9aNZEN!" */
#define STR_OBVIOUSLY_CLOSED            (0x7c7d)    /* char[28]; "OFFENSICHTLICH GESCHLOSSEN!" */
// ?1
#define TMAP_DOUBLE1                    (0x7c9a)    /* unsigned char; 1 = unicorn brought a piece you already have */
#define TMAP_DOUBLE2                    (0x7c9b)    /* unsigned char; 1 = you got a piece you already have from the unicorn */
#define SWAFNILD_TRAVELLED              (0x7c9c)    /* unsigned char; {0,1} */
#define TOWNS_GOSSIP_MAX                (0x7c9d)    /* unsigned char[52] */
#define TOWNS_GOSSIP_OFF                (0x7cd1)    /* unsigned char[52] */
// ?1
#define GOSSIP_STR_TRIPLE_WILDCARD      (0x7d06)    /* char[8]; "%s %s%s" */
#define PALETTE_FIGHT2                  (0x7d0e)    /* struct(3)[20]; struct{unsigned char r,g,b;} */
#define AUTOMAP_BITMASK                 (0x7d4a)    /* unsigned char[8]; { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 } */
#define AUTOMAP_TILE_ARROWUP            (0x7d52)    /* unsigned char[49]; {0,1} */
#define AUTOMAP_TILE_ARROWRIGHT         (0x7d83)    /* unsigned char[49]; {0,1} */
#define AUTOMAP_TILE_CROSS              (0x7db4)    /* unsigned char[49]; {0,1} */
#define AUTOMAP_SELX                    (0x7de5)    /* signed short */
#define AUTOMAP_SELY                    (0x7de7)    /* signed short */
// ?1
#define DNG_GFXTAB_SHIPS                (0x7dea)    /* struct(18)[88] */
#define DNG_GFXTAB_STONES               (0x841a)    /* struct(18)[88] */
#define DNG_GFXTAB_MARBLES              (0x8a4a)    /* struct(18)[88] */
#define DNG_STUB3_UNKN0                 (0x907a)    /* unsigned char[22] */
#define DNG_STUB3_UNKN1                 (0x9090)    /* unsigned char[22] */
#define DNG_STUB3_UNKN2                 (0x90a6)    /* unsigned char[22] */
#define DNG_STUB3_UNKN3                 (0x90bc)    /* unsigned char[22] */
#define DNG_STUB3_UNKN4                 (0x90d2)    /* unsigned char[22] */
#define DNG_STUB3_UNKN5                 (0x90e8)    /* unsigned char[22] */
#define DNG_STUB3_UNKN6                 (0x90fe)    /* unsigned char[22] */
#define DNG_STUB3_UNKN7                 (0x9114)    /* unsigned char[22] */
#define DNG_STUB3_UNKN8                 (0x912a)    /* unsigned char[22] */
#define DNG_STUB3_UNKN9                 (0x9140)    /* unsigned char[22] */
#define DNG_STUB4_UNKN0                 (0x9156)    /* signed short[44] */
#define DNG_STUB4_UNKN1                 (0x91ae)    /* signed short[44] */
#define DNG_STUB4_UNKN2                 (0x9206)    /* signed short[44] */
#define DNG_STUB4_UNKN3                 (0x925e)    /* signed short[44] */
#define DNG_STUB4_UNKN4                 (0x92b6)    /* signed char[22] */
#define DNG_STUB6_STR_COMMA             (0x92cc)    /* char[3]; ", " */
#define DNG_STUB6_STR_AND               (0x92cf)    /* char[6]; " UND " */
// ?1
#define DNG_HANDLERS                    (0x92d6)    /* RealPt[15] */
#define DNG_LEVEL_CHANGED               (0x9312)    /* unsigned short; {1,0} */
#define DNG01_SPECIALCHESTS             (0x9314)    /* struct(21)[9] */
#define DNG01_STR_MARBO                 (0x93d1)    /* char[6] */
// ?1
#define DNG02_SPECIALCHESTS             (0x93d8)    /* struct(21)[8] */
#define DNG03_SPECIALCHESTS             (0x9480)    /* struct(21)[14] */
#define DNG03_STR_SPINNENNETZ           (0x95a6)    /* char[12]; "SPINNENNETZ" */
#define DNG03_STR_MACTANS               (0x95b2)    /* char[8]; "MACTANS" */
#define DNG04_SPECIALCHESTS             (0x95ba)    /* struct(21)[4] */
#define DNG05_TRASH_FLAG                (0x960e)    /* short; {0,1} */
#define DNG06_SPECIALCHESTS             (0x9610)    /* struct(21)[3] */
#define STR_S_WAR                       (0x964f)    /* char[8]; "%s WAR " */
#define STR_S_UND_S_WAREN               (0x9657)    /* char[17]; "%s UND %s WAREN " */
#define DNG07_SPECIALCHESTS             (0x9668)    /* struct(21)[2] */
#define DNG08_SPECIALCHESTS             (0x9692)    /* struct(21)[7] */
#define DNG08_STR_TAIRACH               (0x9725)    /* char[8]; "TAIRACH" */
// ?1
#define DNG09_SPECIALCHESTS             (0x972e)    /* struct(21)[8] */
#define DNG10_SPECIALCHESTS             (0x97d6)    /* struct(21)[6] */
#define DNG11_SPECIALCHESTS             (0x9854)    /* struct(21)[15] */
// ?1
#define DNG14_SPECIALCHESTS             (0x9990)    /* struct(21)[9] */
// ?1
#define DNG15_SPECIALCHESTS             (0x9a4e)    /* struct(21)[33] */
// ?1
#define DNG12_SPECIALCHESTS             (0x9d04)    /* struct(21)[3] */
#define DNG12_OBSTACLE_TRIES            (0x9d43)    /* signed short */
#define DNG12_WATERTRAP_BAK             (0x9d45)    /* signed short */
// ?1
#define DNG13_SPECIALCHESTS             (0x9d48)    /* struct(21)[2] */
// ?18
#define DNG_SPECIALCHEST_INDEX          (0x9d84)    /* RealPt[16] */
#define STR_SINGLE_SPACE                (0x9dc4)    /* char[2]; " " */
#define ROUTES_TAB                      (0x9dc6)    /* struct(9)[59]; struct{char from, to, length, speed_mod, encounters, u1, u2, fights, u3;} */
#define DIRSIGN_ROUTES                  (0x9fd9)    /* char[219]; 105 arrays, each terminated by -1 */
#define DIRECTION_SIGNS                 (0xa0b4)    /* struct(6)[106]; struct{char town, type_id; RealPt routes;} */
#define HARBOR_PASSAGES                 (0xa330)    /* char[115]; 18 arrays, each terminated by -1 */
#define HARBORS                         (0xa3a3)    /* struct(6)[26]; struct{char town, type_id; RealPt passages;} */
#define TOWN_POSITIONS                  (0xa43f)    /* struct(4)[52]; struct{signed short x, y;} */
#define ACTION_TABLE_TRAVELMAP          (0xa50f)    /* struct(10)[35] */
#define TEVENTS_TAB                     (0xa66d)    /* struct(3)[155]; struct{char route_id, place, tevent_id;} */
// ?4
#define TRAVELING                       (0xa842)    /* unsigned char; {0,1} */
#define TEVENTS_REPEATABLE              (0xa843)    /* unsigned char[145]; {0,1} */
#define GRAMMAR_ARTICLES_TABLE          (0xa8d4)    /* char[67]; []{ "DER", "DIE", "DAS", "DES", "DEN", "DEM", "EIN", "EINE", "EINES", "EINER", "EINEN", "EINE", "EINEM", "VON" } */
#define GRAMMAR_ARTICLES_INDEX          (0xa917)    /* RealPt[15] */
#define GRAMMAR_DEF_TABLE               (0xa953)    /* short[24]; [8], by case, then by gender */
#define GRAMMAR_INDEF_TABLE             (0xa983)    /* short[24]; [8], by case, then by gender */
#define GRAMMAR_NOARTICLE_TABLE         (0xa9b3)    /* short[24]; [8], by case, then by gender */
#define STR_S_S_PTR                     (0xa9e3)    /* RealPt; to STR_S_S */
#define STR_VON_S_S_PTR                 (0xa9e7)    /* RealPt; to STR_VON_S_S */
#define GRAMMAR_BUF_NO                  (0xa9eb)    /* unsigned short */
#define GRAMMAR_GENDER_BITMASKS         (0xa9ed)    /* signed short[3]; {0x1000, 0x2000, 0x3000} */
#define GRAMMAR_PRONOUNS_ER             (0xa9f3)    /* char[3]; "ER" */
#define GRAMMAR_PRONOUNS_SIE            (0xa9f6)    /* char[4]; "SIE" */
#define GRAMMAR_PRONOUNS_ES             (0xa9fa)    /* char[3]; "ES" */
#define GRAMMAR_PRONOUNS_SEIN           (0xa9fd)    /* char[5]; "SEIN" */
#define GRAMMAR_PRONOUNS_IHR            (0xaa02)    /* char[4]; "IHR" */
#define GRAMMAR_PRONOUNS_IHN            (0xaa06)    /* char[4]; "IHN" */
#define GRAMMAR_PRONOUNS_IHM            (0xaa0a)    /* char[4]; "IHM" */
#define GRAMMAR_PRONOUNS_IHNEN          (0xaa0e)    /* char[6]; "IHNEN" */
#define GRAMMAR_PRONOUNS_INDEX          (0xaa14)    /* RealPt[7] */
#define GRAMMAR_PRONOUNS_TABLE2         (0xaa30)    /* unsigned char[33]; by gender and ?? */
#define GUI_CHAR_WIDTH                  (0xaa51)    /* struct(3)[75]; struct{char c, font_id, width;} */
#define STR_S_S                         (0xab32)    /* char[6]; "%s %s" */
#define STR_VON_S_S                     (0xab38)    /* char[9]; "VON%s %s" */
// ?1
#define GUI_CHAR_HEIGHT                 (0xab42)    /* struct(3)[67]; struct{char c, font_id, height;} */
#define GUI_BOOL_FLAG                   (0xac0b)    /* unsigned short; {0,1} */
// ?1
#define SPELL_SPECIAL_AECOST            (0xac0e)    /* signed short */
#define ANI_HEAL_PICSTARS               (0xac10)    /* signed short[5]; { 0, 1, 2, 1, 0 } */
#define SPELL_SELECT_STR_KEYVAL         (0xac1a)    /* char[6]; "%s~%d" */
#define SPELL_SELECT_STR_KEY            (0xac20)    /* char[5]; "\xf2%s\xf0" */
#define SPELL_SELECT_STR_KEY_COLOR      (0xac25)    /* char[3]; "%s" */
#define SPELL_SELECT_STR_KEYVAL_COLOR   (0xac28)    /* char[8]; "\xf2%s~%d\xf0 */
#define SPELL_SELECT_ONES               (0xac30)    /* signed char[12]; { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } */
#define ANALUES_ITEMS                   (0xac3c)    /* struct(5)[28]; struct{signed short item_id, barrier; signed char dtp;} */
#define HEXENKNOTEN_GFX_BUF             (0xacc8)    /* RealPt */
#define MONSTER_SPELL_COST              (0xaccc)    /* signed short */
#define SELECT_SKILL_LVLUP              (0xacce)    /* char[6]; "%s~%d" */
#define SELECT_SKILL_DEFAULTS           (0xacd4)    /* signed char[6]; {44, 45, 46, -1, -1, -1} */
#define ALCHEMY_RECIPES                 (0xacda)    /* struct(28)[13] */
#define PREVENT_DROP_EQUIPPED_ITEMS     (0xae46)    /* unsigned char; {0,1} */
// ?1
#define HERO_STARTUP_ITEMS              (0xae48)    /* struct(8)[12] */
#define HERO_STARTUP_ITEMS_ALL          (0xaea8)    /* signed short[4] */
#define USE_SPECIAL_ITEM_HANDLERS       (0xaeb0)    /* long[14] */
#define LIGHT_TYPE                      (0xaee8)    /* unsigned short; 0 = none, 1 = torch, 2 = lantern */
#define TRAVEL_EVENT_HANDLERS           (0xaeea)    /* long[146]; function pointers, */
#define TRAVEL_EVENT_ACTIVE             (0xb132)    /* signed char; {0,1} */
#define TRAVEL_EVENT_TX2                (0xb133)    /* signed short */
#define TRAVEL_EVENT_AXES               (0xb135)    /* signed char[6] */
#define EMPTY_STRING10                  (0xb13b)    /* char; "" */
#define EMPTY_STRING11                  (0xb13c)    /* char; "" */
#define EMPTY_STRING12                  (0xb13d)    /* char; "" */
#define TEVENT014_CORPSE                (0xb13e)    /* struct(22) */
#define TEVENT064_CHEST_DESCR           (0xb154)    /* struct(22) */
#define TEVENT067_CHEST                 (0xb16a)    /* char[8] */
#define TEVENT135_CLIMB_DAMAGE          (0xb172)    /* struct(2)[6]; struct{unsigned char damage_min,damage_max;}; { {0xff,0}, {1,5}, {4,13}, {7,21}, {10,32}, {15,40} } */
#define RANDOM_ENCOUNTER_INDEX          (0xb17e)    /* unsigned char[59] */
#define RANDOM_ENCOUNTER_DESCR          (0xb1b9)    /* struct(7)[14] */
#define TLK_RUIN_HERO_COUNTER           (0xb21b)    /* signed short */
// ?1
#define STR_RAD1                        (0xb21e)    /* char[4]; "RAD" */
#define STR_RAD2                        (0xb222)    /* char[4]; "RAD" */
#define STR_RAD3                        (0xb226)    /* char[4]; "RAD" */
#define COLOR_WHITE                     (0xb22a)    /* unsigned char[3]; { 0x3f, 0x3f, 0x3f } */
#define COLOR_BLACK                     (0xb22d)    /* unsigned char[3]; { 0x00, 0x00, 0x00 } */
#define PALETTE_UNKNOWN2                (0xb230)    /* struct(3)[8]; struct{unsigned char r,g,b;} */
#define PALETTE_UNKNOWN3                (0xb248)    /* struct(3)[3]; struct{unsigned char r,g,b;} */
#define PALETTE_UNKNOWN4                (0xb251)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define PALETTE_GENERAL                 (0xb2b1)    /* struct(3)[32]; struct{unsigned char r,g,b;} */
#define STR_BACKSLASH_TEMP              (0xb311)    /* char[6]; "\\TEMP" */
#define STR_NOT_ENOUGH_MEM              (0xb317)    /* char[124]; "Not enough memory!\x0a\"Realms of Arkania - Blade of Destiny\" needs %ld Byte more memory!\x0aPlease uninstall resident programs!" */
#define STR_DRIVE_X                     (0xb393)    /* char[4]; "X:\" */
#define STR_TEMP_DIR_FAIL               (0xb397)    /* char[280]; "\xf2!!!!! ACHTUNG !!!!!\xf0\x40\x40DAS SCHWARZE AUGE\x40KANN KEIN TEMPOR\x8eRES VERZEICHNIS ANLEGEN UND IST DAHER NICHT LAUFF\x8eHIG. WAHRSCHEINLICH VERSUCHEN SIE, DAS SPIEL AUF EINEM\xf2 SCHREIBGESCH\x9aTZTEN LAUFWERK\xf0\x40ZU STARTEN. BITTE INSTALLIEREN SIE DAS SPIEL AUF EINE FESTPLATTE.\x40\x40-TASTE ZUM BEENDEN-" */
#define ALL_FILES_WILDCARD2             (0xb4af)    /* char[4]; "*.*" */
#define ALL_CHR_WILDCARD4               (0xb4b3)    /* char[6]; "*.CHR" */
#define ALL_FILES_WILDCARD3             (0xb4b9)    /* char[4]; "*.*" */
#define STR_GEN_EXE                     (0xb4bd)    /* char[8]; "gen.exe" */
#define STR_GEN_EXE2                    (0xb4c5)    /* char[8]; "gen.exe" */
#define STR_GEN_B                       (0xb4cd)    /* char[2]; "b" */
#define STR_GEN_A                       (0xb4cf)    /* char[2]; "a" */
#define STR_GEN_N                       (0xb4d1)    /* char[2]; "n" */
#define STR_GEN_1                       (0xb4d3)    /* char[2]; "1" */
#define STR_GEN_GENERATION              (0xb4d5)    /* char[11]; "Generation" */
// ?9
#define CHAR_TYPE_TABLE                 (0xb4e9)    /* signed char[256] */
// ?415
#define BC_FILEHANDLE_FLAGS             (0xb788)    /* unsigned short[600] */
#define CD_AUDIO_PAUSE_POS              (0xbc38)    /* unsigned long */
#define CD_AUDIO_PAUSE_TOD              (0xbc3c)    /* unsigned long */
#define CD_AUDIO_TRACK                  (0xbc40)    /* unsigned short */
// ?8
#define CD_AUDIO_TOD                    (0xbc4a)    /* unsigned long */
#define CD_AUDIO_POS                    (0xbc4e)    /* unsigned long */
#define CD_DRIVE_NO                     (0xbc52)    /* unsigned short */
// ?6
#define SAMPLE_AD_LENGTH                (0xbc5a)    /* unsigned short */
#define SAMPLE_AD_IDX_ENTRY             (0xbc5c)    /* struct(6); struct{signed char; signed char; unsigned long;} */
#define PLAYMASK_US                     (0xbc62)    /* unsigned char; 1 = PLAYM_US, 0 = PLAYM_UK */
#define GFXBUF_WAIT_KEYPRESS            (0xbc63)    /* char[100] */
#define SPLASH_AE                       (0xbcc7)    /* long */
#define SPLASH_LE                       (0xbccb)    /* long */
#define HERO_SPLASH_TIMER               (0xbccf)    /* signed char[7] */
#define SPINLOCK_FLAG                   (0xbcd6)    /* unsigned short; {0,1} */
#define MAP_TOWNMARK_STATE              (0xbcd8)    /* unsigned short; {0, 1, 2, 3, 4} */
#define FREEZE_TIMERS                   (0xbcda)    /* unsigned char; {0,1} */
#define MOUSE_HANDLER_BAK               (0xbcdb)    /* unsigned long */
#define ARCHIVE_FILE_OFFSET             (0xbcdf)    /* unsigned long; start offset in SCHICK.DAT */
#define ARCHIVE_FILE_REMAINING          (0xbce3)    /* unsigned long; flen - off */
#define ARCHIVE_FILE_LENGTH             (0xbce7)    /* unsigned long */
#define AIL_DIGI_DRIVER_BUF2            (0xbceb)    /* RealPt; to buffer of size 5016 */
#define AIL_VOC_BUFFER                  (0xbcef)    /* RealPt */
#define AIL_DIGI_DRIVER_BUF             (0xbcf3)    /* RealPt */
#define AIL_DIGI_DRIVER_DESCR           (0xbcf7)    /* RealPt */
#define AIL_DIGI_DRIVER_ID              (0xbcfb)    /* signed short */
#define USE_CDAUDIO_FLAG                (0xbcfd)    /* unsigned short; {0,1} */
#define LOAD_SOUND_DRIVER               (0xbcff)    /* unsigned short; {0,1} */
#define SAMPLE_AD_HANDLE                (0xbd01)    /* signed short */
#define AIL_TIMBRE_CACHE_SIZE           (0xbd03)    /* unsigned short */
#define AIL_STATE_TABLE_SIZE            (0xbd05)    /* unsigned long */
#define AIL_MUSIC_DRIVER_BUF2           (0xbd09)    /* RealPt */
#define AIL_MIDI_BUFFER                 (0xbd0d)    /* RealPt */
#define AIL_TIMBRE_CACHE                (0xbd11)    /* RealPt */
#define AIL_STATE_TABLE                 (0xbd15)    /* RealPt */
#define AIL_MUSIC_DRIVER_BUF            (0xbd19)    /* RealPt */
#define AIL_MUSIC_DRIVER_DESCR          (0xbd1d)    /* RealPt */
#define AIL_SEQUENCE                    (0xbd21)    /* signed short */
#define AIL_MUSIC_DRIVER_ID             (0xbd23)    /* signed short */
#define PREGAME_STATE                   (0xbd25)    /* unsigned short; {0,1} */
#define CITYCAMP_CITY                   (0xbd27)    /* unsigned char; {0 = camp in dungeon, 1} */
#define CURRENT_FIGHT                   (0xbd28)    /* RealPt */
#define SCENARIO_BUF                    (0xbd2c)    /* long */
#define FIGHTOBJ_BUF                    (0xbd30)    /* RealPt */
#define HEROS                           (0xbd34)    /* long */
#define NEW_MENU_ICONS                  (0xbd38)    /* signed char[9] */
// ?12
#define STEPTARGET_FRONT                (0xbd4d)    /* unsigned char */
#define STEPTARGET_BACK                 (0xbd4e)    /* unsigned char */
#define DIRECTION_UNKN                  (0xbd4f)    /* unsigned char; writeonly (1) */
//define UNKNOWN?                       (0xbd50)    /* signed char[29] */
// ?1
//define UNKNOWN?                       (0xbd6e)    /* unsigned char[29] */
// ?1
//define EMS?                           (0xbd8c)    /* RealPt */
#define EMS_TRAVEL_MAP                  (0xbd90)    /* unsigned short */
//define EMS?                           (0xbd92)    /* unsigned short */
#define DNG_MAP_SIZE                    (0xbd94)    /* unsigned char; {16, 32} */
#define DNG_MAP                         (0xbd95)    /* unsigned char[512] */
#define RADIO_NAME_LIST                 (0xbf95)    /* RealPt[15]; used for items, heroes, spells, skills, recipes */
// ?40
#define GUI_BUFFER_UNKN                 (0xbff9)    /* RealPt */
#define TEXTBOX_WIDTH                   (0xbffd)    /* signed short */
#define TEXTBOX_POS_X                   (0xbfff)    /* signed short; coordinate of upper left corner */
#define TEXTBOX_POS_Y                   (0xc001)    /* signed short; coordinate of upper left corner */
#define GAME_MODE                       (0xc003)    /* signed short; {-1 = Input error, 1 = Beginner, 2 = Advanced } */
#define SELLITEMS                       (0xc005)    /* RealPt; also used for repair items */
#define BUYITEMS                        (0xc009)    /* RealPt; merchant's assortment */
#define PIC_COPY_DST                    (0xc00d)    /* RealPt */
#define PIC_COPY_X1                     (0xc011)    /* signed short */
#define PIC_COPY_Y1                     (0xc013)    /* signed short */
#define PIC_COPY_X2                     (0xc015)    /* signed short */
#define PIC_COPY_Y2                     (0xc017)    /* signed short */
#define PIC_COPY_SRC                    (0xc019)    /* RealPt */
#define PIC_COPY_V1                     (0xc01d)    /* unsigned short */
#define PIC_COPY_V2                     (0xc01f)    /* unsigned short */
#define PIC_COPY_V3                     (0xc021)    /* unsigned short */
#define PIC_COPY_V4                     (0xc023)    /* unsigned short */
#define LOCATIONS_TAB                   (0xc025)    /* struct(6)[150]; struct{short pos; char loc, type; short id;} */
#define BUFFER8_PTR                     (0xc3a9)    /* RealPt; to buffer of size 12008 */
#define TX2_INDEX                       (0xc3ad)    /* RealPt; to index table of secondary text file */
#define TX_INDEX                        (0xc3b1)    /* RealPt; to index table of primary text file */
#define TEXT_LTX_INDEX                  (0xc3b5)    /* RealPt; to index table of TEXT.LTX */
// ?6
#define RANDOM_SCHICK_SEED2             (0xc3bf)    /* signed short */
#define GAME_STATE                      (0xc3c1)    /* signed short; see enum GAME_STATE_* */
// ?2
#define BIOSKEY_EVENT10                 (0xc3c5)    /* unsigned short; {0,1} */
#define HAVE_MOUSE                      (0xc3c7)    /* unsigned short; {0,2} */
#define UNUSED_SPINLOCK_FLAG            (0xc3c9)    /* unsigned short; {0,1} */
#define UPDATE_STATUSLINE               (0xc3cb)    /* unsigned short; {0,1} */
// ?2
#define MOUSE1_DOUBLECLICK              (0xc3cf)    /* unsigned short; {0,1} */
#define MOUSE1_EVENT1                   (0xc3d1)    /* unsigned short; {0,1} */
#define MOUSE2_EVENT                    (0xc3d3)    /* unsigned short; {0,1} */
#define MOUSE1_EVENT2                   (0xc3d5)    /* unsigned short; {0,1} */
#define BIOSKEY_EVENT                   (0xc3d7)    /* unsigned short */
#define ACTION                          (0xc3d9)    /* unsigned short */
#define BUFFER9_PTR                     (0xc3db)    /* RealPt; to buffer of size 180000 (or 203000 if LARGE_BUF), used for NVF */
// ?8
#define ANI_WIDTH                       (0xc3e7)    /* unsigned short */
#define ANI_UNKNOWN1                    (0xc3e9)    /* unsigned short */
#define ANI_UNKNOWN2                    (0xc3eb)    /* unsigned short */
#define ANI_HEIGHT                      (0xc3ed)    /* unsigned char */
#define ANI_AREACOUNT                   (0xc3ee)    /* unsigned char */
#define ANI_AREA_TABLE                  (0xc3ef)    /* struct(263)[10] */
#define ANI_MAIN_PTR                    (0xce35)    /* RealPt */
#define ANI_COMPR_FLAG                  (0xce39)    /* unsigned char; {0,1 = compressed} */
#define ANI_PALETTE_SIZE                (0xce3a)    /* unsigned char */
#define ANI_PALETTE                     (0xce3b)    /* RealPt */
#define ANI_POSY                        (0xce3f)    /* signed short */
#define ANI_POSX                        (0xce41)    /* signed short */
#define ANI_UNKNOWN4                    (0xce43)    /* unsigned long; writeonly (0) */
// ?64
#define GUI_TEXT_BUFFER                 (0xce87)    /* unsigned char[64] */
#define LAST_CURSOR                     (0xcec7)    /* RealPt */
#define CURRENT_CURSOR                  (0xcecb)    /* RealPt */
#define GGST_CURSOR                     (0xcecf)    /* struct(64); struct{char[32]; unsigned short mask[16];} */
#define MOUSE_BG_BAK                    (0xcf0f)    /* unsigned char[256] */
#define DNG_INIT_FLAG                   (0xd00f)    /* unsigned short; {0,1} */
#define DNG_EXTRA_ACTION                (0xd011)    /* unsigned short; {0 = warehouse,1 = open door,2 = open chest,3 = close door,4 = lever,5 = smash door} */
#define REDRAW_MENUICONS                (0xd013)    /* unsigned short; {0,1} */
#define BUFFER9_PTR2                    (0xd015)    /* RealPt; copy of BUFFER9_PTR */
#define BUFFER9_PTR3                    (0xd019)    /* RealPt; copy of BUFFER9_PTR */
#define BUFFER_MONSTER_TAB              (0xd01d)    /* unsigned long[36] */
#define BUFFER_WFIGS_TAB                (0xd0ad)    /* unsigned long[43] */
#define BUFFER_MFIGS_TAB                (0xd159)    /* unsigned long[43] */
#define BUFFER_ANIS_TAB                 (0xd205)    /* unsigned long[37] */
#define TRV_TRACK_PIXEL_BAK             (0xd299)    /* RealPt; to buffer of size 500 */
#define FIG_STAR_GFX                    (0xd29d)    /* RealPt */
#define MONNAMES_BUFFER                 (0xd2a1)    /* RealPt */
#define BUFFER5_PTR                     (0xd2a5)    /* RealPt; to buffer of size 3880 */
#define BUFFER10_PTR                    (0xd2a9)    /* RealPt; to buffer of size 16771, used for NVF and text */
#define POPUP                           (0xd2ad)    /* RealPt */
#define BUFFER6_PTR                     (0xd2b1)    /* RealPt; to buffer of size 2200, used for NVF */
#define BUFFER7_PTR                     (0xd2b5)    /* RealPt; to buffer of size 10000, used for NVF and text */
#define TEXT_LTX_BUFFER                 (0xd2b9)    /* RealPt; to buffer of size 30500 */
#define SPLASH_BUFFER                   (0xd2bd)    /* RealPt; to buffer of size 1000, used for Splashes */
#define BUF_FONT6                       (0xd2c1)    /* RealPt */
#define TEXTCOLOR                       (0xd2c5)    /* short */
#define TEXTCOLOR_BG                    (0xd2c7)    /* short */
#define TEXTCOLOR_FG                    (0xd2c9)    /* short */
#define INIT_COLOR1                     (0xd2cb)    /* unsigned short; writeonly (0xc8) */
#define INIT_COLOR2                     (0xd2cd)    /* unsigned short; writeonly (0xc9) */
#define INIT_COLOR3                     (0xd2cf)    /* unsigned short; writeonly (0xca) */
#define GUI_TEXT_CENTERED               (0xd2d1)    /* signed short; {0,1} */
#define TEXTLINE_UNKNOWN                (0xd2d3)    /* unsigned short; writeonly (103) */
#define TEXTLINE_MAXLEN                 (0xd2d5)    /* unsigned short */
#define TEXTLINE_POSY                   (0xd2d7)    /* unsigned short */
#define TEXTLINE_POSX                   (0xd2d9)    /* unsigned short */
#define FIG_FIGURE2_BUF                 (0xd2db)    /* RealPt; to buffer of size 20000 */
#define FIG_FIGURE1_BUF                 (0xd2df)    /* RealPt */
#define OBJECTS_NVF_BUF                 (0xd2e3)    /* RealPt; to buffer of size 3400 */
#define BUF_ICON                        (0xd2e7)    /* RealPt */
#define TEXT_OUTPUT_BUF                 (0xd2eb)    /* RealPt; to buffer of size 300 */
#define TEXT_INPUT_BUF                  (0xd2ef)    /* RealPt; to buffer of size 24 */
#define DTP2                            (0xd2f3)    /* long */
#define ICON                            (0xd2f7)    /* RealPt */
#define PRINT_STRING_BUFFER             (0xd2fb)    /* RealPt */
#define FRAMEBUF_PTR                    (0xd2ff)    /* RealPt */
#define RENDERBUF_PTR                   (0xd303)    /* RealPt; to buffer of size 65000 */
// ?2
#define GAMEINIT_FLAG                   (0xd309)    /* unsigned short; writeonly (1) */
#define VIDEO_PAGE_BAK                  (0xd30b)    /* signed short */
#define VIDEO_MODE_BAK                  (0xd30d)    /* signed short */
// ?4
#define TXT_TABPOS1                     (0xd313)    /* signed short */
#define TXT_TABPOS2                     (0xd315)    /* signed short */
#define TXT_TABPOS3                     (0xd317)    /* signed short */
#define TXT_TABPOS4                     (0xd319)    /* signed short */
#define TXT_TABPOS5                     (0xd31b)    /* signed short */
#define TXT_TABPOS6                     (0xd31d)    /* signed short */
#define TXT_TABPOS7                     (0xd31f)    /* signed short */
#define TOWNPAL_BUF                     (0xd321)    /* RealPt */
#define FIG_FLEE_POSITION               (0xd325)    /* signed short[4]; see HERO_UNKNOWN9 */
#define WILDCAMP_SLEEP_QUALITY          (0xd32d)    /* signed short */
#define GATHER_HERBS_MOD                (0xd32f)    /* signed short */
#define REPLENISH_STOCKS_MOD            (0xd331)    /* signed short */
#define FIG_MSG_DATA                    (0xd333)    /* struct(4)[6]; struct{signed short type, damage;} */
#define ENEMY_SHEETS                    (0xd34b)    /* struct(62)[20]; struct enemy[20] */
#define FIG_MOVE_PATHDIR                (0xd823)    /* signed char[10] */
//define FIG_UNKNOWN?                   (0xd82d)    /* signed char[10]; see FIG_ACTION_UNKNOWN2 */
#define FIG_MONSTERS_UNKN               (0xd837)    /* unsigned char[20]; writeonly (0) */
#define HERO_IS_TARGET                  (0xd84b)    /* signed char[7] */
#define CHESSBOARD                      (0xd852)    /* RealPt */
#define FIG_SPELLGFX_BUF                (0xd856)    /* RealPt */
#define FIG_SHOT_BOLT_BUF               (0xd85a)    /* RealPt */
#define FIG_CB_SELECTOR_BUF             (0xd85e)    /* RealPt */
#define FIG_CB_MARKER_BUF               (0xd862)    /* RealPt */
#define SPELLOBJ_NVF_BUF                (0xd866)    /* RealPt; to buffer of size 0xf5f */
#define WEAPONS_NVF_BUF                 (0xd86a)    /* RealPt; to buffer of size 0x1953 */
#define FIGHTOBJ_BUF_SEEK_PTR           (0xd86e)    /* RealPt; points to end of FIGHTOBJ buffer */
#define NR_OF_ENEMIES                   (0xd872)    /* short; ? */
#define FIGHTOBJ_LIST                   (0xd874)    /* unsigned char[90] */
#define FIG_ANISHEETS                   (0xd8ce)    /* struct(243)[8] */
#define FIG_LIST_ELEM                   (0xe066)    /* struct(35) */
#define FIG_LIST_ARRAY                  (0xe089)    /* unsigned char[127] */
#define FIG_LIST_HEAD                   (0xe108)    /* RealPt; to a list */
#define LOCATION_MARKET_FLAG            (0xe10c)    /* unsigned char; {0,1} */
#define WALLCLOCK_REDRAW                (0xe10d)    /* unsigned short; {0,1} */
#define WALLCLOCK_Y                     (0xe10f)    /* short */
#define WALLCLOCK_X                     (0xe111)    /* short */
#define WALLCLOCK_UPDATE                (0xe113)    /* unsigned short; 0 = don't update the wallclock */
#define MEM_SLOTS_MON                   (0xe115)    /* long */
#define MEM_SLOTS_WFIG                  (0xe119)    /* long */
#define MEM_SLOTS_MFIG                  (0xe11d)    /* long */
#define MEM_SLOTS_ANIS                  (0xe121)    /* unsigned long; to (struct { unsigned short no, ems_handle; long ani_len; })[36] */
#define MONSTER_DAT_BUF                 (0xe125)    /* RealPt */
#define MONNAMES_INDEX                  (0xe129)    /* RealPt */
#define MARKET_ITEMSALDO_TABLE          (0xe12d)    /* signed char[254] */
#define ITEMSDAT                        (0xe22b)    /* long */
#define ITEMSNAME                       (0xe22f)    /* long */
// ?1
#define GFX_SPINLOCK                    (0xe234)    /* signed long */
#define ANI_CHANGE_DIR                  (0xe238)    /* signed short[10] */
#define ANI_AREA_STATUS                 (0xe24c)    /* signed short[10] */
#define ANI_AREA_TIMEOUT                (0xe260)    /* signed short[10] */
#define BC_TIMER                        (0xe274)    /* RealPt */
#define FIG_GFXBUFFERS                  (0xe278)    /* RealPt[8]; 0x508 byte segments in FIGHTOBJ_BUF */
#define FIG_FIGLIST_READD               (0xe298)    /* signed short[8] */
#define FIG_ANI_STATE                   (0xe2a8)    /* unsigned short[8] */
#define FIG_ACTOR_GRAMMAR_TYPE          (0xe2b8)    /* unsigned short; 2 = hero, 1 = monster */
#define FIG_ACTOR_GRAMMAR_ID            (0xe2ba)    /* unsigned short */
#define FIG_TARGET_GRAMMAR_TYPE         (0xe2bc)    /* unsigned short; 2 = hero, 1 = monster */
#define FIG_TARGET_GRAMMAR_ID           (0xe2be)    /* unsigned short */
#define FIGOBJ_UNKN_Y1                  (0xe2c0)    /* unsigned short */
#define FIGOBJ_UNKN_Y1_BAK              (0xe2c2)    /* unsigned short */
#define FIGOBJ_UNKN_X1                  (0xe2c4)    /* unsigned short */
#define FIGOBJ_UNKN_X1_BAK              (0xe2c6)    /* unsigned short */
#define FIGOBJ_UNKN_Y2                  (0xe2c8)    /* unsigned short */
#define FIGOBJ_UNKN_Y2_BAK              (0xe2ca)    /* unsigned short */
#define FIGOBJ_UNKN_X2                  (0xe2cc)    /* unsigned short */
#define FIGOBJ_UNKN_X2_BAK              (0xe2ce)    /* unsigned short */
#define DELAY_TIMER                     (0xe2d0)    /* unsigned short */
#define SAVED_FILES_BUF                 (0xe2d2)    /* RealPt */
#define LAST_SAVE_TIME                  (0xe2d6)    /* signed long */
#define SAVEGAME_NAMES                  (0xe2da)    /* char[45] */
// ?1
#define DIALOG_TITLE                    (0xe308)    /* RealPt */
#define DIALOG_INFORMER                 (0xe30c)    /* signed short */
#define DIALOG_NEXT_STATE               (0xe30e)    /* signed short */
#define DIALOG_DONE                     (0xe310)    /* signed short; {0,1} */
#define DIALOG_STATE                    (0xe312)    /* signed short */
#define TLK_ID                          (0xe314)    /* signed short */
#define CURRENT_FIG_NO                  (0xe316)    /* unsigned short */
#define AUTOFIGHT                       (0xe318)    /* signed short */
#define FIG_DROPPED_WEAPONS             (0xe31a)    /* signed short[30] */
#define CHESSBOARD_CPY                  (0xe356)    /* RealPt */
#define FIG_TWOFIELDED_TABLE            (0xe35a)    /* signed char[21] */
#define FIG_TWOFIELDED_COUNT            (0xe36f)    /* signed char */
#define FIGHTOBJ_BUF_FREESPACE          (0xe370)    /* signed long */
#define BUFFER_WEAPANIDAT               (0xe374)    /* RealPt; pointer to WEAPANI.DAT */
#define BUFFER_ANIDAT                   (0xe378)    /* RealPt; pointer to ANI.DAT buffer */
#define FIG_LIST_BUFFER                 (0xe37c)    /* RealPt; to buffer of size 4445, initial value of FIG_LIST_HEAD */
#define FIGOBJ_GFXHEIGHT_TABLE          (0xe380)    /* RealPt; to signed short[63] */
#define FIGOBJ_GFXWIDTH_TABLE           (0xe384)    /* RealPt; to signed short[63] */
#define FIGOBJ_GFXBUF_TABLE             (0xe388)    /* RealPt; to RealPt[63] */
#define FIG_SPELLGFX_ID                 (0xe38c)    /* unsigned char */
#define FIG_SHOT_BOLT_ID                (0xe38d)    /* unsigned char */
#define FIG_CB_MAKRER_ID                (0xe38e)    /* unsigned char */
#define FIG_CB_SELECTOR_ID              (0xe38f)    /* unsigned char[20] */
// ?1
#define SPELL_ILLUSIONEN                (0xe3a4)    /* signed short; 1 = spell has effect */
#define DEFENDER_DEAD                   (0xe3a6)    /* signed short; {0,1} */
#define ATTACKER_DEAD                   (0xe3a8)    /* signed short; {0,1} */
#define DEFENDER_ATTACKS                (0xe3aa)    /* unsigned short; {0,1} */
#define ATTACKER_ATTACKS_AGAIN          (0xe3ac)    /* unsigned short; {0,1} */
#define CURRENT_CURSOR_BAK              (0xe3ae)    /* RealPt */
#define INC_SPELLS_COUNTER              (0xe3b2)    /* RealPt */
#define INC_SKILLS_COUNTER              (0xe3b6)    /* RealPt */
#define SKILLS_BUFFER                   (0xe3ba)    /* RealPt */
#define WILDCAMP_GUARDS                 (0xe3be)    /* signed char[3] */
#define WILDCAMP_HERBSTATUS             (0xe3c1)    /* signed char[7] */
#define WILDCAMP_REPLSTATUS             (0xe3c8)    /* signed char[7] */
#define WILDCAMP_MAGICSTATUS            (0xe3cf)    /* signed char[7] */
#define WILDCAMP_GUARDSTATUS            (0xe3d6)    /* signed char[7] */
// ?1
#define CITYCAMP_GUARDS                 (0xe3de)    /* signed char[3] */
#define CITYCAMP_MAGICSTATUS            (0xe3e1)    /* signed char[7] */
#define CITYCAMP_GUARDSTATUS            (0xe3e8)    /* signed char[7] */
// ?1
#define BOOKED_INN_DAYS                 (0xe3f0)    /* signed char */
#define SLEEP_QUALITY                   (0xe3f1)    /* signed char */
#define BUY_SHOPPING_CART               (0xe3f2)    /* RealPt; to buffer of size 250, each item has 4 bytes */
#define PRICE_MODIFICATOR               (0xe3f6)    /* signed short; price modificator for smith and sell */
#define TEMPLE_GOD                      (0xe3f8)    /* signed short; id of current temple's god */
#define SEA_TRAVEL_SLEEP_QUALITY        (0xe3fa)    /* unsigned char */
// ?1
#define BUFFER9_PTR4                    (0xe3fc)    /* RealPt; copy of BUFFER9_PTR */
//define 066_172b?                      (0xe400)    /* unsigned char[4]; dng map border index count? */
#define BUFFER11_PTR                    (0xe404)    /* RealPt; to buffer of size 192, used for palettes */
#define CITY_REFRESH_DIRECTION          (0xe408)    /* signed short */
#define CITY_REFRESH_Y_TARGET           (0xe40a)    /* signed short */
#define CITY_REFRESH_X_TARGET           (0xe40c)    /* signed short */
#define ALWAYS_ZERO1                    (0xe40e)    /* unsigned short; writeonly (0) */
#define ALWAYS_ZERO2                    (0xe410)    /* unsigned short; writeonly (0) */
#define ENTRANCE_ANGLE                  (0xe412)    /* signed short */
#define TEX_FLOOR                       (0xe414)    /* RealPt */
#define TEX_SKY                         (0xe418)    /* RealPt */
#define TEX_HOUSE1                      (0xe41c)    /* RealPt */
#define TEX_HOUSE2                      (0xe420)    /* RealPt */
#define TEX_HOUSE3                      (0xe424)    /* RealPt */
#define TEX_HOUSE4                      (0xe428)    /* RealPt */
#define COMBO_MODE                      (0xe42c)    /* signed short; {0,1,2} */
#define STR_BEORN_HJALL                 (0xe42e)    /* char[20] */
#define AUTOMAP_BUF                     (0xe442)    /* unsigned char[64] */
#define DNG_REFRESH_DIRECTION           (0xe482)    /* unsigned short */
#define DNG_REFRESH_Y_TARGET            (0xe484)    /* unsigned short */
#define DNG_REFRESH_X_TARGET            (0xe486)    /* unsigned short */
#define DNG_MAP_PTR                     (0xe488)    /* RealPt; to DNG_MAP */
//define DNG?                           (0xe48c)    /* unsigned char; {0,1}, changing every timestep */
//define DNG?                           (0xe48d)    /* RealPt; to AUTOMAP_SHIPS, AUTOMAP_STONES or AUTOMAP_MARBLES */
// ?1
#define LOCKPICK_TRY_COUNTER            (0xe492)    /* signed short; {0..4} */
#define DUNGEON_FIGHTS_BUF              (0xe494)    /* RealPt; to buffer of size 630 */
#define DUNGEON_STAIRS_BUF              (0xe498)    /* RealPt; to buffer of size 80 */
#define DUNGEON_DOORS_BUF               (0xe49c)    /* RealPt; to buffer of size 225 */
#define GET_EXTRA_LOOT                  (0xe4a0)    /* signed short; {0,1} */
#define ROUTE59_FLAG                    (0xe4a2)    /* unsigned char; {0, 1 = from Kravik, 2 = from Peilinen, 3 = from Skelellen, 4 = from Rovamund} */
#define TRV_MENU_SELECTION              (0xe4a3)    /* unsigned short */
#define CURRENT_TOWN_OVER               (0xe4a5)    /* unsigned short; {0,1} */
#define CURRENT_TOWN_OVERY              (0xe4a7)    /* unsigned short */
#define CURRENT_TOWN_OVERX              (0xe4a9)    /* unsigned short */
#define SELECTED_TOWN_ANIY              (0xe4ab)    /* unsigned short */
#define SELECTED_TOWN_ANIX              (0xe4ad)    /* unsigned short */
#define CURRENT_TOWN_ANIY               (0xe4af)    /* unsigned short */
#define CURRENT_TOWN_ANIX               (0xe4b1)    /* unsigned short */
// ?1
#define TRV_DETOUR_PIXEL_BAK            (0xe4b4)    /* unsigned char[20] */
#define GOOD_CAMP_PLACE                 (0xe4c8)    /* unsigned char */
#define ROUTE_TEVENT_FLAGS              (0xe4c9)    /* unsigned char[15] */
#define GUI_PRINT_CHAR                  (0xe4d8)    /* unsigned char */
#define DIALOGBOX_INDENT_HEIGHT         (0xe4d9)    /* unsigned short */
#define DIALOGBOX_INDENT_WIDTH          (0xe4db)    /* signed short */
// ?6
#define GRAMMAR_BUFS                    (0xe4e3)    /* char[201] */
#define MENU_DEFAULT_SELECT             (0xe5ac)    /* unsigned short */
#define MENU_INPUT_BUSY                 (0xe5ae)    /* unsigned short */
#define MENU_SELECTED                   (0xe5b0)    /* unsigned short */
#define SPELLTEST_RESULT                (0xe5b2)    /* signed short */
#define SPELLTARGET_E                   (0xe5b4)    /* RealPt; Pointer to enemy */
#define SPELLTARGET                     (0xe5b8)    /* long */
#define SPELLUSER                       (0xe5bc)    /* long */
#define SPELLUSER_E                     (0xe5c0)    /* long */
#define ALCHEMY_MISSING_ITEM            (0xe5c4)    /* short */
#define USED_ITEM_DESC                  (0xe5c6)    /* RealPt; pointer to the item description */
#define USED_ITEM_ID                    (0xe5ca)    /* signed short; used_item ID */
#define USED_ITEM_POS                   (0xe5cc)    /* signed short; used_item position */
#define ITEMUSER                        (0xe5ce)    /* RealPt; pointer to hero */
#define EVENT_ANI_BUSY                  (0xe5d2)    /* unsigned char; {0,1} */
#define WALLCLOCK_UPDATE_BAK            (0xe5d3)    /* unsigned short; {0,1} */
#define TEXTBOX_WIDTH_BAK               (0xe5d5)    /* unsigned short */
#define BASEPOS_Y_BAK                   (0xe5d7)    /* unsigned short */
#define BASEPOS_X_BAK                   (0xe5d9)    /* unsigned short */
// ?1
#define BUFFERSIZE                      (0xe5dc)    /* unsigned long; size of the global buffer */
#define GLOBAL_BUFFER_PTR               (0xe5e0)    /* RealPt; points to the start of the global buffer */
#define LARGE_BUF                       (0xe5e4)    /* signed char; {0,1} */
/* Unknown Bytes: 831, Undetermined symbols: 30/1479 */

#endif
