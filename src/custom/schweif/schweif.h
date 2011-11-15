#ifndef SCHWEIF_H
#define SCHWEIF_H

#include <stdio.h>


#include "cpu.h"
#include "mem.h"

#define SCHWEIF_INFO
#define SCHWEIF_LOG
//#define SCHWEIF_TRAC
//#define SCHWEIF_GFX

#define D2_ERR(...) fprintf(stderr, __VA_ARGS__)

#ifdef SCHWEIF_LOG
#define D2_LOG(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D2_LOG(...) { }
#endif

#ifdef SCHWEIF_INFO
#define D2_INFO(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D2_INFO(...) { }
#endif

#ifdef SCHWEIF_TRAC
#define D2_TRAC(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D2_TRAC(...) { }
#endif

#ifdef SCHWEIF_GFX
#define D2_GFX(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D2_GFX(...) { }
#endif

/* help Visual C++ to know __func__ */
#if _MSC_VER >= 1300
	#define __func__ __FUNCTION__
#endif

/* Stack cleanup for ret i16 (not typical in C) */
static inline void RET(unsigned short val) {
	reg_sp += val;
}

static inline Bit8u* schweif_getCharname(unsigned p) {
    return MemBase+Real2Phys(p)+16;
}

static inline Bit8u* getString(unsigned p) {
        return MemBase+Real2Phys(p);
}

static inline Bit8u* Real2Host(RealPt p)
{
	return MemBase + Real2Phys(p);
}

namespace schweif_common {
extern const char* names_attrib[];
extern const char* names_skill[];
extern const char* names_spell[];
}

extern unsigned short datseg;
extern Bit8u* p_datseg;
extern unsigned short relocation;

static inline Bit8u ds_readb(unsigned short offs) {
	return host_readb(p_datseg + offs);
}
static inline Bit16u ds_readw(unsigned short offs) {
	return host_readw(p_datseg + offs);
}
static inline Bit32u ds_readd(unsigned short offs) {
	return host_readd(p_datseg + offs);
}
static inline void ds_writeb(unsigned short offs, Bit8u val) {
	host_writeb(p_datseg + offs, val);
}
static inline void ds_writew(unsigned short offs, Bit16u val) {
	host_writew(p_datseg + offs, val);
}
static inline void ds_writed(unsigned short offs, Bit32u val) {
	host_writed(p_datseg + offs, val);
}

int schweif_farcall_v100de(unsigned, unsigned);
int schweif_nearcall_v100de(unsigned);

int schweif_farcall_c102de(unsigned, unsigned);
int schweif_nearcall_c102de(unsigned);

int schweif_get_ovrseg(unsigned short);
inline int schweif_is_ovrseg(unsigned short stub_seg) {
	return SegValue(cs) == schweif_get_ovrseg(stub_seg);
}

const char* schweif_get_version(char *);
int schweif_is_en();
#endif
