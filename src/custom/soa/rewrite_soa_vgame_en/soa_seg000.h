#if !defined(__BORLANDC__)

namespace SOA_VGAME_EN {

RealPt bc_getvect(Bit8u);
void bc_setvect(Bit8u, RealPt);

}

#else /* __BORLANDC__ */

#include <DOS.H>

#define bc_getvect getvect
#define bc_setvect setvect

#endif
