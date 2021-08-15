/*
 *	transitional functions for version 3.02_de
 *
 *	Here are function, which should make the transition from
 *	DOXBox emulation to native easier.
 *	These functions will only work with the version mentioned above.
 */

#if !defined V302DE_H
#define V302DE_H

#include <stdlib.h>

#if !defined(__BORLANDC__)
#include <assert.h>
#include "schick.h"
#endif

#include "symbols.h"
#include "datseg.h"

#define MINUTES(n)	((n) * 90L)
#define HOURS(n)	(MINUTES(n) * 60L)
#define DAYS(n)		(HOURS(n) * 24L)
#define MONTHS(n)	(DAYS(n) * 30L)

/* HACK: this cast is not optimized by Borland C++ 3.1 */
static inline unsigned short cast_u16(unsigned char v)
{
	return (unsigned short)v;
}

#if !defined(__BORLANDC__)

#define INTCAST RealPt

typedef RealPt HugePt;

#include "common.h"

static inline RealPt ds_readfp(unsigned short offs)
{
	return (RealPt)host_readd(p_datseg + offs);
}
static inline HugePt ds_readhp(unsigned short offs)
{
	return (HugePt)host_readd(p_datseg + offs);
}
static inline RealPt ds_writefp(unsigned short offs, RealPt ptr)
{
	host_writed(p_datseg + offs, (Bit32u)ptr);
	return ptr;
}
static inline HugePt ds_writehp(unsigned short offs, HugePt ptr)
{
	host_writed(p_datseg + offs, (Bit32u)ptr);
	return ptr;
}

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

static inline Bit8s host_writebs(Bit8u* p, Bit8s val)
{
	host_writeb(p, val);
	return val;
}

static inline Bit16s host_writews(Bit8u* p, Bit16s val)
{
	host_writew(p, val);
	return val;
}

static inline Bit32s host_writeds(Bit8u* p, Bit32s val)
{
	host_writed(p, val);
	return val;
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

static inline Bit8u *get_hero(signed short index) {
	if (index < 0 || index > 6) {
		D1_ERR("ERROR: Versuch auf Held an Position %d zuzugreifen\n", index);
	}
	return Real2Host(ds_readd(HEROES)) + index * SIZEOF_HERO;
}

static inline void add_ds_ws(Bit16u off, Bit16s val)
{
	ds_writew(off, ds_readws(off) + val);
}

static inline void sub_ds_ws(Bit16u off, Bit16s val)
{
	ds_writew(off, ds_readws(off) - val);
}

static inline void mul_ds_ws(Bit16u off, Bit16s val)
{
	ds_writew(off, ds_readws(off) * val);
}

static inline void div_ds_ws(Bit16u off, Bit16s val)
{
	ds_writew(off, ds_readws(off) / val);
}

static inline void mod_ds_ws(Bit16u off, Bit16s val)
{
	ds_writew(off, ds_readws(off) % val);
}

static inline void and_ds_ws(Bit16u off, Bit16s val)
{
	ds_writew(off, ds_readws(off) & val);
}

static inline void shr_ds_ws(Bit16u off, Bit16s val)
{
	ds_writew(off, ds_readw(off) >> val);
}

static inline Bit32s add_ds_ds(Bit16u off, Bit32s val)
{
	return ds_writed(off, ds_readds(off) + val);
}

static inline Bit32s sub_ds_ds(Bit16u off, Bit32s val)
{
	return ds_writed(off, ds_readds(off) - val);
}

static inline RealPt add_ds_fp(Bit16u off, Bit16s val)
{
	const RealPt p_old = ds_readfp(off);
	const RealPt p_new = p_old + val;

	assert(RealSeg(p_old) == RealSeg(p_new));

	ds_writefp(off, p_new);
	return p_new;
}

static inline RealPt sub_ds_fp(Bit16u off, Bit16s val)
{
	const RealPt p_old = ds_readfp(off);
	const RealPt p_new = p_old - val;

	assert(RealSeg(p_old) == RealSeg(p_new));

	ds_writefp(off, p_new);
	return p_new;
}

/* Increment and Decrement on Bit8s variables in the datasegment */

static inline Bit8s inc_ds_bs(Bit16u off)
{
	return ds_writeb(off, ds_readbs(off) + 1);
}

static inline Bit8s dec_ds_bs(Bit16u off)
{
	return ds_writeb(off, ds_readbs(off) - 1);
}

static inline Bit8s inc_ds_bs_post(Bit16u off)
{
	Bit8s val = ds_readbs(off);
	ds_writeb(off, ds_readbs(off) + 1);
	return val;
}

static inline Bit8s dec_ds_bs_post(Bit16u off)
{
	Bit8s val = ds_readbs(off);
	ds_writeb(off, ds_readbs(off) - 1);
	return val;
}


static inline void add_ds_bs(Bit16u off, Bit8s val)
{
	ds_writeb(off, ds_readbs(off) + val);
}

static inline void sub_ds_bs(Bit16u off, Bit8s val)
{
	ds_writeb(off, ds_readbs(off) - val);
}

static inline Bit8u add_ds_bu(Bit16u off, Bit8s val)
{
	return ds_writeb(off, ds_readb(off) + val);
}

static inline void and_ds_bs(Bit16u off, Bit8s val)
{
	ds_writeb(off, ds_readbs(off) & val);
}

static inline void or_ds_bs(Bit16u off, const unsigned char val)
{
	ds_writeb(off, ds_readb(off) | val);
}

static inline void or_ds_ws(Bit16u off, const signed short val)
{
	ds_writew(off, ds_readw(off) | val);
}

static inline void xor_ds_bs(Bit16u off, const unsigned char val)
{
	ds_writeb(off, ds_readb(off) ^ val);
}

static inline signed short inc_ds_ws(Bit16u off)
{
	return ds_writew(off, ds_readws(off) + 1);
}

static inline signed short inc_ds_ws_post(Bit16u off)
{
	return ds_writew(off, ds_readws(off) + 1) - 1;
}

static inline Bit16s dec_ds_ws(Bit16u off)
{
	return ds_writew(off, ds_readws(off) - 1);
}

static inline Bit16s dec_ds_ws_post(Bit16u off)
{
	return ds_writew(off, ds_readws(off) - 1) + 1;
}

static inline void inc_ptr_bs(Bit8u *p)
{
	host_writebs(p, host_readbs(p) + 1);
}

static inline void inc_ptr_ws(Bit8u *p)
{
	host_writews(p, host_readws(p) + 1);
}

static inline Bit8s dec_ptr_bs(Bit8u *p)
{
	host_writebs(p, host_readbs(p) - 1);
	return host_readbs(p) + 1;
}

static inline void dec_ptr_ws(Bit8u *p)
{
	host_writews(p, host_readws(p) - 1);
}

static inline void or_ptr_bs(Bit8u *p, const unsigned char val)
{
	host_writebs(p, host_readbs(p) | val);
}

static inline void and_ptr_bs(Bit8u *p, const unsigned char val)
{
	host_writebs(p, host_readbs(p) & val);
}

static inline void add_ptr_bs(Bit8u *p, const signed char val)
{
	host_writebs(p, host_readbs(p) + val);
}

static inline void sub_ptr_bs(Bit8u *p, const signed char val)
{
	host_writebs(p, host_readbs(p) - val);
}

static inline void add_ptr_ws(Bit8u *p, Bit16s val)
{
	host_writews(p, host_readws(p) + val);
}

static inline void sub_ptr_ws(Bit8u *p, Bit16s val)
{
	host_writews(p, host_readws(p) - val);
}

static inline void mul_ptr_ws(Bit8u *p, Bit16s val)
{
	host_writews(p, host_readws(p) * val);
}

static inline Bit32s add_ptr_ds(Bit8u *p, Bit32s val)
{
	return host_writeds(p, host_readds(p) + val);
}

static inline Bit32s sub_ptr_ds(Bit8u *p, Bit32s val)
{
	return host_writeds(p, host_readds(p) - val);
}

static inline int __abs__(int j)
{
	return abs(j);
}

/**
 * test_bit0() -	check if bit0 ist set
 * @p:			pointer
 *
 * 0 = not set / 1 = set
 */
static inline int test_bit0(Bit8u *p) {
	return (host_readbs(p) & 1) ? 1 : 0;
}

/**
 * test_bit1() -	check if bit1 ist set
 * @p:			pointer
 *
 * 0 = not set / 1 = set
 */
static inline int test_bit1(Bit8u *p) {
	return ((host_readbs(p) >> 1) & 1) ? 1 : 0;
}

/**
 * test_bit2() -	check if bit2 ist set
 * @p:			pointer
 *
 * 0 = not set / 1 = set
 */
static inline int test_bit2(Bit8u *p) {
	return ((host_readbs(p) >> 2) & 1) ? 1 : 0;
}

/**
 * test_bit3() -	check if bit3 ist set
 * @p:			pointer
 *
 * 0 = not set / 1 = set
 */
static inline int test_bit3(Bit8u *p) {
	return ((host_readbs(p) >> 3) & 1) ? 1 : 0;
}

/**
 * test_bit5() -	check if bit5 ist set
 * @p:			pointer
 *
 * 0 = not set / 1 = set
 */
static inline int test_bit5(Bit8u *p) {
	return ((host_readbs(p) >> 5) & 1) ? 1 : 0;
}

/**
 * test_bit6() -	check if bit6 ist set
 * @p:			pointer
 *
 * 0 = not set / 1 = set
 */
static inline int test_bit6(Bit8u *p) {
	return ((host_readbs(p) >> 6) & 1) ? 1 : 0;
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
 * hero_asleep() -	check if hero is sleeping
 * @hero:	ptr to hero
 *
 * 0 = awake / 1 = asleep
 */
static inline unsigned short hero_asleep(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 1) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * hero_petrified() -	check if hero is petrified
 * @hero:	ptr to hero
 *
 * 0 = non-petrified / 1 = petrified
 */
static inline unsigned short hero_petrified(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 2) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * hero_brewing() -	check if hero is brewing
 * @hero:	ptr to hero
 *
 * 0 = not brewing / 1 = brewing
 */
static inline unsigned short hero_brewing(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 3) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * \brief		is hero under chamaelioni spell
 * \param	hero	pointer to hero
 *
 * \return 0 = no / 1 = yes
 */
static inline unsigned short hero_chamaelioni(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 4) & 1) == 0)
		return 0;
	else
		return 1;
}
/**
 * hero_renegade() -	check if hero is renegade
 * @hero:	ptr to hero
 *
 * 0 = no / 1 = yes
 */
static inline unsigned short hero_renegade(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 5) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * hero_unconscious() -	check if hero is unconscious
 * @hero:	ptr to hero
 *
 * 0 = awake / 1 = unconscious
 */
static inline unsigned short hero_unconscious(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 6) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short hero_tied(Bit8u *hero) {

	if (((host_readb(hero + 0xaa) >> 7) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short hero_scared(Bit8u *hero) {

	if (((host_readb(hero + 0xab) >> 0) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short hero_dummy2(Bit8u *hero) {

	if (((host_readb(hero + 0xab) >> 1) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short hero_duplicatus(Bit8u *hero) {

	if (((host_readb(hero + 0xab) >> 2) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short hero_tame(Bit8u *hero) {

	if (((host_readb(hero + 0xab) >> 3) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short hero_seen_phantom(Bit8u *hero) {

	if (((host_readb(hero + 0xab) >> 4) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short hero_gods_pissed(Bit8u *hero) {

	if (((host_readb(hero + 0xab) >> 5) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * hero_transformed() -	check if hero is transformed
 * @hero:	ptr to hero
 *
 * 0 = not transformed / 1 = transformed
 */
static inline unsigned short hero_transformed(Bit8u *hero) {

	if (((host_readb(hero + 0xab) >> 6) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short hero_encouraged(Bit8u *hero) {

	if (((host_readb(hero + 0xab) >> 7) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short hero_seen_phantom_set(Bit8u *hero, unsigned short val)
{
	/* unset this bit */
	host_writeb(hero + HERO_FLAGS2, host_readb(hero + HERO_FLAGS2) & 0xef);
	host_writeb(hero + HERO_FLAGS2, host_readb(hero + HERO_FLAGS2) | ((val & 1) << 4));
	return (val & 1);
}

/**
 * enemy_dead() -	check if enemy is dead
 * @enemy:	ptr to enemy
 *
 * 0 = alive / 1 = dead
 */
static inline unsigned short enemy_dead(Bit8u *enemy) {
	if (((host_readb(enemy + ENEMY_SHEET_FLAGS1) >> 0) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short enemy_asleep(Bit8u *enemy) {
	if (((host_readb(enemy + ENEMY_SHEET_FLAGS1) >> 1) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * enemy_petrified() -	check if enemy is petrified
 * @enemy:	ptr to enemy
 *
 * 0 = not petrified / 1 = petrified
 */
static inline unsigned short enemy_petrified(Bit8u *enemy) {
	if (((host_readb(enemy + ENEMY_SHEET_FLAGS1) >> 2) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short enemy_busy(Bit8u *enemy) {
	if (((host_readb(enemy + ENEMY_SHEET_FLAGS1) >> 3) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short enemy_tied(Bit8u *enemy) {
	if (((host_readb(enemy + ENEMY_SHEET_FLAGS1) >> 5) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short enemy_mushroom(Bit8u *enemy) {
	if (((host_readb(enemy + ENEMY_SHEET_FLAGS1) >> 6) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * enemy_illusion() -	check if enemy is an illusion
 * @enemy:	ptr to enemy
 *
 * 0 = real / 1 = illusion
 */
static inline unsigned short enemy_illusion(Bit8u *enemy) {
	if (((host_readb(enemy + ENEMY_SHEET_FLAGS1) >> 7) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short enemy_tame(Bit8u *enemy) {
	if (((host_readb(enemy + ENEMY_SHEET_FLAGS2) >> 0) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * enemy_renegade() -	check if enemy is under boeser blick spell
 * @enemy:	ptr to enemy
 *
 * 0 = no / 1 = casted
 */
static inline unsigned short enemy_renegade(Bit8u *enemy) {
	if (((host_readb(enemy + ENEMY_SHEET_FLAGS2) >> 1) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short enemy_scared(Bit8u *enemy) {
	if (((host_readb(enemy + ENEMY_SHEET_FLAGS2) >> 2) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short enemy_dancing(Bit8u *enemy) {
	if (((host_readb(enemy + ENEMY_SHEET_FLAGS2) >> 3) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * inventory_broken() -	check if an item in the inventory is broken
 * @item:	ptr to item
 *
 * 0 = not broken / 1 = broken
 */
static inline unsigned short inventory_broken(Bit8u *inventory) {
	if (((host_readb(inventory + INVENTORY_FLAGS) >> 0) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * inventory_half_empty() -	check if an item (only used for ITEM_WATERSKIN) in the inventory is half empty
 * @item:	ptr to item
 *
 * 0 = filled / 1 = half empty
 */
static inline unsigned short inventory_half_empty(Bit8u *inventory) {
	if (((host_readb(inventory + INVENTORY_FLAGS) >> 1) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * inventory_empty() -	check if an item (only used for ITEM_WATERSKIN) in the inventory is empty
 * @item:	ptr to item
 *
 * 0 = filled / 1 = empty
 */
static inline unsigned short inventory_empty(Bit8u *inventory) {
	if (((host_readb(inventory + INVENTORY_FLAGS) >> 2) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * inventory_magic() -	check if an item in the inventory is magic
 * @item:	ptr to item
 *
 * 0 = not magic / 1 = magic
 */
static inline unsigned short inventory_magic(Bit8u *inventory) {
	if (((host_readb(inventory + INVENTORY_FLAGS) >> 3) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * inventory_poison_expurgicum() -	check if an item in the inventory has the poison_expurgicum flag set
 * @item:	ptr to item
 *
 * 0 = no / 1 = yes
 */
static inline unsigned short inventory_poison_expurgicum(Bit8u *inventory) {
	if (((host_readb(inventory + INVENTORY_FLAGS) >> 5) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * inventory_poison_vomicum() -	check if an item in the inventory has the poison_vomicum flag set
 * @item:	ptr to item
 *
 * 0 = no / 1 = yes
 */
static inline unsigned short inventory_poison_vomicum(Bit8u *inventory) {
	if (((host_readb(inventory + INVENTORY_FLAGS) >> 6) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * inventory_magic_revealed() -	check if an item in the inventory is magic and you know
 * @item:	ptr to item
 *
 * 0 = know not / 1 = you know its magic
 */
static inline unsigned short inventory_magic_revealed(Bit8u *inventory) {
	if (((host_readb(inventory + INVENTORY_FLAGS) >> 7) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline void add_inventory_quantity(signed short i1, signed short i2, Bit8u *hero) {
	add_ptr_ws(hero + HERO_INVENTORY + i1 * SIZEOF_INVENTORY + INVENTORY_QUANTITY, host_readw(hero + HERO_INVENTORY + i2 * SIZEOF_INVENTORY + INVENTORY_QUANTITY));
}

/**
 * item_armor() -	check if an item is an armor
 * @item:	ptr to item
 *
 * 0 = non armor / 1 = armor
 */
static inline unsigned short item_armor(Bit8u *item) {
	if (((host_readb(item + ITEM_STATS_FLAGS) >> 0) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_weapon() -	check if an item is a weapon
 * @item:	ptr to item
 *
 * 0 = non weapon / 1 = weapon
 */
static inline unsigned short item_weapon(Bit8u *item) {
	if (((host_readb(item + ITEM_STATS_FLAGS) >> 1) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_useable() -	check if an item is useable
 * @item:	ptr to item
 *
 * 0 = no / 1 = yes
 */
static inline unsigned short item_useable(Bit8u *item) {
	if (((host_readb(item + ITEM_STATS_FLAGS) >> 2) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_food() -	check if an item is food
 * @item:	ptr to item
 *
 * 0 = non food / 1 = food
 */
static inline unsigned short item_food(Bit8u *item) {
	if (((host_readb(item + ITEM_STATS_FLAGS) >> 3) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_stackable() -	check if an item is stackable
 * @item:	ptr to item
 *
 * 0 = non stackable / 1 = stackable
 */
static inline unsigned short item_stackable(Bit8u *item) {
	if (((host_readb(item + ITEM_STATS_FLAGS) >> 4) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_herb_potion() -	check if an item is a herb or potion
 * @item:	ptr to item
 *
 * 0 = non / 1 = herb or potion
 */
static inline unsigned short item_herb_potion(Bit8u *item) {
	if (((host_readb(item + ITEM_STATS_FLAGS) >> 5) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_undropable() -	check if an item is undropable
 * @item:	ptr to item
 *
 * 0 = dropable / 1 = undropable
 */
static inline unsigned short item_undropable(Bit8u *item) {
	if (((host_readb(item + ITEM_STATS_FLAGS) >> 6) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline Bit8u *get_spelltarget_e() {
	return Real2Host(ds_readd(SPELLTARGET_E));
}

static inline Bit8u *get_spelltarget() {
	return Real2Host(ds_readd(SPELLTARGET));
}

static inline Bit8u *get_spelluser() {
	return Real2Host(ds_readd(SPELLUSER));
}

static inline Bit8u *get_spelluser_e() {
	return Real2Host(ds_readd(SPELLUSER_E));
}


static inline Bit8u *get_itemuser() {
	return Real2Host(ds_readd(ITEMUSER));
}

static inline Bit8u *get_fname(unsigned short off) {
	return Real2Host(ds_readd(FNAMES + off * 4));
}

static inline Bit8u *get_monname(unsigned short off)
{
	return Real2Host(host_readd(Real2Host(ds_readd(MONNAMES_INDEX) + off * 4)));
}

#define get_tx2(no) get_tx2_func(4*(no))
static inline Bit8u *get_tx2_func(unsigned short off) {
	return Real2Host(host_readd(Real2Host(ds_readd(TX2_INDEX) + off)));
}

#define get_ttx(no) get_ttx_func(4*(no))
static inline Bit8u *get_ttx_func(unsigned short off) {
	return Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX_INDEX) + off)));
}

#define get_tx(no) get_tx_func(4*(no))
static inline Bit8u *get_tx_func(unsigned short off) {
	return Real2Host(host_readd(Real2Host(ds_readd(TX_INDEX) + off)));
}

static inline signed char get_cb_val(signed short x, signed short y) {
	return host_readbs(Real2Host(ds_readd(CHESSBOARD)) + y * 25 + x);
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

#define DUMMY_WARNING() D1_ERR("Error: %s is not implemented\n", __func__)

// end #if !defined(__BORLANDC__)
#else

#define DUMMY_WARNING()

#undef M302de_ORIGINAL_BUGFIX

#ifdef __cplusplus
#define INTCAST void interrupt (*)(...)
#else
#define INTCAST void interrupt (*)()
#endif

#include <DOS.H>

#include "common.h"

#define NOT_NULL(p) ((p) != NULL)

/* poring functions for Borland C++ */
struct hero_struct {
	char name[16];
	char alias[16];
	char dummy;
	char typus;
};

#define schick_main main

#define reloc_game (0)

#define p_datseg (&ds[0x0000])
#define datseg (_DS)

#define ds_readb(p)		(*(Bit8u*)(ds + p))
#define ds_readw(p)		(*(Bit16u*)(ds + p))
#define ds_readd(p)		(*(Bit32u*)(ds + (p)))

#define ds_readbs(p)		(*(Bit8s*)(ds + p))
#define ds_readws(p)		(*(Bit16s*)(ds + p))
#define ds_readds(p)		(*(Bit32s*)(ds + (p)))
#define ds_readfp(p)		(*(RealPt*)(ds + (p)))
#define ds_readhp(p)		(*(HugePt*)(ds + (p)))

#define ds_writebs(p, d)	(*(Bit8s*)(ds + p) = (d))
#define ds_writews(p, d)	(*(Bit16s*)(ds + p) = (d))

#define ds_writeb(p, d)		(*(Bit8u*)(ds + p) = (d))
#define ds_writew(p, d)		(*(Bit16u*)(ds + p) = (d))
#define ds_writed(p, d)		(*(Bit32u*)(ds + p) = (d))
#define ds_writefp(p, d)		(*(RealPt*)(ds + (p)) = (d))
#define ds_writehp(p, d)		(*(HugePt*)(ds + (p)) = (d))

#define inc_ds_bs(o)		(++(*(Bit8s*)(ds + (o))))
#define dec_ds_bs(o)		(--(*(Bit8s*)(ds + (o))))
#define inc_ds_bs_post(o)	((*(Bit8s*)(ds + (o)))++)
#define dec_ds_bs_post(o)	((*(Bit8s*)(ds + (o)))--)

#define add_ds_bs(o, val)	((*(Bit8s*)(ds + (o)))+= (val))
#define sub_ds_bs(o, val)	((*(Bit8s*)(ds + (o)))-= (val))
#define add_ds_bu(o, val)	((*(Bit8u*)(ds + (o)))+= (val))

#define inc_ds_ws(o)		(++(*(Bit16s*)(ds + (o))))
#define dec_ds_ws(o)		(--(*(Bit16s*)(ds + (o))))

#define inc_ds_ws_post(o)	((*(Bit16s*)(ds + o))++)
#define dec_ds_ws_post(o)	((*(Bit16s*)(ds + o))--)

#define and_ds_bs(o, v)		(*(Bit8s*)(ds + o) &= (v))
#define or_ds_bs(o, v)		(*(Bit8s*)(ds + o) |= (v))
#define or_ds_ws(o, v)		(*(Bit16s*)(ds + o) |= (v))
#define xor_ds_bs(o, v)		(*(Bit8s*)(ds + o) ^= (v))

#define add_ds_ws(o, v)		(*(Bit16s*)(ds + o) += (v))
#define sub_ds_ws(o, v)		(*(Bit16s*)(ds + (o)) -= (v))
#define mul_ds_ws(o, v)		(*(Bit16s*)(ds + (o)) *= (v))
#define div_ds_ws(o, v)		(*(Bit16s*)(ds + (o)) /= (v))
#define mod_ds_ws(o, v)		(*(Bit16s*)(ds + (o)) %= (v))
#define and_ds_ws(o, v)		(*(Bit16s*)(ds + (o)) &= (v))
#define shr_ds_ws(o, v)		(*(Bit16s*)(ds + (o)) >>= (v))

#define add_ds_ds(o, v)		(*(Bit32s*)(ds + (o)) += (v))
#define sub_ds_ds(o, v)		(*(Bit32s*)(ds + (o)) -= (v))

#define add_ds_fp(o, v)		(*(RealPt*)(ds + (o)) += (v))
#define sub_ds_fp(o, v)		(*(RealPt*)(ds + (o)) -= (v))

#define inc_ptr_bs(p)		((*(Bit8s*)(p))++)
#define dec_ptr_bs(p)		((*(Bit8s*)(p))--)

#define inc_ptr_ws(p)		((*(Bit16s*)(p))++)
#define dec_ptr_ws(p)		((*(Bit16s*)(p))--)

#define or_ptr_bs(p, v)		(*(Bit8s*)(p) |= (v))
#define and_ptr_bs(p, v)	(*(Bit8s*)(p) &= (v))
#define add_ptr_bs(p, v)	(*(Bit8s*)(p) += (v))
#define sub_ptr_bs(p, v)	(*(Bit8s*)(p) -= (v))


#define add_ptr_ws(p, v)	(*(Bit16s*)(p) += (v))
#define sub_ptr_ws(p, v)	(*(Bit16s*)(p) -= (v))
#define mul_ptr_ws(p, v)	(*(Bit16s*)(p) *= (v))

#define add_ptr_ds(p, v)	(*(Bit32s*)(p) += (v))
#define sub_ptr_ds(p, v)	(*(Bit32s*)(p) -= (v))

#define mem_readb(p) (*(Bit8u*)(p))
#define mem_readw(p) (*(Bit16u*)(p))
#define mem_readd(p) (*(Bit32u*)(p))

#define mem_readbs(p) (*(Bit8s*)(p))
#define mem_readws(p) (*(Bit16s*)(p))
#define mem_readds(p) (*(Bit32s*)(p))

#define mem_writeb(p, d) (*(Bit8u*)(p) = (d))
#define mem_writew(p, d) (*(Bit16u*)(p) = (d))
#define mem_writed(p, d) (*(Bit32u*)(p) = (d))

#define get_hero(no) ((Bit8u*)ds_readfp(HEROES) + SIZEOF_HERO * (no))

#ifdef M302de_ORIGINAL_BUGFIX
#define ds_writeb_z(addr, val) (if (ds_readb(addr) == 0) ds_writeb(addr, val))
#else
#define ds_writeb_z(addr, val) (ds_writeb(addr, val))
#endif

extern Bit8u* text_ltx_index[];
extern Bit8u* tx_index[];
extern Bit8u* tx2_index[];

#define Real2Phys(p) ((Bit8u*)(p))
#define Real2Host(p) ((Bit8u*)(p))
#define getString(p) ((char*)(p))

#define host_readb(p) (*(Bit8u*)(p))
#define host_readw(p) (*(Bit16u*)(p))
#define host_readd(p) (*(Bit32u*)(p))

#define host_readbs(p) (*(Bit8s*)(p))
#define host_readws(p) (*(Bit16s*)(p))
#define host_readds(p) (*(Bit32s*)(p))

#define host_writeb(p, d)	(*(Bit8u*)(p) = (d))
#define host_writew(p, d)	(*(Bit16u*)(p) = (d))
#define host_writed(p, d)	(*(Bit32u*)(p) = (d))

#define host_writebs(p, d)	(*(Bit8s*)(p) = (d))
#define host_writews(p, d)	(*(Bit16s*)(p) = (d))
#define host_writeds(p, d)	(*(Bit32s*)(p) = (d))

#define real_readb(s,o)		host_readb(RealMake((s),(o)))
#define real_readw(s,o)		host_readw(RealMake((s),(o)))
#define real_writed(s,o,v)		host_writed(RealMake((s),(o)),(v))

/* TODO: ugly hack, BASM does not like 16bit immediate values with imul */
#define calc_twodim_array_ptr(start, width, disp, off, dst) \
asm { mov ax,disp; db 0x69,0xc0,0xc0,0x08; mov dx, [start + 2]; add ax, [start]; add ax, off; mov[dst + 2],dx; mov [dst],ax }
struct bittest {
	unsigned short bit0:1;
	unsigned short bit1:1;
	unsigned short bit2:1;
	unsigned short bit3:1;
	unsigned short bit4:1;
	unsigned short bit5:1;
	unsigned short bit6:1;
	unsigned short bit7:1;
};

#define test_bit0(a)		((*(struct bittest*)(a)).bit0)
#define test_bit1(a)		((*(struct bittest*)(a)).bit1)
#define test_bit2(a)		((*(struct bittest*)(a)).bit2)
#define test_bit3(a)		((*(struct bittest*)(a)).bit3)
#define test_bit5(a)		((*(struct bittest*)(a)).bit5)
#define test_bit6(a)		((*(struct bittest*)(a)).bit6)

#define hero_dead(hero)		((*(struct hero_flags*)(hero + HERO_FLAGS1)).dead)
#define hero_asleep(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).asleep)
#define hero_petrified(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).petrified)
#define hero_brewing(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).brewing)
#define hero_chamaelioni(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).chamaelioni)
#define hero_renegade(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).renegade)
#define hero_unconscious(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).unconscious)
#define hero_tied(hero)		((*(struct hero_flags*)(hero + HERO_FLAGS1)).tied)

#define hero_scared(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).scared)
#define hero_dummy2(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).dummy2)
#define hero_duplicatus(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).duplicatus)
#define hero_tame(hero)		((*(struct hero_flags*)(hero + HERO_FLAGS1)).tame)
#define hero_seen_phantom(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).seen_phantom)
#define hero_gods_pissed(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).gods_pissed)
#define hero_transformed(hero)  ((*(struct hero_flags*)(hero + HERO_FLAGS1)).transformed)
#define hero_encouraged(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).encouraged)

#define hero_seen_phantom_set(hero, v) ((*(struct hero_flags*)(hero + HERO_FLAGS1)).seen_phantom = v)

#define enemy_dead(enemy)	(((struct enemy_sheets*)(enemy))->flags1.dead)
#define enemy_asleep(enemy)	(((struct enemy_sheets*)(enemy))->flags1.asleep)
#define enemy_petrified(enemy)	(((struct enemy_sheets*)(enemy))->flags1.petrified)
#define enemy_busy(enemy)	(((struct enemy_sheets*)(enemy))->flags1.busy)
#define enemy_tied(enemy)	(((struct enemy_sheets*)(enemy))->flags1.tied)
#define enemy_mushroom(enemy)	(((struct enemy_sheets*)(enemy))->flags1.mushroom)
#define enemy_illusion(enemy)	(((struct enemy_sheets*)(enemy))->flags1.illusion)

#define enemy_tame(enemy)	(((struct enemy_sheets*)(enemy))->flags2.tame)
#define enemy_renegade(enemy)	(((struct enemy_sheets*)(enemy))->flags2.renegade)
#define enemy_scared(enemy)	(((struct enemy_sheets*)(enemy))->flags2.scared)
#define enemy_dancing(enemy)	(((struct enemy_sheets*)(enemy))->flags2.dancing)

#define add_inventory_quantity(i1, i2, hero) (    ((struct inventory*)(hero + HERO_INVENTORY))[i1].quantity+=((struct inventory*)(hero + HERO_INVENTORY))[i2].quantity)

#define inventory_broken(inventory)		((*(struct inventory_flags*)(inventory + INVENTORY_FLAGS)).broken)
#define inventory_half_empty(inventory)		((*(struct inventory_flags*)(inventory + INVENTORY_FLAGS)).half_empty)
#define inventory_empty(inventory)		((*(struct inventory_flags*)(inventory + INVENTORY_FLAGS)).empty)
#define inventory_magic(inventory)		((*(struct inventory_flags*)(inventory + INVENTORY_FLAGS)).magic)
#define inventory_poison_expurgicum(inventory)	((*(struct inventory_flags*)(inventory + INVENTORY_FLAGS)).poison_expurgicum)
#define inventory_poison_vomicum(inventory)	((*(struct inventory_flags*)(inventory + INVENTORY_FLAGS)).poison_vomicum)
#define inventory_magic_revealed(inventory)	((*(struct inventory_flags*)(inventory + INVENTORY_FLAGS)).magic_revealed)

#define item_armor(item)	((*(struct item_flags*)(item + ITEM_STATS_FLAGS)).armor)
#define item_weapon(item)	((*(struct item_flags*)(item + ITEM_STATS_FLAGS)).weapon)
#define item_useable(item)	((*(struct item_flags*)(item + ITEM_STATS_FLAGS)).useable)
#define item_food(item)		((*(struct item_flags*)(item + ITEM_STATS_FLAGS)).food)
#define item_stackable(item)	((*(struct item_flags*)(item + ITEM_STATS_FLAGS)).stackable)
#define item_herb_potion(item)	((*(struct item_flags*)(item + ITEM_STATS_FLAGS)).herb_potion)
#define item_undropable(item)	((*(struct item_flags*)(item + ITEM_STATS_FLAGS)).undropable)

#define get_spelltarget_e()	((Bit8u*)ds_readfp(SPELLTARGET_E))
#define get_spelltarget()	((Bit8u*)ds_readfp(SPELLTARGET))
#define get_spelluser()		((Bit8u*)ds_readfp(SPELLUSER))
#define get_spelluser_e()	((Bit8u*)ds_readfp(SPELLUSER_E))

#define get_itemuser() ((Bit8u*)ds_readfp(ITEMUSER))

#define get_ttx(no) (char*)(host_readd(ds_readfp(TEXT_LTX_INDEX) + 4 * (no)))
#define get_tx(no) (char*)(host_readd(ds_readfp(TX_INDEX) + 4 * (no)))
#define get_tx2(no) (char*)(host_readd(ds_readfp(TX2_INDEX) + 4 * (no)))
#define get_monname(no) ((char*)(host_readd(ds_readfp(MONNAMES_INDEX) + 4 * (no))))
#define get_itemsdat(no) ((char*)(ds_readfp(ITEMSDAT) + 12 * (no)))
#define get_itemname(no) ((char*)(host_readd(ds_readfp(ITEMSNAME) + 4 * (no))))

#define get_cb_val(x, y) (host_readbs(ds_readfp(CHESSBOARD) + ((y) * 25) + (x)))
#define set_cb_val(x, y, val) (host_writeb((ds_readfp(CHESSBOARD)) + (y) * 25 + (x), (val)))

#endif
#endif
