#if !defined(__BORLANDC__)
namespace G105de {

void bc_exit(Bit16u);
RealPt bc__dos_getvect(Bit16s);
void bc__dos_setvect(Bit16s, RealPt);
RealPt bc_F_PADD(RealPt, Bit32s);
Bit32s bc_lseek(Bit16u, Bit32u, Bit16s);
Bit16s bc__read(Bit16u, Bit8u*, Bit16u);
signed short bc_bioskey(signed short);
void bc_clrscr();
void bc_free(RealPt);
RealPt bc_calloc(Bit32s, Bit32s);
Bit16s bc_close(Bit16u);
Bit16s bc__close(Bit16u);
Bit16s bc__create(RealPt, Bit16u);
Bit16s bc_flushall();
RealPt bc_memcpy(RealPt, RealPt, Bit16s);
Bit16s bc_open(RealPt, Bit16u);
RealPt bc_strcat(RealPt, RealPt);
RealPt bc_strcpy(RealPt, RealPt);
RealPt bc_strncpy(RealPt, RealPt, Bit16s);
Bit16s bc_write(Bit16s, RealPt, Bit16u);

/* this will set an unused variable, so we put a dummy here */
static inline void bc_randomize() { }
}

#else __BORLANDC__

#include <IO.H>     // lseek, _read, _close, _creat, open, write
#include <STDLIB.H> // free, calloc (also in alloc.h)
#include <BIOS.H>   // bioskey, int86x (also in dos.h)
#include <DOS.H>
#include <CONIO.H> // clrscr

#include "port.h"

#define bc__dos_getvect _dos_getvect
#define bc__dos_setvect _dos_setvect
#define bc_F_PADA(p, v) ((HugePt)(p) += (v))
#define bc_F_PADD(p, v) ((HugePt)(p) + v)
#define bc_bioskey bioskey
#define bc_clrscr clrscr
#define bc_lseek lseek
#define bc__read _read
#define bc_free	free
#define bc_calloc calloc
#define bc_int86x int86x
#define bc_close close
#define bc__close _close
#define bc__creat _creat
#define bc_flushall flushall
#define bc_memcpy memcpy
#define bc_open open
#define bc_strcat strcat
#define bc_strcpy strcpy
#define bc_strncpy strncpy
#define bc_write write

RealPt emu_gen_alloc(Bit32u);

#endif
