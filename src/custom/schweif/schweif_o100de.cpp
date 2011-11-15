#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "SDL.h"

#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "dos_inc.h"

#include "schweif.h"

#include "o100de_seg000.h"
#include "o100de_seg037.h"

using namespace schweif_o100de;
using namespace schweif_common;

static int seg016(unsigned short offs) {
    switch(offs) {
    case 0x000B:{ // decode_image(...)
	D2_LOG("decoding image");
	return 0;
    }
    default: return 0;
    }
}

// "Unbekanntes Segment" zu Forschungszwecken
static int segUnk(unsigned segm, unsigned short offs) {
    switch(offs) {
    case 0x00E8:
	//D2_LOG("img@%04x\n", segm);
	return 0;
    }
    return 0;
}


static int seg037(unsigned short offs) {
    switch(offs) {
    case 0x21:{
	unsigned short range = CPU_Pop16();
	CPU_Push16(range);
	reg_ax = random_schweif(range);
	D2_LOG("random(%d) = %d\n", range & 0xFF, reg_ax);
	return 1;
    }
    case 0x01:{
	unsigned short lowerBound = CPU_Pop16();
	unsigned short upperBound = CPU_Pop16();
	CPU_Push16(upperBound);
	CPU_Push16(lowerBound);
	reg_ax = random_interval(lowerBound, upperBound);
	D2_LOG("randomInterval(%d, %d) = %d\n", lowerBound, upperBound, reg_ax);
	return 1;
    }
    }
    return 0;
}

static int seg20C8(unsigned short offs) {
    // Stub 0138
    switch(offs) {
    case 0x4D:{
	D2_LOG("Probe-Basis\n");
	return 0;
    }
    case 0x48:{
	RealPt hero = CPU_Pop32();
	unsigned short attrib = CPU_Pop16();
	signed short bonus = CPU_Pop16();
	CPU_Push16(bonus);
	CPU_Push16(attrib);
	CPU_Push32(hero);
	D2_LOG("Eigenschafts-Probe: %s + %d\n", names_attrib[attrib], bonus & 0xFF);
	return 0;
    }
    case 0x61:{
	RealPt hero = CPU_Pop32();
	unsigned short skill = CPU_Pop16();
	signed short bonus = CPU_Pop16();
	CPU_Push16(bonus);
	CPU_Push16(skill);
	CPU_Push32(hero);
	D2_LOG("Talentprobe: %s + %d\n", names_skill[skill], bonus & 0xFF);
	return 0;
    }
    default: return 0;
    }
    return 0;
}

//Feilschen: stub:0136=seg:20B7

static int seg2123(unsigned short offs) {
    switch(offs) {
    case 0x2A:{
	RealPt hero = CPU_Pop32();
	unsigned short spell = CPU_Pop16();
	signed short bonus = CPU_Pop16();
	CPU_Push16(bonus);
	CPU_Push16(spell);
	CPU_Push32(hero);
	D2_LOG("Zauberprobe: %s + %d\n", names_spell[spell], bonus & 0xFF);
	return 0;
    }
    default: return 0;
    }
    return 0;
}


// Intercept far CALLs (both 32 and 16 bit)
int schweif_farcall_v100de(unsigned segm, unsigned offs) {
    switch (segm) {
    case 0x0000: return seg000(offs);
    case 0x1B26: return seg037(offs);
    case 0x20C8: return seg20C8(offs);
    case 0x2123: return seg2123(offs);
    default: return segUnk(segm, offs);
    }
    return 0;
}

int schweif_nearcall_v100de(unsigned offs) {
    unsigned short segm = SegValue(cs)-relocation;

    /* Borland C-Lib */
    if (segm == 0x0000) {
	switch(offs) {
	default: return 0;
	}
    }
    if (segm == 0x1B26) {
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
    return 0;
}
