#if !defined(__BORLANDC__)

namespace SOA_VGAME_EN {

void bc_clrscr(void);
signed short bc_bioskey(signed short);
RealPt bc_getvect(Bit8u);
void bc_setvect(Bit8u, RealPt);
RealPt bc_memmove(RealPt, RealPt, Bit16u);
Bit16s bc_strlen(RealPt str);

}

#else /* __BORLANDC__ */

#include <DOS.H>
#include <BIOS.H>

#define bc_clrscr clrscr
#define bc_bioskey bioskey
#define bc_getvect getvect
#define bc_setvect setvect
#define bc_memmove memmove
#define bc_strlen strlen

#endif
