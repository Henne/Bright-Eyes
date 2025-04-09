#if !defined(M302de_COMMON_H)
#define M302de_COMMON_H

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
	NORTH	= 0,
	EAST	= 1,
	SOUTH	= 2,
	WEST	= 3
};

enum {
	SEASON_WINTER = 0,
	SEASON_SPRING = 1,
	SEASON_SUMMER = 2,
	SEASON_AUTUMN = 3
};

enum {
	ANI_AREA_NAME		 = 0x00, /* String of length 5 (zero-terminated) */
	ANI_AREA_X		 = 0x05,
	ANI_AREA_Y		 = 0x07,
	ANI_AREA_HEIGHT		 = 0x08,
	ANI_AREA_WIDTH		 = 0x09,
	ANI_AREA_CYCLIC		 = 0x0b,
	ANI_AREA_PICS		 = 0x0c,
	ANI_AREA_PICS_TAB	 = 0x0d, /* RealPt[20] */
	ANI_AREA_CHANGES	 = 0x5d,
	ANI_AREA_CHANGES_TB	 = 0x5f, /* struct{ short pic, duration; }[42] */
	SIZEOF_ANI_AREA 	 = 0x107
};

enum {
	ACTION_ID_ESC = 1,
	ACTION_ID_CLOSING_SQUARE_BRACKET = 27, /* the ']' key. used in buy_screen() at a merchant to decrease the number of items. */
	ACTION_ID_RETURN = 28,
	ACTION_ID_J = 44, /* the 'J' key. used in seg097.cpp in GUI_menu_input (when called from GUI_bool()). selects the "Ja" (yes) answer. */
	ACTION_ID_N = 49, /* the 'N' key. used in seg097.cpp in GUI_menu_input (when called from GUI_bool()). selects the "Nein" (no) answer. */
	ACTION_ID_SLASH = 53, // the '/' key. used in buy_screen() at a merchant to increase the number of items.
	ACTION_ID_SPACE = 57,
	ACTION_ID_UP = 72,
	ACTION_ID_PAGE_UP = 73, /* opens the menu */
	ACTION_ID_LEFT = 75,
	ACTION_ID_RIGHT = 77,
	ACTION_ID_DOWN = 80,
	ACTION_ID_PAGE_DOWN = 81, /* closes the menu */
	/* in seg048.cpp there is "if (ds_readws(ACTION) >= 128 && ds_readws(ACTION) <= 152) [...]".
	 * I think these 25 numbers are used for the icons in the character screen, probably in the order
	 * head, arms, body, right hand, left hand, legs, feet, knapsack1 .. knapsack16; eye, mouth */
	ACTION_ID_ICON_1 = 129,
	ACTION_ID_ICON_2 = 130,
	ACTION_ID_ICON_3 = 131,
	ACTION_ID_ICON_4 = 132,
	ACTION_ID_ICON_5 = 133,
	ACTION_ID_ICON_6 = 134,
	ACTION_ID_ICON_7 = 135,
	ACTION_ID_ICON_8 = 136,
	ACTION_ID_ICON_9 = 137,
	ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK = 144, /* set in buy_screen() at a merchant if an item is right-clicked to decrease the count. */
	ACTION_ID_240 = 240, /* what does it mean? used in status_menu() in seg048.cpp */
	/* In seg057.cpp and seg058.cpp there is "if (ds_readws(ACTION) >= 241 && ds_readws(ACTION) <= 247) [...]" */
	ACTION_ID_VOID = 999
};

/* 26 ingame timers for (in this order) active miracles (6), unavailable talent usages (2), active spells (2) and unavailable miracles (16). */
enum {
	INGAME_TIMER_RONDRA_NO_SPELLS = 0, /* Rondra miracle active: block spells (6 hours) */
	INGAME_TIMER_EFFERD_FIND_WATER = 1, /* Efferd miracle active: search for water in wildcamp will always be successful (3 days) */
	INGAME_TIMER_EFFERD_SAFE_PASSAGE = 2, /* Efferd miracle active: no incidences on sea traveling (3 days) */
	INGAME_TIMER_FIRUN_HUNT = 3, /* Firun miracle active: hunting in wildcamps will always be successful (3 or 7 days) */
	INGAME_TIMER_TRAVIA_SAFE_REST = 4, /* Travia miracle active: safety during the night (no camp attacks during sleep) active (7 days) */
	INGAME_TIMER_BORON_UNDEAD = 5, /* Boron miracle active: protection from undead (but apparently, does not have an impact anywhere...) (3 days) */
	INGAME_TIMER_AKROBATIK = 6, /* talent usage unavailable: Akrobatik (8 hours) */
	INGAME_TIMER_MUSIZIEREN = 7, /* talent usage unavailable: Musizieren (8 hours) */
	INGAME_TIMER_FLIM_FLAM = 8, /* spell 'Flim Flam' is active */
	INGAME_TIMER_DARKNESS = 9, /* spell 'Dunkelheit' is active */
	INGAME_TIMER_PRAIOS_MU = 10, /* Praios miracle unavailable: MU +1 (1 day) */
	INGAME_TIMER_PRAIOS_MR = 11, /* Praios miracle unavailable: MR +99 (3 days) */
	INGAME_TIMER_RONDRA_SWORDS = 12, /* Rondra miracle unavailable: Schwerter +1, AT + 1 (3 days) */
	INGAME_TIMER_RONDRA_MAGIC_WEAPON = 13, /* Rondra miracle unavailable: make weapon magic (1 day) */
	INGAME_TIMER_EFFERD_SWIM = 14, /* Efferd miracle unavailable: Schwimmen +2 (4 days) */
	INGAME_TIMER_BORON_TA = 15, /* Boron miracle unavailable: TA -1 (4 days) */
	INGAME_TIMER_HESINDE_ANALUES = 16, /* Hesinde miracle unavailable: 'Analues Arkanstruktur +1' (5 days) */
	INGAME_TIMER_HESINDE_MR = 17, /* Hesinde miracle unavailable: MR +5 (4 days) */
	INGAME_TIMER_FIRUN_SATED = 18, /* Firun miracle unavailable: no hunger or thirst (7 days) */
	INGAME_TIMER_PHEX_THIEF = 19, /* Phex miracle unavailable: talents Schleichen +1, Taschendiebstahl +1 (3 days) */
	INGAME_TIMER_PHEX_FEILSCHEN = 20, /* Phex miracle unavailable: Feilschen +1 (3 days) */
	INGAME_TIMER_PHEX_FF = 21, /* Phex miracle unavailable: FF +1 (3 days) */
	INGAME_TIMER_INGERIMM_MAGIC_WEAPON = 22, /* Ingerimm miracle unavailable: make weapon magic (1 day) */
	INGAME_TIMER_RAHJA_TALENTS = 23, /* Rahja miracle unavailable: Tanzen +1, Betoeren +1 (7 days) */
	INGAME_TIMER_RAHJA_CH = 24, /* Rahja miracle unavailable: CH +1 (3 days) */
	INGAME_TIMER_RAHJA_TALENTS_PERMANENT = 25 /* Rahja miracle unavailable: Tanzen +1 (permanent), Betoeren +1 (permanent) (7 days) */
};

/**
 *	struct hero_flags - status of the hero
 *	@dead:		1 = dead / 0 = not dead
 *	@asleep:	1 = asleep / 0 = awake
 *	@petrified:	1 = petrified / 0 = not petrified
 *	@brewing:	1 = brewing an alchemical recipe in an inn / 0 = not brewing
 *	@chamaelioni:	1 = 'Chamaelioni' spell active / 0 = spell not active (effect of active 'Chamaelioni': AT-5 for attacker)
 *	@renegade:	1 = renegade (from 'Boeser Blick' spell) / 0 = normal
 *	@unconscious:	1 = unconscious / 0 = conscious
 *	@tied:		1 = hero tied / 0 = not tied (from hero 'Band und Fessel' spell)
 *
 *	@scared:	1 = scared and wants to flee (from 'Horriphobus' spell or Angstgift) / 0 = not scared
 *	@dummy2:	probably unused
 *	@duplicatus:	1 = 'Duplicatus' spell active / spell not active (effect: AT/2 for attacker, applied after the 'Chamaelioni' effect)
 *	@tame:		1 = tame (from enemy 'Bannbaladin' spell) / 0 = normal
 *	@seen_phantom:	1 = hero has seen the horrible phantom in the 'Verlassene Herberge' dungeon which gave MU -3 for 5 hours (same position where the Sphaerenriss can happen).
 *	@gods_pissed:	1 = gods pissed (no more miracles) / 0 = gods normal (activated by praising the nameless god, deactivated by destroying the statue of the nameless god)
 *	@transformed:	1 = hero is transformed (all positive attributes -1, from the cursed chest on the Totenschiff) / 0 not transformed (can be cured by 'Verwandlung beenden' spell or Praios/Hesinde miracle)
 *	@encouraged:	1 = MU increased by 3 (seg082.cpp, probably a dungeon event) / 0 = attribute back to normal.
 */
struct hero_flags {
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
	/* see https://github.com/shihan42/BrightEyesWiki/wiki/CHR-NPC */
	HERO_NAME			= 0x000, /* 16 bytes */
	HERO_NAME2			= 0x010, /* 16 bytes */
	HERO_NR_INVENTORY_SLOTS_FILLED	= 0x020, /* 1 byte */ /* number of occupied item slots in the inventory. (equipped items are not included (really??)) */
	HERO_TYPE			= 0x021, /* 1 byte */ /* See enum HERO_TYPE_* below. */
	HERO_SEX			= 0x022, /* 1 byte */
	HERO_HEIGHT			= 0x023, /* 1 byte */ /* unit: cm */
	HERO_WEIGHT			= 0x024, /* 2 bytes */ /* unit: ounces */
	HERO_GOD			= 0x026, /* 1 byte */
	HERO_LEVEL			= 0x027, /* 1 byte */
	HERO_AP 			= 0x028, /* 4 bytes */
	HERO_MONEY			= 0x02C, /* 4 bytes */ /* unit: Heller */
	HERO_RS_BONUS1			= 0x030, /* 1 byte */ /* RS-Bonusu= RS-Bonus1 + RS-Bonus2 */
	HERO_RS_BONUS2			= 0x031 /* 1 byte */ ,
	HERO_RS_BE			= 0x032, /* 1 byte */ /* Ruestungsschutzbehinderung */
	HERO_BP_LEFT			= 0x033, /* 1 byte */
	HERO_ATTRIB_ORIG		= 0x034, /* 42 = 14 * 3 bytes. array of 14 entries of structs of 3 bytes of the form <HERO_ATTRIB_ORIG (1 byte), HERO_ATTRIB (1 byte), HERO_ATTRIB_MOD (1 byte)> See enum ATTRIB_* below. */
	HERO_ATTRIB			= 0x035, /* See enum ATTRIB_* below. */
	HERO_ATTRIB_MOD			= 0x036, /* See enum ATTRIB_* below. */
	HERO_LE_ORIG			= 0x05E, /* 2 bytes */
	HERO_LE 			= 0x060, /* 2 bytes */
	HERO_AE_ORIG			= 0x062, /* 2 bytes */
	HERO_AE 			= 0x064, /* 2 bytes */
	HERO_MR 			= 0x066, /* 1 byte */
	HERO_ATPA_BASIS			= 0x067, /* 1 byte */
	HERO_AT 			= 0x068, /* 7 = 7 * 1 bytes */ /* array with 7 entries each of size 1 byte containing the AT values of TA_WAFFENLOS, TA_HIEBWAFFEN, TA_STICHWAFFEN, TA_SCHWERTER, TA_AEXTE, TA_SPEERE, TA_ZWEIHAENDER */
	HERO_PA 			= 0x06F, /* 7 = 7 * 1 bytes */ /* array with 7 entries each of size 1 byte containing the PA values of TA_WAFFENLOS, TA_HIEBWAFFEN, TA_STICHWAFFEN, TA_SCHWERTER, TA_AEXTE, TA_SPEERE, TA_ZWEIHAENDER */
	HERO_AT_MOD			= 0x076, /* 1 byte */
	HERO_PA_MOD			= 0x077, /* 1 byte */
	HERO_WEAPON_TYPE		= 0x078, /* 1 byte */ /* type of the equipped weapon */
	HERO_ATTACK_TYPE		= 0x079, /* 1 byte */ /* 0x00 = normal; 0x02 = aggressiv; 0xFE = vorsichtig */
	HERO_LE_MOD			= 0x07A, /* 1 byte */ /* permanent LE mod */
	HERO_TIMER_ID			= 0x07B, /* 1 byte */
	HERO_START_GEAR			= 0x07C, /* 1 byte */ /* has been picked up at a temple and therefore got the initial gear set: 1 = true, 0 = false. Bit1 is used as a flag if the hero got the IN attribute bonus at the black eye at the Monolith (Einsiedlersee <-> Einsiedlersee, tevent135). Bits2--7 appear to be unused. */ /* TODO: Better name? */
	HERO_HERBS			= 0x07D, /* 1 byte */ /* keine = 0, Belmart = 1, Menchalkaktus = 2 */
	HERO_HUNGER_TIMER		= 0x07E, /* 1 byte */ /* timer for no-hunger-miracle */
	HERO_HUNGER			= 0x07F, /* 1 byte */ /* percentage */
	HERO_THIRST			= 0x080, /* 1 byte */ /* percentage */
	HERO_FIGHTER_ID			= 0x081, /* 1 byte */
	HERO_VIEWDIR			= 0x082, /* 1 byte */
	HERO_ACTIONS			= 0x083, /* 1 byte */ /* corresponds to ENEMY_SHEET_ATTACKS */
	HERO_ACTION_ID			= 0x084, /* 1 byte */ /* last fight action */
	HERO_SPELL_ID			= 0x085, /* 1 byte */ /* last spell in fight */
	HERO_ENEMY_ID			= 0x086, /* 1 byte */ /* last enemy in fight */
	HERO_GROUP_NO			= 0x087, /* 1 byte */
	HERO_TEMPLE_ID			= 0x088, /* 1 byte */
	HERO_NPC_ID			= 0x089, /* 1 byte */ /* See enum NPC_* below. */
	HERO_GROUP_POS			= 0x08A, /* 1 byte */ /* 0x01 bis 0x06, 0x00 = not in group */
	HERO_HEAL_TIMER			= 0x08B, /* 4 bytes */
	HERO_STAFFSPELL_TIMER		= 0x08F, /* 4 bytes */
	HERO_RECIPE_ID			= 0x093, /* 1 byte */ /* id of the alchemic recipe the hero is brewing */
	HERO_RECIPE_TIMER		= 0x094, /* 1 byte */ /* timer till completion of alchemical brewing process, in days */
	HERO_RUHE_KOERPER		= 0x095, /* 1 byte */ /* 1 = Ruhe Koerper spell is active */
	HERO_BLIND			= 0x096, /* 1 byte */ /* remaining fight rounds the hero is blinded from 'Blitz' spell */
	HERO_ECLIPTIFACTUS		= 0x097, /* 1 byte */ /* remaining fight rounds the hero is shadowed from 'Ecliptifactus' spell */
	HERO_SAFTKRAFT			= 0x098, /* 1 byte */ /* stores extra damage of spell 'Saft, Kraft, Monstermacht' */
	HERO_FIREBAN			= 0x099, /* 1 byte */ /* 1 = 'Feuerbann' spell is active, 0 = inactive */
	HERO_INVISIBLE			= 0x09A, /* 1 byte */ /* 1 = 'Visibili' spell is active, 0 = inactive */
	HERO_SPRITE_NO			= 0x09B, /* 1 byte */ /* fight gfx of hero, depending on type and sex */
	HERO_HOSTEL_ID			= 0x09C, /* 1 byte */ /* the id of the inn where the hero is doing alchemy */
	HERO_ESCAPE_POSITION		= 0x09D, /* 2 bytes */ /* the dungeon square the hero escaped to in a fight. read from FIG_FLEE_POSITION */
	HERO_JAIL			= 0x09F, /* 1 byte */ /* 1 = true, i.e. hero is in prison (from breaking into a house), 0 = false */
	HERO_AXXELERATUS		= 0x0A0, /* 1 byte */ /* 1 = 'Axxeleratus' spell is active, 0 = inactive */
	HERO_DRUNK			= 0x0A1, /* 1 byte */ /* 1 = true (hero drunk), 0 = false */
	HERO_UNKNOWN10			= 0x0A2, /* 8 bytes */ /* never used? */
	HERO_FLAGS1			= 0x0AA, /* 1 byte = 8 bits */ /* Bit0 = dead, Bit1 = asleep, Bit2 = petrified, Bit3 = brewing, Bit4 = Chamaelioni, Bit5 = renegade, Bit6 = unconscious, Bit7 = tied */
	HERO_FLAGS2			= 0x0AB, /* 1 byte = 8 bits */ /* Bit0 = scared, Bit1 = unused?, Bit2 = duplicatus, Bit3 = tame, Bit4 = seen_phantom, Bit5 = gods_pissed, Bit6 = transformed, Bit7 = encouraged */
	HERO_UNKNOWN11			= 0x0AC, /* 2 bytes */ /* never used? */
	HERO_ILLNESS			= 0x0AE, /* 40 = 8 * 5 bytes */ /* 5 bytes for each of the following illnesses: 0-none (these 5 bytes appear to be unused!) 1-Wundfieber, 2-Dumpfschädel, 3-Blaue Keuche, 4-Paralyse, 5-Schlachtenfieber, 6-Frostschäden, 7-Tollwut */
	HERO_POISON			= 0x0D6, /* 50 = 10 * 5 bytes */ /* 5 bytes for each of the following poisonings: 0-none (these 5 bytes appear to be unused!) 1-Shurinknollengift, 2-Arax, 3-Angstgift, 4-Schlafgift, 5-Goldleim, 6-Krötenschemel, 7-Lotusgift, 8-Kukris, 9-Bannstaubvergiftung */
	HERO_TALENTS			= 0x108, /* 52 = 52 * 1 bytes, see enum TA_* below */
	HERO_TA_RISE			= 0x13C, /* saved skill increases from last levelups */
	HERO_SPELLS			= 0x13D, /* 86 = 86 * 1 bytes, se enum SP_* below. Note that the first entry does not belong to an actual spell talent and is unused, apparentl. */
	HERO_SP_RISE			= 0x193, /* 1 byte */ /* saved spell increases from last levelups */
	HERO_MAGIC_SCHOOL		= 0x194, /* 1 byte */ /* only for mages */
	HERO_STAFFSPELL_LVL		= 0x195, /* 1 byte */ /* only for mages */
	HERO_INVENTORY			= 0x196, /* 322 = 23 * 14 bytes */ /* 23 inventory slots, each entry has 14 bytes */ /* first 7 slots: equipped inventory (head, arms etc.); following 16 slots: knapsack inventory */
	HERO_LOAD			= 0x2D8, /* 2 bytes */
	HERO_PORTRAIT			= 0x2DA /* 1024 = 32 * 32 bytes */ /* 32 x 32 pixels, 8 bpp */
};

#define SIZEOF_HERO (0x6da)

enum {
	NPC_NONE	= 0,
	NPC_NARIELL	= 1,
	NPC_HARIKA	= 2,
	NPC_CURIAN	= 3,
	NPC_ARDORA	= 4,
	NPC_GARSVIK	= 5,
	NPC_ERWO	= 6
};

enum {
	HERO_TYPE_NONE		= 0,
	HERO_TYPE_JUGGLER	= 1,	/* Gaukler */
	HERO_TYPE_HUNTER	= 2,	/* Jaeger */
	HERO_TYPE_WARRIOR	= 3,	/* Krieger */
	HERO_TYPE_ESTRAY	= 4,	/* Streuner */
	HERO_TYPE_THORWALIAN	= 5,	/* Thorwaler */
	HERO_TYPE_DWARF		= 6,	/* Zwerg */
	/* Magic users > 6 */
	HERO_TYPE_WITCH		= 7,	/* Hexe */
	HERO_TYPE_DRUID		= 8,	/* Druide */
	HERO_TYPE_MAGE		= 9,	/* Magier */
	HERO_TYPE_GREEN_ELF	= 10,	/* Auelf */
	HERO_TYPE_ICE_ELF	= 11,	/* Firnelf */
	HERO_TYPE_SYLVAN_ELF	= 12,	/* Waldelf */
};

enum {
	FIG_ACTION_MOVE			= 1,
	FIG_ACTION_MELEE_ATTACK		= 2,
	FIG_ACTION_GUARD		= 3,
	FIG_ACTION_SPELL		= 4,
	FIG_ACTION_USE_ITEM		= 5,
	FIG_ACTION_DROP_ITEM		= 6,
	FIG_ACTION_EXCHANGE_WEAPON	= 7,
	FIG_ACTION_EXCHANGE_ITEM	= 8,
	FIG_ACTION_CHECK_VALUES		= 9,
	FIG_ACTION_WAIT			= 10,
	FIG_ACTION_COMPUTER_FIGHT	= 11,
	FIG_ACTION_QUIT_AND_LOAD	= 12,
	FIG_ACTION_REPEAT_OPTION	= 13,
	FIG_ACTION_RANGE_ATTACK		= 15,
	FIG_ACTION_FLEE			= 16,
	FIG_ACTION_UNKNOWN2		= 100,
	FIG_ACTION_UNKNOWN3		= 102, /* drink potion? */
	FIG_ACTION_UNKNOWN4		= 103  /* cast spell? */
};

enum {
	ILLNESS_TYPE_NONE		= 0,
	ILLNESS_TYPE_WUNDFIEBER		= 1,
	ILLNESS_TYPE_DUMPFSCHAEDEL	= 2,
	ILLNESS_TYPE_BLAUE_KEUCHE	= 3,
	ILLNESS_TYPE_PARALYSE		= 4,
	ILLNESS_TYPE_SCHLACHTENFIEBER	= 5,
	ILLNESS_TYPE_FROSTSCHAEDEN	= 6,
	ILLNESS_TYPE_TOLLWUT		= 7
};

#define SIZEOF_HERO_ILLNESS (5)

enum {
	POISON_TYPE_NONE		= 0,
	POISON_TYPE_SHURINKNOLLENGIFT	= 1,
	POISON_TYPE_ARAX		= 2,
	POISON_TYPE_ANGSTGIFT		= 3,
	POISON_TYPE_SCHLAFGIFT		= 4,
	POISON_TYPE_GOLDLEIM		= 5,
	POISON_TYPE_KROETENSCHEMEL	= 6,
	POISON_TYPE_LOTUSGIFT		= 7,
	POISON_TYPE_KUKRIS		= 8,
	POISON_TYPE_BANNSTAUB		= 9
};

#define SIZEOF_HERO_POISON (5)

struct enemy_flags1 {
	/* enemy + 0x31 */
	unsigned short dead		:1;
	unsigned short asleep		:1;
	unsigned short petrified	:1; /* 1: enemy is petrified (from 'Paralue' spell) */
	unsigned short busy		:1;
	unsigned short bit4		:1; /* unused? */
	unsigned short tied		:1; /* 1: enemy is tied (from 'Band und Fessel' spell; all enemies in the final fight except the Orkchampion) */
	unsigned short mushroom		:1; /* 1: enemy is a mushroom (from 'Salander' spell) */
	unsigned short illusion		:1;
};

struct enemy_flags2 {
	/* enemy + 0x32 */
	unsigned short tame		:1; /* from 'Bannbaladin', 'Herr der Tiere' or 'Sanftmut' spell */
	unsigned short renegade		:1; /* from 'Boeser Blick' spell. removed by 'Horriphobus' spell or Angstgift. */
	unsigned short scared		:1; /* from 'Horriphobus' spell or Angstgift */
	unsigned short dancing		:1; /* from 'Zwingtanz' spell */
	unsigned short bit12		:1; /* unused? */
	unsigned short bit13		:1; /* unused? */
	unsigned short bit14		:1; /* unused? */
	unsigned short bit15		:1; /* unused? */
};

/* remark siebenstreich 2021-08-15:
 * I tried to combine these two structs into a single 2-byte bitfield 'enemy_flags' (similar to 'inventory_flags' below)
 * Subsequently, the macros enemy_dead, enemy_asleep etc. in v302de.h had to be adjusted.
 * However, no matter what I tried, this always broke binary BCC-compatibility. */

struct item_flags {
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

struct inventory_flags {
	/* inventory position + 0x04 */
	unsigned short broken		:1;
	unsigned short half_empty	:1; /* only used for ITEM_WATERSKIN */
	unsigned short empty		:1; /* only used for ITEM_WATERSKIN. for an empty waterskin, both  'empty' and 'half_empty' flags are set */
	unsigned short magic		:1;
	unsigned short bit4		:1; /* unused */
	unsigned short poison_expurgicum:1; /* flag set if a weapon is poisoned by Expurgicum */
	unsigned short poison_vomicum	:1; /* flag set if a weapon is poisoned by Vomicum */
	unsigned short magic_revealed	:1; /* flag set if it is known to the heroes that the item is magic.
					       obtained from academy in Thorwal or from 'Odem Arcanum' or 'Analues' spell */

	unsigned short bit8		:1; /* unused */
	unsigned short bit9		:1; /* unused */
	unsigned short bit10		:1; /* unused */
	unsigned short bit11		:1; /* unused */
	unsigned short bit12		:1; /* unused */
	unsigned short bit13		:1; /* unused */
	unsigned short bit14		:1; /* unused */
	unsigned short bit15		:1; /* unused */
};

struct inventory {
	/* for future use, currently the HERO_INVENTORY enum is used. */
	signed short item_id; /* +0 */
	signed short quantity; /* +2 */

	struct inventory_flags flags; /* +4 */
	signed char bf; /* +6 */
	signed char rs_lost; /* +7 */

	signed char lighting_timer; /* +8 */
	signed char poison_type; /* +9 */
	signed char nr_poison_charges; /* +10 */
	signed char unused1; /* +11 */

	signed char unused2; /* +12 */
	signed char unused3; /* +13 */
};

struct passages {
	RealPt p1;
	RealPt p2;
	signed char flag;	/* 0 or 1, write only */
	signed char v2;		/* 0, read only */
	unsigned char town;	/* */
	signed char v4;		/* write only */
};

struct informer {
	signed short name;	/* in TEXT.LTX */
	signed char city;	/* -1 = no city, else city id */
	signed char unknown;	/* {0, 1} */
};

/* dummy */
struct enemy_sheets {
	signed char mon_id;
	signed char gfx_id;
	signed char v[0x2f];

	/* 0x31 */
	struct enemy_flags1 flags1;
	
	/* 0x32 */
	struct enemy_flags2 flags2;

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
	ENEMY_SHEET_MON_ID		= 0x00,
	ENEMY_SHEET_GFX_ID		= 0x01,
	ENEMY_SHEET_RS			= 0x02,
	ENEMY_SHEET_ATTRIB_ORIG		= 0x03, /* Only main 7 attributes */
	ENEMY_SHEET_ATTRIB		= 0x04,
	ENEMY_SHEET_LE_ORIG		= 0x11,
	ENEMY_SHEET_LE			= 0x13,
	ENEMY_SHEET_AE_ORIG		= 0x15,
	ENEMY_SHEET_AE			= 0x17,
	ENEMY_SHEET_MR			= 0x19,
	ENEMY_SHEET_FIRSTAP		= 0x1a,
	ENEMY_SHEET_ATTACKS		= 0x1b,
	ENEMY_SHEET_AT			= 0x1c,
	ENEMY_SHEET_PA			= 0x1d,
	ENEMY_SHEET_DAM1		= 0x1e,
	ENEMY_SHEET_DAM2		= 0x20,
	ENEMY_SHEET_BP_ORIG		= 0x22,
	ENEMY_SHEET_BP			= 0x23,
	ENEMY_SHEET_MAGIC		= 0x24,
	ENEMY_SHEET_MAG_ID		= 0x25,
	ENEMY_SHEET_FIGHTER_ID		= 0x26,
	ENEMY_SHEET_VIEWDIR		= 0x27,
	ENEMY_SHEET_ATTACKS_LEFT	= 0x28, /* number attacks left in the current turn of a battle */
	ENEMY_SHEET_LEVEL		= 0x29,
	ENEMY_SHEET_DUMMY3		= 0x2a,
	ENEMY_SHEET_ACTION_ID		= 0x2b,
	ENEMY_SHEET_CUR_SPELL		= 0x2c,
	ENEMY_SHEET_ENEMY_ID		= 0x2d,
	ENEMY_SHEET_SAFTKRAFT		= 0x2e, /* stores extra damage of spell 'Saft, Kraft, Monstermacht' */
	ENEMY_SHEET_BLIND		= 0x2f, /* blind rounds remaining from 'Blitz' spell */
	ENEMY_SHEET_BROKEN		= 0x30, /* weapon broken? 0	= no, 1	= yes */
	ENEMY_SHEET_FLAGS1		= 0x31,
	ENEMY_SHEET_FLAGS2		= 0x32,
	ENEMY_SHEET_UNUSED8		= 0x33,
	ENEMY_SHEET_SIZE		= 0x34,
	ENEMY_SHEET_ROUND_APPEAR	= 0x35,
	ENEMY_SHEET_IS_ANIMAL		= 0x36, /* is the enemy an animal? */
	ENEMY_SHEET_SHOTS		= 0x37,
	ENEMY_SHEET_SHOT_DAM		= 0x38,
	ENEMY_SHEET_THROWS		= 0x3a,
	ENEMY_SHEET_THROW_DAM		= 0x3b,
	ENEMY_SHEET_LE_FLEE		= 0x3d
};

#define SIZEOF_ENEMY_SHEET (62)

enum {
	MONSTER_MON_ID		= 0x00,
	MONSTER_GFX_ID		= 0x01,
	MONSTER_RS		= 0x02,
	MONSTER_ATTRIB		= 0x03, /* Only main 7 attributes */
	MONSTER_LE		= 0x11,
	MONSTER_AE		= 0x13,
	MONSTER_MR		= 0x15,
	MONSTER_FIRSTAP		= 0x17,
	MONSTER_ATTACKS		= 0x18,
	MONSTER_AT		= 0x19,
	MONSTER_PA		= 0x1a,
	MONSTER_DAM1		= 0x1b,
	MONSTER_DAM2		= 0x1d,
	MONSTER_BP		= 0x1f,
	MONSTER_MAGIC		= 0x20,
	MONSTER_MAG_ID		= 0x21,
	MONSTER_LEVEL		= 0x22,
	MONSTER_SIZE		= 0x23,
	MONSTER_IS_ANIMAL	= 0x24,
	MONSTER_SHOTS		= 0x25,
	MONSTER_SHOT_DAM	= 0x26,
	MONSTER_THROWS		= 0x28,
	MONSTER_THROW_DAM	= 0x29,
	MONSTER_LE_FLEE		= 0x2b
};

#define SIZEOF_MONSTER (44)

enum {
	FIGHTER_FIGURE		= 0x00,
	FIGHTER_NVF_NO		= 0x02,
	FIGHTER_CBX		= 0x03,
	FIGHTER_CBY		= 0x04,
	FIGHTER_OFFSETX		= 0x05,
	FIGHTER_OFFSETY		= 0x06,
	FIGHTER_HEIGHT		= 0x07,
	FIGHTER_WIDTH		= 0x08,
	FIGHTER_X1		= 0x09,
	FIGHTER_Y1		= 0x0a,
	FIGHTER_X2		= 0x0b,
	FIGHTER_Y2		= 0x0c,
	FIGHTER_RELOAD		= 0x0d, /* {0, -1	= update gfx data } */
	FIGHTER_SHEET		= 0x0e, /* 0xe274, 0xe2a8, 0xd8ce */
	FIGHTER_WSHEET		= 0x0f, /* 0xe274 */
	FIGHTER_ID		= 0x10, /* position in FIG_LIST_ARRAY */
	FIGHTER_Z		= 0x11,
	FIGHTER_VISIBLE		= 0x12, /* {0,1,2} */
	FIGHTER_TWOFIELDED	= 0x13, /* -1 if fighter is not twofielded. for twofielded fighter: head part: FIGHTER_TWOFIELDED can be used as index for FIG_TWOFIELDED_TABLE which contains the FIGHTER_ID); tail part: entry is FIGHTER_TWOFIELDED+20 of the head part. */
	FIGHTER_OBJ_ID		= 0x14, /* stores the id of the cb_entry of the square before the fighter entered it */
	FIGHTER_IS_ENEMY	= 0x15, /* {0	= hero, 1	= enemy, 2	= hero} */ /* strangly, at one position in seg039.cpp the value 2 is written */
	FIGHTER_SPRITE_NO	= 0x16, /* 0x12c0, 0x1531, 0x1210 */
	FIGHTER_GFXBUF		= 0x17, /* RealPt */
	FIGHTER_NEXT		= 0x1b, /* RealPt */
	FIGHTER_PREV		= 0x1f, /* RealPt */
};

#define SIZEOF_FIGHTER (0x23)


enum {
	ATTRIB_MU = 0,
	ATTRIB_KL = 1,
	ATTRIB_CH = 2,
	ATTRIB_FF = 3,
	ATTRIB_GE = 4,
	ATTRIB_IN = 5,
	ATTRIB_KK = 6,
	ATTRIB_AG = 7,
	ATTRIB_HA = 8,
	ATTRIB_RA = 9,
	ATTRIB_GG = 10,
	ATTRIB_TA = 11,
	ATTRIB_NG = 12,
	ATTRIB_JZ = 13
};

enum {
	/* Kampf */
	TA_WAFFENLOS		= 0,
	TA_HIEBWAFFEN		= 1,
	TA_STICHWAFFEN		= 2,
	TA_SCHWERTER		= 3,
	TA_AEXTE		= 4,
	TA_SPEERE		= 5,
	TA_ZWEIHAENDER		= 6,
	TA_SCHUSSWAFFEN		= 7,
	TA_WURFWAFFEN		= 8,
	/* Körper */
	TA_AKROBATIK		= 9,
	TA_KLETTERN		= 10,
	TA_KOERPERBEHERRSCHUNG	= 11,
	TA_REITEN		= 12,
	TA_SCHLEICHEN		= 13,
	TA_SCHWIMMEN		= 14,
	TA_SELBSTBEHERRSCHUNG	= 15,
	TA_TANZEN		= 16,
	TA_VERSTECKEN		= 17,
	TA_ZECHEN		= 18,
	/* Gesellschaft */
	TA_BEKEHREN		= 19,
	TA_BETOEREN		= 20,
	TA_FEILSCHEN		= 21,
	TA_GASSENWISSEN		= 22,
	TA_LUEGEN		= 23,
	TA_MENSCHENKENNTNIS	= 24,
	TA_SCHAETZEN		= 25,
	/* Natur */
	TA_FAEHRTENSUCHEN	= 26,
	TA_FESSELN		= 27,
	TA_ORIENTIERUNG		= 28,
	TA_PFLANZENKUNDE	= 29,
	TA_TIERKUNDE		= 30,
	TA_WILDNISLEBEN		= 31,
	/* Wissen */
	TA_ALCHIMIE		= 32,
	TA_ALTE_SPRACHEN	= 33,
	TA_GEOGRAPHIE		= 34,
	TA_GESCHICHTE		= 35,
	TA_GOETTER_KULTE	= 36,
	TA_KRIEGSKUNST		= 37,
	TA_LESEN		= 38,
	TA_MAGIEKUNDE		= 39,
	TA_SPRACHEN		= 40,
	/* Handwerk */
	TA_ABRICHTEN		= 41,
	TA_FAHRZEUGE		= 42,
	TA_FALSCHSPIEL		= 43,
	TA_HEILEN_GIFT		= 44,
	TA_HEILEN_KRANKHEITEN	= 45,
	TA_HEILEN_WUNDEN	= 46,
	TA_MUSIZIEREN		= 47,
	TA_SCHLOESSER		= 48,
	TA_TASCHENDIEBSTAHL	= 49,
	/* Intuition */
	TA_GEFAHRENSINN		= 50,
	TA_SINNESSCHAERFE	= 51
};

enum {
	SP_NONE = 0,
	/* Antimagie */
	SP_BEHERRSCHUNG_BRECHEN = 1,
	SP_DESTRUCTIBO_ARCANITAS = 2,
	SP_GARDIANUM_PARADEI = 3,
	SP_ILLUSIONEN_ZERSTOEREN = 4,
	SP_VERWANDLUNG_BEENDEN = 5,
	/* Beherrschung */
	SP_BAND_UND_FESSEL = 6,
	SP_BANNBALADIN = 7,
	SP_BOESER_BLICK = 8,
	SP_GROSSE_GIER = 9,
	SP_GROSSE_VERWIRRUNG = 10,
	SP_HERR_UEBER_DAS_TIERREICH = 11,
	SP_HORRIPHOBUS = 12,
	SP_MAGISCHER_RAUB = 13,
	SP_RESPONDAMI_VERITAR = 14,
	SP_SANFTMUT = 15,
	SP_SOMNIGRAVIS_TAUSENDSCHLAF = 16,
	SP_ZWINGTANZ = 17,
	/* Daemonologie */
	SP_FUROR_BLUT_UND_SULPHURDAMPF = 18,
	SP_GEISTER_AUSTREIBEN = 19,
	SP_GEISTER_BESCHWOEREN = 20,
	SP_HEPTAGON_UND_KROETENEI = 21,
	SP_KRAEHENRUF = 22,
	SP_SKELETTARIUS_KRYPTADUFT = 23,
	/* Elemente */
	SP_ELEMENTARE_BESCHWOEREN = 24,
	SP_NIHILATIO_GRAVITAS = 25,
	SP_SOLIDIRID_FARBENSPIEL = 26,
	/* Bewegung */
	SP_AXXELERATUS_BLITZGESCHWIND = 27,
	SP_FORAMEN_FORAMINOR = 28,
	SP_MOTORICUS_MOTILITICH = 29,
	SP_SPURLOS_TRITTLOS_FAEHRTENLOS = 30,
	SP_TRANSVERSALIS_TELEPORT = 31,
	SP_UEBER_EIS_UND_UEBER_SCHNEE = 32,
	/* Heilung */
	SP_BALSAM_SALABUNDE = 33,
	SP_HEXENSPEICHEL = 34,
	SP_KLARUM_PURUM_KRAEUTERSUD = 35,
	SP_RUHE_KOERPER_RUHE_GEIST = 36,
	SP_TIERE_BESPRECHEN = 37,
	/* Wahrnehmung */
	SP_ADLERAUG_UND_LUCHSENOHR = 38,
	SP_ANALUES_ARCANSTRUKTUR = 39,
	SP_EIGENSCHAFTEN_SEID_GELESEN = 40,
	SP_EXPOSAMI_CREATUR = 41,
	SP_ODEM_ARCANUM = 42,
	SP_PENETRIZZEL_HOLZ_UND_STEIN = 43,
	SP_SENSIBAR_WAHR_UND_KLAR = 44,
	/* Visionen */
	SP_CHAMAELIONI_MIMIKRY = 45,
	SP_DUPLICATUS_DOPPELPEIN = 46,
	SP_HARMLOSE_GESTALT = 47,
	SP_HEXENKNOTEN = 48,
	/* Kampf */
	SP_BLITZ_DICH_FIND = 49,
	SP_ECLIPTIFACTUS = 50,
	SP_EISENROST_UND_GRUENER_SPAN = 51,
	SP_FULMINICTUS_DONNERKEIL = 52,
	SP_IGNIFAXIUS_FLAMMENSTRAHL = 53,
	SP_PLUMBUMBARUM_UND_NARRETEI = 54,
	SP_RADAU = 55,
	SP_SAFT_KRAFT_MONSTERMACHT = 56,
	SP_SCHARFES_AUGE_UND_SICHRE_HAND = 57,
	/* Verstaendigung */
	SP_HEXENBLICK = 58,
	SP_NEKROPATHIA_MODERND_LEICH = 59,
	/* Verwandlung */
	SP_ADLER_WOLF_UND_HAMMERHAI = 60,
	SP_ARCANO_PSYCHOSTABILIS = 61,
	SP_ARMATRUTZ = 62,
	SP_CH_STEIGERN = 63,
	SP_FEUERBANN = 64,
	SP_FF_STEIGERN = 65,
	SP_GE_STEIGERN = 66,
	SP_IN_STEIGERN = 67,
	SP_KK_STEIGERN = 68,
	SP_KL_STEIGERN = 69,
	SP_MU_STEIGERN = 70,
	SP_MUTABILI_HYBRIDIL = 71,
	SP_PARALUE_PARALEIN = 72,
	SP_SALANDER_MUTANDERER = 73,
	SP_IN_SEE_UND_FLUSS = 74,
	SP_VISIBILI_VANITAR = 75,
	/* Veraenderung */
	SP_ABVENENUM_PEST_UND_GALLE = 76,
	SP_AEOLITUS_WINDGEBRAUS = 77,
	SP_BRENNE_TOTER_STOFF = 78,
	SP_CLAUDIBUS_CLAVISTIBOR = 79,
	SP_DUNKELHEIT = 80,
	SP_WEICHES_ERSTARRE = 81,
	SP_FLIM_FLAM_FUNKEL = 82,
	SP_HARTES_SCHMELZE = 83,
	SP_SILENTIUM_SILENTILLE = 84,
	SP_STURMGEBRUELL_BESAENFTGE_DICH = 85
};

enum {
	HERO_INVENTORY_SLOT_HEAD	= 0,
	HERO_INVENTORY_SLOT_ARMS	= 1,
	HERO_INVENTORY_SLOT_BODY	= 2,
	HERO_INVENTORY_SLOT_RIGHT_HAND	= 3,
	HERO_INVENTORY_SLOT_LEFT_HAND	= 4,
	HERO_INVENTORY_SLOT_LEGS	= 5,
	HERO_INVENTORY_SLOT_FEET	= 6,
	HERO_INVENTORY_SLOT_KNAPSACK_1	= 7,
	HERO_INVENTORY_SLOT_KNAPSACK_2	= 8,
	HERO_INVENTORY_SLOT_KNAPSACK_3	= 9,
	HERO_INVENTORY_SLOT_KNAPSACK_4	= 10,
	HERO_INVENTORY_SLOT_KNAPSACK_5	= 11,
	HERO_INVENTORY_SLOT_KNAPSACK_6	= 12,
	HERO_INVENTORY_SLOT_KNAPSACK_7	= 13,
	HERO_INVENTORY_SLOT_KNAPSACK_8	= 14,
	HERO_INVENTORY_SLOT_KNAPSACK_9	= 15,
	HERO_INVENTORY_SLOT_KNAPSACK_10	= 16,
	HERO_INVENTORY_SLOT_KNAPSACK_11	= 17,
	HERO_INVENTORY_SLOT_KNAPSACK_12	= 18,
	HERO_INVENTORY_SLOT_KNAPSACK_13	= 19,
	HERO_INVENTORY_SLOT_KNAPSACK_14	= 20,
	HERO_INVENTORY_SLOT_KNAPSACK_15	= 21,
	HERO_INVENTORY_SLOT_KNAPSACK_16	= 22
};

#define NR_HERO_INVENTORY_SLOTS (23)

enum {
	/* describes an item in a single inventory slot of a hero */
	/* https://github.com/shihan42/BrightEyesWiki/wiki/CHR-NPC#inventarslots */
	INVENTORY_ITEM_ID		= 0,	/* 2 bytes */
	INVENTORY_QUANTITY		= 2,	/* 2 bytes */ /* for stackable items: nr of items in stack; for items with magic charges: nr of charges left */
	INVENTORY_FLAGS			= 4, 	/* 2 bytes */ /* bitfield, see inventory_flags */
	INVENTORY_BF 			= 6,	/* 1 byte */ /* Bruchfaktor. -99 means unbreakable */
	INVENTORY_RS_LOST		= 7, 	/* 1 byte */ /* so far only seen for body armour. (from 'Ignifaxius' spell or from traps in DNG03 (Spinnenhoehle)) */
	INVENTORY_LIGHTING_TIMER	= 8,	/* 1 byte */ /* for burning torch: number of remaining time, unit: 15 minutes */
	INVENTORY_POISON_TYPE		= 9,	/* 1 byte */ /* for poisoned weapon: poison type according to enum POISON_TYPE (0-kein Gift, 1-Schurinknollengift, 2-Arax, 3-Angstgift, 4-Schlafgift, 5-Goldleim, 6-Krötenschemelgift, 7-Lotosgift, 8-Kukris, 9-Bannstaub, 10-Expurgicum, 11-Vomicum). */
	INVENTORY_NR_POISON_CHARGES	= 10,	/* 1 byte */ /* for poisoned weapon: number of remaining poison charges (= successful attacs). */
	/* remaining part: apparently unused */
};
#define SIZEOF_INVENTORY (14)

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/ITEMS.DAT */
	/* structure of the entries of ITEMS.DAT */
	ITEM_STATS_GFX		= 0,
	ITEM_STATS_FLAGS	= 2, /* bitfield. bit 0: armor / bit 1: weapon / bit 2: useable / bit 3: nutrition / bit 4: stackable / bit 5: poison/herb/potion / bit 6: personal item (undropable) / bit 7: not usable by "use object"?? */
	ITEM_STATS_SUBTYPE	= 3, /* meaning depends on item type set in ITEM_STATS_FLAGS. weapon -> WEAPON_TYPE_..., armor -> ARMOR_TYPE_..., nutrition -> NUTRITION_TYPE... */
	ITEM_STATS_TABLE_INDEX	= 4,
	ITEM_STATS_WEIGHT	= 5, /* weight in ounces */
	ITEM_STATS_PRICE_UNIT	= 7, /* 1: Heller / 10: Silberstücke / 100: Dukaten */
	ITEM_STATS_PRICE	= 8, /* unit is ITEM_STATS_PRICE_UNIT. So the price in Heller is ITEM_STATS_PRICE_UNIT * ITEM_STATS_PRICE */
	ITEM_STATS_COMMONNESS	= 10,
	ITEM_STATS_MAGIC	= 11, /* 0: not magic / 1: magic */
	SIZEOF_ITEM_STATS	= 12
};

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/ITEMS.DAT */
	/* except the last entry ITEM_SUBTYPE_ARMOR_LEFT_HAND (shields), all values agree with HERO_INVENTORY_SLOT_... */
	ARMOR_TYPE_HEAD 	= 0,
	ARMOR_TYPE_ARMS 	= 1,
	ARMOR_TYPE_BODY 	= 2,
	ARMOR_TYPE_LEGS 	= 5,
	ARMOR_TYPE_FEET 	= 6,
	ARMOR_TYPE_LEFT_HAND 	= 9, /* shields */
};

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/ITEMS.DAT */
	/* parallel to TA_WAFFENLOS, TA_HIEBWAFFEN, TA_STICHWAFFEN ... */
	WEAPON_TYPE_WAFFENLOS	= 0, /* note that ammunition (arrows, bolts) have this weapon type entry */
	WEAPON_TYPE_HIEBWAFFE	= 1,
	WEAPON_TYPE_STICHWAFFE	= 2,
	WEAPON_TYPE_SCHWERT	= 3,
	WEAPON_TYPE_AXT		= 4,
	WEAPON_TYPE_SPEER	= 5,
	WEAPON_TYPE_ZWEIHAENDER	= 6,
	WEAPON_TYPE_SCHUSSWAFFE	= 7,
	WEAPON_TYPE_WURFWAFFE	= 8
};

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/SCHICKM.EXE#Waffentabelle */
	/* structure of the entries of WEAPONS_TABLE */
	WEAPON_STATS_DAMAGE_D6		= 0,
	WEAPON_STATS_DAMAGE_CONSTANT	= 1,
	WEAPON_STATS_DAMAGE_KK_BONUS	= 2, /* Körperkraft-Zuschlag */
	WEAPON_STATS_BF			= 3, /* Bruchfaktor */
	WEAPON_STATS_RANGED_INDEX	= 4, /* related to ranged weapons? */
	WEAPON_STATS_AT_MOD		= 5,
	WEAPON_STATS_PA_MOD		= 6,
	SIZEOF_WEAPON_STATS		= 7
};

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/SCHICKM.EXE#R%C3%BCstungstabelle */
	/* structure of the entries of ARMORS_TABLE */
	ARMOR_STATS_RS = 0, /* Rüstungsschutz */
	ARMOR_STATS_BE = 1, /* Behinderung */
	SIZEOF_ARMOR_STATS = 2
};

enum {
	/* structure of the entries of RANGED_WEAPON_TABLE */
	RANGED_WEAPON_STATS_DAMAGE_MODIFIER = 0, /* char[7] table with damage modifiers depending on the distance */
	RANGED_WEAPON_STATS_BASE_HANDICAP = 7,
	SIZEOF_RANGED_WEAPON_STATS = 8
	/* According to DSA3 rules (MSZ), there are the following distance types.
	 *
	 * name         distance          encoding in Schicksalsklinge
	 * extrem nah	1-5 Schritt       0
	 * sehr nah     5-10 Schritt	  1
	 * nah          10-15 Schritt     2
	 * mittel       15-25 Schritt     3
	 * weit         25-40 Schritt     4
	 * sehr weit    40-60 Schritt     5
	 * extrem weit  61-100 Schritt    6
	 *
	 * Moreover, there are the following types of sizes of the target
	 *
	 * name         examples                                         encoding in Schicksalsklinge
	 * winzig       Silbertaler, Drachenauge, Maus, Ratte, Kroete    0
	 * sehr klein   Schlange, Fasan, Katze, Rabe                     1
	 * klein        Wolf, Reh, Kobold, Zwerg                         2
	 * mittel       Goblin, Elf, Mensch, Ork                         3
	 * gross        Pferd, Elch, Oger, Troll                         4
	 * sehr gross   Scheunentor, Drache, Elefant, Riese              5
	 *
	 * The skill test handicap for the ranged attack depends on the distance and the size of the target.
	 * In Schicksalsklinge, RANGED_WEAPON_STATS_BASE_HANDICAP is the skill test handicap vs. a target which is extrem nah and winzig (0,0).
	 * The general formula for the handicap is base_handicap + 2 * distance - 2 * target_size.
	 *
	 * The damage is calculated as the base damage of the weapon (like D6 + 3 for the shortbow) + a distance modifier.
	 * The modifier is given in the RANGED_WEAPON_STATS_DAMAGE_MODIFIER array. */
};

enum {
	NUTRITION_TYPE_DRINK	= 0,
	NUTRITION_TYPE_FOOD	= 1
};

enum {
	LOCATION_UNKN1		= 1,
	LOCATION_TEMPLE		= 2,
	LOCATION_TAVERN		= 3,
	LOCATION_HEALER		= 4,
	LOCATION_MERCHANT	= 5,
	LOCATION_WILDCAMP	= 6,
	LOCATION_INN		= 7,
	LOCATION_SMITH		= 8,
	LOCATION_MARKET		= 9,
	LOCATION_CITIZEN	= 10,
	LOCATION_HARBOR 	= 11,
	LOCATION_MAP		= 12,
	LOCATION_INFORMER	= 13,
	LOCATION_DNGENTRY	= 14,
	LOCATION_UNKN2		= 15,
	LOCATION_HOUSE		= 16,
	LOCATION_SPECIAL	= 17,
	LOCATION_CITYCAMP	= 18
};

enum {
	GAME_STATE_MAIN		= 0,
	GAME_STATE_DEAD		= 1, /* all heroes dead */
	GAME_STATE_UNKNOWN	= 2,
	GAME_STATE_QUIT		= 3, /* ordinary quit */
	GAME_STATE_TIMEUP	= 4, /* max. game time is up */
	GAME_STATE_OUTRO	= 5, /* after (successful) outro */
	GAME_STATE_FIGQUIT	= 7, /* quit game during fight */
	GAME_STATE_VICTORY	= 99 /* final fight won (before outro) */
};

enum {
	GOD_PRAIOS		= 1,
	GOD_RONDRA		= 2,
	GOD_EFFERD		= 3,
	GOD_TRAVIA		= 4,
	GOD_BORON		= 5,
	GOD_HESINDE		= 6,
	GOD_FIRUN		= 7,
	GOD_TSA			= 8,
	GOD_PHEX		= 9,
	GOD_PERAINE		= 10,
	GOD_INGERIMM		= 11,
	GOD_RAHJA		= 12,
	GOD_SWAFNIR		= 13,
	GOD_IFIRN		= 14
};

enum {
	INFORMER_JURGE		= 0,
	INFORMER_HJORE		= 1,
	INFORMER_YASMA		= 2,
	INFORMER_UMBRIK		= 3,
	INFORMER_ISLEIF		= 4,
	INFORMER_RAGNA		= 5,
	INFORMER_BEORN		= 6,
	INFORMER_ASGRIMM	= 7,
	INFORMER_ELIANE		= 8,
	INFORMER_OLVIR		= 9,
	INFORMER_SWAFNILD	= 10,
	INFORMER_TREBORN	= 11,
	INFORMER_UNICORN	= 12,
	INFORMER_ALGRID		= 13,
	INFORMER_TIOMAR		= 14
};

/* FIGHT.LST */

enum {
	FIGHT_NAME			= 0x00,
	FIGHT_INTRO_SEEN		= 0x13,
	FIGHT_SCENARIO			= 0x14,
	FIGHT_MONSTERS_ID		= 0x16,	/* List of 20 monsters */
	FIGHT_MONSTERS_X		= 0x17,	/* */
	FIGHT_MONSTERS_Y		= 0x18,	/* */
	FIGHT_MONSTERS_VIEWDIR		= 0x19,	/* */
	FIGHT_MONSTERS_ROUND_APPEAR	= 0x1A,	/* 5 bytes each */
	FIGHT_PLAYERS_X			= 0x7A,	/* List of 7 players */
	FIGHT_PLAYERS_Y			= 0x7B,	/* */
	FIGHT_PLAYERS_VIEWDIR		= 0x7C,	/* */
	FIGHT_PLAYERS_ROUND_APPEAR	= 0x7D,	/* 4 bytes each */
	FIGHT_LOOT			= 0x96,	/* 2 bytes each: ID and 0x00 */
	FIGHT_DUCATS			= 0xD2,
	FIGHT_SILVER			= 0xD4,
	FIGHT_HELLER			= 0xD6
};

#define SIZEOF_FIGHT (216)
#define SIZEOF_FIGHT_MONSTER (5)
#define SIZEOF_FIGHT_PLAYER (4)

enum {
	FIGHTS_ZUFALL1_LAND	= 0,
	FIGHTS_LAND_FIGHT1_1	= 1,
	FIGHTS_ZUFALL2_LAND	= 2,
	FIGHTS_ZUFALL3_LAND	= 3,
	FIGHTS_ZUFALL4_LAND	= 4,
	FIGHTS_SHIP3		= 5,
	FIGHTS_SHIP4		= 6,
	FIGHTS_SHIP5		= 7,
	FIGHTS_SHIP6		= 8,
	FIGHTS_SHIP8		= 9,
	FIGHTS_SHIP9		= 10,
	FIGHTS_SHIP12		= 11,
	FIGHTS_SHIP14		= 12,
	FIGHTS_SHIP15		= 13,
	FIGHTS_SHIP17		= 14,
	FIGHTS_SHIP18		= 15,
	FIGHTS_SHIP19		= 16,
	FIGHTS_SHIP21		= 17,
	FIGHTS_SHIP22		= 18,
	FIGHTS_SHIP23A		= 19,
	FIGHTS_SHIP23B		= 20,
	FIGHTS_SHIP24		= 21,
	FIGHTS_SHIP10		= 22,
	FIGHTS_F061_2		= 23,
	FIGHTS_F061_3		= 24,
	FIGHTS_F061_4B		= 25,
	FIGHTS_F061_4A		= 26,
	FIGHTS_F061_5		= 27,
	FIGHTS_F061_6A		= 28,
	FIGHTS_F108_1		= 29,
	FIGHTS_F108_2		= 30,
	FIGHTS_F108_3A		= 31,
	FIGHTS_F108_3B		= 32,
	FIGHTS_F108_3C		= 33,
	FIGHTS_F108_4		= 34,
	FIGHTS_F108_6		= 35,
	FIGHTS_F108_9		= 36,
	FIGHTS_F108_7		= 37,
	FIGHTS_F051_05_4	= 38,
	FIGHTS_F051_05_3	= 39,
	FIGHTS_F051_05_2	= 40,
	FIGHTS_F051_05_1	= 41,
	FIGHTS_F051_07		= 42,
	FIGHTS_F051_09		= 43,
	FIGHTS_F051_13		= 44,
	FIGHTS_F108_15		= 45,
	FIGHTS_F051_15		= 46,
	FIGHTS_F051_16		= 47,
	FIGHTS_F051_16_1	= 48,
	FIGHTS_F051_03		= 49,
	FIGHTS_F051_04		= 50,
	FIGHTS_F051_17		= 51,
	FIGHTS_F051_18		= 52,
	FIGHTS_F051_19		= 53,
	FIGHTS_F051_20_2	= 54,
	FIGHTS_F051_18_3	= 55,
	FIGHTS_F046_01		= 56,
	FIGHTS_F046_04		= 57,
	FIGHTS_F046_06		= 58,
	FIGHTS_F046_07		= 59,
	FIGHTS_F046_10		= 60,
	FIGHTS_F046_12		= 61,
	FIGHTS_F046_13		= 62,
	FIGHTS_F046_15		= 63,
	FIGHTS_F046_16		= 64,
	FIGHTS_F046_18		= 65,
	FIGHTS_F046_22		= 66,
	FIGHTS_F046_24		= 67,
	FIGHTS_F046_25		= 68,
	FIGHTS_F046_26		= 69,
	FIGHTS_F046_26_5	= 70,
	FIGHTS_F046_27		= 71,
	FIGHTS_F046_28		= 72,
	FIGHTS_F046_31		= 73,
	FIGHTS_F076_04		= 74,
	FIGHTS_F076_06		= 75,
	FIGHTS_F076_07		= 76,
	FIGHTS_F076_10		= 77,
	FIGHTS_F076_11		= 78,
	FIGHTS_F076_13		= 79,
	FIGHTS_F094_02		= 80,
	FIGHTS_F094_05		= 81,
	FIGHTS_F094_10		= 82,
	FIGHTS_F094_13		= 83,
	FIGHTS_F094_17		= 84,
	FIGHTS_F094_19		= 85,
	FIGHTS_F094_22		= 86,
	FIGHTS_F100_01		= 87,
	FIGHTS_F100_03		= 88,
	FIGHTS_F100_05		= 89,
	FIGHTS_F100_12		= 90,
	FIGHTS_F100_13		= 91,
	FIGHTS_F126_03		= 92,
	FIGHTS_F126_07		= 93,
	FIGHTS_F126_08		= 94, /* fleeing cultist */
	FIGHTS_F126_09		= 95,
	FIGHTS_F126_11		= 96,
	FIGHTS_F126_12		= 97,
	FIGHTS_F126_13		= 98,
	FIGHTS_F126_17		= 99,
	FIGHTS_F126_18		= 100,
	FIGHTS_F126_20		= 101,
	FIGHTS_F126_22		= 102,
	FIGHTS_F126_23		= 103,
	FIGHTS_F126_25		= 104,
	FIGHTS_F126_27		= 105,
	FIGHTS_F126_28		= 106,
	FIGHTS_F129_05		= 107,
	FIGHTS_F129_08		= 108,
	FIGHTS_F129_09		= 109,
	FIGHTS_F129_18		= 110,
	FIGHTS_F129_21		= 111,
	FIGHTS_F129_23		= 112,
	FIGHTS_F129_24		= 113,
	FIGHTS_F129_25		= 114,
	FIGHTS_F129_27		= 115,
	FIGHTS_F129_28		= 116,
	FIGHTS_F129_29		= 117,
	FIGHTS_F129_30		= 118,
	FIGHTS_F131_01A		= 119,
	FIGHTS_F131_01B		= 120,
	FIGHTS_F131_04		= 121,
	FIGHTS_F131_05		= 122,
	FIGHTS_F131_06		= 123,
	FIGHTS_F131_07		= 124,
	FIGHTS_F131_08		= 125,
	FIGHTS_F131_10		= 126,
	FIGHTS_F131_11_1	= 127,
	FIGHTS_F131_14A		= 128,
	FIGHTS_F131_14B		= 129,
	FIGHTS_F131_16		= 130,
	FIGHTS_DFIN12		= 131,
	FIGHTS_DFIN16		= 132,
	FIGHTS_DFIN18A		= 133,
	FIGHTS_DFIN18B		= 134,
	FIGHTS_DFIN26		= 135,
	FIGHTS_DFIN27A		= 136,
	FIGHTS_DFIN27B		= 137,
	FIGHTS_DFIN28		= 138,
	FIGHTS_DFIN30		= 139,
	FIGHTS_DPRE10_1		= 140,
	FIGHTS_DOBE07		= 141,
	FIGHTS_DOBE09		= 142,
	FIGHTS_DOBE11		= 143,
	FIGHTS_DOBE20		= 144,
	FIGHTS_DOBE19		= 145,
	FIGHTS_DOBE22		= 146,
	FIGHTS_DTHO03		= 147,
	FIGHTS_DTHO05		= 148,
	FIGHTS_DTHO06		= 149,
	FIGHTS_DTHO09		= 150,
	FIGHTS_DTHO10		= 151,
	FIGHTS_DTHO13		= 152,
	FIGHTS_DTHO15		= 153,
	FIGHTS_DTHO16		= 154,
	FIGHTS_DTHO18		= 155,
	FIGHTS_DTHO19		= 156,
	FIGHTS_DTHO20		= 157,
	FIGHTS_DTHO20_1		= 158,
	FIGHTS_DTHO21A		= 159,
	FIGHTS_DTHO21B		= 160,
	FIGHTS_DTHO23		= 161,
	FIGHTS_DTHO25		= 162,
	FIGHTS_DTHO27		= 163,
	FIGHTS_DTHO43		= 164,
	FIGHTS_DTHO48_1		= 165,
	FIGHTS_DTHO49		= 166,
	FIGHTS_DTHO50		= 167,
	FIGHTS_DTHO53		= 168,
	FIGHTS_DTHO55		= 169,
	FIGHTS_DTHO56		= 170,
	FIGHTS_DTHO57		= 171,
	FIGHTS_DTHO57_1		= 172,
	FIGHTS_DTHO58		= 173,
	FIGHTS_DTHO59		= 174,
	FIGHTS_DTHO60		= 175,
	FIGHTS_DTHO61		= 176,
	FIGHTS_F031		= 177,
	FIGHTS_F035		= 178,
	FIGHTS_F046		= 179,
	FIGHTS_F064		= 180, /* Gorah */
	FIGHTS_F066		= 181,
	FIGHTS_F074		= 182,
	FIGHTS_F075_A		= 183,
	FIGHTS_F075_B		= 184,
	FIGHTS_F075_C		= 185,
	FIGHTS_F077		= 186,
	FIGHTS_F080		= 187,
	FIGHTS_F084		= 188, /* travel event 84 */
	FIGHTS_F099		= 189, /* 4 harpies */
	FIGHTS_F101		= 190,
	FIGHTS_F122		= 191,
	FIGHTS_F144		= 192, /* travel event 144: final fight */
	FIGHTS_DASP1A		= 193,
	FIGHTS_DASP1B		= 194,
	FIGHTS_DASP2		= 195,
	FIGHTS_DASP3		= 196,
	FIGHTS_DASP4		= 197,
	FIGHTS_DASP5		= 198,
	FIGHTS_DASP6A		= 199,
	FIGHTS_DASP6B		= 200,
	FIGHTS_DASP7		= 201,
	FIGHTS_DASP8		= 202,
	FIGHTS_DASP9		= 203,
	FIGHTS_DASP10		= 204,
	FIGHTS_DASP11		= 205,
	FIGHTS_DASP12A		= 206,
	FIGHTS_DASP12B		= 207,
	FIGHTS_DASP13		= 208,
	FIGHTS_DASP14		= 209,
	FIGHTS_DASP16		= 210,
	FIGHTS_DASP17		= 211,
	FIGHTS_DASP18		= 212,
	FIGHTS_THOR8		= 213,
	FIGHTS_CITYFIGHT1	= 214,
	FIGHTS_CITYFIGHT2	= 215,
	FIGHTS_CITYFIGHT3	= 216,
	FIGHTS_CAMPFIGHT1	= 217,
	FIGHTS_CAMPFIGHT2	= 218,
	FIGHTS_CAMPFIGHT3	= 219,
	FIGHTS_SHIP10_1		= 220,
	FIGHTS_F061_6B		= 221,
	FIGHTS_F051_16A		= 222,
	FIGHTS_F051_16B		= 223,
	FIGHTS_F051_14A		= 224,
	FIGHTS_F051_14B		= 225,
	FIGHTS_F051_117		= 226,
	FIGHTS_F129_124		= 227,
	FIGHTS_F131_08A		= 228,
	FIGHTS_F131_08B		= 229,
	FIGHTS_F031_14A		= 230,
	FIGHTS_DTH021A		= 231,
	FIGHTS_CAMPFIGHT4	= 232,
	FIGHTS_SHIP10_2		= 233,
	FIGHTS_F051_02		= 234,
	FIGHTS_F051_5		= 235,
	FIGHTS_PHEX3		= 236,
	FIGHTS_PHEX22		= 237,
	FIGHTS_PHEX23		= 238,
	FIGHTS_PHEX24		= 239,
	FIGHTS_F131_01		= 240,
	FIGHTS_F129_08A		= 241,
	FIGHTS_DTHO14		= 242,
	FIGHTS_WILD1		= 243,
	FIGHTS_WILD2		= 244,
	FIGHTS_WILD3		= 245,
	FIGHTS_WILD4		= 246,
	FIGHTS_WILD5		= 247,
	FIGHTS_WILD6		= 248,
	FIGHTS_WILD7		= 249,
	FIGHTS_WILD8		= 250,
	FIGHTS_S001		= 251,
	FIGHTS_F051_14C		= 252,
	FIGHTS_F129_17		= 253,
	FIGHTS_DCAMPFIGHT1	= 254,
	FIGHTS_DCAMPFIGHT2	= 255,
	FIGHTS_DCAMPFIGHT3	= 256,
	FIGHTS_DCAMPFIGHT4	= 257
};

enum {
	TOWNS_NONE                      =  0, // %0x00
	TOWNS_THORWAL			=  1, // %0x01
	TOWNS_SERSKE			=  2, // %0x02
	TOWNS_BREIDA			=  3, // %0x03
	TOWNS_PEILINEN			=  4, // %0x04
	TOWNS_ROVAMUND			=  5, // %0x05
	TOWNS_NORDVEST			=  6, // %0x06
	TOWNS_KRAVIK			=  7, // %0x07
	TOWNS_SKELELLEN			=  8, // %0x08
	TOWNS_MERSKE			=  9, // %0x09
	TOWNS_EFFERDUN			= 10, // %0x0a
	TOWNS_TJOILA			= 11, // %0x0b
	TOWNS_RUKIAN			= 12, // %0x0c
	TOWNS_ANGBODIRTAL		= 13, // %0x0d
	TOWNS_AUPLOG			= 14, // %0x0e
	TOWNS_VILNHEIM			= 15, // %0x0f
	TOWNS_BODON			= 16, // %0x10
	TOWNS_OBERORKEN			= 17, // %0x11
	TOWNS_PHEXCAER			= 18, // %0x12
	TOWNS_GROENVELDEN		= 19, // %0x13
	TOWNS_FELSTEYN			= 20, // %0x14
	TOWNS_EINSIEDLERSEE		= 21, // %0x15
	TOWNS_ORKANGER			= 22, // %0x16
	TOWNS_CLANEGH			= 23, // %0x17
	TOWNS_LISKOR			= 24, // %0x18
	TOWNS_THOSS			= 25, // %0x19
	TOWNS_TJANSET			= 26, // %0x1a
	TOWNS_ALA			= 27, // %0x1b
	TOWNS_ORVIL			= 28, // %0x1c
	TOWNS_OVERTHORN			= 29, // %0x1d
	TOWNS_ROVIK			= 30, // %0x1e
	TOWNS_HJALSINGOR		= 31, // %0x1f
	TOWNS_GUDDASUNDEN		= 32, // %0x20
	TOWNS_KORD			= 33, // %0x21
	TOWNS_TREBAN			= 34, // %0x22
	TOWNS_ARYN			= 35, // %0x23
	TOWNS_RUNINSHAVEN		= 36, // %0x24
	TOWNS_OTTARJE			= 37, // %0x25
	TOWNS_SKJAL			= 38, // %0x26
	TOWNS_PREM			= 39, // %0x27
	TOWNS_DASPOTA			= 40, // %0x28
	TOWNS_RYBON			= 41, // %0x29
	TOWNS_LJASDAHL			= 42, // %0x2a
	TOWNS_VARNHEIM			= 43, // %0x2b
	TOWNS_VAERMHAG			= 44, // %0x2c
	TOWNS_TYLDON			= 45, // %0x2d
	TOWNS_VIDSAND			= 46, // %0x2e
	TOWNS_BRENDHIL			= 47, // %0x2f
	TOWNS_MANRIN			= 48, // %0x30
	TOWNS_FAEHRSTATION_TJOILA	= 49, // %0x31
	TOWNS_FAEHRE_ANGBODIRTAL	= 50, // %0x32
	TOWNS_HJALLANDER_HOF		= 51, // %0x33
	TOWNS_LEUCHTTURM_RUNIN		= 52  // %0x34
};

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/DAT-%28Stadt%29 seems to be wrong */
	TOWN_TILE_STREET = 0,
	TOWN_TILE_TEMPLE = 1,
	TOWN_TILE_HOUSE_1 = 2, /* usually: stone wall, 3 stairs at the entrance, with a hatch under the window. */ /* dark brown */
	TOWN_TILE_HOUSE_2 = 3, /* usually: wooden blockhouse. red-white signs on the window shutters. */ //dark blue
	TOWN_TILE_HOUSE_3 = 4, /* usually: timber framework, window half-open. */ //olive green
	TOWN_TILE_HOUSE_4 = 5, /* usually: stone wall, red roof, trapezial ornament at the door. (there are exceptions in the city of Thorwal) */ /* dark purple */
	TOWN_TILE_WATER = 6,
	TOWN_TILE_GRASS = 7,
	TOWN_TILE_SIGNPOST = 8,
	TOWN_TILE_INN_OR_TAVERN = 9,
	TOWN_TILE_MERCHANT = 10, /* The lighthouse on Runin is also displayed in Grey */
	TOWN_TILE_SMITH = 11,
	TOWN_TILE_HEALER = 12,
	TOWN_TILE_BLACK_FINGER = 13 /* The black tower in the city of Thorwal */
};

enum {
	DUNGEONS_NONE = 0,
	DUNGEONS_TOTENSCHIFF = 1, /* random encounter in high seas */ /* wood */
	DUNGEONS_VERFALLENE_HERBERGE = 2, /* Ottarje-Daspota */ /* marble */
	DUNGEONS_SPINNENHOEHLE = 3, /* Ottarje-Skjal */ /* stone */
	DUNGEONS_WOLFSHOEHLE = 4, /* Ottarje-Orvil */ /* stone */
	DUNGEONS_GOBLINHOEHLE = 5, /* Felsteyn-Orkanger */ /* stone */
	DUNGEONS_DASPOTASCHATZ = 6, /* Rybon-Thoss */ /* stone */
	DUNGEONS_RUINE_DES_SCHWARZMAGIERS = 7, /* Rybon-Thoss */ /* marble */
	DUNGEONS_ORKBEHAUSUNG = 8, /* Phexcaer-Skelellen */ /* stone */
	DUNGEONS_KULTSTAETTE_DES_NAMENLOSEN = 9, /* on island Hjalland */ /* marble */
	DUNGEONS_DRACHENHORT = 10, /* on island Runin */ /* stone */
	DUNGEONS_PIRATENHOEHLE = 11, /* on island Manrek */ /* marble */
	DUNGEONS_ZWERGENFESTE = 12, /* in town Oberorken */ /* marble */
	DUNGEONS_VERLASSENE_MINE = 13, /* in town Prem */ /* marble */
	DUNGEONS_ZWINGFESTE = 14, /* in town Thorwal */ /* marble */
	DUNGEONS_HYGGELIKS_RUINE = 15 /* Phexcaer-Vilnheim / Phexcaer-Skelellen */ /* marble */
};

enum {
	DNG_MENU_MODE_PLAIN = 0,
	DNG_MENU_MODE_OPEN_DOOR = 1,
	DNG_MENU_MODE_OPEN_CHEST = 2,
	DNG_MENU_MODE_CLOSE_DOOR = 3,
	DNG_MENU_MODE_LEVER = 4,
	DNG_MENU_MODE_UNLOCK_DOOR = 5
};

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/DNG */
	DNG_TILE_CORRIDOR = 0,
	DNG_TILE_CLOSED_DOOR = 1, /* flag 1: 0 = 'locked' / 1 = 'unlocked' */
	DNG_TILE_OPEN_DOOR = 2,
	DNG_TILE_STAIR_DOWN = 3,
	DNG_TILE_STAIR_UP = 4,
	DNG_TILE_PIT_IN_CEILING = 5,
	DNG_TILE_PIT = 6,
	DNG_TILE_BLOCKER = 7, /* what is this? */
	DNG_TILE_CHEST = 8, /* flag 1: 0 = 'closed' / 1 = 'opened' */
	DNG_TILE_SMASHED_DOOR = 9,
	DNG_TILE_REMOVABLE_WALL = 10,
	DNG_TILE_SEMIPERMEABLE_WALL = 11, /* 4 flags for the 4 directions. 0 = 'blocked', 1 = 'accessible' */
	DNG_TILE_FAKE_WALL = 12,
	DNG_TILE_WALL = 15,
};

enum {
	MAP_TILE_BLACK = 0,
	MAP_TILE_RED = 1,
	MAP_TILE_BRIGHT_GREEN = 2,
	MAP_TILE_BLUE = 3,
	MAP_TILE_YELLOW_ARROW = 4,
	MAP_TILE_PINK = 5,
	MAP_TILE_LIGHT_BLUE = 6,
	MAP_TILE_CROSS = 7,
	MAP_TILE_LIGHT_PURPLE = 8, /* unused? */
	MAP_TILE_DARK_GREEN = 9,
	MAP_TILE_DARK_PURPLE = 10,
	MAP_TILE_DARK_RED = 11,
	MAP_TILE_ORANGE = 12,
	MAP_TILE_DARK_PINK = 13, /* unused? */
	MAP_TILE_OLIVE_GREEN = 14, /* unused? */
	MAP_TILE_GREY = 15,
	MAP_TILE_PURPLE_ARROW = 16,
	MAP_TILE_BROWN = 17,
	MAP_TILE_GREEN = 18,
	MAP_TILE_DARK_GREY = 19,
	MAP_TILE_DARK_BLUE = 20, /* unused? */
	MAP_TILE_DARK_BROWN = 21 /* unused? */
};

enum {
	ARCHIVE_FILE_PLAYM_UK		= 0x00,
	ARCHIVE_FILE_KDBACK_DAT		= 0x01,
	ARCHIVE_FILE_KCBACK_DAT		= 0x02,
	ARCHIVE_FILE_KLBACK_DAT		= 0x03,
	ARCHIVE_FILE_KSBACK_DAT		= 0x04,
	ARCHIVE_FILE_KARTE_DAT		= 0x05,
	ARCHIVE_FILE_BICONS 		= 0x06,
	ARCHIVE_FILE_OBJECTS_NVF	= 0x07,
	ARCHIVE_FILE_LROUT_DAT		= 0x08,
	ARCHIVE_FILE_SROUT_DAT		= 0x09,
	ARCHIVE_FILE_HSROUT_DAT		= 0x0a,
	ARCHIVE_FILE_FONT6 		= 0x0b,
	ARCHIVE_FILE_FONT8 		= 0x0c,
	ARCHIVE_FILE_GGSTS_NVF		= 0x0d,
	ARCHIVE_FILE_TEXT_LTX		= 0x0e,
	ARCHIVE_FILE_ICONS 		= 0x0f,
	ARCHIVE_FILE_MONSTER		= 0x10,
	ARCHIVE_FILE_MONSTER_TAB	= 0x11,
	ARCHIVE_FILE_COMPASS		= 0x12,
	ARCHIVE_FILE_MAPTEXT_LTX	= 0x13,
	ARCHIVE_FILE_ZUSTA_UK		= 0x14,
	ARCHIVE_FILE_FIGHTTXT_LTX	= 0x15,
	ARCHIVE_FILE_ANIS 		= 0x16,
	ARCHIVE_FILE_ANIS_TAB		= 0x17,
	ARCHIVE_FILE_MFIGS 		= 0x18,
	ARCHIVE_FILE_MFIGS_TAB		= 0x19,
	ARCHIVE_FILE_CITY_DAT		= 0x1a,
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
	ARCHIVE_FILE_CITY_LTX		= 0x4d,
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
	ARCHIVE_FILE_TAVERN_TLK		= 0x82,
	ARCHIVE_FILE_SPSTAR_NVF		= 0x83,
	ARCHIVE_FILE_GUERTEL_NVF	= 0x84,
	ARCHIVE_FILE_HAFEN_LTX		= 0x85,
	ARCHIVE_FILE_FACE_NVF		= 0x86,
	ARCHIVE_FILE_HYGBACK_NVF	= 0x87,
	ARCHIVE_FILE_HYGGELIK_NVF	= 0x88,
	ARCHIVE_FILE_SKULL_NVF		= 0x89,
	ARCHIVE_FILE_OUTRO1_NVF		= 0x8a,
	ARCHIVE_FILE_OUTRO2_NVF		= 0x8b,
	ARCHIVE_FILE_OUTRO3_NVF		= 0x8c,
	ARCHIVE_FILE_TOWNPAL_DAT	= 0x8d,
	ARCHIVE_FILE_THORWAL_XMI	= 0x8e,
	ARCHIVE_FILE_SUMMARY_XMI	= 0x8f,
	ARCHIVE_FILE_TEMPLE_XMI		= 0x90,
	ARCHIVE_FILE_TERMS_XMI		= 0x91,
	ARCHIVE_FILE_SMITH_XMI		= 0x92,
	ARCHIVE_FILE_INN_XMI		= 0x93,
	ARCHIVE_FILE_HEALER_XMI		= 0x94,
	ARCHIVE_FILE_CAMP_XMI		= 0x95,
	ARCHIVE_FILE_VICTORY_XMI	= 0x96,
	ARCHIVE_FILE_SAMPLE_AD		= 0x97,
	ARCHIVE_FILE_ATTIC		= 0x98,
	ARCHIVE_FILE_POPUP_DAT		= 0x99,
	ARCHIVE_FILE_154		= 0x9a,
	ARCHIVE_FILE_155		= 0x9b,
	ARCHIVE_FILE_DIALOGS_TLK	= 0x9c,
	/*
		F092.TLK, HERBERG.TLK, 158, DASPOTA1.TLK, DASPOTA2.TLK, DASPOTA3.TLK,
		INFO1.TLK, INFO2.TLK, INFO3.TLK, INFO4.TLK, INFO5.TLK, INFO6.TLK,
		INFO7.TLK, SCHMIED.TLK, GHANDEL.TLK, KHANDEL.TLK, WHANDEL.TLK, F115.TLK,
		EREMIT.TLK, HYGGELIK.TLK
	*/
	ARCHIVE_FILE_WEAPONS_NVF	= 0xb0,
	ARCHIVE_FILE_BUCH_DAT		= 0xb1,
	ARCHIVE_FILE_SPELLOBJ_NVF	= 0xb2,
	ARCHIVE_FILE_IN_HEADS_NVF	= 0xb3,
	ARCHIVE_FILE_TEMPICON		= 0xb4,
	ARCHIVE_FILE_SHIPSL_NVF		= 0xb5,
	ARCHIVE_FILE_STONESL_NVF	= 0xb6,
	ARCHIVE_FILE_MARBLESL_NVF	= 0xb7,
	ARCHIVE_FILE_TFLOOR1_NVF	= 0xb8,
	ARCHIVE_FILE_TDIVERSE_NVF	= 0xb9,
	ARCHIVE_FILE_HOUSE1_NVF		= 0xba,
	ARCHIVE_FILE_HOUSE2_NVF		= 0xbb,
	ARCHIVE_FILE_HOUSE3_NVF		= 0xbc,
	ARCHIVE_FILE_HOUSE4_NVF		= 0xbd,
	ARCHIVE_FILE_FEATURE_LTX	= 0xbe,
	ARCHIVE_FILE_FEATURE1_LTX	= 0xbf,
	ARCHIVE_FILE_FEATURE2_LTX	= 0xc0,
	ARCHIVE_FILE_FEATURE3_LTX	= 0xc1,
	ARCHIVE_FILE_FEATURE4_LTX	= 0xc2,
	ARCHIVE_FILE_FEATURE5_LTX	= 0xc3,
	ARCHIVE_FILE_FEATURE6_LTX	= 0xc4,
	ARCHIVE_FILE_FEATURE7_LTX	= 0xc5,
	ARCHIVE_FILE_FEATURE8_LTX	= 0xc6,
	ARCHIVE_FILE_FIGHTOBJ_NVF	= 0xc7,
	ARCHIVE_FILE_SCENARIO_LST	= 0xc8,
	ARCHIVE_FILE_KDLBACK_DAT	= 0xc9,
	ARCHIVE_FILE_KCLBACK_DAT	= 0xca,
	ARCHIVE_FILE_KLLBACK_DAT	= 0xcb,
	ARCHIVE_FILE_KSLBACK_DAT	= 0xcc,
	ARCHIVE_FILE_FIGHT_LST		= 0xcd,
	ARCHIVE_FILE_SKARTE_NVF		= 0xce,
	ARCHIVE_FILE_GAMES_NAM		= 0xcf,
	ARCHIVE_FILE_ITEMNAME		= 0xd0,
	ARCHIVE_FILE_MONNAMES		= 0xd1,
	ARCHIVE_FILE_THORWAL1_LTX	= 0xd2,
	ARCHIVE_FILE_THORWAL2_LTX	= 0xd3,
	ARCHIVE_FILE_DUNGEON_XMI	= 0xd4,
	ARCHIVE_FILE_COMBAT_XMI		= 0xd5,
	ARCHIVE_FILE_PLAYM_US		= 0xd6,
	ARCHIVE_FILE_ZUSTA_US		= 0xd7,
	ARCHIVE_FILE_ANI_DAT		= 0xd8,
	ARCHIVE_FILE_WEAPANI_DAT	= 0xd9,
	ARCHIVE_FILE_MONSTER_DAT	= 0xda,
	ARCHIVE_FILE_BSKILLS_DAT	= 0xdb,
	ARCHIVE_FILE_ITEMS_DAT		= 0xdc,
	ARCHIVE_FILE_CHARTEXT_LTX	= 0xdd,
	ARCHIVE_FILE_SPELLTXT_LTX	= 0xde,
	ARCHIVE_FILE_WONDER_LTX		= 0xdf,
	ARCHIVE_FILE_STRASSE_LTX	= 0xe0,
	ARCHIVE_FILE_NSC_LTX		= 0xe1,
	ARCHIVE_FILE_NPCS		= 0xe2,
	/*
		NARIELL.NPC, HARIKA.NPC, CURIAN.NPC, ARDORA.NPC, GARSVIK.NPC, ERWO.NPC
	*/
	ARCHIVE_FILE_FINGER_NVF		= 0xe8,
	ARCHIVE_FILE_LTURM_NVF		= 0xe9,
	ARCHIVE_FILE_WFIGS		= 0xea,
	ARCHIVE_FILE_WFIGS_TAB		= 0xeb,
	ARCHIVE_FILE_SPLASHES_DAT	= 0xec,
	ARCHIVE_FILE_FEATURE9_LTX	= 0xed,
	ARCHIVE_FILE_PHEX1_LTX		= 0xee,
	ARCHIVE_FILE_PHEX2_LTX		= 0xef,
	ARCHIVE_FILE_WILD_LTX		= 0xf0,
	ARCHIVE_FILE_WAFFINFO_LTX	= 0xf1,
	ARCHIVE_FILE_DNGS		= 0xf2,
	/*
		SHIP.DNG, F046.DNG, F051.DNG, F061.DNG, F076.DNG, F094.DNG, F100.DNG, F108.DNG,
		F126.DNG, F129.DNG, F131.DNG, OBER.DNG, PREM.DNG, THORWAL.DNG, FINAL.DNG
	*/
	ARCHIVE_FILE_DNGS_DDT		= 0x100,
	/*
		SHIP.DDT, F046.DDT, F051.DDT, F061.DDT, F076.DDT, F094.DDT, F100.DDT, F108.DDT,
		F126.DDT, F129.DDT, F131.DDT, OBER.DDT, PREM.DDT, THORWAL.DDT, FINAL.DDT
	*/
	ARCHIVE_FILE_DNGS_DTX		= 0x10f,
	/*
		SHIP.DTX, F046.DTX, F051.DTX, F061.DTX, F076.DTX, F094.DTX, F100.DTX, F108.DTX,
		F126.DTX, F129.DTX, F131.DTX, OBER.DTX, PREM.DTX, THORWAL.DTX, FINAL.DTX
	*/
	ARCHIVE_FILE_TFLOOR2_NVF	= 0x11f,
	ARCHIVE_FILE_STORY_LTX		= 0x120,
	ARCHIVE_FILE_FX1_VOC		= 0x121,
	ARCHIVE_FILE_FX2_VOC		= 0x122,
	ARCHIVE_FILE_FX3_VOC		= 0x123,
	ARCHIVE_FILE_FX4_VOC		= 0x124,
	ARCHIVE_FILE_FX5_VOC		= 0x125,
	ARCHIVE_FILE_FX6_VOC		= 0x126,
	ARCHIVE_FILE_FX7_VOC		= 0x127,
	ARCHIVE_FILE_FX8_VOC		= 0x128,
	ARCHIVE_FILE_FX9_VOC		= 0x129,
	ARCHIVE_FILE_FX10_VOC		= 0x12a,
	ARCHIVE_FILE_FX11_VOC		= 0x12b,
	ARCHIVE_FILE_FX12_VOC		= 0x12c,
	ARCHIVE_FILE_FX13_VOC		= 0x12d,
	ARCHIVE_FILE_FX14_VOC		= 0x12e,
	ARCHIVE_FILE_FX15_VOC		= 0x12f,
	ARCHIVE_FILE_FX16_VOC		= 0x130,
	ARCHIVE_FILE_FX17_VOC		= 0x131,
	ARCHIVE_FILE_FX18_VOC		= 0x132,
	ARCHIVE_FILE_OUTRO_XMI		= 0x133
};

enum {
	ITEM_NONE			= 0x00, /* Bloße Hand */
	ITEM_SWORD			= 0x01, /* Schwert */
	ITEM_CLUB			= 0x02, /* Knüppel */
	ITEM_SABER			= 0x03, /* Säbel */
	ITEM_KNIFE			= 0x04, /* Messer */
	ITEM_SPEAR			= 0x05, /* Speer */
	ITEM_SHORT_SWORD		= 0x06, /* Kurzschwert */
	ITEM_SHIELD			= 0x07, /* Schild */
	ITEM_KRIEGSBEIL_SPECIAL		= 0x08, /* Kriegsbeil [special "Schicksalsklinge" variant, also for witches, druids and mages, 1D6 + 4, KK+14, BF 5, AT-0, PA-3, weight 120, price 45] */
	ITEM_SHORTBOW			= 0x09, /* Kurzbogen */
	ITEM_ARROWS			= 0x0a, /* Pfeil */
	ITEM_STREITAXT			= 0x0b, /* Streitaxt */
	ITEM_CROSSBOW			= 0x0c, /* Armbrust */
	ITEM_BOLTS			= 0x0d, /* Bolzen */
	ITEM_DAGGER			= 0x0e, /* Dolch */
	ITEM_IRON_SHIELD		= 0x0f, /* Eisenschild */
	ITEM_FRANCESCA			= 0x10, /* Wurfbeil */
	ITEM_THROWING_STAR		= 0x11, /* Wurfstern */
	ITEM_TWO_HANDED_SWORD		= 0x12, /* Zweihänder */
	ITEM_LONGBOW			= 0x13, /* Langbogen */
	ITEM_MORNING_STAR		= 0x14, /* Morgenstern */
	ITEM_VOLCANIC_GLASS_DAGGER	= 0x15, /* Vulkanglasdolch */
	ITEM_TORCH_ON			= 0x16, /* Fackel [on] */
	ITEM_BEER			= 0x17, /* Bier */
	ITEM_GRAPPLE			= 0x18, /* Wurfhaken */
	ITEM_LANTERN_OFF		= 0x19, /* Laterne [off] */
	ITEM_CROWBAR			= 0x1a, /* Brecheisen */
	ITEM_HAMMER			= 0x1b, /* Hammer */
	ITEM_FISHHOOK			= 0x1c, /* Angelhaken */
	ITEM_SCROLL			= 0x1d, /* Schriftrolle */
	ITEM_WATERSKIN			= 0x1e, /* Wasserschlauch */
	ITEM_FLASK_GLASS		= 0x1f, /* Glasflasche */
	ITEM_ROPE_LADDER		= 0x20, /* Strickleiter */
	ITEM_THROWING_AXE		= 0x21, /* Wurfaxt */
	ITEM_BRASS_MIRROR		= 0x22, /* Messingspiegel */
	ITEM_PICKLOCKS			= 0x23, /* Dietriche */
	ITEM_SCHREIBZEUG		= 0x24, /* Schreibzeug */
	ITEM_HARP			= 0x25, /* Harfe */
	ITEM_DRINKING_HORN		= 0x26, /* Trinkhorn */
	ITEM_SILVER_JEWELRY		= 0x27, /* Silberschmuck */
	ITEM_CLIMBING_HOOKS		= 0x28, /* Kletterhaken */
	ITEM_OIL			= 0x29, /* Öl */
	ITEM_FLASK_BRONZE		= 0x2a, /* Bronzeflasche */
	ITEM_IRON_HELMET		= 0x2b, /* Eisenhelm */
	ITEM_PIKE			= 0x2c, /* Pike */
	ITEM_FOOD_PACKAGE		= 0x2d, /* Proviantpaket */
	ITEM_FLUTE			= 0x2e, /* Flöte */
	ITEM_ALCHEMY_KIT		= 0x2f, /* Alchemieset */
	ITEM_SHIRT			= 0x30, /* Hemd */
	ITEM_TROUSERS			= 0x31, /* Hose */
	ITEM_SHOES			= 0x32, /* Schuhe */
	ITEM_BOOTS			= 0x33, /* Stiefel */
	ITEM_LACEUP_SHOE		= 0x34, /* Schneeshuhe */
	ITEM_LEATHER_ARMOR		= 0x35, /* Lederharnisch */
	ITEM_SCALE_ARMOR		= 0x36, /* Schuppenrüstung */
	ITEM_SHURIN_POISON		= 0x37, /* Shurinknollengift */
	ITEM_ARAX_POISON		= 0x38, /* Araxgift */
	ITEM_ANGST_POISON		= 0x39, /* Angstgift */
	ITEM_SLEEP_POISON		= 0x3a, /* Schlafgift */
	ITEM_GOLDLEIM			= 0x3b, /* Goldleim */
	ITEM_EINBEERE			= 0x3c, /* Einbeere */
	ITEM_WHIRLWEED			= 0x3d, /* Wirselkraut */
	ITEM_KROTENSCHEMEL		= 0x3e, /* Eitriger Krötenschemel */
	ITEM_GULMOND_LEAF		= 0x3f, /* Gulmond Blatt */
	ITEM_TARNELE			= 0x40, /* Tarnele */
	ITEM_TORCH_OFF			= 0x41, /* Fackel [off] */
	ITEM_MACE			= 0x42, /* Streitkolben */
	ITEM_DEGEN			= 0x43, /* Degen */
	ITEM_FLORET			= 0x44, /* Florett */
	ITEM_QUARTERSTAFF		= 0x45, /* Kampfstab */
	ITEM_CRYSTAL_BALL		= 0x46, /* Kristallkugel */
	ITEM_WHIP			= 0x47, /* Peitsche */
	ITEM_BLANKET			= 0x48, /* Decke */
	ITEM_SHOVEL			= 0x49, /* Schaufel */
	ITEM_GOLD_JEWELRY		= 0x4a, /* Goldschmuck */
	ITEM_ROBE_GREEN			= 0x4b, /* Robe [green, cheap] */
	ITEM_ROBE_RED			= 0x4c, /* Robe [red, expensive] */
	ITEM_POT_HELMET			= 0x4d, /* Topfhelm */
	ITEM_LEATHER_HELMET		= 0x4e, /* Lederhelm */
	ITEM_SURCOAT			= 0x4f, /* Waffenrock */
	ITEM_CHAIN_MAIL			= 0x50, /* Kettenhemd */
	ITEM_KROETENHAUT		= 0x51, /* Kroetenhaut */
	ITEM_PLATTENZEUG		= 0x52, /* Plattenzeug */
	ITEM_KETTENZEUG			= 0x53, /* Kettenzeug */
	ITEM_LEDERZEUG			= 0x54, /* Lederzeug */
	ITEM_TINDERBOX			= 0x55, /* Zunderkästchen */
	ITEM_GRINDSTONE			= 0x56, /* Schleifstein */
	ITEM_CUTLERY			= 0x57, /* Essbesteck */
	ITEM_DISHES			= 0x58, /* Essgeschirr */
	ITEM_LICORICE			= 0x59, /* Lakritze */
	ITEM_BONBONS			= 0x5a, /* Bonbons */
	ITEM_WINE			= 0x5b, /* Weinflasche */
	ITEM_BRANDY			= 0x5c, /* Schnapsflasche */
	ITEM_PICKAXE			= 0x5d, /* Hacke */
	ITEM_PRAIOS_AMULET		= 0x5e, /* Praios Amulett [magic; found in spider cave] */
	ITEM_LUTE			= 0x5f, /* Laute */
	ITEM_WINTER_COAT		= 0x60, /* Wintermantel */
	ITEM_NET			= 0x61, /* Netz */
	ITEM_THROWING_KNIFE		= 0x62, /* Wurfmesser */
	ITEM_SICKLE			= 0x63, /* Sichel */
	ITEM_SCYTHE			= 0x64, /* Sense */
	ITEM_WAR_HAMMER			= 0x65, /* Kriegshammer */
	ITEM_TRIDENT			= 0x66, /* Dreizack */
	ITEM_HALBERD			= 0x67, /* Hellebarde */
	ITEM_FLAIL			= 0x68, /* Dreschflegel */
	ITEM_ZWEILILIEN			= 0x69, /* Zweililien */
	ITEM_OCHSENHERDE		= 0x6a, /* Ochsenherde */
	ITEM_BASILISKENZUNGE		= 0x6b, /* Basiliskenzunge */
	ITEM_OGERFAENGER		= 0x6c, /* Ogerfänger */
	ITEM_MENGBILAR			= 0x6d, /* Mengbilar */
	ITEM_HEAVY_DAGGER		= 0x6e, /* schwerer Dolch */
	ITEM_RONDRAKAMM			= 0x6f, /* Rondrakamm */
	ITEM_CUTLASS			= 0x70, /* Entermesser */
	ITEM_BASTARD_SWORD		= 0x71, /* Bastardschwert */
	ITEM_TUZAK_KNIFE		= 0x72, /* Tuzakmesser */
	ITEM_RABENSCHNABEL		= 0x73, /* Rabenschnabel */
	ITEM_BRABAKBENGEL		= 0x74, /* Brabakbengel */
	ITEM_RAPIER			= 0x75, /* Rapier */
	ITEM_KHUNCHOMER			= 0x76, /* Khunchomer */
	ITEM_DOPPELKHUNCHOMER		= 0x77, /* Doppelkhunchomer */
	ITEM_SLING			= 0x78, /* Schleuder */
	ITEM_ROPE			= 0x79, /* Seil */
	ITEM_SHURIN			= 0x7a, /* Shurinknolle */
	ITEM_BELMART			= 0x7b, /* Belmart Blatt */
	ITEM_DONF_SPRING		= 0x7c, /* Donfstengel */
	ITEM_MENCHAL			= 0x7d, /* Menchalkaktus */
	ITEM_ALRAUNE			= 0x7e, /* Alraune */
	ITEM_ATMONBLUETE		= 0x7f, /* Atmonblüte */
	ITEM_ILMENBLATT			= 0x80, /* Ilmenblatt */
	ITEM_FINAGE_TREE		= 0x81, /* Finagebäumchen */
	ITEM_JORUGA_ROOT		= 0x82, /* Jorugawurzel */
	ITEM_THONNYS			= 0x83, /* Thonnysblüte */
	ITEM_LOTUS			= 0x84, /* Lotusblüte */
	ITEM_MAGIC_WAND			= 0x85, /* Zauberstab */
	ITEM_SKRAJA			= 0x86, /* Skraja */
	ITEM_KRIEGSBEIL			= 0x87, /* Kriegsbeil [the common one, 1D6 + 4, KK+13, BF 2, AT-2, PA-4] */
	ITEM_ORKNASE			= 0x88, /* Orknase */
	ITEM_SCHNEIDZAHN		= 0x89, /* Schneidzahn */
	ITEM_ROBBENTOETER		= 0x8a, /* Robbentöter */
	ITEM_WOLFSMESSER		= 0x8b, /* Wolfsmesser */
	ITEM_WITCHES_BROOM		= 0x8c, /* Hexenbesen */
	ITEM_LOTUS_POISON		= 0x8d, /* Lotusgift */
	ITEM_KUKRIS			= 0x8e, /* Kukris */
	ITEM_BANNSTAUB			= 0x8f, /* Bannstaub */
	ITEM_KROETEN_POISON		= 0x90, /* Krötenschemelgift */
	ITEM_LE_POTION			= 0x91, /* Heiltrank */
	ITEM_STRONG_LE_POTION		= 0x92, /* starker Heiltrank */
	ITEM_MU_ELIXIR			= 0x93, /* MU Elixier */
	ITEM_KL_ELIXIR			= 0x94, /* KL Elixier */
	ITEM_CH_ELIXIR			= 0x95, /* CH Elixier */
	ITEM_FF_ELIXIR			= 0x96, /* FF Elixier */
	ITEM_GE_ELIXIR			= 0x97, /* GE Elixier */
	ITEM_IN_ELIXIR			= 0x98, /* IN Elixier */
	ITEM_KK_ELIXIR			= 0x99, /* KK Elixier */
	ITEM_AE_POTION			= 0x9a, /* Zaubertrank */
	ITEM_STRONG_AE_POTION		= 0x9b, /* Zaubertrank (stark) */
	ITEM_OLGIN_ROOT			= 0x9c, /* Olginwurzel */
	ITEM_KAIRANHALM			= 0x9d, /* Kairanhalm */
	ITEM_BASTARD_SWORD_MAGIC	= 0x9e, /* Bastardschwert [magic: lower BF; found in Daspota] */
	ITEM_ORKNASE_MAGIC		= 0x9f, /* Orknase [magic: damage +5, AT-1, PA-5; found in Daspota] */
	ITEM_SHORT_SWORD_MAGIC		= 0xa0, /* Kurzschwert [magic: AT + 2, unbreakable; found in Daspota ] */
	ITEM_SICKLE_MAGIC		= 0xa1, /* Sichel [magic, in inventory: Pflanzenkunde + 3; found in Daspota] */
	ITEM_AMULET_UNKNOWN_1		= 0xa2, /* Amulett [TODO: which one?? it might be yellow, magic: protects from fire; found at Totenschiff] */
	ITEM_AMULET_BLUE		= 0xa3, /* Amulett [blue, magic: MR + 5; found in ruin of the black wizard and at a fight vs. orcs at the track Phexcaer <-> Einsiedlersee] */
	ITEM_BONE_WITH_RUNE		= 0xa4, /* Knochen mit Rune */
	ITEM_RING_RED			= 0xa5, /* Ring [red, magic, MR + 2, additionally 5 charges of Arcano Psychostabilis (additionally MR + 2) when used; found at Gorah ] */
	ITEM_EXPURGICUM			= 0xa6, /* Expurgicum */
	ITEM_RECIPE_EXPURGICUM		= 0xa7, /* Rezept für Expurgicum */
	ITEM_VOMICUM			= 0xa8, /* Vomicum */
	ITEM_RECIPE_VOMICUM		= 0xa9, /* Rezept für Vomicum */
	ITEM_DOCUMENT_1			= 0xaa, /* Dokument [1, Depotschein?] */
	ITEM_CORONET_SILVER		= 0xab, /* Silberner Stirnreif [magic, use -> Armatrutz +5, i.e. RS + 5] */
	ITEM_SABER_MAGIC		= 0xac, /* Säbel [magic, damage+1 to skeletons and zombies] */
	ITEM_AMULET_RED			= 0xad, /* Amulett [red, magic, protection from fire; found at corpse between Rovamund and Nordvest ] */
	ITEM_AMULET_GREEN		= 0xae, /* Amulett [green, magic: use -> Flim Flam; found in Totenschiff] */
	ITEM_TRAVIA_AMULET		= 0xaf, /* Amulett [golden cross, magic: no hunger or thirst; found in wolf cave ] */
	ITEM_SCHULDBUCH			= 0xb0, /* Schuldbuch */
	ITEM_RED_MOON_DISK		= 0xb1, /* rote Mondscheibe [found in orc cave] */
	ITEM_TWO_HANDED_SWORD_MAGIC	= 0xb2, /* Zweihänder [magic: unbreakable; found in orc cave] */
	ITEM_ORC_DOCUMENT		= 0xb3, /* Orkdokument */
	ITEM_ANTIVENOM			= 0xb4, /* Gegengift */
	ITEM_GRIMRING			= 0xb5, /* Grimring [found at Hyggelik's ruin] */
	ITEM_SKULL_BELT			= 0xb6, /* Totenkopfgürtel [magic: TA - 4; found in Verfallene Herberge ] */
	ITEM_GIRDLE_MIGHT		= 0xb7, /* Kraftgürtel [magic: KK + 5; found in dragon cave] */
	ITEM_MAGIC_BREADBAG		= 0xb8, /* magischer Brotbeutel */
	ITEM_MAGIC_WATERSKIN		= 0xb9, /* magischer Wasserschlauch */
	ITEM_RECIPE_LE_POTION		= 0xba, /* Rezept für Heiltrank */
	ITEM_WRITING_OF_JARDA		= 0xbb, /* Schreiben von Jarda */
	ITEM_AMULET_UNKNOWN_2		= 0xbc, /* Amulett [TODO: which one?? it might be yellow, magic: protects from fire; found at Totenschiff] */
	ITEM_LOBPREISUNGEN		= 0xbd, /* Lobpreisungen */
	ITEM_MITGLIEDERLISTE		= 0xbe, /* Mitgliederliste */
	ITEM_DOCUMENT_2			= 0xbf, /* Dokument [2] */
	ITEM_SEA_CHART			= 0xc0, /* Seekarte */
	ITEM_KUKRIS_DAGGER		= 0xc1, /* Kukrisdolch */
	ITEM_KEY_1			= 0xc2, /* Schlüssel [1] */ /* TODO: Farbe? umbenennen! */
	ITEM_KEY_GOLDEN_1		= 0xc3, /* Goldener Schlüssel [found in the dungeon of the Daspota treasure] */
	ITEM_HELMET_CURSED		= 0xc4, /* Helm [cursed: CH - 1; found in goblin cave and at NPC Ardora von Greifenfurt ] */
	ITEM_CHAIN_MAIL_CURSED		= 0xc5, /* Kettenhemd [magic, cursed, protection +1 vs. skeletons and zombies, but protection -2 vs. everything else, additionally drains LE; found in dragon cave] */
	ITEM_CHAIN_MAIL_MAGIC		= 0xc6, /* Kettenhemd [magic, AT+1, BE-1, RS+1; found in the dungeon of the Daspota treasure (Rybon-Thoss)] */
	ITEM_SWORD_MAGIC		= 0xc7, /* Schwert [magic: AT + 2; found in dragon cave] */
	ITEM_KUKRIS_MENGBILAR		= 0xc8, /* Kukrismengbilar */
	ITEM_KEY_PLATIN			= 0xc9, /* Platinschlüssel */
	ITEM_RECIPE_ANTIVENOM		= 0xca, /* Rezept für Gegengift */
	ITEM_RECIPE_HYLAILIC_FIRE	= 0xcb, /* Rezept für Hylailer Feuer */
	ITEM_RECIPE_KK_ELIXIR		= 0xcc, /* Rezept für Kraftelixier */
	ITEM_RECIPE_MU_ELIXIR		= 0xcd, /* Repezt für Mutelixier */
	ITEM_RECIPE_AE_POTION		= 0xce, /* Rezept für Zaubertrank */
	ITEM_RING_BLUE			= 0xcf, /* Ring [blue, not magic, found in Zwingfeste] */
	ITEM_KEY_2			= 0xd0, /* Schlüssel [2] */ /* TODO: Farbe? */
	ITEM_KEY_IRON_1			= 0xd1, /* Eisenschlüssel [1] */ /* TODO: Farbe? */
	ITEM_KEY_IRON_2			= 0xd2, /* Eisenschlüssel [2] */ /* TODO: Farbe? */
	ITEM_KEY_RED			= 0xd3, /* Roter Schlüssel */
	ITEM_KEY_BRONZE			= 0xd4, /* Bronzeschlüssel */
	ITEM_SILVER_HELMET		= 0xd5, /* Silberhelm [magic: can be worn by all hero classes; found in wolf cave] */
	ITEM_SILVER_MACE		= 0xd6, /* Silberstreitkolben [magic; found in Verfallene Herberge] */
	ITEM_SILVER_JEWELRY_MAGIC	= 0xd7, /* Silberschmuck [magic: TA - 2; found in Verfallene Herberge] */
	ITEM_SPEAR_MAGIC		= 0xd8, /* Speer [magic; found in spider cave] */
	ITEM_CORONET_BLUE		= 0xd9, /* Stirnreif [blau, magic: MR + 2; found in Tempel des Namenlosen] */
	ITEM_THROWING_DAGGER_MAGIC	= 0xda, /* Wurfdolch [magic; found in dragon cave]. note that there is no 'ordinary' throwing dagger in the game */
	ITEM_KEY_GOLDEN_2		= 0xdb, /* Goldschlüssel */
	ITEM_RING_GREEN			= 0xdc, /* Ring [green, magic: protection from magic fire; found in dragon cave] */
	ITEM_BAG			= 0xdd, /* Beutel [found and used in the first level of the ruin of the black wizard to open a passage] */
	ITEM_BOOK_1			= 0xde, /* Buch [1] */
	ITEM_ANTI_SICKNESS_ELIXIR	= 0xdf, /* Antikrankheitselixier */
	ITEM_BOOK_2			= 0xe0, /* Buch [2; found in the dungeon of the Daspota treasure (Rybon-Thoss)] */
	ITEM_CRYSTAL			= 0xe1, /* Kristall */
	ITEM_MU_ELIXIR_BAD		= 0xe2, /* MU Elixier [bad] */
	ITEM_KL_ELIXIR_BAD		= 0xe3, /* KL Elixier [bad] */
	ITEM_CH_ELIXIR_BAD		= 0xe4, /* CH Elixier [bad] */
	ITEM_FF_ELIXIR_BAD		= 0xe5, /* FF Elixier [bad] */
	ITEM_GE_ELIXIR_BAD		= 0xe6, /* GE Elixier [bad] */
	ITEM_IN_ELIXIR_BAD		= 0xe7, /* IN Elixier [bad] */
	ITEM_KK_ELIXIR_BAD		= 0xe8, /* KK Elixier [bad] */
	ITEM_ROBE_GREEN_2		= 0xe9, /* Robe [green] */
	ITEM_GOLDEN_SHIELD		= 0xea, /* goldener Schild [magic: can be equiped by all hero classes; found in Tempel des Namenlosen */
	ITEM_WRITING_OF_HETMAN		= 0xeb, /* Empfehlungsschreiben [Hetman] */
	ITEM_WUNDERKUR			= 0xec, /* Wunderkur */
	ITEM_SLEEPING_POTION		= 0xed, /* Schlaftrunk */
	ITEM_MIASTHMATICUM		= 0xee, /* Miasthmaticum */
	ITEM_HYLAILIC_FIRE		= 0xef, /* Hylailer Feuer */
	ITEM_RECIPE_STRONG_LE_POTION	= 0xf0, /* Rezept für starken Heiltrank [given by Olimone, tevent_114 Phexcaer <-> Skelellen: a swamp]*/
	ITEM_RECIPE_WUNDERKUR		= 0xf1, /* Rezept für Wunderkur */
	ITEM_RECIPE_SLEEPING_POTION	= 0xf2, /* Rezept für Schlaftrunk */
	ITEM_RECIPE_STRONG_AE_POTION	= 0xf3, /* Rezept für starker Zaubertrank */
	ITEM_RECIPE_MIASTHMATICUM	= 0xf4, /* Rezept für Miasthmaticum */
	ITEM_CORONET_GREEN		= 0xf5, /* Stirnreif [green, magic: use -> Armatrutz +5, i.e. RS + 5; found 2x in spider cave] */
	ITEM_BOOK_HEPTAGON		= 0xf6, /* Buch [read -> 'Heptagon' spell +3; found in spider cave] */
	ITEM_WRITING_OF_SIEBENSTEIN	= 0xf7, /* Empfehlungsschreiben [Umbrik Siebenstein] */
	ITEM_BLACK_FIGURINE		= 0xf8, /* schwarze Statuette, found in Tempel des Namenlosen and used as evidence to Eliane Windenbeck */
	ITEM_LANTERN_ON			= 0xf9, /* Laterne [on] */
	ITEM_DUCATS			= 0xfa, /* Dukaten */
	ITEM_200_ARROWS			= 0xfb, /* 200 Pfeile */
	ITEM_50_BOLTS			= 0xfc, /* 50 Bolzen */
	ITEM_20_CLIMBING_HOOKS		= 0xfd  /* 20 Kletterhaken */
};

enum {
	LIGHTING_DARK		= 0,
	LIGHTING_TORCH		= 1,
	LIGHTING_LANTERN	= 2
};

enum {
	RECIPE_ITEM_ID		= 0, /* two bytes */ /* item id of the recipe */
	RECIPE_INGREDIENTS	= 2, /* two bytes [10] */ /* list of ingredients, two bytes each (item ids), terminated by -1 entry */
	RECIPE_OUTCOME		= 22, /* two bytes */ /* item id of the outcome of the recipe */
	RECIPE_AE_COST		= 24, /* AE needed */
	RECIPE_HANDICAP		= 26, /* handicap for alchemy skill test */
	RECIPE_DURATION		= 27, /* time needed to brew the recipe, in hours */
	SIZEOF_RECIPE		= 28
};

#define NR_SEA_ROUTES (45)

enum {
	/* rationale: write SEA_ROUTE_PASSAGE... if the value depends on the current passage on the route (and will change with time).
	 * the other values are constants. */
	SEA_ROUTE_TOWN_1 = 0, /* one byte readonly */ /* ID of the first town of the connection. Note that the routes are undirected; i.e. both endpoint towns are treated equal. */
	SEA_ROUTE_TOWN_2 = 1, /* one byte readonly */ /* ID of the second town of the connection */
	SEA_ROUTE_DISTANCE = 2, /* one byte readonly */
	SEA_ROUTE_FREQUENCY = 3, /* one byte readonly */ /* the higher this value, the rarer a passage on the route is offered */
	SEA_ROUTE_PASSAGE_TIMER = 4, /* one byte rw */ /* after how many days will a ship do a passage on this route? */
	SEA_ROUTE_COSTAL_ROUTE = 5, /* one byte readonly */ /* 0 = high seas route (= Hochseeroute), 1 = costal route */
	SEA_ROUTE_PASSAGE_SHIP_TYPE = 6, /* one byte rw */ /* passage type of the next passage on this route */
	SEA_ROUTE_PASSAGE_PRICE_MOD = 7, /* one byte rw */ /* a number between 70 and 130 which serves as a percentage modifier to the price of the next ship on this route */
	SIZEOF_SEA_ROUTE = 8
};

enum {
	/* strictly speaking, these values encode not only the ship type, but the combined information
	 * route type (high seas vs. costal); ship type (Langschiff, etc.); passage type (Kabinenpassage, etc.)
	 * As there are at most two possible combinations per type of ship, we still call it SHIP_TYPE for simplicity. */

	/* ships on high seas routes */
	SHIP_TYPE_LANGSCHIFF_HIGH_SEAS  = 0,
	SHIP_TYPE_KARRACKE  = 1,
	SHIP_TYPE_SCHNELLSEGLER  = 2,
	SHIP_TYPE_SCHNELLSEGLER_LUXURY = 3,
	/* ships on costal routes */
	SHIP_TYPE_LANGSCHIFF_COSTAL  = 4,
	SHIP_TYPE_KUESTENSEGLER  = 5,
	SHIP_TYPE_KUTTER  = 6,
	SHIP_TYPE_FISCHERBOOT = 7
};

enum {
	SHIP_TABLE_PASSAGE_TYPE = 0, /* one byte */ /*
	       0 = Heuer (Regen. -2);
	       1 = Begleitschutzfahrt (Regen. 0);
	       2 = Luxuspassage (Regen. +5);
	       3 = komfortable Passage (Regen. +4);
	       4 = Kabinenpassage (Regen. +3);
	       5 = Deckpassage (Regen. +1);
	       6 = Mitfahrgelegenheit (Regen. 0)
	*/
	SHIP_TABLE_UNKN = 1, /* one byte */ /* is this byte ever read? */
	SHIP_TABLE_BASE_PRICE_PER_DISTANCE = 2, /* one byte. Unit: [Heller per 10 km] */
	SHIP_TABLE_BASE_SPEED = 3, /* one byte. Unit: [km per day] */
	SIZEOF_SHIP_TABLE_ENTRY = 4
};

enum {
	HARBOR_OPTION_SHIP_NAME_PTR = 0, /* four bytes */ /* pointer to the ship name */
	HARBOR_OPTION_ROUTE_PTR = 4, /* four bytes */ /* pointer to the route */
	HARBOR_OPTION_SHIP_TIMER = 8, /* one byte. 0 = ship leaves today; 1 = ship leaves tomorrow */
	HARBOR_OPTION_SHIP_TYPE = 9, /* one byte */
	HARBOR_OPTION_DESTINATION = 10, /* one byte */ /* ID of the destination town */
	HARBOR_OPTION_ROUTE_ID = 11, /* one byte */ /* ID of the passage */
	SIZEOF_HARBOR_OPTION = 12
};

enum {
	GAME_MODE_UNSPECIFIED	= -1,
	GAME_MODE_BEGINNER	= 1,
	GAME_MODE_ADVANCED	= 2
};

enum {
	STAFFSPELL_DESCRIPTIONS_ATTRIB1 = 0,
	STAFFSPELL_DESCRIPTIONS_ATTRIB2 = 1,
	STAFFSPELL_DESCRIPTIONS_ATTRIB3 = 2,
	STAFFSPELL_DESCRIPTIONS_HANDICAP = 3,
	STAFFSPELL_DESCRIPTIONS_AE_COST = 4,
	STAFFSPELL_DESCRIPTIONS_AE_MOD = 5,
	SIZEOF_STAFFSPELL_DESCRIPTIONS = 6
};

enum { /* descriptions of the 85 hero spells */
	SPELL_DESCRIPTIONS_HEROTYPE = 0, /* 0: druid spell; 1: mage spell; 2: which spell; 3: green elf spell; 4: ice elf spell; 5: sylvan elf spell */
	SPELL_DESCRIPTIONS_ATTRIB1 = 1,
	SPELL_DESCRIPTIONS_ATTRIB2 = 2,
	SPELL_DESCRIPTIONS_ATTRIB3 = 3,
	SPELL_DESCRIPTIONS_AE_COST = 4,
	SPELL_DESCRIPTIONS_WHERE_TO_USE = 5, /* -1: spell cannot be casted in fight / 0: spell can be casted in fight and normal mode / 1: spell can be casted only in fight mode */
	SPELL_DESCRIPTIONS_UNKN6 = 6,
	SPELL_DESCRIPTIONS_TARGET_TYPE = 7,
	SPELL_DESCRIPTIONS_RANGE = 8,
	SPELL_DESCRIPTIONS_FIGHT = 9,
	SIZEOF_SPELL_DESCRIPTIONS = 10
};

enum { /* note that the order differs from the one in HERO_TYPE... :( */
	SPELL_DESC_HEROTYPE_DRUID = 0,
	SPELL_DESC_HEROTYPE_MAGE = 1,
	SPELL_DESC_HEROTYPE_WITCH = 2,
	SPELL_DESC_HEROTYPE_GELF = 3,
	SPELL_DESC_HEROTYPE_IELF = 4,
	SPELL_DESC_HEROTYPE_SELF = 5
};

enum {
	MON_SPELL_DESCRIPTIONS_AE_COST	= 0,
	MON_SPELL_DESCRIPTIONS_MODE	= 1,
	MON_SPELL_DESCRIPTIONS_UNKN1	= 2,
	MON_SPELL_DESCRIPTIONS_ATTRIB1	= 3,
	MON_SPELL_DESCRIPTIONS_ATTRIB2	= 4,
	MON_SPELL_DESCRIPTIONS_ATTRIB3	= 5,
	MON_SPELL_DESCRIPTIONS_VS_MR	= 6,
	MON_SPELL_DESCRIPTIONS_ANI_ID	= 7,
	SIZEOF_MON_SPELL_DESCRIPTIONS	= 8 /* size of entry at MON_SPELL_DESCRIPTIONS in bytes */
};

enum {
	MENU_ICON_NONE			= -1,
	MENU_ICON_HIRE_HERO		= 0,
	MENU_ICON_DISMISS_HERO		= 1,
	MENU_ICON_DELETE_HERO		= 2,
	MENU_ICON_LOAD_GAME		= 3,
	MENU_ICON_SAVE_GAME		= 4,
	MENU_ICON_QUIT_GAME		= 5,
	MENU_ICON_PRAY			= 6,
	MENU_ICON_SACRIFICE		= 7,
	MENU_ICON_LEAVE			= 8,
	MENU_ICON_GUARDS		= 9,
	MENU_ICON_GIVE_GOLD		= 10, /* unused? */
	MENU_ICON_MAGIC			= 11,
	MENU_ICON_SPLIT_GROUP		= 12,
	MENU_ICON_ORDER_FOOD		= 13,
	MENU_ICON_TAKE_GOLD		= 14, /* unused? */
	MENU_ICON_MERGE_GROUP		= 15,
	MENU_ICON_REPLENISH_SUPPLIES	= 16,
	MENU_ICON_SLEEP			= 17,
	MENU_ICON_REPAIR		= 18,
	MENU_ICON_BOOK_BED		= 19,
	MENU_ICON_COLLECT_HERBS		= 20,
	MENU_ICON_TALK			= 21,
	MENU_ICON_BUY			= 22,
	MENU_ICON_BARGAIN		= 23,
	MENU_ICON_SELL			= 24,
	MENU_ICON_APPLY_SKILL		= 25,
	MENU_ICON_SCROLL_RIGHT		= 26,
	MENU_ICON_SCROLL_LEFT		= 27,
	MENU_ICON_HERO			= 28,
	MENU_ICON_SWITCH_GROUP		= 29,
	MENU_ICON_HEAL_WOUNDS		= 30,
	MENU_ICON_HEAL_DISEASE		= 31,
	MENU_ICON_HEAL_POISON		= 32,
	MENU_ICON_OPEN_CLOSE_DOOR	= 33,
	MENU_ICON_SMASH_DOOR		= 34,
	MENU_ICON_PICK_LOCK		= 35,
	MENU_ICON_WIZARD_FLOOR		= 36, /* unused? */
	MENU_ICON_INFO			= 37,
	MENU_ICON_OPEN_CHEST		= 38,
	MENU_ICON_MAP			= 39,
	MENU_ICON_BOOK_SHIP_PASSAGE	= 40,
	MENU_ICON_BOARD_SHIP		= 41,
	MENU_ICON_HARBOR_MASTER		= 42,
	MENU_ICON_MARKET		= 43,
	MENU_ICON_HYGELLIK_MAP		= 44,
	MENU_ICON_MERGE_GROUP_GRAYED	= 45,
	MENU_ICON_MOVE_LEVER		= 46,
	MENU_ICON_ATTACK		= 47, /* unused? */
	MENU_ICON_QUIT_CAMP		= 48,
	MENU_ICON_INN			= 49,
	MENU_ICON_TAVERN		= 50,
	MENU_ICON_BATTLE_FRAME_RATE	= 51,
	MENU_ICON_DIARY			= 52,
	MENU_ICON_SOUND			= 53,
	MENU_ICON_CAMP			= 54
};
#endif
