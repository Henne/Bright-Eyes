#include <stdlib.h>
#include <string.h>

#include "dosbox.h"

#include "schweif.h"

#include "c102de_seg000.h"
#include "c102de_seg029.h"
#include "c102de_seg033.h"
#include "c102de_seg037.h"
#include "c102de_seg136.h"

using namespace C102de;

#if 0
static int seg016(unsigned short offs) {
    switch(offs) {
    case 0x000B:{ // decode_image(...)
	D2_LOG("decoding image");
	return 0;
    }
    default: return 0;
    }
}

static int segUnk(unsigned segm, unsigned short offs) {
    switch(offs) {
    case 0x9C9:
	D2_LOG("img@%04x\n", segm);
	return 0;
    }
	return 0;
}


#endif

static int seg029(unsigned short offs)
{
	switch (offs) {
		case 0x0e: {
			RealPt dst = CPU_Pop32();
			RealPt src = CPU_Pop32();
			RealPt length = CPU_Pop32();
			CPU_Push32(length);
			CPU_Push32(src);
			CPU_Push32(dst);
			//D2_LOG("decomp(%x, %x, %x)\n", dst, src, length);
			decomp_pp20(dst, Real2Host(src), length);
			return 1;
		}
		default: return 0;
	}
}

static int seg033(unsigned short offs)
{
	switch (offs) {
		case 0x0e: {
			RealPt src = CPU_Pop32();
			RealPt dst = CPU_Pop32();
			RealPt src_data = CPU_Pop32();
			Bit32u length = CPU_Pop32();
			CPU_Push32(length);
			CPU_Push32(src_data);
			CPU_Push32(dst);
			CPU_Push32(src);
			//D2_LOG("ppDecrunch(%x, %x, %x, %x)\n",
			//	src, dst, src_data, length);
			return 0;
		}
		default: return 0;
	}
}

static int seg037(unsigned short offs)
{
	switch(offs) {
		case 0x05: {
			Bit16u lowerBound = CPU_Pop16();
			Bit16u upperBound = CPU_Pop16();
			CPU_Push16(upperBound);
			CPU_Push16(lowerBound);
			reg_ax = random_interval(lowerBound, upperBound);
			D2_LOG("randomInterval(%d, %d) = %d\n",
				lowerBound, upperBound, reg_ax);
			return 1;
		}
		case 0x25: {
			Bit16u range = CPU_Pop16();
			CPU_Push16(range);
			reg_ax = random_schweif(range);
			D2_LOG("random(%d) = %d\n", range & 0xFF, reg_ax);
			return 1;
		}
		case 0x73: {
			Bit16u n = CPU_Pop16();
			Bit16u m = CPU_Pop16();
			Bit16s x = CPU_Pop16();
			CPU_Push16(x);
			CPU_Push16(m);
			CPU_Push16(n);
			reg_ax = dice_roll(n, m, x);
			D2_LOG("wuerfel %dW%d%+d = %d\n", n, m, x, reg_ax);
			return 1;
		}
		case 0xa2: {
			Bit16u n = CPU_Pop16();
			Bit16u m = CPU_Pop16();
			Bit16s x = CPU_Pop16();
			RealPt min = CPU_Pop32();
			RealPt max = CPU_Pop32();
			CPU_Push32(max);
			CPU_Push32(min);
			CPU_Push16(x);
			CPU_Push16(m);
			CPU_Push16(n);
			calc_damage_range(n, m, x, Real2Host(min),
				Real2Host(max));
			return 1;
		}
	}
	return 0;
}

static int seg136(unsigned short offs)
{
	switch(offs) {
#if 0
    case 0x4D:{
	D2_LOG("Probe-Basis\n");
	return 0;
    }
#endif
		case 0x48: {
			RealPt hero = CPU_Pop32();
			Bit16u attrib = CPU_Pop16();
			Bit16s bonus = CPU_Pop16();
			CPU_Push16(bonus);
			CPU_Push16(attrib);
			CPU_Push32(hero);
			test_attrib(Real2Host(hero), attrib, bonus);
			return 1;
		}
		case 0x61:{
			RealPt hero = CPU_Pop32();
			Bit16u skill = CPU_Pop16();
			Bit16s bonus = CPU_Pop16();
			CPU_Push16(bonus);
			CPU_Push16(skill);
			CPU_Push32(hero);
			D2_LOG("Talentprobe %s auf %s %+d\n",
				(char*)Real2Host(hero + 0x22),
				schweif_common::names_skill[skill],
				(signed char)bonus);
		return 0;
	    }
	    default:
		return 0;
	}
}

static int seg151(unsigned short offs)
{
	switch (offs) {
		case 0x2a: {
			RealPt hero = CPU_Pop32();
			Bit16u spell = CPU_Pop16();
			Bit16s bonus = CPU_Pop16();
			CPU_Push16(bonus);
			CPU_Push16(spell);
			CPU_Push32(hero);
			D2_LOG("Zauberprobe %s auf %s %+d\n",
					(char*)(Real2Host(hero) + 0x22),
					schweif_common::names_spell[spell],
					(signed char)bonus);
			return 0;
		}
		default: return 0;
	}
}

// Intercept far CALLs (both 32 and 16 bit)
int schweif_farcall_c102de(unsigned segm, unsigned offs)
{
	switch (segm) {
#if 0
    case 0x0000: return seg000(offs);
#endif
	case 0x1a8a: return seg029(offs);
	case 0x1aa4: return seg033(offs);
	case 0x1b27: return seg037(offs);
	case 0x20be: return seg136(offs);
	case 0x2119: return seg151(offs);
#if 0
    default: return segUnk(segm, offs);
#endif
	}
	return 0;
}

int schweif_nearcall_c102de(unsigned offs) {
#if 0
    unsigned short segm = SegValue(cs)-relocation;

    /* Borland C-Lib */
    if (segm == 0x0000) {
	switch(offs) {
	default: return 0;
	}
    }
    if (segm == 0x1B27) {
	switch(offs) {
	case 0x21:{
	    unsigned short range = CPU_Pop16();
	    CPU_Push16(range);
	    D2_LOG("random(%d)\n", range & 0xFF);
	    return 0;
	}
	default:return 0;
	}
    }
    if (segm == 0x20C8) {
	switch(offs) {
	case 0x72D:
	    D2_LOG("Probe-Basis\n");
	    return 0;
	default: return 0;
	}
    }
#endif
    return 0;
}
