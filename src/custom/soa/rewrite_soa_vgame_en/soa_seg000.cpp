#include "dosbox.h"
#include "callback.h"
#include "dos_inc.h"

#include "soa.h"

#include "soa_seg000.h"

namespace SOA_VGAME_EN {

void bc_clrscr(void)
{
	CALLBACK_RunRealFar(soa_relocation + 0, 0x271);
}

signed short bc_bioskey(signed short cmd)
{
	CPU_Push16(cmd);
	CALLBACK_RunRealFar(soa_relocation + 0, 0x187e);
	CPU_Pop16();
	return reg_ax;
}

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

RealPt bc_memmove(RealPt dst, RealPt src, Bit16u len)
{
	PhysPt s, d;

	s = Real2Phys(src);
	d = Real2Phys(dst);

	while (len--) {
		mem_writeb(d++, mem_readb(s++));
	}

	return dst;
}

Bit16s bc_strlen(RealPt str)
{
	CPU_Push32(str);
	CALLBACK_RunRealFar(soa_relocation + 0, 0x248a);
	CPU_Pop32();
	return reg_ax;
}

}
