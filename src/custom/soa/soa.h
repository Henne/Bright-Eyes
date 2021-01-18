#ifndef SOA_H
#define SOA_H

#include <stdio.h>

#include "cpu.h"
#include "mem.h"

#define SOA_INFO
#define SOA_LOG
//#define SOA_TRAC
//#define SOA_GFX

#define SOA_ERR(...) fprintf(stderr, __VA_ARGS__)

#ifdef SOA_LOG
#define SOA_LOG(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void SOA_LOG(...) { }
#endif

#ifdef SOA_INFO
#define SOA_INFO(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void SOA_INFO(...) { }
#endif

#ifdef SOA_TRAC
#define SOA_TRAC(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void SOA_TRAC(...) { }
#endif

#ifdef SOA_GFX
#define SOA_GFX(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void SOA_GFX(...) { }
#endif

/* help Visual C++ to know __func__ */
#if _MSC_VER >= 1300
	#define __func__ __FUNCTION__
#endif

/* Stack cleanup for ret i16 (not typical in C) */
static inline void RET(unsigned short val) {
	reg_sp += val;
}

static inline Bit8u* soa_getCharname(RealPt p) {
    return MemBase+Real2Phys(p)+0x22;
}

static inline Bit8u* getString(unsigned p) {
        return MemBase+Real2Phys(p);
}

static inline Bit8u* Real2Host(RealPt p)
{
	return MemBase + Real2Phys(p);
}

/**
 * struct_copy() - wrapper for struct assignments
 * @dst:	destination
 * @src:	source
 * @len:	length
 *
 * This is only a marker for copying structs.
 */
static inline void struct_copy(Bit8u *dst, Bit8u *src, int len)
{

	if (len < 0) {
		SOA_ERR("ERROR: %s copy negative amount of bytes\n", __func__);
		return;
	}

	memcpy(dst, src, len);
}


extern unsigned short datseg;
extern Bit8u* p_datseg;
extern unsigned short soa_relocation;

#define deref8(arg) *((unsigned char*)Real2Host(arg))
#define deref16(arg) *((unsigned short*)Real2Host(arg))
#define deref32(arg) *((unsigned int*)Real2Host(arg))

static inline Bit8u ds_readb(unsigned short offs) {
	return host_readb(p_datseg + offs);
}
static inline Bit16u ds_readw(unsigned short offs) {
	return host_readw(p_datseg + offs);
}
static inline Bit32u ds_readd(unsigned short offs) {
	return host_readd(p_datseg + offs);
}
static inline Bit8s ds_readbs(unsigned short offs) {
	return (Bit8s)host_readb(p_datseg + offs);
}
static inline Bit16s ds_readws(unsigned short offs) {
	return (Bit16s)host_readw(p_datseg + offs);
}
static inline Bit32s ds_readds(unsigned short offs) {
	return (Bit32s)host_readd(p_datseg + offs);
}
static inline Bit8s ds_writebs(unsigned short offs, Bit8s val) {
	host_writeb(p_datseg + offs, val);
	return val;
}
static inline Bit8u ds_writeb(unsigned short offs, Bit8u val) {
	host_writeb(p_datseg + offs, val);
	return val;
}
static inline Bit16u ds_writew(unsigned short offs, Bit16u val) {
	host_writew(p_datseg + offs, val);
	return val;
}
static inline Bit16s ds_writews(unsigned short offs, Bit16s val) {
	host_writew(p_datseg + offs, val);
	return val;
}
static inline Bit32u ds_writed(unsigned short offs, Bit32u val) {
	host_writed(p_datseg + offs, val);
	return val;
}

static inline signed char host_readbs(Bit8u* p) {
	return (Bit8s)host_readb(p);
}

int soa_farcall_vgame_en(unsigned, unsigned);
int soa_nearcall_vgame_en(unsigned);

const char* soa_get_version(char *);
int soa_is_en();
#endif
