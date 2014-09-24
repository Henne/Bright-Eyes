#if !defined(__BORLANDC__)
namespace M302de {

struct ffblk {
	Bit8u a[44];
};

Bit16s bc_getdisk(void);

RealPt F_PADD(RealPt, Bit32s);
Bit32s F_PSUB(RealPt, RealPt);

static inline Bit8u* H_PADD(Bit8u *p, Bit32s o) { return p + o; }

void bc_exit(Bit16s);
RealPt bc__dos_getvect(Bit8u);
void bc__dos_setvect(Bit8u, RealPt);
Bit32s bc_lseek(Bit16u, Bit32u, Bit16s);
void bc_srand(Bit16u);
Bit16s bc__read(Bit16u, Bit8u*, Bit16u);
Bit32s bc_time(RealPt);
signed short bc_unlink(RealPt);
signed short bc_bioskey(signed short);
void bc_clrscr(void);
void bc_farfree(RealPt);
Bit32u bc_farcoreleft(void);
RealPt bc_farcalloc(Bit32u, Bit32u);

signed short bc_findfirst(RealPt, struct ffblk*, signed short);
signed short bc_findfirst_dosbox(RealPt, RealPt, signed short);
signed short bc_findnext(struct ffblk*);
signed short bc_findnext_dosbox(RealPt);

Bit16s bc_flushall(void);

Bit16s bc_close(Bit16u);
Bit16s bc__close(Bit16u);
RealPt bc_memmove(RealPt, RealPt, Bit16u);
Bit16s bc__creat(RealPt, Bit16u);

Bit16s bc__open(RealPt, Bit16u);

void bc_qsort(RealPt, Bit16u, Bit16u, RealPt);

Bit32s bc__write(Bit16u, RealPt, Bit16u);

Bit16s bc_OvrInitEms(Bit16u, Bit16u, Bit16u);

}
#else

#include <IO.H>
#include <DOS.H>
#include <BIOS.H>
#include <DIR.H>
#include <CTYPE.H>
#include <ALLOC.H>
#include <CONIO.H>
#include <TIME.H>

#define RealMake(s, o) MK_FP(s, o)
#define PhysMake(s, o) RealMake(s, o)

/* helper, use only when neccessary */
#define struct_copy memcpy


#define bc_getdisk getdisk

#define F_PADD(p, o) ((HugePt)(p) + o)
#define F_PSUB(p1, p2) ((HugePt)(p1) - (HugePt)(p2))

#define H_PADD(p, o) F_PADD(p, o)

#define bc_exit exit

#define bc__dos_getvect _dos_getvect
#define bc__dos_setvect _dos_setvect

#define bc_farcalloc farcalloc

#define bc_close close
#define bc__creat _creat

#define bc_lseek lseek
#define bc_srand srand
#define bc__read read
#define bc__write write

#define bc_time time

#define bc_unlink unlink

#define bc_bioskey bioskey
#define bc_clrscr clrscr
#define bc_farfree farfree
#define bc_farcoreleft farcoreleft

#define bc_itoa itoa

#define bc_findfirst findfirst
#define bc_findnext findnext
#define bc_flushall flushall

#define bc__open _open

#define mem_memcpy memcpy
#define bc_memmove memmove

#define bc_qsort qsort

#define DOS_SEEK_SET SEEK_SET

#define bc_OvrInitEms _OvrInitEms
#endif
