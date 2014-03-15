#if !defined(SCHWEIF_H)
#include "v302de.h"
#endif

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
 *	@dummy:		yet unknown, maybe unused
 */
struct hero_status {
	/* hero + 0xaa */
	unsigned short dead	:1;
	unsigned short sleeps	:1;
	unsigned short stoned	:1;
	unsigned short unkn1	:1;
	unsigned short cham	:1;
	unsigned short cursed	:1;
	unsigned short uncon	:1;
	unsigned short unkn2	:1;
	/* hero + 0xab */
	unsigned short unkn3	:1;
	unsigned short dup	:1;
	unsigned short dummy	:6;
};

struct item_status {
	/* item + 0x02 */
	unsigned short armor		 :1;
	unsigned short weapon		 :1;
	unsigned short bit2		 :1;
	unsigned short food		 :1;
	unsigned short stackable	 :1;
	unsigned short herb_potion	 :1;
	unsigned short bit6		 :1;
	unsigned short bit7		 :1;
};

struct knapsack_status {
	/* knapsack position + 0x04 */
	unsigned short bit0		 :1;
	unsigned short half_empty	 :1;
	unsigned short empty		 :1;
	unsigned short bit3		 :1;
	unsigned short bit4		 :1;
	unsigned short bit5		 :1;
	unsigned short bit6		 :1;
	unsigned short bit7		 :1;
};

/* Informer */
enum {	JURGE = 1, HJORE, YASMA, UMBRIK, ISLEIF,
	RAGNA, BEORN, ASGRIMM, ELIANE, OLVIR,
	SWAFNILD, KOLBERG, UNICORN, ALGRID, TIOMAR };

#endif
