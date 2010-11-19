#include <stdio.h>


#include "cpu.h"
#include "mem.h"

#define SCHICK_INFO
//#define SCHICK_LOG
#define SCHICK_TRAC
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


/* Stack cleanup for ret i16 (not typical in C) */
static inline void RET(unsigned short val) {
	reg_sp += val;
}

static inline Bit8u* schick_getCharname(unsigned p) {
    return MemBase+Real2Phys(p)+16;
}

static inline Bit8u* getString(unsigned p) {
        return MemBase+Real2Phys(p);
}

extern const char* names_attrib[];
extern const char* names_skill[];
extern const char* names_spell[];

extern unsigned short datseg;
extern unsigned short relocation;

static inline Bit8u ds_readb(unsigned short offs) {
	return real_readb(datseg, offs);
}
static inline Bit16u ds_readw(unsigned short offs) {
	return real_readw(datseg, offs);
}
static inline Bit32u ds_readd(unsigned short offs) {
	return real_readd(datseg, offs);
}
static void ds_writeb(unsigned short offs, Bit8u val) {
	real_writeb(datseg, offs, val);
}
static void ds_writew(unsigned short offs, Bit16u val) {
	real_writew(datseg, offs, val);
}
static void ds_writed(unsigned short offs, Bit32u val) {
	real_writed(datseg, offs, val);
}

int schick_farcall_gen105(unsigned, unsigned);
int schick_nearcall_gen105(unsigned);
int schick_farcall_v302de(unsigned, unsigned);
int schick_nearcall_v302de(unsigned);

int schick_get_version(char *);
int schick_is_en();
int schick_gen_get_version(char *);
int schick_gen_is_en();

void schick_status_init();
void schick_status_exit();
void schick_status_update(unsigned char*, unsigned short);
