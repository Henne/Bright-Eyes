#if !defined(__BORLANDC__)
namespace G105de {

void bc_exit(Bit16u);
RealPt bc__dos_getvect(Bit16s);
void bc__dos_setvect(Bit16s, RealPt);
Bit32s bc_lseek(Bit16u, Bit32u, Bit16s);
Bit16s bc__read(Bit16u, Bit8u*, Bit16u);
signed short bc_bioskey(signed short);
void bc_clrscr();
void bc_free(RealPt);
Bit16s bc_close(Bit16u);
Bit16s bc__close(Bit16u);
Bit16s bc__create(RealPt, Bit16u);
Bit16s bc_open(RealPt, Bit16u);
Bit16s bc_write(Bit16u, Bit8u*, Bit16u);

/* this will set an unused variable, so we put a dummy here */
static inline void bc_randomize() { }
}

#else __BORLANDC__

#include <IO.H>
#include <BIOS.H>

#define bc_bioskey bioskey
#define bc__read _read
#define bc_close close
#define bc__close _close
#define bc__creat _creat
#define bc_open open

#endif
