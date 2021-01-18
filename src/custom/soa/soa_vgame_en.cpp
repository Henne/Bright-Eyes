#include <stdlib.h>
#include <string.h>

#include "dosbox.h"

#include "custom.h"
#include "soa.h"

#include "soa_seg000.h"
#include "soa_seg001.h"
#include "soa_seg002.h"
#include "soa_seg003.h"
#include "soa_seg004.h"
#include "soa_seg005.h"
#include "soa_seg006.h"
#include "soa_seg007.h"
#include "soa_seg008.h"
#include "soa_seg009.h"
#include "soa_seg010.h"
#include "soa_seg011.h"
#include "soa_seg012.h"

using namespace SOA_VGAME_EN;

static int seg000(unsigned short offs) {
	switch(offs) {
	case 0x0127: {
		return 0;
	}
	case 0x0142: {
		return 0;
	}
	case 0x0185: {
		return 0;
	}
	case 0x01b2: {
		return 0;
	}
	case 0x01f5: {
		return 0;
	}
	case 0x0244: {
		return 0;
	}
	case 0x0271: {
		return 0;
	}
	case 0x029a: {
		return 0;
	}
	case 0x040e: {
		return 0;
	}
	case 0x04cd: {
		return 0;
	}
	case 0x053a: {
		return 0;
	}
	case 0x058c: {
		return 0;
	}
	case 0x0657: {
		return 0;
	}
	case 0x07f0: {
		return 0;
	}
	case 0x0803: {
		return 0;
	}
	case 0x0816: {
		return 0;
	}
	case 0x092a: {
		return 0;
	}
	case 0x0a99: {
		return 0;
	}
	case 0x0c3b: {
		return 0;
	}
	case 0x0c52: {
		return 0;
	}
	case 0x0c9e: {
		return 0;
	}
	case 0x0caf: {
		return 0;
	}
	case 0x0eb1: {
		return 0;
	}
	case 0x0fe0: {
		return 0;
	}
	case 0x113f: {
		return 0;
	}
	case 0x11c2: {
		return 0;
	}
	case 0x121a: {
		return 0;
	}
	case 0x1296: {
		return 0;
	}
	case 0x15c7: {
		return 0;
	}
	case 0x15f3: {
		return 0;
	}
	case 0x161f: {
		return 0;
	}
	case 0x164b: {
		return 0;
	}
	case 0x17b3: {
		return 0;
	}
	case 0x187e: {
		return 0;
	}
	case 0x18bb: {
		return 0;
	}
	case 0x18eb: {
		return 0;
	}
	case 0x1984: {
		return 0;
	}
	case 0x19b1: {
		return 0;
	}
	case 0x19bf: {
		return 0;
	}
	case 0x1a60: {
		return 0;
	}
	case 0x1b3b: {
		return 0;
	}
	case 0x1c2d: {
		return 0;
	}
	case 0x1c6e: {
		return 0;
	}
	case 0x1cde: {
		return 0;
	}
	case 0x1ced: {
		return 0;
	}
	case 0x1d2f: {
		return 0;
	}
	case 0x1dc8: {
		return 0;
	}
	case 0x1e0e: {
		return 0;
	}
	case 0x1e6c: {
		return 0;
	}
	case 0x1e95: {
		return 0;
	}
	case 0x1ed2: {
		return 0;
	}
	case 0x1ee3: {
		return 0;
	}
	case 0x1f0c: {
		return 0;
	}
	case 0x1f8e: {
		return 0;
	}
	case 0x1fda: {
		return 0;
	}
	case 0x2004: {
		return 0;
	}
	case 0x2028: {
		return 0;
	}
	case 0x204c: {
		return 0;
	}
	case 0x206b: {
		return 0;
	}
	case 0x210b: {
		return 0;
	}
	case 0x2257: {
		return 0;
	}
	case 0x22a5: {
		return 0;
	}
	case 0x2363: {
		return 0;
	}
	case 0x237f: {
		return 0;
	}
	case 0x23ac: {
		return 0;
	}
	case 0x23f2: {
		return 0;
	}
	case 0x2431: {
		return 0;
	}
	case 0x2461: {
		return 0;
	}
	case 0x248a: {
		return 0;
	}
	case 0x24a9: {
		return 0;
	}
	case 0x24da: {
		return 0;
	}
	case 0x26c6: {
		return 0;
	}
	case 0x27bd: {
		return 0;
	}
	case 0x27d7: {
		return 0;
	}
	case 0x27ed: {
		return 0;
	}
	case 0x2c8c: {
		return 0;
	}
	case 0x2cb2: {
		return 0;
	}
	case 0x2d2b: {
		return 0;
	}
	case 0x2edd: {
		return 0;
	}
	case 0x2f3e: {
		return 0;
	}
	case 0x2f9a: {
		return 0;
	}
	case 0x2fa4: {
		return 0;
	}
	case 0x3172: {
		return 0;
	}
	case 0x3189: {
		return 0;
	}
	case 0x31ad: {
		return 0;
	}
	case 0x3278: {
		return 0;
	}
	case 0x327b: {
		return 0;
	}
	case 0x3300: {
		return 0;
	}
	case 0x3303: {
		return 0;
	}
	case 0x3360: {
		return 0;
	}
	case 0x3363: {
		return 0;
	}
	case 0x338b: {
		return 0;
	}
	case 0x33a7: {
		return 0;
	}
	case 0x33be: {
		return 0;
	}
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		return 0;
//		exit(1);
	}
	}
}

static int seg001(unsigned short offs) {
	switch(offs) {
	case 0x005: {
		return 0;
	}
	case 0x040: {
		return 0;
	}
	case 0x107: {
		return 0;
	}
	case 0x128: {
		return 0;
	}
	case 0x479: {
		return 0;
	}
	case 0x6dc: {
		return 0;
	}
	case 0x7dc: {
		return 0;
	}
	case 0x8f1: {
		return 0;
	}
	case 0xa60: {
		return 0;
	}
	case 0xb38: {
		return 0;
	}
	case 0xb73: {
		return 0;
	}
	case 0xb86: {
		return 0;
	}
	case 0xbfe: {
		return 0;
	}
	case 0xdb8: {
		return 0;
	}
	case 0xe5a: {
		return 0;
	}
	case 0xe67: {
		return 0;
	}
	case 0xeb7: {
		return 0;
	}
	case 0xf1d: {
		return 0;
	}
	case 0xff1: {
		return 0;
	}
	case 0xff7: {
		return 0;
	}
	case 0xffd: {
		return 0;
	}
	case 0x1021: {
		return 0;
	}
	case 0x109c: {
		return 0;
	}
	case 0x10f0: {
		return 0;
	}
	case 0x13e4: {
		return 0;
	}
	case 0x1469: {
		return 0;
	}
	case 0x150f: {
		return 0;
	}
	case 0x1564: {
		return 0;
	}
	case 0x15d2: {
		return 0;
	}
	case 0x170f: {
		return 0;
	}
	case 0x173b: {
		return 0;
	}
	case 0x1fd7: {
		return 0;
	}
	case 0x21b0: {
		return 0;
	}
	case 0x22c4: {
		return 0;
	}
	case 0x2286: {
		return 0;
	}
	case 0x249e: {
		return 0;
	}
	case 0x24a9: {
		return 0;
	}
	case 0x2556: {
		return 0;
	}
	case 0x26af: {
		return 0;
	}
	case 0x27e4: {
		return 0;
	}
	case 0x288e: {
		return 0;
	}
	case 0x28fe: {
		return 0;
	}
	case 0x2a45: {
		return 0;
	}
	case 0x2ab2: {
		return 0;
	}
	case 0x2b9d: {
		return 0;
	}
	case 0x2bfd: {
		return 0;
	}
	case 0x2c88: {
		return 0;
	}
	case 0x2d4e: {
		return 0;
	}
	case 0x327b: {
		return 0;
	}
	case 0x3293: {
		return 0;
	}
	case 0x3321: {
		return 0;
	}
	case 0x3363: {
		return 0;
	}
	case 0x3411: {
		return 0;
	}
	case 0x3b90: {
		return 0;
	}
	case 0x3bf4: {
		return 0;
	}
	case 0x3f26: {
		return 0;
	}
	case 0x4273: {
		return 0;
	}
	case 0x433c: {
		return 0;
	}
	case 0x43d8: {
		return 0;
	}
	case 0x4493: {
		return 0;
	}
	case 0x49a1: {
		// main function
		return 0;
	}
	case 0x4a31: {
		return 0;
	}
	case 0x4c62: {
		return 0;
	}
	case 0x4d1a: {
		return 0;
	}
	case 0x5e22: {
		return 0;
	}
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		return 0;
//		exit(1);
	}
	}
}

static int seg002(unsigned short offs) {
	switch(offs) {
	case 0x0004: {
		return 0;
	}
	case 0x1bea: {
		return 0;
	}
	case 0x1c85: {
		return 0;
	}
	case 0x1caa: {
		// monastery menu: remove member
		return 0;
	}
	case 0x1e33: {
		return 0;
	}
	case 0x2152: {
		return 0;
	}
	case 0x2d80: {
		return 0;
	}
	case 0x2e44: {
		return 0;
	}
	case 0x2eb5: {
		return 0;
	}
	case 0x2f1d: {
		return 0;
	}
	case 0x304f: {
		return 0;
	}
	case 0x30b7: {
		return 0;
	}
	case 0x3175: {
		return 0;
	}
	case 0x3204: {
		return 0;
	}
	case 0x327b: {
		return 0;
	}
	case 0x3293: {
		return 0;
	}
	case 0x33f7: {
		return 0;
	}
	case 0x34b7: {
		return 0;
	}
	case 0x36e7: {
		return 0;
	}
	case 0x371c: {
		return 0;
	}
	case 0x3751: {
		return 0;
	}
	case 0x3799: {
		return 0;
	}
	case 0x37e1: {
		return 0;
	}
	case 0x3854: {
		return 0;
	}
	case 0x38c7: {
		return 0;
	}
	case 0x390f: {
		return 0;
	}
	case 0x396f: {
		return 0;
	}
	case 0x3995: {
		return 0;
	}
	case 0x399c: {
		return 0;
	}
	case 0x39e9: {
		return 0;
	}
	case 0x39b5: {
		return 0;
	}
	case 0x3a43: {
		return 0;
	}
	case 0x3f93: {
		// monastery menu: load game
		return 0;
	}
	case 0x3f9e: {
		return 0;
	}
	case 0x461b: {
		return 0;
	}
	case 0x44b8: {
		return 0;
	}
	case 0x45b4: {
		return 0;
	}
	case 0x4606: {
		return 0;
	}
	case 0x467d: {
		return 0;
	}
	case 0x496c: {
		return 0;
	}
	case 0x4923: {
		return 0;
	}
	case 0x4acf: {
		return 0;
	}
	case 0x4afc: {
		return 0;
	}
	case 0x4f22: {
		return 0;
	}
	case 0x5122: {
		return 0;
	}
	case 0x5169: {
		return 0;
	}
	case 0x5278: {
		return 0;
	}
	case 0x5292: {
		return 0;
	}
	case 0x5315: {
		return 0;
	}
	case 0x54ad: {
		// monastery menu: remove members
		return 0;
	}
	case 0x54ce: {
		return 0;
	}
	case 0x58b7: {
		return 0;
	}
	case 0x5bb5: {
		return 0;
	}
	case 0x5c04: {
		return 0;
	}
	case 0x5c66: {
		return 0;
	}
	case 0x5c9d: {
		return 0;
	}
	case 0x5da1: {
		return 0;
	}
	case 0x5e22: {
		return 0;
	}
	case 0x5f06: {
		return 0;
	}
	case 0x6136: {
		return 0;
	}
	case 0x63e8: {
		return 0;
	}
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		return 0;
//		exit(1);
	}
	}
}

static int seg003(unsigned short offs) {
	switch(offs) {
	/*
	case 0x0006: {
		return 0;
	}
	*/
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		exit(1);
	}
	}
}

static int seg004(unsigned short offs) {
	switch(offs) {
	case 0x0003: {
		return 0;
	}
	case 0x02bc: {
		return 0;
	}
	case 0x0373: {
		return 0;
	}
	case 0x037d: {
		return 0;
	}
	case 0x03ae: {
		return 0;
	}
	case 0x043b: {
		return 0;
	}
	case 0x05e8: {
		return 0;
	}
	case 0x05f3: {
		return 0;
	}
	case 0x0643: {
		return 0;
	}
	case 0x0733: {
		return 0;
	}
	case 0x0823: {
		return 0;
	}
	case 0x0845: {
		return 0;
	}
	case 0x08f8: {
		return 0;
	}
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
//		exit(1);
		return 0;
	}
	}
}

static int seg005(unsigned short offs) {
	switch(offs) {
	case 0x0003: {
		return 0;
	}
	case 0x00bf: {
		return 0;
	}
	case 0x0303: {
		return 0;
	}
	case 0x03aa: {
		return 0;
	}
	case 0x0462: {
		return 0;
	}
	case 0x054b: {
		return 0;
	}
	case 0x05dd: {
		return 0;
	}
	case 0x069b: {
		return 0;
	}
	case 0x08e5: {
		return 0;
	}
	case 0x0e3e: {
		return 0;
	}
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		//exit(1);
		return 0;
	}
	}
}

static int seg006(unsigned short offs) {
	switch(offs) {
	case 0x37dc: {
		return 0;
	}
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
//		exit(1);
		return 0;
	}
	}
}

static int seg007(unsigned short offs) {
	switch(offs) {
	case 0x0004: {
		return 0;
	}
	case 0x0251: {
		return 0;
	}
	case 0x03a5: {
		return 0;
	}
	case 0x04ea: {
		return 0;
	}
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		return 0;
		//exit(1);
	}
	}
}

static int seg008(unsigned short offs) {
	switch(offs) {
	case 0x000c: {
		Bit16u low = CPU_Pop16();
		Bit16u hi = CPU_Pop16();
		CPU_Push16(hi);
		CPU_Push16(low);
		reg_ax = random_interval(low, hi);
		SOA_INFO("random_interval(%d, %d) = %d\n", low, hi, reg_ax);
		return 1;
	}
	case 0x002a: {
		Bit16u inval = CPU_Pop16();
		CPU_Push16(inval);
		reg_ax = random_soa(inval);
		SOA_INFO("random(%d) = %d\n", inval, reg_ax);
		return 1;
	}
	case 0x0080: {
		Bit16s val = CPU_Pop16();
		RealPt p = CPU_Pop32();
		CPU_Push32(p);
		CPU_Push16(val);
		reg_ax = is_in_word_array(val, (signed short*)Real2Host(p));
		//SOA_INFO("is_in_word_array(%04x, %p) = %d)\n", val, p, reg_ax);
		return 1;
	}
	case 0x00a6: {
		Bit16s val = CPU_Pop16();
		RealPt p = CPU_Pop32();
		CPU_Push32(p);
		CPU_Push16(val);
		reg_ax = is_in_byte_array((signed char)val, (signed char*)Real2Host(p));
		//SOA_INFO("is_in_byte_array(%02x, %p) = %d)\n", (signed char)val, p, reg_ax);
		return 1;
	}
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		exit(1);
	}
	}
}

static int seg009(unsigned short offs) {
	switch(offs) {
	case 0x00: {
		unsigned short inval = CPU_Pop16();
		CPU_Push16(inval);
		reg_ax = swap_word(inval);
		//SOA_LOG("swap_word(track_no = 0x%04x) = 0x%04x\n", inval, reg_ax);
		return 1;
	}
	case 0x13: {
		unsigned short inval = reg_ax;
		reg_ax = swap_word_fast(reg_ax);
		//SOA_LOG("swap_word_fast(track_no = 0x%04x) = 0x%04x\n", inval, reg_ax);
		return 1;
	}
	case 0x1f: {
		return 0;
	}
	case 0x5a: {
		return 0;
	}
	case 0x82: {
		return 0;
	}
	case 0x3b8: {
		return 0;
	}
	case 0x3c5: {
		return 0;
	}
	case 0x3c6: {
		return 0;
	}
	case 0x3e8: {
		return 0;
	}
	case 0x72f: {
		return 0;
	}
	case 0x737: {
		return 0;
	}
	case 0x74b: {
		return 0;
	}
	case 0x771: {
		return 0;
	}
	case 0x787: {
		return 0;
	}
	case 0x79d: {
		return 0;
	}
	case 0x7d1: {
		return 0;
	}
	case 0x7ed: {
		return 0;
	}
	case 0x807: {
		return 0;
	}
	case 0x824: {
		return 0;
	}
	case 0x84c: {
		return 0;
	}
	case 0x87d: {
		return 0;
	}
	case 0x8bb: {
		return 0;
	}
	case 0x8e0: {
		return 0;
	}
	case 0x8ff: {
		return 0;
	}
	case 0x91e: {
		return 0;
	}
	case 0x93d: {
		return 0;
	}
	case 0x962: {
		return 0;
	}
	case 0x987: {
		return 0;
	}
	case 0x9ac: {
		return 0;
	}
	case 0x9e5: {
		return 0;
	}
	case 0xa60: {
		return 0;
	}
	case 0xa8f: {
		return 0;
	}
	case 0xabe: {
		return 0;
	}
	case 0xae1: {
		return 0;
	}
	case 0xb04: {
		return 0;
	}
	case 0xb21: {
		return 0;
	}
	case 0xbb7: {
		return 0;
	}
	case 0xbe0: {
		return 0;
	}
	case 0xc3e: {
		return 0;
	}
	case 0xc89: {
		return 0;
	}
	case 0xd6e: {
		return 0;
	}
	case 0xd90: {
		return 0;
	}
	case 0xdc0: {
		return 0;
	}
	case 0xdf0: {
		return 0;
	}
	case 0xe0e: {
		return 0;
	}
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		exit(1);
	}
	}
}

static int seg010(unsigned short offs) {
	switch(offs) {
	case 0x09: {
		return 0;
	}
	case 0x4d: {
		return 0;
	}
	case 0x128: {
		// monastery menu: talk to principal
		return 0;
	}
	case 0x322c: {
		return 0;
	}
	case 0x385b: {
		return 0;
	}
	case 0x3c89: {
		return 0;
	}
	case 0x3d8b: {
		return 0;
	}
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		return 0;
//		exit(1);
	}
	}
}

static int seg011(unsigned short offs) {
	switch(offs) {
	/*
	case 0x0006: {
		return 0;
	}
	*/
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		exit(1);
	}
	}
}

static int seg012(unsigned short offs) {
	switch(offs) {
	case 0x0025: {
		return 0;
	}
	case 0x0250: {
		return 0;
	}
	case 0x0271: {
		return 0;
	}
	case 0x069f: {
		return 0;
	}
	default: {
		SOA_ERR("Uncatched call to %s:0x%04x\n", __func__, offs);
		//exit(1);
		return 0;
	}
	}
}

static int segUnk(unsigned segm, unsigned short offs)
{
	SOA_ERR("far call at %04x:%04x\n", segm, offs);
	return 0;
}

static int segUnk_near(unsigned segm, unsigned short offs)
{
	SOA_ERR("near call at %04x:%04x\n", segm, offs);
	return 0;
}

// Intercept far CALLs (both 32 and 16 bit)
int soa_farcall_vgame_en(unsigned segm, unsigned offs)
{
	//SOA_INFO("FC to Seg 0x%04x:0x%04x\n", segm, offs);
	switch (segm) {
	case 0x0000: return seg000(offs);
	case 0x0343: return seg001(offs);
	case 0x0816: return seg002(offs);
	case 0x0ec2: return seg003(offs);
	case 0x1260: return seg004(offs);
	case 0x12f3: return seg005(offs);
	case 0x1682: return seg006(offs);
	case 0x1a70: return seg007(offs);
	case 0x1ad0: return seg008(offs);
	case 0x1add: return seg009(offs);
	case 0x1bc1: return seg010(offs);
	case 0x1faf: return seg011(offs);
	case 0x2181: return seg012(offs);
	default:
		return segUnk(segm, offs);
	}
}

int soa_nearcall_vgame_en(unsigned offs)
{
	unsigned short segm = SegValue(cs)-soa_relocation;
	int ret = 0;
	unsigned short temp = CPU_Pop16();

	if (segm == 0x0000) ret = seg000(offs);
	else if (segm == 0x0343) ret = seg001(offs);
	else if (segm == 0x0816) ret = seg002(offs);
	else if (segm == 0x0ec2) ret = seg003(offs);
	else if (segm == 0x1260) ret = seg004(offs);
	else if (segm == 0x12f3) ret = seg005(offs);
	else if (segm == 0x1682) ret = seg006(offs);
	else if (segm == 0x1a70) ret = seg007(offs);
	else if (segm == 0x1ad0) ret = seg008(offs);
	else if (segm == 0x1add) ret = seg009(offs);
	else if (segm == 0x1bc1) ret = seg010(offs);
	else if (segm == 0x1faf) ret = seg011(offs);
	else if (segm == 0x2181) ret = seg012(offs);
	else ret = segUnk_near(segm, offs);

	if (!ret) CPU_Push16(temp);

	return ret;
}
