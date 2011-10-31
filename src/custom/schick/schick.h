#ifndef SCHICK_H
#define SCHICK_H

#include <stdio.h>


#include "cpu.h"
#include "mem.h"

#define SCHICK_INFO
//#define SCHICK_LOG
//#define SCHICK_TRAC
//#define SCHICK_GFX

#define D1_ERR(...) fprintf(stderr, __VA_ARGS__)

#ifdef SCHICK_LOG
#define D1_LOG(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D1_LOG(...) { }
#endif

#ifdef SCHICK_INFO
#define D1_INFO(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D1_INFO(...) { }
#endif

#ifdef SCHICK_TRAC
#define D1_TRAC(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D1_TRAC(...) { }
#endif

#ifdef SCHICK_GFX
#define D1_GFX(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D1_GFX(...) { }
#endif

/* help Visual C++ to know __func__ */
#if _MSC_VER >= 1300
	#define __func__ __FUNCTION__
#endif

/* Stack cleanup for ret i16 (not typical in C) */
static inline void RET(unsigned short val) {
	reg_sp += val;
}

static inline char* schick_getCharname(RealPt p) {
    return (char*)MemBase + Real2Phys(p) + 16;
}

static inline char* getString(RealPt p) {
        return (char*)MemBase + Real2Phys(p);
}

static inline Bit8u* Real2Host(RealPt p) {
        return MemBase + Real2Phys(p);
}

extern const char* names_attrib[];
extern const char* names_skill[];
extern const char* names_spell[];

extern unsigned short datseg;
extern Bit8u *p_datseg;
extern Bitu reloc_game;
extern Bitu reloc_gen;

static inline Bit8u ds_readb(unsigned short offs) {
	return host_readb(p_datseg + offs);
}
static inline Bit16u ds_readw(unsigned short offs) {
	return host_readw(p_datseg + offs);
}
static inline Bit32u ds_readd(unsigned short offs) {
	return host_readd(p_datseg + offs);
}
static void ds_writeb(unsigned short offs, Bit8u val) {
	host_writeb(p_datseg + offs, val);
}
static void ds_writew(unsigned short offs, Bit16u val) {
	host_writew(p_datseg + offs, val);
}
static void ds_writed(unsigned short offs, Bit32u val) {
	host_writed(p_datseg + offs, val);
}

int schick_farcall_gen105(unsigned, unsigned);
int schick_nearcall_gen105(unsigned);
int schick_farcall_v302de(unsigned, unsigned);
int schick_nearcall_v302de(unsigned);

namespace G105de {
void BE_cleanup();
}

int get_ovrseg(unsigned short);
inline int is_ovrseg(unsigned short stub_seg) {
	return SegValue(cs) == get_ovrseg(stub_seg);
}

int schick_get_version(char *);
int schick_is_en();

void schick_status_init();
void schick_status_exit();
void schick_status_enable();
void schick_status_disable();
void schick_status_update(unsigned char*, unsigned short);

void schick_timer_enable();
void schick_timer_disable();
#endif
