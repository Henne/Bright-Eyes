/*
 *	transitional functions for version 3.02_de
 *
 *	Here are function, which should make the transition from
 *	DOXBox emulation to native easier.
 *	These functions will only work with the version mentioned above.
 */

#include "schick.h"

#define EMS_ENABLED	(0x26ab)
#define TIMERS_DISABLED	(0x2c99)
#define CURRENT_ANI	(0x2ccf)
#define DAY_TIMER	(0x2dbb)
#define DAY_OF_WEEK	(0x2dbf)
#define DAY_OF_MONTH	(0x2dc0)
#define MONTH		(0x2dc1)
#define YEAR		(0x2dc2)
#define SPECIAL_DAY	(0x2dc3)
#define UNICORN_MET	(0x2464)
#define UNICORN_GET_MAP	(0x2465)
#define UNICORN_TIMER	(0x2466)
#define GOT_MAIN_QUEST	(0x334c)
#define QUESTED_MONTHS	(0x334e)
#define NPC_MONTHS	(0x3470)
#define SPHERE_TIMER	(0x3c97)
#define SPHERE_ACTIVE	(0x3c98)
#define CHECK_PARTY	(0x4495)
#define DELAY_FACTOR	(0x4b66)
#define FNAMES		(0x4c8c)
#define HEROS		(0xbd34)
#define DIALOG_TEXT	(0xc3b1)
#define CITY_LTX	(0xc3ad)
#define TEXT_LTX	(0xc3b5)
#define ANI_MAIN_PTR	(0xce35)
#define TEXTCOLOR_BG	(0xd2c7)
#define TEXTCOLOR_FG	(0xd2c9)
#define DTP2		(0xd2f3)
#define CHESSBOARD	(0xd852)
#define MEM_SLOTS_MON	(0xe115)
#define MEM_SLOTS_WFIG	(0xe119)
#define MEM_SLOTS_MFIG	(0xe11d)
#define MONNAMES	(0xe129)
#define ITEMSDAT	(0xe22b)
#define ITEMSNAME	(0xe22f)
#define DELAY_TIMER	(0xe2d0)
#define CURRENT_FIG_NR	(0xe316)
#define GRAMMAR_STRING	(0xe4e3)	/* char grammar_string[201] */
#define SPELLTARGET	(0xe5b8)
#define SPELLUSER	(0xe5bc)

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
	if ((host_readb(hero + 0xaa) & 1) == 0)
		return 0;
	else
		return 1;
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
