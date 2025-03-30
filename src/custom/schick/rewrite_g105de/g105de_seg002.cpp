#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "hero.h"

#if !defined(__BORLANDC__)
#include "regs.h"
#include "paging.h"
#include "callback.h"
#include "../../../dos/drives.h"

#include "../schick.h"

#endif

#include "g105de_seg000.h"
#include "g105de_seg001.h"
#include "g105de_seg002.h"
#include "g105de_seg003.h"
#include "g105de_seg004.h"
#include "g105de_seg005.h"
#include "g105de_seg006.h"

#if !defined(__BORLANDC__)
namespace G105de {
#endif

#include "port.h"
#include "symbols.h"

/** Keyboard Constants */
static const unsigned short KEY_ESC = 0x01;
static const unsigned short KEY_1 = 0x02;
static const unsigned short KEY_2 = 0x03;
static const unsigned short KEY_3 = 0x04;
static const unsigned short KEY_4 = 0x05;
static const unsigned short KEY_5 = 0x06;

static const unsigned short KEY_RET = 0x1c;

static const unsigned short KEY_J = 0x24;
static const unsigned short KEY_Y = 0x2c;
static const unsigned short KEY_N = 0x31;

static const unsigned short KEY_UP = 0x48;
static const unsigned short KEY_LEFT = 0x4b;
static const unsigned short KEY_RIGHT = 0x4d;
static const unsigned short KEY_DOWN = 0x50;

static const unsigned short KEY_PGUP = 0x49;
static const unsigned short KEY_PGDOWN = 0x51;

static const unsigned short KEY_CTRL_F3 = 0x60;
static const unsigned short KEY_CTRL_F4 = 0x61;


/**
	struct nvf_desc - nvf descriptor
	@src:   pointer to a buffer containing the nvf file
	@dst:   pointer where to extract the picture
	@no:    number of the picture to extract
	@type:  kind of compression / direction (0 = PP20 / 2-5 RLE / copy)
	@p_height:      pointer where the height of the picture must be stored
	@p_width:       pointer where the width of the picture must be stored
*/

struct struct_color {
	signed char r;
	signed char g;
	signed char b;
};

static const Bit16u MAX_PAGES = 11;
static const Bit16u MAX_TYPES = 13;

struct struct_spelltab {
	signed char origin;
	signed char att1;
	signed char att2;
	signed char att3;
	signed char cost;
};

static const struct struct_spelltab spelltab[87] = {
	{ 0, 0, 0, 0, 0},
	{ 0, 1, 5, 2, -1},
	{ 1, 1, 5, 3, -1},
	{ 1, 5, 2, 6, -1},
	{ 1, 1, 5, 2, -1},
	{ 3, 0, 1, 2, -1},
	{ 2, 1, 2, 6, 12},
	{ 2, 5, 2, 2, 8},
	{ 0, 0, 2, 2, 8},
	{ 3, 1, 5, 2, 8},
	{ 0, 1, 1, 2, -1},
	{ 0, 0, 0, 2, -1},
	{ 1, 0, 5, 2, 7},
	{ 0, 0, 1, 6, 6},
	{ 1, 1, 5, 2, 5},
	{ 3, 0, 2, 2, -1},
	{ 2, 1, 2, 2, 8},
	{ 0, 0, 1, 2, 6},
	{ 1, 0, 0, 2, 13},
	{ 0, 0, 0, 2, 13},
	{ 0, 0, 0, 2, 11},
	{ 1, 0, 0, 2, 23},
	{ 3, 0, 2, 2, 13},
	{ 1, 0, 0, 2, 15},
	{ 0, 0, 1, 2, -1},
	{ 1, 1, 6, 6, 5},
	{ 2, 5, 4, 6, -1},
	{ 2, 1, 4, 6, 7},
	{ 1, 1, 1, 3, -1},
	{ 1, 1, 3, 6, -1},
	{ 2, 5, 4, 4, 7},
	{ 1, 1, 1, 6, -1},
	{ 2, 1, 4, 6, 5},
	{ 2, 1, 5, 2, -1},
	{ 3, 5, 2, 3, -1},
	{ 1, 1, 1, 2, -1},
	{ 2, 1, 2, 6, 7},
	{ 3, 0, 5, 2, -1},
	{ 2, 1, 5, 3, 5},
	{ 1, 1, 1, 5, 10},
	{ 2, 1, 5, 2, 7},
	{ 2, 1, 1, 5, 5},
	{ 2, 1, 5, 2, 5},
	{ 1, 1, 1, 6, -1},
	{ 2, 1, 5, 2, 5},
	{ 2, 5, 2, 4, 5},
	{ 1, 1, 2, 4, 8},
	{ 3, 1, 2, 4, 6},
	{ 3, 1, 5, 2, 4},
	{ 2, 1, 5, 4, 5},
	{ 1, 0, 1, 6, 5},
	{ 1, 1, 2, 4, 5},
	{ 2, 1, 4, 6, -1},
	{ 1, 1, 4, 3, -1},
	{ 1, 2, 4, 6, 5},
	{ 3, 0, 5, 2, 4},
	{ 1, 0, 5, 2, -1},
	{ 2, 5, 4, 3, 5},
	{ 3, 5, 5, 2, 2},
	{ 1, 0, 1, 2, 11},
	{ 2, 0, 5, 4, 7},
	{ 1, 0, 1, 2, 7},
	{ 2, 5, 4, 6, -1},
	{ 1, 1, 2, 2, 7},
	{ 3, 0, 0, 6, 7},
	{ 2, 1, 2, 3, 7},
	{ 2, 1, 2, 4, 7},
	{ 2, 1, 5, 2, 7},
	{ 2, 1, 2, 6, 7},
	{ 1, 1, 1, 2, 7},
	{ 2, 0, 1, 2, 7},
	{ 1, 1, 4, 6, -1},
	{ 1, 5, 2, 6, 13},
	{ 1, 1, 5, 2, -1},
	{ 2, 0, 1, 6, -1},
	{ 2, 1, 1, 4, -1},
	{ 2, 1, 1, 3, 5},
	{ 2, 1, 2, 6, 5},
	{ 1, 0, 1, 6, -1},
	{ 1, 1, 3, 6, -1},
	{ 0, 1, 1, 3, 3},
	{ 1, 0, 1, 6, -1},
	{ 2, 1, 1, 3, 1},
	{ 1, 0, 1, 6, -1},
	{ 2, 1, 1, 2, -1},
	{ 2, 1, 2, 6, 5},
	{ -1, 0, 0, 0, 0},
};

/* DS:0x030b */
static const signed short house_spells[][8] = {
	/* Antimagie */
	{0x01, 0x02, 0x03, 0x04, 0x05, 0x2a, -1},
	/* Beherrschung */
	{0x07, 0x0c, 0x0e, 0x10, 0x2c, -1},
	/* Beschwörung */
	{0x12, 0x13, 0x15, 0x17, 0x18, 0x3b, -1},
	/* Bewegung */
	{0x1b, 0x1c, 0x1d, 0x1e, 0x1f, -1},
	/* Heilung */
	{0x21, 0x23, 0x24, 0x3d, 0x46, -1},
	/* Hellsicht */
	{0x26, 0x27, 0x29, 0x28, 0x2a, 0x2b, 0x2c, -1},
	/* Kampf */
	{0x31, 0x33, 0x34, 0x35, 0x36, 0x39, 0x3e, -1},
	/* Verwandlung */
	{0x3c, 0x3e, 0x48, 0x49, 0x4b, -1},
	/* Veränderung */
	{0x4c, 0x4e, 0x4f, 0x50, 0x52, 0x53, 0x54, -1}
};
/* DS:03ab */
struct struct_school_tab {
	unsigned short first_spell;
	unsigned short spells;
};
static const struct struct_school_tab school_tab[] = {
	{0x01, 0x05},
	{0x06, 0x0c},
	{0x12, 0x09},
	{0x1b, 0x06},
	{0x21, 0x05},
	{0x26, 0x07},
	{0x31, 0x09},
	{0x3c, 0x10},
	{0x4c, 0x0a},
};

struct struct_reqs {
	unsigned char attrib, requirement;
};

static const struct_reqs reqs[13][4] = {
	{ 0 },
	{ {0, 12}, {4, 12}, {3, 12}, {7, 7}, },
	{ {5, 12}, {4, 12}, {9, 7}, {2, 1}, },
	{ {0, 13}, {6, 12}, {13, 0x80 | 4}, {2, 1}, },
	{ {0, 12}, {4, 13}, {3, 13}, {2, 1}, },
	{ {0, 12}, {6, 13}, {7, 7}, {2, 1}, },
	{ {6, 13}, {3, 12}, {10, 7}, {9, 0x80 | 4}, },

	{ {5, 12}, {2, 13}, {8, 0x80 | 4}, {6, 1}, },
	{ {0, 13}, {1, 12}, {11, 0x80 |4}, {2, 1}, },
	{ {1, 13}, {2, 12}, {7, 0x80 | 4}, {6, 1}, },

	{ {1, 13}, {4, 12}, {10, 0x80 | 4}, {2, 1}, },
	{ {5, 12}, {4, 13}, {10, 0x80 | 4}, {2, 1}, },
	{ {5, 13}, {4, 13}, {10, 0x80 | 4}, {2, 1}, },
};

#if 0
static const signed char skills[13][52] = {
	/* DUMMY */
	{ 0},
	/* Gaukler/ Jester*/
	{
		/* Kampf / Combat */
		4, 1, 3, 1, 0, 1, -2, 0, 3,
		/* Körper / Body */
		5, 5, 5, 3, 4, 1, 0, 5, 4, 2,
		/* Gesellschaft / Social */
		-4, 2, 1, 3, 4, 2, 2,
		/* Natur / Nature */
		-3, 5, -1, -3, -2, -3,
		/* Handwerk / Craftsmanship */
		0, -4, 3, -4, 1, -6, -2, -2, 2,
		/* Wissen / Lore */
		3, 0, 4, 0, 0, 2, 4, 2, 2,
		/* Intuition */
		1, 1
	},
	/* Jäger/ Hunter*/
	{
		/* Kampf / Combat */
		3, 2, 1, 1, 1, 4, -4, 5, 3,
		/* Körper / Body */
		-1, 2, 3, 1, 6, 4, 4, -1, 5, 3,
		/* Gesellschaft / Social */
		-3, -3, -1, -3, 0, 0, 0,
		/* Natur / Nature */
		5, 4, 4, 4, 6, 5,
		/* Handwerk / Craftsmanship */
		-5, -3, 2, -4, 0, -4, -1, -3, 1,
		/* Wissen / Lore */
		2, -1, 1, 1, 1, 3, -1, -1, -2,
		/* Intuition */
		3, 3
	},
	/* Krieger/ Warrior*/
	{
		/* Kampf / Combat */
		4, 4, 1, 6, 2, 3, 4, 3, 3,
		/* Körper / Body */
		0, 1, 3, 4, 0, 2, 4, 0, -1, 3,
		/* Gesellschaft / Social */
		-4, -2, -1, 0, 0, 0, 0,
		/* Natur / Nature */
		-1, -2, 1, -3, -2, 2,
		/* Handwerk / Craftsmanship */
		-5, -1, 1, 2, 3, 3, 2, 0, 0,
		/* Wissen / Lore */
		-2, 0, 0, -1, 0, 2, 0,	0, -2,
		/* Intuition */
		0, 0
	},
	/* Streuner/ Rogue*/
	{
		/* Kampf / Combat */
		4, 2, 4, 1, 1, 1, -2, -2, 3,
		/* Körper / Body */
		1, 3, 4, 1, 4, 0, 1, 2, 4, 3,
		/* Gesellschaft / Social */
		2, 4, 3, 7, 5, 3, 3,
		/* Natur / Nature */
		-2, 2, 0, -2, -2, -2,
		/* Handwerk / Craftsmanship */
		-2, -2, 0, -3, 2, -4, -1, -2, 2,
		/* Wissen / Lore */
		-3, 0, 6, 3, 1, 3, 1, 4, 5,
		/* Intuition */
		3, 2
	},
	/* Thorwaler/ Thorwalian*/
	{
		/* Kampf / Combat */
		4, 3, 0, 1, 4, 2, 2, -3, 2,
		/* Körper / Body */
		-1, 3, 2, -1, -1, 5, 4, 0, -2, 6,
		/* Gesellschaft / Social */
		-4, 0, -2, -2, -1, 0, 0,
		/* Natur / Nature */
		-5, 3, 4, -4, -2, -2,
		/* Handwerk / Craftsmanship */
		-4, -1, 2, 0, 2, 0, 1, -3, 1,
		/* Wissen / Lore */
		-4, -2, 0, -1, 1, 2, 1,	0, -2,
		/* Intuition */
		-3, 2
	},
	/* Zwerg/ Dwarf*/
	{
		/* Kampf / Combat */
		3, 3, 0, 2, 4, 3, 1, 2, 2,
		/* Körper / Body */
		-2, 4, 1, -5, 0, -5, 5, -4, 3, 5,
		/* Gesellschaft / Social */
		-5, -2, 0, -4, -2, -3, 5,
		/* Natur / Nature */
		0, 2, 1, 0, -3, 0,
		/* Handwerk / Craftsmanship */
		2, 2, -2, 3, 0, 1, 1, 0, -2,
		/* Wissen / Lore */
		-3, 0, 0, 2, 3, 3, -1, 3, 0,
		/* Intuition */
		2, 4
	},
	/* Hexer/ Warlock*/
	{
		/* Kampf / Combat */
		1, 0, 2, -5, -5, 2, -6, 0, 2,
		/* Körper / Body */
		-2, 0, 2, -1, 2, 1, 4, 4, 2, 1,
		/* Gesellschaft / Social */
		-2, 4, 2, -2, 3, 2, -1,
		/* Natur / Nature */
		2, 2, 2, 5, 3, 2,
		/* Handwerk / Craftsmanship */
		0, -2, 0, 0, 1, -6, 0, 2, 0,
		/* Wissen / Lore */
		2, -2, 3, 4, 3, 1, 2, 0, -2,
		/* Intuition */
		2, 3
	},
	/* Druide/ Druid*/
	{
		/* Kampf / Combat */
		1, -3, 0, -7, -6, 2, -7, 1, 1,
		/* Körper / Body */
		-2, 2, 0, -1, 3, 1, 4, -2, 0, 1,
		/* Gesellschaft / Social */
		-7, -3, 0, -2, 2, 1, 0,
		/* Natur / Nature */
		2, 2, 2, 5, 4, 3,
		/* Handwerk / Craftsmanship */
		0, -4, 1, 2, 1, -4, 0, 4, 0,
		/* Wissen / Lore */
		2, -2, -1, 3, 4, 2, 0,-2, -4,
		/* Intuition */
		2, 3
	},
	/* Magier/ Magician*/
	{
		/* Kampf / Combat */
		1, 0, 2, -4, -5, 3, -5, -3, 0,
		/* Körper / Body */
		-3, 1, 0, 1, 0, 0, 2, -1, 0, 1,
		/* Gesellschaft / Social */
		-2, 0, 1, 0, 3, 2, 1,
		/* Natur / Nature */
		-5, 0, -2, 3, -2, -4,
		/* Handwerk / Craftsmanship */
		6, 4, 0, 3, 3, -4, 6, 7, 6,
		/* Wissen / Lore */
		-2, 0, 0, 2, 2, 2, -2, 0, -2,
		/* Intuition */
		1, 3
	},
	/* Auelf/ Green Elf*/
	{
		/* Kampf / Combat */
		1, 2, 3, 3, -1, 2, -1, 4, 2,
		/* Körper / Body */
		1, 2, 2, 2, 3, 2, 2, 4, 3, -1,
		/* Gesellschaft / Social */
		-1, 3, -1, -3, 3, 0, -2,
		/* Natur / Nature */
		3, 1, 2, 2, 2, 2,
		/* Handwerk / Craftsmanship */
		-5, 2, -1, 1, 0, -5, 1, 3, 2,
		/* Wissen / Lore */
		1, 0, 0, 2, 4, 3, 4, -1, -2,
		/* Intuition */
		1, 4
	},
	/* Firnelf/ Ice Elf*/
	{
		/* Kampf / Combat */
		3, 2, 2, 2, -2, 4, -5, 2, 4,
		/* Körper / Body */
		0, 0, 2, -2, 5, 0, 3, 3, 5, 4,
		/* Gesellschaft / Social */
		-3, 2, -3, -7, 3, -2, -2,
		/* Natur / Nature */
		2, 0, 5, -2, 1, 3,
		/* Handwerk / Craftsmanship */
		-7, 2, -4, -2, -2, -5, 0, 1, 0,
		/* Wissen / Lore */
		1, 2, 0, 0, 2, 4, 3, -5, -4,
		/* Intuition */
		2, 6
	},
	/* Waldelf/ Silvan Elf*/
	{
		/* Kampf / Combat */
		2, 3, 3, 3, -1, 3, -3, 4, 2,
		/* Körper / Body */
		2, 5, 3, 0, 4, -2, 2, 4, 5, -2,
		/* Gesellschaft / Social */
		-1, 2, -2, -6, 3, -1, -2,
		/* Natur / Nature */
		4, 1, 3, 4, 2, 4,
		/* Handwerk / Craftsmanship */
		-6, 2, -2, -1, -1, -5, 0, 2, 1,
		/* Wissen / Lore */
		2, -2, 0, 3, 4, 3, 3, -3, -3,
		/* Intuition */
		2, 5
	}
};
#endif

#if 0
static const signed char spells[6][86] = {
	/* Hexer/ Warlock*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		-4, -10, -9, -7, -5,
		/* Beherrschung / Domination */
		-6, -10, -6, 1, -8, -8, -10, -15, -6, 2, -6, -12,
		/* Dämonologie / Demonology */
		-12, -7, -5, -10, 2, -15,
		/* Elemente / Elements */
		-18, -15, -10,
		/* Bewegung /Movement */
		-8, -6, -6, -5, -18, -15,
		/* Heilung / Healing */
		-8, 5, -8, -10, 3,
		/* Hellsicht / Clairvoyance */
		-4, -10, -4, 0, -1, -8, -5,
		/* Illusion */
		-7, -6, 2, 0,
		/* Kampf / Fight */
		0, -12, -8, -10, -10, -10, 2, -15, -8,
		/* Verständigung / Communication */
		5, -8,
		/* Verwandlung / Transformation */
		-7, -3, -8, 0, 2, 0, -3, 1, -3, -1, -3, -19, -15, -12, -10, -10,
		/* Veränderung / Transmutation */
		-4, -4, -10, -8, -5, -10, 0, -10, -6, -12, },
	/* Druide/ Druid*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		2, -10, -8, -9, -12,
		/* Beherrschung / Domination */
		-6, -10, 3, -6, 2, 2, -6, -4, -4, -8, -8, 1,
		/* Dämonologie / Demonology */
		-10, -3, -2, -12, -8, -12,
		/* Elemente / Elements */
		-15, -15, -12,
		/* Bewegung /Movement */
		-10, -8, -12, -5, -19, -15,
		/* Heilung / Healing */
		-8, -19, -6, -10, -8,
		/* Hellsicht / Clairvoyance */
		-4, -10, -10, -4, -3, -8, -8,
		/* Illusion */
		-7, -6, -6, -10,
		/* Kampf / Fight */
		1, -8, -3, -8, -12, -7, -19, -10, -8,
		/* Verständigung / Communication */
		-19, -6,
		/* Verwandlung / Transformation */
		-8, -8, -8, -5, -6, 0, -3, 0, -3, -3, -1, -18, -15, -15, -10, -12,
		/* Veränderung / Transmutation */
		-6, -4, -8, -8, -1, -8, 0, -8, -6, -15, },
	/* Magier/ Magician*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		-2, -6, -1, -5, -8,
		/* Beherrschung / Domination */
		-6, 0, -6, -9, -12, -10, -2, -10, -2, -10, -3, -15,
		/* Dämonologie / Demonology */
		-5, -7, -5, -8, -12, -6,
		/* Elemente / Elements */
		-18, -10, -10,
		/* Bewegung /Movement */
		-6, 0, -2, -10, -10, -15,
		/* Heilung / Healing */
		2, -19, -2, -10, -10,
		/* Hellsicht / Clairvoyance */
		-7, -4, -4, -10, 3, 0, 0,
		/* Illusion */
		-10, 0, -6, -15,
		/* Kampf / Fight */
		2, -6, -2, 0, -3, -1, -19, -3, -4,
		/* Verständigung / Communication */
		-19, -6,
		/* Verwandlung / Transformation */
		-10, -2, 0, 0, -10, 0, 0, 0, -1, 0, -1, -15, -5, -10, -15, -4,
		/* Veränderung / Transmutation */
		-2, 0, -7, 0, -3, -6, 3, -6, -6, -15, },
	/* Auelf/ Green Elf*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		0, -12, -10, -7, -10,
		/* Beherrschung / Domination */
		-4, 4, -10, -9, -10, -5, -12, -15, -6, -4, -1, -15,
		/* Dämonologie / Demonology */
		-12, -8, -6, -15, -12, -19,
		/* Elemente / Elements */
		-19, -13, -8,
		/* Bewegung /Movement */
		-2, -6, -6, -2, -15, -7,
		/* Heilung / Healing */
		5, -19, -4, -3, -2,
		/* Hellsicht / Clairvoyance */
		2, -10, 2, 1, 3, -2, 2,
		/* Illusion */
		-1, -8, -6, -15,
		/* Kampf / Fight */
		4, -10, -4, 4, -10, -7, -19, -8, -2,
		/* Verständigung / Communication */
		-19, -10,
		/* Verwandlung / Transformation */
		-6, -4, 4, -1, -8, 2, 0, 1, -1, -1, 0, -19, -15, -15, -2, 2,
		/* Veränderung / Transmutation */
		-2, 1, -10, -5, -5, -10, 5, -10, 0, -6, },
	/* Firnelf/ Ice Elf*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		-2, -16, -11, -9, -12,
		/* Beherrschung / Domination */
		-2, 0, -10, -9, -10, -7, -12, -15, -6, -4, 0, -15,
		/* Dämonologie / Demonology */
		-15, -10, -8, -19, -12, -19,
		/* Elemente / Elements */
		-19, -12, -4,
		/* Bewegung /Movement */
		-2, -8, -4, -2, -12, -2,
		/* Heilung / Healing */
		2, -19, -4, 1, -2,
		/* Hellsicht / Clairvoyance */
		1, -10, -2, 2, -1, -2, 1,
		/* Illusion */
		0, -8, -8, -15,
		/* Kampf / Fight */
		4, -12, -6, 0, -10, -7, -19, -6, -1,
		/* Verständigung / Communication */
		-19, -8,
		/* Verwandlung / Transformation */
		-5, -4, 2, -1, -12, 1, 0, 1, 0, -1, 1, -19, -15, -15, -4, 1,
		/* Veränderung / Transmutation */
		-4, 3, -10, -7, -7, -10, 6, -10, -4, -2, },
	/* Waldelf/ Silvan Elf*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		-1, -14, -10, -8, -11,
		/* Beherrschung / Domination */
		0, 2, -10, -9, -10, -5, -12, -15, -6, -2, 1, -15,
		/* Dämonologie / Demonology */
		-15, -9, -7, -19, -12, -19,
		/* Elemente / Elements */
		-19, -14, -8,
		/* Bewegung /Movement */
		2, -7, -5, -1, -15, -7,
		/* Heilung / Healing */
		4, -19, -4, -1, 0,
		/* Hellsicht / Clairvoyance */
		1, -10, 0, 3, 1, -4, -1,
		/* Illusion */
		2, -8, -7, -15,
		/* Kampf / Fight */
		5, -11, -5, 2, -10, -7, -19, -7, 0,
		/* Verständigung / Communication */
		-19, -9,
		/* Verwandlung / Transformation */
		-4, -4, 3, -1, -8, 1, 0, 2, -1, -1, 0, -19, -15, -15, -4, 1,
		/* Veränderung / Transmutation */
		0, 1, -10, -6, -6, -10, 5, -10, -2, -6, },
};
#endif

#if 0
static const Bit16s init_le[MAX_TYPES + 1] = {	0,
						30, 30, 30, 30, 30, 40,
						25, 30, 25, 25, 30, 25};
static const Bit16s init_ae[MAX_TYPES + 1] = {	0,
						0, 0, 0, 0, 0, 0,
						25, 25, 30, 25, 25, 25};
#endif

#if 0
struct minmax {
	unsigned char min;
	unsigned char max;
};

static const struct minmax height_range[13] = {	{0, 0},
						{150, 188},	{155, 170},
						{165, 205},	{156, 194},
						{167, 210},	{130, 140},
						{154, 188},	{154, 188},
						{164, 197},	{170, 204},
						{160, 194},	{170, 210}
};
#endif

#if 0
static const unsigned short weight_mod[13] = {
	0,
	120, 110, 100, 110, 100, 90,
	120, 110, 110, 120, 120, 120
};
#endif

#if 0
static const signed char mr_mod[13] = {
	0,
	2, 0, 0, 2, -2, 2,
	2, 2, 2, 3, 4, 3
};
#endif

#if 0
static const unsigned char initial_skill_incs[13] = {
	0,
	20, 20, 20, 20, 20, 20, 20, 20, 15, 20, 20, 20
};
#endif

#if 0
static const signed char initial_spell_incs[6] = {
	25, 25, 40, 20, 20, 20
};
#endif

#if 0
static const signed char initial_conv_incs[6] = {
	5, 5, 5, 0, 0, 0
};
#endif

struct struct_house_mod {
	signed char no;
	signed short spells[7], mod[7];
};

#if 0
static const struct struct_house_mod house_mod[9] = {
	{6, {0x1, 0x2, 0x3, 0x4, 0x5, 0x2a, 0x0}, {3, 1, 2, 2, 3, 1, 0}},
	{5, {0x7, 0xc, 0xe, 0x10, 0x2c, 0x0, 0x0}, {3, 4, 2, 2, 1, 0, 0}},
	{6, {0x12, 0x13, 0x15, 0x17, 0x18, 0x3b, 0x0}, {3, 2, 2, 2, 2, 1, 0}},
	{5, {0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x0, 0x0}, {3, 3, 2, 2, 2, 0, 0}},
	{5, {0x21, 0x23, 0x24, 0x3d, 0x46, 0x0, 0x0}, {3, 3, 3, 2, 1, 0, 0}},
	{7, {0x26, 0x27, 0x29, 0x28, 0x2a, 0x2b, 0x2c}, {2, 2, 1, 1, 2, 2, 2}},
	{7, {0x31, 0x33, 0x34, 0x35, 0x36, 0x39, 0x3e}, {2, 1, 2, 2, 2, 2, 1}},
	{5, {0x3c, 0x3e, 0x48, 0x49, 0x4b, 0x0, 0x0}, {3, 2, 2, 3, 2, 0, 0}},
	{7, {0x4c, 0x4e, 0x4f, 0x50, 0x52, 0x53, 0x54}, {2, 1, 2, 2, 2, 1, 2}}
};
#endif

#if 0
static const unsigned short autoskills[13][25] = {
	{0 },
	{9, 11, 2, 47, 0, 8, 16, 9, 10, 11, 43, 34, 40,
		13, 20, 24, 22, 46, 50, 15, 47, 12, 27, 23, 49},
	{7, 5, 26, 26, 0, 30, 31, 28, 29, 13, 11, 17,
		15, 10, 1, 27, 31, 30, 51, 50, 46, 41, 13, 14, 28},
	{3, 6, 0, 1, 15, 11, 4, 7, 12, 46, 30, 37, 31,
		5, 28, 8, 50, 51, 15, 2, 35, 36, 12, 46, 10},
	{2, 13, 8, 49, 0, 22, 17, 20, 13, 10, 48, 22,
		11, 21, 49, 50, 51, 17, 10, 46, 24, 18, 48, 22, 12},
	{4, 1, 0, 10, 15, 14, 18, 28, 11, 3, 8, 51,
		10, 14, 18, 35, 25, 27, 34, 40, 46, 47, 50, 20, 24},
	{1, 18, 0, 10, 25, 15, 7, 10, 25, 21, 51, 8,
		21, 32, 35, 33, 43, 46, 13, 44, 50, 18, 17, 42, 40},
	{2, 46, 45, 0, 8, 29, 32, 46, 44, 20, 24, 13,
		39, 5, 51, 50, 17, 31, 45, 20, 29, 30, 23, 44, 11},
	{2, 29, 1, 39, 0, 32, 23, 29, 30, 31, 26, 15,
		46, 51, 45, 44, 32, 13, 14, 35, 33, 41, 28, 45, 27},
	{5, 39, 2, 32, 33, 39, 32, 29, 40, 8, 35, 44,
		51, 46, 50, 36, 25, 33, 29, 38, 40, 13, 44, 23, 30},
	{7, 2, 3, 47, 31, 13, 20, 30, 29, 11, 14, 46,
		16, 0, 51, 23, 17, 47, 31, 51, 39, 40, 33, 20, 30},
	{7, 2, 31, 26, 0, 26, 31, 30, 29, 47, 51, 5,
		13, 15, 11, 10, 14, 30, 29, 46, 11, 10, 13, 20, 17},
	{1, 26, 31, 8, 2, 31, 0, 30, 26, 15, 11, 47,
		13, 10, 14, 28, 46, 15, 17, 7, 27, 11, 13, 28, 41}

};
#endif

#if 0
static unsigned short autospells[6][45] = {
	{5, 9, 15, 22, 34, 37, 47, 48,
	55, 58, 64, 42, 49, 5, 9, 15,
	22, 34, 37, 47, 48, 55, 58, 64,
	5, 9, 15, 22, 34, 37, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, },
	{1, 8, 10, 11, 13, 17, 19, 20,
	80, 24, 42, 5, 41, 1, 8, 10,
	11, 13, 17, 19, 20, 80, 24, 1,
	8, 10, 11, 13, 17, 19, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, },
	{1, 2, 3, 4, 5, 6, 7, 8,
	9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24,
	25, 26, 27, 28, 29, 30, 31, 32,
	33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, },
	{7, 33, 38, 40, 42, 44, 52, 84,
	62, 74, 75, 82, 49, 27, 7, 33,
	38, 40, 42, 44, 52, 84, 62, 74,
	75, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, },
	{32, 77, 82, 26, 85, 70, 68, 67,
	65, 45, 49, 38, 32, 77, 82, 26,
	85, 70, 68, 67, 65, 32, 77, 82,
	26, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, },
	{6, 16, 27, 30, 41, 45, 49, 57,
	76, 60, 62, 52, 38, 6, 16, 27,
	30, 41, 45, 49, 57, 76, 60, 6,
	16, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, },

};
#endif

#if 0
static const signed char head_first_male[12] = {	0, 0, 6, 12,
							18, 24,	30, 36,
							42, 48, 54, 62 };
static const signed char head_first_female[11] = {	0, 3, 9, 15,
							21, 27, 34, 37,
							46, 51, 58 };
#endif

/* DS:0x113b */
static struct struct_color pal_tmp[32];
/* DS:0x119b */
static const struct struct_color pal_genbg[32] = {
	{0x00, 0x00, 0x00},
	{0x38, 0x38, 0x38},
	{0x34, 0x34, 0x34},
	{0x30, 0x30, 0x30},
	{0x2c, 0x2c, 0x2c},
	{0x28, 0x28, 0x28},
	{0x24, 0x24, 0x24},
	{0x20, 0x20, 0x20},
	{0x1c, 0x1c, 0x1c},
	{0x18, 0x18, 0x18},
	{0x14, 0x14, 0x14},
	{0x10, 0x10, 0x10},
	{0x0c, 0x0c, 0x0c},
	{0x08, 0x08, 0x08},
	{0x3c, 0x3c, 0x3c},
	{0x3c, 0x34, 0x18},
	{0x3c, 0x34, 0x08},
	{0x3c, 0x34, 0x00},
	{0x38, 0x30, 0x00},
	{0x34, 0x2c, 0x00},
	{0x30, 0x28, 0x00},
	{0x2c, 0x24, 0x00},
	{0x28, 0x20, 0x00},
	{0x24, 0x1c, 0x00},
	{0x20, 0x18, 0x00},
	{0x1c, 0x14, 0x00},
	{0x38, 0x14, 0x0c},
	{0x34, 0x0c, 0x00},
	{0x2c, 0x08, 0x00},
	{0x24, 0x04, 0x00},
	{0x1c, 0x00, 0x00},
	{0x14, 0x00, 0x00},
};

#if 0
static const unsigned char MASK_SWITCH = 0;

static unsigned short mouse_mask[32] = {
        0x7fff, 0x9fff, 0x87ff, 0xc1ff,
        0xc07f, 0xe01f, 0xe007, 0xf00f,
        0xf01f, 0xf80f, 0xf887, 0xfdc3,
        0xffe3, 0xfff7, 0xffff, 0xffff,
        0x8000, 0x6000, 0x7800, 0x3e00,
        0x3f80, 0x1fe0, 0x1ff8, 0x0ff0,
        0x0fe0, 0x07f0, 0x0778, 0x023c,
        0x001c, 0x0008, 0x0000, 0x0000,
};
#endif

//static Bit16s MOUSE_REFRESH_FLAG = -1;

/* DS:0x135e */
static const struct mouse_action action_default[2] = {
			{ 0, 0, 319, 199, 0xfe},
			{ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff} };

//static Bit8u* DEFAULT_ACTION = (Bit8u*)&action_default;

//static Bit8u *ACTION_TABLE;

/* DS:0x127a */
static const struct mouse_action action_base[9] = {
			{ 272, 8, 304, 41, 0xfd},	/* credits */
			{ 305, 7, 319, 21, KEY_CTRL_F3},	/* change sex */
			{ 145, 13, 175, 21, KEY_CTRL_F4},	/* enter name */
			{ 271, 42, 286, 56, KEY_UP},	/* previous head */
			{ 288, 42, 303, 56, KEY_DOWN},	/* next head */
			{ 145, 178, 164, 192, KEY_LEFT},	/* previous page */
			{ 284, 178, 303, 192, KEY_RIGHT},	/* next page */
			{ 0, 0, 319, 199, 0xfe},
			{ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff} };
/* DS:0x12d4 */
static const struct mouse_action action_skills[4] = {
			{ 145, 178, 164, 192, KEY_LEFT},	/* previous page */
			{ 284, 178, 303, 192, KEY_RIGHT},	/* next page */
			{ 0, 0, 319, 199, 0xfe},
			{ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff} };
/* DS:0x12fc */
static const struct mouse_action action_spells[4] = {
			{ 16, 178, 35, 192, KEY_LEFT},	/* previous page */
			{ 284, 178, 303, 192, KEY_RIGHT},	/* next page */
			{ 0, 0, 319, 199, 0xfe},
			{ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff} };

//static Bit16s GEN_PAGE;

//static const Bit16u ro_zero = 0;

//static struct struct_hero hero;

#if 0
static unsigned short use_cda;
static unsigned short eh_installed;
static Bit8u *bg_buffer[MAX_PAGES] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static Bit32s bg_len[MAX_PAGES] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static Bit8u *typus_buffer[MAX_TYPES] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static Bit32s typus_len[MAX_TYPES] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

static const char fnames_g105de[][13] = { "GEN1.NVF",
					"GEN2.NVF",
					"GEN3.NVF",
					"GEN4.NVF",
					"GEN5.NVF",
					"GEN6.NVF",
					"GEN7.NVF",
					"GEN8.NVF",
					"GEN9.NVF",	/* 0x08 */
					"GEN10.NVF",
					"GEN11.NVF",
					"HEADS.DAT",
					"SEX.DAT",
					"TYPPIC.DAT",
					"FONT6",
					"GENTEXT",
					"DSALOGO.DAT",	/* 0x10 */
					"GENTIT.DAT",
					"ATTIC",
					"POPUP.DAT",
					"DGAUKLER.DAT",
					"DJAEGER.DAT",
					"DKRIEGER.DAT",
					"DSTREUNE.DAT",
					"DTHORWAL.DAT",	/* 0x18 */
					"DZWERG.DAT",
					"DHEXE.DAT",
					"DDRUIDE.DAT",
					"DMAGIER.DAT",
					"DAELF.DAT",
					"DFELF.DAT",
					"DWELF.DAT",
					"DMENGE.DAT",	/* 0x20 */
					"GEN.XMI",
					"FANPRO.NVF",
					"SAMPLE.AD",
					"MT32EMUL.XMI" };

//static const char* str_file_missing[] = { "FILE %s IS MISSING!" };

struct struct_chr_lookup {
	unsigned char chr, idx, width;
};

static const struct struct_chr_lookup chr_lookup[74] = {
	{0x20, 0, 6},
	{0x41, 1, 6},
	{0x42, 2, 6},
	{0x43, 3, 6},
	{0x44, 4, 6},
	{0x45, 5, 6},
	{0x46, 6, 6},
	{0x47, 7, 6},
	{0x48, 8, 6},
	{0x49, 9, 4},
	{0x4a, 10, 6},
	{0x4b, 11, 6},
	{0x4c, 12, 5},
	{0x4d, 13, 6},
	{0x4e, 14, 6},
	{0x4f, 15, 6},
	{0x50, 16, 6},
	{0x51, 17, 6},
	{0x52, 18, 6},
	{0x53, 19, 6},
	{0x54, 20, 6},
	{0x55, 21, 6},
	{0x56, 22, 6},
	{0x57, 23, 6},
	{0x58, 24, 6},
	{0x59, 25, 6},
	{0x5a, 26, 6},
	{0x8e, 27, 6},
	{0x99, 28, 6},
	{0x9a, 29, 6},
	{0x2c, 30, 3},
	{0x2e, 31, 2},
	{0x2d, 32, 6},
	{0x28, 33, 4},
	{0x29, 34, 4},
	{0x21, 35, 3},
	{0x3f, 36, 5},
	{0x30, 37, 6},
	{0x31, 38, 3},
	{0x32, 39, 6},
	{0x33, 40, 6},
	{0x34, 41, 6},
	{0x35, 42, 6},
	{0x36, 43, 6},
	{0x37, 44, 6},
	{0x38, 45, 6},
	{0x39, 46, 6},
	{0x27, 47, 3},
	{0x3a, 48, 3},
	{0x23, 49, 6},
	{0x26, 50, 5},
	{0x25, 51, 5},
	{0x3c, 53, 5},
	{0x3e, 52, 5},
	{0x2f, 54, 6},
	{0x5f, 55, 6},
	{0xc8, 56, 6},
	{0xc9, 57, 6},
	{0xca, 58, 6},
	{0xcb, 59, 5},
	{0xcc, 60, 5},
	{0xcd, 61, 4},
	{0xce, 62, 5},
	{0xcf, 63, 5},
	{0xd0, 64, 2},
	{0xd1, 65, 6},
	{0xd2, 66, 6},
	{0xd3, 67, 5},
	{0xd4, 68, 6},
	{0xd5, 69, 5},
	{0xd6, 70, 6},
	{0xd7, 71, 5},
	{0xd8, 72, 6},
	{0xd9, 73, 5},
};

//static const struct mouse_action ACTION_INPUT[2] = {
//			{ 0, 0, 319, 199, 0x1c},
//			{ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff} };

//static unsigned short bool_mode;
#if 0
static const struct mouse_action *action_page[MAX_PAGES] = {
			(struct mouse_action*)&action_base,
			(struct mouse_action*)&action_skills,
			(struct mouse_action*)&action_skills,
			(struct mouse_action*)&action_skills,
			(struct mouse_action*)&action_skills,
			(struct mouse_action*)&action_spells,
			(struct mouse_action*)&action_spells,
			(struct mouse_action*)&action_spells,
			(struct mouse_action*)&action_spells,
			(struct mouse_action*)&action_spells,
			(struct mouse_action*)&action_spells };
#endif
//static unsigned short need_refresh = 1;

struct type_bitmap {
	char t[13];
};
struct type_bitmap empty_bitmap;

/* DS:0x1cb3 */
static char version[] = "V1.05";
/* DS:0x1cb9 */
static const struct struct_color pal_attic[16] = {
	{0x00, 0x00, 0x00},
	{0x24, 0x24, 0x3c},
	{0x1c, 0x1c, 0x38},
	{0x18, 0x18, 0x34},
	{0x14, 0x14, 0x30},
	{0x0c, 0x10, 0x28},
	{0x0c, 0x0c, 0x24},
	{0x08, 0x08, 0x20},
	{0x04, 0x04, 0x1c},
	{0x00, 0x00, 0x18},
	{0x00, 0x00, 0x14},
	{0x3c, 0x30, 0x28},
	{0x34, 0x34, 0x3c},
	{0x30, 0x30, 0x3c},
	{0x28, 0x28, 0x3c},
	{0x20, 0x26, 0x3c},
};
/* DS:0x1ce9 */
static const struct struct_color pal_dsalogo[32] = {
	{0x00, 0x00, 0x00},
	{0x38, 0x38, 0x38},
	{0x34, 0x34, 0x34},
	{0x30, 0x30, 0x30},
	{0x2c, 0x2c, 0x2c},
	{0x28, 0x28, 0x28},
	{0x24, 0x24, 0x24},
	{0x20, 0x20, 0x20},
	{0x1c, 0x1c, 0x1c},
	{0x18, 0x18, 0x18},
	{0x14, 0x14, 0x14},
	{0x10, 0x10, 0x10},
	{0x0c, 0x0c, 0x0c},
	{0x08, 0x08, 0x08},
	{0x04, 0x04, 0x04},
	{0x2c, 0x00, 0x2c},
	{0x2c, 0x00, 0x2c},
	{0x2c, 0x00, 0x2c},
	{0x2c, 0x00, 0x2c},
	{0x2c, 0x00, 0x2c},
	{0x2c, 0x00, 0x2c},
	{0x3c, 0x30, 0x30},
	{0x38, 0x28, 0x28},
	{0x34, 0x18, 0x18},
	{0x30, 0x0c, 0x0c},
	{0x2c, 0x00, 0x00},
	{0x28, 0x00, 0x00},
	{0x24, 0x00, 0x00},
	{0x1c, 0x00, 0x00},
	{0x18, 0x00, 0x00},
	{0x14, 0x00, 0x00},
	{0x3c, 0x3c, 0x3c},
};
/* DS:0x1d49 */
static const struct struct_color col_white = { 0x3f, 0x3f, 0x3f };
/* DS:0x1d4c */
static const struct struct_color col_black = { 0x00, 0x00, 0x00 };
/* DS:0x1d4f */
static const struct struct_color col_popup[8] = {
	{0x00, 0x00, 0x00 },
	{0x38, 0x30, 0x28 },
	{0x38, 0x30, 0x10 },
	{0x30, 0x28, 0x0c },
	{0x2c, 0x24, 0x08 },
	{0x28, 0x20, 0x04 },
	{0x18, 0x14, 0x00 },
	{0x0b, 0x19, 0x0c }
};
/* DS:0x1d67 */
static const struct struct_color col_misc[3] = {
	{0x28, 0x00, 0x00 },
	{0x28, 0x28, 0x00 },
	{0x00, 0x00, 0x28 }
};
/* DS:0x1d70 */
static const struct struct_color pal_heads[32] = {
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x3f},
	{0x00, 0x00, 0x24},
	{0x3c, 0x30, 0x28},
	{0x3c, 0x2c, 0x24},
	{0x3c, 0x28, 0x1c},
	{0x38, 0x24, 0x18},
	{0x34, 0x20, 0x14},
	{0x30, 0x1c, 0x14},
	{0x28, 0x18, 0x10},
	{0x24, 0x14, 0x0c},
	{0x1c, 0x10, 0x0c},
	{0x18, 0x0c, 0x08},
	{0x14, 0x0c, 0x08},
	{0x38, 0x38, 0x38},
	{0x30, 0x30, 0x30},
	{0x2c, 0x2c, 0x2c},
	{0x28, 0x28, 0x28},
	{0x20, 0x20, 0x20},
	{0x1c, 0x1c, 0x1c},
	{0x14, 0x14, 0x14},
	{0x10, 0x10, 0x10},
	{0x00, 0x3f, 0x00},
	{0x00, 0x24, 0x00},
	{0x3c, 0x38, 0x00},
	{0x34, 0x2c, 0x00},
	{0x28, 0x20, 0x00},
	{0x3c, 0x18, 0x10},
	{0x30, 0x10, 0x08},
	{0x24, 0x0c, 0x04},
	{0x10, 0x08, 0x04},
	{0x3c, 0x3c, 0x3c},
};

//static unsigned short got_ch_bonus;
//static unsigned short got_mu_bonus;

//static Bit32s FLEN;
//static Bit32s FLEN_LEFT;

//signed short HANDLE_TIMBRE;

//void *snd_driver;
//void *form_xmid;
//void *snd_ptr_unkn1;
//void *state_table;


//static Bit16s param_level;

struct inc_states {
	char tries;
	char incs;
};

//static struct inc_states spell_incs[86];
//static struct inc_states skill_incs[52];
//static char attrib_changed[14];
//static RealPt type_names[MAX_TYPES];

/* the index of the last head */
//static signed char head_last;
/* the index of the first head */
//static signed char head_first;
/* the index of the current head */
//static signed char head_current;
/* the typus for the heads e.G. all elves are 10 */
//static signed char head_typus;

//static unsigned short MENU_TILES;
//static unsigned short left_border;
//static unsigned short upper_border;

//static signed short level;


//static RealPt dst_dst;

//static unsigned short dst_x1;
//static unsigned short dst_y1;
//static unsigned short dst_x2;
//static unsigned short dst_y2;
//static RealPt dst_src;
//static unsigned short unkn1;
//static unsigned short unkn2;
//static unsigned short unkn3;
//static unsigned short unkn4;

//static char *texts[300];

#if !defined(__BORLANDC__)
/* usage: output */
static inline char* get_text(Bit16s no) {
	return (char*)Real2Host(ds_readd(TEXTS + 4 * no));
}
/* usage: save pointers somewhere */
static inline RealPt get_text_real(Bit16s no) {
	return (RealPt)ds_readd(TEXTS + 4 * no);
}

#else
#define get_text(no) ((char*)Real2Host(ds_readd(TEXTS + 4 * (no))))
#define get_text_real(no) get_text(no)
#endif

//static unsigned short HAVE_MOUSE;

//static unsigned short WO_VAR;
//static unsigned short IN_KEY_ASCII;
//static unsigned short IN_KEY_EXT;

//static unsigned short *mouse_p1;
//static unsigned short *mouse_p2;

//static char MOUSE_BACKBUFFER[256];
//static Bit8u *buffer_sex_dat;
//static Bit8u *buffer_popup_nvf;

//static Bit8u *buffer_heads_dat;
//static Bit8u *buffer_text;
//static Bit8u *buffer_font6;
//static Bit16u col_index;
//static Bit16u bg_color;
//static Bit16u fg_color[6];
//static Bit16u text_x_end;
//static Bit16u text_y;
//static Bit16u text_x;

//static Bit8u *picbuf3;
//static Bit8u *picbuf2;
//static Bit8u *picbuf1;
//static Bit8u *gen_ptr6;
//static Bit8u *buffer_dmenge_dat;
//static Bit8u *gen_ptr5;
//static Bit8u *gen_ptr4;
//static char *gen_ptr3;
//static char *gen_ptr2;

//Bit8u *page_buffer;

//static Bit16s wo_var2;
//static Bit16s wo_var3;

//static const Bit16u ro_var[7] = {0, 0, 0, 0, 0, 0, 0};

//Bit8u *gen_ptr1;
//Bit8u *gen_ptr1_dis;

#if defined(__BORLANDC__)
/* A little quirk here:
 * This Segment starts at offset 0x0005 with some overlapping code from CD-Audio (5 Bytes).
 * To get the correct code we start at offset 0x0000 with some alignement code
 * to obtain correct alignment and the correct adresses from the switch jump-tables.
 */
void dummy0()
{
	asm {nop; };
}
void dummy()
{
}
#endif

/* Borlandified and identical */
void start_music(Bit16u track)
{
	if (!ds_readw(USE_CDA)) {
		if (ds_readw(MIDI_DISABLED) == 0) {
			play_midi(track);
		}
	} else {
		seg001_0465(track);
	}
}

/* Borlandified and nearly identical */
void read_soundcfg()
{
	Bit16s handle;
	Bit16u port; // This has to be unsigned

	ds_writew(USE_CDA, 0);
	ds_writew(MIDI_DISABLED, 1);

#if !defined(__BORLANDC__)
	handle = bc_open(RealMake(datseg, STR_SOUND_CFG), 0x8001);
#else
	/* This is checked in the following if statement */
	handle = bc_open((char*)&ds[STR_SOUND_CFG], 0x8001);
#endif

	if (handle != -1) {
		bc__read(handle, (Bit8u*)&port, 2);
		bc__close(handle);

#if !defined(__BORLANDC__)
		/* Small hack: enable MIDI instead of CD-Audio */
		D1_INFO("MIDI port 0x%x\n", host_readw((Bit8u*)&port));
		if (port && load_driver(RealMake(datseg, 0x1dda), 3, host_readw((Bit8u*)&port))) {
			/* disable audio-cd */
			ds_writew(USE_CDA, 0);
			return;
		}
#endif
		/* enable audio-cd, disable midi */
		ds_writew(USE_CDA, ds_writew(MIDI_DISABLED, 1));

		/* play audio-cd */
		seg001_0600();
	}
}

/* Borlandified and identical */
void init_music(unsigned long size)
{
	if (ds_writed(FORM_XMID, (Bit32s)gen_alloc(size))) {
		AIL_startup();
		ds_writew(MIDI_DISABLED, 1);
	}
}

/* Borlandified and identical */
void stop_music()
{
	AIL_shutdown(0);

	if (ds_readd(SND_PTR_UNKN1))
		bc_free((RealPt)ds_readd(SND_PTR_UNKN1));

	if (ds_readd(STATE_TABLE))
		bc_free((RealPt)ds_readd(STATE_TABLE));

	if (ds_readd(FORM_XMID))
		bc_free((RealPt)ds_readd(FORM_XMID));

	if (ds_readd(SND_DRIVER))
		bc_free((RealPt)ds_readd(SND_DRIVER));

	seg001_033b();
}

/* Borlandified and nearly identical */
RealPt load_snd_driver(RealPt fname)
{
	Bit32s size;
	RealPt norm_ptr;
	Bit32u in_ptr;
	Bit16s handle;

	if ((handle = bc_open(fname, 0x8001)) != -1) {
		size = 16500;
		ds_writed(SND_DRIVER, (Bit32s)gen_alloc(size + 0x10));
		in_ptr = ds_readd(SND_DRIVER) + 0x0f;
		in_ptr &= 0xfffffff0;

		/* The arguments of read are working, but not identical */
		bc__read(handle, (Bit8u*)Real2Host(norm_ptr = _normalize_ptr(in_ptr)), size);
		bc__close(handle);
		return norm_ptr;
	} else {
		return NULL;
	}
}

/* Borlandified and identical */
void unload_snd_driver()
{
	if (ds_readd(SND_DRIVER)) {
		bc_free((RealPt)ds_readd(SND_DRIVER));
		ds_writed(SND_DRIVER, 0);
	}
}

/* Borlandified and nearly identical */
unsigned short load_seq(Bit16u sequence_num)
{
	Bit16u patch;
	RealPt ptr;
	Bit16u si, di; // di = bank, si = patch

	if ((ds_writew(HANDLE_TIMBRE, open_datfile(35))) != 0xffff) {

		if ((ds_writew(SND_SEQUENCE, AIL_register_sequence(ds_readw(SND_DRIVER_HANDLE),
			(RealPt)ds_readd(FORM_XMID), sequence_num,
			(RealPt)ds_readd(STATE_TABLE), NULL))) != 0xffff) {

			while ((si = AIL_timbre_request(ds_readw(SND_DRIVER_HANDLE), ds_readw(SND_SEQUENCE))) != 0xffff)
			{
				di = si >> 8;

				if ((ptr = get_timbre(di, patch = (si & 0xff))) != 0) {
					/* ptr is passed differently */
					AIL_install_timbre(ds_readw(SND_DRIVER_HANDLE), di, patch, ptr);
					bc_free(ptr);
				}
			}
#if defined(__BORLANDC__)
			// PLACEHOLDER: next line should be used here instead of asm
			//bc_close(ds_readw(HANDLE_TIMBRE));
			asm {db 0x75, 0xff};
#else
			bc_close(ds_readw(HANDLE_TIMBRE));
#endif
			return 1;
		}
		bc_close(ds_readw(HANDLE_TIMBRE));
	}

	return 0;
}

/* Borlandified and identical */
unsigned short play_sequence(Bit16u sequence_num)
{
	if (load_seq(sequence_num) != 0) {
		AIL_start_sequence(ds_readw(SND_DRIVER_HANDLE), sequence_num);
		return 1;
	}

	return 0;
}

/* Borlandified and nearly identical */
RealPt get_timbre(Bit16u bank, Bit16u patch)
{
	RealPt timbre_ptr;

	bc_lseek(ds_readw(HANDLE_TIMBRE), ds_readd(GENDAT_OFFSET), SEEK_SET);

	do {
		read_datfile(ds_readw(HANDLE_TIMBRE), p_datseg + CURRENT_TIMBRE_PATCH, 6);

		if (ds_readbs(CURRENT_TIMBRE_BANK) == -1)
			return 0;

	} while ((ds_readbs(CURRENT_TIMBRE_BANK) != bank) && (ds_readbs(CURRENT_TIMBRE_PATCH) != patch));
//	Remark: In the executable the code produces an infinite loop and is:
//	} while ((ds_readbs(CURRENT_TIMBRE_BANK) != bank) || (ds_readbs(CURRENT_TIMBRE_PATCH) != patch));

	bc_lseek(ds_readw(HANDLE_TIMBRE), ds_readd(GENDAT_OFFSET) + ds_readd(CURRENT_TIMBRE_OFFSET), SEEK_SET);
	read_datfile(ds_readw(HANDLE_TIMBRE), p_datseg + CURRENT_TIMBRE_LENGTH, 2);

	timbre_ptr = gen_alloc(ds_readw(CURRENT_TIMBRE_LENGTH));

#if defined(__BORLANDC__)
	read_datfile(ds_readw(HANDLE_TIMBRE),
		Real2Host(timbre_ptr) + 2,
		host_writew(Real2Host(timbre_ptr), ds_readw(CURRENT_TIMBRE_LENGTH)) - 2);
#else
	host_writew(Real2Host(timbre_ptr), ds_readw(CURRENT_TIMBRE_LENGTH));
	read_datfile(ds_readw(HANDLE_TIMBRE),
		Real2Host(timbre_ptr) + 2,
		host_readw(Real2Host(timbre_ptr) - 2));
#endif

	return timbre_ptr;
}

/* Borlandified and identical */
unsigned short call_load_file(Bit16u index)
{
	return load_file(index);
}

/* Borlandified and nearly identical */
unsigned short load_file(Bit16u index)
{
	Bit16s handle;

	if ((handle = open_datfile(index)) != -1) {
		read_datfile(handle, Real2Host((RealPt)ds_readd(FORM_XMID)), 32767);
		bc_close(handle);
		return 1;
	}

	return 0;
}

/* Borlandified and nearly identical */
unsigned short load_driver(RealPt fname, Bit16u type, Bit16u port)
{
#if defined(__BORLANDC__)
	if (port != 0 &&
		((RealPt)ds_writed(0x3f52, (RealPt)load_snd_driver(fname))) &&
		((ds_writew(SND_DRIVER_HANDLE, AIL_register_driver((RealPt)ds_readd(0x3f52)))) != 0xffff) &&
		host_readw(Real2Host((RealPt)(ds_writed(0x3f56, (Bit32s)AIL_describe_driver(ds_readw(SND_DRIVER_HANDLE))))) + 2) == type)
	{
		if (port == 0xffff) {
			port = host_readw(Real2Host((RealPt)ds_readd(0x3f56)) + 0x0c);
		}

		if (AIL_detect_device(ds_readw(SND_DRIVER_HANDLE), port,
					host_readw(Real2Host((RealPt)ds_readd(0x3f56)) + 0x0e),
					host_readw(Real2Host((RealPt)ds_readd(0x3f56)) + 0x10),
					host_readw(Real2Host((RealPt)ds_readd(0x3f56)) + 0x12)) != 0)
		{
			AIL_init_driver(ds_readw(SND_DRIVER_HANDLE), port,
					host_readw(Real2Host((RealPt)ds_readd(0x3f56)) + 0x0e),
					host_readw(Real2Host((RealPt)ds_readd(0x3f56)) + 0x10),
					host_readw(Real2Host((RealPt)ds_readd(0x3f56)) + 0x12));
			if (type == 3) {
				ds_writed(STATE_TABLE_SIZE, AIL_state_table_size(ds_readw(SND_DRIVER_HANDLE)));
				ds_writed(STATE_TABLE, (Bit32u)gen_alloc(ds_readd(STATE_TABLE_SIZE)));
				ds_writew(TIMBRE_CACHE_SIZE, AIL_default_timbre_cache_size(ds_readw(SND_DRIVER_HANDLE)));

				if (ds_readw(TIMBRE_CACHE_SIZE) != 0) {
					ds_writed(SND_PTR_UNKN1, (Bit32u)gen_alloc(ds_readw(TIMBRE_CACHE_SIZE)));
				#if !defined(__BORLANDC__)
					AIL_define_timbre_cache(ds_readw(SND_DRIVER_HANDLE),
							(RealPt)ds_readd(SND_PTR_UNKN1),
							ds_readw(TIMBRE_CACHE_SIZE));
				#endif
				}
			}

			ds_writew(MIDI_DISABLED, 0);
			return 1;
		} else {
			#if !defined(__BORLANDC__)
			infobox((char*)(p_datseg + STR_SOUNDHW_NOT_FOUND), 0);
			ds_writew(MIDI_DISABLED, 1);
			#else
				asm {nop; } // BCC Sync-point
			#endif
			return 0;
		}
	}

	ds_writew(MIDI_DISABLED, 1);
	return 0;
#else
	CPU_Push16(port);
	CPU_Push16(type);
	CPU_Push32(fname);
	CALLBACK_RunRealFar(reloc_gen + 0x3c6, 0x3d0);
	CPU_Pop32();
	CPU_Pop16();
	CPU_Pop16();
	return reg_ax ? 1 : 0;
#endif
}

/* Borlandified and identical */
void play_midi(Bit16u index)
{
	if ((ds_readw(MIDI_DISABLED) == 0) &&
		(host_readw(Real2Host((RealPt)ds_readd(0x3f56)) + 2) == 3))
	{
		stop_sequence();
		call_load_file(index);
		play_sequence(0);
	}
}

/* Borlandified and identical */
void stop_sequence()
{
	if ((ds_readw(MIDI_DISABLED) == 0) &&
		(host_readw(Real2Host((RealPt)ds_readd(0x3f56)) + 2) == 3))
	{
		AIL_stop_sequence(ds_readw(SND_DRIVER_HANDLE), ds_readw(SND_SEQUENCE));
		AIL_release_sequence_handle(ds_readw(SND_DRIVER_HANDLE), ds_readw(SND_SEQUENCE));
	}
}

/* Borlandified and identical */
void restart_midi()
{
	if ((ds_readw(MIDI_DISABLED) == 0) &&
		(host_readw(Real2Host((RealPt)ds_readd(0x3f56)) + 2) == 3) &&
		(AIL_sequence_status(ds_readw(SND_DRIVER_HANDLE), ds_readw(SND_SEQUENCE)) == 2))
	{
		AIL_start_sequence(ds_readw(SND_DRIVER_HANDLE), ds_readw(SND_SEQUENCE));
	}
}

/**
 * mouse_action -	does mouse programming
 * @p1:		function AX
 * @p2:		depends on AX
 * @p3:		depends on AX
 * @p4:		depends on AX
 * @p5:		depends on AX
 *
 * This function differs a bit. Borlands C-Library has a special function
 * to call interrupts. We use the one of DOSBox, which means, that we
 * put the values in the emulated registers, instead in a structure.
 */
/* Borlandified and nearly identical */
void do_mouse_action(Bit8u *p1, Bit8u *p2, Bit8u *p3, Bit8u *p4, Bit8u *p5)
{
#if !defined(__BORLANDC__)
	if ((signed short)host_readw(p1) < 0)
		return;

	unsigned short ba, bb, bc, bd, be, bsi, bdi;

	/* save register content */
	ba = reg_ax;
	bb = reg_bx;
	bc = reg_cx;
	bd = reg_dx;
	be = SegValue(es);
	bsi = reg_si;
	bdi = reg_di;

	/* write paramters to registers */
	reg_ax = host_readw(p1);
	reg_bx = host_readw(p2);
	reg_cx = host_readw(p3);

	/* respect special functions */
	switch (reg_ax) {
		case 0x9:	/* define Cursor in graphic mode */
		case 0xc:	/* install event handler */
		case 0x14:	/* swap event handler */
		case 0x16:	/* save mouse state */
		case 0x17:	/* load mouse state */
			reg_dx = host_readw(p4);
			SegSet16(es, host_readw(p5));
			break;
		case 0x10:	/* define screen region for update */
			reg_cx = host_readw(p2);
			reg_dx = host_readw(p3);
			reg_si = host_readw(p4);
			reg_di = host_readw(p5);
			break;
		default:
			reg_dx = host_readw(p4);

			D1_LOG("%x %x %x %x %x\n", host_readw(p1),
				host_readw(p2), host_readw(p3),
				host_readw(p4),	host_readw(p5));

	}

	/* Call the interrupt */
	CALLBACK_RunRealInt(0x33);

	/* write the return values */
	if (reg_ax == 0x14)
		host_writew(p2, SegValue(es));
	else
		host_writew(p2, reg_bx);

	host_writew(p1, reg_ax);
	host_writew(p3, reg_cx);
	host_writew(p4, reg_dx);

	/* restore register values */
	reg_ax = ba;
	reg_bx = bb;
	reg_cx = bc;
	reg_dx = bd;
	SegSet16(es, be);
	reg_si = bsi;
	reg_di = bdi;

	return;
#else
	REGS myregs;
	SREGS mysregs;

	if (host_readws(p1) >= 0) {
		myregs.x.ax = host_readw(p1);
		myregs.x.bx = host_readw(p2);
#if !defined(__BORLANDC__)
		myregs.x.cx = host_readw(p3);
#else
		myregs.x.cx = 0xdead; asm {nop};
#endif

		switch (host_readws(p1)) {
			case 0x9:	/* define Cursor in graphic mode */
			case 0xc:	/* install event handler */
			case 0x14:	/* swap event handler */
			case 0x16:	/* save mouse state */
			case 0x17:	/* load mouse state */
				myregs.x.dx = host_readw(p4);
				mysregs.es = host_readw(p5);
				break;
			case 0x10:	/* define screen region for update */
				myregs.x.cx = host_readw(p2);
				myregs.x.dx = host_readw(p3);
				myregs.x.si = host_readw(p4);
				myregs.x.di = host_readw(p5);
				break;
			default:
				myregs.x.dx = host_readw(p4);
		}

		bc_int86x(0x33, &myregs, &myregs, &mysregs);

		host_writew(p2, ((host_readw(p1) == 0x14) ? mysregs.es : myregs.x.bx));
		host_writew(p1, myregs.x.ax);
		host_writew(p3, myregs.x.cx);
		host_writew(p4, myregs.x.dx);
	}
#endif
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void interrupt mouse_isr(void)
{
	signed short l_si = _AX;
	signed short p1;
	signed short p2;
	signed short p3;
	signed short p4;
	signed short p5;
	
	if (ds_readws(MOUSE_LOCKED) == 0) {
		if (l_si & 0x2) {
			ds_writew(MOUSE1_EVENT2, 1);
			ds_writew(MOUSE1_EVENT1, 1);
		}
		if (l_si & 0x8) {
			ds_writew(MOUSE2_EVENT, 1);
		}
		if (l_si & 0x1) {
			p1 = 3;
			p3 = ds_readws(MOUSE_POSX);
			p4 = ds_readws(MOUSE_POSY);
			
			do_mouse_action((Bit8u*)&p1, (Bit8u*)&p2, (Bit8u*)&p3, (Bit8u*)&p4, (Bit8u*)&p5);

			ds_writew(MOUSE_POSX, p3);
			ds_writew(MOUSE_POSY, p4);
			
			if (ds_readws(MOUSE_POSX) > ds_readws(MOUSE_POSX_MAX)) {
				ds_writew(MOUSE_POSX, ds_readws(MOUSE_POSX_MAX));
			}
			if (ds_readws(MOUSE_POSX) < ds_readws(MOUSE_POSX_MIN)) {
				ds_writew(MOUSE_POSX, ds_readws(MOUSE_POSX_MIN));
			}
			if (ds_readws(MOUSE_POSY) < ds_readws(MOUSE_POSY_MIN)) {
				ds_writew(MOUSE_POSY, ds_readws(MOUSE_POSY_MIN));
			}
			if (ds_readws(MOUSE_POSY) > ds_readws(MOUSE_POSY_MAX)) {
				ds_writew(MOUSE_POSY, ds_readws(MOUSE_POSY_MAX));
			}
			
			p1 = 4;
			p3 = ds_readws(MOUSE_POSX);
			p4 = ds_readws(MOUSE_POSY);
			
			do_mouse_action((Bit8u*)&p1, (Bit8u*)&p2, (Bit8u*)&p3, (Bit8u*)&p4, (Bit8u*)&p5);
			
			ds_writew(MOUSE_MOVED, 1);
		}
	}
}
#endif

/* Borlandified and identical */
void mouse_enable()
{
	Bit16u p1, p2, p3, p4, p5;

	if (ds_readw(HAVE_MOUSE) == 2) {

		/* initialize mouse */
		p1 = 0;

		do_mouse_action((Bit8u*)&p1, (Bit8u*)&p2, (Bit8u*)&p3, (Bit8u*)&p4, (Bit8u*)&p5);

		if (p1 == 0) {
			ds_writew(HAVE_MOUSE, 0);
		}

#if defined(__BORLANDC__)
		ds_writed(MOUSE_CURRENT_CURSOR, (Bit32u)(&p_datseg[MOUSE_MASK]));
		ds_writed(MOUSE_LAST_CURSOR, (Bit32u)(&p_datseg[MOUSE_MASK]));
#else
		ds_writed(MOUSE_CURRENT_CURSOR, (Bit32u)RealMake(datseg, MOUSE_MASK));
		ds_writed(MOUSE_LAST_CURSOR, (Bit32u)RealMake(datseg, MOUSE_MASK));
#endif

		if (ds_readws(HAVE_MOUSE) == 2) {

			/* move cursor  to initial position */
			p1 = 4;
			p3 = ds_readw(MOUSE_POSX);
			p4 = ds_readw(MOUSE_POSY);

			do_mouse_action((Bit8u*)&p1, (Bit8u*)&p2, (Bit8u*)&p3, (Bit8u*)&p4, (Bit8u*)&p5);
#if defined(__BORLANDC__)
			mouse_do_enable(0x1f, (RealPt)&mouse_isr);
#else
			mouse_do_enable(0x1f, RealMake(reloc_gen + 0x3c6, 0x68c));
#endif
		}
	}
}

/* Borlandified and identical */
void mouse_disable()
{
	if (ds_readw(HAVE_MOUSE) == 2) {
		mouse_do_disable();
	}
}

#if defined(__BORLANDC__)
/* Borlandified and nearly identical */
void mouse_unused1(Bit8u *p1, Bit8u *p2, Bit8u *p3, Bit8u *p4)
{
	unsigned short l_var;
	host_writew(p1, 5);
	do_mouse_action(p1, p2, p3, p4, (Bit8u*)&l_var);
}

/* Borlandified and identical */
void mouse_call_isr()
{
	mouse_isr();
}
#endif

/* Borlandified and identical */
void mouse_do_enable(Bit16u val, RealPt ptr)
{
	Bit16u p1, p2, p3, p4, p5;

	p1 = 0x0c;
	p3 = val;

#if !defined(__BORLANDC__)
	p4 = 0x86a;
	p5 = reloc_gen + 0x3c6;
#else
	p4 = (Bit16u)FP_OFF(mouse_isr);
	p5 = (Bit16u)FP_SEG(mouse_isr);
#endif

	/* save adress of old IRQ 0x78 */
	ds_writed(IRQ78_BAK, (Bit32u)bc__dos_getvect(0x78));

	/* set new IRQ 0x78 */
	bc__dos_setvect(0x78, (INTCAST)ptr);

	/* set the new mouse event handler */
	do_mouse_action((Bit8u*)&p1, (Bit8u*)&p2, (Bit8u*)&p3, (Bit8u*)&p4, (Bit8u*)&p5);

	ds_writew(MOUSE_HANDLER_INSTALLED, 1);
}

/* Borlandified and identical */
void mouse_do_disable()
{
	Bit16u v1, v2, v3, v4, v5;

	/* restore the old int 0x78 handler */
	bc__dos_setvect(0x78, (INTCAST)ds_readd(IRQ78_BAK));

	/* uninstall mouse event handler */
	v1 = 0x0c;
	v3 = 0;
	v4 = 0;
	v5 = 0;

	do_mouse_action((Bit8u*)&v1, (Bit8u*)&v2, (Bit8u*)&v3, (Bit8u*)&v4, (Bit8u*)&v5);

	ds_writew(MOUSE_HANDLER_INSTALLED, 0);
}

/**
 * mouse_move_cursor -	move the mouse cursor to a position
 * @x:	X - coordinate
 * @y:	Y - coordinate
 */
/* Borlandified and identical */
void mouse_move_cursor(unsigned short x, unsigned short y)
{
	unsigned short p1, p2, p3, p4, p5;

	p1 = 4;
	p3 = x;
	p4 = y;

	do_mouse_action((Bit8u*)&p1, (Bit8u*)&p2, (Bit8u*)&p3, (Bit8u*)&p4, (Bit8u*)&p5);
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void mouse_unused2(unsigned short a1, unsigned short a2, unsigned short a3, unsigned short a4)
{
	unsigned short p1, p2, p3, p4, p5;

	p1 = 9;
	p2 = a1;
	p3 = a2;
	p4 = a3;
	p5 = a4;

	do_mouse_action((Bit8u*)&p1, (Bit8u*)&p2, (Bit8u*)&p3, (Bit8u*)&p4, (Bit8u*)&p5);
}

/* Borlandified and identical */
void mouse_unused3(unsigned short a1)
{
	unsigned short p1, p2, p3, p4, p5;

	p1 = 0x1d;
	p2 = a1;

	do_mouse_action((Bit8u*)&p1, (Bit8u*)&p2, (Bit8u*)&p3, (Bit8u*)&p4, (Bit8u*)&p5);
}
#endif

/* Borlandified and identical */
/* static */
void update_mouse_cursor()
{
	update_mouse_cursor1();
}

/* Borlandified and identical */
/* static */
void call_mouse()
{
	mouse();
}

/* Borlandified and identical */
/* static */
void update_mouse_cursor1()
{
	if (ds_readw(MOUSE_LOCKED) == 0) {

		if (ds_readws(MOUSE_REFRESH_FLAG) == 0) {
			ds_writew(MOUSE_LOCKED, 1);
			restore_mouse_bg();
			ds_writew(MOUSE_LOCKED, 0);
		}
		ds_dec_ws(MOUSE_REFRESH_FLAG);
	}
}

/* Borlandified and identical */
/* static */
void mouse()
{
	if (ds_readw(MOUSE_LOCKED) == 0) {

		ds_inc_ws(MOUSE_REFRESH_FLAG);

		if (ds_readws(MOUSE_REFRESH_FLAG) == 0) {

			ds_writew(MOUSE_LOCKED, 1);

			if (ds_readws(MOUSE_POSX) < ds_readws(MOUSE_POINTER_OFFSETX))
				ds_writew(MOUSE_POSX, ds_readws(MOUSE_POINTER_OFFSETX));

			if (ds_readws(MOUSE_POSX) > 315)
				ds_writew(MOUSE_POSX, 315);

			if (ds_readws(MOUSE_POSY) < ds_readws(MOUSE_POINTER_OFFSETY))
				ds_writew(MOUSE_POSY, ds_readws(MOUSE_POINTER_OFFSETY));

			if (ds_readws(MOUSE_POSY) > 195)
				ds_writew(MOUSE_POSY, 195);

			save_mouse_bg();

			ds_writew(MOUSE_POSX_BAK, ds_readws(MOUSE_POSX));
			ds_writew(MOUSE_POSY_BAK, ds_readws(MOUSE_POSY));
			ds_writew(0x125a, ds_readws(MOUSE_POINTER_OFFSETX));
			ds_writew(0x125c, ds_readws(MOUSE_POINTER_OFFSETY));

			draw_mouse_cursor();

			ds_writew(MOUSE_LOCKED, 0);
		}
	}
}

/* Borlandified and identical */
/* static */
void mouse_compare()
{
	/* these pointers never differ in gen */
	if (ds_readw(MOUSE_MOVED) || ds_readd(MOUSE_LAST_CURSOR) != ds_readd(MOUSE_CURRENT_CURSOR)) {

		/* copy a pointer */
		ds_writed(MOUSE_LAST_CURSOR, ds_readd(MOUSE_CURRENT_CURSOR));
#if !defined(__BORLANDC__)
		if (RealMake(datseg, MOUSE_MASK) == (RealPt)ds_readd(MOUSE_CURRENT_CURSOR)) {
#else
		if ((RealPt)(&ds[MOUSE_MASK]) == (RealPt)ds_readd(MOUSE_CURRENT_CURSOR)) {
#endif
			ds_writew(MOUSE_POINTER_OFFSETX, ds_writew(MOUSE_POINTER_OFFSETY, 0));
		} else {
			ds_writew(MOUSE_POINTER_OFFSETX, ds_writew(MOUSE_POINTER_OFFSETY, 8));
		}
		ds_writew(MOUSE_MOVED, 0);
		update_mouse_cursor1();
#if !defined(__BORLANDC__)
		mouse();
#else
		//mouse();
		asm { nop; nop } // BCC Sync-point
#endif
	}
}

/* Borlandified and identical */
void handle_input()
{
	Bit16s si, i;

	ds_writew(IN_KEY_ASCII, ds_writew(IN_KEY_EXT, si = 0));

	if (CD_bioskey(1)) {

		si = ((Bit16s)(ds_writew(IN_KEY_ASCII, CD_bioskey(0))) >> 8);
		ds_and_ws(IN_KEY_ASCII, 0xff);

		if (si == KEY_J)
			si = KEY_Y;

		if ((ds_readw(IN_KEY_ASCII) == 0x11) && !ds_readbs(0x40b8)) {

			update_mouse_cursor();
			mouse_disable();
			stop_music();
			restore_mouse_isr();
			exit_video();
			bc_clrscr();
			exit(0);
		}
	}

	if (ds_readw(MOUSE1_EVENT2) == 0) {
		// Hm, ...
		if (ds_readw(HAVE_MOUSE) == 0);
	} else {
		ds_writew(MOUSE1_EVENT2, 0);
		si = 0;

		if ((RealPt)ds_readd(ACTION_TABLE))
			si = get_mouse_action(ds_readw(MOUSE_POSX),
				ds_readw(MOUSE_POSY),
				(Bit8u*)Real2Host(ds_readd(ACTION_TABLE)));
				
		if ((si == 0) && ((RealPt)ds_readd(DEFAULT_ACTION)))
			si = get_mouse_action(ds_readw(MOUSE_POSX),
				ds_readw(MOUSE_POSY),
				(Bit8u*)Real2Host(ds_readd(DEFAULT_ACTION)));

		if (ds_readw(HAVE_MOUSE) == 2) {
			for (i = 0; i < 15; i++)
				wait_for_vsync();

			if (ds_readw(MOUSE1_EVENT2) != 0) {
				ds_writew(WO_VAR, 1);
				ds_writew(MOUSE1_EVENT2, 0);
			}

			if (si == 0xfd) {
				si = 0;
				ds_writew(MENU_TILES, 4);
				ds_writew(FG_COLOR + 8, 1);
				infobox(get_text(267), 0);
				ds_writew(FG_COLOR + 8, 0);
				ds_writew(MENU_TILES, 3);
			}
		}
	}
	mouse_compare();
	ds_writew(IN_KEY_EXT, si);
}

/* Borlandified and nearly identical */
/* static */
Bit16u get_mouse_action(Bit16s x, Bit16s y, Bit8u *act)
{
	Bit16u i;
#if !defined(__BORLANDC__)
	struct mouse_action *ptr = (struct mouse_action*)act;

	for (i = 0; ptr[i].action != 0xffff; i++) {

		if (ptr[i].x1 > x)
			continue;
		if (ptr[i].x2 < x)
			continue;
		if (ptr[i].y1 > y)
			continue;
		if (ptr[i].y2 < y)
			continue;

		return ptr[i].action;
	}
#else
	for (i = 0; host_readws(act + 10 * i) != -1; i++) {
	
		if (host_readws(act + 10 * i + 0) > x)
			continue;
		if (host_readws(act + 10 * i + 4) < x)
			continue;
		if (host_readws(act + 10 * i + 2) > y)
			continue;
		if (host_readws(act + 10 * i + 6) < y)
			continue;

		return host_readws(act + 10 * i + 8);			
	}
#endif

	return 0;
}

#if defined(__BORLANDC__)
/* Borlandified and nearly identical */
void unused_func1(RealPt in_ptr, Bit16s x, Bit16s y, Bit8s c1, Bit8s c2)
{
	Bit8s val;
	RealPt ptr;
	Bit16s i, j;

	update_mouse_cursor();

	ptr = (RealPt)ds_readd(VGA_MEMSTART);
	ptr += 320 * y + x;

	for (i = 0; i < c2; ptr+=320 , i++) {
		for (j = 0; j < c1; j++) {
			if ((val = *((Bit16s*)(in_ptr++))) != 0) {
				host_writeb(Real2Host(ptr + j), val);
			}
		}
	}
#if !defined(__BORLANDC__)	
	call_mouse();
#else
	asm {nop;} // BCC Sync-point
#endif
}
#endif


/**
 * decomp_rle() - decompress a RLE compressed picture
 * @dst:	destination
 * @src:	source
 * @y:		y - Coordinate to start
 * @x:		x - Coordinate to start
 * @width:	width of the picture
 * @height:	height of the picture
 * @mode:	if 2 copy pixels with the value 0
 *
*/
/* Borlandified and identical */
void decomp_rle(Bit8u *dst, Bit8u *src, Bit16s x, Bit16s y,
				Bit16s width, Bit16s height, Bit16u mode)
{
	Bit16s i, j, k;
	Bit8s val;
	Bit8u n;
	Bit8s pix;
	Bit8u *dst_loc;

	dst_loc = dst;
	dst_loc += 320 * y + x;
	update_mouse_cursor();

	for (i = 0; i < height; dst_loc += 320, i++) {

		j = 0;

		while (j < width) {

			if ((val = *(src++)) == 0x7f) {
				n = *src++;
				pix = *src++;

				if ((pix != 0) || (mode != 2))
					for (k = 0; k < n; k++)
						host_writeb(dst_loc + j + k, pix);
				j += n;
			} else {
				if ((val != 0) || (mode != 2))
					host_writeb(dst_loc + j, val);
				j++;
			}
		}
	}

	call_mouse();
}

/* Borlandified and nearly identical */
/* static */
void draw_mouse_cursor()
{
	Bit8s Y, X;
	RealPt vgaptr;
	signed short *mouse_cursor;
	Bit16s rangeY;
	register Bit16s mask; //si
	register Bit16s rangeX; //di
	Bit16s diffX;
	Bit16s diffY;

	vgaptr = (RealPt)ds_readd(VGA_MEMSTART);
	mouse_cursor = (signed short*)Real2Host(ds_readd(MOUSE_CURRENT_CURSOR)) + (32 / 2);

	rangeX = ds_readw(MOUSE_POSX) - ds_readw(MOUSE_POINTER_OFFSETX);
	rangeY = ds_readw(MOUSE_POSY) - ds_readw(MOUSE_POINTER_OFFSETY);

	diffX = diffY = 16;

	if (rangeX > 304) diffX = 320 - rangeX;
	if (rangeY > 184) diffY = 200 - rangeY;

	vgaptr += rangeY * 320 + rangeX;

	for (Y = 0; Y < diffY; Y++) {
		mask = host_readw((Bit8u*)mouse_cursor++);
		for (X = 0; X < diffX; X++)
			if ((0x8000 >> X) & mask)
				mem_writeb(Real2Phys(vgaptr) + X, 0xff);
		vgaptr += 320;
	}
}

/* Borlandified and identical */
/* static */
void save_mouse_bg()
{
	RealPt vgaptr;
	Bit16s rangeX;
	Bit16s rangeY;
	Bit16s diffX;
	Bit16s diffY;
	Bit16s Y;
	Bit16s X;

	vgaptr = (RealPt)(ds_readd(VGA_MEMSTART));

	rangeX = ds_readw(MOUSE_POSX) - ds_readw(MOUSE_POINTER_OFFSETX);
	rangeY = ds_readw(MOUSE_POSY) - ds_readw(MOUSE_POINTER_OFFSETY);

	diffX = diffY = 16;

	if (rangeX > 304) diffX = 320 - rangeX;
	if (rangeY > 184) diffY = 200 - rangeY;

	vgaptr += rangeY * 320 + rangeX;

	for (Y = 0; Y < diffY; vgaptr += 320, Y++)
		for (X = 0; X < diffX; X++)
			ds_writeb(MOUSE_BACKBUFFER + 16 * Y + X, mem_readb(Real2Phys(vgaptr) + X));
}

/* Borlandified and identical */
/* static */
void restore_mouse_bg()
{
	RealPt vgaptr;
	Bit16s rangeX;
	Bit16s rangeY;
	Bit16s diffX;
	Bit16s diffY;
	Bit16s i, j;

	vgaptr = (RealPt)ds_readd(VGA_MEMSTART);

	rangeX = ds_readw(MOUSE_POSX_BAK) - ds_readw(0x125a);
	rangeY = ds_readw(MOUSE_POSY_BAK) - ds_readw(0x125c);
	diffX = diffY = 16;

	if (rangeX > 304)
		diffX = 320 - rangeX;
	if (rangeY > 184)
		diffY = 200 - rangeY;

	vgaptr += rangeY * 320 + rangeX;

	for (i = 0; i < diffY; vgaptr += 320, i++)
		for (j = 0; j < diffX; j++)
			mem_writeb(Real2Phys(vgaptr) + j, ds_readb(MOUSE_BACKBUFFER + 16 * i + j));
}

/* Borlandified and nearly identical */
void load_font_and_text()
{
	Bit16s handle;
	Bit32s len;

	handle = open_datfile(14);
	read_datfile(handle, (Bit8u*)Real2Host(ds_readd(BUFFER_FONT6)), 1000);
	bc_close(handle);

	handle = open_datfile(15);
	len = read_datfile(handle, (Bit8u*)Real2Host(ds_readd(BUFFER_TEXT)), 64000);
	bc_close(handle);

	split_textbuffer((Bit8u*)p_datseg + TEXTS, (RealPt)ds_readd(BUFFER_TEXT), len);
#if !defined(__BORLANDC__)
//	split_textbuffer_host(texts, (char*)Real2Host(ds_readd(BUFFER_TEXT)), len);
#endif
}

#if !defined(__BORLANDC__)
static void split_textbuffer_host(char **dst, char *src, Bit32u len)
{
	Bit32u i = 0;

	for (i = 0, *dst++ = src; i != len; src++, i++) {
		/* continue if not the end of the string */
		if (!*src) {

			/* return if "\0\0" (never happens) */
			if (!*(src + 1))
				return;

			/* write the adress of the next string */
			*dst++ = src + 1;
		}
	}
}
#endif

/* Borlandified and nearly identical */
void split_textbuffer(Bit8u *dst, RealPt src, Bit32u len)
{
	Bit32u i = 0;

	host_writed(dst, (Bit32u)src);
	dst += 4;

	for (; i != len; src++, i++) {
		/* continue if not the end of the string */
		if (!host_readbs(Real2Host(src))) {

			/* return if "\0\0" (never happens) */
			if (!host_readbs(Real2Host(src) + 1))
				return;

			/* write the adress of the next string */
			host_writed(dst, (Bit32u)(src + 1));
#if !defined(__BORLANDC__)
			dst += 4;
#else
			asm {nop} // Sync-point
#endif
		}
	}
}

/* Borlandified and identical */
void load_page(Bit16s page)
{
	RealPt ptr;
	Bit16s handle;

	if (page <= 10) {
		/* check if this image is in the buffer */
		//D1_INFO("%s(BG_BUFFER %d = 0x%08x)\n", __func__, page, ds_readd(BG_BUFFER + 4 * page));
		if ((RealPt)ds_readd(BG_BUFFER + 4 * page)) {
			decomp_rle(Real2Host(ds_readd(GEN_PTR1_DIS)),
					Real2Host(ds_readd(BG_BUFFER + 4 * page)),
					0, 0, 320, 200, 0);
			return;
		}

		if (ptr = gen_alloc(get_filelength(handle = open_datfile(page)))) {
			ds_writed(BG_BUFFER + 4 * page, (Bit32u)ptr);
			ds_writed(BG_LEN + 4 * page, get_filelength(handle));
			read_datfile(handle,
				Real2Host(ds_readd(BG_BUFFER + 4 * page)),
				ds_readd(BG_LEN + 4 * page));
			decomp_rle(Real2Host(ds_readd(GEN_PTR1_DIS)),
					Real2Host(ds_readd(BG_BUFFER + 4 * page)),
					0, 0, 320, 200, 0);
		} else {
			read_datfile(handle, Real2Host(ds_readd(PAGE_BUFFER)), 64000);
			decomp_rle(Real2Host(ds_readd(GEN_PTR1_DIS)),
				Real2Host(ds_readd(PAGE_BUFFER)),
				0, 0, 320, 200, 0);
		}
		bc_close(handle);
#if defined(__BORLANDC__)
		asm {db 0xeb, 0x4b} // Sync-point
#endif
	} else {
		/* this should not happen */
		handle = open_datfile(page);
		read_datfile(handle, Real2Host(ds_readd(GEN_PTR1_DIS)) - 8, 64000);
		bc_close(handle);
		decomp_pp20((RealPt)ds_readd(GEN_PTR1_DIS),
			Real2Host(ds_readd(GEN_PTR1_DIS)) - 8,
			get_filelength(handle));
	}
}

#if defined (__BORLANDC__)
/* Borlandified and nearly identical */
void read_datfile_to_buffer(Bit16s index, RealPt dst)
{
	Bit16s handle;
	handle = open_datfile(index);
	read_datfile(handle, Real2Host(dst), 64000);
	bc_close(handle);
}
#endif

/* Borlandified and identical */
void load_typus(Bit16u typus)
{
	Bit16u index;
	RealPt ptr;
	Bit16s handle;

	index = typus + 19;

	/* check if this image is in the buffer */
	//D1_INFO("%s(TYPUS_BUFFER %d = 0x%08x)\n", __func__, typus, ds_readd(TYPUS_BUFFER + 4 * typus));
	if ((RealPt)ds_readd(TYPUS_BUFFER + 4 * typus)) {
		decomp_pp20((RealPt)ds_readd(GEN_PTR5),
			Real2Host(ds_readd(TYPUS_BUFFER + 4 * typus)),
			ds_readd(TYPUS_LEN + 4 * typus));
		return;
	}

	if (ptr = gen_alloc(get_filelength(handle = open_datfile(index)))) {
		/* load the file into the typus buffer */
		ds_writed(TYPUS_BUFFER + 4 * typus, (Bit32u)ptr);
		//D1_INFO("%s(ptr = 0x%08x TYPUS_BUFFER = 0x%08x)\n", ptr, ds_readd(TYPUS_BUFFER));
		ds_writed(TYPUS_LEN + 4 * typus, get_filelength(handle));
		read_datfile(handle,
			Real2Host(ds_readd(TYPUS_BUFFER + 4 * typus)),
			ds_readd(TYPUS_LEN + 4 * typus));
		decomp_pp20((RealPt)ds_readd(GEN_PTR5),
			Real2Host(ds_readd(TYPUS_BUFFER + 4 * typus)),
			ds_readd(TYPUS_LEN + 4 * typus));
	} else {
		/* load the file direct */
		read_datfile(handle, Real2Host(ds_readd(GEN_PTR1_DIS)), 25000);
		decomp_pp20((RealPt)ds_readd(GEN_PTR5),
			Real2Host(ds_readd(GEN_PTR1_DIS)),
			get_filelength(handle));
	}
	bc_close(handle);
}

#if !defined(__BORLANDC__)
static void prepare_path(char *p)
{
	while (*p) {
#if defined (WIN32)
		if (*p == '/')
			*p = '\\';
#else
		if (*p == '\\')
			*p = '/';
#endif
		p++;
	}
}

/**
 * get_pwd() -  get the path to the current directory
 *
 * This must be freed after use.
 * WARNING: Does only work on mounted drives
 */
static char *get_pwd() {

	char *path = (char*)calloc(2048, sizeof(char));

	if (path == NULL)
		return NULL;

	Bit8u drive = DOS_GetDefaultDrive();
	localDrive *dr = dynamic_cast<localDrive*>(Drives[drive]);
	dr->GetSystemFilename((char*)path, "");
	strcat(path, "/");
	strcat(path, Drives[drive]->curdir);
	strcat(path, "/");

	return path;
}
#endif


/**
 * save_chr() - save the hero the a CHR file
 */
/* Borlandified and nearly identical */
void save_chr()
{
	Bit16s tmpw;
	Bit16s tmph;
	char filename[20];
	struct nvf_desc nvf;
	char path[80];

	Bit16s handle; //si
	Bit16s i;      //di

	/* check for typus */
	if (!ds_readbs(HERO_TYPUS)) {
		infobox(get_text(72), 0);
		return;
	}
	/* check for name */
	if (!ds_readbs(HERO_NAME + 0)) {
		infobox(get_text(154), 0);
		return;
	}

	/* Load picture from nvf */
	/* TODO: why not just copy? */
	nvf.dst = (RealPt)ds_readd(GEN_PTR1_DIS);
	nvf.src = (RealPt)ds_readd(BUFFER_HEADS_DAT);
	nvf.no = ds_readbs(HEAD_CURRENT);
	nvf.type = 0;
	nvf.width = &tmpw;
	nvf.height = &tmph;

	process_nvf(&nvf);

	/* copy picture to the character struct */
#if !defined(__BORLANDC__)
	bc_memcpy(RealMake(datseg, HERO_PIC), (RealPt)ds_readd(GEN_PTR1_DIS), 1024);
#else
	bc_memcpy(&ds[HERO_PIC], (RealPt)ds_readd(GEN_PTR1_DIS), 1024);
#endif

	/* put the hero in the first group */
	ds_writeb(HERO_GROUP, 1);

	/* wanna save ? */
	if (!gui_bool((Bit8u*)get_text(3)))
		return;
	/* copy name to alias */
	/* TODO: should use strncpy() here */
#if !defined(__BORLANDC__)
	bc_strcpy(RealMake(datseg, HERO_ALIAS), RealMake(datseg, HERO_NAME));
#else
	bc_strcpy(&ds[HERO_ALIAS], &ds[HERO_NAME]);
#endif

	/* copy name to buffer */
	/* TODO: should use strncpy() here */
#if !defined(__BORLANDC__)
	bc_strcpy((RealPt)ds_readd(GEN_PTR2), RealMake(datseg, HERO_NAME));
#else
	bc_strcpy((RealPt)ds_readd(GEN_PTR2), &ds[HERO_NAME]);
#endif

	/* prepare filename */
	for (i = 0; i < 8; i++) {
		char c;
		/* leave the loop if the string ends */
		if (!host_readbs(Real2Host((RealPt)ds_readd(GEN_PTR2) + i)))
			break;
		if (!isalnum(host_readbs(Real2Host((RealPt)ds_readd(GEN_PTR2) + i)))) {
			/* replace non alphanumerical characters with underscore */
			host_writeb(Real2Host((RealPt)ds_readd(GEN_PTR2)) + i, '_');
		}
	}


#if !defined(__BORLANDC__)

	FILE *fd;
	char *pwd;

	strncpy(filename, (char*)Real2Host(ds_readd(GEN_PTR2)), 8);
	filename[8] = 0;
	strcat(filename, ".CHR");

	pwd = get_pwd();
	strncat(pwd, filename, 12);
	prepare_path(pwd);

	/* try to open the filename */
	fd = fopen(pwd, "rb");

	/* if the file exists ask if should overwrite */
	if (fd) {
		/* Original-Bugfix: the file should be closed */
		fclose(fd);

		if (!gui_bool((Bit8u*)get_text(261)))
			return;
	}

	/* here originally creat() was used */
	fd = fopen(pwd, "wb");
	free(pwd);
	pwd = NULL;

	if (fd) {
		/* write the CHR file to the current directory */
		fwrite(p_datseg + HERO_NAME, 1, 1754, fd);
		fclose(fd);

		/* save it to the TEMP dir if called from with arguments */
		if (ds_readw(CALLED_WITH_ARGS) != 0) {
			strcpy(path, "TEMP\\");
			strcat(path, filename);

			pwd = get_pwd();
			strncat(pwd, path, 80);
			prepare_path(pwd);

			fd = fopen(pwd, "wb");
			free(pwd);
			pwd = NULL;

			if (fd) {
				fwrite(p_datseg + HERO_NAME, 1, 1754, fd);
				fclose(fd);
			}
		}
	} else {
		/* should be replaced with infobox() */
		error_msg(p_datseg + STR_SAVE_ERROR);
	}

#else
	bc_strncpy(filename, (char*)Real2Host(ds_readd(GEN_PTR2)), 8);
	filename[8] = 0;
	bc_strcat(filename, &ds[STR_CHR]);

	if (((handle = bc_open(filename, 0x8001)) == -1) || gui_bool((Bit8u*)get_text(261))) {

		handle = bc__creat(filename, 0);

		if (handle != -1) {
			bc_write(handle, &ds[HERO_NAME], 1754);
			bc_close(handle);

			if (ds_readw(CALLED_WITH_ARGS) == 0) return;

			bc_strcpy(path, &ds[STR_TEMP_DIR]);
			bc_strcat(path, filename);

			if ((handle = bc__creat(path, 0)) != -1) {
				bc_write(handle, &ds[HERO_NAME], 1754);
				bc_close(handle);
			}
		} else {
			/* should be replaced with infobox() */
			error_msg(p_datseg + STR_SAVE_ERROR);
		}
	}
#endif
}

/* Borlandified and nearly identical */
void read_common_files()
{
	Bit16s handle; //si
	Bit16s len; //di

	/* load HEADS.DAT */
	handle = open_datfile(11);
	len = read_datfile(handle, Real2Host(ds_readd(BUFFER_HEADS_DAT)), 64000);
	bc_close(handle);

	/* load POPUP.NVF */
	handle = open_datfile(19);
	len = read_datfile(handle, Real2Host(ds_readd(BUFFER_POPUP)) - 8, 500);
	bc_close(handle);
	decomp_pp20((RealPt)ds_readd(BUFFER_POPUP), Real2Host(ds_readd(BUFFER_POPUP)) - 8, len);

	/* load SEX.DAT */
	handle = open_datfile(12);
	read_datfile(handle, Real2Host(ds_readd(BUFFER_SEX_DAT)), 900);
	bc_close(handle);

	/* load DMENGE.DAT */
	handle = open_datfile(32);
	len = read_datfile(handle, Real2Host(ds_readd(BUFFER_DMENGE_DAT)) - 8, 25000);
	bc_close(handle);
	decomp_pp20((RealPt)ds_readd(BUFFER_DMENGE_DAT), Real2Host(ds_readd(BUFFER_DMENGE_DAT)) - 8, len);
}

#if !defined(__BORLANDC__)
#if 0
static inline Bit32u swap_u32(Bit32u v)
{
	return ((v >> 24) & 0xff) | ((v >> 16) & 0xff) << 8 |
		((v >> 8) & 0xff) << 16 | (v & 0xff) << 24;

}
#endif
#endif

/* Borlandified and far from identical, but works */
Bit32s process_nvf(struct nvf_desc *nvf)
{
	Bit32s offs;
	Bit16s pics;
	Bit16s height;
	Bit16s va;
	Bit32s p_size;
	Bit32s retval;
	Bit8s nvf_type;

	RealPt src;

	Bit16s i;     // si
	Bit16s width; // di
#if 0
	/* Fix: GCC warns about uninitialized values */
	width = height = 0;
	p_size = 0;
	src = NULL;
#endif

	va = (nvf_type = host_readbs(Real2Host(nvf->src))) & 0x80;
	nvf_type &= 0x7f;
	pics = host_readws(Real2Host(bc_F_PADD(nvf->src, 1L)));

	if (nvf->no < 0)
		nvf->no = 0;

	if (nvf->no > pics - 1)
		nvf->no = pics - 1;

	switch (nvf_type) {

	case 0x00:
		width = host_readws(Real2Host(bc_F_PADD(nvf->src, 3L)));
		height = host_readws(Real2Host(bc_F_PADD(nvf->src, 5L)));
		bc_memcpy(bc_F_PADD(nvf->dst, -8L), bc_F_PADD(bc_F_PADD(nvf->src, p_size * nvf->no), 7), p_size = height * width);
		break;
	case 0x01:
		offs = pics * 4 + 3L;
		for (i = 0; i < nvf->no; i++) {
			width = host_readws(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, i * 4), 3L)));
			height = host_readws(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, i * 4), 5L)));
			offs += width * height;
		}

		width = host_readws(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, nvf->no * 4), 3L)));
		height = host_readws(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, nvf->no * 4), 5L)));
		p_size = width * height;
		bc_memcpy(bc_F_PADD(nvf->dst, -8L), bc_F_PADD(nvf->src, offs), p_size);
		break;

	case 0x02:
		width = host_readws(Real2Host(bc_F_PADD(nvf->src, 3L)));
		height = host_readws(Real2Host(bc_F_PADD(nvf->src, 5L)));
		offs = ((Bit32s)(pics * 4)) + 7L;
		for (i = 0; i < nvf->no; i++) {
			/* BCC adds here in offs = offs + value */
			offs += (host_readd(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, i * 4), 7L))));
		}

		p_size = host_readd(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, nvf->no * 4), 7L)));
		bc_memcpy(bc_F_PADD(nvf->dst, -8L), bc_F_PADD(nvf->src, offs), p_size);
		break;

	case 0x03:
		offs = pics * 8 + 3L;
		for (i = 0; i < (Bit16s)nvf->no; i++) {
			/* First two lines are not neccessary */
#if !defined(__BORLANDC__)
			width = host_readws(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, i * 8), 3L)));
			height = host_readws(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, i * 8), 5L)));
#else
			// Sync-Point
			height = host_readws(Real2Host(bc_F_PADD(nvf->src, i * 8)));
			asm {nop; nop}
#endif
			/* BCC adds here in offs = offs + value */
			offs += host_readd(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, i * 8), 7L)));
		}

		// Selected picture nvf->no, and copy it to nvf->dst
		width = host_readws(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, nvf->no * 8), 3L)));
		height = host_readws(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, nvf->no * 8), 5L)));
		p_size = host_readd(Real2Host(bc_F_PADD(bc_F_PADD(nvf->src, i * 8), 7L)));
		bc_memcpy(bc_F_PADD(nvf->dst, -8L), bc_F_PADD(nvf->src, offs), p_size);
		break;
	}

	if (!nvf->type) {
		/* PP20 decompression */
		if (va != 0) {
			/* get size from unpacked picture */
			retval = host_readds(Real2Host(nvf->dst)) - 8L;
			src = bc_F_PADD(nvf->dst, -8L);
			/* BCC: uses F_PADA here */
			src += (retval - 4L);
			retval = host_readd(Real2Host(src));
			retval = swap_u32(retval) >> 8;

		} else {
			retval = width * height;
		}

		decomp_pp20(nvf->dst, Real2Host(bc_F_PADD(nvf->dst, -8L)), p_size);

	} else {
		/* No decompression, just copy */
		memmove(Real2Host(nvf->dst), Real2Host(bc_F_PADD(nvf->dst, -8L)), (Bit16s)p_size);
		retval = p_size;
	}

	*nvf->width = width;
	*nvf->height = height;

	return retval;
}

/* Borlandified and identical */
Bit16s open_datfile(Bit16u index)
{
	Bit8u buf[800];
	Bit16s handle;

	bc_flushall();

#if defined(__BORLANDC__)
	while ((handle = bc_open(&ds[STR_DSAGEN_DAT], 0x8001)) == -1)
#else
	while ((handle = bc_open(RealMake(datseg, STR_DSAGEN_DAT), 0x8001)) == -1)
#endif
	{
#if defined(__BORLANDC__)
		sprintf(Real2Host(ds_readd(GEN_PTR2)),
			&ds[STR_FILE_MISSING],
			ds_readd(FNAMES_G105de + 4 * index));
#else
		sprintf((char*)Real2Host(ds_readd(GEN_PTR2)),
			(const char*)(p_datseg + STR_FILE_MISSING),
			Real2Host(ds_readd(FNAMES_G105de + 4 * index)));
#endif
		ds_writeb(USELESS_VARIABLE, 1);
		infobox((char*)Real2Host(ds_readd(GEN_PTR2)), 0);
		ds_writeb(USELESS_VARIABLE, 0);
	}

	bc__read(handle, buf, 800);

	if ((Bit32s)(ds_writed(GENDAT_OFFSET, get_archive_offset((char*)Real2Host(ds_readd(FNAMES_G105de + 4 * index)), buf))) != -1) {
		bc_lseek(handle, ds_readd(GENDAT_OFFSET), 0);
		return handle;
	} else {
		return 0;
	}

}

/* Borlandified and nearly identical */
/* static */
Bit32s get_archive_offset(const char *name, Bit8u *table)
{
	Bit16s i;

	for (i = 0; i < 50; i++) {

		/* check the filename */
		if (!strncmp((char*)name, (char*)table + i * 16, 12)) {

			/* calculate offset and length */
			ds_writed(FLEN_LEFT, ds_writed(FLEN,
				host_readd(table + (i + 1) * 16 + 0x0c) - host_readd(table + i * 16 + 0x0c)));

			/* save length in 2 variables */

			return host_readd(table + i * 16 + 0x0c);
		}
	}

	return -1;
}

/* Borlandified and identical */
Bit16s read_datfile(Bit16u handle, Bit8u *buf, Bit16u len)
{
	if (len > ds_readd(FLEN_LEFT))
		len = (unsigned short)ds_readd(FLEN_LEFT);

	len = bc__read(handle, buf, len);

#if defined(__BORLANDC__)
	*((Bit32s*)&ds[FLEN_LEFT]) -= len;
	// return len is implicit here
#else
	ds_writed(FLEN_LEFT, ds_readd(FLEN_LEFT) - len);
	return len;
#endif
}

/* Borlandified and identical */
Bit32s get_filelength(Bit16s unused)
{
	return ds_readd(FLEN);
}

/* Borlandified and identical */
Bit16u ret_zero1()
{
	return 0;
}

/* Borlandified and identical */
void wait_for_keypress()
{
	while (CD_bioskey(1)) {
		CD_bioskey(0);
	}
}

/* Borlandified and identical */
void error_msg(Bit8u *msg)
{
	vsync_or_key(print_line((char*)msg) * 150);
}

#if defined(__BORLANDC__)
/* unused */
/* Borlandified and identical */
Bit16s get_bioskey()
{
	return CD_bioskey(0);
}
#endif

/* Borlandified and identical */
void vsync_or_key(Bit16s val)
{
	Bit16s i;

	for (i = 0; i < val; i++) {
		handle_input();
		if (ds_readw(IN_KEY_EXT) || ds_readw(MOUSE2_EVENT)) {
			ds_writew(MOUSE2_EVENT, 0);
			ds_writew(IN_KEY_EXT, KEY_RET);
			return;
		}
		wait_for_vsync();
	}
}

#if defined(__BORLANDC__)
/* unused */
/* Borlandified and identical */
void unused_func09(Bit16s reps)
{
	Bit16s i;

	for (i = 0; i < reps; i++) {
		wait_for_vsync();
	}
}
#endif

/* seems unused on available input values */
/* Borlandified and identical */
Bit32u swap_u32(Bit32u v)
{
	Bit16u l1;
	Bit16u l2;
	Bit8u *p = (Bit8u*)&l2;

	register Bit16u l_si;

	host_writed(p, v); // write v to stack and access subvalues with l1 and l2
	l_si = l2;
	l2 = swap_u16(l1);
	l1 = swap_u16(l_si);

	return host_readd(p);
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
Bit32u unused_func10(Bit32u v)
{
	Bit16u l1;
	Bit16u l2;
	Bit16u l_si;
	Bit8u *p = (Bit8u*)&l2;

	l_si = host_writed(p, v); // write v to stack and access subvalues with l1 and l2
	l2 = l1;
#if 0
	l1 = l_si;
#else
	asm {db 0x8b, 0xc2, 0x90} // Sync-point: 5 bytes, 2 instructions
#endif

	return host_readd(p);
}
#endif

/* Borlandified and identical */
void init_video()
{
#if defined(__BORLANDC__)
	struct struct_color l_white = *(struct_color*)&ds[STRUCT_COL_WHITE2];
#else
	struct struct_color l_white = *(struct_color*)(p_datseg + STRUCT_COL_WHITE2);
#endif

	/* set the video mode to 320x200 8bit */
	set_video_mode(0x13);
	set_color((Bit8u*)&l_white, 0xff);
}

/* Borlandified and identical */
void exit_video()
{
	/* restore old mode */
	set_video_mode(ds_readw(0x47dd));
	/* restore old page */
	set_video_page(ds_readw(0x47db));
}

#if defined(__BORLANDC__)
/* unused EGA hardware io functions */

/* Borlandified and identical */
void ega_unused1(Bit8u val)
{
	outportb(0x3ce, 5);
	outportb(0x3cf, val);
}

/* Borlandified and identical */
void ega_unused2(Bit8u val)
{
	outportb(0x3c4, 2);
	outportb(0x3c5, val);
}

/* Borlandified and identical */
void ega_unused3(Bit8u val)
{
	outportb(0x3ce, 0);
	outportb(0x3cf, val);
}

/* Borlandified and identical */
void ega_unused4(Bit8u val)
{
	outportb(0x3ce, 1);
	outportb(0x3cf, val);
}

/* Borlandified and identical */
void ega_unused5(Bit8u val)
{
	outportb(0x3ce, 4);
	outportb(0x3cf, val);
}

/* Borlandified and identical */
void ega_unused6(Bit8u val)
{
	outportb(0x3ce, 8);
	outportb(0x3cf, val);
}
#endif

#if defined(__BORLANDC__)
/* Borlandified and nearly identical */
void unused_func11(Bit16s x1, Bit16s x2, Bit16s y, Bit16s color)
{
	Bit16s tmp;
	Bit16s count;
	Bit16s offset;
	Bit16s width;

	Bit16s l_si = x1;
	Bit16s l_di = x2;
	width = 320;

	if (l_si > l_di) {
		tmp = l_si;
		l_si = l_di;
		l_di = tmp;
	}
	
	count = l_di - l_si + 1;
	/* not very readable, but here the length of the function fits */
	//offset = y * width + l_si;
	draw_h_line((offset = y * width + l_si), count, color);
}
#endif

/* Borlandified and nearly identical */
void draw_v_line(Bit16s x, Bit16s y1, Bit16s y2, Bit16u color)
{
	Bit16s tmp;
	Bit16s diffY;
	Bit16s offset;
	Bit16s width = 320;

	if (y1 > y2) {
		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	diffY = y2 - y1 + 1;
	/* not very readable, but here the length of the function fits */
	//offset = y1 * width + x;
	draw_h_spaced_dots((offset = y1 * width + x), diffY, color, width);
}

/* Borlandified and identical */
void do_draw_pic(Bit16u mode)
{
	Bit16s d1;
	Bit16s d2;
	Bit16s v1;
	Bit16s v2;
	Bit16s d3;
	Bit16s d4;
	Bit16s w;
	Bit16s h;
	RealPt src;
	RealPt dst;

	register Bit16s x;
	register Bit16s y;

	x = ds_readws(DST_X1);
	y = ds_readws(DST_Y1);

	d1 = ds_readws(DST_X2);
	d2 = ds_readws(DST_Y2);
	v1 = ds_readws(UNKN1);
	v2 = ds_readws(UNKN2);
	d3 = ds_readws(UNKN3);
	d4 = ds_readws(UNKN4);

	w = d1 - x + 1;
	h = d2 - y + 1;

	src = (RealPt)ds_readd(DST_SRC);
	dst = (RealPt)ds_readd(DST_DST);

	update_mouse_cursor();

	pic_copy(dst, x, y, d1, d2, v1, v2, d3, d4, w, h, src, mode);

	call_mouse();
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void unused_func12()
{
	Bit16s diffX;
	Bit16s diffY;
	Bit16s dx2;
	Bit16s dy2;
	RealPt src;
	RealPt dst;

	Bit16s x1 = ds_readws(DST_X1); // si
	Bit16s y1 = ds_readws(DST_Y1); // di

	dx2 = ds_readws(DST_X2);
	dy2 = ds_readws(DST_Y2);
	src = (RealPt)ds_readd(DST_SRC);
	dst = (RealPt)ds_readd(DST_DST);

	bc_F_PADA(dst, (Bit32s)(y1 * 320 + x1));

	diffX = dx2 - x1 + 1;
	//diffY = dy2 - y1 + 1;


	save_rect(FP_SEG(dst), FP_OFF(dst), src, diffX, (diffY = dy2 - y1 + 1));
}
#endif

/* Borlandified and identical */
void call_fill_rect_gen(RealPt ptr, Bit16u x1, Bit16u y1, Bit16u x2, Bit16u y2, Bit16u color)
{
	Bit16s width;
	Bit16s height;

	width = x2 - x1 + 1;
	height = y2 - y1 + 1;
	ptr += y1 * 320 + x1;

#if defined(__BORLANDC__)
	fill_rect(FP_SEG(ptr), FP_OFF(ptr), color, width, height);
#else
	fill_rect(RealSeg(ptr), RealOff(ptr), color, width, height);
#endif
}

/* Borlandified and identical */
void wait_for_vsync()
{
#if !defined(__BORLANDC__)
	CALLBACK_RunRealFar(reloc_gen + 0x3c6, 0x2024);
#else
	outportb(0x3d4, 0x11);
	_AL = inportb(0x3d5);
	_AH = 0;
	_BX = _AX;
	_BX &= 0xffdf;
	outportb(0x3d4, 0x11);
	outportb(0x3d5, _BL);

	do {
		_AL = inportb(0x3da);
		_AH = 0;
		_BX = _AX;
	} while (_BX & 0x8);

	do {
		_AL = inportb(0x3da);
		_AH = 0;
		_BX = _AX;
	} while (!(_BX & 0x8));

#endif
}

/* Borlandified and identical */
/* static */
void blit_smth3(RealPt ptr, Bit16s v1, Bit16s v2)
{
	Bit8u *src;
	Bit16s i, j;

#if !defined(__BORLANDC__)
	src = p_datseg + ARRAY_2;
#else
	src = &ds[ARRAY_2];
#endif

	for (i = 0; i < v1; src += 8 - v2, ptr += 320, i++)
		for (j = 0; j < v2; src++, j++)
			mem_writeb(Real2Phys(ptr) + j, *src);
}

/**
 * str_splitter() - sets the line breaks for a string
 * @s:	string
 *
 * Returns the number of lines the string needs.
 */
/* Borlandified and nearly identical */
/* static */
Bit16u str_splitter(char *s)
{
	char *tp;
	Bit16s unknown_var1;
	Bit16s lines;
	Bit16s c_width;
	Bit16s l_width;

	Bit16s last_space; //di
	Bit16s i; //si

	lines = 1;
#if defined(__BORLANDC__)
	if (!s) {
		return 0;
	}
#else
	if (s == NULL || s == (char*)MemBase)
		return 0;
#endif

	/* replace all CR and LF with spaces */
	for (tp = s; *tp; tp++) {
		if (*tp == 0x0d || *tp == 0x0a) {
			*tp = 0x20;
		}
	}

	tp = s;

	i = last_space = unknown_var1 = 0;

	for (l_width = 0; tp[i] != 0; i++) {

		get_chr_info(tp[i], &c_width);
		l_width += c_width;

		if (l_width >= ds_readws(TEXT_X_END)) {
			if (last_space != unknown_var1) {
				tp[last_space] = 0x0d;
				tp = tp + last_space;
			} else {
				tp[i] = 0x0d;
				tp = &tp[i + 1];
			}
			lines++;
			unknown_var1 = i = last_space = l_width = 0;
		}

		if (tp[i] == 0x20) {
			last_space = i;
		}

		if (tp[i] == 0x40) {
#if defined(__BORLANDC__)
			tp += i + 1;
			// Sync-Point
			asm {db 0xff, 0x46, 0xe0;}
			asm {db 0xff, 0x46, 0xe0;}
#else
			tp = &tp[i + 1];
#endif

			i = -1;
			unknown_var1 = last_space = l_width = 0;
			lines++;
		}
	}

	if (l_width >= ds_readws(TEXT_X_END)) {
		if (unknown_var1 == last_space) {
			tp[i-1] = 0;
		} else {
			tp[last_space] = 0x0d;
			lines++;
		}
	}

	return lines;
}

/* Borlandified and identical */
/* static */
Bit16u print_line(char *str)
{
	Bit16u lines = 1;

	update_mouse_cursor();

	lines = str_splitter(str);

	print_str(str, ds_readws(TEXT_X), ds_readws(TEXT_Y));

	call_mouse();

	return lines;
}

/* Borlandified and identical */
/* static */
void print_str(char *str, Bit16s x, Bit16s y)
{
	Bit16s i;
	Bit16s x_bak;
	Bit8u c;

	i = 0;

	update_mouse_cursor();

	if (ds_readw(FG_COLOR + 8) == 1) x = get_line_start_c(str, x, ds_readws(TEXT_X_END));
	x_bak = x;

	while ((c = str[i++])) {
		if ((c == 0x0d) || (c == 0x40)) {
			/* newline */
			y += 7;

			x = (ds_readw(FG_COLOR + 8) == 1) ? get_line_start_c(str + i, ds_readws(TEXT_X), ds_readws(TEXT_X_END)) : x_bak;

		} else if (c == 0x7e) {
			/* CRUFT */
			if (x < ds_readws(RO_VAR + 0)) {
				x = ds_readw(RO_VAR + 0);
				continue;
			}
			if (x < ds_readws(RO_VAR + 2)) {
				x = ds_readw(RO_VAR + 2);
				continue;
			}
			if (x < ds_readws(RO_VAR + 4)) {
				x = ds_readw(RO_VAR + 4);
				continue;
			}
			if (x < ds_readws(RO_VAR + 6)) {
				x = ds_readw(RO_VAR + 6);
				continue;
			}
			if (x < ds_readws(RO_VAR + 8)) {
				x = ds_readw(RO_VAR + 8);
				continue;
			}
			if (x < ds_readws(RO_VAR + 10)) {
				x = ds_readw(RO_VAR + 10);
				continue;
			}
			if (x < ds_readws(RO_VAR + 12)) {
				x = ds_readw(RO_VAR + 12);
				continue;
			}
		} else if ((c == 0xf0) || (c == 0xf1) || (c == 0xf2) || (c == 0xf3)) {
			/* change text color */
			ds_writew(COL_INDEX, c - 0xf0);
		} else {
			/* print normal */
			x += print_chr(c, x, y);
		}
	}

	call_mouse();
}

/* Borlandified and nearly identical */
Bit16s print_chr(unsigned char c, Bit16s x, Bit16s y)
{
	Bit16s width;
	Bit16s idx;

	idx = get_chr_info(c, &width);

	call_them_all(idx, width, x, y);
#if !defined(__BORLANDC__)
	return width;
#else
	// Sync-Point
	asm { nop; }
#endif
}

/**
 * get_chr_info() - gets font information of a character
 * @c:		the character
 * @width:	pointer to save width
 *
 * Returns the font index.
 */
/* Borlandified and identical */
Bit16s get_chr_info(unsigned char c, Bit16s *width)
{
	Bit16s i;

	for (i = 0; i != 222; i += 3) {
		/* search for the character */
		if (ds_readb(CHR_LOOKUP + i + 0) == c) {

			*width = ds_readbs(CHR_LOOKUP + i + 2) & 0xff;
			return ds_readbs(CHR_LOOKUP + i + 1) & 0xff;
		}
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		return *width = 0;
	} else {
		*width = 6;
		return 0;
	}
}

/* Borlandified and identical */
/* static */
void call_them_all(Bit16s v1, Bit16s v2, Bit16s x, Bit16s y)
{
	RealPt gfx_ptr;
	Bit16s l2;
	Bit32s bogus;

	fill_smth();
#if !defined(__BORLANDC__)
	fill_smth2((Bit8u*)Real2Host(ds_readd(BUFFER_FONT6)) + v1 * 8);
#else
	// BCC Sync-point
	fill_smth2((Bit8u*)Real2Host(ds_readd(BUFFER_FONT6)) + v1);
	asm { nop; db 0x8c, 0x5e, 0xfe; }
#endif

	gfx_ptr = get_gfx_ptr(x, y, &l2);
	bogus = (Bit32s)ret_zero(v2, l2);

	call_blit_smth3(gfx_ptr, 7, (Bit16s)bogus, l2, v2);
}

/* Borlandified and identical */
/* static */
void fill_smth()
{
	RealPt ptr;
	Bit16s i, j;

	if (ds_readb(MASK_SWITCH) != 0)
#if !defined(__BORLANDC__)
		ptr = RealMake(datseg, ARRAY_1);
	else
		ptr = RealMake(datseg, ARRAY_2);
#else
		ptr = &ds[ARRAY_1];
	else
		ptr = &ds[ARRAY_2];
#endif

	for (i = 0; i < 8; ptr += 8, i++)
		for (j = 0; j < 8; j++)
			host_writeb(Real2Host(ptr) + j, (unsigned char)ds_readws(BG_COLOR));
}

/* Borlandified and identical */
/* static */
void fill_smth2(Bit8u* sptr) {

	RealPt ptr;
	Bit16s i, j;
	Bit8u mask;

	if (ds_readb(MASK_SWITCH) != 0)
#if !defined(__BORLANDC__)
		ptr = RealMake(datseg, ARRAY_1);
	else
		ptr = RealMake(datseg, ARRAY_2);
#else
		ptr = &ds[ARRAY_1];
	else
		ptr = &ds[ARRAY_2];
#endif

	for (i = 0; i < 8; ptr += 8, i++) {
		mask = *sptr++;
		for (j = 0; j < 8; j++) {
			if ((0x80 >> j) & mask) {
				host_writeb(Real2Host(ptr) + j,
					(unsigned char)ds_readws(FG_COLOR + 2 * ds_readw(COL_INDEX)));
			}
		}
	}
}

/* Borlandified and identical */
/* static */
RealPt get_gfx_ptr(Bit16s x, Bit16s y, Bit16s* unused)
{
	RealPt start;
	return start = (RealPt)ds_readd(GFX_PTR) + (y * 320 + x);
}

/* Borlandified and identical */
/* static */
Bit16s ret_zero(Bit16s unused1, Bit16s unused2)
{
	return 0;
}

/* Borlandified and identical */
/* static */
void call_blit_smth3(RealPt dst, Bit16s v1, Bit16s v2, Bit16s v3, Bit16s v4)
{
	blit_smth3(dst, v1, v4);
}

/* Borlandified and identical */
/* static */
void set_textcolor(Bit16s fg, Bit16s bg)
{
	ds_writew(FG_COLOR + 0, fg);
	ds_writew(BG_COLOR, bg);
}

/* Borlandified and identical */
/* static */
void get_textcolor(Bit16s *p_fg, Bit16s *p_bg)
{
	host_writew((Bit8u*)p_fg, ds_readw(FG_COLOR + 0));
	host_writew((Bit8u*)p_bg, ds_readw(BG_COLOR));
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
/* static */
Bit16s count_linebreaks(Bit8u *ptr)
{
	Bit16s i = 0;
	
	while (*ptr) {
		if (*ptr++ == 0x0d) {
			i++;
		}
	}

	return i;
}
#endif

/* Borlandified and identical */
Bit16s get_str_width(char *str)
{
	Bit16s sum = 0;
	Bit16s width;

	while (*str) {
		get_chr_info(*str++, &width);
		sum += width;
	}

	return sum;
}

/**
 * get_line_start_c() - calculates the start positon for a centered line
 * @str:	the string
 * @x:		start position of the string
 * @x_max:	end position of the string
 *
 * Returns the X coordinate where the strin must start.
 */
/* Borlandified and identical */
Bit16s get_line_start_c(char *str, Bit16s x, Bit16s x_max)
{
	Bit16s width;

	register Bit16s pos_x;	// si
	register Bit16s val;	// di
	
	for (pos_x = 0; ((val = *str) && (val != 0x40) && (val != 0x0d)); )
	{
		get_chr_info(*str++, &width);
		pos_x += width;
	}

	x_max -= pos_x;
	x_max >>= 1;
	x_max += x;
	// readable: xmax = (x_max - pos_x) / 2 + x;
	return x_max;
}

/* Borlandified and nearly identical */
Bit16s enter_string(char *dst, Bit16s x, Bit16s y, Bit16s num, Bit16s zero)
{
	Bit16s pos;
	Bit16s c;
	Bit16s width;

	Bit16s di;
	register Bit16s si;

	update_mouse_cursor();
	di = x;
	pos = 0;

	if (zero == 0) {
		for (si = 0; si < num; si++) {
			print_chr(0x20, di, y);
			print_chr(0x5f, di, y);
			di += 6;
		}
		di = x;
	} else {
		print_chr(0x20, di, y);
		print_chr(0x5f, di, y);
	}
	wait_for_keypress();
	ds_writew(MOUSE1_EVENT1, 0);

	c = 0;
	while ((c != 0xd) || (pos == 0)) {
		do {
			do {} while (!CD_bioskey(1) && ds_readw(MOUSE1_EVENT1) == 0);

			if (ds_readw(MOUSE1_EVENT1)) {
				ds_writew(IN_KEY_ASCII, 0x0d);
				ds_writew(MOUSE1_EVENT1, ds_writew(MOUSE1_EVENT2, 0));
			} else {
				ds_writew(IN_KEY_EXT, (ds_writews(IN_KEY_ASCII, CD_bioskey(0))) >> 8);
				ds_and_ws(IN_KEY_ASCII, 0xff);
			}
		} while ((ds_readw(IN_KEY_EXT) == 0) && (ds_readw(IN_KEY_ASCII) == 0));

		c = ds_readw(IN_KEY_ASCII);

		if (c == 0xd)
			continue;

		if (ds_readw(IN_KEY_EXT) == KEY_ESC) {
			*dst = 0;
			call_mouse();
			ds_writew(IN_KEY_EXT, 0);
			return 1;
		}

		if (c == 8) {
			if (pos <= 0)
				continue;

			if (zero == 1 && pos != num)
				print_chr(0x20, di, y);
			pos--;
			dst--;
			get_chr_info(*dst, &width);

			di -= (zero != 0) ? width : 6;

			print_chr(0x20, di, y);
			print_chr(0x5f, di, y);
		} else {
			if (!(ds_readbs(0x1ff9 + c) & 0x0e) &&
				(((Bit8u)c) != 0x84) && (((Bit8u)c) != 0x94) &&
				(((Bit8u)c) != 0x81) && (((Bit8u)c) != 0x8e) &&
				(((Bit8u)c) != 0x99) && (((Bit8u)c) != 0x9a) &&
				(c != 0x20) && (c != 0x2e))
					continue;

			/* is_alpha(c) */
			if (ds_readb(0x1ff9 + c) & 0xc)
				c = toupper(c);

			/* ae */
			if ((Bit8u)c == 0x84)
				c = (signed short)0xff8e;
			/* oe */
			if ((Bit8u)c == 0x94)
				c = (signed short)0xff99;
			/* ue */
			if ((Bit8u)c == 0x81)
				c = (signed short)0xff9a;

			/* are we at the end of the input field */
			if (pos == num) {
#if !defined(__BORLANDC__)
				dst--;
#else
				asm {nop;} // BCC Sync-point
#endif
				get_chr_info(*dst, &width);

				di -= (zero != 0) ? width : 6;

				pos--;
			}

			*dst++ = (Bit8u)c;
			print_chr(0x20, di, y);
			print_chr((Bit8u)c, di, y);
			get_chr_info((Bit8u)c, &width);

			di += (zero != 0) ? width : 6;

			pos++;

			if ((zero == 1) && (pos != num)) {
				print_chr(0x20, di, y);
				print_chr(0x5f, di, y);
			}
		}
	}

	/* OK from here */
	if (zero == 0) {
		while (pos < num) {
			print_chr(0x20, di, y);
			di += 6;
			pos++;
		}
	}

	*dst = 0;
	call_mouse();

	return 0;
}

/* Borlandified and nearly identical */
void draw_popup_line(Bit16s line, Bit16s type)
{
	RealPt dst;
	RealPt src;
	Bit16s i;
	Bit16s popup_right;

	register Bit16s popup_left;   // si
	register Bit16s popup_middle; // di

	/* This is a bit bogus */
	dst = (RealPt)ds_readd(VGA_MEMSTART);

	/* (line * 8 + y) * 320  + x */
	dst = ((RealPt)ds_readd(VGA_MEMSTART)) + 320 * (ds_readws(UPPER_BORDER) + 8 * line) +  ds_readw(LEFT_BORDER);

	switch (type) {
		case 0: {
			popup_left = 0;
			popup_middle = 0x380;
			popup_right = 0x80;
			break;
		}
		case 1: {
			popup_left = 0x100;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		}
		case 2: {
			popup_left = 0x200;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		}
		case 3: {
			popup_left = 0x280;
			popup_middle = 0x580;
			popup_right = 0x300;
			break;
		}
	}
#if defined(__BORLANDC__)
	// BCC Sync-Point
	copy_to_screen(Real2Phys(src = ((RealPt)ds_readd(BUFFER_POPUP)) + popup_left), Real2Phys(dst), 16, 8, 0);
#else
	src = ((RealPt)ds_readd(BUFFER_POPUP)) + popup_left;
	copy_to_screen(Real2Phys(src), Real2Phys(dst), 16, 8, 0);
#endif

	src = ((RealPt)ds_readd(BUFFER_POPUP)) + popup_middle;
	dst += 16;
	for (i = 0; i < ds_readws(MENU_TILES); dst += 32, i++)
		copy_to_screen(Real2Phys(src), Real2Phys(dst), 32, 8, 0);
#if defined(__BORLANDC__)
	// BCC Sync-Point
	copy_to_screen(Real2Phys(src = ((RealPt)ds_readd(BUFFER_POPUP)) + popup_right), Real2Phys(dst), 16, 8, 0);
#else
	src = ((RealPt)ds_readd(BUFFER_POPUP)) + popup_right;
	copy_to_screen(Real2Phys(src), Real2Phys(dst), 16, 8, 0);
#endif
}

/**
 *	infobox() - draws and info- or enter_numberbox
 *	@msg:		the message for the box
 *	@enter_num:	number of digits to enter
 *
 *	if @digits is zero the function just delays.
 */
/* Borlandified and nearly identical */
Bit16s infobox(char *msg, Bit16s digits)
{
	RealPt src;
	RealPt dst;
	Bit16s retval;
	Bit16s fg;
	Bit16s bg;
	Bit16s v2;
	Bit16s v3;
	Bit16s v4;
	Bit16s i;

	Bit16s lines; // si
	Bit16s di;    // di

	retval = 0;
	ds_writew(FG_COLOR + 8, 1);
	v2 = ds_readws(TEXT_X);
	v3 = ds_readws(TEXT_Y);
	v4 = ds_readws(TEXT_X_END);

	di = 32 * ds_readws(MENU_TILES) + 32;
	ds_writew(TEXT_X, ds_writew(LEFT_BORDER, (320 - di) / 2 + ds_readw(0x1327)) + 5);
	ds_writews(TEXT_X_END, di - 10);
	lines = str_splitter(msg);

#if !defined(__BORLANDC__)
	if (digits != 0)
		lines += 2;
#else
	asm { db 0x6a, 0x00, 0x6a, 0x00, 0x6a, 0x00; nop;}
	// BCC Sync-Point
#endif

	ds_writew(UPPER_BORDER, (200 - (lines + 2) * 8) / 2);
	ds_add_ws(UPPER_BORDER, ds_readws(RO_ZERO));
	ds_writew(TEXT_Y, ds_readws(UPPER_BORDER) + 7);

	update_mouse_cursor();

	src = (RealPt)ds_readd(VGA_MEMSTART);
	src += ds_readws(UPPER_BORDER) * 320 + ds_readws(LEFT_BORDER);
	dst = (RealPt)ds_readd(GEN_PTR1_DIS);

	copy_to_screen(Real2Phys(src), Real2Phys(dst), di, (lines + 2) * 8, 2);

	/* draw the popup box */
	draw_popup_line(0, 0);

	for (i = 0; i < lines; i++)
		draw_popup_line(i + 1, 1);

	draw_popup_line(lines + 1, 3);

	get_textcolor((Bit16s*)&fg, (Bit16s*)&bg);
	set_textcolor(0xff, 0xdf); // WHITE ON GREEN

	print_line(msg);

	ds_writew(MOUSE2_EVENT, 0);
	call_mouse();

	if (digits) {
		enter_string((char*)Real2Host((RealPt)ds_readd(GEN_PTR3)),
			ds_readws(LEFT_BORDER) + (di - digits * 6) / 2,
			ds_readws(UPPER_BORDER) + 8 * lines - 2, digits, 0);

		retval = (Bit16u)atol((char*)Real2Host((RealPt)ds_readd(GEN_PTR3)));
	} else {
#if !defined(__BORLANDC__)
		ds_writed(ACTION_TABLE,  RealMake(datseg, ACTION_INPUT));
#else
		ds_writed(ACTION_TABLE, (Bit32u)&ds[ACTION_INPUT]);
#endif
		vsync_or_key(150 * lines);
		ds_writed(ACTION_TABLE, (Bit32u)((RealPt)0));
	}

	set_textcolor(fg, bg);
	update_mouse_cursor();

	dst = (RealPt)ds_readd(VGA_MEMSTART);
	dst += ds_readws(UPPER_BORDER) * 320 + ds_readws(LEFT_BORDER);
	src = (RealPt)ds_readd(GEN_PTR1_DIS);

	copy_to_screen(Real2Phys(src), Real2Phys(dst), di, (lines + 2) * 8, 0);
	call_mouse();

#if !defined(__BORLANDC__)
	ds_writew(TEXT_X, v2);
#else
	asm { db 0x6a, 0x00, 0x6a, 0x00;}
	// BCC Sync-Point
#endif
	ds_writew(TEXT_Y, v3);
	ds_writew(TEXT_X_END, v4);

	ds_writew(FG_COLOR + 8, 0);
	ds_writew(IN_KEY_EXT, 0);

	return retval;
}


/**
 * gui_bool() - displays a yes - no radio box
 * @header:	the header of menu
 *
 */
/* Borlandified and identical */
Bit16s gui_bool(Bit8u *msg)
{
	Bit16s retval;

	ds_writew(BOOL_MODE, 1);
	retval = gui_radio(msg, 2, get_text(4), get_text(5));
	ds_writew(BOOL_MODE, 0);

	if (retval == 1)
		return 1;
	else
		return 0;
}

/**
 * fill_radio_button() - marks the active radio button
 * @old_pos:	the position of the last active button (or -1)
 * @new_pos:	the position of the current active button
 * @offset:	the offset of the first radio line
 *
 */
/* Borlandified and identical */
void fill_radio_button(Bit16s old_pos, Bit16s new_pos, Bit16s offset)
{
	Bit16s y;

	Bit16s i;
	Bit16s x;

	update_mouse_cursor();

	/* unmark the old radio button, if any */
	if (old_pos != -1) {
		y = ds_readws(LEFT_BORDER) + 6;

		x = ds_readws(UPPER_BORDER) + (offset + old_pos) * 8 + 2;

		for (i = 0; i < 4; i++)
			draw_v_line(y + i, x, x + 3, 0xd8);
	}

	/* mark the new radio button */
	y = ds_readws(LEFT_BORDER) + 6;

	x = ds_readws(UPPER_BORDER) + (offset + new_pos) * 8 + 2;

	for (i = 0; i < 4; i++)
		draw_v_line(y + i, x, x + 3, 0xd9);

	call_mouse();
}

/**
 * gui_radio() - displays a radio menu
 * @header:	the header of the menu
 * @options:	the number of options
 *
 */
/* Borlandified and nearly identical */
Bit16s gui_radio(Bit8u *header, Bit8s options, ...)
{
	va_list arguments;
	char *str;
	Bit16s r3;
	Bit16s r4;
	Bit16s r5;
	Bit16s retval;
	Bit16s lines_sum;
	Bit16s lines_header;
	Bit16s r6;
	Bit16s fg_bak;
	Bit16s bg_bak;
	Bit16s bak1;
	Bit16s bak2;
	Bit16s bak3;
	RealPt src;
	RealPt dst;
	Bit16s mx_bak;
	Bit16s my_bak;
	Bit16s r7;
	Bit16s r8;
	Bit16s r9;

	register Bit16s di;
	register Bit16s i;

	r5 = 0;
	r6 = -1;
#if !defined(__BORLANDC__)
	di = 1;
#else
	asm {nop;}
#endif
	bak1 = ds_readws(TEXT_X);
	bak2 = ds_readw(TEXT_Y);
	bak3 = ds_readws(TEXT_X_END);
	r9 = 32 * ds_readws(MENU_TILES) + 32;
	ds_writew(TEXT_X, ds_writew(LEFT_BORDER, ((320 - r9) / 2) + ds_readw(0x1327)) + 5);
	ds_writew(TEXT_X_END, 32 * ds_readws(MENU_TILES) + 22);
	lines_header = str_splitter((char*)header);
	lines_sum = lines_header + options;
	ds_writew(TEXT_Y, ds_writew(UPPER_BORDER, (200 - (lines_sum + 2) * 8) / 2) + 7);
	update_mouse_cursor();

	/* save old background */
	src = (RealPt)ds_readd(VGA_MEMSTART);
	src += ds_readws(UPPER_BORDER) * 320 + ds_readws(LEFT_BORDER);
	dst = (RealPt)ds_readd(GEN_PTR1_DIS);
	copy_to_screen(Real2Phys(src), Real2Phys(dst), r9, (lines_sum + 2) * 8, 2);

	/* draw popup */
	draw_popup_line(0, 0);
	for (i = 0; i < lines_header; i++)
		draw_popup_line(i + 1, 1);
	for (i = 0; options > i; i++)
		draw_popup_line(lines_header + i + 1, 2);
	draw_popup_line(lines_sum + 1, 3);

	/* save and set text colors */
	get_textcolor((Bit16s*)&fg_bak, (Bit16s*)&bg_bak);
	set_textcolor(0xff, 0xdf); // WHITE ON GREEN

	/* print header */
	if (lines_header)
		print_line((char*)header);

	r3 = ds_readw(TEXT_X) + 8;
	r4 = ds_readws(UPPER_BORDER) + 8 * (lines_header + 1);

	/* print radio options */
	va_start(arguments, options);
#if !defined(__BORLANDC__)
	for (i = 1; i <= options; r4 += 8, i++) {
#else
	for (i = 1; i <= options; i++) { // BCC Sync-Point
#endif
		str = va_arg(arguments, char*);
		print_str(str, r3, r4);
	}
	va_end(arguments);

	/* save and set mouse position */
	mx_bak = ds_readw(MOUSE_POSX);
	my_bak = ds_readw(MOUSE_POSY);
	ds_writew(MOUSE_POSX_BAK, ds_writew(MOUSE_POSX, ds_readws(LEFT_BORDER) + 90));
	ds_writew(MOUSE_POSY_BAK, ds_writew(MOUSE_POSY, r8 = r7 = ds_readws(UPPER_BORDER) + 8 * (lines_header + 1)));
	mouse_move_cursor(ds_readw(MOUSE_POSX), r8);

	ds_writew(MOUSE_POSX_MAX, ds_readws(LEFT_BORDER) + r9 - 16);
	ds_writew(MOUSE_POSX_MIN, ds_readws(LEFT_BORDER));
	ds_writew(MOUSE_POSY_MIN, ds_readws(UPPER_BORDER) + 8 * (lines_header + 1));
	ds_writew(MOUSE_POSY_MAX, (ds_readws(UPPER_BORDER) + (8 * options + (lines_header + 1) * 8)) - 1);
	call_mouse();
	ds_writew(MOUSE2_EVENT, 0);

#if defined(__BORLANDC__)
	asm {nop;} // BCC Sync-Point
#endif

	while (r5 == 0) {
#if !defined(__BORLANDC__)
		ds_writed(ACTION_TABLE,  RealMake(datseg, ACTION_INPUT));
#else
		ds_writed(ACTION_TABLE, (Bit32u)&ds[ACTION_INPUT]);
#endif
		handle_input();
		ds_writed(ACTION_TABLE, (Bit32u)((RealPt)0));

		if (r6 != di) {
			fill_radio_button(r6, di, lines_header);
			r6 = di;
		}
		if (ds_readw(MOUSE2_EVENT) != 0 ||
			ds_readw(IN_KEY_EXT) == KEY_ESC ||
			ds_readw(IN_KEY_EXT) == KEY_PGDOWN) {
			/* has the selection been canceled */
			retval = -1;
			r5 = 1;
			ds_writew(MOUSE2_EVENT, 0);
		}
		if (ds_readw(IN_KEY_EXT) == KEY_RET) {
			/* has the return key been pressed */
			retval = di;
			r5 = 1;
		}
		if (ds_readw(IN_KEY_EXT) == KEY_UP) {
			/* has the up key been pressed */
			if (di == 1)
				di = options;
			else
				di--;
		}
		if (ds_readw(IN_KEY_EXT) == KEY_DOWN) {
			/* has the down key been pressed */
			if (di == options)
				di = 1;
			else
				di++;
		}
		if (ds_readw(MOUSE_POSY) != r8) {
			/* has the mouse been moved */
			di = ((r8 = ds_readw(MOUSE_POSY)) - r7) / 8 + 1;
		}
		/* is this a bool radiobox ? */
		if (ds_readw(BOOL_MODE)) {
			if (ds_readw(IN_KEY_EXT) == KEY_Y) {
				/* has the 'j' key been pressed */
				retval = 1;
				r5 = 1;
			} else if (ds_readw(IN_KEY_EXT) == KEY_N) {
				/* has the 'n' key been pressed */
				retval = 2;
				r5 = 1;
			}
		}
	}

	update_mouse_cursor();

	ds_writew(MOUSE_POSX_BAK, ds_writew(MOUSE_POSX, mx_bak));
#if !defined(__BORLANDC__)
	ds_writew(MOUSE_POSY_BAK, ds_writew(MOUSE_POSY, my_bak));
#else
	asm { nop; } // BCC Sync-Point
#endif

	ds_writew(MOUSE_POSX_MAX, 319);
	ds_writew(MOUSE_POSX_MIN, 0);
	ds_writew(MOUSE_POSY_MIN, 0);
	ds_writew(MOUSE_POSY_MAX, 199);

	mouse_move_cursor(ds_readws(MOUSE_POSX_BAK), ds_readws(MOUSE_POSY_BAK));

	dst = (RealPt)ds_readd(VGA_MEMSTART);
	dst += ds_readws(UPPER_BORDER) * 320 + ds_readws(LEFT_BORDER);
	src = (RealPt)ds_readd(GEN_PTR1_DIS);
	copy_to_screen(Real2Phys(src), Real2Phys(dst), r9, (lines_sum + 2) * 8, 0);
	call_mouse();
	set_textcolor(fg_bak, bg_bak);

	ds_writew(TEXT_X, bak1);
	ds_writew(TEXT_Y, bak2);
	ds_writew(TEXT_X_END, bak3);
	ds_writew(IN_KEY_EXT, 0);

	return retval;
}

/**
 * enter_name() - enter the name of a hero
 */
/* Borlandified and identical */
void enter_name()
{
	RealPt dst;

	dst = (RealPt)ds_readd(VGA_MEMSTART) + 12 * 320 + 176;

	update_mouse_cursor();
	copy_to_screen(Real2Phys((RealPt)ds_readd(PICBUF1)), Real2Phys(dst), 94, 8, 0);
#if !defined(__BORLANDC__)
	enter_string((char*)p_datseg + HERO_NAME, 180, 12, 15, 1);
#else
	enter_string(&ds[HERO_NAME], 180, 12, 15, 1);
#endif
	copy_to_screen(Real2Phys((RealPt)ds_readd(PICBUF1)), Real2Phys(dst), 94, 8, 0);
	call_mouse();
#if !defined(__BORLANDC__)
	print_str((char*)p_datseg + HERO_NAME, 180, 12);
#else
	print_str(&ds[HERO_NAME], 180, 12);
#endif

}

/* Borlandified and identical */
void change_head()
{
	struct nvf_desc nvf;
	Bit16s width;
	Bit16s height;

	nvf.dst = (RealPt)ds_readd(GEN_PTR6);
	nvf.src = (RealPt)ds_readd(BUFFER_HEADS_DAT);
	nvf.no = ds_readbs(HEAD_CURRENT);
	nvf.type = 0;
	nvf.width = &width;
	nvf.height = &height;

	process_nvf(&nvf);

	ds_writed(DST_SRC, ds_readd(GEN_PTR6));

	ds_writew(DST_X1, 272);
	ds_writew(DST_X2, 303);

	if (ds_readws(GEN_PAGE) == 0) {
		ds_writew(DST_Y1, 8);
		ds_writew(DST_Y2, 39);
		do_draw_pic(0);
	} else if (ds_readws(GEN_PAGE) > 4) {
		ds_writew(DST_Y1, 4);
		ds_writew(DST_Y2, 35);
		do_draw_pic(0);
	}
}

/**
 * change_sex() - changes the sex of the hero
 *
 */
/* Borlandified and identical */
void change_sex()
{
	RealPt dst;
	RealPt src;

	/* change sex of the hero */
	ds_xor_bs(HERO_SEX, 1);

	/* hero has a typus */
	if (ds_readb(HERO_TYPUS)) {
		if (ds_readbs(HERO_SEX) != 0) {
			/* To female */
			ds_writeb(HEAD_FIRST, ds_writeb(HEAD_CURRENT, ds_readb(HEAD_FIRST_FEMALE + ds_readbs(HEAD_TYPUS))));
			ds_writeb(HEAD_LAST, (Bit8s)(ds_readbs(HEAD_FIRST_MALE + ds_readbs(HEAD_TYPUS) + 1) - 1));
		} else {
			/* To male */
			ds_writeb(HEAD_FIRST, ds_writeb(HEAD_CURRENT, ds_readb(HEAD_FIRST_MALE + ds_readbs(HEAD_TYPUS))));
			ds_writeb(HEAD_LAST, (Bit8s)(ds_readbs(HEAD_FIRST_FEMALE + ds_readbs(HEAD_TYPUS)) - 1));
		}
		ds_writew(SCREEN_VAR, 1);
		return;
	} else {
		dst = (RealPt)ds_readd(VGA_MEMSTART) + 7 * 320 + 305;
		src = (RealPt)ds_readd(BUFFER_SEX_DAT) + 256 * ds_readbs(HERO_SEX);
		update_mouse_cursor();
		copy_to_screen(Real2Phys(src), Real2Phys(dst), 16, 16, 0);
		call_mouse();
	}
}

/* Borlandified and identical */
void do_gen()
{
	Bit16s si;
	Bit16s di;

	di = 0;

	ds_writew(SCREEN_VAR, 1);

	/* try to set the level from parameters */
	ds_writew(LEVEL, ((ds_readws(PARAM_LEVEL) == 'a') ? 2 :
			  ((ds_readws(PARAM_LEVEL) == 'n') ? 1 : -1)));

	/* ask for level */
	while (ds_readws(LEVEL) == -1) {
		ds_writew(LEVEL, gui_radio((Bit8u*)get_text(0), 2, get_text(1), get_text(2)));
	}

	ds_writew(MOUSE2_EVENT, 1);

	/* main loop */
	while (!di) {
		if (ds_readw(SCREEN_VAR)) {
			refresh_screen();
			ds_writew(SCREEN_VAR, 0);
		}

		ds_writed(ACTION_TABLE,  (Bit32u)ds_readd(ACTION_PAGE + 4 * ds_readws(GEN_PAGE)));
		handle_input();
		ds_writed(ACTION_TABLE, (Bit32u)(RealPt)0);

		if (ds_readw(MOUSE2_EVENT) || ds_readw(IN_KEY_EXT) == KEY_PGUP) {
			/* print the menu for each page */
			switch (ds_readws(GEN_PAGE)) {
				case 0: {
					si = gui_radio((Bit8u*)get_text(7), 9,
						get_text(10), get_text(11), get_text(15),
						get_text(8),  get_text(14), get_text(12),
						get_text(262),get_text(9),  get_text(258));

					if (si != -1) {
						if ((si >= 4) && (si < 6) &&
							(ds_readbs(HERO_ATT0_NORMAL)) &&
							!gui_bool((Bit8u*)get_text(13))) {
							si = 0;
						}
						ds_writew(IN_KEY_EXT, 0);
						switch (si) {
							case 1: {
								enter_name();
								break;
							}
							case 2: {
								change_sex();
								break;
							}
							case 3: {
								change_attribs();
								break;
							}
							case 4: {
								//memset(&hero, 0, sizeof(hero));
#if !defined(__BORLANDC__)
								bc_memset(RealMake(datseg, HERO_NAME), 0, 0x6da);
#else
								bc_memset(&ds[HERO_NAME], 0, 0x6da);
#endif
								clear_hero();
								ds_writew(MOUSE2_EVENT,	1);
								ds_writew(SCREEN_VAR, 1);
								break;
							}
							case 5: {
								new_values();
								break;
							}
							case 6: {
								select_typus();
								break;
							}
							case 7: {
								choose_typus();
								break;
							}
							case 8: {
								save_chr();
								break;
							}
							case 9: {
								if (gui_bool((Bit8u*)get_text(259)))
									di = 1;
								break;
							}
						}
					}
					break;
				}
				case 1:
				case 2:
				case 3: {
					select_skill();
					break;
				}
				case 4: {
					choose_atpa();
					break;
				}
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10: {
					select_spell();
					break;
				}
			}
		}

		if (ds_readw(IN_KEY_EXT) == KEY_CTRL_F3)
			change_sex();

		if (ds_readw(IN_KEY_EXT) == KEY_CTRL_F4)
			enter_name();

		if ((ds_readw(IN_KEY_EXT) == KEY_UP) && (ds_readws(GEN_PAGE) == 0)) {
			if (!ds_readbs(HERO_TYPUS)) {
				infobox(get_text(17), 0);
			} else {
				if (ds_readbs(HEAD_CURRENT) < ds_readbs(HEAD_LAST)) {
					ds_inc_bs_post(HEAD_CURRENT);
				} else {
					ds_writeb(HEAD_CURRENT, ds_readb(HEAD_FIRST));
				}
				change_head();
			}
		}

		if ((ds_readw(IN_KEY_EXT) == KEY_DOWN) && (ds_readws(GEN_PAGE) == 0)) {
			if (!ds_readbs(HERO_TYPUS)) {
				infobox(get_text(17), 0);
			} else {
				if (ds_readbs(HEAD_CURRENT) > ds_readbs(HEAD_FIRST)) {
					ds_dec_bs_post(HEAD_CURRENT);
				} else {
					ds_writeb(HEAD_CURRENT, ds_readb(HEAD_LAST));
				}
				change_head();
			}
		}

		if ((ds_readw(IN_KEY_EXT) == KEY_RIGHT) && (ds_readw(LEVEL) != 1)) {
			if (!ds_readbs(HERO_TYPUS)) {
				infobox(get_text(72), 0);
			} else {
				ds_writew(SCREEN_VAR, 1);

				if (((ds_readbs(HERO_TYPUS) < 7) ? 4 : 10) > ds_readws(GEN_PAGE)) {
					ds_inc_ws(GEN_PAGE);
				} else {
					ds_writew(GEN_PAGE, 0);
				}
			}
		}

		if (ds_readw(IN_KEY_EXT) == KEY_LEFT) {
			if (ds_readws(GEN_PAGE) > 0) {
				ds_writew(SCREEN_VAR, 1);
				ds_dec_ws(GEN_PAGE);
			} else {
				if (ds_readws(LEVEL) != 1) {

					if (!ds_readbs(HERO_TYPUS)) {
						infobox(get_text(72), 0);
					} else {
						ds_writew(SCREEN_VAR, 1);
						ds_writew(GEN_PAGE, ds_readbs(HERO_TYPUS) < 7 ? 4 : 10);
					}
				}
			}
		}

		if ((ds_readws(IN_KEY_EXT) >= KEY_1) && (ds_readws(IN_KEY_EXT) <= KEY_5) &&
			(ds_readws(LEVEL) == 2) && ds_readbs(HERO_TYPUS)) {

			si = ((ds_readws(IN_KEY_EXT) == KEY_1) ? 0 : (
				(ds_readws(IN_KEY_EXT) == KEY_2) ? 1 : (
				(ds_readws(IN_KEY_EXT) == KEY_3) ? 4 : (
				(ds_readws(IN_KEY_EXT) == KEY_4) ? 5 : 10))));

			if ((si != ds_readws(GEN_PAGE)) && (si < 5 || ds_readbs(HERO_TYPUS) >= 7)) {
				ds_writews(GEN_PAGE, si);
				ds_writew(SCREEN_VAR, 1);
			}
		}
	}
}

/* Borlandified and nearly identical */
void refresh_screen()
{
	RealPt src;
	RealPt dst;
	Bit16s width;
	Bit16s height;
	struct nvf_desc nvf;

	if (ds_readw(SCREEN_VAR)) {
		ds_writed(GFX_PTR, ds_readd(GEN_PTR1_DIS));
		load_page(ds_readws(GEN_PAGE));
		save_picbuf();

		/* page with base values and hero is not male */
		if ((ds_readws(GEN_PAGE) == 0) && (ds_readbs(HERO_SEX) != 0)) {

			dst = (RealPt)ds_readd(GEN_PTR1_DIS) + 7 * 320 + 305;
			src = (RealPt)ds_readd(BUFFER_SEX_DAT) + 256 * ds_readbs(HERO_SEX);
			copy_to_screen(Real2Phys(src), Real2Phys(dst), 16, 16, 0);
		}

		/* page with base values and level is advanced */
		if ((ds_readws(GEN_PAGE) == 0) && (ds_readws(LEVEL) == 1)) {
			dst = (RealPt)ds_readd(GEN_PTR1_DIS) + 178 * 320 + 284;
#if !defined(__BORLANDC__)
			src = (RealPt)ds_readd(BUFFER_SEX_DAT) + 512;
#else
			src = (RealPt)ds_readd(BUFFER_SEX_DAT); // BCC Sync-Point
#endif

			copy_to_screen(Real2Phys(src), Real2Phys(dst), 20, 15, 0);
		}
		/* if the page is lower than 5 */
		if (ds_readws(GEN_PAGE) < 5) {
			/* draw DMENGE.DAT or the typus name */
#if !defined(__BORLANDC__)
			dst = (RealPt)ds_readd(GEN_PTR1_DIS) + 8 * 320 + 16;
#else
			dst = (RealPt)ds_readd(GEN_PTR1_DIS); // BCC Sync-Point
#endif
			if (ds_readbs(HERO_TYPUS) != 0) {

				ds_writeb(NEED_REFRESH, 1);
				copy_to_screen(Real2Phys((RealPt)ds_readd(GEN_PTR5)), Real2Phys(dst), 128, 184, 0);

				if (ds_readbs(HERO_SEX) != 0) {
					print_str(get_text(271 + ds_readbs(HERO_TYPUS)),
						get_line_start_c(get_text(271 + ds_readbs(HERO_TYPUS)), 16, 128),
						184);
				} else {
					print_str(get_text(17 + ds_readbs(HERO_TYPUS)),
						get_line_start_c(get_text(17 + ds_readbs(HERO_TYPUS)), 16, 128),
						184);
				}

			} else {
				if (ds_readb(NEED_REFRESH)) {
					call_fill_rect_gen((RealPt)ds_readd(VGA_MEMSTART), 16, 8, 143, 191, 0);
#if !defined(__BORLANDC__)
					ds_writeb(NEED_REFRESH, 0);
#else
				asm { nop; } // BCC Sync-Point
#endif
				}

				wait_for_vsync();
				set_palette(Real2Host((RealPt)ds_readd(BUFFER_DMENGE_DAT)) + 128 * 184 + 2, 0 , 32);
				copy_to_screen(Real2Phys((RealPt)ds_readd(BUFFER_DMENGE_DAT)), Real2Phys(dst), 128, 184, 0);
			}
		}
		/* if hero has a typus */
		if (ds_readbs(HERO_TYPUS) != 0) {
			/* draw the head */

			nvf.dst = (RealPt)ds_readd(GEN_PTR6);
			nvf.src = (RealPt)ds_readd(BUFFER_HEADS_DAT);
			nvf.no = ds_readbs(HEAD_CURRENT);
;
			nvf.type = 0;
			nvf.width = &width;
			nvf.height = &height;
			process_nvf(&nvf);

			ds_writed(DST_SRC, ds_readd(GEN_PTR6));
			ds_writew(DST_X1, 272);
			ds_writew(DST_X2, 303);
			ds_writed(DST_DST, ds_readd(GEN_PTR1_DIS));

			/* draw the head */
			if (ds_readws(GEN_PAGE) == 0) {
				/* on the base page */
				ds_writew(DST_Y1, 8);
				ds_writew(DST_Y2, 39);
				do_draw_pic(0);
			} else if (ds_readws(GEN_PAGE) > 4) {
				/* on the spell pages */
				ds_writew(DST_Y1, 4);
				ds_writew(DST_Y2, 35);
				do_draw_pic(0);
			}

			ds_writed(DST_DST, ds_readd(VGA_MEMSTART));

		}

		print_values();
		dst = (RealPt)(ds_writed(GFX_PTR, ds_readd(VGA_MEMSTART)));
		src = (RealPt)ds_readd(GEN_PTR1_DIS);
		update_mouse_cursor();
		copy_to_screen(Real2Phys(src), Real2Phys(dst), 320, 200, 0);
		call_mouse();
	} else {
		print_values();
	}
}

/* Borlandified and identical */
/* static */
void clear_hero()
{

	Bit16s i;

	ds_writew(GOT_MU_BONUS, ds_writew(GOT_CH_BONUS, 0));

	ds_writeb(HEAD_TYPUS,
		ds_writeb(HEAD_FIRST,
		ds_writeb(HEAD_LAST,
		ds_writeb(HEAD_CURRENT, 0))));

	for (i = 0; i < 14; i++)
		ds_writeb(ATTRIB_CHANGED + i, 0);

	for (i = 0; i < 86; i++) {
		ds_writeb(SPELL_INCS + 2 * i + 0,
			ds_writeb(SPELL_INCS + 2 * i + 1, 0) );
	}
	for (i = 0; i < 52; i++) {
		// tries + incs
		ds_writeb(SKILL_INCS + 2 * i + 0,
			ds_writeb(SKILL_INCS + 2 * i + 1, 0));
	}

	ds_writeb(HERO_LEVEL, 1);
}

/**
 * new_values() - roll out new attribute values
 *
 */
/* Borlandified and nearly identical */
void new_values()
{
	/* Original-Bugfix:	there once was a char[11],
				which could not hold a char[16] */
	RealPt ds_ptr;
	Bit8s randval;
	Bit8s unset_attribs;
	Bit8s values[8];
	Bit8s sex_bak;
#if !defined(__BORLANDC__)
	char name_bak[17];
#else
	char name_bak[10];
#endif
	Bit16s j;
	Bit16s i;

	Bit16s di;

	/* set variable if hero has a typus */
	if (ds_readbs(HERO_TYPUS))
		ds_writew(SCREEN_VAR, 1);

	/* save the name of the hero */
	/* TODO strncpy() would be better here */

	strcpy(name_bak, (char*)p_datseg + HERO_NAME);

	/* save the sex of the hero */
	sex_bak = ds_readbs(HERO_SEX);

	/* clear the hero */
#if !defined(__BORLANDC__)
	bc_memset(RealMake(datseg, HERO_NAME), 0, 0x6da);
#else
	bc_memset(&ds[HERO_NAME], 0, 0x6da);
#endif

	clear_hero();

	ds_writeb(HERO_SEX, sex_bak);

	/* restore the name of the hero */
	/* TODO strncpy() would be better here */

	strcpy((char*)p_datseg + HERO_NAME, name_bak);

	refresh_screen();

	ds_writew(SCREEN_VAR, 0);

#if !defined(__BORLANDC__)
	ds_ptr = RealMake(datseg, HERO_ATT0_NORMAL);
#else
	ds_ptr = (RealPt)&ds[HERO_ATT0_NORMAL];
#endif

	for (j = 0; j < 7; j++) {
		randval = (Bit8s)random_interval_gen(8, 13);
		unset_attribs = 0;

		for (i = 0; i < 7; i++) {
			// NORMAL
			if (!host_readbs(Real2Host(ds_ptr) + 3 * i)) {
                                // not initialized attribute
				values[unset_attribs] = (signed char)i;
				ds_writed(TYPE_NAMES + 4 * unset_attribs,  (Bit32u)get_text_real(32 + i));
				unset_attribs++;
			}
		}

		sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(46), randval);

		do {
			ds_writew(0x1327, 0xffb0);

			di = gui_radio((Bit8u*)Real2Host(ds_readd(GEN_PTR2)),
				unset_attribs,
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 0)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 1)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 2)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 3)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 4)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 5)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 6)));

			ds_writew(0x1327, 0);

		} while (di == -1);

		di = values[di - 1];
		/* write randval to the selected positive attribute */
		host_writeb(3 * di + (Real2Host(ds_ptr) + 1), randval); // CURRENT
		host_writeb(3 * di + (Real2Host(ds_ptr) + 0), randval); // NORMAL

#if !defined(__BORLANDC__)
		update_mouse_cursor();
#else
		asm {db 0x03, 0xd8; } // BCC Sync-Point
#endif
		refresh_screen();
		call_mouse();
	}
#if !defined(__BORLANDC__)
	ds_ptr = RealMake(datseg, HERO_ATT0_NORMAL + 3 * 7);
#else
	ds_ptr = (RealPt)&ds[HERO_ATT0_NORMAL + 3 * 7];
#endif
	for (j = 0; j < 7; j++) {
		randval = (Bit8s)random_interval_gen(2, 7);
		unset_attribs = 0;

		for (i = 0; i < 7; i++) {
			// NORMAL
			if (!host_readbs(Real2Host(ds_ptr) + 3 * i)) {
				values[unset_attribs] = (signed char)i;
				ds_writed(TYPE_NAMES + 4 * unset_attribs,  (Bit32u)get_text_real(39 + i));
				unset_attribs++;
			}
		}

		sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(46), randval);

		do {
			ds_writew(0x1327, 0xffb0);

			di = gui_radio((Bit8u*)Real2Host(ds_readd(GEN_PTR2)),
				unset_attribs,
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 0)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 1)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 2)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 3)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 4)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 5)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 6)));

			ds_writew(0x1327, 0);

		} while (di == -1);

		di = values[di - 1];

		/* write randval to the selected negative attribute */
		host_writeb(3 * di + (Real2Host(ds_ptr) + 1), randval); // CURRENT
		host_writeb(3 * di + (Real2Host(ds_ptr) + 0), randval); // NORMAL

#if !defined(__BORLANDC__)
		update_mouse_cursor();
#else
		asm {db 0x03, 0xd8; } // BCC Sync-Point
#endif

		refresh_screen();
		call_mouse();
	}
}

/**
 * calc_at_pa() - calculate AT and PA values
 */
/* Borlandified and identical */
/* static */
void calc_at_pa()
{
	div_t res; // BCC <STDLIB.H>
	Bit16s tmp;
	Bit16s i;

	res = div(ds_readbs(HERO_ATT_IN_NORMAL) + ds_readbs(HERO_ATT_KK_NORMAL) + ds_readbs(HERO_ATT_GE_NORMAL), 5);
	/* round up if neccessary */
	if (res.rem >= 3) {
		res.quot++;
	}

	ds_writeb(HERO_ATPA_BASE, res.quot);

	for (i = 0; i < 7; i++) {
		/* Set base AT/PA value for each weapon */
		ds_writeb(HERO_AT_WEAPON + i, ds_writeb(HERO_PA_WEAPON + i, ds_readbs(HERO_ATPA_BASE)));

		if (ds_readbs(HERO_SKILLS + i) < 0) {
			tmp = __abs__(ds_readbs(HERO_SKILLS + i)) / 2;

			/* Calculate weapon AT value */
			ds_writeb(HERO_AT_WEAPON + i, ds_readbs(HERO_AT_WEAPON + i) - tmp);

			/* Calculate weapon PA value */
			ds_writeb(HERO_PA_WEAPON + i, ds_readbs(HERO_PA_WEAPON + i) - tmp);

			if (__abs__(ds_readbs(HERO_SKILLS + i)) != 2 * tmp) {
				ds_dec_bs_post(HERO_PA_WEAPON + i);
			}
		} else {
			/* calculate ATPA for positive weapon skill */
			tmp = ds_readbs(HERO_SKILLS + i) / 2;

			/* Calculate weapon AT value */
			ds_writeb(HERO_AT_WEAPON + i, ds_readbs(HERO_AT_WEAPON + i) + tmp);

			/* Calculate weapon PA value */
			ds_writeb(HERO_PA_WEAPON + i, ds_readbs(HERO_PA_WEAPON + i) + tmp);

			if (ds_readbs(HERO_SKILLS + i) != 2 * tmp) {
				ds_inc_bs_post(HERO_AT_WEAPON + i);
			}
		}
	}
}

/**
 * fill_values() - fills the values if typus is chosen
 *
 */
 /* Borlandified and nearly identical */
void fill_values()
{
	Bit16s i;
	Bit16s v1;
	Bit16s v2;
	Bit8u *ptr;

	Bit16s si, di;

	/* fill skill values */
#if !defined(__BORLANDC__)
	for (i = 0; i < 52; i++) {
#else
	for (i = 0; i < 52; ) { // BCC Sync-Point
#endif
		//ds_writebs(HERO_SKILLS + i, skills[ds_readbs(HERO_TYPUS)][i]);
		ds_writebs(HERO_SKILLS + i, ds_readbs(SKILLS + 52 * ds_readbs(HERO_TYPUS) + i));

		/* set skill_incs and skill_tries to zero */
		ds_writeb(SKILL_INCS + 0 + 2 * i, ds_writebs((SKILL_INCS + 1) + (2 * i), 0));
	}

	/* set skill_attempts */
	//ds_writeb(HERO_SKILL_INCS, initial_skill_incs[ds_readbs(HERO_TYPUS) - 1]);
	ds_writeb(HERO_SKILL_INCS, ds_readbs(INITIAL_SKILL_INCS + ds_readbs(HERO_TYPUS)));

	/* do magic user init */
	if (ds_readbs(HERO_TYPUS) >= 7) {
		/* fill initial spell values */
#if !defined(__BORLANDC__)
		for (i = 0; i < 86; i++) {
#else
		for (i = 0; i < 86; ) { // BCC Sync-Point
#endif
			//ds_writebs(HERO_SPELLS + i, spells[ds_readbs(HERO_TYPUS) - 7][i]);
			ds_writebs(HERO_SPELLS + i, ds_readbs(SPELLS + 86 * (ds_readbs(HERO_TYPUS) - 7) + i));

			/* set spell_incs and spell_tries to zero */
			// tries, incs
			ds_writeb(SPELL_INCS + 2 * i + 0, ds_writeb(SPELL_INCS + 2 * i + 1, 0));
		}

		/* special mage values */
		if (ds_readbs(HERO_TYPUS) == 9) {
			/* set staff spell to level 1 */
			ds_writeb(HERO_STAFF_LEVEL, 1);
			/* select mage school */
			do {
				 ds_writebs(HERO_SPELL_SCHOOL,
						gui_radio((Bit8u*)get_text(47), 9,
							get_text(48), get_text(49),
							get_text(50), get_text(51),
							get_text(52), get_text(53),
							get_text(54), get_text(55),
							get_text(56)) - 1);
			} while (ds_readbs(HERO_SPELL_SCHOOL) == -2);


			/* add magic school modifications */
			//for (i = 0; house_mod[ds_readbs(HERO_SPELL_SCHOOL)].no > i; i++) {
			for (i = 0; ds_readbs(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL)) > i; i++) {
				// Some docu
				//Bit16s spell, mod;
				//spell = house_mod[ds_readbs(HERO_SPELL_SCHOOL)].spells[i];
				//spell = ds_readws(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL) + 1 + 2 * i);
				//mod = house_mod[ds_readbs(HERO_SPELL_SCHOOL)].mod[i];
				//mod = ds_readws(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL) + 15 + 2 * i);
				//ds_add_bs(HERO_SPELLS + spell, mod);

				ds_add_bs(HERO_SPELLS + ds_readws(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL) + 1 + 2 * i),
					ds_readws(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL) + 15 + 2 * i));
			}
		}

		/* set spell attempts */
		ds_writeb(HERO_SPELL_INCS, ds_readbs(INITIAL_SPELL_INCS + ds_readbs(HERO_TYPUS) - 7));

		/* get convertable increase attempts */
		//di = ds_readbs(INITIAL_CONV_INCS + ds_readbs(HERO_TYPUS) - 7);

		if ((di = ds_readbs(INITIAL_CONV_INCS + ds_readbs(HERO_TYPUS) - 7)) && (ds_readws(LEVEL) == 2) && gui_bool((Bit8u*)get_text(269))) {
			/* create string */
			sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(270), di);

			i = infobox((char*)Real2Host(ds_readd(GEN_PTR2)), 1);

			if (i > 0) {
				/* spell attempts to skill attempts */
				if (i > di)
					i = di;
				di -= i;
				/* change spell attempts */
				ds_sub_bs(HERO_SPELL_INCS, i);
				/* change skill attempts */
				ds_add_bs(HERO_SKILL_INCS, i);
			} else {

				/* create string */
				sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(271), di);

				i = infobox((char*)Real2Host(ds_readd(GEN_PTR2)), 1);
				if (i > 0) {
					if (i > di)
						i = di;
					/* change spell attempts */
					ds_add_bs(HERO_SPELL_INCS, i);
					/* change skill attempts */
					ds_sub_bs(HERO_SKILL_INCS, i);
				}
			}
		}
	}

	/* set LE */
	ds_writew(HERO_LE, ds_writews(HERO_LE_MAX, ds_readws(INIT_LE + 2 * ds_readbs(HERO_TYPUS))));

	/* set AE */
	ds_writew(HERO_AE, ds_writews(HERO_AE_MAX, ds_readws(INIT_AE + 2 * ds_readbs(HERO_TYPUS))));


	/* wanna change 10 spell_attempts against 1W6+2 AE ? */
	if ((ds_readbs(HERO_TYPUS) == 9) && (ds_readws(LEVEL) == 2) && gui_bool((Bit8u*)get_text(268))) {
		/* change spell_attempts */
		ds_sub_bs(HERO_SPELL_INCS, 10);
		ds_add_ws(HERO_AE_MAX, random_interval_gen(3, 8));
#if !defined(__BORLANDC__)
		ds_writew(HERO_AE, ds_readws(HERO_AE_MAX)); // BCC Sync-Point
#endif
	}

	/* roll out size */
	ds_writeb(HERO_HEIGHT,
		(unsigned char)random_interval_gen(ds_readb(HEIGHT_RANGE + 2 * ds_readbs(HERO_TYPUS)),
		ds_readb(HEIGHT_RANGE + 1 + 2 * ds_readbs(HERO_TYPUS))));

	/* calculate weight i = (height - weight_mod) * 40 */
	ds_writew(HERO_WEIGHT, (ds_readb(HERO_HEIGHT) - ds_readb(WEIGHT_MOD + ds_readbs(HERO_TYPUS))) * 40);

	/* roll out the money */
	i = random_gen(20);
	ptr = Real2Host(ds_readd(MONEY_TAB + ds_readbs(HERO_TYPUS) * 4));
	for (si = 0; host_readws(ptr + si * 6) < i; si++);

	ds_writed(HERO_MONEY, (Bit32s)(10 * (Bit16s)random_interval_gen(host_readw(ptr + si * 6 + 2),
				host_readw(ptr + si * 6 + 4))));

	/* calculate MR  = (KL + SI + Level) / 3 - 2 * AG */
	ds_writeb(HERO_MR,
		(ds_readbs(HERO_ATT0_NORMAL + 3 * 1) + ds_readbs(HERO_ATT0_NORMAL + 3 * 0) + ds_readbs(HERO_LEVEL)) / 3
		 - 2 * ds_readbs(HERO_ATT0_NORMAL + 3 * 7));
	/* add typus MR Modificator */
	ds_add_bs(HERO_MR, ds_readbs(MR_MOD + ds_readbs(HERO_TYPUS)));

	/* roll out god */
	ds_writeb(HERO_GOD, random_gen(12));

	/* add gods boni */
	switch (ds_readbs(HERO_GOD)) {
		case 1 : {
			/* Praios: MU + 1 */
			ds_writebs(HERO_ATT0_CURRENT + 3 * 0,
				ds_writebs(HERO_ATT0_NORMAL + 3 * 0,
					ds_readbs(HERO_ATT0_NORMAL + 3 * 0) + 1));
			ds_writew(GOT_MU_BONUS, 1);
			break;
		}
		case 2 : {
			/* Rondra: skill swords + 1 */
			ds_inc_bs_post(HERO_SKILLS + 3);
			break;
		}
		case 3 : {
			/* Efferd: skill swim + 1 */
			ds_inc_bs_post(HERO_SKILLS + 14);
			break;
		}
		case 4 : {
			/* Travia: skill treat poison + 1 */
			ds_inc_bs_post(HERO_SKILLS + 44);
			break;
		}
		case 5 : {
			/* Boron: skill human nature + 1 */
			ds_inc_bs_post(HERO_SKILLS + 24);
			break;
		}
		case 6 : {
			/* Hesinde: skill alchemy + 1 */
			ds_inc_bs_post(HERO_SKILLS + 32);
			break;
		}
		case 7 : {
			/* Firun: skills track and missle weapons + 1  */
			ds_inc_bs_post(HERO_SKILLS + 26);
			ds_inc_bs_post(HERO_SKILLS + 7);
			break;
		}
		case 8 : {
			/* Tsa: CH + 1 */
			ds_writebs(HERO_ATT0_CURRENT + 3 * 2,
				ds_writebs(HERO_ATT0_NORMAL + 3 * 2,
					ds_readbs(HERO_ATT0_NORMAL + 3 * 2) + 1));
			ds_writew(GOT_CH_BONUS, 1);
			break;
		}
		case 9 : {
			/* Phex: skills hide and pickpocket + 1 */
			ds_inc_bs_post(HERO_SKILLS + 49);
			ds_inc_bs_post(HERO_SKILLS + 13);
			break;
		}
		case 10 : {
			/* Peraine: skills treat disease and wounds + 1 */
			ds_inc_bs_post(HERO_SKILLS + 45);
			ds_inc_bs_post(HERO_SKILLS + 46);
			break;
		}
		case 11 : {
			/* Ingerimm: skill tactics + 1*/
			ds_inc_bs_post(HERO_SKILLS + 37);
			break;
		}
		case 12 : {
			/* Rhaja: skills dance, seduce and instrument + 1*/
			ds_inc_bs_post(HERO_SKILLS + 20);
			ds_inc_bs_post(HERO_SKILLS + 16);
			ds_inc_bs_post(HERO_SKILLS + 47);
			break;
		}
	}
	/* calclate AT and PA values */
	calc_at_pa();

	/* if mode == novice */
	if (ds_readws(LEVEL) == 1) {
		/* increase skills automatically */
		for (i = 0; ds_readbs(HERO_SKILL_INCS) > 0; i++) {
			skill_inc_novice(v1 = ds_readws(AUTOSKILLS + 50 * ds_readbs(HERO_TYPUS) + 2 * i));
		}

		// Okay, till here !

		si = 0;
		/* prepare mage automatic spell list */
		if (ds_readbs(HERO_TYPUS) == 9) {
			/* 1. house spells */
			for (i = 0; ds_readbs(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL)) > i; si++, i++) {
//				autospells[ds_readbs(HERO_TYPUS) - 7][si] =
				ds_writew(AUTOSPELLS + 2 * si,
					ds_readws(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL) + 1 + 2 * i));
			}
			/* 2. all schools spells */
			//for (i = 0; school_tab[ds_readbs(HERO_SPELL_SCHOOL)].spells > i; si++, i++) {
			for (i = 0; ds_readws(SCHOOL_TAB + 2 + 4 * ds_readbs(HERO_SPELL_SCHOOL)) > i; si++, i++) {
				//autospells[ds_readbs(HERO_TYPUS) - 7][si] =
					//school_tab[ds_readbs(HERO_SPELL_SCHOOL)].first_spell + i);

				ds_writew(AUTOSPELLS + 2 * si,
					ds_readw(SCHOOL_TAB + 0 + 4 * ds_readbs(HERO_SPELL_SCHOOL)) + i);
			}
			/* 3. five domination spells */
				/* Herr der Tiere */
			////autospells[ds_readbs(HERO_TYPUS) - 7][si++] = 0x52;
			ds_writew(AUTOSPELLS + 2 * si++, 0x52);
				/* Horriphobus */
			//autospells[ds_readbs(HERO_TYPUS) - 7][si++] = 0x31;
			ds_writew(AUTOSPELLS + 2 * si++, 0x31);
				/* Mag. Raub */
			//autospells[ds_readbs(HERO_TYPUS) - 7][si++] = 0x35;
			ds_writew(AUTOSPELLS + 2 * si++, 0x35);
				/* Respondami */
			//autospells[ds_readbs(HERO_TYPUS) - 7][si++] = 0x21;
			ds_writew(AUTOSPELLS + 2 * si++, 0x21);
				/* Sanftmut */
			//autospells[ds_readbs(HERO_TYPUS) - 7][si++] = 0x4f;
			ds_writew(AUTOSPELLS + 2 * si++, 0x4f);

			/* 4. all house spells */
//			for (i = 0; ds_readbs(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL)) > i; si++, i++) {
//				autospells[ds_readbs(HERO_TYPUS) - 7][si] =
//					ds_readbs(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL) + 1 * 2 * i);
			for (i = 0; ds_readbs(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL)) > i; si++, i++) {
				ds_writew(AUTOSPELLS + 2 * si,
					ds_readw(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL) + 1 + 2 * i));
			}
			/* 5. all house spells */
//			for (i = 0; ds_readbs(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL)) > i; si++, i++) {
//				autospells[ds_readbs(HERO_TYPUS) - 7][si] =
//					ds_readbs(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL) + 1 * 2 * i);
			for (i = 0; ds_readbs(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL)) > i; si++, i++) {
				ds_writew(AUTOSPELLS + 2 * si,
					ds_readw(HOUSE_MOD + 29 * ds_readbs(HERO_SPELL_SCHOOL) + 1 + 2 * i));
			}
			/* 6. random spells */
			while (si < 45) {
				//autospells[ds_readbs(HERO_TYPUS) - 7][si++] = random_gen(85);
				ds_writew(AUTOSPELLS + 2 * si++, random_gen(85));
			}
		}
		/* automatic increase spells */
		for (i = 0; ds_readbs(HERO_SPELL_INCS) > 0; i++) {
//			spell_inc_novice((v2 = autospells[ds_readbs(HERO_TYPUS) - 9][i]));
			spell_inc_novice((v2 = ds_readw(AUTOSPELLS + 90 * (ds_readbs(HERO_TYPUS) - 9) + 2 * i )));
		}
	}
}

/**
 * skill_inc_novice() - tries to increment a skill in novice mode
 * @skill:	the skill which should be incremented
 *
 *
 */
/* Borlandified and identical */
void skill_inc_novice(Bit16s skill)
{
	Bit16s done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (ds_readbs(SKILL_INCS + 2 * skill + 0) == 3) {
			/* set the flag to leave this loop */
			done = 1;
#if !defined(__BORLANDC__)
		} else

		/* Original-Bugfix: add check if skill_attempts are left */
		if (ds_readbs(HERO_SKILL_INCS) == 0) {
			done++;
#endif
		} else {
			/* decrement counter for skill increments */
			ds_dec_bs_post(HERO_SKILL_INCS);

			/* check if the test is passed */
			if ((Bit16s)random_interval_gen(2, 12) > ds_readbs(HERO_SKILLS + skill)) {
				/* increment skill */
				ds_inc_bs_post(HERO_SKILLS + skill);

				/* set inc tries for this skill to zero */
				ds_writeb(SKILL_INCS + 2 * skill + 0, 0);

				/* set the flag to leave this loop */
				done = 1;

				if (skill <= 6) {

					/* set increment the lower AT/PA value */
					if (ds_readbs(HERO_AT_WEAPON + skill) > ds_readbs(HERO_PA_WEAPON + skill))
						ds_inc_bs_post(HERO_PA_WEAPON + skill);
					else
						ds_inc_bs_post(HERO_AT_WEAPON + skill);
				}
			} else {
				/* inc tries for that skill */
				ds_inc_bs_post(SKILL_INCS + 2 * skill + 0);
			}
		}
	}
}


/**
 * spell_inc_novice() - tries to increment a spell in novice mode
 * @spell:	the spell which should be incremented
 *
 *
 */
/* Borlandified and identical */
void spell_inc_novice(Bit16s spell)
{
	Bit16s done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (ds_readbs(SPELL_INCS + 2 * spell + 0) == 3) {
			done = 1;
			continue;
		}

#if !defined(__BORLANDC__)
			/* Original-Bugfix: add check if skill_attempts are left */
			if (ds_readbs(HERO_SPELL_INCS) == 0) {
				done = 1;
				continue;
			}
#endif
		/* decrement counter for spell increments */
		ds_dec_bs_post(HERO_SPELL_INCS);

		/* check if the test is passed */
		if ((Bit16s)random_interval_gen(2, 12) > ds_readbs(HERO_SPELLS + spell)) {

			/* increment spell */
			ds_inc_bs_post(HERO_SPELLS + spell);

			/* set inc tries for this spell to zero */
			ds_writeb(SPELL_INCS + 2 * spell + 0, 0);

			/* set the flag to leave this loop */
			done = 1;
		} else {
			ds_inc_bs_post(SPELL_INCS + 2 * spell + 0);
		}
	}
}

/**
 * select_typus() - select a possible typus with current attribute values
 *
 */
/* Borlandified and nearly identical */
void select_typus()
{
	Bit8s old_typus;
	Bit8s possible_types;
	Bit8s ltmp2;
	RealPt ptr;
	Bit16s i;
	Bit16s impossible;

	register Bit16s di;
	register Bit16s si;


	struct type_bitmap t;

	old_typus = -1;
	
#if !defined(__BORLANDC__)	
	t = *(struct type_bitmap*)(p_datseg + TYPE_BITMAP);
#else
	t = *(struct type_bitmap*)&ds[TYPE_BITMAP];
#endif

	/* check if attribs have been set */
	if (ds_readbs(HERO_ATT0_NORMAL + 3 * 0) != 0) {

		/* save the old typus */
		old_typus = ds_readbs(HERO_TYPUS);
		
		/* disable MU bonus */
		if (ds_readw(GOT_MU_BONUS)) {
			ds_writebs(HERO_ATT0_CURRENT + 3 * 0,
				ds_writebs(HERO_ATT0_NORMAL + 3 * 0,
					ds_readbs(HERO_ATT0_NORMAL + 3 * 0) - 1));
		}
		/* disable CH bonus */
		if (ds_readw(GOT_CH_BONUS)) {
			ds_writebs(HERO_ATT0_CURRENT + 3 * 2,
				ds_writebs(HERO_ATT0_NORMAL + 3 * 2,
					ds_readbs(HERO_ATT0_NORMAL + 3 * 2) - 1));
		}
		possible_types = 0;

		for (i = 1; i <= 12; i++) {
			impossible = 0;
			for (si = 0; si < 4; si++) {

				//ltmp2 = ds_readbs(HERO_ATT0_NORMAL + 3 * reqs[i][si].attrib);
#if !defined(__BORLANDC__)
				ptr = RealMake(datseg, HERO_ATT0_NORMAL + 3 * ds_readb(REQS + 8 * i + 2 * si));
#else
				ptr = (RealPt)&ds[HERO_ATT0_NORMAL + 3 * ds_readb(REQS + 8 * i + 2 * si)];
#endif
				ltmp2 = host_readbs(Real2Host(ptr));

				if ((ds_readbs(REQS + 1 + 8 * i + 2 * si) & 0x80) != 0) {
					if (ltmp2 > (ds_readb(REQS + 1 + 8 * i + 2 * si) & 0x7f))
						impossible = 1;
				} else {
					if (ds_readb(REQS + 1 + 8 * i + 2 * si) > ltmp2)
						impossible = 1;
				}
			}

			if (!impossible) {

				ds_writed(TYPE_NAMES + 4 * possible_types,
						(Bit32u)get_text_real( (ds_readbs(HERO_SEX) ? 271 : 17 ) + i));
				t.t[possible_types] = (char)i;
				possible_types++;
			}
		}

		if (!possible_types) {
			if (can_change_attribs() == 0) {
				/* totally messed up values */
				infobox(get_text(284), 0);
				return;
			} else {
				infobox(get_text(31), 0);
				return;
			}
			return;
		}

		di = gui_radio((Bit8u*)get_text(30), possible_types,
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 *  0)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 *  1)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 *  2)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 *  3)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 *  4)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 *  5)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 *  6)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 *  7)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 *  8)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 *  9)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 10)),
				(char*)Real2Host((RealPt)ds_readd(TYPE_NAMES + 4 * 11)));

		/*	restore attibute boni when selection is canceled
		 *	or the same typus is selected.
		 */
		if ((di != -1) && (t.t[di - 1] != old_typus)) {

			/* set new typus */
#if !defined(__BORLANDC__)
			ds_writeb(HERO_TYPUS, t.t[di - 1]);
#else
			ds_writeb(HERO_TYPUS, _AL);
#endif
			ds_writew(SCREEN_VAR, 1);

			load_typus(ds_readbs(HERO_TYPUS));
			update_mouse_cursor();
			call_fill_rect_gen((RealPt)ds_readd(VGA_MEMSTART), 16, 8, 143, 191, 0);
			wait_for_vsync();
			set_palette(Real2Host(ds_readd(GEN_PTR5)) + 0x5c02, 0, 32);
			call_mouse();

			ds_writeb(HEAD_TYPUS, (ds_readbs(HERO_TYPUS) > 10 ? 10 : ds_readbs(HERO_TYPUS)));

			if (ds_readbs(HERO_SEX)) {
#if !defined(__BORLANDC__)
				ds_writeb(HEAD_FIRST,
					ds_writeb(HEAD_CURRENT, ds_readbs(HEAD_FIRST_FEMALE + ds_readbs(HEAD_TYPUS))));
#else
				ds_writeb(HEAD_FIRST,
					ds_writeb(HEAD_CURRENT, ds_readbs(HEAD_FIRST_FEMALE + (Bit8s)_AL )));
#endif
				ds_writeb(HEAD_LAST, ds_readbs(HEAD_FIRST_MALE + ds_readbs(HEAD_TYPUS) + 1) - 1);
			} else {
				ds_writeb(HEAD_FIRST,
					ds_writeb(HEAD_CURRENT, ds_readbs(HEAD_FIRST_MALE + ds_readbs(HEAD_TYPUS))));
				ds_writeb(HEAD_LAST, ((Bit8s)ds_readbs(HEAD_FIRST_FEMALE + ds_readbs(HEAD_TYPUS))) - 1);
			}

			/* reset boni flags */
			ds_writew(GOT_MU_BONUS, ds_writew(GOT_CH_BONUS, 0));
			fill_values();
		} else {
			if (ds_readw(GOT_MU_BONUS)) {
				ds_writebs(HERO_ATT0_CURRENT + 3 * 0,
					ds_writebs(HERO_ATT0_NORMAL + 3 * 0,
						ds_readbs(HERO_ATT0_NORMAL + 3 * 0) + 1));
			}
			if (ds_readw(GOT_CH_BONUS)) {
				ds_writebs(HERO_ATT0_CURRENT + 3 * 2,
					ds_writebs(HERO_ATT0_NORMAL + 3 * 2,
						ds_readbs(HERO_ATT0_NORMAL + 3 * 2) + 1));
			}
		}
	} else {
		infobox((char*)get_text(265), 0);
	}
}

#define INC (1)
#define DEC (2)

/**
 * can_change_attribs() - checks if attribute changes are possible
 *
 */
/* Borlandified and nearly identical */
Bit16s can_change_attribs()
{
	Bit16s na_inc;
	volatile Bit16s na_dec;
	Bit8u* p;
	Bit16s i;
	Bit16s pa_inc;
	Bit16s pa_dec;

	pa_inc = 0;
	pa_dec = 0;
	na_inc = 0;
	na_dec = 0;

#if !defined(__BORLANDC__)
	for (i = 0; i < 14; i++)
		D1_LOG("%d ", ds_readb(ATTRIB_CHANGED + i));
	D1_LOG("\n");
#endif


	for (i = 0; i < 7; i++) {
		p = p_datseg + HERO_ATT0_NORMAL + 3 * i;

		if ((ds_readb(ATTRIB_CHANGED + i) != INC) && (host_readbs(p) > 8))
			pa_dec += 8 - host_readbs(p);
		if ((ds_readb(ATTRIB_CHANGED + i) != DEC) && (host_readbs(p) < 13))
			pa_inc += 13 - host_readbs(p);
	}

	for (i = 7; i < 14; i++) {
		p = p_datseg + HERO_ATT0_NORMAL + 3 * i;
		if ((ds_readb(ATTRIB_CHANGED + i) != INC) && (host_readbs(p) > 2))
			na_dec += 2 - host_readbs(p);
		if ((ds_readb(ATTRIB_CHANGED + i) != DEC) && (host_readbs(p) < 8))
#if !defined(__BORLANDC__)
			na_inc += 8 - host_readbs(p);
#else
			asm { db 0x0b, 0xc9; nop; } // BCC Sync-Point
#endif
	}

#if !defined(__BORLANDC__)
	D1_LOG("%d %d %d %d\n", pa_inc, pa_dec, na_inc, na_dec);
#endif

	/* no values from positive attributes left */
	if (((pa_inc == 0) && (pa_dec == 0)) ||
	    ((pa_inc == 0) && (na_dec < 2))  ||
	    ((na_inc < 2) && (pa_dec == 0))  ||
	   ((na_dec < 2) && (na_inc < 2)))  return 0;

	return 1;
}

/**
 * change_attribs() - change attributes
 */
/* Borlandified and nearly identical */
void change_attribs()
{
	Bit16s tmp1;
	volatile Bit16s tmp2;
	volatile Bit16s tmp3;
	Bit8u* ptr1;
	Bit8u* ptr2;
	Bit8s c;

	Bit16s si;
	Bit16s di;

	/* check if attributes have been set */
	if (!ds_readbs(HERO_ATT0_NORMAL + 3 * 0)) {
		infobox(get_text(16), 0);
		return;
	}
	/* check if changing is possible */
	if (!can_change_attribs()) {
		infobox(get_text(266), 0);
		return;
	}
	/* if typus != 0 */
	if (ds_readbs(HERO_TYPUS)) {

		if (!gui_bool((Bit8u*)get_text(73)))
			return;

		/* set typus to 0 */
		ds_writeb(HERO_TYPUS, 0);

		/* remove MU boni */
		if (ds_readw(GOT_MU_BONUS)) {
			ds_writeb(HERO_ATT0_CURRENT + 3 * 0,
				ds_writebs(HERO_ATT0_NORMAL + 3 * 0,
					ds_readbs(HERO_ATT0_NORMAL + 3 * 0) - 1));
			ds_writew(GOT_MU_BONUS, 0);
		}
		/* remove CH boni */
		if (ds_readw(GOT_CH_BONUS)) {
			ds_writeb(HERO_ATT0_CURRENT + 3 * 2,
				ds_writebs(HERO_ATT0_NORMAL + 3 * 2,
					ds_readbs(HERO_ATT0_NORMAL + 3 * 2) - 1));
			ds_writew(GOT_CH_BONUS, 0);
		}
		ds_writew(SCREEN_VAR, 1);
		refresh_screen();
		ds_writew(SCREEN_VAR, 0);
#if defined(__BORLANDC__)
		asm { db 0xeb, 0x03; db 0xe9, 0x6c, 0x03; } // BCC Sync-Point
#endif
	}

	/* check again if changing is possible */
	if (can_change_attribs() == 0) {
		infobox(get_text(266), 0);
		return;
	}
	/* select a positive attribute to change */
	ds_writew(0x1327, 0xffb0);
	tmp2 = gui_radio((Bit8u*)get_text(78), 7,
			get_text(32), get_text(33), get_text(34), get_text(35),
			get_text(36), get_text(37), get_text(38));
	ds_writew(0x1327, 0);

	if (tmp2 == -1)
		return;
	tmp2--;
	/* get the modification type */
	if (!ds_readbs(ATTRIB_CHANGED + tmp2)) {
		/* ask user if inc or dec */
		ds_writew(0x1327, 0xffb0);
		tmp3 = gui_radio((Bit8u*)NULL, 2, get_text(75), get_text(76));
		ds_writew(0x1327, 0);

		if (tmp3 == -1)
			return;
	} else {
		tmp3 = ds_readbs(ATTRIB_CHANGED + tmp2);
	}

	ptr1 = p_datseg + HERO_ATT0_NORMAL + 3 * tmp2;

	if (tmp3 == INC) {
		/* increment */
		if (host_readbs(ptr1) == 13) {
			infobox(get_text(77), 0);
			return;
		}
		c = 0;
		for (di = 7; di < 14; di++) {
			if (ds_readb(ATTRIB_CHANGED + di) != DEC) {
				ptr2 = p_datseg + HERO_ATT0_NORMAL + 3 * di;
				if (host_readbs(ptr2) < 8) {
					c += 8 - host_readbs(ptr2);
				}
			}
		}
		if (c < 2) {
			infobox(get_text(85), 0);
			return;
		}
		/* increment positive attribute */
		//ds_inc_bs_post(HERO_ATT0_CURRENT + 3 * tmp2);
		//ds_inc_bs_post(HERO_ATT0_NORMAL + 3 * tmp2);
#if !defined(__BORLANDC__)
		host_writebs(ptr1, host_writebs(ptr1 + 1, host_readbs(ptr1 + 1) + 1));
#else
		//ds_inc_bs_post(HERO_ATT0_CURRENT + 3 * tmp2);
		ds_inc_bs_post(HERO_ATT0_NORMAL + 3 * tmp2);
		asm { db 0x8b, 0x5e, 0xfc; }; // BCC Sync-Point
#endif

		ds_writeb(ATTRIB_CHANGED + tmp2,  INC);

		refresh_screen();

		tmp1 = 0;
		while (tmp1 != 2) {
		
			do {
				/* ask which negative attribute to increment */
				ds_writew(0x1327, 0xffb0);
				si = gui_radio((Bit8u*)get_text(80), 7,
						get_text(39), get_text(40), get_text(41),
						get_text(42), get_text(43), get_text(44),
						get_text(45));
				ds_writew(0x1327, 0);

			} while (si == -1);

			si--;
			/* check if this attribute has been decremented */
			if (ds_readb(ATTRIB_CHANGED + si + 7) == DEC) {
				infobox(get_text(83), 0);
				continue;
			}
			ptr1 = p_datseg + HERO_ATT_AG_NORMAL + 3 * si;
			/* check if attribute can be incremented */
			if (host_readbs(ptr1) == 8) {
				infobox(get_text(77), 0);
			} else {
				/* increment the negative attribute */
				tmp1++;
				ds_writeb(ATTRIB_CHANGED + si + 7, INC);

#if !defined(__BORLANDC__)
				host_writeb(ptr1,
					host_writebs(ptr1 + 1,
						host_readbs(ptr1 + 1) + 1));
#else
				ds_inc_bs_post(HERO_ATT0_NORMAL + 3 * (si + 7));
				//ds_inc_bs_post(HERO_ATT0_CURRENT + 3 * (si + 7)); // BCC Sync-Point
				asm { db 0xe8, 0xad, 0xde; db 0x83, 0x7e, 0x80, 0x80;}
#endif

				refresh_screen();
			}
		}
	} else {
		/* decrement */
		/* check if the positive attribute can be decremented */
		if (host_readbs(ptr1) == 8) {
			infobox(get_text(81), 0);
			return;
		}
		c = 0;
		for (di = 7; di < 14; di++) {
			if (ds_readb(ATTRIB_CHANGED + di) != INC) {
				ptr2 = p_datseg + HERO_ATT0_NORMAL + 3 * di;
				if (host_readbs(ptr2) > 2) {
#if !defined(__BORLANDC__)
					c += host_readbs(ptr2) - 2;
#else
					c = host_readbs(ptr2) - 2; // BCC Sync-Point
#endif
				}
			}
		}
		if (c < 2) {
			infobox(get_text(84), 0);
			return;
		}
		/* decrement positive attribute */
		//ds_dec_bs_post(HERO_ATT0_NORMAL + 3 * tmp2);
		//ds_dec_bs_post(HERO_ATT0_CURRENT + 3 * tmp2);
#if !defined(__BORLANDC__)
		host_writeb(ptr1,
			host_writebs(ptr1 + 1,
				host_readbs(ptr1 + 1) - 1));
#else
		host_writebs(ptr1 + 1, host_readbs(ptr1 + 1) - 1); // BCC Sync-point
#endif

		/* mark this attribute as decremented */
		ds_writeb(ATTRIB_CHANGED + tmp2, DEC);

		refresh_screen();

		tmp1 = 0;
		while (tmp1 != 2) {

			do {
				/* ask which negative attribute to increment */
				ds_writew(0x1327, 0xffb0);
				si = gui_radio((Bit8u*)get_text(79), 7,
						get_text(39), get_text(40), get_text(41),
						get_text(42), get_text(43), get_text(44),
						get_text(45));
				ds_writew(0x1327, 0);
			} while (si == -1);


			si--;
			/* check if this attribute has been incremented */
			if (ds_readb(ATTRIB_CHANGED + si + 7) == INC) {
				infobox(get_text(82), 0);
				continue;
			}
				
			ptr1 = p_datseg + HERO_ATT_AG_NORMAL + 3 * si;
			
			/* check if attribute can be decremented */
			if (host_readbs(ptr1) == 2) {
				infobox(get_text(81), 0);
				continue;
			}
			/* decrement the negative attribute */
			tmp1++;

			//ds_dec_bs_post(HERO_ATT0_NORMAL + 3 * (si + 7));
			//ds_dec_bs_post(HERO_ATT0_CURRENT + 3 * (si + 7));

#if !defined(__BORLANDC__)
			host_writebs(ptr1,
				host_writebs(ptr1 + 1,
					host_readbs(ptr1 + 1) - 1));
#else
			host_writebs(ptr1 + 1, host_readbs(ptr1 + 1) - 1); // BCC Sync-Point
#endif

			ds_writeb(ATTRIB_CHANGED + si + 7, DEC);

			refresh_screen();
		}
	}
}

#undef INC
#undef DEC

/* Borlandified and nearly identical */
void save_picbuf()
{
	RealPt p;
	Bit16s x_3;
	Bit16s y_1;
	Bit16s y_2;
	Bit16s y_3;
	Bit16s w_1;
	Bit16s w_2;
	Bit16s w_3;
	Bit16s h_1;
	Bit16s h_2;
	Bit16s h_3;

	register Bit16s x_1;
	register Bit16s x_2;

	x_1 = 0;

	/* check on which page we are */
	switch (ds_readws(GEN_PAGE)) {
		/* main page */
		case 0: {
			/* name field */
			x_1 = 176; y_1 = 12; w_1 = 94; h_1 = 8;

			/* positive attributes values */
			x_2 = 205; y_2 = 73; w_2 = 20; h_2 = 90;

			/* negative attribute values */
			x_3 = 273; y_3 = 73; w_3 = 20; h_3 = 90;
			break;
		}
		/* skill pages */
		case 1:	case 2: case 3: {
			/* remaining skill augmention tries */
			x_1 = 270; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of skills */
			x_2 = 205; y_2 = 42; w_2 = 20; h_2 = 140;

			/* right row of skills */
			x_3 = 287; y_3 = 42; w_3 = 20; h_3 = 140;
			break;
		}
		/* ATPA page */
		case 4: {
			/* left row AT values */
			x_2 = 225; y_2 = 48; w_2 = 20; h_2 = 130;

			/* right row PA values */
			x_3 = 260; y_3 = 48; w_3 = 20; h_3 = 130;
			break;
		}
		/* spell pages */
		case 5: case 6:	case 7:
		case 8:	case 9:	case 10: {
			/* remaining spell augmention tries */
			x_1 = 215; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of spells */
			x_2 = 141; y_2 = 42; w_2 = 20;	h_2 = 140;

			/* right row of spells */
			x_3 = 286; y_3 = 42; w_3 = 20;	h_3 = 140;
			break;
		}
	}

	if (x_1) {
		p = (RealPt)ds_readd(GEN_PTR1_DIS) + y_1 * 320 + x_1;
		copy_to_screen(Real2Phys(p), Real2Phys((RealPt)ds_readd(PICBUF1)), w_1, h_1, 2);
	}

	p = (RealPt)ds_readd(GEN_PTR1_DIS) + y_2 * 320 + x_2;
	copy_to_screen(Real2Phys(p), Real2Phys((RealPt)ds_readd(PICBUF2)), w_2, h_2, 2);
#if !defined(__BORLANDC__)
	p = (RealPt)ds_readd(GEN_PTR1_DIS) + y_3 * 320 + x_3;
	copy_to_screen(Real2Phys(p), Real2Phys((RealPt)ds_readd(PICBUF3)), w_3, h_3, 2);
#else
	// BCC Sync-Point
	//p -= y_2 * 320 + x_2; // revert to GEN_PTR1_DIS
	p += y_3 ; // add offset
	copy_to_screen(Real2Phys(p), Real2Phys((RealPt)ds_readd(PICBUF3)), w_3, h_3, 2);
	asm { nop; nop; nop; nop; db 0x6a, 0x02; db 0x6a, 0x02;};
#endif
}

/* Borlandified and nearly identical */
void restore_picbuf(RealPt ptr)
{
	RealPt p;
	Bit16u x_1, x_2, x_3;
	Bit16u y_1, y_2, y_3;
	Bit16u w_1, w_2, w_3;
	Bit16u h_1, h_2, h_3;

	x_1 = 0;

	/* check on which page we are */
	switch (ds_readws(GEN_PAGE)) {
		/* main page */
		case 0: {
			/* name field */
			x_1 = 176; y_1 = 12; w_1 = 94; h_1 = 8;

			/* positive attributes values */
			x_2 = 205; y_2 = 73; w_2 = 20; h_2 = 90;

			/* negative attribute values */
			x_3 = 273; y_3 = 73; w_3 = 20; h_3 = 90;
			break;
		}
		/* skill pages */
		case 1:	case 2: case 3: {
			/* remaining skill augmention tries */
			x_1 = 270; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of skills */
			x_2 = 205; y_2 = 42; w_2 = 20; h_2 = 140;

			/* right row of skills */
			x_3 = 287; y_3 = 42; w_3 = 20; h_3 = 140;
			break;
		}
		/* ATPA page */
		case 4: {
			/* left row AT values */
			x_2 = 225; y_2 = 48; w_2 = 20; h_2 = 130;

			/* right row PA values */
			x_3 = 260; y_3 = 48; w_3 = 20; h_3 = 130;
			break;
		}
		/* spell pages */
		case 5: case 6:	case 7:
		case 8:	case 9:	case 10: {
			/* remaining spell augmention tries */
			x_1 = 215; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of spells */
			x_2 = 141; y_2 = 42; w_2 = 20;	h_2 = 140;

			/* right row of spells */
			x_3 = 286; y_3 = 42; w_3 = 20;	h_3 = 140;
			break;
		}
	}

	if (x_1) {
		p = ptr + y_1 * 320 + x_1;
		copy_to_screen(Real2Phys((RealPt)ds_readd(PICBUF1)), Real2Phys(p), w_1, h_1, 0);
	}

	p = ptr + y_2 * 320 + x_2;
	copy_to_screen(Real2Phys((RealPt)ds_readd(PICBUF2)), Real2Phys(p), w_2, h_2, 0);

#if !defined(__BORLANDC__)
	p = ptr + y_3 * 320 + x_3;
	copy_to_screen(Real2Phys((RealPt)ds_readd(PICBUF3)), Real2Phys(p), w_3, h_3, 0);
#else
	// BCC Sync-Point
	p += y_3 ; // add offset
	copy_to_screen(Real2Phys((RealPt)ds_readd(PICBUF3)), Real2Phys(p), w_3, h_3, 0);
	asm { nop; nop; nop; nop; nop; nop;};
#endif
}

/**
 * print_attribs() -	print the attribute values
 *
 */
/* Borlandified and nearly identical */
void print_attribs()
{
	Bit8u* p;
	char buf[10];

	Bit16s i;

	p = p_datseg + HERO_ATT0_NORMAL;

	for (i = 0; i < 14; p += 3, i++) {
		/* don't print 0s */
		if (host_readbs(p) != 0) {
#if !defined(__BORLANDC__)
			/* convert value to string with itoa() */
			sprintf(buf, "%d", host_readbs(p));
			/* print it */
			print_str(buf, ds_readws(ATTRIB_COORDS_X + i * 4), ds_readws(ATTRIB_COORDS_Y + i * 4));
#else
			/* print it */
			print_str(itoa(host_readbs(p), buf, 10),
				ds_readws(ATTRIB_COORDS_X + i * 4),
				ds_readws(ATTRIB_COORDS_Y + i * 4));
#endif
		}
	}
}


/**
 * print_values() - print the values of the character
 *
 */
/* Borlandified and identical */
void print_values()
{
	char tmp[4];
	Bit16s width;
	Bit16s align_left = 222;
	Bit16s align_right = 302;

	register Bit16s i;
	register Bit16s pos;


	switch (ds_readws(GEN_PAGE)) {

		case 0: {
			restore_picbuf((RealPt)ds_readd(GFX_PTR));

			/* print name */
			print_str((char*)p_datseg + HERO_NAME, 180, 12);

			/* print attributes */
			print_attribs();

			/* return if no typus */
			if (ds_readbs(HERO_TYPUS) == 0)	return;

			/* print height */
			sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(70), ds_readb(HERO_HEIGHT));
			print_str((char*)Real2Host(ds_readd(GEN_PTR2)), 205, 25);

			/* print weight */
			sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(71), ds_readws(HERO_WEIGHT));

			print_str((char*)Real2Host(ds_readd(GEN_PTR2)), 205, 37);

			/* print god name */
			print_str(get_text(56 + ds_readbs(HERO_GOD)), 205, 49);

			/* print money */
			make_valuta_str((char*)Real2Host(ds_readd(GEN_PTR2)), ds_readds(HERO_MONEY));
			print_str((char*)Real2Host(ds_readd(GEN_PTR2)), 205, 61);

#if !defined(__BORLANDC__)
			/* print LE */
			/* originally it was itoa() */
			sprintf(tmp, "%d", ds_readws(HERO_LE_MAX));
			print_str(tmp, 172, 164);

			/* print AE */
			/* originally it was itoa() */
			sprintf(tmp, "%d", ds_readws(HERO_AE_MAX));
			print_str(tmp, 221, 164);

			/* print Endurance */
			/* originally it was itoa() */
			sprintf(tmp, "%d",
				ds_readws(HERO_LE_MAX) + ds_readbs(HERO_ATT0_CURRENT + 3 * 6));
			print_str(tmp, 296, 164);

			/* print MR */
			/* originally it was itoa() */
			sprintf(tmp, "%d", ds_readbs(HERO_MR));
			print_str(tmp, 232, 184);
#else
			/* print LE */
			print_str(itoa(ds_readws(HERO_LE_MAX), tmp, 10), 172, 164);
			/* print AE */
			print_str(itoa(ds_readws(HERO_AE_MAX), tmp, 10), 221, 164);
			/* print Endurance */
			print_str(itoa(ds_readws(HERO_LE_MAX) + ds_readbs(HERO_ATT0_CURRENT + 3 * 6), tmp, 10), 296, 164);
			/* print MR */
			print_str(itoa(ds_readbs(HERO_MR), tmp, 10), 232, 184);
#endif
			break;
		}
		case 1: {
			/* SKILLS Page 1/3 */
#if defined(__BORLANDC__)
			asm { db 0xe9, 0x85, 0x0a}; // BCC Sync-Point
			// a 7-Byte Multi Byte NOP :-)
			//asm { db 0x0f, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00; }; // BCC Sync-Point
#endif
			restore_picbuf((RealPt)ds_readd(GFX_PTR));


			/* print fight skills */
			for (i = 0; i < 9; i++) {
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SKILLS + i));
#else
				itoa(ds_readbs(HERO_SKILLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// i & 1 = right column else left column
				print_str(tmp, ((i & 1) ? align_right - width : align_left - width), (i / 2) * 12 + 42);
			}

			/* print body skills */
			for (i = 9; i < 19; i++) {
				pos = i - 9;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SKILLS + i));
#else
				itoa(ds_readbs(HERO_SKILLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 119);
			}

			/* remaining attempts for skills */
			/* originally it was itoa() */
#if !defined(__BORLANDC__)
			sprintf(tmp, "%d", ds_readbs(HERO_SKILL_INCS));
			print_str(tmp, 271, 184);
#else
			print_str(itoa(ds_readbs(HERO_SKILL_INCS), tmp, 10), 271, 184);
#endif

			break;
		}
		case 2: {
			/* SKILLS Page 2/3 */
			restore_picbuf((RealPt)ds_readd(GFX_PTR));

			/* print social skills */
			for (i = 19; i < 26; i++) {
				pos = i - 19;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SKILLS + i));
#else
				itoa(ds_readbs(HERO_SKILLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 42);
			}

			/* print lore skills */
			for (i = 32; i < 41; i++) {
				pos = i - 32;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SKILLS + i));
#else
				itoa(ds_readbs(HERO_SKILLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 113);
			}

			/* remaining attempts for skills */
			/* originally it was itoa() */
#if !defined(__BORLANDC__)
			sprintf(tmp, "%d", ds_readbs(HERO_SKILL_INCS));
			print_str(tmp, 271, 184);
#else
			print_str(itoa(ds_readbs(HERO_SKILL_INCS), tmp, 10), 271, 184);
#endif
			break;
		}
		case 3: {
			/* SKILLS Page 3/3 */
			restore_picbuf((RealPt)ds_readd(GFX_PTR));

			/* print craftmansship skills */
			for (i = 41; i < 50; i++) {
				pos = i - 41;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SKILLS + i));
#else
				itoa(ds_readbs(HERO_SKILLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 42);

			}

			/* print nature skills */
			for (i = 26; i < 32; i++) {
				pos = i - 26;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SKILLS + i));
#else
				itoa(ds_readbs(HERO_SKILLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 119);

			}

			/* print intuition skills */
			for (i = 50; i < 52; i++) {
				pos = i - 50;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SKILLS + i));
#else
				itoa(ds_readbs(HERO_SKILLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 170);
			}

			/* remaining attempts for skills */
			/* originally it was itoa() */
#if !defined(__BORLANDC__)
			sprintf(tmp, "%d", ds_readbs(HERO_SKILL_INCS));
			print_str(tmp, 271, 184);
#else
			print_str(itoa(ds_readbs(HERO_SKILL_INCS), tmp, 10), 271, 184);
#endif
			break;
		}
		case 4: {
			/* ATPA Page */
			restore_picbuf((RealPt)ds_readd(GFX_PTR));

			/* Print base value  2x the same */
#if !defined(__BORLANDC__)
			sprintf(tmp, "%d", ds_readbs(HERO_ATPA_BASE));
			print_str(tmp, 231, 30);
			sprintf(tmp, "%d", ds_readbs(HERO_ATPA_BASE));
			print_str(tmp, 268, 30);
#else
			print_str(itoa(ds_readbs(HERO_ATPA_BASE), tmp, 10), 231, 30);
			print_str(itoa(ds_readbs(HERO_ATPA_BASE), tmp, 10), 268, 30);
#endif

			for (i = 0; i < 7; i++) {
				/* print AT value */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_AT_WEAPON + i));
#else
				itoa(ds_readbs(HERO_AT_WEAPON + i), tmp, 10);
#endif
				print_str(tmp, 237 - get_str_width(tmp), i * 12 + 48);

				/* print PA value */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_PA_WEAPON + i));
#else
				itoa(ds_readbs(HERO_PA_WEAPON + i), tmp, 10);
#endif
				print_str(tmp, 274 - get_str_width(tmp), i * 12 + 48);

				/* print skill value */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SKILLS + i));
#else
				itoa(ds_readbs(HERO_SKILLS + i), tmp, 10);
#endif
				print_str(tmp, 315 - get_str_width(tmp), i * 12 + 48);
			}

			/* calc range base value (KL+GE+KK)/4 */
			pos = (ds_readbs(HERO_ATT0_NORMAL + 3 * 1)
				 + ds_readbs(HERO_ATT0_NORMAL + 3 * 4)
				 + ds_readbs(HERO_ATT0_NORMAL + 3 * 6)) / 4;

			/* print missle and thrown weapon values */
#if !defined(__BORLANDC__)
			sprintf(tmp, "%d", pos + ds_readbs(HERO_SKILLS + 7));
			print_str(tmp, 231, 144);
			sprintf(tmp, "%d", pos + ds_readbs(HERO_SKILLS + 8));
			print_str(tmp, 231, 156);
#else
			print_str(itoa(pos + ds_readbs(HERO_SKILLS + 7), tmp, 10), 231, 144);
			print_str(itoa(pos + ds_readbs(HERO_SKILLS + 8), tmp, 10), 231, 156);
#endif

			break;
		}

		case 5: {
			/* Spells Page 1/6 */
			restore_picbuf((RealPt)ds_readd(GFX_PTR));

			for (i = 1; i < 6; i++) {
				pos = i - 1;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);

			}
			for (i = 33; i < 38; i++) {
				pos = i - 33;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 95);
			}
			for (i = 6; i <= 11; i++) {
				pos = i - 6;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 146);
			}

			/* print spell attempts */
#if !defined(__BORLANDC__)
			sprintf(tmp, "%d", ds_readbs(HERO_SPELL_INCS));
			print_str(tmp, 217, 184);
#else
			print_str(itoa(ds_readbs(HERO_SPELL_INCS), tmp, 10), 217, 184);
#endif
			break;
		}
		case 6: {
			/* Spells Page 2/6 */
			restore_picbuf((RealPt)ds_readd(GFX_PTR));

			for (i = 12; i <= 17; i++) {
				pos = i - 12;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}
			for (i = 18; i < 24; i++) {
				pos = i - 18;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 95);

			}
			for (i = 24; i < 27; i++) {
				pos = i - 24;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 148);

			}

			/* print spell attempts */
#if !defined(__BORLANDC__)
			sprintf(tmp, "%d", ds_readbs(HERO_SPELL_INCS));
			print_str(tmp, 217, 184);
#else
			print_str(itoa(ds_readbs(HERO_SPELL_INCS), tmp, 10), 217, 184);
#endif
			break;
		}
		case 7: {
			/* Spells Page 3/6 */
			restore_picbuf((RealPt)ds_readd(GFX_PTR));

			for (i = 27; i < 33; i++) {
				pos = i - 27;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}

			for (i = 38; i < 45; i++) {
				pos = i - 38;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 95);
			}

			for (i = 45; i <= 46; i++) {
				pos = i - 45;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 160);
			}

			/* print spell attempts */
#if !defined(__BORLANDC__)
			sprintf(tmp, "%d", ds_readbs(HERO_SPELL_INCS));
			print_str(tmp, 217, 184);
#else
			print_str(itoa(ds_readbs(HERO_SPELL_INCS), tmp, 10), 217, 184);
#endif
			break;
		}
		case 8: {
			/* Spells Page 4/6 */
			restore_picbuf((RealPt)ds_readd(GFX_PTR));

			for (i = 47; i <= 48; i++) {
				pos = i - 47;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}

			for (i = 49; i < 58; i++) {
				pos = i - 49;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 71);
			}

			for (i = 58; i < 60; i++) {
				pos = i - 58;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 148);
			}

			/* print spell attempts */
#if !defined(__BORLANDC__)
			sprintf(tmp, "%d", ds_readbs(HERO_SPELL_INCS));
			print_str(tmp, 217, 184);
#else
			print_str(itoa(ds_readbs(HERO_SPELL_INCS), tmp, 10), 217, 184);
#endif
			break;
		}
		case 9: {
			/* Spells Page 5/6 */
			restore_picbuf((RealPt)ds_readd(GFX_PTR));

			for (i = 60; i < 76; i++) {
				pos = i - 60;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}

			/* print spell attempts */
#if !defined(__BORLANDC__)
			sprintf(tmp, "%d", ds_readbs(HERO_SPELL_INCS));
			print_str(tmp, 217, 184);
#else
			print_str(itoa(ds_readbs(HERO_SPELL_INCS), tmp, 10), 217, 184);
#endif
			break;
		}
		case 10: {
			/* Spells Page 6/6 */
			restore_picbuf((RealPt)ds_readd(GFX_PTR));

			for (i = 76; i < 86; i++) {
				pos = i - 76;
				/* originally it was itoa() */
#if !defined(__BORLANDC__)
				sprintf(tmp, "%d", ds_readbs(HERO_SPELLS + i));
#else
				itoa(ds_readbs(HERO_SPELLS + i), tmp, 10);
#endif
				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);

			}

			/* print spell attempts */
#if !defined(__BORLANDC__)
			sprintf(tmp, "%d", ds_readbs(HERO_SPELL_INCS));
			print_str(tmp, 217, 184);
#else
			print_str(itoa(ds_readbs(HERO_SPELL_INCS), tmp, 10), 217, 184);
#endif
			break;
		}
	}
}

/**
 *	make_valuta_str	-	makes a valuta string
 *	@dst:	the destination
 *	@money:	the money in Heller
 *
 *	This funcion is buggy.
 */
/* Borlandified and identical */
void make_valuta_str(char *dst, Bit32s money)
{
	/* Orig-BUG: d can overflow  on D > 65536*/
	unsigned short d = 0;
	unsigned short s = 0;

	/*	These loops are not very performant.
		They take longer the more money you have.
		Here is a much better solution.
	*/

	/*
	d = money / 100;
	money -= d * 100;

	s = money / 10;
	money -= s * 10;
	*/
	while (money / 100) {
		d++;
		money -= 100;
	}

	while (money / 10) {
		s++;
		money -= 10;
	}

	sprintf(dst, get_text(69), d, s, (Bit16s)money);
}

/* Borlandified and nearly identical */
void inc_skill(Bit16s skill, Bit16s max, char *msg)
{
	/* no more increments than the maximum */
	if (ds_readbs(SKILL_INCS + 2 * skill + 1) >= max) {
		infobox((char*)msg, 0);
		return;
	}
	/* we just have 3 tries to increment */
	if (ds_readbs(SKILL_INCS + 2 * skill + 0) == 3) {
		infobox(get_text(151), 0);
		return;
	}

	/* decrement total number of skill inc tries */
	ds_dec_bs_post(HERO_SKILL_INCS);
	if ((Bit16s)random_interval_gen(2, 12) > ds_readbs(HERO_SKILLS + skill)) {
		/* print sucess message */
		infobox(get_text(152), 0);
		/* increment skill */
		ds_inc_bs_post(HERO_SKILLS + skill);
		/* reset tries */
		ds_writeb(SKILL_INCS + 2 * skill + 0, 0);
		/* increment skill increments */
		ds_inc_bs_post(SKILL_INCS + 2 * skill + 1);

		/* check if we have a melee attack skill */
		if (skill <= 6) {
			/* check if AT > PA */
			if (ds_readbs(HERO_AT_WEAPON + skill) > ds_readbs(HERO_PA_WEAPON + skill)) {
				/* inc PA */
				ds_inc_bs_post(HERO_PA_WEAPON + skill);
			} else {
				/* inc AT */
				ds_inc_bs_post(HERO_AT_WEAPON + skill);
			}
		}
	} else {
		/* print failure message */
		infobox(get_text(153), 0);
		/* increment try */
		ds_inc_bs_post(SKILL_INCS + 2 * skill + 0);
	}

	refresh_screen();
}

void select_skill()
{
	Bit16s skill;
	Bit16s group;

	do {

		/* check skill attempts */
		if (!ds_readbs(HERO_SKILL_INCS)) {
			infobox(get_text(94), 0);
			ds_writew(0x1327, 0x0);
			return;
		}

		ds_writew(0x1327, 0xffb0);

		switch (ds_readws(GEN_PAGE)) {
		case 1: {
			group = gui_radio((Bit8u*)get_text(93), 2, get_text(86), get_text(87));
			if (group != -1) {
#if !defined(__BORLANDC__)
				switch (group)
#else
				switch (_AX)
#endif
				{
				case 1: {
					/* Fight */
					skill = gui_radio((Bit8u*)get_text(147),
						9,
						get_text(95), get_text(96), get_text(97),
						get_text(98), get_text(99), get_text(100),
						get_text(101), get_text(102), get_text(103)) - 1;

					if (skill != -2) {
						inc_skill(skill, 1, get_text(148));
					}
					break;
				}
				case 2: {
					/* Body */
					skill = gui_radio((Bit8u*)get_text(147),
						10,
						get_text(104), get_text(105),
						get_text(106), get_text(107),
						get_text(108), get_text(109),
						get_text(110), get_text(111),
						get_text(112), get_text(113)) - 1;

					if (skill != -2) {
						skill += 9;
						inc_skill(skill, 2, get_text(149));
					}
					break;
				}
				}
			}
			break;
		}
		case 2: {
			group = gui_radio((Bit8u*)get_text(93), 2, get_text(88), get_text(89));
			if (group != -1) {
#if !defined(__BORLANDC__)
				switch (group)
#else
				switch (_AX)
#endif
				{
				case 1: {
					skill = gui_radio((Bit8u*)get_text(147),
							7,
							get_text(114), get_text(115), get_text(116),
							get_text(117), get_text(118), get_text(119),
							get_text(120)) - 1;

					if (skill != -2) {
						skill += 19;
						inc_skill(skill, 2, get_text(149));
					}
					break;
				}
				case 2: {
					skill = gui_radio((Bit8u*)get_text(147),
							9,
							get_text(127), get_text(128), get_text(129),
							get_text(130), get_text(131), get_text(132),
							get_text(133), get_text(134), get_text(135)) - 1;

					if (skill != -2) {
						skill += 32;
						inc_skill(skill, 3, get_text(150));
					}
					break;
				}
				}
			}
			break;
		}
		case 3: {
			group = gui_radio((Bit8u*)get_text(93), 3, get_text(90), get_text(91), get_text(92));
			if (group != -1) {
#if !defined(__BORLANDC__)
				switch (group)
#else
				switch (_AX)
#endif
				{
					case 1: {
						skill = gui_radio((Bit8u*)get_text(147),
							9,
							get_text(136), get_text(137), get_text(138),
							get_text(139), get_text(140), get_text(141),
							get_text(142), get_text(143), get_text(144)) - 1;

						if (skill != -2) {
							skill += 41;
							inc_skill(skill, 2, get_text(149));
						}
						break;
					}
					case 2: {
						skill = gui_radio((Bit8u*)get_text(147),
							6,
							get_text(121), get_text(122), get_text(123),
							get_text(124), get_text(125), get_text(126)) - 1;

						if (skill != -2) {
							skill += 26;
							inc_skill(skill, 2, get_text(149));
						}
						break;
					}
					case 3: {
						skill = gui_radio((Bit8u*)get_text(147),
							2,
							get_text(145),
							get_text(146)) - 1;

						if (skill != -2) {
							skill += 50;
							inc_skill(skill, 1, get_text(148));
						}
						break;
					}
				}
				}
				break;
			}
		}

		ds_writew(0x1327, 0);

	} while (group != -1);
}

/* Borlandified and identical */
void inc_spell(Bit16s spell)
{
	Bit16s max_incs = 1;

	/* if typus == warlock and the origin of the spell is warlock */
	//if ((ds_readbs(HERO_TYPUS) == 7) && (spelltab[spell].origin == 3))
	if ((ds_readbs(HERO_TYPUS) == 7) && (ds_readbs(SPELL_TAB + 5 * spell) == 3))
		max_incs = 2;
	/* if typus == elf and the origin of the spell is elven */
	if ((ds_readbs(HERO_TYPUS) >= 10) && (ds_readbs(SPELL_TAB + 5 * spell) == 2))
		max_incs = 2;
	/* if typus == druid and the origin of the spell is druid */
	if ((ds_readbs(HERO_TYPUS) == 8) && (ds_readbs(SPELL_TAB + 5 * spell) == 0))
		max_incs = 2;
	/* if typus == mage */
	if (ds_readbs(HERO_TYPUS) == 9) {
		/* and the origin of the spell is mage */
		if (ds_readbs(SPELL_TAB + 5 * spell) == 1)
			max_incs = 2;

		/* and is a school spell */
		if (is_in_word_array(spell, (signed short*)Real2Host(ds_readd(HOUSE_SPELLS +  4 * ds_readbs(HERO_SPELL_SCHOOL)))))
			max_incs = 3;
	}

	/* all spell increments used for that spell */
	if (ds_readbs(SPELL_INCS + 2 * spell + 1) >= max_incs) {
		infobox(get_text(257), 0);
		return;

	}
	/* all tries used for that spell */
	if (ds_readbs(SPELL_INCS + 2 * spell + 0) == 3) {
		infobox(get_text(151), 0);
#if !defined(__BORLANDC__)
		return;
#else
		// Fool the BCC a bit with a handcoded return
		asm { db 0xeb, 0x60; }
#endif
	}

	/* decrement spell attempts */
	ds_dec_bs_post(HERO_SPELL_INCS);

	if ((Bit16s)random_interval_gen(2, 12) > ds_readbs(HERO_SPELLS + spell)) {
		/* show success */
		infobox(get_text(152), 0);
		/* increment spell value */
		ds_inc_bs_post(HERO_SPELLS + spell);
		/* reset tries */
		ds_writebs(SPELL_INCS + 2 * spell + 0, 0);
		/* increment incs */
		ds_inc_bs_post(SPELL_INCS + 2 * spell + 1);
	} else {
		/* show failure */
		infobox(get_text(153), 0);
		/* increment tries */
		ds_inc_bs_post(SPELL_INCS + 2 * spell + 0);
	}

	refresh_screen();
}

/* Borlandified and identical */
void select_spell()
{
	Bit16s group;
	Bit16s spell;

	do {

		/* check if we have spell attempts */
		if (!ds_readbs(HERO_SPELL_INCS)) {
			infobox(get_text(94), 0);
			ds_writew(0x1327, 0);
			return;
		}

		ds_writew(0x1327, 0xffa6);

		switch (ds_readws(GEN_PAGE)) {
			case 5: {
				group = gui_radio((Bit8u*)get_text(155),
						3,
						get_text(157), get_text(162),
						get_text(158));
				if (group != -1) {
#if !defined(__BORLANDC__)
				switch (group)
#else
				switch (_AX)
#endif
				{
					case 1: {
						spell = gui_radio((Bit8u*)get_text(156),
								5,
								get_text(169),
								get_text(170),
								get_text(171),
								get_text(172),
								get_text(173)) - 1;

						if (spell != -2) {
							spell++;
							inc_spell(spell);
						}
						break;
					}
					case 2: {
						spell = gui_radio((Bit8u*)get_text(156), 5,
								get_text(201),
								get_text(202),
								get_text(203),
								get_text(204),
								get_text(205)) - 1;

						if (spell != -2) {
							spell += 33;
							inc_spell(spell);
						}
						break;
					}
					case 3: {
						spell = gui_radio((Bit8u*)get_text(156), 6,
								get_text(174),
								get_text(175),
								get_text(176),
								get_text(177),
								get_text(178),
								get_text(179)) - 1;
						if (spell != -2) {
							spell += 6;
							inc_spell(spell);
						}
						break;
					}
				}
				}
				break;
			}
			case 6: {
				group = gui_radio((Bit8u*)get_text(155), 3,
						get_text(158), get_text(159),
						get_text(160));
				if (group != -1) {

#if !defined(__BORLANDC__)
				switch (group)
#else
				switch (_AX)
#endif
				{
					case 1: {
						spell = gui_radio((Bit8u*)get_text(156), 6,
								get_text(180),
								get_text(181),
								get_text(182),
								get_text(183),
								get_text(184),
								get_text(185)) - 1;

						if (spell != -2) {
							spell += 12;
							inc_spell(spell);
						}
						break;
					}
					case 2: {
						spell = gui_radio((Bit8u*)get_text(156), 6,
								get_text(186),
								get_text(187),
								get_text(188),
								get_text(189),
								get_text(190),
								get_text(191)) - 1;

						if (spell != -2) {
							spell += 18;
							inc_spell(spell);
						}
						break;
					}
					case 3: {
						spell = gui_radio((Bit8u*)get_text(156), 3,
								get_text(192),
								get_text(193),
								get_text(194)) - 1;

						if (spell != -2) {
							spell += 24;
							inc_spell(spell);
						}
						break;
					}
				}
				}
				break;
			}
			case 7: {
				group = gui_radio((Bit8u*)get_text(155), 3,
						get_text(161), get_text(163),
						get_text(164));
				if (group != -1) {

#if !defined(__BORLANDC__)
				switch (group)
#else
				switch (_AX)
#endif
				{
					case 1: {
						spell = gui_radio((Bit8u*)get_text(156), 6,
								get_text(195),
								get_text(196),
								get_text(197),
								get_text(198),
								get_text(199),
								get_text(200)) - 1;

						if (spell != -2) {
							spell += 27;
							inc_spell(spell);
						}
						break;
					}
					case 2: {
						spell = gui_radio((Bit8u*)get_text(156), 7,
								get_text(206),
								get_text(207),
								get_text(208),
								get_text(209),
								get_text(210),
								get_text(211),
								get_text(212)) - 1;

						if (spell != -2) {
							spell += 38;
							inc_spell(spell);
						}
						break;
					}
					case 3: {
						spell = gui_radio((Bit8u*)get_text(156), 2,
								get_text(213),
								get_text(214)) - 1;

						if (spell != -2) {
							spell += 45;
							inc_spell(spell);
						}
						break;
					}
				}
				}
				break;
			}
			case 8: {
				group = gui_radio((Bit8u*)get_text(155), 3,
						get_text(164), get_text(86),
						get_text(166));
				if (group != -1) {

#if !defined(__BORLANDC__)
				switch (group)
#else
				switch (_AX)
#endif
				{
					case 1: {
						spell = gui_radio((Bit8u*)get_text(156), 2,
								get_text(215),
								get_text(216)) - 1;

						if (spell != -2) {
							spell += 47;
							inc_spell(spell);
						}
						break;
					}
					case 2: {
						spell = gui_radio((Bit8u*)get_text(156), 9,
								get_text(217), get_text(218), get_text(219),
								get_text(220), get_text(221), get_text(222),
								get_text(223), get_text(224), get_text(225)) - 1;

						if (spell != -2) {
							spell += 49;
							inc_spell(spell);
						}
						break;
					}
					case 3: {
						spell = gui_radio((Bit8u*)get_text(156), 2,
								get_text(226),
								get_text(227)) - 1;


						if (spell != -2) {
							spell += 58;
							inc_spell(spell);
						}
						break;
					}
				}
				}
				break;
			}
			case 9: {
				spell = gui_radio((Bit8u*)get_text(156), 16,
						get_text(228), get_text(229),
						get_text(230), get_text(231),
						get_text(232), get_text(233),
						get_text(234), get_text(235),
						get_text(236), get_text(237),
						get_text(238), get_text(239),
						get_text(240), get_text(241),
						get_text(242), get_text(243)) - 1;

				if (spell != -2) {
					spell += 60;
					inc_spell(spell);
				} else {
					group = -1;
				}
				break;
			}
			case 10: {
				spell = gui_radio((Bit8u*)get_text(156), 10,
						get_text(244), get_text(245),
						get_text(246), get_text(247),
						get_text(248), get_text(249),
						get_text(250), get_text(251),
						get_text(252), get_text(253)) - 1;

				if (spell != -2) {
					spell += 76;
					inc_spell(spell);
				} else {
					group = -1;
				}
				break;
			}
		}

		ds_writew(0x1327, 0);

	} while (group != -1);
}

/* Borlandified and identical */
void choose_atpa()
{
	Bit16s skill;
	Bit16s increase;

	ds_writew(0x1327, 0xffb0);

	do {
		/* print menu with all melee weapons skills */
		skill = gui_radio((Bit8u*)get_text(78), 7,
			get_text(95), get_text(96), get_text(97), get_text(98),
			get_text(99), get_text(100), get_text(101)) - 1;

		if (skill != -2) {
			if (ds_readbs(HERO_SKILLS + skill) <= 0) {
				infobox(get_text(260), 0);
			} else {

				increase = gui_radio((Bit8u*)get_text(254), 2,
					get_text(75), get_text(76));

				if (increase != -1) {
					if (increase == 1) {
						/* increase attack */
						if (ds_readbs(HERO_SKILLS + skill) >= 0 &&
							ds_readbs(HERO_PA_WEAPON + skill) > ds_readbs(HERO_ATPA_BASE)) {
							/* inc AT */
							ds_inc_bs_post(HERO_AT_WEAPON + skill);
							/* dec PA */
							ds_dec_bs_post(HERO_PA_WEAPON + skill);
							refresh_screen();
						} else {
							infobox(get_text(255), 0);
						}
					} else {
						if (ds_readbs(HERO_SKILLS + skill) >= 0 &&
							ds_readbs(HERO_AT_WEAPON + skill) > ds_readbs(HERO_ATPA_BASE)) {
							/* dec AT */
							ds_dec_bs_post(HERO_AT_WEAPON + skill);
							/* inc PA */
							ds_inc_bs_post(HERO_PA_WEAPON + skill);
							refresh_screen();
						} else {
							infobox(get_text(256), 0);
						}
					}
				}
			}
		}

	} while (skill != -2);

	ds_writew(0x1327, 0);
}

#if 1

/**
 * choose_typus() - choose a typus manually
 *
 */
void choose_typus()
{
	char name_bak[20];
	unsigned short i, typus_names;
	signed short choosen_typus;
	unsigned char randval;
	char sex_bak;

	if (!gui_bool((Bit8u*)get_text(264)))
		return;

	if (ds_readbs(HERO_SEX))
		/* famale typus names */
		typus_names = 271;
	else
		/* male tyuse names */
		typus_names = 17;

	choosen_typus = gui_radio((Bit8u*)get_text(30), 12,
				get_text(typus_names + 1), get_text(typus_names + 2),
				get_text(typus_names + 3), get_text(typus_names + 4),
				get_text(typus_names + 5), get_text(typus_names + 6),
				get_text(typus_names + 7), get_text(typus_names + 8),
				get_text(typus_names + 9), get_text(typus_names + 10),
				get_text(typus_names + 11), get_text(typus_names + 12));

	if (choosen_typus == -1)
		return;

	/* clear the hero area with saved name and sex */
	strcpy(name_bak, (char*)p_datseg + HERO_NAME);
	sex_bak = ds_readbs(HERO_SEX);

#if !defined(__BORLANDC__)
	bc_memset(RealMake(datseg, HERO_NAME), 0, 0x6da);
#else
	bc_memset(&ds[HERO_NAME], 0, 0x6da);
#endif
	clear_hero();
	ds_writeb(HERO_SEX, sex_bak);

	strcpy((char*)p_datseg + HERO_NAME, name_bak);

	/* set typus */
	ds_writeb(HERO_TYPUS, (unsigned char)choosen_typus);

	/* roll out good attribute values */
	for (i = 0; i < 7; i ++) {

		randval = (unsigned char)random_interval_gen(8, 13);

		if (randval > 8)
			randval--;

		ds_writeb(HERO_ATT0_NORMAL + 3 * i, randval);
		ds_writeb(HERO_ATT0_CURRENT + 3 * i, randval);
	}

	/* roll out bad attribute values */
	for (i = 0; i < 7; i ++) {

		randval = (unsigned char)random_interval_gen(2, 7);

		if (randval < 7)
			randval++;

		ds_writeb(HERO_ATT0_NORMAL + 3 * (i + 7), randval);
		ds_writeb(HERO_ATT0_CURRENT + 3 * (i + 7), randval);
	}

	/* adjust typus attribute requirements */
	for (i = 0; i < 4; i++) {
		Bit8u ta;
		/* calc pointer to attribute */
		ta = reqs[choosen_typus][i].attrib;

		/* get the required value */
		randval = reqs[choosen_typus][i].requirement;

		if (randval == 1)
			continue;

		if (randval & 0x80) {
			/* attribute upper bound */
			if (ds_readbs(HERO_ATT0_NORMAL + 3 * ta) <= (randval & 0x7f))
				continue;


			ds_writeb(HERO_ATT0_CURRENT + 3 * ta, randval & 0x7f);
			ds_writeb(HERO_ATT0_NORMAL + 3 * ta, randval & 0x7f);
		} else {
			/* attribute lower bound */
			if (ds_readbs(HERO_ATT0_NORMAL + 3 * ta) >= randval)
				continue;

			ds_writeb(HERO_ATT0_CURRENT + 3 * ta, randval);
			ds_writeb(HERO_ATT0_NORMAL + 3 * ta, randval);
		}
	}

	load_typus(ds_readbs(HERO_TYPUS));
	update_mouse_cursor();
	call_fill_rect_gen((RealPt)ds_readd(VGA_MEMSTART), 16, 8, 143, 191, 0);
	wait_for_vsync();
	set_palette(Real2Host(ds_readd(GEN_PTR5)) + 0x5c02, 0, 32);
	call_mouse();

	if (ds_readbs(HERO_TYPUS) > 10)
		ds_writeb(HEAD_TYPUS, 10);
	else
		ds_writeb(HEAD_TYPUS, ds_readbs(HERO_TYPUS));

	if (ds_readbs(HERO_SEX)) {
		ds_writeb(HEAD_CURRENT, ds_readb(HEAD_FIRST_FEMALE + ds_readb(HEAD_TYPUS)));
		ds_writeb(HEAD_FIRST, ds_readb(HEAD_FIRST_FEMALE + ds_readb(HEAD_TYPUS)));
		ds_writeb(HEAD_LAST, ds_readb(HEAD_FIRST_MALE + ds_readb(HEAD_TYPUS) + 1) - 1);
	} else {
		ds_writeb(HEAD_CURRENT, ds_readb(HEAD_FIRST_MALE + ds_readb(HEAD_TYPUS)));
		ds_writeb(HEAD_FIRST, ds_readb(HEAD_FIRST_MALE + ds_readb(HEAD_TYPUS)));
		ds_writeb(HEAD_LAST, ds_readb(HEAD_FIRST_FEMALE + ds_readb(HEAD_TYPUS)) - 1);
	}
	fill_values();
	ds_writew(SCREEN_VAR, 1);
}

void pal_fade_out(Bit8u *dst, Bit8u *src, Bit16u n)
{
	struct struct_color *d = (struct struct_color*)dst;
	struct struct_color *s = (struct struct_color*)src;
	Bit16u i;

	for (i = 0; i < n; i++) {
		if (s[i].r < d[i].r) {
			d[i].r--;
		} else {
			if (s[i].r > d[i].r)
				d[i].r++;
		}

		if (s[i].g < d[i].g) {
			d[i].g--;
		} else {
			if (s[i].g > d[i].g)
				d[i].g++;
		}

		if (s[i].b < d[i].b) {
			d[i].b--;
		} else {
			if (s[i].b > d[i].b)
				d[i].b++;
		}
	}
}

void pal_fade_in(Bit8u *dst, Bit8u *src, Bit16u col, Bit16u n)
{
	Bit16u i;
	Bit16u si;

	si = 0x40 - col;

	for (i = 0; i < n; i++) {
		if (host_readb(src + 3 * i) >= si) {
			if (host_readb(src + i * 3) > host_readb(dst + i * 3))
				host_writeb(dst + 3 * i,
					host_readb(dst + 3 * i) + 1);
		}

		if (host_readb(src + 3 * i + 1) >= si) {
			if (host_readb(src + i * 3 + 1) > host_readb(dst + i * 3 + 1))
				host_writeb(dst + 3 * i + 1,
					host_readb(dst + 3 * i + 1) + 1);
		}

		if (host_readb(src + 3 * i + 2) >= si) {
			if (host_readb(src + i * 3 + 2) > host_readb(dst + i * 3 + 2))
				host_writeb(dst + 3 * i + 2,
					host_readb(dst + 3 * i + 2) + 1);
		}
	}
}

#if !defined(__BORLANDC__)
static Bit16u fd_read_datfile(FILE * fd, Bit8u *buf, Bit16u len);
static FILE * fd_open_datfile(Bit16u);

void BE_cleanup()
{
	long sum = 0;

	bc_free((RealPt)ds_readd(BUFFER_SEX_DAT));
	bc_free((RealPt)ds_readd(BUFFER_POPUP));
	bc_free((RealPt)ds_readd(BUFFER_HEADS_DAT));

	if ((RealPt)ds_readd(BUFFER_TEXT)) {
		bc_free((RealPt)ds_readd(BUFFER_TEXT));
	}

	bc_free((RealPt)ds_readd(BUFFER_FONT6));

	//buffer_sex_dat = NULL;
	//buffer_popup_nvf = NULL;
	//buffer_heads_dat = NULL;
	//buffer_text = NULL;
	//buffer_font6 = NULL;

	bc_free((RealPt)ds_readd(PICBUF3));
	bc_free((RealPt)ds_readd(PICBUF2));
	bc_free((RealPt)ds_readd(PICBUF1));
	bc_free((RealPt)ds_readd(GEN_PTR6) - 8);
	bc_free((RealPt)ds_readd(BUFFER_DMENGE_DAT));
	bc_free((RealPt)ds_readd(GEN_PTR5));
	bc_free((RealPt)ds_readd(GEN_PTR4));
	bc_free((RealPt)ds_readd(GEN_PTR2));

	//picbuf3 = NULL;
	//picbuf2 = NULL;
	//picbuf1 = NULL;
	//gen_ptr6 = NULL;
	//buffer_dmenge_dat = NULL;
	//gen_ptr5 = NULL;
	//gen_ptr4 = NULL;
	//gen_ptr2 = NULL;

	bc_free((RealPt)ds_readd(PAGE_BUFFER));
	bc_free((RealPt)ds_readd(GEN_PTR1_DIS) - 8);

	//page_buffer = NULL;
	//gen_ptr1 = NULL;

	// missed ones
	if (ds_readd(SND_PTR_UNKN1))
		bc_free((RealPt)ds_readd(SND_PTR_UNKN1));
	if (ds_readd(STATE_TABLE))
		bc_free((RealPt)ds_readd(STATE_TABLE));
	if (ds_readd(SND_DRIVER))
		bc_free((RealPt)ds_readd(SND_DRIVER));
	if (ds_readd(FORM_XMID))
		bc_free((RealPt)ds_readd(FORM_XMID));

	for (int i = 0; i < MAX_PAGES; i++) {
		if (ds_readd(BG_BUFFER + 4 * i)) {
			bc_free((RealPt)ds_readd(BG_BUFFER + 4 * i));
		}
	}

	for (int i = 0; i < MAX_TYPES; i++) {
		if (ds_readd(TYPUS_BUFFER + 4 * i)) {
			bc_free((RealPt)ds_readd(TYPUS_BUFFER + 4 * i));
		}
	}
}

static FILE * fd_open_datfile(Bit16u index)
{
	FILE *fd;
	char *fname;
	signed int offset;
	Bit8u buf[800];


	/* build the path to DSAGEN.DAT */
	fname = get_pwd();
	strncat(fname, "DSAGEN.DAT", 10);
	prepare_path(fname);

	fd = fopen(fname, "rb");

	if (fd == NULL) {
		D1_ERR("%s(): failed to open datafile at %s\n",
			__func__, fname);
		free(fname);
		return NULL;
	}
	free(fname);

	if (fread(buf, 1, 800, fd) != 800) {
		D1_ERR("%s(): failed to read datafile\n", __func__);
		fclose(fd);
		return NULL;
	}


	offset = get_archive_offset(fnames_g105de[index], buf);
	ds_writed(GENDAT_OFFSET, offset);

	if (ds_readd(GENDAT_OFFSET) == 0xffffffff) {
		D1_ERR("FILE %s IS MISSING!", fnames_g105de[index]);
		fclose(fd);
		return NULL;
	}

	fseek(fd, ds_readd(GENDAT_OFFSET), SEEK_SET);

	return fd;

}


static Bit16u fd_read_datfile(FILE * fd, Bit8u *buf, Bit16u len)
{

	if (len > ds_readd(FLEN_LEFT))
		len = (unsigned short)ds_readd(FLEN_LEFT);

	len = fread(buf, 1, len, fd);

	ds_writed(FLEN_LEFT, ds_readd(FLEN_LEFT) - len);

	return len;
}
#endif


/**
 *	intro() - play the intro
 */
void intro()
{
	struct nvf_desc nvf;
	signed short tmp;

	FILE *fd;
	Bit8u *pal_dst, *pal_src;
	Bit16u flen;
	Bit16s i;
	Bit8u cnt1;
	Bit8s cnt2;

	ds_writeb(0x40b8, 1);

	/* load ATTIC */
	fd = fd_open_datfile(18);
	if (fd == NULL) {
		D1_ERR("Failed to open\n");
		exit(0);
	}
	fd_read_datfile(fd, Real2Host(ds_readd(BUFFER_HEADS_DAT)), 20000);
	fclose(fd);

	nvf.src = (RealPt)ds_readd(BUFFER_HEADS_DAT);
	nvf.type = 0;
	nvf.width = &tmp;
	nvf.height = &tmp;

	for (i = 7; i >= 0; i--) {
		nvf.dst = (RealPt)ds_readd(GEN_PTR1_DIS) + i * 960 + 9600;
		nvf.no = i + 1;
		process_nvf(&nvf);

	}
	/* set dst */
	nvf.dst = (RealPt)ds_readd(GEN_PTR1_DIS);
	/* set no */
	nvf.no = 0;
	process_nvf(&nvf);

	wait_for_vsync();

	set_palette((Bit8u*)&pal_attic, 0, 16);

	cnt1 = 1;
	cnt2 = 99;

	/* glowing at the bottom */
	for (i = 0; i < 4; i++) {
		ds_writew(DST_X1, 112);
		ds_writew(DST_Y1, 140);
		ds_writew(DST_X2, 207);
		ds_writew(DST_Y2, 149);
		ds_writed(DST_SRC, ds_readd(GEN_PTR1_DIS) + i * 960 + 9600);
		do_draw_pic(0);
		vsync_or_key(20);
	}

	/* elevate the attic logo */
	i = 4;
	ds_writew(IN_KEY_EXT, 0);
	while ((cnt1 <= 100) && (ds_readw(IN_KEY_EXT) == 0)) {
		ds_writew(DST_X1, 0);
		ds_writew(DST_Y1, cnt2 + 60);
		ds_writew(DST_X2, 95);
		ds_writew(DST_Y2, cnt1 + cnt2 + 59);
		ds_writed(DST_DST, ds_readd(GEN_PTR1_DIS));
		ds_writed(DST_SRC, ds_readd(GEN_PTR1_DIS));
		do_draw_pic(0);

		if (cnt1 != 100) {

			ds_writed(DST_SRC, ds_readd(GEN_PTR1_DIS) + i * 960 + 9600);
			if (cnt1 % 4 == 1)
				i++;

			if (i == 8)
				i = 4;

			ds_writew(DST_X1, 0);
			ds_writew(DST_Y1, 150);
			ds_writew(DST_X2, 95);
			ds_writew(DST_Y2, 159);
			ds_writed(DST_DST, ds_readd(GEN_PTR1_DIS));
			do_draw_pic(2);
		}

		ds_writew(DST_X1, 112);
		ds_writew(DST_Y1, 50);
		ds_writew(DST_X2, 207);
		ds_writew(DST_Y2, 149);
		ds_writed(DST_SRC, ds_readd(GEN_PTR1_DIS));

		ds_writew(UNKN1, 0);
		ds_writew(UNKN2, 60);
		ds_writew(UNKN3, 95);
		ds_writew(UNKN4, 159);
		ds_writed(DST_DST, ds_readd(VGA_MEMSTART));
		do_draw_pic(3);
		cnt1++;
		cnt2--;
		if (cnt1 < 37)
			vsync_or_key(2);
		else
			vsync_or_key(1);
	}

	if (ds_readw(IN_KEY_EXT) == 0)
		vsync_or_key(200);

	/* load FANPRO.NVF */
	fd = fd_open_datfile(34);
	if (fd == NULL) {
		D1_ERR("Failed to open\n");
		exit(0);
	}
	flen = fd_read_datfile(fd, Real2Host(ds_readd(BUFFER_HEADS_DAT)), 20000);
	fclose(fd);

	nvf.dst = (RealPt)ds_readd(GEN_PTR1_DIS);
	nvf.src = (RealPt)ds_readd(BUFFER_HEADS_DAT);
	nvf.no = 0;
	nvf.type = 0;
	nvf.width = &tmp;
	nvf.height = &tmp;

	process_nvf(&nvf);

	/* clear screen */
	call_fill_rect_gen((RealPt)ds_readd(VGA_MEMSTART), 0, 0, 319, 199, 0);
	wait_for_vsync();

	/* set palette of FANPRO.NVF */
	set_palette(Real2Host(ds_readd(BUFFER_HEADS_DAT)) + flen - 32*3, 0, 32);

	/* draw the picture */
	ds_writew(DST_X1, 60);
	ds_writew(DST_Y1, 50);
	ds_writew(DST_X2, 259);
	ds_writew(DST_Y2, 149);
	ds_writed(DST_SRC, ds_readd(GEN_PTR1_DIS));
	do_draw_pic(0);
	vsync_or_key(200);

	/* load DSALOGO.DAT */
	fd = fd_open_datfile(16);
	if (fd == NULL) {
		D1_ERR("Failed to open\n");
		exit(0);
	}
	fd_read_datfile(fd, Real2Host(ds_readd(BUFFER_HEADS_DAT)), 20000);
	fclose(fd);

	nvf.dst = (RealPt)ds_readd(GEN_PTR1_DIS);
	nvf.src = (RealPt)ds_readd(BUFFER_HEADS_DAT);
	nvf.no = 0;
	nvf.type = 0;
	nvf.width = &tmp;
	nvf.height = &tmp;

	process_nvf(&nvf);

	/* clear screen */
	call_fill_rect_gen((RealPt)ds_readd(VGA_MEMSTART), 0, 0, 319, 199, 0);
	wait_for_vsync();


	set_palette((Bit8u*)pal_tmp, 0, 32);

	/* draw DSALOGO.DAT */
	ds_writew(DST_X1, 0);
	ds_writew(DST_Y1, 0);
	ds_writew(DST_X2, 319);
	ds_writew(DST_Y2, 99);
	ds_writed(DST_SRC, ds_readd(GEN_PTR1_DIS));
	do_draw_pic(0);

	/* load GENTIT.DAT */
	fd = fd_open_datfile(17);
	if (fd == NULL) {
		D1_ERR("Failed to open\n");
		exit(0);
	}
	fd_read_datfile(fd, Real2Host(ds_readd(BUFFER_HEADS_DAT)), 20000);
	fclose(fd);

	nvf.dst = (RealPt)ds_readd(GEN_PTR1_DIS);
	nvf.src = (RealPt)ds_readd(BUFFER_HEADS_DAT);
	nvf.no = 0;
	nvf.type = 0;
	nvf.width = &tmp;
	nvf.height = &tmp;

	process_nvf(&nvf);

	/* draw DSALOGO.DAT */
	ds_writew(DST_X1, 10);
	ds_writew(DST_Y1, 110);
	ds_writew(DST_X2, 329);
	ds_writew(DST_Y2, 159);
	ds_writed(DST_SRC, ds_readd(GEN_PTR1_DIS));
	do_draw_pic(0);

	memcpy(Real2Host(ds_readd(GEN_PTR1_DIS)) + 500, &pal_dsalogo, 96);

	pal_src = Real2Host(ds_readd(GEN_PTR1_DIS)) + 500;
	pal_dst = Real2Host(ds_readd(GEN_PTR1_DIS));
	memset(pal_dst, 0, 96);

	for (i = 0; i < 64; i++) {
		pal_fade_in(pal_dst, pal_src, i, 32);
		wait_for_vsync();
		set_palette(pal_dst, 0, 32);
	}

	set_textcolor(0xff, 0x00); // WHITE ON BLACK
	print_str(version, 290, 190);
	vsync_or_key(400);

	memcpy(Real2Host(ds_readd(GEN_PTR1_DIS)), &pal_dsalogo, 96);

	pal_src = Real2Host(ds_readd(GEN_PTR1_DIS)) + 500;
	pal_dst = Real2Host(ds_readd(GEN_PTR1_DIS));
	memset(pal_src, 0, 96);

	for (i = 0; i < 64; i++) {
		pal_fade_out(pal_dst, pal_src, 32);
		wait_for_vsync();
		set_palette(pal_dst, 0, 32);
	}

	/* clear screen */
	call_fill_rect_gen((RealPt)ds_readd(VGA_MEMSTART), 0, 0, 319, 199, 0);

	ds_writeb(0x40b8, 0);
	return;
}

void set_mouse_isr()
{
	/* save adress of the old ISR */
	ds_writed(0x247c, RealGetVec(0x1c));
	/* set a the new one */
	RealSetVec(0x1c, RealMake(reloc_gen + 0x3c6, 0x72b3));
}

void restore_mouse_isr()
{
	RealSetVec(0x1c, ds_readd(0x247c));
}

int main_gen(int argc, char **argv)
{
	Bit16u sound_off = 0;


	if (sizeof(struct struct_hero) != 0x6da) {
		D1_ERR("Error: sizeof(struct_hero) == 0x%lx != 0x6da\n",
			sizeof(struct struct_hero));
		exit(1);
	}

	if (argc > 1)
		ds_writew(CALLED_WITH_ARGS, 1);

	if (argc > 2)
		ds_writew(PARAM_LEVEL, argv[2][0]);

	if ((argc > 3) && (argv[3][0] == '0')) {
		ds_writew(MIDI_DISABLED, 1);
		sound_off = 1;
	};

	ds_writeb(0x40b8, 1);

	if (sound_off == 0)
		init_music(13000);

	ds_writew(WO_VAR2, ret_zero1());

	set_mouse_isr();

	bc_randomize();

	save_display_stat(RealMake(datseg, 0x47db));

	alloc_buffers();
	//alloc_buffers_emu();

	ds_writew(WO_VAR3, 2);

	init_video();

	ds_writew(HAVE_MOUSE, 2);

	mouse_enable();

	if (ds_readws(HAVE_MOUSE) == 0)
		ds_writews(MOUSE_REFRESH_FLAG, -2);

	init_stuff();

	read_common_files();

	if (sound_off == 0)
		read_soundcfg();

	start_music(33);

	if (ds_readw(CALLED_WITH_ARGS) == 0) {
		intro();
		read_common_files();
	}

	init_colors();
	wait_for_keypress();
	call_mouse();
	do_gen();
	stop_music();
	update_mouse_cursor();
	mouse_disable();
	restore_mouse_isr();

	if (ds_readw(CALLED_WITH_ARGS) != 0) {
		call_fill_rect_gen((RealPt)ds_readd(VGA_MEMSTART), 0, 0, 319, 199, 0);
	} else {
		exit_video();
		bc_clrscr();
	}

	BE_cleanup();

	/* to make MSVC happy */
	return 0;
}

void alloc_buffers_emu()
{
	CALLBACK_RunRealFar(reloc_gen + 0x3c6, 0x7446);
}

void alloc_buffers()
{
	ds_writed(VGA_MEMSTART, RealMake(0xa000, 0x0));
	ds_writed(GFX_PTR, RealMake(0xa000, 0x0));

	ds_writed(GEN_PTR1_DIS, (RealPt)gen_alloc(64108) + 8);

	ds_writed(PAGE_BUFFER, (Bit32u)gen_alloc(50000));

	ds_writed(GEN_PTR2, (Bit32u)gen_alloc(1524));
	ds_writed(GEN_PTR3, (RealPt)ds_readd(GEN_PTR2) + 1500);

	// unused
	ds_writed(GEN_PTR4, (Bit32u)gen_alloc(200));

	ds_writed(BUFFER_TEXT, (Bit32u)gen_alloc(6000));

	ds_writed(BUFFER_FONT6, (Bit32u)gen_alloc(592));

	load_font_and_text();

	ds_writed(BUFFER_HEADS_DAT, (Bit32u)gen_alloc(39000));

	ds_writed(BUFFER_POPUP, (Bit32u)gen_alloc(1673));

	ds_writed(BUFFER_SEX_DAT, (Bit32u)gen_alloc(812));

	ds_writed(GEN_PTR5, (Bit32u)gen_alloc(23660));

	ds_writed(BUFFER_DMENGE_DAT, (Bit32u)gen_alloc(23660));

	ds_writed(PICBUF1, (Bit32u)gen_alloc(800));

	ds_writed(PICBUF2, (Bit32u)gen_alloc(2800));

	ds_writed(PICBUF3, (Bit32u)gen_alloc(2800));

	ds_writed(GEN_PTR6, (Bit32u)gen_alloc(1100) + 8);

	if (!(RealPt)ds_readd(GEN_PTR6))
		printf("\nMEMORY MALLOCATION ERROR!");
#if !defined(__BORLANDC__)
	memset(p_datseg + TYPUS_BUFFER, 0, 4 * 13);
	memset(p_datseg + TYPUS_LEN, 0, 4 * 13);
#endif
}

void init_colors()
{
	set_palette((Bit8u*)&col_black, 0x00, 1);
	set_palette((Bit8u*)&col_white, 0xff, 1);
	set_palette((Bit8u*)col_popup, 0xd8, 8);
	set_palette((Bit8u*)col_misc, 0xc8, 3);
	set_palette((Bit8u*)pal_genbg, 0x40, 0x20);
	set_palette((Bit8u*)pal_heads, 0x20, 0x20);
	set_textcolor(0xff, 0x0); // WHITE ON BLACK
}

void init_stuff()
{
	init_colors();

	/* these 3 variables are different text colors */
	ds_writew(FG_COLOR + 2, 0x0c8); // RED
	ds_writew(FG_COLOR + 4, 0x0c9); // YELLOW
	ds_writew(FG_COLOR + 6, 0x0ca); // BLUE

	/* number of menu tiles width */
	ds_writew(MENU_TILES, 3);

	ds_writed(DST_DST, ds_readd(VGA_MEMSTART));
}

RealPt gen_alloc(Bit32u nelem)
{
	RealPt p = (RealPt)0;
	p = bc_calloc(nelem, 1);
	//D1_INFO("EMU gen_alloc(%ld) = 0x%08x;\n", nelem, p);
	return p;
}

#endif

#if !defined(__BORLANDC__)
}
#endif
