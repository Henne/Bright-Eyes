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

/* DS:0x0158 */
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
/* DS:0x03cf */
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
/* DS:0x0437 */
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

/* DS:0x06db */
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

/* DS:0x08df */
static const unsigned short init_le[MAX_TYPES + 1] = {	0,
						30, 30, 30, 30, 30, 40,
						25, 30, 25, 25, 30, 25};
/* DS:0x08f9 */
static const unsigned short init_ae[MAX_TYPES + 1] = {	0,
						0, 0, 0, 0, 0, 0,
						25, 25, 30, 25, 25, 25};
/* DS:0x0913 */
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
/* DS:0x092d */
static const unsigned short weight_mod[13] = {
	0,
	120, 110, 100, 110, 100, 90,
	120, 110, 110, 120, 120, 120
};
/* DS:0x093a */
static const signed char mr_mod[13] = {
	0,
	2, 0, 0, 2, -2, 2,
	2, 2, 2, 3, 4, 3
};

/* DS:0x0a85 */
static const unsigned char initial_skill_incs[12] = {
	20, 20, 20, 20, 20, 20, 20, 20, 15, 20, 20, 20
};
/* DS:0x0a91 */
static const signed char initial_spell_incs[6] = {
	25, 25, 40, 20, 20, 20
};
/* DS:0x0a97 */
static const signed char initial_conv_incs[6] = {
	5, 5, 5, 0, 0, 0
};

struct struct_house_mod {
	signed char no;
	signed short spells[7], mod[7];
};

/* DS:0x0a9d */
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


/* DS:0x0ba2 */
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
/* DS:0x0e2c */
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

/* DS:0x1048 */
static const signed char head_first_male[12] = {	0, 0, 6, 12,
							18, 24,	30, 36,
							42, 48, 54, 62 };
/* DS:0x1054 */
static const signed char head_first_female[11] = {	0, 3, 9, 15,
							21, 27, 34, 37,
							46, 51, 58 };
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

/* DS:0x109a */
static const unsigned char unused_ro1 = 0;

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

/* DS:0x1329 */
static const Bit16u ro_zero = 0;

static struct struct_hero hero;

//static unsigned short use_cda;
//static unsigned short eh_installed;
static Bit8u *bg_buffer[MAX_PAGES] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static Bit32s bg_len[MAX_PAGES] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static Bit8u *typus_buffer[MAX_TYPES] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static Bit32s typus_len[MAX_TYPES] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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

static const char* str_file_missing[] = { "FILE %s IS MISSING!" };

struct struct_chr_lookup {
	unsigned char chr, idx, width;
};
/* DS:0x1b85 */
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

/* DS:0x1c77 */
static unsigned short bool_mode;

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

/* DS:0x1ca5 */
static unsigned short need_refresh = 1;

/* DS:0x1ca6 */
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

/* DS:0x2780 */
static unsigned short got_ch_bonus;
/* DS:0x2782 */
static unsigned short got_mu_bonus;

//static Bit32s FLEN;
//static Bit32s FLEN_LEFT;

//signed short HANDLE_TIMBRE;

//void *snd_driver;
//void *form_xmid;
//void *snd_ptr_unkn1;
//void *state_table;

/* DS:0x3f5e */
static Bit8u param_level;

struct inc_states {
	char tries;
	char incs;
};

/* DS:0x3f62 */
static struct inc_states spell_incs[86];
/* DS:0x400e */
static struct inc_states skill_incs[52];
/* DS:0x4076 */
static char attrib_changed[14];
/* DS:0x4084 */
static char *type_names[MAX_TYPES];
/* DS:0x40B4 */
/* the index of the last head */
static signed char head_last;
/* DS:0x40B5 */
/* the index of the first head */
static signed char head_first;
/* the index of the current head */
//static signed char head_current;
/* DS:0x40B7 */
/* the typus for the heads e.G. all elves are 10 */
static signed char head_typus;

//static unsigned short MENU_TILES;
/* DS:0x40bb */
static unsigned short left_border;
/* DS:0x40bd */
static unsigned short upper_border;

/* DS:0x40bf */
static signed short level;


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

static char *texts[300];

#if !defined(__BORLANDC__)
static inline char* get_text(Bit16s no) {
	return texts[no];
}
#else
#define get_text(no) ((char*)Real2Host(ds_readd(TEXTS + 4 * (no))))
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

static Bit8u *buffer_heads_dat;
//static Bit8u *buffer_text;
//static Bit8u *buffer_font6;
/* DS:0x477d */
static Bit16u col_index;
/* DS:0x477f */
static Bit16u bg_color;
/* DS:0x4781 */
static Bit16u fg_color[6];
/* DS:0x478d */
static Bit16u text_x_end;
/* DS:0x478f */
static Bit16u text_y;
/* DS:0x4791 */
static Bit16u text_x;

/* DS:0x4797 */
static Bit8u *picbuf3;
static Bit8u *picbuf2;
static Bit8u *picbuf1;
//static Bit8u *gen_ptr6;
//static Bit8u *buffer_dmenge_dat;

static Bit8u *gen_ptr5;

static Bit8u *gen_ptr4;
//static char *gen_ptr3;
//static char *gen_ptr2;

Bit8u *page_buffer;

/* DS:0x47d7 */
static unsigned short wo_var2;
/* DS:0x47d9 */
static unsigned short wo_var3;

/* DS:0x47e3 */
static const Bit16u ro_var[7] = {0, 0, 0, 0, 0, 0, 0};

/* DS:0x47f3 */
Bit8u *gen_ptr1;
Bit8u *gen_ptr1_dis;

#if !defined(__BORLANDC__)
/* New variables */

/*	This is the buffer, where the hero is stored in little endian.
	This is neccessary to support Big Endian machines.
*/
static char hero_out[1754];

static inline void hero_writeb(unsigned off, char v)
{
	host_writeb((Bit8u*)hero_out + off - HERO_NAME, v);
}

static inline void hero_writew(unsigned off, short v)
{
	host_writew((Bit8u*)hero_out + off - HERO_NAME, v);
}

static inline void hero_writed(unsigned off, int v)
{
	host_writed((Bit8u*)hero_out + off - HERO_NAME, v);
}

static void update_hero_out()
{
	unsigned long i;

	memset(hero_out, 0, 1754);

	strncpy(hero_out, hero.name, 16);
	strncpy(hero_out + 0x10, hero.alias, 16);

	hero_writeb(HERO_TYPUS, hero.typus);
	hero_writeb(0x134e, hero.sex);
	hero_writeb(0x134f, hero.height);
	hero_writew(0x1350, hero.weight);
	hero_writeb(0x1352, hero.god);
	hero_writeb(0x1353, hero.level);
	hero_writed(0x1358, hero.money);

	for (i = 0; i < 14; i++) {
		hero_writeb(0x1360 + i * 3 + 0, hero.attribs[i].normal);
		hero_writeb(0x1360 + i * 3 + 1, hero.attribs[i].current);
		hero_writeb(0x1360 + i * 3 + 2, hero.attribs[i].mod);
	}

	hero_writew(0x138a, hero.le);
	hero_writew(0x138c, hero.le_max);
	hero_writew(0x138e, hero.ae);
	hero_writew(0x1390, hero.ae_max);
	hero_writeb(0x1392, hero.mr);
	hero_writeb(0x1393, hero.atpa);

	for (i = 0; i < 7; i++)
		hero_writeb(0x1394 + i, hero.at[i]);
	for (i = 0; i < 7; i++)
		hero_writeb(0x139b + i, hero.pa[i]);

	hero_writeb(HERO_GROUP, hero.group);

	for (i = 0; i < 52; i++)
		hero_writeb(0x1434 + i, hero.skills[i]);
	hero_writeb(0x1468, hero.skill_incs);
	for (i = 0; i < 86; i++)
		hero_writeb(0x1469 + i, hero.spells[i]);
	hero_writeb(0x14bf, hero.spell_incs);
	hero_writeb(0x14c0, hero.school);
	hero_writeb(0x14c1, hero.staff_level);

	memcpy(hero_out + 0x2da, &hero.pic, 1024);
}


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
	Bit16u port;

	ds_writew(USE_CDA, 0);
	ds_writew(MIDI_DISABLED, 1);

#if !defined(__BORLANDC__)
	handle = bc_open(RealMake(datseg, STR_SOUND_CFG), 0x8001);
#else
	/* This is checked in the following if statement */
	handle = bc_open("SOUND.CFG", 0x8001);
#endif

	if (handle != -1) {
		bc__read(handle, (Bit8u*)&port, 2);
		bc__close(handle);

#if !defined(__BORLANDC__)
		/* Small hack: enable MIDI instead of CD-Audio, produces glich in attic ani */
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
	if (ds_writed(FORM_XMID, (Bit32s)emu_gen_alloc(size))) {
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
		ds_writed(SND_DRIVER, (Bit32s)emu_gen_alloc(size + 0x10));
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

	timbre_ptr = emu_gen_alloc(ds_readw(CURRENT_TIMBRE_LENGTH));

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
	Bit16u handle;

	if ((handle = open_datfile(index)) != 0xffff) {
		read_datfile(handle, Real2Host((RealPt)ds_readd(FORM_XMID)), 32767);
		bc_close(handle);
		return 1;
	}

	return 0;
}

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
				asm {nop; nop; nop; nop}
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
#if !defined(__BORLANDC__)
		play_sequence(0);
#else
	asm {nop; nop; nop; nop }
#endif
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

			ds_writew(0x1250, ds_readws(MOUSE_POSX));
			ds_writew(0x1252, ds_readws(MOUSE_POSY));
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
		// Sync BCC output
		//mouse();
		asm { nop; nop }
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
				ds_writew(0x4789, 1);
				infobox(get_text(267), 0);
				ds_writew(0x4789, 0);
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
	asm {nop;}
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

	rangeX = ds_readw(0x1250) - ds_readw(0x125a);
	rangeY = ds_readw(0x1252) - ds_readw(0x125c);
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
	split_textbuffer_host(texts, (char*)Real2Host(ds_readd(BUFFER_TEXT)), len);
#endif
}

#if !defined(__BORLANDC__)
void split_textbuffer_host(char **dst, char *src, Bit32u len)
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
		if ((RealPt)ds_readd(BG_BUFFER + 4 * page)) {
			decomp_rle(Real2Host(ds_readd(GEN_PTR1_DIS)),
					Real2Host(ds_readd(BG_BUFFER + 4 * page)),
					0, 0, 320, 200, 0);
			return;
		}

		if (ptr = emu_gen_alloc(get_filelength(handle = open_datfile(page)))) {
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
	if ((RealPt)ds_readd(TYPUS_BUFFER + 4 * typus)) {
		decomp_pp20((RealPt)ds_readd(GEN_PTR5),
			Real2Host(ds_readd(TYPUS_BUFFER + 4 * typus)),
			ds_readd(TYPUS_LEN + 4 * typus));
		return;
	}

	if (ptr = emu_gen_alloc(get_filelength(handle = open_datfile(index)))) {
		/* load the file into the typus buffer */
		ds_writed(TYPUS_BUFFER + 4 * typus, (Bit32u)ptr);
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

#if !defined(__BORLANDC__)
	// write all hero changes to DS (intermediately)
	update_hero_out();
	memcpy(p_datseg + HERO_NAME, hero_out, 1754);
#endif

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
#if !defined(__BORLANDC__)
	hero.group = 1;
#endif
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
		update_hero_out();
		fwrite(hero_out, 1, 1754, fd);
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
				fwrite(hero_out, 1, 1754, fd);
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
	;
	if ((Bit32s)(ds_writed(GENDAT_OFFSET, get_archive_offset((char*)Real2Host(ds_readd(FNAMES_G105de + 4* index)), buf))) != -1) {
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
Bit32s get_filelength(Bit16s unused) {

	return ds_readd(FLEN);
}

/* Borlandified and identical */
Bit16u ret_zero1() {
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

void call_fill_rect_gen(PhysPt ptr, Bit16u x1, Bit16u y1, Bit16u x2, Bit16u y2, Bit16u color)
{
	fill_rect(ptr + y1 * 320 + x1, (unsigned char)color, x2 - x1 + 1, y2 - y1 + 1);
}

#if 1

void wait_for_vsync()
{
	CALLBACK_RunRealFar(reloc_gen + 0x3c6, 0x2024);
}

/* static */
void blit_smth3(PhysPt ptr, Bit16u v1, Bit16u v2) {

	Bit8u *src;
	Bit16u i, j;

	src = MemBase + PhysMake(datseg, 0x45e1);

	for (i = 0; i < v1; src += 8 - v2, ptr += 320, i++)
		for (j = 0; j < v2; src++, j++)
			mem_writeb_inline(ptr + j, *src);
}

/* static */
Bit16u print_line(char *str)
{
	Bit16u lines = 1;

	update_mouse_cursor();

	lines = str_splitter(str);

	print_str(str, text_x, text_y);

	call_mouse();

	return lines;
}

/* static */
void print_str(char *str, Bit16u x, Bit16u y)
{
	Bit16u i, x_bak;
	unsigned char c;

	i = 0;

	update_mouse_cursor();

	if (ds_readw(0x4789) == 1)
		x = get_line_start_c(str, x, text_x_end);

	x_bak = x;

	while ((c = str[i++])) {
		switch (c) {
			case 0x0d: case 0x40: {
				/* newline */
				y += 7;

				if (ds_readw(0x4789) == 1)
					x = get_line_start_c(str + i,
						text_x,	text_x_end);
				else
					x = x_bak;
				break;
			}
			case 0x7e: {
				/* CRUFT */
				if (x < ro_var[0]) {
					x = ro_var[0];
					continue;
				}
				if (x < ro_var[1]) {
					x = ro_var[1];
					continue;
				}
				if (x < ro_var[2]) {
					x = ro_var[2];
					continue;
				}
				if (x < ro_var[3]) {
					x = ro_var[3];
					continue;
				}
				if (x < ro_var[4]) {
					x = ro_var[4];
					continue;
				}
				if (x < ro_var[5]) {
					x = ro_var[5];
					continue;
				}
				if (x < ro_var[6]) {
					x = ro_var[6];
					continue;
				}
				break;
		}
		case 0xf0: case 0xf1: case 0xf2: case 0xf3: {
			/* change text color */
			col_index = c - 0xf0;
			break;
		}
		default:
			/* print normal */
			x += print_chr(c, x, y);
		}
	}

	call_mouse();
}

/**
 * str_splitter() - sets the line breaks for a string
 * @s:	string
 *
 * Returns the number of lines the string needs.
 */
/* static */
Bit16u str_splitter(char *s) {

	char *tp;
	Bit16u last_space, lines, l_width, l_width_max;
	Bit16s i;
	unsigned char c_width;

	if (s == NULL || s == (char*)MemBase)
		return 0;

	lines = 1;
	l_width_max = text_x_end;


	/* replace all CR and LF with spaces */
	for (tp = s; *tp; tp++)
		if (*tp == 0x0d || *tp == 0x0a)
			*tp = 0x20;

	tp = s;

	l_width = i = last_space = 0;

	while (tp[i]) {
		get_chr_info(tp[i], &c_width);
		l_width += c_width;

		if (l_width >= l_width_max) {

			if (last_space) {
				tp[last_space] = 0xd;
				tp += last_space;
			} else {
				tp[i] = 0xd;
				tp += i + 1;
			}

			lines++;
			l_width = last_space = i = 0;
		}

		/* remember the last i in last_space */
		if (tp[i] == 0x20)
			last_space = i;

		if (tp[i] == 0x40) {
			tp += i + 1;
			i = -1;
			l_width = last_space = 0;
			lines++;
		}
		i++;
	}

	if (l_width >= l_width_max) {
		if (last_space) {
			tp[last_space] = 0xd;
			lines++;
		} else {
			tp[i - 1] = 0;
		}
	}

	return lines;
}

Bit16u print_chr(unsigned char c, Bit16u x, Bit16u y) {

	Bit16u idx;
	unsigned char width;

	idx = get_chr_info(c, &width);

	call_them_all(idx, width, x, y);

	return width;
}

/**
 * get_chr_info() - gets font information of a character
 * @c:		the character
 * @width:	pointer to save width
 *
 * Returns the font index.
 */
unsigned char get_chr_info(unsigned char c, unsigned char *width) {

	unsigned long i;

	for (i = 0; i != 74; i++) {
		/* search for the character */
		if (chr_lookup[i].chr != c)
			continue;

		*width = chr_lookup[i].width;
		return chr_lookup[i].idx;
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		*width = 0;
		return 0;
	} else {
		*width = 6;
		return 0;
	}
}

/* static */
void call_them_all(Bit16u v1, Bit16u v2, Bit16u x, Bit16u y) {

	PhysPt gfx_ptr;
	Bit32u bogus;

	fill_smth();
	fill_smth2((Bit8u*)Real2Host(ds_readd(BUFFER_FONT6)) + v1 * 8);

	gfx_ptr = get_gfx_ptr(x, y);
	bogus = ret_zero();

	call_blit_smth3(Real2Phys(gfx_ptr), 7, 0, 0, v2);
}

/* static */
void fill_smth() {

	Bit8u *ptr;
	Bit16u i, j;

	if (unused_ro1)
		ptr = MemBase + PhysMake(datseg, 0x45a1);
	else
		ptr = MemBase + PhysMake(datseg, 0x45e1);

	for (i = 0; i < 8; i++, ptr += 8)
		for (j = 0; j < 8; j++)
			host_writeb(ptr + j, (unsigned char)bg_color);
}

/* static */
void fill_smth2(Bit8u* ptr) {

	Bit8u *lp;
	Bit16u i, j;
	Bit8u lv;

	if (unused_ro1)
		lp = MemBase + PhysMake(datseg, 0x45a1);
	else
		lp = MemBase + PhysMake(datseg, 0x45e1);

	for (i = 0; i < 8; i++, lp += 8) {
		lv = *ptr++;
		for (j = 0; j < 8; j++) {
			if (!((0x80 >> j) & lv))
				continue;

			host_writeb(lp + j, (unsigned char)fg_color[col_index]);
		}
	}
}

/* static */
RealPt get_gfx_ptr(Bit16u x, Bit16u y) {

	RealPt start;

	start = ds_readd(0x47c7) + 320 * y + x;

	return start;
}

/* static */
Bit16u ret_zero() {
	return 0;
}

/* static */
void call_blit_smth3(PhysPt dst, Bit16u v1, Bit16u v2, Bit16u v3, Bit16u v4) {
	blit_smth3(dst, v1, v4);
}

/* static */
void set_textcolor(unsigned short fg, unsigned short bg) {
	fg_color[0] = fg;
	bg_color = bg;
}

/* static */
void get_textcolor(Bit8u *p1, Bit8u *p2) {
	host_writew(p1, fg_color[0]);
	host_writew(p2, bg_color);
}

Bit16u get_str_width(char *str) {

	Bit16u sum = 0;
	unsigned char width;

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
Bit16u get_line_start_c(char *str, Bit16u x, Bit16u x_max) {

	Bit16u sum = 0;
	unsigned char width;

	while (*str && *str != 0x40 && *str != 0x0d) {
		get_chr_info(*str++, &width);
		sum += width;
	}
	return (x_max - sum) / 2 + x ;
}

Bit16u enter_string(char *dst, Bit16u x, Bit16u y, Bit16u num, Bit16u zero)
{
	Bit16u pos, di, si;
	Bit16s c;
	unsigned char width;

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
	while (c != 0xd || pos == 0) {
		do {
			do {} while (CD_bioskey(1) == 0 &&
				ds_readw(MOUSE1_EVENT1) == 0);

			if (ds_readw(MOUSE1_EVENT1)) {
				ds_writew(IN_KEY_ASCII, 0x0d);
				ds_writew(MOUSE1_EVENT2, 0);
				ds_writew(MOUSE1_EVENT1, 0);
			} else {
				ds_writew(IN_KEY_ASCII, CD_bioskey(0));
				ds_writew(IN_KEY_EXT, ds_readw(IN_KEY_ASCII) >> 8);
				ds_writew(IN_KEY_ASCII, ds_readw(IN_KEY_ASCII) & 0xff);
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

			if (zero == 0)
				di -= 6;
			else
				di -= width;
		} else {
			if (!(ds_readb(0x1ff9 + c) & 0x0e) &&
				((c & 0xff) != 0x84) && ((c & 0xff) != 0x94) &&
				((c & 0xff) != 0x81) && ((c & 0xff) != 0x8e) &&
				((c & 0xff) != 0x99) && ((c & 0xff) != 0x9a) &&
				(c != 0x20) && (c != 0x2e))
					continue;

			/* is_alpha(c) */
			if (ds_readb(0x1ff9 + c) & 0xc)
				c = toupper(c);

			/* ae */
			if (c == 0x84)
				c = (signed short)0xff8e;
			/* oe */
			if (c == 0x94)
				c = (signed short)0xff99;
			/* ue */
			if (c == 0x81)
				c = (signed short)0xff9a;

			/* are we at the end of the input field */
			if (pos == num) {
				dst--;
				get_chr_info(*dst, &width);

				if (zero != 0)
					di -= width;
				else
					di -= 6;

				pos--;
			}

			*dst++ = c & 0xff;
			print_chr(0x20, di, y);
			print_chr(c & 0xff, di, y);
			get_chr_info(c & 0xff, &width);

			if (zero != 0)
				di += width;
			else
				di += 6;

			pos++;

			if (zero != 1 || pos == num)
				continue;
		}
		print_chr(0x20, di, y);
		print_chr(0x5f, di, y);
	}

	if (zero == 0)
		while (pos < num) {
			print_chr(0x20, di, y);
			di += 6;
			pos++;
		}

	*dst = 0;
	call_mouse();
	return 0;
}

void draw_popup_line(Bit16u line, Bit16u type)
{
	PhysPt dst, src;
	Bit16u i, popup_right, popup_left, popup_middle;

	/* This is a bit bogus */
	dst = Real2Phys(ds_readd(VGA_MEMSTART));

	/* (line * 8 + y) * 320  + x */
	dst = ((line * 8) + upper_border) * 320 + Real2Phys(ds_readd(VGA_MEMSTART)) + left_border;

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

	src = Real2Phys(ds_readd(BUFFER_POPUP)) + popup_left;
	copy_to_screen(src, dst, 16, 8, 0);

	src = Real2Phys(ds_readd(BUFFER_POPUP)) + popup_middle;
	dst += 16;
	for (i = 0; i < ds_readws(MENU_TILES); dst += 32, i++)
		copy_to_screen(src, dst, 32, 8, 0);

	src = Real2Phys(ds_readd(BUFFER_POPUP)) + popup_right;
	copy_to_screen(src, dst, 16, 8, 0);
}

/**
 *	infobox() - draws and info- or enter_numberbox
 *	@msg:		the message for the box
 *	@enter_num:	number of digits to enter
 *
 *	if @digits is zero the function just delays.
 */
Bit16u infobox(char *msg, Bit16u digits)
{
	PhysPt src, dst;
	Bit16u bg, fg;
	Bit16u retval, v2, v3, v4, i, lines;
	Bit16s di;

	retval = 0;
	ds_writew(0x4789, 1);
	v2 = text_x;
	v3 = text_y;
	v4 = text_x_end;

	di = (ds_readws(MENU_TILES) + 1) * 32;
	left_border = abs(320 - di) / 2 + ds_readw(0x1327);
	text_x = abs(320 - di) / 2 + ds_readw(0x1327) + 5;
	text_x_end = di - 10;
	lines = str_splitter(msg);

	if (digits != 0)
		lines += 2;

	upper_border = abs(200 - (lines + 2) * 8) / 2;
	upper_border += ro_zero;
	text_y = upper_border + 7;

	update_mouse_cursor();

	src = Real2Phys(ds_readd(VGA_MEMSTART));
	src += upper_border * 320 + left_border;
	dst = Real2Phys(ds_readd(GEN_PTR1_DIS));

	copy_to_screen(src, dst, di, (lines + 2) * 8, 2);

	/* draw the popup box */
	draw_popup_line(0, 0);

	for (i = 0; i < lines; i++)
		draw_popup_line(i + 1, 1);

	draw_popup_line(lines + 1, 3);

	get_textcolor((Bit8u*)&fg, (Bit8u*)&bg);
	set_textcolor(0xff, 0xdf);

	print_line(msg);

	ds_writew(MOUSE2_EVENT, 0);
	call_mouse();

	if (digits) {
		enter_string((char*)Real2Host(ds_readd(GEN_PTR3)),
			abs(di - digits * 6) / 2 + left_border,
			lines * 8 + upper_border - 2, digits, 0);

		retval = (Bit16u)atol((char*)Real2Host(ds_readd(GEN_PTR3)));
	} else {
#if !defined(__BORLANDC__)
		ds_writed(ACTION_TABLE,  RealMake(datseg, ACTION_INPUT));
#else
		ds_writed(ACTION_TABLE, (Bit32u)&ds[ACTION_INPUT]);
#endif
		vsync_or_key(150 * lines);
		ds_writed(ACTION_TABLE, (RealPt)0);
	}

	set_textcolor(fg, bg);
	update_mouse_cursor();

	dst = Real2Phys(ds_readd(VGA_MEMSTART));
	dst += upper_border * 320 + left_border;
	src = Real2Phys(ds_readd(GEN_PTR1_DIS));

	copy_to_screen(src, dst, di, (lines + 2) * 8, 0);
	call_mouse();

	text_x = v2;
	text_y = v3;
	text_x_end = v4;

	ds_writew(0x4789, 0);
	ds_writew(IN_KEY_EXT, 0);

	return retval;
}

/**
 * gui_bool() - displays a yes - no radio box
 * @header:	the header of menu
 *
 */
Bit16s gui_bool(Bit8u *msg)
{
	Bit16s retval;

	bool_mode = 1;
	retval = gui_radio(msg, 2, get_text(4), get_text(5));
	bool_mode = 0;

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
void fill_radio_button(Bit16s old_pos, Bit16u new_pos, Bit16u offset)
{
	Bit16u i, x, y;

	update_mouse_cursor();

	/* unmark the old radio button, if any */
	if (old_pos != -1) {
		y = left_border + 6;

		x = (offset + old_pos) * 8 + upper_border + 2;

		for (i = 0; i < 4; i++)
			draw_v_line(y + i, x, x + 3, 0xd8);
	}

	/* mark the new radio button */
	y = left_border + 6;

	x = (offset + new_pos) * 8 + upper_border + 2;

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
Bit16s gui_radio(Bit8u *header, Bit8s options, ...)
{
	va_list arguments;

	char *str;
	PhysPt src, dst;
	Bit16u r3, r4, r5, r6, r7, r8, r9;
	Bit16u my_bak, mx_bak;
	Bit16u bak1, bak2, bak3;
	Bit16u fg_bak, bg_bak;
	Bit16u lines_header, lines_sum;
	Bit16s retval;
	Bit16u di;
	Bit16u i;

	r5 = 0;
	r6 = 0xffff;
	di = 1;
	bak1 = text_x;
	bak2 = text_y;
	bak3 = text_x_end;
	r9 = (ds_readws(MENU_TILES) + 1) * 32;
	left_border = (abs(320 - r9) / 2) + ds_readw(0x1327);
	text_x = left_border + 5;
	text_x_end = (ds_readws(MENU_TILES) + 1) * 32 - 10;
	lines_header = str_splitter((char*)header);
	lines_sum = lines_header + options;
	upper_border = abs(200 - (lines_sum + 2) * 8) / 2;
	text_y = upper_border + 7;
	update_mouse_cursor();

	/* save old background */
	src = Real2Phys(ds_readd(VGA_MEMSTART));
	src += upper_border * 320 + left_border;
	dst = Real2Phys(ds_readd(GEN_PTR1_DIS));
	copy_to_screen(src, dst, r9, (lines_sum + 2) * 8, 2);

	/* draw popup */
	draw_popup_line(0, 0);
	for (i = 0; i < lines_header; i++)
		draw_popup_line(i + 1, 1);
	for (i = 0; options > i; i++)
		draw_popup_line(i + lines_header + 1, 2);
	draw_popup_line(lines_sum + 1, 3);

	/* save and set text colors */
	get_textcolor((Bit8u*)&fg_bak, (Bit8u*)&bg_bak);
	set_textcolor(0xff, 0xdf);

	/* print header */
	if (lines_header)
		print_line((char*)header);

	r3 = text_x + 8;
	r4 = (lines_header + 1) * 8 + upper_border;

	/* print radio options */
	va_start(arguments, options);
	for (i = 1; i <= options; r4 += 8, i++) {
		str = va_arg(arguments, char*);
		print_str(str, r3, r4);
	}
	va_end(arguments);

	/* save and set mouse position */
	mx_bak = ds_readw(MOUSE_POSX);
	my_bak = ds_readw(MOUSE_POSY);
	ds_writew(MOUSE_POSX, left_border + 90);
	ds_writew(0x1250, left_border + 90);
	r7 = (lines_header + 1) * 8 + upper_border;
	r8 = r7;
	ds_writew(MOUSE_POSY, r8);
	ds_writew(0x1252, r8);
	mouse_move_cursor(ds_readw(MOUSE_POSX), r8);

	ds_writew(MOUSE_POSX_MAX, left_border + r9 - 16);
	ds_writew(MOUSE_POSX_MIN, left_border);
	ds_writew(MOUSE_POSY_MIN, (lines_header + 1) * 8 + upper_border);
	ds_writew(MOUSE_POSY_MAX,
		upper_border + options * 8 + (lines_header + 1) * 8 - 1);
	call_mouse();
	ds_writew(MOUSE2_EVENT, 0);

	while (r5 == 0) {
#if !defined(__BORLANDC__)
		ds_writed(ACTION_TABLE,  RealMake(datseg, ACTION_INPUT));
#else
		ds_writed(ACTION_TABLE, (Bit32u)&ds[ACTION_INPUT]);
#endif
		handle_input();
		ds_writed(ACTION_TABLE, (RealPt)0);

		if (di != r6) {
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
			/* ihas the mouse been moved */
			r8 = ds_readw(MOUSE_POSY);
			di = (r8 - r7) / 8 + 1;
		}
		/* is this a bool radiobox ? */
		if (bool_mode) {
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

	ds_writew(MOUSE_POSX, mx_bak);
	ds_writew(0x1250, mx_bak);
	ds_writew(MOUSE_POSY, my_bak);
	ds_writew(0x1252, my_bak);

	ds_writew(MOUSE_POSX_MAX, 319);
	ds_writew(MOUSE_POSX_MIN, 0);
	ds_writew(MOUSE_POSY_MIN, 0);
	ds_writew(MOUSE_POSY_MAX, 199);

	mouse_move_cursor(mx_bak, my_bak);

	dst = Real2Phys(ds_readd(VGA_MEMSTART));
	dst += upper_border * 320 + left_border;
	src = Real2Phys(ds_readd(GEN_PTR1_DIS));
	copy_to_screen(src, dst, r9, (lines_sum + 2) * 8, 0);
	call_mouse();
	set_textcolor(fg_bak, bg_bak);

	text_x = bak1;
	text_y = bak2;
	text_x_end = bak3;
	ds_writew(IN_KEY_EXT, 0);

	return retval;
}

/**
 * enter_name() - enter the name of a hero
 */
void enter_name()
{
	PhysPt dst;

	dst = Real2Phys(ds_readd(VGA_MEMSTART) + 12 * 320 + 176);

	update_mouse_cursor();
	copy_to_screen(Real2Phys(ds_readd(0x479f)), dst, 94, 8, 0);
	enter_string(hero.name, 180, 12, 15, 1);
	copy_to_screen(Real2Phys(ds_readd(0x479f)), dst, 94, 8, 0);
	call_mouse();
	print_str(hero.name, 180, 12);
}

void change_head()
{
	struct nvf_desc nvf;
	signed short tmp;

	nvf.dst = (RealPt)ds_readd(GEN_PTR6);
	nvf.src = (RealPt)ds_readd(BUFFER_HEADS_DAT);
	nvf.no = ds_readbs(HEAD_CURRENT);
	nvf.type = 0;
	nvf.width = &tmp;
	nvf.height = &tmp;

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
void change_sex()
{
	PhysPt dst, src;
	signed char tmp;

	/* change sex of the hero */
	hero.sex = hero.sex ^ 1;

	/* hero has a typus */
	if (hero.typus) {
		if (hero.sex != 0) {
			/* To female */
			ds_writeb(HEAD_CURRENT, head_first_female[head_typus]);
			head_first = head_first_female[head_typus];
			head_last = head_first_male[head_typus + 1] - 1;
		} else {
			/* To male */
			tmp = head_first_male[head_typus];
			ds_writeb(HEAD_CURRENT, tmp);
			head_first = tmp;
			head_last = head_first_female[head_typus] - 1;
		}
		ds_writew(0x11fe, 1);
		return;
	} else {
		dst = Real2Phys(ds_readd(VGA_MEMSTART)) + 7 * 320 + 305;
		src = Real2Phys(ds_readd(BUFFER_SEX_DAT)) + hero.sex * 256;
		update_mouse_cursor();
		copy_to_screen(src, dst, 16, 16, 0);
		call_mouse();
	}
}

void do_gen()
{
	Bit16s si, di;

	di = 0;

	ds_writew(0x11fe, 1);

	/* try to set the level from parameters */
	switch (param_level) {
		case 'a': {
			/* ADVANCED */
			level = 2;
			break;
		}
		case 'n': {
			/* NOVICE */
			level = 1;
			break;
		}
		default: {
			level = -1;
		}
	}

	/* ask for level */
	while (level == -1) {
		level = gui_radio((Bit8u*)get_text(0), 2, get_text(1), get_text(2));
	}

	ds_writew(MOUSE2_EVENT, 1);

	/* main loop */
	while (!di) {
		if (ds_readw(0x11fe)) {
			refresh_screen();
			ds_writew(0x11fe, 0);
		}

		ds_writed(ACTION_TABLE,  (Bit32u)ds_readd(ACTION_PAGE + 4 * ds_readws(GEN_PAGE)));
		handle_input();
		ds_writed(ACTION_TABLE, (RealPt)0);

		if (ds_readw(MOUSE2_EVENT) || ds_readw(IN_KEY_EXT) == KEY_PGUP) {
			/* print the menu for each page */
			switch(ds_readws(GEN_PAGE)) {
				case 0: {
					si = gui_radio((Bit8u*)get_text(7), 9,
						get_text(10), get_text(11), get_text(15),
						get_text(8),  get_text(14), get_text(12),
						get_text(262),get_text(9),  get_text(258));

					if (si != -1) {
						if ((si >= 4) && (si < 6) &&
							(hero.attribs[0].normal) &&
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
								memset(&hero, 0, sizeof(hero));
								clear_hero();
								ds_writew(MOUSE2_EVENT,	1);
								ds_writew(0x11fe, 1);
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
			if (hero.typus == 0) {
				infobox(get_text(17), 0);
			} else {
				if (ds_readbs(HEAD_CURRENT) < head_last) {
					ds_writeb(HEAD_CURRENT, ds_readbs(HEAD_CURRENT) + 1);
				} else {
					ds_writeb(HEAD_CURRENT, head_first);
				}
				change_head();
			}
		}

		if ((ds_readw(IN_KEY_EXT) == KEY_DOWN) && (ds_readws(GEN_PAGE) == 0)) {
			if (hero.typus == 0) {
				infobox(get_text(17), 0);
			} else {
				if (ds_readbs(HEAD_CURRENT) > head_first) {
					ds_writeb(HEAD_CURRENT, ds_readbs(HEAD_CURRENT) - 1);
				} else {
					ds_writeb(HEAD_CURRENT, head_last);
				}
				change_head();
			}
		}

		if ((ds_readw(IN_KEY_EXT) == KEY_RIGHT) && (level != 1)) {
			if (hero.typus == 0) {
				infobox(get_text(72), 0);
			} else {
				ds_writew(0x11fe, 1);

				if (((hero.typus >= 7) ? 10 : 4) > ds_readws(GEN_PAGE)) {
					ds_inc_ws(GEN_PAGE);
				} else {
					ds_writew(GEN_PAGE, 0);
				}
			}
		}

		if (ds_readw(IN_KEY_EXT) == KEY_LEFT) {
			if (ds_readws(GEN_PAGE) > 0) {
				ds_writew(0x11fe, 1);
				ds_dec_ws(GEN_PAGE);
			} else {
				if (level != 1) {
					if (hero.typus == 0) {
						infobox(get_text(72), 0);
					} else {
						ds_writew(0x11fe, 1);
						ds_writew(GEN_PAGE, hero.typus < 7 ? 4 : 10);
					}
				}
			}
		}

		if ((ds_readw(IN_KEY_EXT) >= KEY_1) && (ds_readw(IN_KEY_EXT) <= KEY_5) &&
			level == 2 && hero.typus) {
			switch (ds_readw(IN_KEY_EXT)) {
				case KEY_1: {
					si = 0;
					break;
				}
				case KEY_2: {
					si = 1;
					break;
				}
				case KEY_3: {
					si = 4;
					break;
				}
				case KEY_4: {
					si = 5;
					break;
				}
				default: {
					si = 10;
				}
			}
			if ((si != ds_readws(GEN_PAGE)) && (si < 5 || hero.typus >= 7)) {
				ds_writews(GEN_PAGE, si);
				ds_writew(0x11fe, 1);
			}
		}
	}
}

/**
 * calc_at_pa() - calculate AT and PA values
 */
/* static */
void calc_at_pa() {
	Bit16u i;
	Bit16s tmp, base;

	/* base = (GE + IN + KK) / 5 */
	tmp = hero.attribs[5].normal + hero.attribs[6].normal +
		hero.attribs[4].normal ;
	base = tmp / 5;

	/* round up if neccessary */
	if ((tmp % 5) >= 3)
		base++;

	/* save AT/PA base value */
	hero.atpa = (signed char)base;

	for (i = 0; i < 7; i++) {
		/* set the weapon values to base */
		hero.pa[i] = (signed char)base;
		hero.at[i] = (signed char)base;

		if (hero.skills[i] < 0) {
			/* calculate ATPA for negative weapon skill */
			tmp = abs(hero.skills[i]) / 2;

			/* sub skill / 2 from AT */
			hero.at[i] -= tmp;

			/* sub skill / 2 from PA */
			hero.pa[i] -= tmp;

			/* if skill % 2, then decrement PA */
			if (abs(hero.skills[i]) != tmp * 2)
				hero.pa[i]--;
		} else {
			/* calculate ATPA for positive weapon skill */
			tmp = abs(hero.skills[i]) / 2;

			/* add skill / 2 to AT */
			hero.at[i] += tmp;

			/* add skill / 2 to PA */
			hero.pa[i] += tmp;

			/* if skill % 2, then increment AT */
			if (hero.skills[i] != tmp * 2)
				hero.at[i]++;
		}

	}
}

/**
 * fill_values() - fills the values if typus is chosen
 *
 */
void fill_values()
{
	Bit8u *ptr;
	Bit16u i;
	Bit16u si, di;

	/* fill skill values */
	for (i = 0; i < 52; i++) {

		hero.skills[i] = skills[hero.typus][i];

		/* set skill_incs and skill_tries to zero */
		skill_incs[i].incs = 0;
		skill_incs[i].tries = 0;
	}

	/* set skill_attempts */
	hero.skill_incs = initial_skill_incs[hero.typus - 1];

	/* do magic user init */
	if (hero.typus >= 7) {
		/* fill initial spell values */
		for (i = 0; i < 86; i++) {
			hero.spells[i] = spells[hero.typus - 7][i];

			/* set spell_incs and spell_tries to zero */
			spell_incs[i].incs = 0;
			spell_incs[i].tries = 0;
		}
		/* special mage values */
		if (hero.typus == 9) {
			/* set staff spell to level 1 */
			hero.staff_level = 1;
			/* select mage school */
			do {
				hero.school = gui_radio((Bit8u*)get_text(47), 9,
							get_text(48), get_text(49),
							get_text(50), get_text(51),
							get_text(52), get_text(53),
							get_text(54), get_text(55),
							get_text(56)) - 1;
			} while (hero.school == -2);

			/* add magic school modifications */
			for (i = 0; house_mod[hero.school].no > i; i++) {
				Bit16s spell, mod;

				spell = house_mod[hero.school].spells[i];
				mod = house_mod[hero.school].mod[i];
				hero.spells[spell] += mod;
			}
		}

		/* set spell attempts */
		hero.spell_incs = initial_spell_incs[hero.typus - 7];

		/* get convertable increase attempts */
		di = initial_conv_incs[hero.typus - 7];

		if (di && (level == 2) && gui_bool((Bit8u*)get_text(269))) {
			/* create string */
			sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(270), di);

			i = infobox((char*)Real2Host(ds_readd(GEN_PTR2)), 1);

			if (i > 0) {
				/* spell attempts to skill attempts */
				if (i > di)
					i = di;
				di -= i;
				/* change spell attempts */
				hero.spell_incs -= i;
				/* change skill attempts */
				hero.skill_incs += i;
			} else {

				/* create string */
				sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(271), di);

				i = infobox((char*)Real2Host(ds_readd(GEN_PTR2)), 1);
				if (i > 0) {
					if (i > di)
						i = di;
					/* change spell attempts */
					hero.spell_incs += i;
					/* change skill attempts */
					hero.skill_incs -= i;
				}
			}
		}


	}

	/* set LE */
	hero.le = hero.le_max = init_le[hero.typus];

	/* set AE */
	hero.ae = hero.ae_max = init_ae[hero.typus];

	/* wanna change 10 spell_attempts against 1W6+2 AE ? */
	if ((hero.typus == 9) && (level == 2) && gui_bool((Bit8u*)get_text(268))) {
		/* change spell_attempts */
		hero.spell_incs -= 10;
		hero.ae_max = random_interval_gen(3, 8) + hero.ae_max;
		hero.ae = hero.ae_max;
	}

	/* roll out size */
	hero.height = (unsigned char)random_interval_gen(height_range[hero.typus].min,
				height_range[hero.typus].max);

	/* calculate weight i = (height - weight_mod) * 40 */
	hero.weight = (hero.height - weight_mod[hero.typus]) * 40;

	/* roll out the money */
	i = random_gen(20);
	ptr = Real2Host(ds_readd(0xa51 + hero.typus * 4));
	for (si = 0; host_readw(ptr + si * 6) < i; si++);

	hero.money = random_interval_gen(host_readw(ptr + si * 6 + 2),
				host_readw(ptr + si * 6 + 4)) * 10;

	/* calculate MR  = (KL + SI + Level) / 3 - 2 * AG */
	hero.mr = (hero.attribs[1].normal + hero.attribs[0].normal + hero.level) / 3 -
		hero.attribs[7].normal * 2;
	/* add typus MR Modificator */
	hero.mr += mr_mod[hero.typus];

	/* roll out god */
	hero.god = (unsigned char)random_gen(12);

	/* add gods boni */
	switch (hero.god) {
		case 1 : {
			/* Praios: MU + 1 */
			hero.attribs[0].normal++;
			hero.attribs[0].current++;
			got_mu_bonus = 1;
			break;
		}
		case 2 : {
			/* Rondra: skill swords + 1 */
			hero.skills[3]++;
			break;
		}
		case 3 : {
			/* Efferd: skill swim + 1 */
			hero.skills[14]++;
			break;
		}
		case 4 : {
			/* Travia: skill treat poison + 1 */
			hero.skills[44]++;
			break;
		}
		case 5 : {
			/* Boron: skill human nature + 1 */
			hero.skills[24]++;
			break;
		}
		case 6 : {
			/* Hesinde: skill alchemy + 1 */
			hero.skills[32]++;
			break;
		}
		case 7 : {
			/* Firun: skills track and missle weapons + 1  */
			hero.skills[26]++;
			hero.skills[7]++;
			break;
		}
		case 8 : {
			/* Tsa: CH + 1 */
			hero.attribs[2].normal++;
			hero.attribs[2].current++;
			got_ch_bonus = 1;
			break;
		}
		case 9 : {
			/* Phex: skills hide and pickpocket + 1 */
			hero.skills[49]++;
			hero.skills[13]++;
			break;
		}
		case 10 : {
			/* Peraine: skills treat disease and wounds + 1 */
			hero.skills[45]++;
			hero.skills[46]++;
			break;
		}
		case 11 : {
			/* Ingerimm: skill tactics + 1*/
			hero.skills[37]++;
			break;
		}
		case 12 : {
			/* Rhaja: skills dance, seduce and instrument + 1*/
			hero.skills[20]++;
			hero.skills[16]++;
			hero.skills[47]++;
			break;
		}
	}
	/* calclate AT and PA values */
	calc_at_pa();

	/* if mode == novice */
	if (level == 1) {
		/* automatic increase skills */
		i = 0;
		while (hero.skill_incs > 0) {
			skill_inc_novice(autoskills[hero.typus][i++]);
		}

		si = 0;
		/* prepare mage automatic spell list */
		if (hero.typus == 9) {
			/* 1. house spells */
			for (i = 0; house_mod[hero.school].no > i; si++, i++) {
				autospells[hero.typus - 7][si] =
					house_mod[hero.school].spells[i];
			}
			/* 2. all schools spells */
			for (i = 0; school_tab[hero.school].spells > i; si++, i++) {
				autospells[hero.typus - 7][si] =
					school_tab[hero.school].first_spell + i;
			}
			/* 3. five domination spells */
				/* Herr der Tiere */
			autospells[hero.typus - 7][si++] = 0x52;
				/* Horriphobus */
			autospells[hero.typus - 7][si++] = 0x31;
				/* Mag. Raub */
			autospells[hero.typus - 7][si++] = 0x35;
				/* Respondami */
			autospells[hero.typus - 7][si++] = 0x21;
				/* Sanftmut */
			autospells[hero.typus - 7][si++] = 0x4f;

			/* 4. all house spells */
			for (i = 0; house_mod[hero.school].no > i; si++, i++) {
				autospells[hero.typus - 7][si] =
					house_mod[hero.school].spells[i];
			}
			/* 5. all house spells */
			for (i = 0; house_mod[hero.school].no > i; si++, i++) {
				autospells[hero.typus - 7][si] =
					house_mod[hero.school].spells[i];
			}
			/* 6. random spells */
			while (si < 45) {
				autospells[hero.typus - 7][si++] = random_gen(85);
			}
		}
		/* automatic increase spells */
		i = 0;
		while (hero.spell_incs > 0) {
			spell_inc_novice(autospells[hero.typus - 7][i++]);
		}
	}
}


void refresh_screen()
{
	PhysPt src, dst;

	if (ds_readw(0x11fe)) {
		ds_writed(0x47c7, ds_readd(GEN_PTR1_DIS));
		load_page(ds_readws(GEN_PAGE));
		save_picbuf();

		/* page with base values and hero is not male */
		if ((ds_readws(GEN_PAGE) == 0) && (hero.sex != 0)) {

			dst = Real2Phys(ds_readd(GEN_PTR1_DIS)) + 7 * 320 + 305;
			src = Real2Phys(ds_readd(BUFFER_SEX_DAT) + hero.sex * 256);

			copy_to_screen(src, dst, 16, 16, 0);
		}

		/* page with base values and level is advanced */
		if ((ds_readws(GEN_PAGE) == 0) && (level == 1)) {
			dst = Real2Phys(ds_readd(GEN_PTR1_DIS)) + 178 * 320 + 284;
			src = Real2Phys(ds_readd(BUFFER_SEX_DAT) + 512);

			copy_to_screen(src, dst, 20, 15, 0);
		}
		/* if the page is lower than 5 */
		if (ds_readws(GEN_PAGE) < 5) {
			/* draw DMENGE.DAT or the typus name */
			dst = Real2Phys(ds_readd(GEN_PTR1_DIS)) + 0xa10;
			if (hero.typus != 0) {
				need_refresh = 1;
				copy_to_screen(Real2Phys(ds_readd(GEN_PTR5)), dst, 128, 184, 0);

				if (hero.sex != 0) {
					char *p;
					p = get_text(271 + hero.typus);
					print_str(p, get_line_start_c(p, 16, 128), 184);
				} else {
					char *p;
					p = get_text(17 + hero.typus);
					print_str(p, get_line_start_c(p, 16, 128), 184);
				}
			} else {
				if (need_refresh) {
					call_fill_rect_gen((RealPt)ds_readd(VGA_MEMSTART), 16, 8, 143, 191, 0);
					need_refresh = 0;
				}
				wait_for_vsync();
				set_palette(Real2Host(ds_readd(BUFFER_DMENGE_DAT)) + 0x5c02, 0 , 32);
				copy_to_screen(Real2Phys(ds_readd(BUFFER_DMENGE_DAT)), dst, 128, 184, 0);
			}
		}
		/* if hero has a typus */
		if (hero.typus != 0) {
			/* draw the head */
			struct nvf_desc nvf;
			signed short tmp;

			nvf.dst = (RealPt)ds_readd(GEN_PTR6);
			nvf.src = (RealPt)ds_readd(BUFFER_HEADS_DAT);
			nvf.no = ds_readb(HEAD_CURRENT);
;
			nvf.type = 0;
			nvf.width = &tmp;
			nvf.height = &tmp;
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
		ds_writed(0x47c7, ds_readd(VGA_MEMSTART));
		dst = Real2Phys(ds_readd(VGA_MEMSTART));
		src = Real2Phys(ds_readd(GEN_PTR1_DIS));
		update_mouse_cursor();
		copy_to_screen(src, dst, 320, 200, 0);
		call_mouse();
	} else {
		print_values();
	}
}

/* static */
void clear_hero() {

	Bit16u i;

	got_ch_bonus = 0;
	got_mu_bonus = 0;

	ds_writeb(HEAD_CURRENT, 0);
	head_last = 0;
	head_first = 0;
	head_typus = 0;

	for (i = 0; i < 14; i++)
		attrib_changed[i] = 0;

	for (i = 0; i < 86; i++) {
		spell_incs[i].incs = 0;
		spell_incs[i].tries = 0;
	}
	for (i = 0; i < 52; i++) {
		skill_incs[i].tries = 0;
		skill_incs[i].incs = 0;
	}

	hero.level = 1;
}

/**
 * new_values() - roll out new attribute values
 *
 */
void new_values()
{
	/* Original-Bugfix:	there once was a char[11],
				which could not hold a char[16] */

	char name_bak[17];
	signed char values[8];
	Bit16u di, i, j;
	Bit8s bv1, bv2, bv3;

	/* set variable if hero has a typus */
	if (hero.typus)
		ds_writew(0x11fe, 1);

	/* save the name of the hero */
	/* TODO strncpy() would be better here */
	strcpy(name_bak, hero.name);

	/* save the sex of the hero */
	bv3 = hero.sex;

	/* clear the hero */
	memset(&hero, 0 , sizeof(hero));
	clear_hero();

	/* restore the sex of the hero */
	hero.sex = bv3;

	/* restore the name of the hero */
	/* TODO strncpy() would be better here */
	strcpy(hero.name, name_bak);

	refresh_screen();

	ds_writew(0x11fe, 0);

	for (j = 0; j < 7; j++) {
		bv1 = (unsigned char)random_interval_gen(8, 13);
		bv2 = 0;

		for (i = 0; i < 7; i++) {
			if (hero.attribs[i].normal)
				continue;

			values[bv2] = (signed char)i;
			type_names[bv2] = get_text(32 + i);
			bv2++;
		}

		sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(46), bv1);

		do {
			ds_writew(0x1327, 0xffb0);

			di = gui_radio((Bit8u*)Real2Host(ds_readd(GEN_PTR2)),
				bv2,
				type_names[0],
				type_names[1],
				type_names[2],
				type_names[3],
				type_names[4],
				type_names[5],
				type_names[6]);

			ds_writew(0x1327, 0);
		} while (di == 0xffff);
		di = values[di - 1];
		hero.attribs[di].current = bv1;
		hero.attribs[di].normal = bv1;
		update_mouse_cursor();
		refresh_screen();
		call_mouse();
	}

	for (j = 0; j < 7; j++) {
		bv1 = (signed char)random_interval_gen(2, 7);
		bv2 = 0;

		for (i = 0; i < 7; i++) {
			if (hero.attribs[i + 7].normal)
				continue;

			values[bv2] = (signed char)i;
			type_names[bv2] = get_text(39 + i);
			bv2++;
		}

		sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(46), bv1);

		do {
			ds_writew(0x1327, 0xffb0);

			di = gui_radio((Bit8u*)Real2Host(ds_readd(GEN_PTR2)),
				bv2,
				type_names[0],
				type_names[1],
				type_names[2],
				type_names[3],
				type_names[4],
				type_names[5],
				type_names[6]);

			ds_writew(0x1327, 0);
		} while (di == 0xffff);
		di = values[di - 1];
		hero.attribs[di + 7].current = bv1;
		hero.attribs[di + 7].normal = bv1;
		update_mouse_cursor();
		refresh_screen();
		call_mouse();
	}
}

/**
 * skill_inc_novice() - tries to increment a skill in novice mode
 * @skill:	the skill which should be incremented
 *
 *
 */
void skill_inc_novice(Bit16u skill)
{
	Bit16u done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (skill_incs[skill].tries == 3) {
			/* set the flag to leave this loop */
			done++;
			continue;
		}

		/* Original-Bugfix: add check if skill_attempts are left */
		if (hero.skill_incs == 0) {
			done++;
			continue;
		}
		/* decrement counter for skill increments */
		hero.skill_incs--;

		/* check if the test is passed */
		if (random_interval_gen(2, 12) > hero.skills[skill]) {
			/* increment skill */
			hero.skills[skill]++;

			/* set inc tries for this skill to zero */
			skill_incs[skill].tries = 0;

			/* set the flag to leave this loop */
			done++;

			if (skill > 6)
				continue;

			/* set increment the lower AT/PA value */
			if (hero.at[skill] > hero.pa[skill])
				hero.pa[skill]++;
			else
				hero.at[skill]++;
		} else
			/* inc tries for that skill */
			skill_incs[skill].tries++;
	}
}

/**
 * spell_inc_novice() - tries to increment a spell in novice mode
 * @spell:	the spell which should be incremented
 *
 *
 */
void spell_inc_novice(Bit16u spell)
{
	Bit16u done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (spell_incs[spell].tries == 3) {
			/* set the flag to leave this loop */
			done++;
			continue;
		}

		/* Original-Bugfix: add check if skill_attempts are left */
		if (hero.spell_incs == 0) {
			done++;
			continue;
		}
		/* decrement counter for spell increments */
		hero.spell_incs--;

		/* check if the test is passed */
		if (random_interval_gen(2, 12) > hero.spells[spell]) {
			/* increment spell */
			hero.spells[spell]++;

			/* set inc tries for this spell to zero */
			spell_incs[spell].tries = 0;

			/* set the flag to leave this loop */
			done++;
		} else
			spell_incs[spell].tries++;
	}
}

#define INC (1)
#define DEC (2)

/**
 * select_typus() - select a possible typus with current attribute values
 *
 */
void select_typus()
{
	Bit8s old_typus, possible_types, ltmp2;
	unsigned short impossible;
	Bit16s i, si, di;
	struct type_bitmap t;

	old_typus = -1;
	t = empty_bitmap;

	/* check if attribs have bee set */
	if (hero.attribs[0].normal == 0) {
		infobox(get_text(265), 0);
		return;
	}
	/* save the old typus */
	old_typus = hero.typus;
	/* disable MU bonus */
	if (got_mu_bonus) {
		hero.attribs[0].normal--;
		hero.attribs[0].current--;
	}
	/* disable CH bonus */
	if (got_ch_bonus) {
		hero.attribs[2].normal--;
		hero.attribs[2].current--;
	}
	possible_types = 0;

	for (i = 1; i <= 12; i++) {
		impossible = 0;
		for (si = 0; si < 4; si++) {
			Bit8u req;

			ltmp2 = hero.attribs[reqs[i][si].attrib].normal;

			req = reqs[i][si].requirement;

			if (req & 0x80) {
				if (ltmp2 <= (req & 0x7f))
					continue;
				impossible = 1;
			} else {
				if (req <= ltmp2)
					continue;
				impossible = 1;
			}
		}

		if (impossible)
			continue;

		if (hero.sex)
			type_names[possible_types] = get_text(271 + i);
		else
			type_names[possible_types] = get_text(17 + i);

		t.t[possible_types] = (char)i;
		possible_types++;

	}

	if (possible_types == 0) {
		if (can_change_attribs() == 0) {
			/* totally messed up values */
			infobox(get_text(284), 0);
			return;
		} else {
			infobox(get_text(31), 0);
			return;
		}
	}

	di = gui_radio((Bit8u*)get_text(30), possible_types,
			type_names[0],
			type_names[1],
			type_names[2],
			type_names[3],
			type_names[4],
			type_names[5],
			type_names[6],
			type_names[7],
			type_names[8],
			type_names[9],
			type_names[10],
			type_names[11]);

	/*	restore attibute boni when selection is canceled
	 *	or the same typus is selected.
	 */
	if (di == -1 || t.t[di - 1] == old_typus) {
		if (got_mu_bonus) {
			hero.attribs[0].normal++;
			hero.attribs[0].current++;
		}
		if (got_ch_bonus) {
			hero.attribs[2].normal++;
			hero.attribs[2].current++;
		}
		return;
	}

	/* set new typus */
	hero.typus = t.t[di - 1];
	ds_writew(0x11fe, 1);

	load_typus(hero.typus);
	update_mouse_cursor();
	call_fill_rect_gen((RealPt)ds_readd(VGA_MEMSTART), 16, 8, 143, 191, 0);
	wait_for_vsync();
	set_palette(Real2Host(ds_readd(GEN_PTR5)) + 0x5c02, 0, 32);
	call_mouse();

	if (hero.typus > 10)
		head_typus = 10;
	else
		head_typus = hero.typus;

	if (hero.sex) {
		ds_writeb(HEAD_CURRENT, head_first_female[head_typus]);
		head_first = head_first_female[head_typus];
		head_last = head_first_male[head_typus + 1] - 1;
	} else {
		ds_writeb(HEAD_CURRENT, head_first_male[head_typus]);
		head_first = head_first_male[head_typus];
		head_last = head_first_female[head_typus] - 1;
	}

	/* reset boni falags */
	got_ch_bonus = 0;
	got_mu_bonus = 0;
	fill_values();
	return;
}

/**
 * can_change_attribs() - checks if attribute changes are possible
 *
 */
Bit16u can_change_attribs()
{
	Bit16u i;
	Bit16s pa_inc, pa_dec, na_inc, na_dec;

	na_dec = na_inc = pa_dec = pa_inc = 0;

	for (i = 0; i < 14; i++)
		D1_LOG("%d ", attrib_changed[i]);
	D1_LOG("\n");


	for (i = 0; i < 7; i++) {
		if ((attrib_changed[i] != INC) && (hero.attribs[i].normal > 8))
			pa_dec += 8 - hero.attribs[i].normal;
		if ((attrib_changed[i] != DEC) && (hero.attribs[i].normal < 13))
			pa_inc += 13 - hero.attribs[i].normal;
	}

	for (i = 7; i < 14; i++) {
		if ((attrib_changed[i] != INC) && (hero.attribs[i].normal > 2))
			na_dec += 2 - hero.attribs[i].normal;
		if ((attrib_changed[i] != DEC) && (hero.attribs[i].normal < 8))
			na_inc += 8 - hero.attribs[i].normal;
	}

	D1_LOG("%d %d %d %d\n", pa_inc, pa_dec, na_inc, na_dec);

	/* no values from positive attributes left */
	if ((pa_inc == 0) && (pa_dec == 0))
		return 0;

	if ((pa_inc == 0) && (na_dec < 2))
		return 0;

	if ((na_inc < 2) && (pa_dec == 0))
		return 0;

	if (na_dec >= 2)
		return 1;

	if (na_inc >= 2)
		return 1;

	return 0;
}

/**
 * change_attribs() - change attributes
 */
void change_attribs()
{
	Bit16s tmp1, tmp2, tmp3, si, di;
	Bit8u c;

	/* check if attributes have been set */
	if (hero.attribs[0].normal == 0) {
		infobox(get_text(16), 0);
		return;
	}
	/* check if changing is possible */
	if (can_change_attribs() == 0) {
		infobox(get_text(266), 0);
		return;
	}
	/* if typus != 0 */
	if (hero.typus) {
		if (!gui_bool((Bit8u*)get_text(73)))
			return;
		/* set typus to 0 */
		hero.typus = 0;
		/* remove MU boni */
		if (got_mu_bonus) {
			hero.attribs[0].normal--;
			hero.attribs[0].current--;
			got_mu_bonus = 0;
		}
		/* remove CH boni */
		if (got_ch_bonus) {
			hero.attribs[2].normal--;
			hero.attribs[2].current--;
			got_ch_bonus = 0;
		}
		ds_writew(0x11fe, 1);
		refresh_screen();
		ds_writew(0x11fe, 0);
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
	if (attrib_changed[tmp2] == 0) {
		/* ask user if inc or dec */
		ds_writew(0x1327, 0xffb0);
		tmp3 = gui_radio((Bit8u*)NULL, 2, get_text(75), get_text(76));
		ds_writew(0x1327, 0);

		if (tmp3 == -1)
			return;
	} else {
		tmp3 = attrib_changed[tmp2];
	}

	if (tmp3 == INC) {
		/* increment */
		if (hero.attribs[tmp2].normal == 13) {
			infobox(get_text(77), 0);
			return;
		}
		c = 0;
		for (di = 7; di < 14; di++) {
			if (attrib_changed[di] == DEC)
				continue;
			if (hero.attribs[di].normal >= 8)
				continue;
			c += 8 - hero.attribs[di].normal;
		}
		if (c < 2) {
			infobox(get_text(85), 0);
			return;
		}
		/* increment positive attribute */
		hero.attribs[tmp2].current++;
		hero.attribs[tmp2].normal++;

		attrib_changed[tmp2] = INC;

		refresh_screen();

		tmp1 = 0;
		while (tmp1 != 2) {
			/* ask which negative attribute to increment */
			ds_writew(0x1327, 0xffb0);
			si = gui_radio((Bit8u*)get_text(80), 7,
					get_text(39), get_text(40), get_text(41),
					get_text(42), get_text(43), get_text(44),
					get_text(45));
			ds_writew(0x1327, 0);

			if (si == -1)
				continue;

			si--;
			/* check if this attribute has been decremented */
			if (attrib_changed[si + 7] == DEC) {
				infobox(get_text(83), 0);
				continue;
			}
			/* check if attribute can be incremented */
			if (hero.attribs[si + 7].normal == 8) {
				infobox(get_text(77), 0);
				continue;
			}
			/* increment the negative attribute */
			tmp1++;
			attrib_changed[si + 7] = INC;
			hero.attribs[si + 7].normal++;
			hero.attribs[si + 7].current++;

			refresh_screen();
		}
	} else {
		/* decrement */
		/* check if the positive attribute can be decremented */
		if (hero.attribs[tmp2].normal == 8) {
			infobox(get_text(81), 0);
			return;
		}
		c = 0;
		for (di = 7; di < 14; di++) {
			if (attrib_changed[di] == INC)
				continue;
			if (hero.attribs[di].normal <= 2)
				continue;
			c += hero.attribs[di].normal - 2;
		}
		if (c < 2) {
			infobox(get_text(84), 0);
			return;
		}
		/* decrement positive attribute */
		hero.attribs[tmp2].normal--;
		hero.attribs[tmp2].current--;
		/* mark this attribute as decremented */
		attrib_changed[tmp2] = DEC;

		refresh_screen();

		tmp1 = 0;
		while (tmp1 != 2) {
			/* ask which negative attribute to increment */
			ds_writew(0x1327, 0xffb0);
			si = gui_radio((Bit8u*)get_text(79), 7,
					get_text(39), get_text(40), get_text(41),
					get_text(42), get_text(43), get_text(44),
					get_text(45));
			ds_writew(0x1327, 0);

			if (si == -1)
				continue;

			si--;
			/* check if this attribute has been incremented */
			if (attrib_changed[si + 7] == INC) {
				infobox(get_text(82), 0);
				continue;
			}
			/* check if attribute can be decremented */
			if (hero.attribs[si + 7].normal == 2) {
				infobox(get_text(81), 0);
				continue;
			}
			/* deccrement the negative attribute */
			tmp1++;
			hero.attribs[si + 7].normal--;
			hero.attribs[si + 7].current--;
			attrib_changed[si + 7] = DEC;

			refresh_screen();
		}
	}
}

#undef INC
#undef DEC

void save_picbuf()
{
	PhysPt p;
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
		p = Real2Phys(ds_readd(GEN_PTR1_DIS)) + y_1 * 320 + x_1;
		copy_to_screen(p, Real2Phys(ds_readd(0x479f)), w_1, h_1, 2);
	}

	p = Real2Phys(ds_readd(GEN_PTR1_DIS)) + y_2 * 320 + x_2;
	copy_to_screen(p, Real2Phys(ds_readd(0x479b)), w_2, h_2, 2);

	p = Real2Phys(ds_readd(GEN_PTR1_DIS)) + y_3 * 320 + x_3;
	copy_to_screen(p, Real2Phys(ds_readd(0x4797)), w_3, h_3, 2);
}

void restore_picbuf(PhysPt ptr)
{
	PhysPt p;
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
		copy_to_screen(Real2Phys(ds_readd(0x479f)),
			p, w_1, h_1, 0);
	}

	p = ptr + y_2 * 320 + x_2;
	copy_to_screen(Real2Phys(ds_readd(0x479b)), p, w_2, h_2, 0);

	p = ptr + y_3 * 320 + x_3;
	copy_to_screen(Real2Phys(ds_readd(0x4797)), p, w_3, h_3, 0);
}

/**
 * print_attribs() -	print the attribute values
 *
 */
void print_attribs()
{
	char buf[10];
	Bit16u i;

	for (i = 0; i < 14; i++) {
		/* don't print 0s */
		if (hero.attribs[i].normal == 0)
			continue;

		/* convert value to string with itoa() */
		sprintf(buf, "%d", hero.attribs[i].normal);

		/* print it */
		print_str(buf, ds_readw(i * 4 + 0x105f),
			ds_readw(i * 4 + 0x1061));
	}
}

/**
 * print_values() - print the values of the character
 *
 */
void print_values()
{
	char tmp[4];
	const Bit16u align_left = 222;
	const Bit16u align_right = 302;
	Bit16u width;
	Bit16s i, pos;


	switch (ds_readws(GEN_PAGE)) {

		case 0: {
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			/* print name */
			print_str(hero.name, 180, 12);

			/* print attributes */
			print_attribs();

			/* break if no typus */
			if (hero.typus == 0)
				break;

			/* print height */
			sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(70), hero.height);
			print_str((char*)Real2Host(ds_readd(GEN_PTR2)), 205, 25);

			/* print weight */
			sprintf((char*)Real2Host(ds_readd(GEN_PTR2)), get_text(71), hero.weight);

			print_str((char*)Real2Host(ds_readd(GEN_PTR2)), 205, 37);

			/* print god name */
			print_str(get_text(56 + hero.god), 205, 49);

			/* print money */
			make_valuta_str((char*)Real2Host(ds_readd(GEN_PTR2)), hero.money);
			print_str((char*)Real2Host(ds_readd(GEN_PTR2)), 205, 61);

			/* print LE */
			/* originally it was itoa() */
			sprintf(tmp, "%d", hero.le_max);
			print_str(tmp, 172, 164);

			/* print AE */
			/* originally it was itoa() */
			sprintf(tmp, "%d", hero.ae_max);
			print_str(tmp, 221, 164);

			/* print Endurance */
			/* originally it was itoa() */
			sprintf(tmp, "%d",
				hero.le_max + hero.attribs[6].current);
			print_str(tmp, 296, 164);

			/* print MR */
			/* originally it was itoa() */
			sprintf(tmp, "%d", hero.mr);
			print_str(tmp, 232, 184);

			break;
		}
		case 1: {
			/* SKILLS Page 1/3 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			/* print fight skills */
			for (i = 0; i < 9; i++) {
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.skills[i]);
				width = get_str_width(tmp);

				if (i & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(i) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(i) / 2) * 12 + 42);
			}

			/* print body skills */
			for (i = 9; i < 19; i++) {
				pos = i - 9;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.skills[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 119);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 119);
			}

			/* remaining attempts for skills */
			/* originally it was itoa() */
			sprintf(tmp, "%d", hero.skill_incs);
			print_str(tmp, 271, 184);

			break;
		}
		case 2: {
			/* SKILLS Page 2/3 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			/* print social skills */
			for (i = 19; i < 26; i++) {
				pos = i - 19;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.skills[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 42);
			}

			/* print lore skills */
			for (i = 32; i < 41; i++) {
				pos = i - 32;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.skills[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 113);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 113);
			}

			/* remaining attempts for skills */
			/* originally it was itoa() */
			sprintf(tmp, "%d", hero.skill_incs);
			print_str(tmp, 271, 184);

			break;
		}
		case 3: {
			/* SKILLS Page 3/3 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			/* print craftmansship skills */
			for (i = 41; i < 50; i++) {
				pos = i - 41;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.skills[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 42);
			}

			/* print nature skills */
			for (i = 26; i < 32; i++) {
				pos = i - 26;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.skills[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 119);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 119);
			}

			/* print intuition skills */
			for (i = 50; i < 52; i++) {
				pos = i - 50;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.skills[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, align_right - width,
						(abs(pos) / 2) * 12 + 170);
				else
					/* print in left column */
					print_str(tmp, align_left - width,
						(abs(pos) / 2) * 12 + 170);
			}

			/* remaining attempts for skills */
			/* originally it was itoa() */
			sprintf(tmp, "%d", hero.skill_incs);
			print_str(tmp, 271, 184);

			break;
		}
		case 4: {
			/* ATPA Page */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			/* Print base value  2x the same */
			sprintf(tmp, "%d", hero.atpa);
			print_str(tmp, 231, 30);
			sprintf(tmp, "%d", hero.atpa);
			print_str(tmp, 268, 30);

			for (i = 0; i < 7; i++) {
				/* print AT value */
				sprintf(tmp, "%d", hero.at[i]);

				print_str(tmp, 237 - get_str_width(tmp),
					i * 12 + 48);

				/* print PA value */
				sprintf(tmp, "%d", hero.pa[i]);

				print_str(tmp, 274 - get_str_width(tmp),
					i * 12 + 48);

				/* print skill value */
				sprintf(tmp, "%d", hero.skills[i]);
				print_str(tmp, 315 - get_str_width(tmp),
					i * 12 + 48);
			}

			/* calc range base value (KL+GE+KK)/4 */
			pos = (hero.attribs[1].normal + hero.attribs[4].normal +
				hero.attribs[6].normal) / 4;

			/* print missle weapon value */
			sprintf(tmp, "%d", hero.skills[7] + pos);

			print_str(tmp, 231, 144);

			/* print thrown weapon value */
			sprintf(tmp, "%d", hero.skills[8] + pos);
			print_str(tmp, 231, 156);

			break;
		}

		case 5: {
			/* Spells Page 1/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 1; i < 6; i++) {
				pos = i - 1;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}
			for (i = 33; i < 38; i++) {
				pos = i - 33;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 95);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 95);
			}
			for (i = 6; i <= 11; i++) {
				pos = i - 6;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 146);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 146);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			print_str(tmp, 217, 184);

			break;
		}
		case 6: {
			/* Spells Page 2/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 12; i <= 17; i++) {
				pos = i - 12;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}
			for (i = 18; i < 24; i++) {
				pos = i - 18;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 95);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 95);
			}
			for (i = 24; i < 27; i++) {
				pos = i - 24;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 146);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 146);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			print_str(tmp, 217, 184);

			break;
		}
		case 7: {
			/* Spells Page 3/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 27; i < 33; i++) {
				pos = i - 27;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}

			for (i = 38; i < 45; i++) {
				pos = i - 38;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 95);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 95);
			}

			for (i = 45; i <= 46; i++) {
				pos = i - 45;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 160);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 160);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			print_str(tmp, 217, 184);

			break;
		}
		case 8: {
			/* Spells Page 4/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 47; i <= 48; i++) {
				pos = i - 47;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}

			for (i = 49; i < 58; i++) {
				pos = i - 49;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 71);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 71);
			}

			for (i = 58; i < 60; i++) {
				pos = i - 58;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 148);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 148);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			print_str(tmp, 217, 184);

			break;
		}
		case 9: {
			/* Spells Page 5/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 60; i < 76; i++) {
				pos = i - 60;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			print_str(tmp, 217, 184);

			break;
		}
		case 10: {
			/* Spells Page 6/6 */
			restore_picbuf(Real2Phys(ds_readd(0x47c7)));

			for (i = 76; i < 86; i++) {
				pos = i - 76;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				width = get_str_width(tmp);

				if (pos & 1)
					/* print in right column */
					print_str(tmp, 302 - width,
						(abs(pos) / 2) * 12 + 42);
				else
					/* print in left column */
					print_str(tmp, 157 - width,
						(abs(pos) / 2) * 12 + 42);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			print_str(tmp, 217, 184);

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
void make_valuta_str(char *dst, unsigned int money)
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

	sprintf(dst, get_text(69), d, s, money);
}

void inc_skill(Bit16u skill, Bit16u max, char *msg)
{
	/* no more increments than the maximum */
	if (skill_incs[skill].incs >= max) {
		infobox((char*)msg, 0);
		return;
	}
	/* we just have 3 tries to increment */
	if (skill_incs[skill].tries == 3) {
		infobox(get_text(151), 0);
		return;
	}

	/* decrement total number of skill inc tries */
	hero.skill_incs--;
	if (random_interval_gen(2, 12) > hero.skills[skill]) {
		/* print sucess message */
		infobox(get_text(152), 0);
		/* increment skill */
		hero.skills[skill]++;
		/* reset tries */
		skill_incs[skill].tries = 0;
		/* increment skill increments */
		skill_incs[skill].incs++;

		/* check if we have a melee attack skill */
		if (skill <= 6) {
			/* check if AT > PA */
			if (hero.at[skill] > hero.pa[skill]) {
				/* inc PA */
				hero.pa[skill]++;
			} else {
				/* inc AT */
				hero.at[skill]++;
			}
		}
	} else {
		/* print failure message */
		infobox(get_text(153), 0);
		/* increment try */
		skill_incs[skill].tries++;
	}

	refresh_screen();
}

void select_skill()
{
	Bit16s skill, group;

	do {

		/* check skill attempts */
		if (hero.skill_incs == 0) {
			infobox(get_text(94), 0);
			return;
		}

		ds_writew(0x1327, 0xffb0);

		switch (ds_readws(GEN_PAGE)) {
			case 1: {
				group = gui_radio((Bit8u*)get_text(93), 2, get_text(86), get_text(87));
				if (group == -1)
					break;

				switch (group) {
					case 1: {
						/* Fight */
						skill = gui_radio((Bit8u*)get_text(147),
								9,
								get_text(95), get_text(96), get_text(97),
								get_text(98), get_text(99), get_text(100),
								get_text(101), get_text(102), get_text(103));
						skill--;

						if (skill == -2)
							break;

						inc_skill(skill, 1, get_text(148));

						break;
					}
					case 2: {
						/* body */
						skill = gui_radio((Bit8u*)get_text(147),
							10,
							get_text(104), get_text(105),
							get_text(106), get_text(107),
							get_text(108), get_text(109),
							get_text(110), get_text(111),
							get_text(112), get_text(113));
						skill--;

						if (skill == -2)
							break;

						skill += 9;
						inc_skill(skill, 2, get_text(149));
						break;
					}
				}
				break;
			}
			case 2: {
				group = gui_radio((Bit8u*)get_text(93), 2, get_text(88), get_text(89));
				if (group == -1)
					break;
				switch (group) {
					case 1: {
						skill = gui_radio((Bit8u*)get_text(147),
								7,
								get_text(114), get_text(115), get_text(116),
								get_text(117), get_text(118), get_text(119),
								get_text(120));
						skill--;

						if (skill == -2)
							break;

						skill += 19;
						inc_skill(skill, 2, get_text(149));
						break;
					}
					case 2: {
						skill = gui_radio((Bit8u*)get_text(147),
								9,
								get_text(127), get_text(128), get_text(129),
								get_text(130), get_text(131), get_text(132),
								get_text(133), get_text(134), get_text(135));
						skill--;

						if (skill == -2)
							break;

						skill += 32;
						inc_skill(skill, 3, get_text(150));
						break;
					}
				}
				break;
			}
			case 3: {
				group = gui_radio((Bit8u*)get_text(93), 3, get_text(90), get_text(91), get_text(92));
				if (group == -1)
					break;

				switch (group) {
					case 1: {
						skill = gui_radio((Bit8u*)get_text(147),
								9,
								get_text(136), get_text(137), get_text(138),
								get_text(139), get_text(140), get_text(141),
								get_text(142), get_text(143), get_text(144));

						skill--;

						if (skill == -2)
							break;

						skill += 41;
						inc_skill(skill, 2, get_text(149));
						break;
					}
					case 2: {
						skill = gui_radio((Bit8u*)get_text(147),
								6,
								get_text(121), get_text(122), get_text(123),
								get_text(124), get_text(125), get_text(126));

						skill--;

						if (skill == -2)
							break;

						skill += 26;
						inc_skill(skill, 2, get_text(149));
						break;
					}
					case 3: {
						skill = gui_radio((Bit8u*)get_text(147),
								2,
								get_text(145),
								get_text(146));

						skill--;

						if (skill == -2)
							break;

						skill += 50;
						inc_skill(skill, 1, get_text(148));
						break;
					}
				}
				break;
			}
		}
		ds_writew(0x1327, 0);

	} while (group != -1);
}

void inc_spell(Bit16u spell)
{
	Bit16u max_incs = 1;

	/* if typus == warlock and the origin of the spell is warlock */
	if ((hero.typus == 7) && (spelltab[spell].origin == 3))
		max_incs = 2;
	/* if typus == elf and the origin of the spell is elven */
	if ((hero.typus >= 10) && (spelltab[spell].origin == 2))
		max_incs = 2;
	/* if typus == druid and the origin of the spell is druid */
	if ((hero.typus == 8) && (spelltab[spell].origin == 0))
		max_incs = 2;
	/* if typus == mage */
	if (hero.typus == 9) {
		/* and the origin of the spell is mage */
		if (spelltab[spell].origin == 1)
			max_incs = 2;

		Bit8u *array = (Bit8u*)house_spells[hero.school];
		/* and is a school spell */
		if (is_in_word_array(spell, (signed short*)array))
			max_incs = 3;
	}

	/* all spell increments used for that spell */
	if (spell_incs[spell].incs >= max_incs) {
		infobox(get_text(257), 0);
		return;
	}
	/* all tries used for that spell */
	if (spell_incs[spell].tries == 3) {
		infobox(get_text(151), 0);
		return;
	}

	/* decrement spell attempts */
	hero.spell_incs--;

	if (random_interval_gen(2, 12) > hero.spells[spell]) {
		/* show success */
		infobox(get_text(152), 0);
		/* increment spell value */
		hero.spells[spell]++;
		/* reset tries */
		spell_incs[spell].tries = 0;
		/* increment incs */
		spell_incs[spell].incs++;
	} else {
		/* show failure */
		infobox(get_text(153), 0);
		/* increment tries */
		spell_incs[spell].tries++;
	}

	refresh_screen();
}

void select_spell()
{
	Bit16s group, spell;

	do {

		/* check if we have spell attempts */
		if (hero.spell_incs == 0) {
			infobox(get_text(94), 0);
			ds_writew(0x1327, 0);
			return;
		}

		ds_writew(0x1327, 0xffa6);

		switch (ds_readws(GEN_PAGE)) {
			case 5: {
				group = gui_radio((Bit8u*)get_text(155), 3,
						get_text(157), get_text(162),
						get_text(158));
				if (group == -1)
					break;

				switch (group) {
					case 1: {
						spell = gui_radio((Bit8u*)get_text(156), 5,
								get_text(169),
								get_text(170),
								get_text(171),
								get_text(172),
								get_text(173));
						spell--;
						if (spell == -2)
							break;
						spell++;
						inc_spell(spell);
						break;
					}
					case 2: {
						spell = gui_radio((Bit8u*)get_text(156), 5,
								get_text(201),
								get_text(202),
								get_text(203),
								get_text(204),
								get_text(205));
						spell--;
						if (spell == -2)
							break;
						spell += 33;
						inc_spell(spell);
						break;
					}
					case 3: {
						spell = gui_radio((Bit8u*)get_text(156), 6,
								get_text(174),
								get_text(175),
								get_text(176),
								get_text(177),
								get_text(178),
								get_text(179));
						spell--;
						if (spell == -2)
							break;
						spell += 6;
						inc_spell(spell);
						break;
					}
				}
				break;
			}
			case 6: {
				group = gui_radio((Bit8u*)get_text(155), 3,
						get_text(158), get_text(159),
						get_text(160));
				if (group == -1)
					break;

				switch (group) {
					case 1: {
						spell = gui_radio((Bit8u*)get_text(156), 6,
								get_text(180),
								get_text(181),
								get_text(182),
								get_text(183),
								get_text(184),
								get_text(185));

						spell--;

						if (spell == -2)
							break;

						spell += 12;
						inc_spell(spell);
						break;
					}
					case 2: {
						spell = gui_radio((Bit8u*)get_text(156), 6,
								get_text(186),
								get_text(187),
								get_text(188),
								get_text(189),
								get_text(190),
								get_text(191));

						spell--;

						if (spell == -2)
							break;

						spell += 18;
						inc_spell(spell);
						break;
					}
					case 3: {
						spell = gui_radio((Bit8u*)get_text(156), 3,
								get_text(192),
								get_text(193),
								get_text(194));

						spell--;

						if (spell == -2)
							break;

						spell += 24;
						inc_spell(spell);
						break;
					}
				}

				break;
			}
			case 7: {
				group = gui_radio((Bit8u*)get_text(155), 3,
						get_text(161), get_text(163),
						get_text(164));
				if (group == -1)
					break;

				switch (group) {
					case 1: {
						spell = gui_radio((Bit8u*)get_text(156), 6,
								get_text(195),
								get_text(196),
								get_text(197),
								get_text(198),
								get_text(199),
								get_text(200));

						spell--;

						if (spell == -2)
							break;

						spell += 27;
						inc_spell(spell);
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
								get_text(212));

						spell--;

						if (spell == -2)
							break;

						spell += 38;
						inc_spell(spell);
						break;
					}
					case 3: {
						spell = gui_radio((Bit8u*)get_text(156), 2,
								get_text(213),
								get_text(214));

						spell--;

						if (spell == -2)
							break;

						spell += 45;
						inc_spell(spell);
						break;
					}
				}

				break;
			}
			case 8: {
				group = gui_radio((Bit8u*)get_text(155), 3,
						get_text(164), get_text(86),
						get_text(166));
				if (group == -1)
					break;


				switch (group) {
					case 1: {
						spell = gui_radio((Bit8u*)get_text(156), 2,
								get_text(215),
								get_text(216));

						spell--;

						if (spell == -2)
							break;

						spell += 47;
						inc_spell(spell);
						break;
					}
					case 2: {
						spell = gui_radio((Bit8u*)get_text(156), 9,
								get_text(217), get_text(218), get_text(219),
								get_text(220), get_text(221), get_text(222),
								get_text(223), get_text(224), get_text(225));

						spell--;

						if (spell == -2)
							break;

						spell += 49;
						inc_spell(spell);
						break;
					}
					case 3: {
						spell = gui_radio((Bit8u*)get_text(156), 2,
								get_text(226),
								get_text(227));

						spell--;

						if (spell == -2)
							break;

						spell += 58;
						inc_spell(spell);
						break;
					}
				}
				break;
				/* TODO */
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
						get_text(242), get_text(243));
				spell--;

				if (spell == -2) {
					group = -1;
					break;
				}

				spell += 60;
				inc_spell(spell);
				break;
			}
			case 10: {
				spell = gui_radio((Bit8u*)get_text(156), 10,
						get_text(244), get_text(245),
						get_text(246), get_text(247),
						get_text(248), get_text(249),
						get_text(250), get_text(251),
						get_text(252), get_text(253));
				spell--;

				if (spell == -2) {
					group = -1;
					break;
				}

				spell += 76;
				inc_spell(spell);
				break;
			}
		}
		ds_writew(0x1327, 0);
	} while (group != -1);
}

void choose_atpa()
{
	Bit16u skill, increase;

	ds_writew(0x1327, 0xffb0);

	do {
		/* print menu with all melee weapons skills */
		skill = gui_radio((Bit8u*)get_text(78), 7,
			get_text(95), get_text(96), get_text(97), get_text(98),
			get_text(99), get_text(100), get_text(101)) - 1;

		if (skill != 0xfffe) {
			if (hero.skills[skill] > 0) {
				increase = gui_radio((Bit8u*)get_text(254), 2,
					get_text(75), get_text(76));
				if (increase != 0xffff) {
					if (increase == 1) {
						/* increase attack */
						if (hero.skills[skill] >= 0 &&
							hero.pa[skill] > hero.atpa) {
							/* inc AT */
							hero.at[skill]++;
							/* dec PA */
							hero.pa[skill]--;
							refresh_screen();
						} else {
							infobox(get_text(255), 0);
						}
					} else {
						if (hero.skills[skill] >= 0 &&
							hero.at[skill] > hero.atpa) {
							/* dec AT */
							hero.at[skill]--;
							/* inc PA */
							hero.pa[skill]++;
							refresh_screen();
						} else {
							infobox(get_text(256), 0);
						}
					}
				}
			} else {
				infobox(get_text(260), 0);
			}
		}

	} while (skill != 0xfffe);

	ds_writew(0x1327, 0);
}

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

	if (hero.sex)
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
	strcpy(name_bak, hero.name);
	sex_bak = hero.sex;
	memset(&hero, 0, sizeof(hero));
	clear_hero();
	hero.sex = sex_bak;
	strcpy(hero.name, name_bak);

	/* set typus */
	hero.typus = (unsigned char)choosen_typus;

	/* roll out good attribute values */
	for (i = 0; i < 7; i ++) {

		randval = (unsigned char)random_interval_gen(8, 13);

		if (randval > 8)
			randval--;

		hero.attribs[i].normal = randval;
		hero.attribs[i].current = randval;
	}

	/* roll out bad attribute values */
	for (i = 0; i < 7; i ++) {

		randval = (unsigned char)random_interval_gen(2, 7);

		if (randval < 7)
			randval++;

		hero.attribs[i + 7].normal = randval;
		hero.attribs[i + 7].current = randval;
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
			if (hero.attribs[ta].normal <= (randval & 0x7f))
				continue;

			hero.attribs[ta].current = randval & 0x7f;
			hero.attribs[ta].normal = randval & 0x7f;
		} else {
			/* attribute lower bound */
			if (hero.attribs[ta].normal >= randval)
				continue;

			hero.attribs[ta].current = randval;
			hero.attribs[ta].normal = randval;
		}
	}

	load_typus(hero.typus);
	update_mouse_cursor();
	call_fill_rect_gen((RealPt)ds_readd(VGA_MEMSTART), 16, 8, 143, 191, 0);
	wait_for_vsync();
	set_palette(Real2Host(ds_readd(GEN_PTR5)) + 0x5c02, 0, 32);
	call_mouse();

	if (hero.typus > 10)
		head_typus = 10;
	else
		head_typus = hero.typus;

	if (hero.sex) {
		ds_writeb(HEAD_CURRENT, head_first_female[head_typus]);
		head_first = head_first_female[head_typus];
		head_last = head_first_male[head_typus + 1] - 1;
	} else {
		ds_writeb(HEAD_CURRENT, head_first_male[head_typus]);
		head_first = head_first_male[head_typus];
		head_last = head_first_female[head_typus] - 1;
	}
	fill_values();
	ds_writew(0x11fe, 1);
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

	free(picbuf3);
	free(picbuf2);
	free(picbuf1);
	bc_free((RealPt)ds_readd(GEN_PTR6) - 8);
	bc_free((RealPt)ds_readd(BUFFER_DMENGE_DAT));
	free(gen_ptr5);
	free(gen_ptr4);
	bc_free((RealPt)ds_readd(GEN_PTR2));

	picbuf3 = NULL;
	picbuf2 = NULL;
	picbuf1 = NULL;
	//gen_ptr6 = NULL;
	//buffer_dmenge_dat = NULL;
	gen_ptr5 = NULL;
	gen_ptr4 = NULL;
	//gen_ptr2 = NULL;

	free(page_buffer);
	bc_free(ds_readd(GEN_PTR1_DIS) - 8);

	page_buffer = NULL;
	gen_ptr1 = NULL;

	for (long i = 0; i < MAX_PAGES; i++) {
		if (bg_buffer[i]) {
			free(bg_buffer[i]);
			bg_buffer[i] = NULL;
		}
		sum += bg_len[i];
		bg_len[i] = 0;
	}
	for (long i = 0; i < MAX_TYPES; i++) {
		if (typus_buffer[i]) {
			free(typus_buffer[i]);
			typus_buffer[i] = NULL;
		}
		sum += typus_len[i];
		typus_len[i] = 0;
	}
	D1_INFO("Cleanup %ld bytes freed\n", sum);
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

	set_textcolor(0xff, 0);
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
		param_level = argv[2][0];

	if ((argc > 3) && (argv[3][0] == '0')) {
		ds_writew(MIDI_DISABLED, 1);
		sound_off = 1;
	};

	ds_writeb(0x40b8, 1);

	if (sound_off == 0)
		init_music(13000);

	wo_var2 = ret_zero1();

	set_mouse_isr();

	bc_randomize();

	save_display_stat(RealMake(datseg, 0x47db));

	alloc_buffers();
	alloc_buffers_emu();

	wo_var3 = 2;

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
	ds_writed(0x47c7, RealMake(0xa000, 0x0));

	ds_writed(GEN_PTR1_DIS, (RealPt)emu_gen_alloc(64108) + 8);

	ds_writed(PAGE_BUFFER, (Bit32u)emu_gen_alloc(50000));

	ds_writed(GEN_PTR2, (Bit32u)emu_gen_alloc(1524));
	ds_writed(GEN_PTR3, (RealPt)ds_readd(GEN_PTR2) + 1500);

	// unused
	gen_ptr4 = (Bit8u*)gen_alloc(200);

	ds_writed(BUFFER_TEXT, (Bit32u)emu_gen_alloc(6000));

	ds_writed(BUFFER_FONT6, (Bit32u)emu_gen_alloc(592));

	load_font_and_text();

	ds_writed(BUFFER_HEADS_DAT, (Bit32u)emu_gen_alloc(39000));

	ds_writed(BUFFER_POPUP, (Bit32u)emu_gen_alloc(1673));

	ds_writed(BUFFER_SEX_DAT, (Bit32u)emu_gen_alloc(812));

	gen_ptr5 = (Bit8u*)gen_alloc(23660);

	ds_writed(BUFFER_DMENGE_DAT, (Bit32u)emu_gen_alloc(23660));

	picbuf1 = (Bit8u*)gen_alloc(800);

	picbuf2 = (Bit8u*)gen_alloc(2800);

	picbuf3 = (Bit8u*)gen_alloc(2800);

	ds_writed(GEN_PTR6, (Bit32u)emu_gen_alloc(1100) + 8);

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
	set_textcolor(0xff, 0x0);
}

void init_stuff()
{
	init_colors();

	/* these 3 variables are bogus */
	fg_color[1] = 0xc8;
	fg_color[2] = 0xc9;
	fg_color[3] = 0xca;

	/* number of menu tiles width */
	ds_writew(MENU_TILES, 3);

	ds_writed(DST_DST, ds_readd(VGA_MEMSTART));
}

void* gen_alloc(unsigned long size)
{
	D1_INFO("HOST gen_alloc(%ld);\n", size);
	return calloc(size, sizeof(char));
}

#if !defined(__BORLANDC__)
RealPt emu_gen_alloc(Bit32u nelem)
{
	return bc_calloc(nelem, 1);
}
#endif

#endif

#if !defined(__BORLANDC__)
}
#endif
