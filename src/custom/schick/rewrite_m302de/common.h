#if !defined(COMMON_H)
#define COMMON_H

#if defined(__BORLANDC__)
typedef unsigned char Bit8u;
typedef signed char Bit8s;
typedef unsigned short Bit16u;
typedef signed short Bit16s;
typedef unsigned long Bit32u;
typedef signed long Bit32s;
typedef Bit8u* RealPt;
typedef Bit8u* PhysPt;
typedef Bit8u huge * HugePt;
#else
#include "mem.h"
#endif

/**
	struct nvf_desc - nvf descriptor
	@src:	pointer to a buffer containing the nvf file
	@dst:	pointer where to extract the picture
	@no:	number of the picture to extract
	@type:	kind of compression / direction (0 = PP20 / 2-5 RLE / copy)
	@p_height:	pointer where the height of the picture must be stored
	@p_width:	pointer where the width of the picture must be stored
*/
struct nvf_desc {
	Bit8u *dst;
	Bit8u *src;
	short no;
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

enum {
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3
};

enum {
    ANI_AREA_NAME       = 0x00, /* String of length 5 (zero-terminated) */
    ANI_AREA_X          = 0x05,
    ANI_AREA_Y          = 0x07,
    ANI_AREA_HEIGHT     = 0x08,
    ANI_AREA_WIDTH      = 0x09,
    ANI_AREA_CYCLIC     = 0x0b,
    ANI_AREA_PICS       = 0x0c,
    ANI_AREA_PICS_TAB   = 0x0d, /* RealPt[20] */
    ANI_AREA_CHANGES    = 0x5d,
    ANI_AREA_CHANGES_TB = 0x5f, /* struct{ short pic, duration; }[42] */
};

#define SIZEOF_ANI_AREA (0x107)

/**
 *	struct hero_status - status of the hero
 *	@dead:		1 = dead				/ 0 = not dead
 *	@asleep:	1 = asleep				/ 0 = awake
 *	@petrified:	1 = petrified				/ 0 = not petrified
 *	@brewing:	1 = brewing an alchemical recipe in an inn / 0 = not brewing
 *	@chamaelioni:	1 = 'Chamaelioni' spell active		/ 0 = spell not active (effect of active 'Chamaelioni': AT-5 for attacker)
 *	@renegade:	1 = renegade (from 'Boeser Blick' spell)	/ 0 = normal
 *	@unconscious:	1 = unconscious				/ 0 = conscious
 *	@tied:		1 = hero tied                           / 0 = not tied (from hero 'Band und Fessel' spell)
 *
 *	@scared:	1 = scared and wants to flee (from 'Horriphobus' spell or Angstgift)	/ 0 = not scared
 *	@dummy2:	probably unused
 *	@duplicatus:	1 = 'Duplicatus' spell active	/ spell not active (effect: AT/2 for attacker, applied after the 'Chamaelioni' effect)
 *	@tame:		1 = tame (from enemy 'Bannbaladin' spell)	/ 0 = normal
 *	@seen_phantom:	1 = hero has seen the horrible phantom in the 'Verlassene Herberge' dungeon which gave MU -3 for 5 hours (same position where the Sphaerenriss can happen).
 *	@gods_pissed:	1 = gods pissed (no more miracles)	/ 0 = gods normal	(activated by praising the nameless god, deactivated by destroying the statue of the nameless god)
 *	@transformed:	1 = hero is transformed (all positive attributes -1, from the cursed chest on the Totenschiff) / 0 not transformed (can be cured by 'Verwandlung beenden' spell or Praios/Hesinde miracle)
 *	@encouraged:	1 = MU increased by 3 (seg082.cpp, probably a dungeon event) / 0 = attibute back to normal.
 */
struct hero_status {
	/* hero + 0xaa */
	unsigned short dead		:1;
	unsigned short asleep		:1;
	unsigned short petrified	:1;
	unsigned short brewing		:1;
	unsigned short chamaelioni	:1;
	unsigned short renegade		:1;
	unsigned short unconscious	:1;
	unsigned short tied		:1;
	/* hero + 0xab */
	unsigned short scared		:1;
	unsigned short dummy2		:1;
	unsigned short duplicatus	:1;
	unsigned short tame		:1;
	unsigned short seen_phantom	:1;
	unsigned short gods_pissed	:1;
	unsigned short transformed	:1;
	unsigned short encouraged	:1;
};

enum {
    HERO_NAME               = 0x000,
    HERO_NAME2              = 0x010,
    HERO_KS_TAKEN           = 0x020,
    HERO_TYPE               = 0x021, /* See enum HERO_TYPE_* below. */
    HERO_SEX                = 0x022,
    HERO_HEIGHT             = 0x023,
    HERO_WEIGHT             = 0x024,
    HERO_GOD                = 0x026,
    HERO_LEVEL              = 0x027,
    HERO_AP                 = 0x028,
    HERO_MONEY              = 0x02C, /* Heller */
    HERO_RS_BONUS1          = 0x030, /* RS-Bonus = RS-Bonus1 + RS-Bonus2 */
    HERO_RS_BONUS2          = 0x031,
    HERO_RS_BE              = 0x032,
    HERO_BP_LEFT            = 0x033,
    HERO_ATTRIB_ORIG        = 0x034, /* See enum ATTRIB_* below. */
    HERO_ATTRIB             = 0x035, /* See enum ATTRIB_* below. */
    HERO_ATTRIB_MOD         = 0x036, /* See enum ATTRIB_* below. */
    HERO_LE_ORIG            = 0x05E,
    HERO_LE                 = 0x060,
    HERO_AE_ORIG            = 0x062,
    HERO_AE                 = 0x064,
    HERO_MR                 = 0x066,
    HERO_ATPA_BASIS         = 0x067,
    HERO_AT                 = 0x068,
    HERO_PA                 = 0x06F,
    HERO_AT_MOD             = 0x076,
    HERO_PA_MOD             = 0x077,
    HERO_WP_CLASS           = 0x078,
    HERO_ATTACK_TYPE        = 0x079, /* 0x00 = normal; 0x02 = aggressiv; 0xFE = vorsichtig */
    HERO_LE_MOD             = 0x07A, /* permanent LE mod */
    HERO_TIMER_ID           = 0x07B,
    HERO_START_GEAR         = 0x07C, /* has got initial gear set: 1 = true, 0 = false */
    HERO_HERBS              = 0x07D, /* keine = 0, Belmart = 1, Menchalkaktus = 2 */
    HERO_HUNGER_TIMER       = 0x07E, /* timer for no-hunger-miracle */
    HERO_HUNGER             = 0x07F, /* percentage */
    HERO_THIRST             = 0x080, /* percentage */
    HERO_FIGHTER_ID         = 0x081,
    HERO_VIEWDIR            = 0x082,
    HERO_ACTIONS            = 0x083, /* corresponds to ENEMY_SHEET_ATTACKS */
    HERO_ACTION_ID          = 0x084, /* last fight action */
    HERO_SPELL_ID           = 0x085, /* last spell in fight */
    HERO_ENEMY_ID           = 0x086, /* last enemy in fight */
    HERO_GROUP_NO           = 0x087,
    HERO_TEMPLE_ID          = 0x088,
    HERO_NPC_ID             = 0x089, /* See enum NPC_* below. */
    HERO_GROUP_POS          = 0x08A, /* 0x01 bis 0x06, 0x00 = not in group */
    HERO_HEAL_TIMER         = 0x08B,
    HERO_STAFFSPELL_TIMER   = 0x08F,
    HERO_RECIPE_ID          = 0x093, /* alchemy */
    HERO_RECIPE_TIMER       = 0x094, /* timer till completion of alchemical brewing process, in days */
    HERO_RUHE_KOERPER       = 0x095, /* 1 = Ruhe Koerper spell is active */
    HERO_BLIND              = 0x096, /* blind rounds remaining from 'Blitz' spell */
    HERO_ECLIPTIFACTUS      = 0x097, /* shadow rounds remaining */
    HERO_SAFTKRAFT          = 0x098, /* stores extra damage of spell 'Saft, Kraft, Monstermacht' */
    HERO_FIREBAN            = 0x099, /* 1 = active, 0 = inactive */
    HERO_INVISIBLE          = 0x09A, /* 1 = active, 0 = inactive */
    HERO_SPRITE_NO          = 0x09B, /* fight gfx of char, depending on type and sex */
    HERO_HOSTEL_ID          = 0x09C, /* alchemy */
    HERO_ESCAPE_POSITION    = 0x09D, /* the dungeon square the hero escaped to in a fight. read from FIG_FLEE_POSITION */
    HERO_JAIL               = 0x09F, /* 1 = true, 0 = false */
    HERO_AXXELERATUS        = 0x0A0, /* 1 = active, 0 = inactive */
    HERO_DRUNK              = 0x0A1,
    HERO_UNKNOWN10          = 0x0A2, /* never used? */
    HERO_STATUS1            = 0x0AA, /* Bit0 = dead, Bit1 = sleeping, Bit2 = petrified, Bit4 = Chamaelioni, Bit5 = renegade, Bit6 = unconscious */
    HERO_STATUS2            = 0x0AB, /* Bit0 = scared, Bit1 = dummy2, Bit2 = duplicatus, Bit3 = tame, Bit4 = dummy3, Bit5 = gods_pissed, Bit6 = transformed, Bit7 = encouraged */
    HERO_UNKNOWN11          = 0x0AC, /* never used? */
    HERO_ILLNESS_EMPTY      = 0x0AE,
    HERO_ILLNESS            = 0x0B3,
    HERO_POISON_EMPTY       = 0x0D6,
    HERO_POISON             = 0x0DB,
    HERO_TA_FIGHT           = 0x108, /* 9 x 1 Byte, see enum TA_* below */
    HERO_TA_BODY            = 0x111, /* 10 x 1 Byte */
    HERO_TA_SOCIAL          = 0x11B, /* 7 x 1 Byte */
    HERO_TA_NATURE          = 0x122, /* 6 x 1 Byte */
    HERO_TA_KNOWLEDGE       = 0x128, /* 9 x 1 Byte */
    HERO_TA_CRAFT           = 0x131, /* 9 x 1 Byte */
    HERO_TA_INTUITION       = 0x13A, /* 2 x 1 Byte */
    HERO_TA_RISE            = 0x13C, /* saved from last levelup */
    HERO_SPELLS             = 0x13D, /* empty Byte */
    HERO_SP_ANTI            = 0x13E,
    HERO_SP_CONTROL         = 0x143,
    HERO_SP_DEMON           = 0x14F,
    HERO_SP_ELEMENT         = 0x155,
    HERO_SP_MOTION          = 0x158,
    HERO_SP_HEAL            = 0x15E,
    HERO_SP_VISION          = 0x163,
    HERO_SP_ILLUSION        = 0x16A,
    HERO_SP_FIGHT           = 0x16E,
    HERO_SP_INTERACT        = 0x177,
    HERO_SP_TRANSFORM       = 0x179,
    HERO_SP_CHANGE          = 0x189,
    HERO_SP_RISE            = 0x193, /* saved from last levelup */
    HERO_MAGIC_SCHOOL       = 0x194,
    HERO_STAFFSPELL_LVL     = 0x195,
    HERO_ITEM_HEAD          = 0x196,
    HERO_ITEM_ARM           = 0x1A4,
    HERO_ITEM_BODY          = 0x1B2,
    HERO_ITEM_RIGHT         = 0x1C0, /* right hand */
    HERO_ITEM_LEFT          = 0x1CE, /* left hand */
    HERO_ITEM_LEGS          = 0x1DC,
    HERO_ITEM_FEET          = 0x1EA,
    HERO_ITEM_KS1           = 0x1F8,
    HERO_ITEM_KS2           = 0x206,
    HERO_ITEM_KS3           = 0x214,
    HERO_ITEM_KS4           = 0x222,
    HERO_ITEM_KS5           = 0x230,
    HERO_ITEM_KS6           = 0x23E,
    HERO_ITEM_KS7           = 0x24C,
    HERO_ITEM_KS8           = 0x25A,
    HERO_ITEM_KS9           = 0x268,
    HERO_ITEM_KS10          = 0x276,
    HERO_ITEM_KS11          = 0x284,
    HERO_ITEM_KS12          = 0x292,
    HERO_ITEM_KS13          = 0x2A0,
    HERO_ITEM_KS14          = 0x2AE,
    HERO_ITEM_KS15          = 0x2BC,
    HERO_ITEM_KS16          = 0x2CA,
    HERO_LOAD               = 0x2D8,
    HERO_PORTRAIT           = 0x2DA /* 32 x 32 pixels, 8 bpp */
};

#define SIZEOF_HERO (0x6da)

enum {
    NPC_NONE = 0,
    NPC_NARIELL = 1,
    NPC_HARIKA = 2,
    NPC_CURIAN = 3,
    NPC_ARDORA = 4,
    NPC_GARSVIK = 5,
    NPC_ERWO = 6
};

enum {
    HERO_TYPE_NONE = 0,
    HERO_TYPE_JUGGLER = 1,	/* Gaukler */
    HERO_TYPE_HUNTER = 2,	/* Jaeger */
    HERO_TYPE_WARRIOR = 3,	/* Krieger */
    HERO_TYPE_ESTRAY = 4,	/* Streuner */
    HERO_TYPE_THORWALIAN = 5,	/* Thorwaler */
    HERO_TYPE_DWARF = 6,	/* Zwerg */
    /* Magic users > 6 */
    HERO_TYPE_WITCH = 7,	/* Hexe */
    HERO_TYPE_DRUID = 8,	/* Druide */
    HERO_TYPE_MAGE = 9,		/* Magier */
    HERO_TYPE_GREEN_ELF = 10,	/* Auelf */
    HERO_TYPE_ICE_ELF = 11,	/* Firnelf */
    HERO_TYPE_SYLVAN_ELF = 12,	/* Waldelf */
};

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
    FIG_ACTION_FLEE = 16,
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
	unsigned short dead		:1;
	unsigned short asleep		:1;
	unsigned short petrified	:1; /* 1: enemy is petrified (from 'Paralue' spell) */
	unsigned short busy		:1;
	unsigned short bit4		:1;
	unsigned short tied		:1; /* 1: enemy is tied (from 'Band und Fessel' spell; all enemies in the final fight except the Orkchampion) */
	unsigned short mushroom		:1; /* 1: enemy is a mushroom (from 'Salander' spell) */
	unsigned short illusion		:1;
};

struct enemy_status2 {
	unsigned short tame	:1; /* from 'Bannbaladin', 'Herr der Tiere' or 'Sanftmut' spell */
	unsigned short renegade	:1; /* from 'Boeser Blick' spell */
	unsigned short scared	:1; /* from 'Horriphobus' spell */
	unsigned short dancing	:1; /* from 'Zwingtanz' spell */
};

struct item_status {
	/* item + 0x02 */
	unsigned short armor		:1;
	unsigned short weapon		:1;
	unsigned short useable	 	:1;
	unsigned short food		:1;

	unsigned short stackable	:1;
	unsigned short herb_potion	:1;
	unsigned short undropable	:1;
	unsigned short bit7		:1;
};

struct knapsack_status {
	/* knapsack position + 0x04 */
	unsigned short broken		:1;
	unsigned short half_empty	:1;
	unsigned short empty		:1;
	unsigned short magic_hidden	:1;
	unsigned short bit4		:1;
	unsigned short poison1		:1;
	unsigned short poison2		:1;
	unsigned short magic_known	:1;

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
    ENEMY_SHEET_ATTRIB_ORIG     = 0x03, /* Only main 7 attributes */
    ENEMY_SHEET_ATTRIB          = 0x04,
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
    ENEMY_SHEET_FIGHTER_ID      = 0x26,
    ENEMY_SHEET_VIEWDIR         = 0x27,
    ENEMY_SHEET_ATTACKS_LEFT    = 0x28, /* number attacks left in the current turn of a battle */
    ENEMY_SHEET_LEVEL           = 0x29,
    ENEMY_SHEET_DUMMY3          = 0x2a,
    ENEMY_SHEET_ACTION_ID       = 0x2b,
    ENEMY_SHEET_CUR_SPELL       = 0x2c,
    ENEMY_SHEET_ENEMY_ID        = 0x2d,
    ENEMY_SHEET_SAFTKRAFT       = 0x2e, /* stores extra damage of spell 'Saft, Kraft, Monstermacht' */
    ENEMY_SHEET_BLIND           = 0x2f, /* blind rounds remaining from 'Blitz' spell */
    ENEMY_SHEET_BROKEN          = 0x30, /* weapon broken? 0 = no, 1 = yes */
    ENEMY_SHEET_STATUS1         = 0x31,
    ENEMY_SHEET_STATUS2         = 0x32,
    ENEMY_SHEET_UNUSED8         = 0x33,
    ENEMY_SHEET_SIZE            = 0x34,
    ENEMY_SHEET_ROUND_APPEAR    = 0x35,
    ENEMY_SHEET_IS_ANIMAL       = 0x36, /* is the enemy an animal? */
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
    MONSTER_ATTRIB      = 0x03, /* Only main 7 attributes */
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
    MONSTER_IS_ANIMAL   = 0x24,
    MONSTER_SHOTS       = 0x25,
    MONSTER_SHOT_DAM    = 0x26,
    MONSTER_THROWS      = 0x28,
    MONSTER_THROW_DAM   = 0x29,
    MONSTER_LE_FLEE     = 0x2b
};

#define SIZEOF_MONSTER (44)

enum {
    FIGHTER_FIGURE      = 0x00,
    FIGHTER_NVF_NO      = 0x02,
    FIGHTER_CBX         = 0x03,
    FIGHTER_CBY         = 0x04,
    FIGHTER_OFFSETX     = 0x05,
    FIGHTER_OFFSETY     = 0x06,
    FIGHTER_HEIGHT      = 0x07,
    FIGHTER_WIDTH       = 0x08,
    FIGHTER_X1          = 0x09,
    FIGHTER_Y1          = 0x0a,
    FIGHTER_X2          = 0x0b,
    FIGHTER_Y2          = 0x0c,
    FIGHTER_RELOAD      = 0x0d, /* {0, -1 = update gfx data } */
    FIGHTER_SHEET       = 0x0e, /* 0xe274, 0xe2a8, 0xd8ce */
    FIGHTER_WSHEET      = 0x0f, /* 0xe274 */
    FIGHTER_ID          = 0x10, /* position in FIG_LIST_ARRAY */
    FIGHTER_Z           = 0x11,
    FIGHTER_VISIBLE     = 0x12, /* {0,1,2} */
    FIGHTER_TWOFIELDED  = 0x13, /* -1 if fighter is not twofielded. for twofielded fighter: head part: FIGHTER_TWOFIELDED can be used as index for FIG_TWOFIELDED_TABLE which contains the FIGHTER_ID); tail part: entry is FIGHTER_TWOFIELDED+20 of the head part. */
    FIGHTER_OBJ_ID      = 0x14, /* stores the id of the cb_entry of the square before the fighter entered it */
    FIGHTER_IS_ENEMY    = 0x15, /* {0 = hero, 1 = enemy, 2 = hero} */ /* strangly, at one position in seg039.cpp the value 2 is written */
    FIGHTER_SPRITE_NO   = 0x16, /* 0x12c0, 0x1531, 0x1210 */
    FIGHTER_GFXBUF      = 0x17, /* RealPt */
    FIGHTER_NEXT        = 0x1b, /* RealPt */
    FIGHTER_PREV        = 0x1f, /* RealPt */
};

#define SIZEOF_FIGHTER (0x23)


enum {
    ATTRIB_MU = 0, ATTRIB_KL, ATTRIB_CH, ATTRIB_FF, ATTRIB_GE, ATTRIB_IN,
    ATTRIB_KK, ATTRIB_AG, ATTRIB_HA, ATTRIB_RA, ATTRIB_GG, ATTRIB_TA, ATTRIB_NG,
    ATTRIB_JZ
};

enum {
    TA_WAFFENLOS = 0, TA_HIEBWAFFEN, TA_STICHWAFFEN, TA_SCHWERTER, TA_AEXTE,
    TA_SPEERE, TA_ZWEIHAENDER, TA_SCHUSSWAFFEN, TA_WURFWAFFEN, TA_AKROBATIK = 9,
    TA_KLETTERN, TA_KOERPERBEH, TA_REITEN, TA_SCHLEICHEN, TA_SCHWIMMEN,
    TA_SELBSTBEH, TA_TANZEN, TA_VERSTECKEN, TA_ZECHEN, TA_BEKEHREN = 19,
    TA_BETOEREN, TA_FEILSCHEN, TA_GASSENWISSEN, TA_LUEGEN, TA_MENSCHENKENT,
    TA_SCHAETZEN, TA_FAEHRTENSUCHEN = 26, TA_FESSELN, TA_ORIENTIERUNG,
    TA_PFLANZENKUNDE, TA_TIERKUNDE, TA_WILDNISLEBEN, TA_ALCHIMIE = 32,
    TA_ALTE_SPRACHEN, TA_GEOGRAPHIE, TA_GESCHICHTE, TA_GOETTER_KULTE,
    TA_KRIEGSKUNST, TA_LESEN, TA_MAGIEKUNDE, TA_SPRACHEN, TA_ABRICHTEN = 41,
    TA_FAHRZEUGE, TA_FALSCHSPIEL, TA_HEILEN_GIFT, TA_HEILEN_KRANKH,
    TA_HEILEN_WUNDEN, TA_MUSIZIEREN, TA_SCHLOESSER, TA_TASCHENDIEB,
    TA_GEFAHRENSINN = 50, TA_SINNESSCHAERFE
};

enum {
    LOCATION_UNKN1 = 1, LOCATION_TEMPLE, LOCATION_TAVERN, LOCATION_HEALER,
    LOCATION_MERCHANT, LOCATION_WILDCAMP, LOCATION_INN, LOCATION_SMITH,
    LOCATION_MARKET, LOCATION_CITIZEN, LOCATION_HARBOUR, LOCATION_MAP,
    LOCATION_INFORMER, LOCATION_DNGENTRY, LOCATION_UNKN2, LOCATION_HOUSE,
    LOCATION_SPECIAL, LOCATION_CITYCAMP
};

enum {
    GAME_STATE_MAIN = 0,
    GAME_STATE_DEAD = 1, /* all heroes dead */
    GAME_STATE_UNKNOWN = 2,
    GAME_STATE_QUIT = 3, /* ordinary quit */
    GAME_STATE_TIMEUP = 4, /* max. game time is up */
    GAME_STATE_OUTRO = 5, /* after (successfull) outro */
    GAME_STATE_FIGQUIT = 7, /* quit game during fight */
    GAME_STATE_VICTORY = 99 /* final fight won (before outro) */
};

enum {
    INFORMER_JURGE = 0, INFORMER_HJORE, INFORMER_YASMA, INFORMER_UMBRIK,
    INFORMER_ISLEIF, INFORMER_RAGNA, INFORMER_BEORN, INFORMER_ASGRIMM,
    INFORMER_ELIANE, INFORMER_OLVIR, INFORMER_SWAFNILD, INFORMER_TREBORN,
    INFORMER_UNICORN, INFORMER_ALGRID, INFORMER_TIOMAR
};

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
    TOWNS_THORWAL = 1, TOWNS_SERSKE, TOWNS_BREIDA, TOWNS_PEILINEN,
    TOWNS_ROVAMUND = 5, TOWNS_NORDVEST, TOWNS_KRAVIK, TOWNS_SKELELLE, TOWNS_MERSKE,
    TOWNS_EFFERDUN = 10, TOWNS_TJOILA, TOWNS_RUKIAN, TOWNS_ANGBODIRTAL, TOWNS_AUPLOG,
    TOWNS_VILNHEIM = 15, TOWNS_BODON, TOWNS_OBERORKEN, TOWNS_PHEXCAER, TOWNS_GROENVEL,
    TOWNS_FELSTEYN = 20, TOWNS_EINSIEDL, TOWNS_ORKANGER, TOWNS_CLANEGH, TOWNS_LISKOR,
    TOWNS_THOSS = 25, TOWNS_TJANSET, TOWNS_ALA, TOWNS_ORVIL, TOWNS_OVERTHORN,
    TOWNS_ROVIK = 30, TOWNS_HJALSING, TOWNS_GUDDASUN, TOWNS_KORD, TOWNS_TREBAN,
    TOWNS_ARYN = 35, TOWNS_RUNINSHA, TOWNS_OTTARJE, TOWNS_SKJAL, TOWNS_PREM,
    TOWNS_DASPOTA = 40, TOWNS_RYBON, TOWNS_LJASDAHL, TOWNS_VARNHEIM, TOWNS_VAERMHAG,
    TOWNS_TYLDON = 45, TOWNS_VIDSAND, TOWNS_BRENDHIL, TOWNS_MANRIN, TOWNS_FTJOILA,
    TOWNS_FANGBODI = 50, TOWNS_HJALLAND, TOWNS_RUNIN
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
    ARCHIVE_FILE_154            = 0x9a,
    ARCHIVE_FILE_155            = 0x9b,
    ARCHIVE_FILE_DIALOGS_TLK    = 0x9c,
    /*
        F092.TLK, HERBERG.TLK, 158, DASPOTA1.TLK, DASPOTA2.TLK, DASPOTA3.TLK,
        INFO1.TLK, INFO2.TLK, INFO3.TLK, INFO4.TLK, INFO5.TLK, INFO6.TLK,
        INFO7.TLK, SCHMIED.TLK, GHANDEL.TLK, KHANDEL.TLK, WHANDEL.TLK, F115.TLK,
        EREMIT.TLK, HYGGELIK.TLK
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
    ARCHIVE_FILE_DNGS_DDT       = 0x100,
    /*
        SHIP.DDT, F046.DDT, F051.DDT, F061.DDT, F076.DDT, F094.DDT, F100.DDT, F108.DDT,
        F126.DDT, F129.DDT, F131.DDT, OBER.DDT, PREM.DDT, THORWAL.DDT, FINAL.DDT
    */
    ARCHIVE_FILE_DNGS_DTX       = 0x10f,
    /*
        SHIP.DTX, F046.DTX, F051.DTX, F061.DTX, F076.DTX, F094.DTX, F100.DTX, F108.DTX,
        F126.DTX, F129.DTX, F131.DTX, OBER.DTX, PREM.DTX, THORWAL.DTX, FINAL.DTX
    */
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

enum {
  ITEM_ARROWS         = 0x0a,
  ITEM_BOLTS          = 0x0d,
  ITEM_DAGGER         = 0x0e,
  ITEM_LONGBOW        = 0x13,
  ITEM_TORCH_ON       = 0x16,
  ITEM_BEER           = 0x17,
  ITEM_LANTERN_OFF    = 0x19,
  ITEM_FLASK_GLASS    = 0x1f,
  ITEM_PICKLOCKS      = 0x23,
  ITEM_CLIMBING_HOOKS = 0x28,
  ITEM_OIL            = 0x29,
  ITEM_FLASK_BRONZE   = 0x2a,
  ITEM_FOOD_PACKAGES  = 0x2d,
  ITEM_SHIRT          = 0x30,
  ITEM_SHOES          = 0x32,
  ITEM_BOOTS          = 0x33,
  ITEM_LACEUP_SHOE    = 0x34,
  ITEM_ARMOR_LEATHER  = 0x35,
  ITEM_SHURIN_POISON  = 0x37,
  ITEM_ARAX_POISON    = 0x38,
  ITEM_ANGST_POISON   = 0x39,
  ITEM_SLEEP_POISON   = 0x3a,
  ITEM_GOLDLEIM       = 0x3b,
  ITEM_EINBEERE       = 0x3c, /* two bytes */
  ITEM_WHIRLWEED      = 0x3d, /* two bytes */
  ITEM_KROTENSCHEMEL  = 0x3e,
  ITEM_GULMOND_LEAF   = 0x3f, /* two bytes */
  ITEM_TARNELE        = 0x40, /* two bytes */
  ITEM_TORCH_OFF      = 0x41,
  ITEM_QUARTERSTAFF   = 0x45,
  ITEM_BALL_CRYSTAL   = 0x46,
  ITEM_ROBE_GREEN     = 0x4b,
  ITEM_PLATTENZEUG    = 0x52,
  ITEM_KETTENZEUG     = 0x53,
  ITEM_LEDERZEUG      = 0x54,
  ITEM_TINDERBOX      = 0x55,
  ITEM_LICORICE       = 0x59,
  ITEM_BONBONS        = 0x5a,
  ITEM_WINE           = 0x5b, /* two bytes */
  ITEM_BRANDY         = 0x5c, /* two bytes */
  ITEM_NET            = 0x61,
  ITEM_TRIDENT        = 0x66,
  ITEM_MENGBILAR      = 0x6d,
  ITEM_SHURIN         = 0x7a, /* two bytes */
  ITEM_BELMART        = 0x7b, /* two bytes */
  ITEM_DONF_SPRING    = 0x7c, /* two bytes */
  ITEM_MENCHAL        = 0x7d, /* two bytes */
  ITEM_ALRAUNE        = 0x7e,
  ITEM_ILMENBLATT     = 0x80,
  ITEM_FINAGE_TREE    = 0x81, /* two bytes */
  ITEM_JORUGA_ROOT    = 0x82, /* two bytes */
  ITEM_LOTUS          = 0x84,
  ITEM_MAGIC_WAND     = 0x85,
  ITEM_LOTUS_POISON   = 0x8d,
  ITEM_KUKRIS         = 0x8e,
  ITEM_BANNSTAUB      = 0x8f,
  ITEM_KROETEN_POISON = 0x90,
  ITEM_MU_ELIXIR      = 0x93,
  ITEM_KL_ELIXIR      = 0x94,
  ITEM_CH_ELIXIR      = 0x95,
  ITEM_FF_ELIXIR      = 0x96,
  ITEM_GE_ELIXIR      = 0x97,
  ITEM_IN_ELIXIR      = 0x98,
  ITEM_KK_ELIXIR      = 0x99,
  ITEM_OLGIN_ROOT     = 0x9c, /* two bytes */
  ITEM_KAIRANHALM     = 0x9d,
  ITEM_SICKLE         = 0xa1,
  ITEM_AMULET_BLUE    = 0xa3,
  ITEM_RING_RED       = 0xa5, /* four bytes */
  ITEM_EXPURGICUM     = 0xa6,
  ITEM_VOMICUM        = 0xa8,
  ITEM_CORONET_SILVER = 0xab,
  ITEM_ANTIVENOM      = 0xb4,
  ITEM_GRIMRING       = 0xb5,
  ITEM_BELT_SKULL     = 0xb6, /* two bytes */
  ITEM_GIRDLE_MIGHT   = 0xb7,
  ITEM_LOBPREISUNGEN  = 0xbd,
  ITEM_KUKRIS_DAGGER  = 0xc1,
  ITEM_HELMET         = 0xc4,
  ITEM_KUKRIS_MENGBILAR = 0xc8,
  ITEM_SILVER_MACE    = 0xd6,
  ITEM_JEWELRY_SILVER = 0xd7,
  ITEM_CORONET_BLUE   = 0xd9, /* two bytes */
  ITEM_BAG            = 0xdd,
  ITEM_MU_ELIXIR_BAD  = 0xe2,
  ITEM_KL_ELIXIR_BAD  = 0xe3,
  ITEM_CH_ELIXIR_BAD  = 0xe4,
  ITEM_FF_ELIXIR_BAD  = 0xe5,
  ITEM_GE_ELIXIR_BAD  = 0xe6,
  ITEM_IN_ELIXIR_BAD  = 0xe7,
  ITEM_KK_ELIXIR_BAD  = 0xe8,
  ITEM_MIASTHMATICUM  = 0xee,
  ITEM_HYLAILIC_FIRE  = 0xef,
  ITEM_CORONET_GREEN  = 0xf5,
  ITEM_LANTERN_ON     = 0xf9,
  ITEM_200_ARROWS     = 0xfb,
  ITEM_50_BOLTS       = 0xfc,
  ITEM_20_CLIMBING_HOOKS = 0xfd
};

enum {
  IS_DARK    = 0,
  IS_TORCH   = 1,
  IS_LANTERN = 2
};

enum {
  RECIPE_ITEM_ID = 0, /* two bytes */ /* item id of the recipe */
  RECIPE_INGREDIENTS = 2, /* two bytes [10] */ /* list of ingredients, two bytes each (item ids), terminated by -1 entry */
  RECIPE_OUTCOME = 22, /* two bytes */ /* item id of the outcome of the recipe */
  RECIPE_AE = 24, /* AE needed */
  RECIPE_DIFFICULTY = 26, /* Erschwernis fuer die Alchemie-Talentprobe */
  RECIPE_DURATION = 27, /* time needed to brew the recipe, in hours */
  RECIPE_SIZE = 28
};
#endif
