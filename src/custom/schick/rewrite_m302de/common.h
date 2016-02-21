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
    HERO_LE             = 0x05E,
    HERO_LE_ORIG        = 0x060,
    HERO_AE             = 0x062,
    HERO_AE_ORIG        = 0x064,
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
    HERO_UNKNOWN1       = 0x07E,
    HERO_HUNGER         = 0x07F, /* percentage */
    HERO_THIRST         = 0x080, /* percentage */
    HERO_FIGHT_ID       = 0x081,
    HERO_VIEWDIR        = 0x082,
    HERO_ACTIONS        = 0x083, /* corresponds to MONSTER_SHEET_ATTACKS */
    HERO_UNKNOWN2       = 0x084,
    HERO_SPELL_ID       = 0x085,
    HERO_ENEMY_ID       = 0x086, /* last enemy in fight */
    HERO_GROUP_NO       = 0x087,
    HERO_TEMPLE_ID      = 0x088,
    HERO_NPC_ID         = 0x089, /* Held = 0, NARIELL = 1, HARIKA = 2, CURIAN = 3, ARDORA = 4, GARSVIK = 5, ERWO = 6 */
    HERO_GROUP_POS      = 0x08A, /* 0x01 bis 0x06, 0x00 = not in group */
    HERO_UNKNOWN3       = 0x08B,
    HERO_UNKNOWN4       = 0x08F,
    HERO_RECEIPT_ID     = 0x093, /* alchemy */
    HERO_UNKNOWN5       = 0x094,
    HERO_UNKNOWN6       = 0x095,
    HERO_BLIND          = 0x096, /* blind rounds remaining */
    HERO_ECLIPTIFACTUS  = 0x097, /* shadow rounds remaining */
    HERO_UNKNOWN7       = 0x098,
    HERO_FIREBAN        = 0x099, /* 1 = active, 0 = inactive */
    HERO_INVISIBLE      = 0x09A, /* 1 = active, 0 = inactive */
    HERO_UNKNOWN8       = 0x09B,
    HERO_HOSTEL_ID      = 0x09C, /* alchemy */
    HERO_UNKNOWN9       = 0x09D,
    HERO_JAIL           = 0x09F, /* 1 = true, 0 = false */
    HERO_AXXELERATUS    = 0x0A0, /* 1 = active, 0 = inactive */
    HERO_DRUNK          = 0x0A1,
    HERO_UNKNOWN10      = 0x0A2,
    HERO_STATUS1        = 0x0AA, /* Bit0 = tot, Bit1 = schläft, Bit2 = versteinert, Bit4 = Chamaelioni, Bit5 = verflucht, Bit6 = bewusstlos */
    HERO_STATUS2        = 0x0AB, /* Bit1 = Duplicatus */
    HERO_UNKNOWN11      = 0x0AC,
    HERO_ILLNESS_EMPTY  = 0x0AE, /* empty */
    HERO_ILLNESS        = 0x0B3, /* 1-Wundfieber, 2-Dumpfschädel, 3-Blaue Keuche, 4-Paralyse, 5-Schlachtenfieber, 6-Frostschäden, 7-Tollwut */
    HERO_POISON_EMPTY   = 0x0D6, /* empty */
    HERO_POISON         = 0x0DB, /* 1-Shurinknollengift, 2-Arax, 3-Angstgift, 4-Schlafgift, 5-Goldleim, 6-Krötenschemel, 7-Lotusgift, 8-Kukris, 9-Bannstaubvergiftung */
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
    HERO_WAND           = 0x195,
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
    ENEMY_SHEET_FIGHT_ID        = 0x2d,
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

#endif
