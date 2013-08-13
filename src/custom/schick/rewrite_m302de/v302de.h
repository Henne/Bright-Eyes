/*
 *	transitional functions for version 3.02_de
 *
 *	Here are function, which should make the transition from
 *	DOXBox emulation to native easier.
 *	These functions will only work with the version mentioned above.
 */

#if !defined V302DE_H
#define V302DE_H

#if !defined(__BORLANDC__)
#include "schick.h"
#endif

#define UNICORN_MET	(0x2464)
#define UNICORN_GET_MAP	(0x2465)
#define UNICORN_TIMER	(0x2466)
#define TWO_FIELDED_SPRITE_ID (0x25f9)	/* char[5] array */
#define EMS_ENABLED	(0x26ab)
#define TIMERS_DISABLED	(0x2c99)
#define CURRENT_ANI	(0x2ccf)
#define IN_FIGHT	(0x2cd5)
#define CURRENT_GROUP	(0x2d35)
#define DIRECTION	(0x2d3d)
#define X_TARGET	(0x2d44)
#define Y_TARGET	(0x2d46)
#define LOCATION	(0x2d60)
#define CURRENT_TOWN	(0x2d67)
#define DUNGEON_INDEX	(0x2d6e)
#define DUNGEON_LEVEL	(0x2d75)
#define DAY_TIMER	(0x2dbb)
#define DAY_OF_WEEK	(0x2dbf)
#define DAY_OF_MONTH	(0x2dc0)
#define MONTH		(0x2dc1)
#define YEAR		(0x2dc2)
#define SPECIAL_DAY	(0x2dc3)
#define TREASURE_MAPS	(0x3332)	/* array char[9] */
#define GOT_MAIN_QUEST	(0x334c)
#define QUESTED_MONTHS	(0x334e)
#define NPC_MONTHS	(0x3470)
#define CURRENT_INFORMER	(0x3612)
#define TRAVELING	(0x3614)
#define SPHERE_TIMER	(0x3c97)
#define SPHERE_ACTIVE	(0x3c98)
#define MAGE_POISON	(0x3cb6)
#define FOOD_MESSAGE	(0x4219)
#define TYPEINDEX	(0x4224)
#define CHECK_PARTY	(0x4495)
#define FOOD_MOD	(0x4496)
#define CONSUME_QUIET	(0x4648)
#define DELAY_FACTOR	(0x4b66)
#define FNAMES		(0x4c8c)
#define AUTOFIGHT_MAGIC (0x5f31)	/* ?8 */
#define AF_SPELL_LIST	(0x5fac)	/* char[11] */
#define LIGHT_TYPE	(0xaee8)	/* ?16 0 = none, 1 = torch, 2 = lantern */
#define SPLASH_AE	(0xbcc7)
#define SPLASH_LE	(0xbccb)
#define SCENARIO_BUF	(0xbd2c)
#define PTR_FIGHT_LST	(0xbd28)	/* pointer to FIGHT.LST */
#define HEROS		(0xbd34)
#define DIALOG_TEXT	(0xc3b1)
#define CITY_LTX	(0xc3ad)
#define TEXT_LTX	(0xc3b5)
#define ANI_MAIN_PTR	(0xce35)
#define TEXTCOLOR_BG	(0xd2c7)
#define TEXTCOLOR_FG	(0xd2c9)
#define DTP2		(0xd2f3)
#define ENEMY_SHEETS	(0xd34b)	/* struct enemy[20] */
#define CHESSBOARD	(0xd852)
#define NR_OF_ENEMIES	(0xd872)	/* ? short */
#define WALLCLOCK_Y	(0xe10f)
#define WALLCLOCK_X	(0xe111)
#define MEM_SLOTS_MON	(0xe115)
#define MEM_SLOTS_WFIG	(0xe119)
#define MEM_SLOTS_MFIG	(0xe11d)
#define MONNAMES	(0xe129)
#define ITEMSDAT	(0xe22b)
#define ITEMSNAME	(0xe22f)
#define DELAY_TIMER	(0xe2d0)
#define CURRENT_FIG_NR	(0xe316)
#define AUTOFIGHT	(0xe318)	/* ?16 */
#define BUFFER_WEAPANIDAT	(0xe374)	/* pointer to WEAPANI.DAT */
#define BUFFER_ANIDAT	(0xe378)	/* pointer to ANI.DAT buffer */
#define GRAMMAR_STRING	(0xe4e3)	/* char grammar_string[201] */
#define SPELLTARGET	(0xe5b8)
#define SPELLUSER	(0xe5bc)

#if !defined(__BORLANDC__)

static inline Bit8s host_readbs(Bit8u* p)
{
	return (Bit8s)host_readb(p);
}

static inline Bit16s host_readws(Bit8u* p)
{
	return (Bit16s)host_readw(p);
}

static inline Bit32s host_readds(Bit8u* p)
{
	return (Bit32s)host_readd(p);
}

static inline int NOT_NULL(Bit8u* p)
{
	return (p != NULL && p != MemBase);
}

/**
 *	ds_writeb_z() -	write only if target is 0
 *	@addr:	address in datasegment
 *	@val:	value which should be written
 *
 *	A often occuring Original-Bug resets some informants
 *	to older states. This helper writes only that value
 *	if the informer is unknown (0).
 */
static inline void ds_writeb_z(Bitu addr, char val) {
	if (ds_readb(addr) == 0)
		ds_writeb(addr, val);
}

static inline Bit8u *get_hero(unsigned short index) {
	return Real2Host(ds_readd(HEROS)) + index * 0x6da;
}

/**
 * hero_dead() -	check if hero is dead
 * @hero:	ptr to hero
 *
 * 0 = alive / 1 = dead
 */
static inline unsigned short hero_dead(Bit8u *hero) {
	if ((host_readb(hero + 0xaa) & 1))
		return 1;
	else
		return 0;
}

/**
 * hero_sleeps() -	check if hero is sleeping
 * @hero:	ptr to hero
 *
 * 0 = awake / 1 = sleeps
 */
static inline unsigned short hero_sleeps(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 1) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * hero_stoned() -	check if hero is stoned
 * @hero:	ptr to hero
 *
 * 0 = non-stoned / 1 = stoned
 */
static inline unsigned short hero_stoned(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 2) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * hero_unc() -	check if hero is unconscious
 * @hero:	ptr to hero
 *
 * 0 = awake / 1 = unconscious
 */
static inline unsigned short hero_unc(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 6) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline Bit8u *get_spelluser() {
	return Real2Host(ds_readd(SPELLUSER));
}

static inline Bit8u *get_spelltarget() {
	return Real2Host(ds_readd(SPELLTARGET));
}

static inline Bit8u *get_fname(unsigned short off) {
	return Real2Host(ds_readd(FNAMES + off * 4));
}

static inline Bit8u *get_monname(unsigned short off)
{
	return Real2Host(host_readd(Real2Host(ds_readd(MONNAMES) + off * 4)));
}

static inline Bit8u *get_city(unsigned short off) {
	return Real2Host(host_readd(Real2Host(ds_readd(CITY_LTX) + off)));
}

static inline Bit8u *get_ltx(unsigned short off) {
	return Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX) + off)));
}

static inline Bit8u *get_dtp(unsigned short off) {
	return Real2Host(host_readd(Real2Host(ds_readd(DIALOG_TEXT) + off)));
}

static inline signed char get_cb_val(unsigned short x, unsigned short y) {
	return host_readb(Real2Host(ds_readd(CHESSBOARD)) + y * 25 + x);
}

static inline void set_cb_val(unsigned short x, unsigned short y, signed char val) {
	host_writeb(Real2Host(ds_readd(CHESSBOARD)) + y * 25 + x, val);
}

static inline void dump_cb(void)
{
	FILE *fd;
	int y, x;
	signed char val;

	fd = fopen("cb_dump.txt", "w");

	if (fd == NULL)
		return;
	fprintf(fd, "\n");
	for (y = 24; y >= 0; y--) {
		for (x = 0; x < 25; x++) {
			val = get_cb_val(x, y);
			if (val != 0)
				fprintf(fd, "%3d ", val);
			else
				fprintf(fd, "    ");
		}
		fprintf(fd, "\n");
	}
	fclose(fd);
}

static inline Bit8u *get_itemsdat(unsigned short item) {
	return Real2Host(ds_readd(ITEMSDAT)) + item * 12;
}

static inline char* get_itemname(unsigned short item)
{
	return (char*)Real2Host(host_readd(Real2Host(ds_readd(ITEMSNAME)) + item * 4));
}

#else

#include <DOS.H>

#undef M302de_ORIGINAL_BUGFIX

typedef unsigned char Bit8u;
typedef signed char Bit8s;
typedef unsigned short Bit16u;
typedef signed short Bit16s;
typedef unsigned long Bit32u;
typedef signed long Bit32s;

typedef Bit8u* RealPt;
typedef Bit8u* PhysPt;

#define NOT_NULL(p) (p != NULL)

/* poring functions for Borland C++ */
struct hero_struct {
	char name[16];
	char alias[16];
	char dummy;
	char typus;
};

extern char ds[0xffff];
#define p_datseg (&ds[0x0000])
#define datseg (FP_SEG(&ds[0x0000]))

#define ds_readb(p) (*(Bit8u*)(ds + p))
#define ds_readw(p) (*(Bit16u*)(ds + p))
#define ds_readd(p) (*(Bit32u*)(ds + p))

#define ds_readbs(p) *(Bit8s*)(ds + p)
#define ds_readws(p) *(Bit16s*)(ds + p)
#define ds_readds(p) *(Bit32s*)(ds + p)

#define ds_writeb(p, d) *(Bit8u*)(ds + p) = d
#define ds_writew(p, d) *(Bit16u*)(ds + p) = d
#define ds_writed(p, d) *(Bit32u*)(ds + p) = d

#define mem_readb(p) *(Bit8u*)(p)
#define mem_readw(p) *(Bit16u*)(p)
#define mem_readd(p) *(Bit32u*)(p)

#define mem_readbs(p) *(Bit8s*)(p)
#define mem_readws(p) *(Bit16s*)(p)
#define mem_readds(p) *(Bit32s*)(p)

#define mem_writeb(p, d) *(Bit8u*)(p) = d
#define mem_writew(p, d) *(Bit16u*)(p) = d
#define mem_writed(p, d) *(Bit32u*)(p) = d

#define get_hero(nr) ((Bit8u*)ds_readd(HEROS) + 0x6da * (nr))


extern Bit8u* text_ltx[];
extern Bit8u* dialog_text[];
extern Bit8u* city_ltx[];

#define RealMake(s, o) (RealPt)((s << 16) | o)

#define Real2Phys(p) (Bit8u*)(p)
#define Real2Host(p) (Bit8u*)(p)
#define getString(p) (char*)(p)

#define host_readb(p) *(Bit8u*)(p)
#define host_readw(p) *(Bit16u*)(p)
#define host_readd(p) *(Bit32u*)(p)

#define host_readbs(p) *(Bit8s*)(p)
#define host_readws(p) *(Bit16s*)(p)
#define host_readds(p) *(Bit32s*)(p)

#define host_writeb(p, d)	(*(Bit8u*)(p) = d)
#define host_writew(p, d)	(*(Bit16u*)(p) = d)
#define host_writed(p, d)	(*(Bit32u*)(p) = d)

#define mem_memcpy memcpy

#define hero_dead(hero)  (((host_readb(hero + 0xaa)) & 1))
#define hero_unc(hero)  (((host_readb(hero + 0xaa)) >> 6) & 1)

#define get_spelluser() ds_readd(SPELLUSER)
#define get_spelltarget() ds_readd(SPELLTARGET)

#define get_ltx(nr) (Bit8u*)(&text_ltx[nr])
#define get_dtp(nr) (Bit8u*)(&dialog_text[nr])
#define get_city(nr) (Bit8u*)(&city_ltx[nr])

/* Unfunctional, but compiles */
#define get_itemsdat(nr) (Bit8u*)(&text_ltx)

#define get_monname(off) (RealPt)host_readd(Real2Host(ds_readd(MONNAMES) + off * 4))

#define struct_copy memcpy


#define BORLAND_FAR __far

#define get_cb_val(x, y) (host_readb((ds_readd(CHESSBOARD)) + y * 25 + x))

#endif
#endif
