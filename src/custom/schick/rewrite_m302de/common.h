#if !defined(COMMON_H)
#define COMMON_H
/**
	struct nvf_desc - nvf descriptor
	@src:	pointer to a buffer containing the nvf file
	@dst:	pointer where to extract the picture
	@nr:	number of the picture to extract
	@type:	kind of compression / direction (0 = PP20 / 2-5 RLE / copy)
	@p_height:	pointer where the height of the picture must be stored
	@p_width:	pointer where the width of the picture must be stored
*/
struct nvf_desc {
	Bit8u *dst;
	Bit8u *src;
	short nr;
	signed char type;
	Bit8u *width;
	Bit8u *height;
};

/**
 *	struct screen_rect - coordinates for a rectangle on the screen
 *	@y1:	upper Y-Coordinate
 *	@x1:	upper X-Coordinate
 *	@y2:	lower Y-Coordinate
 *	@x2:	lower X-Coordinate
 */
struct screen_rect {
	short y1, x1, y2, x2;
};

/**
 *	struct hero_status - status of the hero
 *	@dead:		1 = dead		/ 0 = not dead
 *	@sleeps:	1 = sleeps		/ 0 = awake
 *	@stoned:	1 = stoned		/ 0 = not stoned
 *	@unkn1:		yet unknown, maybe unused
 *	@cham:		1 = Chamaelioni active	/ 0 = not active
 *	@cursed:	1 = cursed		/ 0 = not cursed
 *	@uncon:		1 = unconscious		/ 0 = conscious
 *	@unkn2:		yet unknown, maybe unused
 *	@unkn3:		yet unknown, but used
 *	@dup:		1 = Duplicatus active	/ not active
 *	@dummy1:	yet unknown, maybe unused
 *	@dummy2:	yet unknown, maybe unused
 *	@dummy3:	yet unknown, maybe unused
 *	@dummy4:	yet unknown, maybe unused
 *	@transf:	1 = hero is transformed / 0 not transformed
 *	@dummy6:	yet unknown, maybe unused
 */
struct hero_status {
	/* hero + 0xaa */
	unsigned short dead	:1;
	unsigned short sleeps	:1;
	unsigned short stoned	:1;
	unsigned short busy	:1;
	unsigned short cham	:1;
	unsigned short cursed	:1;
	unsigned short uncon	:1;
	unsigned short unkn2	:1;
	/* hero + 0xab */
	unsigned short unkn3	:1;
	unsigned short dup	:1;
	unsigned short dummy1	:1;
	unsigned short dummy2	:1;
	unsigned short dummy3	:1;
	unsigned short dummy4	:1;
	unsigned short transf	:1;
	unsigned short dummy6	:1;
};

enum {
    HERO_NAME           = 0x000,
    HERO_NAME2          = 0x010,
    HERO_KS_TAKEN       = 0x020,
    HERO_TYPE           = 0x021,
    HERO_SEX            = 0x022,
    HERO_HEIGHT         = 0x023,
    HERO_WEIGHT         = 0x024,
    HERO_GOD            = 0x026,
    HERO_LEVEL          = 0x027,
    HERO_AP             = 0x028,
    HERO_MONEY          = 0x02C, /* Heller */
    HERO_RS_BONUS1      = 0x030, /* RS-Bonus = RS-Bonus1 + RS-Bonus2 */
    HERO_RS_BONUS2      = 0x031,
    HERO_RS_BE          = 0x032,
    HERO_BP_LEFT        = 0x033,
    HERO_MU_ORIG        = 0x034,
    HERO_MU             = 0x035,
    HERO_MU_MOD         = 0x036,
    HERO_KL_ORIG        = 0x037,
    HERO_KL             = 0x038,
    HERO_KL_MOD         = 0x039,
    HERO_CH_ORIG        = 0x03A,
    HERO_CH             = 0x03B,
    HERO_CH_MOD         = 0x03C,
    HERO_FF_ORIG        = 0x03D,
    HERO_FF             = 0x03E,
    HERO_FF_MOD         = 0x03F,
    HERO_GE_ORIG        = 0x040,
    HERO_GE             = 0x041,
    HERO_GE_MOD         = 0x042,
    HERO_IN_ORIG        = 0x043,
    HERO_IN             = 0x044,
    HERO_IN_MOD         = 0x045,
    HERO_KK_ORIG        = 0x046,
    HERO_KK             = 0x047,
    HERO_KK_MOD         = 0x048,
    HERO_AG_ORIG        = 0x049,
    HERO_AG             = 0x04A,
    HERO_AG_MOD         = 0x04B,
    HERO_HA_ORIG        = 0x04C,
    HERO_HA             = 0x04D,
    HERO_HA_MOD         = 0x04E,
    HERO_RA_ORIG        = 0x04F,
    HERO_RA             = 0x050,
    HERO_RA_MOD         = 0x051,
    HERO_GG_ORIG        = 0x052,
    HERO_GG             = 0x053,
    HERO_GG_MOD         = 0x054,
    HERO_TA_ORIG        = 0x055,
    HERO_TA             = 0x056,
    HERO_TA_MOD         = 0x057,
    HERO_NG_ORIG        = 0x058,
    HERO_NG             = 0x059,
    HERO_NG_MOD         = 0x05A,
    HERO_JZ_ORIG        = 0x05B,
    HERO_JZ             = 0x05C,
    HERO_JZ_MOD         = 0x05D,
    HERO_LE_ORIG        = 0x05E,
    HERO_LE             = 0x060,
    HERO_AE_ORIG        = 0x062,
    HERO_AE             = 0x064,
    HERO_MR             = 0x066,
    HERO_ATPA_BASIS     = 0x067,
    HERO_AT             = 0x068,
    HERO_PA             = 0x06F,
    HERO_AT_MOD         = 0x076,
    HERO_PA_MOD         = 0x077,
    HERO_WP_CLASS       = 0x078,
    HERO_ATTACK_TYPE    = 0x079, /* 0x00 = normal; 0x02 = aggressiv; 0xFE = vorsichtig */
    HERO_LE_MOD         = 0x07A, /* permanent LE mod */
    HERO_TIMER_ID       = 0x07B,
    HERO_START_GEAR     = 0x07C, /* has got initial gear set: 1 = true, 0 = false */
    HERO_HERBS          = 0x07D, /* keine = 0, Belmart = 1, Menchalkaktus = 2 */
    HERO_HUNGER_TIMER   = 0x07E, /* timer for no-hunger-miracle */
    HERO_HUNGER         = 0x07F, /* percentage */
    HERO_THIRST         = 0x080, /* percentage */
    HERO_FIGHTER_ID       = 0x081,
    HERO_VIEWDIR        = 0x082,
    HERO_ACTIONS        = 0x083, /* corresponds to ENEMY_SHEET_ATTACKS */
    HERO_ACTION_ID      = 0x084, /* last fight action */
    HERO_SPELL_ID       = 0x085, /* last spell in fight */
    HERO_ENEMY_ID       = 0x086, /* last enemy in fight */
    HERO_GROUP_NO       = 0x087,
    HERO_TEMPLE_ID      = 0x088,
    HERO_NPC_ID         = 0x089, /* Held = 0, NARIELL = 1, HARIKA = 2, CURIAN = 3, ARDORA = 4, GARSVIK = 5, ERWO = 6 */
    HERO_GROUP_POS      = 0x08A, /* 0x01 bis 0x06, 0x00 = not in group */
    HERO_HEAL_TIMER     = 0x08B,
    HERO_STAFFSPELL_TIMER   = 0x08F,
    HERO_RECIPE_ID      = 0x093, /* alchemy */
    HERO_RECIPE_TIMER   = 0x094, /* timer between failed alchemy attempts */
    HERO_RUHE_KOERPER   = 0x095, /* 1 = Ruhe Koerper spell is active */
    HERO_BLIND          = 0x096, /* blind rounds remaining */
    HERO_ECLIPTIFACTUS  = 0x097, /* shadow rounds remaining */
    HERO_SAFTKRAFT      = 0x098, /* stores extra damage of spell Saft, Kraft, Monstermacht */
    HERO_FIREBAN        = 0x099, /* 1 = active, 0 = inactive */
    HERO_INVISIBLE      = 0x09A, /* 1 = active, 0 = inactive */
    HERO_SPRITE_NO      = 0x09B, /* fight gfx of char, depending on type and sex */
    HERO_HOSTEL_ID      = 0x09C, /* alchemy */
    HERO_UNKNOWN9       = 0x09D,
    HERO_JAIL           = 0x09F, /* 1 = true, 0 = false */
    HERO_AXXELERATUS    = 0x0A0, /* 1 = active, 0 = inactive */
    HERO_DRUNK          = 0x0A1,
    HERO_UNKNOWN10      = 0x0A2, /* never used? */
    HERO_STATUS1        = 0x0AA, /* Bit0 = tot, Bit1 = schläft, Bit2 = versteinert, Bit4 = Chamaelioni, Bit5 = verflucht, Bit6 = bewusstlos */
    HERO_STATUS2        = 0x0AB, /* Bit1 = Duplicatus */
    HERO_UNKNOWN11      = 0x0AC, /* never used? */
    HERO_ILLNESS_EMPTY  = 0x0AE,
    HERO_ILLNESS        = 0x0B3,
    HERO_POISON_EMPTY   = 0x0D6,
    HERO_POISON         = 0x0DB,
    HERO_TA_FIGHT       = 0x108, /* 9 x 1 Byte */
    HERO_TA_BODY        = 0x111, /* 10 x 1 Byte */
    HERO_TA_SOCIAL      = 0x11B, /* 7 x 1 Byte */
    HERO_TA_NATURE      = 0x122, /* 6 x 1 Byte */
    HERO_TA_KNOWLEDGE   = 0x128, /* 9 x 1 Byte */
    HERO_TA_CRAFT       = 0x131, /* 9 x 1 Byte */
    HERO_TA_INTUITION   = 0x13A, /* 2 x 1 Byte */
    HERO_TA_RISE        = 0x13C, /* saved from last levelup */
    HERO_SPELLS         = 0x13D, /* empty Byte */
    HERO_SP_ANTI        = 0x13E,
    HERO_SP_CONTROL     = 0x143,
    HERO_SP_DEMON       = 0x14F,
    HERO_SP_ELEMENT     = 0x155,
    HERO_SP_MOTION      = 0x158,
    HERO_SP_HEAL        = 0x15E,
    HERO_SP_VISION      = 0x163,
    HERO_SP_ILLUSION    = 0x16A,
    HERO_SP_FIGHT       = 0x16E,
    HERO_SP_INTERACT    = 0x177,
    HERO_SP_TRANSFORM   = 0x179,
    HERO_SP_CHANGE      = 0x189,
    HERO_SP_RISE        = 0x193, /* saved from last levelup */
    HERO_MAGIC_SCHOOL   = 0x194,
    HERO_STAFFSPELL_LVL = 0x195,
    HERO_ITEM_HEAD      = 0x196,
    HERO_ITEM_ARM       = 0x1A4,
    HERO_ITEM_BODY      = 0x1B2,
    HERO_ITEM_RIGHT     = 0x1C0, /* right hand */
    HERO_ITEM_LEFT      = 0x1CE, /* left hand */
    HERO_ITEM_LEGS      = 0x1DC,
    HERO_ITEM_FEET      = 0x1EA,
    HERO_ITEM_KS1       = 0x1F8,
    HERO_ITEM_KS2       = 0x206,
    HERO_ITEM_KS3       = 0x214,
    HERO_ITEM_KS4       = 0x222,
    HERO_ITEM_KS5       = 0x230,
    HERO_ITEM_KS6       = 0x23E,
    HERO_ITEM_KS7       = 0x24C,
    HERO_ITEM_KS8       = 0x25A,
    HERO_ITEM_KS9       = 0x268,
    HERO_ITEM_KS10      = 0x276,
    HERO_ITEM_KS11      = 0x284,
    HERO_ITEM_KS12      = 0x292,
    HERO_ITEM_KS13      = 0x2A0,
    HERO_ITEM_KS14      = 0x2AE,
    HERO_ITEM_KS15      = 0x2BC,
    HERO_ITEM_KS16      = 0x2CA,
    HERO_LOAD           = 0x2D8,
    HERO_PORTRAIT       = 0x2DA /* 32 x 32 pixels, 8 bpp */
};

#define SIZEOF_HERO (0x6da)

enum {
    FIG_ACTION_MOVE = 1,
    FIG_ACTION_ATTACK = 2,
    FIG_ACTION_GUARD = 3,
    FIG_ACTION_SPELL = 4,
    FIG_ACTION_USE_ITEM = 5,
    FIG_ACTION_DROP_ITEM = 6,
    FIG_ACTION_EXCHANGE_WEAPON = 7,
    FIG_ACTION_EXCHANGE_ITEM = 8,
    FIG_ACTION_CHECK_VALUES = 9,
    FIG_ACTION_WAIT = 10,
    FIG_ACTION_COMPUTER_FIGHT = 11,
    FIG_ACTION_QUIT_AND_LOAD = 12,
    FIG_ACTION_REPEAT_OPTION = 13,
    FIG_ACTION_RANGE_ATTACK = 15,
    FIG_ACTION_UNKNOWN1 = 16,
    FIG_ACTION_UNKNOWN2 = 100
};

enum {
    ILLNESS_WUNDFIEBER = 0, ILLNESS_DUMPFSCHAEDEL, ILLNESS_BLAUE_KEUCHE,
    ILLNESS_PARALYSE, ILLNESS_SCHLACHTENFIEBER, ILLNESS_FROSTSCHAEDEN,
    ILLNESS_TOLLWUT
};

enum {
    POISON_SHURINKNOLLENGIFT = 0, POISON_ARAX, POISON_ANGSTGIFT,
    POISON_SCHLAFGIFT, POISON_GOLDLEIM, POISON_KROETENSCHEMEL, POISON_LOTUSGIFT,
    POISON_KUKRIS, POISON_BANNSTAUB
};

struct enemy_status1 {
	/* enemy + 0x31 */
	unsigned short dead	:1;
	unsigned short sleeps	:1;
	unsigned short stoned	:1;
	unsigned short busy	:1;
	unsigned short bit4	:1;
	unsigned short cursed	:1;
	unsigned short uncon	:1;
	unsigned short illusion	:1;
};

struct enemy_status2 {
	unsigned short bit8	:1;
	unsigned short bb	:1;
	unsigned short bit10	:1;
	unsigned short bit11	:1;
};

struct item_status {
	/* item + 0x02 */
	unsigned short armor		 :1;
	unsigned short weapon		 :1;
	unsigned short useable	 :1;
	unsigned short food		 :1;

	unsigned short stackable	 :1;
	unsigned short herb_potion	 :1;
	unsigned short undropable	 :1;
	unsigned short bit7		 :1;
};

struct knapsack_status {
	/* knapsack position + 0x04 */
	unsigned short broken		 :1;
	unsigned short half_empty	 :1;
	unsigned short empty		 :1;
	unsigned short magic_hidden	 :1;
	unsigned short bit4		 :1;
	unsigned short poison1		 :1;
	unsigned short poison2		 :1;
	unsigned short magic_known	 :1;

	unsigned short bit8	:1;
	unsigned short bit9	:1;
	unsigned short bit10	:1;
	unsigned short bit11	:1;
	unsigned short bit12	:1;
	unsigned short bit13	:1;
	unsigned short bit14	:1;
	unsigned short bit15	:1;
};

#define SIZEOF_KS_ITEM (14)

struct knapsack_item {
	signed short id;
	signed short counter;

	struct knapsack_status stat;
	signed char bf;
	signed char used;

	signed char timer;
	signed char poison_type;
	signed char poison_counter;
	signed char unused1;

	signed char unused2;
	signed char unused3;
};

struct passages {
	RealPt p1;
	RealPt p2;
	signed char flag;		/* 0 or 1, write only */
	signed char v2;			/* 0, read only */
	unsigned char town;		/* */
	signed char v4;			/*  write only */
};

struct informer {
	signed short name;		/* in TEXT.LTX */
	signed char  city;		/* -1 = no city, else city id */
	signed char unknown;		/* {0, 1} */
};

/* dummy */
struct enemy_sheets {
	signed char mon_id;
	signed char gfx_id;
	signed char v[0x2f];

	/* 0x31 */
	struct enemy_status1 status1;
	/* 0x32 */
	struct enemy_status2 status2;

	/* 0x33 */
	signed char unused_8;
	signed char size;
	signed char round_appear;
	signed char flags3;

	/* 0x37 */
	signed char nr_shoot;
	/* 0x38 */
	signed short damage_shoot;
	/* 0x3a */
	signed char nr_throw;
	/* 0x3b */
	signed short damage_throw;
	/* 0x3d */
	signed char le_flee;
};

enum {
    ENEMY_SHEET_MON_ID          = 0x00,
    ENEMY_SHEET_GFX_ID          = 0x01,
    ENEMY_SHEET_RS              = 0x02,
    ENEMY_SHEET_MU_ORIG         = 0x03,
    ENEMY_SHEET_MU              = 0x04,
    ENEMY_SHEET_KL_ORIG         = 0x05,
    ENEMY_SHEET_KL              = 0x06,
    ENEMY_SHEET_CH_ORIG         = 0x07,
    ENEMY_SHEET_CH              = 0x08,
    ENEMY_SHEET_FF_ORIG         = 0x09,
    ENEMY_SHEET_FF              = 0x0a,
    ENEMY_SHEET_GE_ORIG         = 0x0b,
    ENEMY_SHEET_GE              = 0x0c,
    ENEMY_SHEET_IN_ORIG         = 0x0d,
    ENEMY_SHEET_IN              = 0x0e,
    ENEMY_SHEET_KK_ORIG         = 0x0f,
    ENEMY_SHEET_KK              = 0x10,
    ENEMY_SHEET_LE_ORIG         = 0x11,
    ENEMY_SHEET_LE              = 0x13,
    ENEMY_SHEET_AE_ORIG         = 0x15,
    ENEMY_SHEET_AE              = 0x17,
    ENEMY_SHEET_MR              = 0x19,
    ENEMY_SHEET_FIRSTAP         = 0x1a,
    ENEMY_SHEET_ATTACKS         = 0x1b,
    ENEMY_SHEET_AT              = 0x1c,
    ENEMY_SHEET_PA              = 0x1d,
    ENEMY_SHEET_DAM1            = 0x1e,
    ENEMY_SHEET_DAM2            = 0x20,
    ENEMY_SHEET_BP_ORIG         = 0x22,
    ENEMY_SHEET_BP              = 0x23,
    ENEMY_SHEET_MAGIC           = 0x24,
    ENEMY_SHEET_MAG_ID          = 0x25,
    ENEMY_SHEET_LIST_POS        = 0x26,
    ENEMY_SHEET_VIEWDIR         = 0x27,
    ENEMY_SHEET_DUMMY2          = 0x28,
    ENEMY_SHEET_LEVEL           = 0x29,
    ENEMY_SHEET_DUMMY3          = 0x2a,
    ENEMY_SHEET_DUMMY4          = 0x2b,
    ENEMY_SHEET_CUR_SPELL       = 0x2c,
    ENEMY_SHEET_FIGHTER_ID        = 0x2d,
    ENEMY_SHEET_DUMMY5          = 0x2e,
    ENEMY_SHEET_BLIND           = 0x2f,
    ENEMY_SHEET_BROKEN          = 0x30, /* 0 = no, 1 = yes */
    ENEMY_SHEET_STATUS1         = 0x31,
    ENEMY_SHEET_STATUS2         = 0x32,
    ENEMY_SHEET_UNUSED8         = 0x33,
    ENEMY_SHEET_SIZE            = 0x34,
    ENEMY_SHEET_ROUND_APPEAR    = 0x35,
    ENEMY_SHEET_FLAGS           = 0x36,
    ENEMY_SHEET_SHOTS           = 0x37,
    ENEMY_SHEET_SHOT_DAM        = 0x38,
    ENEMY_SHEET_THROWS          = 0x3a,
    ENEMY_SHEET_THROW_DAM       = 0x3b,
    ENEMY_SHEET_LE_FLEE         = 0x3d
};

#define SIZEOF_ENEMY_SHEET (62)

enum {
    MONSTER_MON_ID      = 0x00,
    MONSTER_GFX_ID      = 0x01,
    MONSTER_RS          = 0x02,
    MONSTER_MU          = 0x03,
    MONSTER_KL          = 0x05,
    MONSTER_CH          = 0x07,
    MONSTER_FF          = 0x09,
    MONSTER_GE          = 0x0b,
    MONSTER_IN          = 0x0d,
    MONSTER_KK          = 0x0f,
    MONSTER_LE          = 0x11,
    MONSTER_AE          = 0x13,
    MONSTER_MR          = 0x15,
    MONSTER_FIRSTAP     = 0x17,
    MONSTER_ATTACKS     = 0x18,
    MONSTER_AT          = 0x19,
    MONSTER_PA          = 0x1a,
    MONSTER_DAM1        = 0x1b,
    MONSTER_DAM2        = 0x1d,
    MONSTER_BP          = 0x1f,
    MONSTER_MAGIC       = 0x20,
    MONSTER_MAG_ID      = 0x21,
    MONSTER_LEVEL       = 0x22,
    MONSTER_SIZE        = 0x23,
    MONSTER_FLAGS       = 0x24,
    MONSTER_SHOTS       = 0x25,
    MONSTER_SHOT_DAM    = 0x26,
    MONSTER_THROWS      = 0x28,
    MONSTER_THROW_DAM   = 0x29,
    MONSTER_LE_FLEE     = 0x2b
};

#define SIZEOF_MONSTER (44)

/* Informer */
enum {	JURGE = 1, HJORE, YASMA, UMBRIK, ISLEIF,
	RAGNA, BEORN, ASGRIMM, ELIANE, OLVIR,
	SWAFNILD, KOLBERG, UNICORN, ALGRID, TIOMAR };

/* FIGHT.LST */

enum {
    FIGHT_NAME                  = 0x00,
    FIGHT_INTRO_SEEN            = 0x13,
    FIGHT_SCENARIO              = 0x14,
    FIGHT_MONSTERS_ID           = 0x16, /* List of 20 monsters */
    FIGHT_MONSTERS_X            = 0x17 ,/*                     */
    FIGHT_MONSTERS_Y            = 0x18, /*                     */
    FIGHT_MONSTERS_VIEWDIR      = 0x19, /*                     */
    FIGHT_MONSTERS_ROUND_APPEAR = 0x1A, /* 5 bytes each        */
    FIGHT_PLAYERS_X             = 0x7A, /* List of 7 players  */
    FIGHT_PLAYERS_Y             = 0x7B, /*                  */
    FIGHT_PLAYERS_VIEWDIR       = 0x7C, /*                  */
    FIGHT_PLAYERS_ROUND_APPEAR  = 0x7D, /* 4 bytes each     */
    FIGHT_LOOT                  = 0x96, /* 2 bytes each: ID and 0x00 */
    FIGHT_DUCATS                = 0xD2,
    FIGHT_SILVER                = 0xD4,
    FIGHT_HELLER                = 0xD6
};

#define SIZEOF_FIGHT (216)
#define SIZEOF_FIGHT_MONSTER (5)
#define SIZEOF_FIGHT_PLAYER (4)

enum {
    FIGHTS_ZUFALL1_LAND = 0, FIGHTS_LAND_FIGHT1_1, FIGHTS_ZUFALL2_LAND, FIGHTS_ZUFALL3_LAND,
    FIGHTS_ZUFALL4_LAND, FIGHTS_SHIP3, FIGHTS_SHIP4, FIGHTS_SHIP5, FIGHTS_SHIP6,
    FIGHTS_SHIP8, FIGHTS_SHIP9, FIGHTS_SHIP12, FIGHTS_SHIP14, FIGHTS_SHIP15, FIGHTS_SHIP17,
    FIGHTS_SHIP18, FIGHTS_SHIP19, FIGHTS_SHIP21, FIGHTS_SHIP22, FIGHTS_SHIP23A,
    FIGHTS_SHIP23B, FIGHTS_SHIP24, FIGHTS_SHIP10, FIGHTS_F061_2, FIGHTS_F061_3,
    FIGHTS_F061_4B, FIGHTS_F061_4A, FIGHTS_F061_5, FIGHTS_F061_6A, FIGHTS_F108_1,
    FIGHTS_F108_2, FIGHTS_F108_3A, FIGHTS_F108_3B, FIGHTS_F108_3C, FIGHTS_F108_4,
    FIGHTS_F108_6, FIGHTS_F108_9, FIGHTS_F108_7, FIGHTS_F051_05_4, FIGHTS_F051_05_3,
    FIGHTS_F051_05_2, FIGHTS_F051_05_1, FIGHTS_F051_07, FIGHTS_F051_09, FIGHTS_F051_13,
    FIGHTS_F108_15, FIGHTS_F051_15, FIGHTS_F051_16, FIGHTS_F051_16_1, FIGHTS_F051_03,
    FIGHTS_F051_04, FIGHTS_F051_17, FIGHTS_F051_18, FIGHTS_F051_19, FIGHTS_F051_20_2,
    FIGHTS_F051_18_3, FIGHTS_F046_01, FIGHTS_F046_04, FIGHTS_F046_06, FIGHTS_F046_07,
    FIGHTS_F046_10, FIGHTS_F046_12, FIGHTS_F046_13, FIGHTS_F046_15, FIGHTS_F046_16,
    FIGHTS_F046_18, FIGHTS_F046_22, FIGHTS_F046_24, FIGHTS_F046_25, FIGHTS_F046_26,
    FIGHTS_F046_26_5, FIGHTS_F046_27, FIGHTS_F046_28, FIGHTS_F046_31, FIGHTS_F076_04,
    FIGHTS_F076_06, FIGHTS_F076_07, FIGHTS_F076_10, FIGHTS_F076_11, FIGHTS_F076_13,
    FIGHTS_F094_02, FIGHTS_F094_05, FIGHTS_F094_10, FIGHTS_F094_13, FIGHTS_F094_17,
    FIGHTS_F094_19, FIGHTS_F094_22, FIGHTS_F100_01, FIGHTS_F100_03, FIGHTS_F100_05,
    FIGHTS_F100_12, FIGHTS_F100_13, FIGHTS_F126_03, FIGHTS_F126_07, FIGHTS_F126_08,
    FIGHTS_F126_09, FIGHTS_F126_11, FIGHTS_F126_12, FIGHTS_F126_13, FIGHTS_F126_17,
    FIGHTS_F126_18, FIGHTS_F126_20, FIGHTS_F126_22, FIGHTS_F126_23, FIGHTS_F126_25,
    FIGHTS_F126_27, FIGHTS_F126_28, FIGHTS_F129_05, FIGHTS_F129_08, FIGHTS_F129_09,
    FIGHTS_F129_18, FIGHTS_F129_21, FIGHTS_F129_23, FIGHTS_F129_24, FIGHTS_F129_25,
    FIGHTS_F129_27, FIGHTS_F129_28, FIGHTS_F129_29, FIGHTS_F129_30, FIGHTS_F131_01A,
    FIGHTS_F131_01B, FIGHTS_F131_04, FIGHTS_F131_05, FIGHTS_F131_06, FIGHTS_F131_07,
    FIGHTS_F131_08, FIGHTS_F131_10, FIGHTS_F131_11_1, FIGHTS_F131_14A, FIGHTS_F131_14B,
    FIGHTS_F131_16, FIGHTS_DFIN12, FIGHTS_DFIN16, FIGHTS_DFIN18A, FIGHTS_DFIN18B,
    FIGHTS_DFIN26, FIGHTS_DFIN27A, FIGHTS_DFIN27B, FIGHTS_DFIN28, FIGHTS_DFIN30,
    FIGHTS_DPRE10_1, FIGHTS_DOBE07, FIGHTS_DOBE09, FIGHTS_DOBE11, FIGHTS_DOBE20,
    FIGHTS_DOBE19, FIGHTS_DOBE22, FIGHTS_DTHO03, FIGHTS_DTHO05, FIGHTS_DTHO06,
    FIGHTS_DTHO09, FIGHTS_DTHO10, FIGHTS_DTHO13, FIGHTS_DTHO15, FIGHTS_DTHO16,
    FIGHTS_DTHO18, FIGHTS_DTHO19, FIGHTS_DTHO20, FIGHTS_DTHO20_1, FIGHTS_DTHO21A,
    FIGHTS_DTHO21B, FIGHTS_DTHO23, FIGHTS_DTHO25, FIGHTS_DTHO27, FIGHTS_DTHO43,
    FIGHTS_DTHO48_1, FIGHTS_DTHO49, FIGHTS_DTHO50, FIGHTS_DTHO53, FIGHTS_DTHO55,
    FIGHTS_DTHO56, FIGHTS_DTHO57, FIGHTS_DTHO57_1, FIGHTS_DTHO58, FIGHTS_DTHO59,
    FIGHTS_DTHO60, FIGHTS_DTHO61, FIGHTS_F031, FIGHTS_F035, FIGHTS_F046, FIGHTS_F064,
    FIGHTS_F066, FIGHTS_F074, FIGHTS_F075_A, FIGHTS_F075_B, FIGHTS_F075_C, FIGHTS_F077,
    FIGHTS_F080, FIGHTS_F084, FIGHTS_F099, FIGHTS_F101, FIGHTS_F122, FIGHTS_F144,
    FIGHTS_DASP1A, FIGHTS_DASP1B, FIGHTS_DASP2, FIGHTS_DASP3, FIGHTS_DASP4, FIGHTS_DASP5,
    FIGHTS_DASP6A, FIGHTS_DASP6B, FIGHTS_DASP7, FIGHTS_DASP8, FIGHTS_DASP9, FIGHTS_DASP10,
    FIGHTS_DASP11, FIGHTS_DASP12A, FIGHTS_DASP12B, FIGHTS_DASP13, FIGHTS_DASP14,
    FIGHTS_DASP16, FIGHTS_DASP17, FIGHTS_DASP18, FIGHTS_THOR8, FIGHTS_CITYFIGHT1,
    FIGHTS_CITYFIGHT2, FIGHTS_CITYFIGHT3, FIGHTS_CAMPFIGHT1, FIGHTS_CAMPFIGHT2,
    FIGHTS_CAMPFIGHT3, FIGHTS_SHIP10_1, FIGHTS_F061_6B, FIGHTS_F051_16A, FIGHTS_F051_16B,
    FIGHTS_F051_14A, FIGHTS_F051_14B, FIGHTS_F051_117, FIGHTS_F129_124, FIGHTS_F131_08A,
    FIGHTS_F131_08B, FIGHTS_F031_14A, FIGHTS_DTH021A, FIGHTS_CAMPFIGHT4, FIGHTS_SHIP10_2,
    FIGHTS_F051_02, FIGHTS_F051_5, FIGHTS_PHEX3, FIGHTS_PHEX22, FIGHTS_PHEX23,
    FIGHTS_PHEX24, FIGHTS_F131_01, FIGHTS_F129_08A, FIGHTS_DTHO14, FIGHTS_WILD1,
    FIGHTS_WILD2, FIGHTS_WILD3, FIGHTS_WILD4, FIGHTS_WILD5, FIGHTS_WILD6, FIGHTS_WILD7,
    FIGHTS_WILD8, FIGHTS_S001, FIGHTS_F051_14C, FIGHTS_F129_17, FIGHTS_DCAMPFIGHT1,
    FIGHTS_DCAMPFIGHT2, FIGHTS_DCAMPFIGHT3, FIGHTS_DCAMPFIGHT4
};

enum {
    ARCHIVE_FILE_PLAYM_UK       = 0x00,
    ARCHIVE_FILE_KDBACK_DAT     = 0x01,
    ARCHIVE_FILE_KCBACK_DAT     = 0x02,
    ARCHIVE_FILE_KLBACK_DAT     = 0x03,
    ARCHIVE_FILE_KSBACK_DAT     = 0x04,
    ARCHIVE_FILE_KARTE_DAT      = 0x05,
    ARCHIVE_FILE_BICONS         = 0x06,
    ARCHIVE_FILE_OBJECTS_NVF    = 0x07,
    ARCHIVE_FILE_LROUT_DAT      = 0x08,
    ARCHIVE_FILE_SROUT_DAT      = 0x09,
    ARCHIVE_FILE_HSROUT_DAT     = 0x0a,
    ARCHIVE_FILE_FONT6          = 0x0b,
    ARCHIVE_FILE_FONT8          = 0x0c,
    ARCHIVE_FILE_GGSTS_NVF      = 0x0d,
    ARCHIVE_FILE_TEXT_LTX       = 0x0e,
    ARCHIVE_FILE_ICONS          = 0x0f,
    ARCHIVE_FILE_MONSTER        = 0x10,
    ARCHIVE_FILE_MONSTER_TAB    = 0x11,
    ARCHIVE_FILE_COMPASS        = 0x12,
    ARCHIVE_FILE_MAPTEXT_LTX    = 0x13,
    ARCHIVE_FILE_ZUSTA_UK       = 0x14,
    ARCHIVE_FILE_FIGHTTXT_LTX   = 0x15,
    ARCHIVE_FILE_ANIS           = 0x16,
    ARCHIVE_FILE_ANIS_TAB       = 0x17,
    ARCHIVE_FILE_MFIGS          = 0x18,
    ARCHIVE_FILE_MFIGS_TAB      = 0x19,
    ARCHIVE_FILE_CITY_DAT       = 0x1a,
    /*
        THORWAL.DAT, SERSKE.DAT, BREIDA.DAT, PEILINEN.DAT, ROVAMUND.DAT, NORDVEST.DAT,
        KRAVIK.DAT, SKELELLE.DAT, MERSKE.DAT, EFFERDUN.DAT, TJOILA.DAT, RUKIAN.DAT,
        ANGBODIRTAL.DAT, AUPLOG.DAT, VILNHEIM.DAT, BODON.DAT, OBERORKEN.DAT,
        PHEXCAER.DAT, GROENVEL.DAT, FELSTEYN.DAT, EINSIEDL.DAT, ORKANGER.DAT,
        CLANEGH.DAT, LISKOR.DAT, THOSS.DAT, TJANSET.DAT, ALA.DAT, ORVIL.DAT,
        OVERTHORN.DAT, ROVIK.DAT, HJALSING.DAT, GUDDASUN.DAT, KORD.DAT, TREBAN.DAT,
        ARYN.DAT, RUNINSHA.DAT, OTTARJE.DAT, SKJAL.DAT, PREM.DAT, DASPOTA.DAT,
        RYBON.DAT, LJASDAHL.DAT, VARNHEIM.DAT, VAERMHAG.DAT, TYLDON.DAT, VIDSAND.DAT,
        BRENDHIL.DAT, MANRIN.DAT, FTJOILA.DAT, FANGBODI.DAT, HJALLAND.DAT, RUNIN.DAT
    */
    ARCHIVE_FILE_CITY_LTX       = 0x4d,
    /*
        THORWAL_LTX, SERSKE_LTX, BREIDA_LTX, PEILINEN_LTX, ROVAMUND_LTX, NORDVEST_LTX,
        KRAVIK_LTX, SKELELLE_LTX, MERSKE_LTX, EFFERDUN_LTX, TJOILA_LTX, RUKIAN_LTX,
        ANGBODIR_LTX, AUPLOG_LTX, VILNHEIM_LTX, BODON_LTX, OBERORKE_LTX, PHEXCAER_LTX,
        GROENVEL_LTX, FELSTEYN_LTX, EINSIEDL_LTX, ORKANGER_LTX, CLANEGH_LTX, LISKOR_LTX,
        THOSS_LTX, TJANSET_LTX, ALA_LTX, ORVIL_LTX, OVERTHOR_LTX, ROVIK_LTX,
        HJALSING_LTX, GUDDASUN_LTX, KORD_LTX, TREBAN_LTX, ARYN_LTX, RUNINSHA_LTX,
        OTTARJE_LTX, SKJAL_LTX, PREM_LTX, DASPOTA_LTX, RYBON_LTX, LJASDAHL_LTX,
        VARNHEIM_LTX, VAERMHAG_LTX, TYLDON_LTX, VIDSAND_LTX, BRENDHIL_LTX, MANRIN_LTX,
        F_TJOILA_LTX, F_ANGBOD_LTX, HJALLAND_LTX, L_RUNIN_LTX
    */
    ARCHIVE_FILE_TAVERN_TLK     = 0x82,
    ARCHIVE_FILE_SPSTAR_NVF     = 0x83,
    ARCHIVE_FILE_GUERTEL_NVF    = 0x84,
    ARCHIVE_FILE_HAFEN_LTX      = 0x85,
    ARCHIVE_FILE_FACE_NVF       = 0x86,
    ARCHIVE_FILE_HYGBACK_NVF    = 0x87,
    ARCHIVE_FILE_HYGGELIK_NVF   = 0x88,
    ARCHIVE_FILE_SKULL_NVF      = 0x89,
    ARCHIVE_FILE_OUTRO1_NVF     = 0x8a,
    ARCHIVE_FILE_OUTRO2_NVF     = 0x8b,
    ARCHIVE_FILE_OUTRO3_NVF     = 0x8c,
    ARCHIVE_FILE_TOWNPAL_DAT    = 0x8d,
    ARCHIVE_FILE_THORWAL_XMI    = 0x8e,
    ARCHIVE_FILE_SUMMARY_XMI    = 0x8f,
    ARCHIVE_FILE_TEMPLE_XMI     = 0x90,
    ARCHIVE_FILE_TERMS_XMI      = 0x91,
    ARCHIVE_FILE_SMITH_XMI      = 0x92,
    ARCHIVE_FILE_INN_XMI        = 0x93,
    ARCHIVE_FILE_HEALER_XMI     = 0x94,
    ARCHIVE_FILE_CAMP_XMI       = 0x95,
    ARCHIVE_FILE_VICTORY_XMI    = 0x96,
    ARCHIVE_FILE_SAMPLE_AD      = 0x97,
    ARCHIVE_FILE_ATTIC          = 0x98,
    ARCHIVE_FILE_POPUP_DAT      = 0x99,
    ARCHIVE_FILE_DIALOGS_TLK    = 0x9a,
    /*
        F092.TLK, HERBERG.TLK, DASPOTA1.TLK, DASPOTA2.TLK, DASPOTA3.TLK, INFO1.TLK,
        INFO2.TLK, INFO3.TLK, INFO4.TLK, INFO5.TLK, INFO6.TLK, INFO7.TLK, SCHMIED.TLK,
        GHANDEL.TLK, KHANDEL.TLK, WHANDEL.TLK, F115.TLK, EREMIT.TLK, HYGGELIK.TLK
    */
    ARCHIVE_FILE_WEAPONS_NVF    = 0xb0,
    ARCHIVE_FILE_BUCH_DAT       = 0xb1,
    ARCHIVE_FILE_SPELLOBJ_NVF   = 0xb2,
    ARCHIVE_FILE_IN_HEADS_NVF   = 0xb3,
    ARCHIVE_FILE_TEMPICON       = 0xb4,
    ARCHIVE_FILE_SHIPSL_NVF     = 0xb5,
    ARCHIVE_FILE_STONESL_NVF    = 0xb6,
    ARCHIVE_FILE_MARBLESL_NVF   = 0xb7,
    ARCHIVE_FILE_TFLOOR1_NVF    = 0xb8,
    ARCHIVE_FILE_TDIVERSE_NVF   = 0xb9,
    ARCHIVE_FILE_HOUSE1_NVF     = 0xba,
    ARCHIVE_FILE_HOUSE2_NVF     = 0xbb,
    ARCHIVE_FILE_HOUSE3_NVF     = 0xbc,
    ARCHIVE_FILE_HOUSE4_NVF     = 0xbd,
    ARCHIVE_FILE_FEATURE_LTX    = 0xbe,
    ARCHIVE_FILE_FEATURE1_LTX   = 0xbf,
    ARCHIVE_FILE_FEATURE2_LTX   = 0xc0,
    ARCHIVE_FILE_FEATURE3_LTX   = 0xc1,
    ARCHIVE_FILE_FEATURE4_LTX   = 0xc2,
    ARCHIVE_FILE_FEATURE5_LTX   = 0xc3,
    ARCHIVE_FILE_FEATURE6_LTX   = 0xc4,
    ARCHIVE_FILE_FEATURE7_LTX   = 0xc5,
    ARCHIVE_FILE_FEATURE8_LTX   = 0xc6,
    ARCHIVE_FILE_FIGHTOBJ_NVF   = 0xc7,
    ARCHIVE_FILE_SCENARIO_LST   = 0xc8,
    ARCHIVE_FILE_KDLBACK_DAT    = 0xc9,
    ARCHIVE_FILE_KCLBACK_DAT    = 0xca,
    ARCHIVE_FILE_KLLBACK_DAT    = 0xcb,
    ARCHIVE_FILE_KSLBACK_DAT    = 0xcc,
    ARCHIVE_FILE_FIGHT_LST      = 0xcd,
    ARCHIVE_FILE_SKARTE_NVF     = 0xce,
    ARCHIVE_FILE_GAMES_NAM      = 0xcf,
    ARCHIVE_FILE_ITEMNAME       = 0xd0,
    ARCHIVE_FILE_MONNAMES       = 0xd1,
    ARCHIVE_FILE_THORWAL1_LTX   = 0xd2,
    ARCHIVE_FILE_THORWAL2_LTX   = 0xd3,
    ARCHIVE_FILE_DUNGEON_XMI    = 0xd4,
    ARCHIVE_FILE_COMBAT_XMI     = 0xd5,
    ARCHIVE_FILE_PLAYM_US       = 0xd6,
    ARCHIVE_FILE_ZUSTA_US       = 0xd7,
    ARCHIVE_FILE_ANI_DAT        = 0xd8,
    ARCHIVE_FILE_WEAPANI_DAT    = 0xd9,
    ARCHIVE_FILE_MONSTER_DAT    = 0xda,
    ARCHIVE_FILE_BSKILLS_DAT    = 0xdb,
    ARCHIVE_FILE_ITEMS_DAT      = 0xdc,
    ARCHIVE_FILE_CHARTEXT_LTX   = 0xdd,
    ARCHIVE_FILE_SPELLTXT_LTX   = 0xde,
    ARCHIVE_FILE_WONDER_LTX     = 0xdf,
    ARCHIVE_FILE_STRASSE_LTX    = 0xe0,
    ARCHIVE_FILE_NSC_LTX        = 0xe1,
    ARCHIVE_FILE_NPCS           = 0xe2,
    /*
        NARIELL.NPC, HARIKA.NPC, CURIAN.NPC, ARDORA.NPC, GARSVIK.NPC, ERWO.NPC
    */
    ARCHIVE_FILE_FINGER_NVF     = 0xe8,
    ARCHIVE_FILE_LTURM_NVF      = 0xe9,
    ARCHIVE_FILE_WFIGS          = 0xea,
    ARCHIVE_FILE_WFIGS_TAB      = 0xeb,
    ARCHIVE_FILE_SPLASHES_DAT   = 0xec,
    ARCHIVE_FILE_FEATURE9_LTX   = 0xed,
    ARCHIVE_FILE_PHEX1_LTX      = 0xee,
    ARCHIVE_FILE_PHEX2_LTX      = 0xef,
    ARCHIVE_FILE_WILD_LTX       = 0xf0,
    ARCHIVE_FILE_WAFFINFO_LTX   = 0xf1,
    ARCHIVE_FILE_DNGS           = 0xf2,
    /*
        SHIP.DNG, F046.DNG, F051.DNG, F061.DNG, F076.DNG, F094.DNG, F100.DNG, F108.DNG,
        F126.DNG, F129.DNG, F131.DNG, OBER.DNG, PREM.DNG, THORWAL.DNG, FINAL.DNG
    */
    ARCHIVE_FILE_SHIP_DDT       = 0x101,
    ARCHIVE_FILE_F046_DDT       = 0x102,
    ARCHIVE_FILE_F051_DDT       = 0x103,
    ARCHIVE_FILE_F061_DDT       = 0x104,
    ARCHIVE_FILE_F076_DDT       = 0x105,
    ARCHIVE_FILE_F094_DDT       = 0x106,
    ARCHIVE_FILE_F100_DDT       = 0x107,
    ARCHIVE_FILE_F108_DDT       = 0x108,
    ARCHIVE_FILE_F126_DDT       = 0x109,
    ARCHIVE_FILE_F129_DDT       = 0x10a,
    ARCHIVE_FILE_F131_DDT       = 0x10b,
    ARCHIVE_FILE_OBER_DDT       = 0x10c,
    ARCHIVE_FILE_PREM_DDT       = 0x10d,
    ARCHIVE_FILE_THORWAL_DDT    = 0x10e,
    ARCHIVE_FILE_FINAL_DDT      = 0x10f,
    ARCHIVE_FILE_SHIP_DTX       = 0x110,
    ARCHIVE_FILE_F046_DTX       = 0x111,
    ARCHIVE_FILE_F051_DTX       = 0x112,
    ARCHIVE_FILE_F061_DTX       = 0x113,
    ARCHIVE_FILE_F076_DTX       = 0x114,
    ARCHIVE_FILE_F094_DTX       = 0x115,
    ARCHIVE_FILE_F100_DTX       = 0x116,
    ARCHIVE_FILE_F108_DTX       = 0x117,
    ARCHIVE_FILE_F126_DTX       = 0x118,
    ARCHIVE_FILE_F129_DTX       = 0x119,
    ARCHIVE_FILE_F131_DTX       = 0x11a,
    ARCHIVE_FILE_OBER_DTX       = 0x11b,
    ARCHIVE_FILE_PREM_DTX       = 0x11c,
    ARCHIVE_FILE_THORWAL_DTX    = 0x11d,
    ARCHIVE_FILE_FINAL_DTX      = 0x11e,
    ARCHIVE_FILE_TFLOOR2_NVF    = 0x11f,
    ARCHIVE_FILE_STORY_LTX      = 0x120,
    ARCHIVE_FILE_FX1_VOC        = 0x121,
    ARCHIVE_FILE_FX2_VOC        = 0x122,
    ARCHIVE_FILE_FX3_VOC        = 0x123,
    ARCHIVE_FILE_FX4_VOC        = 0x124,
    ARCHIVE_FILE_FX5_VOC        = 0x125,
    ARCHIVE_FILE_FX6_VOC        = 0x126,
    ARCHIVE_FILE_FX7_VOC        = 0x127,
    ARCHIVE_FILE_FX8_VOC        = 0x128,
    ARCHIVE_FILE_FX9_VOC        = 0x129,
    ARCHIVE_FILE_FX10_VOC       = 0x12a,
    ARCHIVE_FILE_FX11_VOC       = 0x12b,
    ARCHIVE_FILE_FX12_VOC       = 0x12c,
    ARCHIVE_FILE_FX13_VOC       = 0x12d,
    ARCHIVE_FILE_FX14_VOC       = 0x12e,
    ARCHIVE_FILE_FX15_VOC       = 0x12f,
    ARCHIVE_FILE_FX16_VOC       = 0x130,
    ARCHIVE_FILE_FX17_VOC       = 0x131,
    ARCHIVE_FILE_FX18_VOC       = 0x132,
    ARCHIVE_FILE_OUTRO_XMI      = 0x133
};

#endif
