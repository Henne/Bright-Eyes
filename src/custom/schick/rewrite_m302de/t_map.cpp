/**
 *	Helper function for treasure chests
 *
 *	Treasure chests are implemented, by using function pointers.
 *	These are stored in DS (the Datasegment) as RealMode pointers,
 *	so a mapping is needed for Bright-Eyes to get the adresses of
 *	these functions.
 */
#include <stdio.h>
#include <stdlib.h>

#include "v302de.h"

#include "seg080.h"
#include "seg088.h"
#include "seg090.h"
#include "seg091.h"
#include "seg092.h"
#include "seg110.h"
#include "seg111.h"

#include "t_map.h"

/* enable treasure debug output */
#define SCHICK_TREASURES

#ifdef SCHICK_TREASURES
#define D1_TREAS(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D1_TREAS(...) { }
#endif


namespace M302de {

/**
 * \brief returns a function pointer from a treasure description
 * \param ptr	RealMode pointer to the treasure description
 * \param off	offset of the pointer in the treasure description
 * \return pointer to the tresure function
 */
treasure_type t_map(RealPt ptr, const int off)
{
	/* only 3 offsets are allowed */
	if (off != 3 && off != 7 && off != 11) {
		D1_TREAS("ERROR: off hat einen ungueltigen Wert %d\n", off);
		return (treasure_type)NULL;
	}

	RealPt func = (RealPt)host_readd(Real2Host(ptr) + off);

	/* early out for NULL pointers */
	if (func == (RealPt)0)	return (treasure_type)NULL;

	Bit16u f_seg = RealSeg(func);
	Bit16u f_off = RealOff(func);

	switch (f_seg - reloc_game) {

	case 0x13e4: {
		/* TODO: seg080.cpp: wolfcave, cave */
		if (f_off == 0x25) return (treasure_type)DNG04_corpse0;
		if (f_off == 0x2f) return (treasure_type)DNG04_corpse1;
		if (f_off == 0x39) return (treasure_type)DNG04_corpse2;

		D1_ERR("ERROR: call to seg080:0x%x should not happen\n", f_off);
		exit(-1);
		break;
	}

	case 0x140b: {
		/* TODO: seg088.cpp: Dungeon Thorwal */
		switch (f_off) {
		case 0x2a:	return (treasure_type)DNG14_chest_x1;
		case 0x2f:	return (treasure_type)DNG14_chest_x2;
		case 0x34:	return (treasure_type)DNG14_chest_x3;
		case 0x39:	return (treasure_type)DNG14_chest_x4;
		case 0x3e:	return (treasure_type)DNG14_chest_x5;
		case 0x43:	return (treasure_type)DNG14_chest_x6;
		case 0x48:	return (treasure_type)DNG14_chest_x7;
		case 0x4d:	return (treasure_type)DNG14_chest_x8;
		case 0x52:	return (treasure_type)DNG14_chest_x9;
		}
		D1_TREAS("WARNING: call to seg088:0x%x\n", f_off);
		break;
	}

	case 0x1417: {
		/* TODO: seg090.cpp: Dungeon Oberorken */
		if (f_off == 0x25) return (treasure_type)DNG_oberorken_chest;

		D1_ERR("ERROR: call to seg090:0x%x should not happen\n", f_off);
		exit(-1);
		break;
	}

	case 0x141b: {
		/* TODO: seg091.cpp: Dungeon Prem */
		if (f_off == 0x2f) return (treasure_type)DNG13_corpse1;
		if (f_off == 0x34) return (treasure_type)DNG13_corpse0;
		if (f_off == 0x39) return (treasure_type)DNG13_chest1;
		if (f_off == 0x3e) return (treasure_type)DNG13_chest0;

		D1_ERR("ERROR: call to seg091:0x%x should not happen\n", f_off);
		exit(-1);
		break;
	}

	case 0x1420: {
		/* seg092.cpp: treasures generic */

		switch (f_off) {
		case 0x25:	return (treasure_type)chest_protected_normal;
		case 0x2a:	return (treasure_type)chest_closed;
		case 0x2f:	return (treasure_type)chest_protected_heavy;
		case 0x34:	return (treasure_type)chest_cursed;
		case 0x39:	return (treasure_type)chest_fulminictus;
		case 0x4d:	return (treasure_type)use_lockpicks_on_chest;
		case 0x52:	return (treasure_type)use_key_on_chest;
		case 0x5c:	return (treasure_type)chest_poisoned1;
		case 0x61:	return (treasure_type)chest_poisoned2;
		case 0x66:	return (treasure_type)chest_poisoned3;
		case 0x6b:	return (treasure_type)chest_protected_brutal;
		case 0x70:	return (treasure_type)chest_stoned;
		case 0x75:	return (treasure_type)chest_ignifax_normal;
		case 0x7a:	return (treasure_type)chest_ignifax_brutal;
		case 0x7f:	return (treasure_type)chest_ignifax_heavy;
		case 0x84:	return (treasure_type)chest_crossbow_bolts;
		}

		D1_TREAS("WARNING: call to seg092:0x%x\n", f_off);
		break;
	}

	case 0x14a7: {
		/* seg111.cpp: travel events 3 / 10 */

		switch (f_off) {
		case 0x25:	return (treasure_type)tevent_014_chest;
		}

		D1_TREAS("WARNING: call to seg111:0x%x\n", f_off);
		break;
	}

	case 0x14b4: {
		/* seg111.cpp: travel events 3 / 10 */

		switch (f_off) {
		case 0x20:	return (treasure_type)tevent_064_chest;
		}

		D1_TREAS("WARNING: call to seg111:0x%x\n", f_off);
		break;
	}
	default: {
		D1_TREAS("ERROR: Segment hat einen ungueltigen Wert 0x%x\n", (Bit16u)(f_seg - reloc_game));
	}

	}

	return (treasure_type)NULL;
}

}	// namespace M302de
