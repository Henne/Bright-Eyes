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

/* Informer */
enum {	JURGE = 1, HJORE, YASMA, UMBRIK, ISLEIF,
	RAGNA, BEORN, ASGRIMM, ELIANE, OLVIR,
	SWAFNILD, KOLBERG, UNICORN, ALGRID, TIOMAR };

#endif
