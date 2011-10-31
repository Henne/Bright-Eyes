#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "dos_inc.h"

#include "schick.h"

#include "seg000.h"

namespace M302de {

Bit32s bc_lseek(Bit16u handle, Bit32u offset, Bit16s whence) {

	ds_writew(0xb788 + handle * 2, ds_readw(0xb788 + handle * 2) & 0xfdff);

	if (!DOS_SeekFile(handle, &offset, whence))
		return -1;

	return offset;

}

Bit16s bc__read(Bit16u handle, Bit8u *buf, Bit16u count) {

	if ((ds_readw(0xb788 + handle * 2) & 2))
		return -1;

	if (!DOS_ReadFile(handle, buf, &count))
		return -1;

	if (count == 5952)
		schick_status_update(buf, count);

	return (Bit16s)count;
}

signed short bioskey(signed short cmd) {
	reg_ah = cmd &  0xff;
	reg_al = 0;
	CALLBACK_RunRealInt(0x16);

	if (GETFLAG(ZF)) {
		if (!(cmd & 1))
			return reg_ax;

		return 0;
	} else {
		if (!(cmd & 1))
			return reg_ax;
		if (reg_ax)
			return reg_ax;
		return -1;
	}
}

RealPt bc_farcalloc(Bit32u nmemb, Bit32u size)
{
	CPU_Push32(size);
	CPU_Push32(nmemb);
	CALLBACK_RunRealFar(reloc_game + 0, 0x2287);
	CPU_Pop32();
	CPU_Pop32();

	return RealMake(reg_dx, reg_ax);
}

Bit16s bc_close(Bit16u handle) {

	if (handle >= ds_readw(0xb786))
		return -1;

	ds_writew(0xb788 + handle * 2, 0);

	return bc__close(handle);

}

Bit16s bc__close(Bit16u handle) {

	if (!DOS_CloseFile(handle))
		return -1;

	ds_writew(0xb788 + handle * 2, 0);

	return 0;
}

Bit32s bc__write(Bit16u fd, RealPt buf, Bit16u len)
{
	CPU_Push16(len);
	CPU_Push32(buf);
	CPU_Push16(fd);
	CALLBACK_RunRealFar(reloc_game + 0, 0x4a85);
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop16();

	return reg_ax;
}

}
