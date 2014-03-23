#if !defined(__BORLANDC__)
namespace M302de {

RealPt F_PADD(RealPt, Bit32s);
Bit32s bc_lseek(Bit16u, Bit32u, Bit16s);
Bit16s bc__read(Bit16u, Bit8u*, Bit16u);
signed short bioskey(signed short);
RealPt bc_farcalloc(Bit32u, Bit32u);
Bit16s bc_close(Bit16u);
Bit16s bc__close(Bit16u);
RealPt bc_memmove(RealPt, RealPt, Bit16u);
Bit16s bc__creat(RealPt, Bit16u);

Bit32s bc__write(Bit16u, RealPt, Bit16u);

}
#else

#include <IO.H>
#include <DOS.H>
#include <BIOS.H>

#define RealMake(s, o) MK_FP(s, o)
#define PhysMake(s, o) RealMake(s, o)

/* helper, use only when neccessary */
#define struct_copy memcpy


#define F_PADD(p, o) ((RealPt __huge)(p) + o)

#define bc_close close
#define bc__creat _creat

#define bc_lseek lseek
#define bc__read read
#define bc__write write

#define bc_itoa itoa

#define mem_memcpy memcpy
#define bc_memmove memmove

#define DOS_SEEK_SET SEEK_SET
#endif
