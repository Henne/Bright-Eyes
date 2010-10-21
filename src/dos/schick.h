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

extern const char* names_attrib[];
extern const char* names_skill[];
extern const char* names_spell[];

extern unsigned short datseg;
extern unsigned short relocation;

extern void init_schick(char *, unsigned short, unsigned short, unsigned short);
extern void exit_schick(unsigned char);

extern void schick_create(const char *, unsigned char, unsigned);
extern void schick_open(const char *, unsigned char, unsigned);
extern void schick_close(unsigned);

extern void schick_read(unsigned, unsigned char*, unsigned short);
extern void schick_write(unsigned, unsigned char*, unsigned short);

extern void schick_seek(unsigned, unsigned, unsigned);
extern int schick_callf(unsigned, unsigned, unsigned);
extern void schick_ret();
extern void schick_calln16(unsigned);

extern int schick_farcall_gen105(unsigned, unsigned, unsigned);
extern int schick_farcall_v302de(unsigned, unsigned, unsigned);

extern void schick_status_init();
extern void schick_status_exit();
extern void schick_status_update(unsigned char*, unsigned short);
