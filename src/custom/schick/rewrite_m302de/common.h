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

#endif
