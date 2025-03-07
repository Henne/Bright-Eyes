#include "dosbox.h"
#include "callback.h"
#include "dos_inc.h"

#include "soa.h"

#include "soa_seg000.h"

namespace SOA_VGAME_EN {

RealPt bc_getvect(Bit8u __interruptno)
{
	CPU_Push16(__interruptno);
	CALLBACK_RunRealFar(soa_relocation + 0, 0x1cde);
	CPU_Pop16();
	return RealMake(reg_dx, reg_ax);
}

void bc_setvect(Bit8u __interruptno, RealPt handler)
{
	CPU_Push32(handler);
	CPU_Push16(__interruptno);
	CALLBACK_RunRealFar(soa_relocation + 0, 0x1ced);
	CPU_Pop16();
	CPU_Pop32();
}

}
